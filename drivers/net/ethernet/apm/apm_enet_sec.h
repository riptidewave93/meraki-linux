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

#ifndef  _APM_ENET_SEC_H_
#define  _APM_ENET_SEC_H_

#define INVALID_SEARCH_ADDR 0x7F

/**
 * @struct  apm_enet_ipsec_spi_s
 * @brief   This structure defines the Keys used to add entry to IPSec
 *          SPI2SAP database.
 **
 */
typedef struct apm_enet_ipsec_spi_s {
        u32 spi;        /**< SPI to be added to AVL tree */
        u8 port_id;     /**< 0=Port0,1=Port1             */
        u8 direction;   /**< Rx/Ingress,1=Tx/Egress      */
}apm_enet_ipsec_spi_t;

/**
 * @struct  apm_enet_ipsec_sad_s
 * @brief   This structure defines IPSec SA entry.
 **
 */
typedef struct apm_enet_ipsec_sad_s {
        /* 31-0  */
        u32 start_time_lsb  :12;
        u32 encr_offset     :8;
        u32 seq_no_ctr      :1;
        u32 iv_options      :2;
        u32 hash_alg        :2;
        u32 multitask       :2;
        u32 key_size        :2;
        u32 encry_alg       :2;
        u32 protocol        :1;
        /* 63-32 */
        u32 stop_time_lsb   :12;
        u32 start_time_msb  :20;
        /* 95 - 64 */
        u32 reserved1_1     :5;
        u32 ext_hdr_len     :7;
        u32 stop_time_msb   :20;
        /* 127 - 96 */
        u32 reserved2       :27;
        u32 pad_pattern     :2;
        u32 ipsec_mode      :1;
        u32 reserved1_2     :2;
}apm_enet_ipsec_sad_t;


/**
 * @struct  apm_enet_ipsec_spi_cfg_s
 * @brief   This structure defines IPSec SPI2SAP DB Configuration status
 **
 */
typedef struct apm_enet_ipsec_spi_cfg_s {
        u16 root_addr;  /**< The root address of the AVL Tree */
        u8  spi_cnt;    /**< The amount of spi in the SPISAP  AVL Tree DB */
}apm_enet_ipsec_spi_cfg_t;


/**
 * @brief   This function adds entry into SAPI2SAP AVL Tree DB.
 * @param   apm_enet_ipsec_spi_t spi- structure containing Key for SAPI2SAP DB
 * @param   u32* sap - SA Pointer, Used to add SAD entry.
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    None
 **
 */
int apm_enet_ipsec_spi_sap_add(apm_enet_ipsec_spi_t spi, u32* sap);



/**
 * @brief   This function delete entry from SAPI2SAP AVL Tree DB.
 * @param   apm_enet_ipsec_spi_t spi- structure containing Key for SAPI2SAP DB
 * @param   u32* sap - SA Pointer
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    None
 **
 */
int apm_enet_ipsec_spi_sap_del(apm_enet_ipsec_spi_t spi, u32* sap);


/**
 * @brief   This function search SPI entry from SAPI2SAP AVL Tree DB.
 * @param   apm_enet_ipsec_spi_t spi- structure containing Key for SAPI2SAP DB
 * @param   u32* sap - SA Pointer
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    This is used for debugging purpose only.
 **
 */
int apm_enet_ipsec_spi_sap_search(apm_enet_ipsec_spi_t spi, u32* sap);


/**
 * @brief   This function add the IPSec SA entry to SA Database.
 *          
 * @param   apm_enet_ipsec_sad_t sa - Structure containing SA entry.
 * @param   u32 sap - Address to program SA entry.
 * @param   u8 port_id - port id (0/1)
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    
 **
 */
int apm_enet_ipsec_sa_add(apm_enet_ipsec_sad_t sa, u32 sap, u8 port_id);


/**
 * @brief   This function gest the IPSec SA entry from SA Database.
 *          
 * @param   apm_enet_ipsec_sad_t sa - Pointer to structure containing SA entry.
 * @param   u32 sap - Address to program SA entry.
 * @param   u8 port_id - port id (0/1)
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    
 **
 */
int apm_enet_ipsec_sa_get(apm_enet_ipsec_sad_t *sa, u32 sap, u8 port_id);


/**
 * @brief   This function add the IPSec SA SA Database for given SPI
 *          
 * @param   apm_enet_ipsec_sad_t sa - Structure containing SA entry.
 * @param   apm_enet_ipsec_spi_t spi- structure containing Key for SAPI2SAP DB
 * @param   u32 sap - Point where SA entry added 
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    
 **
 */
int apm_enet_ipsec_add_sa_entry(apm_enet_ipsec_spi_t ipsec_spi ,
                                apm_enet_ipsec_sad_t sa, u32* sap);



/**
 * @brief   This function gets IPSec SPI2SAP DB Configuration status
 *          
 * @param   apm_enet_ipsec_spi_cfg_t*  spi_cfg - Pointer to structure
 *          containing IPSec SPI2SAP DB Configuration status
 * @return  APM_RC_OK or APM_RC_ERROR  
 * @note    
 **
 */
int apm_enet_ipsec_get_spi_cfg(apm_enet_ipsec_spi_cfg_t* spi_cfg);



#endif   /* _APM_ENET_SEC_H_ */
