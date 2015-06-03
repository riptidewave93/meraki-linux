/**
 * AppliedMicro APM862xx SoC Classifier Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Mahesh Pujara <mpujara@apm.com>
 *                      Ravi Patel <rapatel@apm.com>
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
 * @file apm_preclass_api.c
 *
 * This file iplements APIs for APM862xx SoC Classifier Parser module.
 *
 */

#include "apm_preclass_data.h"
#include "apm_preclass_base.h"

#define APM_RET_IVP APM_RC_INVALID_PARM

/* global data */
/* apm_cle_system_id & apm_cle_systems used for CLE resource division for AMP */
u8 apm_cle_system_id;	/* Needs to be updated by BootLoader and OS */
u8 apm_cle_systems;	/* Needs to be updated by BootLoader and OS */
int apm_preclass_init_state = 0;
#ifdef CLE_SHADOW
static int ptree_shadow_done = 0;
static struct apm_ptree_node sys_ptree_node[MAX_NODE_ENTRIES];
static struct apm_cle_dbptr sys_cle_dbptr[MAX_DB_ENTRIES];
#endif
struct apm_preclass_state sys_preclass_state[MAX_NO_OF_PORTS];
struct apm_ptree_config sys_ptree_config[MAX_NO_OF_PORTS];
#ifdef CLE_MANAGER
int apm_cle_mgr_init(void);
#endif

static void shift_left_be(u32 *data, u32 bit_index, u32 bit_length, u32 shift)
{
	while (shift > 0 && bit_index > 0) {
		u32 long_end = (bit_index + bit_length - 1) / 32;
		u32 long_index = (bit_index - 1) / 32;
		u32 long_bit_index = ((bit_index - 1) % 32);
		u32 long_bit_mask = 0xffffffff >> long_bit_index;
		u32 long_bit_end_index = ((bit_index + bit_length) % 32);
		u32 long_bit_end_mask = 0xffffffff <<
			(32 - (long_bit_end_index ? long_bit_end_index : 32));
		u32 i;

		if (long_index == long_end) {
			data[long_index] = (data[long_index] &
				~(long_bit_mask & long_bit_end_mask)) |
				((data[long_index] &
				(long_bit_mask & long_bit_end_mask)) << 1);
		} else {
			data[long_index] = (data[long_index] & ~long_bit_mask) |
				((data[long_index] << 1) & long_bit_mask);

			for (i = long_index; i < long_end; i++) {

				if (i > long_index)
					data[i] <<= 1;

				data[i] = (data[i] & ~1) | (data[i + 1] >> 31);
			}

			data[long_end] = (data[long_end] & ~long_bit_end_mask) |
				((data[long_end] << 1) & long_bit_end_mask);
		}

		bit_index--;
		shift--;
	}
}

#ifndef CLE_SHADOW
static void shift_right_be(u32 *data, u32 data_length, u32 bit_index,
				u32 bit_length, u32 shift)
{
	bit_length = (bit_index + 1) < bit_length ? (bit_index + 1) : bit_length;

	while (shift > 0 && bit_index < ((data_length * 8) - 1)) {
		u32 long_end = (bit_index - bit_length + 1) / 32;
		u32 long_index = (bit_index + 1) / 32;
		u32 long_bit_index = ((bit_index + 1) % 32);
		u32 long_bit_mask = 0xffffffff << (31 - long_bit_index);
		u32 long_bit_end_index = ((bit_index - bit_length + 1) % 32);
		u32 long_bit_end_mask = 0xffffffff >> long_bit_end_index;
		u32 i;

		if (long_index == long_end) {
			data[long_index] = (data[long_index] &
				~(long_bit_mask & long_bit_end_mask)) |
				((data[long_index] &
				(long_bit_mask & long_bit_end_mask)) >> 1);
		} else {
			data[long_index] = (data[long_index] & ~long_bit_mask) |
				((data[long_index] >> 1) & long_bit_mask);

			for (i = long_index; i > long_end; i--) {

				if (i < long_index)
					data[i] >>= 1;

				data[i] = (data[i] & ~0x80000000) |
						(data[i - 1] << 31);
			}

			data[long_end] = (data[long_end] & ~long_bit_end_mask) |
				((data[long_end] & long_bit_end_mask) >> 1);
		}

		bit_index++;
		shift--;
	}
}
#endif

