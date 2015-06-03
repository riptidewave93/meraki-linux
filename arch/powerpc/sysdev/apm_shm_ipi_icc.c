/**
 * APM APM862xx Shared Memory based Intercore communication APIs
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Auther: Vivek Mahajan <vmahajan@apm.com>
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
 * This is the actual intercore communication implementation.
 */

#include <asm/apm_shm_icc.h>
#if defined(CONFIG_NOT_COHERENT_CACHE)
#include <asm/cacheflush.h>
#endif

#define PFX "APMICC: "

static icc_cmd_cb_func_t icc_user_cmd_handler = apm_icc_user_cmd_handler;
static icc_sync_msg_cb_func_t icc_sync_msg_handler =
    apm_icc_default_sync_msg_req_process;
static icc_msg_cb_func_t icc_msg_req_handler =
    apm_icc_default_async_msg_req_handler;

static struct apm_icc_priv *priv_data;
static struct apm_icc_data_tables icc_tbls;
struct kmem_cache *apm_icc_kcache;
int apm_shm_icc_probe_done = 0;

int apm_icc_init(void)
{
	char name[NAME_SZ];
	void *addr;
	int shm_num = 1;
	int ch = 0;
	int i, j;

	priv_data = kzalloc(sizeof(struct apm_icc_priv), GFP_KERNEL);
	if (!priv_data) {
		pr_err(PFX "apm_icc_priv alloc failed!!\n");
		return -ENOMEM;
	}

	priv_data->num_cores = NUM_CORES;
	priv_data->num_sets = (priv_data->num_cores *
			       (priv_data->num_cores - 1)) / 2;

	priv_data->cpu_id = mfspr(SPRN_PIR);
	priv_data->temp_size = MAX_FIFO_SIZE * 2;

	sprintf(name, "%s_%d", "ICC_SHMEM", shm_num++);
	addr = apm_shmem_alloc(name, sizeof(struct apm_icc_sync_msg_data) *
			       priv_data->num_cores);

	if (!addr) {
		pr_err(PFX "shmem alloc for sync msg data failed!\n");
		kfree(priv_data);
		/* NOTE: No support to release an allocated shared memory */
		return -ENOMEM;
	}

	priv_data->apm_icc_sync_msg_data = (struct apm_icc_sync_msg_data *)addr;

	sprintf(name, "%s_%d", "ICC_SHMEM", shm_num);
	addr = apm_shmem_alloc(name, sizeof(struct apm_icc_async_msg_ch));

	if (!addr) {
		pr_err(PFX "shmem alloc for async msg failed!!\n");
		kfree(priv_data);
		/* NOTE: No support to release an allocated shared memory */
		return -ENOMEM;
	}
	priv_data->apm_icc_async_msg_ch = (struct apm_icc_async_msg_ch *)addr;

	for (i = 0; i < priv_data->num_cores - 1; i++) {
		for (j = i + 1; j < priv_data->num_cores; j++) {
			icc_tbls.apm_icc_core_channel_table[j][i] = ch++;
			icc_tbls.apm_icc_core_channel_table[i][j] = ch;
		}
	}

	/* sequence number management buffer */
	priv_data->apm_icc_seq = kzalloc(sizeof(struct apm_icc_seq),
					 GFP_KERNEL);
	if (!priv_data->apm_icc_seq) {
		pr_err(PFX "sequence numbers alloc failed!!\n");
		kfree(priv_data);
		return -ENOMEM;
	}
#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats = kzalloc(sizeof(struct apm_icc_stats) *
					   priv_data->num_sets, GFP_KERNEL);
	if (!priv_data->apm_icc_stats) {
		pr_err(PFX "Couldn't allocate memory for icc_stats\n");
		kfree(priv_data->apm_icc_seq);
		kfree(priv_data);
		return -ENOMEM;
	}
#endif
	memset(icc_tbls.apm_icc_seq_cb_table, 0,
	       sizeof(icc_msg_resp_cb_func_t) * MAX_SEQ);
	memset(&priv_data->apm_icc_async_temp, 0,
	       sizeof(struct apm_icc_async_temp_data));

	for (i = 0; i < priv_data->num_sets; i++) {
		for (j = 0; j < MAX_SEQ; j++)
			priv_data->apm_icc_seq[i].seq_num[j] = j + 1;
		priv_data->apm_icc_seq[i].seq_total = MAX_SEQ;
	}
	for (ch = 0; ch < priv_data->num_cores; ch++) {
		priv_data->apm_icc_async_msg_ch[ch].init = 1;
		priv_data->apm_icc_sync_msg_data[ch].init = 1;
		atomic_set(&priv_data->apm_icc_sync_msg_data[ch].new_req, 0);
		atomic_set(&priv_data->apm_icc_sync_msg_data[ch].new_res, 0);
		priv_data->apm_icc_sync_msg_data[ch].req_front = 0;
		priv_data->apm_icc_sync_msg_data[ch].req_rear = 0;
		priv_data->apm_icc_sync_msg_data[ch].res_front = 0;
		priv_data->apm_icc_sync_msg_data[ch].res_rear = 0;
		priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].
		    icc_rear = 0;
		priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].
		    icc_front = 0;
		atomic_set(&
			   (priv_data->
			    apm_icc_async_msg_ch[ch].async_msg_data[ch].
			    icc_total), 0);
	}
	apm_icc_kcache =
	    kmem_cache_create("iccshm-bulk", PAGE_SIZE, ALIGN_BUF_SZ, 0, NULL);
	if (!apm_icc_kcache) {
		pr_err(PFX "Failed to create cache slab \n");
		return -ENOMEM;
	}
	return 0;
}

static inline int get_send_ch(int target_core)
{
	return icc_tbls.apm_icc_core_channel_table[priv_data->cpu_id]
	    [target_core];
}

static inline int get_rcv_ch(int target_core)
{
	return icc_tbls.apm_icc_core_channel_table[target_core]
	    [priv_data->cpu_id];
}

static int get_seq_num(int ch, int *seq_num)
{
	if (!priv_data->apm_icc_seq->seq_total) {
#if defined(ICC_STATS_ENABLE)
		priv_data->apm_icc_stats->ran_out_of_seq_error++;
#endif
		return -EINVAL;
	}

	priv_data->apm_icc_seq->seq_total--;
	*seq_num =
	    priv_data->apm_icc_seq->seq_num[priv_data->apm_icc_seq->seq_head];
	priv_data->apm_icc_seq->in_use[*seq_num] = 1;
	priv_data->apm_icc_seq->seq_head++;

	if (priv_data->apm_icc_seq->seq_head >= MAX_SEQ)
		priv_data->apm_icc_seq->seq_head = 0;

	return 0;
}

