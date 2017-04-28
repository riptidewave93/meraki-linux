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
#include <ath79.h>
#include <linux/proc_fs.h>

#include "osdep.h"

#include "athrs_mac.h"
#include "athrs_trc.h"
#include "athrs_mac_def.h"
#include "athrs_flowmac.h"

#if ETHDEBUG_ENABLED
#include "ethdebug.h"
#endif
#if ETHDEBUG_ENABLED
struct eth_dbg_params eth_vi_dbg_params;
u_int32_t eth_rx_drop = 0; 
void ethdbg_check_rxdrop(struct sk_buff *skb);
#endif

#define MODULE_NAME "ATHR_GMAC"
MODULE_LICENSE("Dual BSD/GPL");

static int tx_len_per_ds = 0;
static int tx_max_desc_per_ds_pkt=0;

module_param(tx_len_per_ds, int, 0);
MODULE_PARM_DESC(tx_len_per_ds, "Size of DMA chunk");

int sw_only = 0;
module_param(sw_only, int, 0);
MODULE_PARM_DESC(sw_only, "sw only mode setting");

int fifo_3 = 0x1f00140;
module_param(fifo_3, int, 0);
MODULE_PARM_DESC(fifo_3, "fifo cfg 3 settings");

int mii0_if = ATHR_GMAC_MII0_INTERFACE;
module_param(mii0_if, int, 0);
MODULE_PARM_DESC(mii0_if, "mii0 connect");

int mii1_if = ATHR_GMAC_MII1_INTERFACE;
module_param(mii1_if, int, 0);
MODULE_PARM_DESC(mii1_if, "mii1 connect");

int gige_pll = 0x1a000000;
module_param(gige_pll, int, 0);
MODULE_PARM_DESC(gige_pll, "Pll for (R)GMII if");

int fifo_5 = 0xbefff;
module_param(fifo_5, int, 0);
MODULE_PARM_DESC(fifo_5, "fifo cfg 5 settings");

int flowmac_on = 0x0;
module_param(flowmac_on, int, 0x0);
MODULE_PARM_DESC(flowmac_on, "lan wlan flowcontrol on");

static char *mii_str[2][4] = {
    {"GMii", "Mii", "RGMii", "RMii"},
    {"GMii","Mii","RGMii", "RMii"}
};

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

#ifdef CONFIG_PROC_FS

static int athr_gmac_phy_show(struct seq_file *m, void *v);
static int athr_gmac_txdesc_show(struct seq_file *m, void *v);

static int athr_gmac_phy_open(struct inode *inode, struct file *file)
{
    return single_open(file, athr_gmac_phy_show, NULL);
}

static int athr_gmac_txdesc_open(struct inode *inode, struct file *file)
{
    return single_open(file, athr_gmac_txdesc_show, NULL);
}

static struct file_operations athr_gmac_phy_operations = {
    .open       = athr_gmac_phy_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static struct file_operations athr_gmac_txdesc_dump_operations = {
    .open       = athr_gmac_txdesc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static struct proc_dir_entry *ag7240_dir;
#endif /* CONFIG_PROC_FS */

int ignore_packet_inspection = 0;
int frame_sz = 0;
struct sk_buff *bogus_skb = NULL;

int phy_in_reset = 0;

char *spd_str[] = {"10Mbps", "100Mbps", "1000Mbps"};
char *dup_str[] = {"half duplex", "full duplex"};

athr_gmac_t *athr_gmacs[2];

static void athr_gmac_hw_setup(athr_gmac_t *mac);

static void athr_gmac_buffer_free(struct sk_buff *skb);

static void athr_gmac_mii_setup(athr_gmac_t *mac);

void set_packet_inspection_flag(int flag)
{
    if(flag==0)
        ignore_packet_inspection = 0;
    else
        ignore_packet_inspection = 1;
}

void hdr_dump(char *str,int unit,struct ethhdr *ehdr,int qos,int ac)
{
    printk("\n[%s:%x][%x:%x][MAC1:%x,%x,%x,%x,%x,%x"
           "MAC2: %x,%x,%x,%x,%x,%x Prot:%x]\n",str,unit,qos,ac,
           ehdr->h_dest[0],ehdr->h_dest[1],ehdr->h_dest[2],ehdr->h_dest[3],
           ehdr->h_dest[4],ehdr->h_dest[5],ehdr->h_source[0],ehdr->h_source[1],
           ehdr->h_source[2],ehdr->h_source[3],ehdr->h_source[4],
           ehdr->h_source[5],ehdr->h_proto);

}

void pkt_dump(uint8_t *data,int len,const char *dir,int unit)
{
  int i;
  for (i=0;i<=len;i++)
  {
      if (i == 0)
	  printk("%s mac:%d: len:%d ->\n",dir,unit,len);

      printk("%2X:",data[i]);

      if (!(i%20))
        printk("\n");
  }
}

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
        /* In WASP all 5 ports are connected to LAN, so disable all the ports */
        phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_LAN); /* LAN ports */
        phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_WAN); /* WAN port */       
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

    mask = athr_gmac_reset_mask(mac->mac_unit);

    if (!soc_is_ar7240())
        mask |=  ATHR_RESET_GE0_MDIO | ATHR_RESET_GE1_MDIO;

   /*
    * put into reset, hold, pull out.
    */
    spin_lock_irqsave(&mac->mac_lock, flags);
    ath79_device_reset_set(mask);
    /*
     * Increase the delay for MDIO and MAC reset to settle down.
     */
    mdelay(100);
    ath79_device_reset_clear(mask);
    mdelay(200);
    spin_unlock_irqrestore(&mac->mac_lock, flags);

    /*
     * When the mac is taken out of reset it 
     * will put gmac modules in reset. 
     * clr the soft reset in mac to bring out
     * of reset.
     */
    
    if (soc_is_ar934x())
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);

	 if (soc_is_qca955x())
	     mac->ops->soc_gmac_mii_setup(mac);
	 else if(!soc_is_ar71xx() && !soc_is_ar934x())
	     athr_gmac_mii_setup(mac);

    phy->init(mac);

    athr_init_hwaccels(mac);

    athr_gmac_hw_setup(mac);

    mac->mac_speed = ATHR_PHY_SPEED_UNKNOWN;



    printk("Setting PHY...\n");

    phy_in_reset = 1;
    phy->setup(mac);
    phy_in_reset = 0;


    /*
    * set the mac addr
    */
    addr_to_words(dev->dev_addr, w1, w2);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR1, w1);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR2, w2);
    athr_gmac_reg_wr(mac, 0x2c,0x1313);

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

    if (soc_is_ar7240() || soc_is_ar934x())
        athr_gmac_intr_enable_rxovf(mac);

    /*
     * Enable link interrupts for PHYs
     */

    if (mac_has_flag(mac,ETH_LINK_INTERRUPT)) {
      if((mac->mac_unit == 1) && (mac->mac_ifup == 0)) {
         if(!soc_is_ar934x() || !is_s16()) {
            st = request_irq(ATHR_IRQ_ENET_LINK, athr_gmac_link_intr,
               ATHR_MAC_IRQF_DISABLED, "gmac link intr", mac);
         }
         if( st < 0) {
            if(!soc_is_ar934x() || !is_s16()) {
               printk(MODULE_NAME ": request irq %d failed %d\n",
                  ATHR_IRQ_ENET_LINK, st);
               free_irq(ATHR_IRQ_ENET_LINK, mac);
            }
         }
      }
    }

