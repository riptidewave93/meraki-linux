/*
 * AppliedMicro APM862xx SoC Classifier Test code
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * All rights reserved. Pranavkumar Sawargaonkar <psawargaonkar@apm.com>
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
 * @file apm_cle_bridge.c
 *
 * Implementing software packet bridge functionality using Classifier.
 *
 */
#include "apm_enet_access.h"
#include "apm_cle_access.h"
#include "apm_preclass_data.h"
#include "apm_cle_cfg.h"
#include "apm_cle_bridge.h"
#include <net/net_namespace.h>
#include <linux/rtnetlink.h>

struct cle_br_gbl_data {
	atomic_t avl_cnt; /* Number of entries in HW AVL tree */
	struct ptree_kn kn_p0;
	struct ptree_kn kn_p1;
	struct net_device *br_devs[MAX_PORTS];
	int cle_br_enabled; /* To enable/disable Cle bridge functionality dynamically */
	struct proc_dir_entry *entry;
};

/* Global data for classifier */
struct cle_br_gbl_data *gbl_data = NULL;

#if defined(CONFIG_CLE_BR_EXT_FWD)

/* dbptr orginazation
 * Entry 0 --> Eth0 RX
 * Entry 1 --> Eth1-> RX
 * Entry 2 --> Forward packet to Eth0 for TX
 * Entry 3 --> Forward packet to Eth1 for TX
 * Entry 4 --> Forward packet to EXT_FWD_PORT1
 * Entry 5 --> Forward packet to EXT_FWD_PORT2
 * Entry 6 --> Forward packet to EXT_FWD_PORT3
 * Entry 7 --> Forward packet to EXT_FWD_PORT4
 */
static struct apm_cle_dbptr dbptr[8];

struct net_device *ext_port[MAX_EXT_PORTS];

int apm_cle_br_register_ext_port(struct net_device *ndev)
{
	int i;

	for (i = 0; i < MAX_EXT_PORTS; i++) {
		if (!ext_port[i])
			break;
	}
	
	if (i >= MAX_EXT_PORTS)
		return -1;

	ext_port[i] = ndev;
	printk("%s: External Dev %s registerd at port %d\n", 
					__func__, ndev->name, i);

	return i + 4;
}
EXPORT_SYMBOL(apm_cle_br_register_ext_port);

void apm_cle_br_unregister_ext_port(int ext_portid)
{
	ext_port[ext_portid -4] = NULL;
}
EXPORT_SYMBOL(apm_cle_br_unregister_ext_port);

int find_ext_port(struct net_device *ndev)
{
	int i;

	for (i = 0; i < MAX_EXT_PORTS; i++) {
		if (ext_port[i] == ndev)
			return i;
	}

	return -1;
}

void print_ext_port_info(void)
{
	int i;
	struct net_device *ndev;

	printk("PRINTING EXTERNAL DEVICES REGISTERED FOR FORWARDING\n");
	for (i = 0; i < MAX_EXT_PORTS; i++) {
		if (ext_port[i]) {
			ndev = ext_port[i];	
			printk("Ext port %d Device name %s\n", i, ndev->name);
		}
	}
}

int apm_cle_br_fwd_directly(struct sk_buff *skb, 
			    struct apm_qm_msg_desc * rx_msg_desc)
{
	struct apm_qm_msg32 *msg = (struct apm_qm_msg32 *) rx_msg_desc->msg;
	int tx_port;	
	struct net_device *dev = NULL;
	int rc = -1;

	tx_port = msg->msgup8_2.H0Info_lsb;

