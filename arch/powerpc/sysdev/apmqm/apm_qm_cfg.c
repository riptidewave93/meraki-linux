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

#include <asm/apm_qm_csr.h>
#include <asm/apm_qm_access.h>
#include <asm/apm_qm_utils.h>
#include <asm/apm_qm_core.h>
#include <asm/apm_qml_csr.h>
#include <asm/apm_qm_cfg.h>
#include <asm/ipp.h>

static int qm_cfg_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int qm_cfg_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t qm_cfg_read(struct file *file, char __user * buf,
				size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t qm_cfg_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	return count;
}

int qm_cfg_get_qconfig(struct qm_cfg_qconfig * qconfig)
{
	struct apm_qm_qstate qstate;

	if (apm_qm_qstate_rd_cfg(qconfig->qid, &qstate) < 0)
		return -EINVAL;

	qconfig->valid = qstate.valid;

	if (qconfig->valid == 0) {
		return 0;
	}

	qconfig->qtype   = qstate.q_type;
	qconfig->pbn     = qstate.pbn;
	qconfig->ip      = qstate.ip_blk;
	qconfig->dev     = qstate.src_ip_blk;
	qconfig->slave   = qstate.slave_id;
	qconfig->mbox    = qstate.mb_id;
	qconfig->ppc     = qstate.ppc_id;
	qconfig->dir     = qstate.direction;
	qconfig->qsize   = qstate.q_size;
	qconfig->thr     = qstate.thr_set;
	qconfig->parent  = qstate.parent_vqid;
	qconfig->qcount  = 0;

	if (qstate.q_type == QM_CFG_QTYPE_VQ) {
		int i;
		for (i = 0; i < QM_CFG_MAX_QSEL; i++) {
			if (qstate.pq_sel[i] == qconfig->qid) {
				qconfig->qsel_qid[i] = qstate.pq_sel[i];
			} else {
				qconfig->qsel_qid[i] = qstate.pq_sel[i];
				qconfig->qsel_arb[i] = qstate.q_sel_arb[i];
				qconfig->qsel_cfg[i] = qstate.shape_rate[i];
				qconfig->qcount++;
			}
		}
	}

	return 0;
}

int qm_cfg_add_qconfig(struct qm_cfg_qconfig * qconfig)
{
        struct apm_qm_qstate  qstate;
        struct apm_qm_qalloc qalloc;

	if (qconfig->qid != QM_CFG_INVALID) {
		printk(KERN_ERR "%s: 'qid' should not be set", __func__);
		return -1;
	}

	memset(&qalloc, 0, sizeof(struct apm_qm_qalloc));

	qalloc.qstates   = &qstate;
	qalloc.qm_ip_blk = qconfig->ip;
	qalloc.ip_blk    = qconfig->dev;
	qalloc.ppc_id    = qconfig->ppc;
	qalloc.q_type    = qconfig->qtype;
	qalloc.direction = qconfig->dir;
	qalloc.qsize     = qconfig->qsize;
	qalloc.thr_set   = qconfig->thr;
	qalloc.q_count   = 1;

	if (qconfig->qtype == QM_CFG_QTYPE_VQ) {
		int i;

	   	if (qconfig->qcount == 0 ||
		    qconfig->qcount > QM_CFG_MAX_QSEL) {
			printk(KERN_ERR "%s: invalid 'qcount'",
				__func__);
			return -1;
		}

		for (i=0; i < qconfig->qcount; i++) {
			if (qconfig->qsel_qid[i] != QM_CFG_INVALID) {
				printk(KERN_ERR
					"%s: 'qsel_qid' should not be set",
					__func__);
				return -1;
			}

			if (qconfig->qsel_arb[i] == QM_CFG_INVALID) {
				printk(KERN_ERR
					"%s: 'qsel_arb' needs to be set",
					__func__);
				return -1;
			}

			if ((qconfig->qsel_arb[i] == QM_CFG_ARB_DRR ||
			     qconfig->qsel_arb[i] == QM_CFG_ARB_AVB) &&
			     qconfig->qsel_cfg[i] == QM_CFG_INVALID) {
				printk(KERN_ERR
					"%s: 'qsel_cfg' needs to be set",
					__func__);
				return -1;
			}
		}

		qconfig->qid = apm_qm_get_vq(qconfig->dev);

		/* create children PQs */

		qalloc.q_type    = QM_CFG_QTYPE_PQ;
		qalloc.parent_vq = qconfig->qid;
		qalloc.vqen      = 1;

		for (i=0; i < qconfig->qcount; i++) {
			memset(&qstate, 0, sizeof(struct apm_qm_qstate));
			if (apm_qm_alloc_q(&qalloc) != 0) {
				printk(KERN_ERR
					"%s: could not allocate child PQ",
					__func__);
				return -1;
			}

			qconfig->qsel_qid[i] = qstate.q_id;
		}

		/* create parent VQ */

		qalloc.q_type    = QM_CFG_QTYPE_VQ;
		qalloc.parent_vq = 0;
		qalloc.vqen      = 0;

		for (i=0; i < QM_CFG_MAX_QSEL; i++) {
			if (i < qconfig->qcount) {
				qalloc.pq_sel[i] = qconfig->qsel_qid[i];
				qalloc.q_sel_arb[i] = qconfig->qsel_arb[i];
				qalloc.shape_rate[i] = qconfig->qsel_cfg[i];
			} else {
				qalloc.pq_sel[i] = qconfig->qid;
			}
		}

		memset(&qstate, 0, sizeof(struct apm_qm_qstate));
		if (apm_qm_alloc_vq(&qalloc, qconfig->qid) != 0) {
			printk(KERN_ERR "%s: could not allocate VQ", __func__);
			return -1;
		}

		qconfig->qid  = qstate.q_id;
		qconfig->mbox = qstate.mb_id;
		qconfig->pbn  = qstate.pbn;
		qconfig->slave = qstate.slave_id;
	} else if (qconfig->qtype == QM_CFG_QTYPE_PQ ||
		   qconfig->qtype == QM_CFG_QTYPE_FP) {
		memset(&qstate, 0, sizeof(struct apm_qm_qstate));
		if (apm_qm_alloc_q(&qalloc) != 0) {
			printk(KERN_ERR "%s: could not allocate PQ", __func__);
			return -1;
		}

		qconfig->qid   = qstate.q_id;
		qconfig->mbox  = qstate.mb_id;
		qconfig->pbn   = qstate.pbn;
		qconfig->slave = qstate.slave_id;
	} else {
		printk(KERN_INFO "%s: invalid 'qtype'\n", __func__);
		return -1;
	}

	return 0;
}

