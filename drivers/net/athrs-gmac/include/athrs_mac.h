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


#include "athrs_trc.h"
#include "athrs_ioctl.h"
#include "athrs_phy.h"
#include "osdep.h"
#include "athrs_config.h"    
#include "athr_pb.h"
#include "athrs_hw_accels.h"

#ifdef CONFIG_ATHRS_QOS
extern int set_mac_qops(void *mac);
#endif

#if defined (CONFIG_ATHRS_QOS) || defined(CONFIG_ATHR_VLAN_IGMP)
extern void athr_set_vlan_phy_ops(void *arg);
extern int athrs_config_qos(struct eth_cfg_params *ethcfg, int cmd);
#endif

#define pl() printk("%s:%d\n",__func__,__LINE__)

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

#define HDR_PACKET_TYPE_MASK    0x0F            
#define HDR_PRIORITY_SHIFT      0x4
#define HDR_PRIORITY_MASK       0x3
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

#define AC_TO_ENET_TOS(ac) ( \
     (ac == 3) ? 6 : (ac == 2) ? 4: \
     (ac == 1) ? 2 : (ac == 0) ? 3 : 0)


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
#define ATHR_JUMBO_FR           0x00800000

//#define ATHR_GMAC_DEBUG 1

#ifdef ATHR_GMAC_DEBUG
#define DPRINTF(_fmt,...) do {         \
            printk("GMAC:"_fmt, __VA_ARGS__);      \
} while (0)
#else
#define DPRINTF(_fmt,...)
#endif

#define mii_reg(_mac)   (ATHR_MII0_CTRL + ((_mac)->mac_unit * 4))
#define mii_if(_mac)    (((_mac)->mac_unit == 0) ? mii0_if : mii1_if)
#define phy_reg_read    athr_gmac_mii_read
#define phy_reg_write   athr_gmac_mii_write

#define athr_gmac_set_mii_ctrl_speed(_mac, _spd)   do {                        \
    athr_reg_rmw_clear(mii_reg(_mac), (3 << 4));                          \
    athr_reg_rmw_set(mii_reg(_mac), ((_spd) << 4));                       \
}while(0);

/*
 * IP needs 16 bit alignment. But RX DMA needs 4 bit alignment. We sacrifice IP
 * Plus Reserve extra head room for wmac
 */
#define ETHERNET_FCS_SIZE            4
#define ATHR_GMAC_RX_RESERVE           (64)
#define ATHR_GMAC_RX_BUF_SIZE      \
    (ATHR_GMAC_RX_RESERVE + ETH_HLEN + ETH_FRAME_LEN + ETHERNET_FCS_SIZE)


#define athr_gmac_base(_no)    (_no) ? ATHR_GE1_BASE    : ATHR_GE0_BASE
#define athr_gmac_irq(_no)     (_no) ? ATHR_CPU_IRQ_GE1 : ATHR_CPU_IRQ_GE0

#define athr_gmac_reset_mask(_no) (_no) ? (ATHR_RESET_GE1_MAC)   \
                                     : (ATHR_RESET_GE0_MAC)  

#define athr_gmac_unit2mac(_unit)     athr_gmacs[(_unit)]
#define athr_gmac_name2mac(name)      \
    strcmp(name,"eth0") ? athr_gmac_unit2mac(1) : athr_gmac_unit2mac(0)

#define assert(_cond)   do {                                     \
    if(!(_cond)) {                                               \
        athr_gmac_trc_dump();                                       \
        printk("%s:%d: assertion failed\n", __func__, __LINE__); \
        BUG();                                                   \
    }                                                            \
}while(0);
/*
 * Everything but TX
 */
#define ATHR_GMAC_INTR_MASK    (ATHR_GMAC_INTR_RX |  \
                             ATHR_GMAC_INTR_RX_BUS_ERROR |             \
                             ATHR_GMAC_INTR_TX_BUS_ERROR              \
                             /*| ATHR_GMAC_INTR_TX_URN | ATHR_GMAC_INTR_TX*/)

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
    case ENET_AC_BE:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q0, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_BK:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q1, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_VI:                           \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q2, ATHR_GMAC_TXE);  \
        break;                             \
    case ENET_AC_VO:                               \
        athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q3, ATHR_GMAC_TXE);  \
        break;                             \
}                                       

#define athr_gmac_tx_start(_mac)                                       \
            athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL_Q0, ATHR_GMAC_TXE) 

