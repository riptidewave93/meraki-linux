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

#include <linux/stddef.h>
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
#include <linux/igmp.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <net/sch_generic.h>
#include <linux/if_pppox.h>
#include <linux/ip.h>
#include <net/checksum.h>

#include "qca955x_mac.h"
#include "955x.h"

#include "osdep.h"

#include "athrs_trc.h"
#include "athrs_mac_def.h"
#include "athrs_flowmac.h"

#define MODULE_NAME "qca955x_GMAC"
MODULE_LICENSE("Dual BSD/GPL");

extern int flowmac_on;

extern int frame_sz;

extern int phy_in_reset;

static void athr_gmac_buffer_free(struct sk_buff *skb);
extern void serdes_pll_lock_detect_st(void);
extern void mdio_init_device(void);
extern void athr_gmac_sgmii_intr(ATHR_MAC_TASK_ARG);

static unsigned athr_tx_desc_num[ATHR_GMAC_NMACS] =
	{ CONFIG_ATHR_GMAC_NUMBER_TX_PKTS, CONFIG_ATHR_GMAC_NUMBER_TX_PKTS_1 };
static unsigned athr_rx_desc_num[ATHR_GMAC_NMACS] =
	{ CONFIG_ATHR_GMAC_NUMBER_RX_PKTS, CONFIG_ATHR_GMAC_NUMBER_RX_PKTS_1 };
static void athr_gmac_hw_stop(athr_gmac_t *mac);
static void athr_gmac_oom_timer(unsigned long data);
int  athr_gmac_check_link(void *arg,int phyUnit);
int  ag7242_check_link(athr_gmac_t *mac);
static int  athr_gmac_tx_alloc(athr_gmac_t *mac);
static int  athr_gmac_rx_alloc(athr_gmac_t *mac);
static void athr_gmac_rx_free(athr_gmac_t *mac);
static void athr_gmac_tx_free(athr_gmac_t *mac);
static int  athr_gmac_ring_alloc(athr_gmac_t *mac, athr_gmac_ring_t *r, int count, athr_desc_type flag);
static int  athr_gmac_rx_replenish(athr_gmac_t *mac);
static void athr_gmac_get_default_macaddr(athr_gmac_t *mac, u8 *mac_addr);
static int  athr_gmac_tx_reap(athr_gmac_t *mac,int ac);
static void athr_gmac_ring_release(athr_gmac_t *mac, athr_gmac_ring_t  *r);
static void athr_gmac_ring_free(athr_gmac_ring_t *r);
static irqreturn_t athr_gmac_link_intr(ATHR_MAC_ISR_ARGS);
#ifdef CONFIG_ATHR_VLAN_IGMP
extern void athr_remove_hdr(athr_gmac_t *mac, struct sk_buff *skb);
#define ETH_VLAN_HLEN 18
#endif
  
static struct sk_buff * athr_gmac_buffer_alloc(void);

static int tx_len_per_ds = 0;
static int tx_max_desc_per_ds_pkt=0;

static inline int athr_gmac_tx_ring_full(athr_gmac_t *mac,int ac)
{
    athr_gmac_ring_t *r = &mac->mac_txring[ac];

    athr_gmac_trc_new(athr_gmac_ndesc_unused(mac, r),"tx ring full");
    if (athr_gmac_ndesc_unused(mac, r) < tx_max_desc_per_ds_pkt + 2)
	mac->mac_tx_ring_full++;
    return (athr_gmac_ndesc_unused(mac, r) < tx_max_desc_per_ds_pkt + 2);
}

static inline athr_gmac_desc_t *
athr_gmac_get_tx_ds(athr_gmac_t *mac, int *len, unsigned char **start,int ac)
{
    athr_gmac_desc_t      *ds;
    int                len_this_ds;
    athr_gmac_ring_t      *r   = &mac->mac_txring[ac];
    athr_gmac_buffer_t    *bp;

    /* force extra pkt if remainder less than 4 bytes */
#ifdef ATHR_GMAC_SUPPORT_MULTIPLE_DS
    if (*len > tx_len_per_ds)
        if (*len < (tx_len_per_ds + 4))
            len_this_ds = tx_len_per_ds - 4;
        else
            len_this_ds = tx_len_per_ds;
    else
        len_this_ds    = *len;

    *len   -= len_this_ds;
#else
    len_this_ds    = *len;
    *len   = 0;
#endif

    ds = &r->ring_desc[r->ring_head];

    athr_gmac_trc_new(ds,"ds addr");
    athr_gmac_trc_new(ds,"ds len");
    assert(!athr_gmac_tx_owned_by_dma(ds));
    
    if (mac_has_flag(mac,ATHR_JUMBO_FR) && !soc_is_ar7240()) {
        ds->pkt_size = len_this_ds & 0xfff;
        ds->res3     = ((len_this_ds >> 12) & 0x0f);
    }
    else {
        ds->pkt_size = len_this_ds;
    }
    ds->pkt_start_addr = virt_to_phys(*start);
    ds->more           = 1;
    *start += len_this_ds;

    /*
     * Time stamp each packet 
     */
    if (mac_has_flag(mac,CHECK_DMA_STATUS)) {
        bp = &r->ring_buffer[r->ring_head];
        bp->trans_start = jiffies; 
    }

    r->ring_head = athr_gmac_ring_incr(r, r->ring_head);

    return ds;
}

/*
 * Description: Handles TX and RX ring allocation.  
 *              Register Gmac interrupt handler.
 *              Configures the Gmac Hw amd MII       
 */

