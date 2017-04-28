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
 * @file apm_avl_base.c
 *
 * This file defines access layer for Classifier AVL Search Engine.
 *
 */

#include "apm_preclass_data.h"
#include "apm_preclass_base.h"
#include "apm_avl_base.h"

#ifdef CLE_SHADOW
void apm_avl_validate_node(u32 node_index);
void apm_avl_invalidate_node(u32 node_index);
#endif

int apm_cle_avl_add(u32 *node_value, u32 *node_index)
{
	int rc;
	int i;
	u32 cmd;
	u32 command_done;
	u32 counter_for_timeout = 0;

#ifdef CLE_DBG	   
	if (node_value == NULL) {
		PCLS_DBG("Error: Null node_value pointer in calling %s \n",
			__FUNCTION__);
		return APM_RC_INVALID_PARM;
	}
#endif

	/* Write the Node data [268:0] in the dataram 7-15 register */
	for (i = 0; i < REGS_PER_AVL_ADD; i++) {
		rc = apm_gbl_cle_wr32((DATA_RAM15_ADDR - (i * 4)),
					*(node_value + i));
	}

	/* Invoke indirect avl_add command */ 
	cmd = AVL_ADD_WR(1);
	PCLS_DBG("Starting AVL Tree Add Node, cmd[0X%08x] \n", cmd);
	rc |= apm_gbl_cle_wr32(INDCMD_ADDR, cmd);

	/* Poll for command done */
	do {
		rc |= apm_gbl_cle_rd32(INDCMD_STATUS_ADDR, &command_done);
	} while((!(command_done & AVL_ADD_DONE_MASK)) &&
		(counter_for_timeout++ < APM_CLE_ITER));

	if (counter_for_timeout > APM_CLE_ITER) {
		PCLS_DBG("Avl add_node command not done after waiting for"
			"%d iterations \n", APM_CLE_ITER);
		return APM_RC_FATAL;
	} else if (command_done & AVL_ERR_MASK) {
		rc |= apm_cle_int_errors();
		PCLS_ERR("Error %d: AVL Add failed \n", rc);
	} else {
		/* Address of the node added in AVL Tree */
		*node_index = AVL_DEL_SRCH_ADD_ADDR_RD(command_done);
		PCLS_DBG("AVL Tree Node Added node_index[0X%08X] \n",
			*node_index);
#ifdef CLE_SHADOW
		apm_avl_validate_node(*node_index);
#endif
	}

	return rc;
}

int apm_cle_avl_del(u32 *search_string, u32 *node_index)
{
	int rc = APM_RC_OK;
	int i;
	u32 cmd;
	u32 command_done;
	u32 counter_for_timeout = 0;

#ifdef CLE_DBG		
	if (search_string == NULL) {
		PCLS_DBG("Error: Null search_string pointer in calling %s \n",
			__FUNCTION__);
		return APM_RC_INVALID_PARM;
	}
#endif	

	/* Write the search string [255:0] in the dataram 8-15 register */
	for (i = 0; i < REGS_PER_AVL_DEL; i++) {
		rc = apm_gbl_cle_wr32((DATA_RAM15_ADDR - (i * 4)),
					*(search_string + i));
	}

	/* Invoke indirect avl_del command */ 
	cmd = AVL_DEL_WR(1);
	PCLS_DBG("Starting AVL Tree Delete Node, cmd[0X%08x] \n", cmd);
	rc |= apm_gbl_cle_wr32(INDCMD_ADDR, cmd);

	/* Poll for command done */
	do {
		rc |= apm_gbl_cle_rd32(INDCMD_STATUS_ADDR, &command_done);
	} while((!(command_done & AVL_DEL_DONE_MASK)) &&
		(counter_for_timeout++ < APM_CLE_ITER));

	if (counter_for_timeout > APM_CLE_ITER) {
		PCLS_DBG("Avl del_node command not done after waiting for"
			"%d iterations \n", APM_CLE_ITER);
		return APM_RC_FATAL;
	} else if (command_done & AVL_ERR_MASK) {
		rc |= apm_cle_int_errors();
		PCLS_ERR("Error %d: AVL Delete failed \n", rc);
	} else {
		/* Address of the node deleted from AVL Tree */
		*node_index = AVL_DEL_SRCH_ADD_ADDR_RD(command_done);
		PCLS_DBG("AVL Tree Node Delete node_index[0X%08X] \n",
			*node_index);	 
#ifdef CLE_SHADOW
		apm_avl_invalidate_node(*node_index);
#endif
	}

	return (rc);
}

