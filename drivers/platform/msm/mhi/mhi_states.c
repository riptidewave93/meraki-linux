/* Copyright (c) 2014-2016, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "mhi_sys.h"
#include "mhi_hwio.h"
#include "mhi_trace.h"

#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

const char *state_transition_str(enum STATE_TRANSITION state)
{
	static const char * const
		mhi_states_transition_str[STATE_TRANSITION_MAX] = {
		[STATE_TRANSITION_RESET] = "RESET",
		[STATE_TRANSITION_READY] = "READY",
		[STATE_TRANSITION_M0] = "M0",
		[STATE_TRANSITION_M1] = "M1",
		[STATE_TRANSITION_M2] = "M2",
		[STATE_TRANSITION_M3] = "M3",
		[STATE_TRANSITION_BHI] = "BHI",
		[STATE_TRANSITION_SBL] = "SBL",
		[STATE_TRANSITION_AMSS] = "AMSS",
		[STATE_TRANSITION_LINK_DOWN] = "LINK_DOWN",
		[STATE_TRANSITION_WAKE] = "WAKE",
		[STATE_TRANSITION_BHIE] = "BHIE",
		[STATE_TRANSITION_SYS_ERR] = "SYS_ERR",
	};

	return (state < STATE_TRANSITION_MAX) ?
		mhi_states_transition_str[state] : "Invalid";
}

enum MHI_STATE mhi_get_m_state(struct mhi_device_ctxt *mhi_dev_ctxt)
{
	u32 state = mhi_reg_read_field(mhi_dev_ctxt->mmio_info.mmio_addr,
				       MHISTATUS,
				       MHISTATUS_MHISTATE_MASK,
				       MHISTATUS_MHISTATE_SHIFT);

	return (state >= MHI_STATE_LIMIT) ? MHI_STATE_LIMIT : state;
}

void mhi_set_m_state(struct mhi_device_ctxt *mhi_dev_ctxt,
					enum MHI_STATE new_state)
{
	if (MHI_STATE_RESET == new_state) {
		mhi_reg_write_field(mhi_dev_ctxt,
			mhi_dev_ctxt->mmio_info.mmio_addr, MHICTRL,
			MHICTRL_RESET_MASK,
			MHICTRL_RESET_SHIFT,
			1);
	} else {
		mhi_reg_write_field(mhi_dev_ctxt,
			mhi_dev_ctxt->mmio_info.mmio_addr, MHICTRL,
			MHICTRL_MHISTATE_MASK,
			MHICTRL_MHISTATE_SHIFT,
			new_state);
	}
	mhi_reg_read(mhi_dev_ctxt->mmio_info.mmio_addr, MHICTRL);
}

static void conditional_chan_db_write(
				struct mhi_device_ctxt *mhi_dev_ctxt, u32 chan)
{
	u64 db_value;
	unsigned long flags;
	struct mhi_ring *mhi_ring = &mhi_dev_ctxt->mhi_local_chan_ctxt[chan];

	spin_lock_irqsave(&mhi_ring->ring_lock, flags);
	db_value = mhi_v2p_addr(mhi_dev_ctxt,
				MHI_RING_TYPE_XFER_RING,
				chan,
				(uintptr_t)mhi_ring->wp);
	mhi_ring->db_mode.process_db(mhi_dev_ctxt,
				     mhi_dev_ctxt->mmio_info.chan_db_addr,
				     chan,
				     db_value);
	spin_unlock_irqrestore(&mhi_ring->ring_lock, flags);
}

static void ring_all_chan_dbs(struct mhi_device_ctxt *mhi_dev_ctxt,
			      bool reset_db_mode)
{
	u32 i = 0;
	struct mhi_ring *local_ctxt = NULL;

	mhi_log(mhi_dev_ctxt, MHI_MSG_VERBOSE, "Ringing chan dbs\n");
	for (i = 0; i < MHI_MAX_CHANNELS; ++i)
		if (VALID_CHAN_NR(i)) {
			local_ctxt = &mhi_dev_ctxt->mhi_local_chan_ctxt[i];

			/* Reset the DB Mode state to DB Mode */
			if (local_ctxt->db_mode.preserve_db_state == 0
			    && reset_db_mode)
				local_ctxt->db_mode.db_mode = 1;

			if (local_ctxt->wp != local_ctxt->rp)
				conditional_chan_db_write(mhi_dev_ctxt, i);
		}
}