#ifdef KPHACK
    if (mac_has_flag(mac,ETH_LINK_INTERRUPT)) {
	if((mac->mac_unit == 1) && (mac->mac_ifup == 0)) {
		if(soc_is_ar934x() && is_s16()) {
			st = request_irq(ATHR_GPIO_IRQn(4), athr_gmac_link_intr, 
				ATHR_MAC_IRQF_DISABLED, "gmac s16 link intr", mac);
		}else {
			st = request_irq(ATHR_IRQ_ENET_LINK, athr_gmac_link_intr, 
				ATHR_MAC_IRQF_DISABLED, "gmac link intr", mac);	
		}
		if( st < 0) {
			if(soc_is_ar934x() && is_s16()) {
				printk(MODULE_NAME ": request irq %d failed %d\n",
					ATHR_GPIO_IRQn(4), st);
				free_irq(ATHR_IRQ_ENET_LINK, mac);	
			} else {
				printk(MODULE_NAME ": request irq %d failed %d\n",
					ATHR_IRQ_ENET_LINK, st);
				free_irq(ATHR_IRQ_ENET_LINK, mac);
			}
		}
	}	
    }
#endif
    mac->mac_ifup = 1;

    /*
     *  Enable Link poll, Soft led, dma_check timers
     */
    athr_gmac_timer_init(mac);

    if (mac_has_flag(mac,ETH_LINK_INTERRUPT))
        mac->phy->en_link_intrs(mac);

    athr_gmac_rx_start(mac);

    if (is_8021())
        athr_gmac_check_link(mac, 0);

/*
 * Sometimes WAN link interrupts are lost when the LAN init does a switch reset
 * force a link status check just to make sure we dont loose the link status.
 */
    if (mac_has_flag(mac,ETH_LINK_INTERRUPT)) {
        if (mac->mac_unit == 0)
            athr_gmac_force_check_link();
    }
    /*
     * reenable the switch ports
     */
    if(phy->reg_config)
    {
        /* In WASP all 5 ports are connected to LAN, so enable all the ports */
        phy->reg_config(ATHR_GMAC_PORT_ON, ENET_UNIT_LAN); /* LAN ports */
        phy->reg_config(ATHR_GMAC_PORT_ON, ENET_UNIT_WAN); /* WAN port */
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

    spin_unlock_irqrestore(&mac->mac_lock, flags);

    athr_napi_disable(mac);

    free_irq(mac->mac_irq, dev);

    if (mac_has_flag(mac,ETH_LINK_INTERRUPT)) {
	if((mac->mac_unit == 1) && (mac->mac_ifup == 0)) {
		free_irq(ATHR_IRQ_ENET_LINK, mac);
	}
    }

    return 0;
}

static int mdc_check(athr_gmac_t *mac)
{
    int i;

    for (i=0; i<4000; i++) {
        if(mac->phy->read_mac_reg(0x10c) != 0x18007fff)
            return -1;
    }

    return 0;
}

static void
athr_gmac_mii_setup(athr_gmac_t *mac)
{

    uint8_t mgmt_cfg_val = 0, check_cnt = 0;

    /*
     * set the mii if type - NB reg not in the gigE space
     */
    if (soc_is_ar934x() && is_f2e()) {
        printk("WASP ----> F2e PHY\n");

        mgmt_cfg_val = 0xb;
        /*
         *  External RMII Mode on GE0,Master and High speed
        */
        athr_reg_wr(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RMII_GE0
                   | ATHR_GMAC_ETH_CFG_RMII_HISPD_GE0 |ATHR_GMAC_ETH_CFG_RMII_MASTER_MODE);

        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);

        return;
    }

    if (soc_is_ar934x() && (is_f1e() || is_vir_phy())) {
        if (is_vir_phy())
            printk("WASP ----> VIR PHY\n");
        else
            printk("WASP ----> F1e PHY\n");

        mgmt_cfg_val = 6;
        if(mac->mac_unit == 0)
            athr_reg_wr(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RGMII_GE0);  // External RGMII Mode on GE0

        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);

        return;
    }
    if (soc_is_ar934x() && is_s16()) {
        if (is_s17()) {
            mgmt_cfg_val = 0xb;
            printk("WASP  ----> S17 PHY *\n");
        }
        else {
            mgmt_cfg_val = 0x7;
            printk("WASP  ----> S16 PHY *\n");
        }
        if(mac->mac_unit == 0)
            athr_reg_wr(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RGMII_GE0);

        /* Tweaking needed for SST parts */
        athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, 0x3c000);

        athr_reg_rmw_clear(ATHR_SWITCH_CLK_SPARE, (1 << 6));
        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
        athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);

        return;
    }
    if (soc_is_ar934x() && is_s27()) {
        if (!is_emu()) {
            printk("WASP ----> S27 PHY MDIO\n");
            mgmt_cfg_val = 7;
            athr_swap_phy();
            athr_reg_wr(ATHR_SWITCH_CLK_SPARE,(athr_reg_rd(ATHR_SWITCH_CLK_SPARE)|0x40));
            athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
            athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);

            if (mac_has_flag(mac,ETH_SWONLY_MODE)) {
                athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_ONLY_MODE);
                athr_gmac_reg_rmw_set(athr_gmacs[0], ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);;
            }
            //athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_APB_ACCESS);
        }
        else {
           printk("WASP EMULATION ----> S27 PHY\n");
           mgmt_cfg_val = 7;
           athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
           athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
        }
        return;

    }

    if (soc_is_ar7240()) {
        mgmt_cfg_val = 0x2;
        if (mac->mac_unit == 0) {
            athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
            athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
        }
    }
    else {
        switch (ath79_ahb_freq/1000000) {
            case 150:
                mgmt_cfg_val = 0x7;
                break;
            case 175:
                mgmt_cfg_val = 0x5;
                break;
            case 200:
                mgmt_cfg_val = 0x4;
                break;
            case 210:
                mgmt_cfg_val = 0x9;
                break;
            case 220:
                mgmt_cfg_val = 0x9;
                break;
            case 40:
            case 24:
                /* hornet emulation...ahb is either 24 or 40Mhz */
                mgmt_cfg_val = 0x6;
                break;
            default:
                mgmt_cfg_val = 0x7;
        }
        if ((soc_is_ar7241() || soc_is_ar7242())) {

            if (is_8021()) {
               athr_gmac_reg_rmw_set(mac, ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RGMII_GE0);
               athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
               return ;
            }
            /* External MII mode */
            if (mac->mac_unit == 0 && soc_is_ar7242()) {
                mgmt_cfg_val = 0x6;
                athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RGMII_GE0);
                athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
                athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
            }
            /* Virian */
            mgmt_cfg_val = 0x4;

            if (mac_has_flag(mac,ETH_SWONLY_MODE)) {
                athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_ONLY_MODE);
                athr_gmac_reg_rmw_set(athr_gmacs[0], ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);;
            }

            if(athr_gmacs[1] != NULL) {
                athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
                athr_gmac_reg_wr(athr_gmacs[1], ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
            }
            printk("Virian MDC CFG Value ==> %x\n",mgmt_cfg_val);
        }
        else  if(soc_is_ar933x()){
            athr_reg_wr(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_MII_GE0_SLAVE);
            mgmt_cfg_val = 0xF;
            if (mac->mac_unit == 1) {
                while (check_cnt++ < 10) {
                    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
                    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
                    if(mdc_check(mac) == 0)
                        break;
                }
                if(check_cnt == 11)
                    printk("%s: MDC check failed\n", __func__);
            }
        }
        else { /* Python 1.0 & 1.1 */
            if (mac->mac_unit == 0) {
                while (check_cnt++ < 10) {
                    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val | (1 << 31));
                    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CFG, mgmt_cfg_val);
                    if(mdc_check(mac) == 0)
                        break;
                }
                if(check_cnt == 11)
                    printk("%s: MDC check failed\n", __func__);
            }
        }
    }
}

