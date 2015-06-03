/*
 * APM 86xxx PCIe Header File
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
#ifndef _APM_PCIE_CSR_H__
#define _APM_PCIE_CSR_H__

/*	Global Base Address	*/
#define PCIE_AXI_CSR_REGS1_BASE_ADDR			0xddd880000ULL
#define PCIE_AXI_CSR_REGS2_BASE_ADDR			0xddd890000ULL
#define PCIE_AXI_CSR_REGS3_BASE_ADDR			0xddd8f0000ULL

/*    Address P1_PCIE_AXI_CSR_PORT1  Registers */
#define EVENTINTERUPTSTATUS_ADDR                                     0x00001c00
#define EVENTINTERUPTSTATUS_DEFAULT                                  0x00000000
#define EVENTINTERUPTSTATUSMASK_ADDR                                 0x00001c04
#define EVENTCAPTUREENABLE_ADDR                                      0x00001c08
#define EVENTCAPTUREENABLE_DEFAULT                                   0x00000000
#define ERRORINTERRUPTSTATUS_ADDR                                    0x00001c0c
#define ERRORINTERRUPTSTATUS_DEFAULT                                 0x00000000
#define ERRORINTERRUPTSTATUSMASK_ADDR                                0x00001c10
#define ERRORCAPTUREENABLE_ADDR                                      0x00001c14
#define ERRORCAPTUREENABLE_DEFAULT                                   0x00000000
#define INTXSTATUS_ADDR                                              0x00001c18
#define INTXSTATUS_DEFAULT                                           0x00000000
#define INTXSTATUSMASK_ADDR                                          0x00001c1c
#define PCIEAXICONTROL_ADDR                                          0x00001c20
#define PCIEAXICONTROL_DEFAULT                                       0xff00033f
#define TLPSIDEBANDSIGNAL_ADDR                                       0x00001c24
#define TLPSIDEBANDSIGNAL_DEFAULT                                    0x01000000
#define PCIERDERRATTRIBUTES_ADDR                                     0x00001c30
#define PCIERDERRATTRIBUTES_DEFAULT                                  0x00000000
#define PCIEWRERRATTRIBUTES_ADDR                                     0x00001c34
#define PCIEWRERRATTRIBUTES_DEFAULT                                  0x00000000
#define ENDIAN_ADDR                                                  0x00001c50
#define ENDIAN_DEFAULT                                               0xffffffff
#define DEVICETYPE_ADDR                                              0x00001c54
#define DEVICETYPE_DEFAULT                                           0x00000000
#define RBUF_RAM_CTL_ADDR                                            0x00001c60
#define RBUF_RAM_CTL_DEFAULT                                         0x00000000
#define RADM_DATAQ_RAM_CTL_ADDR                                      0x00001c64
#define RADM_DATAQ_RAM_CTL_DEFAULT                                   0x00000000
#define GM_COMP_RAM_CTL_ADDR                                         0x00001c68
#define GM_COMP_RAM_CTL_DEFAULT                                      0x00000000
#define PCIEBID_ADDR                                                 0x00001c80
#define PCIEBID_DEFAULT                                              0x0000006a
#define PCIERSTCTRL_ADDR                                             0x00001c84
#define PCIERSTCTRL_DEFAULT                                          0x00000000
#define SWINT_ADDR                                                   0x00001c88
#define SWINT_DEFAULT                                                0x00000000

/*	Register EventInteruptStatus	*/ 
/*	 Fields PhyInP1Level	 */
#define PHYINP1LEVEL_WIDTH                                                    1
#define PHYINP1LEVEL_SHIFT                                                   26
#define PHYINP1LEVEL_MASK                                            0x04000000
#define PHYINP1LEVEL_RD(src)                         (((src) & 0x04000000)>>26)
#define PHYINP1LEVEL_WR(src)                    (((u32)(src)<<26) & 0x04000000)
#define PHYINP1LEVEL_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields PhyInP2Level	 */
#define PHYINP2LEVEL_WIDTH                                                    1
#define PHYINP2LEVEL_SHIFT                                                   25
#define PHYINP2LEVEL_MASK                                            0x02000000
#define PHYINP2LEVEL_RD(src)                         (((src) & 0x02000000)>>25)
#define PHYINP2LEVEL_WR(src)                    (((u32)(src)<<25) & 0x02000000)
#define PHYINP2LEVEL_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields PhyLinkUpLevel	 */
#define PHYLINKUPLEVEL_WIDTH                                                  1
#define PHYLINKUPLEVEL_SHIFT                                                 24
#define PHYLINKUPLEVEL_MASK                                          0x01000000
#define PHYLINKUPLEVEL_RD(src)                       (((src) & 0x01000000)>>24)
#define PHYLINKUPLEVEL_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define PHYLINKUPLEVEL_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields WakeUpRequest	 */
#define WAKEUPREQUEST_WIDTH                                                   1
#define WAKEUPREQUEST_SHIFT                                                  23
#define WAKEUPREQUEST_MASK                                           0x00800000
#define WAKEUPREQUEST_RD(src)                        (((src) & 0x00800000)>>23)
#define WAKEUPREQUEST_WR(src)                   (((u32)(src)<<23) & 0x00800000)
#define WAKEUPREQUEST_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields PmTurnOff	 */
#define PMTURNOFF_WIDTH                                                       1
#define PMTURNOFF_SHIFT                                                      22
#define PMTURNOFF_MASK                                               0x00400000
#define PMTURNOFF_RD(src)                            (((src) & 0x00400000)>>22)
#define PMTURNOFF_WR(src)                       (((u32)(src)<<22) & 0x00400000)
#define PMTURNOFF_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields MsgUnlock	 */
#define MSGUNLOCK_WIDTH                                                       1
#define MSGUNLOCK_SHIFT                                                      21
#define MSGUNLOCK_MASK                                               0x00200000
#define MSGUNLOCK_RD(src)                            (((src) & 0x00200000)>>21)
#define MSGUNLOCK_WR(src)                       (((u32)(src)<<21) & 0x00200000)
#define MSGUNLOCK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields PmPme	 */
#define PMPME_WIDTH                                                           1
#define PMPME_SHIFT                                                          20
#define PMPME_MASK                                                   0x00100000
#define PMPME_RD(src)                                (((src) & 0x00100000)>>20)
#define PMPME_WR(src)                           (((u32)(src)<<20) & 0x00100000)
#define PMPME_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields PmeToAck	 */
#define PMETOACK_WIDTH                                                        1
#define PMETOACK_SHIFT                                                       19
#define PMETOACK_MASK                                                0x00080000
#define PMETOACK_RD(src)                             (((src) & 0x00080000)>>19)
#define PMETOACK_WR(src)                        (((u32)(src)<<19) & 0x00080000)
#define PMETOACK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields VpdInt	 */
#define VPDINT_WIDTH                                                          1
#define VPDINT_SHIFT                                                         18
#define VPDINT_MASK                                                  0x00040000
#define VPDINT_RD(src)                               (((src) & 0x00040000)>>18)
#define VPDINT_WR(src)                          (((u32)(src)<<18) & 0x00040000)
#define VPDINT_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields HpMsi	 */
#define HPMSI_WIDTH                                                           1
#define HPMSI_SHIFT                                                          17
#define HPMSI_MASK                                                   0x00020000
#define HPMSI_RD(src)                                (((src) & 0x00020000)>>17)
#define HPMSI_WR(src)                           (((u32)(src)<<17) & 0x00020000)
#define HPMSI_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields HpInt	 */
#define HPINT_WIDTH                                                           1
#define HPINT_SHIFT                                                          16
#define HPINT_MASK                                                   0x00010000
#define HPINT_RD(src)                                (((src) & 0x00010000)>>16)
#define HPINT_WR(src)                           (((u32)(src)<<16) & 0x00010000)
#define HPINT_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields HpPme	 */
#define HPPME_WIDTH                                                           1
#define HPPME_SHIFT                                                          15
#define HPPME_MASK                                                   0x00008000
#define HPPME_RD(src)                                (((src) & 0x00008000)>>15)
#define HPPME_WR(src)                           (((u32)(src)<<15) & 0x00008000)
#define HPPME_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields PmeMsi	 */
#define PMEMSI_WIDTH                                                          1
#define PMEMSI_SHIFT                                                         14
#define PMEMSI_MASK                                                  0x00004000
#define PMEMSI_RD(src)                               (((src) & 0x00004000)>>14)
#define PMEMSI_WR(src)                          (((u32)(src)<<14) & 0x00004000)
#define PMEMSI_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields PmeInt	 */
#define PMEINT_WIDTH                                                          1
#define PMEINT_SHIFT                                                         13
#define PMEINT_MASK                                                  0x00002000
#define PMEINT_RD(src)                               (((src) & 0x00002000)>>13)
#define PMEINT_WR(src)                          (((u32)(src)<<13) & 0x00002000)
#define PMEINT_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields BwMgt	 */
#define BWMGT_WIDTH                                                           1
#define BWMGT_SHIFT                                                           8
#define BWMGT_MASK                                                   0x00000100
#define BWMGT_RD(src)                                 (((src) & 0x00000100)>>8)
#define BWMGT_WR(src)                            (((u32)(src)<<8) & 0x00000100)
#define BWMGT_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields LinkAutoBw	 */
#define LINKAUTOBW_WIDTH                                                      1
#define LINKAUTOBW_SHIFT                                                      7
#define LINKAUTOBW_MASK                                              0x00000080
#define LINKAUTOBW_RD(src)                            (((src) & 0x00000080)>>7)
#define LINKAUTOBW_WR(src)                       (((u32)(src)<<7) & 0x00000080)
#define LINKAUTOBW_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields BusMasterDis	 */
#define BUSMASTERDIS_WIDTH                                                    1
#define BUSMASTERDIS_SHIFT                                                    6
#define BUSMASTERDIS_MASK                                            0x00000040
#define BUSMASTERDIS_RD(src)                          (((src) & 0x00000040)>>6)
#define BUSMASTERDIS_WR(src)                     (((u32)(src)<<6) & 0x00000040)
#define BUSMASTERDIS_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields BusMasterEn	 */
#define BUSMASTEREN_WIDTH                                                     1
#define BUSMASTEREN_SHIFT                                                     5
#define BUSMASTEREN_MASK                                             0x00000020
#define BUSMASTEREN_RD(src)                           (((src) & 0x00000020)>>5)
#define BUSMASTEREN_WR(src)                      (((u32)(src)<<5) & 0x00000020)
#define BUSMASTEREN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields LinkReqRst	 */
#define LINKREQRST_WIDTH                                                      1
#define LINKREQRST_SHIFT                                                      4
#define LINKREQRST_MASK                                              0x00000010
#define LINKREQRST_RD(src)                            (((src) & 0x00000010)>>4)
#define LINKREQRST_WR(src)                       (((u32)(src)<<4) & 0x00000010)
#define LINKREQRST_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields DlLinkDown	 */
#define DLLINKDOWN_WIDTH                                                      1
#define DLLINKDOWN_SHIFT                                                      3
#define DLLINKDOWN_MASK                                              0x00000008
#define DLLINKDOWN_RD(src)                            (((src) & 0x00000008)>>3)
#define DLLINKDOWN_WR(src)                       (((u32)(src)<<3) & 0x00000008)
#define DLLINKDOWN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields DlLinkUp	 */
#define DLLINKUP_WIDTH                                                        1
#define DLLINKUP_SHIFT                                                        2
#define DLLINKUP_MASK                                                0x00000004
#define DLLINKUP_RD(src)                              (((src) & 0x00000004)>>2)
#define DLLINKUP_WR(src)                         (((u32)(src)<<2) & 0x00000004)
#define DLLINKUP_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields PhyLinkDown	 */
#define PHYLINKDOWN_WIDTH                                                     1
#define PHYLINKDOWN_SHIFT                                                     1
#define PHYLINKDOWN_MASK                                             0x00000002
#define PHYLINKDOWN_RD(src)                           (((src) & 0x00000002)>>1)
#define PHYLINKDOWN_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define PHYLINKDOWN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields PhyLinkUp	 */
#define PHYLINKUP_WIDTH                                                       1
#define PHYLINKUP_SHIFT                                                       0
#define PHYLINKUP_MASK                                               0x00000001
#define PHYLINKUP_RD(src)                                (((src) & 0x00000001))
#define PHYLINKUP_WR(src)                           (((u32)(src)) & 0x00000001)
#define PHYLINKUP_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register EventInteruptStatusMask	*/
/*    Mask Register Fields PhyInP1LevelMask    */
#define PHYINP1LEVELMASK_WIDTH                                                1
#define PHYINP1LEVELMASK_SHIFT                                               26
#define PHYINP1LEVELMASK_MASK                                        0x04000000
#define PHYINP1LEVELMASK_RD(src)                     (((src) & 0x04000000)>>26)
#define PHYINP1LEVELMASK_WR(src)                (((u32)(src)<<26) & 0x04000000)
#define PHYINP1LEVELMASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields PhyInP2LevelMask    */
#define PHYINP2LEVELMASK_WIDTH                                                1
#define PHYINP2LEVELMASK_SHIFT                                               25
#define PHYINP2LEVELMASK_MASK                                        0x02000000
#define PHYINP2LEVELMASK_RD(src)                     (((src) & 0x02000000)>>25)
#define PHYINP2LEVELMASK_WR(src)                (((u32)(src)<<25) & 0x02000000)
#define PHYINP2LEVELMASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields PhyLinkUpLevelMask    */
#define PHYLINKUPLEVELMASK_WIDTH                                              1
#define PHYLINKUPLEVELMASK_SHIFT                                             24
#define PHYLINKUPLEVELMASK_MASK                                      0x01000000
#define PHYLINKUPLEVELMASK_RD(src)                   (((src) & 0x01000000)>>24)
#define PHYLINKUPLEVELMASK_WR(src)              (((u32)(src)<<24) & 0x01000000)
#define PHYLINKUPLEVELMASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields WakeUpRequestMask    */
#define WAKEUPREQUESTMASK_WIDTH                                               1
#define WAKEUPREQUESTMASK_SHIFT                                              23
#define WAKEUPREQUESTMASK_MASK                                       0x00800000
#define WAKEUPREQUESTMASK_RD(src)                    (((src) & 0x00800000)>>23)
#define WAKEUPREQUESTMASK_WR(src)               (((u32)(src)<<23) & 0x00800000)
#define WAKEUPREQUESTMASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields PmTurnOffMask    */
#define PMTURNOFFMASK_WIDTH                                                   1
#define PMTURNOFFMASK_SHIFT                                                  22
#define PMTURNOFFMASK_MASK                                           0x00400000
#define PMTURNOFFMASK_RD(src)                        (((src) & 0x00400000)>>22)
#define PMTURNOFFMASK_WR(src)                   (((u32)(src)<<22) & 0x00400000)
#define PMTURNOFFMASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields MsgUnlockMask    */
#define MSGUNLOCKMASK_WIDTH                                                   1
#define MSGUNLOCKMASK_SHIFT                                                  21
#define MSGUNLOCKMASK_MASK                                           0x00200000
#define MSGUNLOCKMASK_RD(src)                        (((src) & 0x00200000)>>21)
#define MSGUNLOCKMASK_WR(src)                   (((u32)(src)<<21) & 0x00200000)
#define MSGUNLOCKMASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields PmPmeMask    */
#define PMPMEMASK_WIDTH                                                       1
#define PMPMEMASK_SHIFT                                                      20
#define PMPMEMASK_MASK                                               0x00100000
#define PMPMEMASK_RD(src)                            (((src) & 0x00100000)>>20)
#define PMPMEMASK_WR(src)                       (((u32)(src)<<20) & 0x00100000)
#define PMPMEMASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields PmeToAckMask    */
#define PMETOACKMASK_WIDTH                                                    1
#define PMETOACKMASK_SHIFT                                                   19
#define PMETOACKMASK_MASK                                            0x00080000
#define PMETOACKMASK_RD(src)                         (((src) & 0x00080000)>>19)
#define PMETOACKMASK_WR(src)                    (((u32)(src)<<19) & 0x00080000)
#define PMETOACKMASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields VpdIntMask    */
#define VPDINTMASK_WIDTH                                                      1
#define VPDINTMASK_SHIFT                                                     18
#define VPDINTMASK_MASK                                              0x00040000
#define VPDINTMASK_RD(src)                           (((src) & 0x00040000)>>18)
#define VPDINTMASK_WR(src)                      (((u32)(src)<<18) & 0x00040000)
#define VPDINTMASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields HpMsiMask    */
#define HPMSIMASK_WIDTH                                                       1
#define HPMSIMASK_SHIFT                                                      17
#define HPMSIMASK_MASK                                               0x00020000
#define HPMSIMASK_RD(src)                            (((src) & 0x00020000)>>17)
#define HPMSIMASK_WR(src)                       (((u32)(src)<<17) & 0x00020000)
#define HPMSIMASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields HpIntMask    */
#define HPINTMASK_WIDTH                                                       1
#define HPINTMASK_SHIFT                                                      16
#define HPINTMASK_MASK                                               0x00010000
#define HPINTMASK_RD(src)                            (((src) & 0x00010000)>>16)
#define HPINTMASK_WR(src)                       (((u32)(src)<<16) & 0x00010000)
#define HPINTMASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields HpPmeMask    */
#define HPPMEMASK_WIDTH                                                       1
#define HPPMEMASK_SHIFT                                                      15
#define HPPMEMASK_MASK                                               0x00008000
#define HPPMEMASK_RD(src)                            (((src) & 0x00008000)>>15)
#define HPPMEMASK_WR(src)                       (((u32)(src)<<15) & 0x00008000)
#define HPPMEMASK_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*    Mask Register Fields PmeMsiMask    */
#define PMEMSIMASK_WIDTH                                                      1
#define PMEMSIMASK_SHIFT                                                     14
#define PMEMSIMASK_MASK                                              0x00004000
#define PMEMSIMASK_RD(src)                           (((src) & 0x00004000)>>14)
#define PMEMSIMASK_WR(src)                      (((u32)(src)<<14) & 0x00004000)
#define PMEMSIMASK_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*    Mask Register Fields PmeIntMask    */
#define PMEINTMASK_WIDTH                                                      1
#define PMEINTMASK_SHIFT                                                     13
#define PMEINTMASK_MASK                                              0x00002000
#define PMEINTMASK_RD(src)                           (((src) & 0x00002000)>>13)
#define PMEINTMASK_WR(src)                      (((u32)(src)<<13) & 0x00002000)
#define PMEINTMASK_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*    Mask Register Fields BwMgtMask    */
#define BWMGTMASK_WIDTH                                                       1
#define BWMGTMASK_SHIFT                                                       8
#define BWMGTMASK_MASK                                               0x00000100
#define BWMGTMASK_RD(src)                             (((src) & 0x00000100)>>8)
#define BWMGTMASK_WR(src)                        (((u32)(src)<<8) & 0x00000100)
#define BWMGTMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields LinkAutoBwMask    */
#define LINKAUTOBWMASK_WIDTH                                                  1
#define LINKAUTOBWMASK_SHIFT                                                  7
#define LINKAUTOBWMASK_MASK                                          0x00000080
#define LINKAUTOBWMASK_RD(src)                        (((src) & 0x00000080)>>7)
#define LINKAUTOBWMASK_WR(src)                   (((u32)(src)<<7) & 0x00000080)
#define LINKAUTOBWMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields BusMasterDisMask    */
#define BUSMASTERDISMASK_WIDTH                                                1
#define BUSMASTERDISMASK_SHIFT                                                6
#define BUSMASTERDISMASK_MASK                                        0x00000040
#define BUSMASTERDISMASK_RD(src)                      (((src) & 0x00000040)>>6)
#define BUSMASTERDISMASK_WR(src)                 (((u32)(src)<<6) & 0x00000040)
#define BUSMASTERDISMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields BusMasterEnMask    */
#define BUSMASTERENMASK_WIDTH                                                 1
#define BUSMASTERENMASK_SHIFT                                                 5
#define BUSMASTERENMASK_MASK                                         0x00000020
#define BUSMASTERENMASK_RD(src)                       (((src) & 0x00000020)>>5)
#define BUSMASTERENMASK_WR(src)                  (((u32)(src)<<5) & 0x00000020)
#define BUSMASTERENMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields LinkReqRstMask    */
#define LINKREQRSTMASK_WIDTH                                                  1
#define LINKREQRSTMASK_SHIFT                                                  4
#define LINKREQRSTMASK_MASK                                          0x00000010
#define LINKREQRSTMASK_RD(src)                        (((src) & 0x00000010)>>4)
#define LINKREQRSTMASK_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define LINKREQRSTMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields DlLinkDownMask    */
#define DLLINKDOWNMASK_WIDTH                                                  1
#define DLLINKDOWNMASK_SHIFT                                                  3
#define DLLINKDOWNMASK_MASK                                          0x00000008
#define DLLINKDOWNMASK_RD(src)                        (((src) & 0x00000008)>>3)
#define DLLINKDOWNMASK_WR(src)                   (((u32)(src)<<3) & 0x00000008)
#define DLLINKDOWNMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields DlLinkUpMask    */
#define DLLINKUPMASK_WIDTH                                                    1
#define DLLINKUPMASK_SHIFT                                                    2
#define DLLINKUPMASK_MASK                                            0x00000004
#define DLLINKUPMASK_RD(src)                          (((src) & 0x00000004)>>2)
#define DLLINKUPMASK_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define DLLINKUPMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields PhyLinkDownMask    */
#define PHYLINKDOWNMASK_WIDTH                                                 1
#define PHYLINKDOWNMASK_SHIFT                                                 1
#define PHYLINKDOWNMASK_MASK                                         0x00000002
#define PHYLINKDOWNMASK_RD(src)                       (((src) & 0x00000002)>>1)
#define PHYLINKDOWNMASK_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define PHYLINKDOWNMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields PhyLinkUpMask    */
#define PHYLINKUPMASK_WIDTH                                                   1
#define PHYLINKUPMASK_SHIFT                                                   0
#define PHYLINKUPMASK_MASK                                           0x00000001
#define PHYLINKUPMASK_RD(src)                            (((src) & 0x00000001))
#define PHYLINKUPMASK_WR(src)                       (((u32)(src)) & 0x00000001)
#define PHYLINKUPMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register EventCaptureEnable	*/ 
/*	 Fields PhyInP1LevelEvntEn	 */
#define PHYINP1LEVELEVNTEN_WIDTH                                              1
#define PHYINP1LEVELEVNTEN_SHIFT                                             26
#define PHYINP1LEVELEVNTEN_MASK                                      0x04000000
#define PHYINP1LEVELEVNTEN_RD(src)                   (((src) & 0x04000000)>>26)
#define PHYINP1LEVELEVNTEN_WR(src)              (((u32)(src)<<26) & 0x04000000)
#define PHYINP1LEVELEVNTEN_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields PhyInP2LevelEvntEn	 */
#define PHYINP2LEVELEVNTEN_WIDTH                                              1
#define PHYINP2LEVELEVNTEN_SHIFT                                             25
#define PHYINP2LEVELEVNTEN_MASK                                      0x02000000
#define PHYINP2LEVELEVNTEN_RD(src)                   (((src) & 0x02000000)>>25)
#define PHYINP2LEVELEVNTEN_WR(src)              (((u32)(src)<<25) & 0x02000000)
#define PHYINP2LEVELEVNTEN_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields PhyLinkUpLevelEvntEn	 */
#define PHYLINKUPLEVELEVNTEN_WIDTH                                            1
#define PHYLINKUPLEVELEVNTEN_SHIFT                                           24
#define PHYLINKUPLEVELEVNTEN_MASK                                    0x01000000
#define PHYLINKUPLEVELEVNTEN_RD(src)                 (((src) & 0x01000000)>>24)
#define PHYLINKUPLEVELEVNTEN_WR(src)            (((u32)(src)<<24) & 0x01000000)
#define PHYLINKUPLEVELEVNTEN_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields WakeUpRequestEvntEn	 */
#define WAKEUPREQUESTEVNTEN_WIDTH                                             1
#define WAKEUPREQUESTEVNTEN_SHIFT                                            23
#define WAKEUPREQUESTEVNTEN_MASK                                     0x00800000
#define WAKEUPREQUESTEVNTEN_RD(src)                  (((src) & 0x00800000)>>23)
#define WAKEUPREQUESTEVNTEN_WR(src)             (((u32)(src)<<23) & 0x00800000)
#define WAKEUPREQUESTEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields PmTurnOffEvntEn	 */
#define PMTURNOFFEVNTEN_WIDTH                                                 1
#define PMTURNOFFEVNTEN_SHIFT                                                22
#define PMTURNOFFEVNTEN_MASK                                         0x00400000
#define PMTURNOFFEVNTEN_RD(src)                      (((src) & 0x00400000)>>22)
#define PMTURNOFFEVNTEN_WR(src)                 (((u32)(src)<<22) & 0x00400000)
#define PMTURNOFFEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields MsgUnlockEvntEn	 */
#define MSGUNLOCKEVNTEN_WIDTH                                                 1
#define MSGUNLOCKEVNTEN_SHIFT                                                21
#define MSGUNLOCKEVNTEN_MASK                                         0x00200000
#define MSGUNLOCKEVNTEN_RD(src)                      (((src) & 0x00200000)>>21)
#define MSGUNLOCKEVNTEN_WR(src)                 (((u32)(src)<<21) & 0x00200000)
#define MSGUNLOCKEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields PmPmeEvntEn	 */
#define PMPMEEVNTEN_WIDTH                                                     1
#define PMPMEEVNTEN_SHIFT                                                    20
#define PMPMEEVNTEN_MASK                                             0x00100000
#define PMPMEEVNTEN_RD(src)                          (((src) & 0x00100000)>>20)
#define PMPMEEVNTEN_WR(src)                     (((u32)(src)<<20) & 0x00100000)
#define PMPMEEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields PmeToAckEvntEn	 */
#define PMETOACKEVNTEN_WIDTH                                                  1
#define PMETOACKEVNTEN_SHIFT                                                 19
#define PMETOACKEVNTEN_MASK                                          0x00080000
#define PMETOACKEVNTEN_RD(src)                       (((src) & 0x00080000)>>19)
#define PMETOACKEVNTEN_WR(src)                  (((u32)(src)<<19) & 0x00080000)
#define PMETOACKEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields VpdIntEvntEn	 */
#define VPDINTEVNTEN_WIDTH                                                    1
#define VPDINTEVNTEN_SHIFT                                                   18
#define VPDINTEVNTEN_MASK                                            0x00040000
#define VPDINTEVNTEN_RD(src)                         (((src) & 0x00040000)>>18)
#define VPDINTEVNTEN_WR(src)                    (((u32)(src)<<18) & 0x00040000)
#define VPDINTEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields HpMsiEvntEn	 */
#define HPMSIEVNTEN_WIDTH                                                     1
#define HPMSIEVNTEN_SHIFT                                                    17
#define HPMSIEVNTEN_MASK                                             0x00020000
#define HPMSIEVNTEN_RD(src)                          (((src) & 0x00020000)>>17)
#define HPMSIEVNTEN_WR(src)                     (((u32)(src)<<17) & 0x00020000)
#define HPMSIEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields HpIntEvntEn	 */
#define HPINTEVNTEN_WIDTH                                                     1
#define HPINTEVNTEN_SHIFT                                                    16
#define HPINTEVNTEN_MASK                                             0x00010000
#define HPINTEVNTEN_RD(src)                          (((src) & 0x00010000)>>16)
#define HPINTEVNTEN_WR(src)                     (((u32)(src)<<16) & 0x00010000)
#define HPINTEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields HpPmeEvntEn	 */
#define HPPMEEVNTEN_WIDTH                                                     1
#define HPPMEEVNTEN_SHIFT                                                    15
#define HPPMEEVNTEN_MASK                                             0x00008000
#define HPPMEEVNTEN_RD(src)                          (((src) & 0x00008000)>>15)
#define HPPMEEVNTEN_WR(src)                     (((u32)(src)<<15) & 0x00008000)
#define HPPMEEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields PmeMsiEvntEn	 */
#define PMEMSIEVNTEN_WIDTH                                                    1
#define PMEMSIEVNTEN_SHIFT                                                   14
#define PMEMSIEVNTEN_MASK                                            0x00004000
#define PMEMSIEVNTEN_RD(src)                         (((src) & 0x00004000)>>14)
#define PMEMSIEVNTEN_WR(src)                    (((u32)(src)<<14) & 0x00004000)
#define PMEMSIEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields PmeIntEvntEn	 */
#define PMEINTEVNTEN_WIDTH                                                    1
#define PMEINTEVNTEN_SHIFT                                                   13
#define PMEINTEVNTEN_MASK                                            0x00002000
#define PMEINTEVNTEN_RD(src)                         (((src) & 0x00002000)>>13)
#define PMEINTEVNTEN_WR(src)                    (((u32)(src)<<13) & 0x00002000)
#define PMEINTEVNTEN_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields BwMgtEvntEn	 */
#define BWMGTEVNTEN_WIDTH                                                     1
#define BWMGTEVNTEN_SHIFT                                                     8
#define BWMGTEVNTEN_MASK                                             0x00000100
#define BWMGTEVNTEN_RD(src)                           (((src) & 0x00000100)>>8)
#define BWMGTEVNTEN_WR(src)                      (((u32)(src)<<8) & 0x00000100)
#define BWMGTEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields LinkAutoBwEvntEn	 */
#define LINKAUTOBWEVNTEN_WIDTH                                                1
#define LINKAUTOBWEVNTEN_SHIFT                                                7
#define LINKAUTOBWEVNTEN_MASK                                        0x00000080
#define LINKAUTOBWEVNTEN_RD(src)                      (((src) & 0x00000080)>>7)
#define LINKAUTOBWEVNTEN_WR(src)                 (((u32)(src)<<7) & 0x00000080)
#define LINKAUTOBWEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields BusMasterDisEvntEn	 */
#define BUSMASTERDISEVNTEN_WIDTH                                              1
#define BUSMASTERDISEVNTEN_SHIFT                                              6
#define BUSMASTERDISEVNTEN_MASK                                      0x00000040
#define BUSMASTERDISEVNTEN_RD(src)                    (((src) & 0x00000040)>>6)
#define BUSMASTERDISEVNTEN_WR(src)               (((u32)(src)<<6) & 0x00000040)
#define BUSMASTERDISEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields BusMasterEnEvntEn	 */
#define BUSMASTERENEVNTEN_WIDTH                                               1
#define BUSMASTERENEVNTEN_SHIFT                                               5
#define BUSMASTERENEVNTEN_MASK                                       0x00000020
#define BUSMASTERENEVNTEN_RD(src)                     (((src) & 0x00000020)>>5)
#define BUSMASTERENEVNTEN_WR(src)                (((u32)(src)<<5) & 0x00000020)
#define BUSMASTERENEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields LinkReqRstEvntEn	 */
#define LINKREQRSTEVNTEN_WIDTH                                                1
#define LINKREQRSTEVNTEN_SHIFT                                                4
#define LINKREQRSTEVNTEN_MASK                                        0x00000010
#define LINKREQRSTEVNTEN_RD(src)                      (((src) & 0x00000010)>>4)
#define LINKREQRSTEVNTEN_WR(src)                 (((u32)(src)<<4) & 0x00000010)
#define LINKREQRSTEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields DlLinkDownEvntEn	 */
#define DLLINKDOWNEVNTEN_WIDTH                                                1
#define DLLINKDOWNEVNTEN_SHIFT                                                3
#define DLLINKDOWNEVNTEN_MASK                                        0x00000008
#define DLLINKDOWNEVNTEN_RD(src)                      (((src) & 0x00000008)>>3)
#define DLLINKDOWNEVNTEN_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define DLLINKDOWNEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields DlLinkUpEvntEn	 */
#define DLLINKUPEVNTEN_WIDTH                                                  1
#define DLLINKUPEVNTEN_SHIFT                                                  2
#define DLLINKUPEVNTEN_MASK                                          0x00000004
#define DLLINKUPEVNTEN_RD(src)                        (((src) & 0x00000004)>>2)
#define DLLINKUPEVNTEN_WR(src)                   (((u32)(src)<<2) & 0x00000004)
#define DLLINKUPEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields PhyLinkDownEvntEn	 */
#define PHYLINKDOWNEVNTEN_WIDTH                                               1
#define PHYLINKDOWNEVNTEN_SHIFT                                               1
#define PHYLINKDOWNEVNTEN_MASK                                       0x00000002
#define PHYLINKDOWNEVNTEN_RD(src)                     (((src) & 0x00000002)>>1)
#define PHYLINKDOWNEVNTEN_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define PHYLINKDOWNEVNTEN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields PhyLinkUpEvntEn	 */
#define PHYLINKUPEVNTEN_WIDTH                                                 1
#define PHYLINKUPEVNTEN_SHIFT                                                 0
#define PHYLINKUPEVNTEN_MASK                                         0x00000001
#define PHYLINKUPEVNTEN_RD(src)                          (((src) & 0x00000001))
#define PHYLINKUPEVNTEN_WR(src)                     (((u32)(src)) & 0x00000001)
#define PHYLINKUPEVNTEN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ErrorINTStatus	*/ 
/*	 Fields AXIWrErr	 */
#define AXIWRERR_WIDTH                                                        1
#define AXIWRERR_SHIFT                                                       15
#define AXIWRERR_MASK                                                0x00008000
#define AXIWRERR_RD(src)                             (((src) & 0x00008000)>>15)
#define AXIWRERR_WR(src)                        (((u32)(src)<<15) & 0x00008000)
#define AXIWRERR_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields AXIRdErr	 */
#define AXIRDERR_WIDTH                                                        1
#define AXIRDERR_SHIFT                                                       14
#define AXIRDERR_MASK                                                0x00004000
#define AXIRDERR_RD(src)                             (((src) & 0x00004000)>>14)
#define AXIRDERR_WR(src)                        (((u32)(src)<<14) & 0x00004000)
#define AXIRDERR_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields DmBridgeFlushDone	 */
#define DMBRIDGEFLUSHDONE_WIDTH                                               1
#define DMBRIDGEFLUSHDONE_SHIFT                                              13
#define DMBRIDGEFLUSHDONE_MASK                                       0x00002000
#define DMBRIDGEFLUSHDONE_RD(src)                    (((src) & 0x00002000)>>13)
#define DMBRIDGEFLUSHDONE_WR(src)               (((u32)(src)<<13) & 0x00002000)
#define DMBRIDGEFLUSHDONE_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields AxiTransAfterFlushDone	 */
#define AXITRANSAFTERFLUSHDONE_WIDTH                                          1
#define AXITRANSAFTERFLUSHDONE_SHIFT                                         12
#define AXITRANSAFTERFLUSHDONE_MASK                                  0x00001000
#define AXITRANSAFTERFLUSHDONE_RD(src)               (((src) & 0x00001000)>>12)
#define AXITRANSAFTERFLUSHDONE_WR(src)          (((u32)(src)<<12) & 0x00001000)
#define AXITRANSAFTERFLUSHDONE_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields RadmCplTimeout	 */
#define RADMCPLTIMEOUT_WIDTH                                                  1
#define RADMCPLTIMEOUT_SHIFT                                                 11
#define RADMCPLTIMEOUT_MASK                                          0x00000800
#define RADMCPLTIMEOUT_RD(src)                       (((src) & 0x00000800)>>11)
#define RADMCPLTIMEOUT_WR(src)                  (((u32)(src)<<11) & 0x00000800)
#define RADMCPLTIMEOUT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RadmFatalErr	 */
#define RADMFATALERR_WIDTH                                                    1
#define RADMFATALERR_SHIFT                                                   10
#define RADMFATALERR_MASK                                            0x00000400
#define RADMFATALERR_RD(src)                         (((src) & 0x00000400)>>10)
#define RADMFATALERR_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define RADMFATALERR_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RadmNonfatalErr	 */
#define RADMNONFATALERR_WIDTH                                                 1
#define RADMNONFATALERR_SHIFT                                                 9
#define RADMNONFATALERR_MASK                                         0x00000200
#define RADMNONFATALERR_RD(src)                       (((src) & 0x00000200)>>9)
#define RADMNONFATALERR_WR(src)                  (((u32)(src)<<9) & 0x00000200)
#define RADMNONFATALERR_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RadmCorrectableErr	 */
#define RADMCORRECTABLEERR_WIDTH                                              1
#define RADMCORRECTABLEERR_SHIFT                                              8
#define RADMCORRECTABLEERR_MASK                                      0x00000100
#define RADMCORRECTABLEERR_RD(src)                    (((src) & 0x00000100)>>8)
#define RADMCORRECTABLEERR_WR(src)               (((u32)(src)<<8) & 0x00000100)
#define RADMCORRECTABLEERR_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields SysErrRc	 */
#define SYSERRRC_WIDTH                                                        1
#define SYSERRRC_SHIFT                                                        7
#define SYSERRRC_MASK                                                0x00000080
#define SYSERRRC_RD(src)                              (((src) & 0x00000080)>>7)
#define SYSERRRC_WR(src)                         (((u32)(src)<<7) & 0x00000080)
#define SYSERRRC_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields AerRcErrMsi	 */
#define AERRCERRMSI_WIDTH                                                     1
#define AERRCERRMSI_SHIFT                                                     6
#define AERRCERRMSI_MASK                                             0x00000040
#define AERRCERRMSI_RD(src)                           (((src) & 0x00000040)>>6)
#define AERRCERRMSI_WR(src)                      (((u32)(src)<<6) & 0x00000040)
#define AERRCERRMSI_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields AerRcErr	 */
#define AERRCERR_WIDTH                                                        1
#define AERRCERR_SHIFT                                                        5
#define AERRCERR_MASK                                                0x00000020
#define AERRCERR_RD(src)                              (((src) & 0x00000020)>>5)
#define AERRCERR_WR(src)                         (((u32)(src)<<5) & 0x00000020)
#define AERRCERR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AppParityErrors	 */
#define APPPARITYERRORS_WIDTH                                                 3
#define APPPARITYERRORS_SHIFT                                                 2
#define APPPARITYERRORS_MASK                                         0x0000001c
#define APPPARITYERRORS_RD(src)                       (((src) & 0x0000001c)>>2)
#define APPPARITYERRORS_WR(src)                  (((u32)(src)<<2) & 0x0000001c)
#define APPPARITYERRORS_SET(dst,src) \
                       (((dst) & ~0x0000001c) | (((u32)(src)<<2) & 0x0000001c))