static int
athr_gmac_open(struct net_device *dev)
{
    unsigned int w1 = 0, w2 = 0;
    athr_gmac_t *mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    int st;
    unsigned long flags;
    uint32_t mask;

    assert(mac);

    if(!phy) {
       printk("ERROR: MAC:%d PHY not found !!!\n",mac->mac_unit);
       return -1;
    }
    
    /*
     * disable the switch ports
     */
    if(phy->reg_config)
    {
        if ((mac_has_flag(mac,ATHR_S17_MAC0_SGMII)))
        {
            phy->reg_config(ATHR_GMAC_PORT_OFF, ((mac->mac_unit == 0) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
        }
        else
        {
            phy->reg_config(ATHR_GMAC_PORT_OFF, ((mac->mac_unit == 1) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
        }
    }

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS))
        mac->mac_noacs = 4;
    else
        mac->mac_noacs = 1;

    mac->tx_ac_bt = 0;
    mac->tx_ac_drops = 0;

    st = request_irq(mac->mac_irq, athr_gmac_intr, ATHR_MAC_IRQF_DISABLED, dev->name, dev);
    if (st < 0)
    {
        printk(MODULE_NAME ": request irq %d failed %d\n", mac->mac_irq, st);
        return 1;
    }
    if (athr_gmac_tx_alloc(mac)) goto tx_failed;
    if (athr_gmac_rx_alloc(mac)) goto rx_failed;


    if (mac->mac_unit == 1) 
        mask =  RST_RESET_GE1_MDIO_RESET_SET(1) | RST_RESET_GE1_MAC_RESET_SET(1);
    else 
        mask =  RST_RESET_GE0_MDIO_RESET_SET(1) | RST_RESET_GE0_MAC_RESET_SET(1);

   /*
    * put into reset, hold, pull out.
    */
    spin_lock_irqsave(&mac->mac_lock, flags);
    athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
    /*
     * Increase the delay for MDIO and MAC reset to settle down.
     */
    mdelay(100);
    set_gmac_delay();
    athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
    mdelay(200);
    spin_unlock_irqrestore(&mac->mac_lock, flags);

    /*
     * When the mac is taken out of reset it 
     * will put gmac modules in reset. 
     * clr the soft reset in mac to bring out
     * of reset.
     */
    
    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, MAC_CONFIGURATION_1_SOFT_RESET_SET(1));



  if (mac->ops->soc_gmac_mii_setup)
        mac->ops->soc_gmac_mii_setup(mac);

    phy->init(mac);

    athr_init_hwaccels(mac);


    if (mac->ops->soc_gmac_hw_setup)
        mac->ops->soc_gmac_hw_setup(mac);

    mac->mac_speed = ATHR_PHY_SPEED_UNKNOWN;




    printk("Setting PHY...\n");

    phy_in_reset = 1;
    phy->setup(mac);
    phy_in_reset = 0;


    if ((mac->mii_intf == ATHR_SGMII) && !mac_has_flag(mac, ATHR_SGMII_FORCED)) {
        athr_reg_wr(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_AN_ENABLE_SET(1));
    }


    /*
    * set the mac addr
    */
    addr_to_words(dev->dev_addr, w1, w2);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR1, w1);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR2, w2);


    dev->trans_start = jiffies;
    /*
     * XXX  FIX ME: Enable intterupt only after link is asserted
     * check if interrupts are enabled by default.
     */
     athr_gmac_int_disable(mac);
     

    /*
     * Keep carrier off while initialization and switch it once the link is up.
     */
    netif_carrier_off(dev);
    netif_stop_queue(dev);
    athr_napi_enable(mac);

    athr_gmac_int_enable(mac);

    athr_gmac_intr_enable_rxovf(mac);

    /*
     * Enable link interrupts for PHYs
     */

    if (mac_has_flag(mac, ETH_LINK_INTERRUPT)) {
	if(mac->mii_intf == ATHR_SGMII) {
	    st = request_irq(ATHR_IRQ_ENET_LINK, athr_gmac_link_intr, 
			ATHR_MAC_IRQF_DISABLED, "gmac sgmii link intr", mac);	
	}
	if( st < 0) {
                printk(MODULE_NAME ": request irq %d failed %d\n", ATHR_IRQ_ENET_LINK, st);
		free_irq(ATHR_IRQ_ENET_LINK, mac);
	}
    } 

    mac->mac_ifup = 1;

    /*
     *  Enable Link poll, Soft led, dma_check timers
     */
    athr_gmac_timer_init(mac);
 

    if (mac_has_flag(mac,ETH_LINK_INTERRUPT)) {
        if (mac->mii_intf == ATHR_SGMII ) {
            printk ("Enabling SGMII link intr\n");
            athr_reg_wr(SGMII_INTERRUPT_MASK_ADDRESS, SGMII_INTR);
        }
    }

    if (mac->mii_phy == ATHR_VIR_PHY) {
        if (mac->ops->check_link) 
            mac->ops->check_link(mac, 0);
    }

    athr_gmac_rx_start(mac);

    /*
     * reenable the switch ports
     */
    if(phy->reg_config)
    {
        if ((mac_has_flag(mac,ATHR_S17_MAC0_SGMII)))
        {
            phy->reg_config(ATHR_GMAC_PORT_ON, ((mac->mac_unit == 0) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
        }
        else
        {
            phy->reg_config(ATHR_GMAC_PORT_ON, ((mac->mac_unit == 1) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
        }
    }

    return 0;

rx_failed:
    athr_gmac_tx_free(mac);
tx_failed:
    free_irq(mac->mac_irq, dev);
    return 1;
}

static int
athr_gmac_stop(struct net_device *dev)
{
    athr_gmac_t *mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
    unsigned long flags;

    netif_stop_queue(dev);

    netif_carrier_off(dev);

    athr_napi_disable(mac);

    spin_lock_irqsave(&mac->mac_lock, flags);

    mac->mac_ifup = 0;

    mac->link_up  = 0;

    athr_gmac_timer_del(mac);

    athr_gmac_hw_stop(mac);

    athr_gmac_tx_free(mac);

    athr_gmac_rx_free(mac);


    athr_napi_disable(mac);

    free_irq(mac->mac_irq, dev);
    spin_unlock_irqrestore(&mac->mac_lock, flags);

    if (mac_has_flag(mac, ETH_LINK_INTERRUPT)) {
	if(mac->mii_intf == ATHR_SGMII) {
		free_irq(ATHR_IRQ_ENET_LINK, mac);
	}
    }

    return 0;
}

static void
athr_gmac_hw_stop(athr_gmac_t *mac)
{
    int i;


    athr_gmac_rx_stop(mac);
    athr_gmac_tx_stop(mac);
    athr_gmac_int_disable(mac);

   /*
    * IF SGMII link interrupt is enabled 
    * disable and clear the status.
    */
    if (mac->mii_intf == ATHR_SGMII) {
        if (mac_has_flag(mac, ETH_LINK_INTERRUPT)) {
            athr_reg_wr(SGMII_INTERRUPT_ADDRESS, 0x0);
            athr_reg_wr(SGMII_INTERRUPT_MASK_ADDRESS, 0x0);
        }
    }


   /* During interface up on PHY reset the link status will be updated.
    * Clearing the software link status while bringing the interface down
    * will avoid the race condition during PHY RESET.
    */
    if (mac_has_flag(mac,ETH_SOFT_LED)) {
        if (mac->mac_unit == ENET_UNIT_LAN) {
            for(i = 0;i < 4; i++)
                mac->ledctrl.link[i] = 0;
        }
        else {
            mac->ledctrl.link[4] = 0;
        }
    }
    /*
     * put everything into reset.
     * Dont Reset WAN MAC as we are using eth0 MDIO to access S26 Registers.
     */

    athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS, 
                    MAC_CONFIGURATION_1_SOFT_RESET_SET(1));
}


void
qca955x_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac)
{

    athr_gmac_ring_t  *r     = &mac->mac_txring[0];
    uint32_t rx_ds,tx_ds;
    unsigned long flags,mask,int_mask;
    unsigned int w1 = 0, w2 = 0;

    spin_lock_irqsave(&mac->mac_lock, flags);

    int_mask = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_INTR_MASK);

    athr_gmac_tx_stop(mac);
    athr_gmac_rx_stop(mac);

    rx_ds = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RX_DESC);
    tx_ds = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_TX_DESC);

    if (mac->mac_unit)
        mask = RST_RESET_GE1_MAC_RESET_SET(1);
    else 
        mask = RST_RESET_GE0_MAC_RESET_SET(1);
    

    /*
     * put into reset, hold, pull out.
     */
    athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
    udelay(10);
    athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
    udelay(10);

    athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_1_ADDRESS, 
                      MAC_CONFIGURATION_1_SOFT_RESET_SET(1));

    athr_init_hwaccels(mac);

    mac->ops->soc_gmac_hw_setup(mac);
    
    if (mac->mii_intf == ATHR_SGMII && !mac_has_flag(mac, ATHR_SGMII_FORCED)) {
        athr_reg_wr(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_AN_ENABLE_SET(1));
    }

    if (ds){
        athr_gmac_reg_wr(mac,ATHR_GMAC_DMA_TX_DESC,
                            (athr_gmac_desc_dma_addr(r,ds)));
    }
    else {
        athr_gmac_reg_wr(mac,ATHR_GMAC_DMA_TX_DESC,tx_ds);
    }

    athr_gmac_reg_wr(mac,ATHR_GMAC_DMA_RX_DESC,rx_ds);
    /*
     * set the mac addr
     */

    addr_to_words(mac->mac_dev->dev_addr, w1, w2);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR1, w1);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR2, w2);

    mac->ops->soc_gmac_set_link(mac, mac->mac_speed, mac->mac_fdx);

    mac->dma_check = 1;
    mac->tx_ac_bt = 0;
    mac->tx_ac_drops = 0;

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {
        athr_gmac_tx_start_qos(mac,ac);
    }
    else {
        athr_gmac_tx_start(mac);
    }

    athr_gmac_rx_start(mac);

    /*
     * Restore interrupts
     */
    athr_gmac_reg_wr(mac,ATHR_GMAC_DMA_INTR_MASK,int_mask);

    spin_unlock_irqrestore(&mac->mac_lock,flags);
}

unsigned int
qca955x_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, athr_phy_speed_t *speed,int phyUnit)
{
  athr_phy_ops_t *phy = mac->phy;

  if(phy) {
     *fdx   = phy->is_fdx(mac->mac_unit, phyUnit);
     *speed = phy->speed(mac->mac_unit, phyUnit);
     *link  = phy->is_up(mac->mac_unit);
  }
  else {
     printk("MAC:%d Warning: Phy not found!!!\n",mac->mac_unit);
  }
  return 0;
}

