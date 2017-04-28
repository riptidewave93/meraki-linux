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

#define MODULE_NAME "AG7240_MAC"

extern void athr_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac);

static int
ar7240_set_gmac_caps(void *arg)
{
   athr_gmac_t *mac = (athr_gmac_t *)arg;

#ifdef CONFIG_ATHEROS_HEADER_EN
    if (mac->mac_unit == 1) {
        if (is_s26())
           mac_set_flag(mac,ATHR_S26_HEADER);
        if (is_s27())
           mac_set_flag(mac,ATHR_S27_HEADER);
    }

    if (is_s16())
        mac_set_flag(mac,ATHR_S16_HEADER);
#endif        

#ifdef CONFIG_ATHR_VLAN_IGMP
         mac_set_flag(mac,ATHR_VLAN_IGMP);
#endif

#ifdef CONFIG_ETH_SOFT_LED
     if (is_ar7240() || is_ar933x()) {
        if ( !is_ar933x_12() ) {
            mac_set_flag(mac,ETH_SOFT_LED);
            if (is_ar933x() && mac->mac_unit == 0) {
                athr_reg_wr(ATHR_GPIO_FUNCTIONS, 
                    (athr_reg_rd(ATHR_GPIO_FUNCTIONS) & (~0xf8)));
            }
        }
    }
#endif

#ifdef CONFIG_CHECK_DMA_STATUS
    mac_set_flag(mac,CHECK_DMA_STATUS);
#endif

#ifdef CONFIG_S26_SWITCH_ONLY_MODE
    if (is_ar7241()) {
        mac_set_flag(mac,ETH_SWONLY_MODE);
    }
#endif

#ifdef CONFIG_GMAC0_RXFCTL
    if (mac->mac_unit == 0)
        mac_set_flag(mac,ATHR_RX_FLCTL);
#endif

#ifdef CONFIG_GMAC0_TXFCTL
    if (mac->mac_unit == 0)
        mac_set_flag(mac,ATHR_TX_FLCTL);
#endif

#ifdef CONFIG_GMAC1_RXFCTL
    if (mac->mac_unit == 1 || is_ar7242())
        mac_set_flag(mac,ATHR_RX_FLCTL);
#endif

#ifdef CONFIG_GMAC1_TXFCTL
    if (mac->mac_unit == 1 || is_ar7242())
        mac_set_flag(mac,ATHR_TX_FLCTL);
#endif

#ifdef CONFIG_ATHR_RX_TASK
    mac_set_flag(mac,ATHR_RX_TASK);
#else
    mac_set_flag(mac,ATHR_RX_POLL);
#endif
    mac_set_flag(mac,ETH_LINK_INTERRUPT);

    if (mac->mac_unit == 1 && !is_ar7242()) {
       mac_set_flag(mac,ETH_FORCE_SPEED);
       mac->forced_speed = ATHR_PHY_SPEED_1000T;
    }
    if (mac->mac_unit == 0 && (is_f1e() || 
           is_ar7242() || is_emu() || is_s16()) )  {
        mac_set_flag(mac,ETH_LINK_POLL);
        mac_clear_flag(mac,ETH_LINK_INTERRUPT);
    }

    if (mac_has_flag(mac,ATHR_S26_HEADER)  || 
         mac_has_flag(mac,ATHR_S27_HEADER) || 
         mac_has_flag(mac,ATHR_S16_HEADER)) {
 
         mac_set_flag(mac,ATHR_HEADER_ENABLED);

    }
         
    return 0;
}
#ifdef CONFIG_SET_GMAC_PLL
static void
ar7240_set_pll(void *arg,int speed) {

   athr_gmac_t *mac = (athr_gmac_t *)arg;

   if (is_ar7242() && mac->mac_unit == 0) {
      switch (speed)
       {
       case ATHR_PHY_SPEED_1000T:
           athrs_reg_wr(ATHR_GMAC_PLL_CONFIG,ETH_1000_PLL);
           break;

       case ATHR_PHY_SPEED_100T:
           athrs_reg_wr(ATHR_GMAC_PLL_CONFIG,ETH_100_PLL);
           break;

       case ATHR_PHY_SPEED_10T:
           athrs_reg_wr(ATHR_GMAC_PLL_CONFIG,ETH_10_PLL);
           break;

       default:
           assert(0);
       }
   }
   return 0;
}
#endif
#ifdef CONFIG_CHECK_DMA_STATUS
static int 
check_dma_status_pause(athr_gmac_t *mac) { 

    int RxFsm,TxFsm,RxFD,RxCtrl,TxCtrl;

    /*
     * If get called by tx timeout for other chips we assume
     * the DMA is in pause state and update the watchdog
     * timer to avoid MAC reset.
     */
    if(!is_ar7240())
       return 1;

    RxFsm = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RXFSM);
    TxFsm = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_TXFSM);
    RxFD  = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_XFIFO_DEPTH);
    RxCtrl = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_RX_CTRL);
    TxCtrl = athr_gmac_reg_rd(mac,ATHR_GMAC_DMA_TX_CTRL);


    if ((RxFsm & ATHR_GMAC_DMA_DMA_STATE) == 0x3 
        && ((RxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) == 0x6) {
        return 0;
    }
    else if ((((TxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) <= 0x0) && 
             ((RxFsm & ATHR_GMAC_DMA_DMA_STATE) == 0x0) && 
             (((RxFsm >> 4) & ATHR_GMAC_DMA_AHB_STATE) == 0x0) && 
             (RxFD  == 0x0) && (RxCtrl == 1) && (TxCtrl == 1)) {
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
    while (tail != head)
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

                athr_gmac_fast_reset(mac,ds,ac);

                break;
            }
        }
        athr_gmac_ring_incr(tail);
    }
    return 0;
}
#endif
#ifdef CONFIG_ETH_SOFT_LED

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
void led_event(struct work_struct *work)
#else
void led_event(void *data)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
    athr_gmac_t *active_mac =
               container_of(work, athr_gmac_t, led_event);