static void apm_ptree_node_struct_to_reg(void *data)
{
	u32 i;
	int shift;
	u32 *regs = (u32 *)data;
	struct apm_ptree_node *node = (struct apm_ptree_node *)data;

	switch (node->type) {
	case EIGHT_WAY_NODE:
#ifdef CONFIG_APM862xx
		shift = 5 - PAD_SIZE;
		shift_left_be(regs, 8, 17, shift);
		shift += 2;
#else
		shift = 6 - PAD_SIZE;
		shift_left_be(regs, 8, 18, shift);
		shift += 1;
#endif
		for (i = 0; i < 8; i++) {
			shift += 5;
			shift_left_be(regs, (64 * i) + 32, 59, shift);
		}
		break;

#ifdef CONFIG_APM862xx
	case FOUR_WAY_NODE1_ONLY:
	case FOUR_WAY_NODE2_ONLY:
	case FOUR_WAY_NODE_BOTH:
	{
		u32 j;
		shift = 0 - PAD_SIZE;
		for (j = 0; j < 2; j++) {
			shift += 5;
			shift_left_be(regs, (280 * j) + 8, 17, shift);
			shift += 2;
			for (i = 0; i < 4; i++) {
				shift += 5;
				shift_left_be(regs,
					((64 * i) + (280 * j) + 32), 59, shift);
			}
		}
		break;
	}
#endif

	case KEY_NODE:
#ifdef CONFIG_APM862xx
		shift = 2 - PAD_SIZE;
#else
		shift = 3 - PAD_SIZE;
#endif
		for (i = 0; i < 32; i++) {
			shift += 3;
			shift_left_be(regs, (16 * i) + 8, 13, shift);
		}
	}
}

#ifndef CLE_SHADOW
static void apm_ptree_node_reg_to_struct(void *data)
{
	u32 i;
	int shift;
	u32 *regs = (u32 *)data;
	struct apm_ptree_node *node = (struct apm_ptree_node *)data;
	u32 size = SIZE_PER_PTREE_NODE_STRUCT;

	switch (node->type) {
	case EIGHT_WAY_NODE:
		shift = 52 - PAD_SIZE;
		for (i = 0; i < 8; i++) {
			shift -= 5;
			shift_right_be(regs, size, 491 + PAD_SIZE - (59 * i), 59, shift);
		}
#ifdef CONFIG_APM862xx
		shift -= 7;
		shift_right_be(regs, size, 19 + PAD_SIZE, 17, shift);
#else
		shift -= 6;
		shift_right_be(regs, size, 19 + PAD_SIZE, 18, shift);
#endif
		break;

#ifdef CONFIG_APM862xx
	case FOUR_WAY_NODE1_ONLY:
	case FOUR_WAY_NODE2_ONLY:
	case FOUR_WAY_NODE_BOTH:
	{
		u32 j;
		shift = 59 - PAD_SIZE;
		for (j = 0; j < 2; j++) {
			for (i = 0; i < 4; i++) {
				shift -= 5;
				shift_right_be(regs, size,
					(508 + PAD_SIZE - (59 * i) - (253 * j)), 59, shift);
			}
			shift -= 7;
			shift_right_be(regs, size, 272 + PAD_SIZE - (253 * j), 17, shift);
		}
		break;
	}
#endif

	case KEY_NODE:
#ifdef CONFIG_APM862xx
		shift = 101 - PAD_SIZE;
#else
		shift = 102 - PAD_SIZE;
#endif
		for (i = 0; i < 32; i++) {
			shift -= 3;
#ifdef CONFIG_APM862xx
			shift_right_be(regs, size, 418 + PAD_SIZE - (13 * i), 13, shift);
#else
			shift_right_be(regs, size, 417 + PAD_SIZE - (13 * i), 13, shift);
#endif
		}
	}
}
#endif

int apm_preclassify_init(void)
{
	int rc = APM_RC_OK;

	if (!apm_preclass_init_state) {
		struct apm_cle_dbptr dbptr;

		memset(&sys_preclass_state, 0,
			sizeof(sys_preclass_state));
		memset(&dbptr, 0, sizeof(dbptr));
		dbptr.index = START_DB_INDEX;
		dbptr.drop = 1;
		rc = apm_set_cle_dbptr(&dbptr);
#ifdef CLE_MANAGER
		rc |= apm_cle_mgr_init();
#endif
		apm_preclass_init_state = 1;
	} else {
		PCLS_DBG("Pre-classifier global s/w "
			"config already done \n");
		rc = APM_RC_FATAL;
	}

	return rc;
}

#ifdef CLE_SHADOW
static inline void apm_ptree_shadow_init(void)
{
	if (!ptree_shadow_done) {
		memset(&sys_ptree_node, 0,
			sizeof(sys_ptree_node));
		memset(&sys_cle_dbptr, 0,
			sizeof(sys_cle_dbptr));
		ptree_shadow_done = 1;
	}
}

struct apm_ptree_node *get_shadow_ptree_nodes(void)
{
	return &sys_ptree_node[0];
}

