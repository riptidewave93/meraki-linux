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
 * @file apm_avl_base.h
 * This file defines Linux access layer for Classifier AVL search Engine.
 *
 */

#ifndef _APM_AVL_BASE_H_
#define _APM_AVL_BASE_H_

/**
 * @brief   This function adds the search data at appropriate location in the
 *	    balanced AVL Tree 
 * @param   node_data	Pointer to search data array to write
 * @param   node_adr	Pointer to return the address where new node in AVL
 *			Tree gets added.
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    None
 */
int apm_cle_avl_add(u32 *node_data, u32 *node_adr);

/**
 * @brief   This function deletes the AVL tree node matching the search given
 *	    as input.
 * @param   search_string  Pointer to search string to match to delete
 *			   the node in ALV tree. 
 * @param   node_adr	Pointer to return the address where the node in AVL 
 *			Tree gets deleted.
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    None
 */
int apm_cle_avl_del(u32 *search_string, u32 *node_adr);

/**
 * @brief   This function search the AVL tree node for the search string
 *	    given as input. it returns the address and DB entry index for
 *	    the matching AVL tree node.
 * @param   search_string  Pointer to search string to match in AVL tree. 
 * @param   node_adr	Pointer to return the address of the matching node
 *			in Avl Tree.
 * @param   result_pointer	Pointer to return the DB entry index for matching node
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    None
 */
int apm_cle_avl_search(u32 *search_string, u32 *node_adr, u32 *result_pointer);

/**
 * @brief   This function writes the AVL tree node data at the given
 *	    RAM address
 * @param   node_data	Pointer to AVL Tree Node data array to write
 * @param   node_adr	Address to write Avl Tree Node
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    This is used for debuging purpose only. 
 */
int apm_cle_avl_wr(u32 *node_data, u32 node_adr);

/**
 * @brief   This function reads the AVL tree node data from the given
 *	    RAM address
 * @param   node_data	Pointer to return the AVL Tree Node data 
 * @param   node_adr	Address to read Avl Tree Node
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    This is used for debuging purpose only. 
 */
int apm_cle_avl_rd(u32 *node_data, u32 node_adr);

/**
 * @brief   This function reads Classifier error interrupt status register
 *	    and parse the errors.
 * @param   None
 * @return  APM_RC_OK or APM_RC_ERROR  
 */
int apm_cle_int_errors(void);

#endif	 /* _APM_AVL_BASE_H_ */
 
