/**
 * AppliedMicro AM862xx QM Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Keyur Chudgar <kchudgar@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * @file apm_qm_access.c
 **
 */

#include <asm/apm_qm_csr.h>
#include <asm/apm_qm_access.h>
#include <asm/apm_qm_utils.h>
#include <asm/apm_qm_core.h>
#include <asm/apm_qml_csr.h>
#include <asm/ipp.h>
#include <asm/apm86xxx_pm.h>
#include <asm/cacheflush.h>

#define APM_QM_DRIVER_VERSION  	"0.1"
#define APM_QM_DRIVER_NAME     	"apm_qm_tm"
#define APM_QM_DRIVER_STRING  	"APM QM-TM Driver"
#define PFX			"APM_QM: "

#define RES_SIZE(r)	((r)->end - (r)->start + 1)

#ifdef QM_ALTERNATE_ENQUEUE
u32 queue_baddr = 0;
u32 queue_baddr_p = 0;
u32 queue_eaddr_p = 0;
u32 qmem_size = 0;
#endif

u64 qm_csr_paddr;
u32 qm_csr_addr;
u64 qm_fabric_paddr;
u64 qm_mailbox_paddr;
u32 qm_fabric_addr;
u32 qm_ocm_enq_mbox_addr;
u32 qm_ocm_dq_mbox_addr;
u32 qm_ocm_fp_mbox_addr;
u32 max_qid;
u32 start_qid;
u32 maxmboxes;
u32 start_mboxes;
u32 is_smp; 
int is_noqml;

/* QM lite */
u64 qml_csr_paddr;
u32 qml_csr_addr;
u64 qml_fabric_paddr;
u32 qml_fabric_addr;
u64 qml_ipp_csr_paddr;
u32 qml_ipp_csr_addr;

extern u32 mb_to_q[MB_MAX_QUEUES];
extern apm_qm_msg_fn_ptr apm_qm_cb_fn_table[APM_QM_MAX_RTYPE];
extern void mpic_mask_irq(unsigned int irq);
extern void mpic_unmask_irq(unsigned int irq);
extern struct apm_qm_qstate queue_states[];
extern struct apm_qm_raw_qstate q_raw_states[];
extern u32 pbn_valid[MB_MAX_QUEUES];
extern int apm_qm_alloc_mbox_ocm(void); 
extern int apm_qm_pb_overwrite(int ip, int slv_id, int pbn, int qnum, u8 is_fp);
extern int apm_qm_pb_get(int ip, int slv_id, int pbn);
extern int apm_enet_rx_irq(struct apm_qm_msg_desc *rx_msg_desc);
extern int apm_enet_fp_pb_flush(int pbn);
u8 err_qid = 1;	/* Updated in apm_qm_init_errq */

#define MAX_MAILBOXS	16
u32 irq_mb_map[MAX_MAILBOXS];
u32 mb_to_irq_map[MAX_MAILBOXS];
struct apm_qm_mb_tasklet *apm_qm_mb_tasklet_table[MAX_MAILBOXS];

/* global queue configuration table */
/* :CD: remove this */
#ifdef LINUX_QM_HW_TEST
struct apm_qm_qstate mb_cfg_pqs[1] = {
	{IP_BLK_QM, 1, 9, 1, PB_SLAVE_ID_PPC, 1, P_QUEUE, MSG_32B, 0, 0, NO_NOTIFY, SIZE_16KB, 0, 0, 1, 0, 0, 0, 0, 0},
};
#else
struct apm_qm_qstate mb_cfg_pqs[] = {
	{IP_BLK_QML, 1, DSLEEP_ENET_RX_FP_Q, 0, PB_SLAVE_ID_ETH, DSLEEP_ENET_RX_FP_PBN, FREE_POOL, MSG_16B, 0, 0, NO_NOTIFY, 
		SIZE_512B, 0xd000, 0, 1, 0, 0, 0, 1, 0}, /* Eth ingress FP Queue for QML */
	{IP_BLK_QML, 1, DSLEEP_ENET_RX_Q, 0, PB_SLAVE_ID_IPP, 0x1, P_QUEUE, MSG_32B, 0, 0, NO_NOTIFY, 
		SIZE_512B, 0xd200, 0, 1, 0, 0, 0, 0, 0}, /* Eth to iPP ingress Work Queue*/
	{IP_BLK_QML, 1, DSLEEP_ENET_TX_Q, 0, PB_SLAVE_ID_ETH, 0x10, P_QUEUE, MSG_32B, 0, 0, NO_NOTIFY, 
		SIZE_512B, 0xd400, 0, 1, 0, 0, 0, 1, 0}, /* iPP to Eth0 egress Work Queue*/
	{IP_BLK_QML, 1, DSLEEP_ENET_RX_FQ_TO_DDR, 0, PB_SLAVE_ID_ETH, DSLEEP_ENET_RX_FQ_TO_DDR_PBN, FREE_POOL, MSG_16B, 0, 0, NO_NOTIFY, 
		SIZE_16KB, 0x0, 0, 1, 0, 0, 0, 1, 0}, /* Eth ingress FP Queue for QML */
};
#endif

/* QM raw register read/write routine */
inline int apm_qm_wr32(int ip, u32 offset, u32 data)
{
	u32 csr_base;
        u32 addr;

	if (ip == IP_BLK_QM) {
		csr_base = qm_csr_addr;
	} else if (ip == IP_BLK_QML) {
		if (is_noqml) {
			printk(KERN_ERR 
				"Accessing non-existent QM lite hardware\n");
			return -ENODEV;
		}
		csr_base = qml_csr_addr;
	} else if (ip == IP_BLK_IPP_QML) {
		if (is_noqml) {
			printk(KERN_ERR 
				"Accessing non-existent QM lite hardware\n");
			return -ENODEV;
		}
		csr_base = qml_ipp_csr_addr;
	} else {
                QM_ERR_CHK("Invalid IP block in QM reg write: %d %s %d\n", ip,
                        __FILE__, __LINE__);
		return -1;
	}

        addr = csr_base + offset;
	
#ifdef QM_DEBUG
	if (ip == IP_BLK_IPP_QML) {
		if (offset > 0x240) {
                QM_ERR_CHK("Invalid offset in iPP QML CSR reg write: %x %s %d\n", offset,
                        __FILE__, __LINE__);
                return -1;
		}
	}

        if (offset > CSR_PPC_SAB7_ADDR) {
                QM_ERR_CHK("Invalid offset in QM reg write: %x %s %d\n", offset,
                        __FILE__, __LINE__);
                return -1;
        }
#endif
        QMWRITE_PRINT("Write addr 0x%x data 0x%x\n", addr, data);
        out_be32((void __iomem *) addr, data);

        return 0;
}