#else
    athr_gmac_t *active_mac = (athr_gmac_t *) data;
#endif
    extern athr_gmac_t *athr_gmacs[2];
    extern int phy_in_reset;
    extern atomic_t Ledstatus;

    uint32_t i=0,cnt,reg_addr;
    const LED_BLINK_RATES  *bRateTab; 
    static uint32_t pkt_count;
    athr_gmac_t *mac = athr_gmacs[1];
    ath_led_ctrl_t *pledctrl = &active_mac->ledctrl;

    atomic_inc(&Ledstatus);
  
    /* 
     *  MDIO access will fail While PHY is in RESET phase.
     */
    if(phy_in_reset)
        goto done;
    if (active_mac->mac_unit == 1) {
    /*
     * Process LAN port
     */
        if (mac->mac_ifup) {
            for (i = 0 ; i < 4 ; i ++) {
                bRateTab = BlinkRateTable_100M;
                if (pledctrl->link[i]) { 
                    reg_addr = 0x2003c + ((i + 1) << 8);
                    /* Rx good byte count */
                    cnt = athrs26_reg_read(reg_addr);
                    
                    reg_addr = 0x20084 + ((i + 1) << 8);
                    /* Tx good byte count */
                    cnt = cnt + athrs26_reg_read(reg_addr);
    
                    if (cnt == 0) {
                        if(is_ar933x()) {
                            mac->phy->write_phy_reg(0, i,0x19, 0x3c0);
                            mac->phy->write_phy_reg(0, i,0x18,(0x4));
                        } else {
                            mac->phy->write_phy_reg(0, i,0x19,(mac->phy->read_phy_reg(0, i,0x19) | (0x3c0)));
                        }
                        continue;
                    }
                    if (pledctrl->speed[i] == ATHR_PHY_SPEED_10T) {
                        bRateTab = BlinkRateTable_10M;
                    }
                    while (bRateTab++) {
                        if (cnt <= bRateTab->rate) {
                            if(is_ar933x()&& bRateTab->rate <= MB(5)){
    				            mac->phy->write_phy_reg(0, i,0x18, 0);
                            } else {
                           	    mac->phy->write_phy_reg(0, i,0x18,((bRateTab->timeOn << 12)|
                                    (bRateTab->timeOff << 8)));
                            }
                            if(is_ar933x()) {
                                mac->phy->write_phy_reg(0, i,0x19, 0x140);                            
                            } else {
                                mac->phy->write_phy_reg(0, i,0x19,(mac->phy->read_phy_reg(0, i,0x19) & ~(0x280)));
                            }
                            break;
                        }
                    }
                } else {
    		        mac->phy->write_phy_reg(0, i,0x19,0x0);
                }
            }
            /*
             * MIB counter will be clear to zero after read it, 
             * therefore don't need to set Flush all LAN MIB counters configuration again.
             * Otherwise it may miss processiong some LAN MIB counters.
             */
            if(!is_ar933x()) {
                /* Flush all LAN MIB counters */
                athrs26_reg_write(0x80,((1 << 17) | (1 << 24))); 
                while ((athrs26_reg_read(0x80) & 0x20000) == 0x20000);
                athrs26_reg_write(0x80,0);
            }    
        }
    } else {
    /*
     * Process WAN port
     */
        mac = athr_gmacs[0];
        bRateTab = BlinkRateTable_100M;
        cnt = 0;
        if (mac->mac_ifup) {
            if (pledctrl->link[4]) {
                cnt += athr_gmac_reg_rd(mac,ATHR_GMAC_RX_BYTES_CNTR) +
                           athr_gmac_reg_rd(mac,ATHR_GMAC_TX_BYTES_CNTR);
    
                if (athr_gmac_get_diff(pkt_count,cnt) == 0) {
                    if(is_ar933x()) {
                        mac->phy->write_phy_reg(0, 4,0x19, 0x3c0);
                    } else {
                        mac->phy->write_phy_reg(0, 4,0x19,(mac->phy->read_phy_reg(0, 4,0x19) | (0x3c0)));
                    }
                    goto done;
                }
                if (pledctrl->speed[4] == ATHR_PHY_SPEED_10T) {
                    bRateTab = BlinkRateTable_10M;
                }
                while (bRateTab++) {
                    if (athr_gmac_get_diff(pkt_count,cnt) <= bRateTab->rate) {
                        mac->phy->write_phy_reg(0, 4,0x18,((bRateTab->timeOn << 12)|
                            (bRateTab->timeOff << 8)));
                        if(is_ar933x()) {
                            mac->phy->write_phy_reg(0, 4,0x19,0x140);
                        } else {
                            mac->phy->write_phy_reg(0, 4,0x19,(mac->phy->read_phy_reg(0, 4,0x19) & ~(0x280)));
                        }
                        break;
                    }
                }
                pkt_count = cnt;
            } else {
    	    	mac->phy->write_phy_reg(0, 4,0x19,0x0);
            }
        }
    }
