/**
 * AppliedMicro APM862xx SoC Ethernet Driver
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
 * @file apm_enet_ioctl.c
 *
 * This file defines IOCTL interface for APM862xx SoC Ethernet Driver
 *
 */

#include "apm_enet_access.h"
#include "apm_enet_csr.h"
#include "apm_enet_ioctl.h"
#include "apm_enet_lro.h"
#include "apm_enet_slimpro_ipfw.h"

int apm_enet_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;
	struct apm_enet_ioctl *ioc;
	/* CLE Related parameters */
	char *apm_ptree_id = NULL;
	struct apm_ptree_config *ptree_config = NULL;
	struct apm_cle_dbptr *dbptr = NULL;
	struct avl_node *avl_node_cfg;
	int rc = 0, port;
	u32 data;

	//handle some real ioctls in addition to the apm specific ones
	u32 read_val = 0;
	struct mii_ioctl_data* md = (struct mii_ioctl_data*)(&rq->ifr_ifru.ifru_data);

	switch (cmd) {

		case SIOCGMIIREG:
			apm_genericmiiphy_read(priv, md->phy_id, (unsigned char)md->reg_num, &read_val);
			md->val_out = (uint16_t)read_val;
			return 0;
		break;
		case SIOCSMIIREG:
			apm_genericmiiphy_write(priv, md->phy_id, (unsigned char)md->reg_num, md->val_in);
			return 0;
		break;
		default:
			break;
	}

	ioc = kmalloc(sizeof(*ioc), GFP_KERNEL);
	if (ioc == NULL)
		return -ENOMEM;

	if (copy_from_user(ioc, rq->ifr_ifru.ifru_data,
			sizeof(struct apm_enet_ioctl))) {
		rc = -EFAULT;
		goto err;
	}
	
	switch (ioc->cmd) {
	case SIOREGACCESS:
	{
		struct apm_enet_reg_op *reg_ops;
		reg_ops = &ioc->ioctl_ops.reg_ops;
		
		ENET_DEBUG("cmd id: %d block_id: %d reg_offset: %d "
			"data[0]: %d\n", reg_ops->cmd_id, reg_ops->block_id,
			reg_ops->reg_offset, reg_ops->data[0]);
		
		if (reg_ops->cmd_id == READ_CMD) {			  
			if (reg_ops->block_id == BLOCK_RGMII) {
				apm_miiphy_read(priv, reg_ops->reg_offset,
						&reg_ops->data[0]);
			} else	{
				if (reg_ops->reg_offset) {
					apm_enet_rd32(priv, reg_ops->block_id,
						reg_ops->reg_offset,
						&reg_ops->data[0]);
				} else {
					int i;
					for(i = 0; i < 15; i++) {
						apm_enet_rd32(priv,
							reg_ops->block_id,
							i*4,
							&reg_ops->data[i]);
					}
				}
			}
		} else if (reg_ops->cmd_id == WRITE_CMD) {
			ENET_DEBUG("Writing data: 0x%08x\n", reg_ops->data[0]);
			
			if (reg_ops->block_id == BLOCK_RGMII) {
				apm_miiphy_write(priv, reg_ops->reg_offset,
						reg_ops->data[0]);
			} else {
				apm_enet_wr32(priv, reg_ops->block_id,
					reg_ops->reg_offset,
					reg_ops->data[0]);
			}
		} else {
			printk(KERN_ERR "Invalid cmd in reg access/n");
			rc = -EINVAL;
			goto err;
		}
		break;
	}
	case SIOCIPAUSETHR:
		port = find_port(rq->ifr_name);
		
		apm_enet_rd32(priv, BLOCK_ETH_GBL,
				RXBUF_CONFIG_PAUSE_THRESH_REG_ADDR, &data);
		if (port == GIGE_PORT0) {
			data |= CFG_RXBUF0_PAUSE_THRESH_WR(
				ioc->ioctl_ops.pause_thr);
		} else if (port == GIGE_PORT1) {
			data |= CFG_RXBUF1_PAUSE_THRESH_WR(
				ioc->ioctl_ops.pause_thr);
		}
		apm_enet_wr32(priv, BLOCK_ETH_GBL,
				RXBUF_CONFIG_PAUSE_THRESH_REG_ADDR, data);
		break;
		
	case SIOCESENDPAUSEFRAME:
		break;
		
	/* CLE Operations  */
	case SIOCPRECLASSSTATE:
	{
		struct apm_preclass_state *preclass_state;
		preclass_state = &ioc->ioctl_ops.status_cfg.preclass_state;

		if (preclass_state == NULL) {
			rc = -EINVAL;
			goto err;
		}

		port = ioc->ioctl_ops.status_cfg.port;
		if ((rc = apm_get_preclass_state(port,
					preclass_state)) !=  APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCPRECLASSSTATE"
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCPRECLASSTRACE:
	{
		struct apm_preclass_trace *preclass_trace;
		preclass_trace = &ioc->ioctl_ops.trace_cfg.preclass_trace;

		if (preclass_trace == NULL) {
			rc = -EINVAL;
			goto err;
		}

		port = ioc->ioctl_ops.trace_cfg.port;
		if ((rc = apm_get_preclass_trace(port,
				preclass_trace)) !=  APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL"
				"SIOCPRECLASSTRACE operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCGSYSPTREECONFIG:
	{
		struct apm_ptree_config ptree_config;

		port = ioc->ioctl_ops.ptree_cfg.port;
		if ((rc = apm_get_sys_ptree_config(port,
			     &ptree_config)) !=  APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL "
				"SIOCGSYSPTREECONFIG operation \n", rc);
			rc = -EFAULT;
			goto err;
		}

		if (copy_to_user(ioc->ioctl_ops.ptree_cfg.ptree_config,
				&ptree_config,
				sizeof(struct apm_ptree_config))) {
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCSSYSPTREECONFIG:
	{
		struct apm_ptree_config ptree_config;

		port = ioc->ioctl_ops.ptree_cfg.port;
		if (copy_from_user(&ptree_config,
				ioc->ioctl_ops.ptree_cfg.ptree_config,
				sizeof(struct apm_ptree_config))) {
			rc = -EFAULT;
			goto err;
		}

		if ((rc = apm_set_sys_ptree_config(port,
			     &ptree_config)) !=  APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL "
				"SIOCSSYSPTREECONFIG operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCGPTREECONFIG:
	{
		apm_ptree_id = ioc->ioctl_ops.ptree_cfg.ptree_id;
		port = ioc->ioctl_ops.ptree_cfg.port;
		ptree_config = apm_find_ptree_config(port, apm_ptree_id);

		if (ptree_config == NULL) {
			rc = -EINVAL;
			ENET_DEBUG_ERR("Error %d in IOCTL "
				"SIOCGPTREECONFIG operation \n", rc);
			goto err;
		}

		if (copy_to_user(ioc->ioctl_ops.ptree_cfg.ptree_config,
				ptree_config,
				sizeof(struct apm_ptree_config))) {
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCSPTREECONFIG:
	{
		apm_ptree_id = ioc->ioctl_ops.ptree_cfg.ptree_id;
		port = ioc->ioctl_ops.ptree_cfg.port;
		ptree_config = apm_find_ptree_config(port, apm_ptree_id);

		if (ptree_config == NULL) {
			rc = -EINVAL;
			ENET_DEBUG_ERR("Error %d in IOCTL "
				"SIOCSPTREECONFIG operation \n", rc);
			goto err;
		}

		if (copy_from_user(ptree_config,
				ioc->ioctl_ops.ptree_cfg.ptree_config,
				sizeof(struct apm_ptree_config))) {
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCPRECLASSHALT:
		if ((rc = apm_halt_preclassifier(find_port(rq->ifr_name)))
		    != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCPRECLASSHALT "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
		
	case SIOCSCLEDBPTR:
	{
		dbptr = &ioc->ioctl_ops.dbptr;
		if ((rc = apm_set_cle_dbptr(dbptr)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCSCLEDBPTR "
				       "operation  \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCPTREEALLOC:
	{
		u8 ptnodes, ptdn = 0, ptkn = 0;
		u16 dbptrs, ptbr = 0;
		int i;
		struct ptree_branch *branch = NULL;
		struct ptree_dn *dn = NULL;
		struct ptree_kn *kn = NULL;
		struct ptree_node *node = NULL;
		struct apm_ptree_alloc_param *alloc_cfg;

		alloc_cfg = &ioc->ioctl_ops.ptree_alloc_cfg;

		if (alloc_cfg == NULL) {
			rc = -EINVAL;
			goto err;
		}

		port = alloc_cfg->port;
		ptnodes = alloc_cfg->ptnodes;
		dbptrs = alloc_cfg->dbptrs;
		apm_ptree_id = alloc_cfg->ptree_id;

		if (ptnodes == 0 || ptnodes > MAX_NODE_ENTRIES ||
			dbptrs > MAX_DB_ENTRIES) {
			rc = -EINVAL;
			goto err;
		}

		ptree_config = apm_add_ptree_config(port, apm_ptree_id);
		if (ptree_config == NULL) {
			ENET_DEBUG_ERR("Add Patricia Tree %s Configuration "
				"error for port %d\n", apm_ptree_id, port);
			rc = -EFAULT;
			goto end_ptree_alloc;
		}

		if (dbptrs) {
			dbptr = kmalloc(sizeof(struct apm_cle_dbptr) * dbptrs,
				GFP_KERNEL);
			if (dbptr == NULL) {
				rc = -EINVAL;
				goto end_ptree_alloc;
			}

			if (copy_from_user(dbptr, alloc_cfg->dbptr,
				sizeof(struct apm_cle_dbptr) * dbptrs)) {
				rc = -EFAULT;
				goto end_ptree_alloc;
			}
		}

		node = kmalloc(sizeof(struct ptree_node) * ptnodes,
				GFP_KERNEL);
		if (node == NULL) {
			rc = -ENOMEM;
			goto end_ptree_alloc;
		}

		if (copy_from_user(node, alloc_cfg->node,
				sizeof(struct ptree_node) * ptnodes)) {
			rc = -EFAULT;
			goto end_ptree_alloc;
		}

		for (i = 0; i < ptnodes; i++) {

			if ((rc = apm_ptree_node_precheck(port, &node[i])) !=
				APM_RC_OK) {
				ENET_DEBUG_ERR("Patricia Tree Node Precheck "
					"error %d\n", rc);
				rc = -EFAULT;
				goto end_ptree_alloc;
			}
		
			if (node[i].type != KN)
				ptdn++;
			else
				ptkn++;
		}

		if (!ptdn || !ptkn) {
			rc = -EINVAL;
			goto end_ptree_alloc;
		}

		dn = kmalloc(sizeof(struct ptree_dn) * ptdn,
				GFP_KERNEL);
		if (dn == NULL) {
			rc = -ENOMEM;
			goto end_ptree_alloc;
		}

		kn = kmalloc(sizeof(struct ptree_kn) * ptkn,
				GFP_KERNEL);
		if (kn == NULL) {
			rc = -ENOMEM;
			goto end_ptree_alloc;
		}

		ptdn = 0;
		ptkn = 0;
		for (i = 0; i < ptnodes; i++) {

			if (node[i].type != KN) {

				if (copy_from_user(&dn[ptdn],
						alloc_cfg->node[i].data,
						sizeof(struct ptree_dn))) {
					rc = -EFAULT;
					goto end_ptree_alloc;
				}

				node[i].data = &dn[ptdn];

				if ((rc = apm_ptree_branch_precheck(port, &node[i])) !=
						APM_RC_OK) {
					ENET_DEBUG_ERR("Patricia Tree Branch Precheck "
						"error %d\n", rc);
					rc = -EFAULT;
					goto end_ptree_alloc;
				}

				ptbr += dn[ptdn].num;
				ptdn++;

			} else {

				if (copy_from_user(&kn[ptkn],
						alloc_cfg->node[i].data,
						sizeof(struct ptree_kn))) {
					rc = -EFAULT;
					goto end_ptree_alloc;
				}

				node[i].data = &kn[ptkn];
				ptkn++;
			}
		}

		if (!ptbr) {
			rc = -EINVAL;
			goto end_ptree_alloc;
		}

		branch = kmalloc(sizeof(struct ptree_branch) * ptbr,
				GFP_KERNEL);
		if (branch == NULL) {
			rc = -ENOMEM;
			goto end_ptree_alloc;
		}

		ptbr = 0;
		for (i = 0; i < ptdn; i++) {

			if (copy_from_user(&branch[ptbr],
					dn[i].branch,
					sizeof(struct ptree_branch) *
					dn[i].num)) {
				rc = -EFAULT;
				goto end_ptree_alloc;
			}
			dn[i].branch = &branch[ptbr];
			ptbr += dn[i].num;

		}

		if ((rc = apm_ptree_alloc(port, ptnodes, dbptrs, node,
				dbptr, ptree_config)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL "
					"SIOCPTREEALLOC "
					"operation \n", rc);
			rc = -EFAULT;
			goto err;
		}

		if (copy_to_user(alloc_cfg->ptree_config, ptree_config,
				sizeof(struct apm_ptree_config))) {
			rc = -EFAULT;
			goto end_ptree_alloc;
		}

end_ptree_alloc:
		if (branch)
			kfree(branch);

		if (dn)
			kfree(dn);

		if (kn)
			kfree(kn);

		if (node)
			kfree(node);

		if (dbptr)
			kfree(dbptr);

		if (rc)
			goto err;

		break;
	}
	case SIOCPTREENODECONFIG:
	{
		struct ptree_node_param *node_cfg;
		struct ptree_node *node;

		node_cfg = &ioc->ioctl_ops.ptree_node_cfg;

		if (node_cfg == NULL) {
			rc = -EINVAL;
			goto err;
		}

		node = &node_cfg->node;
		port = node_cfg->port;

		if ((rc = apm_ptree_node_precheck(port, node)) !=
				APM_RC_OK) {
			ENET_DEBUG_ERR("Patricia Tree Node Precheck error %d\n",
				rc);
			rc = -EFAULT;
			goto err;
		}

		if (node->type != KN) {
			struct ptree_dn dn, *dn_ptr;
			struct ptree_branch branch[8];

			dn_ptr = (struct ptree_dn *)node->data;
			if (copy_from_user(&dn, dn_ptr,
					sizeof(struct ptree_dn))) {
				rc = -EFAULT;
				goto err;
			}
			node->data = &dn;

			if ((rc = apm_ptree_branch_precheck(port, node)) !=
					APM_RC_OK) {
				ENET_DEBUG_ERR("Patricia Tree Branch Precheck "
					"error %d\n", rc);
				rc = -EFAULT;
				goto err;
			}

			if (copy_from_user(branch, dn_ptr->branch,
					sizeof(struct ptree_branch) *
					dn.num)) {
				rc = -EFAULT;
				goto err;
			}
			dn.branch = branch;

		} else {
			struct ptree_kn kn;

			if (copy_from_user(&kn, node->data,
					sizeof(struct ptree_kn))) {
				rc = -EFAULT;
				goto err;
			}
			node->data = &kn;

		}

		if ((rc = apm_ptree_node_check(port, node, 0)) !=
				APM_RC_OK) {
			ENET_DEBUG_ERR("Patricia Tree Node-Check error %d\n",
				rc);
			rc = -EFAULT;
			goto err;
		}

		if ((rc = apm_ptree_node_config(port, node)) !=
				APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCPTREENODECONFIG "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}

		break;
	}
	case SIOCPTREENODECLEAR:
	{
		u8 ptnode_index = ioc->ioctl_ops.ptnode_index;
		if ((rc = apm_clear_ptree_node_type(
			     find_port(rq->ifr_name),
			     ptnode_index)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL "
				"SIOCPTREENODECLEAR operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
	}
	case SIOCCLEAVLINIT:
		if ((rc = apm_cle_avl_init(ioc->ioctl_ops.key_size))
		    != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCCLEAVLINIT "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
		
	case SIOCCLEAVLADD:
		avl_node_cfg = &ioc->ioctl_ops.avl_node_cfg;
		if ((rc = apm_cle_avl_add_node(avl_node_cfg)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCCLEAVLADD "
					"operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
		
	case SIOCCLEAVLDEL:
		avl_node_cfg = &ioc->ioctl_ops.avl_node_cfg;
		if ((rc = apm_cle_avl_del_node(avl_node_cfg)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCCLEAVLDEL "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
		
	case SIOCCLEAVLSRCH:
		avl_node_cfg = &ioc->ioctl_ops.avl_node_cfg;
		if ((rc = apm_cle_avl_search_node(avl_node_cfg)) !=
				APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCCLEAVLSRCH "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break;
		
	case SIOCCLEAVLSTATUS:
	{
		struct apm_cle_avl_status *avl_status;
		avl_status = &ioc->ioctl_ops.avl_status;
		if ((rc = apm_cle_avl_get_status(avl_status)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL SIOCCLEAVLSTATUS "
				       "operation \n", rc);
			rc = -EFAULT;
			goto err;
		}
		break; 
	}
	case SIOCAVLALLOC:
	{
		u16 avlnodes;
		u16 dbptrs;
		struct avl_node *node = NULL;
		struct apm_avl_alloc_param *alloc_cfg;

		alloc_cfg = &ioc->ioctl_ops.avl_alloc_cfg;

		if (alloc_cfg == NULL) {
			rc = -EINVAL;
			goto err;
		}

		port = alloc_cfg->port;
		avlnodes = alloc_cfg->avlnodes;
		dbptrs = alloc_cfg->dbptrs;

		if (avlnodes == 0 || avlnodes > AVL_SSIZE8B_DPTH ||
			dbptrs > MAX_DB_ENTRIES) {
			rc = -EINVAL;
			goto err;
		}

		if (dbptrs) {
			dbptr = kmalloc(sizeof(struct apm_cle_dbptr) * dbptrs,
				GFP_KERNEL);
			if (dbptr == NULL) {
				rc = -EINVAL;
				goto end_avl_alloc;
			}

			if (copy_from_user(dbptr, alloc_cfg->dbptr,
				sizeof(struct apm_cle_dbptr) * dbptrs)) {
				rc = -EFAULT;
				goto end_avl_alloc;
			}
		}

		node = kmalloc(sizeof(struct avl_node) * avlnodes,
				GFP_KERNEL);
		if (node == NULL) {
			rc = -ENOMEM;
			goto end_avl_alloc;
		}

		if (copy_from_user(node, alloc_cfg->node,
				sizeof(struct avl_node) * avlnodes)) {
			rc = -EFAULT;
			goto end_avl_alloc;
		}

		if ((rc = apm_avl_alloc(port, avlnodes, dbptrs, node,
				dbptr)) != APM_RC_OK) {
			ENET_DEBUG_ERR("Error %d in IOCTL "
					"SIOCAVLALLOC "
					"operation \n", rc);
			rc = -EFAULT;
			goto err;
		}

end_avl_alloc:
		if (node)
			kfree(node);

		if (dbptr)
			kfree(dbptr);

		if (rc)
			goto err;

		break; 
	}
	case SIOCSQMMSG16:
	{
		u16 qm_msg16 = ioc->ioctl_ops.qm_msg16;
		priv_dev->in_poll_rx_msg_desc.is_msg16 = qm_msg16;
		break;
	}
#if defined(CONFIG_APM_ENET_LRO)
	case SIOLROENABLE: /* Enable LRO */
		apm_enet_lro_enable(priv_dev, ioc->ioctl_ops.lro_enable);
		break;
	case SIOLROPORT:
		apm_enet_lro_set_port(priv_dev, ioc->ioctl_ops.lro_port);
		break;
	case SIOLROTIMEOUT:
		apm_enet_lro_set_timeout(priv_dev, 
					ioc->ioctl_ops.lro_timeoutms);
		break;
	case SIOLROBYTECNT:
		apm_enet_lro_set_maxbytecnt(priv_dev, 
					ioc->ioctl_ops.lro_maxbytecnt);
		break;
#endif		
#if defined(CONFIG_APM_ENET_SLIMPRO_IPFW)
	case SIOSLIMPROIPFWENABLE:
		apm_enet_slimpro_ipfw_enable(priv_dev, 
					ioc->ioctl_ops.slimpro_ipfw_enable);
		break;	
	case SIOSLIMPROIPFWIPADDR:
		apm_enet_slimpro_ipfw_set_ipaddr(priv_dev, 
					ioc->ioctl_ops.slimpro_ipfw_ipaddr);
		break;					
#endif
	default:
		ENET_DEBUG_ERR("Error Invalid IOCTL command \n");
		rc = -EINVAL;
		goto err;
		break;
	}
	/* Copy required data  to user */
	if (copy_to_user(rq->ifr_ifru.ifru_data, ioc, 
			sizeof(struct apm_enet_ioctl))) {
		rc = -EFAULT;
	}
err:
	kfree(ioc);
	return rc;	  
}
