/*
 * Copyright (c) 2008, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef _ATHR_GMAC_H
#define _ATHR_GMAC_H
#include <linux/stddef.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/bitops.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <net/sch_generic.h>
#include <net/inet_ecn.h>                /* XXX for TOS */
#include <net/iw_handler.h>
#include <linux/wireless.h>
#include <linux/bitops.h>
#include <linux/leds.h>

#if 0
#if defined(CONFIG_SOC_AR724X)||defined(CONFIG_MACH_HORNET)
#include "ag7240.h"
#endif

#if defined(CONFIG_SOC_AR934X)
#include "ag934x.h"
#endif

#if defined(CONFIG_MACH_QCA955x)
#include "qca955x_mac.h"
#endif
#endif

#include "athrs_trc.h"
#include "athrs_ioctl.h"
#include "athrs_phy.h"
#include "osdep.h"
#include "athrs_config.h"    
#include "athr_pb.h"
#include "athrs_hw_accels.h"

#ifdef HYBRID_PLC_FILTER
#include "athrs_plc_filter.h"
#endif

#ifdef CONFIG_ATHRS_QOS
extern int set_mac_qops(void *mac);
#endif

#if defined (CONFIG_ATHRS_QOS) || defined(CONFIG_ATHR_VLAN_IGMP)
extern void athr_set_vlan_phy_ops(void *arg);
extern int athrs_config_qos(struct eth_cfg_params *ethcfg, int cmd);
#endif

#define pl() printk("%s:%d\n",__func__,__LINE__)

/*
 * This defines the interconnects between MAC and PHY at compile time
 * There are several constraints - the Kconfig largely takes care of them
 * at compile time.
 */
#if defined (CONFIG_AG7240_GE0_GMII)
    #define     ATHR_GMAC_MII0_INTERFACE   0
#elif defined (CONFIG_AG7240_GE0_MII)
    #define     ATHR_GMAC_MII0_INTERFACE   1
#elif defined (CONFIG_AG7240_GE0_RGMII)
    #define     ATHR_GMAC_MII0_INTERFACE   2
#elif defined (CONFIG_AG7240_GE0_RMII)
    #define     ATHR_GMAC_MII0_INTERFACE   3
#endif /*defined (AG7240_GE0_GMII)*/

#define CONFIG_AG7240_NUMBER_TX_PKTS 100 //KPHACK
#define CONFIG_AG7240_NUMBER_RX_PKTS 252 //KPHACK
#define CONFIG_AG7240_LEN_PER_TX_DS 1536
#define ATHRS_LED_MAX_NAME_LEN        21

/*
 *  QCA955x appears to have a problem transmitting
 *  buffers larger than ~1450 bytes per descriptor
 *  So need to split large packets across multiple
 *  descriptors
 */
#define CONFIG_QCA955X_MAX_LEN_PER_TX_DS  1024

#if defined(CONFIG_AG7240_GE1_IS_CONNECTED)

    #define ATHR_GMAC_NMACS            2

    #if defined (CONFIG_AG7240_GE1_GMII)
        #define ATHR_GMAC_MII1_INTERFACE   0
    #elif defined (CONFIG_AG7240_GE1_MII)
        #define ATHR_GMAC_MII1_INTERFACE   1
    #elif defined (CONFIG_AG7240_GE1_RGMII)
        #define ATHR_GMAC_MII1_INTERFACE   2
    #elif defined (CONFIG_AG7240_GE1_RMII)
        #define ATHR_GMAC_MII1_INTERFACE   3
    #endif /*AG7240_GE1_RGMII*/
#else
    #define ATHR_GMAC_NMACS            1
    #define ATHR_GMAC_MII1_INTERFACE   0xff        /*not connected*/
#endif  /*AG7240_GE1_IS_CONNECTED*/




/* MAC QOS defines */

#ifndef ETHERTYPE_IP
#define ETHERTYPE_IP     0x0800          /* IP protocol */
#endif

#define IP_PRI_SHIFT     5

#define ENET_NUM_AC      4               /* 4 AC categories */
/* QOS stream classes */
#define ENET_AC_BE       0               /* best effort */
#define ENET_AC_BK       1               /* background */
#define ENET_AC_VI       2               /* video */
#define ENET_AC_VO       3               /* voice */

#if defined (CONFIG_AG7240_ATHRS17_PHY)
#define HDR_PACKET_TYPE_MASK    0x07C0
#define HDR_PRIORITY_SHIFT      0x0B
#define HDR_PRIORITY_MASK       0x07
#else
#define HDR_PACKET_TYPE_MASK    0x0F            
#define HDR_PRIORITY_SHIFT      0x4
#define HDR_PRIORITY_MASK       0x3
#endif
#define TOS_ECN_SHIFT           0x2
#define TOS_ECN_MASK            0xFC


/* 5 - Express Forwarding (EF) 4 - Class 4
 * 3  - Class 3  0 - Best effort 
 * 1 - Class 1  2 - Class 2
 */

#define TOS_TO_ENET_AC(_tos) (      \
    (((_tos) == 0) || ((_tos) == 3)) ? ENET_AC_BE : \
    (((_tos) == 1) || ((_tos) == 2)) ? ENET_AC_BK : \
    (((_tos) == 4) || ((_tos) == 5)) ? ENET_AC_VI : \
        ENET_AC_VO)\