/*
 * Tx operation:
 * We do lazy reaping - only when the ring is "thresh" full. If the ring is
 * full and the hardware is not even done with the first pkt we q'd, we turn
 * on the tx interrupt, stop all q's and wait for h/w to
 * tell us when its done with a "few" pkts, and then turn the Qs on again.
 *
 * Locking:
 * The interrupt only touches the ring when Q's stopped  => Tx is lockless,
 * except when handling ring full.
 *
 * Desc Flushing: Flushing needs to be handled at various levels, broadly:
 * - The DDr FIFOs for desc reads.
 * - WB's for desc writes.
 */
static void
athr_gmac_handle_tx_full(athr_gmac_t *mac,int ac)
{
    unsigned long flags;

    /*
     * Do nothing if the link went down while we are here.
     */
    if (netif_queue_stopped(mac->mac_dev) && !mac->link_up)
       return;

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {
        mac->tx_ac_bt |=  (1 << ac);
        if (!((mac->tx_ac_bt & 0xf) == 0xf))
           return;
    }

    assert(!netif_queue_stopped(mac->mac_dev));

    mac->mac_net_stats.tx_fifo_errors ++;

    netif_stop_queue(mac->mac_dev);

    spin_lock_irqsave(&mac->mac_lock, flags);
    athr_gmac_intr_enable_tx(mac);
    spin_unlock_irqrestore(&mac->mac_lock, flags);

}

void
athr_gmac_add_hdr(athr_gmac_t *mac,struct sk_buff *skb)
{
    int ac = skb->priority;

    if (mac_has_flag(mac,ATHR_S26_HEADER))
    {
        skb_push(skb, ATHR_HEADER_LEN);
        skb->data[0] = 0x30;                                /* broadcast = 0; from_cpu = 0; reserved = 1; port_num = 0 */
        skb->data[1] = (0x40 | (ac << HDR_PRIORITY_SHIFT)); /* reserved = 0b10; priority = 0; type = 0 (normal) */
        skb->priority = ENET_AC_BE;
    }
    else if (mac_has_flag(mac,ATHR_S16_HEADER))
    {
        skb_push(skb, ATHR_HEADER_LEN);
        memcpy(skb->data,skb->data + ATHR_HEADER_LEN, 12);

        skb->data[12] = 0x30;                                /* broadcast = 0; from_cpu = 0; reserved = 1; port_num = 0 */
        skb->data[13] = 0x40 | ((ac << HDR_PRIORITY_SHIFT)); /* reserved = 0b10; priority = 0; type = 0 (normal) */
        skb->priority = ENET_AC_BE;
    }
    else if (mac_has_flag(mac,ATHR_S27_HEADER))
    {
        skb_push(skb, ATHR_HEADER_LEN);
        memcpy(skb->data,skb->data + ATHR_HEADER_LEN, 12);

	if(mac_has_flag(mac, ETH_SWONLY_MODE)) {
	    skb->data[13] = 0x3e;
	}else {
            skb->data[13] = 0x1e;                                /* Lower order bits (0 - 7) */
	}
        skb->data[12] = 0x80 | ((ac << HDR_PRIORITY_SHIFT)); /* Higer order bits (8 - 15)*/
        skb->priority = ENET_AC_BE;
    }
    else if (mac_has_flag(mac,ATHR_HEADER_ENABLED))
    {
        /* insert 2 bytes for header */
        skb_push(skb, 2);
        memcpy(skb->data,skb->data + 2, 12);
        /* Acording to S17 team, the header value must be 0x801E*/
        *(uint16_t *)(skb->data + 12) = 0x801E | (ac << HDR_PRIORITY_SHIFT);
        skb->priority = ENET_AC_BE;
    }
}

static int
qca955x_gmac_hard_start(struct sk_buff *skb, struct net_device *dev)
{
    athr_gmac_t         *mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
    athr_gmac_ring_t    *r;
    athr_gmac_buffer_t  *bp;
    athr_gmac_desc_t    *ds, *fds;
    unsigned char       *start;
    int                 len;
    int                 nds_this_pkt;
    int                 ac = 0;

    assert(mac->mac_ifup);

    IF_ATHR_SKB_LEN_CHECK()
    {
        printk(MODULE_NAME ": bad skb, len %d\n", skb->len);
        goto dropit;
    }

    if (athr_gmac_plc_filter(skb, mac))
        goto dropit;

    for (ac = 0;ac < mac->mac_noacs; ac++) {
        if (athr_gmac_tx_reap_thresh(mac,ac))
            athr_gmac_tx_reap(mac,ac);
    }

    skb->priority = ENET_AC_BE;

    athr_gmac_vlan_igmp_tx(mac,skb);
    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {        
        athr_gmac_tx_qos(mac,skb);
    }    
    if (athr_gmac_check_qos_bt(mac,skb))
        goto dropit;

    if (mac_has_flag(mac,ATHR_HEADER_ENABLED))  {
        skb = skb_unshare(skb, GFP_ATOMIC);
        if(unlikely(skb == NULL)) {
               printk("%s(%d) - skb_unshare error\n", __func__, __LINE__);
               goto dropit;
         }
        athr_gmac_add_hdr(mac,skb);
    }

    r = &mac->mac_txring[skb->priority];

    assert(r);

    athr_gmac_trc_new(r->ring_head,"hard-stop hd");
    athr_gmac_trc_new(r->ring_tail,"hard-stop tl");

    athr_gmac_trc_new(skb->len,    "len this pkt");
    athr_gmac_trc_new(skb->data,   "ptr 2 pkt");

    athr_mac_dma_cache_sync(skb->data, skb->len);

    bp          = &r->ring_buffer[r->ring_head];

    assert(bp);

    bp->buf_pkt = skb;
    len         = skb->len;
    start       = skb->data;

    assert(len > 4);

    nds_this_pkt = 1;

    fds = ds = athr_gmac_get_tx_ds(mac, &len, &start, skb->priority);
    wmb();
    ds->res1           = 0;

    athr_nat_process_egress_pkts(mac->mac_unit, skb, ds);

    while ( len > 0)
    {
        ds = athr_gmac_get_tx_ds(mac, &len, &start, skb->priority);
        nds_this_pkt++;
        athr_gmac_tx_give_to_dma(ds);
    }

    athr_acl_enable_tx(mac,ds);

    ds->res2           = 0;
    ds->ftpp_override  = 0;

    if (!mac_has_flag(mac, ATHR_JUMBO_FR))
        ds->res3       = 0;

    ds->more           = 0;

    ath79_ddr_wb_flush(mac->ddr_flush);

    athr_gmac_tx_give_to_dma(fds);

    bp->buf_lastds  = ds;
    bp->buf_nds     = nds_this_pkt;

    athr_gmac_trc_new(ds,"last ds");
    athr_gmac_trc_new(nds_this_pkt,"nmbr ds for this pkt");

    wmb();

    mac->net_tx_packets ++;
    mac->net_tx_bytes += skb->len;

    athr_gmac_trc(athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_TX_CTRL),"dma idle");


    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {
        athr_gmac_tx_start_qos(mac,skb->priority);
    }
    else {
        athr_gmac_tx_start(mac);
    }

    for ( ac = 0;ac < mac->mac_noacs; ac++) {
        if (unlikely(athr_gmac_tx_ring_full(mac,ac))) {
            athr_gmac_handle_tx_full(mac,ac);
    	}
    }

    dev->trans_start = jiffies;

    return NETDEV_TX_OK;

dropit:
    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS))
        mac->tx_ac_drops++;

    printk(MODULE_NAME ": dropping skb %08x\n", (unsigned int)skb);

    athr_gmac_buffer_free(skb);
    return NETDEV_TX_OK;
}

/*
 * Interrupt handling:
 * - Recv NAPI style (refer to Documentation/networking/NAPI)
 *
 *   2 Rx interrupts: RX and Overflow (OVF).
 *   - If we get RX and/or OVF, schedule a poll. Turn off _both_ interurpts.
 *
 *   - When our poll's called, we
 *     a) Have one or more packets to process and replenish
 *     b) The hardware may have stopped because of an OVF.
 *
 *   - We process and replenish as much as we can. For every rcvd pkt
 *     indicated up the stack, the head moves. For every such slot that we
 *     replenish with an skb, the tail moves. If head catches up with the tail
 *     we're OOM. When all's done, we consider where we're at:
 *
 *      if no OOM:
 *      - if we're out of quota, let the ints be disabled and poll scheduled.
 *      - If we've processed everything, enable ints and cancel poll.
 *
 *      If OOM:
 *      - Start a timer. Cancel poll. Ints still disabled.
 *        If the hardware's stopped, no point in restarting yet.
 *
 *      Note that in general, whether we're OOM or not, we still try to
 *      indicate everything recvd, up.
 *
 * Locking:
 * The interrupt doesnt touch the ring => Rx is lockless
 *
 */