/*	 Fields CmposerLookupErr	 */
#define CMPOSERLOOKUPERR_WIDTH                                                2
#define CMPOSERLOOKUPERR_SHIFT                                                0
#define CMPOSERLOOKUPERR_MASK                                        0x00000003
#define CMPOSERLOOKUPERR_RD(src)                         (((src) & 0x00000003))
#define CMPOSERLOOKUPERR_WR(src)                    (((u32)(src)) & 0x00000003)
#define CMPOSERLOOKUPERR_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register ErrorInterruptStatusMask	*/
/*    Mask Register Fields AXIWrErrMask    */
#define AXIWRERRMASK_WIDTH                                                    1
#define AXIWRERRMASK_SHIFT                                                   15
#define AXIWRERRMASK_MASK                                            0x00008000
#define AXIWRERRMASK_RD(src)                         (((src) & 0x00008000)>>15)
#define AXIWRERRMASK_WR(src)                    (((u32)(src)<<15) & 0x00008000)
#define AXIWRERRMASK_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*    Mask Register Fields AXIRdErrMask    */
#define AXIRDERRMASK_WIDTH                                                    1
#define AXIRDERRMASK_SHIFT                                                   14
#define AXIRDERRMASK_MASK                                            0x00004000
#define AXIRDERRMASK_RD(src)                         (((src) & 0x00004000)>>14)
#define AXIRDERRMASK_WR(src)                    (((u32)(src)<<14) & 0x00004000)
#define AXIRDERRMASK_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*    Mask Register Fields DmBridgeFlushDoneMask    */
#define DMBRIDGEFLUSHDONEMASK_WIDTH                                           1
#define DMBRIDGEFLUSHDONEMASK_SHIFT                                          13
#define DMBRIDGEFLUSHDONEMASK_MASK                                   0x00002000
#define DMBRIDGEFLUSHDONEMASK_RD(src)                (((src) & 0x00002000)>>13)
#define DMBRIDGEFLUSHDONEMASK_WR(src)           (((u32)(src)<<13) & 0x00002000)
#define DMBRIDGEFLUSHDONEMASK_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*    Mask Register Fields AxiTransAfterFlushDoneMask    */
#define AXITRANSAFTERFLUSHDONEMASK_WIDTH                                      1
#define AXITRANSAFTERFLUSHDONEMASK_SHIFT                                     12
#define AXITRANSAFTERFLUSHDONEMASK_MASK                              0x00001000
#define AXITRANSAFTERFLUSHDONEMASK_RD(src)           (((src) & 0x00001000)>>12)
#define AXITRANSAFTERFLUSHDONEMASK_WR(src)      (((u32)(src)<<12) & 0x00001000)
#define AXITRANSAFTERFLUSHDONEMASK_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*    Mask Register Fields RadmCplTimeoutMask    */
#define RADMCPLTIMEOUTMASK_WIDTH                                              1
#define RADMCPLTIMEOUTMASK_SHIFT                                             11
#define RADMCPLTIMEOUTMASK_MASK                                      0x00000800
#define RADMCPLTIMEOUTMASK_RD(src)                   (((src) & 0x00000800)>>11)
#define RADMCPLTIMEOUTMASK_WR(src)              (((u32)(src)<<11) & 0x00000800)
#define RADMCPLTIMEOUTMASK_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*    Mask Register Fields RadmFatalErrMask    */
#define RADMFATALERRMASK_WIDTH                                                1
#define RADMFATALERRMASK_SHIFT                                               10
#define RADMFATALERRMASK_MASK                                        0x00000400
#define RADMFATALERRMASK_RD(src)                     (((src) & 0x00000400)>>10)
#define RADMFATALERRMASK_WR(src)                (((u32)(src)<<10) & 0x00000400)
#define RADMFATALERRMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields RadmNonfatalErrMask    */
#define RADMNONFATALERRMASK_WIDTH                                             1
#define RADMNONFATALERRMASK_SHIFT                                             9
#define RADMNONFATALERRMASK_MASK                                     0x00000200
#define RADMNONFATALERRMASK_RD(src)                   (((src) & 0x00000200)>>9)
#define RADMNONFATALERRMASK_WR(src)              (((u32)(src)<<9) & 0x00000200)
#define RADMNONFATALERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields RadmCorrectableErrMask    */
#define RADMCORRECTABLEERRMASK_WIDTH                                          1
#define RADMCORRECTABLEERRMASK_SHIFT                                          8
#define RADMCORRECTABLEERRMASK_MASK                                  0x00000100
#define RADMCORRECTABLEERRMASK_RD(src)                (((src) & 0x00000100)>>8)
#define RADMCORRECTABLEERRMASK_WR(src)           (((u32)(src)<<8) & 0x00000100)
#define RADMCORRECTABLEERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields SysErrRcMask    */
#define SYSERRRCMASK_WIDTH                                                    1
#define SYSERRRCMASK_SHIFT                                                    7
#define SYSERRRCMASK_MASK                                            0x00000080
#define SYSERRRCMASK_RD(src)                          (((src) & 0x00000080)>>7)
#define SYSERRRCMASK_WR(src)                     (((u32)(src)<<7) & 0x00000080)
#define SYSERRRCMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields AerRcErrMsiMask    */
#define AERRCERRMSIMASK_WIDTH                                                 1
#define AERRCERRMSIMASK_SHIFT                                                 6
#define AERRCERRMSIMASK_MASK                                         0x00000040
#define AERRCERRMSIMASK_RD(src)                       (((src) & 0x00000040)>>6)
#define AERRCERRMSIMASK_WR(src)                  (((u32)(src)<<6) & 0x00000040)
#define AERRCERRMSIMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields AerRcErrMask    */
#define AERRCERRMASK_WIDTH                                                    1
#define AERRCERRMASK_SHIFT                                                    5
#define AERRCERRMASK_MASK                                            0x00000020
#define AERRCERRMASK_RD(src)                          (((src) & 0x00000020)>>5)
#define AERRCERRMASK_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define AERRCERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields AppParityErrorsMask    */
#define APPPARITYERRORSMASK_WIDTH                                             3
#define APPPARITYERRORSMASK_SHIFT                                             2
#define APPPARITYERRORSMASK_MASK                                     0x0000001c
#define APPPARITYERRORSMASK_RD(src)                   (((src) & 0x0000001c)>>2)
#define APPPARITYERRORSMASK_WR(src)              (((u32)(src)<<2) & 0x0000001c)
#define APPPARITYERRORSMASK_SET(dst,src) \
                       (((dst) & ~0x0000001c) | (((u32)(src)<<2) & 0x0000001c))
/*    Mask Register Fields CmposerLookupErrMask    */
#define CMPOSERLOOKUPERRMASK_WIDTH                                            2
#define CMPOSERLOOKUPERRMASK_SHIFT                                            0
#define CMPOSERLOOKUPERRMASK_MASK                                    0x00000003
#define CMPOSERLOOKUPERRMASK_RD(src)                     (((src) & 0x00000003))
#define CMPOSERLOOKUPERRMASK_WR(src)                (((u32)(src)) & 0x00000003)
#define CMPOSERLOOKUPERRMASK_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register ErrorCaptureEnable	*/ 
/*	 Fields DmBridgeFlushDoneEn	 */
#define DMBRIDGEFLUSHDONEEN_WIDTH                                             1
#define DMBRIDGEFLUSHDONEEN_SHIFT                                            13
#define DMBRIDGEFLUSHDONEEN_MASK                                     0x00002000
#define DMBRIDGEFLUSHDONEEN_RD(src)                  (((src) & 0x00002000)>>13)
#define DMBRIDGEFLUSHDONEEN_WR(src)             (((u32)(src)<<13) & 0x00002000)
#define DMBRIDGEFLUSHDONEEN_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields AxiTransAfterFlushDoneEn	 */
#define AXITRANSAFTERFLUSHDONEEN_WIDTH                                        1
#define AXITRANSAFTERFLUSHDONEEN_SHIFT                                       12
#define AXITRANSAFTERFLUSHDONEEN_MASK                                0x00001000
#define AXITRANSAFTERFLUSHDONEEN_RD(src)             (((src) & 0x00001000)>>12)
#define AXITRANSAFTERFLUSHDONEEN_WR(src)        (((u32)(src)<<12) & 0x00001000)
#define AXITRANSAFTERFLUSHDONEEN_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields RadmCplTimeoutEn	 */
#define RADMCPLTIMEOUTEN_WIDTH                                                1
#define RADMCPLTIMEOUTEN_SHIFT                                               11
#define RADMCPLTIMEOUTEN_MASK                                        0x00000800
#define RADMCPLTIMEOUTEN_RD(src)                     (((src) & 0x00000800)>>11)
#define RADMCPLTIMEOUTEN_WR(src)                (((u32)(src)<<11) & 0x00000800)
#define RADMCPLTIMEOUTEN_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RadmFatalErrEn	 */
#define RADMFATALERREN_WIDTH                                                  1
#define RADMFATALERREN_SHIFT                                                 10
#define RADMFATALERREN_MASK                                          0x00000400
#define RADMFATALERREN_RD(src)                       (((src) & 0x00000400)>>10)
#define RADMFATALERREN_WR(src)                  (((u32)(src)<<10) & 0x00000400)
#define RADMFATALERREN_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RadmNonfatalErrEn	 */
#define RADMNONFATALERREN_WIDTH                                               1
#define RADMNONFATALERREN_SHIFT                                               9
#define RADMNONFATALERREN_MASK                                       0x00000200
#define RADMNONFATALERREN_RD(src)                     (((src) & 0x00000200)>>9)
#define RADMNONFATALERREN_WR(src)                (((u32)(src)<<9) & 0x00000200)
#define RADMNONFATALERREN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RadmCorrectableErrEn	 */
#define RADMCORRECTABLEERREN_WIDTH                                            1
#define RADMCORRECTABLEERREN_SHIFT                                            8
#define RADMCORRECTABLEERREN_MASK                                    0x00000100
#define RADMCORRECTABLEERREN_RD(src)                  (((src) & 0x00000100)>>8)
#define RADMCORRECTABLEERREN_WR(src)             (((u32)(src)<<8) & 0x00000100)
#define RADMCORRECTABLEERREN_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields SysErrRcEn	 */
#define SYSERRRCEN_WIDTH                                                      1
#define SYSERRRCEN_SHIFT                                                      7
#define SYSERRRCEN_MASK                                              0x00000080
#define SYSERRRCEN_RD(src)                            (((src) & 0x00000080)>>7)
#define SYSERRRCEN_WR(src)                       (((u32)(src)<<7) & 0x00000080)
#define SYSERRRCEN_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields AerRcErrMsiEn	 */
#define AERRCERRMSIEN_WIDTH                                                   1
#define AERRCERRMSIEN_SHIFT                                                   6
#define AERRCERRMSIEN_MASK                                           0x00000040
#define AERRCERRMSIEN_RD(src)                         (((src) & 0x00000040)>>6)
#define AERRCERRMSIEN_WR(src)                    (((u32)(src)<<6) & 0x00000040)
#define AERRCERRMSIEN_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields AerRcErrEn	 */
#define AERRCERREN_WIDTH                                                      1
#define AERRCERREN_SHIFT                                                      5
#define AERRCERREN_MASK                                              0x00000020
#define AERRCERREN_RD(src)                            (((src) & 0x00000020)>>5)
#define AERRCERREN_WR(src)                       (((u32)(src)<<5) & 0x00000020)
#define AERRCERREN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AppParityErrorsEn	 */
#define APPPARITYERRORSEN_WIDTH                                               3
#define APPPARITYERRORSEN_SHIFT                                               2
#define APPPARITYERRORSEN_MASK                                       0x0000001c
#define APPPARITYERRORSEN_RD(src)                     (((src) & 0x0000001c)>>2)
#define APPPARITYERRORSEN_WR(src)                (((u32)(src)<<2) & 0x0000001c)
#define APPPARITYERRORSEN_SET(dst,src) \
                       (((dst) & ~0x0000001c) | (((u32)(src)<<2) & 0x0000001c))
/*	 Fields CmposerLookupErrEn	 */
#define CMPOSERLOOKUPERREN_WIDTH                                              2
#define CMPOSERLOOKUPERREN_SHIFT                                              0
#define CMPOSERLOOKUPERREN_MASK                                      0x00000003
#define CMPOSERLOOKUPERREN_RD(src)                       (((src) & 0x00000003))
#define CMPOSERLOOKUPERREN_WR(src)                  (((u32)(src)) & 0x00000003)
#define CMPOSERLOOKUPERREN_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register INTxStatus	*/ 
/*	 Fields Intd	 */
#define INTD_WIDTH                                                            1
#define INTD_SHIFT                                                            3
#define INTD_MASK                                                    0x00000008
#define INTD_RD(src)                                  (((src) & 0x00000008)>>3)
#define INTD_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields Intc	 */
#define INTC_WIDTH                                                            1
#define INTC_SHIFT                                                            2
#define INTC_MASK                                                    0x00000004
#define INTC_RD(src)                                  (((src) & 0x00000004)>>2)
#define INTC_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields Intb	 */
#define INTB_WIDTH                                                            1
#define INTB_SHIFT                                                            1
#define INTB_MASK                                                    0x00000002
#define INTB_RD(src)                                  (((src) & 0x00000002)>>1)
#define INTB_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Inta	 */
#define INTA_WIDTH                                                            1
#define INTA_SHIFT                                                            0
#define INTA_MASK                                                    0x00000001
#define INTA_RD(src)                                     (((src) & 0x00000001))
#define INTA_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register INTxStatusMask	*/
/*    Mask Register Fields IntdMask    */
#define INTDMASK_WIDTH                                                        1
#define INTDMASK_SHIFT                                                        3
#define INTDMASK_MASK                                                0x00000008
#define INTDMASK_RD(src)                              (((src) & 0x00000008)>>3)
#define INTDMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields IntcMask    */
#define INTCMASK_WIDTH                                                        1
#define INTCMASK_SHIFT                                                        2
#define INTCMASK_MASK                                                0x00000004
#define INTCMASK_RD(src)                              (((src) & 0x00000004)>>2)
#define INTCMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields IntbMask    */
#define INTBMASK_WIDTH                                                        1
#define INTBMASK_SHIFT                                                        1
#define INTBMASK_MASK                                                0x00000002
#define INTBMASK_RD(src)                              (((src) & 0x00000002)>>1)
#define INTBMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields IntaMask    */
#define INTAMASK_WIDTH                                                        1
#define INTAMASK_SHIFT                                                        0
#define INTAMASK_MASK                                                0x00000001
#define INTAMASK_RD(src)                                 (((src) & 0x00000001))
#define INTAMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcieAxiControl	*/ 
/*	 Fields IbWdRdOrderingAdDrMask	 */
#define IBWDRDORDERINGADDRMASK_WIDTH                                         16
#define IBWDRDORDERINGADDRMASK_SHIFT                                         16
#define IBWDRDORDERINGADDRMASK_MASK                                  0xffff0000
#define IBWDRDORDERINGADDRMASK_RD(src)               (((src) & 0xffff0000)>>16)
#define IBWDRDORDERINGADDRMASK_WR(src)          (((u32)(src)<<16) & 0xffff0000)
#define IBWDRDORDERINGADDRMASK_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields DisAtInboundErr	 */
#define DISATINBOUNDERR_WIDTH                                                 1
#define DISATINBOUNDERR_SHIFT                                                15
#define DISATINBOUNDERR_MASK                                         0x00008000
#define DISATINBOUNDERR_RD(src)                      (((src) & 0x00008000)>>15)
#define DISATINBOUNDERR_WR(src)                 (((u32)(src)<<15) & 0x00008000)
#define DISATINBOUNDERR_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields DisInWrRdOr	 */
#define DISINWRRDOR_WIDTH                                                     1
#define DISINWRRDOR_SHIFT                                                    14
#define DISINWRRDOR_MASK                                             0x00004000
#define DISINWRRDOR_RD(src)                          (((src) & 0x00004000)>>14)
#define DISINWRRDOR_WR(src)                     (((u32)(src)<<14) & 0x00004000)
#define DISINWRRDOR_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields EnSnpsInWrRdOrdering	 */
#define ENSNPSINWRRDORDERING_WIDTH                                            1
#define ENSNPSINWRRDORDERING_SHIFT                                           13
#define ENSNPSINWRRDORDERING_MASK                                    0x00002000
#define ENSNPSINWRRDORDERING_RD(src)                 (((src) & 0x00002000)>>13)
#define ENSNPSINWRRDORDERING_WR(src)            (((u32)(src)<<13) & 0x00002000)
#define ENSNPSINWRRDORDERING_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields DisRdAxiErr	 */
#define DISRDAXIERR_WIDTH                                                     1
#define DISRDAXIERR_SHIFT                                                    12
#define DISRDAXIERR_MASK                                             0x00001000
#define DISRDAXIERR_RD(src)                          (((src) & 0x00001000)>>12)
#define DISRDAXIERR_WR(src)                     (((u32)(src)<<12) & 0x00001000)
#define DISRDAXIERR_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields DisWrAxiErr	 */
#define DISWRAXIERR_WIDTH                                                     1
#define DISWRAXIERR_SHIFT                                                    11
#define DISWRAXIERR_MASK                                             0x00000800
#define DISWRAXIERR_RD(src)                          (((src) & 0x00000800)>>11)
#define DISWRAXIERR_WR(src)                     (((u32)(src)<<11) & 0x00000800)
#define DISWRAXIERR_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields DisNoSnoop	 */
#define DISNOSNOOP_WIDTH                                                      1
#define DISNOSNOOP_SHIFT                                                     10
#define DISNOSNOOP_MASK                                              0x00000400
#define DISNOSNOOP_RD(src)                           (((src) & 0x00000400)>>10)
#define DISNOSNOOP_WR(src)                      (((u32)(src)<<10) & 0x00000400)
#define DISNOSNOOP_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields MstrRespErrMask	 */
#define MSTRRESPERRMASK_WIDTH                                                 2
#define MSTRRESPERRMASK_SHIFT                                                 8
#define MSTRRESPERRMASK_MASK                                         0x00000300
#define MSTRRESPERRMASK_RD(src)                       (((src) & 0x00000300)>>8)
#define MSTRRESPERRMASK_WR(src)                  (((u32)(src)<<8) & 0x00000300)
#define MSTRRESPERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields SlvRespErrMask	 */
#define SLVRESPERRMASK_WIDTH                                                  6
#define SLVRESPERRMASK_SHIFT                                                  0
#define SLVRESPERRMASK_MASK                                          0x0000003f
#define SLVRESPERRMASK_RD(src)                           (((src) & 0x0000003f))
#define SLVRESPERRMASK_WR(src)                      (((u32)(src)) & 0x0000003f)
#define SLVRESPERRMASK_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register TLPSidebandSignal	*/ 
/*	 Fields TypeEn	 */
#define TYPEEN_WIDTH                                                          1
#define TYPEEN_SHIFT                                                         31
#define TYPEEN_MASK                                                  0x80000000
#define TYPEEN_RD(src)                               (((src) & 0x80000000)>>31)
#define TYPEEN_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define TYPEEN_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields outband_pwrup_cmd	 */
#define OUTBAND_PWRUP_CMD_WIDTH                                               1
#define OUTBAND_PWRUP_CMD_SHIFT                                              28
#define OUTBAND_PWRUP_CMD_MASK                                       0x10000000
#define OUTBAND_PWRUP_CMD_RD(src)                    (((src) & 0x10000000)>>28)
#define OUTBAND_PWRUP_CMD_WR(src)               (((u32)(src)<<28) & 0x10000000)
#define OUTBAND_PWRUP_CMD_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields app_req_entr_l1	 */
#define APP_REQ_ENTR_L1_WIDTH                                                 1
#define APP_REQ_ENTR_L1_SHIFT                                                27
#define APP_REQ_ENTR_L1_MASK                                         0x08000000
#define APP_REQ_ENTR_L1_RD(src)                      (((src) & 0x08000000)>>27)
#define APP_REQ_ENTR_L1_WR(src)                 (((u32)(src)<<27) & 0x08000000)
#define APP_REQ_ENTR_L1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields app_req_exit_l1	 */
#define APP_REQ_EXIT_L1_WIDTH                                                 1
#define APP_REQ_EXIT_L1_SHIFT                                                26
#define APP_REQ_EXIT_L1_MASK                                         0x04000000
#define APP_REQ_EXIT_L1_RD(src)                      (((src) & 0x04000000)>>26)
#define APP_REQ_EXIT_L1_WR(src)                 (((u32)(src)<<26) & 0x04000000)
#define APP_REQ_EXIT_L1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields apps_pm_xmt_pme	 */
#define APPS_PM_XMT_PME_WIDTH                                                 1
#define APPS_PM_XMT_PME_SHIFT                                                25
#define APPS_PM_XMT_PME_MASK                                         0x02000000
#define APPS_PM_XMT_PME_RD(src)                      (((src) & 0x02000000)>>25)
#define APPS_PM_XMT_PME_WR(src)                 (((u32)(src)<<25) & 0x02000000)
#define APPS_PM_XMT_PME_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields app_ready_entr_l23	 */
#define APP_READY_ENTR_L23_WIDTH                                              1
#define APP_READY_ENTR_L23_SHIFT                                             24
#define APP_READY_ENTR_L23_MASK                                      0x01000000
#define APP_READY_ENTR_L23_RD(src)                   (((src) & 0x01000000)>>24)
#define APP_READY_ENTR_L23_WR(src)              (((u32)(src)<<24) & 0x01000000)
#define APP_READY_ENTR_L23_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields MsgCode	 */
#define MSGCODE_WIDTH                                                         8
#define MSGCODE_SHIFT                                                        13
#define MSGCODE_MASK                                                 0x001fe000
#define MSGCODE_RD(src)                              (((src) & 0x001fe000)>>13)
#define MSGCODE_WR(src)                         (((u32)(src)<<13) & 0x001fe000)
#define MSGCODE_SET(dst,src) \
                      (((dst) & ~0x001fe000) | (((u32)(src)<<13) & 0x001fe000))
/*	 Fields TC	 */
#define TC_WIDTH                                                              3
#define TC_SHIFT                                                             10
#define TC_MASK                                                      0x00001c00
#define TC_RD(src)                                   (((src) & 0x00001c00)>>10)
#define TC_WR(src)                              (((u32)(src)<<10) & 0x00001c00)
#define TC_SET(dst,src) \
                      (((dst) & ~0x00001c00) | (((u32)(src)<<10) & 0x00001c00))
/*	 Fields Attr	 */
#define ATTR_WIDTH                                                            2
#define ATTR_SHIFT                                                            8
#define ATTR_MASK                                                    0x00000300
#define ATTR_RD(src)                                  (((src) & 0x00000300)>>8)
#define ATTR_WR(src)                             (((u32)(src)<<8) & 0x00000300)
#define ATTR_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields TD	 */
#define TD_WIDTH                                                              1
#define TD_SHIFT                                                              7
#define TD_MASK                                                      0x00000080
#define TD_RD(src)                                    (((src) & 0x00000080)>>7)
#define TD_WR(src)                               (((u32)(src)<<7) & 0x00000080)
#define TD_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields EP	 */
#define EP_WIDTH                                                              1
#define EP_SHIFT                                                              6
#define EP_MASK                                                      0x00000040
#define EP_RD(src)                                    (((src) & 0x00000040)>>6)
#define EP_WR(src)                               (((u32)(src)<<6) & 0x00000040)
#define EP_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields BCM	 */
#define BCM_WIDTH                                                             1
#define BCM_SHIFT                                                             5
#define BCM_MASK                                                     0x00000020
#define BCM_RD(src)                                   (((src) & 0x00000020)>>5)
#define BCM_WR(src)                              (((u32)(src)<<5) & 0x00000020)
#define BCM_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields Type	 */
#define TYPE_WIDTH                                                            5
#define TYPE_SHIFT                                                            0
#define TYPE_MASK                                                    0x0000001f
#define TYPE_RD(src)                                     (((src) & 0x0000001f))
#define TYPE_WR(src)                                (((u32)(src)) & 0x0000001f)
#define TYPE_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register PcieRdErrAttributes	*/ 
/*	 Fields CaptureEnable	 */
#define CAPTUREENABLE_WIDTH                                                   1
#define CAPTUREENABLE_SHIFT                                                  31
#define CAPTUREENABLE_MASK                                           0x80000000
#define CAPTUREENABLE_RD(src)                        (((src) & 0x80000000)>>31)
#define CAPTUREENABLE_WR(src)                   (((u32)(src)<<31) & 0x80000000)
#define CAPTUREENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields ErrorWhileDisabled	 */
#define ERRORWHILEDISABLED_WIDTH                                              1
#define ERRORWHILEDISABLED_SHIFT                                             30
#define ERRORWHILEDISABLED_MASK                                      0x40000000
#define ERRORWHILEDISABLED_RD(src)                   (((src) & 0x40000000)>>30)
#define ERRORWHILEDISABLED_WR(src)              (((u32)(src)<<30) & 0x40000000)
#define ERRORWHILEDISABLED_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields ErrorInterruptEnable	 */
#define ERRORINTERRUPTENABLE_WIDTH                                            1
#define ERRORINTERRUPTENABLE_SHIFT                                           29
#define ERRORINTERRUPTENABLE_MASK                                    0x20000000
#define ERRORINTERRUPTENABLE_RD(src)                 (((src) & 0x20000000)>>29)
#define ERRORINTERRUPTENABLE_WR(src)            (((u32)(src)<<29) & 0x20000000)
#define ERRORINTERRUPTENABLE_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields ErrorInterrupt	 */
#define ERRORINTERRUPT_WIDTH                                                  1
#define ERRORINTERRUPT_SHIFT                                                 28
#define ERRORINTERRUPT_MASK                                          0x10000000
#define ERRORINTERRUPT_RD(src)                       (((src) & 0x10000000)>>28)
#define ERRORINTERRUPT_WR(src)                  (((u32)(src)<<28) & 0x10000000)
#define ERRORINTERRUPT_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields AddressDecodeError	 */
#define ADDRESSDECODEERROR_WIDTH                                              1
#define ADDRESSDECODEERROR_SHIFT                                             27
#define ADDRESSDECODEERROR_MASK                                      0x08000000
#define ADDRESSDECODEERROR_RD(src)                   (((src) & 0x08000000)>>27)
#define ADDRESSDECODEERROR_WR(src)              (((u32)(src)<<27) & 0x08000000)
#define ADDRESSDECODEERROR_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields SlaveError	 */
#define SLAVEERROR_WIDTH                                                      1
#define SLAVEERROR_SHIFT                                                     26
#define SLAVEERROR_MASK                                              0x04000000
#define SLAVEERROR_RD(src)                           (((src) & 0x04000000)>>26)
#define SLAVEERROR_WR(src)                      (((u32)(src)<<26) & 0x04000000)
#define SLAVEERROR_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields AXI_Trans_ID9	 */
#define AXI_TRANS_ID9_WIDTH                                                   1
#define AXI_TRANS_ID9_SHIFT                                                   9
#define AXI_TRANS_ID9_MASK                                           0x00000200
#define AXI_TRANS_ID9_RD(src)                         (((src) & 0x00000200)>>9)
#define AXI_TRANS_ID9_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define AXI_TRANS_ID9_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields AXI_Trans_ID8	 */
#define AXI_TRANS_ID8_WIDTH                                                   1
#define AXI_TRANS_ID8_SHIFT                                                   8
#define AXI_TRANS_ID8_MASK                                           0x00000100
#define AXI_TRANS_ID8_RD(src)                         (((src) & 0x00000100)>>8)
#define AXI_TRANS_ID8_WR(src)                    (((u32)(src)<<8) & 0x00000100)
#define AXI_TRANS_ID8_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields AXI_Trans_ID7	 */
#define AXI_TRANS_ID7_WIDTH                                                   1
#define AXI_TRANS_ID7_SHIFT                                                   7
#define AXI_TRANS_ID7_MASK                                           0x00000080
#define AXI_TRANS_ID7_RD(src)                         (((src) & 0x00000080)>>7)
#define AXI_TRANS_ID7_WR(src)                    (((u32)(src)<<7) & 0x00000080)
#define AXI_TRANS_ID7_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields AXI_Trans_ID6	 */
#define AXI_TRANS_ID6_WIDTH                                                   1
#define AXI_TRANS_ID6_SHIFT                                                   6
#define AXI_TRANS_ID6_MASK                                           0x00000040
#define AXI_TRANS_ID6_RD(src)                         (((src) & 0x00000040)>>6)
#define AXI_TRANS_ID6_WR(src)                    (((u32)(src)<<6) & 0x00000040)
#define AXI_TRANS_ID6_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields AXI_Trans_ID5	 */
#define AXI_TRANS_ID5_WIDTH                                                   1
#define AXI_TRANS_ID5_SHIFT                                                   5
#define AXI_TRANS_ID5_MASK                                           0x00000020
#define AXI_TRANS_ID5_RD(src)                         (((src) & 0x00000020)>>5)
#define AXI_TRANS_ID5_WR(src)                    (((u32)(src)<<5) & 0x00000020)
#define AXI_TRANS_ID5_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AXI_Trans_ID4	 */
#define AXI_TRANS_ID4_WIDTH                                                   1
#define AXI_TRANS_ID4_SHIFT                                                   4
#define AXI_TRANS_ID4_MASK                                           0x00000010
#define AXI_TRANS_ID4_RD(src)                         (((src) & 0x00000010)>>4)
#define AXI_TRANS_ID4_WR(src)                    (((u32)(src)<<4) & 0x00000010)
#define AXI_TRANS_ID4_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AXI_Trans_ID3	 */
#define AXI_TRANS_ID3_WIDTH                                                   1
#define AXI_TRANS_ID3_SHIFT                                                   3
#define AXI_TRANS_ID3_MASK                                           0x00000008
#define AXI_TRANS_ID3_RD(src)                         (((src) & 0x00000008)>>3)
#define AXI_TRANS_ID3_WR(src)                    (((u32)(src)<<3) & 0x00000008)
#define AXI_TRANS_ID3_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields AXI_Trans_ID2	 */
#define AXI_TRANS_ID2_WIDTH                                                   1
#define AXI_TRANS_ID2_SHIFT                                                   2
#define AXI_TRANS_ID2_MASK                                           0x00000004
#define AXI_TRANS_ID2_RD(src)                         (((src) & 0x00000004)>>2)
#define AXI_TRANS_ID2_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define AXI_TRANS_ID2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields AXI_Trans_ID1	 */
#define AXI_TRANS_ID1_WIDTH                                                   1
#define AXI_TRANS_ID1_SHIFT                                                   1
#define AXI_TRANS_ID1_MASK                                           0x00000002
#define AXI_TRANS_ID1_RD(src)                         (((src) & 0x00000002)>>1)
#define AXI_TRANS_ID1_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define AXI_TRANS_ID1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields AXI_Trans_ID0	 */
#define AXI_TRANS_ID0_WIDTH                                                   1
#define AXI_TRANS_ID0_SHIFT                                                   0
#define AXI_TRANS_ID0_MASK                                           0x00000001
#define AXI_TRANS_ID0_RD(src)                            (((src) & 0x00000001))
#define AXI_TRANS_ID0_WR(src)                       (((u32)(src)) & 0x00000001)
#define AXI_TRANS_ID0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcieWrErrAttributes	*/ 
/*	 Fields CaptureEnable	 */
#define CAPTUREENABLE_F1_WIDTH                                                1
#define CAPTUREENABLE_F1_SHIFT                                               31
#define CAPTUREENABLE_F1_MASK                                        0x80000000
#define CAPTUREENABLE_F1_RD(src)                     (((src) & 0x80000000)>>31)
#define CAPTUREENABLE_F1_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define CAPTUREENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields ErrorWhileDisabled	 */
#define ERRORWHILEDISABLED_F1_WIDTH                                           1
#define ERRORWHILEDISABLED_F1_SHIFT                                          30
#define ERRORWHILEDISABLED_F1_MASK                                   0x40000000
#define ERRORWHILEDISABLED_F1_RD(src)                (((src) & 0x40000000)>>30)
#define ERRORWHILEDISABLED_F1_WR(src)           (((u32)(src)<<30) & 0x40000000)
#define ERRORWHILEDISABLED_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields ErrorInterruptEnable	 */
#define ERRORINTERRUPTENABLE_F1_WIDTH                                         1
#define ERRORINTERRUPTENABLE_F1_SHIFT                                        29
#define ERRORINTERRUPTENABLE_F1_MASK                                 0x20000000
#define ERRORINTERRUPTENABLE_F1_RD(src)              (((src) & 0x20000000)>>29)
#define ERRORINTERRUPTENABLE_F1_WR(src)         (((u32)(src)<<29) & 0x20000000)
#define ERRORINTERRUPTENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields ErrorInterrupt	 */
#define ERRORINTERRUPT_F1_WIDTH                                               1
#define ERRORINTERRUPT_F1_SHIFT                                              28
#define ERRORINTERRUPT_F1_MASK                                       0x10000000
#define ERRORINTERRUPT_F1_RD(src)                    (((src) & 0x10000000)>>28)
#define ERRORINTERRUPT_F1_WR(src)               (((u32)(src)<<28) & 0x10000000)
#define ERRORINTERRUPT_F1_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields AddressDecodeError	 */
#define ADDRESSDECODEERROR_F1_WIDTH                                           1
#define ADDRESSDECODEERROR_F1_SHIFT                                          27
#define ADDRESSDECODEERROR_F1_MASK                                   0x08000000
#define ADDRESSDECODEERROR_F1_RD(src)                (((src) & 0x08000000)>>27)
#define ADDRESSDECODEERROR_F1_WR(src)           (((u32)(src)<<27) & 0x08000000)
#define ADDRESSDECODEERROR_F1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields SlaveError	 */
#define SLAVEERROR_F1_WIDTH                                                   1
#define SLAVEERROR_F1_SHIFT                                                  26
#define SLAVEERROR_F1_MASK                                           0x04000000
#define SLAVEERROR_F1_RD(src)                        (((src) & 0x04000000)>>26)
#define SLAVEERROR_F1_WR(src)                   (((u32)(src)<<26) & 0x04000000)
#define SLAVEERROR_F1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields AXI_Trans_ID9	 */
#define AXI_TRANS_ID9_F1_WIDTH                                                1
#define AXI_TRANS_ID9_F1_SHIFT                                                9
#define AXI_TRANS_ID9_F1_MASK                                        0x00000200
#define AXI_TRANS_ID9_F1_RD(src)                      (((src) & 0x00000200)>>9)
#define AXI_TRANS_ID9_F1_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define AXI_TRANS_ID9_F1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields AXI_Trans_ID8	 */
#define AXI_TRANS_ID8_F1_WIDTH                                                1
#define AXI_TRANS_ID8_F1_SHIFT                                                8
#define AXI_TRANS_ID8_F1_MASK                                        0x00000100
#define AXI_TRANS_ID8_F1_RD(src)                      (((src) & 0x00000100)>>8)
#define AXI_TRANS_ID8_F1_WR(src)                 (((u32)(src)<<8) & 0x00000100)
#define AXI_TRANS_ID8_F1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields AXI_Trans_ID7	 */
#define AXI_TRANS_ID7_F1_WIDTH                                                1
#define AXI_TRANS_ID7_F1_SHIFT                                                7
#define AXI_TRANS_ID7_F1_MASK                                        0x00000080
#define AXI_TRANS_ID7_F1_RD(src)                      (((src) & 0x00000080)>>7)
#define AXI_TRANS_ID7_F1_WR(src)                 (((u32)(src)<<7) & 0x00000080)
#define AXI_TRANS_ID7_F1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields AXI_Trans_ID6	 */
#define AXI_TRANS_ID6_F1_WIDTH                                                1
#define AXI_TRANS_ID6_F1_SHIFT                                                6
#define AXI_TRANS_ID6_F1_MASK                                        0x00000040
#define AXI_TRANS_ID6_F1_RD(src)                      (((src) & 0x00000040)>>6)
#define AXI_TRANS_ID6_F1_WR(src)                 (((u32)(src)<<6) & 0x00000040)
#define AXI_TRANS_ID6_F1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields AXI_Trans_ID5	 */
#define AXI_TRANS_ID5_F1_WIDTH                                                1
#define AXI_TRANS_ID5_F1_SHIFT                                                5
#define AXI_TRANS_ID5_F1_MASK                                        0x00000020
#define AXI_TRANS_ID5_F1_RD(src)                      (((src) & 0x00000020)>>5)
#define AXI_TRANS_ID5_F1_WR(src)                 (((u32)(src)<<5) & 0x00000020)
#define AXI_TRANS_ID5_F1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AXI_Trans_ID4	 */
#define AXI_TRANS_ID4_F1_WIDTH                                                1
#define AXI_TRANS_ID4_F1_SHIFT                                                4
#define AXI_TRANS_ID4_F1_MASK                                        0x00000010
#define AXI_TRANS_ID4_F1_RD(src)                      (((src) & 0x00000010)>>4)
#define AXI_TRANS_ID4_F1_WR(src)                 (((u32)(src)<<4) & 0x00000010)
#define AXI_TRANS_ID4_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AXI_Trans_ID3	 */
#define AXI_TRANS_ID3_F1_WIDTH                                                1
#define AXI_TRANS_ID3_F1_SHIFT                                                3
#define AXI_TRANS_ID3_F1_MASK                                        0x00000008
#define AXI_TRANS_ID3_F1_RD(src)                      (((src) & 0x00000008)>>3)
#define AXI_TRANS_ID3_F1_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define AXI_TRANS_ID3_F1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields AXI_Trans_ID2	 */
#define AXI_TRANS_ID2_F1_WIDTH                                                1
#define AXI_TRANS_ID2_F1_SHIFT                                                2
#define AXI_TRANS_ID2_F1_MASK                                        0x00000004
#define AXI_TRANS_ID2_F1_RD(src)                      (((src) & 0x00000004)>>2)
#define AXI_TRANS_ID2_F1_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define AXI_TRANS_ID2_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields AXI_Trans_ID1	 */
#define AXI_TRANS_ID1_F1_WIDTH                                                1
#define AXI_TRANS_ID1_F1_SHIFT                                                1
#define AXI_TRANS_ID1_F1_MASK                                        0x00000002
#define AXI_TRANS_ID1_F1_RD(src)                      (((src) & 0x00000002)>>1)
#define AXI_TRANS_ID1_F1_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define AXI_TRANS_ID1_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields AXI_Trans_ID0	 */
#define AXI_TRANS_ID0_F1_WIDTH                                                1
#define AXI_TRANS_ID0_F1_SHIFT                                                0
#define AXI_TRANS_ID0_F1_MASK                                        0x00000001
#define AXI_TRANS_ID0_F1_RD(src)                         (((src) & 0x00000001))
#define AXI_TRANS_ID0_F1_WR(src)                    (((u32)(src)) & 0x00000001)
#define AXI_TRANS_ID0_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Endian	*/ 
/*	 Fields LittleEndian	 */
#define LITTLEENDIAN_WIDTH                                                   32
#define LITTLEENDIAN_SHIFT                                                    0
#define LITTLEENDIAN_MASK                                            0xffffffff
#define LITTLEENDIAN_RD(src)                             (((src) & 0xffffffff))
#define LITTLEENDIAN_WR(src)                        (((u32)(src)) & 0xffffffff)
#define LITTLEENDIAN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DeviceType	*/ 
/*	 Fields DeviceType	 */
#define DEVICETYPE_WIDTH                                                      4
#define DEVICETYPE_SHIFT                                                      0
#define DEVICETYPE_MASK                                              0x0000000f
#define DEVICETYPE_RD(src)                               (((src) & 0x0000000f))
#define DEVICETYPE_WR(src)                          (((u32)(src)) & 0x0000000f)
#define DEVICETYPE_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register RBUF_RAM_CTL	*/ 
/*	 Fields rmeb	 */
#define PCIE_RMEB_WIDTH                                                       1
#define PCIE_RMEB_SHIFT                                                       5
#define PCIE_RMEB_MASK                                               0x00000020
#define PCIE_RMEB_RD(src)                             (((src) & 0x00000020)>>5)
#define PCIE_RMEB_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define PCIE_RMEB_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rmb	 */
#define PCIE_RMB_WIDTH                                                        2
#define PCIE_RMB_SHIFT                                                        3
#define PCIE_RMB_MASK                                                0x00000018
#define PCIE_RMB_RD(src)                              (((src) & 0x00000018)>>3)
#define PCIE_RMB_WR(src)                         (((u32)(src)<<3) & 0x00000018)
#define PCIE_RMB_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields rmea	 */
#define PCIE_RMEA_WIDTH                                                       1
#define PCIE_RMEA_SHIFT                                                       2
#define PCIE_RMEA_MASK                                               0x00000004
#define PCIE_RMEA_RD(src)                             (((src) & 0x00000004)>>2)
#define PCIE_RMEA_WR(src)                        (((u32)(src)<<2) & 0x00000004)
#define PCIE_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rma	 */
#define PCIE_RMA_WIDTH                                                        2
#define PCIE_RMA_SHIFT                                                        0
#define PCIE_RMA_MASK                                                0x00000003
#define PCIE_RMA_RD(src)                                 (((src) & 0x00000003))
#define PCIE_RMA_WR(src)                            (((u32)(src)) & 0x00000003)
#define PCIE_RMA_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register RADM_DATAQ_RAM_CTL	*/ 
/*	 Fields rmeb	 */
#define PCIE_RMEB_F1_WIDTH                                                    1
#define PCIE_RMEB_F1_SHIFT                                                    5
#define PCIE_RMEB_F1_MASK                                            0x00000020
#define PCIE_RMEB_F1_RD(src)                          (((src) & 0x00000020)>>5)
#define PCIE_RMEB_F1_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define PCIE_RMEB_F1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rmb	 */
#define PCIE_RMB_F1_WIDTH                                                     2
#define PCIE_RMB_F1_SHIFT                                                     3
#define PCIE_RMB_F1_MASK                                             0x00000018
#define PCIE_RMB_F1_RD(src)                           (((src) & 0x00000018)>>3)
#define PCIE_RMB_F1_WR(src)                      (((u32)(src)<<3) & 0x00000018)
#define PCIE_RMB_F1_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields rmea	 */
#define PCIE_RMEA_F1_WIDTH                                                    1
#define PCIE_RMEA_F1_SHIFT                                                    2
#define PCIE_RMEA_F1_MASK                                            0x00000004
#define PCIE_RMEA_F1_RD(src)                          (((src) & 0x00000004)>>2)
#define PCIE_RMEA_F1_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define PCIE_RMEA_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rma	 */
#define PCIE_RMA_F1_WIDTH                                                     2
#define PCIE_RMA_F1_SHIFT                                                     0
#define PCIE_RMA_F1_MASK                                             0x00000003
#define PCIE_RMA_F1_RD(src)                              (((src) & 0x00000003))
#define PCIE_RMA_F1_WR(src)                         (((u32)(src)) & 0x00000003)
#define PCIE_RMA_F1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register GM_COMP_RAM_CTL	*/ 
/*	 Fields rmeb	 */
#define PCIE_RMEB_F2_WIDTH                                                    1
#define PCIE_RMEB_F2_SHIFT                                                    5
#define PCIE_RMEB_F2_MASK                                            0x00000020
#define PCIE_RMEB_F2_RD(src)                          (((src) & 0x00000020)>>5)
#define PCIE_RMEB_F2_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define PCIE_RMEB_F2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rmb	 */
#define PCIE_RMB_F2_WIDTH                                                     2
#define PCIE_RMB_F2_SHIFT                                                     3
#define PCIE_RMB_F2_MASK                                             0x00000018
#define PCIE_RMB_F2_RD(src)                           (((src) & 0x00000018)>>3)
#define PCIE_RMB_F2_WR(src)                      (((u32)(src)<<3) & 0x00000018)
#define PCIE_RMB_F2_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields rmea	 */
#define PCIE_RMEA_F2_WIDTH                                                    1
#define PCIE_RMEA_F2_SHIFT                                                    2
#define PCIE_RMEA_F2_MASK                                            0x00000004
#define PCIE_RMEA_F2_RD(src)                          (((src) & 0x00000004)>>2)
#define PCIE_RMEA_F2_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define PCIE_RMEA_F2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rma	 */
#define PCIE_RMA_F2_WIDTH                                                     2
#define PCIE_RMA_F2_SHIFT                                                     0
#define PCIE_RMA_F2_MASK                                             0x00000003
#define PCIE_RMA_F2_RD(src)                              (((src) & 0x00000003))
#define PCIE_RMA_F2_WR(src)                         (((u32)(src)) & 0x00000003)
#define PCIE_RMA_F2_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register PcieBid	*/ 
/*	 Fields revno	 */
#define PCIE_REVNO_WIDTH                                                      2
#define PCIE_REVNO_SHIFT                                                      8
#define PCIE_REVNO_MASK                                              0x00000300
#define PCIE_REVNO_RD(src)                            (((src) & 0x00000300)>>8)
#define PCIE_REVNO_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define PCIE_BUSID_WIDTH                                                      3
#define PCIE_BUSID_SHIFT                                                      5
#define PCIE_BUSID_MASK                                              0x000000e0
#define PCIE_BUSID_RD(src)                            (((src) & 0x000000e0)>>5)
#define PCIE_BUSID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define PCIE_DEVICEID_WIDTH                                                   5
#define PCIE_DEVICEID_SHIFT                                                   0
#define PCIE_DEVICEID_MASK                                           0x0000001f
#define PCIE_DEVICEID_RD(src)                            (((src) & 0x0000001f))
#define PCIE_DEVICEID_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register PCIERSTCTRL	*/ 
/*	 Fields NonStickyRst	 */
#define NONSTICKYRST_WIDTH                                                    1
#define NONSTICKYRST_SHIFT                                                    2
#define NONSTICKYRST_MASK                                            0x00000004
#define NONSTICKYRST_RD(src)                          (((src) & 0x00000004)>>2)
#define NONSTICKYRST_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define NONSTICKYRST_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields StickyRst	 */
#define STICKYRST_WIDTH                                                       1
#define STICKYRST_SHIFT                                                       1
#define STICKYRST_MASK                                               0x00000002
#define STICKYRST_RD(src)                             (((src) & 0x00000002)>>1)
#define STICKYRST_WR(src)                        (((u32)(src)<<1) & 0x00000002)
#define STICKYRST_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CoreRst	 */
#define CORERST_WIDTH                                                         1
#define CORERST_SHIFT                                                         0
#define CORERST_MASK                                                 0x00000001
#define CORERST_RD(src)                                  (((src) & 0x00000001))
#define CORERST_WR(src)                             (((u32)(src)) & 0x00000001)
#define CORERST_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SwInt	*/ 
/*	 Fields Sw_event_int	 */
#define SW_EVENT_INT_WIDTH                                                    1
#define SW_EVENT_INT_SHIFT                                                    5
#define SW_EVENT_INT_MASK                                            0x00000020
#define SW_EVENT_INT_RD(src)                          (((src) & 0x00000020)>>5)
#define SW_EVENT_INT_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define SW_EVENT_INT_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields Sw_error_int	 */
#define SW_ERROR_INT_WIDTH                                                    1
#define SW_ERROR_INT_SHIFT                                                    4
#define SW_ERROR_INT_MASK                                            0x00000010
#define SW_ERROR_INT_RD(src)                          (((src) & 0x00000010)>>4)
#define SW_ERROR_INT_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define SW_ERROR_INT_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields sw_intd	 */
#define SW_INTD_WIDTH                                                         1
#define SW_INTD_SHIFT                                                         3
#define SW_INTD_MASK                                                 0x00000008
#define SW_INTD_RD(src)                               (((src) & 0x00000008)>>3)
#define SW_INTD_WR(src)                          (((u32)(src)<<3) & 0x00000008)
#define SW_INTD_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields sw_intc	 */
#define SW_INTC_WIDTH                                                         1
#define SW_INTC_SHIFT                                                         2
#define SW_INTC_MASK                                                 0x00000004
#define SW_INTC_RD(src)                               (((src) & 0x00000004)>>2)
#define SW_INTC_WR(src)                          (((u32)(src)<<2) & 0x00000004)
#define SW_INTC_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields sw_intb	 */
#define SW_INTB_WIDTH                                                         1
#define SW_INTB_SHIFT                                                         1
#define SW_INTB_MASK                                                 0x00000002
#define SW_INTB_RD(src)                               (((src) & 0x00000002)>>1)
#define SW_INTB_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define SW_INTB_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields sw_inta	 */
#define SW_INTA_WIDTH                                                         1
#define SW_INTA_SHIFT                                                         0
#define SW_INTA_MASK                                                 0x00000001
#define SW_INTA_RD(src)                                  (((src) & 0x00000001))
#define SW_INTA_WR(src)                             (((u32)(src)) & 0x00000001)
#define SW_INTA_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define PCIE_CSR_REGS1_BASE_ADDR			0xddd880000ULL
#define PCIE_CSR_REGS2_BASE_ADDR			0xddd890000ULL
#define PCIE_CSR_REGS3_BASE_ADDR			0xddd8f0000ULL

