/*
 * APM86XXX Shared Memory Media Bearer for TIPC
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Inderpal saluja <isaluja@apm.com>
 *
 * based on net/tipc/eth_media.c: Ethernet bearer support for TIPC
 * Copyright (c) 2001-2007, Ericsson AB
 * Copyright (c) 2005-2007, Wind River Systems
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
 */

#include <net/tipc/tipc.h>
#include <net/tipc/tipc_bearer.h>
#include <net/tipc/tipc_msg.h>
#include <linux/interrupt.h>
#include <asm/apm_shm_icc.h>
#include <linux/delay.h>

#define MEDIA_NAME		"shm"
#define SHM_LINK_PRIORITY	TIPC_MAX_LINK_PRI
#define SHM_LINK_TOLERANCE 	TIPC_MAX_LINK_TOL
#define SHM_LINK_WINDOW		TIPC_DEF_LINK_WIN
#define	SM_SEND_TYPE		6
#define SHM_ALEN		4
#define SHM_MTU			PAGE_SIZE
#define CORE_ID			mfspr(SPRN_PIR)

/*
 * struct shm_bearer - Shared Memory bearer data structure
 * @bearer: ptr to bearer structure
 * @dev_addr: Core number
 * @recv_skb: recv skb
 * @rx_task: tasklet	
 */
struct shm_bearer {
	struct tipc_bearer *bearer;
	u32 dev_addr;
	struct sk_buff *recv_skb;
	struct tasklet_struct rx_task;
	struct sk_buff_head rxq;
};

static struct shm_bearer shmb;
static int shm_started = 0;

/* for debug only ,tx completion callback */
void tx_done_cb(void)
{
	pr_debug(MEDIA_NAME "<%s>\n", __func__);
}

/*
 * send_msg - send a TIPC message out over an SM interface
 */
static int send_msg(struct sk_buff *buf, struct tipc_bearer *tb_ptr,
		    struct tipc_media_addr *dest)
{
	struct shm_bearer *shmb_ptr = (struct shm_bearer *)tb_ptr->usr_handle;
	int ret;
	struct apm_icc_user_tx_msg tx_msg;
	u32 cpu = (*(u32 *) & (shmb_ptr->dev_addr));

	cpu = (1 >> cpu);
	if (buf->len <= ASYNC_SIZE) {
		tx_msg.user_data = buf->data;
		tx_msg.data_size = buf->len;
		tx_msg.msg_type = ICC_MSG_CMD;
		tx_msg.msg_sub_type = SM_SEND_TYPE;
		ret = apm_icc_send_cmd(cpu, &tx_msg);

	} else {
		ret =
		    apm_icc_send_data(cpu, SM_SEND_TYPE, buf->data, buf->len,
				      NULL);
	}
	return ret;
}

/*
 * Push incoming packet to TIPC 
*/
static void shm_rx_task(unsigned long data)
{
	struct tipc_bearer *tb_ptr = (struct tipc_bearer *)data;
	struct shm_bearer *shmb_ptr = (struct shm_bearer *)tb_ptr->usr_handle;
	struct sk_buff *skb;
	skb = skb_dequeue(&shmb_ptr->rxq);
	while (skb) {
		tipc_recv_msg(skb, shmb_ptr->bearer);
		skb = skb_dequeue(&shmb_ptr->rxq);
	}

}

/* IPC async callback  */
int shm_cmd_handler(struct apm_icc_user_cmd *msg)
{
	struct shm_bearer *shm_ptr = &shmb;
	struct sk_buff *skb;

	if (shm_ptr->bearer == NULL)
		return 1;
	if (msg->size) {
		skb = alloc_skb(msg->size, GFP_ATOMIC);
		if (!skb) {
			pr_err(MEDIA_NAME "skb alloc failed\n");
			return -ENOMEM;
		}
		memcpy(skb_put(skb, msg->size), msg->data, msg->size);
		skb_queue_tail(&shm_ptr->rxq, skb);
		tasklet_schedule(&shm_ptr->rx_task);
	}

	return 1;
}

