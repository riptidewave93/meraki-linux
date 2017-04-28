/**
 * APM86xx SlimPRO Offload Driver
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Anup Patel <apatel@apm.com>
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
 * This is the SlimPRO offload driver that configures the SlimPRO queues.
 * Upper offload layer can interface with this module for queues info. 
 *
 */
#ifndef __APM_SLIMPRO_OFFLOAD_H__
#define __APM_SLIMPRO_OFFLOAD_H__

#define SLIMPRO_MAX_Q		4

struct slimpro_queue {
	int wqid[SLIMPRO_MAX_Q];
	int usage[SLIMPRO_MAX_Q];	/* Number of client */
};

/* Retrieve list of work queue */ 
struct slimpro_queue slimpro_query_q(void);

/* Request next less used work queue */ 
int slimpro_queue_request(void);

#endif