static int free_seq_num(int ch, int seq_num)
{
	if ((seq_num > MAX_SEQ) ||
	    (priv_data->apm_icc_seq->in_use[seq_num] == 0)) {
		pr_err(PFX "Wrong sequence number=%d to free\n", seq_num);
		return -EINVAL;
	}

	if (priv_data->apm_icc_seq->seq_total >= MAX_SEQ) {
		pr_err(PFX "Extra sequence num freeing attempted\n");
		return -EINVAL;
	}

	priv_data->apm_icc_seq->seq_total++;
	priv_data->apm_icc_seq->seq_num[priv_data->apm_icc_seq->seq_tail] =
	    seq_num;
	priv_data->apm_icc_seq->seq_tail++;

	if (priv_data->apm_icc_seq->seq_tail >= MAX_SEQ)
		priv_data->apm_icc_seq->seq_tail = 0;

	return 0;
}

int apm_icc_put_msg(struct apm_icc_user_tx_msg *user_msg, int *index)
{
	int buff;

	if (atomic_add_return(0, &(priv_data->apm_icc_async_temp.icc_total))
	    >= priv_data->temp_size) {
		pr_err(PFX "Queue size full\n");
		return -EINVAL;
	}
	/* copy message to rear of tx buffer */
	atomic_inc(&(priv_data->apm_icc_async_temp.icc_total));

	buff = priv_data->apm_icc_async_temp.icc_rear;
	memcpy(&priv_data->apm_icc_async_temp.apm_icc_msg[buff],
	       user_msg, sizeof(struct apm_icc_user_tx_msg));

	*index = buff;
	priv_data->apm_icc_async_temp.icc_rear++;

	return 0;
}

int apm_icc_get_msg(struct apm_icc_user_tx_msg *user_msg)
{
	int buff;

	if (!atomic_add_return(0, &(priv_data->apm_icc_async_temp.icc_total))) {
		pr_err(PFX "Queue size full\n");
		return -EINVAL;
	}
	buff = priv_data->apm_icc_async_temp.icc_front;
	memcpy(user_msg, &priv_data->apm_icc_async_temp.apm_icc_msg[buff],
	       sizeof(struct apm_icc_user_tx_msg));
	priv_data->apm_icc_async_temp.icc_front++;
	atomic_dec(&(priv_data->apm_icc_async_temp.icc_total));

	return 0;
}

int apm_wait_for_rx(int ch)
{
	volatile int index;
	int timeout =  ITER_CNT;

	do {
		pr_debug("WARN TX Crossed THRESHOLD %d for channel %d\n",
			TX_THRESHOLD, ch);
		udelay(1);
		index =
		    atomic_add_return(0,
				      &(priv_data->
					apm_icc_async_msg_ch[ch].async_msg_data
					[ch].icc_total));
		timeout--;
	} while (index >= (TX_THRESHOLD) && timeout);

	if (!timeout)
		return -1;
	return 0;
}

/*
 * send async msg.
 */
int apm_icc_send_message(int core, struct apm_icc_user_tx_msg *user_msg)
{
	int ch, buff;
	struct apm_icc_async_msg *icc_msg;

	ch = get_send_ch(core);
	if (user_msg->msg_type == ICC_MSG_AREQ)
		if (get_seq_num(ch, &user_msg->seq_num))
			return -EINVAL;

	if (atomic_add_return
	    (0,
	     &(priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].
	       icc_total)) >= TX_THRESHOLD) {
		if(apm_wait_for_rx(ch))
			return -ENOMEM;
	}

	/* copy message to rear of tx buffer */
	buff = priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].icc_rear;
	priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].icc_rear++;
	icc_msg =
	    &priv_data->apm_icc_async_msg_ch[ch].
	    async_msg_data[ch].apm_icc_msg[buff];

	icc_msg->msg_type = user_msg->msg_type;
	icc_msg->msg_sub_type = user_msg->msg_sub_type;
	icc_msg->data_size = user_msg->data_size;
	icc_msg->async_bulk_ptr = user_msg->tx_bulk_ptr;
	icc_msg->async_bulk_sz = user_msg->tx_bulk_sz;

	memcpy(icc_msg->user_data, user_msg->user_data, user_msg->data_size);

	/* copy seq num if not cmd & save call back pointer if req */
	if (user_msg->msg_type != ICC_MSG_CMD) {
		icc_msg->seq_num = user_msg->seq_num;
		if (user_msg->msg_type == ICC_MSG_AREQ)
			icc_tbls.apm_icc_seq_cb_table[user_msg->seq_num] =
			    user_msg->callback;
	}
	if (core == priv_data->cpu_id)
		icc_msg->flags |= ICC_FLAG_LB;

#if defined(ICC_STATS_ENABLE)
	if (core == priv_data->cpu_id)
		priv_data->apm_icc_stats[ch].async_rx_count++;

	priv_data->apm_icc_stats[ch].async_tx_count++;
#endif
	atomic_inc(&
		   (priv_data->apm_icc_async_msg_ch[ch].async_msg_data[ch].
		    icc_total));
	mpic_send_ipi(core, 1 << core);
	return 0;
}

/*
 * Get async msg.
 */
int apm_icc_get_message(int core, struct apm_icc_user_rx_msg *user_msg)
{
	int index;
	int ch, buff;
	struct apm_icc_async_msg *icc_msg;

	if (core == priv_data->cpu_id)
		return -EINVAL;

	ch = get_rcv_ch(core);
	index = ch;

	if (!atomic_add_return
	    (0,
	     &(priv_data->apm_icc_async_msg_ch[ch].async_msg_data[index].
	       icc_total))) {
#if defined(ICC_STATS_ENABLE)
		priv_data->apm_icc_stats[ch].error++;
#endif
		return -EINVAL;
	}
	buff =
	    priv_data->apm_icc_async_msg_ch[ch].async_msg_data[index].icc_front;
	icc_msg =
	    &priv_data->apm_icc_async_msg_ch[ch].async_msg_data[index].
	    apm_icc_msg[buff];

	user_msg->msg_type = icc_msg->msg_type;
	user_msg->msg_sub_type = icc_msg->msg_sub_type;
	user_msg->data_size = icc_msg->data_size;

	memcpy(user_msg->user_data, icc_msg->user_data, icc_msg->data_size);

	if (icc_msg->async_bulk_ptr) {
		user_msg->rx_bulk_ptr = icc_msg->async_bulk_ptr;
		user_msg->rx_bulk_sz = icc_msg->async_bulk_sz;
	} else {
		icc_msg->async_bulk_ptr = 0;
	}

	if (icc_msg->seq_num)
		user_msg->seq_num = icc_msg->seq_num;

	priv_data->apm_icc_async_msg_ch[ch].async_msg_data[index].icc_front++;

#if defined(ICC_STATS_ENABLE)
	if (!(icc_msg->flags & ICC_FLAG_LB))	/* not loopback */
		priv_data->apm_icc_stats[ch].async_rx_count++;
#endif
	atomic_dec(&
		   (priv_data->apm_icc_async_msg_ch[ch].async_msg_data[index].
		    icc_total));
	return 0;
}

inline int apm_icc_peek_async(int core)
{
	int index;
	int ch;

	if (core == priv_data->cpu_id)
		return -EINVAL;
	ch = get_rcv_ch(core);
	index = ch;
	index =
	    atomic_add_return(0,
			      &(priv_data->
				apm_icc_async_msg_ch[ch].async_msg_data[ch].
				icc_total));
	return index;
}