int qm_cfg_delete_qconfig(__u16 qid)
{
	int rc = 0;
	printk(KERN_INFO "%s\n", __func__);
	apm_qm_shutdown_q(qid);
	return rc;
}

static LIST_HEAD(qm_cfg_dev_notify_list);

struct qm_cfg_dev_notify_entry {
	struct list_head node;
	struct qm_cfg_dev_notify *notify;
};

int qm_cfg_dev_notify_register(struct qm_cfg_dev_notify *notify)
{
	struct qm_cfg_dev_notify_entry *entry;

	if (notify->priv_data == 0)
        	return -EINVAL;

	if (notify->add_qconfig == NULL)
        	return -EINVAL;

	if (notify->delete_qconfig == NULL)
        	return -EINVAL;

	entry = kmalloc(sizeof(struct qm_cfg_dev_notify_entry), GFP_KERNEL);
	if (entry == NULL)
		return -ENOMEM;

	entry->notify = notify;

	list_add(&entry->node, &qm_cfg_dev_notify_list);

	return 0;
}


int qm_cfg_dev_notify_unregister(struct qm_cfg_dev_notify *notify)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == notify->dev &&
		    entry->notify->priv_data == notify->priv_data) {
                        list_del(&entry->node);
			kfree(entry);
                        return 0;
                }

        return -EINVAL;
}

int qm_cfg_dev_add_qconfig(struct qm_cfg_qconfig * qconfig)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == qconfig->dev &&
		    entry->notify->add_qconfig != NULL) {
			return entry->notify->add_qconfig(
						entry->notify->priv_data,
						qconfig);
		}

	return -EINVAL;
}

int qm_cfg_dev_delete_qconfig(struct qm_cfg_qconfig * qconfig)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == qconfig->dev &&
		    entry->notify->delete_qconfig != NULL) {
			return entry->notify->delete_qconfig(
						entry->notify->priv_data,
						qconfig);
		}

	return -EINVAL;
}

int qm_cfg_dev_stop(u16 dev)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == dev &&
		    entry->notify->stop != NULL) {
			return entry->notify->stop(
					entry->notify->priv_data);
		}

	return -EINVAL;
}

int qm_cfg_dev_start(u16 dev)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == dev &&
		    entry->notify->start != NULL) {
			return entry->notify->start(
					entry->notify->priv_data);
		}

	return -EINVAL;
}

