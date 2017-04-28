/*
 * APM86xx SlimPRO Offload Driver
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Anup Patel <apatel@apm.com>
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
 * This is the SlimPRO offload driver that configures the SlimPRO queues.
 * Upper offload layer can interface with this module for queues info. 
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/spinlock.h>
#include <asm/apm_slimpro_offload.h>
#include <asm/apm_qm_cfg.h>
#include <asm/apm_qm_core.h>

#define DRIVER_VERSION		"1.0"
#define SLIMPROID		"SlimPRO:"

struct slimpro_context {
	struct slimpro_queue queue;
	int q_cnt;
	spinlock_t prot;
};

struct slimpro_context slimpro_ctx;

static int slimpro_offload_probe(struct of_device *ofdev,
				const struct of_device_id *match)
{
	struct device_node *np = ofdev->node;
	struct qm_cfg_qconfig qconfig;
	u32 *val;
	int val_len;
	int err = 0;
	int i;

	printk(KERN_INFO "SlimPRO Offload v%s\n", DRIVER_VERSION);

	memset(&slimpro_ctx, 0, sizeof(slimpro_ctx));
	spin_lock_init(&slimpro_ctx.prot);
	for (i = 0; i < SLIMPRO_MAX_Q; i++) {
		slimpro_ctx.queue.wqid[i] = QM_CFG_INVALID;	
	}
	
	val = (u32 *) of_get_property(np, "queue", &val_len);
        if (val == NULL || val_len < sizeof(u32) * 2) {
                printk(KERN_ERR 
                	"Unable to retrive queue info from DTS\n");
                return -EINVAL;
        }        
        slimpro_ctx.q_cnt = val[1];
        if (slimpro_ctx.q_cnt > SLIMPRO_MAX_Q)
        	slimpro_ctx.q_cnt = SLIMPRO_MAX_Q;

	for (i = 0; i < slimpro_ctx.q_cnt; i++) {	
		memset(&qconfig, 0, sizeof(qconfig));
		qconfig.qid   = QM_CFG_INVALID;
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = IP_IPP;
		qconfig.ppc   = 0;
		qconfig.dir   = QM_CFG_DIR_INGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_PQ;
		if ((err = qm_cfg_add_qconfig(&qconfig))) {
			printk(KERN_ERR SLIMPROID 
				"Failed to configure SlimPRO offload RX queue\n");
			return err;
		}
		
		slimpro_ctx.queue.wqid[i] = qconfig.qid;
		printk(KERN_INFO "SlimPRO WQ[%d] %d\n", i, qconfig.qid);
	}
	return 0;
}

struct slimpro_queue slimpro_query_q(void)
{
	return slimpro_ctx.queue;
}
EXPORT_SYMBOL(slimpro_query_q);

int slimpro_queue_request(void)
{
	struct slimpro_queue *qinfo = &slimpro_ctx.queue;
	int i;
	int selected = -1;
	
	spin_lock(&slimpro_ctx.prot);
	if (slimpro_ctx.q_cnt) {
		selected = 0;
		for (i = 1; i < slimpro_ctx.q_cnt; i++) {
			if (qinfo->usage[i] < qinfo->usage[selected])
				selected = i;
		}
		++qinfo->usage[i];
	}
	spin_unlock(&slimpro_ctx.prot);
	if (selected >= 0)
		return qinfo->wqid[selected];
	return QM_CFG_INVALID;
}
EXPORT_SYMBOL(slimpro_queue_request);

static __initdata struct of_device_id slimpro_offload_match[] = {
	{ .compatible = "apm,slimpro-offload" },
	{}
};

static struct of_platform_driver slimpro_offload_driver = {
	.driver = {
		.name	= "apm-slimpro-offload",
	},
	.match_table = slimpro_offload_match,
	.probe = slimpro_offload_probe,
};

static int __init slimpro_offload_init(void)
{
	int i;
	
	memset(&slimpro_ctx, 0, sizeof(slimpro_ctx));
	for (i = 0; i < SLIMPRO_MAX_Q; i++) 
		slimpro_ctx.queue.wqid[i] = QM_CFG_INVALID;
	return of_register_platform_driver(&slimpro_offload_driver);
}

void __exit slimpro_offload_exit(void)
{
	of_unregister_platform_driver(&slimpro_offload_driver);
}

module_init(slimpro_offload_init);
module_exit(slimpro_offload_exit);

MODULE_AUTHOR("Loc Ho <lho@apm.com>");
MODULE_DESCRIPTION("SlimPRO Offload Driver");
MODULE_LICENSE("GPL");
