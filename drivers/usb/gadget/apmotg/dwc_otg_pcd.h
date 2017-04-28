/*
* DesignWare HS OTG controller driver
* Copyright (C) 2006 Synopsys, Inc.
* Portions Copyright (C) 2010 Applied Micro Circuits Corporation.
*
* This program is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License version 2 for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see http://www.gnu.org/licenses
* or write to the Free Software Foundation, Inc., 51 Franklin Street,
* Suite 500, Boston, MA 02110-1335 USA.
*
* Based on Synopsys driver version 2.60a
* Modified by Mark Miesfeld <mmiesfeld@apm.com>
* Modified by Stefan Roese <sr@denx.de>, DENX Software Engineering
* Modified by Chuck Meade <chuck@theptrgroup.com>
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL SYNOPSYS, INC. BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES
* (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
	     
#ifndef CONFIG_DWC_HOST_ONLY
#if !defined(__DWC_PCD_H__)
#define __DWC_PCD_H__

#include <linux/types.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>

struct lm_device;
struct dwc_otg_device;

#include "dwc_otg_cil.h"

/**
 * @file
 *
 * This file contains the structures, constants, and interfaces for
 * the Perpherial Contoller Driver (PCD).
 *
 * The Peripheral Controller Driver (PCD) for Linux will implement the
 * Gadget API, so that the existing Gadget drivers can be used.	 For
 * the Mass Storage Function driver the File-backed USB Storage Gadget
 * (FBS) driver will be used.  The FBS driver supports the
 * Control-Bulk (CB), Control-Bulk-Interrupt (CBI), and Bulk-Only
 * transports.
 *
 */

/** Invalid DMA Address */
#define DMA_ADDR_INVALID	(~(dma_addr_t)0)
/** Maxpacket size for EP0 */
#define MAX_EP0_SIZE	64
/** Maxpacket size for any EP */
#define MAX_PACKET_SIZE 1024

/**
 * Get the pointer to the core_if from the pcd pointer.
 */
#define GET_CORE_IF( _pcd ) (_pcd->otg_dev->core_if)

/**
 * States of EP0.
 */
typedef enum ep0_state {
	EP0_DISCONNECT = 0,		/* no host */
	EP0_IDLE = 1,
	EP0_IN_DATA_PHASE = 2,
	EP0_OUT_DATA_PHASE = 3,
	EP0_STATUS = 4,
	EP0_STALL = 5,
} ep0state_e;

/** Fordward declaration.*/
struct dwc_otg_pcd;

/**	  PCD EP structure.
 * This structure describes an EP, there is an array of EPs in the PCD
 * structure.
 */
typedef struct dwc_otg_pcd_ep {
	/** USB EP data */
	struct usb_ep		ep;
	/** USB EP Descriptor */
	const struct usb_endpoint_descriptor	*desc;

	/** queue of dwc_otg_pcd_requests. */
	struct list_head	queue;
	unsigned stopped : 1;
	unsigned disabling : 1;
	unsigned dma : 1;
	unsigned queue_sof : 1;

	/** DWC_otg ep data. */
	dwc_ep_t dwc_ep;

	/** Pointer to PCD */
	struct dwc_otg_pcd *pcd;
}dwc_otg_pcd_ep_t;



/** DWC_otg PCD Structure.
 * This structure encapsulates the data for the dwc_otg PCD.
 */
typedef struct dwc_otg_pcd {
	/** USB gadget */
	struct usb_gadget gadget;
	/** USB gadget driver pointer*/
	struct usb_gadget_driver *driver;
	/** The DWC otg device pointer. */
	struct dwc_otg_device *otg_dev;

	/** State of EP0 */
	ep0state_e	ep0state;
	/** EP0 Request is pending */
	unsigned	ep0_pending : 1;
	/** Indicates when SET CONFIGURATION Request is in process */
	unsigned	request_config : 1;
	/** The state of the Remote Wakeup Enable. */
	unsigned	remote_wakeup_enable : 1;
	/** The state of the B-Device HNP Enable. */
	unsigned	b_hnp_enable : 1;
	/** The state of A-Device HNP Support. */
	unsigned	a_hnp_support : 1;
	/** The state of the A-Device Alt HNP support. */
	unsigned	a_alt_hnp_support : 1;
	/** Count of pending Requests */
	unsigned	request_pending;

	/** SETUP packet for EP0
	 * This structure is allocated as a DMA buffer on PCD initialization
	 * with enough space for up to 3 setup packets.
	 */
    union {
			struct usb_ctrlrequest	req;
			uint32_t	d32[2];
	} *setup_pkt;

	dma_addr_t setup_pkt_dma_handle;

	/** 2-byte dma buffer used to return status from GET_STATUS */
	uint16_t *status_buf;
	dma_addr_t status_buf_dma_handle;

	/** Array of EPs. */
	dwc_otg_pcd_ep_t ep0;
	/** Array of IN EPs. */
	dwc_otg_pcd_ep_t in_ep[ MAX_EPS_CHANNELS - 1];
	/** Array of OUT EPs. */
	dwc_otg_pcd_ep_t out_ep[ MAX_EPS_CHANNELS - 1];
	/** number of valid EPs in the above array. */
//	  unsigned	num_eps : 4;
	spinlock_t	lock;
	/** Timer for SRP.	If it expires before SRP is successful
	 * clear the SRP. */
	struct timer_list srp_timer;

	/** Tasklet to defer starting of TEST mode transmissions until
	 *	Status Phase has been completed.
	 */
	struct tasklet_struct test_mode_tasklet;

	/** Tasklet to delay starting of xfer in DMA mode */
	struct tasklet_struct *start_xfer_tasklet;

	/** The test mode to enter when the tasklet is executed. */
	unsigned test_mode;

} dwc_otg_pcd_t;


/** DWC_otg request structure.
 * This structure is a list of requests.
 */
typedef struct dwc_otg_pcd_request {
	struct usb_request	req; /**< USB Request. */
	struct list_head	queue;	/**< queue of these requests. */
	unsigned mapped:1;
} dwc_otg_pcd_request_t;


extern int  __init dwc_otg_pcd_init(struct device *_dev);

extern void dwc_otg_pcd_remove( struct device *_dev );
extern int32_t dwc_otg_pcd_handle_intr( dwc_otg_pcd_t *_pcd );
extern void dwc_otg_pcd_start_srp_timer(dwc_otg_pcd_t *_pcd );

extern void dwc_otg_pcd_initiate_srp(dwc_otg_pcd_t *_pcd);
extern void dwc_otg_pcd_remote_wakeup(dwc_otg_pcd_t *_pcd, int set);
extern int32_t dwc_otg_pcd_handle_usb_reset_intr(dwc_otg_pcd_t * _pcd);

#endif
#endif /* CONFIG_DWC_HOST_ONLY */