	switch(tx_port)	{
	case EXT_FWD_PORT1:
		dev = ext_port[0];
		if (!dev)
			break;
		CLE_BR_DBG_EXT_FWD("Ext port %d Trying to xmit\n", tx_port);
		skb_push(skb, ETH_HLEN);
		rc = dev->netdev_ops->ndo_start_xmit(skb, dev);
		CLE_BR_DBG_EXT_FWD("After dev %s xmit rc %d\n", dev->name, rc);
		break;
	case EXT_FWD_PORT2:
		dev = ext_port[1];
		if (!dev)
			break;
		CLE_BR_DBG_EXT_FWD("Ext port %d Trying to xmit\n", tx_port);
		skb_push(skb, ETH_HLEN);
		rc = dev->netdev_ops->ndo_start_xmit(skb, dev);
		CLE_BR_DBG_EXT_FWD("After dev %s xmit rc %d\n", dev->name, rc);
		break;
	case EXT_FWD_PORT3:
		dev = ext_port[2];
		if (!dev)
			break;
		CLE_BR_DBG_EXT_FWD("Ext port %d Trying to xmit\n", tx_port);
		skb_push(skb, ETH_HLEN);
		rc = dev->netdev_ops->ndo_start_xmit(skb, dev);
		CLE_BR_DBG_EXT_FWD("After dev %s xmit rc %d\n", dev->name, rc);
		break;
	case EXT_FWD_PORT4:
		dev = ext_port[3];
		if (!dev)
			break;
		CLE_BR_DBG_EXT_FWD("Ext port %d Trying to xmit\n", tx_port);
		skb_push(skb, ETH_HLEN);
		rc = dev->netdev_ops->ndo_start_xmit(skb, dev);
		CLE_BR_DBG_EXT_FWD("After dev %s xmit rc %d\n", dev->name, rc);
		break;
	default:
		break;
	}

	return rc;
}
#else
/* For Defining Result RAM entries pointed by Result Ptr */
/* dbptr orginazation
 * Entry 0 --> Eth0 RX
 * Entry 1 --> Eth1-> RX
 * Entry 2 --> Forward packet to Eth0 for TX
 * Entry 3 --> Forward packet to Eth1 for TX
 */
static struct apm_cle_dbptr dbptr[4];
#endif

/*
 * Parser Node branches for EWDN/FWDN ,they defines actual
 * comparision data at each node
 *
 */
static struct ptree_branch branch[] = {
	{ 0xFFFF,    0x0, EQT, PTREE_ALLOC(1), 0, 2, 0, 0, 0 },  /* MAC Address 0-1 bytes */
	{ 0XFFFF,    0X0, EQT, PTREE_ALLOC(2), 0, 4, 0, 0, 0 },  /* MAC Address 2-3 bytes */
	{ 0xFFFF,    0x0, EQT, PTREE_ALLOC(3), 0, 0, 0, 0, 0 },  /* MAC Address 4-5 bytes */
	{ 0XFFFF,    0X0, EQT, PTREE_ALLOC(4), 0, 0, 0, 0, 0 },
};

/*
 * Parser Node data, they refer to to branch for individual branch
 * configurations within EWDN/FWDN.
 */
static struct ptree_dn dn[] = {
	{ START_NODE,	DBPTR_DROP(0), 3, 0, 0, 0, 1, &branch[0] },
	{ INTERIM_NODE, DBPTR_DROP(0), 3, 0, 0, 0, 1, &branch[1] },
	{ INTERIM_NODE, DBPTR_DROP(0), 3, 0, 0, 0, 1, &branch[2] },
	{ LAST_NODE,	DBPTR_DROP(0), 0, 0, 0, 0, 1, &branch[3] },
};

/*
 *  Parser Node configurations for port 0.
 */
struct ptree_node node_p0[] = {
	{ PTREE_ALLOC(0), EWDN, 0, (struct ptree_dn*)&dn[0] },
	{ PTREE_ALLOC(1), EWDN, 0, (struct ptree_dn*)&dn[1] },
	{ PTREE_ALLOC(2), EWDN, 0, (struct ptree_dn*)&dn[2] },
	{ PTREE_ALLOC(3), EWDN, 0, (struct ptree_dn*)&dn[3] },
	{ PTREE_ALLOC(4), KN,   0, NULL },
};

