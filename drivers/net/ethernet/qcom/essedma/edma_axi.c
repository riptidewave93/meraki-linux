/*
 * Copyright (c) 2014 - 2017, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/cpu_rmap.h>
#include <linux/of_net.h>
#include <linux/timer.h>
#include <linux/bitmap.h>
#include "edma.h"
#include "ess_edma.h"
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/clk.h>
#include <linux/string.h>
#include <linux/reset.h>
#include <linux/phy.h>

typedef struct
{
   u32 switchreg_base_addr;
   u32 switchreg_size;
   u32 psgmiireg_base_addr;
   u32 psgmiireg_size;
   char *reg_access_mode;
   char *psgmii_reg_access_str;
   struct clk *ess_clk;
   u32      mac_mode;
} ssdk_dt_cfg;

ssdk_dt_cfg ssdk_dt_global = {0};
u8  __iomem      *ess_hw_addr = NULL;
u8  __iomem      *psgmii_hw_addr = NULL;

/* Weight round robin and virtual QID mask */
#define EDMA_WRR_VID_SCTL_MASK 0xffff

/* Weight round robin and virtual QID shift */
#define EDMA_WRR_VID_SCTL_SHIFT 16

char edma_axi_driver_name[] = "ess_edma";
static const u32 default_msg = NETIF_MSG_DRV | NETIF_MSG_PROBE |
	NETIF_MSG_LINK | NETIF_MSG_TIMER | NETIF_MSG_IFDOWN | NETIF_MSG_IFUP;

static u32 edma_hw_addr;

struct timer_list edma_stats_timer;
struct mii_bus *miibus_gb;

char edma_tx_irq[16][64];
char edma_rx_irq[8][64];
struct net_device *edma_netdev[EDMA_MAX_PORTID_SUPPORTED];
static struct phy_device *edma_phydev[EDMA_MAX_PORTID_SUPPORTED];
static int edma_link_detect_bmp;
static int phy_dev_state[EDMA_MAX_PORTID_SUPPORTED];
static u16 tx_start[4] = {EDMA_TXQ_START_CORE0, EDMA_TXQ_START_CORE1,
			EDMA_TXQ_START_CORE2, EDMA_TXQ_START_CORE3};
static u32 tx_mask[4] = {EDMA_TXQ_IRQ_MASK_CORE0, EDMA_TXQ_IRQ_MASK_CORE1,
			EDMA_TXQ_IRQ_MASK_CORE2, EDMA_TXQ_IRQ_MASK_CORE3};

static u32 edma_default_ltag  __read_mostly = EDMA_LAN_DEFAULT_VLAN;
static u32 edma_default_wtag  __read_mostly = EDMA_WAN_DEFAULT_VLAN;
static u32 edma_default_group1_vtag  __read_mostly = EDMA_DEFAULT_GROUP1_VLAN;
static u32 edma_default_group2_vtag  __read_mostly = EDMA_DEFAULT_GROUP2_VLAN;
static u32 edma_default_group3_vtag  __read_mostly = EDMA_DEFAULT_GROUP3_VLAN;
static u32 edma_default_group4_vtag  __read_mostly = EDMA_DEFAULT_GROUP4_VLAN;
static u32 edma_default_group5_vtag  __read_mostly = EDMA_DEFAULT_GROUP5_VLAN;

static u32 edma_default_group1_bmp  __read_mostly = EDMA_DEFAULT_GROUP1_BMP;
static u32 edma_default_group2_bmp  __read_mostly = EDMA_DEFAULT_GROUP2_BMP;
static u32 edma_disable_rss __read_mostly = EDMA_DEFAULT_DISABLE_RSS;

static int edma_weight_assigned_to_q __read_mostly;
static int edma_queue_to_virtual_q __read_mostly;
static bool edma_enable_rstp  __read_mostly;
static int edma_athr_hdr_eth_type __read_mostly;
static bool edma_enable_cpu_port __read_mostly;

static int page_mode;
module_param(page_mode, int, 0);
MODULE_PARM_DESC(page_mode, "enable page mode");

static int overwrite_mode;
module_param(overwrite_mode, int, 0);
MODULE_PARM_DESC(overwrite_mode, "overwrite default page_mode setting");

static int jumbo_mru = EDMA_RX_HEAD_BUFF_SIZE;
module_param(jumbo_mru, int, 0);
MODULE_PARM_DESC(jumbo_mru, "enable fraglist support");

static int num_rxq = 4;
module_param(num_rxq, int, 0);
MODULE_PARM_DESC(num_rxq, "change the number of rx queues");

void edma_write_reg(u16 reg_addr, u32 reg_value)
{
	writel(reg_value, ((void __iomem *)(edma_hw_addr + reg_addr)));
}

void edma_read_reg(u16 reg_addr, volatile u32 *reg_value)
{
	*reg_value = readl((void __iomem *)(edma_hw_addr + reg_addr));
}

void ess_write_reg(u16 reg_addr, u32 reg_value)
{
	writel(reg_value, ((void __iomem *)(ess_hw_addr + reg_addr)));
}

void ess_read_reg(u16 reg_addr, volatile u32 *reg_value)
{
	*reg_value = readl((void __iomem *)(ess_hw_addr + reg_addr));
}

/* edma_change_tx_coalesce()
 *	change tx interrupt moderation timer
 */
void edma_change_tx_coalesce(int usecs)
{
	u32 reg_value;

	/* Here, we right shift the value from the user by 1, this is
	 * done because IMT resolution timer is 2usecs. 1 count
	 * of this register corresponds to 2 usecs.
	 */
	edma_read_reg(EDMA_REG_IRQ_MODRT_TIMER_INIT, &reg_value);
	reg_value = ((reg_value & 0xffff) | ((usecs >> 1) << 16));
	edma_write_reg(EDMA_REG_IRQ_MODRT_TIMER_INIT, reg_value);
}

/* edma_change_rx_coalesce()
 *	change rx interrupt moderation timer
 */
void edma_change_rx_coalesce(int usecs)
{
	u32 reg_value;

	/* Here, we right shift the value from the user by 1, this is
	 * done because IMT resolution timer is 2usecs. 1 count
	 * of this register corresponds to 2 usecs.
	 */
	edma_read_reg(EDMA_REG_IRQ_MODRT_TIMER_INIT, &reg_value);
	reg_value = ((reg_value & 0xffff0000) | (usecs >> 1));
	edma_write_reg(EDMA_REG_IRQ_MODRT_TIMER_INIT, reg_value);
}

/* edma_get_tx_rx_coalesce()
 *	Get tx/rx interrupt moderation value
 */
void edma_get_tx_rx_coalesce(u32 *reg_val)
{
	edma_read_reg(EDMA_REG_IRQ_MODRT_TIMER_INIT, reg_val);
}

void edma_read_append_stats(struct edma_common_info *edma_cinfo)
{
	u64 *p;
	int i;
	u32 stat;

	spin_lock(&edma_cinfo->stats_lock);
	p = (u64 *)&(edma_cinfo->edma_ethstats);

	for (i = 0; i < EDMA_MAX_TRANSMIT_QUEUE; i++) {
		edma_read_reg(EDMA_REG_TX_STAT_PKT_Q(i), &stat);
		*p += stat;
		p++;
	}

	for (i = 0; i < EDMA_MAX_TRANSMIT_QUEUE; i++) {
		edma_read_reg(EDMA_REG_TX_STAT_BYTE_Q(i), &stat);
		*p += stat;
		p++;
	}

	for (i = 0; i < EDMA_MAX_RECEIVE_QUEUE; i++) {
		edma_read_reg(EDMA_REG_RX_STAT_PKT_Q(i), &stat);
		*p += stat;
		p++;
	}

	for (i = 0; i < EDMA_MAX_RECEIVE_QUEUE; i++) {
		edma_read_reg(EDMA_REG_RX_STAT_BYTE_Q(i), &stat);
		*p += stat;
		p++;
	}

	spin_unlock(&edma_cinfo->stats_lock);
}