static void
athr_gmac_hw_setup(athr_gmac_t *mac)
{
    athr_gmac_ring_t *tx, *rx = &mac->mac_rxring;
    athr_gmac_desc_t *r0, *t0;
    uint8_t ac;

    /* clear the rx fifo state if any */
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_STATUS,
                        athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS));



    if (!soc_is_ar934x())
        athr_gmac_reg_wr(mac, ATHR_GMAC_CFG1, (ATHR_GMAC_CFG1_RX_EN | ATHR_GMAC_CFG1_TX_EN));

    if ((mac->mac_unit == 1))
       athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2, (ATHR_GMAC_CFG2_PAD_CRC_EN |
               ATHR_GMAC_CFG2_LEN_CHECK | ATHR_GMAC_CFG2_IF_1000));

    if (mac->mac_unit == 0)
       athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2, (ATHR_GMAC_CFG2_PAD_CRC_EN |
               ATHR_GMAC_CFG2_LEN_CHECK | ATHR_GMAC_CFG2_IF_10_100));

    athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_0, 0x1f00);

    if (mac_has_flag(mac,ATHR_RX_FLCTL))
       athr_gmac_reg_rmw_set(mac,ATHR_GMAC_CFG1,ATHR_GMAC_CFG1_RX_FCTL);

    if (mac_has_flag(mac,ATHR_TX_FLCTL)) {
        /* 
         * If it is GMAC0, set flow control based on half or full duplex.
         * If it is GMAC1, no changes.
         */
        if (mac->mac_unit == 0 && ((is_s27() || is_s26()) || is_f1e())) {
            if (mac->mac_fdx == 0) {
                /* half duplex.*/
                athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_TX_FCTL);
            }
            else {
                /* full duplex */
                athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_TX_FCTL);
            }
        }
        else {
           athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_TX_FCTL);
        }
    }

    /* Disable ATHR_GMAC_CFG2_LEN_CHECK to fix the bug that
     * the mac address is mistaken as length when enabling Atheros header
     */
    if (mac_has_flag(mac,ATHR_HEADER_ENABLED))
       athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG2, ATHR_GMAC_CFG2_LEN_CHECK)


    athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_1, 0x10ffff);

    if (soc_is_ar934x() && is_s27()) {
        athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_2, 0x03ff0155);
    } else {
        athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_2, 0x015500aa);
    }
    /*
     * Weed out junk frames (CRC errored, short collision'ed frames etc.)
     */
    athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_4, 0x3ffff);

    /*
     * Drop CRC Errors, Pause Frames ,Length Error frames, Truncated Frames
     * dribble nibble and rxdv error frames.
     */
    printk("Setting Drop CRC Errors, Pause Frames and Length Error frames \n");

    if (is_8021()) {
       athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_5, 0xbefff);
    }
    else if (mac_has_flag(mac,ATHR_HEADER_ENABLED)) {
       athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_5, 0xe6be2);
    }
    else if ((mac->mac_unit == 0 || mac->mac_unit == 1) &&
                 (soc_is_ar7242() || is_s16() || mac_has_flag(mac,ATHR_JUMBO_FR))) {
       athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_5, 0xe6be2);
    }
    else if (is_f2e()) {
       athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_5, 0xbefff);
    }
    else {
       /*
        * Need to set the Out-of-range bit to receive the packets with 1518 bytes, when
        * VLAN is enabled.
        */
       athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_5, 0x66bc2);
    }

    if (mac_has_flag(mac,ATHR_JUMBO_FR)) {
       athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2,
                      ATHR_GMAC_CFG2_HUGE_FRAME_EN);
       athr_gmac_reg_wr(mac, ATHR_GMAC_MAX_PKTLEN,frame_sz);
    }

    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {
        athr_gmac_reg_wr(mac,ATHR_GMAC_DMA_TX_ARB_CFG,ATHR_GMAC_TX_QOS_MODE_WEIGHTED
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
            }
        }
    }
    else {
        tx = &mac->mac_txring[0];
        t0  =  &tx->ring_desc[0];
        athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q0, athr_gmac_desc_dma_addr(tx, t0));
    }

    r0  =  &rx->ring_desc[0];
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_DESC, athr_gmac_desc_dma_addr(rx, r0));

    DPRINTF(MODULE_NAME ": cfg1 %#x cfg2 %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_CFG1),
        athr_gmac_reg_rd(mac, ATHR_GMAC_CFG2));
}

static void
athr_gmac_hw_stop(athr_gmac_t *mac)
{
    athr_phy_ops_t *phy = mac->phy;
    int i;


    athr_gmac_rx_stop(mac);
    athr_gmac_tx_stop(mac);
    athr_gmac_int_disable(mac);

    if (mac_has_flag(mac,ETH_LINK_INTERRUPT))
        phy->dis_link_intrs(mac);

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

   if(!is_ar9330_emu()    &&
      (mac->mac_unit == 1 || soc_is_ar7241() ||
       soc_is_ar7242()        || is_emu()    || soc_is_ar934x()) )
   {
           athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);
   }

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
void
athr_gmac_set_mac_from_link(athr_gmac_t *mac, athr_phy_speed_t speed, int fdx)
{

    athr_gmac_ops_t *ops = mac->ops;

    printk(MODULE_NAME ": unit %d ETH_FORCE_SPEED 0x%x speed %d\n", mac->mac_unit,
		mac_has_flag(mac,ETH_FORCE_SPEED), speed);

    if (mac_has_flag(mac,ETH_FORCE_SPEED)) {
        speed = mac->forced_speed;
        fdx   = 1;
    }

    mac->mac_speed =  speed;
    mac->mac_fdx   =  fdx;
    athr_gmac_set_mac_duplex(mac, fdx);
    athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_3, fifo_3);
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_FIFO_THRESH ,0x01d80160);

    switch (speed)
    {
    case ATHR_PHY_SPEED_1000T:
	printk(MODULE_NAME ": SPEED 1000T\n");
        athr_gmac_set_mac_if(mac, 1);
        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_FIFO_CFG_5, (1 << 19));

        if (ops->set_pll)
	   ops->set_pll(mac,mac->mac_speed);

        if (soc_is_ar7242() &&  mac->mac_unit == 0)
            ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x16000000);

        if (soc_is_ar934x() && mac->mac_unit == 0 && is_s16())
            ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x06000000);

        if (soc_is_ar934x() && mac->mac_unit == 0 && is_f1e() )
            ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x0e000000);

        if (soc_is_ar934x() && mac->mac_unit == 0 && is_vir_phy() ){
#ifdef CONFIG_VIR_XMII_CFG
            ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,CONFIG_VIR_XMII_CFG);
#else
            ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x82000000);
#endif

            athr_reg_wr(ATHR_GMAC_ETH_CFG,0x000c0001);
	}

        if (soc_is_ar934x() && mac->mac_unit == 0 && is_f1e() ) {
            athr_reg_rmw_set(ATHR_GMAC_ETH_CFG,ATHR_GMAC_ETH_CFG_RXD_DELAY);
            athr_reg_rmw_set(ATHR_GMAC_ETH_CFG,ATHR_GMAC_ETH_CFG_RDV_DELAY);
        }

        break;

    case ATHR_PHY_SPEED_100T:
	printk(MODULE_NAME ": SPEED 100T, is_emu() %d\n",is_emu());
        athr_gmac_set_mac_if(mac, 0);
        athr_gmac_set_mac_speed(mac, 1);
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_FIFO_CFG_5, (1 << 19));

        if (mac->mac_unit == 0 && !mac->mac_fdx) {
           athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_FIFO_THRESH ,0x00880060);
           athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_3, 0x00f00040);
        }

#ifdef KPHACK
	if (is_emu() && is_f1e()) {
            athr_reg_wr(0xb804006c,0x2);
            athr_reg_rmw_clear(ATHR_GPIO_OUT_FUNCTION1, (0xff << 16));
            athr_reg_rmw_set(ATHR_GPIO_OE, 0x4);
            athr_reg_rmw_set(ATHR_GPIO_OUT, 0x4);
            udelay(10);
            athr_reg_rmw_clear(ATHR_GPIO_OUT, 0x4);
        }