inline int apm_icc_peek_sync(int core)
{
	int ch;

	if (core == priv_data->cpu_id)
		return -EINVAL;
	ch = get_rcv_ch(core);
	return atomic_add_return(0,
				 &priv_data->apm_icc_sync_msg_data[ch].new_req);
}

int apm_icc_send_sync_message(int core, struct apm_icc_user_sync_msg *user_msg)
{
	int ch = get_send_ch(core);
	int cnt;

	if (user_msg->msg_type == ICC_MSG_SREQ) {
		cnt = priv_data->apm_icc_sync_msg_data[ch].req_front;
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		    msg_type = ICC_MSG_SREQ;
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		    msg_sub_type = user_msg->msg_sub_type;
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		    data_size = user_msg->data_size;
		memcpy(priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].user_data,
		       user_msg->user_data,
		       priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		       data_size);
		priv_data->apm_icc_sync_msg_data[ch].req_front++;
		atomic_inc(&priv_data->apm_icc_sync_msg_data[ch].new_req);
		mpic_send_ipi(core, 1 << core);
	} else if (user_msg->msg_type == ICC_MSG_SRSP) {
		cnt = priv_data->apm_icc_sync_msg_data[ch].res_front;
		/* write msg */
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    msg_type = ICC_MSG_SRSP;
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    msg_sub_type = user_msg->msg_sub_type;
		priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    data_size = user_msg->data_size;
		memcpy(priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].user_data,
		       user_msg->user_data, user_msg->data_size);
		priv_data->apm_icc_sync_msg_data[ch].res_front++;
		atomic_inc(&priv_data->apm_icc_sync_msg_data[ch].new_res);
	} else {
		return -EINVAL;
	}
	return 0;
}

int apm_icc_get_sync_message(int core, struct apm_icc_user_sync_msg *user_msg)
{
	unsigned int iter;
	int ch = get_rcv_ch(core);
	int cnt;
	volatile int new_res;

	if (user_msg->msg_type == ICC_MSG_SREQ) {
		if (!atomic_add_return(0,
				       &priv_data->apm_icc_sync_msg_data[ch].
				       new_req)) {
#if defined(ICC_STATS_ENABLE)
			priv_data->apm_icc_stats[ch].sync_error++;
#endif
			pr_err(PFX " Sync Err in apm_icc_get_sync_message\n");
			return -EINVAL;
		}
		cnt = priv_data->apm_icc_sync_msg_data[ch].req_rear;
		/* read & copy message and clear new_req */
		user_msg->msg_sub_type =
		    priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		    msg_sub_type;
		user_msg->data_size =
		    priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].
		    data_size;
		memcpy(user_msg->user_data,
		       priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_req[cnt].user_data,
		       user_msg->data_size);
		priv_data->apm_icc_sync_msg_data[ch].req_rear++;
		atomic_dec(&priv_data->apm_icc_sync_msg_data[ch].new_req);
	} else if (user_msg->msg_type == ICC_MSG_SRSP) {
		iter = 0;
		/*
		 * Poll until new_res is set
		 */
		new_res = atomic_add_return(0,
					    &priv_data->
					    apm_icc_sync_msg_data[ch].new_res);
		iter = 0;
		while ((!new_res) && (iter++ < ITER_CNT)) {
			new_res = atomic_add_return(0,
						    &priv_data->
						    apm_icc_sync_msg_data[ch].
						    new_res);
			udelay(1);
		}

		if (iter >= ITER_CNT) {
#if defined(ICC_STATS_ENABLE)
			priv_data->apm_icc_stats[ch].sync_timeout_error++;
#endif
			return -EINVAL;
		}
		cnt = priv_data->apm_icc_sync_msg_data[ch].res_rear;
		/* read & copy message & clear flags */
		user_msg->msg_type =
		    priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    msg_type;
		user_msg->msg_sub_type =
		    priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    msg_sub_type;
		user_msg->data_size =
		    priv_data->apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		    data_size;
		memcpy(user_msg->user_data,
		       priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].user_data,
		       priv_data->
		       apm_icc_sync_msg_data[ch].apm_icc_msg_res[cnt].
		       data_size);
		priv_data->apm_icc_sync_msg_data[ch].res_rear++;
		atomic_dec(&priv_data->apm_icc_sync_msg_data[ch].new_res);
	} else {
		return -EINVAL;
	}
	return 0;
}

/**
 * Post pktdma processing for async data tx.
 */
int apm_icc_async_data_post_proc(struct apm_pktdma_op_result *pktdma_result)
{
	int rc = 0;
	struct apm_icc_user_tx_msg *user_tx_msg;
	struct test_context *ctx;

	ctx = (struct test_context *)pktdma_result->ctx;
	user_tx_msg = &ctx->utx_msg;
	if ((!pktdma_result->err) && ctx->len) {
		rc = apm_icc_send_message(user_tx_msg->core, user_tx_msg);
		if (rc < 0)
			return rc;

		if (user_tx_msg->msg_type == ICC_MSG_CMD) {
#if defined(ICC_STATS_ENABLE)
			int ch = get_send_ch(user_tx_msg->core);
			priv_data->apm_icc_stats[ch].cmd_tx_count++;
#endif
		}
		/* notify user that data is copied */
		if (user_tx_msg->local)
			user_tx_msg->local(user_tx_msg->local_data_ptr);
	} else {
		pr_err(PFX "Spurious ICC PktDMA callback!!\n");
	}
	if (user_tx_msg->data_size)
		apm_icc_free_ptr(user_tx_msg->user_data);
	kfree(ctx);

	return rc;
}

int apm_icc_send_cmd(int core, struct apm_icc_user_tx_msg *tx_msg)
{
	int rc;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (tx_msg->data_size > ASYNC_SIZE) {
		pr_err(PFX "async sz %d exceeded!\n", tx_msg->data_size);
		return -EINVAL;
	}
	if ((rc = apm_icc_send_message(core, tx_msg)) < 0)
		return rc;

#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].cmd_tx_count++;
#endif
	return rc;
}

EXPORT_SYMBOL(apm_icc_send_cmd);