/*    Address P1_PCIE_CSR_PORT1  Registers */
#define PIM1_1L_ADDR                                                 0x00001010
#define PIM1_1L_DEFAULT                                              0x00000000
#define PIM1_1H_ADDR                                                 0x00001014
#define PIM1_1H_DEFAULT                                              0x00000000
#define PIM1_2L_ADDR                                                 0x00001018
#define PIM1_2L_DEFAULT                                              0x00000000
#define PIM1_2H_ADDR                                                 0x0000101c
#define PIM1_2H_DEFAULT                                              0x00000000
#define PIM1SL_ADDR                                                  0x00001028
#define PIM1SL_DEFAULT                                               0x00000000
#define PIM1SH_ADDR                                                  0x0000102c
#define PIM1SH_DEFAULT                                               0x00000000
#define IBAR2L_ADDR                                                  0x00001000
#define IBAR2L_DEFAULT                                               0x00000004
#define IBAR2H_ADDR                                                  0x00001004
#define IBAR2H_DEFAULT                                               0x00000000
#define IR2MSKL_ADDR                                                 0x00001050
#define IR2MSKL_DEFAULT                                              0x00000000
#define IR2MSKH_ADDR                                                 0x00001054
#define IR2MSKH_DEFAULT                                              0x00000000
#define PIM2_1L_ADDR                                                 0x00001040
#define PIM2_1L_DEFAULT                                              0x00000000
#define PIM2_1H_ADDR                                                 0x00001044
#define PIM2_1H_DEFAULT                                              0x00000000
#define PIM2_2L_ADDR                                                 0x00001048
#define PIM2_2L_DEFAULT                                              0x00000000
#define PIM2_2H_ADDR                                                 0x0000104c
#define PIM2_2H_DEFAULT                                              0x00000000
#define PIM2SL_ADDR                                                  0x00001058
#define PIM2SL_DEFAULT                                               0x00000000
#define PIM2SH_ADDR                                                  0x0000105c
#define PIM2SH_DEFAULT                                               0x00000000
#define IBAR3_ADDR                                                   0x00001008
#define IBAR3_DEFAULT                                                0x00000001
#define PIM3L_ADDR                                                   0x00001030
#define PIM3L_DEFAULT                                                0x00000000
#define PIM3H_ADDR                                                   0x00001034
#define PIM3H_DEFAULT                                                0x00000000
#define IR3MSK_ADDR                                                  0x00001038
#define IR3MSK_DEFAULT                                               0x00000000
#define PIM4L_ADDR                                                   0x00001060
#define PIM4L_DEFAULT                                                0x00000000
#define PIM4H_ADDR                                                   0x00001064
#define PIM4H_DEFAULT                                                0x00000000
#define OMR1BARL_ADDR                                                0x00001100
#define OMR1BARL_DEFAULT                                             0x00000000
#define OMR1BARH_ADDR                                                0x00001104
#define OMR1BARH_DEFAULT                                             0x00000000
#define OMR1MSKL_ADDR                                                0x00001108
#define OMR1MSKL_DEFAULT                                             0x00000000
#define OMR1MSKH_ADDR                                                0x0000110c
#define OMR1MSKH_DEFAULT                                             0x00000000
#define POM1L_ADDR                                                   0x00001110
#define POM1L_DEFAULT                                                0x00000000
#define POM1H_ADDR                                                   0x00001114
#define POM1H_DEFAULT                                                0x00000000
#define OMR2BARL_ADDR                                                0x00001120
#define OMR2BARL_DEFAULT                                             0x00000000
#define OMR2BARH_ADDR                                                0x00001124
#define OMR2BARH_DEFAULT                                             0x00000000
#define OMR2MSKL_ADDR                                                0x00001128
#define OMR2MSKL_DEFAULT                                             0x00000000
#define OMR2MSKH_ADDR                                                0x0000112c
#define OMR2MSKH_DEFAULT                                             0x00000000
#define POM2L_ADDR                                                   0x00001130
#define POM2L_DEFAULT                                                0x00000000
#define POM2H_ADDR                                                   0x00001134
#define POM2H_DEFAULT                                                0x00000000
#define OMR3BARL_ADDR                                                0x00001140
#define OMR3BARL_DEFAULT                                             0x00000000
#define OMR3BARH_ADDR                                                0x00001144
#define OMR3BARH_DEFAULT                                             0x00000000
#define OMR3MSKL_ADDR                                                0x00001148
#define OMR3MSKL_DEFAULT                                             0x00000000
#define OMR3MSKH_ADDR                                                0x0000114c
#define OMR3MSKH_DEFAULT                                             0x00000000
#define POM3L_ADDR                                                   0x00001150
#define POM3L_DEFAULT                                                0x00000000
#define POM3H_ADDR                                                   0x00001154
#define POM3H_DEFAULT                                                0x00000000
#define MSGBARL_ADDR                                                 0x00001160
#define MSGBARL_DEFAULT                                              0x00000000
#define MSGBARH_ADDR                                                 0x00001164
#define MSGBARH_DEFAULT                                              0x00000000
#define MSGMSK_ADDR                                                  0x00001168
#define MSGMSK_DEFAULT                                               0x00000000
#define POM4L_ADDR                                                   0x00001170
#define POM4L_DEFAULT                                                0x00000000
#define POM4H_ADDR                                                   0x00001174
#define POM4H_DEFAULT                                                0x00000000
#define CFGBARL_ADDR                                                 0x00001180
#define CFGBARL_DEFAULT                                              0x00000000
#define CFGBARH_ADDR                                                 0x00001184
#define CFGBARH_DEFAULT                                              0x00000000
#define CFGMSK_ADDR                                                  0x00001188
#define CFGMSK_DEFAULT                                               0x00000000
#define RTDID_ADDR                                                   0x0000118c
#define RTDID_DEFAULT                                                0x00080000
#define POM5L_ADDR                                                   0x00001190
#define POM5L_DEFAULT                                                0x00000000
#define POM5H_ADDR                                                   0x00001194
#define POM5H_DEFAULT                                                0x00000000
#define PCIECORECTRL_ADDR                                            0x00001200
#define PCIECORECTRL_DEFAULT                                         0x00000800
#define PCIECORESTATUS_ADDR                                          0x00001204
#define PCIECORESTATUS_DEFAULT                                       0x00000000
#define PCIEMISCCTRL_ADDR                                            0x00001208
#define PCIEMISCCTRL_DEFAULT                                         0x00000000
#define PCIEDIAGNOSTIC_ADDR                                          0x0000120c
#define PCIEDIAGNOSTIC_DEFAULT                                       0x00000000
#define PCIEAPPRST_ADDR                                              0x00001210
#define PCIEAPPRST_DEFAULT                                           0x00000000

/*	Register PIM1_1L	*/ 
/*	 Fields Bal	 */
#define BAL_WIDTH                                                            22
#define BAL_SHIFT                                                            10
#define BAL_MASK                                                     0xfffffc00
#define BAL_RD(src)                                  (((src) & 0xfffffc00)>>10)
#define BAL_WR(src)                             (((u32)(src)<<10) & 0xfffffc00)
#define BAL_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))

/*	Register PIM1_1H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_WIDTH                                                          1
#define WRATTR_SHIFT                                                          5
#define WRATTR_MASK                                                  0x00000020
#define WRATTR_RD(src)                                (((src) & 0x00000020)>>5)
#define WRATTR_WR(src)                           (((u32)(src)<<5) & 0x00000020)
#define WRATTR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_WIDTH                                                          1
#define RDATTR_SHIFT                                                          4
#define RDATTR_MASK                                                  0x00000010
#define RDATTR_RD(src)                                (((src) & 0x00000010)>>4)
#define RDATTR_WR(src)                           (((u32)(src)<<4) & 0x00000010)
#define RDATTR_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_WIDTH                                                             4
#define BAH_SHIFT                                                             0
#define BAH_MASK                                                     0x0000000f
#define BAH_RD(src)                                      (((src) & 0x0000000f))
#define BAH_WR(src)                                 (((u32)(src)) & 0x0000000f)
#define BAH_SET(dst,src) (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PIM1_2L	*/ 
/*	 Fields Bal	 */
#define BAL_F1_WIDTH                                                         12
#define BAL_F1_SHIFT                                                         20
#define BAL_F1_MASK                                                  0xfff00000
#define BAL_F1_RD(src)                               (((src) & 0xfff00000)>>20)
#define BAL_F1_WR(src)                          (((u32)(src)<<20) & 0xfff00000)
#define BAL_F1_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))

/*	Register PIM1_2H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_F1_WIDTH                                                       1
#define WRATTR_F1_SHIFT                                                       5
#define WRATTR_F1_MASK                                               0x00000020
#define WRATTR_F1_RD(src)                             (((src) & 0x00000020)>>5)
#define WRATTR_F1_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define WRATTR_F1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_F1_WIDTH                                                       1
#define RDATTR_F1_SHIFT                                                       4
#define RDATTR_F1_MASK                                               0x00000010
#define RDATTR_F1_RD(src)                             (((src) & 0x00000010)>>4)
#define RDATTR_F1_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDATTR_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_F1_WIDTH                                                          4
#define BAH_F1_SHIFT                                                          0
#define BAH_F1_MASK                                                  0x0000000f
#define BAH_F1_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F1_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PIM1SL	*/ 
/*	 Fields Bal	 */
#define BAL_F2_WIDTH                                                         22
#define BAL_F2_SHIFT                                                         10
#define BAL_F2_MASK                                                  0xfffffc00
#define BAL_F2_RD(src)                               (((src) & 0xfffffc00)>>10)
#define BAL_F2_WR(src)                          (((u32)(src)<<10) & 0xfffffc00)
#define BAL_F2_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))

/*	Register PIM1SH	*/ 
/*	 Fields Bah	 */
#define BAH_F2_WIDTH                                                          4
#define BAH_F2_SHIFT                                                          0
#define BAH_F2_MASK                                                  0x0000000f
#define BAH_F2_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F2_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register IBAR2L	*/ 
/*	 Fields Bal	 */
#define BAL_F3_WIDTH                                                         12
#define BAL_F3_SHIFT                                                         20
#define BAL_F3_MASK                                                  0xfff00000
#define BAL_F3_RD(src)                               (((src) & 0xfff00000)>>20)
#define BAL_F3_WR(src)                          (((u32)(src)<<20) & 0xfff00000)
#define BAL_F3_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))
/*	 Fields PREF	 */
#define PREF_WIDTH                                                            1
#define PREF_SHIFT                                                            3
#define PREF_MASK                                                    0x00000008
#define PREF_RD(src)                                  (((src) & 0x00000008)>>3)
#define PREF_WR(src)                             (((u32)(src)<<3) & 0x00000008)
#define PREF_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields LT	 */
#define LT_WIDTH                                                              2
#define LT_SHIFT                                                              1
#define LT_MASK                                                      0x00000006
#define LT_RD(src)                                    (((src) & 0x00000006)>>1)
#define LT_WR(src)                               (((u32)(src)<<1) & 0x00000006)
#define LT_SET(dst,src) \
                       (((dst) & ~0x00000006) | (((u32)(src)<<1) & 0x00000006))
/*	 Fields MSI	 */
#define MSI_WIDTH                                                             1
#define MSI_SHIFT                                                             0
#define MSI_MASK                                                     0x00000001
#define MSI_RD(src)                                      (((src) & 0x00000001))
#define MSI_SET(dst,src) (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IBAR2H	*/ 
/*	 Fields Bah	 */
#define BAH_F3_WIDTH                                                         32
#define BAH_F3_SHIFT                                                          0
#define BAH_F3_MASK                                                  0xffffffff
#define BAH_F3_RD(src)                                   (((src) & 0xffffffff))
#define BAH_F3_WR(src)                              (((u32)(src)) & 0xffffffff)
#define BAH_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IR2MSKL	*/ 
/*	 Fields Baml	 */
#define BAML_WIDTH                                                           12
#define BAML_SHIFT                                                           20
#define BAML_MASK                                                    0xfff00000
#define BAML_RD(src)                                 (((src) & 0xfff00000)>>20)
#define BAML_WR(src)                            (((u32)(src)<<20) & 0xfff00000)
#define BAML_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))
/*	 Fields Val	 */
#define VAL_WIDTH                                                             1
#define VAL_SHIFT                                                             0
#define VAL_MASK                                                     0x00000001
#define VAL_RD(src)                                      (((src) & 0x00000001))
#define VAL_WR(src)                                 (((u32)(src)) & 0x00000001)
#define VAL_SET(dst,src) (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IR2MSKH	*/ 
/*	 Fields Bamh	 */
#define BAMH_WIDTH                                                            4
#define BAMH_SHIFT                                                            0
#define BAMH_MASK                                                    0x0000000f
#define BAMH_RD(src)                                     (((src) & 0x0000000f))
#define BAMH_WR(src)                                (((u32)(src)) & 0x0000000f)
#define BAMH_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PIM2_1L	*/ 
/*	 Fields Bal	 */
#define BAL_F4_WIDTH                                                         22
#define BAL_F4_SHIFT                                                         10
#define BAL_F4_MASK                                                  0xfffffc00
#define BAL_F4_RD(src)                               (((src) & 0xfffffc00)>>10)
#define BAL_F4_WR(src)                          (((u32)(src)<<10) & 0xfffffc00)
#define BAL_F4_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))

/*	Register PIM2_1H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_F2_WIDTH                                                       1
#define WRATTR_F2_SHIFT                                                       5
#define WRATTR_F2_MASK                                               0x00000020
#define WRATTR_F2_RD(src)                             (((src) & 0x00000020)>>5)
#define WRATTR_F2_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define WRATTR_F2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_F2_WIDTH                                                       1
#define RDATTR_F2_SHIFT                                                       4
#define RDATTR_F2_MASK                                               0x00000010
#define RDATTR_F2_RD(src)                             (((src) & 0x00000010)>>4)
#define RDATTR_F2_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDATTR_F2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_F4_WIDTH                                                          4
#define BAH_F4_SHIFT                                                          0
#define BAH_F4_MASK                                                  0x0000000f
#define BAH_F4_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F4_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F4_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PIM2_2L	*/ 
/*	 Fields Bal	 */
#define BAL_F5_WIDTH                                                         12
#define BAL_F5_SHIFT                                                         20
#define BAL_F5_MASK                                                  0xfff00000
#define BAL_F5_RD(src)                               (((src) & 0xfff00000)>>20)
#define BAL_F5_WR(src)                          (((u32)(src)<<20) & 0xfff00000)
#define BAL_F5_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))

/*	Register PIM2_2H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_F3_WIDTH                                                       1
#define WRATTR_F3_SHIFT                                                       5
#define WRATTR_F3_MASK                                               0x00000020
#define WRATTR_F3_RD(src)                             (((src) & 0x00000020)>>5)
#define WRATTR_F3_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define WRATTR_F3_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_F3_WIDTH                                                       1
#define RDATTR_F3_SHIFT                                                       4
#define RDATTR_F3_MASK                                               0x00000010
#define RDATTR_F3_RD(src)                             (((src) & 0x00000010)>>4)
#define RDATTR_F3_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDATTR_F3_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_F5_WIDTH                                                          4
#define BAH_F5_SHIFT                                                          0
#define BAH_F5_MASK                                                  0x0000000f
#define BAH_F5_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F5_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F5_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PIM2SL	*/ 
/*	 Fields Bal	 */
#define BAL_F6_WIDTH                                                         22
#define BAL_F6_SHIFT                                                         10
#define BAL_F6_MASK                                                  0xfffffc00
#define BAL_F6_RD(src)                               (((src) & 0xfffffc00)>>10)
#define BAL_F6_WR(src)                          (((u32)(src)<<10) & 0xfffffc00)
#define BAL_F6_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))

/*	Register PIM2SH	*/ 
/*	 Fields Bah	 */
#define BAH_F6_WIDTH                                                          4
#define BAH_F6_SHIFT                                                          0
#define BAH_F6_MASK                                                  0x0000000f
#define BAH_F6_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F6_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F6_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register IBAR3	*/ 
/*	 Fields Bal	 */
#define BAL3_WIDTH                                                           25
#define BAL3_SHIFT                                                            7
#define BAL3_MASK                                                    0xffffff80
#define BAL3_RD(src)                                  (((src) & 0xffffff80)>>7)
#define BAL3_WR(src)                             (((u32)(src)<<7) & 0xffffff80)
#define BAL3_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))
/*	 Fields Spi	 */
#define SPI3_WIDTH                                                            1
#define SPI3_SHIFT                                                            0
#define SPI3_MASK                                                    0x00000001
#define SPI3_RD(src)                                     (((src) & 0x00000001))
#define SPI3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PIM3L	*/ 
/*	 Fields Bal	 */
#define BAL_F7_WIDTH                                                         25
#define BAL_F7_SHIFT                                                          7
#define BAL_F7_MASK                                                  0xffffff80
#define BAL_F7_RD(src)                                (((src) & 0xffffff80)>>7)
#define BAL_F7_WR(src)                           (((u32)(src)<<7) & 0xffffff80)
#define BAL_F7_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register PIM3H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_F4_WIDTH                                                       1
#define WRATTR_F4_SHIFT                                                       5
#define WRATTR_F4_MASK                                               0x00000020
#define WRATTR_F4_RD(src)                             (((src) & 0x00000020)>>5)
#define WRATTR_F4_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define WRATTR_F4_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_F4_WIDTH                                                       1
#define RDATTR_F4_SHIFT                                                       4
#define RDATTR_F4_MASK                                               0x00000010
#define RDATTR_F4_RD(src)                             (((src) & 0x00000010)>>4)
#define RDATTR_F4_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDATTR_F4_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_F7_WIDTH                                                          4
#define BAH_F7_SHIFT                                                          0
#define BAH_F7_MASK                                                  0x0000000f
#define BAH_F7_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F7_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F7_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register IR3MSK	*/ 
/*	 Fields Bam	 */
#define BAM_WIDTH                                                             8
#define BAM_SHIFT                                                             7
#define BAM_MASK                                                     0x00007f80
#define BAM_RD(src)                                   (((src) & 0x00007f80)>>7)
#define BAM_WR(src)                              (((u32)(src)<<7) & 0x00007f80)
#define BAM_SET(dst,src) \
                       (((dst) & ~0x00007f80) | (((u32)(src)<<7) & 0x00007f80))
/*	 Fields Val	 */
#define VAL_F1_WIDTH                                                          1
#define VAL_F1_SHIFT                                                          0
#define VAL_F1_MASK                                                  0x00000001
#define VAL_F1_RD(src)                                   (((src) & 0x00000001))
#define VAL_F1_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PIM4L	*/ 
/*	 Fields Bal	 */
#define BAL_F8_WIDTH                                                         21
#define BAL_F8_SHIFT                                                         11
#define BAL_F8_MASK                                                  0xfffff800
#define BAL_F8_RD(src)                               (((src) & 0xfffff800)>>11)
#define BAL_F8_WR(src)                          (((u32)(src)<<11) & 0xfffff800)
#define BAL_F8_SET(dst,src) \
                      (((dst) & ~0xfffff800) | (((u32)(src)<<11) & 0xfffff800))

/*	Register PIM4H	*/ 
/*	 Fields WrAttr	 */
#define WRATTR_F5_WIDTH                                                       1
#define WRATTR_F5_SHIFT                                                       5
#define WRATTR_F5_MASK                                               0x00000020
#define WRATTR_F5_RD(src)                             (((src) & 0x00000020)>>5)
#define WRATTR_F5_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define WRATTR_F5_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdAttr	 */
#define RDATTR_F5_WIDTH                                                       1
#define RDATTR_F5_SHIFT                                                       4
#define RDATTR_F5_MASK                                               0x00000010
#define RDATTR_F5_RD(src)                             (((src) & 0x00000010)>>4)
#define RDATTR_F5_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDATTR_F5_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Bah	 */
#define BAH_F8_WIDTH                                                          4
#define BAH_F8_SHIFT                                                          0
#define BAH_F8_MASK                                                  0x0000000f
#define BAH_F8_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F8_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F8_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register OMR1BARL	*/ 
/*	 Fields Bal	 */
#define BAL_F9_WIDTH                                                          5
#define BAL_F9_SHIFT                                                         27
#define BAL_F9_MASK                                                  0xf8000000
#define BAL_F9_RD(src)                               (((src) & 0xf8000000)>>27)
#define BAL_F9_WR(src)                          (((u32)(src)<<27) & 0xf8000000)
#define BAL_F9_SET(dst,src) \
                      (((dst) & ~0xf8000000) | (((u32)(src)<<27) & 0xf8000000))

/*	Register OMR1BARH	*/ 
/*	 Fields Bah	 */
#define BAH_F9_WIDTH                                                          4
#define BAH_F9_SHIFT                                                          0
#define BAH_F9_MASK                                                  0x0000000f
#define BAH_F9_RD(src)                                   (((src) & 0x0000000f))
#define BAH_F9_WR(src)                              (((u32)(src)) & 0x0000000f)
#define BAH_F9_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register OMR1MSKL	*/ 
/*	 Fields Baml	 */
#define BAML_F1_WIDTH                                                         5
#define BAML_F1_SHIFT                                                        27
#define BAML_F1_MASK                                                 0xf8000000
#define BAML_F1_RD(src)                              (((src) & 0xf8000000)>>27)
#define BAML_F1_WR(src)                         (((u32)(src)<<27) & 0xf8000000)
#define BAML_F1_SET(dst,src) \
                      (((dst) & ~0xf8000000) | (((u32)(src)<<27) & 0xf8000000))
/*	 Fields Val	 */
#define VAL_F2_WIDTH                                                          1
#define VAL_F2_SHIFT                                                          0
#define VAL_F2_MASK                                                  0x00000001
#define VAL_F2_RD(src)                                   (((src) & 0x00000001))
#define VAL_F2_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register OMR1MSKH	*/ 
/*	 Fields Bamh	 */
#define BAMH_F1_WIDTH                                                         4
#define BAMH_F1_SHIFT                                                         0
#define BAMH_F1_MASK                                                 0x0000000f
#define BAMH_F1_RD(src)                                  (((src) & 0x0000000f))
#define BAMH_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAMH_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register POM1L	*/ 
/*	 Fields Bal	 */
#define BAL_F10_WIDTH                                                         5
#define BAL_F10_SHIFT                                                        27
#define BAL_F10_MASK                                                 0xf8000000
#define BAL_F10_RD(src)                              (((src) & 0xf8000000)>>27)
#define BAL_F10_WR(src)                         (((u32)(src)<<27) & 0xf8000000)
#define BAL_F10_SET(dst,src) \
                      (((dst) & ~0xf8000000) | (((u32)(src)<<27) & 0xf8000000))

/*	Register POM1H	*/ 
/*	 Fields Bah	 */
#define BAH_F10_WIDTH                                                        32
#define BAH_F10_SHIFT                                                         0
#define BAH_F10_MASK                                                 0xffffffff
#define BAH_F10_RD(src)                                  (((src) & 0xffffffff))
#define BAH_F10_WR(src)                             (((u32)(src)) & 0xffffffff)
#define BAH_F10_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register OMR2BARL	*/ 
/*	 Fields Bal	 */
#define BAL_F11_WIDTH                                                        12
#define BAL_F11_SHIFT                                                        20
#define BAL_F11_MASK                                                 0xfff00000
#define BAL_F11_RD(src)                              (((src) & 0xfff00000)>>20)
#define BAL_F11_WR(src)                         (((u32)(src)<<20) & 0xfff00000)
#define BAL_F11_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))

/*	Register OMR2BARH	*/ 
/*	 Fields Bah	 */
#define BAH_F11_WIDTH                                                         4
#define BAH_F11_SHIFT                                                         0
#define BAH_F11_MASK                                                 0x0000000f
#define BAH_F11_RD(src)                                  (((src) & 0x0000000f))
#define BAH_F11_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAH_F11_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register OMR2MSKL	*/ 
/*	 Fields Baml	 */
#define BAML_F2_WIDTH                                                        12
#define BAML_F2_SHIFT                                                        20
#define BAML_F2_MASK                                                 0xfff00000
#define BAML_F2_RD(src)                              (((src) & 0xfff00000)>>20)
#define BAML_F2_WR(src)                         (((u32)(src)<<20) & 0xfff00000)
#define BAML_F2_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))
/*	 Fields Val	 */
#define VAL_F3_WIDTH                                                          1
#define VAL_F3_SHIFT                                                          0
#define VAL_F3_MASK                                                  0x00000001
#define VAL_F3_RD(src)                                   (((src) & 0x00000001))
#define VAL_F3_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register OMR2MSKH	*/ 
/*	 Fields Bamh	 */
#define BAMH_F2_WIDTH                                                         4
#define BAMH_F2_SHIFT                                                         0
#define BAMH_F2_MASK                                                 0x0000000f
#define BAMH_F2_RD(src)                                  (((src) & 0x0000000f))
#define BAMH_F2_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAMH_F2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register POM2L	*/ 
/*	 Fields Bal	 */
#define BAL_F12_WIDTH                                                        12
#define BAL_F12_SHIFT                                                        20
#define BAL_F12_MASK                                                 0xfff00000
#define BAL_F12_RD(src)                              (((src) & 0xfff00000)>>20)
#define BAL_F12_WR(src)                         (((u32)(src)<<20) & 0xfff00000)
#define BAL_F12_SET(dst,src) \
                      (((dst) & ~0xfff00000) | (((u32)(src)<<20) & 0xfff00000))

/*	Register POM2H	*/ 
/*	 Fields Bah	 */
#define BAH_F12_WIDTH                                                        32
#define BAH_F12_SHIFT                                                         0
#define BAH_F12_MASK                                                 0xffffffff
#define BAH_F12_RD(src)                                  (((src) & 0xffffffff))
#define BAH_F12_WR(src)                             (((u32)(src)) & 0xffffffff)
#define BAH_F12_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register OMR3BARL	*/ 
/*	 Fields Bal	 */
#define BAL_F13_WIDTH                                                        25
#define BAL_F13_SHIFT                                                         7
#define BAL_F13_MASK                                                 0xffffff80
#define BAL_F13_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F13_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F13_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register OMR3BARH	*/ 
/*	 Fields Bah	 */
#define BAH_F13_WIDTH                                                         4
#define BAH_F13_SHIFT                                                         0
#define BAH_F13_MASK                                                 0x0000000f
#define BAH_F13_RD(src)                                  (((src) & 0x0000000f))
#define BAH_F13_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAH_F13_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register OMR3MSKL	*/ 
/*	 Fields Baml	 */
#define BAML_F3_WIDTH                                                        25
#define BAML_F3_SHIFT                                                         7
#define BAML_F3_MASK                                                 0xffffff80
#define BAML_F3_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAML_F3_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAML_F3_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))
/*	 Fields IO	 */
#define IO_WIDTH                                                              1
#define IO_SHIFT                                                              1
#define IO_MASK                                                      0x00000002
#define IO_RD(src)                                    (((src) & 0x00000002)>>1)
#define IO_WR(src)                               (((u32)(src)<<1) & 0x00000002)
#define IO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Val	 */
#define VAL_F4_WIDTH                                                          1
#define VAL_F4_SHIFT                                                          0
#define VAL_F4_MASK                                                  0x00000001
#define VAL_F4_RD(src)                                   (((src) & 0x00000001))
#define VAL_F4_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register OMR3MSKH	*/ 
/*	 Fields Bamh	 */
#define BAMH_F3_WIDTH                                                         4
#define BAMH_F3_SHIFT                                                         0
#define BAMH_F3_MASK                                                 0x0000000f
#define BAMH_F3_RD(src)                                  (((src) & 0x0000000f))
#define BAMH_F3_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAMH_F3_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register POM3L	*/ 
/*	 Fields Bal	 */
#define BAL_F14_WIDTH                                                        25
#define BAL_F14_SHIFT                                                         7
#define BAL_F14_MASK                                                 0xffffff80
#define BAL_F14_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F14_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F14_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register POM3H	*/ 
/*	 Fields Bah	 */
#define BAH_F14_WIDTH                                                        32
#define BAH_F14_SHIFT                                                         0
#define BAH_F14_MASK                                                 0xffffffff
#define BAH_F14_RD(src)                                  (((src) & 0xffffffff))
#define BAH_F14_WR(src)                             (((u32)(src)) & 0xffffffff)
#define BAH_F14_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MSGBARL	*/ 
/*	 Fields Bal	 */
#define BAL_F15_WIDTH                                                        25
#define BAL_F15_SHIFT                                                         7
#define BAL_F15_MASK                                                 0xffffff80
#define BAL_F15_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F15_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F15_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register MSGBARH	*/ 
/*	 Fields Bah	 */
#define BAH_F15_WIDTH                                                         4
#define BAH_F15_SHIFT                                                         0
#define BAH_F15_MASK                                                 0x0000000f
#define BAH_F15_RD(src)                                  (((src) & 0x0000000f))
#define BAH_F15_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAH_F15_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register MSGMSK	*/ 
/*	 Fields Bam	 */
#define BAM_F1_WIDTH                                                          8
#define BAM_F1_SHIFT                                                          7
#define BAM_F1_MASK                                                  0x00007f80
#define BAM_F1_RD(src)                                (((src) & 0x00007f80)>>7)
#define BAM_F1_WR(src)                           (((u32)(src)<<7) & 0x00007f80)
#define BAM_F1_SET(dst,src) \
                       (((dst) & ~0x00007f80) | (((u32)(src)<<7) & 0x00007f80))