static irqreturn_t
athr_gmac_intr(ATHR_MAC_ISR_ARGS)
{
    struct net_device *dev  = (struct net_device *)dev_id;
    athr_gmac_t      *mac  = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
    int   isr, imr, handled = 0;

    isr   = athr_gmac_get_isr(mac);
    imr   = athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_INTR_MASK);

    isr &= 0x7fffffff; /* XXX:WORKAROUND NEED TO BE FIXED */

    athr_gmac_trc(isr,"isr");
    athr_gmac_trc(imr,"imr");

    assert(mac->mac_ifup);
    assert(isr == (isr & imr));

    if (unlikely(isr & (ATHR_GMAC_INTR_RX_OVF)))
    {
        handled = 1;
        athr_gmac_reg_rmw_set(mac,ATHR_GMAC_DMA_INTR, ATHR_GMAC_INTR_RX_OVF);
        athr_gmac_intr_disable_recv(mac);
        ATH_SCHEDULE_TQUEUE(&mac->rxtq,mac);
        mac->eth_overfl_cntr++;
        
    }
    if (likely(isr & ATHR_GMAC_INTR_RX))
    {
        handled = 1;
        if (mac_has_flag(mac,ATHR_RX_POLL)) {
            if (likely(athr_mac_rx_sched_prep(mac,dev)))
            {
                int status = athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS);
                athr_gmac_intr_disable_recv(mac);
                assert((status & ATHR_GMAC_RX_STATUS_PKT_RCVD));
                assert((status >> 16));
                __athr_mac_rx_sched(mac,dev);
            }
            else
            {
                //printk(MODULE_NAME ": driver bug! interrupt while in poll\n");
                //assert(0);
                athr_gmac_intr_disable_recv(mac);
            }
        }
        else {
            athr_gmac_intr_disable_recv(mac);
	    ATH_SCHEDULE_TQUEUE(&mac->rxtq,mac);
        }
    }
    if (likely(isr & ATHR_GMAC_INTR_TX))
    {
        handled = 1;
        athr_gmac_intr_ack_tx(mac);
        athr_gmac_intr_disable_tx(mac);
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_DMA_INTR, 1);
	ATH_SCHEDULE_TQUEUE(&mac->txreaptq,mac);
    }

    if (!handled)
    {
	/*
	 * Mac fast reset will clear the status register
	 * if we get a previously queued interrupt after reset
	 * ignore it.
 	 */
        if (mac->dma_check || (isr == 0x0))
    	{
            mac->dma_check = 0;
            return IRQ_HANDLED;
        }
        printk(MODULE_NAME ": unhandled intr isr %#x\n", isr);
        assert(0);
    }

    return IRQ_HANDLED;
}

static irqreturn_t
athr_gmac_link_intr(ATHR_MAC_ISR_ARGS)
{
    athr_gmac_t  *mac   = (athr_gmac_t *)dev_id;
    int isr;
    

    isr =athr_reg_rd(SGMII_INTERRUPT_ADDRESS);
    DPRINTF (MODULE_NAME "Debug:isr %x\n",isr);
    
    athr_reg_rmw_clear(SGMII_INTERRUPT_MASK_ADDRESS, isr);

    /* 
     * Let's  go out of the interrupt context!
     */
    schedule_work(&mac->mac_sgmii_intr);

    return IRQ_HANDLED;
}

static int
ATHR_MAC_POLL()
{
    ATHR_MAC_NAPI_MAC();
    int work_done = 0, status = 0;
    int max_work  = ATHR_MAC_GET_MAX_WORK(budget, dev);
    athr_gmac_rx_status_t  ret;
    unsigned long flags;
    ATHR_GET_DEV();

    assert(mac->mac_ifup);

    assert(mac_has_flag(mac,ATHR_RX_POLL));

    ret = athr_gmac_rx_pkt();

    ATHR_MAC_UPDATE_QUOTA_BUDGET(dev->quota, budget, work_done);
/*
 * Fix ME: 2.6.31 kernel hits a corner case when budget == work done
 * It tries to queue another request assuming that more packets are ready
 * to be processed and if napi_complete is called by the driver it
 * results in panic. Might not work for 2.6.15
 */
    IF_RX_STATUS_DONE(ret,ATHR_GMAC_RX_STATUS_DONE,work_done,budget)
    {
        ATHR_NAPI_HANDLE_RX_DONE();
    }
    else if (likely(ret == ATHR_GMAC_RX_STATUS_NOT_DONE))
    {
        /*
         * We have work left
         */
        status = 1;

        ATHR_NAPI_HANDLE_RX_NOT_DONE();
    }
    else if (ret == ATHR_GMAC_RX_STATUS_OOM)
    {
        printk(MODULE_NAME ": oom..?\n");
        /*
         * Start timer, stop polling, but do not enable rx interrupts.
         */
        mod_timer(&mac->mac_oom_timer, jiffies+1);
        ATHR_MAC_RX_COMPLETE(mac);
    }

    return status;
}

athr_receive_pkt()
{
    ATHR_TASK_DEFS()
    athr_gmac_ring_t       *r     = &mac->mac_rxring; 				\
    athr_gmac_desc_t       *ds;
    athr_gmac_buffer_t     *bp;
    struct sk_buff      *skb;
    athr_gmac_rx_status_t   ret   = ATHR_GMAC_RX_STATUS_DONE;
    int head = r->ring_head, len, status, iquota = quota, more_pkts, rep;

    athr_gmac_trc(iquota,"iquota");
    status = athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS);

    if(unlikely(mac->mac_ifup == 0  && ((status & ATHR_GMAC_RX_STATUS_PKT_RCVD) == 1))) {
        goto done;
    }


process_pkts:
    athr_gmac_trc(status,"status");

    assert(mac->mac_ifup);

    assert(quota > 0); /* WCL */

    if (mac->dma_check) {
        mac->dma_check = 0;
    }
    else {
        ATHR_NAPI_CHECK_STATUS();
    }

    while(quota)
    {
        ds    = &r->ring_desc[head];

        athr_gmac_trc(head,"hd");
        athr_gmac_trc(ds,  "ds");

        if (athr_gmac_rx_owned_by_dma(ds))
        {
            /*
            * Flush the DDR FIFOs for our gmac once and check for descriptor empty flag
            */
	    ath79_ddr_wb_flush(mac->ddr_flush);
            if (athr_gmac_rx_owned_by_dma(ds))
            {
                assert(quota != iquota); /* WCL */
                break;
            }
        }
        athr_gmac_intr_ack_rx(mac);

        bp                  = &r->ring_buffer[head];
        skb                 = bp->buf_pkt;

        if (mac_has_flag(mac, ATHR_JUMBO_FR))
        {
           len = ((ds->pkt_size & 0xffff) |
                     ((ds->res3 & 0x03) << 12));
        }
        else
        {
            len = ds->pkt_size;
        }

        assert(skb);
        skb_put(skb, len - ETHERNET_FCS_SIZE);

        /*
         *Invalidate cache before accessing the skb->data
         */
        if(unlikely(skb->len != 0))
	        athr_mac_cache_inv((unsigned long)(skb->data), skb->len);

        athr_ssdk_process_arp_header(mac, skb);

        if (mac_has_flag(mac,ATHR_HEADER_ENABLED))  {
            athr_gmac_rx_qos(mac,skb);
        }

        athr_gmac_vlan_igmp(mac,skb);

        athr_plc_dmac_filter(mac,skb);

        mac->net_rx_packets ++;
        mac->net_rx_bytes += skb->len;
        /*
         * also pulls the ether header
         */
        skb->protocol       = eth_type_trans(skb, dev);
        skb->dev            = dev;
        bp->buf_pkt         = NULL;
        dev->last_rx        = jiffies;


        quota--;

        athr_nat_process_ingress_pkts(mac->mac_unit, skb, ds);
        netif_receive_skb(skb);

        if (mac->rx_dma_check) {
            mac->rx_dma_check = 0;
        }

        head = athr_gmac_ring_incr(r, head);
    }

    assert(iquota != quota);
    r->ring_head   =  head;

    rep = athr_gmac_rx_replenish(mac);

    /*
    * let's see what changed while we were slogging.
    * ack Rx in the loop above is no flush version. It will get flushed now.
    */
    status       =  athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS);
    more_pkts    =  (status & ATHR_GMAC_RX_STATUS_PKT_RCVD);

    athr_gmac_trc(more_pkts,"more_pkts");

    if (!more_pkts) goto done;
    /*
    * more pkts arrived; if we have quota left, get rolling again
    */
    if (quota)      goto process_pkts;
    /*
    * out of quota
    */
    ret = ATHR_GMAC_RX_STATUS_NOT_DONE;