static void ring_all_cmd_dbs(struct mhi_device_ctxt *mhi_dev_ctxt)
{
	u64 db_value;
	u64 rp = 0;
	struct mhi_ring *local_ctxt = NULL;

	mhi_log(mhi_dev_ctxt, MHI_MSG_VERBOSE, "Ringing chan dbs\n");

	local_ctxt = &mhi_dev_ctxt->mhi_local_cmd_ctxt[PRIMARY_CMD_RING];
	rp = mhi_v2p_addr(mhi_dev_ctxt, MHI_RING_TYPE_CMD_RING,
						PRIMARY_CMD_RING,
						(uintptr_t)local_ctxt->rp);
	db_value = mhi_v2p_addr(mhi_dev_ctxt,
				MHI_RING_TYPE_CMD_RING,
				PRIMARY_CMD_RING,
				(uintptr_t)local_ctxt->wp);
	if (rp != db_value)
		local_ctxt->db_mode.process_db(mhi_dev_ctxt,
				mhi_dev_ctxt->mmio_info.cmd_db_addr,
				0,
				db_value);
}

static void ring_all_ev_dbs(struct mhi_device_ctxt *mhi_dev_ctxt)
{
	u32 i;
	u64 db_value = 0;
	struct mhi_event_ctxt *event_ctxt = NULL;
	struct mhi_ring *mhi_ring;
	spinlock_t *lock = NULL;
	unsigned long flags;

	for (i = 0; i < mhi_dev_ctxt->mmio_info.nr_event_rings; ++i) {
		mhi_ring = &mhi_dev_ctxt->mhi_local_event_ctxt[i];
		lock = &mhi_ring->ring_lock;
		spin_lock_irqsave(lock, flags);
		event_ctxt = &mhi_dev_ctxt->dev_space.ring_ctxt.ec_list[i];
		db_value = mhi_v2p_addr(mhi_dev_ctxt,
					MHI_RING_TYPE_EVENT_RING,
					i,
					(uintptr_t)mhi_ring->wp);
		mhi_ring->db_mode.process_db(mhi_dev_ctxt,
				mhi_dev_ctxt->mmio_info.event_db_addr,
				i,
				db_value);
		spin_unlock_irqrestore(lock, flags);
	}
}

static int process_bhie_transition(struct mhi_device_ctxt *mhi_dev_ctxt,
				   enum STATE_TRANSITION cur_work_item)
{
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Entered\n");
	mhi_dev_ctxt->dev_exec_env = MHI_EXEC_ENV_BHIE;
	wake_up(mhi_dev_ctxt->mhi_ev_wq.bhi_event);
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Exited\n");

	return 0;
}

static int process_m0_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Entered With State %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));

	switch (mhi_dev_ctxt->mhi_state) {
	case MHI_STATE_M2:
		mhi_dev_ctxt->counters.m2_m0++;
		break;
	case MHI_STATE_M3:
		mhi_dev_ctxt->counters.m3_m0++;
		break;
	default:
		break;
	}

	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->mhi_state = MHI_STATE_M0;
	mhi_dev_ctxt->mhi_pm_state = MHI_PM_M0;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->assert_wake(mhi_dev_ctxt, true);

	if (mhi_dev_ctxt->flags.mhi_initialized) {
		ring_all_ev_dbs(mhi_dev_ctxt);
		ring_all_chan_dbs(mhi_dev_ctxt, true);
		ring_all_cmd_dbs(mhi_dev_ctxt);
	}

	mhi_dev_ctxt->deassert_wake(mhi_dev_ctxt);
	read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	wake_up(mhi_dev_ctxt->mhi_ev_wq.m0_event);
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Exited\n");

	return 0;
}