/*
 *  Parser Node configurations for port 1.
 */
struct ptree_node node_p1[] = {
	{ PTREE_ALLOC(0), EWDN, 0, (struct ptree_dn*)&dn[0] },
	{ PTREE_ALLOC(1), EWDN, 0, (struct ptree_dn*)&dn[1] },
	{ PTREE_ALLOC(2), EWDN, 0, (struct ptree_dn*)&dn[2] },
	{ PTREE_ALLOC(3), EWDN, 0, (struct ptree_dn*)&dn[3] },
	{ PTREE_ALLOC(4), KN,   0, NULL },
};

/*
 * Receive Packet Priorities -
 * 1. AVL Match Found (Highest "Pri 0")
 * 2. Patricia Tree
 * 3. Default  (Lowest Pri "7")
 */
static int apm_cle_set_def_res_pri(u8 port_id)
{
	u32 data;
	int rc = APM_RC_OK;

	if (port_id == PORT_1G_0)
		apm_gbl_cle_rd32(DFCLSRESDBPTR0_ADDR, &data);
	else if(port_id == PORT_1G_1)
		apm_gbl_cle_rd32(DFCLSRESDBPTR1_ADDR, &data);
	else
		return -ENODEV;

	/* Setting lowest priority for default result */
	data |= DFCLSRESDBPRIORITY0_WR(0x7);

	if (port_id == PORT_1G_0)
		rc = apm_gbl_cle_wr32(DFCLSRESDBPTR0_ADDR, data);
	else if (port_id == PORT_1G_1)
		rc = apm_gbl_cle_wr32(DFCLSRESDBPTR1_ADDR, data);

	return rc;
}

/*
 * This function programs Classifier Parser nodes and starts the classifier
 * engine for given port. Parser walk forms the serach string for the Search
 * Engine lookup
 */
int apm_cle_br_config_parser_tree(u8 port_id)
{
	struct ptree_node *node;
	struct apm_ptree_config *br_ptree_config_p0;
	struct apm_ptree_config *br_ptree_config_p1;
	struct apm_ptree_config *ptree_config;
	int rc = APM_RC_OK;
	u32 node_cfg_size;
	u32 preclass_db_size;

	if (port_id == PORT_1G_0) {
		br_ptree_config_p0 = 
			apm_add_ptree_config(port_id, CLE_BR_CONFIG_PORT0);
	} else if (port_id == PORT_1G_1) {
		br_ptree_config_p1 = 
			apm_add_ptree_config(port_id, CLE_BR_CONFIG_PORT1);
	} else {
		rc = -ENODEV;
		goto exit_config;
	}

	/*
	 * Assign the Parser Result Ptr to Key Node, its priority is assigned 1
	 * hence SEARCH_RES_PTR takes precedence
	 */
	if (port_id == PORT_1G_0) {
		memset(&gbl_data->kn_p0, 0, sizeof(&gbl_data->kn_p0));
		gbl_data->kn_p0.result_pointer = dbptr[CLE_ETH0_RX_DB_IDX].index;
		gbl_data->kn_p0.priority = 1;
		node_p0[4].data = &(gbl_data->kn_p0);
		node_cfg_size = ARRAY_SIZE(node_p0);
		ptree_config = br_ptree_config_p0;
		node = node_p0;
	} else if (port_id == PORT_1G_1) {
		memset(&gbl_data->kn_p1, 0, sizeof(&gbl_data->kn_p1));
		gbl_data->kn_p1.result_pointer = dbptr[CLE_ETH1_RX_DB_IDX].index;
		gbl_data->kn_p1.priority = 1;
		node_p1[4].data = &(gbl_data->kn_p1);
		node_cfg_size = ARRAY_SIZE(node_p1);
		ptree_config = br_ptree_config_p1;
		node = node_p1;
	}

	preclass_db_size = ARRAY_SIZE(dbptr);
	ptree_config->start_pkt_ptr = 0;
	ptree_config->default_result = 
			DBPTR_DROP(0) | CLE_SET_DEF_RES_PRI(CLE_DEF_RES_PRI);
	ptree_config->max_hop = 16;

	CLE_BR_DBG("Creating a Bridge mode Patricia Tree Nodes for "
						"port %d\n", port_id);

	if ((rc = apm_ptree_alloc(port_id, node_cfg_size,
			preclass_db_size, node, &dbptr[0], ptree_config)) !=
			APM_RC_OK) {
		PCLS_ERR("Preclass init error %d \n", rc);
		return rc;
	}

	if ((rc = apm_cle_set_def_res_pri(port_id)) != APM_RC_OK) {
		CLE_BR_ERR("%s %d Error %d in setting default priority for "
			"port :%d", __FILE__, __LINE__, rc, port_id);
	}

exit_config:
	return rc;
}

