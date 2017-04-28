/*
 * APM86xxx CLE Configuration Header
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * Ravi Patel <rapatel@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * @file apm_cle_mgr.c
 *
 * This file implements Management APIs for
 * APM862xx SoC Classifier module.
 */

#include "apm_cle_mgr.h"

static u32 node_alloc_index = 0;
static u32 dbptr_alloc_index = 0;
static u32 ptree_node_branch_count = 0;
static u32 ptree_node_count = 0;
static u32 dbptr_entry_count = 0;
static u32 node_bitmask[MAX_NODE_ENTRIES / 32] = {0};
static u32 dbptr_bitmask[MAX_DB_ENTRIES / 32] = {0};

int apm_cle_mgr_init(void)
{
	node_alloc_index = SYSTEM_START_NODE;
	dbptr_alloc_index = SYSTEM_START_DBPTR +
				(1 & (!apm_cle_system_id));

	memset(&node_bitmask, 0, sizeof(node_bitmask));
	memset(&dbptr_bitmask, 0, sizeof(dbptr_bitmask));

	return APM_RC_OK;
}

/* Defining Patricia Tree Management API's */
int apm_ptree_node_precheck(u8 port, struct ptree_node *node)
{
	int rc = APM_RC_OK;
	u8 type;

	if (node == NULL) {
		PCLS_ERR("Null node pointer \n");
		rc = APM_RC_INVALID_PARM;
	} else if (node->data == NULL) {
		PCLS_ERR("Invalid DN or KN pointer\n");
		rc = APM_RC_INVALID_PARM;
	} else if (port > MAX_NO_OF_PORTS) {
		PCLS_ERR("Invalid port number %d\n", port);
		rc = APM_RC_INVALID_PARM;
	} else if ((type = node->type) < EWDN ||
		type > KN) {
		PCLS_ERR("Invalid node type %d\n", type);
		rc = APM_RC_INVALID_PARM;
	}

	return rc;
}

int apm_ptree_branch_precheck(u8 port, struct ptree_node *node)
{
	int rc = APM_RC_OK;
	u8 type = node->type;

	if (type != KN) {
		u8 num = ((struct ptree_dn *)node->data)->num;

		if ((type == EWDN && num > 8) ||
			(type == FWDN && num > 4)) {
			PCLS_ERR("Invalid number of branches %d for "
			"node type %d\n", num, type);
			return APM_RC_INVALID_PARM;
		}

		ptree_node_branch_count += num;
	}

	return rc;
}

int apm_ptree_node_check(u8 port, struct ptree_node *node, u32 pass_aflag)
{
	u8 type = node->type;
	int rc = APM_RC_OK;
	u32 result_pointer;
	u32 node_index = node->index & ~PTREE_AFLAG(pass_aflag);
	struct ptree_dn *dn = NULL;

	/* 8-way node & 4-way node */
	if (type != KN) {
		dn = (struct ptree_dn *)node->data;
		result_pointer = dn->result_pointer;
	} else {
		result_pointer = ((struct ptree_kn *)
				node->data)->result_pointer;
	}

	if (node_index > END_NODE_INDEX) {
		PCLS_ERR("Invalid node_index %d\n", node_index);
		rc = APM_RC_INVALID_PARM;
	} else if (NODE_OUT_RANGE(node_index, pass_aflag)) {
		PCLS_ERR("Invalid node_index %d for system %d\n",
			node_index, apm_cle_system_id);
		rc = APM_RC_INVALID_PARM;

	} else if (DBPTR_OUT_RANGE(result_pointer, pass_aflag)) {
		PCLS_ERR("Invalid default/key result pointer %d "
			"for system %d\n", result_pointer, apm_cle_system_id);
		rc = APM_RC_INVALID_PARM;
	} else if (type != KN) {
	/* 8-way node & 4-way node */
		int i;

		for (i = 0; i < dn->num; i++) {
			node_index = dn->branch[i].next_node_index &
					~PTREE_AFLAG(pass_aflag);

			if (NODE_OUT_RANGE(node_index, pass_aflag)) {

				PCLS_ERR("Invalid Next Node location %d"
					" for system %d\n",
					node_index, apm_cle_system_id);
				rc = APM_RC_INVALID_PARM;
				break;
			}
		}
	}

	if (rc == APM_RC_OK && type != EWDN) {
	/* 4-way node & Key node */
		struct apm_preclass_node_state *pns;
		u32 max_location_index = (type == FWDN) ?
						1 : KEYNODE_INDEX;

		node_index = node->index & ~PTREE_AFLAG(pass_aflag);
		pns = &sys_preclass_state[port].
			node_entry_state[node_index];

		if (node->location > max_location_index) {
			PCLS_ERR("Invalid node location %d\n",
				node->location);
			rc = APM_RC_INVALID_PARM;
		} else if (pns->type && (pns->type != type)) {
			PCLS_ERR("Invalid node entry %d "
				"reconfig detected !!\n", node_index);
			rc = APM_RC_INVALID_PARM;
		}

	}

	return rc;
}

