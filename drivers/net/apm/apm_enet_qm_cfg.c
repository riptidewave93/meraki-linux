/*
 * AppliedMicro APM862xx SoC Ethernet Driver QM Configuration
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
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
 */

#include <linux/kernel.h>
#include "apm_enet_access.h"

extern struct apm_enet_dev_base * qid2priv[MB_MAX_QUEUES];

static void apm_enet_qm_avb_poll(unsigned long data)
{
	struct apm_enet_dev_base *priv_dev = (struct apm_enet_dev_base*)data;
	struct apm_qm_raw_qstate qstate = {0};
	u32 num_message = 0;
	int rc = 0;
	u32 pbn = 0;
	u32 buf = 0;
	u32 core;
	u16 tx_idx;

	if(priv_dev->q_poll_timer_status == Q_POLL_TIMER_STOP) {
		priv_dev->q_poll_timer_status = Q_POLL_TIMER_OFF;
		return;
	}
	
	/* FIXME: This only handles one VQ per port */
	/* FIXME: This needs to be fixed for SMP */
	/* FIXME: Also tx[0] is not correct when multiple VQ are used*/
	/* FIXME: Need to also fix RX case, if RX VQs are used */

#ifdef CONFIG_SMP
	core = 0;
#else
	core = apm_processor_id();
#endif

	tx_idx = priv_dev->qm_queues[core].tx_idx;
	if (priv_dev->qm_queues[core].tx[tx_idx].arb != QM_CFG_ARB_AVB)
		goto avb_poll_exit;

	pbn = priv_dev->qm_queues[core].tx[tx_idx].pbn;
			
	rc = apm_qm_raw_qstate_rd(IP_BLK_QM,
				 priv_dev->qm_queues[core].tx[tx_idx].qid,
				 &qstate);

	num_message = qstate.w3 & 0x1FFFF;
#if 0 	
	rc = apm_qm_cstate_rd(11, cstate);
	printk("Credit State[11]: cstate[0]: %d and cstate[1]: %d \n",
	       cstate[0], cstate[1]);

	printk("Qstate[%d]: w[0]: 0x%x, w[1]: 0x%x, w[2]: 0x%x, w[3]: 0x%x \n",
	       port1_tx_vqid[0],qstate.w0, qstate.w1, qstate.w2, qstate.w3);

	memset(&qstate, 0, sizeof(struct q_raw_state));
	rc = apm_qm_raw_qstate_rd(IP_BLK_QM, 11, &qstate);
 
	printk("Qstate[11]: w[0]: 0x%x, w[1]: 0x%x, w[2]: 0x%x, w[3]: 0x%x \n",
	       qstate.w0, qstate.w1, qstate.w2, qstate.w3);


	memset(&qstate, 0, sizeof(struct q_raw_state));
	rc = apm_qm_raw_qstate_rd(IP_BLK_QM, 12, &qstate);
	
	printk("Qstate[12]: w[0]: 0x%x, w[1]: 0x%x, w[2]: 0x%x, w[3]: 0x%x \n",
	       qstate.w0, qstate.w1, qstate.w2, qstate.w3);
#endif 	

#if 0
	if (num_message >= 0)
		printk("After nummsgs: %d \n", num_message);
#endif 	
	
	if (num_message == 0 && priv_dev->pb_enabled) {
		/* Disable PBN */
		buf = apm_qm_pb_get(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn);
		printk("Disable: Before pbn_buf:0x%X ", buf);
		priv_dev->pb_enabled = 0;
		buf &= ~(1 << 14);
		printk("after pbn_buf:0x%X \n", buf);
		apm_qm_pb_set(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn, buf);
		
	} else if (num_message >= 8 && (!priv_dev->pb_enabled)) {
		buf = apm_qm_pb_get(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn);
		printk("Enable: Before pbn_buf:0x%X ", buf);
		buf |= (1 << 14);
		printk("after pbn_buf:0x%X \n", buf);
		priv_dev->pb_enabled = 1;
		apm_qm_pb_set(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn, buf);
		
	}
avb_poll_exit:
	priv_dev->q_poll_timer.expires = jiffies + jiffies_to_msecs(100);
	add_timer(&priv_dev->q_poll_timer);
	return;
}

