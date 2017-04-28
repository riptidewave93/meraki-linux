/**
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Mahesh Pujara <mpujara@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * @file apm_enet_tools.c
 *
 * Ethtool functions for APM86290 ethernet subsystem
 *
 **
 */

#include "apm_enet_tools.h"

const struct ethtool_ops apm_ethtool_ops = {
	.get_settings = apm_ethtool_get_settings,
	.set_settings = apm_ethtool_set_settings,
	.get_drvinfo = apm_ethtool_get_drvinfo,
	.nway_reset = apm_ethtool_nway_reset,
	.get_pauseparam = apm_ethtool_get_pauseparam,
	.set_pauseparam = apm_ethtool_set_pauseparam,
	/*	.get_rx_csum = apm_ethtool_get_rx_csum,
	.get_tx_csum = apm_ethtool_get_tx_csum,
	.set_rx_csum = apm_ethtool_set_rx_csum,
	.set_tx_csum = apm_ethtool_set_tx_csum,
	.get_sg = apm_ethtool_get_sg,
	.set_sg = apm_ethtool_set_sg,
	.get_tso = apm_ethtool_get_tso,
	.set_tso = apm_ethtool_set_tso, */
	.get_ethtool_stats = apm_ethtool_get_ethtool_stats,
	.get_sset_count = apm_get_sset_count,
	.get_strings = apm_get_strings,
	.get_link = apm_ethtool_op_get_link,
};
EXPORT_SYMBOL(apm_ethtool_ops);

struct apm_stats {
	char stat_string[ETH_GSTRING_LEN];
	int sizeof_stat;
	int stat_offset;
};

#define APM_STAT(m) FIELD_SIZEOF(struct apm_enet_dev_base, m), \
		offsetof(struct apm_enet_dev_base, m)
		      
static const struct apm_stats apm_gstrings_stats[] = {
	{ "rx_packets", APM_STAT(hw_stats.rx_stats.rx_packet_count) },
	{ "rx_bytes", APM_STAT(hw_stats.rx_stats.rx_byte_count) },
	{ "tx_packets", APM_STAT(hw_stats.tx_stats.tx_pkt_count) },
	{ "tx_bytes", APM_STAT(hw_stats.tx_stats.tx_byte_count) },
	{ "rx_drop", APM_STAT(hw_stats.rx_stats.rx_drop_pkt_count) },
	{ "tx_drop", APM_STAT(hw_stats.tx_stats.tx_drop_frm_count) },
	{ "c_64B", APM_STAT(hw_stats.eth_combined_stats.c_64B_frames) },
	{ "c_65_127B",
	  APM_STAT(hw_stats.eth_combined_stats.c_65_127B_frames) },
	{ "c_128_255B",
	  APM_STAT(hw_stats.eth_combined_stats.c_128_255B_frames) },
	{ "c_256_511B",
	  APM_STAT(hw_stats.eth_combined_stats.c_256_511B_frames) },
	{ "c_512_1023B",
	  APM_STAT(hw_stats.eth_combined_stats.c_512_1023B_frames) },
	{ "c_1024_1518B",
	  APM_STAT(hw_stats.eth_combined_stats.c_1024_1518B_frames) },
	{ "c_1519_1522B",
	  APM_STAT(hw_stats.eth_combined_stats.c_1519_1522B_frames) },
	{ "rx_multicast", APM_STAT(hw_stats.rx_stats.rx_multicast_pkt_count) },
	{ "rx_broadcast", APM_STAT(hw_stats.rx_stats.rx_broadcast_pkt_count) },
	{ "rx_cntrl", APM_STAT(hw_stats.rx_stats.rx_cntrl_frame_pkt_count) },
	{ "rx_pause", APM_STAT(hw_stats.rx_stats.rx_pause_frame_pkt_count) },
	{ "rx_unknown_op",
	  	APM_STAT(hw_stats.rx_stats.rx_unknown_op_pkt_count)},
	{ "rx_fcs_err", APM_STAT(hw_stats.rx_stats.rx_fcs_err_count) },
	{ "rx_alignment_err",
	  	APM_STAT(hw_stats.rx_stats.rx_alignment_err_pkt_count) },
	{ "rx_frm_len_err",
	  	APM_STAT(hw_stats.rx_stats.rx_frm_len_err_pkt_count) },
	{ "rx_code_err", APM_STAT(hw_stats.rx_stats.rx_code_err_pkt_count) }, 
	{ "rx_carrier_sense_err",
	  	APM_STAT(hw_stats.rx_stats.rx_carrier_sense_err_pkt_count) },
	{ "rx_undersize", APM_STAT(hw_stats.rx_stats.rx_undersize_pkt_count) },
	{ "rx_oversize", APM_STAT(hw_stats.rx_stats.rx_oversize_pkt_count) },
	{ "rx_fragment", APM_STAT(hw_stats.rx_stats.rx_fragment_count) },
	{ "rx_jabber", APM_STAT(hw_stats.rx_stats.rx_jabber_count) },
	{ "tx_multicast", APM_STAT(hw_stats.tx_stats.tx_multicast_pkt_count) },
	{ "tx_broadcast", APM_STAT(hw_stats.tx_stats.tx_broadcast_pkt_count) },
	{ "tx_pause", APM_STAT(hw_stats.tx_stats.tx_pause_frame_count) },
	{ "tx_deferra", APM_STAT(hw_stats.tx_stats.tx_deferral_pkt_count) },
	{ "tx_exesiv_def",
	  	APM_STAT(hw_stats.tx_stats.tx_exesiv_def_pkt_count) },
	{ "tx_single_coll",
	  	APM_STAT(hw_stats.tx_stats.tx_single_coll_pkt_count) },
	{ "tx_multi_coll",
	  	APM_STAT(hw_stats.tx_stats.tx_multi_coll_pkt_count) },
	{ "tx_late_coll", APM_STAT(hw_stats.tx_stats.tx_late_coll_pkt_count) }, 
	{ "tx_exesiv_coll",
	  	APM_STAT(hw_stats.tx_stats.tx_exesiv_coll_pkt_count) },
	{ "tx_toll_coll", APM_STAT(hw_stats.tx_stats.tx_toll_coll_pkt_count) },
	{ "tx_pause_hon", APM_STAT(hw_stats.tx_stats.tx_pause_frm_hon_count) },
	{ "tx_jabber", APM_STAT(hw_stats.tx_stats.tx_jabber_frm_count) },
	{ "tx_fcs_err", APM_STAT(hw_stats.tx_stats.tx_fcs_err_frm_count) },
	{ "tx_control", APM_STAT(hw_stats.tx_stats.tx_control_frm_count) },
	{ "tx_oversize", APM_STAT(hw_stats.tx_stats.tx_oversize_frm_count) },
	{ "tx_undersize", APM_STAT(hw_stats.tx_stats.tx_undersize_frm_count) },
	{ "tx_fragments", APM_STAT(hw_stats.tx_stats.tx_fragments_frm_count) },
	{ "tx_wd_timeouts", APM_STAT(estats.tx_wd_timeouts) },
		
};