#define athr_gmac_tx_stop(_mac)                        \
    athr_gmac_reg_wr((_mac), ATHR_GMAC_DMA_TX_CTRL, 0)

#define athr_gmac_ring_incr(_idx)                                             \
        if(unlikely(++(_idx) == r->ring_nelem)) (_idx) = 0;

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
    athr_gmac_reg_wr(_mac, ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_MASK)

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
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_DMA_INTR_MASK,ATHR_GMAC_INTR_RX );

#define athr_gmac_intr_enable_rxovf(_mac)                                  \
    athr_gmac_reg_rmw_set((_mac), ATHR_GMAC_DMA_INTR_MASK, ATHR_GMAC_INTR_RX_OVF);

#define athr_gmac_intr_disable_rxovf(_mac)                                 \
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_DMA_INTR_MASK,(ATHR_GMAC_INTR_RX_OVF));

#define athr_gmac_intr_enable_recv(_mac)                                   \
    athr_gmac_reg_rmw_set(mac, ATHR_GMAC_DMA_INTR_MASK,ATHR_GMAC_INTR_RX);


/* 
 * Timers 
 */

#define ATH_GMAC_POLL_LINK_FREQ (2 * HZ)
#define ATH_GMAC_CHECK_DMA_FREQ (HZ / 2)
#define ATH_GMAC_LED_BLINK_FREQ (HZ / 10)

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
    uint32_t    res3           :  2;
    uint32_t    pkt_size       : 14;

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
    void                *flow_mac_handle;
#endif
}athr_gmac_t;


#if defined(CONFIG_ATHR_VLAN_IGMP)
extern void athr_filter_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb);
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

static inline int 
athr_gmac_attach(athr_gmac_t *mac)
{
#ifdef CONFIG_MACH_AR7100
   return (ar7100_gmac_attach(mac));
#endif
#if defined(CONFIG_MACH_AR7240)||defined(CONFIG_MACH_HORNET) 
  return (ar7240_gmac_attach(mac));
#endif
#if defined(CONFIG_SOC_AR934x)
  return (ar934x_gmac_attach(mac));
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
    struct eth_cfg_params *ethcfg;
    struct mii_ioctl_data* md;
    athr_gmac_t *mac = (athr_gmac_t*) ATHR_MAC_PRIV(dev);

    ethcfg = (struct eth_cfg_params *)ifr->ifr_data;
    md = (struct mii_ioctl_data*)(&ifr->ifr_data);

    switch (cmd){
        case SIOCGMIIREG:
            md->val_out = (uint16_t) mac->phy->read_phy_reg(0, md->phy_id, md->reg_num);
            return 0;
            break;
        case SIOCSMIIREG:
            mac->phy->write_phy_reg(0, md->phy_id, md->reg_num, md->val_in);
            return 0;
            break;
        case ATHR_GMAC_CTRL_IOC:
            ret = athr_gmac_ctrl(dev,ifr,cmd);
            break;
        case ATHR_PHY_CTRL_IOC:
            ret = athr_phy_ctrl(dev,ifr,cmd);
            break;
#ifdef CONFIG_ATHRS_QOS
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
#ifdef CONFIG_ATHRS_QOS
    set_mac_qops(mac);
#endif
    return 0;
}

static inline int
athr_register_vlan_igmp(void *mac)
{
#if defined (CONFIG_ATHRS_QOS) || defined(CONFIG_ATHR_VLAN_IGMP)
    athr_set_vlan_phy_ops(mac);
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

static inline void
athr_gmac_desc_alloc(athr_gmac_ring_t *r,int count)
{
     volatile uint8_t *s_ram = (volatile uint8_t *)ATHR_SRAM_START;
     static int sram_desc_cnt = 0;

     assert(!r->ring_desc);

     r->ring_desc  = (athr_gmac_desc_t  *)(s_ram + sram_desc_cnt);
     r->ring_desc_dma = ((dma_addr_t) (s_ram + sram_desc_cnt) & 0x1fffffff);

     sram_desc_cnt += (sizeof(athr_gmac_desc_t) * count);
     assert(r->ring_desc);
}
#else
static inline void
athr_gmac_desc_alloc(athr_gmac_ring_t *r,int count)
{
        r->ring_desc  =  (athr_gmac_desc_t *)dma_alloc_coherent(NULL,
        sizeof(athr_gmac_buffer_t) * count,
        &r->ring_desc_dma,
        GFP_DMA);


}
#endif //CONFIG_ATHR_DESC_SRAM

#endif //_ATHR_GMAC_H
