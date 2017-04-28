/*
 * notify_shm_drv.h
 *
 * Syslink driver support for OMAP Processors.
 *
 * Copyright (C) 2008-2009 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef NOTIFY_SHM_DRV_H_
#define NOTIFY_SHM_DRV_H_


/* Notify*/
#include <syslink/notify.h>
#include <syslink/notify_driverdefs.h>

/* Module ID for NotifyDriverShm. */
#define NOTIFY_SHMDRIVER_MODULEID	((u16) 0xb9d4)

#define VOLATILE volatile



/* module configuration structure */
struct notify_shm_drv_config {
	u32 reserved;
};

/* This structure defines the configuration structure for
 * initialization of the Notify driver. */
struct notify_shm_drv_params {
	void *shared_addr;
	/* Address in shared memory where this instance will be placed */
	bool cache_enabled;
	/* Whether cache operations will be performed */
	u32 cache_line_size;
	/* The cache line size of the shared memory */
	u32 remote_proc_id;
	/* Processor Id of remote processor required for communication */
	u32 line_id;
	/* Line ID for the interrupt */
	u32 local_int_id;
	/* Local interrupt ID for interrupt line for incoming interrupts */
	u32 remote_int_id;
	/* Remote interrupt ID for interrupt line for outgoing interrupts */
};

/* Defines the structure of event entry within the event chart.
 * Each entry contains occured event-specific information.
 * Used to flag a remote event and determine if a local event has been
 * flagged. This struct is placed in shared memory. */
struct notify_shm_drv_event_entry {
	VOLATILE u32 flag;
	/* Flag indicating whether event is set */
	VOLATILE u32 payload;
	/* Payload associated with event */
	VOLATILE u32 reserved;
	/* Reserved field */
	/* Padding for cache line alignment */
};

/* Defines the NotifyDriverShm control structure, which contains all
 * information for one processor. This structure is shared between the
 * two processors. */
struct notify_shm_drv_proc_ctrl {
	VOLATILE u32 recv_init_status;
	/* Initialization status for receiving events */
	VOLATILE u32 send_init_status;
	/* Initialization status for sending events */
	VOLATILE u32 event_reg_mask;
	/* Event Registered mask */
	VOLATILE u32 event_enable_mask;
	/* Event Enabled mask */
};


/* Function to get the default configuration for the notify_shm_drv driver
 * module. */
void notify_shm_drv_get_config(struct notify_shm_drv_config *cfg);

/* Function to setup the notify ducati driver with the given configuration*/
int notify_shm_drv_setup(struct notify_shm_drv_config *cfg);

/* Function to destroy the notify ducati driver */
int notify_shm_drv_destroy(void);

/*Function to initialize the given parameters  */
void notify_shm_drv_params_init(struct notify_shm_drv_params *params);

/* Function to create the ducati driver handle and performs initialization. */
struct notify_shm_drv_object *notify_shm_drv_create(
				const struct notify_shm_drv_params *params);

/* Function to delete the ducati driver handle and performs de initialization.*/
int notify_shm_drv_delete(struct notify_shm_drv_object **handle);

/* Get the shared memory requirements for the notify ducati driver. */
uint notify_shm_drv_shared_mem_req(
				const struct notify_shm_drv_params *params);


/* Register a callback for an event with the Notify driver. */
int notify_shm_drv_register_event(struct notify_driver_object *handle,
					u32 event_id);

/* Unregister a callback for an event with the Notify driver. */
int notify_shm_drv_unregister_event(struct notify_driver_object *handle,
					u32 event_id);

/* Send a notification event to the registered users for this
notification on the specified processor. */
int notify_shm_drv_send_event(struct notify_driver_object *handle,
				u32 event_id, u32 payload, bool wait_clear);

/* Disable all events for this Notify driver. */
int notify_shm_drv_disable(struct notify_driver_object *handle);

/* Restore the Notify driver to the state before the last disable was called. */
void notify_shm_drv_enable(struct notify_driver_object *handle);

/* Disable a specific event for this Notify driver. */
void notify_shm_drv_disable_event(struct notify_driver_object *handle,
					u32 event_id);

/* Enable a specific event for this Notify driver. */
void notify_shm_drv_enable_event(struct notify_driver_object *handle,
					u32 event_id);


#endif  /* !defined  NOTIFY_SHMDRIVER_H_ */
