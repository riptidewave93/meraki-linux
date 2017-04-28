#include <generated/autoconf.h>
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
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <net/sch_generic.h>
#include <asm/bootinfo.h>

#include "ath79.h"

#include "ag7100.h"
#include "ag7100_phy.h"
#include "ag7100_trc.h"

static ag7100_mac_t *ag7100_macs[2];
static void ag7100_hw_setup(ag7100_mac_t *mac);
static void ag7100_hw_stop(ag7100_mac_t *mac);
static void ag7100_check_link(unsigned long data);
static int  ag7100_tx_alloc(ag7100_mac_t *mac);
static int  ag7100_rx_alloc(ag7100_mac_t *mac);
static void ag7100_rx_free(ag7100_mac_t *mac);
static void ag7100_tx_free(ag7100_mac_t *mac);
static int  ag7100_ring_alloc(ag7100_ring_t *r, int count);
static int  ag7100_tx_reap(ag7100_mac_t *mac);
static void ag7100_ring_release(ag7100_mac_t *mac, ag7100_ring_t  *r);
static void ag7100_ring_free(ag7100_ring_t *r);
static void ag7100_tx_timeout_task(struct work_struct *work);
static int  ag7100_poll(struct napi_struct *napi, int budget);
#ifdef CONFIG_AR9100
void ag7100_dma_reset(ag7100_mac_t *mac);
#endif
static int  ag7100_recv_packets(struct net_device *dev, ag7100_mac_t *mac,
    int max_work);
static irqreturn_t ag7100_intr(int cpl, void *dev_id);
static struct sk_buff * ag7100_buffer_alloc(void);

static int ag7100_regdump_show(struct seq_file *m, void *v);
static int ag7100_phy_show(struct seq_file *m, void *v);
static int ag7100_txdesc_show(struct seq_file *m, void *v);

static int ag7100_regdump_open(struct inode *inode, struct file *file)
{
    return single_open(file, ag7100_regdump_show, NULL);
}

static int ag7100_phy_open(struct inode *inode, struct file *file)
{
    return single_open(file, ag7100_phy_show, NULL);
}

static int ag7100_txdesc_open(struct inode *inode, struct file *file)
{
    return single_open(file, ag7100_txdesc_show, NULL);
}

