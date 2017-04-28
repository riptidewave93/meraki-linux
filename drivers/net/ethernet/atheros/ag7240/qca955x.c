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

#include "athrs_mac.h"
#include "934x.h"
#include "955x.h"
#include "qca955x_mac.h"
#define MODULE_NAME "955x_GMAC"
#define MAX_SIZE 10

extern int frame_sz;
extern athr_gmac_t *athr_gmacs[ATHR_GMAC_NMACS];

extern void athr_gmac_sgmii_get_duplex(athr_gmac_t *mac, athr_gmac_duplex_t *duplex);
extern void athr_sgmii_get_link_status(athr_gmac_t *mac,athr_gmac_sgmii_link_t *link);
extern void athrs_sgmii_get_speed(athr_gmac_t *mac, athr_phy_speed_t  *rate);

extern unsigned int qca955x_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, 
         athr_phy_speed_t *speed,int phyUnit);

extern void qca955x_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac);
extern void athr_gmac_sgmii_setup(void *arg);
extern void athr_remove_hdr(athr_gmac_t *mac, struct sk_buff *skb);
extern void athr_gmac_add_hdr(athr_gmac_t *mac,struct sk_buff *skb);
static void qca955x_gmac_sgmii_res_cal(void);
static void qca955x_gmac_rgmii_cal(void);

static char mii_intf[][MAX_SIZE]={"GMII","MII","RGMII","RMII","SGMII"};


static inline void qca955x_soc_gmac_set_mac_duplex(athr_gmac_t *mac, int fdx)
{
    if (fdx) {
        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS, ATHR_GMAC_CFG2_FDX);
    }
    else {
        athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_2_ADDRESS, ATHR_GMAC_CFG2_FDX);
    }
    printk (MODULE_NAME ": %s \n",__func__);
}

#if defined (CONFIG_ATHRS_QOS)
void 
qca_gmac_rx_qos(athr_gmac_t *mac, struct sk_buff *skb)
{
    uint16_t hdr_type = 0;
    int tos;
    struct ethhdr       *eh;
    unsigned short diffs[2];

    /* extract the header */
    hdr_type = *(uint16_t *)(skb->data + 12);  

    if ((hdr_type & HDR_PACKET_TYPE_MASK) == ATHR_NORMAL_PKT) {
        /* get the priority from the header */
        tos = AC_TO_ENET_TOS (((hdr_type >> HDR_PRIORITY_SHIFT) & HDR_PRIORITY_MASK));
#ifndef CONFIG_ATHR_VLAN_IGMP
        athr_remove_hdr(mac,skb);
#endif
        eh = (struct ethhdr *)skb->data;
        if (eh->h_proto  == __constant_htons(ETHERTYPE_IP))
        {
            struct iphdr *ip = (struct iphdr *)
                    (skb->data + sizeof (struct ethhdr));
            /*
                       * IP frame: exclude ECN bits 0-1 and map DSCP bits 2-7
                       * from TOS byte.
                       */
            tos = (tos << TOS_ECN_SHIFT ) & TOS_ECN_MASK;
            if ( tos != ip->tos ) {
                diffs[0] = htons(ip->tos) ^ 0xFFFF;
                ip->tos = ((ip->tos &  ~(TOS_ECN_MASK)) | tos);
                diffs[1] = htons(ip->tos);
                ip->check = csum_fold(csum_partial((char *)diffs,
                             sizeof(diffs),ip->check ^ 0xFFFF));
            }
            skb->priority = tos;
        }
    }
    else
    {
#ifndef CONFIG_ATHR_VLAN_IGMP
        athr_remove_hdr(mac,skb);
#endif
    }
}

void
qca_gmac_tx_qos(athr_gmac_t *mac,struct sk_buff *skb)
{
    struct ethhdr       *eh;
    int                 ac = 0;

    /*
     * Select the TX based on access category
     */

    ac = ENET_AC_BE;
    /*
     * Default priority
     */
    eh = (struct ethhdr *) skb->data;

    if (eh->h_proto  == __constant_htons(ETHERTYPE_IP))
    {
        const struct iphdr *ip = (struct iphdr *)
                    (skb->data + sizeof (struct ethhdr));
        /*
         * IP frame: exclude ECN bits 0-1 and map DSCP bits 2-7
         * from TOS byte.
         */
        ac = TOS_TO_ENET_AC ((ip->tos >> TOS_ECN_SHIFT) & 0x3F);
    }
    
    skb->priority = ac;

    /*hdr_dump("Tx",mac->mac_unit,skb->data,ac,0);*/
}

int
qca_gmac_check_qos_bt(athr_gmac_t *mac,struct sk_buff *skb)
{
    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {         
        if (mac->tx_ac_bt & (1 << skb->priority)) {      
            mac->mac_dev->trans_start = jiffies;          
            return 1;
        }                                                
    }
    return 0;
}
#endif /* CONFIG_ATHRS_QOS */

void mdio_init_device(void)
{
    uint32_t mask;

    ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_GE0_MDIO_RESET_SET(1));
    udelay(100);

        //phy address setting
    ath_reg_wr(MDIO_PHY_ADDR_ADDRESS, ATH_MDIO_PHY_ADDR);

    mask = ath_reg_rd(GPIO_IN_ENABLE3_ADDRESS) &
             ~(GPIO_IN_ENABLE3_BOOT_EXT_MDC_MASK |
                        GPIO_IN_ENABLE3_BOOT_EXT_MDO_MASK);
    mask |= GPIO_IN_ENABLE3_BOOT_EXT_MDC_SET(ATH_MDC_GPIO_PIN) |
              GPIO_IN_ENABLE3_BOOT_EXT_MDO_SET(ATH_MDIO_GPIO_PIN);
    ath_reg_wr(GPIO_IN_ENABLE3_ADDRESS, mask);

    mask = ath_reg_rd(GPIO_OE_ADDRESS);
    mask |= ((1 << ATH_MDC_GPIO_PIN) | (1 << ATH_MDIO_GPIO_PIN));
    ath_reg_wr(GPIO_OE_ADDRESS, mask);
    mask = ath_reg_rd(GPIO_OUT_FUNCTION4_ADDRESS) &
              ~(ATH_GPIO_OUT_FUNC_EN_MASK);
    mask |= ATH_GPIO_OUT_FUNC_EN_SET(ATH_MII_EXT_MDI);

    ath_reg_wr(GPIO_OUT_FUNCTION4_ADDRESS, mask);

}