#define APM_GLOBAL_STATS_LEN ARRAY_SIZE(apm_gstrings_stats)

/* Ethtool APIs */
int apm_ethtool_get_settings(struct net_device *ndev,
		    		struct ethtool_cmd *cmd)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	cmd->supported = (SUPPORTED_10baseT_Full |
			SUPPORTED_100baseT_Full |
			SUPPORTED_1000baseT_Full |
			SUPPORTED_Autoneg |
			SUPPORTED_TP);

	cmd->advertising = (ADVERTISED_10baseT_Full |
			ADVERTISED_100baseT_Full |
			ADVERTISED_1000baseT_Full |
			ADVERTISED_Autoneg |
			ADVERTISED_TP);

	cmd->speed = apm_get_link_speed(priv);
	cmd->speed_hi = 0;
	cmd->duplex = DUPLEX_FULL;
	cmd->port = PORT_TP;
	cmd->phy_address = priv->phy_addr;
	cmd->transceiver = XCVR_EXTERNAL;
	cmd->autoneg = priv->speed ? AUTONEG_DISABLE : AUTONEG_ENABLE;

	return 0;
}

int apm_ethtool_set_settings(struct net_device *ndev,
			    struct ethtool_cmd *cmd)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;
	int phy_bmcr;

	switch (cmd->speed) {
	case SPEED_1000:
		phy_bmcr = 0x1300;
		break;

	case SPEED_100:
		phy_bmcr = 0x2100;
		break;

	case SPEED_10:
		phy_bmcr = 0x0100;
		break;

	default:
		phy_bmcr = 0;
	}

	local_bh_disable();

	if (cmd->duplex == DUPLEX_FULL && cmd->speed_hi == 0 && phy_bmcr) {
		apm_gmac_init(priv, ndev->dev_addr, cmd->speed, HW_MTU(ndev->mtu), priv->crc);
		if (priv_dev->ipg > 0)
			apm_gmac_set_ipg(priv, priv_dev->ipg); 	
		
		apm_genericmiiphy_write(priv, priv->phy_addr,
				RTL_BMCR_ADR, phy_bmcr);

		if (cmd->speed == SPEED_10 || cmd->speed == SPEED_100)
			priv->speed = cmd->speed;
		else
			priv->speed = 0;
	}

	local_bh_enable();
	return 0;
}