void process_m1_transition(struct work_struct *work)
{
	struct mhi_device_ctxt *mhi_dev_ctxt;

	mhi_dev_ctxt = container_of(work,
				    struct mhi_device_ctxt,
				    process_m1_worker);
	mutex_lock(&mhi_dev_ctxt->pm_lock);
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Processing M1 state transition from state %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));

	/* We either Entered M3 or we did M3->M0 Exit */
	if (mhi_dev_ctxt->mhi_pm_state != MHI_PM_M1) {
		write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
		mutex_unlock(&mhi_dev_ctxt->pm_lock);
		return;
	}

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Transitioning to M2 Transition\n");
	mhi_dev_ctxt->mhi_pm_state = MHI_PM_M1_M2_TRANSITION;
	mhi_dev_ctxt->counters.m1_m2++;
	mhi_dev_ctxt->mhi_state = MHI_STATE_M2;
	mhi_set_m_state(mhi_dev_ctxt, MHI_STATE_M2);
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	msleep(MHI_M2_DEBOUNCE_TMR_MS);
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	/* During DEBOUNCE Time We could be receiving M0 Event */
	if (mhi_dev_ctxt->mhi_pm_state == MHI_PM_M1_M2_TRANSITION) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"Entered M2 State\n");
		mhi_dev_ctxt->mhi_pm_state = MHI_PM_M2;
	}
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	if (unlikely(atomic_read(&mhi_dev_ctxt->counters.device_wake))) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"Exiting M2 Immediately, count:%d\n",
			atomic_read(&mhi_dev_ctxt->counters.device_wake));
		read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
		mhi_dev_ctxt->assert_wake(mhi_dev_ctxt, true);
		mhi_dev_ctxt->deassert_wake(mhi_dev_ctxt);
		read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	} else if (mhi_dev_ctxt->core.pci_master) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Schedule RPM suspend");
		pm_runtime_mark_last_busy(&mhi_dev_ctxt->pcie_device->dev);
		pm_request_autosuspend(&mhi_dev_ctxt->pcie_device->dev);
	}
	mutex_unlock(&mhi_dev_ctxt->pm_lock);
}

static int process_m3_transition(
		struct mhi_device_ctxt *mhi_dev_ctxt,
		enum STATE_TRANSITION cur_work_item)
{

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Entered with State %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));

	switch (mhi_dev_ctxt->mhi_state) {
	case MHI_STATE_M1:
		mhi_dev_ctxt->counters.m1_m3++;
		break;
	case MHI_STATE_M0:
		mhi_dev_ctxt->counters.m0_m3++;
		break;
	default:
		break;
	}

	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->mhi_state = MHI_STATE_M3;
	mhi_dev_ctxt->mhi_pm_state = MHI_PM_M3;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	wake_up(mhi_dev_ctxt->mhi_ev_wq.m3_event);
	return 0;
}

static int process_link_down_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Entered with State %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));
	return -EIO;
}

static int process_wake_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Entered with State %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));
	return -EIO;
}

static int process_bhi_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Entered\n");
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->mhi_state = MHI_STATE_BHI;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	wake_up_interruptible(mhi_dev_ctxt->mhi_ev_wq.bhi_event);
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Exited\n");
	return 0;
}