inline int apm_qm_rd32(int ip, u32 offset, u32 *data)
{
	u32 csr_base;
        u32 addr;

	if (ip == IP_BLK_QM) {
		csr_base = qm_csr_addr;
	} else if (ip == IP_BLK_QML) {
		if (is_noqml) {
			printk(KERN_ERR
				"Accessing non-existent QM lite hardware\n");
			return -ENODEV;
		}
		csr_base = qml_csr_addr;
	} else if (ip == IP_BLK_IPP_QML) {
		if (is_noqml) {
			printk(KERN_ERR
				"Accessing non-existent QM lite hardware\n");
			return -ENODEV;
		}
		csr_base = qml_ipp_csr_addr;
	} else {
                QM_ERR_CHK("Invalid IP block in QM reg read: %d %s %d\n", ip,
                        __FILE__, __LINE__);
		return -1;
	}

        addr = csr_base + offset;
#ifdef QM_DEBUG
	if (ip == IP_BLK_IPP_QML) {
		if (offset > 0x240) {
                QM_ERR_CHK("Invalid offset in iPP QML CSR reg read: %x %s %d\n", offset,
                        __FILE__, __LINE__);
                return -1;
		}
	}

        if (offset > CSR_PPC_SAB7_ADDR) {
                QM_ERR_CHK("Invalid offset in QM reg read: %x %s %d\n", offset,
                        __FILE__, __LINE__);
                return -1;
        }
#endif
        *data = in_be32((void __iomem *) addr);
        QMREAD_PRINT("Read addr 0x%x data 0x%x\n", addr, *data);

        return 0;
}

static int apm_qm_driver_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int apm_qm_driver_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t apm_qm_driver_read(struct file *file, char __user * buf,
					size_t count, loff_t *ppos)
{
	printk("Read from Mamba QM device \n");
	return 0;
}

static u32 ascii_to_decimal(const char __user *buf, size_t len)
{
	u32 decimal = 0;

	while (len) {
		if (!(*buf >= '0' && *buf <= '9'))
			break;
		decimal *= 10;
		decimal += *buf - '0';
		len--;
		buf++;
	}

	return decimal;
}

static ssize_t apm_qm_driver_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
#define CMD_DUMP_PBN		'0'
#define CMD_READ_CSR		'1'
#define CMD_QUEUE_STATE_RAW	'2'
#define CMD_QUEUE_STATE 	'3'
#define CMD_SEND_MSG		'4'
#define CMD_RECV_MSG		'5'
#define CMD_CREATE_Q0		'6'

	struct apm_qm_qstate qstate = {0};
	struct apm_qm_raw_qstate qstate_raw = {0};
	int qid;
        struct apm_qm_qstate *qm_qstates = NULL;
        struct apm_qm_qalloc qm_qalloc = {0};
	int i;
	int pbn_val;
	u32 val;
	u32 reg_offset;

/* :CD: change this to switch-case statement */
	if (*buf == CMD_DUMP_PBN) {
		printk("Dumping PBN info for Ethernet port 0 work queues: \n");
		for (i = 0; i < 8; i++) {
			pbn_val = apm_qm_pb_get(IP_BLK_QM, PB_SLAVE_ID_ETH, i);
			printk("PBN: %d Num of msgs in buf: %d, queue number: %d, is it freepool: %d, is PBN enabled: %d\n", 
					i, pbn_val & 0xF, (pbn_val >> 4) & 0xFF, pbn_val >> (31 - 19) & 1,
					pbn_val >> (31 - 17) & 0x1);
		}
		printk("Dumping PBN info for Ethernet port 0 free queues: \n");
		for (i = 0; i < 17; i++) {
			pbn_val = apm_qm_pb_get(IP_BLK_QM, PB_SLAVE_ID_ETH, (i + 0x20));
			printk("PBN: %x, Num of msgs in buf: %d, queue number: %d, is it freepool: %d, is PBN enabled: %d\n", 
					(i + 0x20), pbn_val & 0xF, (pbn_val >> 4) & 0xFF, pbn_val >> (31 - 19) & 1,
					pbn_val >> (31 - 17) & 0x1);
		}
	} else if (*buf == CMD_READ_CSR) {
		buf++; /* skip space */
		buf++; /* get reg offset */
		reg_offset = ascii_to_decimal(buf, count);
		apm_qm_rd32(IP_BLK_QM, reg_offset, &val);
		printk("QM register at offset: 0x%x val: 0x%x\n", reg_offset, val);
	} else if (*buf == CMD_QUEUE_STATE_RAW) {
		buf++; /* skip space */
		buf++; /* get queue number */
		qid = ascii_to_decimal(buf, count);
		printk("Reading queue state for qid: %d\n", qid);
		apm_qm_raw_qstate_rd(IP_BLK_QM, qid, &qstate_raw);
		QM_PRINT("QM QState Read: QID %d location 0x%x\n", 
			qid, CSR_QSTATE_ADDR);
#if defined(CONFIG_APM867xx)
		QM_PRINT("0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n", 
			qstate_raw.w0, qstate_raw.w1, qstate_raw.w2,
			qstate_raw.w3, qstate_raw.w4);
#else
		QM_PRINT("0x%08X 0x%08X 0x%08X 0x%08X\n", 
			qstate_raw.w0, qstate_raw.w1, qstate_raw.w2,
			qstate_raw.w3);
#endif
	} else if (*buf == CMD_QUEUE_STATE) {
		buf++; /* skip space */
		buf++; /* get queue number */
		qid = ascii_to_decimal(buf, count);
		printk("Reading queue state for qid: %d\n", qid);
		apm_qm_qstate_rd(IP_BLK_QM, qid, &qstate);
		dump_qstate(&qstate);
	} else if (*buf == CMD_SEND_MSG) {
		buf++; /* skip space */
		buf++; /* get queue number */
		qid = ascii_to_decimal(buf, count);
		printk("Sending msg on queue for core 0: %d\n", qid);
		apm_qm_send_msg_util(qid, 0);
	} else if (*buf == CMD_RECV_MSG) {
		buf++; /* skip space */
		buf++; /* get queue number */
		qid = ascii_to_decimal(buf, count);
		printk("Receiving msg from queue: %d\n", qid);
		apm_qm_qstate_rd(IP_BLK_QM, qid, &qstate);
		apm_qm_rx_msg_util(qid, qstate.mb_id);
	} else if (*buf == CMD_CREATE_Q0) {
		/* create dummy queue for PPC0 */
                qm_qstates =
                        kmalloc(sizeof(struct apm_qm_qstate), GFP_KERNEL);

                if (qm_qstates == NULL) {
                        printk("Couldn't allocate memory for QM queues state\n");
                        return -ENOMEM;
               	}

       	        memset(qm_qstates, 0, sizeof(struct apm_qm_qstate));
	
        	/* Allocate egress work queue for PPC1 to PPC0 */
	        qm_qalloc.qstates = qm_qstates;

        	qm_qalloc.qm_ip_blk = IP_BLK_QM;
	        qm_qalloc.ip_blk = IP_PPC0;
        	qm_qalloc.ppc_id = 1;
	        qm_qalloc.q_type = P_QUEUE;
        	qm_qalloc.q_count = 1;
	        qm_qalloc.direction = DIR_EGRESS;
        	qm_qalloc.qsize = SIZE_16KB;
	        qm_qalloc.thr_set = 1;

	        if (apm_qm_alloc_q(&qm_qalloc)) {
        	        printk("Error allocating ingress work queue\n");
	        }
		printk("Created queue with qid: %d, mbid: %d\n", 
			qm_qstates->q_id, qm_qstates->mb_id);

		dump_qstate(qm_qstates);

		kfree(qm_qstates);
	} else {
		printk("QM util help\n");
		printk("Dump QM PBN: cmd id %c\n", CMD_DUMP_PBN);
		printk("Read QM CSR: cmd id %c, reg offset in decimal\n", CMD_READ_CSR);
		printk("Check queue state raw: cmd id %c, queue no\n", CMD_QUEUE_STATE_RAW);
		printk("Check queue state: cmd id %c, queue no\n", CMD_QUEUE_STATE);
		printk("Send msg on a queue: cmd id %c, queue no\n", CMD_SEND_MSG);
		printk("Receive msg on a queue: cmd id %c, queue no\n", CMD_RECV_MSG);
		printk("Create dummy queue dst to PPC0: cmd id %c\n", CMD_CREATE_Q0);
	}

	return count;
}