int qca955x_gmac_poll_link(void *arg)
{

    athr_gmac_t *mac = (athr_gmac_t *) arg;
    struct net_device  *dev     = mac->mac_dev;
    int                 carrier = netif_carrier_ok(dev), fdx, phy_up = 0;
    athr_phy_speed_t  speed;
    athr_phy_ops_t *phy = mac->phy;
    int  phyUnit = 0;
    static int last_link;

    assert(mac->mac_ifup);


    switch (mac->mii_intf) {
        case ATHR_RGMII:
            qca955x_gmac_get_link_st(mac, &phy_up, &fdx, &speed, phyUnit);
            break;
        case ATHR_SGMII:
            if (mac_has_flag(mac, ATHR_SGMII_FORCED))  {
                 fdx    = 1;
                 phy_up = 1;
                 speed  = 2;
                 if (mac->mii_phy == ATHR_S17_PHY) {
                     if (is_drqfn()) {
                         phy_up = phy->is_up(0);
                     } else {
                         phy_up = phy->is_up(mac->mac_unit);
                     }
                }
            } else {
                athr_sgmii_get_link_status(mac, (athr_gmac_sgmii_link_t *)&phy_up);
                athr_gmac_sgmii_get_duplex(mac, &fdx);
                athrs_sgmii_get_speed(mac, &speed);
                if (mac->mii_phy == ATHR_S17_PHY) {
                    if (!phy_up) {
                        goto done;
                    }
                    if (is_drqfn()) {
                       phy_up = phy->is_up(0);
                    } else {
                       phy_up = phy->is_up(mac->mac_unit);
                    }
                }
            }
            break;
        default:
            break;
    }
       
    if (!phy_up)
    {
        if (carrier)
        {
            printk(MODULE_NAME ": unit %d: phy %0d not up carrier %d\n", mac->mac_unit, phyUnit, carrier);

            /* A race condition is hit when the queue is switched on while tx interrupts are enabled.
             * To avoid that disable tx interrupts when phy is down.
             */
            mac->link_up = 0;
            athr_gmac_intr_disable_tx(mac);
            netif_carrier_off(dev);
            netif_stop_queue(dev);

            athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_1_ADDRESS, 
                (MAC_CONFIGURATION_1_RX_ENABLE_SET(1) | MAC_CONFIGURATION_1_TX_ENABLE_SET(1)));


       }
       goto done;
    }

    if(!mac->mac_ifup) {
        goto done;
    }

    if ((fdx < 0) || (speed < 0))
    {
        printk(MODULE_NAME ": phy not connected?\n");
        return 0;
    }

    if (carrier && (speed == mac->mac_speed ) && (fdx == mac->mac_fdx)) {
    /*
     *  Per port threshold value and Global threshold value of 
     *  s17 switch is changed based on the no of liks detected.
     */

        if (mac->mii_phy == ATHR_S17_PHY) {

            if (last_link != phy_up) {
                last_link = phy_up;
                switch (phy_up) {

                    case 1 :
                    case 2 :
                    case 3 :
                        phy->write_mac_reg(S17_P0FLCTL_REG, 0x00cc00ff);
                        phy->write_mac_reg(S17_GLOBAL_FLCTL_REG, 0x017a01ff);
                        return 0;

                    case 4:
                        phy->write_mac_reg(S17_P0FLCTL_REG, 0x009200d8);
                        phy->write_mac_reg(S17_GLOBAL_FLCTL_REG, 0x017a01ff);
                        return 0;
                    default:
                        goto done;
                }
            }
        }
        goto done;
    }

    printk(MODULE_NAME ": enet unit:%d is up...\n", mac->mac_unit);

    printk("eth%d  %s  %s  %s\n", mac->mac_unit, mii_intf[mac->mii_intf],
               spd_str[speed], dup_str[fdx]);

    mac->ops->soc_gmac_set_link(mac, speed, fdx);


    printk(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl  \n",
       athr_gmac_reg_rd(mac, MAC_CONFIGURATION_2_ADDRESS),
       athr_gmac_reg_rd(mac, ATHR_GMAC_IFCTL));

    if (mac->mii_phy == ATHR_AR8033_PHY)
       qca955x_gmac_fast_reset(mac, NULL, 0);

   /* 
    * only Enable Tx and Rx  afeter configuring the mac speed.
    */

    athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS, 
     (MAC_CONFIGURATION_1_RX_ENABLE_SET(1) | MAC_CONFIGURATION_1_TX_ENABLE_SET(1)));

    /*
     * in business
     */
    netif_carrier_on(dev);
    netif_start_queue(dev);
    mac->link_up = 1;