int qm_cfg_dev_set_qgroup(struct qm_cfg_qgroup * qgroup)
{
	struct qm_cfg_dev_notify_entry *entry;

        list_for_each_entry(entry, &qm_cfg_dev_notify_list, node)
                if (entry->notify->dev == qgroup->dev &&
		    entry->notify->set_qgroup != NULL) {
			return entry->notify->set_qgroup(
					entry->notify->priv_data,
					qgroup);
		}

	return -EINVAL;
}

static LIST_HEAD(qm_cfg_qgroup_list);

struct qm_cfg_qgroup_entry {
	struct list_head node;
	struct qm_cfg_qgroup qgroup;
};

int qm_cfg_get_qgroup(struct qm_cfg_qgroup * qgroup)
{
	struct qm_cfg_qgroup_entry *entry;

        list_for_each_entry(entry, &qm_cfg_qgroup_list, node) {
                if (strcmp(entry->qgroup.name, qgroup->name) == 0 &&
		    entry->qgroup.dev  == qgroup->dev &&
		    entry->qgroup.ppc  == qgroup->ppc) {
			memcpy(qgroup, &entry->qgroup,
					sizeof(struct qm_cfg_qgroup));
                        return 0;
                }
	}

        return -EINVAL;
}

int qm_cfg_set_qgroup(struct qm_cfg_qgroup * qgroup)
{
	struct qm_cfg_qgroup_entry *entry;

        list_for_each_entry(entry, &qm_cfg_qgroup_list, node)
                if (strcmp(entry->qgroup.name, qgroup->name) == 0 &&
		    entry->qgroup.dev  == qgroup->dev &&
		    entry->qgroup.ppc  == qgroup->ppc) {
			return qm_cfg_dev_set_qgroup(&entry->qgroup);
                }

        return -EINVAL;
}

int qm_cfg_add_qgroup(struct qm_cfg_qgroup * qgroup)
{
	struct qm_cfg_qgroup_entry *entry;

	if (qgroup->name[0] == 0)
        	return -EINVAL;

	entry = kmalloc(sizeof(struct qm_cfg_qgroup_entry), GFP_KERNEL);
	if (entry == NULL)
		return -ENOMEM;

	memcpy(&entry->qgroup, qgroup, sizeof(struct qm_cfg_qgroup));

	list_add(&entry->node, &qm_cfg_qgroup_list);

	return 0;
}

int qm_cfg_delete_qgroup(struct qm_cfg_qgroup * qgroup)
{
	struct qm_cfg_qgroup_entry *entry;

        list_for_each_entry(entry, &qm_cfg_qgroup_list, node)
                if (strcmp(entry->qgroup.name, qgroup->name) == 0 &&
		    entry->qgroup.dev  == qgroup->dev &&
		    entry->qgroup.ppc  == qgroup->ppc) {
                        list_del(&entry->node);
			kfree(entry);
                        return 0;
                }

        return -EINVAL;
}