static long apm_qm_driver_ioctl(struct file *file,
				unsigned int cmd, unsigned long arg)
{
	int retval = 0;
	printk("IOCTL to Mamba QM device \n");
	return retval;
}

irqreturn_t apm_qm_sab_int(int value, void *id)
{
	printk("QM SAB interrupt occured\n");
	return IRQ_HANDLED;
}

irqreturn_t apm_qm_sys_int(int value, void *id)
{
	printk("QM Error interrupt occured\n");
	return IRQ_HANDLED;
}

#if defined(CONFIG_QM_IRQ_COALECING)

#define MSG_NOT_EMPTY_COAL_TAP 0x7
#define MBOX_COAL_TAP	0x4

/* This will programe CSR_PBM_COAL's qne_ctick_sel bits which sets a tap selects for
 * queue not empty interrupt.
 */
void apm_qm_msg_not_empty_intr_coal_set(void)
{
	u32 val;

	apm_qm_rd32(IP_BLK_QM, CSR_PBM_COAL_ADDR, &val);
	val |= QNE_CTICK_SEL_WR(0x7);
	apm_qm_wr32(IP_BLK_QM, CSR_PBM_COAL_ADDR, val);
}

void apm_qm_mbox_intr_coal_select(int mbox_id)
{
	u32 val;

	switch (mbox_id) {
	case 0:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX00_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 1:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX10_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 2:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX20_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 3:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX30_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 4:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX40_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 5:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX50_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 6:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX60_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 7:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, &val);
		val |= MBOX70_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK0_ADDR, val);
		break;
	case 8:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX81_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 9:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX91_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 10:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX101_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 11:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX111_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 12:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX121_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 13:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX131_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 14:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX141_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	case 15:
		apm_qm_rd32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, &val);
		val |= MBOX151_WR(MBOX_COAL_TAP);
		apm_qm_wr32(IP_BLK_QM, CSR_PBM_CTICK1_ADDR, val);
		break;
	default:
		printk("%s: Wrong mail box id: %d\n", __func__, mbox_id);
	}
}

#endif /* CONFIG_QM_IRQ_COALECING */

int apm_qm_mb_tasklet_unregister(struct apm_qm_mb_tasklet *handler)
{
	int rc = -1;
	u32 mailbox;

	if (handler == NULL) {
                QM_PRINT("Mailbox tasklet handler is NULL\n");
		goto _ret_qm_mb_tasklet_unregister;
	}

	mailbox = handler->mailbox;
	if (mailbox < 0 || mailbox > MAX_MAILBOXS) {
                QM_PRINT("Mailbox %d out of range\n", mailbox);
		goto _ret_qm_mb_tasklet_unregister;
	} else if (apm_qm_mb_tasklet_table[mailbox] == NULL) {
                QM_PRINT("Mailbox %d handler never registered\n", mailbox);
		goto _ret_qm_mb_tasklet_unregister;
	}

	tasklet_kill(&handler->tasklet);
	kfree(handler);
        apm_qm_mb_tasklet_table[mailbox] = NULL;
	rc = 0;

_ret_qm_mb_tasklet_unregister:
	return rc;
}

int apm_qm_mb_tasklet_register(u32 mailbox, u32 queue, u32 core, void *private,
		void (*func)(unsigned long))
{
	int rc = -1;
	struct apm_qm_mb_tasklet *handler;

	if (mailbox < 0 || mailbox > MAX_MAILBOXS) {
                QM_PRINT("Mailbox %d out of range\n", mailbox);
		goto _ret_qm_mb_tasklet_register;
	} else if (apm_qm_mb_tasklet_table[mailbox]) {
                QM_PRINT("Mailbox %d handler already registered\n", mailbox);
		goto _ret_qm_mb_tasklet_register;
	}

	handler = kmalloc(sizeof(struct apm_qm_mb_tasklet), GFP_KERNEL);
	if (handler == NULL) {
                QM_PRINT("Mailbox %d tasklet handler allocation failed\n", mailbox);
		goto _ret_qm_mb_tasklet_register;
	}

	handler->mailbox = mailbox;
	handler->queue = queue;
	handler->core = core;
	handler->private = private;
	tasklet_init(&handler->tasklet, func, (unsigned long)handler);
        apm_qm_mb_tasklet_table[mailbox] = handler;
	rc = 0;

_ret_qm_mb_tasklet_register:
	return rc;
}