int apm_cle_avl_search(u32 *search_string, u32 *node_index, u32 *result_pointer)
{
	int rc = APM_RC_OK;
	int i;
	u32 cmd;
	u32 command_done;
	u32 counter_for_timeout = 0;

#ifdef CLE_DBG			   
	if (search_string == NULL)  {
		PCLS_DBG("Error: Null search_string pointer in calling %s \n",
			__FUNCTION__);
		return APM_RC_INVALID_PARM;
	}
#endif	

	/* Write the search string [255:0] in the dataram 8-15 register */
	for (i = 0; i < REGS_PER_AVL_SEARCH; i++) {
		rc = apm_gbl_cle_wr32((DATA_RAM15_ADDR - (i * 4)),
					*(search_string + i));
	}

	/* Invoke indirect avl_search command */ 
	cmd = AVL_SEARCH_WR(1);
	PCLS_DBG("Starting AVL Tree Search, cmd[0X%08x] \n", cmd);
	rc |= apm_gbl_cle_wr32(INDCMD_ADDR, cmd);

	/* Poll for command done */
	do {
		rc |= apm_gbl_cle_rd32(INDCMD_STATUS_ADDR, &command_done);
	} while((!(command_done &
		(AVL_SEARCH_DONE_HIT_MASK | AVL_SEARCH_DONE_MISS_MASK ))) &&
		(counter_for_timeout++ < APM_CLE_ITER));

	if (counter_for_timeout > APM_CLE_ITER) {
		PCLS_DBG("Avl search command not done after waiting for"
			"%d iterations \n", APM_CLE_ITER);
		return APM_RC_FATAL;
	} else if (command_done & AVL_SEARCH_DONE_MISS_MASK) {
		PCLS_DBG("AVL Tree search MISS cmd_status[0X%08X] \n",
			command_done);
		if (command_done & AVL_ERR_MASK) {
			rc |= apm_cle_int_errors();
			PCLS_ERR("Error %d: AVL Search failed \n", rc);
		}
		return APM_CLE_AVL_SERACH_MISS;
	} else if (command_done & AVL_SEARCH_DONE_HIT_MASK) {
		*result_pointer = AVL_SEARCH_HIT_DB_PTR_RD(command_done);
		*node_index = AVL_DEL_SRCH_ADD_ADDR_RD(command_done);
		PCLS_DBG("AVL Tree search HIT - result_pointer [%d] "
			"node_index[0X%08x] \n", *result_pointer, *node_index);	
	}

	return rc;
}

int apm_cle_avl_wr(u32 *node_value, u32 node_index)
{
	int rc;
	int i ;
	u32 addr;
	u32 cmd;
	u32 command_done;
	u32 counter_for_timeout = 0;

#ifdef CLE_DBG		   
	if (node_value == NULL) {
		PCLS_DBG("Error: Null node_value pointer in calling %s \n",
			__FUNCTION__);
		return APM_RC_INVALID_PARM;	
	}
#endif

	addr = (node_index | IND_ADDR_AVL_RAM);
	rc = apm_gbl_cle_wr32(INDADDR_ADDR, addr);

	/* Write the Node data [293:0] in the dataram 6-15 register */
	for (i = 0; i < REGS_PER_AVL_NODE; i++) {
		rc = apm_gbl_cle_wr32((DATA_RAM15_ADDR - (i * 4)),
					*(node_value + i));
	}

	/* Invoke indirect write command */ 
	cmd = WRITE_F2_WR(1);
	PCLS_DBG("Starting AVL Tree Ram Write cmd[0X%08x] Indirect "
		"Addr[0X%08X] \n", cmd, addr);
	rc |= apm_gbl_cle_wr32(INDCMD_ADDR, cmd);

	/* Poll for command done */
	do {
		rc |= apm_gbl_cle_rd32(INDCMD_STATUS_ADDR, &command_done);
	} while((!(command_done & WRITE_DONE_MASK)) &&
		(counter_for_timeout++ < APM_CLE_ITER));

	if (counter_for_timeout > APM_CLE_ITER) {
		PCLS_DBG("Avl Tree Ram Write command not done after waiting"
			"for %d iterations \n", APM_CLE_ITER);
		return APM_RC_FATAL;
	}

	return rc;
}	 