#if defined (CONFIG_AG7240_ATHRS17_PHY)
#define AC_TO_ENET_TOS(ac)ac
#else
#define AC_TO_ENET_TOS(ac) ( \
     (ac == 3) ? 6 : (ac == 2) ? 4: \
     (ac == 1) ? 2 : (ac == 0) ? 3 : 0)
#endif

/*
 *  MAC FLAGS 
 */

#define CHECK_DMA_STATUS        0x00000001
#define ETH_SOFT_LED            0x00000002
#define WAN_QOS_SOFT_CLASS      0x00000004
#define ETH_SWONLY_MODE         0x00000008
#define ATHR_S26_HEADER         0x00000010
#define ATHR_S16_HEADER         0x00000020
#define ETH_PKT_INSPECT         0x00000040
#define ATHR_RX_FLCTL           0x00000080
#define ATHR_TX_FLCTL           0x00000100
#define ETH_LINK_POLL           0x00000200
#define ETH_LINK_INTERRUPT      0x00000400
#define ETH_FORCE_SPEED         0x00000800
#define ATHR_VLAN_IGMP          0x00001000
#define ATHR_RX_TASK		0x00002000
#define ATHR_RX_POLL		0x00004000
#define ATHR_SWITCH_QOS         0x00008000
#define ATHR_S27_HEADER         0x00010000
#define ATHR_HEADER_ENABLED     0x00020000
#define ATHR_DUAL_PHY           0x00040000
#define ATHR_JUMBO_FR           0x00080000
#define ATHR_AP123_GPIO_SWAP    0x00100000
#define ATHR_SGMII_FLAG         0x00200000
#define ATHR_GMAC0_SGMII        0x00400000
#define ATHR_GMAC1_SGMII        0x00800000
#define ATHR_GMAC0_RGMII	0x01000000
#define ATHR_SGMII_FORCED       0x02000000
#define S17_MAC6_CONNECTED      0x04000000
#define SLAVE_SCO_CONNECTED     0x08000000 
#define ATHR_S17_MAC0_SGMII     0x10000000   

/*
 * Config/Mac Register definitions
 */
#define ATHR_GMAC_CFG1                 0x00
#define ATHR_GMAC_CFG2                 0x04
#define ATHR_GMAC_MAX_PKTLEN           0x10
#define ATHR_GMAC_IFCTL                0x38

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
#define ATHR_GMAC_RX_BYTES_CNTR              0x9c                      /*Rx Byte counter*/
#define ATHR_GMAC_RX_PKT_CNTR                0xa0                      /*Rx Packet counter*/
#define ATHR_GMAC_RX_CRC_ERR_CNTR            0xa4                      /*Rx rcv FCR error counter */
#define ATHR_GMAC_RX_MULT_CNTR               0xa8                      /*Rx rcv Mcast packet counter*/ 
#define ATHR_GMAC_RX_RBCA_CNTR               0xac                      /*Rx Broadcast packet counter*/
#define ATHR_GMAC_RX_RXCF_CNTR               0xb0                      /*Rx rcv control frame packet counter */
#define ATHR_GMAC_RX_RXPF_CNTR               0xb4                      /*Rx rcv pause frame work packet counter*/
#define ATHR_GMAC_RX_RXUO_CNTR               0xb8                      /*Rx rcv unknown opcode counter*/
#define ATHR_GMAC_RX_FRM_ERR_CNTR            0xbc                      /*Rx rcv alignment error counter*/
#define ATHR_GMAC_RX_LEN_ERR_CNTR            0xc0                      /*Rx Frame length error counter*/
#define ATHR_GMAC_RX_CODE_ERR_CNTR           0xc4                      /*Rx rcv code error counter */  
#define ATHR_GMAC_RX_CRS_ERR_CNTR            0xc8                      /*Rx rcv carrier sense error counter*/
#define ATHR_GMAC_RX_RUND_CNTR               0xcc                      /*Rx rcv undersize packet counter*/
#define ATHR_GMAC_RX_OVL_ERR_CNTR            0xd0                      /*Rx rcv oversize packet counter*/
#define ATHR_GMAC_RX_RFRG_CNTR               0xd4                      /*Rx rcv fragments counter */
#define ATHR_GMAC_RX_RJBR_CNTR               0xd8                      /*Rx rcv jabber counter*/ 
#define ATHR_GMAC_RX_DROP_CNTR               0xdc                      /*Rx rcv drop*/  
#define ATHR_GMAC_TOTAL_COL_CNTR             0x10c

/*
 * Tx counter registers
 */
