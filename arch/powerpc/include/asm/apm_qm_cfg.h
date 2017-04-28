/*
 * APM86xxx QM Configuration Interface
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

#ifndef __APM_QM_CFG_H__
#define __APM_QM_CFG_H__

#include <linux/types.h>
#include <linux/errno.h>

#define QM_CFG_VERSION		1
#define QM_CFG_NAME		"qm_cfg"
#define QM_CFG_PROCNAME		"/proc/qm_cfg"
#define QM_CFG_MAX_QSEL		8
#define QM_CFG_INVALID		0xFFFF
#define QM_CFG_NAME_LEN		24

enum qm_cfg_req {
	QM_CFG_REQ_ADD,
	QM_CFG_REQ_DELETE,
	QM_CFG_REQ_STOP,
	QM_CFG_REQ_START,
	QM_CFG_REQ_GETGROUP,
	QM_CFG_REQ_SETGROUP,
	QM_CFG_REQ_ADDGROUP,
	QM_CFG_REQ_DELGROUP,
};

enum qm_cfg_slave_id {
	QM_CFG_SLAVE_ID_DMA,
	QM_CFG_SLAVE_ID_OCMM,
	QM_CFG_SLAVE_ID_SEC,
	QM_CFG_SLAVE_ID_CLASS,
	QM_CFG_SLAVE_ID_IPP,
	QM_CFG_SLAVE_ID_ETH,
	QM_CFG_SLAVE_ID_RES6,
	QM_CFG_SLAVE_ID_RES7,
	QM_CFG_SLAVE_ID_RES8,
	QM_CFG_SLAVE_ID_RES9,
	QM_CFG_SLAVE_ID_RESA,
	QM_CFG_SLAVE_ID_RESB,
	QM_CFG_SLAVE_ID_RESC,
	QM_CFG_SLAVE_ID_RESD,
	QM_CFG_SLAVE_ID_RESE,
	QM_CFG_SLAVE_ID_PPC,
};

enum qm_cfg_ip {
	QM_CFG_IP_UNKNOWN,
	QM_CFG_IP_QM,
	QM_CFG_IP_QML,
	QM_CFG_IP_IPP_QML,
};

enum qm_cfg_dev_ip {
	QM_CFG_DEV_IP_PPC0,
	QM_CFG_DEV_IP_PPC1,
	QM_CFG_DEV_IP_ETH0,
	QM_CFG_DEV_IP_ETH1,
	QM_CFG_DEV_IP_IPP,
	QM_CFG_DEV_IP_DMA,
	QM_CFG_DEV_IP_SEC,
	QM_CFG_DEV_IP_OCMM,
	QM_CFG_DEV_IP_CLASS,
};

enum qm_cfg_qtype {
	QM_CFG_QTYPE_QD,
	QM_CFG_QTYPE_PQ,
	QM_CFG_QTYPE_FP,
	QM_CFG_QTYPE_VQ,
};

enum qm_cfg_dir {
	QM_CFG_DIR_INGRESS,
	QM_CFG_DIR_EGRESS,
};

enum qm_cfg_qsize {
	QM_CFG_QSIZE_512B,
	QM_CFG_QSIZE_2KB,
	QM_CFG_QSIZE_16KB,
	QM_CFG_QSIZE_64KB,
	QM_CFG_QSIZE_512KB,
};

enum qm_cfg_arb {
        QM_CFG_ARB_NO,
        QM_CFG_ARB_SP,
        QM_CFG_ARB_DRR,
        QM_CFG_ARB_AVB,
};

struct qm_cfg_qconfig {
	__u16 request;		/**< request: UPDATE, STOP, START */
	__u16 valid;		/**< 1 if configured else 0 */
	__u16 qid;		/**< queue id */
	__u16 qtype;		/**< 0 = Q is disabled, PQ=1, FP=2, VQ=3 */
	__u16 pbn;		/**< prefetch buffer number */
	__u16 ip;		/**< QM or QM lite */
	__u16 dev;		/**< Device IP block: ETH0, ETH1 */
	__u16 slave;		/**< prefetch mgr slave id */
	__u16 mbox;		/**< mailbox id, used for ingress */
	__u16 ppc;		/**< PPC core */
	__u16 qsize;		/**< Size of queue */
	__u16 dir;		/**< 0: Ingress, 1: Egress */
	__u16 thr;		/**< Queue threshold */
	__u16 parent;		/**< parent qid, if this PQ belongs to VQ */
	__u16 qcount;		/**< Number of queues to allocate */
	__u16 qsel_qid[QM_CFG_MAX_QSEL];  /**< PQ id mapped to VQ */ 
	__u16 qsel_arb[QM_CFG_MAX_QSEL];  /**< arbitration type for PQ */
	__u16 qsel_cfg[QM_CFG_MAX_QSEL];  /**< addition arb config */
};