int apm_icc_send_data(int core, int type, void *bulk_data, int size,
		      void *tx_done_cb)
{
	struct apm_pktdma_m2m_params m2m_tx;
	int temp[SYNC_MSG_SIZE];
	int sz = sizeof(int);
	u64 sa = __pa(bulk_data);
	u64 da;
	u16 len = (u16) size;
	int rc;
	struct dma_chan_info info;
	struct test_context *ctx;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	ctx = (struct test_context *)
	    kzalloc(sizeof(struct test_context), GFP_ATOMIC);
	if (!ctx)
		return -1;

	if (size > (int) BULK_SIZE) {
		pr_err(PFX "Max bulk data size (0x%x) exceeded!\n", size);
		return -EINVAL;
	}
#ifdef CONFIG_NOT_COHERENT_CACHE
	flush_dcache_range((u32)bulk_data, (u32)bulk_data + size);
#endif

	/* get pointer via sync msg - this is 32 bit PA */
	temp[0] = size;

	rc = apm_icc_send_sync_msg(core, ICC_REQ_SEND_HPTR, (char *)temp, &sz);
	if (rc < 0) {
		pr_err(PFX "apm_icc_send_sync_msg fails!\n");
		return rc;
	}
	da = temp[0];
	apm_pktdma_get_total_chan(&info);
	m2m_tx.m2m_mode = IODMA_COPY;
	m2m_tx.sa = &sa;
	m2m_tx.da = &da;
	m2m_tx.byte_count = &len;
	m2m_tx.context = (void *)ctx;

	ctx->ptr = &m2m_tx;
	ctx->saddr = bulk_data;
	ctx->len = len;
	ctx->utx_msg.msg_type = ICC_MSG_CMD;
	ctx->utx_msg.msg_sub_type = type;
	ctx->utx_msg.data_size = 0;
	ctx->utx_msg.tx_bulk_sz = size;
	ctx->utx_msg.local = tx_done_cb;
	ctx->utx_msg.core = core;
	ctx->utx_msg.tx_bulk_ptr = temp[0];
	ctx->utx_msg.local_data_ptr = (int)bulk_data;

	m2m_tx.cb = apm_icc_async_data_post_proc;
	m2m_tx.chid = info.chan_en[0];
	rc = apm_pktdma_m2m_xfer(&m2m_tx);
	if (rc)
		pr_err(PFX "PktDMA mem-mem transfer fails!\n");

	return rc;
}

EXPORT_SYMBOL(apm_icc_send_data);

int apm_icc_send_async_msg_req(int core, int type, char *msg, int size,
			       void *callback)
{
	struct apm_icc_user_tx_msg user_tx_msg;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (size > ASYNC_SIZE) {
		pr_err(PFX "Max data size (0x%x) exceeded!\n", size);
		return -EINVAL;
	}
	memset(&user_tx_msg, 0, sizeof(struct apm_icc_user_tx_msg));
	user_tx_msg.msg_type = ICC_MSG_AREQ;
	user_tx_msg.msg_sub_type = type;
	user_tx_msg.data_size = size;
	user_tx_msg.user_data = msg;
	user_tx_msg.callback = callback;
	return apm_icc_send_message(core, &user_tx_msg);
}

EXPORT_SYMBOL(apm_icc_send_async_msg_req);

/**
 * request msg with data.
 */
int apm_icc_send_async_data_req(int core, struct apm_icc_user_tx_msg *tx_msg)
{
	struct apm_pktdma_m2m_params m2m_tx;
	int temp[2];
	int sz = sizeof(int);
	u64 sa = __pa(tx_msg->tx_bulk_ptr);
	u64 da;
	u16 len = (u16) tx_msg->tx_bulk_sz;
	int rc;
	struct test_context *ctx;
	struct dma_chan_info info;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	ctx = (struct test_context *)
	    kzalloc(sizeof(struct test_context), GFP_ATOMIC);
	if (!ctx)
		return -ENOMEM;
	if (tx_msg->data_size > ASYNC_SIZE) {
		pr_err(PFX "Max data size (0x%x) exceeded!\n",
		       tx_msg->data_size);
		return -EINVAL;
	}
	if (tx_msg->tx_bulk_sz > BULK_SIZE) {
		pr_err(PFX "Max bulk data size (0x%x) exceeded!\n", len);
		return -EINVAL;
	}
#ifdef CONFIG_NOT_COHERENT_CACHE
	flush_dcache_range((u32)tx_msg->tx_bulk_ptr, (u32)tx_msg->tx_bulk_ptr + len);
#endif

	apm_pktdma_get_total_chan(&info);
	/* get pointer via sync msg - this is 32 bit PA */
	temp[0] = len;
	rc = apm_icc_send_sync_msg(core, ICC_REQ_SEND_HPTR, (char *)temp, &sz);
	if (rc < 0)
		return rc;
	da = temp[0];
	ctx->utx_msg.msg_type = tx_msg->msg_type;
	ctx->utx_msg.msg_sub_type = tx_msg->msg_sub_type;
	ctx->utx_msg.data_size = tx_msg->data_size;
	if (tx_msg->data_size) {
		ctx->utx_msg.user_data =
		    kmem_cache_alloc(apm_icc_kcache, GFP_DMA);
		if (!ctx->utx_msg.user_data) {
			pr_err(PFX "%s Err in cache alloc\n", __func__);
			return -ENOMEM;
		}
		memcpy(ctx->utx_msg.user_data,
		       tx_msg->user_data, tx_msg->data_size);
	}
	ctx->utx_msg.tx_bulk_sz = tx_msg->tx_bulk_sz;
	ctx->utx_msg.callback = tx_msg->callback;
	ctx->utx_msg.local = tx_msg->local;
	ctx->utx_msg.core = core;
	ctx->utx_msg.tx_bulk_ptr = temp[0];
	ctx->utx_msg.local_data_ptr = tx_msg->tx_bulk_ptr;
	m2m_tx.m2m_mode = IODMA_COPY;
	m2m_tx.sa = &sa;
	m2m_tx.da = &da;
	m2m_tx.byte_count = &len;
	m2m_tx.context = (void *)ctx;
	m2m_tx.cb = apm_icc_async_data_post_proc;
	m2m_tx.chid = info.chan_en[0];
	ctx->ptr = &m2m_tx;
	ctx->saddr = (void *)tx_msg->tx_bulk_ptr;
	ctx->len = len;
	rc = apm_pktdma_m2m_xfer(&m2m_tx);
	if (rc)
		pr_err(PFX "PktDMA mem-mem transfer failed !!\n");

	return rc;
}

EXPORT_SYMBOL(apm_icc_send_async_data_req);

/**
 * Send async request msg.
 */
int apm_icc_send_async_msg_resp(int core, int type, int seq, char *msg,
				int size)
{
	struct apm_icc_user_tx_msg user_tx_msg;

	if (size > ASYNC_SIZE) {
		pr_err(PFX "Max data size (0x%x) exceeded!\n", size);
		return -EINVAL;
	}
	memset(&user_tx_msg, 0, sizeof(struct apm_icc_user_tx_msg));
	user_tx_msg.msg_type = ICC_MSG_ARSP;
	user_tx_msg.msg_sub_type = type;
	user_tx_msg.seq_num = seq;
	user_tx_msg.data_size = size;
	user_tx_msg.user_data = msg;

	return apm_icc_send_message(core, &user_tx_msg);
}

/**
 * Send async response msg with data.
 */
