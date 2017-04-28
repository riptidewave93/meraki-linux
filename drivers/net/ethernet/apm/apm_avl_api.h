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
 * @file apm_avl_api.h
 *
 * This file defines Classifier AVL Search Engine APIs and data structures.
 *
 */

#ifndef _APM_AVL_API_H_
#define _APM_AVL_API_H_

enum apm_cle_avl_search_str_type {
	SEARCH_STRING_SIZE8B,
	SEARCH_STRING_SIZE16B,
	SEARCH_STRING_SIZE32B,
	SEARCH_STRING_RSVD
};

#define AVL_MAX_SEARCH_STRING	8	/* 256 bits */
#define AVL_MAX_STEPS_THRS	4095
#define AVL_SSIZE8B_DPTH	1535
#define AVL_SSIZE16B_DPTH	1023
#define AVL_SSIZE32B_DPTH	511
#define AVL_DB_PTR_MASK		0X3FF
#define AVL_PRI_MASK		0X7
#define AVL_BAL_MASK		0X7
#define AVL_LADR_MASK		0X7FF
#define AVL_RADR_MASK		0X7FF
#define AVL_NODES_PER_SYSTEM	(AVL_SSIZE8B_DPTH / apm_cle_systems)

struct avl_node {
	u32 index;		/**< AVL Node index */
	u32 search_key[AVL_MAX_SEARCH_STRING];
				/**< Search string to be matched 256 bits */
	u8 priority;		/**< 3b: Priority */
	u32 result_pointer;	/**< 10b: address/pointer to Database RAM */
	u16 left_address;	/**< 11b: The pointer to the next entry
					on the left side in the AVL tree  */
	u16 right_address;	/**< 11b: The pointer to the next entry
					on the right side in the AVL tree */
	u8 balance;		/**< 3b: The difference in the height of
					the two branches that exists
					from this Entry	 */
};

struct apm_cle_avl_status {
	u16 root_addr;		/**< The root address of the AVL Tree	 */
	u16 node_cnt;		/**< The amount of nodes in the AVL Tree */
};

#ifdef CLE_SHADOW
/**
 * @brief   Gets the start AVL node pointer of Shadow AVL Nodes
 * @return  Shadow AVL Node Start Pointer
 */
struct avl_node *get_shadow_avl_nodes(void);
#endif

/**
 * @brief   Adds the search string into AVL Tree database.
 * @param   node->search_key	Search string to be added.
 * @param   node->index	Address in the AVL RAM where node gets added.
 * @param   node->result_pointer Result Pointer
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_add_node(struct avl_node *node);

/**
 * @brief   Deletes the search string from AVL Tree database.
 * @param   node->search_key	Search string to be deleted.
 * @param   node->index		Address in the AVL RAM from where node gets deleted
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_del_node(struct avl_node *node);

/**
 * @brief   Finds the search string into AVL Tree database.
 * @param   node->search_key	Search string 
 * @param   node->index	Address in the AVL RAM from where node found
 * @param   node->result_pointer Result Pointer
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_search_node(struct avl_node *node);

/**
 * @brief   Write the node into AVL Ram at given address
 * @param   node	AVL node data to write into AVL Tree.
 * @param   node->index	Address in the AVL RAM where to write node
 * @return  APM_RC_OK or APM_RC_ERROR
 * @note    This function is used when SW maintain and build its own AVL Tree
 *	    and want to write the node at specific address in the AVL Ram.
 *	    Software shall maintain the address allocation in AVL RAM in that
 *	    case 
 */
int apm_cle_avl_write_node(struct avl_node *node);

/**
 * @brief   Read the node from AVL Ram at given address
 * @param   node	AVL node data read from AVL Tree.
 * @param   node->index	Address in the AVL RAM where to read node
 * @return  APM_RC_OK or APM_RC_ERROR
 * @note    This function is used when SW maintain and build its own AVL Tree
 *	    and want to read the node at specific address in the AVL Ram.
 *	    Software shall maintain the address allocation in AVL RAM in that
 *	    case 
 */
int apm_cle_avl_read_node(struct avl_node *node);

/**
 * @brief  Gets the status of AVL search data base  
 * @param  avl_status	AVL Search Manager status 
 * @return APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_get_status(struct apm_cle_avl_status *avl_status);

/**
 * @brief   Does init configurations for AVL Search Manager.
 * @param   key_size	Search key size for AVL search database.
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_config(enum apm_cle_avl_search_str_type key_size);

/**
 * @brief   Initialize Classifier Search Engine
 * @param   key_size - Search Key size
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_cle_avl_init(enum apm_cle_avl_search_str_type key_size);

#endif	 /* _APM_AVL_API_H_ */