static void apm_enet_start_qm_avb_poll(struct apm_enet_dev_base *priv_dev)
{
	if (priv_dev->q_poll_timer_status == Q_POLL_TIMER_ON)
		return; /* already on */

	init_timer(&priv_dev->q_poll_timer);
	priv_dev->q_poll_timer_status = Q_POLL_TIMER_ON;
	priv_dev->q_poll_timer.expires = jiffies + jiffies_to_msecs(1000);
	priv_dev->q_poll_timer.data = (unsigned long) priv_dev;
	priv_dev->q_poll_timer.function = apm_enet_qm_avb_poll;
	printk("Add timer ....... \n");
	add_timer(&priv_dev->q_poll_timer);
}

void apm_enet_stop_qm_avb_poll(struct apm_enet_dev_base *priv_dev)
{
	priv_dev->q_poll_timer_status = Q_POLL_TIMER_STOP;
	while (priv_dev->q_poll_timer_status != Q_POLL_TIMER_OFF)
		udelay(ACCESS_DELAY_TIMEMS);
}

int apm_enet_add_qconfig(unsigned long priv_data,
			 struct qm_cfg_qconfig *qconfig)
{
	struct apm_enet_dev_base *priv_dev;
	struct eth_queue_ids *queue_ids;
	unsigned int port;
	unsigned int core;
	int i, q;
	int rc = 0;

	priv_dev = (struct apm_enet_dev_base *)priv_data;
	port = priv_dev->port_id;
	core = qconfig->ppc;
	queue_ids = &priv_dev->qm_queues[core];