irqreturn_t apm_qm_msg_int(int value, void *id)
{
	struct apm_qm_msg64 msg;
	struct apm_qm_msg16 *msg16 = &(msg.msg32_1.msg16);
	struct apm_qm_msg_desc msg_desc;
	u32 mb_id = *(u32 *)id; /* this will be mailbox number */

	QM_DBG("QM Mailbox interrupt occured Value: %d\n", mb_id);

	msg_desc.msg = &msg;
	memset(&msg, 0, sizeof(msg)); /* FIXME remove this later for performance */

/* :CD: change this to callback */
	/* retrieve message */
	msg_desc.mb_id = mb_id; 
	msg_desc.qid = mb_to_q[mb_id];

#if defined(CONFIG_APM_NET)
	/* Ethernet RX frame, don't read msg yet, call enet function directly */
	if (mb_id == IP_ETH0 || mb_id == IP_ETH1 || 
		mb_id ==  (8 + IP_ETH0) || mb_id ==  (8 + IP_ETH1)) {
		apm_enet_rx_irq(&msg_desc);
		return IRQ_HANDLED;
	}
#endif

	if (apm_qm_mb_tasklet_table[mb_id]) {
		apm_qm_disable_mb_irq(mb_id);
		tasklet_schedule(&apm_qm_mb_tasklet_table[mb_id]->tasklet);
		return IRQ_HANDLED;
	}

	QM_DBG("Retriving msg from MB: %d Queue ID: %d\n", msg_desc.mb_id, 
								msg_desc.qid);
	msg_desc.is_msg16 = 0;
	if (apm_qm_pull_msg(&msg_desc) == -1) {/* Return if invalid interrupt */
		QM_DBG("Returning from IRQ without calling callback\n");
		return IRQ_HANDLED;
	}

	QM_DBG("Message received with rtype: %d\n", msg16->RType);
#if !defined CONFIG_SLAB_HW
        if (msg_desc.qid == err_qid) {
                QM_DBG("QM Error: Msg Rx on Err Q: %d with RType: %d\n", 
			msg_desc.qid, msg16->RType);
		print_hex_dump(KERN_INFO, "Err q MSG: ", DUMP_PREFIX_ADDRESS, 16, 4, msg16, msg16->NV ? 64 : 32, 1);
		apm_qm_parse_error(&msg_desc);
                return IRQ_HANDLED;
        }
#endif
	/* call registered callback for this rtype else discard */
	if (apm_qm_cb_fn_table[msg16->RType]) {
		QM_DBG("QM Calling registered callback  Rtype is [%d]\n",
                       msg16->RType);
		(*apm_qm_cb_fn_table[msg16->RType])(&msg_desc);
	} else {
                printk(KERN_ERR 
                	"Error: QM No Callback Registered for Rtype [%d]\n",
                       msg16->RType);
                printk(KERN_ERR
                	"Message received for Queue ID = %d, mb_id = %d \n",
                       msg_desc.qid, msg_desc.mb_id);
		print_hex_dump(KERN_ERR, "READ MSG ", DUMP_PREFIX_ADDRESS, 
				16, 4, msg_desc.msg, 32, 1); 
        }
        return IRQ_HANDLED;
}