struct apm_cle_dbptr *get_shadow_cle_dbptrs(void)
{
	return &sys_cle_dbptr[0];
}
#endif

int apm_set_ptree_node(u8 port, u8 node_index, struct apm_ptree_node *node)
{
	int rc;

#ifdef CLE_SHADOW
	apm_ptree_shadow_init();

	memcpy(&sys_ptree_node[node_index], node,
		sizeof(struct apm_ptree_node));
#endif

	apm_ptree_node_struct_to_reg(node);
	rc = apm_preclass_ptram_write(port, node_index, (u32 *)node);

	return rc;
}

int apm_get_ptree_node(u8 port, u8 node_index, struct apm_ptree_node *node)
{
	int rc = APM_RC_OK;

#ifdef CLE_SHADOW
	apm_ptree_shadow_init();

	memcpy(node, &sys_ptree_node[node_index],
		sizeof(struct apm_ptree_node));
#else
	rc = apm_preclass_ptram_read(port, node_index, (u32 *)node);
	apm_ptree_node_reg_to_struct(node);
#endif

	return rc;
}

int apm_set_cle_dbptr(struct apm_cle_dbptr *dbptr)
{
	int rc;
	u32 index;

#ifdef CLE_DEBUG
	if (dbptr == NULL) {
		PCLS_DBG("Null dbptr pointer \n");
		return APM_RC_INVALID_PARM;
	}
#endif

	index = dbptr->index;
	/* Making sure RAM is not written with unintended value */
	dbptr->index = 0;
	rc = apm_preclass_cldb_write(index, (u32 *)dbptr);

#ifdef CLE_SHADOW
	apm_ptree_shadow_init();

	/* index field Will be used for cle_dbptr validity by user */
	dbptr->index = 1;
	memcpy(&sys_cle_dbptr[index], dbptr,
		sizeof(struct apm_cle_dbptr));
#endif

	/* Putting back the index vaule */
	dbptr->index = index;

	return rc;
}

int apm_get_cle_dbptr(struct apm_cle_dbptr *dbptr)
{
	int rc = APM_RC_OK;
	u32 index;

#ifdef CLE_DEBUG
	if (dbptr == NULL) {
		PCLS_DBG("Null dbptr pointer \n");
		return APM_RC_INVALID_PARM;
	}
#endif

	index = dbptr->index;

#ifdef CLE_SHADOW
	apm_ptree_shadow_init();

	memcpy(dbptr, &sys_cle_dbptr[index],
		sizeof(struct apm_cle_dbptr));
#else
	dbptr->index = 0;
	rc = apm_preclass_cldb_read(index, (u32 *)dbptr);
#endif

	return rc;
}

