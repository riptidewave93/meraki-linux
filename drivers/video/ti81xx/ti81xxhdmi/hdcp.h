/*
 * hdcp.h
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

#ifndef _HDCP_H_
#define _HDCP_H_

#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/fs.h>


/* Event source */
#define HDCP_SRC_SHIFT		8
#define HDCP_IOCTL_SRC		(0x1 << HDCP_SRC_SHIFT)
#define HDCP_HDMI_SRC		(0x2 << HDCP_SRC_SHIFT)
#define HDCP_IRQ_SRC		(0x4 << HDCP_SRC_SHIFT)
#define HDCP_WORKQUEUE_SRC	(0x8 << HDCP_SRC_SHIFT)

/* Workqueue events */
/* Note: HDCP_ENABLE_CTL, HDCP_R0_EXP_EVENT, HDCP_KSV_TIMEOUT_EVENT and
 * HDCP_AUTH_REATT_EVENT can be cancelled by HDCP disabling
 */
#define HDCP_ENABLE_CTL		(HDCP_IOCTL_SRC		| 0)
#define HDCP_DISABLE_CTL	(HDCP_IOCTL_SRC		| 1)
#define HDCP_START_FRAME_EVENT	(HDCP_HDMI_SRC		| 2)
#define HDCP_STOP_FRAME_EVENT	(HDCP_HDMI_SRC		| 3)
#define HDCP_HPD_LOW_EVENT	(HDCP_IRQ_SRC		| 4)
#define HDCP_RI_FAIL_EVENT	(HDCP_IRQ_SRC		| 5)
#define HDCP_KSV_LIST_RDY_EVENT	(HDCP_IRQ_SRC		| 6)
#define HDCP_R0_EXP_EVENT	(HDCP_WORKQUEUE_SRC	| 7)
#define HDCP_KSV_TIMEOUT_EVENT	(HDCP_WORKQUEUE_SRC	| 8)
#define HDCP_AUTH_REATT_EVENT	(HDCP_WORKQUEUE_SRC	| 9)


enum hdcp_states {
	HDCP_DISABLED,
	HDCP_ENABLE_PENDING,
	HDCP_AUTHENTICATION_START,
	HDCP_WAIT_R0_DELAY,
	HDCP_WAIT_KSV_LIST,
	HDCP_LINK_INTEGRITY_CHECK,
};

enum hdmi_states {
	HDMI_STOPPED,
	HDMI_STARTED
};

struct hdcp_delayed_work {
	struct delayed_work work;
	int event;
};

struct hdcp {

	struct mutex lock;
	/* HDCP Enable configurations, supplied by apps. Currently takes in
		argument to specify the number of re-trys to be attempted before
		giving up autentication */
	struct ti81xxhdmi_hdcp_ena_ctrl en_ctrl;
	/* Specifies if HDMI is ON or OFF. HDMI requires to be ON before HDCP
		can start autentication process */
	enum hdmi_states hdmi_state;
	/* Specifies curret state of HDCP autentication status, drives the state
		machine */
	enum hdcp_states hdcp_state;
	/* Specifies the status of HDCP autentication that would be returned to
		applications, when queried */
	int auth_state;

	/* Wait state indicator - has been enabled - unable to enable due to
		HDMI not streaming */
	struct delayed_work *pending_start;

	/* Following variable should store works submitted from workqueue
	 * context
	 * WARNING: only ONE work at a time can be stored (no conflict
	 * should happen). It is used to allow cancelled pending works when
	 * disabling HDCP
	 */
	struct delayed_work *pending_wq_event;
	/* No of re-trys required before giving up autentication */
	int retry_cnt;
	/* Not used as of now */
	int dss_state;
	/* Not used as of now, would require to indicate HDMI to restart when
		AKSV errors are encountered */
	int hdmi_restart;
	/* Indicates the sink was di-connected or SINK drove the HPD low.
		Not used as of now. */
	int hpd_low;
	/* Primary work queue, used to handle enable request, state transitions
		etc... */
	struct workqueue_struct *workqueue;

	int hdcp_up_event;
	int hdcp_down_event;

	u8 metadata[10];

	/* Remove the following */
	void __iomem *hdmi_wp_base_addr;
};

/* Functions */
void hdcp_exit(void);
int hdcp_init(void);

#endif /* _HDCP_H_ */