void apm_ptree_node_prepare(u32 remove_aflag,
			struct ptree_node *dst_node,
			struct ptree_dn *dst_dn,
			struct ptree_kn *dst_kn,
			struct ptree_branch *dst_branch,
			struct ptree_node *src_node)
{
	/* Copy ptree_node */
	memcpy(dst_node, src_node, sizeof(struct ptree_node));

	if (dst_node->index & PTREE_ALLOC(0)) {
		dst_node->index = (dst_node->index & ~PTREE_AFLAG(remove_aflag)) +
					node_alloc_index;
		if (remove_aflag && GET_BITMASK(dst_node->index, node_bitmask) == 0)
			ptree_node_count++;
	}

	if (remove_aflag)
		SET_BITMASK(dst_node->index, node_bitmask, 1);

	if (dst_node->type == KN) {
		dst_node->data = dst_kn;
		/* Copy KN */
		memcpy(dst_kn,
			(struct ptree_kn *)src_node->data,
			sizeof(struct ptree_kn));

		if (dst_kn->result_pointer & DBPTR_ALLOC(0)) {
			dst_kn->result_pointer = (dst_kn->result_pointer &
					~DBPTR_AFLAG(remove_aflag)) +
					dbptr_alloc_index;
			if (remove_aflag && GET_BITMASK(dst_kn->result_pointer,
					dbptr_bitmask) == 0)
				dbptr_entry_count++;
		} else if (dst_kn->result_pointer & DBPTR_DROP(0)) {
			dst_kn->result_pointer = START_DB_INDEX;
		}

		if (remove_aflag)
			SET_BITMASK(dst_kn->result_pointer, dbptr_bitmask, 1);

	} else {
		int i;

		dst_node->data = dst_dn;
		/* Copy DN */
		memcpy(dst_dn,
			(struct ptree_dn *)src_node->data,
			sizeof(struct ptree_dn));

		if (dst_dn->result_pointer & DBPTR_ALLOC(0)) {
			dst_dn->result_pointer = (dst_dn->result_pointer &
					~DBPTR_AFLAG(remove_aflag)) +
					dbptr_alloc_index;
			if (remove_aflag && GET_BITMASK(dst_dn->result_pointer,
					dbptr_bitmask) == 0)
				dbptr_entry_count++;
		} else if (dst_dn->result_pointer & DBPTR_DROP(0)) {
			dst_dn->result_pointer = START_DB_INDEX;
		}

		if (remove_aflag)
			SET_BITMASK(dst_dn->result_pointer, dbptr_bitmask, 1);

		dst_dn->branch = dst_branch;
		for (i = 0; i < dst_dn->num; i++) {
			memcpy(&dst_branch[i],
				&((struct ptree_dn *)
					src_node->data)->branch[i],
				sizeof(struct ptree_branch));

			if (dst_branch[i].next_node_index & PTREE_ALLOC(0)) {
				dst_branch[i].next_node_index =
					(dst_branch[i].next_node_index &
					~PTREE_AFLAG(remove_aflag)) +
					node_alloc_index;
			}
		}
	}
}

int apm_ptree_alloc(u8 port, u8 ptnodes, u16 dbptrs,
		   struct ptree_node *node,
		   struct apm_cle_dbptr *dbptr,
		   struct apm_ptree_config *ptree_config)
{
	int i, rc = APM_RC_OK;
	struct ptree_node dst_node;
	struct ptree_dn dst_dn;
	struct ptree_kn dst_kn;
	struct ptree_branch dst_branch[8];

	if (!apm_preclass_init_state)
		apm_preclassify_init();

	ptree_node_branch_count = 0;
	ptree_node_count = 0;
	dbptr_entry_count = 0;