void apm_ptree_set_data(void *data,
		       struct ptree_node *pnode,
		       u8 set_type)
{
	u8 index = pnode->location;
	int i;
	struct apm_ptree_node *node;
	struct apm_ptree_ewdn *ewdn;
#ifdef CONFIG_APM862xx
	struct apm_ptree_fwdn *fwdn;
#endif
	struct ptree_dn *dn;
	struct ptree_kn *kn;

	node = (struct apm_ptree_node *)data;

	PCLS_DBG("pnode->type[%d] \n", pnode->type);

	if (!data) {
		PCLS_DBG("Invalid data pointer");
		return;
	}

#ifdef CLE_DEBUG
	for (i = 0; i < REGS_PER_PTREE_NODE; i++) {
		PCLS_DBG("data[%d] is 0X%08x \t", i, ((u32 *)data)[i]);
	}
	PCLS_DBG(" \n");
#endif

	switch(pnode->type) {
	case EWDN:
		if (set_type)
			node->type = set_type & DN_TYPE_SIZE;
		dn = (struct ptree_dn *)pnode->data;
		ewdn = &node->entry.ewdn;
		ewdn->result_pointer = dn->result_pointer & DN_DEFPTR_SIZE;
		ewdn->search_byte_store = dn->search_byte_store & DN_SEARCHB_SIZE;
		ewdn->byte_store = dn->byte_store & DN_BYTE_SIZE;
		ewdn->header_length_store = dn->header_length_store & DN_HDRL_SIZE;
		ewdn->last_node = dn->node_position & DN_LN_SIZE;
		ewdn->extension = dn->extension & DN_EXTENSION_SIZE;
		CLE_NODE_DBG(" EW: Type %d "
				"RPtr %d SBSt %d BySt %d HdSt %d Last %d CorC %d"
				"\n", node->type, ewdn->result_pointer,
				ewdn->search_byte_store, ewdn->byte_store,
				ewdn->header_length_store, ewdn->last_node,
				ewdn->extension);
		for (i = 0; i < dn->num; i++) {
			struct apm_ptree_branch *branch = &ewdn->branch[i];
			struct ptree_branch *pbranch = &dn->branch[i];
			branch->valid = pbranch->valid & DN_VALID_SIZE;
			branch->mask = pbranch->mask & DN_MASK_SIZE;
			branch->data = pbranch->data & DN_MASK_SIZE;
			branch->operation = pbranch->operation & DN_OP_SIZE;
			branch->next_node_branch =
				((pbranch->next_node_index & DN_NN_SIZE) |
				((pbranch->next_node_loc & 0x1F) << 7)) & DN_NNP_SIZE;
			branch->jump_rel =
				pbranch->jump_rel & DN_JREL_SIZE;
			branch->jump_bw =
				pbranch->jump_bw & DN_JBW_SIZE;
			CLE_NODE_DBG("  %d: VBit %d Mask %04x Data %04x OTyp %d "
					"NNBr %d%s,%d JRel %d JBkw %d NPOf %d\n",
					i, branch->valid, branch->mask, branch->data,
					branch->operation, pbranch->next_node_index,
					((pbranch->next_node_loc & 1) ? ".1" : ""),
					pbranch->next_node_loc >> 1,
					branch->jump_rel, branch->jump_bw,
					pbranch->next_packet_pointer);
#ifdef CARBON_MODEL
			branch->next_packet_pointer =
				pbranch->next_packet_pointer ?
				(pbranch->next_packet_pointer + 
					 ((!branch->jump_rel) * 4)) & DN_NBP_SIZE : 0;
#else
			branch->next_packet_pointer =
				pbranch->next_packet_pointer & DN_NBP_SIZE;
#endif
		}
		break;

#ifdef CONFIG_APM862xx
	case FWDN:
		if (set_type)
			node->type = set_type & DN_TYPE_SIZE;
		dn = (struct ptree_dn *)pnode->data;
		fwdn = &node->entry.fwdn[index];
		fwdn->result_pointer = dn->result_pointer & DN_DEFPTR_SIZE;
		fwdn->search_byte_store = dn->search_byte_store & DN_SEARCHB_SIZE;
		fwdn->byte_store = dn->byte_store & DN_BYTE_SIZE;
		fwdn->header_length_store = dn->header_length_store & DN_HDRL_SIZE;
		fwdn->last_node = dn->node_position & DN_LN_SIZE;
		fwdn->extension = dn->extension & DN_EXTENSION_SIZE;
		CLE_NODE_DBG("FW%d: Type %d "
				"RPtr %d SBSt %d BySt %d HdSt %d Last %d CorC %d"
				"\n", index, node->type, fwdn->result_pointer,
				fwdn->search_byte_store, fwdn->byte_store,
				fwdn->header_length_store, fwdn->last_node,
				fwdn->extension);
		for (i = 0; i < dn->num; i++) {
			struct apm_ptree_branch *branch = &fwdn->branch[i];
			struct ptree_branch *pbranch = &dn->branch[i];
			branch->valid = pbranch->valid & DN_VALID_SIZE;
			branch->mask = pbranch->mask & DN_MASK_SIZE;
			branch->data = pbranch->data & DN_MASK_SIZE;
			branch->operation = pbranch->operation & DN_OP_SIZE;
			branch->next_node_branch =
				((pbranch->next_node_index & DN_NN_SIZE) |
				((pbranch->next_node_loc & 0x1F) << 7)) & DN_NNP_SIZE;
			branch->jump_rel =
				pbranch->jump_rel & DN_JREL_SIZE;
			branch->jump_bw =
				pbranch->jump_bw & DN_JBW_SIZE;
			CLE_NODE_DBG("  %d: VBit %d Mask %04x Data %04x OTyp %d "
					"NNBr %d%s,%d JRel %d JBkw %d NPOf %d\n",
					i, branch->valid, branch->mask, branch->data,
					branch->operation, pbranch->next_node_index,
					((pbranch->next_node_loc & 1) ? ".1" : ""),
					pbranch->next_node_loc >> 1,
					branch->jump_rel, branch->jump_bw,
					pbranch->next_packet_pointer);
#ifdef CARBON_MODEL
			branch->next_packet_pointer =
				pbranch->next_packet_pointer ?
				(pbranch->next_packet_pointer + 
					((!branch->jump_rel) * 4)) & DN_NBP_SIZE : 0;
#else
			branch->next_packet_pointer =
				pbranch->next_packet_pointer & DN_NBP_SIZE;
#endif
		}
		break;
#endif

	case KN:
		if (set_type)
			node->type = set_type & DN_TYPE_SIZE;
		kn = (struct ptree_kn *)pnode->data;
#ifdef CONFIG_APM862xx
		node->entry.kn[31 - index].priority = kn->priority & KN_PRI_SIZE;
		node->entry.kn[31 - index].result_pointer =
						kn->result_pointer & KN_PTR_SIZE;
		CLE_NODE_DBG(" KN: Type %d Indx %d Prio %d RPtr %d\n",
				node->type, index,
				node->entry.kn[31 - index].priority,
				node->entry.kn[31 - index].result_pointer);
#else
		node->entry.kn[index].priority = kn->priority & KN_PRI_SIZE;
		node->entry.kn[index].result_pointer =
						kn->result_pointer & KN_PTR_SIZE;
		CLE_NODE_DBG(" KN: Type %d Indx %d Prio %d RPtr %d\n",
				node->type, index,
				node->entry.kn[index].priority,
				node->entry.kn[index].result_pointer);
#endif
		break;
	}
}