#endif
        if (ops->set_pll)
	   ops->set_pll(mac,mac->mac_speed);

        if ((soc_is_ar7242() || soc_is_ar934x()) &&
                    mac->mac_unit == 0 && (is_s16() || is_f1e()))
	    ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x0101);

        if (soc_is_ar934x() && mac->mac_unit == 0 && is_f1e()) {
            athr_reg_rmw_clear(ATHR_GMAC_ETH_CFG,ATHR_GMAC_ETH_CFG_RXD_DELAY);
            athr_reg_rmw_clear(ATHR_GMAC_ETH_CFG,ATHR_GMAC_ETH_CFG_RDV_DELAY);
        }
        break;

    case ATHR_PHY_SPEED_10T:
	printk(MODULE_NAME ": SPEED 10T\n");
        athr_gmac_set_mac_if(mac, 0);
        athr_gmac_set_mac_speed(mac, 0);
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_FIFO_CFG_5, (1 << 19));

        if (mac->mac_unit == 0 && !mac->mac_fdx) {
           athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_FIFO_THRESH ,0x00880060);
           athr_gmac_reg_wr(mac, ATHR_GMAC_FIFO_CFG_3, 0x00f00040);
        }

        if (ops->set_pll)
	   ops->set_pll(mac,mac->mac_speed);

     	if ((soc_is_ar7242() || soc_is_ar934x()) &&
                    mac->mac_unit == 0 && is_s16())
	    ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x1616);

        if ((soc_is_ar934x() || soc_is_ar7242()) &&
		mac->mac_unit == 0 && is_f1e()) {
	    ath79_pll_wr(ATH_PLL_ETH_XMII_CONFIG,0x1313);
        }
        if (is_f2e()) {
            athr_reg_rmw_clear(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_RMII_HISPD_GE0);
        }
        break;

    default:
        assert(0);
    }
    DPRINTF(MODULE_NAME ": cfg_1: %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_FIFO_CFG_1));
    DPRINTF(MODULE_NAME ": cfg_2: %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_FIFO_CFG_2));
    DPRINTF(MODULE_NAME ": cfg_3: %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_FIFO_CFG_3));
    DPRINTF(MODULE_NAME ": cfg_4: %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_FIFO_CFG_4));
    DPRINTF(MODULE_NAME ": cfg_5: %#x\n", athr_gmac_reg_rd(mac, ATHR_GMAC_FIFO_CFG_5));
}

void
athr_gmac_fast_reset(athr_gmac_t *mac,athr_gmac_desc_t *ds,int ac)
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

    mask = athr_gmac_reset_mask(mac->mac_unit);

    /*
     * put into reset, hold, pull out.
     */
    if (!soc_is_ar933x()) {
	ath79_device_reset_set(mask);
        udelay(10);
	ath79_device_reset_clear(mask);
        udelay(10);
    }
    if (soc_is_ar934x())
        athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);

    athr_init_hwaccels(mac);

    athr_gmac_hw_setup(mac);

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

    athr_gmac_set_mac_from_link(mac, mac->mac_speed, mac->mac_fdx);

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
athr_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, athr_phy_speed_t *speed,int phyUnit)
{
  athr_phy_ops_t *phy = mac->phy;

  if(phy) {
     *fdx   = phy->is_fdx(mac->mac_unit, phyUnit);
     *speed = phy->speed(mac->mac_unit, phyUnit);
#if HYBRID_LINK_CHANGE_EVENT
     {
     int rc = 0;
     *link = phy->is_up(mac->mac_unit, &rc);
     return rc;
     }
#else
     *link  = phy->is_up(mac->mac_unit);
#endif
  }
  else {
     printk("MAC:%d Warning: Phy not found!!!\n",mac->mac_unit);
  }
  return 0;
}


/*
 * phy link state management
 */
int
athr_gmac_check_link(void *arg,int phyUnit)
{
    athr_phy_speed_t   speed;
    athr_gmac_t        *mac = (athr_gmac_t *) arg;
    struct net_device  *dev = mac->mac_dev;
    athr_phy_ops_t     *phy = mac->phy;
    int                carrier = netif_carrier_ok(dev);
    int                fdx, phy_up;
    int                rc;
    unsigned long      flags;

    spin_lock_irqsave(&mac->mac_lock, flags);

    assert(mac);
    assert(phy);

    rc = athr_gmac_get_link_st(mac, &phy_up, &fdx, &speed, phyUnit);
    
    if (mac->mac_unit == 1 && mac_has_flag(mac,ETH_FORCE_SPEED) && is_s27()) {
       speed = ATHR_PHY_SPEED_1000T;
       fdx =  1;
    }

    if(phy->stab_wr)
        phy->stab_wr(phyUnit,phy_up,speed);

    if (rc < 0)
        goto done;

    if (!phy_up)
    {
        if (carrier)
        {
            printk(MODULE_NAME ":unit %d: phy %0d not up carrier %d\n", mac->mac_unit,
			phyUnit, carrier);
            /* A race condition is hit when the queue is switched on while tx interrupts are enabled.
             * To avoid that disable tx interrupts when phy is down.
             */
            mac->link_up = 0;
            athr_gmac_intr_disable_tx(mac);
            netif_carrier_off(dev);
            netif_stop_queue(dev);
        }
        /*
         * Flush the unicast entries of the port.
         */
        if (soc_is_ar934x() && is_s27())
            phy->write_mac_reg(0x50, ((athr_get_portno(phyUnit) << 8 ) + 0x0d));

        /* 
         * Always turn off LED when there is no phy port up
         */
        if (mac_has_flag(mac,ETH_SOFT_LED)) {

            mac->ledctrl.link[phyUnit] = 0;
            phy->write_phy_reg(mac->mac_unit,phyUnit,0x19,0x0);
            /*
             * Turn off LED
             */
#ifdef KPHACK
            if (soc_is_ar933x()) {
                athr_reg_wr(ATHR_GPIO_FUNCTIONS,
                    (athr_reg_rd(ATHR_GPIO_FUNCTIONS)& (~(1 << (phyUnit + 3)))));
            }
#endif
        }
        if (soc_is_ar934x() && mac->mac_unit == 0)
            athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, (ATHR_GMAC_CFG1_RX_EN | ATHR_GMAC_CFG1_TX_EN));


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


    /*
     * Configure the  pause off and pause
     * on for Global and port threshold
     */
    if (mac->mac_unit == 1 && (soc_is_ar934x() && is_s27())) {

        phy->write_mac_reg(0x38, 0x16162020);

        switch(phy_up) {

           case 1 :
               phy->write_mac_reg(0x34, 0x16602090);
               break;
           case 2 :
               phy->write_mac_reg(0x34, 0x90bcc0ff);
               break;
           case 3 :
               phy->write_mac_reg(0x34, 0x78bca0ff);
               break;
           case 4 :
               phy->write_mac_reg(0x34, 0x60bc80ff);
               break;
           default :
               break;

        }

    }

    if (carrier && (speed == mac->mac_speed) && (fdx == mac->mac_fdx)) 
        goto done;

    if (phy->is_alive(phyUnit) || is_ar9330_emu())
    {
        DPRINTF(MODULE_NAME ": Enet Unit:%d PHY:%d is UP ", mac->mac_unit,phyUnit);
        DPRINTF("%s %s %s\n", mii_str[mac->mac_unit][mii_if(mac)],
           spd_str[speed], dup_str[fdx]);

        athr_gmac_set_mac_from_link(mac, speed, fdx);

        DPRINTF(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl  \n",
           athr_gmac_reg_rd(mac, ATHR_GMAC_CFG2),
           athr_gmac_reg_rd(mac, ATHR_GMAC_IFCTL));

        if (!mac_has_flag(mac,WAN_QOS_SOFT_CLASS))
           athr_gmac_fast_reset(mac,NULL,0);



       /* 
        * only Enable Tx and Rx  afeter configuring the mac speed.
        */

        if (soc_is_ar934x())
            athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, (ATHR_GMAC_CFG1_RX_EN | ATHR_GMAC_CFG1_TX_EN));


        /*
         * in business
         */
        netif_carrier_on(dev);
        netif_start_queue(dev);
        mac->link_up = 1;

        /*
         * WAR: Enable link LED to glow if speed is negotiated as 10 Mbps
         */
        if (mac_has_flag(mac,ETH_SOFT_LED)) {
            mac->ledctrl.link[phyUnit] = 1;
            mac->ledctrl.speed[phyUnit] = speed;

            if (soc_is_ar933x()) {
                /*
                 * Turn on LED
                 */
#ifdef KPHACK
	    	     athr_reg_wr(ATHR_GPIO_FUNCTIONS,
                    (athr_reg_rd(ATHR_GPIO_FUNCTIONS) | (1 << (phyUnit + 3))));
#endif
	        }
            phy->write_phy_reg(mac->mac_unit,phyUnit,0x19,0x3c0);
        }
    }
    else {
        if (mac_has_flag(mac,ETH_SOFT_LED)) {
            mac->ledctrl.link[phyUnit] = 0;
            phy->write_phy_reg(mac->mac_unit,phyUnit,0x19,0x0);

            if (soc_is_ar933x()) {
                phy->write_phy_reg(mac->mac_unit,phyUnit,0x18,0x0);
                /*
                 * Turn off LED
                 */
#ifdef KPHACK
                athr_reg_wr(ATHR_GPIO_FUNCTIONS,
                    athr_reg_rd(ATHR_GPIO_FUNCTIONS) & (~(1 << (phyUnit + 3))));
#endif
            }
        }
        DPRINTF(MODULE_NAME ": Enet Unit:%d PHY:%d is Down.\n", mac->mac_unit,phyUnit);
    }

done:
    spin_unlock_irqrestore(&mac->mac_lock, flags);
    return 0;
}