int apm_qm_config_set(struct device_node *np)
{
	int rc = 0;
	int inum = 0, i = 0, j = 0;
	struct resource	   res;
	u32 csr_addr_size;
	u32 qm_fabric_size;
	u32 qml_fabric_size;
	u32 qml_ipp_csr_size;
#if defined CONFIG_MB_MEMQ || defined CONFIG_ENQ_MB_MEMQ
	u32 qm_ocm_mbox_size;
#endif
#if defined CONFIG_MB_DDR || defined CONFIG_MB_OCM || defined CONFIG_ENQ_MB_MEMQ 
	u32 enq_mbox_addr_phy;
	u32 dq_mbox_addr_phy;
	u32 fq_mbox_addr_phy;
#endif
	u32 *maxq;
	u32 *qstart;
	u32 *maxmbs;
	u32 maxq_len;
	u32 qstart_len;
	u32 maxmbs_len;
#ifdef QM_ALTERNATE_ENQUEUE
	u32 *val;
	u32 val_len;
#endif
	//u32 val;
	u32 val_rd;

	/* Retrieve QM CSR register address and size */
	rc = of_address_to_resource(np, 0, &res);
	if (rc)
		return -ENODEV;

	qm_csr_paddr = res.start;
	csr_addr_size = RES_SIZE(&res);
	qm_csr_addr = (u32) ioremap_nocache(qm_csr_paddr, csr_addr_size);
	QM_DBG("QM csr phy addr: %010llx, size: %x, virt addr: %x\n", 
		qm_csr_paddr, csr_addr_size, qm_csr_addr);

	if (!qm_csr_addr) {
		printk(KERN_ERR "Failed to ioremap QM CSR region\n");
		return -ENODEV;
	}
	if ((num_online_cpus() > 1) || !mfspr(SPRN_PIR)) {
		/* Bring QM CSR memory out of shutdown */
		rc = apm86xxx_disable_mem_shutdown((u32 __iomem *)(qm_csr_addr+
						APM_GLBL_DIAG_OFFSET+
						QM_CFG_MEM_RAM_SHUTDOWN_ADDR),
						QMTM_F2_MASK);
		if (rc) {
			printk(KERN_ERR "Failed to bring QM CSR RAM"
			       "out of shutdown\n");
			return -ENODEV;
		}
	}
	/* Retrieve Primary Fabric address and size */
	rc = of_address_to_resource(np, 1, &res);
	if (rc)
		return -ENODEV;

	qm_fabric_paddr = res.start;
	qm_fabric_size = RES_SIZE(&res);
	qm_fabric_addr = (u32) ioremap_nocache(qm_fabric_paddr, qm_fabric_size);
	QM_DBG("QM fabric phy addr: %010llx, size: %x, virt addr: %x\n", 
		qm_fabric_paddr, qm_fabric_size, qm_fabric_addr);

#if defined CONFIG_ENQ_MB_MEMQ /* only ENQ MB in MEMQ, DQ and FQ are in DDR and cached */
	/* Retrieve mailbox address and size */
	rc = of_address_to_resource(np, 2, &res);
	if (rc)
		return -ENODEV;

	qm_mailbox_paddr = res.start;
	qm_ocm_mbox_size = RES_SIZE(&res);
	/* Enq mailbox non cacheable */
	qm_ocm_enq_mbox_addr = (u32) ioremap_nocache(qm_mailbox_paddr, 4096);
	enq_mbox_addr_phy = qm_mailbox_paddr;

	/* Dq and FQ mailboxes cacheable */
	qm_ocm_dq_mbox_addr = (u32) ioremap_flags(0x30010000, 4096, 0);
	dq_mbox_addr_phy = 0x30010000;

#ifdef CHECK_UINFO
	memset((void *) qm_ocm_dq_mbox_addr, 0, 4096);
	flush_dcache_range((u32) qm_ocm_dq_mbox_addr, (u32) (qm_ocm_dq_mbox_addr + 1024));
#endif

	qm_ocm_fp_mbox_addr = (u32) ioremap_flags(0x30020000, 4096, 0);
	fq_mbox_addr_phy = 0x30020000;

#ifdef CHECK_UINFO
	memset((void *) qm_ocm_fp_mbox_addr, 0, 4096);
	flush_dcache_range((u32) qm_ocm_fp_mbox_addr, (u32) (qm_ocm_fp_mbox_addr + 1024));
#endif

	apm_qm_wr32(IP_BLK_QM, CSR_PPC_MBOX_BASE_ADDR, 0x80000000 | (dq_mbox_addr_phy >> 12));
	apm_qm_wr32(IP_BLK_QM, CSR_PPC_FPOOL_BASE_ADDR, 0x80000000 | (fq_mbox_addr_phy >> 12));

	apm_qm_rd32(IP_BLK_QM, CSR_ENQ_BASE_A_ADDR, &val_rd);
	apm_qm_rd32(IP_BLK_QM, CSR_PPC_MBOX_BASE_ADDR, &val_rd);
	apm_qm_rd32(IP_BLK_QM, CSR_PPC_FPOOL_BASE_ADDR, &val_rd);
#elif defined CONFIG_MB_MEMQ /* All mailboxes are in MEMQ, they are non-cacheable */
	/* Retrieve mailbox address and size */
	rc = of_address_to_resource(np, 2, &res);
	if (rc)
		return -ENODEV;

	qm_mailbox_paddr = res.start;
	qm_ocm_mbox_size = RES_SIZE(&res);
	qm_ocm_enq_mbox_addr = (u32) ioremap_nocache(qm_mailbox_paddr, 
					qm_ocm_mbox_size);
	qm_ocm_dq_mbox_addr = qm_ocm_enq_mbox_addr + 0x1000;
	qm_ocm_fp_mbox_addr = qm_ocm_dq_mbox_addr + 0x1000;

#ifdef CHECK_UINFO
	memset((void *) qm_ocm_dq_mbox_addr, 0, 4096);
#endif

	QM_DBG("QM OCM MBox phy addr: %010llx, size: %x, en addr: %x, \
	dq addr: %x, fp addr: %x\n", qm_mailbox_paddr, qm_ocm_mbox_size, 
		qm_ocm_enq_mbox_addr, qm_ocm_dq_mbox_addr, qm_ocm_fp_mbox_addr);
#elif defined CONFIG_MB_OCM /* All mailboxes are in OCM, they are cacheable */ 
	qm_ocm_enq_mbox_addr = (u32) ioremap_flags(0xeffffc000, 
					4096, _PAGE_COHERENT);
	enq_mbox_addr_phy = 0xeffffc00;

	qm_ocm_dq_mbox_addr = (u32) ioremap_flags(0xeffffd000,
					4096, _PAGE_COHERENT);
	dq_mbox_addr_phy = 0xeffffd00;

	qm_ocm_fp_mbox_addr = (u32) ioremap_flags(0xeffffe000,
					4096, _PAGE_COHERENT);
	fq_mbox_addr_phy = 0xeffffe00;

	apm_qm_wr32(IP_BLK_QM, CSR_ENQ_BASE_A_ADDR, 0x80000000 | (enq_mbox_addr_phy >> 10));
	apm_qm_wr32(IP_BLK_QM, CSR_PPC_MBOX_BASE_ADDR, 0x80000000 | (dq_mbox_addr_phy >> 12));
	apm_qm_wr32(IP_BLK_QM, CSR_PPC_FPOOL_BASE_ADDR, 0x80000000 | (fq_mbox_addr_phy >> 12));

#elif defined CONFIG_MB_DDR
#if 1 /* Don't allocate memory, use fixed memory location outside kernel */
	qm_ocm_enq_mbox_addr = (u32) ioremap_flags(0x30000000, 4096, 0);
	enq_mbox_addr_phy = (u32) 0x30000000;

#ifdef CHECK_UINFO
	memset((void *) qm_ocm_enq_mbox_addr, 0, 4096);
	flush_dcache_range((u32) qm_ocm_enq_mbox_addr, (u32) (qm_ocm_enq_mbox_addr + 1024));
#endif
	qm_ocm_dq_mbox_addr = (u32) ioremap_flags(0x30010000,	4096, 0);
	dq_mbox_addr_phy = (u32) 0x30010000;
#ifdef CHECK_UINFO
	memset((void *) qm_ocm_dq_mbox_addr, 0, 4096);
	flush_dcache_range((u32) qm_ocm_dq_mbox_addr, (u32) (qm_ocm_dq_mbox_addr + 1024));
#endif

	qm_ocm_fp_mbox_addr = (u32) ioremap_flags(0x30020000, 4096, 0);
	fq_mbox_addr_phy = (u32) 0x30020000;
#ifdef CHECK_UINFO
	memset((void *) qm_ocm_fp_mbox_addr, 0, 4096);
	flush_dcache_range((u32) qm_ocm_fp_mbox_addr, (u32) (qm_ocm_fp_mbox_addr + 1024));
#endif

#else /* allocate memory for mailboxes */
	/* create mailboxes in DDR */
	qm_ocm_enq_mbox_addr = (u32) MEMALLOC(8 * 1024);
	qm_ocm_enq_mbox_addr = qm_ocm_enq_mbox_addr + (4 * 1024);
	qm_ocm_enq_mbox_addr = qm_ocm_enq_mbox_addr & 0xfffff000;
	enq_mbox_addr_phy = (u32)virt_to_phys((void *) qm_ocm_enq_mbox_addr);

	if (qm_ocm_enq_mbox_addr == 0x0) {
		QM_PRINT("Couldn't allocate memory for Enq mailboxes\n");
		return -1;
	}

	qm_ocm_dq_mbox_addr = (u32) MEMALLOC(8 * 1024);
	qm_ocm_dq_mbox_addr = qm_ocm_dq_mbox_addr + (4 * 1024);
	qm_ocm_dq_mbox_addr = qm_ocm_dq_mbox_addr & 0xfffff000;
	dq_mbox_addr_phy = (u32)virt_to_phys((void *) qm_ocm_dq_mbox_addr);
	if (qm_ocm_dq_mbox_addr == 0x0) {
		QM_PRINT("Couldn't allocate memory for DQ mailboxes\n");
		return -1;
	} 

	qm_ocm_fp_mbox_addr = (u32) MEMALLOC(8 * 1024);
	qm_ocm_fp_mbox_addr = qm_ocm_fp_mbox_addr + (4 * 1024);
	qm_ocm_fp_mbox_addr = qm_ocm_fp_mbox_addr & 0xfffff000;
	fq_mbox_addr_phy = (u32)virt_to_phys((void *) qm_ocm_fp_mbox_addr);
	if (qm_ocm_fp_mbox_addr == 0x0) {
		QM_PRINT("Couldn't allocate memory for FQ mailboxes\n");
		return -1;
	}

#endif
	apm_qm_wr32(IP_BLK_QM, CSR_ENQ_BASE_A_ADDR, 0x80000000 | (enq_mbox_addr_phy >> 10));
	apm_qm_wr32(IP_BLK_QM, CSR_PPC_MBOX_BASE_ADDR, 0x80000000 | (dq_mbox_addr_phy >> 12));
	apm_qm_wr32(IP_BLK_QM, CSR_PPC_FPOOL_BASE_ADDR, 0x80000000 | (fq_mbox_addr_phy >> 12));
#endif

#ifdef QM_ALTERNATE_ENQUEUE
	val = (u32 *) of_get_property(np, "dma-reg", &val_len);
	if (val == NULL || val_len < 3) {
		printk(KERN_ERR "No DTS dma-reg\n");
		return -ENODEV;
	}		
	queue_baddr_p = val[1];
	qmem_size = val[2];
	queue_eaddr_p = queue_baddr_p + qmem_size;
	queue_baddr = (u32) ioremap_nocache(queue_baddr_p, qmem_size);
#endif

	/* Check and skip QM lite init if hardware not available */
	if (is_noqml) {
		qml_fabric_paddr = 0;
		qml_fabric_size = 0;
		qml_fabric_addr = 0;
		qml_csr_paddr = 0;
		qml_csr_addr = 0;
		qml_ipp_csr_paddr = 0;
		qml_ipp_csr_size = 0;
		qml_ipp_csr_addr = 0;
		goto skip_qml;
	}
	
	/* Retrieve QM lite Fabric address and size */
	rc = of_address_to_resource(np, 3, &res);
	if (rc)
		return -ENODEV;

	qml_fabric_paddr = res.start;
	qml_fabric_size = RES_SIZE(&res);
	qml_fabric_addr = (u32) ioremap_nocache(qml_fabric_paddr, csr_addr_size);
	QM_DBG("QM light Fabric phy addr: %010llx, size: %x, virt addr: %x\n", 
		qml_fabric_paddr, qml_fabric_size, qml_fabric_addr);

	/* Retrieve QM lite CSR register address and size */
	rc = of_address_to_resource(np, 4, &res);
	if (rc)
		return -ENODEV;

	qml_csr_paddr = res.start;
	csr_addr_size = RES_SIZE(&res);
	qml_csr_addr = (u32) ioremap_nocache(qml_csr_paddr, csr_addr_size);
	QM_DBG("QM light CSR phy addr: %010llx, size: %x, virt addr: %x\n", 
		qml_csr_paddr, csr_addr_size, qml_csr_addr);

	/* Retrieve QM lite iPP CSR register address and size */
	rc = of_address_to_resource(np, 5, &res);
	if (rc)
		return -ENODEV;

	qml_ipp_csr_paddr = res.start;
	qml_ipp_csr_size = RES_SIZE(&res);
	qml_ipp_csr_addr = (u32) ioremap_nocache(qml_ipp_csr_paddr, qml_ipp_csr_size);
	QM_DBG("QM light iPP CSR phy addr: %010llx, size: %x, virt addr: %x\n", 
		qml_ipp_csr_paddr, qml_ipp_csr_size, qml_ipp_csr_addr);

skip_qml:
	memset(&apm_qm_mb_tasklet_table, 0, sizeof(apm_qm_mb_tasklet_table));
	maxmbs = (u32 *) of_get_property(np, "maxmboxes", &maxmbs_len);
        if (maxmbs == NULL || maxmbs_len < sizeof(u32)) {
		maxmboxes = 16;
		start_mboxes = 0;
        } else {
		if (*maxmbs == 16) {
			start_mboxes = 0;
		} else {
			start_mboxes = 8 * mfspr(SPRN_PIR);
		}
		maxmboxes = *maxmbs;
	}
	/* configure mailboxes */
	apm_qm_alloc_mbox_ocm();

	/* get interrupts */

	if (is_smp || !mfspr(SPRN_PIR)) { /* error irq will be handled by core 0 only */ 

		/* get error interrupt */
		inum = of_irq_to_resource(np, i, NULL);
		if (inum == NO_IRQ) {
			printk(KERN_ERR "Failed to map system interrupt QM Err\n");
		} else {
			rc = request_irq(inum, apm_qm_sys_int, 0, "QM-Err", NULL);
			if (rc) {
				printk(KERN_ERR 
					"Could not register for intr: %d\n", inum);
				return rc;
			} else {
				QM_DBG("Registered Error Interrupt. %d \n", inum);
			}
		}
		i++;

		/* get SAB event interrupt */
		inum = of_irq_to_resource(np, i, NULL);
		if (inum == NO_IRQ) {
			printk(KERN_ERR "Failed to map SAB interrupt\n");
		} else {
			rc = request_irq(inum, apm_qm_sab_int, 0, "QM-SAB", NULL);
			if (rc) {
				printk(KERN_ERR 
					"Could not register for intr: %d\n", inum);
				return rc;
			} else {
				QM_DBG("Registered Error Interrupt. %d \n", inum);
			}
		}
		i++;
	}

/* :CD: No need to use SMP or AMP, just read resources from DTS */
	if (!is_smp) {
		for (j = 0; j < maxmboxes; j++)  {
			irq_mb_map[j] = j + start_mboxes;
		}
	} else {
		for (j = 0; j < maxmboxes; j++) {
			irq_mb_map[j] = j;
		}
	}

	j = 0;
#if defined(CONFIG_QM_IRQ_COALECING)
	apm_qm_msg_not_empty_intr_coal_set();
#endif
	/* get mailbox interrupts */
	while(1) {
		inum = of_irq_to_resource(np, i, NULL);
		QM_DBG("Cfg QM Mailbox: %d Interrupt: %d, userval: %d  \n", 
					i, inum, irq_mb_map[j]);
		
		if (inum != NO_IRQ) {
#if defined(CONFIG_QM_IRQ_COALECING)
			apm_qm_mbox_intr_coal_select(j);
#endif
			rc = request_irq(inum, apm_qm_msg_int, 0, "QM-mailbox", 
						(void *) &irq_mb_map[j]);
			if (rc) {
				printk(KERN_ERR \
					"Could not register intr: %d\n",inum);
				return rc;
			} else {
				QM_DBG("Registered QM Intr. %d \n", inum);
				if (!is_smp) {
					mb_to_irq_map[j + (8 * mfspr(SPRN_PIR))] = inum;
				} else {
					mb_to_irq_map[j] = inum;
				}
				QM_DBG("Registered QM Intr. %d \n", inum);
				j++;
			}
		} else {
			break;
		}
		i++; 
	}
	QM_DBG("Registered Mailbox Interrupts.\n");

	/* Enable QM mailbox interrupts and deq axi error interrupt */
	apm_qm_wr32(IP_BLK_QM, QM_INTERRUPTMASK_ADDR, 0x00000005);

	maxq = (u32 *) of_get_property(np, "maxqid", &maxq_len);

        if (maxq == NULL || maxq_len < sizeof(u32)) {
                printk(KERN_ERR "Unable to retrive Max Queue value from DTS\n");
                return -EINVAL;
        }

	max_qid = *maxq;

	qstart = (u32 *) of_get_property(np, "qstart", &qstart_len);

        if (qstart == NULL || qstart_len < sizeof(u32)) {
                printk(KERN_ERR "Unable to retrive start of queue value from DTS\n");
                return -EINVAL;
        }

	start_qid = *qstart;

	apm_qm_rd32(IP_BLK_QM, CSR_ENQ_BASE_A_ADDR, &val_rd);
	apm_qm_rd32(IP_BLK_QM, CSR_PPC_MBOX_BASE_ADDR, &val_rd);
	apm_qm_rd32(IP_BLK_QM, CSR_PPC_FPOOL_BASE_ADDR, &val_rd);
	
	return rc;
}