int apm_ptree_node_config(u8 port, struct ptree_node *node)
{
	int rc = APM_RC_OK;
	u32 data[LONG_PER_PTREE_NODE_STRUCT];
	u32 node_index = node->index;
	struct apm_preclass_state *pcs;
	struct apm_preclass_node_state *pns;

	pcs = &sys_preclass_state[port];
	pns = &sys_preclass_state[port].node_entry_state[node_index];

	if (!apm_preclass_init_state)
		apm_preclassify_init();

	if (node->type == EWDN) { /* 8-way node */
		memset(data, 0, sizeof(data));

		/* check if this is the first config for this entry */
		if (!pns->type) {
			/* config + set type in hw */
			apm_ptree_set_data(data, node, EIGHT_WAY_NODE);
			/* set type */
			pns->type = EWDN;
			/* set location mask */
			pns->config_mask = 1;

			pcs->decision_node_8w_count++;
		/* check if its the same type of node */
		} else if (pns->type == node->type) {
			/* check if the entry is already configured */
			if (pns->config_mask) {
				/* prompt for reconfig */
				PCLS_PRNT("8 way node entry %d reconfig "
					"detected !!\n", node_index);
			} else {
				/* set location mask */
				pns->config_mask = 1;
			}
			/* always write everything */
			apm_ptree_set_data(data, node, EIGHT_WAY_NODE);
		/* This is a reconfig of a different type
		   (can allow for EWDN only) */
		} else {
			/* prompt for new type config */
			PCLS_PRNT("Node entry %d reconfig with new "
				"type detected !!\n", node_index);
			apm_ptree_set_data(data, node, EIGHT_WAY_NODE);

			if (pns->type == KN)
				pcs->key_node_count--;
			else
				pcs->decision_node_4w_count--;

			/* set type */
			pns->type = EWDN;
			/* set location mask */
			pns->config_mask = 1;

			pcs->decision_node_8w_count++;
		}

#ifdef CONFIG_APM862xx
	} else if (node->type == FWDN) { /* 4-way node */
		/* set specific fields of data array from node */
		/* based upon location, update the corresponding data
		   array check if this is the first config for this
		   entry */
		if (!pns->type) {

			PCLS_DBG("node type[%d], location[%d] \n",
				node->type, node->location);
 
			memset(data, 0, sizeof(data));

			/* config + set type in hw */
			if (node->location)
				apm_ptree_set_data(data, node,
					FOUR_WAY_NODE2_ONLY);
			else
				apm_ptree_set_data(data, node,
					FOUR_WAY_NODE1_ONLY);
			/* set type */
			pns->type = FWDN;
			/* set location mask */
			pns->config_mask |= 1 << node->location;

			pcs->decision_node_4w_count++;

		/* check if its the same type of node */
		} else if (pns->type == node->type) {

			PCLS_DBG("node type[%d], location[%d] \n",
				node->type, node->location);
 
			/* needs read modify write */
			rc = apm_get_ptree_node(port, node_index,
				(struct apm_ptree_node *)data);

			/* check if the entry is already configured */
			if ((pns->config_mask >> node->location) & 1) {

				/* prompt for reconfig */
				PCLS_PRNT("4 way decision node "
					"entry %d location %d reconfig "
					"detected !!\n",
					node_index,node->location);
				/* config with no node type update */
				apm_ptree_set_data(data,node,0);
			} else {
				/* set location mask */
				pns->config_mask |= 1 << node->location;
					
				PCLS_DBG("node type[%d], location[%d] \n",
					node->type, node->location);

				/* config with node type update */
				apm_ptree_set_data(data, node,
					FOUR_WAY_NODE_BOTH);
					pcs->decision_node_4w_count++;
			}
		}
#endif

	} else { /* Key node */
		/* check if this is the first config for this entry */
		if (!pns->type) {
			memset(data, 0, sizeof(data));
			/* config + set type in hw */
			apm_ptree_set_data(data,node,KEY_NODE);
			/* set type */
			pns->type = KN;
			/* set location mask */
			pns->config_mask |= 1 << node->location;

			pcs->key_node_count++;
		} else if (pns->type == node->type) {
			/* check if its the same type of node */
			/* check if the entry is already configured */
			if ((pns->config_mask >> node->location) & 1) {
				/* prompt for reconfig */
				PCLS_PRNT("Key node entry %d "
					"location %d reconfig detected !!\n",
					node_index, node->location);
			} else {
				/* set location mask */
				pns->config_mask |= 1 << node->location;
			}

			/* needs read modify write */
			rc = apm_get_ptree_node(port, node_index,
				(struct apm_ptree_node *)data);

			/* config */
			apm_ptree_set_data(data,node,0);

			pcs->key_node_count++;
		}
	}

	rc |= apm_set_ptree_node(port, node_index,
			(struct apm_ptree_node *)data);

	return rc;
}