done:
    
    return 0;
  
}
static int
qca955x_set_gmac_caps(void *arg)
{
    athr_gmac_t *mac = (athr_gmac_t *)arg;

    printk (MODULE_NAME ": %s\n",__func__);

#ifdef CONFIG_ATHRS_QOS
    /* 
         * On board rev 020 , S17 mac0 conneted on SGMII (eth1) and on rev 010, S17 mac0 connected on RGMII(eth0).
         * Use this differece identify the board revision
         */
#ifdef ATH_S17_MAC0_SGMII
    /* rev 020 */
    if (mac->mac_unit == 0)
    {
      mac_set_flag(mac,WAN_QOS_SOFT_CLASS);
    }
    else
    {
      mac_set_flag(mac,ATHR_HEADER_ENABLED);
    }   
#else
    /* rev 010 */
    if (mac->mac_unit == 0)
    {
        mac_set_flag(mac,ATHR_HEADER_ENABLED);
    }
    else
    {
        mac_set_flag(mac,WAN_QOS_SOFT_CLASS);
    }    
#endif
#endif

#ifdef CONFIG_ATHR_VLAN_IGMP
    mac_set_flag(mac, ATHR_VLAN_IGMP);
#endif


#ifdef CONFIG_ATHR_SWITCH_ONLY_MODE
    mac_set_flag(mac, ETH_SWONLY_MODE);
#endif

#ifdef CONFIG_ATHR_SUPPORT_DUAL_PHY
    mac_set_flag(mac, ATHR_DUAL_PHY);
#endif

    if (!is_vir_phy()) {
        mac_set_flag(mac, ETH_LINK_POLL);
        printk("Currently in polling mode unit%d\n",mac->mac_unit);
    }

#ifdef CONFIG_SCO_SLAVE_CONNECTED
    mac_set_flag(mac, SLAVE_SCO_CONNECTED);
#endif
#ifdef CONFIG_S17_SWMAC6_CONNECTED
    mac_set_flag(mac, S17_MAC6_CONNECTED);
#endif
#ifdef ATH_SGMII_FORCED_MODE
    if (mac->mii_intf == ATHR_SGMII)
        mac_set_flag(mac, ATHR_SGMII_FORCED);
#endif 

#ifdef CONFIG_ATH_SGMII_LINK_INTR
    if (mac->mii_intf == ATHR_SGMII)
        mac_set_flag(mac, ETH_LINK_INTERRUPT);
#endif

#ifdef CONFIG_ATHR_RX_TASK
    mac_set_flag(mac,ATHR_RX_TASK);
#else
    mac_set_flag(mac,ATHR_RX_POLL);
#endif

#if 0 /* AACHARYA - use auto neg */
    if (mac->mac_unit == 0 ) {
       mac_set_flag(mac, ETH_FORCE_SPEED);
       
    if (mac->mii_intf == ATHR_MII)
        mac->forced_speed = ATHR_PHY_SPEED_100T;
    else
        mac->forced_speed = ATHR_PHY_SPEED_1000T;
    }
#endif

#ifdef ATH_S17_MAC0_SGMII
    mac_set_flag(mac, ATHR_S17_MAC0_SGMII);
#endif

    return 0;
}
/* 
 * SGMII serdes pll should be in the locked state.
 * To ensure the locked status. If the pll is not 
 * in locked state ? Play safe. 
 */

void serdes_pll_lock_detect_st(void)
{

    int sgmii_lock_st = 0;
    uint32_t mask = 0;
    
    sgmii_lock_st = athr_reg_rd(SGMII_SERDES_ADDRESS);

    if (!SGMII_SERDES_LOCK_DETECT_STATUS_GET(sgmii_lock_st)) {
        printk (MODULE_NAME ": Serdes PLL is not locked. Resetting...\n");
        mask =  RST_RESET_ETH_SGMII_RESET_SET(1) | 
                RST_RESET_ETH_SGMII_ARESET_SET(1);
        /* reset SGMII serdes */
        athr_reg_rmw_set(RST_RESET_ADDRESS, mask);

    	athr_reg_wr(ETH_SGMII_SERDES_ADDRESS,
    			ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK |
    			ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK |
    			ETH_SGMII_SERDES_EN_PLL_MASK);

        athr_reg_wr(SGMII_SERDES_ADDRESS, SGMII_SERDES_CDR_BW_SET(3)
                                         | SGMII_SERDES_TX_DR_CTRL_SET(1)
                                         | SGMII_SERDES_PLL_BW_SET(1)
                                         | SGMII_SERDES_EN_SIGNAL_DETECT_SET(1)
                                         | SGMII_SERDES_FIBER_SDO_SET(1)
                                         | SGMII_SERDES_VCO_REG_SET(3)
                                         | SGMII_SERDES_RES_CALIBRATION_SET(5));
        mdelay(50);
        athr_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_ETH_SGMII_ARESET_SET(1));
        mdelay(100);
        athr_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_ETH_SGMII_RESET_SET(1));
        mdelay(100);

        if (!(ath_reg_rd(SGMII_SERDES_ADDRESS) & SGMII_SERDES_LOCK_DETECT_STATUS_MASK))
        {
        	/* If SGMI serdes is not up we can't continue */
            assert(0);
        }
    } 

    printk (MODULE_NAME ": Serdes PLL is locked value %#x\n",athr_reg_rd(SGMII_SERDES_ADDRESS));
}