/*	 Fields Val	 */
#define VAL_F5_WIDTH                                                          1
#define VAL_F5_SHIFT                                                          0
#define VAL_F5_MASK                                                  0x00000001
#define VAL_F5_RD(src)                                   (((src) & 0x00000001))
#define VAL_F5_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register POM4L	*/ 
/*	 Fields Bal	 */
#define BAL_F16_WIDTH                                                        25
#define BAL_F16_SHIFT                                                         7
#define BAL_F16_MASK                                                 0xffffff80
#define BAL_F16_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F16_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F16_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register POM4H	*/ 
/*	 Fields Bah	 */
#define BAH_F16_WIDTH                                                        32
#define BAH_F16_SHIFT                                                         0
#define BAH_F16_MASK                                                 0xffffffff
#define BAH_F16_RD(src)                                  (((src) & 0xffffffff))
#define BAH_F16_WR(src)                             (((u32)(src)) & 0xffffffff)
#define BAH_F16_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFGBARL	*/ 
/*	 Fields Bal	 */
#define BAL_F17_WIDTH                                                        25
#define BAL_F17_SHIFT                                                         7
#define BAL_F17_MASK                                                 0xffffff80
#define BAL_F17_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F17_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F17_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register CFGBARH	*/ 
/*	 Fields Bah	 */
#define BAH_F17_WIDTH                                                         4
#define BAH_F17_SHIFT                                                         0
#define BAH_F17_MASK                                                 0x0000000f
#define BAH_F17_RD(src)                                  (((src) & 0x0000000f))
#define BAH_F17_WR(src)                             (((u32)(src)) & 0x0000000f)
#define BAH_F17_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CFGMSK	*/ 
/*	 Fields Baml	 */
#define BAML_F4_WIDTH                                                        25
#define BAML_F4_SHIFT                                                         7
#define BAML_F4_MASK                                                 0xffffff80
#define BAML_F4_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAML_F4_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAML_F4_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))
/*	 Fields Val	 */
#define VAL_F6_WIDTH                                                          1
#define VAL_F6_SHIFT                                                          0
#define VAL_F6_MASK                                                  0x00000001
#define VAL_F6_RD(src)                                   (((src) & 0x00000001))
#define VAL_F6_WR(src)                              (((u32)(src)) & 0x00000001)
#define VAL_F6_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RTDID	*/ 
/*	 Fields Bn	 */
#define BN_WIDTH                                                              8
#define BN_SHIFT                                                             24
#define BN_MASK                                                      0xff000000
#define BN_RD(src)                                   (((src) & 0xff000000)>>24)
#define BN_WR(src)                              (((u32)(src)<<24) & 0xff000000)
#define BN_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields Dn	 */
#define DN_WIDTH                                                              5
#define DN_SHIFT                                                             19
#define DN_MASK                                                      0x00f80000
#define DN_RD(src)                                   (((src) & 0x00f80000)>>19)
#define DN_WR(src)                              (((u32)(src)<<19) & 0x00f80000)
#define DN_SET(dst,src) \
                      (((dst) & ~0x00f80000) | (((u32)(src)<<19) & 0x00f80000))
/*	 Fields Fn	 */
#define FN_WIDTH                                                              3
#define FN_SHIFT                                                             16
#define FN_MASK                                                      0x00070000
#define FN_RD(src)                                   (((src) & 0x00070000)>>16)
#define FN_WR(src)                              (((u32)(src)<<16) & 0x00070000)
#define FN_SET(dst,src) \
                      (((dst) & ~0x00070000) | (((u32)(src)<<16) & 0x00070000))
/*	 Fields Cfg1	 */
#define CFG1_WIDTH                                                            1
#define CFG1_SHIFT                                                           15
#define CFG1_MASK                                                    0x00008000
#define CFG1_RD(src)                                 (((src) & 0x00008000)>>15)
#define CFG1_WR(src)                            (((u32)(src)<<15) & 0x00008000)
#define CFG1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register POM5L	*/ 
/*	 Fields Bal	 */
#define BAL_F18_WIDTH                                                        25
#define BAL_F18_SHIFT                                                         7
#define BAL_F18_MASK                                                 0xffffff80
#define BAL_F18_RD(src)                               (((src) & 0xffffff80)>>7)
#define BAL_F18_WR(src)                          (((u32)(src)<<7) & 0xffffff80)
#define BAL_F18_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))

/*	Register POM5H	*/ 
/*	 Fields Bah	 */
#define BAH_F18_WIDTH                                                        32
#define BAH_F18_SHIFT                                                         0
#define BAH_F18_MASK                                                 0xffffffff
#define BAH_F18_RD(src)                                  (((src) & 0xffffffff))
#define BAH_F18_WR(src)                             (((u32)(src)) & 0xffffffff)
#define BAH_F18_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PCIECORECTRL	*/ 
/*	 Fields PhyLatency	 */
#define PHYLATENCY_WIDTH                                                      4
#define PHYLATENCY_SHIFT                                                      8
#define PHYLATENCY_MASK                                              0x00000f00
#define PHYLATENCY_RD(src)                            (((src) & 0x00000f00)>>8)
#define PHYLATENCY_WR(src)                       (((u32)(src)<<8) & 0x00000f00)
#define PHYLATENCY_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields ClrLtssmEnLnkdn	 */
#define CLRLTSSMENLNKDN_WIDTH                                                 1
#define CLRLTSSMENLNKDN_SHIFT                                                 7
#define CLRLTSSMENLNKDN_MASK                                         0x00000080
#define CLRLTSSMENLNKDN_RD(src)                       (((src) & 0x00000080)>>7)
#define CLRLTSSMENLNKDN_WR(src)                  (((u32)(src)<<7) & 0x00000080)
#define CLRLTSSMENLNKDN_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields TxLaneFlipEn	 */
#define TXLANEFLIPEN_WIDTH                                                    1
#define TXLANEFLIPEN_SHIFT                                                    6
#define TXLANEFLIPEN_MASK                                            0x00000040
#define TXLANEFLIPEN_RD(src)                          (((src) & 0x00000040)>>6)
#define TXLANEFLIPEN_WR(src)                     (((u32)(src)<<6) & 0x00000040)
#define TXLANEFLIPEN_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RxLaneFlipEn	 */
#define RXLANEFLIPEN_WIDTH                                                    1
#define RXLANEFLIPEN_SHIFT                                                    5
#define RXLANEFLIPEN_MASK                                            0x00000020
#define RXLANEFLIPEN_RD(src)                          (((src) & 0x00000020)>>5)
#define RXLANEFLIPEN_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define RXLANEFLIPEN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AppLtssmEnable	 */
#define APPLTSSMENABLE_WIDTH                                                  1
#define APPLTSSMENABLE_SHIFT                                                  4
#define APPLTSSMENABLE_MASK                                          0x00000010
#define APPLTSSMENABLE_RD(src)                        (((src) & 0x00000010)>>4)
#define APPLTSSMENABLE_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define APPLTSSMENABLE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields DeviceType	 */
#define DEVICETYPE_F2_WIDTH                                                   4
#define DEVICETYPE_F2_SHIFT                                                   0
#define DEVICETYPE_F2_MASK                                           0x0000000f
#define DEVICETYPE_F2_RD(src)                            (((src) & 0x0000000f))
#define DEVICETYPE_F2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PCIECORESTATUS	*/ 
/*	 Fields ltssm_state	 */
#define LTSSM_STATE_WIDTH                                                     6
#define LTSSM_STATE_SHIFT                                                     6
#define LTSSM_STATE_MASK                                             0x00000fc0
#define LTSSM_STATE_RD(src)                           (((src) & 0x00000fc0)>>6)
#define LTSSM_STATE_SET(dst,src) \
                       (((dst) & ~0x00000fc0) | (((u32)(src)<<6) & 0x00000fc0))
/*	 Fields DmBridgeFlushDone	 */
#define DMBRIDGEFLUSHDONE_F2_WIDTH                                            1
#define DMBRIDGEFLUSHDONE_F2_SHIFT                                            5
#define DMBRIDGEFLUSHDONE_F2_MASK                                    0x00000020
#define DMBRIDGEFLUSHDONE_F2_RD(src)                  (((src) & 0x00000020)>>5)
#define DMBRIDGEFLUSHDONE_F2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RdhlLinkUp	 */
#define RDHLLINKUP_WIDTH                                                      1
#define RDHLLINKUP_SHIFT                                                      4
#define RDHLLINKUP_MASK                                              0x00000010
#define RDHLLINKUP_RD(src)                            (((src) & 0x00000010)>>4)
#define RDHLLINKUP_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields XmlhLinkUp	 */
#define XMLHLINKUP_WIDTH                                                      1
#define XMLHLINKUP_SHIFT                                                      3
#define XMLHLINKUP_MASK                                              0x00000008
#define XMLHLINKUP_RD(src)                            (((src) & 0x00000008)>>3)
#define XMLHLINKUP_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields AppParityErrs2	 */
#define APPPARITYERRS2_WIDTH                                                  1
#define APPPARITYERRS2_SHIFT                                                  2
#define APPPARITYERRS2_MASK                                          0x00000004
#define APPPARITYERRS2_RD(src)                        (((src) & 0x00000004)>>2)
#define APPPARITYERRS2_WR(src)                   (((u32)(src)<<2) & 0x00000004)
#define APPPARITYERRS2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields AppParityErrs1	 */
#define APPPARITYERRS1_WIDTH                                                  1
#define APPPARITYERRS1_SHIFT                                                  1
#define APPPARITYERRS1_MASK                                          0x00000002
#define APPPARITYERRS1_RD(src)                        (((src) & 0x00000002)>>1)
#define APPPARITYERRS1_WR(src)                   (((u32)(src)<<1) & 0x00000002)
#define APPPARITYERRS1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields AppParityErrs0	 */
#define APPPARITYERRS0_WIDTH                                                  1
#define APPPARITYERRS0_SHIFT                                                  0
#define APPPARITYERRS0_MASK                                          0x00000001
#define APPPARITYERRS0_RD(src)                           (((src) & 0x00000001))
#define APPPARITYERRS0_WR(src)                      (((u32)(src)) & 0x00000001)
#define APPPARITYERRS0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PCIEMISCCTRL	*/ 
/*	 Fields AppsSysEmlInterlockEngaged	 */
#define APPSSYSEMLINTERLOCKENGAGED_WIDTH                                      1
#define APPSSYSEMLINTERLOCKENGAGED_SHIFT                                      8
#define APPSSYSEMLINTERLOCKENGAGED_MASK                              0x00000100
#define APPSSYSEMLINTERLOCKENGAGED_RD(src)            (((src) & 0x00000100)>>8)
#define APPSSYSEMLINTERLOCKENGAGED_WR(src)       (((u32)(src)<<8) & 0x00000100)
#define APPSSYSEMLINTERLOCKENGAGED_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields AppsSysCmdCpledInt	 */
#define APPSSYSCMDCPLEDINT_WIDTH                                              1
#define APPSSYSCMDCPLEDINT_SHIFT                                              7
#define APPSSYSCMDCPLEDINT_MASK                                      0x00000080
#define APPSSYSCMDCPLEDINT_RD(src)                    (((src) & 0x00000080)>>7)
#define APPSSYSCMDCPLEDINT_WR(src)               (((u32)(src)<<7) & 0x00000080)
#define APPSSYSCMDCPLEDINT_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields AppsSysMrlSensorChged	 */
#define APPSSYSMRLSENSORCHGED_WIDTH                                           1
#define APPSSYSMRLSENSORCHGED_SHIFT                                           6
#define APPSSYSMRLSENSORCHGED_MASK                                   0x00000040
#define APPSSYSMRLSENSORCHGED_RD(src)                 (((src) & 0x00000040)>>6)
#define APPSSYSMRLSENSORCHGED_WR(src)            (((u32)(src)<<6) & 0x00000040)
#define APPSSYSMRLSENSORCHGED_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields AppsSysPwrFaultDet	 */
#define APPSSYSPWRFAULTDET_WIDTH                                              1
#define APPSSYSPWRFAULTDET_SHIFT                                              5
#define APPSSYSPWRFAULTDET_MASK                                      0x00000020
#define APPSSYSPWRFAULTDET_RD(src)                    (((src) & 0x00000020)>>5)
#define APPSSYSPWRFAULTDET_WR(src)               (((u32)(src)<<5) & 0x00000020)
#define APPSSYSPWRFAULTDET_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields AppsSysMrlSensorState	 */
#define APPSSYSMRLSENSORSTATE_WIDTH                                           1
#define APPSSYSMRLSENSORSTATE_SHIFT                                           4
#define APPSSYSMRLSENSORSTATE_MASK                                   0x00000010
#define APPSSYSMRLSENSORSTATE_RD(src)                 (((src) & 0x00000010)>>4)
#define APPSSYSMRLSENSORSTATE_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define APPSSYSMRLSENSORSTATE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AppsSysPreDetChged	 */
#define APPSSYSPREDETCHGED_WIDTH                                              1
#define APPSSYSPREDETCHGED_SHIFT                                              3
#define APPSSYSPREDETCHGED_MASK                                      0x00000008
#define APPSSYSPREDETCHGED_RD(src)                    (((src) & 0x00000008)>>3)
#define APPSSYSPREDETCHGED_WR(src)               (((u32)(src)<<3) & 0x00000008)
#define APPSSYSPREDETCHGED_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields AppsSysPredetState	 */
#define APPSSYSPREDETSTATE_WIDTH                                              1
#define APPSSYSPREDETSTATE_SHIFT                                              2
#define APPSSYSPREDETSTATE_MASK                                      0x00000004
#define APPSSYSPREDETSTATE_RD(src)                    (((src) & 0x00000004)>>2)
#define APPSSYSPREDETSTATE_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define APPSSYSPREDETSTATE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields AppsAttenButtonPressed	 */
#define APPSATTENBUTTONPRESSED_WIDTH                                          1
#define APPSATTENBUTTONPRESSED_SHIFT                                          1
#define APPSATTENBUTTONPRESSED_MASK                                  0x00000002
#define APPSATTENBUTTONPRESSED_RD(src)                (((src) & 0x00000002)>>1)
#define APPSATTENBUTTONPRESSED_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define APPSATTENBUTTONPRESSED_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields AppsPmXmtTurnoff	 */
#define APPSPMXMTTURNOFF_WIDTH                                                1
#define APPSPMXMTTURNOFF_SHIFT                                                0
#define APPSPMXMTTURNOFF_MASK                                        0x00000001
#define APPSPMXMTTURNOFF_RD(src)                         (((src) & 0x00000001))
#define APPSPMXMTTURNOFF_WR(src)                    (((u32)(src)) & 0x00000001)
#define APPSPMXMTTURNOFF_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PCIEDIAGNOSTIC	*/ 
/*	 Fields RadmPmPme	 */
#define RADMPMPME_WIDTH                                                       1
#define RADMPMPME_SHIFT                                                       1
#define RADMPMPME_MASK                                               0x00000002
#define RADMPMPME_RD(src)                             (((src) & 0x00000002)>>1)
#define RADMPMPME_WR(src)                        (((u32)(src)<<1) & 0x00000002)
#define RADMPMPME_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RadmPmToAck	 */
#define RADMPMTOACK_WIDTH                                                     1
#define RADMPMTOACK_SHIFT                                                     0
#define RADMPMTOACK_MASK                                             0x00000001
#define RADMPMTOACK_RD(src)                              (((src) & 0x00000001))
#define RADMPMTOACK_WR(src)                         (((u32)(src)) & 0x00000001)
#define RADMPMTOACK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PCIEAPPRST	*/ 
/*	 Fields AppinitRstSw	 */
#define APPINITRSTSW_WIDTH                                                    1
#define APPINITRSTSW_SHIFT                                                   10
#define APPINITRSTSW_MASK                                            0x00000400
#define APPINITRSTSW_RD(src)                         (((src) & 0x00000400)>>10)
#define APPINITRSTSW_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define APPINITRSTSW_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields AppReqRetryEn	 */
#define APPREQRETRYEN_WIDTH                                                   1
#define APPREQRETRYEN_SHIFT                                                   9
#define APPREQRETRYEN_MASK                                           0x00000200
#define APPREQRETRYEN_RD(src)                         (((src) & 0x00000200)>>9)
#define APPREQRETRYEN_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define APPREQRETRYEN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields AppinitRst	 */
#define APPINITRST_WIDTH                                                      1
#define APPINITRST_SHIFT                                                      8
#define APPINITRST_MASK                                              0x00000100
#define APPINITRST_RD(src)                            (((src) & 0x00000100)>>8)
#define APPINITRST_WR(src)                       (((u32)(src)<<8) & 0x00000100)
#define APPINITRST_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))

/*	Global Base Address	*/
#define PCIE_CSR_SYNOPSYS1_BASE_ADDR			0xddd880000ULL
#define PCIE_CSR_SYNOPSYS2_BASE_ADDR			0xddd890000ULL
#define PCIE_CSR_SYNOPSYS3_BASE_ADDR			0xddd8f0000ULL

/*    Address P1_PCIE_CSR_SYNOPSYS_PORT1  Registers */
#define PCIE_DEV_ID_ADDR                                             0x00000000
#define PCIE_DEV_ID_DEFAULT                                          0x911010e8
#define PCIE_CFG_CMD_ADDR                                            0x00000004
#define PCIE_CFG_CMD_DEFAULT                                         0x00100000
#define PCIE_CFG_CLASS_ADDR                                          0x00000008
#define PCIE_CFG_CLASS_DEFAULT                                       0x00000000
#define PCIE_CFG_HDRTYPE_ADDR                                        0x0000000c
#define PCIE_CFG_HDRTYPE_DEFAULT                                     0x00000000
#define PCIE_CFG_BAR0_ADDR                                           0x00000010
#define PCIE_CFG_BAR0_DEFAULT                                        0x00000004
#define PCIE_CFG_BAR1_ADDR                                           0x00000014
#define PCIE_CFG_BAR1_DEFAULT                                        0x00000000
#define PCIE_CFG_BAR2_ADDR                                           0x00000018
#define PCIE_CFG_BAR2_DEFAULT                                        0x00000001
#define PCIE_CFG_BAR3_ADDR                                           0x0000001c
#define PCIE_CFG_BAR3_DEFAULT                                        0x00000000
#define PCIE_CFG_BAR4_ADDR                                           0x00000020
#define PCIE_CFG_BAR4_DEFAULT                                        0x00000004
#define PCIE_CFG_BAR5_ADDR                                           0x00000024
#define PCIE_CFG_BAR5_DEFAULT                                        0x00000000
#define PCIE_CFG_BAR_ROM_ADDR                                        0x00000030
#define PCIE_CFG_BAR_ROM_DEFAULT                                     0x00000000
#define PCIE_CFG_BAR_ROM_RC_ADDR                                     0x00000038
#define PCIE_CFG_BAR_ROM_RC_DEFAULT                                  0x00000000
#define PCIE_CFG_INT_ADDR                                            0x0000003c
#define PCIE_CFG_INT_DEFAULT                                         0x000000ff
#define PCIE_PM_CAP_ADDR                                             0x00000040
#define PCIE_PM_CAP_DEFAULT                                          0x00035001
#define PCIE_PM_CTRL_STAT_ADDR                                       0x00000044
#define PCIE_PM_CTRL_STAT_DEFAULT                                    0x00000000
#define PCIE_MSI_CAP_ADDR                                            0x00000050
#define PCIE_MSI_CAP_DEFAULT                                         0x008a7005
#define PCIE_MSI_LOW_ADDR_ADDR                                       0x00000054
#define PCIE_MSI_LOW_ADDR_DEFAULT                                    0x00000000
#define PCIE_MSI_UPP_ADDR_ADDR                                       0x00000058
#define PCIE_MSI_UPP_ADDR_DEFAULT                                    0x00000000
#define PCIE_MSI_DATA_ADDR                                           0x0000005c
#define PCIE_MSI_DATA_DEFAULT                                        0x00000000
#define PCIE_CAP_ADDR                                                0x00000070
#define PCIE_CAP_DEFAULT                                             0x0002b010
#define PCIE_DEV_CTRL_ADDR                                           0x00000078
#define PCIE_DEV_CTRL_DEFAULT                                        0x00102810
#define PCIE_LINK_CAP_ADDR                                           0x0000007c
#define PCIE_LINK_CAP_DEFAULT                                        0x00404c42
#define PCIE_LINK_CTRL_ADDR                                          0x00000080
#define PCIE_LINK_CTRL_DEFAULT                                       0x10110000
#define PCIE_SLOT_CAP_ADDR                                           0x00000084
#define PCIE_SLOT_CAP_DEFAULT                                        0x00000000
#define PCIE_SLOT_CTRL_ADDR                                          0x00000088
#define PCIE_SLOT_CTRL_DEFAULT                                       0x00000000
#define PCIE_ROOT_CTRL_ADDR                                          0x0000008c
#define PCIE_ROOT_CTRL_DEFAULT                                       0x00000000
#define PCIE_DEV_CAP_2_ADDR                                          0x00000094
#define PCIE_DEV_CAP_2_DEFAULT                                       0x0000001f
#define PCIE_LINK_CTRL_2_ADDR                                        0x000000a0
#define PCIE_LINK_CTRL_2_DEFAULT                                     0x00010002
#define PCIE_MSIX_CAP_ADDR                                           0x000000b0
#define PCIE_MSIX_CAP_DEFAULT                                        0x007fd011
#define PCIE_MSIX_TABLE_ADDR                                         0x000000b4
#define PCIE_MSIX_TABLE_DEFAULT                                      0x00000000
#define PCIE_MSIX_PBA_ADDR                                           0x000000b8
#define PCIE_MSIX_PBA_DEFAULT                                        0x00000000
#define PCIE_VPD_CTRL_CAP_ADDR                                       0x000000d0
#define PCIE_VPD_CTRL_CAP_DEFAULT                                    0x00000003
#define PCIE_UNCORR_ERR_STAT_ADDR                                    0x00000104
#define PCIE_UNCORR_ERR_STAT_DEFAULT                                 0x00000000
#define PCIE_UNCORR_ERR_SVRTY_ADDR                                   0x0000010c
#define PCIE_UNCORR_ERR_SVRTY_DEFAULT                                0x00462030
#define PCIE_CORR_ERR_STAT_ADDR                                      0x00000110
#define PCIE_CORR_ERR_STAT_DEFAULT                                   0x00000000
#define PCIE_ADV_CAP_CTRL_ADDR                                       0x00000118
#define PCIE_ADV_CAP_CTRL_DEFAULT                                    0x000000a0
#define PCIE_ROOT_ERROR_CMD_ADDR                                     0x0000012c
#define PCIE_ROOT_ERROR_CMD_DEFAULT                                  0x00000000
#define PCIE_PORT_LINK_CTRL_ADDR                                     0x00000710
#define PCIE_PORT_LINK_CTRL_DEFAULT                                  0x00070120
#define MASTER_RESP_COMP_CTRL_REG0_ADDR                              0x00000818
#define MASTER_RESP_COMP_CTRL_REG0_DEFAULT                           0x00000005
#define MASTER_RESP_COMP_CTRL_REG1_ADDR                              0x0000081c
#define MASTER_RESP_COMP_CTRL_REG1_DEFAULT                           0x00000000
#define BAR0_MSK_L_ADDR                                              0x00001810
#define BAR0_MSK_L_DEFAULT                                           0x00000004
#define BAR0_MSK_H_ADDR                                              0x00001814
#define BAR0_MSK_H_DEFAULT                                           0x00000000
#define BAR3_MSK_ADDR                                                0x00001818
#define BAR3_MSK_DEFAULT                                             0x00000001
#define BAR2_MSK_L_ADDR                                              0x00001820
#define BAR2_MSK_L_DEFAULT                                           0x00000004
#define BAR2_MSK_H_ADDR                                              0x00001824
#define BAR2_MSK_H_DEFAULT                                           0x00000000
#define EXPROM_MSK_ADDR                                              0x00001830
#define EXPROM_MSK_DEFAULT                                           0x00000000
#define EXPROM_RC_MSK_ADDR                                           0x00001838
#define EXPROM_RC_MSK_DEFAULT                                        0x00000000

/*	Register pcie_dev_id	*/ 
/*	 Fields pcie_dev_id	 */
#define PCIE_DEV_ID_WIDTH                                                    32
#define PCIE_DEV_ID_SHIFT                                                     0
#define PCIE_DEV_ID_MASK                                             0xffffffff
#define PCIE_DEV_ID_RD(src)                              (((src) & 0xffffffff))
#define PCIE_DEV_ID_WR(src)                         (((u32)(src)) & 0xffffffff)
#define PCIE_DEV_ID_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_cmd	*/ 
/*	 Fields pcie_cfg_cmd	 */
#define PCIE_CFG_CMD_WIDTH                                                   32
#define PCIE_CFG_CMD_SHIFT                                                    0
#define PCIE_CFG_CMD_MASK                                            0xffffffff
#define PCIE_CFG_CMD_RD(src)                             (((src) & 0xffffffff))
#define PCIE_CFG_CMD_WR(src)                        (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_CMD_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_class	*/ 
/*	 Fields pcie_cfg_class	 */
#define PCIE_CFG_CLASS_WIDTH                                                 32
#define PCIE_CFG_CLASS_SHIFT                                                  0
#define PCIE_CFG_CLASS_MASK                                          0xffffffff
#define PCIE_CFG_CLASS_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_CLASS_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_CLASS_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_hdrtype	*/ 
/*	 Fields pcie_cfg_hdrtype	 */
#define PCIE_CFG_HDRTYPE_WIDTH                                               32
#define PCIE_CFG_HDRTYPE_SHIFT                                                0
#define PCIE_CFG_HDRTYPE_MASK                                        0xffffffff
#define PCIE_CFG_HDRTYPE_RD(src)                         (((src) & 0xffffffff))
#define PCIE_CFG_HDRTYPE_WR(src)                    (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_HDRTYPE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar0	*/ 
/*	 Fields pcie_cfg_bar0	 */
#define PCIE_CFG_BAR00_WIDTH                                                 32
#define PCIE_CFG_BAR00_SHIFT                                                  0
#define PCIE_CFG_BAR00_MASK                                          0xffffffff
#define PCIE_CFG_BAR00_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR00_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar1	*/ 
/*	 Fields pcie_cfg_bar1	 */
#define PCIE_CFG_BAR11_WIDTH                                                 32
#define PCIE_CFG_BAR11_SHIFT                                                  0
#define PCIE_CFG_BAR11_MASK                                          0xffffffff
#define PCIE_CFG_BAR11_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR11_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar2	*/ 
/*	 Fields pcie_cfg_bar2	 */
#define PCIE_CFG_BAR22_WIDTH                                                 32
#define PCIE_CFG_BAR22_SHIFT                                                  0
#define PCIE_CFG_BAR22_MASK                                          0xffffffff
#define PCIE_CFG_BAR22_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR22_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar3	*/ 
/*	 Fields pcie_cfg_bar3	 */
#define PCIE_CFG_BAR33_WIDTH                                                 32
#define PCIE_CFG_BAR33_SHIFT                                                  0
#define PCIE_CFG_BAR33_MASK                                          0xffffffff
#define PCIE_CFG_BAR33_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR33_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar4	*/ 
/*	 Fields pcie_cfg_bar4	 */
#define PCIE_CFG_BAR44_WIDTH                                                 32
#define PCIE_CFG_BAR44_SHIFT                                                  0
#define PCIE_CFG_BAR44_MASK                                          0xffffffff
#define PCIE_CFG_BAR44_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR44_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar5	*/ 
/*	 Fields pcie_cfg_bar5	 */
#define PCIE_CFG_BAR55_WIDTH                                                 32
#define PCIE_CFG_BAR55_SHIFT                                                  0
#define PCIE_CFG_BAR55_MASK                                          0xffffffff
#define PCIE_CFG_BAR55_RD(src)                           (((src) & 0xffffffff))
#define PCIE_CFG_BAR55_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar_rom	*/ 
/*	 Fields pcie_cfg_bar_rom	 */
#define PCIE_CFG_BAR_ROM_WIDTH                                               32
#define PCIE_CFG_BAR_ROM_SHIFT                                                0
#define PCIE_CFG_BAR_ROM_MASK                                        0xffffffff
#define PCIE_CFG_BAR_ROM_RD(src)                         (((src) & 0xffffffff))
#define PCIE_CFG_BAR_ROM_WR(src)                    (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR_ROM_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_bar_rom_rc	*/ 
/*	 Fields pcie_cfg_bar_rom_rc	 */
#define PCIE_CFG_BAR_ROM_RC_WIDTH                                            32
#define PCIE_CFG_BAR_ROM_RC_SHIFT                                             0
#define PCIE_CFG_BAR_ROM_RC_MASK                                     0xffffffff
#define PCIE_CFG_BAR_ROM_RC_RD(src)                      (((src) & 0xffffffff))
#define PCIE_CFG_BAR_ROM_RC_WR(src)                 (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_BAR_ROM_RC_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cfg_int	*/ 
/*	 Fields pcie_cfg_int	 */
#define PCIE_CFG_INT_WIDTH                                                   32
#define PCIE_CFG_INT_SHIFT                                                    0
#define PCIE_CFG_INT_MASK                                            0xffffffff
#define PCIE_CFG_INT_RD(src)                             (((src) & 0xffffffff))
#define PCIE_CFG_INT_WR(src)                        (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_INT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_pm_cap	*/ 
/*	 Fields pcie_pm_cap	 */
#define PCIE_PM_CAP_WIDTH                                                    32
#define PCIE_PM_CAP_SHIFT                                                     0
#define PCIE_PM_CAP_MASK                                             0xffffffff
#define PCIE_PM_CAP_RD(src)                              (((src) & 0xffffffff))
#define PCIE_PM_CAP_WR(src)                         (((u32)(src)) & 0xffffffff)
#define PCIE_PM_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_pm_ctrl_stat	*/ 
/*	 Fields pcie_pm_ctrl_stat	 */
#define PCIE_PM_CTRL_STAT_WIDTH                                              32
#define PCIE_PM_CTRL_STAT_SHIFT                                               0
#define PCIE_PM_CTRL_STAT_MASK                                       0xffffffff
#define PCIE_PM_CTRL_STAT_RD(src)                        (((src) & 0xffffffff))
#define PCIE_PM_CTRL_STAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_PM_CTRL_STAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msi_cap	*/ 
/*	 Fields pcie_msi_cap	 */
#define PCIE_MSI_CAP_WIDTH                                                   32
#define PCIE_MSI_CAP_SHIFT                                                    0
#define PCIE_MSI_CAP_MASK                                            0xffffffff
#define PCIE_MSI_CAP_RD(src)                             (((src) & 0xffffffff))
#define PCIE_MSI_CAP_WR(src)                        (((u32)(src)) & 0xffffffff)
#define PCIE_MSI_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msi_low_addr	*/ 
/*	 Fields pcie_msi_low_addr	 */
#define PCIE_MSI_LOW_ADDR_WIDTH                                              32
#define PCIE_MSI_LOW_ADDR_SHIFT                                               0
#define PCIE_MSI_LOW_ADDR_MASK                                       0xffffffff
#define PCIE_MSI_LOW_ADDR_RD(src)                        (((src) & 0xffffffff))
#define PCIE_MSI_LOW_ADDR_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_MSI_LOW_ADDR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msi_upp_addr	*/ 
/*	 Fields pcie_msi_upp_addr	 */
#define PCIE_MSI_UPP_ADDR_WIDTH                                              32
#define PCIE_MSI_UPP_ADDR_SHIFT                                               0
#define PCIE_MSI_UPP_ADDR_MASK                                       0xffffffff
#define PCIE_MSI_UPP_ADDR_RD(src)                        (((src) & 0xffffffff))
#define PCIE_MSI_UPP_ADDR_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_MSI_UPP_ADDR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msi_data	*/ 
/*	 Fields pcie_msi_data	 */
#define PCIE_MSI_DATA_WIDTH                                                  32
#define PCIE_MSI_DATA_SHIFT                                                   0
#define PCIE_MSI_DATA_MASK                                           0xffffffff
#define PCIE_MSI_DATA_RD(src)                            (((src) & 0xffffffff))
#define PCIE_MSI_DATA_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_MSI_DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_cap	*/ 
/*	 Fields pcie_cap	 */
#define PCIE_CAP_WIDTH                                                       32
#define PCIE_CAP_SHIFT                                                        0
#define PCIE_CAP_MASK                                                0xffffffff
#define PCIE_CAP_RD(src)                                 (((src) & 0xffffffff))
#define PCIE_CAP_WR(src)                            (((u32)(src)) & 0xffffffff)
#define PCIE_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_dev_ctrl	*/ 
/*	 Fields pcie_dev_ctrl	 */
#define PCIE_DEV_CTRL_WIDTH                                                  32
#define PCIE_DEV_CTRL_SHIFT                                                   0
#define PCIE_DEV_CTRL_MASK                                           0xffffffff
#define PCIE_DEV_CTRL_RD(src)                            (((src) & 0xffffffff))
#define PCIE_DEV_CTRL_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_DEV_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_link_cap	*/ 
/*	 Fields pcie_link_cap	 */
#define PCIE_LINK_CAP_WIDTH                                                  32
#define PCIE_LINK_CAP_SHIFT                                                   0
#define PCIE_LINK_CAP_MASK                                           0xffffffff
#define PCIE_LINK_CAP_RD(src)                            (((src) & 0xffffffff))
#define PCIE_LINK_CAP_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_LINK_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_link_ctrl	*/ 
/*	 Fields pcie_link_ctrl	 */
#define PCIE_LINK_CTRL_WIDTH                                                 32
#define PCIE_LINK_CTRL_SHIFT                                                  0
#define PCIE_LINK_CTRL_MASK                                          0xffffffff
#define PCIE_LINK_CTRL_RD(src)                           (((src) & 0xffffffff))
#define PCIE_LINK_CTRL_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_LINK_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_slot_cap	*/ 
/*	 Fields pcie_slot_cap	 */
#define PCIE_SLOT_CAP_WIDTH                                                  32
#define PCIE_SLOT_CAP_SHIFT                                                   0
#define PCIE_SLOT_CAP_MASK                                           0xffffffff
#define PCIE_SLOT_CAP_RD(src)                            (((src) & 0xffffffff))
#define PCIE_SLOT_CAP_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_SLOT_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_slot_ctrl	*/ 
/*	 Fields pcie_slot_ctrl	 */
#define PCIE_SLOT_CTRL_WIDTH                                                 32
#define PCIE_SLOT_CTRL_SHIFT                                                  0
#define PCIE_SLOT_CTRL_MASK                                          0xffffffff
#define PCIE_SLOT_CTRL_RD(src)                           (((src) & 0xffffffff))
#define PCIE_SLOT_CTRL_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_SLOT_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_root_ctrl	*/ 
/*	 Fields pcie_root_ctrl	 */
#define PCIE_ROOT_CTRL_WIDTH                                                 32
#define PCIE_ROOT_CTRL_SHIFT                                                  0
#define PCIE_ROOT_CTRL_MASK                                          0xffffffff
#define PCIE_ROOT_CTRL_RD(src)                           (((src) & 0xffffffff))
#define PCIE_ROOT_CTRL_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_ROOT_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_dev_cap_2	*/ 
/*	 Fields pcie_dev_cap_2	 */
#define PCIE_DEV_CAP_22_WIDTH                                                32
#define PCIE_DEV_CAP_22_SHIFT                                                 0
#define PCIE_DEV_CAP_22_MASK                                         0xffffffff
#define PCIE_DEV_CAP_22_RD(src)                          (((src) & 0xffffffff))
#define PCIE_DEV_CAP_22_WR(src)                     (((u32)(src)) & 0xffffffff)
#define PCIE_DEV_CAP_22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_link_ctrl_2	*/ 
/*	 Fields pcie_link_ctrl_2	 */
#define PCIE_LINK_CTRL_22_WIDTH                                              32
#define PCIE_LINK_CTRL_22_SHIFT                                               0
#define PCIE_LINK_CTRL_22_MASK                                       0xffffffff
#define PCIE_LINK_CTRL_22_RD(src)                        (((src) & 0xffffffff))
#define PCIE_LINK_CTRL_22_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_LINK_CTRL_22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msix_cap	*/ 
/*	 Fields pcie_msix_cap	 */
#define PCIE_MSIX_CAP_WIDTH                                                  32
#define PCIE_MSIX_CAP_SHIFT                                                   0
#define PCIE_MSIX_CAP_MASK                                           0xffffffff
#define PCIE_MSIX_CAP_RD(src)                            (((src) & 0xffffffff))
#define PCIE_MSIX_CAP_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_MSIX_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msix_table	*/ 
/*	 Fields pcie_msix_table	 */
#define PCIE_MSIX_TABLE_WIDTH                                                32
#define PCIE_MSIX_TABLE_SHIFT                                                 0
#define PCIE_MSIX_TABLE_MASK                                         0xffffffff
#define PCIE_MSIX_TABLE_RD(src)                          (((src) & 0xffffffff))
#define PCIE_MSIX_TABLE_WR(src)                     (((u32)(src)) & 0xffffffff)
#define PCIE_MSIX_TABLE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_msix_pba	*/ 
/*	 Fields pcie_msix_pba	 */
#define PCIE_MSIX_PBA_WIDTH                                                  32
#define PCIE_MSIX_PBA_SHIFT                                                   0
#define PCIE_MSIX_PBA_MASK                                           0xffffffff
#define PCIE_MSIX_PBA_RD(src)                            (((src) & 0xffffffff))
#define PCIE_MSIX_PBA_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_MSIX_PBA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_vpd_ctrl_cap	*/ 
/*	 Fields pcie_vpd_ctrl_cap	 */
#define PCIE_VPD_CTRL_CAP_WIDTH                                              32
#define PCIE_VPD_CTRL_CAP_SHIFT                                               0
#define PCIE_VPD_CTRL_CAP_MASK                                       0xffffffff
#define PCIE_VPD_CTRL_CAP_RD(src)                        (((src) & 0xffffffff))
#define PCIE_VPD_CTRL_CAP_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_VPD_CTRL_CAP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_uncorr_err_stat	*/ 
/*	 Fields pcie_uncorr_err_stat	 */
#define PCIE_UNCORR_ERR_STAT_WIDTH                                           32
#define PCIE_UNCORR_ERR_STAT_SHIFT                                            0
#define PCIE_UNCORR_ERR_STAT_MASK                                    0xffffffff
#define PCIE_UNCORR_ERR_STAT_RD(src)                     (((src) & 0xffffffff))
#define PCIE_UNCORR_ERR_STAT_WR(src)                (((u32)(src)) & 0xffffffff)
#define PCIE_UNCORR_ERR_STAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_uncorr_err_svrty	*/ 
/*	 Fields pcie_uncorr_err_svrty	 */
#define PCIE_UNCORR_ERR_SVRTY_WIDTH                                          32
#define PCIE_UNCORR_ERR_SVRTY_SHIFT                                           0
#define PCIE_UNCORR_ERR_SVRTY_MASK                                   0xffffffff
#define PCIE_UNCORR_ERR_SVRTY_RD(src)                    (((src) & 0xffffffff))
#define PCIE_UNCORR_ERR_SVRTY_WR(src)               (((u32)(src)) & 0xffffffff)
#define PCIE_UNCORR_ERR_SVRTY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_corr_err_stat	*/ 
/*	 Fields pcie_corr_err_stat	 */
#define PCIE_CORR_ERR_STAT_WIDTH                                             32
#define PCIE_CORR_ERR_STAT_SHIFT                                              0
#define PCIE_CORR_ERR_STAT_MASK                                      0xffffffff
#define PCIE_CORR_ERR_STAT_RD(src)                       (((src) & 0xffffffff))
#define PCIE_CORR_ERR_STAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define PCIE_CORR_ERR_STAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_adv_cap_ctrl	*/ 
/*	 Fields pcie_adv_cap_ctrl	 */
#define PCIE_ADV_CAP_CTRL_WIDTH                                              32
#define PCIE_ADV_CAP_CTRL_SHIFT                                               0
#define PCIE_ADV_CAP_CTRL_MASK                                       0xffffffff
#define PCIE_ADV_CAP_CTRL_RD(src)                        (((src) & 0xffffffff))
#define PCIE_ADV_CAP_CTRL_WR(src)                   (((u32)(src)) & 0xffffffff)
#define PCIE_ADV_CAP_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_root_error_cmd	*/ 
/*	 Fields pcie_root_error_cmd	 */
#define PCIE_ROOT_ERROR_CMD_WIDTH                                            32
#define PCIE_ROOT_ERROR_CMD_SHIFT                                             0
#define PCIE_ROOT_ERROR_CMD_MASK                                     0xffffffff
#define PCIE_ROOT_ERROR_CMD_RD(src)                      (((src) & 0xffffffff))
#define PCIE_ROOT_ERROR_CMD_WR(src)                 (((u32)(src)) & 0xffffffff)
#define PCIE_ROOT_ERROR_CMD_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register pcie_port_link_ctrl	*/ 
/*	 Fields pcie_port_link_ctrl	 */
#define PCIE_PORT_LINK_CTRL_WIDTH                                            32
#define PCIE_PORT_LINK_CTRL_SHIFT                                             0
#define PCIE_PORT_LINK_CTRL_MASK                                     0xffffffff
#define PCIE_PORT_LINK_CTRL_RD(src)                      (((src) & 0xffffffff))
#define PCIE_PORT_LINK_CTRL_WR(src)                 (((u32)(src)) & 0xffffffff)
#define PCIE_PORT_LINK_CTRL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register master_resp_comp_ctrl_reg0	*/ 
/*	 Fields master_resp_comp_ctrl_reg0	 */
#define MASTER_RESP_COMP_CTRL_REG00_WIDTH                                    32
#define MASTER_RESP_COMP_CTRL_REG00_SHIFT                                     0
#define MASTER_RESP_COMP_CTRL_REG00_MASK                             0xffffffff
#define MASTER_RESP_COMP_CTRL_REG00_RD(src)              (((src) & 0xffffffff))
#define MASTER_RESP_COMP_CTRL_REG00_WR(src)         (((u32)(src)) & 0xffffffff)
#define MASTER_RESP_COMP_CTRL_REG00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register master_resp_comp_ctrl_reg1	*/ 
/*	 Fields master_resp_comp_ctrl_reg1	 */
#define MASTER_RESP_COMP_CTRL_REG11_WIDTH                                    32
#define MASTER_RESP_COMP_CTRL_REG11_SHIFT                                     0
#define MASTER_RESP_COMP_CTRL_REG11_MASK                             0xffffffff
#define MASTER_RESP_COMP_CTRL_REG11_RD(src)              (((src) & 0xffffffff))
#define MASTER_RESP_COMP_CTRL_REG11_WR(src)         (((u32)(src)) & 0xffffffff)
#define MASTER_RESP_COMP_CTRL_REG11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register bar0_msk_l	*/ 
/*	 Fields bar0_msk_l	 */
#define BAR0_MSK_L_WIDTH                                                     32
#define BAR0_MSK_L_SHIFT                                                      0
#define BAR0_MSK_L_MASK                                              0xffffffff
#define BAR0_MSK_L_RD(src)                               (((src) & 0xffffffff))
#define BAR0_MSK_L_WR(src)                          (((u32)(src)) & 0xffffffff)
#define BAR0_MSK_L_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register bar0_msk_h	*/ 
/*	 Fields bar0_msk_h	 */
#define BAR0_MSK_H_WIDTH                                                     32
#define BAR0_MSK_H_SHIFT                                                      0
#define BAR0_MSK_H_MASK                                              0xffffffff
#define BAR0_MSK_H_RD(src)                               (((src) & 0xffffffff))
#define BAR0_MSK_H_WR(src)                          (((u32)(src)) & 0xffffffff)
#define BAR0_MSK_H_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register bar3_msk	*/ 
/*	 Fields bar3_msk	 */
#define BAR3_MSK_WIDTH                                                       32
#define BAR3_MSK_SHIFT                                                        0
#define BAR3_MSK_MASK                                                0xffffffff
#define BAR3_MSK_RD(src)                                 (((src) & 0xffffffff))
#define BAR3_MSK_WR(src)                            (((u32)(src)) & 0xffffffff)
#define BAR3_MSK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register bar2_msk_l	*/ 
/*	 Fields bar2_msk_l	 */
#define BAR2_MSK_L_WIDTH                                                     32
#define BAR2_MSK_L_SHIFT                                                      0
#define BAR2_MSK_L_MASK                                              0xffffffff
#define BAR2_MSK_L_RD(src)                               (((src) & 0xffffffff))
#define BAR2_MSK_L_WR(src)                          (((u32)(src)) & 0xffffffff)
#define BAR2_MSK_L_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register bar2_msk_h	*/ 
/*	 Fields bar2_msk_h	 */
#define BAR2_MSK_H_WIDTH                                                     32
#define BAR2_MSK_H_SHIFT                                                      0
#define BAR2_MSK_H_MASK                                              0xffffffff
#define BAR2_MSK_H_RD(src)                               (((src) & 0xffffffff))
#define BAR2_MSK_H_WR(src)                          (((u32)(src)) & 0xffffffff)
#define BAR2_MSK_H_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register exprom_msk	*/ 
/*	 Fields exprom_msk	 */
#define EXPROM_MSK_WIDTH                                                     32
#define EXPROM_MSK_SHIFT                                                      0
#define EXPROM_MSK_MASK                                              0xffffffff
#define EXPROM_MSK_RD(src)                               (((src) & 0xffffffff))
#define EXPROM_MSK_WR(src)                          (((u32)(src)) & 0xffffffff)
#define EXPROM_MSK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register exprom_rc_msk	*/ 
/*	 Fields exprom_rc_msk	 */
#define EXPROM_RC_MSK_WIDTH                                                  32
#define EXPROM_RC_MSK_SHIFT                                                   0
#define EXPROM_RC_MSK_MASK                                           0xffffffff
#define EXPROM_RC_MSK_RD(src)                            (((src) & 0xffffffff))
#define EXPROM_RC_MSK_WR(src)                       (((u32)(src)) & 0xffffffff)
#define EXPROM_RC_MSK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Global Base Address	*/
#define PCIE_GLBL_DIAG_CSR2_BASE_ADDR				0xddd8f7000ULL
#define PCIE_GLBL_DIAG_CSR1_BASE_ADDR				0xddd897000ULL
#define PCIE_GLBL_DIAG_CSR0_BASE_ADDR				0xddd887000ULL