done:

    if (mac_has_flag(mac,ATHR_RX_POLL))
        *work_done   = (iquota - quota);

    if (mac_has_flag(mac,ATHR_RX_POLL) &&
           unlikely(athr_gmac_rx_ring_full(mac)))
        return GMAC_RX_STATUS_OOM;
    /*
    * !oom; if stopped, restart h/w
    */

    if (unlikely(status & ATHR_GMAC_RX_STATUS_OVF))
    {
        mac->net_rx_over_errors ++;
        athr_gmac_intr_ack_rxovf(mac);
        athr_gmac_rx_start(mac);
    }

    if (mac_has_flag(mac,ATHR_RX_TASK) && mac->mac_ifup == 1)
        athr_gmac_intr_enable_recv(mac);

    RX_RETURN();
}

static struct sk_buff *
athr_gmac_buffer_alloc(void)
{
    struct sk_buff *skb;
    u_int32_t off;

    skb = dev_alloc_skb((ATHR_GMAC_RX_BUF_SIZE + 2 + frame_sz) + L1_CACHE_BYTES - 1);
    if (unlikely(!skb))
        return NULL;

    off = ((unsigned long) skb->data) & (L1_CACHE_BYTES - 1);
    if (off != 0)
        skb_reserve(skb, L1_CACHE_BYTES - off);
#ifdef CONFIG_ATHEROS_HEADER_EN
    /* assume the len of atheros header is 2 bytes */
    skb_reserve(skb, (ATHR_GMAC_RX_RESERVE));
#else
    skb_reserve(skb, (ATHR_GMAC_RX_RESERVE + 2));
#endif
    memset((unsigned char *)skb->data, 0x0, 12);
    /*
     * Return unaligned address for skb data
     * so that header processing will be aligned
     */
    return skb;
}

static void
athr_gmac_buffer_free(struct sk_buff *skb)
{
    if (in_irq())
        dev_kfree_skb_irq(skb);
    else
        dev_kfree_skb(skb);
}

/*
 * Head is the first slot with a valid buffer. Tail is the last slot
 * replenished. Tries to refill buffers from tail to head.
 */
static int
athr_gmac_rx_replenish(athr_gmac_t *mac)
{
    athr_gmac_ring_t   *r     = &mac->mac_rxring;
    int              head  = r->ring_head, tail = r->ring_tail, refilled = 0;
    athr_gmac_desc_t   *ds;
    athr_gmac_buffer_t *bf;

    athr_gmac_trc(head,"hd");
    athr_gmac_trc(tail,"tl");

    while ( tail != head )
    {
        bf                  = &r->ring_buffer[tail];
        ds                  = &r->ring_desc[tail];

        athr_gmac_trc(ds,"ds");

        assert(!athr_gmac_rx_owned_by_dma(ds));

        assert(!bf->buf_pkt);

        bf->buf_pkt         = athr_gmac_buffer_alloc();
        if (!bf->buf_pkt)
        {
            printk(MODULE_NAME ": outta skbs!\n");
            break;
        }

        /* Invalidate chache should be chache aligned */
        athr_mac_cache_inv((unsigned long)(bf->buf_pkt->data - 2), ATHR_GMAC_RX_BUF_SIZE + frame_sz);

        ds->pkt_start_addr  = virt_to_phys(bf->buf_pkt->data);

        athr_gmac_rx_give_to_dma(ds);
        refilled ++;

        tail = athr_gmac_ring_incr(r, tail);

    }

   /*
    * Flush descriptors
    */
   wmb();


    r->ring_tail = tail;
    athr_gmac_trc(refilled,"refilled");

    return refilled;
}

/*
 * Reap from tail till the head or whenever we encounter an unxmited packet.
 */
static int
athr_gmac_tx_reap(athr_gmac_t *mac,int ac)
{
    athr_gmac_ring_t   *r;
    athr_gmac_desc_t   *ds;
    athr_gmac_buffer_t *bf;
    int head, tail, reaped = 0, i;

    r = &mac->mac_txring[ac];
    head = r->ring_head;
    tail = r->ring_tail;

    athr_gmac_trc_new(head,"hd");
    athr_gmac_trc_new(tail,"tl");

    while(tail != head)
    {
        ds   = &r->ring_desc[tail];

        athr_gmac_trc_new(ds,"ds");

        if(athr_gmac_tx_owned_by_dma(ds) || athr_gmac_tx_owned_by_dma(ds))
            break;

        bf      = &r->ring_buffer[tail];
        assert(bf->buf_pkt);

        athr_gmac_trc_new(bf->buf_lastds,"lastds");

        if(athr_gmac_tx_owned_by_dma(bf->buf_lastds) || athr_gmac_tx_owned_by_dma(bf->buf_lastds))
            break;

        for(i = 0; i < bf->buf_nds; i++)
        {
            athr_gmac_intr_ack_tx(mac);
            tail = athr_gmac_ring_incr(r, tail);
        }

        dev_kfree_skb_irq( bf->buf_pkt );
        bf->buf_pkt = NULL;

        reaped ++;
    }

    r->ring_tail = tail;

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS) &&
       (athr_gmac_ndesc_unused(mac, r) >= mac->qstart_thresh) &&
       netif_carrier_ok(mac->mac_dev))
    {
            mac->tx_ac_bt &= ~(1 << ac);
    }

    if (netif_queue_stopped(mac->mac_dev) &&
        netif_carrier_ok(mac->mac_dev))
    {
        if ((athr_gmac_ndesc_unused(mac, r) >= mac->qstart_thresh))
        {
            /*
             * Keep reaping till all the queues have enough descriptors.
             */
            if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS) && (mac->tx_ac_bt != 0))
            {
                athr_gmac_intr_enable_tx(mac);
                goto done;
            }
            netif_wake_queue(mac->mac_dev);
        }
        else
        {
            athr_gmac_intr_enable_tx(mac);
        }

    }

done:

    return reaped;
}

static void
athr_gmac_txreap_task(TQUEUE_ARG data)
{
    int ac;

    athr_gmac_t *mac = (athr_gmac_t *)data;
    for(ac = 0; ac < mac->mac_noacs; ac++)
        athr_gmac_tx_reap(mac,ac);
}

/*
 * allocate and init rings, descriptors etc.
 */
static int
athr_gmac_tx_alloc(athr_gmac_t *mac)
{
    athr_gmac_ring_t *r ;
    athr_gmac_desc_t *ds;
    int ac, i, next;
    athr_desc_type flag = ATHR_TX;

    for(ac = 0;ac < mac->mac_noacs; ac++) {

       r  = &mac->mac_txring[ac];
       if (athr_gmac_ring_alloc(mac, r, mac->num_tx_desc, flag))
           return 1;

       athr_gmac_trc(r->ring_desc,"ring_desc");

       ds = r->ring_desc;
       for(i = 0; i < r->ring_nelem; i++ )
       {
            athr_gmac_trc_new(ds,"tx alloc ds");
            next                =   (i == (r->ring_nelem - 1)) ? 0 : (i + 1);
            ds[i].next_desc     =   athr_gmac_desc_dma_addr(r, &ds[next]);
            athr_gmac_tx_own(&ds[i]);
       }
   }
   return 0;
}