static void edma_statistics_timer(unsigned long data)
{
	struct edma_common_info *edma_cinfo = (struct edma_common_info *)data;

	edma_read_append_stats(edma_cinfo);

	mod_timer(&edma_stats_timer, jiffies + 1*HZ);
}

static int edma_enable_stp_rstp(struct ctl_table *table, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos)
{
	int ret;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write)
		edma_set_stp_rstp(edma_enable_rstp);

	return ret;
}

static int edma_ath_hdr_eth_type(struct ctl_table *table, int write,
				 void __user *buffer, size_t *lenp,
				 loff_t *ppos)
{
	int ret;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write)
		edma_assign_ath_hdr_type(edma_athr_hdr_eth_type);

	return ret;
}

static int edma_get_port_from_phydev(struct phy_device *phydev)
{
	int i;

	for (i = 0; i < EDMA_MAX_PORTID_SUPPORTED; i++) {
		if (phydev == edma_phydev[i])
			return i;
	}

	pr_err("Invalid PHY devive\n");
	return -1;
}

static int edma_is_port_used(int portid)
{
	int used_portid_bmp;
	used_portid_bmp = edma_link_detect_bmp >> 1;

	while (used_portid_bmp) {
		int port_bit_set = ffs(used_portid_bmp);
		if (port_bit_set == portid)
			return 1;
		used_portid_bmp &= ~(1 << (port_bit_set - 1));
	}

	return 0;
}