static int
check_dma_status_pause(athr_gmac_t *mac) {

    int RxFsm,TxFsm,RxFD,RxCtrl,TxCtrl;

    /*
     * If DMA is in pause state update the watchdog
     * timer to avoid MAC reset.
     */
    RxFsm = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RXFSM);
    TxFsm = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_TXFSM);
    RxFD  = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_XFIFO_DEPTH);
    RxCtrl = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RX_CTRL);
    TxCtrl = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_TX_CTRL);


    if (((RxFsm & ATHR_GMAC_DMA_DMA_STATE) == 0x3)
        && (((RxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) == 0x1)
        && (RxCtrl == 0x1) && (((RxFsm >> 11) & 0x1ff) == 0x1ff))  {
        DPRINTF("mac:%d RxFsm:%x TxFsm:%x\n",mac->mac_unit,RxFsm,TxFsm);
        return 0;
    }
    else if (((((TxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) <= 0x4) &&
            ((RxFsm & ATHR_GMAC_DMA_DMA_STATE) == 0x0) &&
            (((RxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) == 0x0)) ||
            (((RxFD >> 16) <= 0x20) && (RxCtrl == 1)) ) {
        return 1;
    }
    else {
        DPRINTF(" FIFO DEPTH = %x",RxFD);
        DPRINTF(" RX DMA CTRL = %x",RxCtrl);
        DPRINTF("mac:%d RxFsm:%x TxFsm:%x\n",mac->mac_unit,RxFsm,TxFsm);
        return 2;
    }
}

static int
check_for_dma_status(void *arg,int ac) {

    athr_gmac_t *mac         = (athr_gmac_t *)arg;
    athr_gmac_ring_t   *r    = &mac->mac_txring[ac];
    int                head  = r->ring_head, tail = r->ring_tail;
    athr_gmac_desc_t   *ds;
    athr_gmac_buffer_t *bp;

    /* If Tx hang is asserted reset the MAC and restore the descriptors
     * and interrupt state.
     */
    while (tail != head) // Check for TX DMA.
    {
        ds   = &r->ring_desc[tail];
        bp   =  &r->ring_buffer[tail];

        if(athr_gmac_tx_owned_by_dma(ds)) {
            if ((athr_gmac_get_diff(bp->trans_start,jiffies)) > ((1 * HZ/10))) {

                 /*
                  * If the DMA is in pause state reset kernel watchdog timer
                  */
                if(check_dma_status_pause(mac)) {
                    mac->mac_dev->trans_start = jiffies;
                    return 0;
                }

                printk(MODULE_NAME ": Tx Dma status eth%d : %s\n",mac->mac_unit,
                            athr_gmac_tx_stopped(mac) ? "inactive" : "active");

                qca955x_gmac_fast_reset(mac,ds,ac);

                break;
            }
        }
        tail = athr_gmac_ring_incr(r, tail);
    }

    if (check_dma_status_pause(mac) == 0)  //Check for RX DMA
    {
        if (mac->rx_dma_check) { // see if we holding the rx for 100ms
            uint32_t RxFsm;

            if (check_dma_status_pause(mac) == 0) {
                RxFsm = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RXFSM);
                printk(MODULE_NAME ": Rx Dma status eth%d : %X\n",mac->mac_unit,RxFsm);
                qca955x_gmac_fast_reset(mac,NULL,ac);
            }
            mac->rx_dma_check = 0;
        }
        else {
            mac->rx_dma_check = 1;
        }
    }

    return 0;
}

/*
 * Several fields need to be programmed based on what the PHY negotiated
 * Ideally we should quiesce everything before touching the pll, but:
 * 1. If its a linkup/linkdown, we dont care about quiescing the traffic.
 * 2. If its a single gigE PHY, this can only happen on lup/ldown.
 * 3. If its a 100Mpbs switch, the link will always remain at 100 (or nothing)
 * 4. If its a gigE switch then the speed should always be set at 1000Mpbs,
 *    and the switch should provide buffering for slower devices.
 *
 * XXX Only gigE PLL can be changed as a parameter for now. 100/10 is hardcoded.
 * XXX Need defines for them -
 * XXX FIFO settings based on the mode
 */
void qca955x_soc_gmac_set_link(void *arg, athr_phy_speed_t speed, int fdx)
{
   
    athr_gmac_t *mac = (athr_gmac_t *)arg;

    if (mac_has_flag(mac,ETH_FORCE_SPEED)) {
        speed = mac->forced_speed;
        fdx   = 1;
    }

    mac->mac_speed =  speed;
    mac->mac_fdx   =  fdx;
    qca955x_soc_gmac_set_mac_duplex(mac, fdx);
    
    athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_2_ADDRESS, (ATHR_GMAC_CFG2_IF_1000|
                         ATHR_GMAC_CFG2_IF_10_100));


    switch (speed)
    {
    case ATHR_PHY_SPEED_1000T:


        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS, ATHR_GMAC_CFG2_IF_1000);
        athr_gmac_reg_rmw_set(mac, AMCXFIF_CFG_5_ADDRESS, ATHR_GMAC_BYTE_PER_CLK_EN);

#ifndef ATH_RGMII_CAL
        if (mac->mii_phy == ATHR_S17_PHY)
             athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ETH_CFG_ETH_RXD_DELAY_SET(3)| 
                                                ETH_CFG_ETH_RXDV_DELAY_SET(3));