/*    Address P1_GLBL_DIAG_CSR_PORT1  Registers */
#define PCIE_CFG_DIAG_SEL_ADDR                                       0x00000000
#define PCIE_CFG_DIAG_SEL_DEFAULT                                    0x00000000
#define PCIE_CFG_READ_BW_LAT_ADDR_MASK_ADDR                          0x00000004
#define PCIE_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define PCIE_CFG_READ_BW_LAT_ADDR_PAT_ADDR                           0x00000008
#define PCIE_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define PCIE_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                         0x0000000c
#define PCIE_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                      0x00000000
#define PCIE_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                          0x00000010
#define PCIE_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                       0xffffffff
#define PCIE_CFG_DIAG_START_STOP_ADDR                                0x00000014
#define PCIE_CFG_DIAG_START_STOP_DEFAULT                             0x0000001f
#define PCIE_CFG_BW_MSTR_STOP_CNT_ADDR                               0x00000018
#define PCIE_CFG_BW_MSTR_STOP_CNT_DEFAULT                            0x00040004
#define PCIE_CFG_BW_SLV_STOP_CNT_ADDR                                0x0000001c
#define PCIE_CFG_BW_SLV_STOP_CNT_DEFAULT                             0x00040004
#define PCIE_STS_READ_LATENCY_OUTPUT_ADDR                            0x00000020
#define PCIE_STS_READ_LATENCY_OUTPUT_DEFAULT                         0x00000000
#define PCIE_STS_AXI_MRD_BW_CLK_CNT_ADDR                             0x00000024
#define PCIE_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define PCIE_STS_AXI_MRD_BW_BYTE_CNT_ADDR                            0x00000028
#define PCIE_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define PCIE_STS_AXI_MWR_BW_CLK_CNT_ADDR                             0x0000002c
#define PCIE_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define PCIE_STS_AXI_MWR_BW_BYTE_CNT_ADDR                            0x00000030
#define PCIE_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define PCIE_STS_AXI_SRD_BW_CLK_CNT_ADDR                             0x00000034
#define PCIE_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define PCIE_STS_AXI_SRD_BW_BYTE_CNT_ADDR                            0x00000038
#define PCIE_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define PCIE_STS_AXI_SWR_BW_CLK_CNT_ADDR                             0x0000003c
#define PCIE_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define PCIE_STS_AXI_SWR_BW_BYTE_CNT_ADDR                            0x00000040
#define PCIE_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define PCIE_CFG_DBG_TRIG_CTRL_ADDR                                  0x00000044
#define PCIE_CFG_DBG_TRIG_CTRL_DEFAULT                               0x00000000
#define PCIE_CFG_DBG_PAT_REG_0_ADDR                                  0x00000048
#define PCIE_CFG_DBG_PAT_REG_0_DEFAULT                               0x00000000
#define PCIE_CFG_DBG_PAT_MASK_REG_0_ADDR                             0x0000004c
#define PCIE_CFG_DBG_PAT_MASK_REG_0_DEFAULT                          0x00000000
#define PCIE_CFG_DBG_PAT_REG_1_ADDR                                  0x00000050
#define PCIE_CFG_DBG_PAT_REG_1_DEFAULT                               0x00000000
#define PCIE_CFG_DBG_PAT_MASK_REG_1_ADDR                             0x00000054
#define PCIE_CFG_DBG_PAT_MASK_REG_1_DEFAULT                          0x00000000
#define PCIE_DBG_TRIG_OUT_ADDR                                       0x00000058
#define PCIE_DBG_TRIG_OUT_DEFAULT                                    0x00000000
#define PCIE_DBG_ECC_INT_OUT_ADDR                                    0x0000005c
#define PCIE_DBG_ECC_INT_OUT_DEFAULT                                 0x00000000
#define PCIE_DBG_ECC_INT_OUTMASK_ADDR                                0x00000060
#define PCIE_CFG_SECC_INT_THRSH_REG_ADDR                             0x00000064
#define PCIE_CFG_SECC_INT_THRSH_REG_DEFAULT                          0x000000fe
#define PCIE_CFG_MEM_ECC_BYPASS_ADDR                                 0x00000068
#define PCIE_CFG_MEM_ECC_BYPASS_DEFAULT                              0x00000000
#define PCIE_CFG_MEM_PWRDN_DIS_ADDR                                  0x0000006c
#define PCIE_CFG_MEM_PWRDN_DIS_DEFAULT                               0x00000000
#define PCIE_CFG_MEM_RAM_SHUTDOWN_ADDR                               0x00000070
#define PCIE_CFG_MEM_RAM_SHUTDOWN_DEFAULT                            0xffffffff
#define PCIE_BLOCK_MEM_RDY_ADDR                                      0x00000074
#define PCIE_BLOCK_MEM_RDY_DEFAULT                                   0xffffffff
#define PCIE_CFG_AXI_SLV_RD_TMO_ADDR                                 0x00000078
#define PCIE_CFG_AXI_SLV_RD_TMO_DEFAULT                              0x83ff83ff
#define PCIE_CFG_AXI_SLV_WR_TMO_ADDR                                 0x0000007c
#define PCIE_CFG_AXI_SLV_WR_TMO_DEFAULT                              0x83ff83ff
#define PCIE_STS_AXI_SLV_TMO_ADDR                                    0x00000080
#define PCIE_STS_AXI_SLV_TMO_DEFAULT                                 0x00000000
#define PCIE_CFG_AXI_SS_CSR_TMO_ADDR                                 0x00000084
#define PCIE_CFG_AXI_SS_CSR_TMO_DEFAULT                              0x02008000
#define PCIE_STS_READ_LATENCY_TOT_READ_REQS_ADDR                     0x0000008c
#define PCIE_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                  0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                 1
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                11
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_MASK                         0x00000800
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)      (((src) & 0x00000800)>>11)
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define PCIE_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define PCIE_CFG_MUX_SELECTOR_WIDTH                                          11
#define PCIE_CFG_MUX_SELECTOR_SHIFT                                           0
#define PCIE_CFG_MUX_SELECTOR_MASK                                   0x000007ff
#define PCIE_CFG_MUX_SELECTOR_RD(src)                    (((src) & 0x000007ff))
#define PCIE_CFG_MUX_SELECTOR_WR(src)               (((u32)(src)) & 0x000007ff)
#define PCIE_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define PCIE_READ_ADDR_MASK_WIDTH                                            32
#define PCIE_READ_ADDR_MASK_SHIFT                                             0
#define PCIE_READ_ADDR_MASK_MASK                                     0xffffffff
#define PCIE_READ_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define PCIE_READ_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define PCIE_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define PCIE_READ_ADDR_PAT_WIDTH                                             32
#define PCIE_READ_ADDR_PAT_SHIFT                                              0
#define PCIE_READ_ADDR_PAT_MASK                                      0xffffffff
#define PCIE_READ_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define PCIE_READ_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define PCIE_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define PCIE_WRITE_ADDR_MASK_WIDTH                                           32
#define PCIE_WRITE_ADDR_MASK_SHIFT                                            0
#define PCIE_WRITE_ADDR_MASK_MASK                                    0xffffffff
#define PCIE_WRITE_ADDR_MASK_RD(src)                     (((src) & 0xffffffff))
#define PCIE_WRITE_ADDR_MASK_WR(src)                (((u32)(src)) & 0xffffffff)
#define PCIE_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define PCIE_WRITE_ADDR_PAT_WIDTH                                            32
#define PCIE_WRITE_ADDR_PAT_SHIFT                                             0
#define PCIE_WRITE_ADDR_PAT_MASK                                     0xffffffff
#define PCIE_WRITE_ADDR_PAT_RD(src)                      (((src) & 0xffffffff))
#define PCIE_WRITE_ADDR_PAT_WR(src)                 (((u32)(src)) & 0xffffffff)
#define PCIE_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define PCIE_START_RD_LATENCY_WIDTH                                           1
#define PCIE_START_RD_LATENCY_SHIFT                                           4
#define PCIE_START_RD_LATENCY_MASK                                   0x00000010
#define PCIE_START_RD_LATENCY_RD(src)                 (((src) & 0x00000010)>>4)
#define PCIE_START_RD_LATENCY_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define PCIE_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define PCIE_START_SRD_BW_WIDTH                                               1
#define PCIE_START_SRD_BW_SHIFT                                               3
#define PCIE_START_SRD_BW_MASK                                       0x00000008
#define PCIE_START_SRD_BW_RD(src)                     (((src) & 0x00000008)>>3)
#define PCIE_START_SRD_BW_WR(src)                (((u32)(src)<<3) & 0x00000008)
#define PCIE_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define PCIE_START_MRD_BW_WIDTH                                               1
#define PCIE_START_MRD_BW_SHIFT                                               2
#define PCIE_START_MRD_BW_MASK                                       0x00000004
#define PCIE_START_MRD_BW_RD(src)                     (((src) & 0x00000004)>>2)
#define PCIE_START_MRD_BW_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define PCIE_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define PCIE_START_SWR_BW_WIDTH                                               1
#define PCIE_START_SWR_BW_SHIFT                                               1
#define PCIE_START_SWR_BW_MASK                                       0x00000002
#define PCIE_START_SWR_BW_RD(src)                     (((src) & 0x00000002)>>1)
#define PCIE_START_SWR_BW_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define PCIE_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define PCIE_START_MWR_BW_WIDTH                                               1
#define PCIE_START_MWR_BW_SHIFT                                               0
#define PCIE_START_MWR_BW_MASK                                       0x00000001
#define PCIE_START_MWR_BW_RD(src)                        (((src) & 0x00000001))
#define PCIE_START_MWR_BW_WR(src)                   (((u32)(src)) & 0x00000001)
#define PCIE_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define PCIE_MSTR_STOP_RD_CNT_WIDTH                                          16
#define PCIE_MSTR_STOP_RD_CNT_SHIFT                                          16
#define PCIE_MSTR_STOP_RD_CNT_MASK                                   0xffff0000
#define PCIE_MSTR_STOP_RD_CNT_RD(src)                (((src) & 0xffff0000)>>16)
#define PCIE_MSTR_STOP_RD_CNT_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define PCIE_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define PCIE_MSTR_STOP_WR_CNT_WIDTH                                          16
#define PCIE_MSTR_STOP_WR_CNT_SHIFT                                           0
#define PCIE_MSTR_STOP_WR_CNT_MASK                                   0x0000ffff
#define PCIE_MSTR_STOP_WR_CNT_RD(src)                    (((src) & 0x0000ffff))
#define PCIE_MSTR_STOP_WR_CNT_WR(src)               (((u32)(src)) & 0x0000ffff)
#define PCIE_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define PCIE_SLV_STOP_RD_CNT_WIDTH                                           16
#define PCIE_SLV_STOP_RD_CNT_SHIFT                                           16
#define PCIE_SLV_STOP_RD_CNT_MASK                                    0xffff0000
#define PCIE_SLV_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define PCIE_SLV_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define PCIE_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define PCIE_SLV_STOP_WR_CNT_WIDTH                                           16
#define PCIE_SLV_STOP_WR_CNT_SHIFT                                            0
#define PCIE_SLV_STOP_WR_CNT_MASK                                    0x0000ffff
#define PCIE_SLV_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define PCIE_SLV_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define PCIE_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define PCIE_READ_MAX_WIDTH                                                  10
#define PCIE_READ_MAX_SHIFT                                                  22
#define PCIE_READ_MAX_MASK                                           0xffc00000
#define PCIE_READ_MAX_RD(src)                        (((src) & 0xffc00000)>>22)
#define PCIE_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define PCIE_READ_TOT_WIDTH                                                  22
#define PCIE_READ_TOT_SHIFT                                                   0
#define PCIE_READ_TOT_MASK                                           0x003fffff
#define PCIE_READ_TOT_RD(src)                            (((src) & 0x003fffff))
#define PCIE_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define PCIE_MSTR_READ_BW_CLK_CNT_WIDTH                                      32
#define PCIE_MSTR_READ_BW_CLK_CNT_SHIFT                                       0
#define PCIE_MSTR_READ_BW_CLK_CNT_MASK                               0xffffffff
#define PCIE_MSTR_READ_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define PCIE_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define PCIE_MSTR_READ_BW_BYTE_CNT_WIDTH                                     32
#define PCIE_MSTR_READ_BW_BYTE_CNT_SHIFT                                      0
#define PCIE_MSTR_READ_BW_BYTE_CNT_MASK                              0xffffffff
#define PCIE_MSTR_READ_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define PCIE_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define PCIE_MSTR_WRITE_BW_CLK_CNT_WIDTH                                     32
#define PCIE_MSTR_WRITE_BW_CLK_CNT_SHIFT                                      0
#define PCIE_MSTR_WRITE_BW_CLK_CNT_MASK                              0xffffffff
#define PCIE_MSTR_WRITE_BW_CLK_CNT_RD(src)               (((src) & 0xffffffff))
#define PCIE_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define PCIE_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                    32
#define PCIE_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                     0
#define PCIE_MSTR_WRITE_BW_BYTE_CNT_MASK                             0xffffffff
#define PCIE_MSTR_WRITE_BW_BYTE_CNT_RD(src)              (((src) & 0xffffffff))
#define PCIE_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define PCIE_SLV_READ_BW_CLK_CNT_WIDTH                                       32
#define PCIE_SLV_READ_BW_CLK_CNT_SHIFT                                        0
#define PCIE_SLV_READ_BW_CLK_CNT_MASK                                0xffffffff
#define PCIE_SLV_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define PCIE_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define PCIE_SLV_READ_BW_BYTE_CNT_WIDTH                                      32
#define PCIE_SLV_READ_BW_BYTE_CNT_SHIFT                                       0
#define PCIE_SLV_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define PCIE_SLV_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define PCIE_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define PCIE_SLV_WRITE_BW_CLK_CNT_WIDTH                                      32
#define PCIE_SLV_WRITE_BW_CLK_CNT_SHIFT                                       0
#define PCIE_SLV_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define PCIE_SLV_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define PCIE_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define PCIE_SLV_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define PCIE_SLV_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define PCIE_SLV_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define PCIE_SLV_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define PCIE_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define PCIE_TRIG_EN_WIDTH                                                    1
#define PCIE_TRIG_EN_SHIFT                                                    4
#define PCIE_TRIG_EN_MASK                                            0x00000010
#define PCIE_TRIG_EN_RD(src)                          (((src) & 0x00000010)>>4)
#define PCIE_TRIG_EN_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define PCIE_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define PCIE_AND_E_WIDTH                                                      2
#define PCIE_AND_E_SHIFT                                                      2
#define PCIE_AND_E_MASK                                              0x0000000c
#define PCIE_AND_E_RD(src)                            (((src) & 0x0000000c)>>2)
#define PCIE_AND_E_WR(src)                       (((u32)(src)<<2) & 0x0000000c)
#define PCIE_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define PCIE_OR_E_WIDTH                                                       2
#define PCIE_OR_E_SHIFT                                                       0
#define PCIE_OR_E_MASK                                               0x00000003
#define PCIE_OR_E_RD(src)                                (((src) & 0x00000003))
#define PCIE_OR_E_WR(src)                           (((u32)(src)) & 0x00000003)
#define PCIE_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define PCIE_PATTERN0_WIDTH                                                  32
#define PCIE_PATTERN0_SHIFT                                                   0
#define PCIE_PATTERN0_MASK                                           0xffffffff
#define PCIE_PATTERN0_RD(src)                            (((src) & 0xffffffff))
#define PCIE_PATTERN0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define PCIE_PAT_MASK0_WIDTH                                                 32
#define PCIE_PAT_MASK0_SHIFT                                                  0
#define PCIE_PAT_MASK0_MASK                                          0xffffffff
#define PCIE_PAT_MASK0_RD(src)                           (((src) & 0xffffffff))
#define PCIE_PAT_MASK0_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define PCIE_PATTERN1_WIDTH                                                  32
#define PCIE_PATTERN1_SHIFT                                                   0
#define PCIE_PATTERN1_MASK                                           0xffffffff
#define PCIE_PATTERN1_RD(src)                            (((src) & 0xffffffff))
#define PCIE_PATTERN1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PCIE_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define PCIE_PAT_MASK1_WIDTH                                                 32
#define PCIE_PAT_MASK1_SHIFT                                                  0
#define PCIE_PAT_MASK1_MASK                                          0xffffffff
#define PCIE_PAT_MASK1_RD(src)                           (((src) & 0xffffffff))
#define PCIE_PAT_MASK1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PCIE_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define PCIE_DBG_OUT_WIDTH                                                   32
#define PCIE_DBG_OUT_SHIFT                                                    0
#define PCIE_DBG_OUT_MASK                                            0xffffffff
#define PCIE_DBG_OUT_RD(src)                             (((src) & 0xffffffff))
#define PCIE_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define PCIE_SLV_TMO_WIDTH                                                    1
#define PCIE_SLV_TMO_SHIFT                                                    3
#define PCIE_SLV_TMO_MASK                                            0x00000008
#define PCIE_SLV_TMO_RD(src)                          (((src) & 0x00000008)>>3)
#define PCIE_SLV_TMO_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define PCIE_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define PCIE_SECC_INT_WIDTH                                                   1
#define PCIE_SECC_INT_SHIFT                                                   2
#define PCIE_SECC_INT_MASK                                           0x00000004
#define PCIE_SECC_INT_RD(src)                         (((src) & 0x00000004)>>2)
#define PCIE_SECC_INT_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define PCIE_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define PCIE_DECC_INT_WIDTH                                                   1
#define PCIE_DECC_INT_SHIFT                                                   1
#define PCIE_DECC_INT_MASK                                           0x00000002
#define PCIE_DECC_INT_RD(src)                         (((src) & 0x00000002)>>1)
#define PCIE_DECC_INT_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define PCIE_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define PCIE_DBG_INT_WIDTH                                                    1
#define PCIE_DBG_INT_SHIFT                                                    0
#define PCIE_DBG_INT_MASK                                            0x00000001
#define PCIE_DBG_INT_RD(src)                             (((src) & 0x00000001))
#define PCIE_DBG_INT_WR(src)                        (((u32)(src)) & 0x00000001)
#define PCIE_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define PCIE_SLV_TMOMASK_WIDTH                                                1
#define PCIE_SLV_TMOMASK_SHIFT                                                3
#define PCIE_SLV_TMOMASK_MASK                                        0x00000008
#define PCIE_SLV_TMOMASK_RD(src)                      (((src) & 0x00000008)>>3)
#define PCIE_SLV_TMOMASK_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define PCIE_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define PCIE_SECC_INTMASK_WIDTH                                               1
#define PCIE_SECC_INTMASK_SHIFT                                               2
#define PCIE_SECC_INTMASK_MASK                                       0x00000004
#define PCIE_SECC_INTMASK_RD(src)                     (((src) & 0x00000004)>>2)
#define PCIE_SECC_INTMASK_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define PCIE_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define PCIE_DECC_INTMASK_WIDTH                                               1
#define PCIE_DECC_INTMASK_SHIFT                                               1
#define PCIE_DECC_INTMASK_MASK                                       0x00000002
#define PCIE_DECC_INTMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define PCIE_DECC_INTMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define PCIE_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define PCIE_DBG_INTMASK_WIDTH                                                1
#define PCIE_DBG_INTMASK_SHIFT                                                0
#define PCIE_DBG_INTMASK_MASK                                        0x00000001
#define PCIE_DBG_INTMASK_RD(src)                         (((src) & 0x00000001))
#define PCIE_DBG_INTMASK_WR(src)                    (((u32)(src)) & 0x00000001)
#define PCIE_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define PCIE_SECC_INT_THRSH_WIDTH                                             8
#define PCIE_SECC_INT_THRSH_SHIFT                                             0
#define PCIE_SECC_INT_THRSH_MASK                                     0x000000ff
#define PCIE_SECC_INT_THRSH_RD(src)                      (((src) & 0x000000ff))
#define PCIE_SECC_INT_THRSH_WR(src)                 (((u32)(src)) & 0x000000ff)
#define PCIE_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define PCIE_CFG_ECC_BYPASS_WIDTH                                            16
#define PCIE_CFG_ECC_BYPASS_SHIFT                                             0
#define PCIE_CFG_ECC_BYPASS_MASK                                     0x0000ffff
#define PCIE_CFG_ECC_BYPASS_RD(src)                      (((src) & 0x0000ffff))
#define PCIE_CFG_ECC_BYPASS_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define PCIE_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define PCIE_CFG_ECC_PWRDN_DIS_WIDTH                                         16
#define PCIE_CFG_ECC_PWRDN_DIS_SHIFT                                         16
#define PCIE_CFG_ECC_PWRDN_DIS_MASK                                  0xffff0000
#define PCIE_CFG_ECC_PWRDN_DIS_RD(src)               (((src) & 0xffff0000)>>16)
#define PCIE_CFG_ECC_PWRDN_DIS_WR(src)          (((u32)(src)<<16) & 0xffff0000)
#define PCIE_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define PCIE_CFG_PWRDN_DIS_WIDTH                                             16
#define PCIE_CFG_PWRDN_DIS_SHIFT                                              0
#define PCIE_CFG_PWRDN_DIS_MASK                                      0x0000ffff
#define PCIE_CFG_PWRDN_DIS_RD(src)                       (((src) & 0x0000ffff))
#define PCIE_CFG_PWRDN_DIS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define PCIE_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define PCIE_CFG_RAM_SHUTDOWN_EN_WIDTH                                       32
#define PCIE_CFG_RAM_SHUTDOWN_EN_SHIFT                                        0
#define PCIE_CFG_RAM_SHUTDOWN_EN_MASK                                0xffffffff
#define PCIE_CFG_RAM_SHUTDOWN_EN_RD(src)                 (((src) & 0xffffffff))
#define PCIE_CFG_RAM_SHUTDOWN_EN_WR(src)            (((u32)(src)) & 0xffffffff)
#define PCIE_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define PCIE_MEM_RDY_WIDTH                                                   32
#define PCIE_MEM_RDY_SHIFT                                                    0
#define PCIE_MEM_RDY_MASK                                            0xffffffff
#define PCIE_MEM_RDY_RD(src)                             (((src) & 0xffffffff))
#define PCIE_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define PCIE_SLV_ID_TMO_ENABLE_WIDTH                                          1
#define PCIE_SLV_ID_TMO_ENABLE_SHIFT                                         31
#define PCIE_SLV_ID_TMO_ENABLE_MASK                                  0x80000000
#define PCIE_SLV_ID_TMO_ENABLE_RD(src)               (((src) & 0x80000000)>>31)
#define PCIE_SLV_ID_TMO_ENABLE_WR(src)          (((u32)(src)<<31) & 0x80000000)
#define PCIE_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define PCIE_SS_ID_TRK_ENABLE_WIDTH                                           1
#define PCIE_SS_ID_TRK_ENABLE_SHIFT                                          30
#define PCIE_SS_ID_TRK_ENABLE_MASK                                   0x40000000
#define PCIE_SS_ID_TRK_ENABLE_RD(src)                (((src) & 0x40000000)>>30)
#define PCIE_SS_ID_TRK_ENABLE_WR(src)           (((u32)(src)<<30) & 0x40000000)
#define PCIE_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define PCIE_SLV_ID_TMO_VALUE_WIDTH                                          10
#define PCIE_SLV_ID_TMO_VALUE_SHIFT                                          16
#define PCIE_SLV_ID_TMO_VALUE_MASK                                   0x03ff0000
#define PCIE_SLV_ID_TMO_VALUE_RD(src)                (((src) & 0x03ff0000)>>16)
#define PCIE_SLV_ID_TMO_VALUE_WR(src)           (((u32)(src)<<16) & 0x03ff0000)
#define PCIE_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define PCIE_SLV_RDY_TMO_ENABLE_WIDTH                                         1
#define PCIE_SLV_RDY_TMO_ENABLE_SHIFT                                        15
#define PCIE_SLV_RDY_TMO_ENABLE_MASK                                 0x00008000
#define PCIE_SLV_RDY_TMO_ENABLE_RD(src)              (((src) & 0x00008000)>>15)
#define PCIE_SLV_RDY_TMO_ENABLE_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define PCIE_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define PCIE_SLV_RDY_TMO_VALUE_WIDTH                                         10
#define PCIE_SLV_RDY_TMO_VALUE_SHIFT                                          0
#define PCIE_SLV_RDY_TMO_VALUE_MASK                                  0x000003ff
#define PCIE_SLV_RDY_TMO_VALUE_RD(src)                   (((src) & 0x000003ff))
#define PCIE_SLV_RDY_TMO_VALUE_WR(src)              (((u32)(src)) & 0x000003ff)
#define PCIE_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define PCIE_SLV_ID_TMO_ENABLE_F1_WIDTH                                       1
#define PCIE_SLV_ID_TMO_ENABLE_F1_SHIFT                                      31
#define PCIE_SLV_ID_TMO_ENABLE_F1_MASK                               0x80000000
#define PCIE_SLV_ID_TMO_ENABLE_F1_RD(src)            (((src) & 0x80000000)>>31)
#define PCIE_SLV_ID_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<31) & 0x80000000)
#define PCIE_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define PCIE_SS_ID_TRK_ENABLE_F1_WIDTH                                        1
#define PCIE_SS_ID_TRK_ENABLE_F1_SHIFT                                       30
#define PCIE_SS_ID_TRK_ENABLE_F1_MASK                                0x40000000
#define PCIE_SS_ID_TRK_ENABLE_F1_RD(src)             (((src) & 0x40000000)>>30)
#define PCIE_SS_ID_TRK_ENABLE_F1_WR(src)        (((u32)(src)<<30) & 0x40000000)
#define PCIE_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define PCIE_SLV_ID_TMO_VALUE_F1_WIDTH                                       10
#define PCIE_SLV_ID_TMO_VALUE_F1_SHIFT                                       16
#define PCIE_SLV_ID_TMO_VALUE_F1_MASK                                0x03ff0000
#define PCIE_SLV_ID_TMO_VALUE_F1_RD(src)             (((src) & 0x03ff0000)>>16)
#define PCIE_SLV_ID_TMO_VALUE_F1_WR(src)        (((u32)(src)<<16) & 0x03ff0000)
#define PCIE_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define PCIE_SLV_RDY_TMO_ENABLE_F1_WIDTH                                      1
#define PCIE_SLV_RDY_TMO_ENABLE_F1_SHIFT                                     15
#define PCIE_SLV_RDY_TMO_ENABLE_F1_MASK                              0x00008000
#define PCIE_SLV_RDY_TMO_ENABLE_F1_RD(src)           (((src) & 0x00008000)>>15)
#define PCIE_SLV_RDY_TMO_ENABLE_F1_WR(src)      (((u32)(src)<<15) & 0x00008000)
#define PCIE_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define PCIE_SLV_RDY_TMO_VALUE_F1_WIDTH                                      10
#define PCIE_SLV_RDY_TMO_VALUE_F1_SHIFT                                       0
#define PCIE_SLV_RDY_TMO_VALUE_F1_MASK                               0x000003ff
#define PCIE_SLV_RDY_TMO_VALUE_F1_RD(src)                (((src) & 0x000003ff))
#define PCIE_SLV_RDY_TMO_VALUE_F1_WR(src)           (((u32)(src)) & 0x000003ff)
#define PCIE_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define PCIE_SLV_ARID_TMO_WIDTH                                               1
#define PCIE_SLV_ARID_TMO_SHIFT                                               3
#define PCIE_SLV_ARID_TMO_MASK                                       0x00000008
#define PCIE_SLV_ARID_TMO_RD(src)                     (((src) & 0x00000008)>>3)
#define PCIE_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define PCIE_SLV_ARRDY_TMO_WIDTH                                              1
#define PCIE_SLV_ARRDY_TMO_SHIFT                                              2
#define PCIE_SLV_ARRDY_TMO_MASK                                      0x00000004
#define PCIE_SLV_ARRDY_TMO_RD(src)                    (((src) & 0x00000004)>>2)
#define PCIE_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define PCIE_SLV_AWID_TMO_WIDTH                                               1
#define PCIE_SLV_AWID_TMO_SHIFT                                               1
#define PCIE_SLV_AWID_TMO_MASK                                       0x00000002
#define PCIE_SLV_AWID_TMO_RD(src)                     (((src) & 0x00000002)>>1)
#define PCIE_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define PCIE_SLV_AWRDY_TMO_WIDTH                                              1
#define PCIE_SLV_AWRDY_TMO_SHIFT                                              0
#define PCIE_SLV_AWRDY_TMO_MASK                                      0x00000001
#define PCIE_SLV_AWRDY_TMO_RD(src)                       (((src) & 0x00000001))
#define PCIE_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define PCIE_CFG_CSR_TMO_WIDTH                                               16
#define PCIE_CFG_CSR_TMO_SHIFT                                               16
#define PCIE_CFG_CSR_TMO_MASK                                        0xffff0000
#define PCIE_CFG_CSR_TMO_RD(src)                     (((src) & 0xffff0000)>>16)
#define PCIE_CFG_CSR_TMO_WR(src)                (((u32)(src)<<16) & 0xffff0000)
#define PCIE_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define PCIE_CSR_ERR_RESP_EN_WIDTH                                            1
#define PCIE_CSR_ERR_RESP_EN_SHIFT                                           15
#define PCIE_CSR_ERR_RESP_EN_MASK                                    0x00008000
#define PCIE_CSR_ERR_RESP_EN_RD(src)                 (((src) & 0x00008000)>>15)
#define PCIE_CSR_ERR_RESP_EN_WR(src)            (((u32)(src)<<15) & 0x00008000)
#define PCIE_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define PCIE_TOT_READS_WIDTH                                                 16
#define PCIE_TOT_READS_SHIFT                                                  0
#define PCIE_TOT_READS_MASK                                          0x0000ffff
#define PCIE_TOT_READS_RD(src)                           (((src) & 0x0000ffff))
#define PCIE_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Global Base Address	*/
#define PCIE1_SERDES_CSR_BASE_ADDR		0xddd884000ULL
#define PCIE2_SERDES_CSR_BASE_ADDR		0xddd894000ULL

/*    Address P1_PCIE_SERDES_CSR_PORT1  Registers */
#define PCSCSR_CTL0_ADDR                                             0x00000000
#define PCSCSR_CTL0_DEFAULT                                          0x10000229
#define PCSCSR_CTL1_ADDR                                             0x00000004
#define PCSCSR_CTL1_DEFAULT                                          0x00800000
#define PCSCSR_STATUS1_ADDR                                          0x00000008
#define PCSCSR_STATUS1_DEFAULT                                       0x00000000
#define PCSCSR_REGADDR_ADDR                                          0x0000000c
#define PCSCSR_REGADDR_DEFAULT                                       0x00000000
#define PCSCSR_REGWRDATA_ADDR                                        0x00000010
#define PCSCSR_REGWRDATA_DEFAULT                                     0x00000000
#define PCSCSR_REGCMD_ADDR                                           0x00000014
#define PCSCSR_REGCMD_DEFAULT                                        0x00000000
#define PCSCSR_REGCMDDONE_ADDR                                       0x00000018
#define PCSCSR_REGCMDDONE_DEFAULT                                    0x00000000
#define PCSCSR_REGRDDATA_ADDR                                        0x0000001c
#define PCSCSR_REGRDDATA_DEFAULT                                     0x00000000
#define PCSCSR_INTERRUPT_ADDR                                        0x00000020
#define PCSCSR_INTERRUPT_DEFAULT                                     0x00000000
#define PCSCSR_INTERRUPTMASK_ADDR                                    0x00000024
#define PCSCSR_CLKCTL_ADDR                                           0x00000028
#define PCSCSR_CLKCTL_DEFAULT                                        0xffffffff
#define PCSCSR_CTL2_ADDR                                             0x0000002c
#define PCSCSR_CTL2_DEFAULT                                          0x00000000
#define PCSCSR_CTL3_ADDR                                             0x00000030
#define PCSCSR_CTL3_DEFAULT                                          0x00000000
#define SATA_IOCOH_START0_ADDR                                       0x00000034
#define SATA_IOCOH_START0_DEFAULT                                    0x00000000
#define SATA_IOCOH_START1_ADDR                                       0x00000038
#define SATA_IOCOH_START1_DEFAULT                                    0x00000000
#define SATA_IOCOH_END0_ADDR                                         0x0000003c
#define SATA_IOCOH_END0_DEFAULT                                      0x00000000
#define SATA_IOCOH_END1_ADDR                                         0x00000040
#define SATA_IOCOH_END1_DEFAULT                                      0x00000000

