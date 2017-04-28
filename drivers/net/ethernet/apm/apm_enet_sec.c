/**
 * AppliedMicro APM862xx SoC Ethernet Driver
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
 * @file apm_enet_sec.h
 *
 * This file implements access layer for APM86290 SoC Ethernet Inline
 * Security. 
 *
 **
 */

#include "apm_enet_access.h"
#include "apm_enet_sec.h"
#include "apm_enet_csr.h"

extern struct apm_data_priv *privinfo[MAX_PORTS];

int apm_enet_ipsec_spi_sap_add(apm_enet_ipsec_spi_t ipsec_spi, u32* sap)
{
	int rc = 0;
	u32 data = 0;
	u32 status = 0;
	struct apm_data_priv *priv = NULL;
	u32 count = 0;
	
	priv = privinfo[ipsec_spi.port_id];
#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	ENET_DEBUG_SEC("spi: 0X%08X, port: %d, Directon: %d \n",
		       ipsec_spi.spi, ipsec_spi.port_id, ipsec_spi.direction);
	
	data = CFG_SPI2SAP_ADD_RID_WR(ipsec_spi.spi);
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_ADDSPI_REG_ADDR, data);
	data = 0;
	data |= CFG_SPI2SAP_ADD_RID_PORT_WR(ipsec_spi.port_id)	|
		CFG_SPI2SAP_ADD_RID_DIR_WR(ipsec_spi.direction) |
		CFG_SPI2SAP_ADD_RID_EN_WR(1);
	
	/* Initiate add operation */
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_ADDSPIEN_REG_ADDR, data);
	
	/* Check for add operation completion */
	while(1) {
		rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
				   SPI2SAP_ADDSPIEN_REG_ADDR, &status);
		
		if (!(status & CFG_SPI2SAP_ADD_RID_EN_MASK))
			break;
		
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			printk("%s %d - SAI to SAP Add operation failed \n",
			       __FUNCTION__, __LINE__);
			return -1;
		}
	}
	/* Read SAP */
	data = 0;
	rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			   SPI2SAP_STSREG3_ADDR, &data);
	*sap = STS_AVLMGR_ADDADDR3_RD(data);
	ENET_DEBUG_SEC("SAI to SAP Add operation successful, SAP: 0X%08X  \n",
		       *sap);
	return rc;
}


int apm_enet_ipsec_spi_sap_del(apm_enet_ipsec_spi_t ipsec_spi, u32* sap)
{
	int rc = 0;
	u32 data = 0;
	u32 status = 0;
	struct apm_data_priv *priv = NULL;
	u32 count = 0;
	
	priv = privinfo[ipsec_spi.port_id];
#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	ENET_DEBUG_SEC("spi: 0X%08X, port: %d, Directon: %d \n",
		       ipsec_spi.spi, ipsec_spi.port_id, ipsec_spi.direction);
	
	data = CFG_SPI2SAP_DEL_RID_WR(ipsec_spi.spi);
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_DELSPI_REG_ADDR, data);
	data = 0;
	data |= CFG_SPI2SAP_DEL_RID_PORT_WR(ipsec_spi.port_id)	|
		CFG_SPI2SAP_DEL_RID_DIR_WR(ipsec_spi.direction) |
		CFG_SPI2SAP_DEL_RID_EN_WR(1);
	
	/* Initiate delete operation */
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_DELSPIEN_REG_ADDR, data);
	
	/* Check for delete operation completion */
	while(1) {
		rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
				   SPI2SAP_DELSPIEN_REG_ADDR, &status);
		
		if (!(status & CFG_SPI2SAP_DEL_RID_EN_MASK))
			break;
		
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			printk("%s %d - SAI to SAP Delete operation failed \n",
			       __FUNCTION__, __LINE__);
			return -1;
		}
	}
	/* Read SAP */
	data = 0;
	rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			   SPI2SAP_STSREG3_ADDR, &data);
	*sap = STS_AVLMGR_DELADDR3_RD(data);
	ENET_DEBUG_SEC("SAI to SAP Delete operation successful,"
		       "SAP: 0X%08X  \n",*sap);
	return rc;
}

int apm_enet_ipsec_spi_sap_search(apm_enet_ipsec_spi_t ipsec_spi, u32* sap)
{
	int rc = 0;
	u32 data = 0;
	u32 status = 0;
	struct apm_data_priv *priv = NULL;
	u32 count = 0;
	
	priv = privinfo[ipsec_spi.port_id];
#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	ENET_DEBUG_SEC("spi: 0X%08X, port: %d, Directon: %d \n",
		       ipsec_spi.spi, ipsec_spi.port_id, ipsec_spi.direction);
	
	data = CFG_SPI2SAP_SRCH_RID_WR(ipsec_spi.spi);
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_SEARCHSPI_REG_ADDR, data);
	data = 0;
	data |= CFG_SPI2SAP_SRCH_RID_PORT_WR(ipsec_spi.port_id)	 |
		CFG_SPI2SAP_SRCH_RID_DIR_WR(ipsec_spi.direction) |
		CFG_SPI2SAP_SRCH_RID_EN_WR(1);
	
	/* Initiate search operation */
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  SPI2SAP_SEARCHSPIEN_REG_ADDR, data);
	/* Check for search operation completion */
	while(1) {
		rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
				   SPI2SAP_SEARCHSPIEN_REG_ADDR, &status);
		
		if (!(status & CFG_SPI2SAP_SRCH_RID_EN_MASK))
			break;
		
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			printk("%s %d - SAI to SAP Add operation failed \n",
			       __FUNCTION__, __LINE__);
			return -1;
		}
	}
	/* Read SAP */
	data = 0;
	rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			   SPI2SAP_STSREG3_ADDR, &data);
	*sap = STS_AVLMGR_SRCHADDR3_RD(data);
	if (*sap == INVALID_SEARCH_ADDR) {
		ENET_DEBUG_SEC("SAI to SAP Search operation failed,"
			       " SAP: 0X%08X  \n", *sap);
		return -1;
	}
	ENET_DEBUG_SEC("SAI to SAP Add operation successful, SAP: 0X%08X  \n",
		       *sap);
	return rc;
}