#endif
        
        if (mac->mii_intf == ATHR_SGMII) {
            athr_reg_wr(ETH_SGMII_ADDRESS, ETH_SGMII_GIGE_SET(1) |
                                           ETH_SGMII_CLK_SEL_SET(1));
        }

        if (mac->mii_intf == ATHR_MII) {
           athr_reg_wr(ETH_XMII_ADDRESS, ETH_XMII_PHASE1_COUNT_SET(1) |
                                         ETH_XMII_PHASE0_COUNT_SET(1));
        }

        if (mac->mii_intf == ATHR_RGMII) {
            ath_reg_wr(ATHR_GMAC_ETH_CFG, ETH_CFG_ETH_RXDV_DELAY_SET(3) |
                             ETH_CFG_ETH_RXD_DELAY_SET(3)|
                             ATHR_GMAC_ETH_CFG_RGMII_GE0);

            ath_reg_wr(ETH_XMII_ADDRESS, ETH_XMII_TX_INVERT_SET(1) |
                             ETH_XMII_RX_DELAY_SET(2)  |
                             ETH_XMII_TX_DELAY_SET(1)  |
                             ETH_XMII_GIGE_SET(1));
        }

        break;

    case ATHR_PHY_SPEED_100T:


        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS, ATHR_GMAC_CFG2_IF_10_100);
        athr_gmac_reg_rmw_clear(mac, AMCXFIF_CFG_5_ADDRESS, ATHR_GMAC_BYTE_PER_CLK_EN);

        athr_gmac_reg_rmw_set(mac, INTERFACE_CONTROL_ADDRESS, ATHR_GMAC_IFCTL_SPEED);


        if (mac->mac_unit == 0 && !mac->mac_fdx) {
           athr_gmac_reg_wr(mac, TXFIFO_TH_ADDRESS, TXFIFO_TH_HD_VAL);
           athr_gmac_reg_wr(mac, AMCXFIF_CFG_3_ADDRESS, AMCXFIF_CFG_3_HD_VAL);
        }


        if (mac->mii_intf == ATHR_SGMII) {
            athr_reg_wr(ETH_SGMII_ADDRESS, ETH_SGMII_PHASE0_COUNT_SET(1) |
                                           ETH_SGMII_PHASE1_COUNT_SET(1));
        }

        if (mac->mii_intf == ATHR_RGMII) {
            ath_reg_wr(ATHR_GMAC_ETH_CFG, ETH_CFG_ETH_RXDV_DELAY_SET(3) |
                            ETH_CFG_ETH_RXD_DELAY_SET(3)|
                            ATHR_GMAC_ETH_CFG_RGMII_GE0);

            ath_reg_wr(ETH_XMII_ADDRESS, ETH_XMII_TX_INVERT_SET(1) |
                             ETH_XMII_RX_DELAY_SET(2)  |
                            ETH_XMII_PHASE1_COUNT_SET (0x01)  |
                            ETH_XMII_PHASE0_COUNT_SET (0x01));
        }

        break;

    case ATHR_PHY_SPEED_10T:

        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS, ATHR_GMAC_CFG2_IF_10_100);
        athr_gmac_reg_rmw_clear(mac, AMCXFIF_CFG_5_ADDRESS, ATHR_GMAC_BYTE_PER_CLK_EN);

        athr_gmac_reg_rmw_clear(mac, INTERFACE_CONTROL_ADDRESS, ATHR_GMAC_IFCTL_SPEED);


        if (mac->mac_unit == 0 && !mac->mac_fdx) {
           athr_gmac_reg_wr(mac, TXFIFO_TH_ADDRESS , TXFIFO_TH_HD_VAL);
           athr_gmac_reg_wr(mac, AMCXFIF_CFG_3_ADDRESS, AMCXFIF_CFG_3_HD_VAL);
        }

        if (mac->mii_intf == ATHR_SGMII)
            athr_reg_wr(ETH_SGMII_ADDRESS, ETH_SGMII_PHASE0_COUNT_SET(19) |
                                           ETH_SGMII_PHASE1_COUNT_SET(19));

        if (mac->mii_intf == ATHR_RGMII) {
            ath_reg_wr(ATHR_GMAC_ETH_CFG, ETH_CFG_ETH_RXDV_DELAY_SET(3) |
                            ETH_CFG_ETH_RXD_DELAY_SET(3)|
                            ATHR_GMAC_ETH_CFG_RGMII_GE0);

            ath_reg_wr(ETH_XMII_ADDRESS, ETH_XMII_TX_INVERT_SET(1) |
                            ETH_XMII_PHASE1_COUNT_SET (0x13)  |
                            ETH_XMII_PHASE0_COUNT_SET (0x13));
        }

        break;

    default:
        assert(0);
    }
    DPRINTF(MODULE_NAME ": cfg_1: %#x\n", athr_gmac_reg_rd(mac, MAC_CONFIGURATION_1_ADDRESS));
    DPRINTF(MODULE_NAME ": cfg_2: %#x\n", athr_gmac_reg_rd(mac, MAC_CONFIGURATION_2_ADDRESS));
    DPRINTF(MODULE_NAME ": cfg_3: %#x\n", athr_gmac_reg_rd(mac, AMCXFIF_CFG_3_ADDRESS));
    DPRINTF(MODULE_NAME ": cfg_4: %#x\n", athr_gmac_reg_rd(mac, AMCXFIF_CFG_4_ADDRESS));
    DPRINTF(MODULE_NAME ": cfg_5: %#x\n", athr_gmac_reg_rd(mac, AMCXFIF_CFG_5_ADDRESS));
    printk (MODULE_NAME ": %s Done\n",__func__);

}



/*
 *Phy link state management.
 */

int qca955x_gmac_check_link(void *arg, int phyUnit)
{
    
    athr_phy_speed_t   speed;
    athr_gmac_t        *mac = (athr_gmac_t *) arg;
    struct net_device  *dev = mac->mac_dev;
    athr_phy_ops_t     *phy = mac->phy;
    int                carrier = netif_carrier_ok(dev);
    int                fdx, phy_up;
    int		       flags;

    spin_lock_irqsave(&mac->mac_lock, flags);

    assert(mac);
    assert(phy);

    qca955x_gmac_get_link_st(mac, &phy_up, &fdx, &speed, phyUnit);

    if (mac_has_flag(mac, ETH_FORCE_SPEED)) {
       if (mac->mii_intf == ATHR_MII)
           speed = ATHR_PHY_SPEED_100T;
       else
           speed = ATHR_PHY_SPEED_1000T;
           fdx =  1;
    }

    

    if (!phy_up)
    {
        if (carrier)
        {
            printk(MODULE_NAME ":unit %d: phy %0d not up carrier %d\n", mac->mac_unit, phyUnit, carrier);
            /* A race condition is hit when the queue is switched on while tx interrupts are enabled.
             * To avoid that disable tx interrupts when phy is down.
             */
            mac->link_up = 0;
            athr_gmac_intr_disable_tx(mac);
            netif_carrier_off(dev);
            netif_stop_queue(dev);
        }
        
        if (mac->mac_unit == 0)
            athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_1_ADDRESS, 
                (MAC_CONFIGURATION_1_RX_ENABLE_SET(1) | MAC_CONFIGURATION_1_TX_ENABLE_SET(1)));

        goto done;
    }

    if(!mac->mac_ifup)
        goto done;
    /*
     * phy is up. Either nothing changed or phy setttings changed while we
     * were sleeping.
     */

    if ((fdx < 0) || (speed < 0))
    {
        printk(MODULE_NAME ": phy not connected?\n");
        goto done;
    }

    if (carrier && (speed == mac->mac_speed) && (fdx == mac->mac_fdx))
        goto done;

    
    if (phy->is_alive(phyUnit))
    {
        printk(MODULE_NAME ": Enet Unit:%d PHY:%d is UP ", mac->mac_unit,phyUnit);
        printk("eth%d %s %s %s\n", mac->mac_unit, mii_intf[mac->mii_intf],
               spd_str[speed], dup_str[fdx]);

        if(mac->ops->soc_gmac_set_link)
            mac->ops->soc_gmac_set_link(mac, speed, fdx);

        printk(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl  \n",
           athr_gmac_reg_rd(mac, MAC_CONFIGURATION_2_ADDRESS),
           athr_gmac_reg_rd(mac, ATHR_GMAC_IFCTL));

        if (!mac_has_flag(mac,WAN_QOS_SOFT_CLASS) && !(mac->mii_phy == ATHR_VIR_PHY))
           qca955x_gmac_fast_reset(mac,NULL,0);

       /* 
        * only Enable Tx and Rx  afeter configuring the mac speed.
        */

        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS,
             (MAC_CONFIGURATION_1_RX_ENABLE_SET(1) | MAC_CONFIGURATION_1_TX_ENABLE_SET(1)));


        /*
         * in business
         */
        netif_carrier_on(dev);
        netif_start_queue(dev);
        mac->link_up = 1;

    } else {
        printk(MODULE_NAME ": Enet Unit:%d PHY:%d is Down.\n", mac->mac_unit,phyUnit);
    }