/*
 * This function programs given search key into Search Engine AVL database.
 */
int apm_cle_add_search_key(void *key, int pri, int db_idx)
{
	int rc = APM_RC_OK;
	struct avl_node node_data;
	u32 *search_key = (u32 *) key;

	memset(&node_data, 0 , sizeof(node_data));

	/* Assign the Priority and Result Ptr */
	node_data.priority = pri;
	node_data.result_pointer = db_idx;

	/* Copy the search sting to add to Search Engien database */
	memcpy (node_data.search_key, search_key, sizeof(node_data.search_key));

	if ((rc |= apm_cle_avl_add_node(&node_data) !=  APM_RC_OK)) {
		CLE_BR_ERR("%s:%d - Error %d - In Ading sarch string to AVl \n",
			   __FUNCTION__, __LINE__, rc);
		return rc;
	}

	atomic_inc(&(gbl_data->avl_cnt));
	return rc;
}

/*
 * This function deletes given search key from Search Engine AVL database.
 */
static int apm_cle_delete_search_key(void *key)
{
	int rc = APM_RC_OK;
	struct avl_node node_data;
	u32 node_addr = 0;

	memset(&node_data, 0 , sizeof(node_data));
	/* Copy the search sting to add to Search Engien database */
	memcpy (node_data.search_key, key, sizeof(node_data.search_key));


	if ((rc |= apm_cle_avl_del_node(&node_data) !=  APM_RC_OK)) {
		CLE_BR_ERR("%s:%d - Error %d - In Ading sarch string to AVl \n",
			   __FUNCTION__, __LINE__, rc);
		return rc;
	}

	node_addr = node_data.index;
	CLE_BR_DBG("%s Node %d deleted from avl tree\n", __func__, node_addr);
	atomic_dec(&(gbl_data->avl_cnt));

	return rc;
}

/* 
 * key -> Dest MAC address
 * fw_dst_port -> Port on which packets to be forwarded.
 */
