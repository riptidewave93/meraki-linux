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

#ifndef _GMAC_DEFS_H
#define _GMAC_DEFS_H

#include "atheros.h"
#include "athrs_mac.h"
#include "athrs_phy.h"

#define CONFIG_ATHR_GMAC_LOCATION            CONFIG_AG7240_MAC_LOCATION

#define CONFIG_ATHR_GMAC_LEN_PER_TX_DS       CONFIG_AG7240_LEN_PER_TX_DS

#define CONFIG_ATHR_GMAC_NUMBER_TX_PKTS      CONFIG_AG7240_NUMBER_TX_PKTS
#define CONFIG_ATHR_GMAC_NUMBER_RX_PKTS      CONFIG_AG7240_NUMBER_RX_PKTS

#if ATHR_GMAC_NMACS > 1
#define CONFIG_ATHR_GMAC_NUMBER_TX_PKTS_1    CONFIG_AG7240_NUMBER_TX_PKTS_1
#define CONFIG_ATHR_GMAC_NUMBER_RX_PKTS_1    CONFIG_AG7240_NUMBER_RX_PKTS_1
#else
#define CONFIG_ATHR_GMAC_NUMBER_TX_PKTS_1
#define CONFIG_ATHR_GMAC_NUMBER_RX_PKTS_1
#endif



/*
 * Function definitions. 
 */

extern athr_gmac_t *athr_gmacs[];

/*
 * Externs
 */

extern void athr_gmac_timer_init(athr_gmac_t *mac);
extern void athr_gmac_timer_del(athr_gmac_t *mac);

#ifdef CONFIG_ATHRS_HW_NAT
extern void athr_nat_process_ingress_pkts(int unit,
                                          struct sk_buff *skb,
                                          athr_gmac_desc_t *ds);
extern void athr_nat_process_egress_pkts(int unit,
                                         struct sk_buff *skb,
                                       athr_gmac_desc_t *ds);
#else

#define athr_nat_process_ingress_pkts(a, b, c)  /* Do nothing */
#define athr_nat_process_egress_pkts(a,b,c)     /* Do nothing */

#endif /* CONFIG_ATHRS_HW_NAT */

#ifdef CONFIG_ATHRS_HW_ACL

extern void athr_acl_enable_tx(athr_gmac_t *mac,athr_gmac_desc_t *ds);

#else

#define athr_acl_enable_tx(...)

#endif /* CONFIG_ATHRS_HW_ACL */



#ifdef CONFIG_ATHRS17_HNAT

extern void aos_header_arp_in(struct sk_buff *skb);

void athr_ssdk_process_arp_header(athr_gmac_t *mac, struct sk_buff *skb)
{
    if (mac->mac_unit == 0)
    {
        aos_header_arp_in(skb);
    }
}

#else

#define athr_ssdk_process_arp_header(a,b) /* do nothing */

#endif /* CONFIG_ATHRS17_HNAT */

/*
 * Defines specific to this implemention
 */

#define addr_to_words(addr, w1, w2)  {                                 \
    w1 = (addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]; \
    w2 = (addr[4] << 24) | (addr[5] << 16) | 0;                        \
}

#ifndef CONFIG_ATHR_GMAC_LEN_PER_TX_DS
#error Please run menuconfig and define CONFIG_ATHR_GMAC_LEN_PER_TX_DS
#endif

#ifndef CONFIG_ATHR_GMAC_NUMBER_TX_PKTS
#error Please run menuconfig and define CONFIG_ATHR_GMAC_NUMBER_TX_PKTS
#endif

#ifndef CONFIG_ATHR_GMAC_NUMBER_RX_PKTS
#error Please run menuconfig and define CONFIG_ATHR_GMAC_NUMBER_RX_PKTS
#endif
#define ATHR_GMAC_TX_FIFO_LEN          2048
#define ATHR_GMAC_TX_MIN_DS_LEN        128
#define ATHR_GMAC_TX_MAX_DS_LEN        ATHR_GMAC_TX_FIFO_LEN

#define ATHR_GMAC_TX_MTU_LEN           1536

#define ATHR_GMAC_NAPI_WEIGHT          64
#define ATHR_PHY_POLL_SECONDS     2
#define ATHR_GMAC_LED_POLL_SECONDS    (HZ/10)

#define ENET_UNIT_LAN 1
#define ENET_UNIT_WAN 0

#define ATHR_GMAC_PORT_ON  1
#define ATHR_GMAC_PORT_OFF 0

#define SW_PLL 0x1f000000ul

/*
 * Externs
 */

extern uint32_t ar7240_ahb_freq;
extern void ar7240_s26_intr(void);

#ifdef ATH_CAL_NAND_FLASH
/* NAND FLASH Read method */
extern int ath_nand_local_read(u_char *cal_part,loff_t from, size_t len,
		size_t *retlen, u_char *buf);

#define OS_NAND_FLASH_READ(_calpart, _addr, _len, _retlen, _buf)     \
           ath_nand_local_read((u_char *)_calpart, _addr,(size_t)_len,(size_t *)_retlen, _buf)