int apm_set_ptree_node_branch(u8 port, u8 node_index,
			u8 sub_node, u8 branch_index,
			struct ptree_branch pbranch)
{
	int rc = APM_RC_INVALID_PARM;
	struct apm_preclass_node_state *pns;
	struct apm_ptree_node node;
	struct apm_ptree_branch *branch;

	pns = &sys_preclass_state[port].node_entry_state[node_index];

	switch (pns->type) {
	case EWDN:
		if (branch_index < 8) {
			rc = apm_get_ptree_node(port, node_index, &node);
			branch = &node.entry.ewdn.branch[branch_index];
			branch->valid = pbranch.valid & DN_VALID_SIZE;
			branch->mask = pbranch.mask & DN_MASK_SIZE;
			branch->data = pbranch.data & DN_MASK_SIZE;
			branch->operation = pbranch.operation & DN_OP_SIZE;
			branch->next_node_branch =
				((pbranch.next_node_index & DN_NN_SIZE) |
				((pbranch.next_node_loc & 0x1F) << 7)) & DN_NNP_SIZE;
			branch->jump_rel =
				pbranch.jump_rel & DN_JREL_SIZE;
			branch->jump_bw =
				pbranch.jump_bw & DN_JBW_SIZE;
#ifdef CARBON_MODEL
			branch->next_packet_pointer =
				pbranch.next_packet_pointer ?
			(pbranch.next_packet_pointer + 4) & DN_NBP_SIZE : 0;
#else
			branch->next_packet_pointer =
				pbranch.next_packet_pointer & DN_NBP_SIZE;
#endif
			rc |= apm_set_ptree_node(port, node_index, &node);
		} else {
			PCLS_PRNT("Invalid branch number for EWDN\n");
		}
		break;

#ifdef CONFIG_APM862xx
	case FWDN:
		if (sub_node < 2 && branch_index < 4) {
			rc = apm_get_ptree_node(port, node_index, &node);
			branch = &node.entry.fwdn[sub_node].branch[branch_index];
			branch->valid = pbranch.valid & DN_VALID_SIZE;
			branch->mask = pbranch.mask & DN_MASK_SIZE;
			branch->data = pbranch.data & DN_MASK_SIZE;
			branch->operation = pbranch.operation & DN_OP_SIZE;
			branch->next_node_branch =
				((pbranch.next_node_index & DN_NN_SIZE) |
				((pbranch.next_node_loc & 0x1F) << 7)) & DN_NNP_SIZE;
			branch->jump_rel =
				pbranch.jump_rel & DN_JREL_SIZE;
			branch->jump_bw =
				pbranch.jump_bw & DN_JBW_SIZE;
#ifdef CARBON_MODEL
			branch->next_packet_pointer =
				pbranch.next_packet_pointer ?
			(pbranch.next_packet_pointer + 4) & DN_NBP_SIZE : 0;
#else
			branch->next_packet_pointer =
				pbranch.next_packet_pointer & DN_NBP_SIZE;
#endif
			rc |= apm_set_ptree_node(port, node_index, &node);
		} else {
			PCLS_PRNT("Invalid sub_node or branch number for FWDN\n");
		}
		break;
#endif

	case KN:
		PCLS_PRNT("Entry [%d] is a key node, cannot "
				"configure a branch \n", node_index);
		break;

	default:
		PCLS_PRNT("Node type unknown for entry [%d], "
			"configure it first \n", node_index);
	} 

	return rc;
}