done:
    atomic_dec(&Ledstatus);
}

static int ar7240_gmac_create_thread(void *p)
{
    athr_gmac_t *mac = (athr_gmac_t *)p;
    /* Create keventd */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
    INIT_WORK(&mac->led_event, led_event, mac);
#else
    INIT_WORK(&mac->led_event, led_event);
#endif

    return 0;
}

static int 
led_control_func(void *arg) 
{
    athr_gmac_t *mac = (athr_gmac_t *)arg;
    if (!schedule_work(&mac->led_event))
    {
        //Fails is Normal
    }
    return 0;
}
#endif

int ar7240_gmac_attach(void *arg )
{
    athr_gmac_t *mac = (athr_gmac_t *)arg;
    athr_gmac_ops_t *ops = mac->ops;

    if (!ops) {
       ops = (athr_gmac_ops_t *)kmalloc(sizeof(athr_gmac_ops_t), GFP_KERNEL);
    }
    
    memset(ops,0,sizeof(athr_gmac_ops_t));

#ifdef CONFIG_CHECK_DMA_STATUS
    ops->check_dma_st = check_for_dma_status;
#endif

#ifdef CONFIG_ETH_SOFT_LED
    ar7240_gmac_create_thread(mac);
    ops->soft_led = led_control_func;
#endif

#ifdef CONFIG_SET_PLL
    ops->set_pll = ar7240_set_pll;
#endif

    ops->set_caps = ar7240_set_gmac_caps;
    mac->ops = ops;

    return 0;
}