#endif

/*
 * Inline functions
 */

static inline int
athr_gmac_ndesc_unused(athr_gmac_t *mac, athr_gmac_ring_t *ring)
{
    int head = ring->ring_head, tail = ring->ring_tail;

    return ((tail > head ? 0 : ring->ring_nelem) + tail - head);
}

static inline int athr_gmac_rx_ring_full(athr_gmac_t *mac)
{
    athr_gmac_ring_t *r    = &mac->mac_rxring;

    /*
     * Instead of inspecting whether head == tail means empty or full here,
     * actually just count the number of remaining slots in the ring.  Since
     * a packet might be split into multiple descriptors, we want to ensure
     * not only that there are enough in the ring but that there is one more
     * so that head and tail never collide.  If any of the following
     * #defines change, then the threshold below must be updated.
     *
     *     ATHR_GMAC_MAX_PKTLEN
     *     ATHR_GMAC_TX_FIFO_LEN
     *     ATHR_GMAC_TX_MAX_DS_LEN
     *     ATHR_GMAC_TX_MTU_LEN
     *     CONFIG_QCA955X_MAX_LEN_PER_TX_DS
     */

    return athr_gmac_ndesc_unused(mac, r) <= 2;
}

static inline void athr_gmac_set_mac_duplex(athr_gmac_t *mac, int fdx)
{
    if (fdx) {
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2, ATHR_GMAC_CFG2_FDX);
    }
    else {
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG2, ATHR_GMAC_CFG2_FDX);
    }
}

static inline void athr_gmac_set_mac_if(athr_gmac_t *mac, int is_1000)
{
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG2, (ATHR_GMAC_CFG2_IF_1000|
                         ATHR_GMAC_CFG2_IF_10_100));
    if (is_1000) {
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2, ATHR_GMAC_CFG2_IF_1000);
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_FIFO_CFG_5, ATHR_GMAC_BYTE_PER_CLK_EN);
    }
    else {
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2, ATHR_GMAC_CFG2_IF_10_100);
        athr_gmac_reg_rmw_clear(mac,ATHR_GMAC_FIFO_CFG_5, ATHR_GMAC_BYTE_PER_CLK_EN);
    }
}

static inline void athr_gmac_set_mac_speed(athr_gmac_t *mac, int is100)
{
    if (is100) {
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_IFCTL, ATHR_GMAC_IFCTL_SPEED);
    }
    else {
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_IFCTL, ATHR_GMAC_IFCTL_SPEED);
    }
}

static inline int athr_gmac_tx_reap_thresh(athr_gmac_t *mac,int ac)
{
    athr_gmac_ring_t *r = &mac->mac_txring[ac];

    return (athr_gmac_ndesc_unused(mac, r) < mac->reap_thresh);
}

static inline void athr_get_gmac_addr(athr_gmac_t *mac,u8 **mac_addr)
{
    *mac_addr = mac->mac_addr;
}

static int ATHR_MAC_POLL();

static irqreturn_t athr_gmac_intr(ATHR_MAC_ISR_ARGS);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
static struct net_device_ops athr_gmac_net_ops;
#endif

static void athr_gmac_tx_timeout_task(ATHR_MAC_TASK_ARG);

static int ATHR_MAC_POLL();

static irqreturn_t athr_gmac_intr(ATHR_MAC_ISR_ARGS);

#define ATH_INIT_TX_TASK()  							\
	ATH_INIT_TQUEUE(&mac->txreaptq,athr_gmac_txreap_task,mac);

#ifdef CONFIG_ATHR_RX_TASK

#define ATHR_TASK_DEFS() 							\
    athr_gmac_t            *mac  = (athr_gmac_t *)data; 			\
    struct net_device *dev  = mac->mac_dev; 					\
    int quota = CONFIG_ATHR_RX_PKTS_CNT;					\
    int *work_done = NULL;

#define athr_receive_pkt()  							\
	static void 								\
	athr_gmac_recv_packets(TQUEUE_ARG data)

#define ATH_INIT_RX_TASK()  							\
	ATH_INIT_TQUEUE(&mac->rxtq,athr_gmac_recv_packets,mac);

#define athr_gmac_rx_pkt() 							\
	max_work = 0;								\
	dev = NULL; 	

#define GMAC_RX_STATUS_OOM  /* norhing */
#define RX_RETURN() /* do nothing */

#else 

#define CONFIG_ATHR_RX_PKTS_CNT		0
#define ATHR_TASK_DEFS() 							\

#define athr_receive_pkt()  							\
	static int 								\
	athr_gmac_recv_packets (struct net_device *dev, athr_gmac_t *mac, 	\
	    int quota, int *work_done)

#define athr_gmac_rx_pkt() 							\
         athr_gmac_recv_packets(dev, mac, max_work, &work_done);

#define GMAC_RX_STATUS_OOM ATHR_GMAC_RX_STATUS_OOM

#define RX_RETURN()  return ret

#define ATH_INIT_RX_TASK()  /* do nothing */