done:
    spin_unlock_irqrestore(&mac->mac_lock, flags);
    return 0;
    
}

int qca955x_gmac_mii_setup(void *arg)
{
   athr_gmac_t *mac = (athr_gmac_t *)arg;
   uint8_t mgmt_cfg_val = 0 ; 

   if (mac == NULL) {
       printk (MODULE_NAME "Error while configuring the mii\n");
       return -1;
   }
   
   /* 
    * eth0 RGMII and eth1 SGMII == 0x1;
    * eth0 SGMII and eth1 RGMII == 0x41;
    */
   switch (mac->mii_intf) {
       case ATHR_SGMII:
           printk (MODULE_NAME": eth%d in SGMII MODE\n", mac->mac_unit);
           if (mac->mac_unit == 0)
               athr_reg_rmw_set(ETH_CFG_ADDRESS, ETH_CFG_GE0_SGMII_SET(1));     
           break;

       case ATHR_RGMII:
           printk (MODULE_NAME": eth%d in RGMII MODE\n", mac->mac_unit);
           athr_reg_rmw_set(ETH_CFG_ADDRESS, ETH_CFG_RGMII_GE0_SET(1));
           break;
       case ATHR_MII:
           printk (MODULE_NAME": eth%d in MII MODE\n", mac->mac_unit);
           athr_reg_wr(ETH_CFG_ADDRESS, ETH_CFG_MII_GE0_SET(1)|
                                        ETH_CFG_MII_GE0_SLAVE_SET(1));
           /* 
            * Gpio mappings for mii
            * GPIO4 Tx_error
            * GPIO13 Rx_error
            * GPIO22 CRS
            * GPIO23 Collisions.
            */
            athr_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << 4));
            ath_reg_rmw_set(GPIO_OUT_FUNCTION4_ADDRESS, 0x4b); 

            ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 13));
            ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 22));
            ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 23));

            athr_reg_wr(GPIO_IN_ENABLE2_ADDRESS, (athr_reg_rd(GPIO_IN_ENABLE2_ADDRESS) & 
                                                 (GPIO_IN_ENABLE2_SLICEXT_MCLK_MASK))  |
                                                  GPIO_IN_ENABLE2_ETH__RX_ERR_SET(0xd) |
                                                  GPIO_IN_ENABLE2_ETH__RX_COL_SET(0x16)|
                                                  GPIO_IN_ENABLE2_ETH__RX_CRS_SET(0x17));
           

       default:
           break;
   }

   qca955x_gmac_sgmii_res_cal();
   if (mac->mii_phy == ATHR_S17_PHY) {
        printk ("Scorpion -----> S17 PHY\n");
        //mgmt_cfg_val = 0xb;
        mgmt_cfg_val = 0x7;
        qca955x_gmac_rgmii_cal();
        athr_gmac_reg_wr(mac, MII_MGMT_CONFIGURATION_ADDRESS, mgmt_cfg_val 
            | MII_MGMT_CONFIGURATION_RESET_MGMT_SET(1));

        athr_gmac_reg_wr(mac, MII_MGMT_CONFIGURATION_ADDRESS, mgmt_cfg_val);
        return 0;

    }
    if (mac->mii_phy == ATHR_AR8033_PHY) {
        printk ("Scorpion -----> 8033 PHY\n");
        mgmt_cfg_val = 0x7;
        athr_gmac_reg_wr(mac, MII_MGMT_CONFIGURATION_ADDRESS, mgmt_cfg_val 
            | MII_MGMT_CONFIGURATION_RESET_MGMT_SET(1));

        athr_gmac_reg_wr(mac, MII_MGMT_CONFIGURATION_ADDRESS, mgmt_cfg_val);
        return 0;

    }
   return 0;

   
}

/*
 * Configures the CFG registers of GMAC
 * in this function. RX TX flowcontrol of G-MAC  
 * are enabled by deafault.
 * Return 0 -- success.
         -1 -- failure.
 */