static int
athr_gmac_rx_alloc(athr_gmac_t *mac)
{
    athr_gmac_ring_t *r  = &mac->mac_rxring;
    athr_gmac_desc_t *ds;
    int i, next, tail = r->ring_tail;
    athr_gmac_buffer_t *bf;
    athr_desc_type flag = ATHR_RX;
    if (athr_gmac_ring_alloc(mac, r, mac->num_rx_desc, flag))
        return 1;

    athr_gmac_trc(r->ring_desc,"ring_desc");

    ds = r->ring_desc;
    for(i = 0; i < r->ring_nelem; i++ )
    {
        next                =   (i == (r->ring_nelem - 1)) ? 0 : (i + 1);
        ds[i].next_desc     =   athr_gmac_desc_dma_addr(r, &ds[next]);
    }

    for (i = 0; i < mac->num_rx_desc; i++)
    {
        bf                  = &r->ring_buffer[tail];
        ds                  = &r->ring_desc[tail];

        bf->buf_pkt         = athr_gmac_buffer_alloc();
        if (!bf->buf_pkt)
            goto error;

        athr_mac_cache_inv((unsigned long)(bf->buf_pkt->data - 2), ATHR_GMAC_RX_BUF_SIZE + frame_sz);

        ds->pkt_start_addr  = virt_to_phys(bf->buf_pkt->data);

        ds->res1          = 0;
        ds->res2          = 0;
        ds->ftpp_override = 0;
        ds->res3          = 0;
        ds->more          = 0;
        athr_gmac_rx_give_to_dma(ds);
        tail = athr_gmac_ring_incr(r, tail);
    }

    return 0;
error:
    printk(MODULE_NAME ": unable to allocate rx\n");
    athr_gmac_rx_free(mac);
    return 1;
}

static void
athr_gmac_tx_free(athr_gmac_t *mac)
{
    int ac;

    for(ac = 0;ac < mac->mac_noacs; ac++) {
        athr_gmac_ring_release(mac, &mac->mac_txring[ac]);
        athr_gmac_ring_free(&mac->mac_txring[ac]);
    }
}

static void
athr_gmac_rx_free(athr_gmac_t *mac)
{
    athr_gmac_ring_release(mac, &mac->mac_rxring);
    athr_gmac_ring_free(&mac->mac_rxring);
}

static int
athr_gmac_ring_alloc(athr_gmac_t *mac, athr_gmac_ring_t *r, int count, athr_desc_type flag)
{
    int desc_alloc_size, buf_alloc_size;

    desc_alloc_size = sizeof(athr_gmac_desc_t)   * count;
    buf_alloc_size  = sizeof(athr_gmac_buffer_t) * count;

    memset(r, 0, sizeof(athr_gmac_ring_t));

    r->ring_buffer = (athr_gmac_buffer_t *)kmalloc(buf_alloc_size, GFP_KERNEL);

    printk("%s Allocated %d at 0x%lx\n",__func__,buf_alloc_size,(unsigned long) r->ring_buffer);

    if (!r->ring_buffer)
    {
        printk(MODULE_NAME ": unable to allocate buffers\n");
        return 1;
    }

    athr_gmac_desc_alloc(mac, r,count, flag);

    if (! r->ring_desc)
    {
        printk(MODULE_NAME ": unable to allocate coherent descs\n");
        kfree(r->ring_buffer);
        printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) r->ring_buffer);
        return 1;
    }

    memset(r->ring_buffer, 0, buf_alloc_size);
    memset(r->ring_desc,   0, desc_alloc_size);
    r->ring_nelem   = count;

    return 0;
}

static void
athr_gmac_ring_release(athr_gmac_t *mac, athr_gmac_ring_t  *r)
{
    int i;

    for(i = 0; i < r->ring_nelem; i++)
        if (r->ring_buffer[i].buf_pkt)
            athr_gmac_buffer_free(r->ring_buffer[i].buf_pkt);
}

/*
 * Error timers
 */
static void
athr_gmac_oom_timer(unsigned long data)
{
    athr_gmac_t *mac = (athr_gmac_t *)data;
    int val;

    athr_gmac_trc(data,"data");
    athr_gmac_rx_replenish(mac);
    if (athr_gmac_rx_ring_full(mac))
    {
        val = mod_timer(&mac->mac_oom_timer, jiffies+1);
        assert(!val);
    }
    else
        athr_mac_rx_sched(mac);
}

static void
athr_gmac_tx_timeout(struct net_device *dev)
  {
      athr_gmac_t *mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
#ifdef CONFIG_AG7240_ATHRS17_PHY
      athr_phy_ops_t *phy = mac->phy;    
#endif
      athr_gmac_trc(dev,"dev");
      printk("eth%d %s\n", mac->mac_unit, __func__);

#ifdef CONFIG_AG7240_ATHRS17_PHY
      if(mac->mii_phy == ATHR_S17_PHY)
      {
          /* 
               * WAR for EV111027: Do off & on all the S17 ports, mac0 & mac6. 
               * So that the traffic becomes stable and the timeout not occured again. 
               * It's implemented inside the timeout handler because it has to be executed
               * immediately even CPU full load condition. 
               */
          if (mac->mac_unit)
          {
              phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_LAN); /* disable all the LAN ports */
              phy->write_mac_reg(S17_P0STATUS_REG, 0);
              mdelay(50); /* wait minimum 50 ms for allowing mac HW to settel before turning on */
              phy->write_mac_reg(S17_P0STATUS_REG, S17_PORT_STATUS_AZ_DEFAULT);
              phy->reg_config(ATHR_GMAC_PORT_ON, ENET_UNIT_LAN); /* enable all the LAN ports */
          }
          else
          {
              phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_WAN); /* disable all the WAN ports */
              phy->write_mac_reg(S17_P6STATUS_REG, 0);
              mdelay(50); /* wait minimum 50 ms for allowing mac HW to settel before turning on */
              phy->write_mac_reg(S17_P6STATUS_REG, S17_PORT_STATUS_AZ_DEFAULT);
              phy->reg_config(ATHR_GMAC_PORT_ON, ENET_UNIT_WAN); /* enable all the WAN ports */
          }
      }
      
#endif

      /*
       * Do the reset outside of interrupt context
       */
      schedule_work(&mac->mac_tx_timeout);
  }


static void
athr_gmac_tx_timeout_task(ATHR_MAC_TASK_ARG)
{
    ATHR_MAC_TASK_MAC();
    athr_gmac_trc(mac,"mac");

    if (mac->ops->check_dma_st)
       mac->ops->check_dma_st(mac,0);
}

static void
athr_gmac_get_default_macaddr(athr_gmac_t *mac, u8 *mac_addr)
{
    u8 *eep_mac_addr = NULL;
    /* Use MAC address stored in Flash */
    if (!is_emu()) {

        athr_get_gmac_addr(mac,&eep_mac_addr);
        printk(MODULE_NAME ": Mac address for unit %d:%p\n",mac->mac_unit,eep_mac_addr);
        printk(MODULE_NAME ": %02x:%02x:%02x:%02x:%02x:%02x \n",
            eep_mac_addr[0],eep_mac_addr[1],eep_mac_addr[2],
            eep_mac_addr[3],eep_mac_addr[4],eep_mac_addr[5]);

        /*
        ** Check for a valid manufacturer prefix.  If not, then use the defaults
        */

        if(eep_mac_addr[0] == 0x00 &&
           eep_mac_addr[1] == 0x03 &&
           eep_mac_addr[2] == 0x7f)
        {
            memcpy(mac_addr, eep_mac_addr, 6);
            return ;
        }
    }
    /* Use Default address at top of range */
    mac_addr[0] = 0x00;
    mac_addr[1] = 0x03;
    mac_addr[2] = 0x7F;
    mac_addr[3] = 0xFF;
    mac_addr[4] = 0xFF;
    mac_addr[5] = 0xFF - mac->mac_unit;
}

static int
athr_gmac_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    return (athrs_do_ioctl(dev,ifr,cmd));
}

