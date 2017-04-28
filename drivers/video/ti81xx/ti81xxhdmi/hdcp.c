/*
 * hdcp.c
 *
 * HDCP interface DSS driver setting for TI's OMAP4 family of processor.
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com/
 * Authors: Fabrice Olivero
 *	Fabrice Olivero <f-olivero@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Rev history:
 * Sujith Shivalingapps <sujith.s@ti.com>	Updated to include support for
 *	DM81xx. HDCP functionality is part of HDMI character driver,
 *	Was an independent character driver.
 * Sujith Shivalingappa <sujith.s>@ti.com>	Customized to TI81XX platform.
 */

/* TODO FIX ME Sujith - memory leak in hdmi_lib.c hdmi_lib_init - mem allocated
	for omap is not released - if silicon is ne/ce - Done - fixed -
	raise an IR.

	Memory leak in hdmi_init - If mem alloc fails for second alloc
	first allocated memory is not released - Done - fixed - raise an IR.

	When we have AKSV errors, we require to stop HDMI transmitter and
	re-start autentication. Require to update the user guide for same.

	Move verification of BKSV to applications (walk the revocation list).
	Driver checks for syntax error of BKSV but not the revocation list.

*/
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/completion.h>
#include <linux/miscdevice.h>
#include <linux/firmware.h>
#include <linux/ti81xxhdmi.h>
#include <plat/hdmi_lib.h>
#include <plat/hdcp_ddc.h>
#include <plat/hdcp_lib.h>
#include "../vpss/display_interface.h"

#include "hdcp.h"
#include "hdmi.h"

