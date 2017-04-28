#include "athrs_mac.h"

#define MODULE_NAME "ATH_MAC_TIMER"

extern int athr_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, 
                                 athr_phy_speed_t *speed,int phyUnit);

extern void athr_gmac_set_mac_from_link(athr_gmac_t *mac, 
                                        athr_phy_speed_t speed, int fdx);

static int enable_timer = 0;
static int mii0_if = ATHR_GMAC_MII0_INTERFACE;
static int mii1_if = ATHR_GMAC_MII1_INTERFACE;

static char *mii_str[2][4] = {
    {"GMii", "Mii", "RGMii", "RMii"},
    {"GMii","Mii","RGMii", "RMii"}
};



/*
 * phy link state management
 */
static int
athr_gmac_poll_link(athr_gmac_t *mac)
{
    struct net_device  *dev     = mac->mac_dev;
    int                 carrier = netif_carrier_ok(dev), fdx, phy_up;
    athr_phy_speed_t  speed;
    int  rc,phyUnit = 0;

    assert(mac->mac_ifup);
    if (mac->mac_unit == 0)
        phyUnit = 4;

    rc = athr_gmac_get_link_st(mac, &phy_up, &fdx, &speed, phyUnit);

    if (rc < 0)
        goto done;

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

            if (is_ar934x() && mac->mac_unit == 0 )
                athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, (ATHR_GMAC_CFG1_RX_EN | ATHR_GMAC_CFG1_TX_EN));


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

    if (athr_chk_phy_in_rst(mac))
        goto done;

    if (carrier && (speed == mac->mac_speed ) && (fdx == mac->mac_fdx)) {
        goto done;
    }

    printk(MODULE_NAME ": enet unit:%d is up...\n", mac->mac_unit);
    printk("%s %s %s\n", mii_str[mac->mac_unit][mii_if(mac)],
       spd_str[speed], dup_str[fdx]);

    athr_gmac_set_mac_from_link(mac, speed, fdx);

    printk(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl  \n",
       athr_gmac_reg_rd(mac, ATHR_GMAC_CFG2),
       athr_gmac_reg_rd(mac, ATHR_GMAC_IFCTL));

   /* 
    * only Enable Tx and Rx  afeter configuring the mac speed.
    */

    if (is_ar934x())
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, (ATHR_GMAC_CFG1_RX_EN | ATHR_GMAC_CFG1_TX_EN));

   
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
athr_gmac_timer_func(athr_gmac_t *mac)
{
   athr_gmac_ops_t *ops = mac->ops;

   if (mac_has_flag(mac,ETH_SOFT_LED) && ops->soft_led) {
      if (athr_gmac_get_diff(mac->led_blink_ts,jiffies) >= (ATH_GMAC_LED_BLINK_FREQ)) {
         ops->soft_led(mac);
         mac->led_blink_ts = jiffies;
      }
   }
   if (mac_has_flag(mac,CHECK_DMA_STATUS)&& ops->check_dma_st) { 
      if(athr_gmac_get_diff(mac->dma_chk_ts,jiffies) >= (ATH_GMAC_CHECK_DMA_FREQ)) {
         if (mac->mac_ifup) ops->check_dma_st(mac,0);
         mac->dma_chk_ts = jiffies;
      }
   }
   if (mac_has_flag(mac,ETH_LINK_POLL)) {
      if (athr_gmac_get_diff(mac->link_ts,jiffies) >= (ATH_GMAC_POLL_LINK_FREQ)) {
         athr_gmac_poll_link(mac);
         mac->link_ts = jiffies;
      }
   }
   mod_timer(&mac->mac_phy_timer,(jiffies + mac->timer_freq));
   return 0;
}

void 
athr_gmac_timer_init(athr_gmac_t *mac) 
{
 
    if (mac_has_flag(mac,ETH_LINK_POLL)) {
        mac->timer_freq = ATH_GMAC_POLL_LINK_FREQ; 
        enable_timer = 1;
    }
    if (mac_has_flag(mac,CHECK_DMA_STATUS)) {
        mac->timer_freq = ATH_GMAC_CHECK_DMA_FREQ; 
        enable_timer = 1;
    }
    if (mac_has_flag(mac,ETH_SOFT_LED)) {
        mac->timer_freq = ATH_GMAC_LED_BLINK_FREQ; 
        enable_timer = 1;
    }
    if (enable_timer) {
        init_timer(&mac->mac_phy_timer);
        mac->mac_phy_timer.data     = (unsigned long)mac;
        mac->mac_phy_timer.function = (void *)athr_gmac_timer_func;
        athr_gmac_timer_func(mac);
    }

}

void
athr_gmac_timer_del(athr_gmac_t *mac)
{
   if(enable_timer)
      del_timer(&mac->mac_phy_timer);
}