static struct file_operations ag7100_regdump_operations = {
    .open       = ag7100_regdump_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static struct file_operations ag7100_phy_operations = {
    .open       = ag7100_phy_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static struct file_operations ag7100_txdesc_dump_operations = {
    .open       = ag7100_txdesc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static const char *mii_str[2][4] = {
    {"GMii", "Mii", "RGMii", "RMii"},
    {"RGMii", "RMii"}
};
static const char *spd_str[] = {"10Mbps", "100Mbps", "1000Mbps"};
static const char *dup_str[] = {"half", "full"};

#define MODULE_NAME "AG7100"

/* if 0 compute in init */
static int tx_len_per_ds = 0;
module_param(tx_len_per_ds, int, 0);
MODULE_PARM_DESC(tx_len_per_ds, "Size of DMA chunk");

/* if 0 compute in init */
static int tx_max_desc_per_ds_pkt=0;

/* if 0 compute in init */
#ifdef CONFIG_AR9100
static int fifo_3 = 0x780008;
#else
static int fifo_3 = 0;
#endif
module_param(fifo_3, int, 0);
MODULE_PARM_DESC(fifo_3, "fifo cfg 3 settings");

static int mii0_if = AG7100_MII0_INTERFACE;
module_param(mii0_if, int, 0);
MODULE_PARM_DESC(mii0_if, "mii0 connect");

static int mii1_if = AG7100_MII1_INTERFACE;
module_param(mii1_if, int, 0);
MODULE_PARM_DESC(mii1_if, "mii1 connect");
#ifndef CONFIG_AR9100
static int gige_pll = 0x0110000;
#else
#define SW_PLL 0x1f000000ul
static int gige_pll = 0x1a000000;
#endif
module_param(gige_pll, int, 0);
MODULE_PARM_DESC(gige_pll, "Pll for (R)GMII if");

/*
* Cfg 5 settings
* Weed out junk frames (CRC errored, short collision'ed frames etc.)
*/
static int fifo_5 = 0x7ffef;
module_param(fifo_5, int, 0);
MODULE_PARM_DESC(fifo_5, "fifo cfg 5 settings");

#define addr_to_words(addr, w1, w2)  {                                 \
    w1 = (addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]; \
    w2 = (addr[4] << 24) | (addr[5] << 16) | 0;                        \
}

#ifdef AG7100_PHY_RUNTIME
struct ag7100_phy_disp_t *ag7100_phy_disp[2];

struct ag7100_phy_disp_t *ag7100_phy_disp_list[] =
  {
#ifdef CONFIG_AG7100_ATHRF1_PHY
    &athr_phy_disp,
#endif

#ifdef CONFIG_AG7100_ATHRS16_PHY
    &athrs16_phy_disp,
#endif

#ifdef CONFIG_AG7100_ATHRS26_PHY
    &athrs26_phy_disp,
#endif

#ifdef CONFIG_AG7100_VITESSE_8601_PHY
    &vsc8601_phy_disp,
#endif

#ifdef CONFIG_AG7100_VITESSE_8601_7395_PHY
    &vsc73xx_phy_disp,
#endif

#ifdef CONFIG_AG7100_ICPLUS_PHY
    &ip_phy_disp,
#endif

#ifdef CONFIG_AG7100_REALTEK_PHY
    &rt_phy_disp,
#endif

#ifdef CONFIG_AG7100_ADM6996FC_PHY
    &adm_phy_disp,
#endif

  };

#define AG7100_PHY_DISP_COUNT ((sizeof(ag7100_phy_disp_list)/sizeof(struct ag7100_phy_disp_t *)))

#endif /* AG7100_PHY_RUNTIME */

/*
 * Defines specific to this implemention
 */

#ifndef CONFIG_AG7100_LEN_PER_TX_DS
#error Please run menuconfig and define CONFIG_AG7100_LEN_PER_TX_DS
#endif

#ifndef CONFIG_AG7100_NUMBER_TX_PKTS
#error Please run menuconfig and define CONFIG_AG7100_NUMBER_TX_PKTS
#endif

#ifndef CONFIG_AG7100_NUMBER_RX_PKTS
#error Please run menuconfig and define CONFIG_AG7100_NUMBER_RX_PKTS
#endif
#define AG7100_TX_FIFO_LEN          2048
#define AG7100_TX_MIN_DS_LEN        128
#define AG7100_TX_MAX_DS_LEN        AG7100_TX_FIFO_LEN

#define AG7100_TX_MTU_LEN           1536

#define AG7100_TX_DESC_CNT           CONFIG_AG7100_NUMBER_TX_PKTS*tx_max_desc_per_ds_pkt
#define AG7100_TX_REAP_THRESH        AG7100_TX_DESC_CNT/2

#define AG7100_RX_DESC_CNT           CONFIG_AG7100_NUMBER_RX_PKTS

#define AG7100_NAPI_WEIGHT           64
#define AG7100_PHY_POLL_SECONDS      1

static void ag7100_unhang(ag7100_mac_t *mac)
{
    unsigned long flags;
    ag7100_desc_t *ds;
    ag7100_ring_t *txr = &mac->mac_txring;
    spin_lock_irqsave(&mac->mac_lock, flags);
    ds = (ag7100_desc_t *)KSEG1ADDR(ag7100_reg_rd(mac, AG7100_DMA_TX_DESC));
    ar7100_flush_ge(mac->mac_unit);
    if (ds->is_empty && ds != &txr->ring_desc[txr->ring_head]) {
        ag7100_desc_t *ds2 = (ag7100_desc_t *)KSEG1ADDR(ag7100_reg_rd(mac, AG7100_DMA_TX_DESC));
        if (ds2 == ds) {
            mac->mac_hang_war++;
            ag7100_reg_wr(mac, AG7100_DMA_TX_DESC, ((uint32_t)ds->next_desc));
            ag7100_tx_start(mac);
        }
    }
    spin_unlock_irqrestore(&mac->mac_lock, flags);
}

static inline int ag7100_tx_reap_thresh(ag7100_mac_t *mac)
{
    ag7100_ring_t *r = &mac->mac_txring;

    return (ag7100_ndesc_unused(mac, r) < AG7100_TX_REAP_THRESH);
}

static inline int ag7100_tx_ring_full(ag7100_mac_t *mac)
{
    ag7100_ring_t *r = &mac->mac_txring;

    ag7100_trc_new(ag7100_ndesc_unused(mac, r),"tx ring full");

    if (ag7100_ndesc_unused(mac, r) < tx_max_desc_per_ds_pkt + 2)
      mac->tx_ring_full++;

    return (ag7100_ndesc_unused(mac, r) < tx_max_desc_per_ds_pkt + 2);
}

static int
ag7100_open(struct net_device *dev)
{
    unsigned int w1 = 0, w2 = 0;
    ag7100_mac_t *mac = (ag7100_mac_t *)netdev_priv(dev);
    int st;
#if defined(CONFIG_AR9100) && defined(SWITCH_AHB_FREQ)
    u32 tmp_pll, pll;
#endif
#ifdef AG7100_PHY_RUNTIME
    int phy_disp_index;
#endif

    assert(mac);
    mac->mac_hang_war = 0;

    st = request_irq(mac->mac_irq, ag7100_intr, 0, dev->name, dev);
    if (st < 0)
    {
        printk(MODULE_NAME ": request irq %d failed %d\n", mac->mac_irq, st);
        return 1;
    }
    if (ag7100_tx_alloc(mac)) goto tx_failed;
    if (ag7100_rx_alloc(mac)) goto rx_failed;

    ag7100_hw_setup(mac);
#if defined(CONFIG_AR9100) && defined(SWITCH_AHB_FREQ)
    /* 
    * Reduce the AHB frequency to 100MHz while setting up the 
    * S26 phy. 
    */
    pll= ar7100_reg_rd(AR7100_PLL_CONFIG);
    tmp_pll = pll& ~((PLL_DIV_MASK << PLL_DIV_SHIFT) | (PLL_REF_DIV_MASK << PLL_REF_DIV_SHIFT));
    tmp_pll = tmp_pll | (0x64 << PLL_DIV_SHIFT) |
        (0x5 << PLL_REF_DIV_SHIFT) | (1 << AHB_DIV_SHIFT);

    ar7100_reg_wr_nf(AR7100_PLL_CONFIG, tmp_pll);
    udelay(100*1000);
#endif

#if defined(CONFIG_AG7100_ATHRS26_PHY)
    /* if using header for register configuration, we have to     */
    /* configure s26 register after frame transmission is enabled */
    if (mac->mac_unit == 1) /* wan phy */
        athrs26_reg_init();
#elif defined(CONFIG_AG7100_ATHRS16_PHY)
    athrs16_reg_init();
#endif

#ifdef AG7100_PHY_RUNTIME
    for (phy_disp_index = 0; phy_disp_index < AG7100_PHY_DISP_COUNT;
	     phy_disp_index++) {
      if ((*ag7100_phy_disp_list[phy_disp_index]->phy_setup)(mac->mac_unit))
	break;
    }
    if (phy_disp_index == AG7100_PHY_DISP_COUNT)
      goto phy_failed;
    ag7100_phy_disp[mac->mac_unit] = ag7100_phy_disp_list[phy_disp_index];
    mii_if(mac) = ag7100_phy_disp_list[phy_disp_index]->mii;
#else
    if (!ag7100_phy_setup(mac->mac_unit))
      goto phy_failed;
#endif

#if defined(CONFIG_AR9100) && defined(SWITCH_AHB_FREQ)
    ar7100_reg_wr_nf(AR7100_PLL_CONFIG, pll);
    udelay(100*1000);
#endif
    /*
    * set the mac addr
    */
    addr_to_words(dev->dev_addr, w1, w2);
    ag7100_reg_wr(mac, AG7100_GE_MAC_ADDR1, w1);
    ag7100_reg_wr(mac, AG7100_GE_MAC_ADDR2, w2);

    /*
    * phy link mgmt
    */
    init_timer(&mac->mac_phy_timer);
    mac->mac_phy_timer.data     = (unsigned long)mac;
    mac->mac_phy_timer.function = ag7100_check_link;
    ag7100_check_link((unsigned long)mac);

    dev->trans_start = jiffies;

    napi_enable(&mac->napi);
    ag7100_int_enable(mac);
    ag7100_rx_start(mac);

    ag7100_start_rx_count(mac);

    return 0;

phy_failed:
    ag7100_hw_stop(mac);
    ag7100_rx_free(mac);
rx_failed:
    ag7100_tx_free(mac);
tx_failed:
    free_irq(mac->mac_irq, dev);
    return 1;
}

static int
ag7100_stop(struct net_device *dev)
{
    ag7100_mac_t *mac = (ag7100_mac_t *)netdev_priv(dev);
    unsigned long flags;

    spin_lock_irqsave(&mac->mac_lock, flags);
    netif_stop_queue(dev);
    netif_carrier_off(dev);

    ag7100_hw_stop(mac);
    free_irq(mac->mac_irq, dev);

    ag7100_tx_free(mac);
    ag7100_rx_free(mac);

    del_timer(&mac->mac_phy_timer);

    spin_unlock_irqrestore(&mac->mac_lock, flags);
    /*ag7100_trc_dump();*/
    return 0;
}

static void
ag7100_hw_setup(ag7100_mac_t *mac)
{
    ag7100_ring_t *tx = &mac->mac_txring, *rx = &mac->mac_rxring;
    ag7100_desc_t *r0, *t0;

    ag7100_reg_wr(mac, AG7100_MAC_CFG1, (AG7100_MAC_CFG1_RX_EN |
        AG7100_MAC_CFG1_TX_EN));
    ag7100_reg_rmw_set(mac, AG7100_MAC_CFG2, (AG7100_MAC_CFG2_PAD_CRC_EN |
        AG7100_MAC_CFG2_LEN_CHECK));

    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_0, 0x1f00);
    /*
    * set the mii if type - NB reg not in the gigE space
    */
    ar7100_reg_wr(mii_reg(mac), mii_if(mac));
    ag7100_reg_wr(mac, AG7100_MAC_MII_MGMT_CFG, AG7100_MGMT_CFG_CLK_DIV_20);

#ifdef CONFIG_AR7100_EMULATION
    ag7100_reg_rmw_set(mac, AG7100_MAC_FIFO_CFG_4, 0x3ffff);
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_1, 0xfff0000);
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_2, 0x1fff);
#else
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_1, 0xfff0000);
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_2, 0x1fff);
    /*
    * Weed out junk frames (CRC errored, short collision'ed frames etc.)
    */
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_4, 0xffff);
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_5, 0x7ffef);
#endif

    t0  =  &tx->ring_desc[0];
    r0  =  &rx->ring_desc[0];

    ag7100_reg_wr(mac, AG7100_DMA_TX_DESC, ag7100_desc_dma_addr(tx, t0));
    ag7100_reg_wr(mac, AG7100_DMA_RX_DESC, ag7100_desc_dma_addr(rx, r0));

    //cfg1 should normally be 3f if things are working.  it will be 35 if the phy is not functioning properly
    printk(MODULE_NAME ": cfg1 %#x cfg2 %#x\n", ag7100_reg_rd(mac, AG7100_MAC_CFG1),
        ag7100_reg_rd(mac, AG7100_MAC_CFG2));
}