	if (qconfig->dir == QM_CFG_DIR_EGRESS &&
	    (qconfig->qtype == QM_CFG_QTYPE_PQ ||
	     qconfig->qtype == QM_CFG_QTYPE_VQ)) {
		if (queue_ids->tx_count >= ENET_MAX_PBN) {
			printk(KERN_ERR "Error: MAX PBNs reached\n");
			printk(KERN_ERR "Error: could not allocate egress "
				"work queues for ETH%d PPC=%d\n", port, core);
			goto exit_update_qconfig;
		}

		if ((rc = qm_cfg_add_qconfig(qconfig)) != 0) {
			printk(KERN_ERR "Error: could not allocate egress "
				"work queues for ETH%d PPC=%d\n", port, core);
			goto exit_update_qconfig;
		}

		q = queue_ids->tx_count;
		memset(&queue_ids->tx[q], 0, sizeof(struct eth_qids));
		queue_ids->tx_count++;

		queue_ids->tx[q].qtype = qconfig->qtype;
		queue_ids->tx[q].qid   = qconfig->qid;
		queue_ids->tx[q].pbn   = qconfig->pbn;
		queue_ids->tx[q].mbox  = qconfig->mbox;

		if (qconfig->qtype == QM_CFG_QTYPE_VQ) {
			queue_ids->tx[q].qcount = qconfig->qcount;
			queue_ids->tx[q].arb = qconfig->qsel_arb[0];
			for (i = 0; i < qconfig->qcount; i++) {
				queue_ids->tx[q].qsel[i] =
					qconfig->qsel_qid[i];
			}
		}

		if (queue_ids->tx[q].arb == QM_CFG_ARB_AVB) {
			u32 buf;
			u32 pbn = queue_ids->tx[q].pbn;
			buf = apm_qm_pb_get(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn);
			printk("Disable: Before pbn_buf :0x%X ", buf);
			priv_dev->pb_enabled = 0; /* FIXME: should be per VQ */
			buf &= ~(1 << 14);
			printk("after pbn_buf:0x%X \n", buf);
			apm_qm_pb_set(IP_BLK_QM, PB_SLAVE_ID_ETH, pbn, buf);

			apm_enet_start_qm_avb_poll(priv_dev);
		}
	} else if (qconfig->dir == QM_CFG_DIR_INGRESS &&
		   (qconfig->qtype == QM_CFG_QTYPE_PQ ||
		    qconfig->qtype == QM_CFG_QTYPE_VQ)) {

		if (queue_ids->rx_count >= ENET_MAX_PBN) {
			printk(KERN_ERR "Error: MAX PBNs reached\n");
			printk(KERN_ERR "Error: could not allocate ingress "
				"work queues for ETH%d PPC=%d\n", port, core);
			goto exit_update_qconfig;
		}

		if ((rc = qm_cfg_add_qconfig(qconfig)) != 0) {
			printk(KERN_ERR "Error: could not allocate ingress "
				"work queues for ETH%d PPC=%d\n", port, core);
			goto exit_update_qconfig;
		}

		q = queue_ids->rx_count;
		memset(&queue_ids->rx[q], 0, sizeof(struct eth_qids));
		queue_ids->rx_count++;

		queue_ids->rx[q].qtype = qconfig->qtype;
		queue_ids->rx[q].qid   = qconfig->qid;
		queue_ids->rx[q].pbn   = qconfig->pbn;
		queue_ids->rx[q].mbox  = qconfig->mbox;

		if (qconfig->qtype == QM_CFG_QTYPE_VQ) {
			int i;
			queue_ids->rx[q].qcount = qconfig->qcount;
			queue_ids->rx[q].arb = qconfig->qsel_arb[0];
			for (i = 0; i < qconfig->qcount; i++) {
				queue_ids->rx[q].qsel[i] =
					qconfig->qsel_qid[i];
				/* Save this for reverse lookup */
				qid2priv[qconfig->qsel_qid[i]] = priv_dev;
			}
		}

		qid2priv[qconfig->qid] = priv_dev;
#ifdef SMP_LOAD_BALANCE		
		if (port == GIGE_PORT1 &&
	    	    (rc = apm_qm_set_mb_affinity(qconfig->mbox, CORE1)) != 0) {
			printk(KERN_ERR "Error %d directing MB %d IRQ to "
				"core %d\n", rc, qconfig->mbox, core); 
		}
#endif	
	} else {
		printk(KERN_ERR "Error: request not supported\n");
	}
	
exit_update_qconfig:
	return rc;
}

static int apm_enet_delete_qconfig(unsigned long priv_data,
				struct qm_cfg_qconfig *qconfig)
{
	struct apm_enet_dev_base *priv_dev;
	struct eth_queue_ids *queue_ids;
	unsigned int port;
	unsigned int core;
	int i;
	int q = 0;
	int rc = 0;

	priv_dev = (struct apm_enet_dev_base *)priv_data;
	port = priv_dev->port_id;
	core = qconfig->ppc;
	queue_ids = &priv_dev->qm_queues[core];

	if (qconfig->parent > 0 && qconfig->parent < 256) {
		printk(KERN_ERR "Error: PQ is a child Queue of VQ\n");
		return -EINVAL;
	}