static struct net_device_stats
    *athr_gmac_get_stats(struct net_device *dev)
{

    athr_gmac_t *mac = ATHR_MAC_PRIV(dev);
    int carrier = netif_carrier_ok(dev);

    if (mac->mac_speed == ATHR_PHY_SPEED_UNKNOWN) {
        return &mac->mac_net_stats;
    }

    /*
     *  MIB registers reads will fail if link is lost while resetting PHY.
     */
    if (carrier && !phy_in_reset)
    {

        mac->mac_net_stats.rx_packets = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_PKT_CNTR);
        mac->mac_net_stats.tx_packets = athr_gmac_reg_rd(mac,ATHR_GMAC_TX_PKT_CNTR);
        mac->mac_net_stats.rx_bytes   = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_BYTES_CNTR);
        mac->mac_net_stats.tx_bytes   = athr_gmac_reg_rd(mac,ATHR_GMAC_TX_BYTES_CNTR);
        mac->mac_net_stats.tx_errors  = athr_gmac_reg_rd(mac,ATHR_GMAC_TX_CRC_ERR_CNTR);
        mac->mac_net_stats.rx_dropped = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_DROP_CNTR);
        mac->mac_net_stats.tx_dropped = athr_gmac_reg_rd(mac,ATHR_GMAC_TX_DROP_CNTR);
        mac->mac_net_stats.collisions = athr_gmac_reg_rd(mac,ATHR_GMAC_TOTAL_COL_CNTR);

            /* detailed rx_errors: */
        mac->mac_net_stats.rx_length_errors = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_LEN_ERR_CNTR);
        mac->mac_net_stats.rx_over_errors   = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_OVL_ERR_CNTR);
        mac->mac_net_stats.rx_crc_errors    = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CRC_ERR_CNTR);
        mac->mac_net_stats.rx_frame_errors  = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_FRM_ERR_CNTR);

        mac->mac_net_stats.rx_errors  = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CODE_ERR_CNTR) +
                                        athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CRS_ERR_CNTR) +
                                        mac->mac_net_stats.rx_length_errors +
                                        mac->mac_net_stats.rx_crc_errors +
                                        mac->mac_net_stats.rx_frame_errors;

        mac->mac_net_stats.multicast  = athr_gmac_reg_rd(mac,ATHR_GMAC_RX_MULT_CNTR) +
                                        athr_gmac_reg_rd(mac,ATHR_GMAC_TX_MULT_CNTR);

        if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS))
            mac->mac_net_stats.tx_dropped += mac->tx_ac_drops;
    }
    return &mac->mac_net_stats;
}

static void
athr_gmac_vet_tx_len_per_pkt(unsigned int *len)
{
    unsigned int l;

    /* make it into words */
    l = *len & ~3;

    /*
    * Not too small
    */
    if (l < ATHR_GMAC_TX_MIN_DS_LEN) {
        l = ATHR_GMAC_TX_MIN_DS_LEN;
    }
    else {
    /* Avoid len where we know we will deadlock, that
    * is the range between fif_len/2 and the MTU size
    */
        if (l > ATHR_GMAC_TX_FIFO_LEN/2) {
            if (l < ATHR_GMAC_TX_MTU_LEN)
                l = ATHR_GMAC_TX_MTU_LEN;
            else if (l > ATHR_GMAC_TX_MAX_DS_LEN)
                l = ATHR_GMAC_TX_MAX_DS_LEN;
                *len = l;
        }
    }
}

/*
 * All allocations (except irq and rings).
 */
int __init
qca955x_gmac_init(void)
{
    int i;
    struct net_device *dev;
    athr_gmac_t      *mac;
    uint32_t mask = 0;


    if (!soc_is_qca955x())
	return -ENODEV;

    /*
    * tx_len_per_ds is the number of bytes per data transfer in word increments.
    *
    * If the value is 0 than we default the value to a known good value based
    * on benchmarks. Otherwise we use the value specified - within some
    * cosntraints of course.
    *
    * Tested working values are 256, 512, 768, 1024 & 1536.
    *
    * A value of 256 worked best in all benchmarks. That is the default.
    *
    */

    /* Tested 256, 512, 768, 1024, 1536 OK, 1152 and 1280 failed*/
    if (0 == tx_len_per_ds)
        tx_len_per_ds = CONFIG_ATHR_GMAC_LEN_PER_TX_DS;

    athr_gmac_vet_tx_len_per_pkt( &tx_len_per_ds);

    printk(MODULE_NAME ": Length per segment %d\n", tx_len_per_ds);

    /*
    * Compute the number of descriptors for an MTU
    */
    tx_max_desc_per_ds_pkt =1;


    /* 
    ** Do the rest of the initializations 
    */
    for(i = 0; i < ATHR_GMAC_NMACS; i++)
    {

        dev = alloc_etherdev(ATHR_MAC_ETHERDEV_SZ);
        if (!dev)
        {
            printk(MODULE_NAME ": unable to allocate mac\n");
            return 1;
        }
        ATHR_MAC_NETPRIV(mac,dev);
        memset(mac, 0, sizeof(athr_gmac_t));

        mac->mac_dev       = dev;
        mac->mac_unit      = i;
        mac->mac_base      = athr_gmac_base(i);
        mac->mac_irq       = athr_gmac_irq(i);
        mac->mac_noacs     = 1;
        mac->num_tx_desc   = athr_tx_desc_num[i];
        mac->num_rx_desc   = athr_rx_desc_num[i];
        mac->reap_thresh   = athr_tx_desc_num[i] / 2;
        mac->qstart_thresh = 4 * tx_max_desc_per_ds_pkt;
        mac->mac_speed     = ATHR_PHY_SPEED_UNKNOWN;
        mac->no_of_macs    = ATHR_GMAC_NMACS;
        snprintf(mac->led_name, sizeof(athr_gmacs[0]->led_name), "lan");

        /*
         * Assign MII typs and PHY associated.
         */
        if (i) {
            //For MAC1
            mac->mii_intf      = CFG_ATHR_GMAC1_MII; 
            mac->mii_phy       = CFG_ATHR_GMAC1_MII_PHY;
        } else {
            //For MAC0
            mac->mii_intf      = CFG_ATHR_GMAC0_MII; 
            mac->mii_phy       = CFG_ATHR_GMAC0_MII_PHY;
        }

        athr_gmacs[i]  = mac;

        athr_gmac_attach(mac);

	if(!mac->ops) {
           printk(MODULE_NAME "ERROR: No MAC found !!!\n");
	   assert(0);
           return 1;
        }

        mac->ops->set_caps(mac);

        if (mac_has_flag(mac, ETH_SWONLY_MODE))
        {
            if (mac->mac_unit == 0)
                continue;
        }

        athr_gmac_phy_attach(mac, mac->mac_unit);


        spin_lock_init(&mac->mac_lock);
       /*
        * out of memory timer
        */
        init_timer(&mac->mac_oom_timer);
        mac->mac_oom_timer.data     = (unsigned long)mac;
        mac->mac_oom_timer.function = (void *)athr_gmac_oom_timer;

        ATHR_MAC_INIT_WORK(mac, athr_gmac_tx_timeout_task);

        if (mac->mii_intf == ATHR_SGMII && mac_has_flag(mac, ETH_LINK_INTERRUPT)) {

            ATHR_MAC_INIT_SGMII_INT(mac, athr_gmac_sgmii_intr);

        }
       /*
        * watchdog task
        */
        ATHR_MAC_SET_DEV_NET_OPS(dev, mac,
                                 athr_gmac_get_stats,
                                 athr_gmac_open,
                                 athr_gmac_stop,
                                 qca955x_gmac_hard_start,
                                 athr_gmac_do_ioctl,
                                 athr_gmac_poll,
                                 athr_gmac_tx_timeout,
                                 ATHR_GMAC_NAPI_WEIGHT,
                                 ATH_GMAC_TXQUEUELEN);

        ATH_INIT_RX_TASK();
        ATH_INIT_TX_TASK();

        if (mac_has_flag(mac,ATHR_RX_TASK))
	    printk(MODULE_NAME ": RX TASKLET - Pkts per Intr:%d\n",CONFIG_ATHR_RX_PKTS_CNT);

        athr_gmac_get_default_macaddr(mac, dev->dev_addr);


        athr_register_hwaccels(mac);

        if (register_netdev(dev))
        {
            printk(MODULE_NAME ": register netdev failed\n");
            goto failed;
        }

        netif_carrier_off(dev);

        athr_register_qos(mac);
        athr_register_vlan_igmp(mac);

	if(!mac->phy) {
            printk("MAC:%d Warning: Phy not found!!!\n",mac->mac_unit);
	} else if(mac->phy->led_set != NULL) {
	  mac->led_dev.name = mac->led_name;
	  mac->led_dev.default_trigger = NULL;
	  mac->led_dev.brightness_set  = mac->phy->led_set;
	}

        athr_gmac_trc_init();

	/*
         * Initialize timers.
         */

        if (mac_has_flag(mac,CHECK_DMA_STATUS)) {
            mac->dma_chk_ts = jiffies;
        }
        if (mac_has_flag(mac,ETH_SOFT_LED)) {
            mac->led_blink_ts = jiffies;
        }
        if (mac_has_flag(mac,ETH_LINK_POLL)) {
            mac->link_ts = jiffies;
        }
        /* 
         * When flowmac is enabled by default, all registrations must happen
         * by default. Wireless driver comes with default state and would be
         * enabled on fly
         */
        athr_gmac_flowmac_register(dev);
        printk(MODULE_NAME ": Max segments per packet :   %d\n", tx_max_desc_per_ds_pkt);
        printk(MODULE_NAME ": Max tx descriptor count :   %d\n", mac->num_tx_desc);
        printk(MODULE_NAME ": Max rx descriptor count :   %d\n", mac->num_rx_desc);
        printk(MODULE_NAME ": Mac capability flags    :   %X\n", mac->mac_flags);
    }
    
    if (athr_gmacs[0]->mii_intf == ATHR_SGMII ||
        ( (ATHR_GMAC_NMACS > 1) && athr_gmacs[1]->mii_intf == ATHR_SGMII)) {
        serdes_pll_lock_detect_st();
    }

    //in order to provide the same led class interface as other
    //products a single gmac is registered.
    if(athr_gmacs[0]->led_dev.brightness_set != NULL) {
      led_classdev_register(&dev->dev, &athr_gmacs[0]->led_dev);
    }

   /*
    * MDIO init for slave scorpion if connected !!
    */
    if (mac_has_flag(mac, SLAVE_SCO_CONNECTED)) {
        printk (MODULE_NAME ": Initializing the mdio's for slave\n");
        mdio_init_device();
    }
    
    /*
     * Do the resets.
     */
    for (i = 0; i < ATHR_GMAC_NMACS; i++) {

        athr_gmac_reg_rmw_set(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS, 
                ( MAC_CONFIGURATION_1_SOFT_RESET_SET(1)   |
                  MAC_CONFIGURATION_1_RESET_RX_MAC_SET(1) | 
                  MAC_CONFIGURATION_1_RESET_TX_MAC_SET(1)));

        udelay(20);

               
        if (i) 
            mask = (RST_RESET_GE1_MDIO_RESET_SET(1) | RST_RESET_GE1_MAC_RESET_SET(1));
        else 
            mask = (RST_RESET_GE0_MDIO_RESET_SET(1) | RST_RESET_GE0_MAC_RESET_SET(1));

       /*
        * put into reset, hold, pull out.
        */

        athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
        mdelay(100);
        athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
        /*
         * Increase the delay for MDIO and MAC reset to settle down.
         */
        mdelay(200);

        athr_gmac_reg_rmw_clear(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS, 
                                     MAC_CONFIGURATION_1_SOFT_RESET_SET(1));

    }

    athr_register_hwaccels_notifiers(athr_gmacs[0]);

    // Register Push button notification handler
    if (is_push_button() && athr_gmacs[0]) {
        mac = athr_gmacs[0];
        dev = mac->mac_dev;
        register_simple_config_callback (dev->name, (void *) pushbutton_intr, (void *) dev,
                                         (void *)&mac->push_dur);
    }

   return 0;

failed:

    for(i = 0; i < ATHR_GMAC_NMACS; i++)
    {
        if (!athr_gmacs[i])
            continue;

        if (athr_gmacs[i]->mac_dev)  {
        if (mac_has_flag(athr_gmacs[i],ETH_LINK_INTERRUPT) && (i == ATHR_GMAC_NMACS))
            printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) athr_gmacs[i]);
            free_netdev(athr_gmacs[i]->mac_dev);
        }

    }
    return 1;
}