#endif /*CONFIG_ATHR_RX_TASK */

#ifdef CONFIG_ATHR_PHY_SWAP
#define athr_swap_phy()   							\
	athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_PHY_SWAP);
#define athr_gmac_force_check_link()					\
        athr_gmac_check_link(mac, 0);
#else
#define athr_swap_phy() /* do nothing */
#define athr_gmac_force_check_link()					\
        athr_gmac_check_link(mac, 4);
#endif

#ifdef CONFIG_ATHRS_QOS
#if defined (CONFIG_AG7240_ATHRS17_PHY)
void qca_gmac_rx_qos(athr_gmac_t *mac, struct sk_buff *skb);
void qca_gmac_tx_qos(athr_gmac_t *mac,struct sk_buff *skb);
int qca_gmac_check_qos_bt(athr_gmac_t *mac,struct sk_buff *skb);

#define athr_gmac_tx_qos(a,b) qca_gmac_tx_qos(a,b)
#define athr_gmac_check_qos_bt(a,b) qca_gmac_check_qos_bt(a,b)
#define athr_gmac_rx_qos(a,b) qca_gmac_rx_qos(a,b)
#else
#include "athrs_qos_def.h"
#endif
#else
#define athr_gmac_tx_qos(a,b) /* nothing */
#define athr_gmac_check_qos_bt(a,b) 0
#define athr_gmac_rx_qos(a,b) /* nothing */

#endif //CONFIG_ATHRS_QOS

#ifdef CONFIG_ATHR_VLAN_IGMP
#define IF_ATHR_SKB_LEN_CHECK()				\
    if(unlikely((skb->len <= 0) || 			\
	(skb->len > (dev->mtu + ETH_VLAN_HLEN +6 ))))	

static inline void
athr_gmac_vlan_igmp(athr_gmac_t *mac, struct sk_buff *skb)
{
    athr_vlan_igmp_filter(mac, skb);
     /*
      * For S26 Switch, if vlan_id == 0, we will remove the tag header.
      */
     if(mac->mac_unit==1)
     {
         athr_remove_hdr(mac,skb);
         if((skb->data[12]==0x81)&&(skb->data[13]==0x00)&&
           ((skb->data[14]&0xf)==0x00)&&(skb->data[15]==0x00)) {
             memmove(&skb->data[4],&skb->data[0],12);
             skb_pull(skb,4);
         }
     }

}
static inline void
athr_gmac_vlan_igmp_tx(athr_gmac_t *mac, struct sk_buff *skb)
{
    athr_vlan_igmp_add(mac, skb);
}

#else
#define athr_gmac_vlan_igmp(a,b) /* nothing */
#define athr_gmac_vlan_igmp_tx(a,b) /* nothing */
#define IF_ATHR_SKB_LEN_CHECK()				\
    if(unlikely((skb->len <= 0) || 			\
	(skb->len > (dev->mtu + ETH_HLEN + 4))))	
#endif


#ifdef CONFIG_AR7240_PLC_FILTER
static inline
int athr_gmac_plc_filter(struct sk_buff *skb, athr_gmac_t *mac)
{
    int ret = 0;
    struct ethhdr *eh;
    unsigned char *int_oui;
    unsigned short ethertype;
    unsigned short mmtype;
    unsigned char plc_mac[] = {0x00, 0xB0, 0x52, 0x00, 0x00, 0x01};

    /* Only filter... */
    if(mac->mac_unit != 1) goto out;

    eh = (struct ethhdr *) skb->data;

    if(unlikely(!memcmp(plc_mac, eh->h_source, ETH_ALEN) ||
                !memcmp(plc_mac, eh->h_dest, ETH_ALEN))) {
        /* Check VLAN tag */
        if(unlikely(skb->data[0x0C] == 0x81 && skb->data[0x0D] == 0x00)) {
            ethertype = *(unsigned short *)(skb->data + 0x10);
            int_oui = skb->data + 0x15;
            mmtype = *(unsigned short *)(skb->data + 0x13);
        } else {
            ethertype = eh->h_proto;
            int_oui = skb->data + 0x11;
            mmtype = *(unsigned short *)(skb->data + 0x0f);
        }

        if(likely(ethertype == __constant_htons(0x88E1)) &&
                !(memcmp(plc_mac, int_oui, ETH_ALEN >> 1)) &&
                (__constant_cpu_to_le16(0xa060) == (mmtype & ~(0x11)))) {
            printk(KERN_DEBUG "PLC filter condition match\n");
            ret = 1;
        }
    }
out:
    return ret;
}
#else
#define athr_gmac_plc_filter(x,y) 0
#endif

athr_receive_pkt(); /* prototype  */

static int inline athr_get_portno(int phyUnit)
{
#ifdef CONFIG_ATHR_PHY_SWAP
    if (phyUnit == 4)
        return 1;
    else if(phyUnit == 0)
        return 5;
    else
        return (phyUnit +1);
#else
    return ((phyUnit +1));
#endif
}
#endif
