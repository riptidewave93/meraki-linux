/**
 * AppliedMicro APM862xx SoC Ethernet Driver
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
 * @file apm_enet_tools.h
 *
 * This file defines Ethtool functions for APM862xxSoC Ethernet subsystem
 *
 **
 */

#ifndef __APM_ENET_TOOLS_H__
#define __APM_ENET_TOOLS_H__

#include "apm_enet_access.h"

#define APM_ENET_MIN_MTU		40
#define APM_ENET_MAX_MTU		9050

/**
 * @brief   This function retrives device settings
 * @param   ndev Ethernet netdev structure
 *	    cmd Pointer to set configurations to
 * @return  0 - success or -1 - failure
 * @note    Retrives settings like auto negotiation, duplex mode etc
 **
 */
int apm_ethtool_get_settings(struct net_device *ndev, 
			    struct ethtool_cmd *cmd);

/**
 * @brief   This function configures device settings
 * @param   ndev Ethernet netdev structure
 *	    cmd Pointer to set configurations from
 * @return  0 - success or -1 - failure
 * @note    Configures settings like auto negotiation, duplex mode etc
 **
 */
int apm_ethtool_set_settings(struct net_device *ndev, 
			    struct ethtool_cmd *cmd);

/**
 * @brief   This function sets pause parameters
 * @param   ndev Ethernet netdev structure
 *	    pp Pause parameter pointer
 * @return  0 - success or -1 - failure
 **
 */
int apm_ethtool_set_pauseparam(struct net_device *ndev,
			      struct ethtool_pauseparam *pp);

/**
 * @brief   This function retrives pause parameters
 * @param   ndev Ethernet netdev structure
 *	    pp Pause parameter pointer
 * @return  0 - success or -1 - failure
 **
 */
void apm_ethtool_get_pauseparam(struct net_device *ndev,
			       struct ethtool_pauseparam *pp);

/**
 * @brief   This function configures RX checksum
 * @param   ndev Ethernet netdev structure
 *	    set Set or clear flag
 * @return  0 - success or -1 - failure
 **
 */
int apm_ethtool_set_rx_csum(struct net_device *ndev, u32 set);

/**
 * @brief   This function retrives RX checksum
 * @param   ndev Ethernet netdev structure
 * @return  RX checksum status
 **
 */
u32 apm_ethtool_get_rx_csum(struct net_device *ndev);

/**
 * @brief   This function configures TX checksum
 * @param   ndev Ethernet netdev structure
 *	    set Set or clear flag
 * @return  0 - success or -1 - failure
 **
 */
int apm_ethtool_set_tx_csum(struct net_device *ndev, u32 set);

/**
 * @brief   This function retrives TX checksum
 * @param   ndev Ethernet netdev structure
 **
 */
u32 apm_ethtool_get_tx_csum(struct net_device *ndev);

/**
 * @brief   This function retrives scatter/gather status
 * @param   ndev Ethernet netdev structure
 **
 */
u32 apm_ethtool_get_sg(struct net_device *ndev);

/**
 * @brief   This function sets scatter/gather status
 * @param   ndev Ethernet netdev structure
 *	    set Set or clear flag
 * @return  0 - success or -1 - failure
 **
 */
int apm_ethtool_set_sg(struct net_device *ndev, u32 set);


/**
 * @brief   This function resets ethernet subsystem
 * @param   ndev Ethernet netdev structure
 * @return  0 - success or -1 - failure
 **
 */
int apm_ethtool_nway_reset(struct net_device *ndev);

/**
 * @brief   This function retrives statistics
 * @param   ndev Ethernet netdev structure
 *	    estats Statistics structure to copy stats to
 *	    tmp_stats Temporay status structures
 * @return  None
 **
 */
void apm_ethtool_get_ethtool_stats(struct net_device *ndev,
				  struct ethtool_stats *estats,
				  u64 *tmp_stats);

/**
 * @brief   This function retrives general driver information
 * @param   ndev Ethernet netdev structure
 *	    info Driver info struct
 * @return  None
 **
 */
void apm_ethtool_get_drvinfo(struct net_device *ndev,
			    struct ethtool_drvinfo *info);


/**
 * @brief   This function retrives link status
 * @param   ndev Ethernet netdev structure
 * @return  Link status
 **
 */
u32 apm_ethtool_op_get_link(struct net_device *ndev);

/**
 * @brief   This function retrives netdev statistics
 * @param   ndev Ethernet netdev structure
 * @return  Netdev statistics
 **
 */
struct net_device_stats *emac_stats(struct net_device *ndev);

/**
 * @brief   This function configures mac address into device
 * @param   ndev Ethernet netdev structure
 *	    p Pointer to address structure
 * @return  0 - success or -1 - failure
 **
 */
int apm_enet_set_mac_addr(struct net_device *ndev, void *p);

u32 apm_ethtool_get_tso(struct net_device *ndev);
int apm_ethtool_set_tso(struct net_device *dev, u32 set);

int apm_get_sset_count(struct net_device *ndev, int sset);
void apm_get_strings(struct net_device *netdev, u32 stringset, u8 *data);

extern const struct ethtool_ops apm_ethtool_ops;

#endif