/*
 * enable_bearer - attach TIPC bearer to an Shared Memory interface
 */
static int enable_bearer(struct tipc_bearer *tb_ptr)
{
	struct shm_bearer *shmb_ptr = &shmb;

	/* Associate TIPC bearer with Shared Memory bearer */
	shmb_ptr->bearer = tb_ptr;
	tb_ptr->usr_handle = (void *)shmb_ptr;
	tb_ptr->mtu = SHM_MTU;
	tb_ptr->blocked = 0;
	tb_ptr->addr.type = htonl(TIPC_MEDIA_TYPE_SHM);
	shmb_ptr->dev_addr = CORE_ID;
	memcpy(&tb_ptr->addr.dev_addr, &shmb_ptr->dev_addr, SHM_ALEN);
	tasklet_init(&shmb.rx_task, shm_rx_task, (unsigned long)tb_ptr);
	skb_queue_head_init(&shmb_ptr->rxq);

	return 0;
}

/*
 * disable_bearer - detach TIPC bearer from an Shared Memory interface
 */
static void disable_bearer(struct tipc_bearer *tb_ptr)
{
	struct shm_bearer *shmb_ptr = (struct shm_bearer *)tb_ptr->usr_handle;
	tb_ptr->blocked = 1;
	shmb_ptr->bearer = NULL;
	tasklet_kill(&shmb_ptr->rx_task);
	apm_icc_unregister_async_msg_handler();
	apm_icc_unregister_cmd_handler();
}

static char *shm_addr2str(struct tipc_media_addr *a,
			  char *str_buf, int str_size)
{
	u32 cpu;

	cpu = ntohl(*(u32 *) & (a->dev_addr));
	if (str_size < 18)
		*str_buf = '\0';
	else
		sprintf(str_buf, "%u", cpu);

	return str_buf;
}

void tipc_shm_media_suspend(void)
{
	pr_err("%s\n",__func__);
	if(!tipc_block_bearer(MEDIA_NAME))
		pr_err("%s shm Media Blocked \n",__func__);
}

void tipc_shm_media_resume(void)
{
	struct shm_bearer *shmb_ptr = &shmb;
	pr_err( "%s\n",__func__);
	tipc_continue(shmb_ptr->bearer);
}

/*
 * tipc_shm_media_start - activate Shared Memory bearer support
 *
 * Register Shared Memory media type with TIPC bearer code.  Also register
 * with OS for notifications about device state changes.
 */
int tipc_shm_media_start(void)
{
	struct tipc_media_addr bcast_addr;
	int res;

	if (shm_started)
		return -EINVAL;

	bcast_addr.type = htonl(TIPC_MEDIA_TYPE_SHM);
	/* broadcast Address 4 bytes setting bb for shm */
	memset(&bcast_addr.dev_addr, 0xbb, SHM_ALEN);
	memset(&shmb, 0, sizeof(shmb));

	res = tipc_register_media(TIPC_MEDIA_TYPE_SHM, MEDIA_NAME,
				  enable_bearer, disable_bearer, send_msg,
				  shm_addr2str, &bcast_addr, SHM_LINK_PRIORITY,
				  SHM_LINK_TOLERANCE, SHM_LINK_WINDOW);

	/* register callback */
	res = apm_icc_register_cmd_handler(shm_cmd_handler);
	if (res)
		pr_err("iapm_icc_register_cmd_handler failed err: %d\n", res);

	if (!res)
		shm_started = 1;

	apm_icc_register_suspend(tipc_shm_media_suspend);
	apm_icc_register_resume(tipc_shm_media_resume);

	return res;
}

/*
 * tipc_shm_media_stop - deactivate Shared Memory bearer support
 */
void tipc_shm_media_stop(void)
{
	if (!shm_started)
		return;
	skb_queue_purge(&shmb.rxq);
	shm_started = 0;
	memset(&shmb, 0, sizeof(shmb));
}