int apm_icc_send_async_data_resp(int core, int type, int seq, char *msg,
				 int size, int bulk_data_size, void *bulk_data,
				 void *tx_done_cb)
{
	struct apm_pktdma_m2m_params m2m_tx;
	struct apm_icc_user_tx_msg user_tx_msg;
	int temp[SYNC_MSG_SIZE];
	int sz = sizeof(int);
	u64 sa = __pa(bulk_data);
	u64 da;
	u16 len = (u16) bulk_data_size;
	int rc;
	struct test_context *ctx;
	struct dma_chan_info info;

	if (size > ASYNC_SIZE) {
		pr_err(PFX "Max data size (0x%x) exceeded!\n", size);
		return -EINVAL;
	}

	if (bulk_data_size > BULK_SIZE) {
		pr_err(PFX "Max bulk data size (0x%x) exceeded!\n",
		       bulk_data_size);
		return -EINVAL;
	}
	ctx = (struct test_context *)
	    kzalloc(sizeof(struct test_context), GFP_ATOMIC);
	if (!ctx)
		return -1;
#ifdef CONFIG_NOT_COHERENT_CACHE
	flush_dcache_range((u32)bulk_data, (u32)bulk_data + len);
#endif

	apm_pktdma_get_total_chan(&info);

	temp[0] = bulk_data_size;
	rc = apm_icc_send_sync_msg(core, ICC_REQ_SEND_HPTR, (char *)temp, &sz);
	if (rc < 0)
		return rc;

	da = temp[0];
	ctx->utx_msg.msg_type = ICC_MSG_ARSP;
	ctx->utx_msg.msg_sub_type = type;
	ctx->utx_msg.seq_num = seq;
	ctx->utx_msg.data_size = size;
	if (size) {
		ctx->utx_msg.user_data =
		    kmem_cache_alloc(apm_icc_kcache, GFP_DMA);
		if (!ctx->utx_msg.user_data) {
			pr_err(PFX "%s Err in cache alloc\n", __func__);
			return -ENOMEM;
		}
		memcpy(ctx->utx_msg.user_data, msg, size);
	}
	ctx->utx_msg.tx_bulk_sz = bulk_data_size;
	ctx->utx_msg.local = tx_done_cb;
	ctx->utx_msg.core = core;
	ctx->utx_msg.local_data_ptr = (int)bulk_data;
	user_tx_msg.tx_bulk_ptr = temp[0];
	m2m_tx.m2m_mode = IODMA_COPY;
	m2m_tx.sa = &sa;
	m2m_tx.da = &da;
	m2m_tx.byte_count = &len;
	m2m_tx.context = (void *)ctx;
	m2m_tx.chid = info.chan_en[0];
	m2m_tx.cb = apm_icc_async_data_post_proc;

	rc = apm_pktdma_m2m_xfer(&m2m_tx);
	if (rc)
		pr_err(PFX "PktDMA mem-mem transfer failed !!\n");

	return rc;
}

/**
 * Send sync request msg and get back the response.
 */
int apm_icc_send_sync_msg(int core, int type, char *msg, int *size)
{
	int rc;
	struct apm_icc_user_sync_msg sync_msg;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (*size > sizeof(struct apm_icc_info)) {
		pr_err(PFX "data sz %d exceeded !!\n", *size);
		return -EINVAL;
	}

	memset(&sync_msg, 0, sizeof(struct apm_icc_user_sync_msg));
	sync_msg.msg_type = ICC_MSG_SREQ;
	sync_msg.msg_sub_type = type;
	sync_msg.data_size = *size;
	memcpy(sync_msg.user_data, msg, sync_msg.data_size);
	rc = apm_icc_send_sync_message(core, &sync_msg);
	if (rc < 0) {
		pr_err(PFX "apm_icc_send_sync_message failed !\n");
		return rc;
	}
	sync_msg.msg_type = ICC_MSG_SRSP;
	rc = apm_icc_get_sync_message(core, &sync_msg);
	if (rc < 0) {
		pr_err(PFX "apm_icc_get_sync_message failed !!\n");
		return rc;
	}
	memcpy(msg, &sync_msg.user_data, sync_msg.data_size);
	*size = sync_msg.data_size;
#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].sync_tx_count++;
#endif
	return rc;
}

EXPORT_SYMBOL(apm_icc_send_sync_msg);

/**
 * Get sync request msg & send back response.
 */
int apm_icc_sync_msg_req_handler(int core, icc_sync_msg_cb_func_t msg_cb_func)
{
	int rc;
	struct apm_icc_user_sync_msg sync_msg;

	memset(&sync_msg, 0, sizeof(struct apm_icc_user_sync_msg));
	sync_msg.msg_type = ICC_MSG_SREQ;
	rc = apm_icc_get_sync_message(core, &sync_msg);
	if (rc < 0) {
		pr_err(PFX "apm_icc_get_sync_message fails!\n");
		return rc;
	}
	rc = msg_cb_func(core, &sync_msg);
	if (rc < 0) {
		pr_err(PFX "message callback failed!!\n");
		return rc;
	}
	sync_msg.msg_type = ICC_MSG_SRSP;
	rc = apm_icc_send_sync_message(core, &sync_msg);
	if (rc < 0) {
		pr_err(PFX "apm_icc_send_sync_message failed!!\n");
		return rc;
	}
#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].sync_rx_count++;
#endif
	return rc;
}

int apm_icc_register_cmd_handler(icc_cmd_cb_func_t cmd_cb_func)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (!cmd_cb_func) {
		pr_err(PFX "Invalid register cmd handler!!\n");
		return -EINVAL;
	}
	icc_user_cmd_handler = cmd_cb_func;
	return 0;
}

EXPORT_SYMBOL(apm_icc_register_cmd_handler);

void apm_icc_unregister_cmd_handler(void)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return;
	}

	icc_user_cmd_handler = apm_icc_user_cmd_handler;
}

EXPORT_SYMBOL(apm_icc_unregister_cmd_handler);

int apm_icc_register_sync_msg_handler(icc_sync_msg_cb_func_t msg_cb_func)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (!msg_cb_func) {
		pr_err(PFX "Invalid sync msg request handler!!\n");
		return -EINVAL;
	}
	icc_sync_msg_handler = msg_cb_func;
	return 0;
}

EXPORT_SYMBOL(apm_icc_register_sync_msg_handler);

void apm_icc_unregister_sync_msg_handler(void)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return;
	}

	icc_sync_msg_handler = apm_icc_default_sync_msg_req_process;
}

EXPORT_SYMBOL(apm_icc_unregister_sync_msg_handler);

int apm_icc_register_async_msg_handler(icc_msg_cb_func_t msg_cb_func)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (!msg_cb_func) {
		pr_err(PFX "Invalid async msg request handler!!\n");
		return -EINVAL;
	}
	icc_msg_req_handler = msg_cb_func;
	return 0;
}

EXPORT_SYMBOL(apm_icc_register_async_msg_handler);

void apm_icc_unregister_async_msg_handler(void)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return;
	}

	icc_msg_req_handler = apm_icc_default_async_msg_req_handler;
}

EXPORT_SYMBOL(apm_icc_unregister_async_msg_handler);

/*
 * Register sync request
 */