uint16_t
athr_gmac_mii_read(int unit, uint32_t phy_addr, uint8_t reg)
{
    athr_gmac_t *mac   = athr_gmac_unit2mac(unit);
    uint16_t      addr  = (phy_addr << ATHR_GMAC_ADDR_SHIFT) | reg, val = 0;
    volatile int           rddata;
    uint16_t      ii = 0x1000;

    if(is_ar9330_emu())
        mac = athr_gmac_unit2mac(0);

    /*
     * Check for previous transactions are complete. Added to avoid
     * race condition while running at higher frequencies.
     */
    do
    {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, ATHR_GMAC_MGMT_CMD_READ);

    do
    {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    val = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_STATUS);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);

    return val;
}

void
athr_gmac_mii_write(int unit, uint32_t phy_addr, uint8_t reg, uint16_t data)
{
    athr_gmac_t *mac   = athr_gmac_unit2mac(unit);
    uint16_t      addr  = (phy_addr << ATHR_GMAC_ADDR_SHIFT) | reg;
    volatile int rddata;
    uint16_t      ii = 0x1000;

    if(is_ar9330_emu())
        mac = athr_gmac_unit2mac(0);


    /*
     * Check for previous transactions are complete. Added to avoid
     * race condition while running at higher frequencies.
     */
    do
    {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CTRL, data);

    do
    {
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);
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

static void
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

}

