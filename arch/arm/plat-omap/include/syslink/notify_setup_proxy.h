/*
 * notify_setup_proxy.h
 *
 * Proxy to connect notify setup to device specific implementation
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

#if !defined(_NOTIFYSETUPPROXY_H_0x5f84)
#define _NOTIFYSETUPPROXY_H_0x5f84


/* Function that will be called in Notify_attach */
extern int notify_setup_omap_attach(u16 proc_id, void *shared_addr);
#define notify_setup_proxy_attach(proc_id, shared_addr)		\
			notify_setup_omap_attach(proc_id, shared_addr)

/* Function that will be called in notify_stop */
extern int notify_setup_omap_detach(u16 proc_id);
#define notify_setup_proxy_detach notify_setup_omap_detach

/* Shared Memory Required for notify setup */
extern uint notify_setup_omap_shared_mem_req(u16 proc_id, void *shared_addr);
#define notify_setup_proxy_shared_mem_req(proc_id, shared_addr)	\
			notify_setup_omap_shared_mem_req(proc_id, shared_addr)

/* Is interrupt line available? */
extern bool notify_setup_omap_int_line_available(u16 remote_proc_id);
#define notify_setup_proxy_int_line_available(remote_proc_id)	\
	notify_setup_omap_int_line_available(remote_proc_id)
/* Is interrupt line available? */
extern u16 notify_setup_omap_numintlines(u16 remote_proc_id);
#define notify_setup_proxy_numintlines(remote_proc_id)	\
	notify_setup_omap_numintlines(remote_proc_id)

#endif  /* !defined(_NOTIFYSETUPPROXY_H_0x5f84) */