int apm_icc_register_sync_req(int sync_req, icc_user_cb_func_t callback)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	if (sync_req > ICC_REQ_SEND_FPTR &&
	    sync_req < NUM_TBLS + ICC_REQ_SEND_FPTR) {
		icc_tbls.sync_table[sync_req - ICC_REQ_SEND_FPTR]
		    = (int)callback;
		return 0;
	}
	pr_err(PFX "Invalid sync register req=%d\n", sync_req);
	return -EINVAL;
}

EXPORT_SYMBOL(apm_icc_register_sync_req);

/*
 * Unregister sync request
 */
void apm_icc_unregister_sync_req(int sync_req)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return;
	}

	if (sync_req > ICC_REQ_SEND_FPTR &&
	    sync_req < NUM_TBLS + ICC_REQ_SEND_FPTR) {
		memset(&icc_tbls.sync_table[sync_req - ICC_REQ_SEND_FPTR], 0,
		       sizeof(int));
	} else {
		pr_err(PFX "Invalid sync unregister req=%d!!\n", sync_req);
	}
}

EXPORT_SYMBOL(apm_icc_unregister_sync_req);

int apm_icc_cmd_handler(int core, int cmd, void *data, int size, int bulk)
{
	struct apm_icc_user_cmd cmd_ptr;
	struct apm_icc_async_msg *icc_msg;
	int buff;

#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].cmd_rx_count++;
#endif
	pr_debug(PFX "Received cmd=0x%x buf=%p sz=%x\n", cmd, data, size);
	buff =
	    priv_data->
	    apm_icc_async_msg_ch[get_send_ch(core)].async_msg_data[core].
	    icc_front;
	icc_msg =
	    &priv_data->
	    apm_icc_async_msg_ch[get_send_ch(core)].async_msg_data[core].
	    apm_icc_msg[buff];

	switch (cmd) {
	case ICC_CMD_TEST:
		pr_info(PFX "ICC_CMD_TEST\n");
		if (bulk) {
			icc_msg->async_bulk_ptr = 0;
			icc_msg->async_bulk_sz = 0;
			apm_icc_free_ptr(data);
		}
		break;
	case ICC_CMD_ALIVE:
		pr_info(PFX "ICC_CMD_ALIVE\n");
		break;
	default:
		if (!icc_user_cmd_handler) {
			pr_err(PFX "No user cmd handler found!!\n");
			return 1;
		}
		cmd_ptr.cmd = cmd;
		cmd_ptr.data = data;
		cmd_ptr.size = size;
		return icc_user_cmd_handler(&cmd_ptr);
	}
	return 0;
}

int apm_icc_default_sync_msg_req_process(int core,
					 struct apm_icc_user_sync_msg *msg_ptr)
{
	if (msg_ptr->msg_sub_type > ICC_REQ_SEND_FPTR &&
	    msg_ptr->msg_sub_type < NUM_TBLS + ICC_REQ_SEND_FPTR) {
		if (icc_tbls.sync_table[msg_ptr->msg_sub_type -
					ICC_REQ_SEND_FPTR]) {
			((icc_user_cb_func_t)
			 icc_tbls.sync_table[msg_ptr->msg_sub_type -
					     ICC_REQ_SEND_FPTR])
			    (msg_ptr->msg_sub_type, msg_ptr->user_data,
			     msg_ptr->data_size);
			return 0;
		}
	}
	msg_ptr->msg_sub_type = ICC_RESP_ERR;
	msg_ptr->user_data[0] = ICC_ERR_NO_APP;
	msg_ptr->data_size = 4;
#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].orphan_msg_count++;
#endif
	return 0;
}

int apm_icc_sync_msg_req_process(int core,
				 struct apm_icc_user_sync_msg *msg_ptr)
{
	int *user_data = (int *)msg_ptr->user_data;
	struct apm_icc_info icc_info;
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	unsigned int fptr = 0;
	int p0, p1, ch;
	void *ppp;

	if (msg_ptr->msg_sub_type <= ICC_REQ_SEND_FPTR) {
		switch (msg_ptr->msg_sub_type) {
		case ICC_REQ_TEST:
			pr_debug(PFX "SREQ = ICC_REQ_TEST\n");
			msg_ptr->msg_sub_type = ICC_RESP_OK;
			msg_ptr->user_data[0] = ICC_RESP_OK;
			msg_ptr->data_size = 4;
			break;
		case ICC_REQ_SEND_STATS:
			if (core == priv_data->cpu_id)
				return -EINVAL;
			pr_debug(PFX "SREQ = ICC_REQ_SEND_STATS\n");
			ch = get_send_ch(user_data[0]);
#if defined(ICC_STATS_ENABLE)
			memcpy(&icc_info.stats, &priv_data->apm_icc_stats[ch],
			       sizeof(struct apm_icc_stats));
#endif
			icc_info.available_seq =
			    priv_data->apm_icc_seq[ch].seq_total;
			p0 = priv_data->cpu_id;
			p1 = core;
			icc_info.async_tx_msg_pend =
				atomic_add_return(0,
					       &(priv_data->
						apm_icc_async_msg_ch
						[ch].async_msg_data
						[p0].
						icc_total));
			icc_info.async_rx_msg_pend =
			    atomic_add_return(0,
					      &(priv_data->
						apm_icc_async_msg_ch
						[ch].async_msg_data[p1].
						icc_total));
			icc_info.sync_req_flag =
			    atomic_add_return(0,
					      &priv_data->
					      apm_icc_sync_msg_data[ch].
					      new_req);
			icc_info.sync_resp_flag =
			    atomic_add_return(0,
					      &priv_data->
					      apm_icc_sync_msg_data[ch].
					      new_res);
			msg_ptr->msg_sub_type = ICC_RESP_STATS;
			memcpy(msg_ptr->user_data, &icc_info,
			       sizeof(struct apm_icc_info));
			msg_ptr->data_size = sizeof(struct apm_icc_info);
			pr_debug(PFX "Size:%d data 0x%x\n",
				 msg_ptr->data_size, user_data[0]);
			break;

		case ICC_REQ_SEND_HPTR:
			msg_ptr->msg_sub_type = ICC_RESP_HPTR;
			ppp = kmem_cache_alloc(apm_icc_kcache, GFP_DMA);
			if (!ppp) {
				pr_err(PFX "user_data alloc fails\n");
				return -EINVAL;
			}
			user_data[0] = virt_to_phys(ppp);
#ifdef CONFIG_NOT_COHERENT_CACHE
			invalidate_dcache_range((int)ppp, (int)ppp + PAGE_SIZE);
#endif
			msg_ptr->data_size = sizeof(int);
			break;

		case ICC_REQ_SEND_FPTR:
			msg_ptr->msg_sub_type = ICC_RESP_FPTR;
			msg.BufDataLen = (5 << 12);	/* 2K */
			msg.FPQNum = user_data[0];
			msg.DataAddrMSB = 0;
			msg.UserInfo = (u32) (&fptr);
			msg.DataAddrLSB = (u32) __pa(&fptr);
			fp_msg_desc.qid = user_data[0];
			fp_msg_desc.mb_id = user_data[0];
			fp_msg_desc.msg = &msg;
			if (apm_qm_fp_dealloc_buf(&fp_msg_desc)) {
				pr_err(PFX "Can't fill buf pool!\n");
				user_data[0] = 0;
				return -EINVAL;
			}
			user_data[0] = msg.DataAddrLSB;
			msg_ptr->data_size = sizeof(int);
			break;
		}
	} else {
		icc_sync_msg_handler(core, msg_ptr);
	}