#ifdef PRINT_DEBUG_MSG
#define HDCP_DBG(format, ...) \
	printk(KERN_INFO "HDCP: " format, ## __VA_ARGS__)
#else

#define HDCP_DBG(format, ...)

#endif

#ifdef PRINT_STATE_TRANS_MSG
#define HDCP_STT_DBG(format, ...) \
	printk(KERN_INFO "HDCP: " format, ## __VA_ARGS__)
#else

#define HDCP_STT_DBG(format, ...)

#endif

struct hdcp hdcp;

/* Stores data that would be used to autenticate a repeater */
static struct hdcp_sha_in sha_input;

/* State machine / workqueue */
static void hdcp_wq_start_authentication(void);
static void hdcp_wq_check_r0(void);
static void hdcp_wq_step2_authentication(void);
static void hdcp_wq_authentication_failure(void);
static void hdcp_work_queue(struct work_struct *work);
static struct delayed_work *hdcp_submit_work(int event, int delay);
static void hdcp_cancel_work(struct delayed_work **work);
static void hdcp_wq_disable(void);

/* Callbacks */
static void hdcp_start_frame_cb(void);
static void hdcp_irq_cb(int hpd_low);

/* Control functions */
static long hdcp_enable_ctl(void __user *argp);
static long hdcp_disable_ctl(void);
static long hdcp_query_status_ctl(void __user *argp);
static long hdcp_wait_event_ctl(void __user *argp);
static long hdcp_done_ctl(void __user *argp);

static DECLARE_WAIT_QUEUE_HEAD(hdcp_up_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(hdcp_down_wait_queue);

/*-----------------------------------------------------------------------------
 * Function: hdcp_user_space_task
 *-----------------------------------------------------------------------------
 */
int hdcp_user_space_task(int flags)
{
	int ret;

	HDCP_DBG("Wait for user space task %x\n", flags);
	hdcp.hdcp_up_event = flags & 0xFF;
	hdcp.hdcp_down_event = flags & 0xFF;
	wake_up_interruptible(&hdcp_up_wait_queue);
	wait_event_interruptible(hdcp_down_wait_queue,
				(hdcp.hdcp_down_event & 0xFF) == 0);
	ret = (hdcp.hdcp_down_event & 0xFF00) >> 8;

	HDCP_DBG("User space task done %x\n", hdcp.hdcp_down_event);
	hdcp.hdcp_down_event = 0;

	return ret;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_wq_disable
 *-----------------------------------------------------------------------------
 */
static void hdcp_wq_disable(void)
{
	HDCP_STT_DBG( "HDCP: disabled\n");

	hdcp_cancel_work(&hdcp.pending_wq_event);
	hdcp_lib_disable();
	hdcp_lib_clear_pending_disable();

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_wq_start_authentication
 *-----------------------------------------------------------------------------
 */
static void hdcp_wq_start_authentication(void)
{
	int status = HDCP_OK;

	hdcp.hdcp_state = HDCP_AUTHENTICATION_START;

	HDCP_STT_DBG( "HDCP: authentication start\n");

	/* Step 1 part 1 (until R0 calc delay) */
	status = hdcp_lib_step1_start();

	if (status == -HDCP_AKSV_ERROR) {
		hdcp_wq_authentication_failure();
	} else if (status == -HDCP_CANCELLED_AUTH) {
		HDCP_DBG("Authentication step 1 cancelled.");
		return;
	} else if (status != HDCP_OK) {
		hdcp_wq_authentication_failure();
	} else {
		hdcp.hdcp_state = HDCP_WAIT_R0_DELAY;
		hdcp.auth_state = HDCP_STATE_AUTH_1ST_STEP;
		hdcp.pending_wq_event = hdcp_submit_work(HDCP_R0_EXP_EVENT,
							 HDCP_R0_DELAY);
	}
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_wq_check_r0
 *-----------------------------------------------------------------------------
 */
static void hdcp_wq_check_r0(void)
{
	int status;

	memset(hdcp.metadata, 0, 8);
	status = hdcp_lib_step1_r0_check(hdcp.metadata);

	if (status == -HDCP_CANCELLED_AUTH) {
		HDCP_DBG("Authentication step 1/R0 cancelled.");
		return;
	} else if (status < 0)
		hdcp_wq_authentication_failure();
	else {
		if (hdcp_lib_check_repeater_bit_in_tx()) {
			/* Repeater */
			HDCP_STT_DBG( "HDCP: authentication step 1 "
					 "successful - Repeater\n");

			hdcp.hdcp_state = HDCP_WAIT_KSV_LIST;
			hdcp.auth_state = HDCP_STATE_AUTH_2ND_STEP;

			hdcp.pending_wq_event =
				hdcp_submit_work(HDCP_KSV_TIMEOUT_EVENT,
						 HDCP_KSV_TIMEOUT_DELAY);
		} else {
			/* Receiver */
			HDCP_STT_DBG( "HDCP: authentication step 1 "
					 "successful - Receiver\n");

			hdcp.hdcp_state = HDCP_LINK_INTEGRITY_CHECK;
			hdcp.auth_state = HDCP_STATE_AUTH_3RD_STEP;

			/* Restore retry counter */
			if (hdcp.en_ctrl.nb_retry == -1)
				hdcp.retry_cnt = HDCP_INFINITE_REAUTH;
			else
				hdcp.retry_cnt = hdcp.en_ctrl.nb_retry;
		}

		memset(sha_input.data, 0, 5);
		hdcp_lib_read_bksv(sha_input.data);
		sha_input.byte_counter = 5;

		status = hdcp_user_space_task(TI81XXHDMI_HDCP_EVENT_STEP1);
		if (status) {
			printk(KERN_ERR "HDCP: BKSV error %d\n", status);
			hdcp_wq_authentication_failure();
		}
	}
}


/*-----------------------------------------------------------------------------
 * Function: hdcp_wq_step2_authentication
 *-----------------------------------------------------------------------------
 */
static void hdcp_wq_step2_authentication(void)
{
	int status = HDCP_OK;
	int i;

	/* KSV list timeout is running and should be canceled */
	hdcp_cancel_work(&hdcp.pending_wq_event);

	status = hdcp_lib_step2();

	if (status == HDCP_OK){
		status = hdcp_lib_get_sha_data(&sha_input);
	}

	if (status == HDCP_OK){
		/* Add the meta data */
		for (i = 0; i < 8; i++)
			sha_input.data[sha_input.byte_counter++] =
				hdcp.metadata[i];

		/* Wait for user space, to confirm */
		status = hdcp_user_space_task(TI81XXHDMI_HDCP_EVENT_STEP2);
		if (status) {
			printk(KERN_ERR "HDCP: CHECH_V error %d\n", status);
		}
		if (status == HDCP_OK) {
			/* Re-enable Ri check */
#ifdef _9032_AUTO_RI_
			hdcp_lib_auto_ri_check(true);
#endif
		}
	}

	if (status == -HDCP_CANCELLED_AUTH) {
		HDCP_DBG("Authentication step 2 cancelled.");
		return;
	} else if ((status == 0xFF) || (status == -HDCP_AUTH_FAILURE))
		hdcp_wq_authentication_failure();
	else {
		HDCP_STT_DBG( "HDCP: (Repeater) authentication step 2 "
				 "successful\n");

		hdcp.hdcp_state = HDCP_LINK_INTEGRITY_CHECK;
		hdcp.auth_state = HDCP_STATE_AUTH_3RD_STEP;

		/* Restore retry counter */
		if (hdcp.en_ctrl.nb_retry == -1)
			hdcp.retry_cnt = HDCP_INFINITE_REAUTH;
		else
			hdcp.retry_cnt = hdcp.en_ctrl.nb_retry;
	}
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_wq_authentication_failure
 *-----------------------------------------------------------------------------
 */
static void hdcp_wq_authentication_failure(void)
{
	if (hdcp.hdmi_state == HDMI_STOPPED) {
		hdcp.auth_state = HDCP_STATE_AUTH_FAILURE;
		return;
	}

	hdcp_lib_auto_ri_check(false);
	hdcp_lib_auto_bcaps_rdy_check(false);
	hdcp_lib_set_av_mute(AV_MUTE_SET);
	hdcp_lib_set_encryption(HDCP_ENC_OFF);

	hdcp_cancel_work(&hdcp.pending_wq_event);

	hdcp_lib_disable();
	hdcp_lib_clear_pending_disable();

	if (hdcp.retry_cnt && (hdcp.hdmi_state != HDMI_STOPPED)) {
		if (hdcp.retry_cnt < HDCP_INFINITE_REAUTH) {
			hdcp.retry_cnt--;
			HDCP_STT_DBG( "HDCP: authentication failed - "
					"retrying, attempts=%d\n",
					hdcp.retry_cnt);
		} else
			HDCP_STT_DBG( "HDCP: authentication failed - "
					 "retrying\n");

		hdcp.hdcp_state = HDCP_AUTHENTICATION_START;
		hdcp.auth_state = HDCP_STATE_AUTH_FAIL_RESTARTING;

		hdcp.pending_wq_event = hdcp_submit_work(HDCP_AUTH_REATT_EVENT,
							 HDCP_REAUTH_DELAY);
	} else {
		HDCP_STT_DBG("HDCP: authentication failed - "
				"HDCP disabled\n");
		hdcp.hdcp_state = HDCP_ENABLE_PENDING;
		hdcp.auth_state = HDCP_STATE_AUTH_FAILURE;
		/* Notify user space exit */
		hdcp_user_space_task(TI81XXHDMI_HDCP_EVENT_EXIT);
	}

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_work_queue
 *-----------------------------------------------------------------------------
 */
static void hdcp_work_queue(struct work_struct *work)
{
	struct hdcp_delayed_work *hdcp_w =
		container_of(work, struct hdcp_delayed_work, work.work);
	int event = hdcp_w->event;

	mutex_lock(&hdcp.lock);

	HDCP_DBG("hdcp_work_queue() - START - %u hdmi=%d hdcp=%d auth=%d"
		" evt= %x %d hdcp_ctrl=%02x",
		jiffies_to_msecs(jiffies),
		hdcp.hdmi_state,
		hdcp.hdcp_state,
		hdcp.auth_state,
		(event & 0xFF00) >> 8,
		event & 0xFF,
		RD_REG_32(hdcp.hdmi_wp_base_addr + HDMI_IP_CORE_SYSTEM,
			HDMI_IP_CORE_SYSTEM__HDCP_CTRL));

	/* Clear pending_wq_event
	 * In case a delayed work is scheduled from the state machine
	 * "pending_wq_event" is used to memorize pointer on the event to be
	 * able to cancel any pending work in case HDCP is disabled
	 */
	if (event & HDCP_WORKQUEUE_SRC)
		hdcp.pending_wq_event = 0;

	/* First handle HDMI state */
	if (event == HDCP_START_FRAME_EVENT) {
		hdcp.pending_start = 0;
		hdcp.hdmi_state = HDMI_STARTED;
	}

	/* Handle HDCP disable (from any state) */
	if ((event == HDCP_DISABLE_CTL) ||
	    (event == HDCP_HPD_LOW_EVENT)) {
		if (hdcp.hdcp_state != HDCP_DISABLED) {
			if (hdcp.hdcp_state != HDCP_ENABLE_PENDING)
				hdcp_wq_disable();

			if (event == HDCP_DISABLE_CTL) {
				hdcp.hdcp_state = HDCP_DISABLED;
				/* Notify user space exit */
				hdcp_user_space_task(TI81XXHDMI_HDCP_EVENT_EXIT);
			}
			else
				hdcp.hdcp_state = HDCP_ENABLE_PENDING;

			hdcp.auth_state = HDCP_STATE_DISABLED;
		}

		hdcp_lib_clear_pending_disable();
	}

	if (hdcp.hpd_low) {
		hdcp.hpd_low = 0;
		if (event & HDCP_WORKQUEUE_SRC)
			goto exit_wq;
	}

	/**********************/
	/* HDCP state machine */
	/**********************/
	switch (hdcp.hdcp_state) {

	/* State */
	/*********/
	case HDCP_DISABLED:
		/* HDCP enable control or re-authentication event */
		if (event == HDCP_ENABLE_CTL) {
			if (hdcp.en_ctrl.nb_retry == -1)
				hdcp.retry_cnt = HDCP_INFINITE_REAUTH;
			else
				hdcp.retry_cnt = hdcp.en_ctrl.nb_retry;

			if (hdcp.hdmi_state == HDMI_STARTED)
				hdcp_wq_start_authentication();
			else
				hdcp.hdcp_state = HDCP_ENABLE_PENDING;
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state"
				" - Disabled\n");
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: Event enabled\n");
		} else {
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state"
				" - Disabled\n");
		}
		break;

	/* State */
	/*********/
	case HDCP_ENABLE_PENDING:
		/* HDMI start frame event */
		if (event == HDCP_START_FRAME_EVENT){
			hdcp_wq_start_authentication();
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state - "
				"HDCP Enable Pending - F start\n");
		} else {
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state - "
				"HDCP Enable Pending\n");
		}
		break;

	/* State */
	/*********/
	case HDCP_AUTHENTICATION_START:
		/* Re-authentication */
		if (event == HDCP_AUTH_REATT_EVENT) {
			hdcp_wq_start_authentication();
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state - "
				"Auth start - restart attempt\n");
		} else {
			HDCP_DBG(KERN_INFO "DEBUG-STATE-MC: hdcp_state - "
				"Auth start\n");
		}
		break;

	/* State */
	/*********/
	case HDCP_WAIT_R0_DELAY:
		/* R0 timer elapsed */
		if (event == HDCP_R0_EXP_EVENT)
			hdcp_wq_check_r0();

		break;

	/* State */
	/*********/
	case HDCP_WAIT_KSV_LIST:
		/* Ri failure */
		if (event == HDCP_RI_FAIL_EVENT) {
			HDCP_STT_DBG( "HDCP: Ri check failure\n");

			hdcp_wq_authentication_failure();
		}
		/* KSV list ready event */
		else if (event == HDCP_KSV_LIST_RDY_EVENT)
			hdcp_wq_step2_authentication();
		/* Timeout */
		else if (event == HDCP_KSV_TIMEOUT_EVENT) {
			HDCP_STT_DBG( "HDCP: BCAPS polling timeout\n");
			hdcp_wq_authentication_failure();
		}
		break;

	/* State */
	/*********/
	case HDCP_LINK_INTEGRITY_CHECK:
		/* Ri failure */
		if (event == HDCP_RI_FAIL_EVENT) {
			HDCP_STT_DBG( "HDCP: Ri check failure\n");
			hdcp_wq_authentication_failure();
		}
		break;

	default:
		printk(KERN_WARNING "HDCP: error - unknow HDCP state\n");
		break;
	}

exit_wq :
	kfree(hdcp_w);
	hdcp_w = 0;
	if (event == HDCP_START_FRAME_EVENT){
		hdcp.pending_start = 0;
	}
	if (event == HDCP_KSV_LIST_RDY_EVENT ||
	    event == HDCP_R0_EXP_EVENT) {
		hdcp.pending_wq_event = 0;
	}

	HDCP_DBG("hdcp_work_queue() - END - %u hdmi=%d hdcp=%d "
		"auth=%d evt=%x %d ",
		jiffies_to_msecs(jiffies),
		hdcp.hdmi_state,
		hdcp.hdcp_state,
		hdcp.auth_state,
		(event & 0xFF00) >> 8,
		event & 0xFF);

	mutex_unlock(&hdcp.lock);
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_submit_work
 *-----------------------------------------------------------------------------
 */
static struct delayed_work *hdcp_submit_work(int event, int delay)
{
	struct hdcp_delayed_work *work;

	work = kmalloc(sizeof(struct hdcp_delayed_work), GFP_ATOMIC);

	if (work) {
		INIT_DELAYED_WORK(&work->work, hdcp_work_queue);
		work->event = event;
		queue_delayed_work(hdcp.workqueue,
				   &work->work,
				   msecs_to_jiffies(delay));
	} else {
		printk(KERN_WARNING "HDCP: Cannot allocate memory to "
				    "create work\n");
		return 0;
	}

	return &work->work;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_cancel_work
 *-----------------------------------------------------------------------------
 */
static void hdcp_cancel_work(struct delayed_work **work)
{
	int ret = 0;

	if (*work) {
		ret = cancel_delayed_work(*work);
		if (ret != 1) {
			/* OK, the work was being executed
			wait for it to finish */
			ret = cancel_work_sync(&((*work)->work));
		}
		kfree(*work);
		*work = 0;
	}
}


/******************************************************************************
 * HDCP callbacks
 *****************************************************************************/

/*-----------------------------------------------------------------------------
 * Function: hdcp_start_frame_cb
 *-----------------------------------------------------------------------------
 */
static void hdcp_start_frame_cb(void)
{
	HDCP_DBG("hdcp_start_frame_cb() %u", jiffies_to_msecs(jiffies));

	/* Cancel any pending work */
	if (hdcp.pending_start){
		hdcp_cancel_work(&hdcp.pending_start);
	}
	if (hdcp.pending_wq_event){
		hdcp_cancel_work(&hdcp.pending_wq_event);
	}

	hdcp.hpd_low = 0;
	hdcp_lib_clear_pending_disable();
	hdcp.retry_cnt = hdcp.en_ctrl.nb_retry;
	hdcp.pending_start = hdcp_submit_work(HDCP_START_FRAME_EVENT,
							HDCP_ENABLE_DELAY);
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_irq_cb
 *-----------------------------------------------------------------------------
 */

static void hdcp_irq_cb(int status)
{
	HDCP_DBG("hdcp_irq_cb() status=%x", status);

	/* Disable auto Ri/BCAPS immediately */
	if (((status & HDMI_RI_ERR) ||
	    (status & HDMI_BCAP) ||
	    (status & HDMI_HPD_LOW)) &&
	    (hdcp.hdcp_state != HDCP_ENABLE_PENDING)) {
		hdcp_lib_auto_ri_check(false);
		hdcp_lib_auto_bcaps_rdy_check(false);
	}

	/* Work queue execution not required if HDCP is disabled */
	/* TODO: ignore interrupts if they are masked (cannnot access UMASK
	 * here so should use global variable
	 */
	if ((hdcp.hdcp_state != HDCP_DISABLED) &&
	    (hdcp.hdcp_state != HDCP_ENABLE_PENDING)) {
		if (status & HDMI_HPD_LOW) {
			hdcp_lib_set_encryption(HDCP_ENC_OFF);
		}

		if (status & HDMI_RI_ERR) {
			hdcp_lib_set_av_mute(AV_MUTE_SET);
			hdcp_lib_set_encryption(HDCP_ENC_OFF);
			hdcp_submit_work(HDCP_RI_FAIL_EVENT, 0);
		}
		/* RI error takes precedence over BCAP */
		else if (status & HDMI_BCAP)
			hdcp_submit_work(HDCP_KSV_LIST_RDY_EVENT, 0);
	}

	if (status & HDMI_HPD_LOW) {
		hdcp_lib_set_pending_disable();	/* Used to exit on-going HDCP
						* work */
		hdcp.hpd_low = 0;		/* Used to cancel HDCP works */
		hdcp_lib_disable();
		hdcp.hdmi_state = HDMI_STOPPED;
		hdcp.hdcp_state = HDCP_DISABLED;
		hdcp.auth_state = HDCP_STATE_DISABLED;

		/* Clear up any waits, if pending */
		if (hdcp.hdcp_up_event != 0x0){
			hdcp.hdcp_up_event = TI81XXHDMI_HDCP_EVENT_EXIT;
			wake_up_interruptible(&hdcp_up_wait_queue);
		}

		if (hdcp.hdcp_down_event != 0x0){
			hdcp.hdcp_down_event =
				0x1100 | TI81XXHDMI_HDCP_EVENT_EXIT;
			wake_up_interruptible(&hdcp_down_wait_queue);
		}
	}
}

/******************************************************************************
 * HDCP control from ioctl
 *****************************************************************************/


/*-----------------------------------------------------------------------------
 * Function: hdcp_enable_ctl
 *-----------------------------------------------------------------------------
 */
static long hdcp_enable_ctl(void __user *argp)
{
	HDCP_DBG("hdcp_ioctl() - ENABLE %u", jiffies_to_msecs(jiffies));

	if (copy_from_user(&(hdcp.en_ctrl), argp,
			   sizeof(struct ti81xxhdmi_hdcp_ena_ctrl))) {
		printk(KERN_WARNING "HDCP: Error copying from user space "
				    "- enable ioctl\n");
		return -EFAULT;
	}

	/* Post event to workqueue */
	if (hdcp_submit_work(HDCP_ENABLE_CTL, 0) == 0)
		return -EFAULT;

	return 0;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_disable_ctl
 *-----------------------------------------------------------------------------
 */
static long hdcp_disable_ctl(void)
{
	HDCP_DBG("hdcp_ioctl() - DISABLE %u", jiffies_to_msecs(jiffies));

	hdcp_cancel_work(&hdcp.pending_start);
	hdcp_cancel_work(&hdcp.pending_wq_event);

	hdcp_lib_set_pending_disable();
	/* Post event to workqueue */
	if (hdcp_submit_work(HDCP_DISABLE_CTL, 0) == 0)
		return -EFAULT;

	return 0;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_query_status_ctl
 *-----------------------------------------------------------------------------
 */
static long hdcp_query_status_ctl(void __user *argp)
{
	uint32_t *status = (uint32_t *)argp;

	HDCP_DBG("hdcp_ioctl() - QUERY %u", jiffies_to_msecs(jiffies));

	*status = hdcp.auth_state;

	return 0;
}

static int hdcp_wait_re_entrance;

/*-----------------------------------------------------------------------------
 * Function: hdcp_wait_event_ctl
 *-----------------------------------------------------------------------------
 */
static long hdcp_wait_event_ctl(void __user *argp)
{
	struct ti81xxhdmi_hdcp_wait_ctrl ctrl;

	HDCP_DBG("hdcp_ioctl() - WAIT %u %d", jiffies_to_msecs(jiffies),
					 hdcp.hdcp_up_event);

	if (hdcp_wait_re_entrance == 0) {
		if (((struct ti81xxhdmi_hdcp_wait_ctrl *)argp)->data == 0){
			printk(KERN_WARNING "HDCP: Error copying from user "
					"space - wait ioctl");
			return -EFAULT;
		}
		if (copy_from_user(&ctrl, argp,
				   sizeof(struct ti81xxhdmi_hdcp_wait_ctrl))) {
			printk(KERN_WARNING "HDCP: Error copying from user "
					"space - wait ioctl");
			return -EFAULT;
		}
		hdcp_wait_re_entrance = 1;
		wait_event_interruptible(hdcp_up_wait_queue,
					 (hdcp.hdcp_up_event & 0xFF) != 0);

		ctrl.event = hdcp.hdcp_up_event;

		if ((ctrl.event & 0xFF) == TI81XXHDMI_HDCP_EVENT_STEP2 ||
			(ctrl.event & 0xFF) == TI81XXHDMI_HDCP_EVENT_STEP1){
			if (copy_to_user(ctrl.data, &sha_input,
						sizeof(struct hdcp_sha_in))) {
				printk(KERN_WARNING "HDCP: Error copying to "
						    "user space - wait ioctl");
				return -EFAULT;
			}
		}

		hdcp.hdcp_up_event = 0;
		hdcp_wait_re_entrance = 0;
	} else {
		printk(KERN_WARNING "HDCP Wait Q exited due to re-entrent\n");
		ctrl.event = TI81XXHDMI_HDCP_EVENT_EXIT;
	}
	/* Store output data to output pointer */
	if (copy_to_user(argp, &ctrl,
			 sizeof(struct ti81xxhdmi_hdcp_wait_ctrl))) {
		printk(KERN_WARNING "HDCP: Error copying to user space -"
				    " wait ioctl");
		return -EFAULT;
	}

	return 0;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_done_ctl
 *-----------------------------------------------------------------------------
 */
static long hdcp_done_ctl(void __user *argp)
{
	uint32_t *status = (uint32_t *)argp;

	HDCP_DBG("hdcp_ioctl() - DONE %u %d",
		jiffies_to_msecs(jiffies), *status);

	hdcp.hdcp_down_event &= ~(*status & 0xFF);
	hdcp.hdcp_down_event |= *status & 0xFF00;

	wake_up_interruptible(&hdcp_down_wait_queue);

	return 0;
}

extern struct hdmi hdmi;
/*-----------------------------------------------------------------------------
 * Function: hdcp_init
 *-----------------------------------------------------------------------------
 */
int hdcp_init(void)
{
	HDCP_DBG("hdcp_init() %u", jiffies_to_msecs(jiffies));

	/* Map HDMI WP address */
	hdcp.hdmi_wp_base_addr = ioremap(TI81xx_HDMI_WP, 0x1000);

	if (!hdcp.hdmi_wp_base_addr) {
		printk(KERN_ERR "HDCP: HDMI WP IOremap error\n");
		return -EFAULT;
	}

	mutex_init(&hdcp.lock);
	mutex_lock(&hdcp.lock);

	/* Variable init */
	hdcp.en_ctrl.nb_retry  = 0;
	hdcp.hdcp_state = HDCP_DISABLED;
	hdcp.pending_start = 0;
	hdcp.pending_wq_event = 0;
	hdcp.retry_cnt = 0;
	hdcp.auth_state = HDCP_STATE_DISABLED;
	hdcp_lib_clear_pending_disable();
	hdcp.hdcp_up_event = 0;
	hdcp.hdcp_down_event = 0;
	hdcp.hpd_low = 0;
	hdcp_wait_re_entrance = 0;


	hdcp.workqueue = create_singlethread_workqueue("hdcp");
	if (hdcp.workqueue == NULL)
		goto err_release_res;

	hdmi.isr_cb = &hdcp_irq_cb;
	hdmi.frame_start_event = &hdcp_start_frame_cb;

	hdmi.enable_ctl = &hdcp_enable_ctl;
	hdmi.disable_ctl = &hdcp_disable_ctl;
	hdmi.query_status_ctl = &hdcp_query_status_ctl;
	hdmi.wait_event_ctl = &hdcp_wait_event_ctl;
	hdmi.done_ctl = &hdcp_done_ctl;

	hdcp_lib_init();

	mutex_unlock(&hdcp.lock);
	printk(KERN_INFO "HDCP initialized\n");
	return 0;

err_release_res:
	mutex_destroy(&hdcp.lock);
	iounmap(hdcp.hdmi_wp_base_addr);

	return -EFAULT;
}

EXPORT_SYMBOL(hdcp_init);

/*-----------------------------------------------------------------------------
 * Function: hdcp_exit
 *-----------------------------------------------------------------------------
 */
void hdcp_exit(void)
{
	HDCP_DBG("hdcp_exit() %u", jiffies_to_msecs(jiffies));

	mutex_lock(&hdcp.lock);

	hdcp.en_ctrl.nb_retry = 0;

	hdmi.isr_cb = 0;
	hdmi.frame_start_event = 0;

	hdmi.enable_ctl = 0;
	hdmi.disable_ctl = 0;
	hdmi.query_status_ctl = 0;
	hdmi.wait_event_ctl = 0;
	hdmi.done_ctl = 0;

	/* Unmap HDMI WP */
	iounmap(hdcp.hdmi_wp_base_addr);

	destroy_workqueue(hdcp.workqueue);

	mutex_unlock(&hdcp.lock);
	mutex_destroy(&hdcp.lock);
}

EXPORT_SYMBOL(hdcp_exit);
