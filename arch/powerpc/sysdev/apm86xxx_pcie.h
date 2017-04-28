/**
 * APM APM86xxx PCIe Header File
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>, Feng Kan <fkan@apm.com>.
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
 * This module defines the CSR register, header types, and function
 * for PCIE module under Linux. It supports setup and configure the
 * PCIE controllers to work as root complex or endpoint.
 *
 */
#ifndef __APM_PCIE_H__
#define __APM_PCIE_H__

#include <asm/apm_pcie_csr.h>

#define APM_PCIE_HOST_TOTAL	3
#define APM_PCIE_LINK_WAIT_US	(500*1000)

#define PCIE_CFG_MEM_RAM_SHUTDOWN_ADDR		0x00000070

/* NOTE: XXX_PIMX_HI encode as:
 *			[31]   - PCIE_WR_ATTR_STASHING_SIGNAL_AWCACHEALLOC
 *		      	[30]   - PCIE_WR_ATTR_STASHING_SIGNAL_AWCACHEALLOCLOCK
 *			[29]   - PCIE_WR_ATTR_WRITE_COHERENCY
 *			[28:26] - PCIE_WR_ATTR_PCACHE_WRITE_CHANNEL
 *			[25:23] - PCIE_WR_ATTR_WRITE_CMD_PRIORITY
 *			[22]    - PCIE_RD_ATTR_READ_COHERENCY
 *			[21]    - PCIE_RD_ATTR_READ_CACHE_FLUSH
 *			[20:19] - PCIE_RD_ATTR_PCACHE_READ_CHANNEL
 *			[18:16] - PCIE_RD_ATTR_READ_CMD_PRIORITY
 *			[15:0]  - PIM address bit [15:0]
 *       XXX_PIMX-LO	[21:0]  - PIM address bit [37:16]
 *
 * Message Region - Range 2^7 - 2^15
 *		    w/o data     [13]
 *		    Msg Routing  [12:10]
 *		    Message Code [9:2]
 *
 */

/**
 * @struct apm_pcie_map_tbl
 * @brief Host to PCIE translation address map
 * @note Although, pci_controller structure stores some of these values,
 *       it isn't complete. Therefore, we will keep a complete map here.
 *
 */
struct apm_pcie_map_tbl {
	u64 csr_addr;    		/* Host bridge CSR address */
	u32 csr_addr_size;

	/* Outbound AXI address for translation to PCIE */
	u32 cfg_addr_hi;    		/* Configuration AXI address for all */
	u32 cfg_addr_lo;		/* PCI devices */
	u32 cfg_addr_size;		/* use 0 for not used */
	u32 cfg_pcie_hi;
	u32 cfg_pcie_lo;		
	void *cfg_vaddr;		/* Virtual address */
	struct {
		u32 hi;   		/* Host address map and size */
		u32 lo;
		u64 size;
		u32 pcie_hi;   		/* PCIE address map  and size */
		u32 pcie_lo;
	} ob_mem_addr[3];		/* Inbound two memory and one IO regions */
	u32 ob_msg_addr_hi;		/* Message region address map */
	u32 ob_msg_addr_lo;
	u64 ob_msg_addr_size;		/* use 0 for not used */
	u32 ob_msg_pcie_addr_hi;	/* PCIE address map */
	u32 ob_msg_pcie_addr_lo;

	/* Inbound AXI address for translation from PCIE */
	struct {
		u32 pcie_hi;   		/* PCIE address map */
		u32 pcie_lo;
		u64 pcie_size; 		/* use 0 for not used */
		u32 pim1_hi;   		/* Host address map first half */
		u32 pim1_lo;
		u32 pim2_hi;   		/* To set R/W attribute, OR's them
					   with address */
		u32 pim2_lo;
		u64 pim_size;  		/* Host address map second half */
	} ib_mem_addr[2];		/* Inbound two memory regions */
	u32 ib_rom_addr_hi;		/* Inbound expansion rom map address */
	u32 ib_rom_addr_lo;
	u32 ib_rom_addr_size;  		/* use 0 for not used */
};

struct apm_pciex_port {
	struct pci_controller	*hose;
	struct device_node	*node;
	unsigned int		index;
	int endpoint;
	int link;
	u32 pwred_off_flags;
	int last_busno;		/* Save this as PCI scan code changes to MAX */
	int rmt_mrrqs;		/* Remote max read request size */
	int phy_reset;		/* Always reset/init PHY */
	
	struct apm_pcie_map_tbl pcie_map_tbl;
	void __iomem		*scu_base;
};

/**
 * @brief  This function determines if PCIE controller at bus 'port' present
 *         or not.
 * @param  port    PCIE controller port to check
 * @return 1 if present, 0 otherwise
 *
 */
int apm_pcie_is_card_present(int port);

/**
 * @brief  This function determines if PCIE controller is host or endpoint
 * @param  port    A integer port number
 * @return 1 if present, 0 otherwise
 *
 */
int apm_pcie_is_card_host(int port);

/**
 * @brief  This function determines if PCIE controller is host or endpoint
 * @param  host   Host controller structure
 * @return 1 for configure as HOST, 0 otherwise
 *
 */
int apm_pcie_is_host(struct pci_controller *host);

#endif