static int process_ready_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	int r = 0;

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Processing READY state transition\n");

	r = mhi_reset_all_thread_queues(mhi_dev_ctxt);
	if (r) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_ERROR,
			"Failed to reset thread queues\n");
		return r;
	}

	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->mhi_state = MHI_STATE_READY;
	r = mhi_init_mmio(mhi_dev_ctxt);
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	/* Initialize MMIO */
	if (r) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_ERROR,
			"Failure during MMIO initialization\n");
		return r;
	}
	read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	r = mhi_add_elements_to_event_rings(mhi_dev_ctxt,
				cur_work_item);
	read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	if (r) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_ERROR,
			"Failure during event ring init\n");
		return r;
	}

	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->flags.stop_threads = 0;
	mhi_reg_write_field(mhi_dev_ctxt,
			mhi_dev_ctxt->mmio_info.mmio_addr, MHICTRL,
			MHICTRL_MHISTATE_MASK,
			MHICTRL_MHISTATE_SHIFT,
			MHI_STATE_M0);
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	return r;
}

static void mhi_reset_chan_ctxt(struct mhi_device_ctxt *mhi_dev_ctxt,
				int chan)
{
	struct mhi_chan_ctxt *chan_ctxt =
			&mhi_dev_ctxt->dev_space.ring_ctxt.cc_list[chan];
	struct mhi_ring *local_chan_ctxt =
			&mhi_dev_ctxt->mhi_local_chan_ctxt[chan];
	chan_ctxt->mhi_trb_read_ptr = chan_ctxt->mhi_trb_ring_base_addr;
	chan_ctxt->mhi_trb_write_ptr = chan_ctxt->mhi_trb_ring_base_addr;
	local_chan_ctxt->rp = local_chan_ctxt->base;
	local_chan_ctxt->wp = local_chan_ctxt->base;
	local_chan_ctxt->ack_rp = local_chan_ctxt->base;
}

static int process_reset_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	int r = 0, i = 0;
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Processing RESET state transition\n");
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->mhi_state = MHI_STATE_RESET;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	mhi_dev_ctxt->counters.mhi_reset_cntr++;
	r = mhi_test_for_device_reset(mhi_dev_ctxt);
	if (r)
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"Device not RESET ret %d\n", r);
	r = mhi_test_for_device_ready(mhi_dev_ctxt);
	if (r) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_ERROR,
			"timed out waiting for ready ret:%d\n", r);
		return r;
	}

	for (i = 0; i < NR_OF_CMD_RINGS; ++i) {
		mhi_dev_ctxt->mhi_local_cmd_ctxt[i].rp =
				mhi_dev_ctxt->mhi_local_cmd_ctxt[i].base;
		mhi_dev_ctxt->mhi_local_cmd_ctxt[i].wp =
				mhi_dev_ctxt->mhi_local_cmd_ctxt[i].base;
		mhi_dev_ctxt->dev_space.ring_ctxt.cmd_ctxt[i].
						mhi_cmd_ring_read_ptr =
		mhi_v2p_addr(mhi_dev_ctxt,
			MHI_RING_TYPE_CMD_RING,
			i,
			(uintptr_t)mhi_dev_ctxt->mhi_local_cmd_ctxt[i].rp);
	}
	for (i = 0; i < mhi_dev_ctxt->mmio_info.nr_event_rings; ++i)
		mhi_reset_ev_ctxt(mhi_dev_ctxt, i);

	for (i = 0; i < MHI_MAX_CHANNELS; ++i) {
		if (VALID_CHAN_NR(i))
			mhi_reset_chan_ctxt(mhi_dev_ctxt, i);
	}
	r = mhi_init_state_transition(mhi_dev_ctxt,
				STATE_TRANSITION_READY);
	if (0 != r)
		mhi_log(mhi_dev_ctxt, MHI_MSG_CRITICAL,
			"Failed to initiate %s state trans\n",
			state_transition_str(STATE_TRANSITION_READY));
	return r;
}

static int process_syserr_transition(
			struct mhi_device_ctxt *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Entered with State %s\n",
		TO_MHI_STATE_STR(mhi_dev_ctxt->mhi_state));
	return -EIO;
}

static void enable_clients(struct mhi_device_ctxt *mhi_dev_ctxt,
					enum MHI_EXEC_ENV exec_env)
{
	struct mhi_client_handle *client_handle = NULL;
	struct mhi_cb_info cb_info;
	int i = 0, r = 0;
	struct mhi_chan_info chan_info;