#define ATHR_GMAC_TX_PKT_CNTR                0xe4
#define ATHR_GMAC_TX_BYTES_CNTR              0xe0
#define ATHR_GMAC_TX_DROP_CNTR               0x114
#define ATHR_GMAC_TX_MULT_CNTR               0xe8
#define ATHR_GMAC_TX_CRC_ERR_CNTR            0x11c
#define ATHR_GMAC_TX_BRD_CNTR                0xEC
#define ATHR_GMAC_TX_PCTRL_CNTR              0xf0                       /*Tx pause control frame counter */
#define ATHR_GMAC_TX_DFRL_CNTR               0xf4                       /*TX Deferral packet counter */
#define ATHR_GMAC_TX_TEDF_CNTR               0x1ec                      /*Tx Excessive deferal packet counter */
#define ATHR_GMAC_TX_TSCL_CNTR               0xfc                       /*Tx Single collision pcket counter */
#define ATHR_GMAC_TX_TMCL_CNTR               0x100                      /*TX Multiple collision packet counter */
#define ATHR_GMAC_TX_TLCL_CNTR               0x104                      /*Tx Late collision packet counter */
#define ATHR_GMAC_TX_TXCL_CNTR               0x108                      /*Tx excesive collision packet counter */
#define ATHR_GMAC_TX_TNCL_CNTR               0x10c                      /*Tx total collison counter */
#define ATHR_GMAC_TX_TPFH_CNTR               0x110                      /*Tx pause frames honoured counter */
#define ATHR_GMAC_TX_TJBR_CNTR               0x118                      /*Tx jabber frame counter */
#define ATHR_GMAC_TX_TXCF_CNTR               0x120                      /*Tx Control frame counter */
#define ATHR_GMAC_TX_TOVR_CNTR               0x124                      /*Tx oversize frame counter */
#define ATHR_GMAC_TX_TUND_CNTR               0x128                      /*TX under size frame counter */
#define ATHR_GMAC_TX_TFRG_CNTR               0x12c                      /*TX Fragments frame counter */



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
#define ATHR_GMAC_ETH_CFG_RDV_DELAY	     (1 << 16)



//#define ATHR_GMAC_DEBUG 1

#ifdef ATHR_GMAC_DEBUG
#define DPRINTF(_fmt,...) do {         \
            printk("GMAC:"_fmt, __VA_ARGS__);      \
} while (0)
#else
#define DPRINTF(_fmt,...)
#endif

extern int mii0_if;
extern int mii1_if;

#define mii_reg(_mac)   (ATHR_MII0_CTRL + ((_mac)->mac_unit * 4))
#define mii_if(_mac)    (((_mac)->mac_unit == 0) ? mii0_if : mii1_if)

#define athr_gmac_set_mii_ctrl_speed(_mac, _spd)   do {                        \
    athr_reg_rmw_clear(mii_reg(_mac), (3 << 4));                          \
    athr_reg_rmw_set(mii_reg(_mac), ((_spd) << 4));                       \
}while(0);

/*
 * IP needs 16 bit alignment. But RX DMA needs 4 bit alignment. We sacrifice IP
 * Plus Reserve extra head room for wmac
 */
#define ETHERNET_FCS_SIZE            4

#ifdef ATH_HY_PATH_SWITCH
#define ATHR_GMAC_RX_RESERVE           (64+16)
#else
#define ATHR_GMAC_RX_RESERVE           (64)
#endif

#define ATHR_GMAC_RX_BUF_SIZE      \
    (ETH_HLEN + ETH_FRAME_LEN + ETHERNET_FCS_SIZE)

#define ATHR_GMAC_RX_ALLOC_SIZE \
    (ATHR_GMAC_RX_RESERVE + ATHR_GMAC_RX_BUF_SIZE)

#define ATHR_GMAC_RX_PADDING(size) \
    ((-(size)) & (L1_CACHE_BYTES-1))

#define is_drqfn()      	(!(ath_reg_rd(RST_REVISION_ID_ADDRESS) & 0x1000))


#define ATHR_IRQ_ENET_LINK                   (ATH79_MISC_IRQ_BASE + 12)
#define ATHR_GE0_BASE                        ATH_GE0_BASE
#define ATHR_GE1_BASE                        ATH_GE1_BASE
#define ATHR_CPU_IRQ_GE0                     ATH_CPU_IRQ_GE0
#define ATHR_CPU_IRQ_GE1                     ATH_CPU_IRQ_GE1
#define ATHR_SRAM_START 		     0xbd000000

#define athr_gmac_base(_no)    (_no) ? ATHR_GE1_BASE    : ATHR_GE0_BASE
#define athr_gmac_irq(_no)     (_no) ? ATHR_CPU_IRQ_GE1 : ATHR_CPU_IRQ_GE0

#define athr_gmac_reset_mask(_no) (_no) ? (ATHR_RESET_GE1_MAC)   \
                                     : (ATHR_RESET_GE0_MAC)  

#define athr_gmac_unit2mac(_unit)     athr_gmacs[(_unit)]
// #define athr_gmac_unit2mac(_unit)    is_drqfn() ? athr_gmacs[1] : athr_gmacs[(_unit)]
#define athr_gmac_name2mac(name)      \
    strcmp(name,"eth0") ? athr_gmac_unit2mac(1) : athr_gmac_unit2mac(0)

#define assert(_cond)   do {                                     \
    if(!(_cond)) {                                               \
        athr_gmac_trc_dump();                                       \
        printk("%s:%d: assertion failed\n", __func__, __LINE__); \
        BUG();                                                   \
    }                                                            \
}while(0);

#define athr_gmac_reg_rd(_mac, _reg)                                       \
            (athr_reg_rd((_mac)->mac_base + (_reg)))

#define athr_gmac_reg_wr(_mac, _reg, _val)                                 \
                athr_reg_wr((_mac)->mac_base + (_reg), (_val));