int
athr_gmac_hard_start(struct sk_buff *skb, struct net_device *dev)
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

    ath79_ddr_wb_flush(mac->ddr_flush);

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
    athr_gmac_tx_qos(mac,skb);

    if (athr_gmac_check_qos_bt(mac,skb))
        goto dropit;

    if (mac_has_flag(mac,ATHR_HEADER_ENABLED))  {
	skb = skb_unshare(skb, GFP_ATOMIC);
	if(unlikely(skb == NULL)) {
		printk("%s(%d) - skb_unshare error\n", __FUNCTION__, __LINE__);
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

    if (soc_is_ar7240())
        ds->res3       = 0;

    ds->more           = 0;

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

    if (unlikely(athr_gmac_tx_ring_full(mac,skb->priority))) {
        athr_gmac_handle_tx_full(mac,skb->priority);
    } else if (athr_gmac_tx_reap_thresh(mac,skb->priority)){
        unsigned long flags;
        spin_lock_irqsave(&mac->mac_lock, flags);
        athr_gmac_intr_enable_tx(mac);
        spin_unlock_irqrestore(&mac->mac_lock, flags);
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

    /* Valid bits are bits:0-11 */
    if (soc_is_qca955x() || soc_is_ar724x()) {
      isr &= 0xFFF;
      imr &= 0xFFF;
    }

    athr_gmac_trc(isr,"isr");
    athr_gmac_trc(imr,"imr");

    assert(mac->mac_ifup);
    assert(isr == (isr & imr));

    if (isr & (ATHR_GMAC_INTR_RX_OVF))
    {
        handled = 1;
        if (soc_is_ar934x()) {
            athr_gmac_reg_rmw_set(mac,ATHR_GMAC_DMA_INTR, ATHR_GMAC_INTR_RX_OVF);
            mac->eth_overfl_cntr++;
        } else if (soc_is_ar7240() || soc_is_ar933x()) {
            athr_gmac_reg_wr(mac,ATHR_GMAC_CFG1,(athr_gmac_reg_rd(mac,ATHR_GMAC_CFG1)&0xfffffff3));
            athr_gmac_intr_ack_rxovf(mac);
        }

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
    if (unlikely(isr & ATHR_GMAC_INTR_RX_BUS_ERROR))
    {
        assert(0);
        handled = 1;
        athr_gmac_intr_ack_rxbe(mac);
    }
    if (unlikely(isr & ATHR_GMAC_INTR_TX_BUS_ERROR))
    {
        assert(0);
        handled = 1;
        athr_gmac_intr_ack_txbe(mac);
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
            printk(MODULE_NAME ": unhandled intr eth%d isr %#x imr %#x\n", mac->mac_unit, isr, imr);
            return IRQ_HANDLED;
        }
        assert(0);
    }

    return IRQ_HANDLED;
}

static irqreturn_t
athr_gmac_link_intr(ATHR_MAC_ISR_ARGS)
{
    athr_gmac_t  *mac   = (athr_gmac_t *)dev_id;

    mac->phy->link_isr(0);

    return IRQ_HANDLED;
}

static int
ATHR_MAC_POLL()
#if 0
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
#else
{
    ATHR_MAC_NAPI_MAC();
    int work_done=0;
    int max_work;
    int ac;
    unsigned long flags;
    struct net_device *dev = mac->mac_dev;
    max_work  = ATHR_MAC_GET_MAX_WORK(budget, dev);

    athr_gmac_rx_pkt();

    if (work_done != budget) {
        napi_complete(napi);
        spin_lock_irqsave(&mac->mac_lock, flags);
        athr_gmac_intr_enable_recv(mac);
        spin_unlock_irqrestore(&mac->mac_lock, flags);
    }

    return work_done;
}
#endif


void
athr_remove_hdr(athr_gmac_t *mac, struct sk_buff *skb)
{

   if (mac_has_flag(mac,ATHR_S26_HEADER)) {
       skb_pull(skb, 2); /* remove attansic header */
   }
   else if (mac_has_flag(mac,ATHR_S16_HEADER) ||
              mac_has_flag(mac,ATHR_S27_HEADER) ||
	    mac_has_flag(mac, ATHR_HEADER_ENABLED)) {
       int i;

       for (i = 13 ; i > 1; i --) {
           skb->data[i] = skb->data[i - ATHR_HEADER_LEN];
       }
       skb_pull(skb,2);
   }

}

#define MEMLAT_OPT 1

athr_receive_pkt()
{
    ATHR_TASK_DEFS()
    athr_gmac_ring_t       *r     = &mac->mac_rxring; 				\
    athr_gmac_desc_t       *ds;
    athr_gmac_buffer_t     *bp;
    struct sk_buff      *skb;
    athr_gmac_rx_status_t   ret   = ATHR_GMAC_RX_STATUS_DONE;
    int head = r->ring_head, len, status, iquota = quota, more_pkts, rep;
#ifdef MEMLAT_OPT
    athr_gmac_desc_t       ds_pre[2];
    int head_pre;
#endif

    athr_gmac_trc(iquota,"iquota");
    status = athr_gmac_reg_rd(mac, ATHR_GMAC_DMA_RX_STATUS);

process_pkts:
    athr_gmac_trc(status,"status");

    assert(mac->mac_ifup);

    /*
     * Flush the DDR FIFOs for our gmac
     */
    ath79_ddr_wb_flush(mac->ddr_flush);

    assert(quota > 0); /* WCL */

    if (mac->dma_check) {
        mac->dma_check = 0;
    }
    else {
        ATHR_NAPI_CHECK_STATUS();
    }

#ifdef MEMLAT_OPT
    head_pre = head;
    ds_pre[quota & 1] = r->ring_desc[head_pre];
#endif
    while(quota)
    {
#ifndef MEMLAT_OPT
        ds    = &r->ring_desc[head];
#else
        ds = ds_pre + (quota & 1);
        if (quota - 1) {
            head_pre = athr_gmac_ring_incr(r, head_pre);
            ds_pre[(quota & 1) ^ 1] = r->ring_desc[head_pre];
        }
#endif

        athr_gmac_trc(head,"hd");
        athr_gmac_trc(ds,  "ds");

        if (athr_gmac_rx_owned_by_dma(ds))
        {
            assert(quota != iquota); /* WCL */
            break;
        }
        athr_gmac_intr_ack_rx(mac);

        bp                  = &r->ring_buffer[head];
        skb                 = bp->buf_pkt;

        if (mac_has_flag(mac, ATHR_JUMBO_FR) && !soc_is_ar7240())
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

        if(unlikely(skb->len != 0))
            athr_mac_cache_inv((unsigned long)(skb->data), skb->len);

        athr_ssdk_process_arp_header(mac, skb);

        athr_gmac_rx_qos(mac,skb);

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

        ds->pkt_start_addr  = virt_to_phys(bogus_skb->data);

        quota--;

        athr_nat_process_ingress_pkts(mac->mac_unit, skb, ds);
#if ETHDEBUG_ENABLED
    if (eth_vi_dbg_params.vi_dbg_cfg & 1) {
        ethdbg_check_rxdrop(skb);
    }    
#endif
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

    if (mac_has_flag(mac,ATHR_RX_TASK))
        athr_gmac_intr_enable_recv(mac);

    RX_RETURN();
}

#if ETHDEBUG_ENABLED
/* This function is used to collect stats from the received skbuff after the marker
 * filtering has been completed 
 */
void athr_vi_dbg_stats(struct sk_buff *skb, int stream_num)
{
    u_int8_t *payload;
    u_int32_t i;
    u_int32_t rx_seq_num = 0;
    int32_t   diff;
    static u_int32_t prev_rx_seq_num[MAX_NUM_STREAMS] = {0, 0, 0, 0}; 
    static u_int32_t current_seq_num[MAX_NUM_STREAMS]   = {0, 0, 0, 0}; 
    static u_int32_t last_seq_num[MAX_NUM_STREAMS]    = {0, 0, 0, 0};
    static u_int8_t start[MAX_NUM_STREAMS]           = {0, 0, 0, 0};
    static u_int16_t offset, num_bytes;
	
   /* If restart is set, re-initialize internal variables & reset the restart parameter */
    if (eth_vi_dbg_params.vi_dbg_restart) 	{
	    for (i = 0; i < MAX_NUM_STREAMS; i++) {
	        start[i]           = 0;
	        prev_rx_seq_num[i] = 0;
	        current_seq_num[i]   = 0;
	        last_seq_num[i]    = 0;
	    }	
        eth_rx_drop = 0;
        eth_vi_dbg_params.vi_dbg_restart = 0;
    }

    payload   = (u_int8_t *)skb->data;
    offset    =  eth_vi_dbg_params.vi_dbg.rxseq_offset;
    num_bytes =  eth_vi_dbg_params.vi_dbg.rxseq_num_bytes;
	
    for (i = 0; i < num_bytes; i++) {
	    rx_seq_num += (payload[offset + i] << (8 * (3 - i)));
    }

    rx_seq_num = rx_seq_num >> eth_vi_dbg_params.vi_rx_seq_rshift;
    if (start[stream_num] == 0)
    { // 31st bit is set for iperf 1st packet
        printk("stream :%d  start seq no : 0x%08x \n",stream_num+1,rx_seq_num);
        if (rx_seq_num & 0x80000000)
            rx_seq_num &=0x7fffffff;
      
    }
    current_seq_num[stream_num] = rx_seq_num;
    diff = current_seq_num[stream_num] - prev_rx_seq_num[stream_num];

    if ((diff < 0) && start[stream_num] == 1)
    {  // wrap case & Loss in Wrap
        diff = eth_vi_dbg_params.vi_rx_seq_max-prev_rx_seq_num[stream_num]+current_seq_num[stream_num];
        // not handling out of order case
    }
        
    if ((diff > 1) && start[stream_num] == 1) 
    {
        eth_rx_drop += diff;
        eth_vi_dbg_params.vi_rx_seq_drop = 1;
    }

    prev_rx_seq_num[stream_num] = current_seq_num[stream_num];
    start[stream_num] = 1;

}

/* This function is used to filter the received skbuff based on the defined markers.
 * Normally we will give UDP source/dest port as a marker, In specified location we will check for
 * patteren, right now we are supporting 4 streams with 4 different markers.
 */ 
u_int32_t
athr_vi_dbg_check_markers(struct sk_buff *skb)
{
    u_int8_t *payload;
    u_int8_t match;
    u_int8_t i, j, s; 
    payload = (u_int8_t *)skb->data;
    u_int32_t mfail;

    for (s = 0; s < eth_vi_dbg_params.vi_num_streams; s++) {
	    mfail = 0;  /* Reset marker failures for new stream. mfail is used to reduce checks if maker checks
	                 * fail for any stream. If a filure is detected immediately move on to the markers for
	  	             * the next stream */ 
	    for (i = 0; i < eth_vi_dbg_params.vi_num_markers; i++) {
	        for (j = 0; j < eth_vi_dbg_params.vi_dbg.markers[s][i].num_bytes; j++) {
	            match = ((eth_vi_dbg_params.vi_dbg.markers[s][i].match) >> (8*(eth_vi_dbg_params.vi_dbg.markers[s][i].num_bytes-1-j))) & 0x000000FF;
		        if (payload[ eth_vi_dbg_params.vi_dbg.markers[s][i].offset + j] != match) {
		            mfail = 1;  
		            break;
		        }
	        }
	        if (mfail == 1) {
		        break;       
	        }
	    }
	    
	    if (!mfail) {
		   return s;
	    }
    }
    return -1;
}

/*
 * This function used to debug iperf3/chariot/TestMepegStreamer streams using match patteren
 * configured by the application.
 * Get sequence number from specified location and compare the with the previous sequence number 
 * for packet drops.
 */

void ethdbg_check_rxdrop(struct sk_buff *skb)
{
    int stream;

    stream = athr_vi_dbg_check_markers(skb);

    if (stream != -1) {
	    athr_vi_dbg_stats(skb, stream);	
    }

}
#endif

static struct sk_buff *
athr_gmac_buffer_alloc(void)
{
    struct sk_buff *skb;
    u_int32_t off;

    skb = dev_alloc_skb(ATHR_GMAC_RX_ALLOC_SIZE + ARCH_DMA_MINALIGN +
        ATHR_GMAC_RX_PADDING(ATHR_GMAC_RX_ALLOC_SIZE + ARCH_DMA_MINALIGN));
    if (unlikely(!skb))
        return NULL;

    skb_reserve(skb, (ATHR_GMAC_RX_RESERVE));

    off = ((unsigned long) skb->data) & (ARCH_DMA_MINALIGN - 1);
    if (off != 0)
        skb_reserve(skb, ARCH_DMA_MINALIGN - off);

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

#ifndef MEMLAT_OPT
        assert(!athr_gmac_rx_owned_by_dma(ds));
#endif

        assert(!bf->buf_pkt);

        bf->buf_pkt         = athr_gmac_buffer_alloc();
        if (!bf->buf_pkt)
        {
            printk(MODULE_NAME ": outta skbs!\n");
            break;
        }

        /* Invalidate chache should be chache aligned */
        athr_mac_cache_inv((unsigned long)bf->buf_pkt->data, ATHR_GMAC_RX_BUF_SIZE);

        ds->pkt_start_addr  = virt_to_phys(bf->buf_pkt->data);

        athr_gmac_rx_give_to_dma(ds);
        refilled ++;

        tail = athr_gmac_ring_incr(r, tail);

    }

   /*
    * Flush descriptors
    */
   wmb();

   if (soc_is_ar7240()||soc_is_ar933x()) {
        athr_gmac_reg_wr(mac,ATHR_GMAC_CFG1,(athr_gmac_reg_rd(mac,ATHR_GMAC_CFG1)|0xc));
        athr_gmac_rx_start(mac);
    }

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
    unsigned long flags;

    spin_lock_irqsave(&mac->mac_lock, flags);
    r = &mac->mac_txring[ac];
    head = r->ring_head;
    tail = r->ring_tail;

    athr_gmac_trc_new(head,"hd");
    athr_gmac_trc_new(tail,"tl");

    ath79_ddr_wb_flush(mac->ddr_flush);

    while(tail != head)
    {
        ds   = &r->ring_desc[tail];

        athr_gmac_trc_new(ds,"ds");

        if(athr_gmac_tx_owned_by_dma(ds))
            break;

        bf      = &r->ring_buffer[tail];
        assert(bf->buf_pkt);

        athr_gmac_trc_new(bf->buf_lastds,"lastds");

        if(athr_gmac_tx_owned_by_dma(bf->buf_lastds))
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

    if (athr_gmac_tx_reap_thresh(mac,ac))
        athr_gmac_intr_enable_tx(mac);

    if (netif_queue_stopped(mac->mac_dev) &&
        netif_carrier_ok(mac->mac_dev)){
        if (athr_gmac_tx_ring_full(mac,ac)) {
            athr_gmac_intr_enable_tx(mac);
        } else {
            netif_wake_queue(mac->mac_dev);
        }
    }

done:

    spin_unlock_irqrestore(&mac->mac_lock, flags);

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
            ds[i].next_desc     =  KSEG1ADDR(athr_gmac_desc_dma_addr(r, &ds[next]));
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

        athr_mac_cache_inv((unsigned long)bf->buf_pkt->data, ATHR_GMAC_RX_BUF_SIZE);

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

    athr_gmac_desc_alloc(mac, r, count, flag);

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
    athr_gmac_trc(dev,"dev");
    printk("%s\n",__func__);
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

    athr_gmac_fast_reset(mac,NULL,0);
}

static void
athr_gmac_get_default_macaddr(athr_gmac_t *mac, u8 *mac_addr)
{
    u8 *eep_mac_addr = NULL;
    /* Use MAC address stored in Flash */
    if (!is_emu() && !is_ar9330_emu()) {

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
             /* Fix for EV 97515 - Over sized packets' count are shown in ifconfig*/
             /* These are read only regs, so not adding it so that it wont show up
                in ifconfig */ 
                                     /* mac->mac_net_stats.rx_over_errors +*/
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

    /* QCA955x has it's own max len */
    if(soc_is_qca955x() && (*len > CONFIG_QCA955X_MAX_LEN_PER_TX_DS))
      *len = CONFIG_QCA955X_MAX_LEN_PER_TX_DS;

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

static int athr_gmac_phy_show(struct seq_file *m, void *v)
{
    athr_gmac_t      *mac;
    int carrier_ok;
    int i;
#ifdef CONFIG_AG7240_S16_PHY
    int base;
#endif

    seq_printf(m, "<phy>\n");
    for(i = 0; i < ATHR_GMAC_NMACS; i++) {
        struct net_device *root_dev;
        if (athr_gmacs[i])
           root_dev = athr_gmacs[i]->mac_dev;
        else  {
            continue;
        }

        mac = netdev_priv(root_dev);
        carrier_ok = netif_carrier_ok(mac->mac_dev);

        seq_printf(m, "<phy_info\n");
        seq_printf(m, " phy_no='%d'\n", i);
        seq_printf(m, " carrier='%d'\n", carrier_ok);
        if (carrier_ok) {
            seq_printf(m, " speed='%s'\n", spd_str[mac->mac_speed]);
            seq_printf(m, " duplex='%s'\n", dup_str[mac->mac_fdx]);
        }
        seq_printf(m, " mii='%s'\n", mii_str[mac->mac_unit][mii_if(mac)]);
        seq_printf(m, "/>\n");
    }
    seq_printf(m, "</phy>\n");

#ifdef CONFIG_AG7240_S16_PHY
    seq_printf(m, "Dumping the Global Control Registers\n");
    for (i=0; i < 0x98; i+=4) {
        uint32_t value = athrs16_reg_read(i);
        seq_printf(m, "0x%04x %08x\n", i, value);
    }

    seq_printf(m, "\nDumping the Port Control Registers\n");
    for (base = 0x0100; base <= 0x600; base+=0x100) {
        seq_printf(m, "Phy Register %d Physical Addr %d\n", (base / 0x100) - 1, port_addr_to_physical(base));
        for (i = 0; i <= 0x20; i+=4) {
            uint32_t value = athrs16_reg_read(i + base);
            seq_printf(m, "0x%04x %08x\n", i + base, value);
        }
    }
#endif

    return 0;
}

static int athr_gmac_txdesc_show(struct seq_file *m, void *v)
{
    int i, j;
    for(i = 0; i < ATHR_GMAC_NMACS; i++) {
        struct net_device *root_dev;
        if (athr_gmacs[i])
           root_dev = athr_gmacs[i]->mac_dev;
        else  {
            continue;
        }
        if (root_dev) {
            athr_gmac_t      *mac = netdev_priv(root_dev);
            athr_gmac_ring_t *txr = (athr_gmac_ring_t *) &mac->mac_txring;
            seq_printf(m, "MAC %d\n", i);
            seq_printf(m, "tx head: %d tail %d num_descs %d\n", txr->ring_head, txr->ring_tail, mac->num_tx_desc);
            seq_printf(m, "unused buffers %d reap threshold %d\n", athr_gmac_ndesc_unused(mac, txr), mac->reap_thresh);
            seq_printf(m, "tx_ring_full %d\n", mac->mac_tx_ring_full);
            seq_printf(m, "\n");

            for (j = 0; j< (mac->num_tx_desc); j++) {
                athr_gmac_desc_t *ds = &txr->ring_desc[j];
                seq_printf(m, "desc[%d-%03d] %d %08x :: %08x %08x %08x%s%s\n",
                        i, j, athr_gmac_tx_owned_by_dma(ds), (uint32_t)ds,
                        ds->pkt_start_addr, *(((uint32_t *)ds)+1), ds->next_desc,
                        (j == txr->ring_head) ? " head" : "",
                        (j == txr->ring_tail) ? " tail" : "");
            }
        }
    }
    return 0;
}


/*
 * All allocations (except irq and rings).
 */
static int __init
athr_gmac_init(void)
{
    int i;
    struct net_device *dev;
    athr_gmac_t      *mac;
    uint32_t mask = 0;

#ifdef CONFIG_SOC_QCA955X
    if (soc_is_qca955x()) {
	extern int qca955x_gmac_init(void);
	i = qca955x_gmac_init();
	if (i)
	    return i;
	goto worked;
    }
#endif

    if (!soc_has_ag7240())
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
    * Let hydra know how much to put into the fifo in words (for tx)
    */
    if (0 == fifo_3)
        fifo_3 = 0x000001ff | ((ATHR_GMAC_TX_FIFO_LEN-tx_len_per_ds)/4)<<16;

    printk(MODULE_NAME ": fifo cfg 3 %08x\n", fifo_3);

    bogus_skb = athr_gmac_buffer_alloc();
    if(!bogus_skb) {
        printk(MODULE_NAME ": unable to allocate bogus_skb\n");
        return 1;
    }

    /* 
    ** Do the rest of the initializations 
    */
    for(i = 0; i < ATHR_GMAC_NMACS; i++)
    {
        if (i != 0 && soc_is_qca955x()) {
            athr_gmacs[i] = NULL;
            continue;
        }

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

        athr_gmacs[i]  = mac;

        athr_gmac_attach(mac);

	if(!mac->ops) {
           printk(MODULE_NAME "ERROR: No MAC found !!!\n");
	   assert(0);
           return 1;
        }
        mac->ops->check_link = athr_gmac_check_link;
        mac->ops->set_caps(mac);

        athr_gmac_phy_attach(mac,mac->mac_unit);

        if (mac_has_flag(mac,ETH_SWONLY_MODE))
        {
            if (mac->mac_unit == 0)
                continue;
        }

        spin_lock_init(&mac->mac_lock);
        /*
        * out of memory timer
        */
        init_timer(&mac->mac_oom_timer);
        mac->mac_oom_timer.data     = (unsigned long)mac;
        mac->mac_oom_timer.function = (void *)athr_gmac_oom_timer;

        ATHR_MAC_INIT_WORK(mac, athr_gmac_tx_timeout_task);
        ATHR_MAC_RESET_INIT_WORK(mac, athr_gmac_reset_task);
	/*
        * watchdog task
        */
        ATHR_MAC_SET_DEV_NET_OPS(dev, mac,
                                 athr_gmac_get_stats,
                                 athr_gmac_open,
                                 athr_gmac_stop,
                                 athr_gmac_hard_start,
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

        athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST
                | ATHR_GMAC_CFG1_RX_RST | ATHR_GMAC_CFG1_TX_RST);

        udelay(20);

        /* Reset PHY */
        mask = ATHR_RESET_GE0_PHY;
	ath79_device_reset_set(mask);
        /*
         * Increase the delay for PHY reset to settle down.
         */
        mdelay(50);
	ath79_device_reset_clear(mask);
        mdelay(200);

        mask = athr_gmac_reset_mask(mac->mac_unit);

        if (soc_is_ar7241() || soc_is_ar7242() ||  soc_is_ar934x())
            mask |=  ATHR_RESET_GE0_MDIO | ATHR_RESET_GE1_MDIO;
       /*
        * put into reset, hold, pull out.
        */
	ath79_device_reset_set(mask);
        mdelay(100);
	ath79_device_reset_clear(mask);
        /*
         * Increase the delay for MDIO and MAC reset to settle down.
         */
        mdelay(200);

        if (soc_is_ar934x())
            athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1, ATHR_GMAC_CFG1_SOFT_RST);


        athr_register_qos(mac);
        athr_register_vlan_igmp(mac);

	if(!mac->phy)
            printk("MAC:%d Warning: Phy not found!!!\n",mac->mac_unit);

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

        /*
         * Special for the Z1. PHY needs to be available
         * to the Meraki swioctl's before open() is called.
         */
        if (soc_is_ar934x())
	    athr_gmac_mii_setup(mac);
    }

    athr_register_hwaccels_notifiers(athr_gmacs[0]);

    // Register Push button notification handler
    if (is_push_button() && athr_gmacs[0]) {
        mac = athr_gmacs[0];
        dev = mac->mac_dev;
        register_simple_config_callback (dev->name, (void *) pushbutton_intr, (void *) dev,
                                         (void *)&mac->push_dur);
    }

#ifdef CONFIG_ATH79_MACH_MERAKI_Z1
    if (mac->phy) {
        int i;
        //disable 4 lan phys, leave internet on
        for (i = 0; i < 4; i++)
            mac->phy->write_phy_reg(0, i, 0, S17_CTRL_POWER_DOWN);
        printk(MODULE_NAME ": Powered down LAN port phys\n");

        //set up the switch port leds to the appropriate configuration
        mac->phy->write_phy_reg(0, 0x18, 0x0, 0x0);
        mac->phy->write_phy_reg(0, 0x11, 0xf, 0x3ff);
        mac->phy->write_phy_reg(0, 0x11, 0xe, 0xff09);
        mac->phy->write_phy_reg(0, 0x18, 0x0, 0x0);
        mac->phy->write_phy_reg(0, 0x11, 0xd, 0xcf00);
        mac->phy->write_phy_reg(0, 0x11, 0xc, 0xcf00);
        mac->phy->write_phy_reg(0, 0x18, 0x0, 0x0);
        mac->phy->write_phy_reg(0, 0x11, 0x9, 0xcc35);
        mac->phy->write_phy_reg(0, 0x11, 0x8, 0xcc35);
        mac->phy->write_phy_reg(0, 0x18, 0x0, 0x0);
        mac->phy->write_phy_reg(0, 0x11, 0xb, 0xcb35);
        mac->phy->write_phy_reg(0, 0x11, 0xa, 0xcb35);
    } else {
        printk(MODULE_NAME ": Couldn't disable switch ports, no phy found\n");
    }
#endif // CONFIG_ATH79_MACH_MERAKI_Z1

worked:

#ifdef CONFIG_PROC_FS
    {
        struct proc_dir_entry *reg_e;
        struct proc_dir_entry *phy_e;
        struct proc_dir_entry *txdesc_e;
        char *proc_dir_name = soc_is_ar724x() ? "ag7240" : soc_is_qca955x() ? "qca955x":"ag7100";
        /* XXX:  SHOULD BE REVISITED (AACHARYA) */
        if (!(ag7240_dir = proc_mkdir(proc_dir_name, init_net.proc_net)) ||
            !(reg_e = create_proc_entry("regdump", 0444, ag7240_dir)) ||
            !(phy_e = create_proc_entry("phy", 0444, ag7240_dir)) ||
            !(txdesc_e = create_proc_entry("txdesc", 0444, ag7240_dir))) {
            printk(KERN_ERR "ag7240: cannot create proc entry.\n");
            return -ENOENT;
        }
        //reg_e->proc_fops = &ag7240_regdump_operations;
        phy_e->proc_fops = &athr_gmac_phy_operations;
        txdesc_e->proc_fops = &athr_gmac_txdesc_dump_operations;
    }
#endif /* CONFIG_PROC_FS */

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

static void __exit
athr_gmac_cleanup(void)
{
    int i;

#ifdef CONFIG_SOC_QCA955X
    if (soc_is_qca955x()) {
	extern void qca955x_gmac_cleanup(void);
	qca955x_gmac_cleanup();
	return;
    }
#endif

    if (!soc_has_ag7240())
	return;

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

     if(bogus_skb != NULL) {
        athr_gmac_buffer_free(bogus_skb);
	bogus_skb = NULL;
     }
#ifdef CONFIG_PROC_FS
     {
        char *proc_dir_name = soc_is_ar724x() ? "ag7240" : soc_is_qca955x() ? "qca955x":"ag7100";
        remove_proc_entry("phy",ag7240_dir);
        remove_proc_entry("txdesc",ag7240_dir);
        remove_proc_entry("regdump",ag7240_dir);
        remove_proc_entry(proc_dir_name, init_net.proc_net);
     }
#endif

    printk(MODULE_NAME ": cleanup done\n");
}

#ifdef CONFIG_ATH79_MACH_MERAKI_Z1
//XXX needs to init earlier than device_initcall, but anything earlier than fs_initcall causes a panic
fs_initcall(athr_gmac_init);
#else
module_init(athr_gmac_init);
#endif

module_exit(athr_gmac_cleanup);
