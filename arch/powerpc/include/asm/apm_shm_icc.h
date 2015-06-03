/**
 * APM AM86xxx Shared Memory Manager and IPI based intercore
 * communication headers
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Author: Vivek Mahajan <vmahajan@apm.com>
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

#ifndef __APM_SHM_ICC_H__
#define __APM_SHM_ICC_H__

#include <asm/mpic.h>
#include <asm/apm_shm.h>
#include <asm/apm_qm_core.h>
#include <asm/apm_pktdma.h>

/* configurable items */

#define ICC_STATS_ENABLE
/* NB: APM_QM_FPID will go away, when QM adds exports API to add queues */
#define APM_QM_FPID	7
#define NUM_CORES	2		/* FIXME - This need to be a run-time parameters */	
#define MAX_FIFO_SIZE	256		/* FIFO entries */
#define MAX_SEQ		1024 		/* unique identifiers/seq-nums/async */
#define ITER_CNT	0x55555
#define ASYNC_SIZE	1500		/* Max user bytes in an async msg */
#define MIN_BULK_SIZE	32
#define BULK_SIZE	65535
#define NUM_TBLS	128		/* Max user callback registries */
#define SYNC_MSG_SIZE	2
#define ICC_FLAG_LB	1
#define PG_ODR		0
#define TX_THRESHOLD	(MAX_FIFO_SIZE - 10)
#define ALIGN_BUF_SZ	512
#define NON_BULK	0
#define BULK		1

enum wrt_echo_codes {
	ICC_CMD_TX = 1,
	ICC_SYNC_MSG_TX_REQ,
	ICC_ASYNC_MSG_TX,
	ICC_ASYNC_MSG_RX,
	ICC_SYNC_MSG_TX_HPTR,
	ICC_SYNC_MSG_TX_FPTR,
	ICC_SYNC_DATA_TX,
	ICC_ASYNC_DATA_TX,
	ICC_LCL_STATS,
	ICC_RMT_STATS,
};

#if defined(ICC_STATS_ENABLE)
struct apm_icc_stats {
	int cmd_tx_count;
	int cmd_rx_count;
	int sync_tx_count;
	int sync_rx_count;
	int async_tx_count;
	int async_rx_count;
	int unknown_msg_count;
	int orphan_msg_count;
	int ran_out_of_seq_error;
	int ran_out_of_buffer_error;
	int sync_timeout_error;
	int sync_error;
	int error;
};
#endif

struct apm_icc_info {
#if defined(ICC_STATS_ENABLE)
	struct apm_icc_stats stats;
#endif
	int available_seq;
	int async_tx_msg_pend;
	int async_rx_msg_pend;
	int sync_req_flag;
	int sync_resp_flag;
	void *callback;
};

enum apm_icc_msg_type {
	ICC_MSG_CMD = 1,
	ICC_MSG_AREQ,   /**< Async Request*/
	ICC_MSG_ARSP,   /**< Async Response */
	ICC_MSG_SREQ,   /**< Sync Request */
	ICC_MSG_SRSP,   /**< Sync Response */
};

enum apm_icc_cmd_type {
	ICC_CMD_TEST = 1,
	ICC_CMD_ALIVE,  /**< Heartbeat command */
};

enum apm_icc_req_type {
	ICC_REQ_TEST = 1,
	ICC_REQ_SEND_STATS,
	ICC_REQ_SEND_HPTR,  /**< Heap pointer request (expects memory size in
				 the first word of message) */
	ICC_REQ_SEND_FPTR,  /**< FP pointer request (expects free pool id in
				 the first word of message */
};

enum apm_icc_resp_type {
	ICC_RESP_OK = 1,
	ICC_RESP_STATS,
	ICC_RESP_HPTR,  /**< Heap pointer response */
	ICC_RESP_FPTR,  /**< FP pointer response */
	ICC_RESP_ERR,
};

enum apm_icc_resp_err {
	ICC_ERR = 1,
	ICC_ERR_NO_SEQ,
	ICC_ERR_NO_MEM,
	ICC_ERR_NO_APP, 	/* no application handler for the request */
	ICC_ERR_WRONG_ID, 	/* wrong msg identifier */
};

/* format of physical async data */
struct apm_icc_async_msg {
	int msg_type:4;
	int msg_sub_type:26;
	int seq_num;
	char user_data[ASYNC_SIZE];
	int data_size;
	int flags:16; /* valid[0], lb[1] */
	int async_bulk_sz:16;
	int async_bulk_ptr;
};

struct apm_icc_async_msg_data {
	atomic_t icc_total;
	char icc_front;
	char icc_rear;
	struct apm_icc_async_msg apm_icc_msg[MAX_FIFO_SIZE];
};

struct apm_icc_async_msg_ch {
	int init;
	struct apm_icc_async_msg_data async_msg_data[NUM_CORES];
};

struct apm_icc_sync_msg {
	int msg_type:4;
	int msg_sub_type:26;
	char user_data[sizeof(struct apm_icc_info)];
	int data_size;
};

struct apm_icc_sync_msg_data {
	int init;
	atomic_t new_req;
	char req_front;
	char req_rear;
	atomic_t new_res;
	char res_front;
	char res_rear;
	struct apm_icc_sync_msg apm_icc_msg_req[MAX_FIFO_SIZE];
	struct apm_icc_sync_msg apm_icc_msg_res[MAX_FIFO_SIZE];
};

/* seqnum control & data */
struct apm_icc_seq {
	int seq_total;
	int seq_head;
	int seq_tail;
	int seq_num[MAX_SEQ];
	char in_use[MAX_SEQ];
};

typedef void(*lcb) (int);

