/**
 * AppliedMicro APM862xx SoC Classifier Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Mahesh Pujara <mpujara@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * @file apm_preclass_base.h
 *
 * This file defines APM862xx SoC Classifier Parser driver.
 *
 */

#ifndef __APM_PRECLASS_BASE_H__
#define __APM_PRECLASS_BASE_H__

#define GE_PRECLASS_DB		0	/**< 1 GE ethernet type
					   preclassification database */
#define APM_CLE_ITER 100

#define IND_ADDR_PKT_RAM_0	0x00000000   /* Addr[0:2]-  000 */ 
#define IND_ADDR_PKT_RAM_1	0x20000000   /* Addr[0:2]-  001 */ 
#define IND_ADDR_PKT_RAM_2	0x40000000   /* Addr[0:2]-  010 */ 
#define IND_ADDR_PT_RAM		0x60000000   /* Addr[0:2]-  011	*/ 
#define IND_ADDR_AVL_RAM	0x80000000   /* Addr[0:2]-  100	*/ 
#define IND_ADDR_DB_RAM		0xA0000000   /* Addr[0:2]-  101	*/ 

/* Defines for compatibility with legacy Mamba SoC */
#ifdef CONFIG_APM867xx
#define READ_F2_RD CLE_READ_RD
#define READ_F2_WR CLE_READ_WR
#define WRITE_F2_RD CLE_WRITE_RD
#define WRITE_F2_WR CLE_WRITE_WR
#define WOL_MODE0_RD CLE_WOL_MODE0_RD
#define WOL_MODE0_WR CLE_WOL_MODE0_WR
#endif

/**
 * @brief   Patricia tree node read routine.
 * @param   port - GE Port number
 * @param   node_index - Offset of node (0 - 127).
 * @param   *node_value - Pointer to 32 bit array to read data into.
 * @return  APM_RC_OK or APM_RC_ERROR
 *
 */
int apm_preclass_ptram_read(u32 port, u8 node_index,
		   	u32* node_value);

/**
 * @brief   Patricia tree node write routine.
 * @param   port - GE Port number
 * @param   node_index - Offset of node (0 - 127).
 * @param   *node_value - Pointer to 32 bit array containing data to write.
 * @return  APM_RC_OK or APM_RC_ERROR
 */
int apm_preclass_ptram_write(u32 port, u8 node_index,
		      	u32* node_value);

/**
 * @brief   Packet ram user data read routine.
 * @param   port - GE Port number
 * @param   data_hi - return hi value of read data.
 * @param   data_lo - return low value of read data.
 * @return  APM_RC_OK or APM_RC_ERROR
 *
 */
int apm_preclass_pktram_read(u32 port, u32 *data_hi,
	   		u32 *data_lo);

/**
 * @brief   Packet ram user data write routine.
 * @param   port - GE Port number
 * @param   data_hi - hi value to write.
 * @param   data_lo - low value to write.
 * @return  APM_RC_OK or APM_RC_ERROR
 *
 */
int apm_preclass_pktram_write(u32 port, u32 data_hi,
		       	u32 data_lo);

/**
 * @brief   Classification database read routine.
 * @param   db_index - Database entry index
 * @param   * data - Pointer to DB entry data.
 * @return  APM_RC_OK or APM_RC_ERROR
 *
 */
int apm_preclass_cldb_read(u16 db_index, u32* data);

/**
 * @brief   Classification database write routine.
 * @param   db_index - Database entry index
 * @param   * data - Pointer to DB entry data.
 * @return  APM_RC_OK or APM_RC_ERROR
 *
 */
int apm_preclass_cldb_write(u16 db_index, u32* data);

#endif	/* __APM_PRECLASS_BASE_H__ */

