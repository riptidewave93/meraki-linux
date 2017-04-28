/**
 * AppliedMicro APM862xx SoC Classifier Driver
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
 * @file apm_avl_api.c
 *
 * This file implements Classifier AVL Search Engine APIs.
 *
 */

#include "apm_preclass_data.h"
#include "apm_avl_base.h"
#include "apm_avl_api.h"

#ifdef CLE_SHADOW
static int avl_shadow_done = 0;
static struct avl_node sys_avl_node[AVL_SSIZE8B_DPTH];

static inline void apm_avl_shadow_init(void)
{
	if (!avl_shadow_done) {
		int i;
		memset(&sys_avl_node, 0,
			sizeof(sys_avl_node));
		for (i = 0; i < AVL_SSIZE8B_DPTH; i++)
			sys_avl_node[i].index = -1;
		avl_shadow_done = 1;
	}
}

void apm_avl_validate_node(u32 node_index)
{
	if (node_index >= 0 && node_index < AVL_SSIZE8B_DPTH)
		sys_avl_node[node_index].index = 1;
}

void apm_avl_invalidate_node(u32 node_index)
{
	if (node_index >= 0 && node_index < AVL_SSIZE8B_DPTH)
		sys_avl_node[node_index].index = 0;
}

struct avl_node *get_shadow_avl_nodes(void)
{
	return &sys_avl_node[0];
}
#endif

int apm_cle_avl_add_node(struct avl_node *node)
{
	int rc;
	u32 index;
	u32 node_value[REGS_PER_AVL_ADD];
	int i;
	node_value[0] = ((u32)(node->result_pointer
				& AVL_DB_PTR_MASK)) |
				(((u32)(node->priority &
				AVL_PRI_MASK)) << 10) |
				(node->search_key[0] << 13);
	for (i = 1; i < 8; i++) {
		node_value[i] = (node->search_key[i - 1] >> 19) |
			(node->search_key[i] << 13) ;
	}
	node_value[8] = (node->search_key[7] >> 19);
	if ((rc = apm_cle_avl_add(node_value, &index) != APM_RC_OK)) {
		PCLS_ERR("Error %d In AVL Add node operation \n", rc);
	}

#ifdef CLE_SHADOW
	if (rc == APM_RC_OK && index >= 0 && index < AVL_SSIZE8B_DPTH) {
		apm_avl_shadow_init();

		/* index field Will be used for cle_dbptr validity by user */
		node->index = 1;
		memcpy(&sys_avl_node[index], node,
			sizeof(struct avl_node));
	}
#endif

	/* Putting back the index vaule */
	node->index = index;

	return rc;
}

int apm_cle_avl_del_node(struct avl_node *node)
{
	int rc = apm_cle_avl_del(node->search_key, &node->index);

	if (rc != APM_RC_OK) {
		PCLS_ERR("Error %d In AVL Del node operation \n", rc);
	}

	return rc;
}

int apm_cle_avl_search_node(struct avl_node *node)
{
	int rc = apm_cle_avl_search(node->search_key, &node->index,
			&node->result_pointer);

	if (rc != APM_RC_OK && rc != APM_CLE_AVL_SERACH_MISS) {
		PCLS_ERR("Error %d In AVL Search node operation \n", rc);
	}

	return rc;
}

int apm_cle_avl_write_node(struct avl_node *node)
{
	int rc;
	u32 node_value[REGS_PER_AVL_NODE];
	int i;

	node_value[0] = (node->balance & AVL_BAL_MASK) |
			((node->right_address & AVL_RADR_MASK) << 3) |
			((node->left_address & AVL_LADR_MASK) << 14) |
			((node->result_pointer & 0x7F) << 25);
							/* result_pointer_7b */
	node_value[1] = ((node->result_pointer >> 7) & 0x7) |
							/* result_pointer_3b */
			((node->priority & AVL_PRI_MASK) << 3) |
			((node->search_key[0] & 0x3FFFFFF) << 6);
							/* key_0_26b */
	for (i = 2; i < 9; i++) {
		node_value[i] = ((node->search_key[i - 2]
				>> 26) & 0x3F) |
				((node->search_key[i - 1]
				& 0x3FFFFFF) << 6);
	}
	node_value[9] = ((node->search_key[7] >> 26) & 0x3F);
	if ((rc = apm_cle_avl_wr(node_value, node->index) != APM_RC_OK)) {
		PCLS_ERR("Error %d In AVL Write node operation \n", rc);
	}

#ifdef CLE_SHADOW
	apm_avl_shadow_init();
	if (node->index >= 0 && node->index < AVL_SSIZE8B_DPTH)
		memcpy(&sys_avl_node[node->index], node,
			sizeof(struct avl_node));
#endif

	return rc;
}