static int apm_cle_add_fw_entry(u32 *key, int fw_dst_port)
{
	u32 search_key[AVL_MAX_SEARCH_STRING] = {0};
	int rc = APM_RC_OK;
	int db_index;
	int pri = DEFAULT_BR_PRI;

	if (atomic_read(&(gbl_data->avl_cnt)) >= MAX_AVL_ENTRIES)
		return -ENOMEM;

	switch (fw_dst_port) {
	case PORT_1G_0:	
			db_index = dbptr[CLE_ETH0_FW_DB_IDX].index;
			break;
	case PORT_1G_1:
			db_index = dbptr[CLE_ETH1_FW_DB_IDX].index;
			break;
#if defined(CONFIG_CLE_BR_EXT_FWD)
	case EXT_FWD_PORT1:
			db_index = dbptr[EXT_FWD_PORT1].index;
#if 0
			if ((dev = find_netdev(1))) {
				priv_dev = netdev_priv(dev);
				priv_dev->in_poll_rx_msg_desc.is_msg16 = 0;
			}
#endif
			break;
	case EXT_FWD_PORT2:
			db_index = dbptr[EXT_FWD_PORT2].index;
			break;
	case EXT_FWD_PORT3:
			db_index = dbptr[EXT_FWD_PORT3].index;
			break;
	case EXT_FWD_PORT4:
			db_index = dbptr[EXT_FWD_PORT4].index;
			break;
#endif
	default:
		printk("Unknown port %d for forwarding, assigning drop db!!\n", 
				fw_dst_port);
		db_index = 0;
	}

	search_key[0] = cpu_to_le32(key[0]);
	search_key[1] = cpu_to_le32((key[1] & 0xffff0000));

#if 0
	CLE_BR_DBG("Key 0 %x swapped key %x\n", key[0], cpu_to_le32(key[0]));
	CLE_BR_DBG("Key 1 %x swapped key %x\n", key[1], 
					cpu_to_le32((key[1] & 0xffff0000)));
	printk("forward this to port %d\n", fw_dst_port);
#endif

	/* Add any number of serach entry to Search  Engine database */
	if ((rc |= apm_cle_add_search_key(search_key, pri, db_index) !=  APM_RC_OK)) {
		CLE_BR_ERR("%s:%d - Error %d - In Classifier search"
			   "engine config\n",
			   __FUNCTION__, __LINE__, rc);
	}

	return rc;
}

/* 
 * key -> Dest MAC address 
 */
static int apm_cle_delete_fw_entry(u32 *key)
{
	u32 search_key[AVL_MAX_SEARCH_STRING] = {0};
	int rc = APM_RC_OK;

	search_key[0] =  cpu_to_le32(key[0]);
	search_key[1] = cpu_to_le32((key[1] & 0xffff0000));

#if 0
	CLE_BR_DBG("Key 0 %x swapped key %x\n", key[0], cpu_to_le32(key[0]));
	CLE_BR_DBG("Key 1 %x swapped key %x\n", key[1], 
					cpu_to_le32((key[1] & 0xffff0000)));
#endif

	/* Add any number of serach entry to Search  Engine database */
	if ((rc |= apm_cle_delete_search_key(search_key) !=  APM_RC_OK)) {
		CLE_BR_ERR("%s:%d - Error %d - In Classifier search"
			   "engine config\n",
			   __FUNCTION__, __LINE__, rc);
	}

	return rc;
}

#if defined(APM_CLE_TEST_BR)
static void apm_br_add_test_key(void)
{
	int rc = APM_RC_OK;
	u32 search_key[AVL_MAX_SEARCH_STRING] = {0};

	/* To compare MAC Address 00:01:73:01:F2:60 set as following */
	/* Prepare Search Key  */
	search_key[0] = 0x00017301;
	search_key[1] = 0xF2600000;
	rc = apm_cle_add_fw_entry(search_key, 0);

	memset(search_key, 0, sizeof(search_key));
	search_key[0] = 0x00017321;
	search_key[1] = 0xF2600000;
	rc = apm_cle_add_fw_entry(search_key, 0);

	memset(search_key, 0, sizeof(search_key));
	search_key[0] = 0x0000C0A8;
	search_key[1] = 0x05640000;
	rc = apm_cle_add_fw_entry(search_key, 1);
}
#endif

static void apm_cle_check_and_set_ptree(int port_id)
{
	char *ptree_id;
#if defined(CONFIG_CLE_BR_EXT_FWD)
	struct net_device *dev;
	struct apm_enet_dev_base *priv_dev;
#endif

	ptree_id = apm_get_sys_ptree_id(port_id);

	if (port_id == PORT_1G_0) {
		if (strcmp(ptree_id, CLE_BR_CONFIG_PORT0)) {
			 apm_preclass_switch_tree(port_id, 
					CLE_BR_CONFIG_PORT0, 0);
		}
	} else if (port_id == PORT_1G_1) {
		if (strcmp(ptree_id, CLE_BR_CONFIG_PORT1)) {
			 apm_preclass_switch_tree(port_id, 
				CLE_BR_CONFIG_PORT1, 0);
		}
	}

#if defined(CONFIG_CLE_BR_EXT_FWD)
	if (!(dev = find_netdev(port_id)))
		return;

	priv_dev = netdev_priv(dev);
	priv_dev->in_poll_rx_msg_desc.is_msg16 = 0;
#endif
}