int apm_qm_config_clear(void)
{
	int rc = 0;

	return rc;
}

int apm_qm_init_errq(int ppc_id)
{
	int rc = 0;
	u32 val;
	struct apm_qm_qalloc eth_qalloc;
	struct apm_qm_qstate enet_qstates;

	/* program threshold set 1 and all hysteresis */
	apm_qm_wr32(IP_BLK_QM, CSR_THRESHOLD0_SET1_ADDR, 100);
	apm_qm_wr32(IP_BLK_QM, CSR_THRESHOLD1_SET1_ADDR, 200);
	apm_qm_wr32(IP_BLK_QM, CSR_HYSTERESIS_ADDR, 0xFFFFFFFF);

	/* create error queue */
	memset(&eth_qalloc, 0, sizeof (struct apm_qm_qalloc));
	memset(&enet_qstates, 0, sizeof (struct apm_qm_qstate));
	eth_qalloc.qstates = &enet_qstates;
	eth_qalloc.qm_ip_blk = IP_BLK_QM;
	eth_qalloc.ip_blk = IP_OCMM; /* using OCM MB for now */
	eth_qalloc.ppc_id = ppc_id;  /* currently all err msgs go to core 0 */
	eth_qalloc.q_type = P_QUEUE;
	eth_qalloc.q_count = 1;
	eth_qalloc.direction = DIR_INGRESS;
	eth_qalloc.qsize = SIZE_16KB;
	eth_qalloc.thr_set = 1;

	if ((rc = apm_qm_alloc_q(&eth_qalloc)) != 0) {
		printk("Error allocating error queue\n");
		return rc;
	}

	err_qid = enet_qstates.q_id;

	/* Enable QPcore and assign error queue */
	val = 0;
	val = ENABLE_F6_SET(val, 1);
#ifdef CONFIG_APM862xx
	val = ERROR_QID_F2_SET(val, err_qid);
	val = ERROR_QUEUE_ENABLE_F2_SET(val, APM_QM_ERROR_Q_ENABLE);
#endif
	apm_qm_wr32(IP_BLK_QM, CSR_QM_CONFIG_ADDR, val);

#ifdef CONFIG_APM867xx
	val = 0;
	val = UNEXPECTED_EN_SET(val, 1);
	val = UNEXPECTED_QID_SET(val, err_qid);
	val = EXPECTED_EN_SET(val, 1);
	val = EXPECTED_QID_SET(val, err_qid);
	apm_qm_wr32(IP_BLK_QM, CSR_DOM_0_ERRQ_ADDR, val);
	apm_qm_wr32(IP_BLK_QM, CSR_DOM_1_ERRQ_ADDR, val);
	apm_qm_wr32(IP_BLK_QM, CSR_DOM_2_ERRQ_ADDR, val);
	apm_qm_wr32(IP_BLK_QM, CSR_DOM_3_ERRQ_ADDR, val);
#endif
	
	/* Enable QM lite if hardware available */
	if (!is_noqml) {
		val = 0;
		val = ENABLE_F6_SET(val, 1);
#ifdef CONFIG_APM862xx
		val = ERROR_QID_F2_SET(val, DSLEEP_ENET_RX_Q);
		val = ERROR_QUEUE_ENABLE_F2_SET(val, APM_QM_ERROR_Q_ENABLE);
#endif
		apm_qm_wr32(IP_BLK_QML, CSR_QM_CONFIG_ADDR, val);
	}

	return rc;
}

