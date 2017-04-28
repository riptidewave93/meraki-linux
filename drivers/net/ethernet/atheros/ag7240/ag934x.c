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

#include "atheros.h"
#include "athrs_mac.h"

#define MODULE_NAME "AG934X_MAC"

#define ATHR_GMAC_PORT_LED(p)                  (18 + p)

extern void athr_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac);
#if defined(CONFIG_ETH_SOFT_LED) || defined(CONFIG_ATHRS27_PHY)
static int ag934x_gmac_create_thread(void *p);
static int led_control_func(void *arg);
#endif

extern int sw_only;

static int
ar934x_set_gmac_caps(void *arg)
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

#ifdef CONFIG_ATHRS_QOS
#if !HYBRID_APH126_128_S17_WAR
    if (mac->mac_unit == 0)
        mac_set_flag(mac,WAN_QOS_SOFT_CLASS);
#endif
    if ((is_s27() || is_s26()) && mac->mac_unit == 1)
         mac_set_flag(mac,ATHR_SWITCH_QOS);

    if (is_s16())
         mac_set_flag(mac,ATHR_SWITCH_QOS);
#endif

#ifdef CONFIG_ATHR_VLAN_IGMP
         mac_set_flag(mac,ATHR_VLAN_IGMP);
#endif

    if ( soc_is_ar934x()) {
#ifdef CONFIG_ETH_SOFT_LED
        mac_set_flag(mac, ETH_SOFT_LED);
#else
#ifdef CONFIG_ATHR_PHY_SWAP
        /* Activity LED does not work when PHY swap feature is enabled before 
         * Wasp 1.3 and needs soft led support */
        if (is_s27() && !is_ar934x_13_or_later()) {
            mac_set_flag(mac, ETH_SOFT_LED);
        }
#endif /* CONFIG_ATHR_PHY_SWAP */
#endif /* CONFIG_ETH_SOFT_LED */

#if defined(CONFIG_ETH_SOFT_LED) || defined(CONFIG_ATHRS27_PHY)
        if (mac_has_flag(mac, ETH_SOFT_LED)) {
            athr_reg_wr(ATHR_GPIO_OUT_FUNCTION4,
                (athr_reg_rd(ATHR_GPIO_OUT_FUNCTION4) & 0xffff));
            athr_reg_wr(ATHR_GPIO_OUT_FUNCTION5,
                (athr_reg_rd(ATHR_GPIO_OUT_FUNCTION5) & 0xff000000));
            athr_reg_wr(ATHR_GPIO_OUT_FUNCTION4,
                (athr_reg_rd(ATHR_GPIO_OUT_FUNCTION4) & 0xffff));
            athr_reg_wr(ATHR_GPIO_OUT_FUNCTION5,
                (athr_reg_rd(ATHR_GPIO_OUT_FUNCTION5) & 0xff000000));
            ag934x_gmac_create_thread(mac);
            mac->ops->soft_led = led_control_func;
        } else {
            mac->ops->soft_led  = NULL;
            if (is_s27()) {
                    athr_reg_wr(ATHR_GPIO_OUT_FUNCTION4,
                        ((athr_reg_rd(ATHR_GPIO_OUT_FUNCTION4) & 0xffff) | (0x292d << 16)));
                    athr_reg_wr(ATHR_GPIO_OUT_FUNCTION5,
                        ((athr_reg_rd(ATHR_GPIO_OUT_FUNCTION5) & (0xff << 24)) | 0x2c2b2a));
            }
    }
#endif
    }

    if (is_s17())
        mac_set_flag(mac,CHECK_DMA_STATUS);

#ifdef CONFIG_GMAC0_RXFCTL
    if (mac->mac_unit == 0)
        mac_set_flag(mac,ATHR_RX_FLCTL);
#endif

#ifdef CONFIG_GMAC0_TXFCTL
    if (mac->mac_unit == 0)
        mac_set_flag(mac,ATHR_TX_FLCTL);
#endif

#ifdef CONFIG_GMAC1_RXFCTL
    if (mac->mac_unit == 1)
        mac_set_flag(mac,ATHR_RX_FLCTL);
#endif

#ifdef CONFIG_ATHR_SWITCH_ONLY_MODE
	if(sw_only) {
            mac_set_flag(mac,ETH_SWONLY_MODE);
	}
#endif

#ifdef CONFIG_ATHR_SUPPORT_DUAL_PHY
	mac_set_flag(mac,ATHR_DUAL_PHY);
#endif

#ifdef CONFIG_GMAC1_TXFCTL
    if (mac->mac_unit == 1)
        mac_set_flag(mac,ATHR_TX_FLCTL);