/*
 * Function adds a new forwarding entry in to CLE bridge.
 */
int apm_cle_br_fdb_insert(struct net_device *dev, 
			  const unsigned char *addr)
{
	int rc = APM_RC_OK;
	u32 search_key[AVL_MAX_SEARCH_STRING] = {0};

	if (!gbl_data->cle_br_enabled)
		return -1;

	if ((rc = find_port(dev->name)) == -1) {
#if defined(CONFIG_CLE_BR_EXT_FWD)
		if((rc = find_ext_port(dev)) != -1) {
			rc += 4;
			goto add_db_entry;
		}
#endif
		CLE_BR_ERR("Not a cle device dev->name %s\n", dev->name);
		return rc;
	}

	apm_cle_check_and_set_ptree(rc);

#if defined(CONFIG_CLE_BR_EXT_FWD)
add_db_entry:
#endif

	CLE_BR_FDB_DBG("Dest MAC: %02x.%02x.%02x.%02x.%02x.%02x --> ",
			addr[0],
			addr[1],
			addr[2],
			addr[3],
			addr[4],
			addr[5]);
	CLE_BR_FDB_DBG("FWD Port: %d\n", rc);
	memcpy(search_key, addr, 6);
	apm_cle_add_fw_entry((u32 *)search_key, rc);

	return rc;
}
EXPORT_SYMBOL(apm_cle_br_fdb_insert);

/*
 * Function delets a forwarding mac entry from CLE bridge.
 */
int apm_cle_br_fdb_delete(struct net_device *dev, 
			  const unsigned char *addr)
{
	int rc = APM_RC_OK;
	u32 search_key[AVL_MAX_SEARCH_STRING] = {0};


	if ((rc = find_port(dev->name)) == -1) {
#if defined(CONFIG_CLE_BR_EXT_FWD)
		if((rc = find_ext_port(dev)) != -1) {
			rc += 4;
			goto delete_db_entry;
		}
#endif
		CLE_BR_ERR("Not a cle device dev->name %s\n", dev->name);
		return rc;
	}

#if defined(CONFIG_CLE_BR_EXT_FWD)
delete_db_entry:
#endif

	if (!gbl_data)
		return -1;

	CLE_BR_FDB_DBG("MAC address to be deleted from AVL:" 
			"%02x.%02x.%02x.%02x.%02x.%02x\n",
			addr[0],
			addr[1],
			addr[2],
			addr[3],
			addr[4],
			addr[5]);

	memcpy(search_key, addr, 6);
	apm_cle_delete_fw_entry((u32 *)search_key);

	return rc;
}
EXPORT_SYMBOL(apm_cle_br_fdb_delete);

/*
 * Function adds an interface in CLE bridge.
 */
int apm_cle_br_add_if(struct net_device *dev)
{
	int rc = APM_RC_OK;
	int port_id;

	if ((rc = find_port(dev->name)) == -1)
		return rc;

	port_id = rc;
	if ((rc = apm_cle_br_config_parser_tree(port_id)) != APM_RC_OK)
		return rc;

	dev->priv_flags |= IFF_BRIDGE_PORT;
	gbl_data->br_devs[port_id] = dev;

	return rc;			
}
EXPORT_SYMBOL(apm_cle_br_add_if);

/*
 * Function deletes an interface fro CLE bridge.
 */