struct qm_cfg_qgroup {
	char  name[QM_CFG_NAME_LEN];	/**< group name */
	__u16 dev;		/**< Device IP block: ETH0, ETH1 */
	__u16 ppc;		/**< PPC core */
	__u16 eqid;		/**< egress queue id (VQ or PQ) */
	__u16 iqid;		/**< ingress queue id (VQ or PQ) */
	__u16 fqid;		/**< freepool queue id */
	__u16 hqid;		/**< hw freepool queue id */
	__u16 cqid;		/**< completion queue id */
};

struct qm_cfg_stat_cfg {
	__u8 enq_qid;
	__u8 deq_qid;
	__u8 res1;
	__u8 res2;
};
struct qm_cfg_stat {
	__u32 enq_count;	/**< number of enqueued messages */
	__u32 deq_count;	/**< number of dequeued messages */
};

#define QM_CFG_GET_VERSION	_IOR ('Q', 0, __u32)
#define QM_CFG_GET_MAX_QUEUES	_IOR ('Q', 1, __u32)
#define QM_CFG_GET_TM_SUPPORT	_IOR ('Q', 2, __u32)
#define QM_CFG_GET_QCONFIG	_IOWR('Q', 3, struct qm_cfg_qconfig)
#define QM_CFG_ADD_QCONFIG	_IOWR('Q', 4, struct qm_cfg_qconfig)
#define QM_CFG_DELETE_QCONFIG	_IOWR('Q', 5, struct qm_cfg_qconfig)

#define QM_CFG_STOP_DEV		_IOWR('Q', 6, __u16)
#define QM_CFG_START_DEV	_IOWR('Q', 7, __u16)

#define QM_CFG_GET_QGROUP	_IOWR('Q', 8, struct qm_cfg_qgroup)
#define QM_CFG_SET_QGROUP	_IOWR('Q', 9, struct qm_cfg_qgroup)
#define QM_CFG_ADD_QGROUP	_IOWR('Q',10, struct qm_cfg_qgroup)
#define QM_CFG_DELETE_QGROUP	_IOWR('Q',11, struct qm_cfg_qgroup)

#define QM_CFG_GET_STAT_CFG	_IOWR('Q',12, struct qm_cfg_stat_cfg)
#define QM_CFG_SET_STAT_CFG	_IOWR('Q',13, struct qm_cfg_stat_cfg)
#define QM_CFG_GET_STAT		_IOWR('Q',14, struct qm_cfg_stat)

#ifdef __KERNEL__

int qm_cfg_get_qconfig(struct qm_cfg_qconfig * qconfig);
int qm_cfg_add_qconfig(struct qm_cfg_qconfig * qconfig);
int qm_cfg_delete_qconfig(__u16 qid);

int qm_cfg_get_qgroup(struct qm_cfg_qgroup * qgroup);
int qm_cfg_set_qgroup(struct qm_cfg_qgroup * qgroup);
int qm_cfg_add_qgroup(struct qm_cfg_qgroup * qgroup);
int qm_cfg_delete_qgroup(struct qm_cfg_qgroup * qgroup);

struct qm_cfg_dev_notify {
	__u16 dev;	/**< Device IP block: ETH0, ETH1 */
	unsigned long priv_data;
	int (*stop)(unsigned long priv_data);
	int (*start)(unsigned long priv_data);
	int (*add_qconfig)(unsigned long priv_data,
			   struct qm_cfg_qconfig *qconfig);
	int (*delete_qconfig)(unsigned long priv_data,
			      struct qm_cfg_qconfig *qconfig);
	int (*set_qgroup)(unsigned long priv_data,
			  struct qm_cfg_qgroup *qgroup);
};

int qm_cfg_dev_notify_register(struct qm_cfg_dev_notify *notify);
int qm_cfg_dev_notify_unregister(struct qm_cfg_dev_notify *notify);

#endif /* __KERNEL__ */

#endif /* __APM_QM_CFG_H__ */