	if (qconfig->dir == QM_CFG_DIR_EGRESS &&
	    (qconfig->qtype == QM_CFG_QTYPE_PQ ||
	     qconfig->qtype == QM_CFG_QTYPE_VQ)) {

		for (i = 0; i < queue_ids->tx_count; i++) {
			if (queue_ids->tx[i].qid == qconfig->qid)
				q = i;
		}

		if (qconfig->qtype == QM_CFG_QTYPE_VQ) {
#if 0
			/* FIXME */
			apm_enet_flush_queue(priv_dev, queue_ids->comp_mb,
					     queue_ids->comp_qid);
#endif
			for (i = 0; i < qconfig->qcount; i++)
				qm_cfg_delete_qconfig(qconfig->qsel_qid[i]);
		}

		qm_cfg_delete_qconfig(qconfig->qid);
		memset(&queue_ids->tx[q], 0, sizeof(struct eth_qids));
		queue_ids->tx_count--;

	} else if (qconfig->dir == QM_CFG_DIR_INGRESS &&
		   (qconfig->qtype == QM_CFG_QTYPE_PQ ||
		    qconfig->qtype == QM_CFG_QTYPE_VQ)) {

		for (i = 0; i < queue_ids->rx_count; i++) {
			if (queue_ids->rx[i].qid == qconfig->qid)
				q = i;
		}

		if (qconfig->qtype == QM_CFG_QTYPE_VQ) {
			for (i = 0; i < qconfig->qcount; i++) {
#if 0
				/* FIXME */
				apm_enet_flush_queue(priv_dev, qconfig->mbox,	
						     qconfig->qsel_qid[i]);
#endif
				qm_cfg_delete_qconfig(qconfig->qsel_qid[i]);
			}
		}

		qm_cfg_delete_qconfig(qconfig->qid);
		memset(&queue_ids->rx[q], 0, sizeof(struct eth_qids));
		queue_ids->rx_count--;

	} else {
		printk(KERN_ERR "Error: request not supported\n");
		return -EINVAL;
	}

	return rc;
}

int apm_enet_set_qgroup(unsigned long priv_data, struct qm_cfg_qgroup *qgroup)
{
	struct apm_enet_dev_base *priv_dev;
	struct eth_queue_ids *queue_ids;
	unsigned int core;
	u16 i;
	u16 tx_idx = QM_CFG_INVALID;
	u16 rx_idx = QM_CFG_INVALID;
	int rc = 0;

	priv_dev = (struct apm_enet_dev_base *)priv_data;
	core = qgroup->ppc;
	queue_ids = &priv_dev->qm_queues[core];

	for (i = 0; i < queue_ids->tx_count; i++) {
		if (queue_ids->tx[i].qid == qgroup->eqid)
			tx_idx = i;
	}

	for (i = 0; i < queue_ids->rx_count; i++) {
		if (queue_ids->rx[i].qid == qgroup->iqid)
			rx_idx = i;
	}

	if (tx_idx != QM_CFG_INVALID && rx_idx != QM_CFG_INVALID) {
		queue_ids->tx_idx = tx_idx;
		queue_ids->rx_idx = rx_idx;
		printk("port %d tx_idx %d rx_idx %d\n", priv_dev->port_id, tx_idx, rx_idx);
	} else {
		printk(KERN_ERR "%s: could not set qgroup\n", __func__);
		rc = -EINVAL;
	}

	printk("port %d tx_qid %d rx_qid %d\n", priv_dev->port_id, queue_ids->tx[tx_idx].qid, queue_ids->rx[rx_idx].qid);
	/* FIXME: do we need to flush queues */
	
	return rc;
}

static struct qm_cfg_dev_notify apm_enet_qm_cfg_notify[] = {
	{
	.dev		= IP_ETH0,
	.priv_data	= 0, /* fill in before registering */
	.add_qconfig	= apm_enet_add_qconfig,
	.delete_qconfig	= apm_enet_delete_qconfig,
	.set_qgroup	= apm_enet_set_qgroup,
	},
	{
	.dev		= IP_ETH1,
	.priv_data	= 0, /* fill in before registering */
	.add_qconfig	= apm_enet_add_qconfig,
	.delete_qconfig	= apm_enet_delete_qconfig,
	.set_qgroup	= apm_enet_set_qgroup,
	},
};

int apm_enet_qm_cfg_register(unsigned long priv_data, int port)
{
	apm_enet_qm_cfg_notify[port].priv_data = priv_data;
	qm_cfg_dev_notify_register(&apm_enet_qm_cfg_notify[port]);
	return 0;
}