#endif

#ifdef CONFIG_ATHR_RX_TASK
    mac_set_flag(mac,ATHR_RX_TASK);
#else
    mac_set_flag(mac,ATHR_RX_POLL);
#endif
    mac_set_flag(mac,ETH_LINK_INTERRUPT);

#ifdef ATHR_PORT1_LED_GPIO
    athr_reg_wr(ATHR_GPIO_OE,
          (athr_reg_rd(ATHR_GPIO_OE) & (~(0x1<<ATHR_PORT1_LED_GPIO))));
    athr_reg_wr(ATHR_GPIO_OUT_FUNCTION0+((ATHR_PORT1_LED_GPIO/4) << 2), 
            ((athr_reg_rd(ATHR_GPIO_OUT_FUNCTION0+((ATHR_PORT1_LED_GPIO/4) << 2)) 
            & (~(0xff<<((ATHR_PORT1_LED_GPIO%4) << 3))) | (0x29 << ((ATHR_PORT1_LED_GPIO%4) << 3)))));
#endif

    if (mac->mac_unit == 1) {
       mac_set_flag(mac,ETH_FORCE_SPEED);
       mac->forced_speed = ATHR_PHY_SPEED_1000T;
    }
    if (mac->mac_unit == 0 && (is_f1e() ||  is_emu()
        || is_s16() || is_vir_phy() || is_f2e()) )  {
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

                athr_gmac_fast_reset(mac,ds,ac);

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
#if !HYBRID_APH126_128_S17_WAR
                printk(MODULE_NAME ": Rx Dma status eth%d : %X\n",mac->mac_unit,RxFsm);
                athr_gmac_fast_reset(mac,NULL,ac);
#endif
            }
            mac->rx_dma_check = 0;
        }
        else {
            mac->rx_dma_check = 1;
        }
    }

    return 0;
}

#if defined(CONFIG_ETH_SOFT_LED) || defined(CONFIG_ATHRS27_PHY)

inline const LED_BLINK_RATES *
athr_gmac_get_blink_table(athr_phy_speed_t s)
{
    if (s == ATHR_PHY_SPEED_10T) {
        return BlinkRateTable_10M;
    }
    return BlinkRateTable_100M;
}

#define ath_gpio_out_val(gpio, val) gpio_set_value(gpio, val)


#define athr_gmac_led_on_off(l, p, g, s)  do { \
    l->led[p] = s;                             \
    l->intvl[p] = 0;                           \
    l->hold[p] = 0;                            \
    l->rate[p] = NULL;                         \
    ath_gpio_out_val(g, !s);                   \
} while (0)

#define ATHR_GMAC_LED_OFF                      0
#define ATHR_GMAC_LED_ON                       1

#define athr_led_get_next_state(s)             \
    (ATHR_GMAC_LED_ON + ATHR_GMAC_LED_OFF - s)


void inline
athr_gmac_off_led(ath_led_ctrl_t *lc, int port)
{
    athr_gmac_led_on_off(lc, port,
        ATHR_GMAC_PORT_LED(port), ATHR_GMAC_LED_OFF);
}

void inline
athr_gmac_on_led(ath_led_ctrl_t *lc, int port)
{
    athr_gmac_led_on_off(lc, port,
        ATHR_GMAC_PORT_LED(port), ATHR_GMAC_LED_ON);
}

void
athr_gmac_blink_led(ath_led_ctrl_t *lc, uint32_t rate, int port)
{
    const LED_BLINK_RATES *bl = athr_gmac_get_blink_table(lc->speed[port]);
#define blink_debug       0
#if blink_debug
    const LED_BLINK_RATES *tmp;

    tmp = bl;
#endif

    if (rate == 0 && lc->rate[port]) {
        rate = lc->rate[port]->rate;
    }
    for (;; bl ++) {
        if (rate <= bl->rate) {
            uint8_t max;

            if (bl != lc->rate[port]) {
                lc->intvl[port] = 0;
                lc->led[port] = ATHR_GMAC_LED_ON;
                lc->rate[port] = bl;
                lc->hold[port] = bl->timeOn + bl->timeOff;
            }

            max = (lc->led[port] ? bl->timeOn : bl->timeOff);

            lc->intvl[port] ++;
            lc->hold[port] --;

            if (lc->intvl[port] >= max) {
                // toggle led state
                lc->led[port] = athr_led_get_next_state(lc->led[port]);
                lc->intvl[port] = 0;
                ath_gpio_out_val(ATHR_GMAC_PORT_LED(port), lc->led[port]);
            }

#if blink_debug
            printk("[%d] rate %u led %u intvl %u max %u bl %u (%d) hold %d\n",
                    port, rate, lc->led[port], lc->intvl[port], max,
                    bl->rate, bl - tmp, lc->hold[port]);
#endif
            break;
        }
    }
}