static int edma_change_default_lan_vlan(struct ctl_table *table, int write,
					void __user *buffer, size_t *lenp,
					loff_t *ppos)
{
	struct edma_adapter *adapter;
	int ret;

	if (!edma_netdev[1]) {
		pr_err("Netdevice for default_lan does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[1]);

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_ltag;

	return ret;
}

static int edma_change_default_wan_vlan(struct ctl_table *table, int write,
					void __user *buffer, size_t *lenp,
					loff_t *ppos)
{
	struct edma_adapter *adapter;
	int ret;

	if (!edma_netdev[0]) {
		pr_err("Netdevice for default_wan does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[0]);

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_wtag;

	return ret;
}

static int edma_change_group1_vtag(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	int ret;

	if (!edma_netdev[0]) {
		pr_err("Netdevice for Group 1 does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[0]);
	edma_cinfo = adapter->edma_cinfo;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_group1_vtag;

	return ret;
}

static int edma_change_group2_vtag(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	int ret;

	if (!edma_netdev[1]) {
		pr_err("Netdevice for Group 2 does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[1]);
	edma_cinfo = adapter->edma_cinfo;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_group2_vtag;

	return ret;
}

static int edma_change_group3_vtag(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	int ret;

	if (!edma_netdev[2]) {
		pr_err("Netdevice for Group 3 does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[2]);
	edma_cinfo = adapter->edma_cinfo;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_group3_vtag;

	return ret;
}

static int edma_change_group4_vtag(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	int ret;

	if (!edma_netdev[3]) {
		pr_err("Netdevice for Group 4 does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[3]);
	edma_cinfo = adapter->edma_cinfo;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_group4_vtag;

	return ret;
}

static int edma_change_group5_vtag(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	int ret;

	if (!edma_netdev[4]) {
		pr_err("Netdevice for Group 5 does not exist\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[4]);
	edma_cinfo = adapter->edma_cinfo;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (write)
		adapter->default_vlan_tag = edma_default_group5_vtag;

	return ret;
}

static int edma_change_group1_bmp(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	struct net_device *ndev;
	struct phy_device *phydev;
	int ret, num_ports_enabled;
	u32 portid_bmp, port_bit, prev_bmp, port_id;

	ndev = edma_netdev[0];
	if (!ndev) {
		pr_err("Netdevice for Group 0 does not exist\n");
		return -1;
	}

	prev_bmp = edma_default_group1_bmp;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if ((!write) || (prev_bmp == edma_default_group1_bmp))
		return ret;

	adapter = netdev_priv(ndev);
	edma_cinfo = adapter->edma_cinfo;

	/* We ignore the bit for CPU Port */
	portid_bmp = edma_default_group1_bmp >> 1;
	port_bit = ffs(portid_bmp);
	if (port_bit > EDMA_MAX_PORTID_SUPPORTED)
		return -1;

	/* If this group has no ports,
	 * we disable polling for the adapter, stop the queues and return
	 */
	if (!port_bit) {
		adapter->dp_bitmap = edma_default_group1_bmp;
		if (adapter->poll_required) {
			adapter->poll_required = 0;
			adapter->link_state = __EDMA_LINKDOWN;
			netif_carrier_off(ndev);
			netif_tx_stop_all_queues(ndev);
		}
		return 0;
	}

	/* Our array indexes are for 5 ports (0 - 4) */
	port_bit--;
	edma_link_detect_bmp = 0;

	/* Do we have more ports in this group */
	num_ports_enabled = bitmap_weight((long unsigned int *)&portid_bmp, 32);

	/* If this group has more then one port,
	 * we disable polling for the adapter as link detection
	 * should be disabled, stop the phy state machine of previous
	 * phy adapter attached to group and start the queues
	 */
	if (num_ports_enabled > 1) {
		if (adapter->poll_required) {
			adapter->poll_required = 0;
			if (adapter->phydev) {
				port_id = edma_get_port_from_phydev(
						adapter->phydev);

				/* We check if phydev attached to this group is
				 * already started and if yes, we stop
				 * the state machine for the phy
				 */
				if (phy_dev_state[port_id]) {
					phy_stop_machine(adapter->phydev);
					phy_dev_state[port_id] = 0;
				}

				adapter->phydev = NULL;
			}

			/* Start the tx queues for this netdev
			 * with link detection disabled
			 */
			if (adapter->link_state == __EDMA_LINKDOWN) {
				adapter->link_state = __EDMA_LINKUP;
				netif_tx_start_all_queues(ndev);
				netif_carrier_on(ndev);
			}
		}
		goto set_bitmap;
	}

	adapter->poll_required = adapter->poll_required_dynamic;

	if (!adapter->poll_required)
		goto set_bitmap;

	phydev = adapter->phydev;

	/* If this group has only one port,
	 * if phydev exists we start the phy state machine
	 * and if it doesn't we create a phydev and start it.
	 */
	if (edma_phydev[port_bit]) {
		adapter->phydev = edma_phydev[port_bit];
		set_bit(port_bit, (long unsigned int*)&edma_link_detect_bmp);

		/* If the Phy device has changed group,
		 * we need to reassign the netdev
		 */
		if (adapter->phydev->attached_dev != ndev)
			adapter->phydev->attached_dev = ndev;

		if (!phy_dev_state[port_bit]) {
			phy_start_machine(adapter->phydev);
			phy_dev_state[port_bit] = 1;
		}
	} else {
		snprintf(adapter->phy_id,
			MII_BUS_ID_SIZE + 3, PHY_ID_FMT,
			miibus_gb->id,
			port_bit);

		adapter->phydev = phy_connect(ndev,
					(const char *)adapter->phy_id,
					&edma_adjust_link,
					PHY_INTERFACE_MODE_SGMII);

		if (IS_ERR(adapter->phydev)) {
			adapter->phydev = phydev;
			pr_err("PHY attach FAIL for port %d", port_bit);
			return -1;
		}

		if (adapter->phydev->attached_dev != ndev)
			adapter->phydev->attached_dev = ndev;

		edma_phydev[port_bit] = adapter->phydev;
		phy_dev_state[port_bit] = 1;
		set_bit(port_bit, (long unsigned int *)&edma_link_detect_bmp);
		adapter->phydev->advertising |=
			(ADVERTISED_Pause |
			ADVERTISED_Asym_Pause);
		adapter->phydev->supported |=
			(SUPPORTED_Pause |
			SUPPORTED_Asym_Pause);
		phy_start(adapter->phydev);
		phy_start_aneg(adapter->phydev);
	}

	/* We check if this phydev is in use by other Groups
	 * and stop phy machine only if it is not stopped
	 */
	if (phydev) {
		port_id = edma_get_port_from_phydev(phydev);
		if (phy_dev_state[port_id]) {
			phy_stop_machine(phydev);
			phy_dev_state[port_id] = 0;
		}
	}

	adapter->poll_required = 1;
	adapter->link_state = __EDMA_LINKDOWN;

set_bitmap:
	while (portid_bmp) {
		int port_bit_set = ffs(portid_bmp);
		edma_cinfo->portid_netdev_lookup_tbl[port_bit_set] = ndev;
		portid_bmp &= ~(1 << (port_bit_set - 1));
	}

	adapter->dp_bitmap = edma_default_group1_bmp;

	return 0;
}

static int edma_change_group2_bmp(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	struct net_device *ndev;
	struct phy_device *phydev;
	int ret;
	u32 prev_bmp, portid_bmp, port_bit, num_ports_enabled, port_id;

	ndev = edma_netdev[1];
	if (!ndev) {
		pr_err("Netdevice for Group 1 does not exist\n");
		return -1;
	}

	prev_bmp = edma_default_group2_bmp;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if ((!write) || (prev_bmp == edma_default_group2_bmp))
		return ret;

	adapter = netdev_priv(ndev);
	edma_cinfo = adapter->edma_cinfo;

	/* We ignore the bit for CPU Port */
	portid_bmp = edma_default_group2_bmp >> 1;
	port_bit = ffs(portid_bmp);
	if (port_bit > EDMA_MAX_PORTID_SUPPORTED)
		return -1;

	/* If this group has no ports,
	 * we disable polling for the adapter, stop the queues and return
	 */
	if (!port_bit) {
		adapter->dp_bitmap = edma_default_group2_bmp;
		if (adapter->poll_required) {
			adapter->poll_required = 0;
			adapter->link_state = __EDMA_LINKDOWN;
			netif_carrier_off(ndev);
			netif_tx_stop_all_queues(ndev);
		}
		return 0;
	}

	/* Our array indexes are for 5 ports (0 - 4) */
	port_bit--;

	/* Do we have more ports in this group */
	num_ports_enabled = bitmap_weight((long unsigned int *)&portid_bmp, 32);

	/* If this group has more then one port,
	 * we disable polling for the adapter as link detection
	 * should be disabled, stop the phy state machine of previous
	 * phy adapter attached to group and start the queues
	 */
	if (num_ports_enabled > 1) {
		if (adapter->poll_required) {
			adapter->poll_required = 0;
			if (adapter->phydev) {
				port_id = edma_get_port_from_phydev(
							adapter->phydev);

				/* We check if this phydev is in use by
				 * other Groups and stop phy machine only
				 * if that is NOT the case
				 */
				if (!edma_is_port_used(port_id)) {
					if (phy_dev_state[port_id]) {
						phy_stop_machine(
							adapter->phydev);
						phy_dev_state[port_id] = 0;
					}
				}

				adapter->phydev = NULL;
			}

			/* Start the tx queues for this netdev
			 * with link detection disabled
			 */
			if (adapter->link_state == __EDMA_LINKDOWN) {
				adapter->link_state = __EDMA_LINKUP;
				netif_carrier_on(ndev);
				netif_tx_start_all_queues(ndev);
			}
		}
		goto set_bitmap;
	}

	adapter->poll_required = adapter->poll_required_dynamic;

	if (!adapter->poll_required)
		goto set_bitmap;

	phydev = adapter->phydev;

	/* If this group has only one port,
	 * if phydev exists we start the phy state machine
	 * and if it doesn't we create a phydev and start it.
	 */
	if (edma_phydev[port_bit]) {
		adapter->phydev = edma_phydev[port_bit];

		/* If the Phy device has changed group,
		 * we need to reassign the netdev
		 */
		if (adapter->phydev->attached_dev != ndev)
			adapter->phydev->attached_dev = ndev;

		if (!phy_dev_state[port_bit]) {
			phy_start_machine(adapter->phydev);
			phy_dev_state[port_bit] = 1;
			set_bit(port_bit,
				(long unsigned int *)&edma_link_detect_bmp);
		}
	} else {
		snprintf(adapter->phy_id,
			MII_BUS_ID_SIZE + 3, PHY_ID_FMT,
			miibus_gb->id,
			port_bit);

		adapter->phydev = phy_connect(ndev,
					(const char *)adapter->phy_id,
					&edma_adjust_link,
					PHY_INTERFACE_MODE_SGMII);

		if (IS_ERR(adapter->phydev)) {
			adapter->phydev = phydev;
			pr_err("PHY attach FAIL for port %d", port_bit);
			return -1;
		}

		if (adapter->phydev->attached_dev != ndev)
			adapter->phydev->attached_dev = ndev;

		edma_phydev[port_bit] = adapter->phydev;
		phy_dev_state[port_bit] = 1;
		set_bit(port_bit, (long unsigned int *)&edma_link_detect_bmp);
		adapter->phydev->advertising |=
			(ADVERTISED_Pause |
			ADVERTISED_Asym_Pause);
		adapter->phydev->supported |=
			(SUPPORTED_Pause |
			SUPPORTED_Asym_Pause);
		phy_start(adapter->phydev);
		phy_start_aneg(adapter->phydev);
	}

	/* We check if this phydev is in use by other Groups
	 * and stop phy machine only if that is NOT the case
	 */
	if (phydev) {
		port_id = edma_get_port_from_phydev(phydev);
		if (!edma_is_port_used(port_id)) {
			if (phy_dev_state[port_id]) {
				phy_stop_machine(phydev);
				phy_dev_state[port_id] = 0;
			}
		}
	}

	adapter->poll_required = 1;
	adapter->link_state = __EDMA_LINKDOWN;

set_bitmap:
	while (portid_bmp) {
		int port_bit_set = ffs(portid_bmp);
		edma_cinfo->portid_netdev_lookup_tbl[port_bit_set] = ndev;
		portid_bmp &= ~(1 << (port_bit_set - 1));
	}

	adapter->dp_bitmap = edma_default_group2_bmp;

	return 0;
}

static int edma_disable_rss_func(struct ctl_table *table, int write,
				   void __user *buffer, size_t *lenp,
				   loff_t *ppos)
{
	struct edma_adapter *adapter;
	struct edma_common_info *edma_cinfo;
	struct edma_hw *hw;
	int ret;

	if (!edma_netdev[0]) {
		pr_err("Invalid Netdevice\n");
		return -1;
	}

	adapter = netdev_priv(edma_netdev[0]);
	edma_cinfo = adapter->edma_cinfo;
	hw = &edma_cinfo->hw;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if ((!write) || (ret))
		return ret;

	switch (edma_disable_rss) {
	case EDMA_RSS_DISABLE:
		hw->rss_type = 0;
		edma_write_reg(EDMA_REG_RSS_TYPE, hw->rss_type);
		break;
	case EDMA_RSS_ENABLE:
		hw->rss_type = EDMA_RSS_TYPE_IPV4TCP |
				EDMA_RSS_TYPE_IPV6_TCP |
				EDMA_RSS_TYPE_IPV4_UDP |
				EDMA_RSS_TYPE_IPV6UDP |
				EDMA_RSS_TYPE_IPV4 |
				EDMA_RSS_TYPE_IPV6;
		edma_write_reg(EDMA_REG_RSS_TYPE, hw->rss_type);
		break;
	default:
		pr_err("Invalid input\n");
		ret = -1;
		break;
	}

	return ret;
}

static int ess_enable_cpu_port(struct ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	u32 regval;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write) {
		if (0x3 == ssdk_dt_global.mac_mode) {
			ess_write_reg(ESS_RGMII_CTRL, 0x400);
		}
		ess_write_reg(ESS_PORT0_STATUS, ESS_PORT_1G_FDX);
		ess_write_reg(ESS_PORT1_STATUS, ESS_PORT_1G_FDX);
		ess_write_reg(ESS_PORT5_STATUS, ESS_PORT_1G_FDX);
		ess_write_reg(ESS_PORT0_HEADER_CTRL, 0);
#define SET_PORT_BMP(r,v) \
			ess_read_reg(r, &regval); \
			ess_write_reg(r, ((regval & ~0x3F) | v));

		SET_PORT_BMP(ESS_PORT0_LOOKUP_CTRL,0x22); /* port 0 -> 1,5 */
		SET_PORT_BMP(ESS_PORT1_LOOKUP_CTRL,0x01); /* port 1 -> 0   */
		SET_PORT_BMP(ESS_PORT5_LOOKUP_CTRL,0x01); /* port 5 -> 0   */
#undef SET_PORT_BMP
	}

	return ret;
}

static int edma_weight_assigned_to_queues(struct ctl_table *table, int write,
					  void __user *buffer, size_t *lenp,
					  loff_t *ppos)
{
	int ret, queue_id, weight;
	u32 reg_data, data, reg_addr;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write) {
		queue_id = edma_weight_assigned_to_q & EDMA_WRR_VID_SCTL_MASK;
		if (queue_id < 0 || queue_id > 15) {
			pr_err("queue_id not within desired range\n");
			return -EINVAL;
		}

		weight = edma_weight_assigned_to_q >> EDMA_WRR_VID_SCTL_SHIFT;
		if (weight < 0 || weight > 0xF) {
			pr_err("queue_id not within desired range\n");
			return -EINVAL;
		}

		data = weight << EDMA_WRR_SHIFT(queue_id);

		reg_addr = EDMA_REG_WRR_CTRL_Q0_Q3 + (queue_id & ~0x3);
		edma_read_reg(reg_addr, &reg_data);
		reg_data &= ~(1 << EDMA_WRR_SHIFT(queue_id));
		edma_write_reg(reg_addr, data | reg_data);
	}

	return ret;
}

static int edma_queue_to_virtual_queue_map(struct ctl_table *table, int write,
					   void __user *buffer, size_t *lenp,
					   loff_t *ppos)
{
	int ret, queue_id, virtual_qid;
	u32 reg_data, data, reg_addr;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write) {
		queue_id = edma_queue_to_virtual_q & EDMA_WRR_VID_SCTL_MASK;
		if (queue_id < 0 || queue_id > 15) {
			pr_err("queue_id not within desired range\n");
			return -EINVAL;
		}

		virtual_qid = edma_queue_to_virtual_q >>
			EDMA_WRR_VID_SCTL_SHIFT;
		if (virtual_qid < 0 || virtual_qid > 8) {
			pr_err("queue_id not within desired range\n");
			return -EINVAL;
		}

		data = virtual_qid << EDMA_VQ_ID_SHIFT(queue_id);

		reg_addr = EDMA_REG_VQ_CTRL0 + (queue_id & ~0x3);
		edma_read_reg(reg_addr, &reg_data);
		reg_data &= ~(1 << EDMA_VQ_ID_SHIFT(queue_id));
		edma_write_reg(reg_addr, data | reg_data);
	}

	return ret;
}

static struct ctl_table edma_table[] = {
	{
		.procname	= "default_lan_tag",
		.data		= &edma_default_ltag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_default_lan_vlan
	},
	{
		.procname	= "default_wan_tag",
		.data		= &edma_default_wtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_default_wan_vlan
	},
	{
		.procname	= "weight_assigned_to_queues",
		.data		= &edma_weight_assigned_to_q,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_weight_assigned_to_queues
	},
	{
		.procname	= "queue_to_virtual_queue_map",
		.data		= &edma_queue_to_virtual_q,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_queue_to_virtual_queue_map
	},
	{
		.procname	= "enable_stp_rstp",
		.data		= &edma_enable_rstp,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_enable_stp_rstp
	},
	{
		.procname	= "athr_hdr_eth_type",
		.data		= &edma_athr_hdr_eth_type,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_ath_hdr_eth_type
	},
	{
		.procname	= "default_group1_vlan_tag",
		.data		= &edma_default_group1_vtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_group1_vtag
	},
	{
		.procname	= "default_group2_vlan_tag",
		.data		= &edma_default_group2_vtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_group2_vtag
	},
	{
		.procname	= "default_group3_vlan_tag",
		.data		= &edma_default_group3_vtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_group3_vtag
	},
	{
		.procname	= "default_group4_vlan_tag",
		.data		= &edma_default_group4_vtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_group4_vtag
	},
	{
		.procname	= "default_group5_vlan_tag",
		.data		= &edma_default_group5_vtag,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= edma_change_group5_vtag
	},
	{
		.procname       = "default_group1_bmp",
		.data           = &edma_default_group1_bmp,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = edma_change_group1_bmp
	},
	{
		.procname       = "default_group2_bmp",
		.data           = &edma_default_group2_bmp,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = edma_change_group2_bmp
	},
	{
		.procname       = "edma_disable_rss",
		.data           = &edma_disable_rss,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = edma_disable_rss_func
	},
	{
		.procname       = "enable_cpu_port",
		.data           = &edma_enable_cpu_port,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = ess_enable_cpu_port
	},
	{}
};

static int __init ssdk_dt_parse(void)
{
	struct device_node *switch_node = NULL;
	struct device_node *psgmii_node = NULL;
	u32 len = 0;
	const __be32 *reg_cfg, *mac_mode;

	switch_node = of_find_node_by_name(NULL, "ess-switch");
	if (!switch_node) {
		printk("cannot find ess-switch node\n");
		return -1;
	}

	reg_cfg = of_get_property(switch_node, "reg", &len);
	if(!reg_cfg) {
		printk("%s: error reading device node properties for reg\n", switch_node->name);
		return -1;
	}

	ssdk_dt_global.switchreg_base_addr = be32_to_cpup(reg_cfg);
	ssdk_dt_global.switchreg_size = be32_to_cpup(reg_cfg + 1);

	if (!request_mem_region(ssdk_dt_global.switchreg_base_addr,
			 ssdk_dt_global.switchreg_size, "switch_mem")) {
		 printk("%s Unable to request resource.", __func__);
		 return -1;
	 }

	ess_hw_addr = ioremap_nocache(ssdk_dt_global.switchreg_base_addr,
			 ssdk_dt_global.switchreg_size);
	if (!ess_hw_addr) {
		printk("%s ioremap fail.", __func__);
		return -1;
	}

	psgmii_node = of_find_node_by_name(NULL, "ess-psgmii");
	if (!psgmii_node) {
		return -1;
	}
	reg_cfg = of_get_property(psgmii_node, "reg", &len);
	if(!reg_cfg) {
		printk("%s: error reading device node properties for reg\n", psgmii_node->name);
		return -1;
	}

	ssdk_dt_global.psgmiireg_base_addr = be32_to_cpup(reg_cfg);
	ssdk_dt_global.psgmiireg_size = be32_to_cpup(reg_cfg + 1);

	if (!request_mem_region(ssdk_dt_global.psgmiireg_base_addr,
				ssdk_dt_global.psgmiireg_size, "psgmii_mem")) {
		printk("%s Unable to request psgmii resource.", __func__);
		return -1;
	}

	psgmii_hw_addr = ioremap_nocache(ssdk_dt_global.psgmiireg_base_addr,
					 ssdk_dt_global.psgmiireg_size);
	if (!psgmii_hw_addr) {
		printk("%s ioremap fail.", __func__);
		return -1;
	}

	ssdk_dt_global.ess_clk = of_clk_get_by_name(switch_node, "ess_clk");
	clk_prepare_enable(ssdk_dt_global.ess_clk);

	mac_mode = of_get_property(switch_node, "switch_mac_mode", &len);
	if (mac_mode) {
		ssdk_dt_global.mac_mode = be32_to_cpup(mac_mode);
	}

	return 0;
}

static int edma_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	struct mii_ioctl_data *mdio = if_mii(ifr);
	int phy_dest_addr;
	uint32_t reg_off = ifr->ifr_map.mem_start;
	uint32_t *reg_val = &ifr->ifr_map.mem_end;

	switch (cmd) {
		case SIOCDEVPRIVATE:
			if ((reg_off & ~(uint32_t)0x3) == reg_off) {
				ess_read_reg(reg_off, reg_val);
			}
			break;
		case SIOCDEVPRIVATE+1:
			if ((reg_off & ~(uint32_t)0x3) == reg_off) {
				ess_write_reg(reg_off, *reg_val);
			}
			break;
		case SIOCGMIIREG:
			phy_dest_addr = mdio->phy_id;
			mdio->val_out  = mdiobus_read(miibus_gb, phy_dest_addr, mdio->reg_num);
			break;
		case SIOCSMIIREG:
			phy_dest_addr = mdio->phy_id;
			mdiobus_write(miibus_gb, phy_dest_addr, mdio->reg_num, mdio->val_in);
			break;
		default:
			return -EOPNOTSUPP;
	}

	return 0;
}

/* edma_axi_netdev_ops
 *	Describe the operations supported by registered netdevices
 *
 * static const struct net_device_ops edma_axi_netdev_ops = {
 *	.ndo_open               = edma_open,
 *	.ndo_stop               = edma_close,
 *	.ndo_start_xmit         = edma_xmit_frame,
 *	.ndo_set_mac_address    = edma_set_mac_addr,
 * }
 */
static const struct net_device_ops edma_axi_netdev_ops = {
	.ndo_open		= edma_open,
	.ndo_stop		= edma_close,
	.ndo_do_ioctl			= edma_ioctl,
	.ndo_start_xmit		= edma_xmit,
	.ndo_set_mac_address	= edma_set_mac_addr,
	.ndo_select_queue	= edma_select_xps_queue,
	.ndo_get_stats64	= edma_get_stats64,
	.ndo_change_mtu		= edma_change_mtu,
};

/* edma_axi_probe()
 *	Initialise an adapter identified by a platform_device structure.
 *
 * The OS initialization, configuring of the adapter private structure,
 * and a hardware reset occur in the probe.
 */
static int edma_axi_probe(struct platform_device *pdev)
{
	struct edma_common_info *edma_cinfo;
	struct edma_hw *hw;
	struct edma_adapter *adapter[EDMA_MAX_PORTID_SUPPORTED];
	struct resource *res;
	struct device_node *np = pdev->dev.of_node;
	struct device_node *pnp;
	struct device_node *mdio_node = NULL;
	struct platform_device *mdio_plat = NULL;
	struct mii_bus *miibus = NULL;
	struct edma_mdio_data *mdio_data = NULL;
	int i, j, k, err = 0;
	u32 portid_bmp;
	int idx = 0, idx_mac = 0;
	int ret;

	ret = ssdk_dt_parse();
	if (ret < 0) {
		pr_err("ESS Failed to parse Device Tree\n");
	}

	if (CONFIG_NR_CPUS != EDMA_CPU_CORES_SUPPORTED) {
		dev_err(&pdev->dev, "Invalid CPU Cores\n");
		return -EINVAL;
	}

	if ((num_rxq != 4) && (num_rxq != 8)) {
		dev_err(&pdev->dev, "Invalid RX queue, edma probe failed\n");
		return -EINVAL;
	}
	edma_cinfo = kzalloc(sizeof(struct edma_common_info), GFP_KERNEL);
	if (!edma_cinfo) {
		err = -ENOMEM;
		goto err_alloc;
	}

	edma_cinfo->pdev = pdev;

	of_property_read_u32(np, "qcom,num-gmac", &edma_cinfo->num_gmac);
	if (edma_cinfo->num_gmac > EDMA_MAX_PORTID_SUPPORTED) {
		pr_err("Invalid DTSI Entry for qcom,num_gmac\n");
		err = -EINVAL;
		goto err_cinfo;
	}

	/* Initialize the netdev array before allocation
	 * to avoid double free
	 */
	for (i = 0 ; i < edma_cinfo->num_gmac ; i++)
		edma_netdev[i] = NULL;

	for (i = 0 ; i < edma_cinfo->num_gmac ; i++) {
		edma_netdev[i] = alloc_etherdev_mqs(sizeof(struct edma_adapter),
			EDMA_NETDEV_TX_QUEUE, EDMA_NETDEV_RX_QUEUE);

		if (!edma_netdev[i]) {
			dev_err(&pdev->dev,
				"net device alloc fails for index=%d\n", i);
			err = -ENODEV;
			goto err_ioremap;
		}

		SET_NETDEV_DEV(edma_netdev[i], &pdev->dev);
		platform_set_drvdata(pdev, edma_netdev[i]);
		edma_cinfo->netdev[i] = edma_netdev[i];
	}

	/* Fill ring details */
	edma_cinfo->num_tx_queues = EDMA_MAX_TRANSMIT_QUEUE;
	edma_cinfo->num_txq_per_core = (EDMA_MAX_TRANSMIT_QUEUE / 4);
	edma_cinfo->tx_ring_count = EDMA_TX_RING_SIZE;

	/* Update num rx queues based on module parameter */
	edma_cinfo->num_rx_queues = num_rxq;
	edma_cinfo->num_rxq_per_core = ((num_rxq == 4) ? 1 : 2);

	edma_cinfo->rx_ring_count = EDMA_RX_RING_SIZE;

	hw = &edma_cinfo->hw;

	/* Fill HW defaults */
	hw->tx_intr_mask = EDMA_TX_IMR_NORMAL_MASK;
	hw->rx_intr_mask = EDMA_RX_IMR_NORMAL_MASK;

	of_property_read_u32(np, "qcom,page-mode", &edma_cinfo->page_mode);
	of_property_read_u32(np, "qcom,rx-head-buf-size",
			     &hw->rx_head_buff_size);

	if (overwrite_mode) {
		dev_info(&pdev->dev, "page mode overwritten");
		edma_cinfo->page_mode = page_mode;
	}

	if (jumbo_mru)
		edma_cinfo->fraglist_mode = 1;

	if (edma_cinfo->page_mode)
		hw->rx_head_buff_size = EDMA_RX_HEAD_BUFF_SIZE_JUMBO;
	else if (edma_cinfo->fraglist_mode)
		hw->rx_head_buff_size = jumbo_mru;
	else if (!hw->rx_head_buff_size)
		hw->rx_head_buff_size = EDMA_RX_HEAD_BUFF_SIZE;

	hw->misc_intr_mask = 0;
	hw->wol_intr_mask = 0;

	hw->intr_clear_type = EDMA_INTR_CLEAR_TYPE;
	hw->intr_sw_idx_w = EDMA_INTR_SW_IDX_W_TYPE;

	/* configure RSS type to the different protocol that can be
	 * supported
	 */
	hw->rss_type = EDMA_RSS_TYPE_IPV4TCP | EDMA_RSS_TYPE_IPV6_TCP |
		EDMA_RSS_TYPE_IPV4_UDP | EDMA_RSS_TYPE_IPV6UDP |
		EDMA_RSS_TYPE_IPV4 | EDMA_RSS_TYPE_IPV6;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	edma_cinfo->hw.hw_addr = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(edma_cinfo->hw.hw_addr)) {
		err = PTR_ERR(edma_cinfo->hw.hw_addr);
		goto err_ioremap;
	}

	edma_hw_addr = (u32)edma_cinfo->hw.hw_addr;

	/* Parse tx queue interrupt number from device tree */
	for (i = 0; i < edma_cinfo->num_tx_queues; i++)
		edma_cinfo->tx_irq[i] = platform_get_irq(pdev, i);

	/* Parse rx queue interrupt number from device tree
	 * Here we are setting j to point to the point where we
	 * left tx interrupt parsing(i.e 16) and run run the loop
	 * from 0 to 7 to parse rx interrupt number.
	 */
	for (i = 0, j = edma_cinfo->num_tx_queues, k = 0;
			i < edma_cinfo->num_rx_queues; i++) {
		edma_cinfo->rx_irq[k] = platform_get_irq(pdev, j);
		k += ((num_rxq == 4) ?  2 : 1);
		j += ((num_rxq == 4) ?  2 : 1);
	}

	edma_cinfo->rx_head_buffer_len = edma_cinfo->hw.rx_head_buff_size;
	edma_cinfo->rx_page_buffer_len = PAGE_SIZE;

	err = edma_alloc_queues_tx(edma_cinfo);
	if (err) {
		dev_err(&pdev->dev, "Allocation of TX queue failed\n");
		goto err_tx_qinit;
	}

	err = edma_alloc_queues_rx(edma_cinfo);
	if (err) {
		dev_err(&pdev->dev, "Allocation of RX queue failed\n");
		goto err_rx_qinit;
	}

	err = edma_alloc_tx_rings(edma_cinfo);
	if (err) {
		dev_err(&pdev->dev, "Allocation of TX resources failed\n");
		goto err_tx_rinit;
	}

	err = edma_alloc_rx_rings(edma_cinfo);
	if (err) {
		dev_err(&pdev->dev, "Allocation of RX resources failed\n");
		goto err_rx_rinit;
	}

	/* Initialize netdev and netdev bitmap for transmit descriptor rings */
	for (i = 0; i < edma_cinfo->num_tx_queues; i++) {
		struct edma_tx_desc_ring *etdr =  edma_cinfo->tpd_ring[i];
		int j;

		etdr->netdev_bmp = 0;
		for (j = 0; j < EDMA_MAX_NETDEV_PER_QUEUE; j++) {
			etdr->netdev[j] = NULL;
			etdr->nq[j] = NULL;
		}
	}

	if (of_property_read_bool(np, "qcom,mdio-supported")) {
		mdio_node = of_find_compatible_node(NULL, NULL,
						    "qcom,ipq40xx-mdio");
		if (!mdio_node) {
			dev_err(&pdev->dev, "cannot find mdio node by phandle");
			err = -EIO;
			goto err_mdiobus_init_fail;
		}

		mdio_plat = of_find_device_by_node(mdio_node);
		if (!mdio_plat) {
			dev_err(&pdev->dev,
				"cannot find platform device from mdio node");
			of_node_put(mdio_node);
			err = -EIO;
			goto err_mdiobus_init_fail;
		}

		mdio_data = dev_get_drvdata(&mdio_plat->dev);
		if (!mdio_data) {
			dev_err(&pdev->dev,
				"cannot get mii bus reference from device data");
			of_node_put(mdio_node);
			err = -EIO;
			goto err_mdiobus_init_fail;
		}

		miibus = mdio_data->mii_bus;
		miibus_gb = mdio_data->mii_bus;
	}

	ess_reset(pdev);

	for_each_available_child_of_node(np, pnp) {
		const char *mac_addr;

		/* this check is needed if parent and daughter dts have
		 * different number of gmac nodes
		 */
		if (idx_mac == edma_cinfo->num_gmac) {
			of_node_put(np);
			break;
		}

		mac_addr = of_get_mac_address(pnp);
		if (mac_addr)
			memcpy(edma_netdev[idx_mac]->dev_addr, mac_addr, ETH_ALEN);

		idx_mac++;
	}

	/* Populate the adapter structure register the netdevice */
	for (i = 0; i < edma_cinfo->num_gmac; i++) {
		int k, m;

		adapter[i] = netdev_priv(edma_netdev[i]);
		adapter[i]->netdev = edma_netdev[i];
		adapter[i]->pdev = pdev;
		for (j = 0; j < CONFIG_NR_CPUS; j++) {
			m = i % 2;
			adapter[i]->tx_start_offset[j] =
				((j << EDMA_TX_CPU_START_SHIFT) + (m << 1));
			/* Share the queues with available net-devices.
			 * For instance , with 5 net-devices
			 * eth0/eth2/eth4 will share q0,q1,q4,q5,q8,q9,q12,q13
			 * and eth1/eth3 will get the remaining.
			 */
			for (k = adapter[i]->tx_start_offset[j]; k <
			     (adapter[i]->tx_start_offset[j] + 2); k++) {
				if (edma_fill_netdev(edma_cinfo, k, i, j)) {
					pr_err("Netdev overflow Error\n");
					goto err_register;
				}
			}
		}

		adapter[i]->edma_cinfo = edma_cinfo;
		edma_netdev[i]->netdev_ops = &edma_axi_netdev_ops;
		edma_netdev[i]->features = NETIF_F_HW_CSUM | NETIF_F_RXCSUM
				| NETIF_F_HW_VLAN_CTAG_TX
				| NETIF_F_HW_VLAN_CTAG_RX | NETIF_F_SG |
				NETIF_F_TSO | NETIF_F_TSO6 | NETIF_F_GRO;
		edma_netdev[i]->hw_features = NETIF_F_HW_CSUM | NETIF_F_RXCSUM |
				NETIF_F_HW_VLAN_CTAG_RX | NETIF_F_HW_VLAN_CTAG_TX
				| NETIF_F_SG | NETIF_F_TSO | NETIF_F_TSO6 |
				NETIF_F_GRO;
		edma_netdev[i]->vlan_features = NETIF_F_HW_CSUM | NETIF_F_SG |
					NETIF_F_TSO | NETIF_F_TSO6 |
					NETIF_F_GRO;
		edma_netdev[i]->wanted_features = NETIF_F_HW_CSUM | NETIF_F_SG |
					NETIF_F_TSO | NETIF_F_TSO6 |
					NETIF_F_GRO;

#ifdef CONFIG_RFS_ACCEL
		edma_netdev[i]->features |=  NETIF_F_RXHASH | NETIF_F_NTUPLE;
		edma_netdev[i]->hw_features |=  NETIF_F_RXHASH | NETIF_F_NTUPLE;
		edma_netdev[i]->vlan_features |= NETIF_F_RXHASH | NETIF_F_NTUPLE;
		edma_netdev[i]->wanted_features |= NETIF_F_RXHASH | NETIF_F_NTUPLE;
#endif
		if (edma_cinfo->fraglist_mode) {
			edma_netdev[i]->features |= NETIF_F_FRAGLIST;
			edma_netdev[i]->hw_features |= NETIF_F_FRAGLIST;
			edma_netdev[i]->vlan_features |= NETIF_F_FRAGLIST;
			edma_netdev[i]->wanted_features |= NETIF_F_FRAGLIST;
		}

		edma_set_ethtool_ops(edma_netdev[i]);

		/* This just fill in some default MAC address
		 */
		if (!is_valid_ether_addr(edma_netdev[i]->dev_addr)) {
			random_ether_addr(edma_netdev[i]->dev_addr);
			pr_info("EDMA using MAC@ - using");
			pr_info("%02x:%02x:%02x:%02x:%02x:%02x\n",
			*(edma_netdev[i]->dev_addr),
			*(edma_netdev[i]->dev_addr + 1),
			*(edma_netdev[i]->dev_addr + 2),
			*(edma_netdev[i]->dev_addr + 3),
			*(edma_netdev[i]->dev_addr + 4),
			*(edma_netdev[i]->dev_addr + 5));
		}

		err = register_netdev(edma_netdev[i]);
		if (err)
			goto err_register;

		/* carrier off reporting is important to
		 * ethtool even BEFORE open
		 */
		netif_carrier_off(edma_netdev[i]);

		/* Allocate reverse irq cpu mapping structure for
		* receive queues
		*/
#ifdef CONFIG_RFS_ACCEL
		edma_netdev[i]->rx_cpu_rmap =
			alloc_irq_cpu_rmap(EDMA_NETDEV_RX_QUEUE);
		if (!edma_netdev[i]->rx_cpu_rmap) {
			err = -ENOMEM;
			goto err_rmap_alloc_fail;
		}
#endif
	}

	for (i = 0; i < EDMA_MAX_PORTID_BITMAP_INDEX; i++)
		edma_cinfo->portid_netdev_lookup_tbl[i] = NULL;

	for_each_available_child_of_node(np, pnp) {
		const uint32_t *vlan_tag = NULL;
		int len;

		/* this check is needed if parent and daughter dts have
		 * different number of gmac nodes
		 */
		if (idx == edma_cinfo->num_gmac)
			break;

		/* Populate port-id to netdev lookup table */
		vlan_tag = of_get_property(pnp, "vlan-tag", &len);
		if (!vlan_tag) {
			pr_err("Vlan tag parsing Failed.\n");
			goto err_rmap_alloc_fail;
		}

		adapter[idx]->default_vlan_tag = of_read_number(vlan_tag, 1);
		vlan_tag++;
		portid_bmp = of_read_number(vlan_tag, 1);
		adapter[idx]->dp_bitmap = portid_bmp;

		portid_bmp = portid_bmp >> 1; /* We ignore CPU Port bit 0 */
		while (portid_bmp) {
			int port_bit = ffs(portid_bmp);

			if (port_bit > EDMA_MAX_PORTID_SUPPORTED)
				goto err_rmap_alloc_fail;
			edma_cinfo->portid_netdev_lookup_tbl[port_bit] =
				edma_netdev[idx];
			portid_bmp &= ~(1 << (port_bit - 1));
		}

		if (of_property_read_u32(pnp, "qcom,poll-required-dynamic",
					  &adapter[idx]->poll_required_dynamic))
			adapter[idx]->poll_required_dynamic = 0;

		if (!of_property_read_u32(pnp, "qcom,poll-required",
					  &adapter[idx]->poll_required)) {
			if (adapter[idx]->poll_required) {
				of_property_read_u32(pnp, "qcom,phy-mdio-addr",
						     &adapter[idx]->phy_mdio_addr);
				of_property_read_u32(pnp, "qcom,forced-speed",
						     &adapter[idx]->forced_speed);
				of_property_read_u32(pnp, "qcom,forced-duplex",
						     &adapter[idx]->forced_duplex);

				/* create a phyid using MDIO bus id
				 * and MDIO bus address
				 */
				snprintf(adapter[idx]->phy_id,
					 MII_BUS_ID_SIZE + 3, PHY_ID_FMT,
					 miibus->id,
					 adapter[idx]->phy_mdio_addr);
			}
		} else {
			adapter[idx]->poll_required = 0;
			adapter[idx]->forced_speed = SPEED_1000;
			adapter[idx]->forced_duplex = DUPLEX_FULL;
		}

		idx++;
	}

	edma_cinfo->edma_ctl_table_hdr = register_net_sysctl(&init_net,
							     "net/edma",
							     edma_table);
	if (!edma_cinfo->edma_ctl_table_hdr) {
		dev_err(&pdev->dev, "edma sysctl table hdr not registered\n");
		goto err_unregister_sysctl_tbl;
	}

	/* Disable all 16 Tx and 8 rx irqs */
	edma_irq_disable(edma_cinfo);

	err = edma_reset(edma_cinfo);
	if (err) {
		err = -EIO;
		goto err_reset;
	}

	/* populate per_core_info, do a napi_Add, request 16 TX irqs,
	 * 8 RX irqs, do a napi enable
	 */
	for (i = 0; i < CONFIG_NR_CPUS; i++) {
		u8 rx_start;

		edma_cinfo->edma_percpu_info[i].napi.state = 0;

		netif_napi_add(edma_netdev[0],
			       &edma_cinfo->edma_percpu_info[i].napi,
			       edma_poll, 64);
		napi_enable(&edma_cinfo->edma_percpu_info[i].napi);
		edma_cinfo->edma_percpu_info[i].tx_mask = tx_mask[i];
		edma_cinfo->edma_percpu_info[i].rx_mask = EDMA_RX_PER_CPU_MASK
				<< (i << EDMA_RX_PER_CPU_MASK_SHIFT);
		edma_cinfo->edma_percpu_info[i].tx_start = tx_start[i];
		edma_cinfo->edma_percpu_info[i].rx_start =
			i << EDMA_RX_CPU_START_SHIFT;
		rx_start = i << EDMA_RX_CPU_START_SHIFT;
		edma_cinfo->edma_percpu_info[i].tx_status = 0;
		edma_cinfo->edma_percpu_info[i].rx_status = 0;
		edma_cinfo->edma_percpu_info[i].edma_cinfo = edma_cinfo;

		/* Request irq per core */
		for (j = edma_cinfo->edma_percpu_info[i].tx_start;
		     j < tx_start[i] + 4; j++) {
			snprintf(&edma_tx_irq[j][0], sizeof(edma_tx_irq[0]), "edma_eth_tx%d", j);
			err = request_irq(edma_cinfo->tx_irq[j],
					  edma_interrupt,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 18, 21))
					  IRQF_DISABLED,
#else
					  0,
#endif
					  &edma_tx_irq[j][0],
					  &edma_cinfo->edma_percpu_info[i]);
			if (err)
				goto err_reset;
		}

		for (j = edma_cinfo->edma_percpu_info[i].rx_start;
		     j < (rx_start +
		     ((edma_cinfo->num_rx_queues == 4) ? 1 : 2));
		     j++) {
			snprintf(&edma_rx_irq[j][0], sizeof(edma_rx_irq[0]), "edma_eth_rx%d", j);
			err = request_irq(edma_cinfo->rx_irq[j],
					  edma_interrupt,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 18, 21))
					  IRQF_DISABLED,
#else
					  0,
#endif
					  &edma_rx_irq[j][0],
					  &edma_cinfo->edma_percpu_info[i]);
			if (err)
				goto err_reset;
		}

#ifdef CONFIG_RFS_ACCEL
		for (j = edma_cinfo->edma_percpu_info[i].rx_start;
		     j < rx_start + 2; j += 2) {
			err = irq_cpu_rmap_add(edma_netdev[0]->rx_cpu_rmap,
					       edma_cinfo->rx_irq[j]);
			if (err)
				goto err_rmap_add_fail;
		}
#endif
	}

	/* Used to clear interrupt status, allocate rx buffer,
	 * configure edma descriptors registers
	 */
	err = edma_configure(edma_cinfo);
	if (err) {
		err = -EIO;
		goto err_configure;
	}

	/* Configure RSS indirection table.
	 * 128 hash will be configured in the following
	 * pattern: hash{0,1,2,3} = {Q0,Q2,Q4,Q6} respectively
	 * and so on
	 */
	for (i = 0; i < EDMA_NUM_IDT; i++)
		edma_write_reg(EDMA_REG_RSS_IDT(i), EDMA_RSS_IDT_VALUE);

	/* Configure load balance mapping table.
	 * 4 table entry will be configured according to the
	 * following pattern: load_balance{0,1,2,3} = {Q0,Q1,Q3,Q4}
	 * respectively.
	 */
	edma_write_reg(EDMA_REG_LB_RING, EDMA_LB_REG_VALUE);

	/* Configure Virtual queue for Tx rings
	 * User can also change this value runtime through
	 * a sysctl
	 */
	edma_write_reg(EDMA_REG_VQ_CTRL0, EDMA_VQ_REG_VALUE);
	edma_write_reg(EDMA_REG_VQ_CTRL1, EDMA_VQ_REG_VALUE);

	/* Configure Max AXI Burst write size to 128 bytes*/
	edma_write_reg(EDMA_REG_AXIW_CTRL_MAXWRSIZE,
		       EDMA_AXIW_MAXWRSIZE_VALUE);

	/* Enable All 16 tx and 8 rx irq mask */
	edma_irq_enable(edma_cinfo);
	edma_enable_tx_ctrl(&edma_cinfo->hw);
	edma_enable_rx_ctrl(&edma_cinfo->hw);

	for (i = 0; i < edma_cinfo->num_gmac; i++) {
		u32 port_id;
		if (!(adapter[i]->poll_required)) {
			adapter[i]->phydev = NULL;
		} else {
			adapter[i]->phydev =
				phy_connect(edma_netdev[i],
					    (const char *)adapter[i]->phy_id,
					    &edma_adjust_link,
					    PHY_INTERFACE_MODE_SGMII);
			if (IS_ERR(adapter[i]->phydev)) {
				dev_dbg(&pdev->dev, "PHY attach FAIL");
				err = -EIO;
				goto edma_phy_attach_fail;
			} else {
				adapter[i]->phydev->advertising |=
					ADVERTISED_Pause |
					ADVERTISED_Asym_Pause;
				adapter[i]->phydev->supported |=
					SUPPORTED_Pause |
					SUPPORTED_Asym_Pause;
				portid_bmp = adapter[i]->dp_bitmap >> 1;
				port_id = ffs(portid_bmp);
				edma_phydev[port_id - 1] = adapter[i]->phydev;
				phy_dev_state[port_id - 1] = 1;
			}
		}
	}

	spin_lock_init(&edma_cinfo->stats_lock);

	init_timer(&edma_stats_timer);
	edma_stats_timer.expires = jiffies + 1*HZ;
	edma_stats_timer.data = (unsigned long)edma_cinfo;
	edma_stats_timer.function = edma_statistics_timer; /* timer handler */
	add_timer(&edma_stats_timer);

	return 0;

edma_phy_attach_fail:
	miibus = NULL;
err_configure:
#ifdef CONFIG_RFS_ACCEL
	for (i = 0; i < edma_cinfo->num_gmac; i++) {
		free_irq_cpu_rmap(adapter[i]->netdev->rx_cpu_rmap);
		adapter[i]->netdev->rx_cpu_rmap = NULL;
	}
#endif
err_rmap_add_fail:
	edma_free_irqs(adapter[0]);
	for (i = 0; i < CONFIG_NR_CPUS; i++)
		napi_disable(&edma_cinfo->edma_percpu_info[i].napi);
err_reset:
err_unregister_sysctl_tbl:
err_rmap_alloc_fail:
	for (i = 0; i < edma_cinfo->num_gmac; i++)
		unregister_netdev(edma_netdev[i]);
err_register:
err_mdiobus_init_fail:
	edma_free_rx_rings(edma_cinfo);
err_rx_rinit:
	edma_free_tx_rings(edma_cinfo);
err_tx_rinit:
	edma_free_queues(edma_cinfo);
err_rx_qinit:
err_tx_qinit:
	iounmap(edma_cinfo->hw.hw_addr);
err_ioremap:
	for (i = 0; i < edma_cinfo->num_gmac; i++) {
		if (edma_netdev[i])
			free_netdev(edma_netdev[i]);
	}
err_cinfo:
	kfree(edma_cinfo);
err_alloc:
	return err;
}

/* edma_axi_remove()
 *	Device Removal Routine
 *
 * edma_axi_remove is called by the platform subsystem to alert the driver
 * that it should release a platform device.
 */
static int edma_axi_remove(struct platform_device *pdev)
{
	struct edma_adapter *adapter = netdev_priv(edma_netdev[0]);
	struct edma_common_info *edma_cinfo = adapter->edma_cinfo;
	struct edma_hw *hw = &edma_cinfo->hw;
	int i;

	for (i = 0; i < edma_cinfo->num_gmac; i++)
		unregister_netdev(edma_netdev[i]);

	edma_stop_rx_tx(hw);
	for (i = 0; i < CONFIG_NR_CPUS; i++)
		napi_disable(&edma_cinfo->edma_percpu_info[i].napi);

	edma_irq_disable(edma_cinfo);
	edma_write_reg(EDMA_REG_RX_ISR, 0xff);
	edma_write_reg(EDMA_REG_TX_ISR, 0xffff);
#ifdef CONFIG_RFS_ACCEL
	for (i = 0; i < edma_cinfo->num_gmac; i++) {
		free_irq_cpu_rmap(edma_netdev[0]->rx_cpu_rmap);
		edma_netdev[0]->rx_cpu_rmap = NULL;
	}
#endif

	for (i = 0; i < EDMA_MAX_PORTID_SUPPORTED; i++) {
		if (edma_phydev[i])
			phy_disconnect(edma_phydev[i]);
	}

	del_timer_sync(&edma_stats_timer);
	edma_free_irqs(adapter);
	unregister_net_sysctl_table(edma_cinfo->edma_ctl_table_hdr);
	edma_free_tx_resources(edma_cinfo);
	edma_free_rx_resources(edma_cinfo);
	edma_free_tx_rings(edma_cinfo);
	edma_free_rx_rings(edma_cinfo);
	edma_free_queues(edma_cinfo);
	for (i = 0; i < edma_cinfo->num_gmac; i++)
		free_netdev(edma_netdev[i]);

	kfree(edma_cinfo);

	iounmap(ess_hw_addr);

	return 0;
}

static const struct of_device_id edma_of_mtable[] = {
	{.compatible = "qcom,ess-edma" },
	{}
};
MODULE_DEVICE_TABLE(of, edma_of_mtable);

static struct platform_driver edma_axi_driver = {
	.driver = {
		.name    = edma_axi_driver_name,
		.of_match_table = edma_of_mtable,
	},
	.probe    = edma_axi_probe,
	.remove   = edma_axi_remove,
};

module_platform_driver(edma_axi_driver);

MODULE_DESCRIPTION("QCA ESS EDMA driver");
MODULE_LICENSE("Dual BSD/GPL");