int apm_cle_br_del_if(struct net_device *dev)
{
	int rc = APM_RC_OK;
	struct apm_enet_dev_base *priv_dev;

	if ((rc = find_port(dev->name)) == -1)
		return rc;

	priv_dev =  netdev_priv(dev);
	dev->priv_flags &= ~IFF_BRIDGE_PORT;
	gbl_data->br_devs[rc] = NULL;
	apm_preclass_switch_tree(priv_dev->port_id, CLE_PTREE_DEFAULT, 0);

	return rc;
}
EXPORT_SYMBOL(apm_cle_br_del_if);

static int apm_cle_br_driver_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t apm_cle_br_driver_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("\nClassifier based bridge module\n");
	if (gbl_data->cle_br_enabled)
		printk("Bridge optimization: Enable\n");
	else
		printk("Bridge optimization: Disable\n");

	return 0;
}

static ssize_t apm_cle_br_driver_write(struct file *file, const char __user *buf,
                                        size_t count, loff_t *ppos)
{
	int i = 0;

	switch(*buf) {
	case CMD_DISABLE_CLE_BR:
		printk("\nDisabling apm classifier Bridge\n");
		gbl_data->cle_br_enabled = 0;
		for (i = 0; i < MAX_PORTS; i++)
			if (gbl_data->br_devs[i])
				apm_preclass_switch_tree(i, 
					CLE_PTREE_DEFAULT, 0);
		break;
	case CMD_ENABLE_CLE_BR:
		printk("\nEnabling apm classifier bridge\n");
		gbl_data->cle_br_enabled = 1;
		break;
	}

	return count;
}

struct file_operations apm_cle_br_driver_fops = {
	.owner = THIS_MODULE,
	.open = apm_cle_br_driver_open,
	.read = apm_cle_br_driver_read,
	.write = apm_cle_br_driver_write,
};