int apm_cle_avl_rd(u32 *node_value, u32 node_index)
{
	int rc;
	int i;
	u32 addr;
	u32 cmd;
	u32 command_done;
	u32 counter_for_timeout = 0;

#ifdef CLE_DBG
	if (node_value == NULL) {
		PCLS_DBG("Error: Null node_value pointer in calling %s \n",
			__FUNCTION__);
		return APM_RC_INVALID_PARM;
	}
#endif	
	addr = (node_index | IND_ADDR_AVL_RAM);
	rc = apm_gbl_cle_wr32(INDADDR_ADDR, addr);

	/* Invoke indirect read command */ 
	cmd = READ_F2_WR(1);

	PCLS_DBG("Starting AVL Tree Ram Read Indirect Addr: 0X%08X, "
		"Cmd:0X%08X \n", addr, cmd);
	rc |= apm_gbl_cle_wr32(INDCMD_ADDR, cmd);

	/* Poll for command done */
	do {
		rc |= apm_gbl_cle_rd32(INDCMD_STATUS_ADDR, &command_done);
	} while((!(command_done & READ_DONE_MASK)) &&
		(counter_for_timeout++ < APM_CLE_ITER));

	if (counter_for_timeout > APM_CLE_ITER) {
		PCLS_DBG("Avl Tree Ram Read command not done after waiting "
			"for %d iterations \n", APM_CLE_ITER);
		return APM_RC_FATAL;
	} else {
		/* Read the Node data [293:0] from dataram 6-15 register */
		for (i = 0; i < REGS_PER_AVL_NODE; i++) {
			rc |= apm_gbl_cle_rd32((DATA_RAM15_ADDR - (i * 4)),
						(node_value + i));
		}
	}

	return rc;
} 

int apm_cle_int_errors(void)
{
	int rc;
	u32 int_err;
	u32 int_mask = 0;

	rc = apm_gbl_cle_rd32(AVL_SEARCH_INT_ADDR, &int_err);

	if (int_err & FATAL_WRSELFPOINTERR_MASK) {
		int_mask = FATAL_WRSELFPOINTERR_WR(1);
		rc = APM_AVL_FATAL_WRSELF;		  
		PCLS_FATAL("Fatal Error %d: write node points to itself \n",
			rc);
	} else if (int_err & FATAL_RDSELFPOINTERR_MASK) {
		int_mask = FATAL_RDSELFPOINTERR_WR(1);
		rc = APM_AVL_FATAL_RDSELF;
		PCLS_FATAL("Fatal Error %d: read node points to itself \n",
			rc);
	} else if (int_err & FATAL_WRBALERR_MASK ) {
		int_mask = FATAL_WRBALERR_WR(1);
		rc = APM_AVL_FATAL_WRBAL;
		PCLS_FATAL("Fatal Error %d: write node has incorrect balance "
			"and address \n", rc);
	} else if (int_err & FATAL_RDBALERR_MASK) {
		int_mask = FATAL_RDBALERR_WR(1);
		rc = APM_AVL_FATAL_RDBAL;
		PCLS_FATAL("Fatal Error %d: read node has incorrect balance "
			"and address \n", rc);
	} else if(int_err & FATAL_MAXSTEPSERR_MASK) {
		int_mask = FATAL_MAXSTEPSERR_WR(1);
		rc = APM_AVL_FATAL_MAXSTEP;
		PCLS_FATAL("Fatal Error %d: max number of steps exceeded"
			"error \n", rc);
	} else if (int_err & FATAL_RDERR_RAM_MASK) {
		int_mask = FATAL_RDERR_RAM_WR(1);
		rc = APM_AVL_FATAL_RDRAM;
		PCLS_FATAL("Fatal Error %d: AVL read error from RAM \n", rc);
	} else if (int_err & RDERR_RAM_MASK) {
		int_mask = RDERR_RAM_WR(1);
		rc = APM_AVL_RDRAM;
		PCLS_ERR("Error %d: AVL read error from RAM \n", rc);
	} else if (int_err & DELERR_RIDNOTFOUND_MASK) {
		int_mask = DELERR_RIDNOTFOUND_WR(1);
		rc = APM_AVL_DEL_NOSTR;
		PCLS_ERR("Error %d: Search String does not exist \n", rc);
	} else if (int_err & DELERR_EMPTY_MASK) {
		int_mask = DELERR_EMPTY_WR(1);
		rc = APM_AVL_DEL_TREE_EMPTY;
		PCLS_ERR("Error %d: AVL Tree is empty \n", rc);
	} else if (int_err & ADDERR_RIDDUPLICATE_MASK) {
		int_mask = ADDERR_RIDDUPLICATE_WR(1);
		rc = APM_AVL_ADD_DUP;
		PCLS_ERR("Error %d: Search String already exists"
			"in the AVL Tree \n", rc);
	} else if(int_err & ADDERR_FULL_MASK) {
		int_mask = ADDERR_FULL_WR(1);
		rc = APM_AVL_ADD_TREE_FULL;
		PCLS_ERR("Error %d: AVL Tree is already full \n", rc);
	}

	PCLS_DBG("Clear the AVL search Interrupt register mask[0X%08X] \n",
		int_mask);
	rc = apm_gbl_cle_wr32(AVL_SEARCH_INT_ADDR, int_mask);

	return rc;
}
