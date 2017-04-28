/*
 * APM 86xxx QM Utility 
 *
 * Copyright (c) 2011, Applied Micro Circuits Corporation
 * Author: Loc Ho <lho@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */
#include <asm/apm_qm_core.h>
#include <asm/apm_qm_utils.h>
#include <asm/apm_qm_access.h>

char *mb_qtype_str[] = {
			"Queue disabled",
			"Physical queue",
			"Free pool queue",
			"Virtual queue"
};

void dump_qstate(struct apm_qm_qstate *qstate)
{
	QM_PRINT("\nQUEUE STATE DUMP\n");

	if (qstate->ip_blk == IP_BLK_QM)
		QM_PRINT("\nThis queue belongs to: QM\n");
	else if (qstate->ip_blk == IP_BLK_QML)
		QM_PRINT("\nThis queue belongs to: QM lite\n");
	else
		QM_PRINT("\nThis queue belongs to: Invalid IP blk\n");

	if (qstate->valid)
		QM_PRINT("This queue is: Valid\n");
	else
		QM_PRINT("This queue is: Invalid\n");

	QM_PRINT("Queue ID: %d\n", qstate->q_id);
	QM_PRINT("Assigned mailbox ID: %d\n", qstate->mb_id);
	QM_PRINT("Belongs to slave ID: %d\n", qstate->slave_id);
	QM_PRINT("Belongs to Prefetch Buffer: 0x%x\n", qstate->pbn);

	switch(qstate->q_type) {
	case(P_QUEUE):
		QM_PRINT("This queue is: Physical queue\n");
		break;
	case(FREE_POOL):
		QM_PRINT("This queue is: Free queue\n");
		break;
	case(V_QUEUE):
		QM_PRINT("This queue is: Virtual queue\n");
		break;
	default:
		QM_PRINT("This queue is: Invalid queue\n");
		break;
	}

	switch(qstate->q_size) {
	case(SIZE_512B):
		QM_PRINT("Queue size: 512 Bytes\n");
		break;
	case(SIZE_2KB):
		QM_PRINT("Queue size: 2 KB\n");
		break;
	case(SIZE_16KB):
		QM_PRINT("Queue size: 16 KB\n");
		break;
	case(SIZE_64KB):
		QM_PRINT("Queue size: 64 KB\n");
		break;
	case(SIZE_512KB):
		QM_PRINT("Queue size: 512 KB\n");
		break;
	default:
		QM_PRINT("Queue size: Invalid\n");
		break;
	}

	QM_PRINT("Queue Physical start address: 0x%x\n", qstate->q_start_addr << 8);
	QM_PRINT("Queue Virtual start address: 0x%x\n", (u32) phys_to_virt(qstate->q_start_addr << 8));

	QM_PRINT("Number of message in queue: %d\n", qstate->nummsgs);
	QM_PRINT("Current region ID of the queue: %d\n", qstate->rid);

	if (qstate->direction == DIR_EGRESS)
		QM_PRINT("This queue is used in: Egress direction\n");
	else if (qstate->direction == DIR_INGRESS)
		QM_PRINT("This queue is used in: Ingress direction\n");
	else
		QM_PRINT("This queue is used in: Invalid direction\n");

	QM_PRINT("Config SAB Enable for this queue: %d\n", qstate->cfgsaben);
	QM_PRINT("Config PPC Notify for this queue: %d\n", qstate->ppc_notify);

	return;
}

int apm_qm_rx_msg_util(int qid, int mbid)
{
	struct apm_qm_msg_desc msg_desc;
	struct apm_qm_msg32 msg;
	u8 *temp_msg = NULL;

	msg_desc.msg = &msg;

	temp_msg = (u8 *) msg_desc.msg;

	memset(&msg, 0, sizeof(msg));

	msg_desc.qid = qid;
	msg_desc.mb_id = mbid;
	msg_desc.is_msg16 = 0;	

	apm_qm_pull_msg(&msg_desc);

	QM_PRINT("READ MSG: %x\n", *(u32 *)temp_msg);
	temp_msg+=4;
	QM_PRINT("READ MSG: %x\n", *(u32 *)temp_msg);
	temp_msg+=4;
	QM_PRINT("READ MSG: %x\n", *(u32 *)temp_msg);
	temp_msg+=4;
	QM_PRINT("READ MSG: %x\n", *(u32 *)temp_msg);

	return 0;
}

int apm_qm_send_msg_util(int qid, int mbid)
{
	struct apm_qm_msg_desc msg_desc;
	struct apm_qm_msg32 msg;
	struct apm_qm_msg16 *msg16 = &(msg.msg16);

	memset(&msg, 0, sizeof(msg));

	/* prepare dummy message to send */
	msg16->srcDomID = 1;
	msg16->BufDataLen = 3;
	msg16->DataAddrLSB = 0x0d00ffff;

	msg_desc.qid = qid;
	msg_desc.mb_id = mbid;
	msg_desc.msg = &msg;

	apm_qm_push_msg(&msg_desc);

	return 0;
}

int apm_qm_dealloc_buf_util(int qid, int mbid)
{
	struct apm_qm_msg_desc msg_desc;
	struct apm_qm_msg16 msg;

	memset(&msg, 0, sizeof(msg));

	/* prepare dummy message to send */
	msg.srcDomID = 1;
	msg.BufDataLen = 3;
	msg.DataAddrLSB = 0x0d00ffff;
	msg.FPQNum = qid;

	msg_desc.qid = qid;
	msg_desc.mb_id = mbid;
	msg_desc.msg = &msg;

	apm_qm_fp_dealloc_buf(&msg_desc);

	return 0;
}

int apm_qm_alloc_buf_util(int qid, int mbid)
{
	struct apm_qm_msg_desc msg_desc;
	struct apm_qm_msg16 msg;

	msg_desc.msg = &msg;

	memset(&msg, 0, sizeof(msg));

	/* prepare dummy message to send */
	msg.srcDomID = 1;
	msg.BufDataLen = 3;
	msg.DataAddrLSB = 0x0d00ffff;
	msg.FPQNum = qid;

	msg_desc.qid = qid;
	msg_desc.mb_id = mbid;
	msg_desc.msg = &msg;

	apm_qm_fp_alloc_buf(&msg_desc);

	QM_PRINT("msg.DataAddrLSB: %x\n", msg.DataAddrLSB);
	QM_PRINT("msg.FPQNum: %x\n", msg.FPQNum);

	return 0;
}