struct file_operations apm_qm_driver_fops = {
	.owner = THIS_MODULE,
	.open = apm_qm_driver_open,
	.release = apm_qm_driver_release,
	.read = apm_qm_driver_read,
	.write = apm_qm_driver_write,
	.compat_ioctl = apm_qm_driver_ioctl,
};

static int __devinit apm_qm_probe(struct platform_device *pdev)
{
	int rc;
	struct proc_dir_entry *entry;
	struct clk *clk;
	u32 val;
        
	printk(KERN_INFO "%s v%s\n", APM_QM_DRIVER_STRING,
	       APM_QM_DRIVER_VERSION);

#ifdef CONFIG_SMP
	is_smp = 1;
#else
	is_smp = 0;
#endif

	/* Detect if QM lite available */
	is_noqml = is_apm86xxx_lite() ? 1 : (!is_smp && mfspr(SPRN_PIR) ? 1 : 0);
	       
	/* FIXME - Need locking for AMP */
	if ((num_online_cpus() > 1) || !mfspr(SPRN_PIR)) {
		/* reset QM */
		clk = clk_get(&pdev->dev, "qm-tm");
		if (IS_ERR(clk)) {
			printk("clk_get qm-tm failed\n");
			return -ENODEV;
		}
		clk_enable(clk);
		
		/* reset QML */
		if (!is_noqml) {
		clk = clk_get(&pdev->dev, "qml");
		if (IS_ERR(clk)) {
			printk("clk_get qml failed\n");
			return -ENODEV;
		}
		/* disable and enable clock/reset */
		clk_enable(clk);
	}
	}
	
	rc = apm_qm_config_set(pdev->dev.of_node);
	if (rc) {
		printk("apm_qm_config_set failed\n");
		return -1;
	}
	
	if (!is_noqml) {
	/* Configure QML QID Remap table */
	val = QID00_WR(DSLEEP_ENET_RX_FP_Q) |
		QID10_WR(DSLEEP_ENET_RX_Q)  |
		QID20_WR(DSLEEP_ENET_TX_Q);
	apm_qm_wr32(IP_BLK_QML, CSR_QML_REMAP0_ADDR, val);
	}	
	val = INIT_F2_WR(249);
	apm_qm_wr32(IP_BLK_QM, CSR_CU_TIMER_ADDR, val);
	apm_qm_rd32(IP_BLK_QM, CSR_CU_TIMER_ADDR, &val);
	QM_DBG("CSR_CU_TIMER: %d \n", val);
	
	/* configure and initialize QM block */
	apm_qm_init(ARRAY_SIZE(mb_cfg_pqs));
	
	QM_DBG("Creating proc entry\n");
	entry = create_proc_entry(APM_QM_DRIVER_NAME, 0, NULL);
	if (entry == NULL) {
		printk(KERN_ERR PFX " init failed\n");
		return -1;
	}

	entry->proc_fops = &apm_qm_driver_fops;
	/* Enable Enqueu/Dequeue coutners */
	val = QID_ENQ_COUNTER_F2_WR(12) |
		QID_DEQ_COUNTER_F2_WR(12);
	
	apm_qm_wr32(IP_BLK_QM, CSR_QM_STATS_CFG_ADDR, val);
	apm_qm_rd32(IP_BLK_QM, CSR_QM_STATS_CFG_ADDR, &val);
	QM_DBG("CSR_QM_STATS_CFG: 0x%x \n", val);

	return rc;
}