static void apm_cle_br_set_db(void)
{
	struct net_device *dev;
	struct apm_enet_dev_base *priv_dev;
	int core_id = apm_cle_system_id;
	int i;

	/* Create Result RAM Entries */
	for (i = 0; i < ARRAY_SIZE(dbptr); i++) {
		switch (i) {
		case CLE_ETH0_RX_DB_IDX:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid =  
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(CLE_ETH0_RX_DB_IDX);
			break;
		case CLE_ETH1_RX_DB_IDX:
			if (!(dev = find_netdev(1)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(CLE_ETH1_RX_DB_IDX);
			break;
		case CLE_ETH0_FW_DB_IDX:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_tx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].hw_fp_pbn - 0x20);
			dbptr[i].h0_en = 1;
			dbptr[i].h0_hr = 1;
			dbptr[i].h0_enqnum = 189;
			dbptr[i].h0_info_msb = 0x10;
			dbptr[i].h0_info = 0;
			dbptr[i].index = DBPTR_ALLOC(CLE_ETH0_FW_DB_IDX);
			break;
		case CLE_ETH1_FW_DB_IDX:
			if (!(dev = find_netdev(1)))
				break;
 			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_tx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].hw_fp_pbn - 0x20);
			dbptr[i].h0_en = 1;
			dbptr[i].h0_hr = 1;
			dbptr[i].h0_enqnum = 189;
			dbptr[i].h0_info_msb = 0x10;
			dbptr[i].h0_info = 0;
			dbptr[i].index = DBPTR_ALLOC(CLE_ETH1_FW_DB_IDX);
			break;
#if defined(CONFIG_CLE_BR_EXT_FWD)
		case EXT_FWD_PORT1:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid =  
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(EXT_FWD_PORT1);
			dbptr[i].h0_en = 1;
                        dbptr[i].h0_dr = 1;
			dbptr[i].h0_info_msb = 0xdd;
			dbptr[i].h0_info = 0;
			dbptr[i].h1_en = 1;
			dbptr[i].h1_dr = 1;
			dbptr[i].h1_info_msb = 0xdd;
			dbptr[i].h1_info = EXT_FWD_PORT1;
			break;
		case EXT_FWD_PORT2:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(EXT_FWD_PORT2);
			dbptr[i].h0_en = 1;
                        dbptr[i].h0_dr = 1;
			dbptr[i].h0_info_msb = 0xdd;
			dbptr[i].h0_info = 0;
			dbptr[i].h1_en = 1;
			dbptr[i].h1_dr = 1;
			dbptr[i].h1_info_msb = 0xdd;
			dbptr[i].h1_info = EXT_FWD_PORT2;
			break;
		case EXT_FWD_PORT3:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(EXT_FWD_PORT3);
			dbptr[i].h0_en = 1;
                        dbptr[i].h0_dr = 1;
			dbptr[i].h0_info_msb = 0xdd;
			dbptr[i].h0_info = 0;
			dbptr[i].h1_en = 1;
			dbptr[i].h1_dr = 1;
			dbptr[i].h1_info_msb = 0xdd;
			dbptr[i].h1_info = EXT_FWD_PORT3;
			break;
		case EXT_FWD_PORT4:
			if (!(dev = find_netdev(0)))
				break;
			priv_dev = netdev_priv(dev);
			dbptr[i].dstqid = 
				priv_dev->qm_queues[core_id].default_rx_qid;
			dbptr[i].fpsel = 
				(priv_dev->qm_queues[core_id].rx_fp_pbn - 0x20);
			dbptr[i].index = DBPTR_ALLOC(EXT_FWD_PORT4);
			dbptr[i].h0_en = 1;
                        dbptr[i].h0_dr = 1;
			dbptr[i].h0_info_msb = 0xdd;
			dbptr[i].h0_info = 0;
			dbptr[i].h1_en = 1;
			dbptr[i].h1_dr = 1;
			dbptr[i].h1_info_msb = 0xdd;
			dbptr[i].h1_info = EXT_FWD_PORT4;
			break;
#endif
		default:
			dbptr[i].drop = 1;
			break;
		}
	}

	/* Program the Result RAM entries */
	apm_dbptr_alloc(0, ARRAY_SIZE(dbptr), dbptr);

}

static int apm_cle_init_br(void)
{
	int rc = 0;

	if (!(gbl_data = kmalloc(sizeof(struct cle_br_gbl_data), GFP_ATOMIC))) {
		rc = -ENOMEM;
		goto exit_init_br;
	}

	memset(gbl_data, 0, sizeof(struct cle_br_gbl_data));
	/* Initialize Search Engine, 8 Byte search string is
	   sufficient in this case  */
	apm_cle_avl_init(SEARCH_STRING_SIZE8B);
	atomic_set(&(gbl_data->avl_cnt), 0);
	apm_cle_br_set_db();
#if defined(APM_CLE_TEST_BR)
	apm_br_add_test_key();
#endif
	gbl_data->entry = create_proc_entry(APM_CLE_BR_DRIVER_NAME, 0, NULL);
	gbl_data->entry->proc_fops = &apm_cle_br_driver_fops;
	gbl_data->cle_br_enabled = 1;
#if defined(CONFIG_CLE_BR_EXT_FWD)
	memset(ext_port, 0, sizeof(ext_port));
#endif

exit_init_br:
	return rc;
}

static int __init apm_cle_br_init(void)
{
	return apm_cle_init_br();
}

static void __exit apm_cle_br_exit(void)
{
	remove_proc_entry(APM_CLE_BR_DRIVER_NAME, NULL);
	kfree(gbl_data);
}

module_init(apm_cle_br_init);
module_exit(apm_cle_br_exit);

MODULE_AUTHOR("Pranavkumar Sawargaonkar <psawargaonkar@apm.com>");
MODULE_DESCRIPTION("APM862xx SoC Classifier Bridge Driver");
MODULE_LICENSE("GPL");