void __exit
qca955x_gmac_cleanup(void)
{
    int i;

    for(i = 0; i < ATHR_GMAC_NMACS; i++)
    {
        if (is_push_button() && (i == 0)) {
            unregister_simple_config_callback(athr_gmacs[0]->mac_dev->name);
        }
	if (mac_has_flag(athr_gmacs[ATHR_GMAC_NMACS - 1],ETH_SWONLY_MODE) && i == 0) {
             continue;
         }

        athr_cleanup_hwaccels(athr_gmacs[i]);
        athr_gmac_timer_del(athr_gmacs[i]);
        if (flowmac_on) {
            athr_gmac_flowmac_un_register(athr_gmacs[i]->mac_dev);
        }
        unregister_netdev(athr_gmacs[i]->mac_dev);
        printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) athr_gmacs[i]);
        free_netdev(athr_gmacs[i]->mac_dev);
    }
    for(i = 0; i < ATHR_GMAC_NMACS; i++)
    {
         athr_gmacs[i] = NULL;
     }


    printk(MODULE_NAME ": cleanup done\n");
}

int
mdio_boot_init(int unit)
{
	athr_gmac_t *mac = athr_gmacs[unit];

	if (!mac) {
		printk("%s, invalid unit number %d\n", __func__, unit);
		return -EINVAL;
	}

	/*
	 * In current code this looks has been done, redundent setting,
	 * still keeping here, if some one changes the other setting, this
	 * should work always.
	 */
	athr_gmac_reg_wr(mac, ATHR_GMAC_CFG1,
			(MAC_CONFIGURATION_1_RX_ENABLE_MASK | MAC_CONFIGURATION_1_TX_ENABLE_MASK));

	athr_gmac_reg_wr(mac, ATHR_GMAC_CFG2,
			MAC_CONFIGURATION_2_PREAMBLE_LENGTH_SET(7) |
			MAC_CONFIGURATION_2_IF_MODE_SET(2) |
			MAC_CONFIGURATION_2_HUGE_FRAME_SET(1) |
			MAC_CONFIGURATION_2_LENGTH_CHECK_SET(1) |
			MAC_CONFIGURATION_2_PAD_CRC_SET(1) |
			MAC_CONFIGURATION_2_FULL_DUPLEX_SET(1));

	athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_0,
			AMCXFIF_CFG_0_FTFENREQ_SET(1) |
			AMCXFIF_CFG_0_STFENREQ_SET(1) |
			AMCXFIF_CFG_0_FRFENREQ_SET(1) |
			AMCXFIF_CFG_0_SRFENREQ_SET(1) |
			AMCXFIF_CFG_0_WTMENREQ_SET(1));

	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, ATHR_GMAC_MGMT_CFG_CLK_DIV_28);

	printk("Value of mac_mii_mgmt_cfg %x\n",
		athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_CFG));

	printk("%s, success \n", __func__);

	return 0;
}

void
mdio_reg_write(int unit, int addr, uint16_t data)
{
	athr_gmac_t *mac = athr_gmacs[unit];
	uint32_t	reg;

	addr |= (ATH_MDIO_PHY_ADDR << ATHR_GMAC_ADDR_SHIFT);

	reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
	while (reg == 1)
		reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;

	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);
	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CTRL, data);

	reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
	while (reg == 1)
		reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;


	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);
}

uint16_t
mdio_reg_read(int unit,int addr)
{
	athr_gmac_t *mac = athr_gmacs[unit];
	uint32_t	reg;

	addr |= (ATH_MDIO_PHY_ADDR << ATHR_GMAC_ADDR_SHIFT);
	reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
	while (reg == 1)
		reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;

	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);
	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x1);

        udelay(1);
	reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
	while (reg == 1)
		reg = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;


	athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);

	return athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_STATUS) & 0xFFFF;
}

EXPORT_SYMBOL(mdio_boot_init);
EXPORT_SYMBOL(mdio_reg_read);
EXPORT_SYMBOL(mdio_reg_write);