int apm_cle_avl_read_node(struct avl_node *node)
{
	int rc;
	u32 node_value[REGS_PER_AVL_NODE];
	int i;

	if ((rc = apm_cle_avl_rd(node_value, node->index) != APM_RC_OK)) {
		PCLS_ERR("Error %d In AVL Read node operation \n", rc);
		return rc;
	}
	node->balance = (node_value[0] & AVL_BAL_MASK);
	node->right_address = ((node_value[0] >> 3) & AVL_RADR_MASK);
	node->left_address = ((node_value[0] >> 14) & AVL_LADR_MASK);
	node->result_pointer = ((node_value[0] >> 25) & 0x7F) |
				((node_value[1] << 7)  & 0x7);
	node->priority = ((node_value[1] >> 3) & AVL_PRI_MASK);
	for (i = 0; i < 8; i++) {
		node->search_key[0] = ((node_value[i + 1] >> 6) & 0x3FFFFFF) |
				((node_value[i + 2] << 26) & 0x3F);
	}

#ifdef CLE_SHADOW
	apm_avl_shadow_init();
	if (node->index >= 0 && node->index < AVL_SSIZE8B_DPTH)
		memcpy(&sys_avl_node[node->index], node,
			sizeof(struct avl_node));
#endif

	PCLS_DBG("balance[%d], right_addres[0X%08X],"
		" left_addres[0X%08X], result_pointer[%d] \n",
		node->balance, node->right_address,
		node->left_address, node->result_pointer);
	return rc;
}

int apm_cle_avl_get_status(struct apm_cle_avl_status *avl_status)
{
	int rc;
	u32 status;
	if((rc = apm_gbl_cle_rd32(AVL_STATUS_ADDR, &status)
		!= APM_RC_OK)) {
		PCLS_ERR("Error %d In AVL get status operation \n", rc);
	} else {
		avl_status->root_addr = ROOT_ADDR_RD(status);
		avl_status->node_cnt  = NODE_CNT_RD(status);
		PCLS_DBG("root_addr[0X%08X], node_cnt [%d] \n",
			avl_status->root_addr, avl_status->node_cnt);
	}
	return rc;
}

int apm_cle_avl_config(enum apm_cle_avl_search_str_type key_size)
{
	int rc;
	u32 avl_config = 0;
	u32 srchdb_depth;
	
	switch (key_size) {
	case SEARCH_STRING_SIZE8B:
		srchdb_depth = AVL_SSIZE8B_DPTH;
		break;
	case SEARCH_STRING_SIZE16B:
		srchdb_depth = AVL_SSIZE16B_DPTH;
		break;
	case SEARCH_STRING_SIZE32B:
		srchdb_depth = AVL_SSIZE32B_DPTH;
		break;
	default:
		PCLS_ERR("Error: Invalid search key_size \n");
		return APM_RC_INVALID_PARM;
		break;
	}
	avl_config |= MAXSTEPS_THRESH_SET(avl_config, AVL_MAX_STEPS_THRS);
	avl_config |= SRCHDB_DEPTH_SET(avl_config, srchdb_depth);
	avl_config |= STRING_SIZE_SET(avl_config, key_size);
	
	PCLS_DBG("key_size[%d], srchdb_depth[%d], avl_config[0X%08X] \n",
		key_size, srchdb_depth, avl_config);
	rc = apm_gbl_cle_wr32(AVL_CONFIG_ADDR, avl_config);
	return rc;
}

int apm_cle_avl_init(enum apm_cle_avl_search_str_type key_size)
{
	int rc = APM_RC_OK;
	u32 int_mask;
	static int avl_init_done = 0;
	static enum apm_cle_avl_search_str_type init_key_size = SEARCH_STRING_RSVD;

	if (avl_init_done && (key_size == init_key_size))
		return rc;

	if ((rc = apm_cle_avl_config(key_size)) != APM_RC_OK) {
		PCLS_ERR("Error %d In CLE AVL Init Operation \n", rc);
		return rc;
	}
	int_mask = FATAL_WRSELFPOINTERRMASK_WR(0) |
		FATAL_RDSELFPOINTERRMASK_WR(0)	  |
		FATAL_WRBALERRMASK_WR(0)	  |
		FATAL_RDBALERRMASK_WR(0)	  |
		FATAL_MAXSTEPSERRMASK_WR(0)	  |
		FATAL_RDERR_RAMMASK_WR(0)	  |
		RDERR_RAMMASK_WR(0)		  |  
		DELERR_RIDNOTFOUNDMASK_WR(0)	  |
		DELERR_EMPTYMASK_WR(0)		  |
		ADDERR_RIDDUPLICATEMASK_WR(0)	  | 
		ADDERR_FULLMASK_WR(0);
	rc = apm_gbl_cle_wr32(AVL_SEARCH_INTMASK_ADDR, int_mask);

	if (rc == APM_RC_OK) {
		avl_init_done = 1;
		init_key_size = key_size;
#ifdef CLE_SHADOW
		avl_shadow_done = 0;
		apm_avl_shadow_init();
#endif
	}

	return rc;
}
