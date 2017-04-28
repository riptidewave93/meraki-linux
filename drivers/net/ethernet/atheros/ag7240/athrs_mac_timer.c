#include "athrs_mac.h"

#define MODULE_NAME "ATH_MAC_TIMER"

extern int athr_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, 
                                 athr_phy_speed_t *speed,int phyUnit);

extern int  qca955x_gmac_poll_link(athr_gmac_t *mac);
extern void athr_gmac_set_mac_from_link(athr_gmac_t *mac, 
                                        athr_phy_speed_t speed, int fdx);
extern void athr_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac);


static int enable_timer = 0;
#if defined(CONFIG_SOC_AR724X) || defined(CONFIG_SOC_AR934X) || defined(CONFIG_MACH_HORNET)
static char *mii_str[2][4] = {
    {"GMii", "Mii", "RGMii", "RMii"},
    {"GMii","Mii","RGMii", "RMii"}
};
static int last_link = 0;
#endif




#if HYBRID_LINK_CHANGE_EVENT
athr_hybrid_lce_t athr_hybrid_lce[HYBRID_MAX_VLAN];
static int rc_old = 0;

static inline void hybrid_netif_on(char *name)
{
    struct net_device *dev;
    dev = dev_get_by_name(&init_net, name);
    if(dev != NULL){
        netif_carrier_on(dev);
        dev_put(dev);
    }
}
static inline void hybrid_netif_off(char *name)
{
    struct net_device *dev;
    dev = dev_get_by_name(&init_net, name);
    if(dev != NULL){
        netif_carrier_off(dev);
        dev_put(dev);
    }
}
#endif
/*
 * phy link state management
 */
#if defined(CONFIG_SOC_AR724X) || defined(CONFIG_SOC_AR934X) || defined(CONFIG_MACH_HORNET)
int
athr_gmac_poll_link(void *arg)
{
    athr_gmac_t *mac = (athr_gmac_t *)arg;
    struct net_device  *dev     = mac->mac_dev;
    int                 carrier = netif_carrier_ok(dev), fdx, phy_up;
    athr_phy_speed_t  speed;

    int  rc,phyUnit = 0;
    athr_phy_ops_t *phy = mac->phy;
#if HYBRID_LINK_CHANGE_EVENT
    int i;
#endif


    assert(mac->mac_ifup);
    if (mac->mac_unit == 0)
        phyUnit = 4;

    rc = athr_gmac_get_link_st(mac, &phy_up, &fdx, &speed, phyUnit);

#if HYBRID_LINK_CHANGE_EVENT
    for(i=0; i<HYBRID_MAX_VLAN; i++){
        if(athr_hybrid_lce[i].vlanid == 0)
            continue;
        /* For hybrid with vlan -- 
         * rc is divided into (HYBRID_MAX_VLAN)units, each unit includs 3(HYBIRD_COUNT_BIT) bits, 
         * these 3 bits are uesed to count the port which is UP in the vlan.
         */
        if((((rc>>(i*HYBIRD_COUNT_BIT))&HYBIRD_COUNT_BITMAP) != 0) && 
                (((rc_old>>(i*HYBIRD_COUNT_BIT))&HYBIRD_COUNT_BITMAP) == 0)){
            hybrid_netif_on(athr_hybrid_lce[i].name);
        }else if(((rc>>(i*HYBIRD_COUNT_BIT))&HYBIRD_COUNT_BITMAP) == 0){
            hybrid_netif_off(athr_hybrid_lce[i].name);
        }
    }
    rc_old = rc;
#endif
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

            if (soc_is_ar934x() && mac->mac_unit == 0 )
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
    /*
     *  Per port threshold value and Global threshold value of 
     *  s17 switch is changed based on the no of liks detected.
     */

        if (soc_is_ar934x() && is_s17()) {

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
    printk("%s %s %s\n", mii_str[mac->mac_unit][mii_if(mac)],
       spd_str[speed], dup_str[fdx]);

    athr_gmac_set_mac_from_link(mac, speed, fdx);
    

    printk(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl  \n",
       athr_gmac_reg_rd(mac, ATHR_GMAC_CFG2),
       athr_gmac_reg_rd(mac, ATHR_GMAC_IFCTL));

    if (is_f1e())
       athr_gmac_fast_reset(mac,NULL,0);
   /* 
    * only Enable Tx and Rx  after configuring the mac speed.
    */

    if (soc_is_ar934x())
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
#endif

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
	 athr_poll_link(mac);
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
