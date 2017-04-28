/*
 * Texas Instruments Ethernet Switch Driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Userspace API for Switch Configuration
 */

#ifndef __NET_CONFIG_SWITCH_H__
#define __NET_CONFIG_SWITCH_H__

enum {
	CONFIG_SWITCH_INVALID,
	CONFIG_SWITCH_ADD_MULTICAST,
	CONFIG_SWITCH_ADD_UNICAST,
	CONFIG_SWITCH_ADD_OUI,
	CONFIG_SWITCH_FIND_ADDR,
	CONFIG_SWITCH_DEL_MULTICAST,
	CONFIG_SWITCH_DEL_UNICAST,
	CONFIG_SWITCH_ADD_VLAN,
	CONFIG_SWITCH_FIND_VLAN,
	CONFIG_SWITCH_DEL_VLAN,
	CONFIG_SWITCH_SET_PORT_VLAN_CONFIG,
	CONFIG_SWITCH_TIMEOUT,
	CONFIG_SWITCH_DUMP,
	CONFIG_SWITCH_SET_FLOW_CONTROL,
	CONFIG_SWITCH_SET_PRIORITY_MAPPING,
	CONFIG_SWITCH_PORT_STATISTICS_ENABLE,
	CONFIG_SWITCH_CONFIG_DUMP,
	CONFIG_SWITCH_RATELIMIT,
	CONFIG_SWITCH_VID_INGRESS_CHECK,
	CONFIG_SWITCH_ADD_UNKNOWN_VLAN_INFO,
	CONFIG_SWITCH_802_1,
	CONFIG_SWITCH_MACAUTH,
	CONFIG_SWITCH_SET_PORT_CONFIG,
	CONFIG_SWITCH_GET_PORT_CONFIG,
	CONFIG_SWITCH_PORT_STATE,
	CONFIG_SWITCH_RESET,
};

enum {
	ADDR_TYPE_BROADCAST = 1,
	ADDR_TYPE_MULTICAST,
	ADDR_TYPE_UNICAST,
};

enum {
	PORT_STATE_DISABLED = 0,
	PORT_STATE_BLOCKED,
	PORT_STATE_LEARN,
	PORT_STATE_FORWARD,
};

struct net_switch_cmd {
	unsigned int	port;
	unsigned int	vid;		/*VLAN identifier*/
	unsigned int	prio_port;	/*port priority 0 -7*/
	unsigned int	CFI_port;	/*port CFI  0 /1 */
	unsigned int	mem_port;	/* port number 0 - 2 */
	unsigned char	unreg_multi;	/*unregistered multicast Egress Ports*/
	unsigned char	reg_multi;	/*register multicast Egress ports*/
	unsigned char	untag_port;	/* Untag ports */
	unsigned char	addr[6];
	unsigned int	flag;
	unsigned int	blocked;
	unsigned int	secure;
	unsigned int	ageable;
	unsigned int	ale_timeout;
	unsigned char	aledump;	/* if 1 print ale table for debug
						purpose*/
};

struct net_switch_port_cmd {
	unsigned int	port;		/* Port number */
	unsigned int	direction;	/* Tx -1 / Rx - 0 */
	unsigned int	addr_type;	/* Address type BroadMulti/Uni cast */
	unsigned int	limit;		/* multicast/broadcast limit */
	unsigned int	vlan_ingress_check;
	unsigned int	port_state;
	unsigned int	vlan_port_mask;
	unsigned char	drop_untagged;
	unsigned char	enable;		/* 1-enable/0-Disable */
	unsigned int	reg_multi_port_mask;
	unsigned int	unknown_reg_multi_port_mask;
	unsigned char	unknown_vlan_member;
};

struct net_switch_prio_cmd {
	unsigned char port;		/*Port number*/
	unsigned char prio_rx;		/* Receiving packet priority */
	unsigned char prio_tx;		/* Transmitting packet priority */
	unsigned char prio_switch;	/* switch priority */
	unsigned char hostRxCh;		/* packet with switch priority receive
						on this channel */
	unsigned char hostRxChSuper;	/* supervisory packet receive on
						this channel */
};

struct net_switch_config {
	unsigned int cmd;		/* Name of API to be invoked by the
						kernel driver */
	union {
		struct net_switch_cmd switchcmd;
		struct net_switch_port_cmd portcmd;
		struct net_switch_prio_cmd priocmd;
		char buf[4096];		/* Buffer for Ale Dump */
	} cmd_data;
	int ret_type;   /* Return  Success/Failure */
};

#endif /* __NET_CONFIG_SWITCH_H__*/