/*	Register PcsCsr_Ctl0	*/ 
/*	 Fields enet_sgmii_enable	 */
#define ENET_SGMII_ENABLE0_WIDTH                                              1
#define ENET_SGMII_ENABLE0_SHIFT                                             30
#define ENET_SGMII_ENABLE0_MASK                                      0x40000000
#define ENET_SGMII_ENABLE0_RD(src)                   (((src) & 0x40000000)>>30)
#define ENET_SGMII_ENABLE0_WR(src)              (((u32)(src)<<30) & 0x40000000)
#define ENET_SGMII_ENABLE0_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields phy_pcs_rx0_sigdet	 */
#define PHY_PCS_RX0_SIGDET0_WIDTH                                             1
#define PHY_PCS_RX0_SIGDET0_SHIFT                                            29
#define PHY_PCS_RX0_SIGDET0_MASK                                     0x20000000
#define PHY_PCS_RX0_SIGDET0_RD(src)                  (((src) & 0x20000000)>>29)
#define PHY_PCS_RX0_SIGDET0_WR(src)             (((u32)(src)<<29) & 0x20000000)
#define PHY_PCS_RX0_SIGDET0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields phy_tx_term_cal_in	 */
#define PHY_TX_TERM_CAL_IN0_WIDTH                                             4
#define PHY_TX_TERM_CAL_IN0_SHIFT                                            25
#define PHY_TX_TERM_CAL_IN0_MASK                                     0x1e000000
#define PHY_TX_TERM_CAL_IN0_RD(src)                  (((src) & 0x1e000000)>>25)
#define PHY_TX_TERM_CAL_IN0_WR(src)             (((u32)(src)<<25) & 0x1e000000)
#define PHY_TX_TERM_CAL_IN0_SET(dst,src) \
                      (((dst) & ~0x1e000000) | (((u32)(src)<<25) & 0x1e000000))
/*	 Fields mac_pcs_x2_mode	 */
#define PCIE_MAC_PCS_X2_MODE0_WIDTH                                           1
#define PCIE_MAC_PCS_X2_MODE0_SHIFT                                          24
#define PCIE_MAC_PCS_X2_MODE0_MASK                                   0x01000000
#define PCIE_MAC_PCS_X2_MODE0_RD(src)                (((src) & 0x01000000)>>24)
#define PCIE_MAC_PCS_X2_MODE0_WR(src)           (((u32)(src)<<24) & 0x01000000)
#define PCIE_MAC_PCS_X2_MODE0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields mac_pcs_pll_pcs_div	 */
#define MAC_PCS_PLL_PCS_DIV0_WIDTH                                            6
#define MAC_PCS_PLL_PCS_DIV0_SHIFT                                           18
#define MAC_PCS_PLL_PCS_DIV0_MASK                                    0x00fc0000
#define MAC_PCS_PLL_PCS_DIV0_RD(src)                 (((src) & 0x00fc0000)>>18)
#define MAC_PCS_PLL_PCS_DIV0_WR(src)            (((u32)(src)<<18) & 0x00fc0000)
#define MAC_PCS_PLL_PCS_DIV0_SET(dst,src) \
                      (((dst) & ~0x00fc0000) | (((u32)(src)<<18) & 0x00fc0000))
/*	 Fields mac_pcs_pll_div	 */
#define MAC_PCS_PLL_DIV0_WIDTH                                                6
#define MAC_PCS_PLL_DIV0_SHIFT                                               12
#define MAC_PCS_PLL_DIV0_MASK                                        0x0003f000
#define MAC_PCS_PLL_DIV0_RD(src)                     (((src) & 0x0003f000)>>12)
#define MAC_PCS_PLL_DIV0_WR(src)                (((u32)(src)<<12) & 0x0003f000)
#define MAC_PCS_PLL_DIV0_SET(dst,src) \
                      (((dst) & ~0x0003f000) | (((u32)(src)<<12) & 0x0003f000))
/*	 Fields mac_pcs_pll_op_range	 */
#define MAC_PCS_PLL_OP_RANGE0_WIDTH                                           3
#define MAC_PCS_PLL_OP_RANGE0_SHIFT                                           9
#define MAC_PCS_PLL_OP_RANGE0_MASK                                   0x00000e00
#define MAC_PCS_PLL_OP_RANGE0_RD(src)                 (((src) & 0x00000e00)>>9)
#define MAC_PCS_PLL_OP_RANGE0_WR(src)            (((u32)(src)<<9) & 0x00000e00)
#define MAC_PCS_PLL_OP_RANGE0_SET(dst,src) \
                       (((dst) & ~0x00000e00) | (((u32)(src)<<9) & 0x00000e00))
/*	 Fields mac_pcs_pll_refclk_sel	 */
#define MAC_PCS_PLL_REFCLK_SEL0_WIDTH                                         3
#define MAC_PCS_PLL_REFCLK_SEL0_SHIFT                                         6
#define MAC_PCS_PLL_REFCLK_SEL0_MASK                                 0x000001c0
#define MAC_PCS_PLL_REFCLK_SEL0_RD(src)               (((src) & 0x000001c0)>>6)
#define MAC_PCS_PLL_REFCLK_SEL0_WR(src)          (((u32)(src)<<6) & 0x000001c0)
#define MAC_PCS_PLL_REFCLK_SEL0_SET(dst,src) \
                       (((dst) & ~0x000001c0) | (((u32)(src)<<6) & 0x000001c0))
/*	 Fields reset	 */
#define PCIE_RESET0_WIDTH                                                     1
#define PCIE_RESET0_SHIFT                                                     5
#define PCIE_RESET0_MASK                                             0x00000020
#define PCIE_RESET0_RD(src)                           (((src) & 0x00000020)>>5)
#define PCIE_RESET0_WR(src)                      (((u32)(src)<<5) & 0x00000020)
#define PCIE_RESET0_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields init_ctlifc_use_mgmt	 */
#define INIT_CTLIFC_USE_MGMT0_WIDTH                                           1
#define INIT_CTLIFC_USE_MGMT0_SHIFT                                           4
#define INIT_CTLIFC_USE_MGMT0_MASK                                   0x00000010
#define INIT_CTLIFC_USE_MGMT0_RD(src)                 (((src) & 0x00000010)>>4)
#define INIT_CTLIFC_USE_MGMT0_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define INIT_CTLIFC_USE_MGMT0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields init_ctlifc_pipe_compliant	 */
#define INIT_CTLIFC_PIPE_COMPLIANT0_WIDTH                                     1
#define INIT_CTLIFC_PIPE_COMPLIANT0_SHIFT                                     3
#define INIT_CTLIFC_PIPE_COMPLIANT0_MASK                             0x00000008
#define INIT_CTLIFC_PIPE_COMPLIANT0_RD(src)           (((src) & 0x00000008)>>3)
#define INIT_CTLIFC_PIPE_COMPLIANT0_WR(src)      (((u32)(src)<<3) & 0x00000008)
#define INIT_CTLIFC_PIPE_COMPLIANT0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields init_refclk_ac_mode	 */
#define PCIE_INIT_REFCLK_AC_MODE0_WIDTH                                       1
#define PCIE_INIT_REFCLK_AC_MODE0_SHIFT                                       2
#define PCIE_INIT_REFCLK_AC_MODE0_MASK                               0x00000004
#define PCIE_INIT_REFCLK_AC_MODE0_RD(src)             (((src) & 0x00000004)>>2)
#define PCIE_INIT_REFCLK_AC_MODE0_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define PCIE_INIT_REFCLK_AC_MODE0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields init_tx_term_sel	 */
#define INIT_TX_TERM_SEL0_WIDTH                                               1
#define INIT_TX_TERM_SEL0_SHIFT                                               1
#define INIT_TX_TERM_SEL0_MASK                                       0x00000002
#define INIT_TX_TERM_SEL0_RD(src)                     (((src) & 0x00000002)>>1)
#define INIT_TX_TERM_SEL0_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define INIT_TX_TERM_SEL0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcie_mode_sel	 */
#define PCIE_PCIE_MODE_SEL0_WIDTH                                             1
#define PCIE_PCIE_MODE_SEL0_SHIFT                                             0
#define PCIE_PCIE_MODE_SEL0_MASK                                     0x00000001
#define PCIE_PCIE_MODE_SEL0_RD(src)                      (((src) & 0x00000001))
#define PCIE_PCIE_MODE_SEL0_WR(src)                 (((u32)(src)) & 0x00000001)
#define PCIE_PCIE_MODE_SEL0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl1	*/ 
/*	 Fields mac_pcs_tx_clk_inv	 */
#define MAC_PCS_TX_CLK_INV1_WIDTH                                             1
#define MAC_PCS_TX_CLK_INV1_SHIFT                                            26
#define MAC_PCS_TX_CLK_INV1_MASK                                     0x04000000
#define MAC_PCS_TX_CLK_INV1_RD(src)                  (((src) & 0x04000000)>>26)
#define MAC_PCS_TX_CLK_INV1_WR(src)             (((u32)(src)<<26) & 0x04000000)
#define MAC_PCS_TX_CLK_INV1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields mac_pcs_rx_clk_inv	 */
#define MAC_PCS_RX_CLK_INV1_WIDTH                                             1
#define MAC_PCS_RX_CLK_INV1_SHIFT                                            25
#define MAC_PCS_RX_CLK_INV1_MASK                                     0x02000000
#define MAC_PCS_RX_CLK_INV1_RD(src)                  (((src) & 0x02000000)>>25)
#define MAC_PCS_RX_CLK_INV1_WR(src)             (((u32)(src)<<25) & 0x02000000)
#define MAC_PCS_RX_CLK_INV1_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields mac_pcs_rx0_encdet	 */
#define MAC_PCS_RX0_ENCDET1_WIDTH                                             1
#define MAC_PCS_RX0_ENCDET1_SHIFT                                            24
#define MAC_PCS_RX0_ENCDET1_MASK                                     0x01000000
#define MAC_PCS_RX0_ENCDET1_RD(src)                  (((src) & 0x01000000)>>24)
#define MAC_PCS_RX0_ENCDET1_WR(src)             (((u32)(src)<<24) & 0x01000000)
#define MAC_PCS_RX0_ENCDET1_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields mac_pcs_sata0_host	 */
#define MAC_PCS_SATA0_HOST1_WIDTH                                             1
#define MAC_PCS_SATA0_HOST1_SHIFT                                            23
#define MAC_PCS_SATA0_HOST1_MASK                                     0x00800000
#define MAC_PCS_SATA0_HOST1_RD(src)                  (((src) & 0x00800000)>>23)
#define MAC_PCS_SATA0_HOST1_WR(src)             (((u32)(src)<<23) & 0x00800000)
#define MAC_PCS_SATA0_HOST1_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields mac_pcs_rx0_edet_lb	 */
#define MAC_PCS_RX0_EDET_LB1_WIDTH                                            1
#define MAC_PCS_RX0_EDET_LB1_SHIFT                                           22
#define MAC_PCS_RX0_EDET_LB1_MASK                                    0x00400000
#define MAC_PCS_RX0_EDET_LB1_RD(src)                 (((src) & 0x00400000)>>22)
#define MAC_PCS_RX0_EDET_LB1_WR(src)            (((u32)(src)<<22) & 0x00400000)
#define MAC_PCS_RX0_EDET_LB1_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields mac_pcs_rx0_edet_eie	 */
#define MAC_PCS_RX0_EDET_EIE1_WIDTH                                           1
#define MAC_PCS_RX0_EDET_EIE1_SHIFT                                          21
#define MAC_PCS_RX0_EDET_EIE1_MASK                                   0x00200000
#define MAC_PCS_RX0_EDET_EIE1_RD(src)                (((src) & 0x00200000)>>21)
#define MAC_PCS_RX0_EDET_EIE1_WR(src)           (((u32)(src)<<21) & 0x00200000)
#define MAC_PCS_RX0_EDET_EIE1_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields mac_pcs_rx0_edet_ts	 */
#define MAC_PCS_RX0_EDET_TS1_WIDTH                                            1
#define MAC_PCS_RX0_EDET_TS1_SHIFT                                           20
#define MAC_PCS_RX0_EDET_TS1_MASK                                    0x00100000
#define MAC_PCS_RX0_EDET_TS1_RD(src)                 (((src) & 0x00100000)>>20)
#define MAC_PCS_RX0_EDET_TS1_WR(src)            (((u32)(src)<<20) & 0x00100000)
#define MAC_PCS_RX0_EDET_TS1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields mac_pcs_rx0_edet_skp	 */
#define MAC_PCS_RX0_EDET_SKP1_WIDTH                                           1
#define MAC_PCS_RX0_EDET_SKP1_SHIFT                                          19
#define MAC_PCS_RX0_EDET_SKP1_MASK                                   0x00080000
#define MAC_PCS_RX0_EDET_SKP1_RD(src)                (((src) & 0x00080000)>>19)
#define MAC_PCS_RX0_EDET_SKP1_WR(src)           (((u32)(src)<<19) & 0x00080000)
#define MAC_PCS_RX0_EDET_SKP1_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields mac_pcs_ctl0_op_range	 */
#define MAC_PCS_CTL0_OP_RANGE1_WIDTH                                          3
#define MAC_PCS_CTL0_OP_RANGE1_SHIFT                                         16
#define MAC_PCS_CTL0_OP_RANGE1_MASK                                  0x00070000
#define MAC_PCS_CTL0_OP_RANGE1_RD(src)               (((src) & 0x00070000)>>16)
#define MAC_PCS_CTL0_OP_RANGE1_WR(src)          (((u32)(src)<<16) & 0x00070000)
#define MAC_PCS_CTL0_OP_RANGE1_SET(dst,src) \
                      (((dst) & ~0x00070000) | (((u32)(src)<<16) & 0x00070000))
/*	 Fields mac_pcs_ctl0_smpl_rate	 */
#define MAC_PCS_CTL0_SMPL_RATE1_WIDTH                                         3
#define MAC_PCS_CTL0_SMPL_RATE1_SHIFT                                        13
#define MAC_PCS_CTL0_SMPL_RATE1_MASK                                 0x0000e000
#define MAC_PCS_CTL0_SMPL_RATE1_RD(src)              (((src) & 0x0000e000)>>13)
#define MAC_PCS_CTL0_SMPL_RATE1_WR(src)         (((u32)(src)<<13) & 0x0000e000)
#define MAC_PCS_CTL0_SMPL_RATE1_SET(dst,src) \
                      (((dst) & ~0x0000e000) | (((u32)(src)<<13) & 0x0000e000))
/*	 Fields mac_pcs_ctl0_rx_lane_div	 */
#define MAC_PCS_CTL0_RX_LANE_DIV1_WIDTH                                       2
#define MAC_PCS_CTL0_RX_LANE_DIV1_SHIFT                                      11
#define MAC_PCS_CTL0_RX_LANE_DIV1_MASK                               0x00001800
#define MAC_PCS_CTL0_RX_LANE_DIV1_RD(src)            (((src) & 0x00001800)>>11)
#define MAC_PCS_CTL0_RX_LANE_DIV1_WR(src)       (((u32)(src)<<11) & 0x00001800)
#define MAC_PCS_CTL0_RX_LANE_DIV1_SET(dst,src) \
                      (((dst) & ~0x00001800) | (((u32)(src)<<11) & 0x00001800))
/*	 Fields mac_pcs_ctl0_tx_lane_div	 */
#define MAC_PCS_CTL0_TX_LANE_DIV1_WIDTH                                       2
#define MAC_PCS_CTL0_TX_LANE_DIV1_SHIFT                                       9
#define MAC_PCS_CTL0_TX_LANE_DIV1_MASK                               0x00000600
#define MAC_PCS_CTL0_TX_LANE_DIV1_RD(src)             (((src) & 0x00000600)>>9)
#define MAC_PCS_CTL0_TX_LANE_DIV1_WR(src)        (((u32)(src)<<9) & 0x00000600)
#define MAC_PCS_CTL0_TX_LANE_DIV1_SET(dst,src) \
                       (((dst) & ~0x00000600) | (((u32)(src)<<9) & 0x00000600))
/*	 Fields mac_pcs_ctl0_lane_pwr_off	 */
#define MAC_PCS_CTL0_LANE_PWR_OFF1_WIDTH                                      1
#define MAC_PCS_CTL0_LANE_PWR_OFF1_SHIFT                                      8
#define MAC_PCS_CTL0_LANE_PWR_OFF1_MASK                              0x00000100
#define MAC_PCS_CTL0_LANE_PWR_OFF1_RD(src)            (((src) & 0x00000100)>>8)
#define MAC_PCS_CTL0_LANE_PWR_OFF1_WR(src)       (((u32)(src)<<8) & 0x00000100)
#define MAC_PCS_CTL0_LANE_PWR_OFF1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields mac_pcs_ctl0_rx_mode	 */
#define PCIE_MAC_PCS_CTL0_RX_MODE1_WIDTH                                      4
#define PCIE_MAC_PCS_CTL0_RX_MODE1_SHIFT                                      4
#define PCIE_MAC_PCS_CTL0_RX_MODE1_MASK                              0x000000f0
#define PCIE_MAC_PCS_CTL0_RX_MODE1_RD(src)            (((src) & 0x000000f0)>>4)
#define PCIE_MAC_PCS_CTL0_RX_MODE1_WR(src)       (((u32)(src)<<4) & 0x000000f0)
#define PCIE_MAC_PCS_CTL0_RX_MODE1_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields mac_pcs_ctl0_tx_mode	 */
#define PCIE_MAC_PCS_CTL0_TX_MODE1_WIDTH                                      4
#define PCIE_MAC_PCS_CTL0_TX_MODE1_SHIFT                                      0
#define PCIE_MAC_PCS_CTL0_TX_MODE1_MASK                              0x0000000f
#define PCIE_MAC_PCS_CTL0_TX_MODE1_RD(src)               (((src) & 0x0000000f))
#define PCIE_MAC_PCS_CTL0_TX_MODE1_WR(src)          (((u32)(src)) & 0x0000000f)
#define PCIE_MAC_PCS_CTL0_TX_MODE1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PcsCsr_Status1	*/ 
/*	 Fields sds_pcs_rx0_cdr_status	 */
#define SDS_PCS_RX0_CDR_STATUS1_WIDTH                                        16
#define SDS_PCS_RX0_CDR_STATUS1_SHIFT                                        12
#define SDS_PCS_RX0_CDR_STATUS1_MASK                                 0x0ffff000
#define SDS_PCS_RX0_CDR_STATUS1_RD(src)              (((src) & 0x0ffff000)>>12)
#define SDS_PCS_RX0_CDR_STATUS1_SET(dst,src) \
                      (((dst) & ~0x0ffff000) | (((u32)(src)<<12) & 0x0ffff000))
/*	 Fields sds_pcs_clock_ready	 */
#define SDS_PCS_CLOCK_READY1_WIDTH                                            1
#define SDS_PCS_CLOCK_READY1_SHIFT                                           11
#define SDS_PCS_CLOCK_READY1_MASK                                    0x00000800
#define SDS_PCS_CLOCK_READY1_RD(src)                 (((src) & 0x00000800)>>11)
#define SDS_PCS_CLOCK_READY1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields sds_pcs_pll_lock	 */
#define SDS_PCS_PLL_LOCK1_WIDTH                                               1
#define SDS_PCS_PLL_LOCK1_SHIFT                                              10
#define SDS_PCS_PLL_LOCK1_MASK                                       0x00000400
#define SDS_PCS_PLL_LOCK1_RD(src)                    (((src) & 0x00000400)>>10)
#define SDS_PCS_PLL_LOCK1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields pcs_mac_rx0_ctrl	 */
#define PCS_MAC_RX0_CTRL1_WIDTH                                               6
#define PCS_MAC_RX0_CTRL1_SHIFT                                               4
#define PCS_MAC_RX0_CTRL1_MASK                                       0x000003f0
#define PCS_MAC_RX0_CTRL1_RD(src)                     (((src) & 0x000003f0)>>4)
#define PCS_MAC_RX0_CTRL1_SET(dst,src) \
                       (((dst) & ~0x000003f0) | (((u32)(src)<<4) & 0x000003f0))
/*	 Fields phy_tx_term_cal_out	 */
#define PHY_TX_TERM_CAL_OUT1_WIDTH                                            4
#define PHY_TX_TERM_CAL_OUT1_SHIFT                                            0
#define PHY_TX_TERM_CAL_OUT1_MASK                                    0x0000000f
#define PHY_TX_TERM_CAL_OUT1_RD(src)                     (((src) & 0x0000000f))
#define PHY_TX_TERM_CAL_OUT1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PcsCsr_RegAddr	*/ 
/*	 Fields mgmt_pcs_mmd_addr	 */
#define MGMT_PCS_MMD_ADDR_WIDTH                                               5
#define MGMT_PCS_MMD_ADDR_SHIFT                                              18
#define MGMT_PCS_MMD_ADDR_MASK                                       0x007c0000
#define MGMT_PCS_MMD_ADDR_RD(src)                    (((src) & 0x007c0000)>>18)
#define MGMT_PCS_MMD_ADDR_WR(src)               (((u32)(src)<<18) & 0x007c0000)
#define MGMT_PCS_MMD_ADDR_SET(dst,src) \
                      (((dst) & ~0x007c0000) | (((u32)(src)<<18) & 0x007c0000))
/*	 Fields mgmt_pcs_lane_sel	 */
#define MGMT_PCS_LANE_SEL_WIDTH                                               2
#define MGMT_PCS_LANE_SEL_SHIFT                                              16
#define MGMT_PCS_LANE_SEL_MASK                                       0x00030000
#define MGMT_PCS_LANE_SEL_RD(src)                    (((src) & 0x00030000)>>16)
#define MGMT_PCS_LANE_SEL_WR(src)               (((u32)(src)<<16) & 0x00030000)
#define MGMT_PCS_LANE_SEL_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields mgmt_pcs_reg_addr	 */
#define MGMT_PCS_REG_ADDR_WIDTH                                              16
#define MGMT_PCS_REG_ADDR_SHIFT                                               0
#define MGMT_PCS_REG_ADDR_MASK                                       0x0000ffff
#define MGMT_PCS_REG_ADDR_RD(src)                        (((src) & 0x0000ffff))
#define MGMT_PCS_REG_ADDR_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define MGMT_PCS_REG_ADDR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_RegWrdata	*/ 
/*	 Fields mgmt_pcs_wr_data	 */
#define MGMT_PCS_WR_DATA_WIDTH                                               16
#define MGMT_PCS_WR_DATA_SHIFT                                                0
#define MGMT_PCS_WR_DATA_MASK                                        0x0000ffff
#define MGMT_PCS_WR_DATA_RD(src)                         (((src) & 0x0000ffff))
#define MGMT_PCS_WR_DATA_WR(src)                    (((u32)(src)) & 0x0000ffff)
#define MGMT_PCS_WR_DATA_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_RegCmd	*/ 
/*	 Fields mgmt_pcs_reg_wr	 */
#define MGMT_PCS_REG_WR_WIDTH                                                 1
#define MGMT_PCS_REG_WR_SHIFT                                                 1
#define MGMT_PCS_REG_WR_MASK                                         0x00000002
#define MGMT_PCS_REG_WR_RD(src)                       (((src) & 0x00000002)>>1)
#define MGMT_PCS_REG_WR_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define MGMT_PCS_REG_WR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mgmt_pcs_reg_rd	 */
#define MGMT_PCS_REG_RD_WIDTH                                                 1
#define MGMT_PCS_REG_RD_SHIFT                                                 0
#define MGMT_PCS_REG_RD_MASK                                         0x00000001
#define MGMT_PCS_REG_RD_RD(src)                          (((src) & 0x00000001))
#define MGMT_PCS_REG_RD_WR(src)                     (((u32)(src)) & 0x00000001)
#define MGMT_PCS_REG_RD_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_RegCmdDone	*/ 
/*	 Fields mgmt_pcs_reg_wr_done	 */
#define MGMT_PCS_REG_WR_DONE_WIDTH                                            1
#define MGMT_PCS_REG_WR_DONE_SHIFT                                            1
#define MGMT_PCS_REG_WR_DONE_MASK                                    0x00000002
#define MGMT_PCS_REG_WR_DONE_RD(src)                  (((src) & 0x00000002)>>1)
#define MGMT_PCS_REG_WR_DONE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mgmt_pcs_reg_rd_done	 */
#define MGMT_PCS_REG_RD_DONE_WIDTH                                            1
#define MGMT_PCS_REG_RD_DONE_SHIFT                                            0
#define MGMT_PCS_REG_RD_DONE_MASK                                    0x00000001
#define MGMT_PCS_REG_RD_DONE_RD(src)                     (((src) & 0x00000001))
#define MGMT_PCS_REG_RD_DONE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_RegRddata	*/ 
/*	 Fields pcs_mgmt_rd_data	 */
#define PCS_MGMT_RD_DATA_WIDTH                                               16
#define PCS_MGMT_RD_DATA_SHIFT                                                0
#define PCS_MGMT_RD_DATA_MASK                                        0x0000ffff
#define PCS_MGMT_RD_DATA_RD(src)                         (((src) & 0x0000ffff))
#define PCS_MGMT_RD_DATA_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_INT	*/ 
/*	 Fields pcs_mac_ctl0_rx_ack	 */
#define PCS_MAC_CTL0_RX_ACK_WIDTH                                             1
#define PCS_MAC_CTL0_RX_ACK_SHIFT                                             5
#define PCS_MAC_CTL0_RX_ACK_MASK                                     0x00000020
#define PCS_MAC_CTL0_RX_ACK_RD(src)                   (((src) & 0x00000020)>>5)
#define PCS_MAC_CTL0_RX_ACK_WR(src)              (((u32)(src)<<5) & 0x00000020)
#define PCS_MAC_CTL0_RX_ACK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields pcs_mac_ctl0_ack	 */
#define PCS_MAC_CTL0_ACK_WIDTH                                                1
#define PCS_MAC_CTL0_ACK_SHIFT                                                4
#define PCS_MAC_CTL0_ACK_MASK                                        0x00000010
#define PCS_MAC_CTL0_ACK_RD(src)                      (((src) & 0x00000010)>>4)
#define PCS_MAC_CTL0_ACK_WR(src)                 (((u32)(src)<<4) & 0x00000010)
#define PCS_MAC_CTL0_ACK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pcs_mac_ctl0_rx_mgmt_ack	 */
#define PCS_MAC_CTL0_RX_MGMT_ACK_WIDTH                                        1
#define PCS_MAC_CTL0_RX_MGMT_ACK_SHIFT                                        3
#define PCS_MAC_CTL0_RX_MGMT_ACK_MASK                                0x00000008
#define PCS_MAC_CTL0_RX_MGMT_ACK_RD(src)              (((src) & 0x00000008)>>3)
#define PCS_MAC_CTL0_RX_MGMT_ACK_WR(src)         (((u32)(src)<<3) & 0x00000008)
#define PCS_MAC_CTL0_RX_MGMT_ACK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields pcs_mac_ctl0_mgmt_ack	 */
#define PCS_MAC_CTL0_MGMT_ACK_WIDTH                                           1
#define PCS_MAC_CTL0_MGMT_ACK_SHIFT                                           2
#define PCS_MAC_CTL0_MGMT_ACK_MASK                                   0x00000004
#define PCS_MAC_CTL0_MGMT_ACK_RD(src)                 (((src) & 0x00000004)>>2)
#define PCS_MAC_CTL0_MGMT_ACK_WR(src)            (((u32)(src)<<2) & 0x00000004)
#define PCS_MAC_CTL0_MGMT_ACK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields pcs_mac_rx0_prbs_error	 */
#define PCS_MAC_RX0_PRBS_ERROR_WIDTH                                          1
#define PCS_MAC_RX0_PRBS_ERROR_SHIFT                                          1
#define PCS_MAC_RX0_PRBS_ERROR_MASK                                  0x00000002
#define PCS_MAC_RX0_PRBS_ERROR_RD(src)                (((src) & 0x00000002)>>1)
#define PCS_MAC_RX0_PRBS_ERROR_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define PCS_MAC_RX0_PRBS_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcs_mac_rx0_8b10b_error	 */
#define PCS_MAC_RX0_8B10B_ERROR_WIDTH                                         1
#define PCS_MAC_RX0_8B10B_ERROR_SHIFT                                         0
#define PCS_MAC_RX0_8B10B_ERROR_MASK                                 0x00000001
#define PCS_MAC_RX0_8B10B_ERROR_RD(src)                  (((src) & 0x00000001))
#define PCS_MAC_RX0_8B10B_ERROR_WR(src)             (((u32)(src)) & 0x00000001)
#define PCS_MAC_RX0_8B10B_ERROR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_InterruptMask	*/
/*    Mask Register Fields pcs_mac_ctl0_rx_ackMask    */
#define PCS_MAC_CTL0_RX_ACKMASK_WIDTH                                         1
#define PCS_MAC_CTL0_RX_ACKMASK_SHIFT                                         5
#define PCS_MAC_CTL0_RX_ACKMASK_MASK                                 0x00000020
#define PCS_MAC_CTL0_RX_ACKMASK_RD(src)               (((src) & 0x00000020)>>5)
#define PCS_MAC_CTL0_RX_ACKMASK_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define PCS_MAC_CTL0_RX_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields pcs_mac_ctl0_ackMask    */
#define PCS_MAC_CTL0_ACKMASK_WIDTH                                            1
#define PCS_MAC_CTL0_ACKMASK_SHIFT                                            4
#define PCS_MAC_CTL0_ACKMASK_MASK                                    0x00000010
#define PCS_MAC_CTL0_ACKMASK_RD(src)                  (((src) & 0x00000010)>>4)
#define PCS_MAC_CTL0_ACKMASK_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define PCS_MAC_CTL0_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields pcs_mac_ctl0_rx_mgmt_ackMask    */
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_WIDTH                                    1
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_SHIFT                                    3
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_MASK                            0x00000008
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_RD(src)          (((src) & 0x00000008)>>3)
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define PCS_MAC_CTL0_RX_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields pcs_mac_ctl0_mgmt_ackMask    */
#define PCS_MAC_CTL0_MGMT_ACKMASK_WIDTH                                       1
#define PCS_MAC_CTL0_MGMT_ACKMASK_SHIFT                                       2
#define PCS_MAC_CTL0_MGMT_ACKMASK_MASK                               0x00000004
#define PCS_MAC_CTL0_MGMT_ACKMASK_RD(src)             (((src) & 0x00000004)>>2)
#define PCS_MAC_CTL0_MGMT_ACKMASK_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define PCS_MAC_CTL0_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields pcs_mac_rx0_prbs_errorMask    */
#define PCS_MAC_RX0_PRBS_ERRORMASK_WIDTH                                      1
#define PCS_MAC_RX0_PRBS_ERRORMASK_SHIFT                                      1
#define PCS_MAC_RX0_PRBS_ERRORMASK_MASK                              0x00000002
#define PCS_MAC_RX0_PRBS_ERRORMASK_RD(src)            (((src) & 0x00000002)>>1)
#define PCS_MAC_RX0_PRBS_ERRORMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define PCS_MAC_RX0_PRBS_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pcs_mac_rx0_8b10b_errorMask    */
#define PCS_MAC_RX0_8B10B_ERRORMASK_WIDTH                                     1
#define PCS_MAC_RX0_8B10B_ERRORMASK_SHIFT                                     0
#define PCS_MAC_RX0_8B10B_ERRORMASK_MASK                             0x00000001
#define PCS_MAC_RX0_8B10B_ERRORMASK_RD(src)              (((src) & 0x00000001))
#define PCS_MAC_RX0_8B10B_ERRORMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define PCS_MAC_RX0_8B10B_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_ClkCtl	*/ 
/*	 Fields pcs_rx0_clk_en	 */
#define PCS_RX0_CLK_EN_WIDTH                                                  1
#define PCS_RX0_CLK_EN_SHIFT                                                 10
#define PCS_RX0_CLK_EN_MASK                                          0x00000400
#define PCS_RX0_CLK_EN_RD(src)                       (((src) & 0x00000400)>>10)
#define PCS_RX0_CLK_EN_WR(src)                  (((u32)(src)<<10) & 0x00000400)
#define PCS_RX0_CLK_EN_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields pcs_tx0_clk_en	 */
#define PCS_TX0_CLK_EN_WIDTH                                                  1
#define PCS_TX0_CLK_EN_SHIFT                                                  9
#define PCS_TX0_CLK_EN_MASK                                          0x00000200
#define PCS_TX0_CLK_EN_RD(src)                        (((src) & 0x00000200)>>9)
#define PCS_TX0_CLK_EN_WR(src)                   (((u32)(src)<<9) & 0x00000200)
#define PCS_TX0_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields mac_pcs_rx_clk_en	 */
#define MAC_PCS_RX_CLK_EN_WIDTH                                               1
#define MAC_PCS_RX_CLK_EN_SHIFT                                               8
#define MAC_PCS_RX_CLK_EN_MASK                                       0x00000100
#define MAC_PCS_RX_CLK_EN_RD(src)                     (((src) & 0x00000100)>>8)
#define MAC_PCS_RX_CLK_EN_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define MAC_PCS_RX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields mac_pcs_sata_rx_clk_en	 */
#define MAC_PCS_SATA_RX_CLK_EN_WIDTH                                          1
#define MAC_PCS_SATA_RX_CLK_EN_SHIFT                                          7
#define MAC_PCS_SATA_RX_CLK_EN_MASK                                  0x00000080
#define MAC_PCS_SATA_RX_CLK_EN_RD(src)                (((src) & 0x00000080)>>7)
#define MAC_PCS_SATA_RX_CLK_EN_WR(src)           (((u32)(src)<<7) & 0x00000080)
#define MAC_PCS_SATA_RX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields mac_pcs_pcie_tx_clk_en	 */
#define MAC_PCS_PCIE_TX_CLK_EN_WIDTH                                          1
#define MAC_PCS_PCIE_TX_CLK_EN_SHIFT                                          6
#define MAC_PCS_PCIE_TX_CLK_EN_MASK                                  0x00000040
#define MAC_PCS_PCIE_TX_CLK_EN_RD(src)                (((src) & 0x00000040)>>6)
#define MAC_PCS_PCIE_TX_CLK_EN_WR(src)           (((u32)(src)<<6) & 0x00000040)
#define MAC_PCS_PCIE_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields mac_pcs_sata_tx_clk_en	 */
#define MAC_PCS_SATA_TX_CLK_EN_WIDTH                                          1
#define MAC_PCS_SATA_TX_CLK_EN_SHIFT                                          5
#define MAC_PCS_SATA_TX_CLK_EN_MASK                                  0x00000020
#define MAC_PCS_SATA_TX_CLK_EN_RD(src)                (((src) & 0x00000020)>>5)
#define MAC_PCS_SATA_TX_CLK_EN_WR(src)           (((u32)(src)<<5) & 0x00000020)
#define MAC_PCS_SATA_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields mac_pcs_tx_clk_en	 */
#define MAC_PCS_TX_CLK_EN_WIDTH                                               1
#define MAC_PCS_TX_CLK_EN_SHIFT                                               4
#define MAC_PCS_TX_CLK_EN_MASK                                       0x00000010
#define MAC_PCS_TX_CLK_EN_RD(src)                     (((src) & 0x00000010)>>4)
#define MAC_PCS_TX_CLK_EN_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define MAC_PCS_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields sds_pcs_pll_pcs_clk_en	 */
#define SDS_PCS_PLL_PCS_CLK_EN_WIDTH                                          1
#define SDS_PCS_PLL_PCS_CLK_EN_SHIFT                                          3
#define SDS_PCS_PLL_PCS_CLK_EN_MASK                                  0x00000008
#define SDS_PCS_PLL_PCS_CLK_EN_RD(src)                (((src) & 0x00000008)>>3)
#define SDS_PCS_PLL_PCS_CLK_EN_WR(src)           (((u32)(src)<<3) & 0x00000008)
#define SDS_PCS_PLL_PCS_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields sds_pcs_refclk_in_en	 */
#define SDS_PCS_REFCLK_IN_EN_WIDTH                                            1
#define SDS_PCS_REFCLK_IN_EN_SHIFT                                            2
#define SDS_PCS_REFCLK_IN_EN_MASK                                    0x00000004
#define SDS_PCS_REFCLK_IN_EN_RD(src)                  (((src) & 0x00000004)>>2)
#define SDS_PCS_REFCLK_IN_EN_WR(src)             (((u32)(src)<<2) & 0x00000004)
#define SDS_PCS_REFCLK_IN_EN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields mgmt_clk_en	 */
#define MGMT_CLK_EN_WIDTH                                                     1
#define MGMT_CLK_EN_SHIFT                                                     1
#define MGMT_CLK_EN_MASK                                             0x00000002
#define MGMT_CLK_EN_RD(src)                           (((src) & 0x00000002)>>1)
#define MGMT_CLK_EN_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define MGMT_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields clk_pmalive_clk_en	 */
#define CLK_PMALIVE_CLK_EN_WIDTH                                              1
#define CLK_PMALIVE_CLK_EN_SHIFT                                              0
#define CLK_PMALIVE_CLK_EN_MASK                                      0x00000001
#define CLK_PMALIVE_CLK_EN_RD(src)                       (((src) & 0x00000001))
#define CLK_PMALIVE_CLK_EN_WR(src)                  (((u32)(src)) & 0x00000001)
#define CLK_PMALIVE_CLK_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl2	*/ 
/*	 Fields PcsCsr_Ctl2	 */
#define PCSCSR_CTL22_WIDTH                                                   32
#define PCSCSR_CTL22_SHIFT                                                    0
#define PCSCSR_CTL22_MASK                                            0xffffffff
#define PCSCSR_CTL22_RD(src)                             (((src) & 0xffffffff))
#define PCSCSR_CTL22_WR(src)                        (((u32)(src)) & 0xffffffff)
#define PCSCSR_CTL22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PcsCsr_Ctl3	*/ 
/*	 Fields PcsCsr_Ctl3	 */
#define PCSCSR_CTL33_WIDTH                                                   32
#define PCSCSR_CTL33_SHIFT                                                    0
#define PCSCSR_CTL33_MASK                                            0xffffffff
#define PCSCSR_CTL33_RD(src)                             (((src) & 0xffffffff))
#define PCSCSR_CTL33_WR(src)                        (((u32)(src)) & 0xffffffff)
#define PCSCSR_CTL33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Sata_IoCoh_Start0	*/ 
/*	 Fields Addr_High	 */
#define ADDR_HIGH0_WIDTH                                                     32
#define ADDR_HIGH0_SHIFT                                                      0
#define ADDR_HIGH0_MASK                                              0xffffffff
#define ADDR_HIGH0_RD(src)                               (((src) & 0xffffffff))
#define ADDR_HIGH0_WR(src)                          (((u32)(src)) & 0xffffffff)
#define ADDR_HIGH0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Sata_IoCoh_Start1	*/ 
/*	 Fields Addr_Low	 */
#define ADDR_LOW1_WIDTH                                                      32
#define ADDR_LOW1_SHIFT                                                       0
#define ADDR_LOW1_MASK                                               0xffffffff
#define ADDR_LOW1_RD(src)                                (((src) & 0xffffffff))
#define ADDR_LOW1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define ADDR_LOW1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Sata_IoCoh_End0	*/ 
/*	 Fields Addr_High	 */
#define ADDR_HIGH0_F1_WIDTH                                                  32
#define ADDR_HIGH0_F1_SHIFT                                                   0
#define ADDR_HIGH0_F1_MASK                                           0xffffffff
#define ADDR_HIGH0_F1_RD(src)                            (((src) & 0xffffffff))
#define ADDR_HIGH0_F1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define ADDR_HIGH0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Sata_IoCoh_End1	*/ 
/*	 Fields Addr_Low	 */
#define ADDR_LOW1_F1_WIDTH                                                   32
#define ADDR_LOW1_F1_SHIFT                                                    0
#define ADDR_LOW1_F1_MASK                                            0xffffffff
#define ADDR_LOW1_F1_RD(src)                             (((src) & 0xffffffff))
#define ADDR_LOW1_F1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define ADDR_LOW1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))
		