int apm_ethtool_set_pauseparam(struct net_device *ndev,
			      struct ethtool_pauseparam *pp)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	apm_gmac_rx_flow_control(priv, pp->rx_pause);
	apm_gmac_tx_flow_control(priv, pp->tx_pause);

	return 0;
}

void apm_ethtool_get_pauseparam(struct net_device *ndev,
			       struct ethtool_pauseparam *pp)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	pp->rx_pause = apm_gmac_is_rx_flow_control(priv);
	pp->tx_pause = apm_gmac_is_tx_flow_control(priv);
}

int apm_ethtool_set_rx_csum(struct net_device *ndev, u32 set)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	if (set)
		priv_dev->flags |= FLAG_RX_CSUM_ENABLED;
	else
		priv_dev->flags &= ~FLAG_RX_CSUM_ENABLED;
	return 0;
}

u32 apm_ethtool_get_rx_csum(struct net_device * ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	return (priv_dev->flags & FLAG_RX_CSUM_ENABLED);
}

int apm_ethtool_set_tx_csum(struct net_device *ndev, u32 set)
{
	if (set)
		ndev->features |= NETIF_F_IP_CSUM;
	else
		ndev->features &= ~NETIF_F_IP_CSUM;
	
	return 0;
}

u32 apm_ethtool_get_tx_csum(struct net_device * ndev)
{
	if (ndev->features & NETIF_F_IP_CSUM)
		return 1;
	else
		return 0;
}

u32 apm_ethtool_get_sg(struct net_device * ndev)
{
	if (ndev->features & NETIF_F_SG)
		return 1;
	else
		return 0;
}

int apm_ethtool_set_sg(struct net_device *ndev, u32 set)
{
	if (set)
		ndev->features |= NETIF_F_SG;
	else
		ndev->features &= ~NETIF_F_SG;
	return 0;
}


int apm_ethtool_nway_reset(struct net_device *ndev)
{
	int speed;
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	local_bh_disable();
	apm_genericmiiphy_write(priv, priv->phy_addr,
			RTL_BMCR_ADR, 0x1300);

	priv->speed = 0;
	speed = apm_get_link_speed(priv);
	apm_gmac_init(priv, ndev->dev_addr, speed, HW_MTU(ndev->mtu), priv->crc);
	if (priv_dev->ipg > 0)
		apm_gmac_set_ipg(priv, priv_dev->ipg); 	

	local_bh_enable();

	return 0;
}

void apm_get_strings(struct net_device *ndev, u32 stringset,
		    u8 *data)
{
	u8 *p = data;
	int i;
	
	switch (stringset) {
	case ETH_SS_TEST:
	case ETH_SS_STATS:
		for (i = 0; i < APM_GLOBAL_STATS_LEN; i++) {
			memcpy(p, apm_gstrings_stats[i].stat_string,
			       ETH_GSTRING_LEN);
			p += ETH_GSTRING_LEN;
		}
		break;
	}
}


int apm_get_sset_count(struct net_device *ndev, int sset)
{
	switch (sset) {
	case ETH_SS_TEST:
		return APM_GLOBAL_STATS_LEN; 
	case ETH_SS_STATS:
		return APM_GLOBAL_STATS_LEN; 
	default:
		return -EOPNOTSUPP;
	}
	
}

void apm_ethtool_get_ethtool_stats(struct net_device *ndev,
				  struct ethtool_stats *stats,
				  u64 *data)
{

	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;
	struct eth_detailed_stats *hw_stats = &priv_dev->hw_stats;
	int i;

	apm_gmac_get_detailed_stats(priv, hw_stats);
	for (i = 0; i < APM_GLOBAL_STATS_LEN; i++) {
		char *p = (char *)priv_dev + apm_gstrings_stats[i].stat_offset;
		data[i] = (apm_gstrings_stats[i].sizeof_stat ==
		      		sizeof(u64)) ? *(u64 *)p : *(u32 *)p;
	}
}

void apm_ethtool_get_drvinfo(struct net_device *ndev,
			    struct ethtool_drvinfo *info)
{
	char firmware_version[] = "N/A";
	
	strcpy(info->driver, ndev->name);
	strcpy(info->version, DRV_VERSION);
	strcpy(info->fw_version, firmware_version);
}


u32 apm_ethtool_op_get_link(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;
	
	return apm_get_phy_link_status(priv);
}

u32 apm_ethtool_get_tso(struct net_device *ndev)
{
	if (ndev->features & NETIF_F_TSO)
		return 1;
	else
		return 0;
}

int apm_ethtool_set_tso(struct net_device *dev, u32 set)
{
	if (set)
		dev->features |= NETIF_F_TSO;
	else
		dev->features &= ~NETIF_F_TSO;
	
	return 0;
}