static long qm_cfg_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg)
{
	struct qm_cfg_qconfig qconfig;
	struct qm_cfg_qgroup  qgroup;
	struct qm_cfg_stat_cfg stat_cfg;
	struct qm_cfg_stat stat;
	u16 dev;
	u32 data;
	int rc = 0;

	switch(cmd) {
	case QM_CFG_GET_VERSION:
		data = QM_CFG_VERSION;
		copy_to_user((u32*)arg, &data, sizeof(u32));
		break;
	case QM_CFG_GET_MAX_QUEUES:
		data = MB_MAX_QUEUES;
		copy_to_user((u32*)arg, &data, sizeof(u32));
		break;
	case QM_CFG_GET_TM_SUPPORT:
		data = (is_apm86xxx_lite())? 0 : 1;
		copy_to_user((u32*)arg, &data, sizeof(u32));
		break;
	case QM_CFG_GET_QCONFIG:
		rc = copy_from_user(&qconfig, (struct qm_cfg_qconfig*)arg,
				sizeof(struct qm_cfg_qconfig));
		if (rc == 0)
			rc = qm_cfg_get_qconfig(&qconfig);

		if (rc == 0)
			copy_to_user((struct qm_cfg_qconfig*)arg, &qconfig,
				sizeof(struct qm_cfg_qconfig));
		break;
	case QM_CFG_ADD_QCONFIG:
		rc = copy_from_user(&qconfig, (struct qm_cfg_qconfig*)arg,
				sizeof(struct qm_cfg_qconfig));
		if (rc == 0)
			rc = qm_cfg_dev_add_qconfig(&qconfig);

		if (rc == 0)
			copy_to_user((struct qm_cfg_qconfig*)arg, &qconfig,
				sizeof(struct qm_cfg_qconfig));
		break;
	case QM_CFG_DELETE_QCONFIG:
		rc = copy_from_user(&qconfig, (struct qm_cfg_qconfig*)arg,
				sizeof(struct qm_cfg_qconfig));
		if (rc == 0)
		 	rc = qm_cfg_get_qconfig(&qconfig);

		if (rc == 0)
			rc = qm_cfg_dev_delete_qconfig(&qconfig);
		break;
	case QM_CFG_STOP_DEV:
		rc = copy_from_user(&dev, (u16*)arg, sizeof(u16));
		if (rc == 0)
			rc = qm_cfg_dev_stop(dev);
		break;
	case QM_CFG_START_DEV:
		rc = copy_from_user(&dev, (u16*)arg, sizeof(u16));
		if (rc == 0)
			rc = qm_cfg_dev_start(dev);
		break;
	case QM_CFG_GET_QGROUP:
		rc = copy_from_user(&qgroup, (struct qm_cfg_qgroup*)arg,
				sizeof(struct qm_cfg_qgroup));
		if (rc == 0)
			rc = qm_cfg_get_qgroup(&qgroup);

		if (rc == 0)
			copy_to_user((struct qm_cfg_qgroup*)arg, &qgroup,
				sizeof(struct qm_cfg_qgroup));
		break;
	case QM_CFG_SET_QGROUP:
		rc = copy_from_user(&qgroup, (struct qm_cfg_qgroup*)arg,
				sizeof(struct qm_cfg_qgroup));
		if (rc == 0)
			rc = qm_cfg_set_qgroup(&qgroup);
		break;
	case QM_CFG_ADD_QGROUP:
		rc = copy_from_user(&qgroup, (struct qm_cfg_qgroup*)arg,
				sizeof(struct qm_cfg_qgroup));
		if (rc == 0)
			rc = qm_cfg_add_qgroup(&qgroup);
		break;
	case QM_CFG_DELETE_QGROUP:
		rc = copy_from_user(&qgroup, (struct qm_cfg_qgroup*)arg,
				sizeof(struct qm_cfg_qgroup));
		if (rc == 0)
			rc = qm_cfg_delete_qgroup(&qgroup);
		break;
	case QM_CFG_GET_STAT_CFG:
		apm_qm_rd32(IP_BLK_QM, CSR_QM_STATS_CFG_ADDR, &data);
		copy_to_user((struct qm_cfg_stat_cfg*)arg, &data,
				sizeof(struct qm_cfg_stat_cfg));
		break;
	case QM_CFG_SET_STAT_CFG:
		rc = copy_from_user(&stat_cfg, (struct qm_cfg_stat_cfg*)arg,
				sizeof(struct qm_cfg_stat_cfg));
		if (rc == 0) {
			data = (stat_cfg.enq_qid << 24) | (stat_cfg.deq_qid << 16);	
			data &= 0xFFFF0000;
			apm_qm_wr32(IP_BLK_QM, CSR_QM_STATS_CFG_ADDR, data);
		}
		break;
	case QM_CFG_GET_STAT:
		apm_qm_rd32(IP_BLK_QM, CSR_ENQ_STATISTICS_ADDR, &stat.enq_count);
		apm_qm_rd32(IP_BLK_QM, CSR_DEQ_STATISTICS_ADDR, &stat.deq_count);
		copy_to_user((struct qm_cfg_stat*)arg, &stat,
				sizeof(struct qm_cfg_stat));
		break;
	default:
		rc = -EINVAL;
	}

	return rc;
}

struct file_operations qm_cfg_fops = {
	.owner   = THIS_MODULE,
	.open    = qm_cfg_open,
	.release = qm_cfg_release,
	.read    = qm_cfg_read,
	.write   = qm_cfg_write,
	.compat_ioctl   = qm_cfg_ioctl,
};

static int __init qm_cfg_init(void)
{
	struct proc_dir_entry *entry;
        
	printk(KERN_INFO "%s: %d\n", QM_CFG_NAME, QM_CFG_VERSION);
	
	entry = create_proc_entry(QM_CFG_NAME, 0, NULL);
	if (entry == NULL) {
		printk(KERN_ERR "%s: failed to initialize\n", QM_CFG_NAME);
		return -1;
	}

	entry->proc_fops = &qm_cfg_fops;
	return 0;
}

void __exit qm_cfg_exit(void)
{
	remove_proc_entry(QM_CFG_NAME, NULL);
	return;
}

module_init(qm_cfg_init);
module_exit(qm_cfg_exit);

MODULE_VERSION(QM_CFG_VERSION);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Gallardo<vgallardo@amcc.com>");
MODULE_DESCRIPTION("Queue Manager configuration interface");