/*	Global Base Address	*/
#define PCIE_4X_PCS_SDS_CSR_REGS3_BASE_ADDR			0xddd8f4000ULL

/*    Address PCIE_4X_PCS_SDS_CSR_REGS3  Registers */
#define X4_PCSCSR_COMMONCTL0_ADDR                                       0x00000000
#define X4_PCSCSR_COMMONCTL0_DEFAULT                                    0x0053e131
#define X4_PCSCSR_COMMONCTL1_ADDR                                       0x00000004
#define X4_PCSCSR_COMMONCTL1_DEFAULT                                    0x0012aa31
#define X4_PCSCSR_STATUS0_ADDR                                          0x00000008
#define X4_PCSCSR_STATUS0_DEFAULT                                       0x00000000
#define X4_PCSCSR_STATUS1_ADDR                                          0x0000000c
#define X4_PCSCSR_STATUS1_DEFAULT                                       0x0000000f
#define X4_PCSCSR_REGADDR_ADDR                                          0x00000010
#define X4_PCSCSR_REGADDR_DEFAULT                                       0x00000000
#define X4_PCSCSR_REGWRDATA_ADDR                                        0x00000014
#define X4_PCSCSR_REGWRDATA_DEFAULT                                     0x00000000
#define X4_PCSCSR_REGCMD_ADDR                                           0x00000018
#define X4_PCSCSR_REGCMD_DEFAULT                                        0x00000000
#define X4_PCSCSR_REGCMDDONE_ADDR                                       0x0000001c
#define X4_PCSCSR_REGCMDDONE_DEFAULT                                    0x00000000
#define X4_PCSCSR_REGRDDATA_ADDR                                        0x00000020
#define X4_PCSCSR_REGRDDATA_DEFAULT                                     0x00000000
#define X4_PCSCSR_INTERRUPT0_ADDR                                       0x00000024
#define X4_PCSCSR_INTERRUPT0_DEFAULT                                    0x00000000
#define X4_PCSCSR_INTERRUPT0MASK_ADDR                                   0x00000028
#define X4_PCSCSR_INTERRUPT1_ADDR                                       0x0000002c
#define X4_PCSCSR_INTERRUPT1_DEFAULT                                    0x00000000
#define X4_PCSCSR_INTERRUPT1MASK_ADDR                                   0x00000030
#define X4_PCSCSR_INTERRUPT2_ADDR                                       0x00000034
#define X4_PCSCSR_INTERRUPT2_DEFAULT                                    0x00000000
#define X4_PCSCSR_INTERRUPT2MASK_ADDR                                   0x00000038
#define X4_PCSCSR_INTERRUPT3_ADDR                                       0x0000003c
#define X4_PCSCSR_INTERRUPT3_DEFAULT                                    0x00000000
#define X4_PCSCSR_INTERRUPT3MASK_ADDR                                   0x00000040
#define X4_PCSCSR_CLKCTL_ADDR                                           0x00000044
#define X4_PCSCSR_CLKCTL_DEFAULT                                        0x000007ff
#define X4_PCSCSR_CTL0_ADDR                                             0x00000048
#define X4_PCSCSR_CTL0_DEFAULT                                          0x0000009a
#define X4_PCSCSR_CTL1_ADDR                                             0x0000004c
#define X4_PCSCSR_CTL1_DEFAULT                                          0x0000009a
#define X4_PCSCSR_CTL2_ADDR                                             0x00000050
#define X4_PCSCSR_CTL2_DEFAULT                                          0x0000009a
#define X4_PCSCSR_CTL3_ADDR                                             0x00000054
#define X4_PCSCSR_CTL3_DEFAULT                                          0x0000009a
#define X4_PCSCSR_TEST_ADDR                                             0x00000058
#define X4_PCSCSR_TEST_DEFAULT                                          0x00000000

/*	Register PcsCsr_CommonCtl0	*/ 
/*	 Fields noise_clk	 */
#define X4_NOISE_CLK0_WIDTH                                                      1
#define X4_NOISE_CLK0_SHIFT                                                     25
#define X4_NOISE_CLK0_MASK                                              0x02000000
#define X4_NOISE_CLK0_RD(src)                           (((src) & 0x02000000)>>25)
#define X4_NOISE_CLK0_WR(src)                      (((u32)(src)<<25) & 0x02000000)
#define X4_NOISE_CLK0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields noise_src	 */
#define X4_NOISE_SRC0_WIDTH                                                      1
#define X4_NOISE_SRC0_SHIFT                                                     24
#define X4_NOISE_SRC0_MASK                                              0x01000000
#define X4_NOISE_SRC0_RD(src)                           (((src) & 0x01000000)>>24)
#define X4_NOISE_SRC0_WR(src)                      (((u32)(src)<<24) & 0x01000000)
#define X4_NOISE_SRC0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields init_ctlifc_use_mgmt	 */
#define X4_INIT_CTLIFC_USE_MGMT0_WIDTH                                           1
#define X4_INIT_CTLIFC_USE_MGMT0_SHIFT                                          23
#define X4_INIT_CTLIFC_USE_MGMT0_MASK                                   0x00800000
#define X4_INIT_CTLIFC_USE_MGMT0_RD(src)                (((src) & 0x00800000)>>23)
#define X4_INIT_CTLIFC_USE_MGMT0_WR(src)           (((u32)(src)<<23) & 0x00800000)
#define X4_INIT_CTLIFC_USE_MGMT0_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields init_ctlifc_pipe_compliant	 */
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_WIDTH                                     1
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_SHIFT                                    22
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_MASK                             0x00400000
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_RD(src)          (((src) & 0x00400000)>>22)
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_WR(src)     (((u32)(src)<<22) & 0x00400000)
#define X4_INIT_CTLIFC_PIPE_COMPLIANT0_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields init_refclk_ac_mode	 */
#define X4_INIT_REFCLK_AC_MODE0_WIDTH                                            1
#define X4_INIT_REFCLK_AC_MODE0_SHIFT                                           21
#define X4_INIT_REFCLK_AC_MODE0_MASK                                    0x00200000
#define X4_INIT_REFCLK_AC_MODE0_RD(src)                 (((src) & 0x00200000)>>21)
#define X4_INIT_REFCLK_AC_MODE0_WR(src)            (((u32)(src)<<21) & 0x00200000)
#define X4_INIT_REFCLK_AC_MODE0_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields mac_pcs_x4_mode	 */
#define X4_MAC_PCS_X4_MODE0_WIDTH                                                1
#define X4_MAC_PCS_X4_MODE0_SHIFT                                               20
#define X4_MAC_PCS_X4_MODE0_MASK                                        0x00100000
#define X4_MAC_PCS_X4_MODE0_RD(src)                     (((src) & 0x00100000)>>20)
#define X4_MAC_PCS_X4_MODE0_WR(src)                (((u32)(src)<<20) & 0x00100000)
#define X4_MAC_PCS_X4_MODE0_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields mac_pcs_23_x2_mode	 */
#define X4_MAC_PCS_23_X2_MODE0_WIDTH                                             1
#define X4_MAC_PCS_23_X2_MODE0_SHIFT                                            19
#define X4_MAC_PCS_23_X2_MODE0_MASK                                     0x00080000
#define X4_MAC_PCS_23_X2_MODE0_RD(src)                  (((src) & 0x00080000)>>19)
#define X4_MAC_PCS_23_X2_MODE0_WR(src)             (((u32)(src)<<19) & 0x00080000)
#define X4_MAC_PCS_23_X2_MODE0_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields mac_pcs_01_x2_mode	 */
#define X4_MAC_PCS_01_X2_MODE0_WIDTH                                             1
#define X4_MAC_PCS_01_X2_MODE0_SHIFT                                            18
#define X4_MAC_PCS_01_X2_MODE0_MASK                                     0x00040000
#define X4_MAC_PCS_01_X2_MODE0_RD(src)                  (((src) & 0x00040000)>>18)
#define X4_MAC_PCS_01_X2_MODE0_WR(src)             (((u32)(src)<<18) & 0x00040000)
#define X4_MAC_PCS_01_X2_MODE0_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields init_tx_term_sel	 */
#define X4_INIT_TX_TERM_SEL0_WIDTH                                               1
#define X4_INIT_TX_TERM_SEL0_SHIFT                                              17
#define X4_INIT_TX_TERM_SEL0_MASK                                       0x00020000
#define X4_INIT_TX_TERM_SEL0_RD(src)                    (((src) & 0x00020000)>>17)
#define X4_INIT_TX_TERM_SEL0_WR(src)               (((u32)(src)<<17) & 0x00020000)
#define X4_INIT_TX_TERM_SEL0_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields phy_tx_term_cal_in	 */
#define X4_PHY_TX_TERM_CAL_IN0_WIDTH                                             4
#define X4_PHY_TX_TERM_CAL_IN0_SHIFT                                            13
#define X4_PHY_TX_TERM_CAL_IN0_MASK                                     0x0001e000
#define X4_PHY_TX_TERM_CAL_IN0_RD(src)                  (((src) & 0x0001e000)>>13)
#define X4_PHY_TX_TERM_CAL_IN0_WR(src)             (((u32)(src)<<13) & 0x0001e000)
#define X4_PHY_TX_TERM_CAL_IN0_SET(dst,src) \
                      (((dst) & ~0x0001e000) | (((u32)(src)<<13) & 0x0001e000))
/*	 Fields mac_pcs_pll_pcs_div	 */
#define X4_MAC_PCS_PLL_PCS_DIV0_WIDTH                                            9
#define X4_MAC_PCS_PLL_PCS_DIV0_SHIFT                                            4
#define X4_MAC_PCS_PLL_PCS_DIV0_MASK                                    0x00001ff0
#define X4_MAC_PCS_PLL_PCS_DIV0_RD(src)                  (((src) & 0x00001ff0)>>4)
#define X4_MAC_PCS_PLL_PCS_DIV0_WR(src)             (((u32)(src)<<4) & 0x00001ff0)
#define X4_MAC_PCS_PLL_PCS_DIV0_SET(dst,src) \
                       (((dst) & ~0x00001ff0) | (((u32)(src)<<4) & 0x00001ff0))
/*	 Fields mac_pcs_pll_refclk_sel	 */
#define X4_MAC_PCS_PLL_REFCLK_SEL0_WIDTH                                         3
#define X4_MAC_PCS_PLL_REFCLK_SEL0_SHIFT                                         1
#define X4_MAC_PCS_PLL_REFCLK_SEL0_MASK                                 0x0000000e
#define X4_MAC_PCS_PLL_REFCLK_SEL0_RD(src)               (((src) & 0x0000000e)>>1)
#define X4_MAC_PCS_PLL_REFCLK_SEL0_WR(src)          (((u32)(src)<<1) & 0x0000000e)
#define X4_MAC_PCS_PLL_REFCLK_SEL0_SET(dst,src) \
                       (((dst) & ~0x0000000e) | (((u32)(src)<<1) & 0x0000000e))
/*	 Fields reset	 */
#define X4_RESET0_F2_WIDTH                                                       1
#define X4_RESET0_F2_SHIFT                                                       0
#define X4_RESET0_F2_MASK                                               0x00000001
#define X4_RESET0_F2_RD(src)                                (((src) & 0x00000001))
#define X4_RESET0_F2_WR(src)                           (((u32)(src)) & 0x00000001)
#define X4_RESET0_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_CommonCtl1	*/ 
/*	 Fields mac_pcs_pll_op_range	 */
#define X4_MAC_PCS_PLL_OP_RANGE1_WIDTH                                           3
#define X4_MAC_PCS_PLL_OP_RANGE1_SHIFT                                          21
#define X4_MAC_PCS_PLL_OP_RANGE1_MASK                                   0x00e00000
#define X4_MAC_PCS_PLL_OP_RANGE1_RD(src)                (((src) & 0x00e00000)>>21)
#define X4_MAC_PCS_PLL_OP_RANGE1_WR(src)           (((u32)(src)<<21) & 0x00e00000)
#define X4_MAC_PCS_PLL_OP_RANGE1_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields mac_pcs_pcie0_width	 */
#define X4_MAC_PCS_PCIE0_WIDTH1_WIDTH                                            1
#define X4_MAC_PCS_PCIE0_WIDTH1_SHIFT                                           20
#define X4_MAC_PCS_PCIE0_WIDTH1_MASK                                    0x00100000
#define X4_MAC_PCS_PCIE0_WIDTH1_RD(src)                 (((src) & 0x00100000)>>20)
#define X4_MAC_PCS_PCIE0_WIDTH1_WR(src)            (((u32)(src)<<20) & 0x00100000)
#define X4_MAC_PCS_PCIE0_WIDTH1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields mac_pcs_ctl0_op_range	 */
#define X4_MAC_PCS_CTL0_OP_RANGE1_WIDTH                                          3
#define X4_MAC_PCS_CTL0_OP_RANGE1_SHIFT                                         17
#define X4_MAC_PCS_CTL0_OP_RANGE1_MASK                                  0x000e0000
#define X4_MAC_PCS_CTL0_OP_RANGE1_RD(src)               (((src) & 0x000e0000)>>17)
#define X4_MAC_PCS_CTL0_OP_RANGE1_WR(src)          (((u32)(src)<<17) & 0x000e0000)
#define X4_MAC_PCS_CTL0_OP_RANGE1_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields mac_pcs_ctl0_rx_mode	 */
#define X4_MAC_PCS_CTL0_RX_MODE1_WIDTH                                           4
#define X4_MAC_PCS_CTL0_RX_MODE1_SHIFT                                          13
#define X4_MAC_PCS_CTL0_RX_MODE1_MASK                                   0x0001e000
#define X4_MAC_PCS_CTL0_RX_MODE1_RD(src)                (((src) & 0x0001e000)>>13)
#define X4_MAC_PCS_CTL0_RX_MODE1_WR(src)           (((u32)(src)<<13) & 0x0001e000)
#define X4_MAC_PCS_CTL0_RX_MODE1_SET(dst,src) \
                      (((dst) & ~0x0001e000) | (((u32)(src)<<13) & 0x0001e000))
/*	 Fields mac_pcs_ctl0_tx_mode	 */
#define X4_MAC_PCS_CTL0_TX_MODE1_WIDTH                                           4
#define X4_MAC_PCS_CTL0_TX_MODE1_SHIFT                                           9
#define X4_MAC_PCS_CTL0_TX_MODE1_MASK                                   0x00001e00
#define X4_MAC_PCS_CTL0_TX_MODE1_RD(src)                 (((src) & 0x00001e00)>>9)
#define X4_MAC_PCS_CTL0_TX_MODE1_WR(src)            (((u32)(src)<<9) & 0x00001e00)
#define X4_MAC_PCS_CTL0_TX_MODE1_SET(dst,src) \
                       (((dst) & ~0x00001e00) | (((u32)(src)<<9) & 0x00001e00))
/*	 Fields mac_pcs_pll_div	 */
#define X4_MAC_PCS_PLL_DIV1_WIDTH                                                9
#define X4_MAC_PCS_PLL_DIV1_SHIFT                                                0
#define X4_MAC_PCS_PLL_DIV1_MASK                                        0x000001ff
#define X4_MAC_PCS_PLL_DIV1_RD(src)                         (((src) & 0x000001ff))
#define X4_MAC_PCS_PLL_DIV1_WR(src)                    (((u32)(src)) & 0x000001ff)
#define X4_MAC_PCS_PLL_DIV1_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register PcsCsr_Status0	*/ 
/*	 Fields pcs_mac_rx3_ctrl	 */
#define X4_PCS_MAC_RX3_CTRL0_WIDTH                                               6
#define X4_PCS_MAC_RX3_CTRL0_SHIFT                                              18
#define X4_PCS_MAC_RX3_CTRL0_MASK                                       0x00fc0000
#define X4_PCS_MAC_RX3_CTRL0_RD(src)                    (((src) & 0x00fc0000)>>18)
#define X4_PCS_MAC_RX3_CTRL0_SET(dst,src) \
                      (((dst) & ~0x00fc0000) | (((u32)(src)<<18) & 0x00fc0000))
/*	 Fields pcs_mac_rx2_ctrl	 */
#define X4_PCS_MAC_RX2_CTRL0_WIDTH                                               6
#define X4_PCS_MAC_RX2_CTRL0_SHIFT                                              12
#define X4_PCS_MAC_RX2_CTRL0_MASK                                       0x0003f000
#define X4_PCS_MAC_RX2_CTRL0_RD(src)                    (((src) & 0x0003f000)>>12)
#define X4_PCS_MAC_RX2_CTRL0_SET(dst,src) \
                      (((dst) & ~0x0003f000) | (((u32)(src)<<12) & 0x0003f000))
/*	 Fields pcs_mac_rx1_ctrl	 */
#define X4_PCS_MAC_RX1_CTRL0_WIDTH                                               6
#define X4_PCS_MAC_RX1_CTRL0_SHIFT                                               6
#define X4_PCS_MAC_RX1_CTRL0_MASK                                       0x00000fc0
#define X4_PCS_MAC_RX1_CTRL0_RD(src)                     (((src) & 0x00000fc0)>>6)
#define X4_PCS_MAC_RX1_CTRL0_SET(dst,src) \
                       (((dst) & ~0x00000fc0) | (((u32)(src)<<6) & 0x00000fc0))
/*	 Fields pcs_mac_rx0_ctrl	 */
#define X4_PCS_MAC_RX0_CTRL0_WIDTH                                               6
#define X4_PCS_MAC_RX0_CTRL0_SHIFT                                               0
#define X4_PCS_MAC_RX0_CTRL0_MASK                                       0x0000003f
#define X4_PCS_MAC_RX0_CTRL0_RD(src)                        (((src) & 0x0000003f))
#define X4_PCS_MAC_RX0_CTRL0_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register PcsCsr_Status1	*/ 
/*	 Fields sds_pcs_rx0_cdr_status	 */
#define X4_SDS_PCS_RX0_CDR_STATUS1_WIDTH                                        16
#define X4_SDS_PCS_RX0_CDR_STATUS1_SHIFT                                        12
#define X4_SDS_PCS_RX0_CDR_STATUS1_MASK                                 0x0ffff000
#define X4_SDS_PCS_RX0_CDR_STATUS1_RD(src)              (((src) & 0x0ffff000)>>12)
#define X4_SDS_PCS_RX0_CDR_STATUS1_SET(dst,src) \
                      (((dst) & ~0x0ffff000) | (((u32)(src)<<12) & 0x0ffff000))
/*	 Fields sds_pcs_clock_ready	 */
#define X4_SDS_PCS_CLOCK_READY1_WIDTH                                            1
#define X4_SDS_PCS_CLOCK_READY1_SHIFT                                           11
#define X4_SDS_PCS_CLOCK_READY1_MASK                                    0x00000800
#define X4_SDS_PCS_CLOCK_READY1_RD(src)                 (((src) & 0x00000800)>>11)
#define X4_SDS_PCS_CLOCK_READY1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields sds_pcs_pll_lock	 */
#define X4_SDS_PCS_PLL_LOCK1_WIDTH                                               1
#define X4_SDS_PCS_PLL_LOCK1_SHIFT                                              10
#define X4_SDS_PCS_PLL_LOCK1_MASK                                       0x00000400
#define X4_SDS_PCS_PLL_LOCK1_RD(src)                    (((src) & 0x00000400)>>10)
#define X4_SDS_PCS_PLL_LOCK1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields phy_tx_term_cal_out	 */
#define X4_PHY_TX_TERM_CAL_OUT1_WIDTH                                            4
#define X4_PHY_TX_TERM_CAL_OUT1_SHIFT                                            0
#define X4_PHY_TX_TERM_CAL_OUT1_MASK                                    0x0000000f
#define X4_PHY_TX_TERM_CAL_OUT1_RD(src)                     (((src) & 0x0000000f))
#define X4_PHY_TX_TERM_CAL_OUT1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PcsCsr_RegAddr	*/ 
/*	 Fields mgmt_pcs_mmd_addr	 */
#define X4_MGMT_PCS_MMD_ADDR_WIDTH                                               5
#define X4_MGMT_PCS_MMD_ADDR_SHIFT                                              20
#define X4_MGMT_PCS_MMD_ADDR_MASK                                       0x01f00000
#define X4_MGMT_PCS_MMD_ADDR_RD(src)                    (((src) & 0x01f00000)>>20)
#define X4_MGMT_PCS_MMD_ADDR_WR(src)               (((u32)(src)<<20) & 0x01f00000)
#define X4_MGMT_PCS_MMD_ADDR_SET(dst,src) \
                      (((dst) & ~0x01f00000) | (((u32)(src)<<20) & 0x01f00000))