uint32_t
athr_gmac_get_count(athr_gmac_t *mac, ath_led_ctrl_t *lc, uint32_t port)
{
    if (port) { // lan
        uint32_t reg = 0x2003c + (((port % 4) + 1) << 8);

        /*
	 * MIB counter will be clear to zero after read it, therefore don't need
	 * to set Flush all LAN MIB counters configuration again.  Otherwise it
	 * may miss processiong some LAN MIB counters.
         */
	return athrs27_reg_read(reg) +       // rx good byte count
               athrs27_reg_read(reg + 0x48); // tx
    } else {
        uint32_t               diff, cnt;

        cnt = athr_gmac_reg_rd(mac, ATHR_GMAC_RX_BYTES_CNTR) +
              athr_gmac_reg_rd(mac, ATHR_GMAC_TX_BYTES_CNTR);

        diff = athr_gmac_get_diff(lc->pkt_count[port], cnt);
        lc->pkt_count[port] = cnt;
        return diff;
    }
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
void led_event(struct work_struct *work)
#else
void led_event(void *data)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
    athr_gmac_t *mac = container_of(work, athr_gmac_t, led_event);
#else
    athr_gmac_t *mac = (athr_gmac_t *) data;
#endif
    extern int             phy_in_reset;
    extern atomic_t        Ledstatus;

    uint32_t               i = 0, cnt, ep;
    ath_led_ctrl_t         *pledctrl = &mac->ledctrl;

    atomic_inc(&Ledstatus);

    /*
     *  MDIO access will fail While PHY is in RESET phase.
     */
    if (phy_in_reset) {
        goto done;
    }

    if (mac->mac_unit == 1) {
        i = 1; ep = 5;
    } else {
        i = 0; ep = 1;
    }

    for (; i < ep ; i ++) {
        cnt = 0;
        if (pledctrl->link[i] && mac->mac_ifup) {
            if (pledctrl->hold[i] > 0) {
                athr_gmac_blink_led(pledctrl, cnt, i);
            } else {
                cnt = athr_gmac_get_count(mac, pledctrl, i);
		if (cnt && pledctrl->rate[i]) {
                    /* scale the count to 100 msecs */
                    cnt = cnt / (pledctrl->rate[i]->timeOff +
			          pledctrl->rate[i]->timeOn);
		}
                if (cnt) {
                    athr_gmac_blink_led(pledctrl, cnt, i);
                } else {
                    athr_gmac_on_led(pledctrl, i);
                }
            }
        } else {
            athr_gmac_off_led(pledctrl, i);
        }
    }

done:
    atomic_dec(&Ledstatus);
}

static int ag934x_gmac_create_thread(void *p)
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

int ar934x_gmac_attach(void *arg )
{
#ifdef KPHACK
    int i;
#endif

    athr_gmac_t     *mac = (athr_gmac_t *)arg;
    athr_gmac_ops_t *ops = mac->ops;

    if (!ops) {
       ops = (athr_gmac_ops_t *)kmalloc(sizeof(athr_gmac_ops_t), GFP_KERNEL);
    }

    memset(ops,0,sizeof(athr_gmac_ops_t));

    ops->check_dma_st = check_for_dma_status;

#ifdef KPHACK
    for (i = 0; i < 5; i++) {
        ath_gpio_config_output(ATHR_GMAC_PORT_LED(i));
    }
#endif

    ops->set_pll   = NULL;

    ops->set_caps = ar934x_set_gmac_caps;
    ops->poll_link = athr_gmac_poll_link;

    mac->ops = ops;

    mac->intr_mask = ATHR_GMAC_INTR_RX  |
                     ATHR_GMAC_INTR_RX_BUS_ERROR |  \
                     ATHR_GMAC_INTR_TX_BUS_ERROR    \
		     ; /*| ATHR_GMAC_INTR_TX_URN | ATHR_GMAC_INTR_TX */

    mac->mac_addr = (char *) (mac->mac_unit ? ATH_EEPROM_GE1_MAC_ADDR :
      ATH_EEPROM_GE0_MAC_ADDR);

    mac->ddr_flush = mac->mac_unit ? AR934X_DDR_REG_FLUSH_GE1 :
      AR934X_DDR_REG_FLUSH_GE0;

    return 0;
}