int apm_clear_ptree_node_type(u8 port, u8 node_index)
{
	int rc;
	struct apm_ptree_node node;

#ifdef CLE_DEBUG
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	/* read node */
	rc = apm_get_ptree_node(port, node_index, &node);
	/* clear node type in node */
	node.type = 0;
	/* write back */
	rc |= apm_set_ptree_node(port, node_index, &node);
	/* update state */
	sys_preclass_state[port].node_entry_state[node_index].type = 0;
	return rc;
}

int apm_get_preclass_state(u8 port,
			  struct apm_preclass_state *preclass_state)
{
	int rc = APM_RC_OK;
	
#ifdef CLE_DEBUG
	if (preclass_state == NULL) {
		PCLS_DBG("Null preclass_state pointer \n");
		return APM_RC_INVALID_PARM;
	}
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	memcpy(preclass_state, &sys_preclass_state[port],
		sizeof(struct apm_preclass_state));
	return rc;
}

int apm_set_sys_ptree_config(u8 port,
			struct apm_ptree_config *ptree_config)
{
	int rc = APM_RC_OK;
	u32 data;
	
#ifdef CLE_DEBUG
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	
	if (!apm_preclass_init_state)
		apm_preclassify_init();

	if (ptree_config != NULL) {
		/* update default config */
		memcpy(&sys_ptree_config[port], ptree_config,
			sizeof(struct apm_ptree_config));
	}

	PCLS_PRNT("Configuring start node ptr [%d] "
		"start packet ptr [%d] max hop [%d] for port %d\n",
		sys_ptree_config[port].start_node_ptr,
		sys_ptree_config[port].start_pkt_ptr,
		sys_ptree_config[port].max_hop, port);

	if (port == 0) {
		/* snptr */
		data = SNPTR0_WR(sys_ptree_config[port].start_node_ptr);
		rc = apm_gbl_cle_wr32(SNPTR0_ADDR, data);
		
		/* spptr */ 
		data = SPPTR0_WR(sys_ptree_config[port].start_pkt_ptr);
#ifdef CARBON_MODEL  /* FIXME */
		data = data + 4;
#endif		      
		rc |= apm_gbl_cle_wr32(SPPTR0_ADDR, data);
		/* Tree Max Hops */ 
		data = TMAXHOP0_WR(sys_ptree_config[port].max_hop);
		rc |= apm_gbl_cle_wr32(TMAXHOP0_ADDR, data);
		
		if (sys_ptree_config[port].start_engine == 0) {
			data = PARSER_HALT0_WR(1);
			rc |= apm_gbl_cle_wr32(PARSER_CTL0_ADDR, data);
		} else {
			sys_preclass_state[port].enabled = 1;
		}
		data = (DFCLSRESDBPRIORITY0_MASK & 
			sys_ptree_config[port].default_result) |
			(DFCLSRESDBPTR0_MASK &
			sys_ptree_config[port].default_result);
		rc |= apm_gbl_cle_wr32(DFCLSRESDBPTR0_ADDR, data);
		/* Assign Default Classification DB Ptr */
		PCLS_PRNT("Assign Default Classification Priority %d DB Ptr %d\n",
			DFCLSRESDBPRIORITY0_RD(data), DFCLSRESDBPTR0_RD(data));
	} else if (port == 1) {
		/* snptr */
		data = SNPTR1_WR(sys_ptree_config[port].start_node_ptr);
		rc = apm_gbl_cle_wr32(SNPTR1_ADDR, data);
		
		/* spptr */ 
		data = SPPTR1_WR(sys_ptree_config[port].start_pkt_ptr);
#ifdef CARBON_MODEL  /* FIXME */
		data = data + 4;
#endif			
		rc |= apm_gbl_cle_wr32(SPPTR1_ADDR, data);
		/* Tree Max Hops */ 
		data = TMAXHOP1_WR(sys_ptree_config[port].max_hop);
		rc |= apm_gbl_cle_wr32(TMAXHOP1_ADDR, data);
		
		if (sys_ptree_config[port].start_engine == 0) {
			data = PARSER_HALT1_WR(1);
			rc |= apm_gbl_cle_wr32(PARSER_CTL1_ADDR, data);
		} else {
			sys_preclass_state[port].enabled = 1;
		}
		data = (DFCLSRESDBPRIORITY1_MASK & 
			sys_ptree_config[port].default_result) |
			(DFCLSRESDBPTR1_MASK &
			sys_ptree_config[port].default_result);
		rc |= apm_gbl_cle_wr32(DFCLSRESDBPTR1_ADDR, data);
		/* Assign Default Classification DB Ptr */
		PCLS_PRNT("Assign Default Classification Priority %d DB Ptr %d\n",
			DFCLSRESDBPRIORITY1_RD(data), DFCLSRESDBPTR1_RD(data));
	}
	return rc;
}