int qca955x_gmac_hw_setup(void *arg) 
{
    athr_gmac_t *mac = (athr_gmac_t *)arg;
    athr_gmac_ring_t *tx, *rx = &mac->mac_rxring;
    athr_gmac_desc_t *r0, *t0;
    uint8_t ac;

    if (mac == NULL) {
        printk (MODULE_NAME "Error while setting up the G-mac\n");
       return -1;
    } 

    /* sgmii  mac mode init */
    if ( mac->mii_intf == ATHR_SGMII) {
        athr_gmac_sgmii_setup(mac);
    }

    /* clear the rx fifo state if any */
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_STATUS,
                  athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS));

    /* set mac basic configs */
    athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS, 
                    (MAC_CONFIGURATION_2_PAD_CRC_SET(1) |
                    MAC_CONFIGURATION_2_LENGTH_CHECK_SET(1)));

    athr_gmac_reg_wr(mac, AMCXFIF_CFG_0_ADDRESS, AMCXFIF_CFG_0_VAL);

    athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS, 
                   MAC_CONFIGURATION_1_RX_FLOW_SET(1));

   /* 
    * If PHY is connected enable the TX FLOW CONTROL
    * based on duplex cfg. If half duplex disable tx flowctrl.
    * currently handled for 8033 PHY.
    */
    if (!(mac->mii_phy == ATHR_AR8033_PHY)) {
        athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS, 
                   MAC_CONFIGURATION_1_TX_FLOW_SET(1));
    } else {
        if (mac->mac_fdx) {
                /* full duplex */
            athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS, 
                   MAC_CONFIGURATION_1_TX_FLOW_SET(1));
        } else {
                /* half duplex.*/
            athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_1_ADDRESS, 
                   MAC_CONFIGURATION_1_TX_FLOW_SET(1));
        }
    } 

    /* Disable ATHR_GMAC_CFG2_LEN_CHECK to fix the bug that
     * the mac address is mistaken as length when enabling Atheros header
     */
    if (mac_has_flag(mac, ATHR_HEADER_ENABLED))
        athr_gmac_reg_rmw_clear(mac, MAC_CONFIGURATION_2_ADDRESS,
                   MAC_CONFIGURATION_2_LENGTH_CHECK_SET(1));


    athr_gmac_reg_wr(mac, AMCXFIF_CFG_1_ADDRESS, AMCXFIF_CFG_1_VAL);

    athr_gmac_reg_wr(mac, AMCXFIF_CFG_2_ADDRESS, AMCXFIF_CFG_2_VAL);

    /*
     * Weed out junk frames (CRC errored, short collision'ed frames etc.)
     */
    athr_gmac_reg_wr(mac, AMCXFIF_CFG_4_ADDRESS, AMCXFIF_CFG_4_VAL);

    /*
     * Drop CRC Errors, Pause Frames ,Length Error frames, Truncated Frames
     * dribble nibble and rxdv error frames.
     */
    printk("Setting Drop CRC Errors, Pause Frames and Length Error frames \n");

    athr_gmac_reg_wr(mac, AMCXFIF_CFG_5_ADDRESS, 0xe6be2);
    
     /*
      * Configure CFG3 and TX FIFO
      */
    athr_gmac_reg_wr(mac, AMCXFIF_CFG_3_ADDRESS, AMCXFIF_CFG_3_VAL);
    athr_gmac_reg_wr(mac, TXFIFO_TH_ADDRESS, TXFIFO_TH_VAL);

    if (mac_has_flag(mac,ATHR_JUMBO_FR)) {
       athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_2_ADDRESS,
                      MAC_CONFIGURATION_2_HUGE_FRAME_SET(1));
       athr_gmac_reg_wr(mac, MAXIMUM_FRAME_LENGTH_OFFSET, frame_sz);
    }

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {
        athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_ARB_CFG, ATHR_GMAC_TX_QOS_MODE_WEIGHTED
                    | ATHR_GMAC_TX_QOS_WGT_0(0x7)
                    | ATHR_GMAC_TX_QOS_WGT_1(0x5)
                    | ATHR_GMAC_TX_QOS_WGT_2(0x3)
                    | ATHR_GMAC_TX_QOS_WGT_3(0x1));

        for(ac = 0;ac < mac->mac_noacs; ac++) {
            tx = &mac->mac_txring[ac];
            t0  =  &tx->ring_desc[0];
            switch(ac) {
                case ENET_AC_VO:
                    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q0, athr_gmac_desc_dma_addr(tx, t0));
                    break;
                case ENET_AC_VI:
                    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q1, athr_gmac_desc_dma_addr(tx, t0));
                    break;
                case ENET_AC_BK:
                    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q2, athr_gmac_desc_dma_addr(tx, t0));
                    break;
                case ENET_AC_BE:
                    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q3, athr_gmac_desc_dma_addr(tx, t0));
                    break;
            };
        }
    }
    else {
        tx = &mac->mac_txring[0];
        t0  =  &tx->ring_desc[0];
        athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q0, athr_gmac_desc_dma_addr(tx, t0));
    }

    r0  =  &rx->ring_desc[0];
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_DESC, athr_gmac_desc_dma_addr(rx, r0));

    DPRINTF(MODULE_NAME ": cfg1 %#x cfg2 %#x\n", athr_gmac_reg_rd(mac, MAC_CONFIGURATION_1_ADDRESS),
        athr_gmac_reg_rd(mac, MAC_CONFIGURATION_2_ADDRESS));
        

    return 0;
}