/* format of user msg */
struct apm_icc_user_tx_msg {
	int core;
	int msg_type; /* redundant for ICC_MSG_ARSP */
	int msg_sub_type;
	int seq_num; /* redundant for ICC_MSG_ARSP */
	char *user_data;
	int data_size;
	int tx_bulk_ptr;
	int tx_bulk_sz;
	void *callback;
	lcb local;
	int local_data_ptr;
};

/* temp control & data format */
struct apm_icc_async_temp_data {
	atomic_t icc_total;
	char icc_front;
	char icc_rear;
	struct apm_icc_user_tx_msg apm_icc_msg[(MAX_FIFO_SIZE *
						NUM_CORES) * 2];
};

/* response message for users */
struct apm_icc_user_rx_msg_resp {
	int msg_sub_type;                   
	char *user_data; 
				/**< User data within msg (size is interpreted 
				     from response type) */
	int data_size;                  
	int rx_rsp_bulk_ptr;                  
	int rx_rsp_bulk_sz;                 
};

struct apm_icc_priv {
	int num_cores;
	int num_sets;
	int cpu_id;
	int temp_size;
	struct apm_icc_sync_msg_data *apm_icc_sync_msg_data;
	struct apm_icc_async_msg_ch *apm_icc_async_msg_ch;
	struct apm_icc_async_temp_data apm_icc_async_temp;
	struct apm_icc_seq *apm_icc_seq;
#if defined(ICC_STATS_ENABLE)
	struct apm_icc_stats *apm_icc_stats;
#endif
};

typedef int (*icc_msg_resp_cb_func_t) (struct apm_icc_user_rx_msg_resp *);

struct apm_icc_data_tables {
	int sync_table[NUM_TBLS];
	int async_table[NUM_TBLS];
	int cmd_table[NUM_TBLS];
	int apm_icc_core_channel_table[NUM_CORES][NUM_CORES];
	icc_msg_resp_cb_func_t apm_icc_seq_cb_table[MAX_SEQ]; 
					/* async callback table per seq id */
};

/* user async message request */
struct apm_icc_user_rx_msg {
	int msg_type;		/**< Msg type (ICC_MSG_CMD or ICC_MSG_AREQ) */
	int msg_sub_type;       /**< Request type, user defined */
	int seq_num;            /**< Unique identifier */
	char *user_data; /**< User data within message */
	int data_size;                      
	int rx_bulk_ptr;                  
	int rx_bulk_sz;                 
};

struct apm_icc_user_sync_msg {
	int msg_type;                      
	int msg_sub_type;                  
	char user_data[sizeof(struct apm_icc_info)]; 
				/**< User data within message */
	int data_size;                     
};

struct apm_icc_user_cmd {
	int cmd;   
	char *data; 
	int size;  
};
struct test_context {
	void *ptr;
	void *saddr;
	void *daddr;
	u32 len;
	struct apm_icc_user_tx_msg utx_msg;
};

typedef int (*icc_cmd_cb_func_t) (struct apm_icc_user_cmd *);
typedef int (*icc_user_cb_func_t) (int mtype, char *data, int size);
typedef void (*icc_msg_cb_func_t) (int, struct apm_icc_user_rx_msg *);
typedef int (*icc_sync_msg_cb_func_t) (int, struct apm_icc_user_sync_msg *);
typedef int (*icc_user_msg_cb_func_t) (int mtype, int seq_num, char *data,
					int size, void *bulk, int bsize);
typedef void (*icc_suspend_cb_func_t) (void);
typedef void  (*icc_resume_cb_func_t) (void);
extern int apm_shm_icc_probe_done;
int apm_icc_init(void);
int apm_icc_get_async_msg(int core);
int apm_icc_register_cmd_handler(icc_cmd_cb_func_t);
int apm_icc_register_sync_msg_handler(icc_sync_msg_cb_func_t);
int apm_icc_register_async_msg_handler(icc_msg_cb_func_t);
int apm_icc_register_sync_req(int sync_req, icc_user_cb_func_t);
void apm_icc_unregister_cmd_handler(void);
void apm_icc_unregister_sync_msg_handler(void);
void apm_icc_unregister_async_msg_handler(void);
int apm_icc_user_cmd_handler(struct apm_icc_user_cmd *);
int apm_icc_send_cmd(int core, struct apm_icc_user_tx_msg * );
int apm_icc_send_sync_msg(int core, int type, char *msg, int *size);
int apm_icc_default_sync_msg_req_process(int, struct apm_icc_user_sync_msg *);
int apm_icc_send_async_msg_resp(int core, int type, int seq, char *msg, 
				int size);
int apm_icc_send_data(int core, int type, void *bulk_data, int size,
			void *tx_done_cb);
int apm_icc_send_async_msg_req(int core, int type, char *msg, int size,
				void *resp_callback);
int apm_icc_send_async_data_req(int core,  struct apm_icc_user_tx_msg *);
int apm_icc_send_async_data_resp(int core, int type, int seq, char *msg, 
				int size, int bulk_data_size, void *bulk_data,
			     	void *tx_done_cb);
void apm_icc_unregister_sync_req(int);
void apm_icc_get_sync_msg(int core);
void apm_icc_msg_interrupt(void *);
void apm_icc_print_local_info(void);
void apm_icc_print_remote_info(int);
void apm_icc_default_async_msg_req_handler(int, struct apm_icc_user_rx_msg *);
void apm_icc_free_ptr(void *ptr);
int apm_icc_register_suspend(icc_suspend_cb_func_t suspend_cb_func);
int apm_icc_register_resume(icc_resume_cb_func_t resume_cb_func);

#endif /* __APM_SHM_ICC_H__ */