static void
ag7100_hw_stop(ag7100_mac_t *mac)
{
    ag7100_rx_stop(mac);
    ag7100_tx_stop(mac);
    ag7100_int_disable(mac);
    /*
    * put everything into reset.
    */
    ag7100_reg_rmw_set(mac, AG7100_MAC_CFG1, AG7100_MAC_CFG1_SOFT_RST);
}

/*
 * program the usb pll (misnomer) to genrate appropriate clock
 * Write 2 into control field
 * Write pll value 
 * Write 3 into control field 
 * Write 0 into control field 
 */
#ifdef CONFIG_AR9100
#define ag7100_pll_shift(_mac)      (((_mac)->mac_unit) ? 22: 20)
#define ag7100_pll_offset(_mac)     \
    (((_mac)->mac_unit) ? AR9100_ETH_INT1_CLK : \
                          AR9100_ETH_INT0_CLK)
#else
#define ag7100_pll_shift(_mac)      (((_mac)->mac_unit) ? 19: 17)
#define ag7100_pll_offset(_mac)     \
    (((_mac)->mac_unit) ? AR7100_USB_PLL_GE1_OFFSET : \
                          AR7100_USB_PLL_GE0_OFFSET)
#endif
static void
ag7100_set_pll(ag7100_mac_t *mac, unsigned int pll)
{
#ifdef CONFIG_AR9100
#define ETH_PLL_CONFIG AR9100_ETH_PLL_CONFIG
#else
#define ETH_PLL_CONFIG AR7100_USB_PLL_CONFIG
#endif 
    uint32_t shift, reg, val;

    shift = ag7100_pll_shift(mac);
    reg   = ag7100_pll_offset(mac);

    val  = ar7100_reg_rd(ETH_PLL_CONFIG);
    val &= ~(3 << shift);
    val |=  (2 << shift);
    ar7100_reg_wr(ETH_PLL_CONFIG, val);
    udelay(100);

    ar7100_reg_wr(reg, pll);

    val |=  (3 << shift);
    ar7100_reg_wr(ETH_PLL_CONFIG, val);
    udelay(100);

    val &= ~(3 << shift);
    ar7100_reg_wr(ETH_PLL_CONFIG, val);
    udelay(100);

    printk(MODULE_NAME ": pll reg %#x: %#x  \n", reg, ar7100_reg_rd(reg));
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
#ifdef CONFIG_AG7100_ATHRS16_PHY
static int is_setup_done = 0;
#endif
static void
ag7100_set_mac_from_link(ag7100_mac_t *mac, ag7100_phy_speed_t speed, int fdx)
{
#ifdef CONFIG_AG7100_ATHRS26_PHY
    int change_flag = 0;

    if(mac->mac_speed !=  speed)
        change_flag = 1;

    if(change_flag)
    {
        athrs26_phy_off(mac);
        athrs26_mac_speed_set(mac, speed);
    }
#endif
#ifdef CONFIG_AG7100_ATHRS16_PHY
    if(!is_setup_done &&
            mac->mac_unit == 1 &&
            (mac->mac_speed !=  speed || mac->mac_fdx !=  fdx))
    {
        /* workaround for WAN port thru RGMII */
        athrs16_phy_mode_setup();
        is_setup_done = 1;
    }
#endif

    mac->mac_speed =  speed;
    mac->mac_fdx   =  fdx;

    ag7100_set_mii_ctrl_speed(mac, speed);
    ag7100_set_mac_duplex(mac, fdx);
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_3, fifo_3);
#ifndef CONFIG_AR9100
    ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_5, fifo_5);
#endif

    switch (speed)
    {
    case AG7100_PHY_SPEED_1000T:
#ifdef CONFIG_AR9100
        ag7100_reg_wr(mac, AG7100_MAC_FIFO_CFG_3, 0x780fff);
#endif
        ag7100_set_mac_if(mac, 1);
#ifdef CONFIG_AR9100
        if (mac->mac_unit == 0)
        { /* eth0 */
            ag7100_set_pll(mac, gige_pll);
        }
        else
        {
            ag7100_set_pll(mac, SW_PLL);
        }
#else
        ag7100_set_pll(mac, gige_pll);
#endif
        ag7100_reg_rmw_set(mac, AG7100_MAC_FIFO_CFG_5, (1 << 19));
        break;

    case AG7100_PHY_SPEED_100TX:
        ag7100_set_mac_if(mac, 0);
        ag7100_set_mac_speed(mac, 1);
#ifndef CONFIG_AR7100_EMULATION
#ifdef CONFIG_AR9100
        if (mac->mac_unit == 0)
        { /* eth0 */
            ag7100_set_pll(mac, 0x13000a44);
        }
        else
        {
            ag7100_set_pll(mac, SW_PLL);
        }
#else
        ag7100_set_pll(mac, 0x0001099);
#endif
#endif
        ag7100_reg_rmw_clear(mac, AG7100_MAC_FIFO_CFG_5, (1 << 19));
        break;

    case AG7100_PHY_SPEED_10T:
        ag7100_set_mac_if(mac, 0);
        ag7100_set_mac_speed(mac, 0);
#ifdef CONFIG_AR9100
        if (mac->mac_unit == 0)
        { /* eth0 */
            ag7100_set_pll(mac, 0x00441099);
        }
        else
        {
            ag7100_set_pll(mac, SW_PLL);
        }
#else
        ag7100_set_pll(mac, 0x00991099);
#endif
        ag7100_reg_rmw_clear(mac, AG7100_MAC_FIFO_CFG_5, (1 << 19));
        break;

    default:
        assert(0);
    }

#ifdef CONFIG_AG7100_ATHRS26_PHY
    if(change_flag) 
        athrs26_phy_on(mac);
#endif

    //printk(MODULE_NAME ": cfg_1: %#x\n", ag7100_reg_rd(mac, AG7100_MAC_FIFO_CFG_1));
    //printk(MODULE_NAME ": cfg_2: %#x\n", ag7100_reg_rd(mac, AG7100_MAC_FIFO_CFG_2));
    //printk(MODULE_NAME ": cfg_3: %#x\n", ag7100_reg_rd(mac, AG7100_MAC_FIFO_CFG_3));
    //printk(MODULE_NAME ": cfg_4: %#x\n", ag7100_reg_rd(mac, AG7100_MAC_FIFO_CFG_4));
    //printk(MODULE_NAME ": cfg_5: %#x\n", ag7100_reg_rd(mac, AG7100_MAC_FIFO_CFG_5));
}

/*
 * phy link state management
 */
static void
ag7100_check_link(unsigned long data)
{
    ag7100_mac_t       *mac     = (ag7100_mac_t *)data;
    struct net_device  *dev     = mac->mac_dev;
    int                 carrier = netif_carrier_ok(dev); /* ag7100_mii_read(0, 1, 1) & 0x04; */
    int                 fdx, phy_up;
    ag7100_phy_speed_t  speed;
    int                 rc;

    /* The vitesse switch uses an indirect method to communicate phy status
    * so it is best to limit the number of calls to what is necessary.
    * However a single call returns all three pieces of status information.
    *
    * This is a trivial change to the other PHYs ergo this change.
    *
    */

    rc = ag7100_get_link_status(mac->mac_unit, &phy_up, &fdx, &speed);
    if (rc < 0) {
        goto done;
    }

    if (!phy_up)
    {
        if (carrier) {
            printk(MODULE_NAME ": unit %d: phy not up carrier %d\n", mac->mac_unit, carrier);
            netif_carrier_off(dev);
            netif_stop_queue(dev);
        } else {
            //printk(MODULE_NAME ": unit %d: phy not up carrier %d\n", mac->mac_unit, carrier);
        }
        goto done;
    }

    /*
    * phy is up. Either nothing changed or phy settings changed while we
    * were sleeping.
    */

    if ((fdx < 0) || (speed < 0)) {
        printk(MODULE_NAME ": phy not connected?\n");
        return;
    }

    if (carrier && (speed == mac->mac_speed) && (fdx == mac->mac_fdx)) {
        goto done;
    }

    printk(MODULE_NAME ": unit %d phy is up...", mac->mac_unit);
    printk("%s %s %s duplex\n", mii_str[mac->mac_unit][mii_if(mac)],
        spd_str[speed], dup_str[fdx]);

    ag7100_set_mac_from_link(mac, speed, fdx);

    printk(MODULE_NAME ": done cfg2 %#x ifctl %#x miictrl %#x \n",
        ag7100_reg_rd(mac, AG7100_MAC_CFG2),
        ag7100_reg_rd(mac, AG7100_MAC_IFCTL),
        ar7100_reg_rd(mii_reg(mac)));
    /*
    * in business
    */
    netif_carrier_on(dev);
    netif_start_queue(dev);
done:
    mod_timer(&mac->mac_phy_timer, jiffies + AG7100_PHY_POLL_SECONDS*HZ);
}