	cb_info.cb_reason = MHI_CB_MHI_ENABLED;

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Enabling Clients, exec env %d.\n", exec_env);
	for (i = 0; i < MHI_MAX_CHANNELS; ++i) {
		if (!VALID_CHAN_NR(i))
			continue;
		client_handle = mhi_dev_ctxt->client_handle_list[i];
		r = get_chan_props(mhi_dev_ctxt, i, &chan_info);
		if (!r && client_handle &&
		    exec_env == GET_CHAN_PROPS(CHAN_BRINGUP_STAGE,
						chan_info.flags))
			mhi_notify_client(client_handle, MHI_CB_MHI_ENABLED);
	}

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Done.\n");
}

static int process_sbl_transition(
				struct mhi_device_ctxt *mhi_dev_ctxt,
				enum STATE_TRANSITION cur_work_item)
{

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Enabled\n");
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->dev_exec_env = MHI_EXEC_ENV_SBL;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	enable_clients(mhi_dev_ctxt, mhi_dev_ctxt->dev_exec_env);
	return 0;
}

static int process_amss_transition(
				struct mhi_device_ctxt *mhi_dev_ctxt,
				enum STATE_TRANSITION cur_work_item)
{
	int r = 0;

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Processing AMSS state transition\n");
	write_lock_irq(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->dev_exec_env = MHI_EXEC_ENV_AMSS;
	write_unlock_irq(&mhi_dev_ctxt->pm_xfer_lock);

	if (!mhi_dev_ctxt->flags.mhi_initialized) {
		r = mhi_add_elements_to_event_rings(mhi_dev_ctxt,
					cur_work_item);
		mhi_dev_ctxt->flags.mhi_initialized = 1;
		if (r) {
			mhi_log(mhi_dev_ctxt, MHI_MSG_CRITICAL,
				"Failed to set local chan state ret %d\n", r);
			mhi_dev_ctxt->deassert_wake(mhi_dev_ctxt);
			return r;
		}
		read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
		ring_all_chan_dbs(mhi_dev_ctxt, true);
		read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"Notifying clients that MHI is enabled\n");
		enable_clients(mhi_dev_ctxt, mhi_dev_ctxt->dev_exec_env);
	} else {
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"MHI is initialized\n");
	}

	read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	ring_all_ev_dbs(mhi_dev_ctxt);
	read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	complete(&mhi_dev_ctxt->cmd_complete);

	/*
	 * runtime_allow will decrement usage_count, counts were
	 * incremented by pci fw pci_pm_init() or by
	 * mhi shutdown/ssr apis.
	 */
	if (mhi_dev_ctxt->core.pci_master) {
		mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
			"Allow runtime suspend\n");

		pm_runtime_mark_last_busy(&mhi_dev_ctxt->pcie_device->dev);
		pm_runtime_allow(&mhi_dev_ctxt->pcie_device->dev);
	}

	/* During probe we incremented, releasing that count */
	read_lock_bh(&mhi_dev_ctxt->pm_xfer_lock);
	mhi_dev_ctxt->deassert_wake(mhi_dev_ctxt);
	read_unlock_bh(&mhi_dev_ctxt->pm_xfer_lock);

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO, "Exited\n");
	return 0;
}

static int process_stt_work_item(
			struct mhi_device_ctxt  *mhi_dev_ctxt,
			enum STATE_TRANSITION cur_work_item)
{
	int r = 0;

	mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
		"Transitioning to %s\n",
		state_transition_str(cur_work_item));
	trace_mhi_state(cur_work_item);
	switch (cur_work_item) {
	case STATE_TRANSITION_BHI:
		r = process_bhi_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_RESET:
		r = process_reset_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_READY:
		r = process_ready_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_SBL:
		r = process_sbl_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_AMSS:
		r = process_amss_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_M0:
		r = process_m0_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_M3:
		r = process_m3_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_SYS_ERR:
		r = process_syserr_transition(mhi_dev_ctxt,
						   cur_work_item);
		break;
	case STATE_TRANSITION_LINK_DOWN:
		r = process_link_down_transition(mhi_dev_ctxt,
							cur_work_item);
		break;
	case STATE_TRANSITION_WAKE:
		r = process_wake_transition(mhi_dev_ctxt, cur_work_item);
		break;
	case STATE_TRANSITION_BHIE:
		r = process_bhie_transition(mhi_dev_ctxt, cur_work_item);
		break;
	default:
		mhi_log(mhi_dev_ctxt, MHI_MSG_ERROR,
			"Unrecongized state: %s\n",
			state_transition_str(cur_work_item));
		BUG();
		break;
	}
	return r;
}

int mhi_state_change_thread(void *ctxt)
{
	int r = 0;
	unsigned long flags = 0;
	struct mhi_device_ctxt *mhi_dev_ctxt = (struct mhi_device_ctxt *)ctxt;
	enum STATE_TRANSITION cur_work_item;
	struct mhi_state_work_queue *work_q =
			&mhi_dev_ctxt->state_change_work_item_list;
	struct mhi_ring *state_change_q = &work_q->q_info;

	for (;;) {
		r = wait_event_interruptible(
				*mhi_dev_ctxt->mhi_ev_wq.state_change_event,
				((work_q->q_info.rp != work_q->q_info.wp) &&
				 !mhi_dev_ctxt->flags.st_thread_stopped));
		if (r) {
			mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
				"Caught signal %d, quitting\n", r);
			return 0;
		}

		if (mhi_dev_ctxt->flags.kill_threads) {
			mhi_log(mhi_dev_ctxt, MHI_MSG_INFO,
				"Caught exit signal, quitting\n");
			return 0;
		}
		mhi_dev_ctxt->flags.st_thread_stopped = 0;
		spin_lock_irqsave(work_q->q_lock, flags);
		cur_work_item = *(enum STATE_TRANSITION *)(state_change_q->rp);
		r = ctxt_del_element(&work_q->q_info, NULL);
		MHI_ASSERT(r == 0,
			"Failed to delete element from STT workqueue\n");
		spin_unlock_irqrestore(work_q->q_lock, flags);
		r = process_stt_work_item(mhi_dev_ctxt, cur_work_item);
	}
	return 0;
}

/**
 * mhi_init_state_transition - Add a new state transition work item to
 *			the state transition thread work item list.
 *
 * @mhi_dev_ctxt	The mhi_dev_ctxt context
 * @new_state		The state we wish to transition to
 *
 */
int mhi_init_state_transition(struct mhi_device_ctxt *mhi_dev_ctxt,
		enum STATE_TRANSITION new_state)
{
	unsigned long flags = 0;
	int r = 0, nr_avail_work_items = 0;
	enum STATE_TRANSITION *cur_work_item = NULL;

	struct mhi_ring *stt_ring =
		&mhi_dev_ctxt->state_change_work_item_list.q_info;
	struct mhi_state_work_queue *work_q =
			&mhi_dev_ctxt->state_change_work_item_list;

	spin_lock_irqsave(work_q->q_lock, flags);
	nr_avail_work_items =
		get_nr_avail_ring_elements(mhi_dev_ctxt, stt_ring);

	BUG_ON(nr_avail_work_items <= 0);
	mhi_log(mhi_dev_ctxt, MHI_MSG_VERBOSE,
		"Processing state transition %s\n",
		state_transition_str(new_state));
	*(enum STATE_TRANSITION *)stt_ring->wp = new_state;
	r = ctxt_add_element(stt_ring, (void **)&cur_work_item);
	BUG_ON(r);
	spin_unlock_irqrestore(work_q->q_lock, flags);
	wake_up_interruptible(mhi_dev_ctxt->mhi_ev_wq.state_change_event);
	return r;
}