int apm_get_sys_ptree_config(u8 port,
				struct apm_ptree_config *ptree_config)
{
	int rc = APM_RC_OK;
	
#ifdef CLE_DEBUG
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
	if (ptree_config  == NULL) {
		PCLS_DBG("Null ptree_config pointer \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	memcpy(ptree_config, &sys_ptree_config[port],
	       sizeof(struct apm_ptree_config));
	return rc;
}

int apm_halt_preclassifier(u8 port)
{
	int rc = APM_RC_OK;
	u32 data;
	
#ifdef CLE_DEBUG
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	
	if (!apm_preclass_init_state)
		apm_preclassify_init();

	if (sys_preclass_state[port].enabled) {
		PCLS_DBG("Shutting down preclassification "
			"engine for port %d \n", port);
		data = PARSER_HALT0_WR(1);
		rc |= apm_gbl_cle_wr32(PARSER_CTL0_ADDR, data);
	} else {
		PCLS_PRNT("Preclassification engine for "
			"port %d already shut !!\n", port);
	}
	return rc;
}

int apm_start_preclassifier(u8 port)
{
	int rc = APM_RC_OK;
	u32 data;
	
#ifdef CLE_DEBUG
	if (port > MAX_NO_OF_PORTS) {
		PCLS_DBG("Invalid port number \n");
		return APM_RC_INVALID_PARM;
	}
#endif
	if (!apm_preclass_init_state)
		apm_preclassify_init();

	if (sys_preclass_state[port].enabled == 0) {
		PCLS_DBG("Starting preclassification "
			"engine for port %d \n", port);
		data = PARSER_HALT0_WR(0);
		rc |= apm_gbl_cle_wr32(PARSER_CTL0_ADDR, data);
	} else {
		PCLS_PRNT("Preclassification engine for "
			"port %d already running !!\n", port);
	}
	return rc;
}

int apm_get_preclass_trace(u8 port, struct apm_preclass_trace *preclass_trace)
{
	int i, rc = APM_RC_OK;
	u32 data = 0;

	switch(port) {
	case 0:
		rc |= apm_gbl_cle_rd32(LSTNVST0_ADDR, &data);	
		preclass_trace->last_node_visited = LSTNVST0_RD(data);
		
		for (i = 0; i< MAX_LAST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (LSTTRCNVST0_0_ADDR + (i<<2),
				       		&data);
			preclass_trace->last_visited_node_trace[i] =
							LTRCNVST0_RD(data);
		}
		for (i = 0; i< MAX_FIRST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (FTRCNVST0_0_ADDR + (i<<2),
					      	&data);
			preclass_trace->first_visited_node_trace[i] =
							FTRCNVST0_RD(data);
		}
		break;
		
	case 1:

		rc |= apm_gbl_cle_rd32(LSTNVST1_ADDR, &data);	
		preclass_trace->last_node_visited = LSTNVST1_RD(data);
		
		for (i = 0; i< MAX_LAST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (LSTTRCNVST1_0_ADDR + (i<<2),
					       	&data);
			preclass_trace->last_visited_node_trace[i] =
							LTRCNVST1_RD(data);
		}
		for (i = 0; i< MAX_FIRST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (FTRCNVST1_0_ADDR + (i<<2),
					       	&data);
			preclass_trace->first_visited_node_trace[i] =
							FTRCNVST1_RD(data);
		}
		break;
		
	case 2:
		rc |= apm_gbl_cle_rd32(LSTNVST2_ADDR, &data);	
		preclass_trace->last_node_visited = LSTNVST2_RD(data);
		
		for (i = 0; i< MAX_LAST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (LSTTRCNVST2_0_ADDR + (i<<2),
					       &data);
			preclass_trace->last_visited_node_trace[i] =
							LTRCNVST2_RD(data);
		}
		for (i = 0; i< MAX_FIRST_NODES_TRACE; i++) {
			rc |= apm_gbl_cle_rd32 (FTRCNVST2_0_ADDR + (i<<2),
					       &data);
			preclass_trace->first_visited_node_trace[i] =
							FTRCNVST2_RD(data);
		}
		break;
		
	default:
		break;
	}
	return rc;
}

int apm_preclass_wol_mode(int enable)
{

	int rc;
	u32 snptr;
	rc = apm_gbl_cle_rd32(SNPTR0_ADDR, &snptr);
	snptr = WOL_MODE0_SET(snptr, enable);
	rc |= apm_gbl_cle_wr32(SNPTR0_ADDR, snptr);
	return rc;
}