/*
 * The no flush version
 */
#define athr_gmac_reg_wr_nf(_mac, _reg, _val)                             \
                athr_reg_wr_nf((_mac)->mac_base + (_reg), (_val));

#define athr_gmac_reg_rmw_set(_mac, _reg, _mask)                           \
                 athr_reg_rmw_set((_mac)->mac_base + (_reg), (_mask));

#define athr_gmac_reg_rmw_clear(_mac, _reg, _mask)                          \
                 athr_reg_rmw_clear((_mac)->mac_base + (_reg), (_mask));


#define athr_gmac_desc_dma_addr(_r, _ds)                                   \
    (u32)((athr_gmac_desc_t *)(_r)->ring_desc_dma + ((_ds) - ((_r)->ring_desc)))


/*
 * tx/rx stop start
 */
#define athr_gmac_tx_stopped(_mac)                                         \
    (!(athr_gmac_reg_rd((_mac), ATHR_GMAC_DMA_TX_CTRL) & ATHR_GMAC_TXE))

#define athr_gmac_rx_start(_mac)                                           \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_RX_CTRL, ATHR_GMAC_RXE)

#define athr_gmac_rx_stop(_mac)                                            \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_RX_CTRL, 0)

#define athr_gmac_tx_start_qos(_mac,ac)                               \
switch(ac) {                               \
    case ENET_AC_VO:                               \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q0, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_VI:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q1, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_BK:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q2, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_BE:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q3, ATHR_GMAC_TXE);  \
        break;                             \
}                                       

#define athr_gmac_tx_start(_mac)                                       \
            athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q0, ATHR_GMAC_TXE) 

#define athr_gmac_tx_stop(_mac)                        \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL, 0)

/*
 * ownership of descriptors between DMA and cpu
 */
#define athr_gmac_rx_owned_by_dma(_ds)     ((_ds)->is_empty == 1)
#define athr_gmac_rx_give_to_dma(_ds)      ((_ds)->is_empty = 1)
#define athr_gmac_tx_owned_by_dma(_ds)     ((_ds)->is_empty == 0)
#define athr_gmac_tx_give_to_dma(_ds)      ((_ds)->is_empty = 0)
#define athr_gmac_tx_own(_ds)              ((_ds)->is_empty = 1)

/*
 * Interrupts 
 * ----------
 */
#define athr_gmac_get_isr(_mac) athr_gmac_reg_rd((_mac), ATHR_GMAC_DMA_INTR);
#define athr_gmac_int_enable(_mac)                                         \
    athr_gmac_reg_wr(_mac, ATHR_GMAC_DMA_INTR_MASK, _mac->intr_mask)

#define athr_gmac_int_disable(_mac)                                        \
    athr_gmac_reg_wr(_mac, ATHR_GMAC_DMA_INTR_MASK, 0)
/*
 * ACKS:
 * - We just write our bit - its write 1 to clear.
 * - These are not rmw's so we dont need locking around these.
 * - Txurn and rxovf are not fastpath and need consistency, so we use the flush
 *   version of reg write.
 * - ack_rx is done every packet, and is largely only for statistical purposes;
 *   so we use the no flush version and later cause an explicite flush.
 */
#define athr_gmac_intr_ack_txurn(_mac)                                           \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_STATUS, ATHR_GMAC_TX_STATUS_URN);
#define athr_gmac_intr_ack_rx(_mac)                                              \
    athr_gmac_reg_wr_nf((_mac), ATHR_GMAC_DMA_RX_STATUS, ATHR_GMAC_RX_STATUS_PKT_RCVD);
#define athr_gmac_intr_ack_rxovf(_mac)                                           \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_RX_STATUS, ATHR_GMAC_RX_STATUS_OVF);
/*
 * Not used currently
 */
#define athr_gmac_intr_ack_tx(_mac)                                              \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_STATUS, ATHR_GMAC_TX_STATUS_PKT_SENT);
#define athr_gmac_intr_ack_txbe(_mac)                                            \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_STATUS, ATHR_GMAC_TX_STATUS_BUS_ERROR);
#define athr_gmac_intr_ack_rxbe(_mac)                                            \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_RX_STATUS, ATHR_GMAC_RX_STATUS_BUS_ERROR);

/*
 * Enable Disable. These are Read-Modify-Writes. Sometimes called from ISR
 * sometimes not. So the caller explicitely handles locking.
 */