static void
ag7100_choose_phy(uint32_t phy_addr)
{
#ifdef CONFIG_AR7100_EMULATION
    if (phy_addr == 0x10)
    {
        ar7100_reg_rmw_set(AR7100_MII0_CTRL, (1 << 6));
    }
    else
    {
        ar7100_reg_rmw_clear(AR7100_MII0_CTRL, (1 << 6));
    }
#endif
}

uint16_t
ag7100_mii_read(int unit, uint32_t phy_addr, uint8_t reg)
{
    ag7100_mac_t *mac   = ag7100_unit2mac(0);
    uint16_t      addr  = (phy_addr << AG7100_ADDR_SHIFT) | reg, val;
    volatile int           rddata;
    uint16_t      ii = 0x1000;

    ag7100_choose_phy(phy_addr);

    ag7100_reg_wr(mac, AG7100_MII_MGMT_CMD, 0x0);
    ag7100_reg_wr(mac, AG7100_MII_MGMT_ADDRESS, addr);
    ag7100_reg_wr(mac, AG7100_MII_MGMT_CMD, AG7100_MGMT_CMD_READ);

    do
    {
        udelay(5);
        rddata = ag7100_reg_rd(mac, AG7100_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    val = ag7100_reg_rd(mac, AG7100_MII_MGMT_STATUS);
    ag7100_reg_wr(mac, AG7100_MII_MGMT_CMD, 0x0);

    return val;
}

void
ag7100_mii_write(int unit, uint32_t phy_addr, uint8_t reg, uint16_t data)
{
    ag7100_mac_t *mac   = ag7100_unit2mac(0);
    uint16_t      addr  = (phy_addr << AG7100_ADDR_SHIFT) | reg;
    volatile int rddata;
    uint16_t      ii = 0x1000;

    ag7100_choose_phy(phy_addr);

    ag7100_reg_wr(mac, AG7100_MII_MGMT_ADDRESS, addr);
    ag7100_reg_wr(mac, AG7100_MII_MGMT_CTRL, data);

    do
    {
        rddata = ag7100_reg_rd(mac, AG7100_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);
}

/* ******************************
 * 
 * Code under test - do not use
 *
 * ******************************
 */

static ag7100_desc_t *
ag7100_get_tx_ds(ag7100_mac_t *mac, int *len, unsigned char **start)
{
    ag7100_desc_t      *ds;
    int                len_this_ds;
    ag7100_ring_t      *r   = &mac->mac_txring;

    /* force extra pkt if remainder less than 4 bytes */
    if (*len > tx_len_per_ds)
        if (*len < (tx_len_per_ds + 4))
            len_this_ds = tx_len_per_ds - 4;
        else
            len_this_ds = tx_len_per_ds;
    else
        len_this_ds    = *len;

    ds = &r->ring_desc[r->ring_head];

    ag7100_trc_new(ds,"ds addr");
    ag7100_trc_new(ds,"ds len");
#ifdef CONFIG_AR9100
    if(ag7100_tx_owned_by_dma(ds))
        ag7100_dma_reset(mac);
#else
    assert(!ag7100_tx_owned_by_dma(ds));
#endif

    ds->pkt_size       = len_this_ds;
    ds->pkt_start_addr = virt_to_phys(*start);
    ds->more           = 1;

    *len   -= len_this_ds;
    *start += len_this_ds;

    r->ring_head = ag7100_ring_incr(r, r->ring_head);

    return ds;
}

#if defined(CONFIG_AG7100_ATHRS26_PHY)
int
#else
static int
#endif
ag7100_hard_start(struct sk_buff *skb, struct net_device *dev)
{
    ag7100_mac_t       *mac = (ag7100_mac_t *)netdev_priv(dev);
    ag7100_ring_t      *r   = &mac->mac_txring;
    ag7100_buffer_t    *bp;
    ag7100_desc_t      *ds, *fds;
    unsigned char      *start;
    int                len;
    int                nds_this_pkt;

#ifdef VSC73XX_DEBUG
    {
        static int vsc73xx_dbg;
        if (vsc73xx_dbg == 0) {
            vsc73xx_get_link_status_dbg();
            vsc73xx_dbg = 1;
        }
        vsc73xx_dbg = (vsc73xx_dbg + 1) % 10;
    }
#endif

    if (unlikely((skb->len <= 0) || (skb->len > AG7100_TX_MAX_DS_LEN)))
    {
        printk(MODULE_NAME ": bad skb, len %d\n", skb->len);
        goto dropit;
    }
    if (unlikely((skb->len > 1536)))
        printk("larger packet %d\n", skb->len);

    ag7100_unhang(mac);

    ag7100_trc_new(r->ring_head,"hard-stop hd");
    ag7100_trc_new(r->ring_tail,"hard-stop tl");

    ag7100_trc_new(skb->len,    "len this pkt");
    ag7100_trc_new(skb->data,   "ptr 2 pkt");

    dma_cache_wback((unsigned long)skb->data, skb->len);

    bp          = &r->ring_buffer[r->ring_head];
    bp->buf_pkt = skb;
    len         = skb->len;
    start       = skb->data;

    assert(len>4);

    nds_this_pkt = 1;
    fds = ds = ag7100_get_tx_ds(mac, &len, &start);

    while (len>0)
    {
        ds = ag7100_get_tx_ds(mac, &len, &start);
        nds_this_pkt++;
        ag7100_tx_give_to_dma(ds);
    }

    ds->more        = 0;
    ag7100_tx_give_to_dma(fds);

    bp->buf_lastds  = ds;
    bp->buf_nds     = nds_this_pkt;

    ag7100_trc_new(ds,"last ds");
    ag7100_trc_new(nds_this_pkt,"nmbr ds for this pkt");

    wmb();

    mac->net_tx_packets ++;
    mac->net_tx_bytes += skb->len;

    ag7100_trc(ag7100_reg_rd(mac, AG7100_DMA_TX_CTRL),"dma idle");

    ag7100_tx_start(mac);

    if (ag7100_tx_reap_thresh(mac)) {
        if (unlikely(ag7100_tx_ring_full(mac)))
            netif_stop_queue(mac->mac_dev);

        unsigned long flags;
        spin_lock_irqsave(&mac->mac_lock, flags);
        ag7100_intr_enable_tx(mac);
        spin_unlock_irqrestore(&mac->mac_lock, flags);
    }

    dev->trans_start = jiffies;

    return NETDEV_TX_OK;

dropit:
    printk(MODULE_NAME ": dropping skb\n");
    kfree_skb(skb);
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
ag7100_intr(int cpl, void *dev_id)
{
    struct net_device *dev  = (struct net_device *)dev_id;
    ag7100_mac_t      *mac  = (ag7100_mac_t *)netdev_priv(dev);
    int   isr, imr, handled = 0;

    isr   = ag7100_get_isr(mac);
    imr   = ag7100_reg_rd(mac, AG7100_DMA_INTR_MASK);

    ag7100_trc(isr,"isr");
    ag7100_trc(imr,"imr");

    assert(isr == (isr & imr));

    if (likely(isr & (AG7100_INTR_RX | AG7100_INTR_RX_OVF | AG7100_INTR_TX)))
    {
        handled = 1;
        if (isr & (AG7100_INTR_RX | AG7100_INTR_RX_OVF))
            ag7100_intr_disable_recv(mac);

        if (isr & AG7100_INTR_TX)
            ag7100_intr_disable_tx(mac);

	napi_schedule(&mac->napi); // Done on TX also to reap buffers
    }
    if (unlikely(isr & AG7100_INTR_RX_BUS_ERROR))
    {
        assert(0);
        handled = 1;
        ag7100_intr_ack_rxbe(mac);
    }
    if (unlikely(isr & AG7100_INTR_TX_BUS_ERROR))
    {
        assert(0);
        handled = 1;
        ag7100_intr_ack_txbe(mac);
    }

    if (!handled)
    {
        static int count = 0;
        printk(MODULE_NAME ": unhandled intr isr:%#x imr:%#x\n", isr, imr);
        /* cliff - 2010.09.11 - I saw assert failures, so added this
           count to try and help.  If this "fixes" the problem, then
           we can remove the assertion completely.  If this is a real
           bug that causes infinite interrupts, then this will still
           fail/reboot somewhat quickly. */
        if (count++ > 20000)
            assert(0);
    }

    return IRQ_HANDLED;
}

 /*
  * Rx and Tx DMA hangs and goes to an invalid state in HOWL boards 
  * when the link partner is forced to 10/100 Mode.By resetting the MAC
  * we are able to recover from this state.This is a software  WAR and
  * will be removed once we have a hardware fix. 
  */

#ifdef CONFIG_AR9100

void ag7100_dma_reset(ag7100_mac_t *mac)
{
    uint32_t mask;

    if(mac->mac_unit)
        mask = AR7100_RESET_GE1_MAC;
    else
        mask = AR7100_RESET_GE0_MAC;

    ar7100_reg_rmw_set(AR7100_RESET, mask);
    mdelay(100);
    ar7100_reg_rmw_clear(AR7100_RESET, mask);
    mdelay(100);

    ag7100_intr_disable_recv(mac);
    schedule_work(&mac->mac_tx_timeout);
}

#endif

static int
ag7100_poll(struct napi_struct *napi, int budget)
{
    ag7100_mac_t *mac = container_of(napi, ag7100_mac_t, napi);
    int work_done;
    unsigned long flags;
    struct net_device *dev = mac->mac_dev;

    work_done = ag7100_recv_packets(dev, mac, budget);

    if (work_done != budget) {
        napi_complete(napi);
        spin_lock_irqsave(&mac->mac_lock, flags);
        ag7100_intr_enable_recv(mac);
        spin_unlock_irqrestore(&mac->mac_lock, flags);
    }

    if (ag7100_tx_reap_thresh(mac))
	ag7100_tx_reap(mac);

    return work_done;
}

static int
ag7100_recv_packets(struct net_device *dev, ag7100_mac_t *mac,
		    int quota)
{
    ag7100_ring_t       *r     = &mac->mac_rxring;
    ag7100_desc_t       *ds;
    ag7100_buffer_t     *bp;
    struct sk_buff      *skb;
    int head = r->ring_head, tail = r->ring_tail;
    int len, status, iquota = quota, more_pkts;

    ag7100_trc(iquota,"iquota");

process_pkts:
    ag7100_trc(status,"status");
    /*
    * Flush the DDR FIFOs for our gmac
    */
    ar7100_flush_ge(mac->mac_unit);

    assert(quota > 0); /* WCL */

    while(quota)
    {
        struct sk_buff *new_skb;
        ag7100_intr_ack_rx(mac);

        ds    = &r->ring_desc[head];

        ag7100_trc(head,"hd");
        ag7100_trc(ds,  "ds");

        if (ag7100_rx_owned_by_dma(ds))
            break;

        new_skb = ag7100_buffer_alloc();
        if (!new_skb)
            break;
        bp                  = &r->ring_buffer[head];
        len                 = ds->pkt_size;
        skb                 = bp->buf_pkt;
        assert(skb);
        skb_put(skb, len - ETHERNET_FCS_SIZE);

        /*
          // if one really wants packets to be properly aligned, this code works, but is very slow.
          memmove(skb->data + 2, skb->data, skb->len);
          skb_pull(skb, 2);
          skb_put(skb, 2);
        */


#if defined(CONFIG_AG7100_ATHRS26_PHY) && defined(HEADER_EN)
#error
#endif
        mac->net_rx_packets ++;
        mac->net_rx_bytes += skb->len;
        skb->protocol       = eth_type_trans(skb, dev);
        skb->dev            = dev;
        dev->last_rx        = jiffies;

        quota--;

        netif_receive_skb(skb);


        dma_cache_inv((unsigned long)new_skb->data, AG7100_RX_BUF_SIZE);
        ds->pkt_start_addr  = virt_to_phys(new_skb->data);
        bp->buf_pkt         = new_skb;

        ag7100_rx_give_to_dma(ds);

        tail = ag7100_ring_incr(r, tail);
        head = ag7100_ring_incr(r, head);
    }

    r->ring_head = head;
    r->ring_tail = tail;

    /*
    * let's see what changed while we were slogging.
    * ack Rx in the loop above is no flush version. It will get flushed now.
    */
    status       =  ag7100_reg_rd(mac, AG7100_DMA_RX_STATUS);
    more_pkts    =  (status & AG7100_RX_STATUS_PKT_RCVD);

    ag7100_trc(more_pkts,"more_pkts");

    if (!more_pkts) goto done;
    /*
    * more pkts arrived; if we have quota left, get rolling again
    */
    if (quota)      goto process_pkts;

done:

    /*
    * if stopped, restart h/w
    */
    if (unlikely(status & AG7100_RX_STATUS_OVF))
    {
        mac->net_rx_over_errors ++;
        ag7100_intr_ack_rxovf(mac);
        ag7100_rx_start(mac);
    }

    return (iquota - quota);
}

static struct sk_buff *
ag7100_buffer_alloc(void)
{
    struct sk_buff *skb;

    skb = dev_alloc_skb(AG7100_RX_BUF_ALLOC_SIZE + ((-AG7100_RX_BUF_ALLOC_SIZE) & (ARCH_DMA_MINALIGN-1)));
    if (unlikely(!skb))
        return NULL;
    skb_reserve(skb, AG7100_RX_RESERVE);

    return skb;
}

static void
ag7100_buffer_free(struct sk_buff *skb)
{
    if (in_irq())
        dev_kfree_skb_irq(skb);
    else
        dev_kfree_skb(skb);
}

static void
print_desc(ag7100_desc_t *ds, const char *tag)
{
    printk("%s:\n", tag);
    if (!ds) {
        printk(MODULE_NAME "%s NULL\n", tag);
        return;
    }
    printk("\tpkt_start_addr: %x\n", ds->pkt_start_addr);
    printk("\tis_empty      : %d\n", ds->is_empty);
    printk("\tres1          : %d\n", ds->res1);
    printk("\tmore          : %d\n", ds->more);
    printk("\tres2          : %d\n", ds->res2);
    printk("\tftpp_override : %d\n", ds->ftpp_override);
    printk("\tres3          : %d\n", ds->res3);
    printk("\tpkt_size      : %d\n", ds->pkt_size);
}

/* 
 * Reap from tail till the head or whenever we encounter an unxmited packet.
 */
static int
ag7100_tx_reap(ag7100_mac_t *mac)
{
    ag7100_ring_t   *r     = &mac->mac_txring;
    int              head, tail, reaped = 0, i;
    ag7100_desc_t   *ds;
    ag7100_buffer_t *bf;
    unsigned long flags;

    // need locking because this is called from both hard_start_xmit
    // and from ag7100_poll
    spin_lock_irqsave(&mac->mac_lock, flags);

    head = r->ring_head;
    tail = r->ring_tail;
    ag7100_trc_new(head,"hd");
    ag7100_trc_new(tail,"tl");

    while(tail != head)
    {
        ds   = &r->ring_desc[tail];

        ag7100_trc_new(ds,"ds");

        if(ag7100_tx_owned_by_dma(ds))
            break;

        bf      = &r->ring_buffer[tail];
        if (!bf->buf_pkt) {
            printk(MODULE_NAME ": head %d, tail %d, nelem %x, reaped %d\n", head, tail, r->ring_nelem, reaped);
            print_desc(ds, "ds");
            printk(MODULE_NAME ": bf->buf_nds: %d\n", bf->buf_nds);
            print_desc(bf->buf_lastds, "bf->buf_lastds");
        }
        assert(bf->buf_pkt);

        ag7100_trc_new(bf->buf_lastds,"lastds");

        if(ag7100_tx_owned_by_dma(bf->buf_lastds))
            break;

        for(i = 0; i < bf->buf_nds; i++)
        {
            ag7100_intr_ack_tx(mac);
            tail = ag7100_ring_incr(r, tail);
        }

        ag7100_buffer_free(bf->buf_pkt);
        bf->buf_pkt = NULL;

        reaped ++;
    }

    r->ring_tail = tail;

    if (ag7100_tx_reap_thresh(mac))
        ag7100_intr_enable_tx(mac);

    if (netif_queue_stopped(mac->mac_dev) &&
        netif_carrier_ok(mac->mac_dev)) {
        if (ag7100_tx_ring_full(mac)) {
            ag7100_intr_enable_tx(mac);
        } else {
            netif_wake_queue(mac->mac_dev);
        }
    }

    spin_unlock_irqrestore(&mac->mac_lock, flags);

    return reaped;
}

/*
 * allocate and init rings, descriptors etc.
 */
static int
ag7100_tx_alloc(ag7100_mac_t *mac)
{
    ag7100_ring_t *r = &mac->mac_txring;
    ag7100_desc_t *ds;
    int i, next;

    if (ag7100_ring_alloc(r, AG7100_TX_DESC_CNT))
        return 1;

    ag7100_trc(r->ring_desc,"ring_desc");

    ds = r->ring_desc;
    for(i = 0; i < r->ring_nelem; i++ )
    {
        ag7100_trc_new(ds,"tx alloc ds");
        next                =   (i == (r->ring_nelem - 1)) ? 0 : (i + 1);
        ds[i].next_desc     =   ag7100_desc_dma_addr(r, &ds[next]);
        ag7100_tx_own(&ds[i]);
    }

    return 0;
}

static int
ag7100_rx_alloc(ag7100_mac_t *mac)
{
    ag7100_ring_t *r  = &mac->mac_rxring;
    ag7100_desc_t *ds;
    int i, next, tail = r->ring_tail;
    ag7100_buffer_t *bf;

    if (ag7100_ring_alloc(r, AG7100_RX_DESC_CNT))
        return 1;

    ag7100_trc(r->ring_desc,"ring_desc");

    ds = r->ring_desc;
    for(i = 0; i < r->ring_nelem; i++ )
    {
        next                =   (i == (r->ring_nelem - 1)) ? 0 : (i + 1);
        ds[i].next_desc     =   ag7100_desc_dma_addr(r, &ds[next]);
    }

    for (i = 0; i < AG7100_RX_DESC_CNT; i++)
    {
        bf                  = &r->ring_buffer[tail];
        ds                  = &r->ring_desc[tail];

        bf->buf_pkt         = ag7100_buffer_alloc();
        if (!bf->buf_pkt) 
            goto error;

        dma_cache_inv((unsigned long)bf->buf_pkt->data, AG7100_RX_BUF_SIZE);
        ds->pkt_start_addr  = virt_to_phys(bf->buf_pkt->data);

        ag7100_rx_give_to_dma(ds);
        tail = ag7100_ring_incr(r, tail);
    }

    return 0;
error:
    printk(MODULE_NAME ": unable to allocate rx\n");
    ag7100_rx_free(mac);
    return 1;
}

static void
ag7100_tx_free(ag7100_mac_t *mac)
{
    ag7100_ring_release(mac, &mac->mac_txring);
    ag7100_ring_free(&mac->mac_txring);
}

static void
ag7100_rx_free(ag7100_mac_t *mac)
{
    ag7100_ring_release(mac, &mac->mac_rxring);
    ag7100_ring_free(&mac->mac_rxring);
}

static int
ag7100_ring_alloc(ag7100_ring_t *r, int count)
{
    int desc_alloc_size, buf_alloc_size;

    desc_alloc_size = sizeof(ag7100_desc_t)   * count;
    buf_alloc_size  = sizeof(ag7100_buffer_t) * count;

    memset(r, 0, sizeof(ag7100_ring_t));

    r->ring_buffer = (ag7100_buffer_t *)kmalloc(buf_alloc_size, GFP_KERNEL);
    //printk("%s Allocated %d at 0x%lx\n",__func__,buf_alloc_size,(unsigned long) r->ring_buffer);
    if (!r->ring_buffer)
    {
        printk(MODULE_NAME ": unable to allocate buffers\n");
        return 1;
    }

    r->ring_desc  =  (ag7100_desc_t *)dma_alloc_coherent(NULL, 
        desc_alloc_size,
        &r->ring_desc_dma, 
        GFP_DMA);
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
ag7100_ring_release(ag7100_mac_t *mac, ag7100_ring_t  *r)
{
    int i;

    for(i = 0; i < r->ring_nelem; i++)
        if (r->ring_buffer[i].buf_pkt)
            ag7100_buffer_free(r->ring_buffer[i].buf_pkt);
}

static void
ag7100_ring_free(ag7100_ring_t *r)
{
    dma_free_coherent(NULL, sizeof(ag7100_desc_t)*r->ring_nelem, r->ring_desc,
        r->ring_desc_dma);
    kfree(r->ring_buffer);
    printk("%s Freeing at 0x%lx\n",__func__,(unsigned long) r->ring_buffer);
}

/*
 * Error timers
 */

static void
ag7100_tx_timeout(struct net_device *dev)
{
    ag7100_mac_t *mac = (ag7100_mac_t *)netdev_priv(dev);
    ag7100_trc(dev,"dev");
    printk("%s\n",__func__);
    /* 
    * Do the reset outside of interrupt context 
    */
    schedule_work(&mac->mac_tx_timeout);
}

static void
ag7100_tx_timeout_task(struct work_struct *work)
{
    ag7100_mac_t *mac = (ag7100_mac_t *)container_of(work, ag7100_mac_t, mac_tx_timeout);
    ag7100_trc(mac,"mac");
    ag7100_stop(mac->mac_dev);
    ag7100_open(mac->mac_dev);
}

static void
ag7100_get_default_macaddr(ag7100_mac_t *mac, u8 *mac_addr)
{
    /* Use MAC address stored in Flash */
    
    u8 *eep_mac_addr = (u8 *) ((mac->mac_unit) ? AR7100_EEPROM_GE1_MAC_ADDR:
                               AR7100_EEPROM_GE0_MAC_ADDR);

    printk(MODULE_NAME "CHH: Mac address for unit %d\n",mac->mac_unit);
    printk(MODULE_NAME "CHH: %02x:%02x:%02x:%02x:%02x:%02x \n",
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
    }
    else
    {
        /* Use Default address at top of range */
        mac_addr[0] = 0x00;
        mac_addr[1] = 0x03;
        mac_addr[2] = 0x7F;
        mac_addr[3] = 0xFF;
        mac_addr[4] = 0xFF;
        mac_addr[5] = 0xFF - mac->mac_unit;
    }
}

static struct net_device_stats 
    *ag7100_get_stats(struct net_device *dev)
{
    ag7100_mac_t *mac = netdev_priv(dev);
    struct Qdisc *sch;
    int i;

    sch = rcu_dereference(dev->qdisc);
    mac->mac_net_stats.tx_dropped = sch->qstats.drops;

    i = ag7100_get_rx_count(mac) - mac->net_rx_packets;
    if (i<0)
        i=0;

    mac->mac_net_stats.rx_missed_errors = i;

    return &mac->mac_net_stats;
}

static void
ag7100_vet_tx_len_per_pkt(unsigned int *len)
{
    unsigned int l;

    /* make it into words */
    l = *len & ~3;

    /* 
    * Not too small 
    */
    if (l < AG7100_TX_MIN_DS_LEN)
        l = AG7100_TX_MIN_DS_LEN;
    else if (l > AG7100_TX_FIFO_LEN/2) {
        /* Avoid len where we know we will deadlock, that
         * is the range between fif_len/2 and the MTU size
         */
        if (l < AG7100_TX_MTU_LEN)
            l = AG7100_TX_MTU_LEN;
        else if (l > AG7100_TX_MAX_DS_LEN)
            l = AG7100_TX_MAX_DS_LEN;
    }
    *len = l;
}

/*
 * All allocations (except irq and rings).
 */

static const struct net_device_ops ag7100_netdev_ops = {
	.ndo_open		= ag7100_open,
	.ndo_stop		= ag7100_stop,
	.ndo_start_xmit		= ag7100_hard_start,
	.ndo_do_ioctl		= ag7100_do_ioctl,
	.ndo_tx_timeout		= ag7100_tx_timeout,
	.ndo_get_stats		= ag7100_get_stats,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
};

static int __init
ag7100_init(void)
{
    int i;
    struct net_device *dev;
    ag7100_mac_t      *mac;
    uint32_t mask;

    if (!soc_is_ar71xx())
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
        tx_len_per_ds = CONFIG_AG7100_LEN_PER_TX_DS;

    ag7100_vet_tx_len_per_pkt( &tx_len_per_ds);

    /* 
    * Compute the number of descriptors for an MTU 
    */
#ifndef CONFIG_AR9100
    tx_max_desc_per_ds_pkt = AG7100_TX_MAX_DS_LEN / tx_len_per_ds;
    if (AG7100_TX_MAX_DS_LEN % tx_len_per_ds) tx_max_desc_per_ds_pkt++;
#else
    tx_max_desc_per_ds_pkt =1;
#endif

    printk(MODULE_NAME ": len per seg %d, segs per packet %d\n", tx_len_per_ds, tx_max_desc_per_ds_pkt);
    printk(MODULE_NAME ": Max tx/rx desc count %d/%d\n", AG7100_TX_DESC_CNT, AG7100_RX_DESC_CNT);

    /* 
    * Let hydra know how much to put into the fifo in words (for tx) 
    */
    if (0 == fifo_3)
        fifo_3 = 0x000001ff | ((AG7100_TX_FIFO_LEN-tx_len_per_ds)/4)<<16;

    printk(MODULE_NAME ": fifo cfg 3 %08x\n", fifo_3);

    /* 
    ** Do the rest of the initializations 
    */

    for(i = 0; i < AG7100_NMACS; i++)
    {

        dev = alloc_etherdev(sizeof(ag7100_mac_t));
        if (!dev)
        {
            printk(MODULE_NAME ": unable to allocate etherdev\n");
            return 1;
        }

	dev->netdev_ops = &ag7100_netdev_ops;

	mac = (ag7100_mac_t *)netdev_priv(dev);
        mac->mac_dev         =  dev;
        mac->mac_unit        =  i;
        mac->mac_base        =  ag7100_mac_base(i);
        mac->mac_irq         =  ag7100_mac_irq(i);
        ag7100_macs[i]       =  mac;
        spin_lock_init(&mac->mac_lock);
	netif_napi_add(dev, &mac->napi, ag7100_poll, AG7100_NAPI_WEIGHT);

        /*
        * watchdog task
        */
        INIT_WORK(&mac->mac_tx_timeout, ag7100_tx_timeout_task);


        ag7100_get_default_macaddr(mac, dev->dev_addr);

        if (register_netdev(dev))
        {
            printk(MODULE_NAME ": register netdev failed\n");
            goto failed;
        }
        ag7100_reg_rmw_set(mac, AG7100_MAC_CFG1, AG7100_MAC_CFG1_SOFT_RST);
        udelay(20);
        mask = ag7100_reset_mask(mac->mac_unit);

        /*
        * put into reset, hold, pull out.
        */
        ar7100_reg_rmw_set(AR7100_RESET, mask);
        mdelay(100);
        ar7100_reg_rmw_clear(AR7100_RESET, mask);
        mdelay(100);
    }

    ag7100_trc_init();

#if defined(CONFIG_AG7100_ATHRS26_PHY)
    athrs26_reg_dev(ag7100_macs);
#endif

#ifdef CONFIG_PROC_FS
    {
        struct proc_dir_entry *ag7100_dir;
        struct proc_dir_entry *reg_e;
        struct proc_dir_entry *phy_e;
        struct proc_dir_entry *txdesc_e;
        if (!(ag7100_dir = proc_mkdir("ag7100", init_net.proc_net)) ||
            !(reg_e = create_proc_entry("regdump", 0444, ag7100_dir)) ||
            !(phy_e = create_proc_entry("phy", 0444, ag7100_dir)) ||
            !(txdesc_e = create_proc_entry("txdesc", 0444, ag7100_dir))) {
            printk(KERN_ERR "ag7100: cannot create proc entry.\n");
            return -ENOENT;
        }
        reg_e->proc_fops = &ag7100_regdump_operations;
        phy_e->proc_fops = &ag7100_phy_operations;
        txdesc_e->proc_fops = &ag7100_txdesc_dump_operations;
    }
#endif /* CONFIG_PROC_FS */

    return 0;

failed:
    for(i = 0; i < AG7100_NMACS; i++)
    {
        if (!ag7100_macs[i]) 
            continue;
        if (ag7100_macs[i]->mac_dev)
            free_netdev(ag7100_macs[i]->mac_dev);
	ag7100_macs[i] = NULL;
    }
    return 1;
}

static void __exit
ag7100_cleanup(void)
{
    int i;

    for(i = 0; i < AG7100_NMACS; i++)
    {
        unregister_netdev(ag7100_macs[i]->mac_dev);
        free_netdev(ag7100_macs[i]->mac_dev);
	ag7100_macs[i] = NULL;
    }
    printk(MODULE_NAME ": cleanup done\n");
}

void ag7100_labeller(struct seq_file *m, uint32_t address, uint32_t mask)
{
    switch (address & mask) {
    case 0x0: seq_printf(m, "Mac Configuration 1\n"); break;
    case 0x4: seq_printf(m, "Mac Configuration 2\n"); break;
    case 0x8: seq_printf(m, "IPG/IFG\n"); break;
    case 0xC: seq_printf(m, "Half-Duplex\n"); break;
    case 0x10: seq_printf(m, "Maximum Frame Length\n"); break;
    case 0x14: seq_printf(m, "MII Configuration\n"); break;
    case 0x18: seq_printf(m, "MII Command\n"); break;
    case 0x1C: seq_printf(m, "MII Address\n"); break;
    case 0x20: seq_printf(m, "AG7100_MAC_MII_MGMT_CFG (control)\n"); break;
    case 0x24: seq_printf(m, "AG7100_MII_MGMT_CMD (status)\n"); break;
    case 0x28: seq_printf(m, "AG7100_MII_MGMT_ADDRESS\n"); break;
    case 0x2c: seq_printf(m, "AG7100_MII_MGMT_CTRL\n"); break;
    case 0x30: seq_printf(m, "AG7100_MII_MGMT_STATUS\n"); break;
    case 0x34: seq_printf(m, "AG7100_MII_MGMT_IND\n"); break;
    case 0x38: seq_printf(m, "AG7100_MAC_IFCTL\n"); break;
    case 0x40: seq_printf(m, "AG7100_GE_MAC_ADDR1\n"); break;
    case 0x44: seq_printf(m, "AG7100_GE_MAC_ADDR2\n"); break;
    case 0x48: seq_printf(m, "AG7100_MAC_FIFO_CFG_0\n"); break;
    case 0x4C: seq_printf(m, "AG7100_MAC_FIFO_CFG_1\n"); break;
    case 0x50: seq_printf(m, "AG7100_MAC_FIFO_CFG_2\n"); break;
    case 0x54: seq_printf(m, "AG7100_MAC_FIFO_CFG_3 (Transmit FIFO)\n"); break;
    case 0x58: seq_printf(m, "AG7100_MAC_FIFO_CFG_4 (Dropped Frames)\n"); break;
    case 0x5c: seq_printf(m, "AG7100_MAC_FIFO_CFG_5 (Dropped Frames)\n"); break;
    case 0x60: seq_printf(m, "AG7100_MAC_FIFO_RAM_0 (tx related?)\n"); break;
    case 0x64: seq_printf(m, "AG7100_MAC_FIFO_RAM_1\n"); break;
    case 0x68: seq_printf(m, "AG7100_MAC_FIFO_RAM_2\n"); break;
    case 0x6c: seq_printf(m, "AG7100_MAC_FIFO_RAM_3\n"); break;
    case 0x70: seq_printf(m, "AG7100_MAC_FIFO_RAM_4 (rx related?)\n"); break;
    case 0x74: seq_printf(m, "AG7100_MAC_FIFO_RAM_5\n"); break;
    case 0x78: seq_printf(m, "AG7100_MAC_FIFO_RAM_6\n"); break;
    case 0x7c: seq_printf(m, "AG7100_MAC_FIFO_RAM_7\n"); break;
    case 0x180: seq_printf(m, "AG7100_DMA_TX_CTRL\n"); break;
    case 0x184: seq_printf(m, "AG7100_DMA_TX_DESC\n"); break;
    case 0x188: seq_printf(m, "AG7100_DMA_TX_STATUS\n"); break;
    case 0x18c: seq_printf(m, "AG7100_DMA_RX_CTRL\n"); break;
    case 0x190: seq_printf(m, "AG7100_DMA_RX_DESC\n"); break;
    case 0x194: seq_printf(m, "AG7100_DMA_RX_STATUS\n"); break;
    case 0x198: seq_printf(m, "AG7100_DMA_INTR_MASK\n"); break;
    case 0x19c: seq_printf(m, "AG7100_DMA_INTR\n"); break;
    //case : seq_printf(m, "\n"); break;
    default: seq_printf(m, "\n"); break;
    }
}

/* set show_zeroes to 1 if you want to print zeroes */
/* limit is inclusive */
static void seq_dump_memory(struct seq_file *m, uint32_t base, uint32_t limit, uint32_t mask, uint16_t show_zeroes, void (*label_function)(struct seq_file *, uint32_t, uint32_t))
{
    uint32_t i;
    uint32_t value;
    int prevNull = 0;
    for (i = base; i <= base + limit; i+=4) {
        value = ar7100_reg_rd(i);
        if (value != 0 || show_zeroes)  {
            seq_printf(m, "%08x : %08x ", i, value);
            prevNull = 0;
            if (0 != label_function)
                (*label_function)(m, i, mask);
            else {
                seq_printf(m, "\n");
            }
        } else {
            if (prevNull == 0) {
                prevNull = 1;
                seq_printf(m, "%08x : *\n", i);
            }
        }
    }
}

static int ag7100_regdump_show(struct seq_file *m, void *v)
{
    uint32_t base;
    uint32_t i, j;

    seq_printf(m, "dumping registers base addresses %x %x\n\n", ag7100_macs[0] ? ag7100_macs[0]->mac_base : 0, ag7100_macs[1] ? ag7100_macs[1]->mac_base : 0);

    for (i = 0; i < 2; i++) {
        if (ag7100_macs[i]) {
            struct net_device *root_dev = ag7100_macs[i]->mac_dev;
            if (root_dev) {
                ag7100_mac_t *mac = netdev_priv(root_dev);
                ag7100_tx_reap(mac);
                seq_printf(m, "mac_hang_war triggered: %d\n\n", mac->mac_hang_war);
                seq_printf(m, "dumping ag7100_macs[%d]\n", i);
                base = ag7100_macs[i]->mac_base;
                /* You get ahb errors if you try to ar7100_reg_rd on random memory*/
                /* 0x19c is the highest memory location in ar7100.h */
                seq_dump_memory(m, base, 0x19c, 0x00FFFFFF, 1, &ag7100_labeller);
            } else
                continue;
        }
    }

    for (j = 0; j < 5; j++)
        for (i = 0; i < 30; i++)
            seq_printf(m, "mii phy %d reg %2d : 0x%04x\n", j, i, ag7100_mii_read(0, j, i));

    return 0;
}

#ifdef CONFIG_AG7100_ATHRS16_PHY
static int port_addr_to_physical(int port_addr)
{
    if (port_addr == 0x100)
        return -1;
    else if (port_addr == 0x200)
       return 2;
    else if (port_addr == 0x300)
       return 5;
    else if (port_addr == 0x400)
       return 4;
    else if (port_addr == 0x500)
       return 3;
    else if (port_addr == 0x600)
       return 1;
    return -1;
}
#endif

static int ag7100_phy_show(struct seq_file *m, void *v)
{
    ag7100_mac_t *mac;
    int carrier_ok;
    int i;
#ifdef CONFIG_AG7100_ATHRS16_PHY
    int base;
#endif

    seq_printf(m, "<phy>\n");
    for (i = 0; i < 2; i++) {
        struct net_device *root_dev;
        if (ag7100_macs[i])
           root_dev = ag7100_macs[i]->mac_dev;
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

#ifdef CONFIG_AG7100_ATHRS16_PHY
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

static int ag7100_txdesc_show(struct seq_file *m, void *v)
{
    int i, j;
    for (i = 0; i < 2; i++) {
        struct net_device *root_dev;
        if (ag7100_macs[i])
           root_dev = ag7100_macs[i]->mac_dev;
        else  {
            continue;
        }
        if (root_dev) {
            ag7100_mac_t *mac = netdev_priv(root_dev);
            ag7100_ring_t *txr = &mac->mac_txring;
            seq_printf(m, "MAC %d\n", i);
            seq_printf(m, "tx head: %d tail %d num_descs %d\n", txr->ring_head, txr->ring_tail, AG7100_TX_DESC_CNT);
            seq_printf(m, "unused buffers %d reap threshold %d\n", ag7100_ndesc_unused(mac, txr), AG7100_TX_REAP_THRESH);
	    seq_printf(m, "tx_ring_full %d\b", mac->tx_ring_full);
            seq_printf(m, "\n");

            for (j = 0; j< (AG7100_TX_DESC_CNT); j++) {
                ag7100_desc_t *ds = &txr->ring_desc[j];
                seq_printf(m, "desc[%d-%03d] %d %08x :: %08x %08x %08x%s%s\n",
                        i, j, ag7100_tx_owned_by_dma(ds), (uint32_t)ds,
                        ds->pkt_start_addr, *(((uint32_t *)ds)+1), ds->next_desc,
                        (j == txr->ring_head) ? " head" : "",
                        (j == txr->ring_tail) ? " tail" : "");
            }
        }
    }
    return 0;
}

module_init(ag7100_init);
module_exit(ag7100_cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Atheros AR71xx SoC Gigabit ethernet");