int apm_enet_ipsec_sa_add(apm_enet_ipsec_sad_t sa, u32 sap, u8 port_id)
{
	int rc = 0;
	u32 data = 0;
	u32 status = 0;
	struct apm_data_priv *priv = NULL;
	u32 count = 0;
	u32 *temp = (u32 *)&sa;
	
	
	priv = privinfo[port_id];
#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	ENET_DEBUG_SEC("%s %d - sap: 0X%08X port_id: %d \n",
		       __FUNCTION__, __LINE___, sap, port_id);
	
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_WDATA0_REG_ADDR, *temp);
	temp++;
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_WDATA1_REG_ADDR, *temp);
	temp++;
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_WDATA2_REG_ADDR, *temp);
	temp++;
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_WDATA3_REG_ADDR, *temp);

	data = CFG_ENET_CSR_SAD_RDWR_WR(1) |
		CFG_ENET_CSR_SAD_REQ_WR(1) |
		CFG_ENET_CSR_SAD_ADDR_WR(sap);
	
	/* Initiate add/write operation */
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_ADDRCMD_REG_ADDR,data);
	/* Check for add/write operation completion */
	while(1) {
		rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
				   ENET_SAD_CMDDONE_REG_ADDR, &status);
		
		if (status & CFG_ENET_CSR_SAD_DONE_MASK )
			break;
		
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			printk("%s %d - SAD entry Add operation failed \n",
			       __FUNCTION__, __LINE__);
			return -1;
		}
	}
	printk("%s %d - SAD entry Add operation successful \n",
	       __FUNCTION__, __LINE__);
	return rc;
}

int apm_enet_ipsec_sa_get(apm_enet_ipsec_sad_t *sa, u32 sap, u8 port_id)
{

	int rc = 0;
	u32 data = 0;
	u32 status = 0;
	struct apm_data_priv *priv = NULL;
	u32 count = 0;
	u32 *temp = (u32 *)sa;
		
	priv = privinfo[port_id];
#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	ENET_DEBUG_SEC("%s %d - sap: 0X%08X port_id: %d \n",
		       __FUNCTION__, __LINE___, sap, port_id);

	data = CFG_ENET_CSR_SAD_RDWR_WR(1) |
		CFG_ENET_CSR_SAD_REQ_WR(0) |
	       CFG_ENET_CSR_SAD_ADDR_WR(sap);
	
	/* Initiate read operation */
	rc = apm_enet_wr32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_ADDRCMD_REG_ADDR, data);
	/* Check for read operation completion */
	while(1) {
		rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
				   ENET_SAD_CMDDONE_REG_ADDR, &status);
		
		if (status & CFG_ENET_CSR_SAD_DONE_MASK )
			break;
		
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			printk("%s %d - SAD entry read operation failed \n",
			       __FUNCTION__, __LINE__);
			return -1;
		}
	}
	printk("%s %d - SAD entry Add operation successful \n",
	       __FUNCTION__, __LINE__);
		
	rc = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_RDATA0_REG_ADDR, temp);
	temp++;

	rc = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_RDATA1_REG_ADDR, temp);
	temp++;

	rc = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_RDATA2_REG_ADDR, temp);
	temp++;
	rc = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			  ENET_SAD_RDATA3_REG_ADDR, temp);
	return rc;
}

int apm_enet_ipsec_add_sa_entry(apm_enet_ipsec_spi_t ipsec_spi ,
				apm_enet_ipsec_sad_t sa, u32* sap)
{
	int rc = 0;
	
	if ((rc =  apm_enet_ipsec_spi_sap_add(ipsec_spi, sap)) != 0) {
		printk("%s %d - SAI to SAP Add operation failed \n",
		       __FUNCTION__, __LINE__);
		return rc;
	}
	if ( (rc = apm_enet_ipsec_sa_add(sa, *sap, ipsec_spi.port_id)) != 0) {
		printk("%s %d - SAD entry Add operation failed \n",
		       __FUNCTION__, __LINE__);
		return rc;
	}
	return rc;
}


int apm_enet_ipsec_get_spi_cfg(apm_enet_ipsec_spi_cfg_t* spi_cfg)
{
	int rc = 0;
	u32 data = 0;
	struct apm_data_priv *priv = privinfo[0];

#ifdef ENET_SEC_DBG
	if (priv == NULL) {
		printk(KERN_INFO "Could not find priv for intf: %d\n",
		       port_id);
		return -1;
	}
#endif
	rc  = apm_enet_rd32(priv, BLOCK_ETH_GBL,
			   SPI2SAP_STSREG0_ADDR,
			   &data);
	spi_cfg->root_addr = STS_AVLMGR_ROOTADDR0_RD(data);
	spi_cfg->spi_cnt = STS_AVLMGR_RID_CNT0_RD(data);
	ENET_DEBUG_SEC("%s %d - root_addr: 0X%0X spi_cnt: %d \n",
		       __FUNCTION__, __LINE___,
		       spi_cfg->root_addr, spi_cfg->spi_cnt);
	return rc;
}