#define athr_gmac_intr_disable_txurn(_mac)                                 \
    athr_gmac_reg_rmw_clear((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_TX_URN);

#define athr_gmac_intr_enable_txurn(_mac)                                  \
    athr_gmac_reg_rmw_set((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_TX_URN);

#define athr_gmac_intr_enable_tx(_mac)                                     \
    athr_gmac_reg_rmw_set((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_TX);

#define athr_gmac_intr_disable_tx(_mac)                                    \
    athr_gmac_reg_rmw_clear((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_TX);

#define athr_gmac_intr_disable_recv(_mac)                                  \
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_DMA_INTR_MASK,(ATHR_GMAC_INTR_RX | ATHR_GMAC_INTR_RX_OVF));

#define athr_gmac_intr_enable_rxovf(_mac)                                  \
    athr_gmac_reg_rmw_set((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_RX_OVF);

#define athr_gmac_intr_disable_rxovf(_mac)                                 \
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_DMA_INTR_MASK,(ATHR_GMAC_INTR_RX_OVF));

#define athr_gmac_intr_enable_recv(_mac)                                   \
    athr_gmac_reg_rmw_set(mac, ATHR_GMAC_DMA_INTR_MASK,(ATHR_GMAC_INTR_RX | ATHR_GMAC_INTR_RX_OVF));

/* 
 * Timers 
 */

#define ATH_GMAC_POLL_LINK_FREQ (2 * HZ)
#define ATH_GMAC_CHECK_DMA_FREQ (HZ / 2)
#define ATH_GMAC_LED_BLINK_FREQ (HZ / 10)

#if HYBRID_LINK_CHANGE_EVENT
typedef struct {
    uint8_t  name[10];
    uint32_t vlanid;
    uint32_t vlanmap;
    uint16_t p6used;  /* used to judge whether this vlan is PLC (port6) */
    uint16_t count;   /* used to judge whether this vlan is LAN(count > 1) */
}athr_hybrid_lce_t;

#define HYBRID_MAX_VLAN 5
#define HYBRID_PLC_PORT 6
#define HYBIRD_COUNT_BIT 0x3
#define HYBIRD_COUNT_BITMAP 0x7
#endif

/*
 * h/w descriptor
 */
typedef struct {
    uint32_t    pkt_start_addr;

    uint32_t    is_empty       :  1;
    uint32_t    res1           :  6;
    uint32_t    more           :  1;
    uint32_t    res2           :  3;
    uint32_t    ftpp_override  :  5;
    uint32_t    res3           :  4;
    uint32_t    pkt_size       : 12;

    uint32_t    next_desc      ;
}athr_gmac_desc_t;


/*
 * s/w descriptor
 */
typedef struct {
    struct sk_buff    *buf_pkt;       /*ptr to skb*/
    int               buf_nds;        /*no. of desc for this skb*/
    athr_gmac_desc_t  *buf_lastds;    /*the last desc. (for convenience)*/
    unsigned long     trans_start;    /*  descriptor time stamp */
}athr_gmac_buffer_t;

/*
 * Tx and Rx descriptor rings;
 */
typedef struct {
    athr_gmac_desc_t   *ring_desc;          /* hardware descriptors */
    dma_addr_t         ring_desc_dma;       /* dma addresses of desc*/
    athr_gmac_buffer_t *ring_buffer;        /* OS buffer info       */
    int                ring_head;           /* producer index       */
    int                ring_tail;           /* consumer index       */
    int                ring_nelem;          /* nelements            */
}athr_gmac_ring_t;

static inline int athr_gmac_ring_incr(athr_gmac_ring_t *r, int val)
{
    val++;
    if(unlikely(val == r->ring_nelem))
        val = 0;
    return val;
}

typedef struct {
    int stats;
}athr_gmac_stats_t;

/*
 * 0, 1, 2: based on hardware values for mii ctrl bits [5,4]
 */
typedef enum {
    ATHR_PHY_SPEED_UNKNOWN = -1,
    ATHR_PHY_SPEED_10T     =  0,
    ATHR_PHY_SPEED_100T,
    ATHR_PHY_SPEED_1000T,
}athr_phy_speed_t;

typedef enum {
    ATHR_RX = 0,
    ATHR_TX,
    ATHR_NONE,
}athr_desc_type;

typedef enum {
    SGMII_LINK_IS_DEAD  = 0,
    SGMII_LINK_IS_ALIVE = 1,
}athr_gmac_sgmii_link_t; 

typedef enum {
    ATHR_SGMII_DUPLEX_UNKNOWN = -1,
    ATHR_SGMII_HALF_DUPLEX = 0,
    ATHR_SGMII_FULL_DUPLEX,
}athr_gmac_duplex_t;

typedef enum {
    ATHR_PHY_UNKNOWN = -1,
    ATHR_S27_PHY=0,
    ATHR_S17_PHY,
    ATHR_AR8033_PHY,
    ATHR_VIR_PHY,
    ATHR_S26_PHY,
    ATHR_S16_PHY,
    ATHR_F1E_PHY,
    ATHR_MARVEL_PHY,

}athr_gmac_mii_phy_t;


typedef enum {
    ATHR_MII_UNKOWN = -1,
    ATHR_GMII = 0,
    ATHR_MII,
    ATHR_RGMII,
    ATHR_RMII,
    ATHR_SGMII,
    ATHR_SERDES,
}athr_gmac_mii_intf_t;




/**
 * Added for customizing LED control operations
 */

typedef struct {
    u_int32_t    rate;        // Rate per second
    u_int32_t    timeOn;      // LED ON time in ms
    u_int32_t    timeOff;     // LED OFF time in ms
} LED_BLINK_RATES;

typedef struct eth_led_control {
    uint8_t                link[5],      // current link state
                           led[5],       // current led state
                           intvl[5];     // how many blinks completed
    int8_t                 hold[5];      // how long for current pattern
    uint32_t               pkt_count[5];
    LED_BLINK_RATES const  *rate[5];
    athr_phy_speed_t       speed[5];     // current link speed
    struct timer_list      timer;
} ath_led_ctrl_t;


#ifdef CONFIG_MACH_HORNET
#define MB(x) ((x * 100000) / 8)  /* Mbits/sec */
/* Make LED_ON_TIME and LED_OFF_TIME the same for 100M table.
 * So even there's a bug in Hornet 1.1 ETH LED,
 * (i.e. LED_ON_TIME is actually time period for LAN LED off)
 * the blinking rate will be the same for WAN and LAN LEDs.
 *
 * For 10M table, since the blinking rate is slow,
 * remain it unchanged except the last entry in the table.
 */
static const
LED_BLINK_RATES BlinkRateTable_100M[] = {
    {  MB(10),   5,  3 }, /* on:168ms off:168ms */
    {  MB(30),   4,  2 }, /* on:84ms  off:84ms */
    {  MB(50),   3,  1 }, /* on:42ms  off:42ms */
    {  MB(100),  2,  0 }, /* on:21ms  off:21ms */
    {  0xffffffff, 2 , 0 }
};

static const
LED_BLINK_RATES BlinkRateTable_10M[] = {
    {  MB(1),   5,  5 }, /* on:168ms off:670ms */
    {  MB(3),   4,  3 }, /* on:84ms  off:168ms */
    {  MB(5),   2,  2 }, /* on:42ms  off:84ms */
    {  MB(7),   2,  1 }, /* on:21ms  off:42ms */
    {  MB(10),  1,  1 }, /* on:10ms  off:42ms */
    {  0xffffffff, 1 , 1 }
};
#else
#define MB(x) (((x * 100000) / 8) / 10)  /* Mbits/100 msec */
static const
LED_BLINK_RATES BlinkRateTable_100M[] = {
    {  MB(1),   2,  5 },
    {  MB(5),   4,  3 },
    {  MB(10),  2,  2 },
    {  MB(50),  2,  1 },
    {  MB(100), 1,  1 },
    {  0xffffffff, 1, 1 }
};

static const
LED_BLINK_RATES BlinkRateTable_10M[] = {
    {  MB(1),   2,  5 },
    {  MB(3),   4,  3 },
    {  MB(5),   2,  2 },
    {  MB(7),   2,  1 },
    {  MB(10),  1,  1 },
    {  0xffffffff, 1, 1 }
};
#endif

/* 
 * Mac opertations structure
 */

typedef struct {
    int  (*check_dma_st)        (void *arg,int ac);
    int  (*set_pll)             (void *arg,athr_phy_speed_t speed);
    int  (*set_caps)            (void *arg);
    int  (*soft_led)            (void *arg);
    int  (*check_link)          (void *arg,int phyUnit);
    int  (*poll_link)		(void *arg);
    int  (*soc_gmac_hw_setup)   (void *arg);
    int  (*soc_gmac_mii_setup)  (void *arg);
    void (*soc_gmac_set_link)   (void *arg, athr_phy_speed_t speed, int fdx);
}athr_gmac_ops_t;


/*
 * Represents an ethernet MAC. Contains ethernet devices (LAN and WAN)
 */


typedef struct {
    struct net_device      *mac_dev;
    uint32_t                mac_unit;
    uint32_t                mac_base;
    int                     mac_irq;
    uint8_t                 mac_ifup;
    uint8_t                 mac_noacs;
    uint8_t                 no_of_macs;
    athr_gmac_ring_t        mac_txring[4];
    athr_gmac_ring_t        mac_rxring;
    athr_gmac_stats_t       mac_stats;
    spinlock_t              mac_lock;
    struct timer_list       mac_oom_timer;
    struct work_struct      mac_tx_timeout;
    struct net_device_stats mac_net_stats;
    athr_phy_speed_t        mac_speed;
    athr_phy_speed_t        forced_speed;
    int                     mac_fdx;
    struct timer_list       mac_phy_timer;
    athr_gmac_trc_t         tb;
    uint32_t                mac_flags;
    uint8_t                 dma_check;
    uint8_t		    rx_dma_check;
    struct ath_qops         *qos;
    athr_phy_ops_t          *phy;
    char                    led_name[ATHRS_LED_MAX_NAME_LEN];
    struct led_classdev     led_dev;
// DTHACK - Just include these fields
// #if defined (CONFIG_MACH_QCA955x)
    athr_gmac_mii_phy_t     mii_phy;
    athr_gmac_mii_intf_t    mii_intf;  
    struct work_struct      mac_sgmii_intr;
// #endif
    athr_gmac_ops_t         *ops;
    ath_led_ctrl_t          ledctrl;
#ifdef __linux__
    struct work_struct      led_event;
#endif  
    unsigned long           dma_chk_ts;
    unsigned long           led_blink_ts;
    unsigned long           link_ts;
    unsigned long           timer_freq;
    unsigned long           link_up;
    unsigned long           eth_overfl_cntr;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    struct napi_struct      napi;
#endif
    unsigned                num_tx_desc,
                            num_rx_desc,
                            reap_thresh,
                            qstart_thresh;
    struct tq_struct        rxtq;        /* rx intr tasklet */
    struct tq_struct        txreaptq;    /* tx reap tasklet */
    uint8_t                 tx_ac_bt;    /* Tx access control bit map */
    uint32_t                tx_ac_drops;
    uint32_t                push_dur;
#ifdef CONFIG_ATHRS_HW_ACCELS
    athr_hw_accels_t        *hw_accels;
#endif
#if defined(CONFIG_FLOWMAC) || defined(CONFIG_FLOWMAC_MODULE)
    void                    *flow_mac_handle;
    u_int8_t                mac_flowcntrl_enable;
#endif
#if defined(CONFIG_ATHRS16_PHY) || defined(CONFIG_AR7242_S16_PHY)
    struct work_struct      mac_reset_wqueue;
#endif 
    u_int32_t		    intr_mask;
    char		    *mac_addr;
    u_int32_t		    ddr_flush;
    u_int32_t		    mac_tx_ring_full;
}athr_gmac_t;



#if defined(CONFIG_ATHR_VLAN_IGMP)
extern void athr_filter_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb);
extern void athr_add_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb);
#endif

#define net_rx_packets      mac_net_stats.rx_packets
#define net_rx_fifo_errors  mac_net_stats.rx_fifo_errors
#define net_tx_packets      mac_net_stats.tx_packets
#define net_rx_bytes        mac_net_stats.rx_bytes
#define net_tx_bytes        mac_net_stats.tx_bytes
#define net_rx_over_errors  mac_net_stats.rx_over_errors
#define net_tx_dropped      mac_net_stats.tx_dropped;

#define athr_gmac_dev_up(_dev)                                     \
    (((_dev)->flags & (IFF_RUNNING|IFF_UP)) != (IFF_RUNNING|IFF_UP))

typedef enum {
    ATHR_GMAC_RX_STATUS_DONE,
    ATHR_GMAC_RX_STATUS_NOT_DONE,
    ATHR_GMAC_RX_STATUS_OOM,
}athr_gmac_rx_status_t;


uint16_t athr_gmac_mii_read(int unit, uint32_t phy_addr, uint8_t reg);
void athr_gmac_mii_write(int unit, uint32_t phy_addr, uint8_t reg, uint16_t data);
extern char *spd_str[];
extern char *dup_str[];

extern int ar7240_gmac_attach(void *);
extern int ar934x_gmac_attach(void *);
extern int qca955x_gmac_attach(void *);

/*
 * The list of valid SOC in soc_has_ag7240() MUST match
 * the list of valid SOC in athr_gmac_attach() (following)
 */

static inline int
soc_has_ag7240(void) {
    return (soc_is_ar724x() || soc_is_ar933x() || soc_is_ar934x());
}

static inline int 
athr_gmac_attach(athr_gmac_t *mac)
{
    if (soc_is_ar724x() || soc_is_ar933x())
	return (ar7240_gmac_attach(mac));

#ifdef CONFIG_SOC_AR934X
    if (soc_is_ar934x())
	return (ar934x_gmac_attach(mac));
#endif

#ifdef CONFIG_SOC_QCA955X
    if (soc_is_qca955x())
	return (qca955x_gmac_attach(mac));
#endif

    return 0;
}

static inline int
mac_has_flag(athr_gmac_t *mac, uint32_t flag)
{
      return ((mac->mac_flags & flag) != 0);
}

static inline void
mac_set_flag(athr_gmac_t *mac, uint32_t flag)
{
    mac->mac_flags |= flag;
    return;
    
}

static inline void
mac_clear_flag(athr_gmac_t *mac, uint32_t flag)
{
    mac->mac_flags &= ~flag;
    return;
}

static inline uint32_t
athr_gmac_get_diff(uint32_t t1,uint32_t t2)
{
    return (t1 > t2 ? (0xffffffff - (t1 - t2)) : t2 - t1);
}

//for use with the generic mii ioctls
struct mii_ioctl_data {
    uint16_t phy_id;
    uint16_t reg_num;
    uint16_t val_in;
    uint16_t val_out;
};

static inline int
athrs_do_ioctl(struct net_device *dev,struct ifreq *ifr, int cmd)
{
    int ret = -EPERM;
    struct mii_ioctl_data* md;
    athr_gmac_t *mac = (athr_gmac_t*) ATHR_MAC_PRIV(dev);

    md = (struct mii_ioctl_data*)(&ifr->ifr_data);

    switch (cmd){
        case SIOCGMIIREG:
            md->val_out = (uint16_t) mac->phy->read_phy_reg(mac->mac_unit,
			md->phy_id, md->reg_num);
            return 0;
            break;
        case SIOCSMIIREG:
            mac->phy->write_phy_reg(mac->mac_unit,
			md->phy_id, md->reg_num, md->val_in);
            return 0;
            break;
        case ATHR_GMAC_CTRL_IOC:
            ret = athr_gmac_ctrl(dev,ifr,cmd);
            break;
        case ATHR_PHY_CTRL_IOC:
            ret = athr_phy_ctrl(dev,ifr,cmd);
            break;
#if defined (CONFIG_ATHRS_QOS) && !defined(CONFIG_AG7240_ATHRS17_PHY)
        case ATHR_GMAC_QOS_CTRL_IOC:
            ret = athr_config_qos(ifr->ifr_data,cmd);
            break;
#endif
#ifdef CONFIG_ATHR_VLAN_IGMP
        case ATHR_VLAN_IGMP_IOC:
           ret = athr_vlan_ctrls(dev,ifr,cmd);
           break;
#endif
#ifdef CONFIG_ATHRS_HW_ACL
        case ATHR_HW_ACL_IOC:
           ret = athr_hw_acl_config(ifr->ifr_data, cmd, dev);
           break;
#endif
        default:
           break;
    }
  
  return ret;
}

static inline int
athr_register_qos(void *mac)
{
#if defined (CONFIG_ATHRS_QOS) && !defined(CONFIG_AG7240_ATHRS17_PHY)
    set_mac_qops(mac);
#endif
    return 0;
}

static inline int
athr_register_vlan_igmp(void *mac)
{
#if (defined (CONFIG_ATHRS_QOS) || defined(CONFIG_ATHR_VLAN_IGMP)) && !defined(CONFIG_AG7240_ATHRS17_PHY)
    athr_set_vlan_phy_ops(mac);
#endif
    return 0;
}
static inline int 
athr_plc_dmac_filter(athr_gmac_t *mac, struct sk_buff *skb)
{
#ifdef HYBRID_PLC_FILTER
	athr_plc_dmac_change(mac, skb);
#endif
    return 0;
}

static inline int 
athr_vlan_igmp_filter(athr_gmac_t *mac, struct sk_buff *skb)
{
#if defined(CONFIG_ATHR_VLAN_IGMP)
    athr_filter_igmp_pkg(mac, skb);
#endif
    return 0;
}
static inline int 
athr_vlan_igmp_add(athr_gmac_t *mac, struct sk_buff *skb)
{
#if defined(CONFIG_ATHR_VLAN_IGMP)
    athr_add_igmp_pkg(mac, skb);
#endif
    return 0;
}

#if !defined(ATHR_HEADER_LEN) || !defined(HEADER_LEN)
#define ATHR_HEADER_LEN 2

typedef enum {
    ATHR_NORMAL_PKT,
    ATHR_RES,
    ATHR_MIB_1ST,
    ATHR_RES1,
    ATHR_RES2,
    ATHR_RDWR_REG,
    ATHR_RDWR_REG_ACK,
    ATHR_RES3
} ATHR_HEADER_TYPE;
#endif

#ifdef CONFIG_ATHR_DESC_SRAM
typedef struct {
    athr_desc_type flag;
    athr_gmac_desc_t *ring_desc;
    dma_addr_t ring_desc_dma;
}_athr_elemt;

typedef struct {
    _athr_elemt desc[2];
}_athr_sram_map;


static inline void
athr_gmac_desc_alloc(athr_gmac_t *mac, athr_gmac_ring_t *r, int count, athr_desc_type flag)
{
     volatile uint8_t *s_ram = (volatile uint8_t *)ATHR_SRAM_START;
     static int sram_desc_cnt = 0;
     static _athr_sram_map athr_sram_map[] = { 
         {{{ATHR_NONE, NULL, 0}, {ATHR_NONE, NULL, 0}}},
         {{{ATHR_NONE, NULL, 0}, {ATHR_NONE, NULL, 0}}}
     };
     _athr_elemt *athr_elemt;

     assert(!r->ring_desc);
     athr_elemt = &athr_sram_map[mac->mac_unit].desc[flag];
     if(athr_elemt->flag != ATHR_NONE){
         r->ring_desc = athr_elemt->ring_desc;
         r->ring_desc_dma = athr_elemt->ring_desc_dma;
     }else{
         r->ring_desc  = (athr_gmac_desc_t  *)(s_ram + sram_desc_cnt);
         r->ring_desc_dma = ((dma_addr_t) (s_ram + sram_desc_cnt) & 0x1fffffff);
         athr_elemt->ring_desc = r->ring_desc;
         athr_elemt->ring_desc_dma = r->ring_desc_dma;
         athr_elemt->flag = flag;

         sram_desc_cnt += (sizeof(athr_gmac_desc_t) * count);
     }
     printk("sram_desc_cnt %d,mac Unit %d,%s r->ring_desc 0x%p\n", sram_desc_cnt, mac->mac_unit, flag==ATHR_RX?"Rx":"Tx", r->ring_desc);
     assert(r->ring_desc);
}

static inline void
athr_gmac_ring_free(athr_gmac_ring_t *r)
{
    kfree(r->ring_buffer);
    printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) r->ring_buffer);
}

#else
static inline void
athr_gmac_desc_alloc(athr_gmac_t *mac, athr_gmac_ring_t *r,int count, athr_desc_type flag)
{
    r->ring_desc  =  (athr_gmac_desc_t *)dma_alloc_coherent(NULL,
        sizeof(athr_gmac_buffer_t) * count,
        &r->ring_desc_dma,
        GFP_DMA);
}

static inline void
athr_gmac_ring_free(athr_gmac_ring_t *r)
{
    dma_free_coherent(NULL, sizeof(athr_gmac_desc_t)*r->ring_nelem, r->ring_desc,
              r->ring_desc_dma);
    kfree(r->ring_buffer);
    printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) r->ring_buffer);
}

#endif //CONFIG_ATHR_DESC_SRAM


/*
 *Based on the soc defines define the function
 *and implement in the soc specific file. 
 */

#define athr_poll_link(_mac)    (_mac->ops->poll_link(mac))

extern int athr_gmac_poll_link(void *);

#endif //_ATHR_GMAC_H