void apm_qm_disable_mb_irq(u32 mb_id)
{
	disable_irq_nosync(mb_to_irq_map[mb_id]);
}

void apm_qm_enable_mb_irq(u32 mb_id)
{
	enable_irq(mb_to_irq_map[mb_id]);
}

int apm_qm_set_mb_affinity(u32 mb_id, u32 core_id)
{
	int rc;
	if ((rc = irq_set_affinity(mb_to_irq_map[mb_id],
				cpumask_of(core_id))) != 0) {
		printk("Error %d in irq_set_affinity \n", rc); 
	}
	return rc;
}

static int __devexit apm_qm_remove(struct platform_device *dev)
{
	remove_proc_entry(APM_QM_DRIVER_NAME, NULL);
	printk(KERN_NOTICE PFX "Unloaded %s ..\n", APM_QM_DRIVER_STRING);
	return 0;
}

int apm_qm_shutdown_q(int q_num)
{
	struct apm_qm_raw_qstate raw_qstate = {0, 0, 0, 0};
	struct apm_qm_qstate *qstate;

	if (q_num < 0 || q_num >= MB_MAX_QUEUES) {
		QM_PRINT("Queue number is not valid\n");

	}

	qstate = &queue_states[q_num];

	if (qstate->valid && pbn_valid[qstate->q_id]) {
		QM_DBG("Disable qid:%d, slave_id: %d," "pbn: %d \n",
			qstate->q_id, qstate->slave_id, qstate->pbn);

		apm_qm_pb_disable(qstate->ip_blk, qstate->slave_id,
		qstate->pbn, qstate->q_id);
		apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id, &raw_qstate);
		qstate->valid = 0;
	}

	return 0;
}

int apm_qm_shutdown(void)
{
#ifndef CONFIG_SMP
	struct apm_qm_raw_qstate raw_qstate;
	int i;
	struct apm_qm_qstate *qstate;
	
	printk("QM Shutdown Starts ... \n");
	
	if (mfspr(SPRN_PIR)) {
		memset(&raw_qstate, 0, sizeof(struct apm_qm_raw_qstate));
		for (i = 125; i < 140; i++) {
			qstate = &queue_states[i];
			
			if (pbn_valid[i]) {
				QM_DBG("Disable qid:%d, slave_id: %d,"
					"pbn: %d \n",
					qstate->q_id, qstate->slave_id,
					qstate->pbn);
			       
				apm_qm_pb_disable(qstate->ip_blk,
					       	qstate->slave_id,
					      	qstate->pbn, qstate->q_id);
				apm_qm_raw_qstate_wr(IP_BLK_QM, i, &raw_qstate);
				pbn_valid[i] = 0;
			}
			
		}
		qstate = &queue_states[151];
		if  (pbn_valid[151]) {
			QM_DBG("Disable qid:%d, slave_id: %d, pbn: %d \n",
				qstate->q_id, qstate->slave_id, qstate->pbn);
			
			apm_qm_pb_disable(qstate->ip_blk,
					qstate->slave_id,
					qstate->pbn, qstate->q_id);
			apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id,
					&raw_qstate);
			pbn_valid[151] = 0;
		}
		qstate = &queue_states[153];
		if  (pbn_valid[153]) {
			QM_DBG("Disable qid:%d, slave_id: %d, pbn: %d \n",
			       qstate->q_id, qstate->slave_id, qstate->pbn);
			
			apm_qm_pb_disable(qstate->ip_blk,
					 qstate->slave_id,
					 qstate->pbn, qstate->q_id);
			apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id,
					    &raw_qstate);
			pbn_valid[153] = 0;
		}
	} else {
		memset(&raw_qstate, 0, sizeof(struct apm_qm_raw_qstate));
		for (i = 1; i < 10; i++) {
			qstate = &queue_states[i];
			if  (pbn_valid[i]) {
				QM_DBG("Disable qid:%d, slave_id: %d,"
				     	"pbn: %d \n",
				      	qstate->q_id, qstate->slave_id,
				     	qstate->pbn);
				apm_qm_pb_disable(qstate->ip_blk,
					      	qstate->slave_id,
					      	qstate->pbn, qstate->q_id);
				apm_qm_raw_qstate_wr(IP_BLK_QM, i, &raw_qstate);
				pbn_valid[i] = 0;
			}
		}
		qstate = &queue_states[150];

		if  (pbn_valid[150]) {
			QM_DBG("Disable qid:%d, slave_id: %d, pbn: %d \n",
			     	qstate->q_id, qstate->slave_id, qstate->pbn);
			       
			apm_qm_pb_disable(qstate->ip_blk,
				     	qstate->slave_id,
				      	qstate->pbn, qstate->q_id);
			apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id,
				       	&raw_qstate);
			pbn_valid[150] = 0;
		}
		qstate = &queue_states[152];

		if  (pbn_valid[152]) {
			QM_DBG("Disable qid:%d, slave_id: %d, pbn: %d \n",
			       qstate->q_id, qstate->slave_id, qstate->pbn);
			       
			apm_qm_pb_disable(qstate->ip_blk,
					 qstate->slave_id,
					 qstate->pbn, qstate->q_id);
			apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id,
					    &raw_qstate);
			pbn_valid[152] = 0;
		}
		
		qstate = &queue_states[254];

		if  (pbn_valid[254]) {
			QM_DBG("Disable Security qid:%d, slave_id: %d, pbn: %d \n",
			       qstate->q_id, qstate->slave_id, qstate->pbn);
			       
			apm_qm_pb_disable(qstate->ip_blk,
					 qstate->slave_id,
					 qstate->pbn, qstate->q_id);
			apm_qm_raw_qstate_wr(IP_BLK_QM, qstate->q_id,
					    &raw_qstate);
			pbn_valid[254] = 0;
		}
		
	}
	printk("QM Shutdown End ... \n");
#endif	
	return 0;
}


static struct of_device_id apm_qm_match[] = {
	{ .compatible	= "mb-qmtm", },
	{ .compatible	= "apm,mb-qmtm", },
	{ },
};

static struct platform_driver apm_qm_driver = {
	.driver = {
		.name		= "mb-qmtm",
		.of_match_table	= apm_qm_match,
	},
	.probe		= apm_qm_probe,
	.remove		= apm_qm_remove,
};


static int __init apm_qm_driver_init(void)
{
	return platform_driver_register(&apm_qm_driver);
}

void __exit apm_qm_driver_cleanup(void)
{
	platform_driver_unregister(&apm_qm_driver);
	printk(KERN_INFO PFX "%s exited.\n", APM_QM_DRIVER_STRING);
}

module_init(apm_qm_driver_init);
module_exit(apm_qm_driver_cleanup);

MODULE_VERSION(APM_QM_DRIVER_VERSION);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Keyur Chudgar <kchudgar@amcc.com>");
MODULE_DESCRIPTION("Core driver for Mamba SoC Queue manager and Traffic Manager");