/*	 Fields mgmt_pcs_lane_sel	 */
#define X4_MGMT_PCS_LANE_SEL_WIDTH                                               4
#define X4_MGMT_PCS_LANE_SEL_SHIFT                                              16
#define X4_MGMT_PCS_LANE_SEL_MASK                                       0x000f0000
#define X4_MGMT_PCS_LANE_SEL_RD(src)                    (((src) & 0x000f0000)>>16)
#define X4_MGMT_PCS_LANE_SEL_WR(src)               (((u32)(src)<<16) & 0x000f0000)
#define X4_MGMT_PCS_LANE_SEL_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))
/*	 Fields mgmt_pcs_reg_addr	 */
#define X4_MGMT_PCS_REG_ADDR_WIDTH                                              16
#define X4_MGMT_PCS_REG_ADDR_SHIFT                                               0
#define X4_MGMT_PCS_REG_ADDR_MASK                                       0x0000ffff
#define X4_MGMT_PCS_REG_ADDR_RD(src)                        (((src) & 0x0000ffff))
#define X4_MGMT_PCS_REG_ADDR_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define X4_MGMT_PCS_REG_ADDR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_RegWrdata	*/ 
/*	 Fields mgmt_pcs_wr_data	 */
#define X4_MGMT_PCS_WR_DATA_WIDTH                                               16
#define X4_MGMT_PCS_WR_DATA_SHIFT                                                0
#define X4_MGMT_PCS_WR_DATA_MASK                                        0x0000ffff
#define X4_MGMT_PCS_WR_DATA_RD(src)                         (((src) & 0x0000ffff))
#define X4_MGMT_PCS_WR_DATA_WR(src)                    (((u32)(src)) & 0x0000ffff)
#define X4_MGMT_PCS_WR_DATA_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_RegCmd	*/ 
/*	 Fields mgmt_pcs_reg_wr	 */
#define X4_MGMT_PCS_REG_WR_WIDTH                                                 1
#define X4_MGMT_PCS_REG_WR_SHIFT                                                 1
#define X4_MGMT_PCS_REG_WR_MASK                                         0x00000002
#define X4_MGMT_PCS_REG_WR_RD(src)                       (((src) & 0x00000002)>>1)
#define X4_MGMT_PCS_REG_WR_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define X4_MGMT_PCS_REG_WR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mgmt_pcs_reg_rd	 */
#define X4_MGMT_PCS_REG_RD_WIDTH                                                 1
#define X4_MGMT_PCS_REG_RD_SHIFT                                                 0
#define X4_MGMT_PCS_REG_RD_MASK                                         0x00000001
#define X4_MGMT_PCS_REG_RD_RD(src)                          (((src) & 0x00000001))
#define X4_MGMT_PCS_REG_RD_WR(src)                     (((u32)(src)) & 0x00000001)
#define X4_MGMT_PCS_REG_RD_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_RegCmdDone	*/ 
/*	 Fields mgmt_pcs_reg_wr_done	 */
#define X4_MGMT_PCS_REG_WR_DONE_WIDTH                                            1
#define X4_MGMT_PCS_REG_WR_DONE_SHIFT                                            1
#define X4_MGMT_PCS_REG_WR_DONE_MASK                                    0x00000002
#define X4_MGMT_PCS_REG_WR_DONE_RD(src)                  (((src) & 0x00000002)>>1)
#define X4_MGMT_PCS_REG_WR_DONE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mgmt_pcs_reg_rd_done	 */
#define X4_MGMT_PCS_REG_RD_DONE_WIDTH                                            1
#define X4_MGMT_PCS_REG_RD_DONE_SHIFT                                            0
#define X4_MGMT_PCS_REG_RD_DONE_MASK                                    0x00000001
#define X4_MGMT_PCS_REG_RD_DONE_RD(src)                     (((src) & 0x00000001))
#define X4_MGMT_PCS_REG_RD_DONE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_RegRddata	*/ 
/*	 Fields pcs_mgmt_rd_data	 */
#define X4_PCS_MGMT_RD_DATA_WIDTH                                               16
#define X4_PCS_MGMT_RD_DATA_SHIFT                                                0
#define X4_PCS_MGMT_RD_DATA_MASK                                        0x0000ffff
#define X4_PCS_MGMT_RD_DATA_RD(src)                         (((src) & 0x0000ffff))
#define X4_PCS_MGMT_RD_DATA_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PcsCsr_INT0	*/ 
/*	 Fields pcs_mac_ctl0_rx_ack	 */
#define X4_PCS_MAC_CTL0_RX_ACK0_WIDTH                                            1
#define X4_PCS_MAC_CTL0_RX_ACK0_SHIFT                                            5
#define X4_PCS_MAC_CTL0_RX_ACK0_MASK                                    0x00000020
#define X4_PCS_MAC_CTL0_RX_ACK0_RD(src)                  (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL0_RX_ACK0_WR(src)             (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL0_RX_ACK0_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields pcs_mac_ctl0_ack	 */
#define X4_PCS_MAC_CTL0_ACK0_WIDTH                                               1
#define X4_PCS_MAC_CTL0_ACK0_SHIFT                                               4
#define X4_PCS_MAC_CTL0_ACK0_MASK                                       0x00000010
#define X4_PCS_MAC_CTL0_ACK0_RD(src)                     (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL0_ACK0_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL0_ACK0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pcs_mac_ctl0_rx_mgmt_ack	 */
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_WIDTH                                       1
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_SHIFT                                       3
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_MASK                               0x00000008
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_RD(src)             (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_WR(src)        (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL0_RX_MGMT_ACK0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields pcs_mac_ctl0_mgmt_ack	 */
#define X4_PCS_MAC_CTL0_MGMT_ACK0_WIDTH                                          1
#define X4_PCS_MAC_CTL0_MGMT_ACK0_SHIFT                                          2
#define X4_PCS_MAC_CTL0_MGMT_ACK0_MASK                                  0x00000004
#define X4_PCS_MAC_CTL0_MGMT_ACK0_RD(src)                (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL0_MGMT_ACK0_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL0_MGMT_ACK0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields pcs_mac_rx0_prbs_error	 */
#define X4_PCS_MAC_RX0_PRBS_ERROR0_WIDTH                                         1
#define X4_PCS_MAC_RX0_PRBS_ERROR0_SHIFT                                         1
#define X4_PCS_MAC_RX0_PRBS_ERROR0_MASK                                 0x00000002
#define X4_PCS_MAC_RX0_PRBS_ERROR0_RD(src)               (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX0_PRBS_ERROR0_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX0_PRBS_ERROR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcs_mac_rx0_8b10b_error	 */
#define X4_PCS_MAC_RX0_8B10B_ERROR0_WIDTH                                        1
#define X4_PCS_MAC_RX0_8B10B_ERROR0_SHIFT                                        0
#define X4_PCS_MAC_RX0_8B10B_ERROR0_MASK                                0x00000001
#define X4_PCS_MAC_RX0_8B10B_ERROR0_RD(src)                 (((src) & 0x00000001))
#define X4_PCS_MAC_RX0_8B10B_ERROR0_WR(src)            (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX0_8B10B_ERROR0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Interrupt0Mask	*/
/*    Mask Register Fields pcs_mac_ctl0_rx_ackMask    */
#define X4_PCS_MAC_CTL0_RX_ACKMASK_WIDTH                                         1
#define X4_PCS_MAC_CTL0_RX_ACKMASK_SHIFT                                         5
#define X4_PCS_MAC_CTL0_RX_ACKMASK_MASK                                 0x00000020
#define X4_PCS_MAC_CTL0_RX_ACKMASK_RD(src)               (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL0_RX_ACKMASK_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL0_RX_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields pcs_mac_ctl0_ackMask    */
#define X4_PCS_MAC_CTL0_ACKMASK_WIDTH                                            1
#define X4_PCS_MAC_CTL0_ACKMASK_SHIFT                                            4
#define X4_PCS_MAC_CTL0_ACKMASK_MASK                                    0x00000010
#define X4_PCS_MAC_CTL0_ACKMASK_RD(src)                  (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL0_ACKMASK_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL0_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields pcs_mac_ctl0_rx_mgmt_ackMask    */
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_WIDTH                                    1
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_SHIFT                                    3
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_MASK                            0x00000008
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_RD(src)          (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL0_RX_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields pcs_mac_ctl0_mgmt_ackMask    */
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_WIDTH                                       1
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_SHIFT                                       2
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_MASK                               0x00000004
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_RD(src)             (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL0_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields pcs_mac_rx0_prbs_errorMask    */
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_WIDTH                                      1
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_SHIFT                                      1
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_MASK                              0x00000002
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_RD(src)            (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX0_PRBS_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pcs_mac_rx0_8b10b_errorMask    */
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_WIDTH                                     1
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_SHIFT                                     0
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_MASK                             0x00000001
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_RD(src)              (((src) & 0x00000001))
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX0_8B10B_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_INT1	*/ 
/*	 Fields pcs_mac_ctl1_rx_ack	 */
#define X4_PCS_MAC_CTL1_RX_ACK1_WIDTH                                            1
#define X4_PCS_MAC_CTL1_RX_ACK1_SHIFT                                            5
#define X4_PCS_MAC_CTL1_RX_ACK1_MASK                                    0x00000020
#define X4_PCS_MAC_CTL1_RX_ACK1_RD(src)                  (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL1_RX_ACK1_WR(src)             (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL1_RX_ACK1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields pcs_mac_ctl1_ack	 */
#define X4_PCS_MAC_CTL1_ACK1_WIDTH                                               1
#define X4_PCS_MAC_CTL1_ACK1_SHIFT                                               4
#define X4_PCS_MAC_CTL1_ACK1_MASK                                       0x00000010
#define X4_PCS_MAC_CTL1_ACK1_RD(src)                     (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL1_ACK1_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL1_ACK1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pcs_mac_ctl1_rx_mgmt_ack	 */
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_WIDTH                                       1
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_SHIFT                                       3
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_MASK                               0x00000008
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_RD(src)             (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_WR(src)        (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL1_RX_MGMT_ACK1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields pcs_mac_ctl1_mgmt_ack	 */
#define X4_PCS_MAC_CTL1_MGMT_ACK1_WIDTH                                          1
#define X4_PCS_MAC_CTL1_MGMT_ACK1_SHIFT                                          2
#define X4_PCS_MAC_CTL1_MGMT_ACK1_MASK                                  0x00000004
#define X4_PCS_MAC_CTL1_MGMT_ACK1_RD(src)                (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL1_MGMT_ACK1_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL1_MGMT_ACK1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields pcs_mac_rx1_prbs_error	 */
#define X4_PCS_MAC_RX1_PRBS_ERROR1_WIDTH                                         1
#define X4_PCS_MAC_RX1_PRBS_ERROR1_SHIFT                                         1
#define X4_PCS_MAC_RX1_PRBS_ERROR1_MASK                                 0x00000002
#define X4_PCS_MAC_RX1_PRBS_ERROR1_RD(src)               (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX1_PRBS_ERROR1_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX1_PRBS_ERROR1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcs_mac_rx1_8b10b_error	 */
#define X4_PCS_MAC_RX1_8B10B_ERROR1_WIDTH                                        1
#define X4_PCS_MAC_RX1_8B10B_ERROR1_SHIFT                                        0
#define X4_PCS_MAC_RX1_8B10B_ERROR1_MASK                                0x00000001
#define X4_PCS_MAC_RX1_8B10B_ERROR1_RD(src)                 (((src) & 0x00000001))
#define X4_PCS_MAC_RX1_8B10B_ERROR1_WR(src)            (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX1_8B10B_ERROR1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Interrupt1Mask	*/
/*    Mask Register Fields pcs_mac_ctl1_rx_ackMask    */
#define X4_PCS_MAC_CTL1_RX_ACKMASK_WIDTH                                         1
#define X4_PCS_MAC_CTL1_RX_ACKMASK_SHIFT                                         5
#define X4_PCS_MAC_CTL1_RX_ACKMASK_MASK                                 0x00000020
#define X4_PCS_MAC_CTL1_RX_ACKMASK_RD(src)               (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL1_RX_ACKMASK_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL1_RX_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields pcs_mac_ctl1_ackMask    */
#define X4_PCS_MAC_CTL1_ACKMASK_WIDTH                                            1
#define X4_PCS_MAC_CTL1_ACKMASK_SHIFT                                            4
#define X4_PCS_MAC_CTL1_ACKMASK_MASK                                    0x00000010
#define X4_PCS_MAC_CTL1_ACKMASK_RD(src)                  (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL1_ACKMASK_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL1_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields pcs_mac_ctl1_rx_mgmt_ackMask    */
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_WIDTH                                    1
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_SHIFT                                    3
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_MASK                            0x00000008
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_RD(src)          (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL1_RX_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields pcs_mac_ctl1_mgmt_ackMask    */
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_WIDTH                                       1
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_SHIFT                                       2
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_MASK                               0x00000004
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_RD(src)             (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL1_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields pcs_mac_rx1_prbs_errorMask    */
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_WIDTH                                      1
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_SHIFT                                      1
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_MASK                              0x00000002
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_RD(src)            (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX1_PRBS_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pcs_mac_rx1_8b10b_errorMask    */
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_WIDTH                                     1
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_SHIFT                                     0
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_MASK                             0x00000001
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_RD(src)              (((src) & 0x00000001))
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX1_8B10B_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_INT2	*/ 
/*	 Fields pcs_mac_ctl2_rx_ack	 */
#define X4_PCS_MAC_CTL2_RX_ACK2_WIDTH                                            1
#define X4_PCS_MAC_CTL2_RX_ACK2_SHIFT                                            5
#define X4_PCS_MAC_CTL2_RX_ACK2_MASK                                    0x00000020
#define X4_PCS_MAC_CTL2_RX_ACK2_RD(src)                  (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL2_RX_ACK2_WR(src)             (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL2_RX_ACK2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields pcs_mac_ctl2_ack	 */
#define X4_PCS_MAC_CTL2_ACK2_WIDTH                                               1
#define X4_PCS_MAC_CTL2_ACK2_SHIFT                                               4
#define X4_PCS_MAC_CTL2_ACK2_MASK                                       0x00000010
#define X4_PCS_MAC_CTL2_ACK2_RD(src)                     (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL2_ACK2_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL2_ACK2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pcs_mac_ctl2_rx_mgmt_ack	 */
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_WIDTH                                       1
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_SHIFT                                       3
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_MASK                               0x00000008
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_RD(src)             (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_WR(src)        (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL2_RX_MGMT_ACK2_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields pcs_mac_ctl2_mgmt_ack	 */
#define X4_PCS_MAC_CTL2_MGMT_ACK2_WIDTH                                          1
#define X4_PCS_MAC_CTL2_MGMT_ACK2_SHIFT                                          2
#define X4_PCS_MAC_CTL2_MGMT_ACK2_MASK                                  0x00000004
#define X4_PCS_MAC_CTL2_MGMT_ACK2_RD(src)                (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL2_MGMT_ACK2_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL2_MGMT_ACK2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields pcs_mac_rx2_prbs_error	 */
#define X4_PCS_MAC_RX2_PRBS_ERROR2_WIDTH                                         1
#define X4_PCS_MAC_RX2_PRBS_ERROR2_SHIFT                                         1
#define X4_PCS_MAC_RX2_PRBS_ERROR2_MASK                                 0x00000002
#define X4_PCS_MAC_RX2_PRBS_ERROR2_RD(src)               (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX2_PRBS_ERROR2_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX2_PRBS_ERROR2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcs_mac_rx2_8b10b_error	 */
#define X4_PCS_MAC_RX2_8B10B_ERROR2_WIDTH                                        1
#define X4_PCS_MAC_RX2_8B10B_ERROR2_SHIFT                                        0
#define X4_PCS_MAC_RX2_8B10B_ERROR2_MASK                                0x00000001
#define X4_PCS_MAC_RX2_8B10B_ERROR2_RD(src)                 (((src) & 0x00000001))
#define X4_PCS_MAC_RX2_8B10B_ERROR2_WR(src)            (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX2_8B10B_ERROR2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Interrupt2Mask	*/
/*    Mask Register Fields pcs_mac_ctl2_rx_ackMask    */
#define X4_PCS_MAC_CTL2_RX_ACKMASK_WIDTH                                         1
#define X4_PCS_MAC_CTL2_RX_ACKMASK_SHIFT                                         5
#define X4_PCS_MAC_CTL2_RX_ACKMASK_MASK                                 0x00000020
#define X4_PCS_MAC_CTL2_RX_ACKMASK_RD(src)               (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL2_RX_ACKMASK_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL2_RX_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields pcs_mac_ctl2_ackMask    */
#define X4_PCS_MAC_CTL2_ACKMASK_WIDTH                                            1
#define X4_PCS_MAC_CTL2_ACKMASK_SHIFT                                            4
#define X4_PCS_MAC_CTL2_ACKMASK_MASK                                    0x00000010
#define X4_PCS_MAC_CTL2_ACKMASK_RD(src)                  (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL2_ACKMASK_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL2_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields pcs_mac_ctl2_rx_mgmt_ackMask    */
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_WIDTH                                    1
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_SHIFT                                    3
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_MASK                            0x00000008
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_RD(src)          (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL2_RX_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields pcs_mac_ctl2_mgmt_ackMask    */
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_WIDTH                                       1
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_SHIFT                                       2
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_MASK                               0x00000004
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_RD(src)             (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL2_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields pcs_mac_rx2_prbs_errorMask    */
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_WIDTH                                      1
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_SHIFT                                      1
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_MASK                              0x00000002
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_RD(src)            (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX2_PRBS_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pcs_mac_rx2_8b10b_errorMask    */
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_WIDTH                                     1
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_SHIFT                                     0
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_MASK                             0x00000001
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_RD(src)              (((src) & 0x00000001))
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX2_8B10B_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_INT3	*/ 
/*	 Fields pcs_mac_ctl3_rx_ack	 */
#define X4_PCS_MAC_CTL3_RX_ACK3_WIDTH                                            1
#define X4_PCS_MAC_CTL3_RX_ACK3_SHIFT                                            5
#define X4_PCS_MAC_CTL3_RX_ACK3_MASK                                    0x00000020
#define X4_PCS_MAC_CTL3_RX_ACK3_RD(src)                  (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL3_RX_ACK3_WR(src)             (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL3_RX_ACK3_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields pcs_mac_ctl3_ack	 */
#define X4_PCS_MAC_CTL3_ACK3_WIDTH                                               1
#define X4_PCS_MAC_CTL3_ACK3_SHIFT                                               4
#define X4_PCS_MAC_CTL3_ACK3_MASK                                       0x00000010
#define X4_PCS_MAC_CTL3_ACK3_RD(src)                     (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL3_ACK3_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL3_ACK3_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pcs_mac_ctl3_rx_mgmt_ack	 */
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_WIDTH                                       1
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_SHIFT                                       3
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_MASK                               0x00000008
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_RD(src)             (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_WR(src)        (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL3_RX_MGMT_ACK3_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields pcs_mac_ctl3_mgmt_ack	 */
#define X4_PCS_MAC_CTL3_MGMT_ACK3_WIDTH                                          1
#define X4_PCS_MAC_CTL3_MGMT_ACK3_SHIFT                                          2
#define X4_PCS_MAC_CTL3_MGMT_ACK3_MASK                                  0x00000004
#define X4_PCS_MAC_CTL3_MGMT_ACK3_RD(src)                (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL3_MGMT_ACK3_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL3_MGMT_ACK3_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields pcs_mac_rx3_prbs_error	 */
#define X4_PCS_MAC_RX3_PRBS_ERROR3_WIDTH                                         1
#define X4_PCS_MAC_RX3_PRBS_ERROR3_SHIFT                                         1
#define X4_PCS_MAC_RX3_PRBS_ERROR3_MASK                                 0x00000002
#define X4_PCS_MAC_RX3_PRBS_ERROR3_RD(src)               (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX3_PRBS_ERROR3_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX3_PRBS_ERROR3_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pcs_mac_rx3_8b10b_error	 */
#define X4_PCS_MAC_RX3_8B10B_ERROR3_WIDTH                                        1
#define X4_PCS_MAC_RX3_8B10B_ERROR3_SHIFT                                        0
#define X4_PCS_MAC_RX3_8B10B_ERROR3_MASK                                0x00000001
#define X4_PCS_MAC_RX3_8B10B_ERROR3_RD(src)                 (((src) & 0x00000001))
#define X4_PCS_MAC_RX3_8B10B_ERROR3_WR(src)            (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX3_8B10B_ERROR3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Interrupt3Mask	*/
/*    Mask Register Fields pcs_mac_ctl3_rx_ackMask    */
#define X4_PCS_MAC_CTL3_RX_ACKMASK_WIDTH                                         1
#define X4_PCS_MAC_CTL3_RX_ACKMASK_SHIFT                                         5
#define X4_PCS_MAC_CTL3_RX_ACKMASK_MASK                                 0x00000020
#define X4_PCS_MAC_CTL3_RX_ACKMASK_RD(src)               (((src) & 0x00000020)>>5)
#define X4_PCS_MAC_CTL3_RX_ACKMASK_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define X4_PCS_MAC_CTL3_RX_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields pcs_mac_ctl3_ackMask    */
#define X4_PCS_MAC_CTL3_ACKMASK_WIDTH                                            1
#define X4_PCS_MAC_CTL3_ACKMASK_SHIFT                                            4
#define X4_PCS_MAC_CTL3_ACKMASK_MASK                                    0x00000010
#define X4_PCS_MAC_CTL3_ACKMASK_RD(src)                  (((src) & 0x00000010)>>4)
#define X4_PCS_MAC_CTL3_ACKMASK_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define X4_PCS_MAC_CTL3_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields pcs_mac_ctl3_rx_mgmt_ackMask    */
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_WIDTH                                    1
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_SHIFT                                    3
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_MASK                            0x00000008
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_RD(src)          (((src) & 0x00000008)>>3)
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define X4_PCS_MAC_CTL3_RX_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields pcs_mac_ctl3_mgmt_ackMask    */
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_WIDTH                                       1
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_SHIFT                                       2
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_MASK                               0x00000004
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_RD(src)             (((src) & 0x00000004)>>2)
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define X4_PCS_MAC_CTL3_MGMT_ACKMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields pcs_mac_rx3_prbs_errorMask    */
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_WIDTH                                      1
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_SHIFT                                      1
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_MASK                              0x00000002
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_RD(src)            (((src) & 0x00000002)>>1)
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define X4_PCS_MAC_RX3_PRBS_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pcs_mac_rx3_8b10b_errorMask    */
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_WIDTH                                     1
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_SHIFT                                     0
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_MASK                             0x00000001
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_RD(src)              (((src) & 0x00000001))
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define X4_PCS_MAC_RX3_8B10B_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_ClkCtl	*/ 
/*	 Fields pcs_rx0_clk_en	 */
#define X4_PCS_RX0_CLK_EN_WIDTH                                                  1
#define X4_PCS_RX0_CLK_EN_SHIFT                                                 10
#define X4_PCS_RX0_CLK_EN_MASK                                          0x00000400
#define X4_PCS_RX0_CLK_EN_RD(src)                       (((src) & 0x00000400)>>10)
#define X4_PCS_RX0_CLK_EN_WR(src)                  (((u32)(src)<<10) & 0x00000400)
#define X4_PCS_RX0_CLK_EN_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields pcs_tx0_clk_en	 */
#define X4_PCS_TX0_CLK_EN_WIDTH                                                  1
#define X4_PCS_TX0_CLK_EN_SHIFT                                                  9
#define X4_PCS_TX0_CLK_EN_MASK                                          0x00000200
#define X4_PCS_TX0_CLK_EN_RD(src)                        (((src) & 0x00000200)>>9)
#define X4_PCS_TX0_CLK_EN_WR(src)                   (((u32)(src)<<9) & 0x00000200)
#define X4_PCS_TX0_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields mac_pcs_rx_clk_en	 */
#define X4_MAC_PCS_RX_CLK_EN_WIDTH                                               1
#define X4_MAC_PCS_RX_CLK_EN_SHIFT                                               8
#define X4_MAC_PCS_RX_CLK_EN_MASK                                       0x00000100
#define X4_MAC_PCS_RX_CLK_EN_RD(src)                     (((src) & 0x00000100)>>8)
#define X4_MAC_PCS_RX_CLK_EN_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define X4_MAC_PCS_RX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields mac_pcs_sata_rx_clk_en	 */
#define X4_MAC_PCS_SATA_RX_CLK_EN_WIDTH                                          1
#define X4_MAC_PCS_SATA_RX_CLK_EN_SHIFT                                          7
#define X4_MAC_PCS_SATA_RX_CLK_EN_MASK                                  0x00000080
#define X4_MAC_PCS_SATA_RX_CLK_EN_RD(src)                (((src) & 0x00000080)>>7)
#define X4_MAC_PCS_SATA_RX_CLK_EN_WR(src)           (((u32)(src)<<7) & 0x00000080)
#define X4_MAC_PCS_SATA_RX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields mac_pcs_pcie_tx_clk_en	 */
#define X4_MAC_PCS_PCIE_TX_CLK_EN_WIDTH                                          1
#define X4_MAC_PCS_PCIE_TX_CLK_EN_SHIFT                                          6
#define X4_MAC_PCS_PCIE_TX_CLK_EN_MASK                                  0x00000040
#define X4_MAC_PCS_PCIE_TX_CLK_EN_RD(src)                (((src) & 0x00000040)>>6)
#define X4_MAC_PCS_PCIE_TX_CLK_EN_WR(src)           (((u32)(src)<<6) & 0x00000040)
#define X4_MAC_PCS_PCIE_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields mac_pcs_sata_tx_clk_en	 */
#define X4_MAC_PCS_SATA_TX_CLK_EN_WIDTH                                          1
#define X4_MAC_PCS_SATA_TX_CLK_EN_SHIFT                                          5
#define X4_MAC_PCS_SATA_TX_CLK_EN_MASK                                  0x00000020
#define X4_MAC_PCS_SATA_TX_CLK_EN_RD(src)                (((src) & 0x00000020)>>5)
#define X4_MAC_PCS_SATA_TX_CLK_EN_WR(src)           (((u32)(src)<<5) & 0x00000020)
#define X4_MAC_PCS_SATA_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields mac_pcs_tx_clk_en	 */
#define X4_MAC_PCS_TX_CLK_EN_WIDTH                                               1
#define X4_MAC_PCS_TX_CLK_EN_SHIFT                                               4
#define X4_MAC_PCS_TX_CLK_EN_MASK                                       0x00000010
#define X4_MAC_PCS_TX_CLK_EN_RD(src)                     (((src) & 0x00000010)>>4)
#define X4_MAC_PCS_TX_CLK_EN_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define X4_MAC_PCS_TX_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields sds_pcs_pll_pcs_clk_en	 */
#define X4_SDS_PCS_PLL_PCS_CLK_EN_WIDTH                                          1
#define X4_SDS_PCS_PLL_PCS_CLK_EN_SHIFT                                          3
#define X4_SDS_PCS_PLL_PCS_CLK_EN_MASK                                  0x00000008
#define X4_SDS_PCS_PLL_PCS_CLK_EN_RD(src)                (((src) & 0x00000008)>>3)
#define X4_SDS_PCS_PLL_PCS_CLK_EN_WR(src)           (((u32)(src)<<3) & 0x00000008)
#define X4_SDS_PCS_PLL_PCS_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields sds_pcs_refclk_in_en	 */
#define X4_SDS_PCS_REFCLK_IN_EN_WIDTH                                            1
#define X4_SDS_PCS_REFCLK_IN_EN_SHIFT                                            2
#define X4_SDS_PCS_REFCLK_IN_EN_MASK                                    0x00000004
#define X4_SDS_PCS_REFCLK_IN_EN_RD(src)                  (((src) & 0x00000004)>>2)
#define X4_SDS_PCS_REFCLK_IN_EN_WR(src)             (((u32)(src)<<2) & 0x00000004)
#define X4_SDS_PCS_REFCLK_IN_EN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields mgmt_clk_en	 */
#define X4_MGMT_CLK_EN_WIDTH                                                     1
#define X4_MGMT_CLK_EN_SHIFT                                                     1
#define X4_MGMT_CLK_EN_MASK                                             0x00000002
#define X4_MGMT_CLK_EN_RD(src)                           (((src) & 0x00000002)>>1)
#define X4_MGMT_CLK_EN_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define X4_MGMT_CLK_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields clk_pmalive_clk_en	 */
#define X4_CLK_PMALIVE_CLK_EN_WIDTH                                              1
#define X4_CLK_PMALIVE_CLK_EN_SHIFT                                              0
#define X4_CLK_PMALIVE_CLK_EN_MASK                                      0x00000001
#define X4_CLK_PMALIVE_CLK_EN_RD(src)                       (((src) & 0x00000001))
#define X4_CLK_PMALIVE_CLK_EN_WR(src)                  (((u32)(src)) & 0x00000001)
#define X4_CLK_PMALIVE_CLK_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl0	*/ 
/*	 Fields mac_pcs_rx0_edet_lb	 */
#define X4_MAC_PCS_RX0_EDET_LB0_WIDTH                                            1
#define X4_MAC_PCS_RX0_EDET_LB0_SHIFT                                           13
#define X4_MAC_PCS_RX0_EDET_LB0_MASK                                    0x00002000
#define X4_MAC_PCS_RX0_EDET_LB0_RD(src)                 (((src) & 0x00002000)>>13)
#define X4_MAC_PCS_RX0_EDET_LB0_WR(src)            (((u32)(src)<<13) & 0x00002000)
#define X4_MAC_PCS_RX0_EDET_LB0_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields mac_pcs_rx0_edet_eie	 */
#define X4_MAC_PCS_RX0_EDET_EIE0_WIDTH                                           1
#define X4_MAC_PCS_RX0_EDET_EIE0_SHIFT                                          12
#define X4_MAC_PCS_RX0_EDET_EIE0_MASK                                   0x00001000
#define X4_MAC_PCS_RX0_EDET_EIE0_RD(src)                (((src) & 0x00001000)>>12)
#define X4_MAC_PCS_RX0_EDET_EIE0_WR(src)           (((u32)(src)<<12) & 0x00001000)
#define X4_MAC_PCS_RX0_EDET_EIE0_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields mac_pcs_rx0_edet_ts	 */
#define X4_MAC_PCS_RX0_EDET_TS0_WIDTH                                            1
#define X4_MAC_PCS_RX0_EDET_TS0_SHIFT                                           11
#define X4_MAC_PCS_RX0_EDET_TS0_MASK                                    0x00000800
#define X4_MAC_PCS_RX0_EDET_TS0_RD(src)                 (((src) & 0x00000800)>>11)
#define X4_MAC_PCS_RX0_EDET_TS0_WR(src)            (((u32)(src)<<11) & 0x00000800)
#define X4_MAC_PCS_RX0_EDET_TS0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields mac_pcs_rx0_edet_skp	 */
#define X4_MAC_PCS_RX0_EDET_SKP0_WIDTH                                           1
#define X4_MAC_PCS_RX0_EDET_SKP0_SHIFT                                          10
#define X4_MAC_PCS_RX0_EDET_SKP0_MASK                                   0x00000400
#define X4_MAC_PCS_RX0_EDET_SKP0_RD(src)                (((src) & 0x00000400)>>10)
#define X4_MAC_PCS_RX0_EDET_SKP0_WR(src)           (((u32)(src)<<10) & 0x00000400)
#define X4_MAC_PCS_RX0_EDET_SKP0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields mac_pcs_ctl0_smpl_rate	 */
#define X4_MAC_PCS_CTL0_SMPL_RATE0_WIDTH                                         3
#define X4_MAC_PCS_CTL0_SMPL_RATE0_SHIFT                                         7
#define X4_MAC_PCS_CTL0_SMPL_RATE0_MASK                                 0x00000380
#define X4_MAC_PCS_CTL0_SMPL_RATE0_RD(src)               (((src) & 0x00000380)>>7)
#define X4_MAC_PCS_CTL0_SMPL_RATE0_WR(src)          (((u32)(src)<<7) & 0x00000380)
#define X4_MAC_PCS_CTL0_SMPL_RATE0_SET(dst,src) \
                       (((dst) & ~0x00000380) | (((u32)(src)<<7) & 0x00000380))
/*	 Fields mac_pcs_ctl0_tx_lane_div	 */
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_WIDTH                                       2
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_SHIFT                                       5
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_MASK                               0x00000060
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_RD(src)             (((src) & 0x00000060)>>5)
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_WR(src)        (((u32)(src)<<5) & 0x00000060)
#define X4_MAC_PCS_CTL0_TX_LANE_DIV0_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields mac_pcs_ctl0_rx_lane_div	 */
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_WIDTH                                       2
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_SHIFT                                       3
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_MASK                               0x00000018
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_RD(src)             (((src) & 0x00000018)>>3)
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_WR(src)        (((u32)(src)<<3) & 0x00000018)
#define X4_MAC_PCS_CTL0_RX_LANE_DIV0_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields mac_pcs_ctl0_lane_pwr_off	 */
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_WIDTH                                      1
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_SHIFT                                      2
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_MASK                              0x00000004
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_RD(src)            (((src) & 0x00000004)>>2)
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_WR(src)       (((u32)(src)<<2) & 0x00000004)
#define X4_MAC_PCS_CTL0_LANE_PWR_OFF0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields phy_pcs_rx0_sigdet	 */
#define X4_PHY_PCS_RX0_SIGDET0_WIDTH                                             1
#define X4_PHY_PCS_RX0_SIGDET0_SHIFT                                             1
#define X4_PHY_PCS_RX0_SIGDET0_MASK                                     0x00000002
#define X4_PHY_PCS_RX0_SIGDET0_RD(src)                   (((src) & 0x00000002)>>1)
#define X4_PHY_PCS_RX0_SIGDET0_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define X4_PHY_PCS_RX0_SIGDET0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mac_pcs_rx0_encdet	 */
#define X4_MAC_PCS_RX0_ENCDET0_WIDTH                                             1
#define X4_MAC_PCS_RX0_ENCDET0_SHIFT                                             0
#define X4_MAC_PCS_RX0_ENCDET0_MASK                                     0x00000001
#define X4_MAC_PCS_RX0_ENCDET0_RD(src)                      (((src) & 0x00000001))
#define X4_MAC_PCS_RX0_ENCDET0_WR(src)                 (((u32)(src)) & 0x00000001)
#define X4_MAC_PCS_RX0_ENCDET0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl1	*/ 
/*	 Fields mac_pcs_rx1_edet_lb	 */
#define X4_MAC_PCS_RX1_EDET_LB1_WIDTH                                            1
#define X4_MAC_PCS_RX1_EDET_LB1_SHIFT                                           13
#define X4_MAC_PCS_RX1_EDET_LB1_MASK                                    0x00002000
#define X4_MAC_PCS_RX1_EDET_LB1_RD(src)                 (((src) & 0x00002000)>>13)
#define X4_MAC_PCS_RX1_EDET_LB1_WR(src)            (((u32)(src)<<13) & 0x00002000)
#define X4_MAC_PCS_RX1_EDET_LB1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields mac_pcs_rx1_edet_eie	 */
#define X4_MAC_PCS_RX1_EDET_EIE1_WIDTH                                           1
#define X4_MAC_PCS_RX1_EDET_EIE1_SHIFT                                          12
#define X4_MAC_PCS_RX1_EDET_EIE1_MASK                                   0x00001000
#define X4_MAC_PCS_RX1_EDET_EIE1_RD(src)                (((src) & 0x00001000)>>12)
#define X4_MAC_PCS_RX1_EDET_EIE1_WR(src)           (((u32)(src)<<12) & 0x00001000)
#define X4_MAC_PCS_RX1_EDET_EIE1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields mac_pcs_rx1_edet_ts	 */
#define X4_MAC_PCS_RX1_EDET_TS1_WIDTH                                            1
#define X4_MAC_PCS_RX1_EDET_TS1_SHIFT                                           11
#define X4_MAC_PCS_RX1_EDET_TS1_MASK                                    0x00000800
#define X4_MAC_PCS_RX1_EDET_TS1_RD(src)                 (((src) & 0x00000800)>>11)
#define X4_MAC_PCS_RX1_EDET_TS1_WR(src)            (((u32)(src)<<11) & 0x00000800)
#define X4_MAC_PCS_RX1_EDET_TS1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields mac_pcs_rx1_edet_skp	 */
#define X4_MAC_PCS_RX1_EDET_SKP1_WIDTH                                           1
#define X4_MAC_PCS_RX1_EDET_SKP1_SHIFT                                          10
#define X4_MAC_PCS_RX1_EDET_SKP1_MASK                                   0x00000400
#define X4_MAC_PCS_RX1_EDET_SKP1_RD(src)                (((src) & 0x00000400)>>10)
#define X4_MAC_PCS_RX1_EDET_SKP1_WR(src)           (((u32)(src)<<10) & 0x00000400)
#define X4_MAC_PCS_RX1_EDET_SKP1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields mac_pcs_ctl1_smpl_rate	 */
#define X4_MAC_PCS_CTL1_SMPL_RATE1_WIDTH                                         3
#define X4_MAC_PCS_CTL1_SMPL_RATE1_SHIFT                                         7
#define X4_MAC_PCS_CTL1_SMPL_RATE1_MASK                                 0x00000380
#define X4_MAC_PCS_CTL1_SMPL_RATE1_RD(src)               (((src) & 0x00000380)>>7)
#define X4_MAC_PCS_CTL1_SMPL_RATE1_WR(src)          (((u32)(src)<<7) & 0x00000380)
#define X4_MAC_PCS_CTL1_SMPL_RATE1_SET(dst,src) \
                       (((dst) & ~0x00000380) | (((u32)(src)<<7) & 0x00000380))
/*	 Fields mac_pcs_ctl1_tx_lane_div	 */
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_WIDTH                                       2
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_SHIFT                                       5
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_MASK                               0x00000060
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_RD(src)             (((src) & 0x00000060)>>5)
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_WR(src)        (((u32)(src)<<5) & 0x00000060)
#define X4_MAC_PCS_CTL1_TX_LANE_DIV1_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields mac_pcs_ctl1_rx_lane_div	 */
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_WIDTH                                       2
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_SHIFT                                       3
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_MASK                               0x00000018
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_RD(src)             (((src) & 0x00000018)>>3)
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_WR(src)        (((u32)(src)<<3) & 0x00000018)
#define X4_MAC_PCS_CTL1_RX_LANE_DIV1_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields mac_pcs_ctl1_lane_pwr_off	 */
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_WIDTH                                      1
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_SHIFT                                      2
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_MASK                              0x00000004
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_RD(src)            (((src) & 0x00000004)>>2)
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_WR(src)       (((u32)(src)<<2) & 0x00000004)
#define X4_MAC_PCS_CTL1_LANE_PWR_OFF1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields phy_pcs_rx1_sigdet	 */
#define X4_PHY_PCS_RX1_SIGDET1_WIDTH                                             1
#define X4_PHY_PCS_RX1_SIGDET1_SHIFT                                             1
#define X4_PHY_PCS_RX1_SIGDET1_MASK                                     0x00000002
#define X4_PHY_PCS_RX1_SIGDET1_RD(src)                   (((src) & 0x00000002)>>1)
#define X4_PHY_PCS_RX1_SIGDET1_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define X4_PHY_PCS_RX1_SIGDET1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mac_pcs_rx1_encdet	 */
#define X4_MAC_PCS_RX1_ENCDET1_WIDTH                                             1
#define X4_MAC_PCS_RX1_ENCDET1_SHIFT                                             0
#define X4_MAC_PCS_RX1_ENCDET1_MASK                                     0x00000001
#define X4_MAC_PCS_RX1_ENCDET1_RD(src)                      (((src) & 0x00000001))
#define X4_MAC_PCS_RX1_ENCDET1_WR(src)                 (((u32)(src)) & 0x00000001)
#define X4_MAC_PCS_RX1_ENCDET1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl2	*/ 
/*	 Fields mac_pcs_rx2_edet_lb	 */
#define X4_MAC_PCS_RX2_EDET_LB2_WIDTH                                            1
#define X4_MAC_PCS_RX2_EDET_LB2_SHIFT                                           13
#define X4_MAC_PCS_RX2_EDET_LB2_MASK                                    0x00002000
#define X4_MAC_PCS_RX2_EDET_LB2_RD(src)                 (((src) & 0x00002000)>>13)
#define X4_MAC_PCS_RX2_EDET_LB2_WR(src)            (((u32)(src)<<13) & 0x00002000)
#define X4_MAC_PCS_RX2_EDET_LB2_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields mac_pcs_rx2_edet_eie	 */
#define X4_MAC_PCS_RX2_EDET_EIE2_WIDTH                                           1
#define X4_MAC_PCS_RX2_EDET_EIE2_SHIFT                                          12
#define X4_MAC_PCS_RX2_EDET_EIE2_MASK                                   0x00001000
#define X4_MAC_PCS_RX2_EDET_EIE2_RD(src)                (((src) & 0x00001000)>>12)
#define X4_MAC_PCS_RX2_EDET_EIE2_WR(src)           (((u32)(src)<<12) & 0x00001000)
#define X4_MAC_PCS_RX2_EDET_EIE2_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields mac_pcs_rx2_edet_ts	 */
#define X4_MAC_PCS_RX2_EDET_TS2_WIDTH                                            1
#define X4_MAC_PCS_RX2_EDET_TS2_SHIFT                                           11
#define X4_MAC_PCS_RX2_EDET_TS2_MASK                                    0x00000800
#define X4_MAC_PCS_RX2_EDET_TS2_RD(src)                 (((src) & 0x00000800)>>11)
#define X4_MAC_PCS_RX2_EDET_TS2_WR(src)            (((u32)(src)<<11) & 0x00000800)
#define X4_MAC_PCS_RX2_EDET_TS2_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields mac_pcs_rx2_edet_skp	 */
#define X4_MAC_PCS_RX2_EDET_SKP2_WIDTH                                           1
#define X4_MAC_PCS_RX2_EDET_SKP2_SHIFT                                          10
#define X4_MAC_PCS_RX2_EDET_SKP2_MASK                                   0x00000400
#define X4_MAC_PCS_RX2_EDET_SKP2_RD(src)                (((src) & 0x00000400)>>10)
#define X4_MAC_PCS_RX2_EDET_SKP2_WR(src)           (((u32)(src)<<10) & 0x00000400)
#define X4_MAC_PCS_RX2_EDET_SKP2_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields mac_pcs_ctl2_smpl_rate	 */
#define X4_MAC_PCS_CTL2_SMPL_RATE2_WIDTH                                         3
#define X4_MAC_PCS_CTL2_SMPL_RATE2_SHIFT                                         7
#define X4_MAC_PCS_CTL2_SMPL_RATE2_MASK                                 0x00000380
#define X4_MAC_PCS_CTL2_SMPL_RATE2_RD(src)               (((src) & 0x00000380)>>7)
#define X4_MAC_PCS_CTL2_SMPL_RATE2_WR(src)          (((u32)(src)<<7) & 0x00000380)
#define X4_MAC_PCS_CTL2_SMPL_RATE2_SET(dst,src) \
                       (((dst) & ~0x00000380) | (((u32)(src)<<7) & 0x00000380))
/*	 Fields mac_pcs_ctl2_tx_lane_div	 */
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_WIDTH                                       2
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_SHIFT                                       5
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_MASK                               0x00000060
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_RD(src)             (((src) & 0x00000060)>>5)
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_WR(src)        (((u32)(src)<<5) & 0x00000060)
#define X4_MAC_PCS_CTL2_TX_LANE_DIV2_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields mac_pcs_ctl2_rx_lane_div	 */
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_WIDTH                                       2
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_SHIFT                                       3
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_MASK                               0x00000018
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_RD(src)             (((src) & 0x00000018)>>3)
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_WR(src)        (((u32)(src)<<3) & 0x00000018)
#define X4_MAC_PCS_CTL2_RX_LANE_DIV2_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields mac_pcs_ctl2_lane_pwr_off	 */
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_WIDTH                                      1
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_SHIFT                                      2
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_MASK                              0x00000004
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_RD(src)            (((src) & 0x00000004)>>2)
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_WR(src)       (((u32)(src)<<2) & 0x00000004)
#define X4_MAC_PCS_CTL2_LANE_PWR_OFF2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields phy_pcs_rx2_sigdet	 */
#define X4_PHY_PCS_RX2_SIGDET2_WIDTH                                             1
#define X4_PHY_PCS_RX2_SIGDET2_SHIFT                                             1
#define X4_PHY_PCS_RX2_SIGDET2_MASK                                     0x00000002
#define X4_PHY_PCS_RX2_SIGDET2_RD(src)                   (((src) & 0x00000002)>>1)
#define X4_PHY_PCS_RX2_SIGDET2_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define X4_PHY_PCS_RX2_SIGDET2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mac_pcs_rx2_encdet	 */
#define X4_MAC_PCS_RX2_ENCDET2_WIDTH                                             1
#define X4_MAC_PCS_RX2_ENCDET2_SHIFT                                             0
#define X4_MAC_PCS_RX2_ENCDET2_MASK                                     0x00000001
#define X4_MAC_PCS_RX2_ENCDET2_RD(src)                      (((src) & 0x00000001))
#define X4_MAC_PCS_RX2_ENCDET2_WR(src)                 (((u32)(src)) & 0x00000001)
#define X4_MAC_PCS_RX2_ENCDET2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Ctl3	*/ 
/*	 Fields mac_pcs_rx3_edet_lb	 */
#define X4_MAC_PCS_RX3_EDET_LB3_WIDTH                                            1
#define X4_MAC_PCS_RX3_EDET_LB3_SHIFT                                           13
#define X4_MAC_PCS_RX3_EDET_LB3_MASK                                    0x00002000
#define X4_MAC_PCS_RX3_EDET_LB3_RD(src)                 (((src) & 0x00002000)>>13)
#define X4_MAC_PCS_RX3_EDET_LB3_WR(src)            (((u32)(src)<<13) & 0x00002000)
#define X4_MAC_PCS_RX3_EDET_LB3_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields mac_pcs_rx3_edet_eie	 */
#define X4_MAC_PCS_RX3_EDET_EIE3_WIDTH                                           1
#define X4_MAC_PCS_RX3_EDET_EIE3_SHIFT                                          12
#define X4_MAC_PCS_RX3_EDET_EIE3_MASK                                   0x00001000
#define X4_MAC_PCS_RX3_EDET_EIE3_RD(src)                (((src) & 0x00001000)>>12)
#define X4_MAC_PCS_RX3_EDET_EIE3_WR(src)           (((u32)(src)<<12) & 0x00001000)
#define X4_MAC_PCS_RX3_EDET_EIE3_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields mac_pcs_rx3_edet_ts	 */
#define X4_MAC_PCS_RX3_EDET_TS3_WIDTH                                            1
#define X4_MAC_PCS_RX3_EDET_TS3_SHIFT                                           11
#define X4_MAC_PCS_RX3_EDET_TS3_MASK                                    0x00000800
#define X4_MAC_PCS_RX3_EDET_TS3_RD(src)                 (((src) & 0x00000800)>>11)
#define X4_MAC_PCS_RX3_EDET_TS3_WR(src)            (((u32)(src)<<11) & 0x00000800)
#define X4_MAC_PCS_RX3_EDET_TS3_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields mac_pcs_rx3_edet_skp	 */
#define X4_MAC_PCS_RX3_EDET_SKP3_WIDTH                                           1
#define X4_MAC_PCS_RX3_EDET_SKP3_SHIFT                                          10
#define X4_MAC_PCS_RX3_EDET_SKP3_MASK                                   0x00000400
#define X4_MAC_PCS_RX3_EDET_SKP3_RD(src)                (((src) & 0x00000400)>>10)
#define X4_MAC_PCS_RX3_EDET_SKP3_WR(src)           (((u32)(src)<<10) & 0x00000400)
#define X4_MAC_PCS_RX3_EDET_SKP3_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields mac_pcs_ctl3_smpl_rate	 */
#define X4_MAC_PCS_CTL3_SMPL_RATE3_WIDTH                                         3
#define X4_MAC_PCS_CTL3_SMPL_RATE3_SHIFT                                         7
#define X4_MAC_PCS_CTL3_SMPL_RATE3_MASK                                 0x00000380
#define X4_MAC_PCS_CTL3_SMPL_RATE3_RD(src)               (((src) & 0x00000380)>>7)
#define X4_MAC_PCS_CTL3_SMPL_RATE3_WR(src)          (((u32)(src)<<7) & 0x00000380)
#define X4_MAC_PCS_CTL3_SMPL_RATE3_SET(dst,src) \
                       (((dst) & ~0x00000380) | (((u32)(src)<<7) & 0x00000380))
/*	 Fields mac_pcs_ctl3_tx_lane_div	 */
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_WIDTH                                       2
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_SHIFT                                       5
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_MASK                               0x00000060
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_RD(src)             (((src) & 0x00000060)>>5)
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_WR(src)        (((u32)(src)<<5) & 0x00000060)
#define X4_MAC_PCS_CTL3_TX_LANE_DIV3_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields mac_pcs_ctl3_rx_lane_div	 */
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_WIDTH                                       2
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_SHIFT                                       3
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_MASK                               0x00000018
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_RD(src)             (((src) & 0x00000018)>>3)
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_WR(src)        (((u32)(src)<<3) & 0x00000018)
#define X4_MAC_PCS_CTL3_RX_LANE_DIV3_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields mac_pcs_ctl3_lane_pwr_off	 */
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_WIDTH                                      1
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_SHIFT                                      2
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_MASK                              0x00000004
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_RD(src)            (((src) & 0x00000004)>>2)
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_WR(src)       (((u32)(src)<<2) & 0x00000004)
#define X4_MAC_PCS_CTL3_LANE_PWR_OFF3_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields phy_pcs_rx3_sigdet	 */
#define X4_PHY_PCS_RX3_SIGDET3_WIDTH                                             1
#define X4_PHY_PCS_RX3_SIGDET3_SHIFT                                             1
#define X4_PHY_PCS_RX3_SIGDET3_MASK                                     0x00000002
#define X4_PHY_PCS_RX3_SIGDET3_RD(src)                   (((src) & 0x00000002)>>1)
#define X4_PHY_PCS_RX3_SIGDET3_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define X4_PHY_PCS_RX3_SIGDET3_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mac_pcs_rx3_encdet	 */
#define X4_MAC_PCS_RX3_ENCDET3_WIDTH                                             1
#define X4_MAC_PCS_RX3_ENCDET3_SHIFT                                             0
#define X4_MAC_PCS_RX3_ENCDET3_MASK                                     0x00000001
#define X4_MAC_PCS_RX3_ENCDET3_RD(src)                      (((src) & 0x00000001))
#define X4_MAC_PCS_RX3_ENCDET3_WR(src)                 (((u32)(src)) & 0x00000001)
#define X4_MAC_PCS_RX3_ENCDET3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PcsCsr_Test	*/ 
/*	 Fields mpic_test_en	 */
#define X4_MPIC_TEST_EN_WIDTH                                                    1
#define X4_MPIC_TEST_EN_SHIFT                                                    0
#define X4_MPIC_TEST_EN_MASK                                            0x00000001
#define X4_MPIC_TEST_EN_RD(src)                             (((src) & 0x00000001))
#define X4_MPIC_TEST_EN_WR(src)                        (((u32)(src)) & 0x00000001)
#define X4_MPIC_TEST_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))
	
/*****************************************************************************
 * APM862xx Compatible 
 *
 * These entry are manually added for compatiblity.
 *****************************************************************************/
#define X1_PCSCSR_REGADDR_ADDR		PCSCSR_REGADDR_ADDR
#define X1_PCSCSR_REGWRDATA_ADDR	PCSCSR_REGWRDATA_ADDR
#define X1_MGMT_PCS_REG_WR_F2_MASK	MGMT_PCS_REG_WR_MASK
#define X1_MGMT_PCS_REG_WR_DONE_F2_MASK MGMT_PCS_REG_WR_DONE_MASK        
#define X1_PCSCSR_REGCMDDONE_ADDR	PCSCSR_REGCMDDONE_ADDR
#define X1_PCSCSR_REGCMD_ADDR		PCSCSR_REGCMD_ADDR
#define X1_MGMT_PCS_REG_RD_F2_MASK	MGMT_PCS_REG_RD_MASK
#define X1_MGMT_PCS_REG_RD_DONE_F2_MASK MGMT_PCS_REG_RD_DONE_MASK
#define X1_PCSCSR_REGRDDATA_ADDR	PCSCSR_REGRDDATA_ADDR     
#define DEVICETYPE_F6_MASK		DEVICETYPE_F2_MASK
#define X1_PCSCSR_CTL0_ADDR		PCSCSR_CTL0_ADDR
#define X1_PCIE_MODE_SEL0_SET		PCIE_PCIE_MODE_SEL0_SET
#define X1_PCIE_MODE_SEL0_WR		PCIE_PCIE_MODE_SEL0_WR
#define X1_PHY_TX_TERM_CAL_IN0_F2_WR	PHY_TX_TERM_CAL_IN0_WR
#define X1_PHY_PCS_RX0_SIGDET0_F2_WR	PHY_PCS_RX0_SIGDET0_WR
#define X1_MAC_PCS_X2_MODE0_WR		PCIE_MAC_PCS_X2_MODE0_WR
#define X1_MAC_PCS_PLL_PCS_DIV0_F2_WR	MAC_PCS_PLL_PCS_DIV0_WR
#define X1_INIT_TX_TERM_SEL0_F2_WR	INIT_TX_TERM_SEL0_WR
#define X1_INIT_REFCLK_AC_MODE0_F2_WR   PCIE_INIT_REFCLK_AC_MODE0_WR
#define X1_INIT_CTLIFC_PIPE_COMPLIANT0_F2_WR INIT_CTLIFC_PIPE_COMPLIANT0_WR
#define X1_INIT_CTLIFC_USE_MGMT0_F2_WR  INIT_CTLIFC_USE_MGMT0_WR
#define X1_RESET0_F4_WR			PCIE_RESET0_WR
#define X1_RESET0_F4_MASK		PCIE_RESET0_MASK
#define X1_MAC_PCS_PLL_REFCLK_SEL0_F2_WR MAC_PCS_PLL_REFCLK_SEL0_WR 
#define X1_MAC_PCS_PLL_OP_RANGE0_WR	MAC_PCS_PLL_OP_RANGE0_WR	
#define X1_MAC_PCS_PLL_DIV0_WR		MAC_PCS_PLL_DIV0_WR
#define X1_PCSCSR_CTL1_ADDR		PCSCSR_CTL1_ADDR
#define X1_MAC_PCS_RX0_EDET_TS1_WR	MAC_PCS_RX0_EDET_TS1_WR
#define X1_MAC_PCS_RX0_ENCDET1_WR	MAC_PCS_RX0_ENCDET1_WR
#define X1_MAC_PCS_SATA0_HOST1_WR	MAC_PCS_SATA0_HOST1_WR
#define X1_MAC_PCS_RX0_EDET_LB1_WR	MAC_PCS_RX0_EDET_LB1_WR
#define X1_MAC_PCS_RX0_EDET_EIE1_WR	MAC_PCS_RX0_EDET_EIE1_WR
#define X1_MAC_PCS_RX0_EDET_SKP1_WR	MAC_PCS_RX0_EDET_SKP1_WR
#define X1_MAC_PCS_CTL0_RX_LANE_DIV1_WR MAC_PCS_CTL0_RX_LANE_DIV1_WR
#define X1_MAC_PCS_CTL0_OP_RANGE1_F2_WR	MAC_PCS_CTL0_OP_RANGE1_WR
#define X1_MAC_PCS_CTL0_SMPL_RATE1_WR	MAC_PCS_CTL0_SMPL_RATE1_WR
#define X1_MAC_PCS_CTL0_TX_LANE_DIV1_WR MAC_PCS_CTL0_TX_LANE_DIV1_WR
#define X1_MAC_PCS_CTL0_LANE_PWR_OFF1_WR MAC_PCS_CTL0_LANE_PWR_OFF1_WR
#define X1_MAC_PCS_CTL0_RX_MODE1_F2_WR	PCIE_MAC_PCS_CTL0_RX_MODE1_WR
#define X1_MAC_PCS_CTL0_TX_MODE1_F2_WR	PCIE_MAC_PCS_CTL0_TX_MODE1_WR
#define X1_PCSCSR_STATUS1_ADDR		PCSCSR_STATUS1_ADDR
#define X1_SDS_PCS_CLOCK_READY1_F2_MASK SDS_PCS_CLOCK_READY1_MASK
#define X1_SDS_PCS_PLL_LOCK1_F2_MASK	SDS_PCS_PLL_LOCK1_MASK

#endif /* _APM_PCIE_CSR_H__ */