	return 0;
}

void apm_icc_free_ptr(void *ptr)
{
	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return;
	}

	kmem_cache_free(apm_icc_kcache, ptr);
}

EXPORT_SYMBOL(apm_icc_free_ptr);
inline void apm_icc_get_sync_msg(int core)
{
	if (apm_icc_peek_sync(core))
		apm_icc_sync_msg_req_handler(core,
					     &apm_icc_sync_msg_req_process);
}

inline void apm_icc_send_resp(int core, int seq, int error)
{
	char buf[sizeof(int)] = { error };
	apm_icc_send_async_msg_resp(core, error, seq, buf, sizeof(int));
}

/**
 * used when user does not register any other one.
 */
void apm_icc_default_async_msg_req_handler(int core,
					   struct apm_icc_user_rx_msg *req_msg)
{
	int error = ICC_ERR_NO_APP;
	char *data;

#if defined(ICC_STATS_ENABLE)
	priv_data->apm_icc_stats[get_send_ch(core)].orphan_msg_count++;
#endif
	pr_info(PFX "%s bsz %d mtype %d sty %d\n", __func__,
		req_msg->rx_bulk_sz, req_msg->msg_type, req_msg->msg_sub_type);

	if (req_msg->msg_sub_type < NUM_TBLS) {
		if (icc_tbls.async_table[req_msg->msg_sub_type]) {
			((icc_user_msg_cb_func_t)
			 icc_tbls.async_table[req_msg->msg_sub_type])
			    (req_msg->msg_sub_type, req_msg->seq_num,
			     req_msg->user_data, req_msg->data_size,
			     (void *)req_msg->rx_bulk_ptr, 
			     	req_msg->rx_bulk_sz);
			return;
		}
	} else {
		error = ICC_ERR_WRONG_ID;
	}

	if (req_msg->rx_bulk_sz) {
		data = (char *)__va(req_msg->rx_bulk_ptr);
		if (data)
			apm_icc_free_ptr(data);
	}

	apm_icc_send_resp(core, req_msg->seq_num, error);
}

int apm_icc_get_async_msg(int core)
{
	struct apm_icc_user_rx_msg act_msg;
	struct apm_icc_user_rx_msg_resp user_msg_resp;
	int ch;
	int rc = 0;

	if (unlikely(!apm_shm_icc_probe_done)) {
		pr_err(PFX "%s: APM SHM ICC not initialize\n", __func__);
		return -ENODEV;
	}

	ch = get_send_ch(core);

	while (apm_icc_peek_async(core)) {

		memset(&act_msg, 0, sizeof(struct apm_icc_user_rx_msg));

		act_msg.user_data =
		    kmem_cache_alloc(apm_icc_kcache, GFP_ATOMIC);
		if (!act_msg.user_data) {
			pr_err(PFX "<%s> kmem_cache_alloc failed\n", __func__);
			return -ENOMEM;
		}
		rc = apm_icc_get_message(core, &act_msg);
		if (rc < 0)
			goto fail;
		switch (act_msg.msg_type) {
		case ICC_MSG_CMD:
			if (act_msg.data_size) {
				apm_icc_cmd_handler(core, act_msg.msg_sub_type,
						    act_msg.user_data,
						    act_msg.data_size, NON_BULK);
			} else {

#ifdef CONFIG_NOT_COHERENT_CACHE
				invalidate_dcache_range((u32)
					(u32) __va(act_msg.rx_bulk_ptr),
					(u32) __va(act_msg.rx_bulk_ptr)
						+ act_msg.rx_bulk_sz);
#endif

				if (apm_icc_cmd_handler(core,
					act_msg.msg_sub_type,
					(int *)__va(act_msg.
						    rx_bulk_ptr),
					act_msg.rx_bulk_sz,
					BULK) == 1) {
					apm_icc_free_ptr(__va
							 (act_msg.rx_bulk_ptr));
				}
			}
			break;

		case ICC_MSG_AREQ:
			if (act_msg.rx_bulk_ptr) {
#ifdef CONFIG_NOT_COHERENT_CACHE
				invalidate_dcache_range((int)
					(int) __va(act_msg.rx_bulk_ptr),
					(int) __va(act_msg.rx_bulk_ptr)
						+ act_msg.rx_bulk_sz);
#endif
				icc_msg_req_handler(core, &act_msg);
			}
			break;

		case ICC_MSG_ARSP:
			memset(&user_msg_resp, 0,
			       sizeof(struct apm_icc_user_rx_msg_resp));
			user_msg_resp.user_data =
			    kmem_cache_alloc(apm_icc_kcache, GFP_ATOMIC);
			if (!user_msg_resp.user_data) {
				pr_err(PFX "<%s> kmem_cache_alloc failed\n",
				       __func__);
				return -ENOMEM;
			}

			rc = free_seq_num(ch, act_msg.seq_num);
			if (rc < 0) {
#if defined(ICC_STATS_ENABLE)
				priv_data->apm_icc_stats[ch].error++;
#endif
				apm_icc_free_ptr(user_msg_resp.user_data);
				return rc;
			}

			memcpy(user_msg_resp.user_data, act_msg.user_data,
			       act_msg.data_size);
			user_msg_resp.data_size = act_msg.data_size;
			user_msg_resp.rx_rsp_bulk_ptr =
			    (int)phys_to_virt(act_msg.rx_bulk_ptr);
			user_msg_resp.rx_rsp_bulk_sz = act_msg.rx_bulk_sz;

			if (icc_tbls.apm_icc_seq_cb_table[act_msg.seq_num]) {
				icc_tbls.apm_icc_seq_cb_table[act_msg.seq_num]
				    (&user_msg_resp);
			} else
				apm_icc_free_ptr(phys_to_virt
						 (act_msg.rx_bulk_ptr));

			apm_icc_free_ptr(user_msg_resp.user_data);
			break;
		default:
			pr_err(PFX "Received unknown async message !!\n");
#if defined(ICC_STATS_ENABLE)
			priv_data->apm_icc_stats[ch].unknown_msg_count++;
#endif
			apm_icc_send_resp(core, act_msg.seq_num, ICC_ERR);
			rc = -EINVAL;
			goto fail;
		}
		apm_icc_free_ptr(act_msg.user_data);
		apm_icc_get_sync_msg(core);
	}
	return 0;

fail:
	apm_icc_free_ptr(act_msg.user_data);
	return rc;
}

EXPORT_SYMBOL(apm_icc_get_async_msg);

void apm_icc_msg_interrupt(void *dev)
{
	apm_icc_get_sync_msg((int)dev);
	apm_icc_get_async_msg((int)dev);
}