	/* Check for errors in ptree nodes configuration */
	for (i = 0; i < ptnodes; i++) {

		if ((rc = apm_ptree_node_precheck(port, &node[i])) !=
				APM_RC_OK) {
			PCLS_ERR("Patricia Tree Node-Precheck error %d\n", rc);
			return rc;
		}

		if ((rc = apm_ptree_branch_precheck(port, &node[i])) !=
				APM_RC_OK) {
			PCLS_ERR("Patricia Tree Branch-Precheck error %d\n", rc);
			return rc;
		}

		apm_ptree_node_prepare(0, &dst_node, &dst_dn,
			&dst_kn, &dst_branch[0], &node[i]);

		if ((rc = apm_ptree_node_check(port, &dst_node, 1)) != APM_RC_OK) {
			PCLS_ERR("Patricia Tree Node-Check error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Prepared-N-Checked index %d\n", __func__, i);
	}

	/* Check for branch count in ptree nodes configuration */
	if (ptree_node_branch_count == 0) {
		PCLS_ERR("No Patricia Tree Branch defined\n");
		return APM_RC_INVALID_PARM;
	}

	ptree_node_count = 0;
	dbptr_entry_count = 0;

	for (i = 0; i < ptnodes; i++) {

		apm_ptree_node_prepare(1, &dst_node, &dst_dn,
			&dst_kn, &dst_branch[0], &node[i]);

		if ((rc = apm_ptree_node_config(port, &dst_node)) != APM_RC_OK) {
			PCLS_ERR("Patricia Tree Node-Alloc error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Configured node index %d\n", __func__, dst_node.index);
	}

	for (i = 0; i < dbptrs; i++) {

		struct apm_cle_dbptr dst_dbptr;
		u32 index = (dbptr[i].index & DBPTR_ALLOC(0)) ?
				(DBPTR_INDEX(dbptr[i].index) +
				dbptr_alloc_index) :
				(dbptr[i].index);
		memcpy(&dst_dbptr, &dbptr[i], sizeof(dst_dbptr));
		dst_dbptr.index = index;

		if ((rc = apm_set_cle_dbptr(&dst_dbptr)) != APM_RC_OK) {
			PCLS_ERR("Patricia Tree DBPTR-Alloc error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Configured dbptr index %d\n", __func__, index);
	}

	if (ptree_config) {
		if (ptree_node_count) {
			ptree_config->start_node_ptr = node_alloc_index;
			node_alloc_index += ptree_node_count;
		}

		if (dbptr_entry_count) {
			ptree_config->start_dbptr = dbptr_alloc_index;
			dbptr_alloc_index += dbptr_entry_count;
		}

		ptree_config->max_hop = ptree_node_branch_count;
	}

	return rc;
}

/* Defining AVL Node Management API's */
int apm_avl_node_check(u8 port, struct avl_node *node, u32 pass_aflag)
{
	int rc = APM_RC_OK;
	u32 result_pointer = node->result_pointer;

	if (DBPTR_OUT_RANGE(result_pointer, pass_aflag)) {
		PCLS_ERR("Invalid default/key result pointer %d "
			"for system %d\n", result_pointer, apm_cle_system_id);
		rc = APM_RC_INVALID_PARM;
	}

	return rc;
}

void apm_avl_node_prepare(u32 remove_aflag,
			struct avl_node *dst_node,
			struct avl_node *src_node)
{
	/* Copy avl_add_node_node */
	memcpy(dst_node, src_node, sizeof(struct avl_node));

	if (dst_node->result_pointer & DBPTR_ALLOC(0)) {
		dst_node->result_pointer = (dst_node->result_pointer &
				~DBPTR_AFLAG(remove_aflag)) +
			dbptr_alloc_index;
		if (remove_aflag && GET_BITMASK(dst_node->result_pointer,
				dbptr_bitmask) == 0)
			dbptr_entry_count++;
	} else if (dst_node->result_pointer & DBPTR_DROP(0)) {
		dst_node->result_pointer = START_DB_INDEX;
	}

	if (remove_aflag)
		SET_BITMASK(dst_node->result_pointer, dbptr_bitmask, 1);
}

int apm_avl_alloc(u8 port, u16 avlnodes, u16 dbptrs,
		   struct avl_node *node,
		   struct apm_cle_dbptr *dbptr)
{
	int i, rc = APM_RC_OK;
	struct avl_node dst_node;

	if (!apm_preclass_init_state)
		apm_preclassify_init();

	dbptr_entry_count = 0;

	for (i = 0; i < avlnodes; i++) {

		apm_avl_node_prepare(0, &dst_node, &node[i]);

		if ((rc = apm_avl_node_check(port, &dst_node, 1)) != APM_RC_OK) {
			PCLS_ERR("AVL Engine Node-Check error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Prepared-N-Checked index %d\n", __func__, i);
	}

	dbptr_entry_count = 0;

	for (i = 0; i < avlnodes; i++) {

		apm_avl_node_prepare(1, &dst_node, &node[i]);

		if ((rc = apm_cle_avl_add_node(&dst_node)) != APM_RC_OK) {
			PCLS_ERR("AVL Engine Node-Alloc error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Configured node index %d\n", __func__, dst_node.index);
		node[i].index = dst_node.index;
	}

	for (i = 0; i < dbptrs; i++) {

		struct apm_cle_dbptr dst_dbptr;
		u32 index = (dbptr[i].index & DBPTR_ALLOC(0)) ?
				(DBPTR_INDEX(dbptr[i].index) +
				dbptr_alloc_index) :
				(dbptr[i].index);
		memcpy(&dst_dbptr, &dbptr[i], sizeof(dst_dbptr));
		dst_dbptr.index = index;

		if ((rc = apm_set_cle_dbptr(&dst_dbptr)) != APM_RC_OK) {
			PCLS_ERR("AVL Engine DBPTR-Alloc error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Configured dbptr index %d\n", __func__, index);

		dbptr[i].index = index;		/* Updating for Caller */
	}

	if (dbptr_entry_count) {
		dbptr_alloc_index += dbptr_entry_count;
	}

	return rc;
}

/* Defining Result Pointer Management API's */
int apm_dbptr_check(u8 port, struct apm_cle_dbptr *dbptr, u32 pass_aflag)
{
	int rc = APM_RC_OK;
	u32 result_pointer = dbptr->index;

	if (DBPTR_OUT_RANGE(result_pointer, pass_aflag)) {
		PCLS_ERR("Invalid default/key result pointer %d "
			"for system %d\n", result_pointer, apm_cle_system_id);
		rc = APM_RC_INVALID_PARM;
	}

	return rc;
}

void apm_dbptr_prepare(u32 remove_aflag,
			struct apm_cle_dbptr *dst_dbptr,
			struct apm_cle_dbptr *src_dbptr)
{
	/* Copy result_pointer */
	memcpy(dst_dbptr, src_dbptr, sizeof(struct apm_cle_dbptr));

	if (dst_dbptr->index & DBPTR_ALLOC(0)) {
		dst_dbptr->index = (dst_dbptr->index &
				~DBPTR_AFLAG(remove_aflag)) +
			dbptr_alloc_index;
		if (remove_aflag && GET_BITMASK(dst_dbptr->index,
				dbptr_bitmask) == 0)
			dbptr_entry_count++;
	} else if (dst_dbptr->index & DBPTR_DROP(0)) {
		dst_dbptr->index = START_DB_INDEX;
	}

	if (remove_aflag)
		SET_BITMASK(dst_dbptr->index, dbptr_bitmask, 1);
}

int apm_dbptr_alloc(u8 port, u16 dbptrs, struct apm_cle_dbptr *dbptr)
{
	int i, rc = APM_RC_OK;
	struct apm_cle_dbptr dst_dbptr;

	if (!apm_preclass_init_state)
		apm_preclassify_init();

	dbptr_entry_count = 0;

	for (i = 0; i < dbptrs; i++) {

		apm_dbptr_prepare(0, &dst_dbptr, &dbptr[i]);

		if ((rc = apm_dbptr_check(port, &dst_dbptr, 1)) != APM_RC_OK) {
			PCLS_ERR("Result Pointer-Check error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Prepared-N-Checked index %d\n", __func__, i);
	}

	dbptr_entry_count = 0;

	for (i = 0; i < dbptrs; i++) {

		apm_dbptr_prepare(1, &dst_dbptr, &dbptr[i]);

		if ((rc = apm_set_cle_dbptr(&dst_dbptr)) != APM_RC_OK) {
			PCLS_ERR("Result Pointer-Alloc error %d \n", rc);
			return rc;
		}
		PCLS_PRNT("%s: Configured dbptr index %d\n", __func__, dst_dbptr.index);

		dbptr[i].index = dst_dbptr.index;	/* Updating for Caller */
	}

	if (dbptr_entry_count) {
		dbptr_alloc_index += dbptr_entry_count;
	}

	return rc;
}
