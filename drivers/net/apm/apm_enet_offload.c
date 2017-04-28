/**
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * All rights reserved. Ravi Patel <rapatel@apm.com>
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
 * @file apm_enet_offload.c
 *
 * Offload functions for APM86290 ethernet subsystem
 *
 **
 */

#include <linux/etherdevice.h>
#include <net/route.h>

#include "apm_enet_offload.h"
#include "apm_enet_mac.h"
#include "apm_cle_mgr.h"
#include "apm_avl_base.h"	/* apm_cle_avl_del API */

struct apm_result_ptr_list {
        struct list_head node;
	u32 index;
};

struct apm_enet_offload_list {
        struct list_head node;
	u32 key[OFFLOAD_KEY_SIZE];
	u32 index;
	union {
		u32 ref_count;					/* Result Ptr */
		struct apm_enet_offload_list *result_ptr_entry;	/* Search Str */
	};
};

static int ipfwd_list_head_init = 0;
static int apm_mkroute_ipv4_init_done[MAX_NO_OF_PORTS] = {0};
static struct list_head unuse_result_ptr_head = {0};
static struct list_head ipfwd_result_ptr_head[MAX_NO_OF_PORTS] = {{0}};
static struct list_head ipfwd_search_str_head[MAX_NO_OF_PORTS] = {{0}};
static struct eth_queue_ids *iport_qm_queue[MAX_NO_OF_PORTS] = {0};
static DEFINE_SPINLOCK(spinlock_avl);

#define apm_cle_lock(n, flags) spin_lock_irqsave(&spinlock_##n, flags)
#define apm_cle_unlock(n, flags) spin_unlock_irqrestore(&spinlock_##n, flags)

extern struct net_device *devices[MAX_PORTS];

static inline int apm_put_unuse_result_ptr(u32 index)
{
	struct list_head *head = &unuse_result_ptr_head;
	struct apm_result_ptr_list *entry;

	if (INVALID_LIST_HEAD(head))
		return -EINVAL;

	entry = kmalloc(sizeof (struct apm_result_ptr_list),
			GFP_KERNEL | __GFP_ZERO);

	if (entry == NULL)
		return -ENOMEM;

	entry->index = index;
	list_add(&entry->node, head);

	return APM_RC_OK;
}

static inline int apm_get_unuse_result_ptr(u32 *index)
{
	struct list_head *head = &unuse_result_ptr_head;
	struct apm_result_ptr_list *entry;

	if (INVALID_LIST_HEAD(head) || list_empty(head))
		return -EINVAL;

	entry = list_first_entry(head, struct apm_result_ptr_list, node);

	if (entry == NULL)
		return -EINVAL;

	*index = entry->index;
	list_del(&entry->node);
	kfree(entry);

	return APM_RC_OK;
}

static inline struct apm_enet_offload_list *
	apm_find_enet_offload_entry(struct list_head *head, u32 *key)
{
	struct apm_enet_offload_list *entry;

	list_for_each_entry(entry, head, node) {
		if (memcmp(entry->key, key, sizeof(entry->key)) == 0) {
			return entry;
		}
	}

	return NULL;
}

static inline struct apm_enet_offload_list *
	apm_add_enet_offload_entry(struct list_head *head, u32 *key)
{
	struct apm_enet_offload_list *entry;

	if (key == NULL || head == NULL || INVALID_LIST_HEAD(head))
		return NULL;

	entry = kmalloc(sizeof (struct apm_enet_offload_list),
			GFP_KERNEL | __GFP_ZERO);

	if (entry == NULL)
		return NULL;

	memcpy(entry->key, key, sizeof(entry->key));
	entry->index = 0xFFFFFFFF;
	list_add(&entry->node, head);

	return entry;
}

static inline void apm_del_enet_offload_entry(struct apm_enet_offload_list *entry)
{
	list_del(&entry->node);
	kfree(entry);
}

