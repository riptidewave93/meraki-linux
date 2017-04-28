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

#ifndef	 __APM_ENET_QM_CFG_H__
#define	 __APM_ENET_QM_CFG_H__

#include <asm/apm_qm_cfg.h>

/* Queues related parameters per Enet port */
#define ENET_MAX_PBN	8
#define ENET_MAX_QSEL	8

struct eth_qids {
	u16 qtype;
	u16 qid;
	u16 pbn;
	u16 mbox;
	u16 arb; 
	u16 qcount;
	u16 qsel[ENET_MAX_QSEL];
};

struct eth_queue_ids {
	u16 default_tx_qid;
	u16 tx_count;
	u16 tx_idx;
	struct eth_qids tx[ENET_MAX_PBN];
	u16 default_rx_qid;
	u16 rx_count;
	u16 rx_idx;
	struct eth_qids rx[ENET_MAX_PBN];
	u16 rx_fp_qid;
	u16 rx_fp_mbid;
	u16 rx_fp_pbn;
	u16 hw_fp_qid;
	u16 hw_fp_pbn;
	u16 comp_qid;		/* completion q id    */
	u16 comp_mb;		/* completion mailbox */
	u32 comp_cnt;		/* Polling count to check for completion msg */
};

int apm_enet_qm_cfg_register(unsigned long priv_data, int port);

int apm_enet_add_qconfig(unsigned long priv_data,
			 struct qm_cfg_qconfig *qconfig);

#endif	/* __APM_ENET_QM_CFG_H__ */