int qca955x_gmac_attach(void *arg )
{

    athr_gmac_t     *mac = (athr_gmac_t *)arg;
    athr_gmac_ops_t *ops = mac->ops;

    if (!ops) {
       ops = (athr_gmac_ops_t *)kmalloc(sizeof(athr_gmac_ops_t), GFP_KERNEL);
    }
     
    printk(MODULE_NAME ": %s\n",__func__); 

    memset(ops,0,sizeof(athr_gmac_ops_t));

    ops->set_caps           = qca955x_set_gmac_caps;
    ops->soc_gmac_hw_setup  = qca955x_gmac_hw_setup;
    ops->soc_gmac_mii_setup = qca955x_gmac_mii_setup;
    ops->check_link 	    = qca955x_gmac_check_link;
    ops->check_dma_st       = check_for_dma_status;
    ops->soc_gmac_set_link  = qca955x_soc_gmac_set_link;
    ops->soft_led           = NULL;
    ops->set_pll            = NULL;
    ops->poll_link	    = qca955x_gmac_poll_link;

    mac->ops = ops;

    mac->intr_mask = (u_int32_t) ATHR_GMAC_INTR_RX;
    mac->mac_addr = (char *) (mac->mac_unit ? ATH_EEPROM_GE1_MAC_ADDR :
      ATH_EEPROM_GE0_MAC_ADDR);

    mac->ddr_flush = mac->mac_unit ? AR934X_DDR_REG_FLUSH_GE1 :
      AR934X_DDR_REG_FLUSH_GE0;

   if(mac->mac_unit == 0) {
      mac->mii_intf      = ATHR_RGMII;
      mac->mii_phy       = ATHR_AR8033_PHY;
   }
    return 0;
}

/*
 * Copied from U-Boot Atheros LSDK-9.5.5.36
 */
static void
qca955x_gmac_sgmii_res_cal(void)
{
   unsigned int read_data, read_data_otp, otp_value, otp_per_val, rbias_per;
   unsigned int read_data_spi;
   unsigned int *address_spi = (unsigned int *)0xbffffffc;
   unsigned int rbias_pos_or_neg, res_cal_val;
   unsigned int sgmii_pos, sgmii_res_cal_value;
   unsigned int reversed_sgmii_value, use_value;

   ath_reg_wr(OTP_INTF2_ADDRESS, 0x7d);
   ath_reg_wr(OTP_LDO_CONTROL_ADDRESS, 0x0);

   while (ath_reg_rd(OTP_LDO_STATUS_ADDRESS) & OTP_LDO_STATUS_POWER_ON_MASK);

   read_data = ath_reg_rd(OTP_MEM_0_ADDRESS + 4);

   while (!(ath_reg_rd(OTP_STATUS0_ADDRESS) & OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK));

   read_data_otp = ath_reg_rd(OTP_STATUS1_ADDRESS);

   if (read_data_otp & 0x1fff) {
      read_data = read_data_otp;
   } else {
      read_data_spi = *(address_spi);
      if ((read_data_spi & 0xffff0000) == 0x5ca10000) {
         read_data = read_data_spi;
      } else {
         read_data = read_data_otp;
      }
   }

   if (read_data & 0x00001000) {
      otp_value = (read_data & 0xfc0) >> 6;
   } else {
      otp_value = read_data & 0x3f;
   }

   if (otp_value > 31) {
      otp_per_val = 63 - otp_value;
      rbias_pos_or_neg = 1;
   } else {
      otp_per_val = otp_value;
      rbias_pos_or_neg = 0;
   }

   rbias_per = otp_per_val * 15;

   if (rbias_pos_or_neg == 1) {
      res_cal_val = (rbias_per + 34) / 21;
      sgmii_pos = 1;
   } else {
      if (rbias_per > 34) {
         res_cal_val = (rbias_per - 34) / 21;
         sgmii_pos = 0;
      } else {
         res_cal_val = (34 - rbias_per) / 21;
         sgmii_pos = 1;
      }
   }

  if (sgmii_pos == 1) {
      sgmii_res_cal_value = 8 + res_cal_val;
   } else {
      sgmii_res_cal_value = 8 - res_cal_val;
   }

   reversed_sgmii_value = 0;
   use_value = 0x8;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) >> 3);
   use_value = 0x4;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) >> 1);
   use_value = 0x2;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) << 1);
   use_value = 0x1;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) << 3);

   reversed_sgmii_value &= 0xf;

   printk (MODULE_NAME ": %s cal value = 0x%x\n", __func__, reversed_sgmii_value);

   // To Check the locking of the SGMII PLL

   read_data = (ath_reg_rd(SGMII_SERDES_ADDRESS) &
            ~SGMII_SERDES_RES_CALIBRATION_MASK) |
         SGMII_SERDES_RES_CALIBRATION_SET(reversed_sgmii_value);

   ath_reg_wr(SGMII_SERDES_ADDRESS, read_data);


   ath_reg_wr(ETH_SGMII_SERDES_ADDRESS,
         ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK |
         ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK |
         ETH_SGMII_SERDES_EN_PLL_MASK);

   ath_reg_rmw_set(SGMII_SERDES_ADDRESS,
         SGMII_SERDES_CDR_BW_SET(3) |
         SGMII_SERDES_TX_DR_CTRL_SET(1) |
         SGMII_SERDES_PLL_BW_SET(1) |
         SGMII_SERDES_EN_SIGNAL_DETECT_SET(1) |
         SGMII_SERDES_FIBER_SDO_SET(1) |
         SGMII_SERDES_VCO_REG_SET(3));

   ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_ETH_SGMII_ARESET_MASK);
   udelay(25);
   ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_ETH_SGMII_RESET_MASK);

   while (!(ath_reg_rd(SGMII_SERDES_ADDRESS) & SGMII_SERDES_LOCK_DETECT_STATUS_MASK));
}

static void
qca955x_gmac_rgmii_cal(void)
{
   ath_reg_wr(SWITCH_CLOCK_SPARE_ADDRESS, 0x520);

   ath_reg_wr(ATHR_GMAC_ETH_CFG, ETH_CFG_ETH_RXDV_DELAY_SET(3) |
               ETH_CFG_ETH_RXD_DELAY_SET(3)|
               ATHR_GMAC_ETH_CFG_RGMII_GE0);

   ath_reg_wr(ETH_XMII_ADDRESS, ETH_XMII_TX_INVERT_SET(1) |
                             ETH_XMII_RX_DELAY_SET(2)  |
                             ETH_XMII_TX_DELAY_SET(1)  |
                    ETH_XMII_GIGE_SET(1));

   udelay(1000);

   return;
}