static inline int apm_get_arp(__be32 rt_gateway, u8 *rt_mac, u32 eport,
	u32 iport, void **arp, int mk_entry)
{
	int rc = APM_RC_ERROR;
	struct apm_enet_offload_list *result_ptr_entry = NULL;
	struct list_head *result_ptr_head = NULL;
	u32 *key, new_key[OFFLOAD_KEY_SIZE];
	int count = 0;

	result_ptr_head = &ipfwd_result_ptr_head[eport];

	if (INVALID_LIST_HEAD(result_ptr_head))
		goto _ret_get_arp;

	*arp = NULL;

	if (!mk_entry && !apm_ethoffload_ops.arp_cache_entries)
		goto _ret_get_arp;

	list_for_each_entry(result_ptr_entry, result_ptr_head, node) {
		if (result_ptr_entry->key[0] == rt_gateway) {
			u32 index = result_ptr_entry->index;

			count++;
			*arp = (void *)result_ptr_entry;

			if (index < MAX_DB_ENTRIES &&
				get_shadow_cle_dbptrs()[index].h0_info_msb) {
				rc = APM_RC_OK;
				goto _ret_get_arp;
			}
		}
	}

	if (count > 1) {
		ENET_ERROR_OFFLOAD("duplicate arp entries in list for "
			"rt_gateway %d.%d.%d.%d port %d\n",
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		goto _ret_get_arp;
	}

	if ((apm_ethoffload_ops.arp_cache_entries >= (DBPTR_PER_SYSTEM - 3)) ||
			!mk_entry)
		goto _ret_get_arp;

	result_ptr_entry = (struct apm_enet_offload_list *)(*arp);
	if (result_ptr_entry) {
		key = &result_ptr_entry->key[0];
	} else {
		ENET_DEBUG_OFFLOAD("%s: arp entry not found in list for "
			"rt_gateway %d.%d.%d.%d port %d\n", __func__,
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		key = &new_key[0];
		key[0] = rt_gateway;     /* Gateway IP */
		key[1] = 0;
		key[2] = 0;
	}

	if (!is_zero_ether_addr(rt_mac))
		memcpy(&key[1], rt_mac, IFHWADDRLEN);

	if (iport < MAX_NO_OF_PORTS)
		key[2] = (key[2] & ~0xFFFF) | (0xCC50 | iport);

	ENET_DEBUG_OFFLOAD("%s: result_pointer key[0] %08x key[1] %08x "
		"key[2] %08x\n", __func__, key[0], key[1], key[2]);

	if (result_ptr_entry) {
		ENET_DEBUG_OFFLOAD("%s: checking arp entry found in list for "
			"rt_gateway %d.%d.%d.%d port %d\n", __func__,
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		goto _check_arp;
	}

	result_ptr_entry = apm_add_enet_offload_entry(result_ptr_head, key);

	if (!result_ptr_entry) {
		ENET_ERROR_OFFLOAD("Unable to add arp entry in list for "
			"rt_gateway %d.%d.%d.%d port %d\n",
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		goto _ret_get_arp;
	}

	apm_ethoffload_ops.arp_unuse_entries++;
	memcpy(result_ptr_entry->key, key, sizeof(result_ptr_entry->key));
	*arp = (void *)result_ptr_entry;

_check_arp:
	if (is_zero_ether_addr((u8 *)&result_ptr_entry->key[1]) ||
			(result_ptr_entry->key[2] & 0xCC50) != 0xCC50) {
		ENET_ERROR_OFFLOAD("Invalid arp entry in list for "
			"rt_gateway %d.%d.%d.%d port %d\n",
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		goto _ret_get_arp;
	}

	apm_get_unuse_result_ptr(&result_ptr_entry->index);
	rc = APM_RC_MISS;

_ret_get_arp:
	return rc;
}

static inline int apm_del_arp(u32 eport, u32 *key)
{
	int rc = APM_RC_OK;
	struct apm_enet_offload_list *result_ptr_entry = NULL, *next = NULL;
	struct list_head *result_ptr_head = &ipfwd_result_ptr_head[eport];

	if (INVALID_LIST_HEAD(result_ptr_head)) {
		rc = APM_RC_ERROR;
		goto _ret_del_arp;
	}

	if (list_empty(result_ptr_head))
		goto _ret_del_arp;

	list_for_each_entry_safe(result_ptr_entry, next, result_ptr_head, node) {
		__be32 rt_gateway = result_ptr_entry->key[0];
		u8 *rt_mac = (u8 *)&result_ptr_entry->key[1];
		u32 iport = result_ptr_entry->key[2] & 0xF;
		u32 index = result_ptr_entry->index;
		u32 valid_index = (index < MAX_DB_ENTRIES);
		u32 key_match = (key && key[0] == rt_gateway);
		u32 valid_zero_ref_count;
		u32 valid_without_result_ptr;
		u32 valid_with_result_ptr = key ? key_match : 1;

		valid_with_result_ptr = valid_with_result_ptr &
			(!is_zero_ether_addr(rt_mac) &&
			(result_ptr_entry->key[2] & 0xCC50) == 0xCC50 &&
			valid_index);

		/* Clear Entry */
		if (valid_with_result_ptr) {
			struct apm_cle_dbptr dbptr;

			ENET_DEBUG_OFFLOAD("%s: Clearing "
				"rt_gateway %d.%d.%d.%d "
				"rt_mac %02x:%02x:%02x:%02x:%02x:%02x "
				"eport %d iport %d\n", __func__,
				(rt_gateway & 0xFF000000) >> 24,
				(rt_gateway & 0x00FF0000) >> 16,
				(rt_gateway & 0x0000FF00) >>  8,
				(rt_gateway & 0x000000FF) >>  0,
				rt_mac[0], rt_mac[1], rt_mac[2],
				rt_mac[3], rt_mac[4], rt_mac[5],
				eport, iport);

			memset(&dbptr, 0, sizeof(dbptr));
			dbptr.index = index;
			dbptr.dstqid = iport_qm_queue[iport]->default_rx_qid;
			dbptr.fpsel  = iport_qm_queue[iport]->rx_fp_pbn - 0x20;

			if (apm_dbptr_alloc(iport, 1, &dbptr)) {
				ENET_ERROR_OFFLOAD("apm_dbptr_alloc failed !!\n");
				rc = APM_RC_ERROR;
				if (key)
					goto _ret_del_arp;
			}

			apm_ethoffload_ops.arp_cache_entries--;
			apm_ethoffload_ops.arp_unuse_entries++;
			result_ptr_entry->key[1] = 0;	/* MAC[0-3] */
			result_ptr_entry->key[2] &= 0xFFFF;	/* MAC[4-5] */
		}

		valid_zero_ref_count = key ? (key_match ||
			(key[0] == 0xFFFFFFFF && is_zero_ether_addr(rt_mac))) :
			1;

		valid_without_result_ptr = valid_zero_ref_count;

		valid_zero_ref_count &= (rt_gateway &&
			result_ptr_entry->ref_count == 0);

		valid_without_result_ptr &= ((!valid_index) || valid_zero_ref_count);

		/* Clear Entry */
		if (valid_zero_ref_count) {
			if (valid_index && (apm_put_unuse_result_ptr(index) !=
					APM_RC_OK)) {
				rc = APM_RC_ERROR;
				goto _ret_del_arp;
			}

			result_ptr_entry->key[0] = 0;	/* Gateway */
			result_ptr_entry->key[1] = 0;	/* MAC[0-3] */
			result_ptr_entry->key[2] = 0;	/* MAC[4-5], Signature & iport info */
		}

		/* Delete Entry */
		if (valid_without_result_ptr) {
			ENET_DEBUG_OFFLOAD("%s: Deleting "
				"rt_gateway %d.%d.%d.%d "
				"rt_mac %02x:%02x:%02x:%02x:%02x:%02x "
				"eport %d iport %d\n", __func__,
				(rt_gateway & 0xFF000000) >> 24,
				(rt_gateway & 0x00FF0000) >> 16,
				(rt_gateway & 0x0000FF00) >>  8,
				(rt_gateway & 0x000000FF) >>  0,
				rt_mac[0], rt_mac[1], rt_mac[2],
				rt_mac[3], rt_mac[4], rt_mac[5],
				eport,
				(result_ptr_entry->key[2] & 0xCC50) == 0xCC50 ?
					iport : -1);

			apm_ethoffload_ops.arp_unuse_entries--;
			apm_del_enet_offload_entry(result_ptr_entry);
		}

		if (key_match)
			break;
	}

_ret_del_arp:
	return rc;
}

static inline int apm_get_route(__be32 rt_dst, __be32 rt_src, __be32 rt_gateway,
	u32 eport, void **route, int mk_entry)
{
	int rc = APM_RC_ERROR;
	struct list_head *search_str_head = NULL;
	static struct avl_node node = {0};

	search_str_head = &ipfwd_search_str_head[eport];

	if (INVALID_LIST_HEAD(search_str_head))
		goto _ret_get_route;

	node.search_key[0] = cpu_to_le32(rt_src);     /* SRC IP */
	node.search_key[1] = cpu_to_le32(rt_dst);     /* DST IP */
	node.search_key[2] = 0;
	ENET_DEBUG_OFFLOAD("%s: search_string key[0] %08x key[1] %08x "
			"key[2] %08x\n", __func__, node.search_key[0],
			node.search_key[1], rt_gateway);

	rc = apm_cle_avl_search_node(&node);

	if (rc != APM_RC_OK && rc != APM_CLE_AVL_SERACH_MISS) {
		goto _ret_get_route;
	} else if (rc == APM_RC_OK) {
		ENET_DEBUG_OFFLOAD("%s: Found AVL Entry at node %d with "
			"result_pointer %d\n", __func__,
			node.index, node.result_pointer);
	}

	if ((rc == APM_CLE_AVL_SERACH_MISS) || !mk_entry) {
		node.search_key[2] = rt_gateway;     /* Gateway IP */
		*route = (void *)apm_find_enet_offload_entry(search_str_head,
			node.search_key);

		if (mk_entry && *route == NULL) {
			*route = (void *)apm_add_enet_offload_entry(search_str_head,
				node.search_key);
			if (*route)
				apm_ethoffload_ops.route_unuse_entries++;
			else
				rc = APM_RC_ERROR;
		}
	}

_ret_get_route:
	return rc;
}

static inline int apm_add_route(
	struct apm_enet_offload_list *result_ptr_entry,
	struct apm_enet_offload_list *search_str_entry)
{
	int rc = APM_RC_OK;
	static struct avl_node node = {0};

	if (apm_ethoffload_ops.route_cache_entries >= AVL_NODES_PER_SYSTEM) {
		ENET_ERROR_OFFLOAD("No AVL Nodes left to add !!\n");
		rc = APM_RC_ERROR;
		goto _ret_add_route;
	}

	node.search_key[0] = search_str_entry->key[0];
	node.search_key[1] = search_str_entry->key[1];
	node.priority = 0;
	node.result_pointer = result_ptr_entry->index;
	if (apm_cle_avl_add_node(&node)) {
		ENET_ERROR_OFFLOAD("apm_cle_avl_add_node failed !!\n");
		rc = APM_RC_ERROR;
		goto _ret_add_route;
	}

	apm_ethoffload_ops.route_cache_entries++;
	apm_ethoffload_ops.route_unuse_entries--;
	result_ptr_entry->ref_count++;
	search_str_entry->index = node.index;
	search_str_entry->result_ptr_entry = result_ptr_entry;
	ENET_DEBUG_OFFLOAD("added AVL Entry at node %d using result pointer %d\n",
				search_str_entry->index, result_ptr_entry->index);

_ret_add_route:
	return rc;
}

static inline void apm_del_route(
	struct apm_enet_offload_list *search_str_entry)
{
	static struct avl_node node = {0};

	if (search_str_entry->result_ptr_entry) {
		node.search_key[0] = search_str_entry->key[0];
		node.search_key[1] = search_str_entry->key[1];
		apm_cle_avl_del_node(&node);
		ENET_DEBUG_OFFLOAD("deleted AVL Entry at node %d\n", node.index);
		search_str_entry->result_ptr_entry->ref_count--;
		if (apm_ethoffload_ops.route_cache_entries) {
			apm_ethoffload_ops.route_cache_entries--;
		} else {
			ENET_ERROR_OFFLOAD("No AVL Nodes left to delete !!\n");
		}
	} else {
		apm_ethoffload_ops.route_unuse_entries--;
		ENET_DEBUG_OFFLOAD("deleted AVL Entry from list\n");
	}
	apm_del_enet_offload_entry(search_str_entry);
}

static inline int apm_set_routes(__be32 rt_gateway, u32 eport, void *arp)
{
	int rc = APM_RC_OK;
	struct apm_enet_offload_list *result_ptr_entry = NULL;
	struct apm_enet_offload_list *search_str_entry = NULL;
	struct list_head *search_str_head = NULL;

	if (arp == NULL) {
		ENET_ERROR_OFFLOAD("arp entry is NULL for "
			"rt_gateway %d.%d.%d.%d port %d\n",
			(rt_gateway & 0xFF000000) >> 24,
			(rt_gateway & 0x00FF0000) >> 16,
			(rt_gateway & 0x0000FF00) >>  8,
			(rt_gateway & 0x000000FF) >>  0,
			eport);
		goto _ret_set_routes;
	}

	search_str_head = &ipfwd_search_str_head[eport];

	if (INVALID_LIST_HEAD(search_str_head))
		goto _ret_set_routes;

	result_ptr_entry = (struct apm_enet_offload_list *)arp;
	list_for_each_entry(search_str_entry, search_str_head, node) {
		if (search_str_entry->key[2] == rt_gateway &&
				search_str_entry->result_ptr_entry == NULL) {
			ENET_DEBUG_OFFLOAD("%s: For port %d ", __func__, eport);
			if (apm_add_route(result_ptr_entry, search_str_entry)) {
				rc = APM_RC_ERROR;
				break;
			}
		}
	}

_ret_set_routes:
	return rc;
}

static int apm_mk_arp(__be32 rt_gateway, u8 *ha, u32 eport, u32 iport)
{
	int rc = APM_RC_ERROR;
	struct apm_enet_offload_list *result_ptr_entry = NULL;
	struct apm_cle_dbptr dbptr;
	u8 rt_mac[IFHWADDRLEN];
	unsigned long flags;

	memcpy(rt_mac, ha, IFHWADDRLEN);
	apm_cle_lock(avl, flags);

	rc = apm_get_arp(rt_gateway, rt_mac, eport, iport,
			(void **)&result_ptr_entry, 1);

	if (rc == APM_RC_ERROR || (rc == APM_RC_OK &&
			compare_ether_addr(rt_mac,
				(u8 *)&result_ptr_entry->key[1]) == 0))
		goto _ret_mk_arp;

	if (is_zero_ether_addr(rt_mac))
		memcpy(rt_mac, &result_ptr_entry->key[1], IFHWADDRLEN);

	if (iport >= MAX_NO_OF_PORTS)
		iport = result_ptr_entry->key[2] & 0xF;

	ENET_DEBUG_OFFLOAD("%s: "
		"rt_gateway %d.%d.%d.%d "
		"rt_mac %02x:%02x:%02x:%02x:%02x:%02x "
		"eport %d iport %d\n",
		__func__,
		(rt_gateway & 0xFF000000) >> 24,
		(rt_gateway & 0x00FF0000) >> 16,
		(rt_gateway & 0x0000FF00) >>  8,
		(rt_gateway & 0x000000FF) >>  0,
		rt_mac[0], rt_mac[1], rt_mac[2],
		rt_mac[3], rt_mac[4], rt_mac[5],
		eport, iport);

	memset(&dbptr, 0, sizeof(dbptr));

	if (result_ptr_entry->index >= MAX_DB_ENTRIES)
		dbptr.index = DBPTR_ALLOC(CLE_DB_INDEX);
	else
		dbptr.index = result_ptr_entry->index;

	dbptr.dstqid = iport_qm_queue[iport]->default_rx_qid;
	dbptr.fpsel  = iport_qm_queue[iport]->hw_fp_pbn - 0x20;
	dbptr.h0_en = 1;
	dbptr.h0_dr = 1;
	dbptr.h0_info_msb = 0xCC;
	dbptr.h0_info = (rt_mac[0] <<  8) |
			(rt_mac[1] <<  0);
	dbptr.h1_en = 1;
	dbptr.h1_dr = 1;
	dbptr.h1_info = (rt_mac[2] << 24) |
			(rt_mac[3] << 16) |
			(rt_mac[4] <<  8) |
			(rt_mac[5] <<  0);

	switch (eport) {
	case PORT_1G_0:
		dbptr.h1_info_msb = 0x50;
		break;
	case PORT_1G_1:
		dbptr.h1_info_msb = 0x51;
		break;
	default:
		dbptr.h1_info = 0;
	}

	ENET_DEBUG_OFFLOAD("%s: dstqid %d fpsel %d "
			"h0_info 0x%02x%08x h1_info 0x%02x%08x\n",
			__func__, dbptr.dstqid, dbptr.fpsel,
			dbptr.h0_info_msb, dbptr.h0_info,
			dbptr.h1_info_msb, dbptr.h1_info);

	if (apm_dbptr_alloc(iport, 1, &dbptr)) {
		ENET_ERROR_OFFLOAD("apm_dbptr_alloc failed !!\n");
		if (result_ptr_entry->index >= MAX_DB_ENTRIES) {
			apm_ethoffload_ops.arp_unuse_entries--;
			apm_del_enet_offload_entry(result_ptr_entry);
		} else if (apm_put_unuse_result_ptr(result_ptr_entry->index) ==
				APM_RC_OK) {
			apm_del_enet_offload_entry(result_ptr_entry);
		}
		rc = APM_RC_ERROR;
		goto _ret_mk_arp;
	}

	if (result_ptr_entry->index >= MAX_DB_ENTRIES)
		result_ptr_entry->index = dbptr.index;

	if (rc == APM_RC_MISS) {
		apm_ethoffload_ops.arp_cache_entries++;
		apm_ethoffload_ops.arp_unuse_entries--;
	}

	if (!result_ptr_entry->ref_count) {
		if (apm_set_routes(rt_gateway, eport, result_ptr_entry))
			goto _ret_mk_arp;
	}

	rc = APM_RC_OK;

_ret_mk_arp:
	apm_cle_unlock(avl, flags);
	return rc;
}

int apm_rm_arp(__be32 rt_gateway, u8 *rt_mac, u32 eport)
{
	int rc = APM_RC_ERROR;
	u32 key[OFFLOAD_KEY_SIZE];
	unsigned long flags;

	key[0] = rt_gateway;     /* Gateway IP */
	memcpy(&key[1], rt_mac, IFHWADDRLEN);
	key[2] = (key[2] & ~0xFFFF);
	ENET_DEBUG_OFFLOAD("%s: result_pointer key[0] %08x key[1] %08x "
			"key[2] %08x\n", __func__, key[0], key[1], key[2]);

	apm_cle_lock(avl, flags);
	rc = apm_del_arp(eport, key);
	apm_cle_unlock(avl, flags);

	return rc;
}

static int apm_fl_arps(u32 eport, u32 *key)
{
	int rc = APM_RC_OK;
	int search_all_ports = 0;
	unsigned long flags;

	if (eport > MAX_NO_OF_PORTS)
		return -EINVAL;

	if (eport == MAX_NO_OF_PORTS)
		search_all_ports = 1;

	apm_cle_lock(avl, flags);

	if (!apm_ethoffload_ops.arp_cache_entries &&
			!apm_ethoffload_ops.arp_unuse_entries)
		goto _ret_fl_arps;

	if (apm_ethoffload_ops.arp_unuse_entries) {
		search_all_ports = 1;
		eport = MAX_NO_OF_PORTS;
	}

_start_fl_arps:
	if (search_all_ports)
		eport--;

	if (apm_del_arp(eport, key)) {
		ENET_ERROR_OFFLOAD("Flush Result Pointer error "
				"for port %d\n", eport);
		rc |= APM_RC_ERROR;
	}

	if (search_all_ports && eport)
		goto _start_fl_arps;

_ret_fl_arps:
	apm_cle_unlock(avl, flags);

	return rc;
}

static void apm_show_arp(u32 eport)
{
	u32 unuse_result_ptr = 0;
	int search_all_ports = 0;
	struct list_head *head;
	struct apm_enet_offload_list *entry;
	struct apm_result_ptr_list *r_entry;
	char arp_entries[10];

	head = &unuse_result_ptr_head;
	printk("Showing unuse allocated result pointer indices\n");
	if (!(INVALID_LIST_HEAD(head))) {
		list_for_each_entry(r_entry, head, node) {
			printk("%4d ", r_entry->index);
			unuse_result_ptr++;
		}
		if (unuse_result_ptr)
			printk("\n");
	}
	printk("Total unuse allocated result pointer %d\n", unuse_result_ptr);

	if (eport > MAX_NO_OF_PORTS)
		return;

	if (eport == MAX_NO_OF_PORTS)
		search_all_ports = 1;

	snprintf(arp_entries, sizeof(arp_entries), "%d_%d",
		apm_ethoffload_ops.arp_cache_entries, apm_ethoffload_ops.arp_unuse_entries);
	printk("Gateway         MAC Address        OutIf RefCnt | "
		"  InIf ResPtr DstQID Valid_%s\n", arp_entries);
_start_show_arp:
	if (search_all_ports)
		eport--;

	head = &ipfwd_result_ptr_head[eport];

	if (INVALID_LIST_HEAD(head))
		goto _skip_show_arp;

	list_for_each_entry(entry, head, node) {
		char gateway[16];
		__be32 rt_gateway = entry->key[0];
		u8 *rt_mac = (u8 *)&entry->key[1];
		u32 index = entry->index;
		u32 valid = ((index < MAX_DB_ENTRIES) ?
				get_shadow_cle_dbptrs()[index].index :
				0);
		u32 iport = ((entry->key[2] & 0xCC50) == 0xCC50) ?
				entry->key[2] & 0xF :
				-1;
		char *iport_devname = (iport < MAX_NO_OF_PORTS) ?
				devices[iport]->name :
				NULL;
		u32 dst_qid = ((index < MAX_DB_ENTRIES) ?
				get_shadow_cle_dbptrs()[index].dstqid :
				-1);
		valid = (!is_zero_ether_addr(rt_mac) && iport_devname) ?
				valid : 0;
		sprintf(gateway, "%pI4", &rt_gateway);
		printk("%-15s %02x:%02x:%02x:%02x:%02x:%02x %6s %6d | "
			"%6s %6d %6d %s\n",
			gateway, 
			rt_mac[0], rt_mac[1], rt_mac[2],
			rt_mac[3], rt_mac[4], rt_mac[5],
			devices[eport]->name,
			entry->ref_count,
			iport_devname,
			index,
			dst_qid,
			valid ? "Yes" : "No");
	}

_skip_show_arp:
	if (search_all_ports && eport)
		goto _start_show_arp;
}

static int apm_mk_route(__be32 rt_dst, __be32 rt_src, __be32 rt_gateway, u32 eport)
{
	int rc = APM_RC_ERROR;
	struct apm_enet_offload_list *result_ptr_entry = NULL;
	struct apm_enet_offload_list *search_str_entry = NULL;
	unsigned long flags;

	apm_cle_lock(avl, flags);
	rc = apm_get_route(rt_dst, rt_src, rt_gateway, eport,
		(void **)&search_str_entry, 1);

	if (rc != APM_RC_OK && rc != APM_CLE_AVL_SERACH_MISS) {
		ENET_ERROR_OFFLOAD("apm_cle_avl_search_node failed with "
				"rc %d !!\n", rc);
		goto _ret_mk_route;
	} else if (rc == APM_RC_OK) {
		goto _ret_mk_route;
	}

	if (search_str_entry == NULL) {
		ENET_ERROR_OFFLOAD("apm_add_enet_offload_entry "
			"search_str failed !!\n");
		rc = APM_RC_ERROR;
		goto _ret_mk_route;
	}

	if (apm_get_arp(rt_gateway, NULL, eport, -1,
			(void **)&result_ptr_entry, 0))
		goto _ret_mk_route;

	ENET_DEBUG_OFFLOAD("%s: For port %d ", __func__, eport);
	if (apm_add_route(result_ptr_entry, search_str_entry))
		goto _ret_mk_route;

	rc = APM_RC_OK;

_ret_mk_route:
	apm_cle_unlock(avl, flags);
	return rc;
}

static int apm_rm_route(__be32 rt_dst, __be32 rt_src, __be32 rt_gateway, u32 eport)
{
	int rc = APM_RC_OK;
	int search_all_ports = 0;
	struct apm_enet_offload_list *search_str_entry = NULL;
	unsigned long flags;

	if (eport > MAX_NO_OF_PORTS)
		return -EINVAL;

	if (eport == MAX_NO_OF_PORTS)
		search_all_ports = 1;

	apm_cle_lock(avl, flags);

	if (!apm_ethoffload_ops.route_cache_entries)
		goto _ret_rm_route;

_start_rm_route:
	if (search_all_ports)
		eport--;

	rc = apm_get_route(rt_dst, rt_src, rt_gateway, eport,
		(void **)&search_str_entry, 0);

	if (rc != APM_RC_OK && rc != APM_CLE_AVL_SERACH_MISS) {
		ENET_ERROR_OFFLOAD("apm_cle_avl_search_node failed with "
				"rc %d !!\n", rc);
		goto _skip_rm_route;
	}

	if (search_str_entry == NULL) {
		ENET_DEBUG_OFFLOAD("%s: Search String not found in list "
			"for port %d\n", __func__, eport);
		goto _skip_rm_route;
	}

	ENET_DEBUG_OFFLOAD("%s: For port %d ", __func__, eport);
	apm_del_route(search_str_entry);

_skip_rm_route:
	if (search_all_ports && eport)
		goto _start_rm_route;

_ret_rm_route:
	apm_cle_unlock(avl, flags);

	return rc;
}

static int apm_fl_routes(u32 eport)
{
	int rc = APM_RC_OK;
	int search_all_ports = 0;
	struct list_head *search_str_head;
	struct apm_enet_offload_list *search_str_entry, *next;
	unsigned long flags;

	if (eport > MAX_NO_OF_PORTS)
		return -EINVAL;

	if (eport == MAX_NO_OF_PORTS)
		search_all_ports = 1;

	apm_cle_lock(avl, flags);

	if (!apm_ethoffload_ops.route_cache_entries &&
			!apm_ethoffload_ops.route_unuse_entries)
		goto _ret_fl_routes;

	if (apm_ethoffload_ops.route_unuse_entries) {
		search_all_ports = 1;
		eport = MAX_NO_OF_PORTS;
	}

_start_fl_routes:
	if (search_all_ports)
		eport--;

	search_str_head = &ipfwd_search_str_head[eport];

	if (INVALID_LIST_HEAD(search_str_head)) {
		ENET_ERROR_OFFLOAD("Search String list error "
				"for port %d\n", eport);
		rc |= APM_RC_ERROR;
		goto _skip_fl_routes;
	}

	if (list_empty(search_str_head))
		goto _skip_fl_routes;

	list_for_each_entry_safe(search_str_entry, next, search_str_head, node) {
		ENET_DEBUG_OFFLOAD("%s: For port %d ", __func__, eport);
		apm_del_route(search_str_entry);
	}

_skip_fl_routes:
	if (search_all_ports && eport)
		goto _start_fl_routes;

_ret_fl_routes:
	apm_cle_unlock(avl, flags);

	return rc;
}

static void apm_show_route(u32 eport)
{
	int search_all_ports = 0;
	struct list_head *search_str_head;
	struct apm_enet_offload_list *entry;
	char route_entries[10];

	if (eport > MAX_NO_OF_PORTS)
		return;

	if (eport == MAX_NO_OF_PORTS)
		search_all_ports = 1;

	snprintf(route_entries, sizeof(route_entries), "%d_%d",
		apm_ethoffload_ops.route_cache_entries, apm_ethoffload_ops.route_unuse_entries);
	printk("Source          Destination     Gateway          OutIf AVL-Index Valid_%-9s | "
		"  InIf ResPtr DstQID Valid\n", route_entries);
_start_show_route:
	if (search_all_ports)
		eport--;

	search_str_head = &ipfwd_search_str_head[eport];

	if (INVALID_LIST_HEAD(search_str_head))
		goto _skip_show_route;

	list_for_each_entry(entry, search_str_head, node) {
		char src[16];
		char dst[16];
		char gateway[16];
		__be32 rt_src = le32_to_cpu(entry->key[0]);
		__be32 rt_dst = le32_to_cpu(entry->key[1]);
		__be32 rt_gateway = entry->key[2];
		u32 index = entry->index;
		struct apm_enet_offload_list *result_ptr_entry =
			entry->result_ptr_entry;
		u32 valid = ((index < AVL_SSIZE8B_DPTH) ?
				get_shadow_avl_nodes()[index].index :
				0);
		u32 iport = (result_ptr_entry &&
				((result_ptr_entry->key[2] & 0xCC50) == 0xCC50)) ?
				result_ptr_entry->key[2] & 0xF :
				-1;
		char *iport_devname = (iport < MAX_NO_OF_PORTS) ?
				devices[iport]->name :
				NULL;
		u32 rt_mac_valid = (iport_devname && (!is_zero_ether_addr(
				(u8 *)&result_ptr_entry->key[1]))) ? 1 : 0;
		u32 result_ptr = ((index < AVL_SSIZE8B_DPTH) ?
				get_shadow_avl_nodes()[index].result_pointer :
				-1);
		u32 result_ptr_valid = (rt_mac_valid &&
				(result_ptr < MAX_DB_ENTRIES) ?
				get_shadow_cle_dbptrs()[result_ptr].index :
				0);
		u32 dst_qid = ((result_ptr < MAX_DB_ENTRIES) ?
				get_shadow_cle_dbptrs()[result_ptr].dstqid :
				-1);
		sprintf(src, "%pI4", &rt_src);
		sprintf(dst, "%pI4", &rt_dst);
		sprintf(gateway, "%pI4", &rt_gateway);
		printk("%-15s %-15s %-15s %6s %9d %-15s | "
			"%6s %6d %6d %s\n",
			src, dst, gateway,
			devices[eport]->name,
			index,
			valid ? "Yes" : "No",
			iport_devname,
			result_ptr,
			dst_qid,
			result_ptr_valid ? "Yes" : "No");
	}

_skip_show_route:
	if (search_all_ports && eport)
		goto _start_show_route;
}

static inline void apm_list_head_ipv4_init(void)
{
	int i;

	if (ipfwd_list_head_init)
		return;

	INIT_LIST_HEAD(&unuse_result_ptr_head);

	for (i = PORT_1G_0; i < MAX_NO_OF_PORTS; i++) {
		struct apm_enet_dev_base *ipriv_dev = netdev_priv(devices[i]);

		iport_qm_queue[i] = &ipriv_dev->qm_queues[apm_cle_system_id];
		INIT_LIST_HEAD(&ipfwd_result_ptr_head[i]);
		INIT_LIST_HEAD(&ipfwd_search_str_head[i]);
	}

	ipfwd_list_head_init = 1;
}

static inline void apm_switch_ipv4_forward(u32 iport)
{
	struct apm_enet_dev_base *ipriv_dev = netdev_priv(devices[iport]);

	if (ipriv_dev->ethoffload == ETHOFFLOAD_IPV4_FORWARD) {
		char *ptree_id = apm_get_sys_ptree_id(iport);

		if (strncmp(ptree_id, CLE_PTREE_IPV4FWD, CLE_PTREE_ID_SIZE)) {
			struct apm_ptree_config *ptree_config =
				apm_find_ptree_config(iport, CLE_PTREE_IPV4FWD);
			ipriv_dev->in_poll_rx_msg_desc.is_msg16 = 0;
			apm_set_sys_ptree_config(iport, ptree_config);
		}
	}
}

static int apm_mkarp_ipv4_forward(struct neighbour *n)
{
	__be32 rt_gateway = cpu_to_be32(*(u32 *)n->primary_key);
	u8 *rt_mac = n->ha;
	u32 eport = ((struct apm_enet_dev_base *)netdev_priv(n->dev))->port_id;
	u32 iport = -1;

	ENET_DEBUG_OFFLOAD("%s: %s: "
		"rt_gateway %d.%d.%d.%d "
		"rt_mac %02x:%02x:%02x:%02x:%02x:%02x "
		"eport %d\n",
		__func__, n->dev->name,
		(rt_gateway & 0xFF000000) >> 24,
		(rt_gateway & 0x00FF0000) >> 16,
		(rt_gateway & 0x0000FF00) >>  8,
		(rt_gateway & 0x000000FF) >>  0,
		rt_mac[0], rt_mac[1], rt_mac[2],
		rt_mac[3], rt_mac[4], rt_mac[5],
		eport);

	if (is_zero_ether_addr(rt_mac))
		goto _ret_mkarp_ipv4_forward;

	apm_list_head_ipv4_init();

	if (apm_mk_arp(rt_gateway, rt_mac, eport, iport))
		goto _ret_mkarp_ipv4_forward;

	for (iport = PORT_1G_0; iport < MAX_NO_OF_PORTS; iport++)
		apm_switch_ipv4_forward(iport);

_ret_mkarp_ipv4_forward:
	return 0;
}

static void apm_rmarp_ipv4_forward(struct neighbour *n)
{
	__be32 rt_gateway = cpu_to_be32(*(u32 *)n->primary_key);
	u8 *rt_mac = n->ha;
	u32 eport = ((struct apm_enet_dev_base *)netdev_priv(n->dev))->port_id;

	ENET_DEBUG_OFFLOAD("%s: %s: "
		"rt_gateway %d.%d.%d.%d "
		"rt_mac %02x:%02x:%02x:%02x:%02x:%02x "
		"eport %d\n",
		__func__, n->dev->name,
		(rt_gateway & 0xFF000000) >> 24,
		(rt_gateway & 0x00FF0000) >> 16,
		(rt_gateway & 0x0000FF00) >>  8,
		(rt_gateway & 0x000000FF) >>  0,
		rt_mac[0], rt_mac[1], rt_mac[2],
		rt_mac[3], rt_mac[4], rt_mac[5],
		eport);

	apm_rm_arp(rt_gateway, rt_mac, eport);
}

static void apm_flarp_ipv4_forward(struct net_device *dev)
{
	u32 eport = ((struct apm_enet_dev_base *)netdev_priv(dev))->port_id;

	ENET_DEBUG_OFFLOAD("%s: flushing Result Pointers for port %d\n",
			__func__, eport);

	apm_fl_arps(eport, NULL);
}

static int apm_mkroute_ipv4_init(struct net_device *dev)
{
	struct apm_enet_dev_base *ipriv_dev = netdev_priv(dev);
	u32 iport = ipriv_dev->port_id;
	struct apm_ptree_config *ptree_config = apm_find_ptree_config(iport,
					CLE_PTREE_DEFAULT);
	struct ptree_kn kn;
	int i, rc = APM_RC_OK;

	struct ptree_branch branch[] = {
	{      0, 0xffff,   EQT, PTREE_ALLOC(0),  EW_BRANCH(1),  2, 0, 0, 0 },   /* 0-1 bytes of MAC Address */
	{      0, 0xffff,   EQT, PTREE_ALLOC(0),  EW_BRANCH(2),  4, 0, 0, 0 },   /* 2-3 bytes of MAC Address */
	{      0, 0xffff,   EQT, PTREE_ALLOC(0),  EW_BRANCH(3), 12, 0, 0, 0 },   /* 4-5 bytes of MAC Address */
	{      0, 0x0800,   EQT, PTREE_ALLOC(0),  EW_BRANCH(4), 14, 0, 0, 0 },   /* Packet type IPv4 */
	{ 0x00ff, 0x4500,   EQT, PTREE_ALLOC(0),  EW_BRANCH(5), 22, 0, 0, 0 },   /* IPv4 version and Header Length */
	{ 0x00ff, 0x0200, LTEQT, PTREE_ALLOC(1), FW0_BRANCH(0), 26, 0, 0, 0 },   /* 2 <= TTL */
	{ 0xffff, 0x0000,   EQT, PTREE_ALLOC(1), FW0_BRANCH(1), 28, 0, 0, 0 },   /* Src IP Address 0-1 */
	{ 0xffff, 0x0000,   EQT, PTREE_ALLOC(1), FW0_BRANCH(2), 30, 0, 0, 0 },   /* Src IP Address 2-3 */
	{ 0xffff, 0x0000,   EQT, PTREE_ALLOC(1), FW0_BRANCH(3), 32, 0, 0, 0 },   /* Dst IP Address 0-1 */
	{ 0xffff, 0x0000,   EQT, PTREE_ALLOC(1), FW1_BRANCH(0),  0, 0, 0, 0 },   /* Dst IP Address 2-3 */
	{ 0xffff, 0x0000,   EQT, PTREE_ALLOC(2),  KEY_INDEX(0),  0, 0, 0, 0 },   /* Allow all of the above */
	};

	struct ptree_dn dn[] = {
	{ START_NODE,   DBPTR_DROP(0), AVL_SEARCH(NO_BYTE),    0, 0, 0, 6, &branch[0] },
	{ INTERIM_NODE, DBPTR_DROP(0), AVL_SEARCH(BOTH_BYTES), 0, 0, 0, 4, &branch[6] },
	{ LAST_NODE,    DBPTR_DROP(0), AVL_SEARCH(NO_BYTE),    0, 0, 0, 1, &branch[10] },
	};

	struct ptree_node node[] = {
	{ PTREE_ALLOC(0), EWDN, 0, (struct ptree_dn*)&dn[0] },
	{ PTREE_ALLOC(1), FWDN, 0, (struct ptree_dn*)&dn[1] },
	{ PTREE_ALLOC(1), FWDN, 1, (struct ptree_dn*)&dn[2] },
	{ PTREE_ALLOC(2),   KN, 0,    (struct ptree_kn*)&kn },
	};

	if (apm_mkroute_ipv4_init_done[iport])
		goto _ret_mkroute_ipv4_init;

	/* Update Default Result Pointer in decision node */
	for (i = 0; i < ARRAY_SIZE(dn); i++)
		dn[i].result_pointer = ptree_config->start_dbptr;

	/* Update MAC Address in branch rule */
	for (i = 0; i < 3; i++)
		branch[i].data = (dev->dev_addr[i * 2] << 8) |
					dev->dev_addr[(i * 2) + 1];

	kn.priority = 1;
	kn.result_pointer = ptree_config->start_dbptr;

	ptree_config = apm_add_ptree_config(iport, CLE_PTREE_IPV4FWD);
	if ((rc = apm_ptree_alloc(iport, ARRAY_SIZE(node), 0, &node[0], NULL,
			ptree_config)) != APM_RC_OK) {
		ENET_ERROR_OFFLOAD("apm_ptree_alloc error %d "
				"for port %d\n", rc, iport);
		rc = APM_RC_ERROR;
		goto _ret_mkroute_ipv4_init;
	}
	ptree_config->start_pkt_ptr = 0;
	ptree_config->max_hop = 16;

	/* Initialize Search Engine, 8 Byte search string for SRC IP and DST IP */
	if ((rc = apm_cle_avl_init(SEARCH_STRING_SIZE8B)) != APM_RC_OK) {
		ENET_ERROR_OFFLOAD("apm_cle_avl_init error %d "
				"for port %d\n", rc, iport);
		rc = APM_RC_ERROR;
		goto _ret_mkroute_ipv4_init;
	}

	apm_mkroute_ipv4_init_done[iport] = 1;
	apm_list_head_ipv4_init();

_ret_mkroute_ipv4_init:
	return rc;
}

static int apm_mkroute_ipv4_forward(struct sk_buff *skb, struct rtable *rth)
{
	struct apm_enet_dev_base *ipriv_dev = NULL, *epriv_dev = NULL;
	int iport = -1, eport = -1;
	int rc = APM_RC_OK;
	__be32 rt_dst = rth->rt_dst;
	__be32 rt_src = rth->rt_src;
	__be32 rt_gateway = rth->rt_gateway;
	u8 *rt_mac = (rth->u.dst.neighbour) ? (rth->u.dst.neighbour->ha) : NULL;

	ENET_DEBUG_OFFLOAD("%s: %s -> %s: "
		"rt_dst %d.%d.%d.%d "
		"rt_src %d.%d.%d.%d "
		"rt_gateway %d.%d.%d.%d\n",
		__func__, skb->dev->name, rth->u.dst.dev->name,
		(rt_dst & 0xFF000000) >> 24,
		(rt_dst & 0x00FF0000) >> 16,
		(rt_dst & 0x0000FF00) >>  8,
		(rt_dst & 0x000000FF) >>  0,
		(rt_src & 0xFF000000) >> 24,
		(rt_src & 0x00FF0000) >> 16,
		(rt_src & 0x0000FF00) >>  8,
		(rt_src & 0x000000FF) >>  0,
		(rt_gateway & 0xFF000000) >> 24,
		(rt_gateway & 0x00FF0000) >> 16,
		(rt_gateway & 0x0000FF00) >>  8,
		(rt_gateway & 0x000000FF) >>  0);

	if (rth->u.dst.neighbour == NULL) {
		ENET_DEBUG_OFFLOAD("%s: neighbour is NULL, "
			"ignoring\n", __func__);
		goto _ret_mkroute_ipv4_forward;
	} else if (is_broadcast_ether_addr(rt_mac)) {
		ENET_DEBUG_OFFLOAD("%s: neighbour mac ff:ff:ff:ff:ff:ff, "
			"ignoring\n", __func__);
		goto _ret_mkroute_ipv4_forward;
	}

	ENET_DEBUG_OFFLOAD("%s: neighbour mac %02x:%02x:%02x:%02x:%02x:%02x\n",
		__func__,
		rt_mac[0], rt_mac[1], rt_mac[2], rt_mac[3], rt_mac[4], rt_mac[5]);

	ipriv_dev = netdev_priv(skb->dev);
	iport = ipriv_dev->port_id;

	epriv_dev = netdev_priv(rth->u.dst.dev);
	eport = epriv_dev->port_id;

	if (!apm_mkroute_ipv4_init_done[iport]) {
		if (apm_mkroute_ipv4_init(skb->dev))
			goto _ret_mkroute_ipv4_forward;
	}

	rc = apm_mk_route(rt_dst, rt_src, rt_gateway, eport);

	if (rc == APM_RC_OK)
		goto _switch_ipv4_forward;

	if (rc != APM_CLE_AVL_SERACH_MISS)
		goto _ret_mkroute_ipv4_forward;

	if (apm_mk_arp(rt_gateway, rt_mac, eport, iport))
		goto _ret_mkroute_ipv4_forward;

_switch_ipv4_forward:
	apm_switch_ipv4_forward(iport);

_ret_mkroute_ipv4_forward:
	return 0;
}

static void apm_rmroute_ipv4_forward(struct rtable *rth)
{
	struct apm_enet_dev_base *epriv_dev = NULL;
	int eport = -1;
	__be32 rt_dst = rth->rt_dst;
	__be32 rt_src = rth->rt_src;
	__be32 rt_gateway = rth->rt_gateway;
	u8 *rt_mac = (rth->u.dst.neighbour) ? (rth->u.dst.neighbour->ha) : NULL;

	ENET_DEBUG_OFFLOAD("%s: %s: "
		"rt_dst %d.%d.%d.%d "
		"rt_src %d.%d.%d.%d "
		"rt_gateway %d.%d.%d.%d\n",
		__func__, rth->u.dst.dev->name,
		(rt_dst & 0xFF000000) >> 24,
		(rt_dst & 0x00FF0000) >> 16,
		(rt_dst & 0x0000FF00) >>  8,
		(rt_dst & 0x000000FF) >>  0,
		(rt_src & 0xFF000000) >> 24,
		(rt_src & 0x00FF0000) >> 16,
		(rt_src & 0x0000FF00) >>  8,
		(rt_src & 0x000000FF) >>  0,
		(rt_gateway & 0xFF000000) >> 24,
		(rt_gateway & 0x00FF0000) >> 16,
		(rt_gateway & 0x0000FF00) >>  8,
		(rt_gateway & 0x000000FF) >>  0);

	if (rth->u.dst.neighbour == NULL) {
		ENET_DEBUG_OFFLOAD("%s: neighbour is NULL, "
			"ignoring\n", __func__);
		goto _ret_rmroute_ipv4_forward;
	} else if (is_broadcast_ether_addr(rt_mac)) {
		ENET_DEBUG_OFFLOAD("%s: neighbour mac ff:ff:ff:ff:ff:ff, "
			"ignoring\n", __func__);
		goto _ret_rmroute_ipv4_forward;
	}

	ENET_DEBUG_OFFLOAD("%s: neighbour mac %02x:%02x:%02x:%02x:%02x:%02x\n",
		__func__,
		rt_mac[0], rt_mac[1], rt_mac[2], rt_mac[3], rt_mac[4], rt_mac[5]);

	epriv_dev = netdev_priv(rth->u.dst.dev);
	eport = epriv_dev->port_id;
	apm_rm_route(rt_dst, rt_src, rt_gateway, eport);

_ret_rmroute_ipv4_forward:
	return;
}

static void apm_flroute_ipv4_forward(struct net_device *dev)
{
	__be32 key = 0xFFFFFFFF;

	ENET_DEBUG_OFFLOAD("%s: flusing AVL Entries for port %d\n",
			__func__,
			((struct apm_enet_dev_base *)netdev_priv(dev))->port_id);
	apm_fl_routes(MAX_NO_OF_PORTS);
	apm_fl_arps(MAX_NO_OF_PORTS, &key);
}

struct apm_ethoffload_mode {
	const char  *name;
};

static struct apm_ethoffload_mode apm_ethoffload_mode[ETHOFFLOAD_MAX] = {
	[ETHOFFLOAD_DEFAULT]		= { "default" },
	[ETHOFFLOAD_IPV4_FORWARD]	= { "ipv4_forward" },
	[ETHOFFLOAD_ADD]		= { "add" },
	[ETHOFFLOAD_DEL]		= { "del" },
	[ETHOFFLOAD_SHOW]		= { "show" },
};

static ssize_t apm_ethoffload_store(struct net_device *dev, const char *buf, size_t len)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(dev);
	u32 iport = priv_dev->port_id;
	int i;
	char strbuf[128];
	size_t ret = -EINVAL;
	struct apm_ptree_config *ptree_config = NULL;

	if (len > 128)
		goto _ret_ethoffload_store;

	memcpy(strbuf, buf, len);
	strbuf[len-1] = '\0';

	for (i = 0; i < ETHOFFLOAD_MAX; i++) {
		u8 src_ipv4[4], dst_ipv4[4], gwy_ipv4[4];
		__be32 rt_src, rt_dst, rt_gateway;
		u32 eport;
		char dev_name[IFNAMSIZ];

		if (strcmp(strbuf, apm_ethoffload_mode[i].name) == 0) {
			ret = len;
			if (priv_dev->ethoffload != i)
				goto ethoffload_update;
			else
				break;
		} else if (strncmp(strbuf, apm_ethoffload_mode[ETHOFFLOAD_ADD].name, 3) == 0) {
			sscanf(strbuf, "add "
				"%hhu.%hhu.%hhu.%hhu "
				"%hhu.%hhu.%hhu.%hhu "
				"%hhu.%hhu.%hhu.%hhu "
				"%s",
				&src_ipv4[0], &src_ipv4[1],
				&src_ipv4[2], &src_ipv4[3],
				&dst_ipv4[0], &dst_ipv4[1],
				&dst_ipv4[2], &dst_ipv4[3],
				&gwy_ipv4[0], &gwy_ipv4[1],
				&gwy_ipv4[2], &gwy_ipv4[3],
				dev_name);

			rt_src = (src_ipv4[0] << 24) |
				(src_ipv4[1] << 16) |
				(src_ipv4[2] <<  8) |
				(src_ipv4[3] <<  0);

			rt_dst = (dst_ipv4[0] << 24) |
				(dst_ipv4[1] << 16) |
				(dst_ipv4[2] <<  8) |
				(dst_ipv4[3] <<  0);

			rt_gateway = (gwy_ipv4[0] << 24) |
				(gwy_ipv4[1] << 16) |
				(gwy_ipv4[2] <<  8) |
				(gwy_ipv4[3] <<  0);

			eport = find_port(dev_name);
			ENET_DEBUG_OFFLOAD("%s: Adding to AVL search_string "
				"key[0] %08x key[1] %08x gateway %08x "
				"for out port %d\n", __func__,
				cpu_to_le32(rt_src), cpu_to_le32(rt_dst),
				rt_gateway, eport);

			apm_mk_route(rt_dst, rt_src, rt_gateway, eport);

			ret = len;
			break;
		} else if (strncmp(strbuf, apm_ethoffload_mode[ETHOFFLOAD_DEL].name, 3) == 0) {
			sscanf(strbuf, "del "
				"%hhu.%hhu.%hhu.%hhu "
				"%hhu.%hhu.%hhu.%hhu "
				"%hhu.%hhu.%hhu.%hhu "
				"%s",
				&src_ipv4[0], &src_ipv4[1],
				&src_ipv4[2], &src_ipv4[3],
				&dst_ipv4[0], &dst_ipv4[1],
				&dst_ipv4[2], &dst_ipv4[3],
				&gwy_ipv4[0], &gwy_ipv4[1],
				&gwy_ipv4[2], &gwy_ipv4[3],
				dev_name);

			rt_src = (src_ipv4[0] << 24) |
				(src_ipv4[1] << 16) |
				(src_ipv4[2] <<  8) |
				(src_ipv4[3] <<  0);

			rt_dst = (dst_ipv4[0] << 24) |
				(dst_ipv4[1] << 16) |
				(dst_ipv4[2] <<  8) |
				(dst_ipv4[3] <<  0);

			rt_gateway = (gwy_ipv4[0] << 24) |
				(gwy_ipv4[1] << 16) |
				(gwy_ipv4[2] <<  8) |
				(gwy_ipv4[3] <<  0);

			eport = find_port(dev_name);
			ENET_DEBUG_OFFLOAD("%s: Deleting from AVL search_string "
				"key[0] %08x key[1] %08x gateway %08x "
				"for out port %d\n", __func__,
				cpu_to_le32(rt_src), cpu_to_le32(rt_dst),
				rt_gateway, eport);

			apm_rm_route(rt_dst, rt_src, rt_gateway, eport);

			ret = len;
			break;
		} else if (strncmp(strbuf, apm_ethoffload_mode[ETHOFFLOAD_SHOW].name, 4) == 0) {
			char command[8];

			memset(command, 0, sizeof(command));
			memset(dev_name, 0, sizeof(dev_name));
			sscanf(strbuf, "show %s %s", command, dev_name);
			eport = find_port(dev_name);
			if (eport > MAX_NO_OF_PORTS)
				eport = MAX_NO_OF_PORTS;

			if (strncmp(command, "arp", 3) == 0)
				apm_show_arp(eport);
			else if (strncmp(command, "route", 5) == 0)
				apm_show_route(eport);

			ret = len;
			break;
		}
	}

	goto _ret_ethoffload_store;

ethoffload_update:
	ptree_config = apm_find_ptree_config(iport, CLE_PTREE_DEFAULT);
	if (ptree_config == NULL) {
		printk("Bring up %s interface and try again\n", dev->name);
		goto _ret_ethoffload_store;
	}

	switch (i) {
	case ETHOFFLOAD_IPV4_FORWARD:
		ENET_DEBUG_OFFLOAD("ETHOFFLOAD_IPV4_FORWARD\n");
		if (!apm_mkroute_ipv4_init_done[iport]) {
			if (apm_mkroute_ipv4_init(dev)) {
				goto _ret_ethoffload_store;
			}
		}
		ptree_config = apm_find_ptree_config(iport, CLE_PTREE_IPV4FWD);
		priv_dev->in_poll_rx_msg_desc.is_msg16 = 0;
		break;
	case ETHOFFLOAD_DEFAULT:
	default:
		ENET_DEBUG_OFFLOAD("ETHOFFLOAD_DEFAULT\n");
		break;
	}

	apm_set_sys_ptree_config(iport, ptree_config);
	priv_dev->ethoffload = i;

_ret_ethoffload_store:
	return ret;
}

static ssize_t apm_ethoffload_show(struct net_device *dev, char *buf)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(dev);
	char *s = buf;
	int i;

	for (i = 0; i < ETHOFFLOAD_MAX; i++) {
		if (priv_dev->ethoffload == i)
			s += sprintf(s, "[%s] ", apm_ethoffload_mode[i].name);
		else
			s += sprintf(s, "%s ", apm_ethoffload_mode[i].name);
	}

	*(s-1) = '\n'; /* convert the last space to a newline */

	return (s - buf);
}

struct ethoffload_ops apm_ethoffload_ops = {
	.ethoffload_interfaces = 0,
	.store_offload = apm_ethoffload_store,
	.show_offload = apm_ethoffload_show,
	.mkarp_ipv4_forward = apm_mkarp_ipv4_forward,
	.rmarp_ipv4_forward = apm_rmarp_ipv4_forward,
	.flarp_ipv4_forward = apm_flarp_ipv4_forward,
	.arp_cache_entries = 0,
	.arp_unuse_entries = 0,
	.mkroute_ipv4_forward = apm_mkroute_ipv4_forward,
	.rmroute_ipv4_forward = apm_rmroute_ipv4_forward,
	.flroute_ipv4_forward = apm_flroute_ipv4_forward,
	.route_cache_entries = 0,
	.route_unuse_entries = 0,
};

EXPORT_SYMBOL(apm_ethoffload_ops);