int apm_icc_user_cmd_handler(struct apm_icc_user_cmd *cmd_ptr)
{
	if (cmd_ptr->cmd > ICC_CMD_ALIVE
	    && cmd_ptr->cmd < NUM_TBLS + ICC_CMD_ALIVE) {
		if (icc_tbls.cmd_table[cmd_ptr->cmd - ICC_CMD_ALIVE]) {
			((icc_user_cb_func_t)
			 icc_tbls.cmd_table[cmd_ptr->cmd - ICC_CMD_ALIVE])
			    (cmd_ptr->cmd, cmd_ptr->data, cmd_ptr->size);
			return 1;
		}
	}
	pr_debug(PFX "Unsupported cmd=0x%x\n", cmd_ptr->cmd);
	return 1;
}

#if defined(ICC_STATS_ENABLE)
void apm_icc_get_status(int core, struct apm_icc_info *icc_info)
{
	int i, p0, p1, ch;
	int size = sizeof(int);

	/* copy info based upon number of cores */
	if (core == priv_data->cpu_id) {
		for (i = 0; i < priv_data->num_cores; i++) {
			if (i == priv_data->cpu_id)
				continue;

			ch = get_send_ch(i);
			memcpy(&icc_info->stats, &priv_data->apm_icc_stats[ch],
			       sizeof(struct apm_icc_stats));
			icc_info->available_seq =
			    priv_data->apm_icc_seq[ch].seq_total;

			p0 = priv_data->cpu_id;
			p1 = i;

			icc_info->async_tx_msg_pend =
			    atomic_add_return(0,
					      &(priv_data->apm_icc_async_msg_ch
						[ch].async_msg_data[p0].
						icc_total));
			icc_info->async_rx_msg_pend =
			    atomic_add_return(0,
					      &(priv_data->apm_icc_async_msg_ch
						[ch].async_msg_data[p1].
						icc_total));
			icc_info->sync_req_flag =
			    atomic_add_return(0,
					      &priv_data->
					      apm_icc_sync_msg_data[ch].
					      new_req);
			icc_info->sync_resp_flag =
			    atomic_add_return(0,
					      &priv_data->
					      apm_icc_sync_msg_data[ch].
					      new_res);
			icc_info++;
		}
	} else {
		/* get stats via sync msg */
		for (i = 0; i < priv_data->num_cores; i++) {
			if (i == priv_data->cpu_id)
				continue;

			icc_info->stats.cmd_tx_count = i;
			apm_icc_send_sync_msg(core, ICC_REQ_SEND_STATS,
					      (char *)icc_info, &size);
			icc_info++;
		}
	}
}

void apm_icc_print_local_info(void)
{
	int i, j = 0;
	struct apm_icc_info icc_info[NUM_CORES - 1];

	apm_icc_get_status(priv_data->cpu_id, icc_info);

	pr_err("\nCore %d ICC info:\n", priv_data->cpu_id);
	pr_err("================\n");

	for (i = 0; i < NUM_CORES; i++) {
		if (i == priv_data->cpu_id)
			continue;

		pr_err("[To Core: %d]\n", i);
		pr_err("Pending async messages: Tx= %d, Rx= %d\n",
		       icc_info[j].async_tx_msg_pend,
		       icc_info[j].async_rx_msg_pend);
		pr_err("Available sequences   : %d\n",
		       icc_info[j].available_seq);
		pr_err("Sync flags            : Req= %d, Resp= %d\n",
		       icc_info[j].sync_req_flag, icc_info[j].sync_resp_flag);
		pr_err("Command tx count      : %d\n",
		       icc_info[j].stats.cmd_tx_count);
		pr_err("Command rx count      : %d\n",
		       icc_info[j].stats.cmd_rx_count);
		pr_err("Sync tx count         : %d\n",
		       icc_info[j].stats.sync_tx_count);
		pr_err("Sync rx count         : %d\n",
		       icc_info[j].stats.sync_rx_count);
		pr_err("Async tx count        : %d\n",
		       icc_info[j].stats.async_tx_count);
		pr_err("Async rx count        : %d\n",
		       icc_info[j].stats.async_rx_count);
		pr_err("Unknown msg count     : %d\n",
		       icc_info[j].stats.unknown_msg_count);
		pr_err("Orphan msg count      : %d\n",
		       icc_info[j].stats.orphan_msg_count);
		pr_err("Tx ran out of seq nums: %d\n",
		       icc_info[j].stats.ran_out_of_seq_error);
		pr_err("Tx ran out of buffers : %d\n",
		       icc_info[j].stats.ran_out_of_buffer_error);
		pr_err("Sync timeout errors   : %d\n",
		       icc_info[j].stats.sync_timeout_error);
		pr_err("Sync protocol errors  : %d\n",
		       icc_info[j].stats.sync_error);
		pr_err("Generic error         : %d\n\n",
		       icc_info[j].stats.error);
		j++;
	}
}

void apm_icc_print_remote_info(int core)
{
	int i, j = 0;
	struct apm_icc_info icc_info[NUM_CORES - 1];

	apm_icc_get_status(core, icc_info);

	pr_err("\nCore %d ICC info:\n", core);
	pr_err("================\n");

	for (i = 0; i < NUM_CORES; i++) {
		if (i == core)
			continue;

		pr_err("[To Core: %d]\n", i);
		pr_err("Pending async messages: Tx= %d, Rx= %d\n",
		       icc_info[j].async_tx_msg_pend,
		       icc_info[j].async_rx_msg_pend);
		pr_err("Available sequences   : %d\n",
		       icc_info[j].available_seq);
		pr_err("Sync flags            : Req= %d, Resp= %d\n",
		       icc_info[j].sync_req_flag, icc_info[j].sync_resp_flag);
		pr_err("Command tx count      : %d\n",
		       icc_info[j].stats.cmd_tx_count);
		pr_err("Command rx count      : %d\n",
		       icc_info[j].stats.cmd_rx_count);
		pr_err("Sync tx count         : %d\n",
		       icc_info[j].stats.sync_tx_count);
		pr_err("Sync rx count         : %d\n",
		       icc_info[j].stats.sync_rx_count);
		pr_err("Async tx count        : %d\n",
		       icc_info[j].stats.async_tx_count);
		pr_err("Async rx count        : %d\n",
		       icc_info[j].stats.async_rx_count);
		pr_err("Unknown msg count     : %d\n",
		       icc_info[j].stats.unknown_msg_count);
		pr_err("Orphan msg count      : %d\n",
		       icc_info[j].stats.orphan_msg_count);
		pr_err("Tx ran out of seq nums: %d\n",
		       icc_info[j].stats.ran_out_of_seq_error);
		pr_err("Tx ran out of buffers : %d\n",
		       icc_info[j].stats.ran_out_of_buffer_error);
		pr_err("Sync timeout errors   : %d\n",
		       icc_info[j].stats.sync_timeout_error);
		pr_err("Sync protocol errors  : %d\n",
		       icc_info[j].stats.sync_error);
		pr_err("Generic error         : %d\n\n",
		       icc_info[j].stats.error);
		j++;
	}
}
#endif
