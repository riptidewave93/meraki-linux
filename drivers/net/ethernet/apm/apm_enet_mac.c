/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Keyur Chudgar <kchudgar@apm.com>
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
 * @file apm_enet_mac.c
 *
 * This file implements driver for RGMII, MAC and statistics blocks of
 * APM862xx SoC Ethernet subsystem
 *
 */
#include "apm_enet_access.h"
#include "apm_enet_mac.h"
#include "apm_enet_csr.h"

#undef APM_ENET_MAC_LOOPBACK	

#define MAX_LOOP_POLL_TIMEMS	500
#define MAX_LOOP_POLL_CNT	10
#define ACCESS_DELAY_TIMEMS	(MAX_LOOP_POLL_TIMEMS / MAX_LOOP_POLL_CNT)

/* RGMII related functions */
int apm_extquadmiiphy_read(struct apm_data_priv *priv,
			  unsigned char reg, u32 *data)
{
	u32 value;
	u8 addr = 0x01;
	
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(addr) | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_ADDRESS_ADDR,value);
	
	/* Write read command */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_COMMAND_ADDR,READ_CYCLE_MASK);
	
	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv,
			       BLOCK_ETH_MAC,MII_MGMT_INDICATORS_ADDR,&value);
		if (!(value & BUSY_F14_MASK))
			break;
	}
	apm_emac_read32(priv,BLOCK_ETH_MAC,MII_MGMT_STATUS_ADDR,&value);
	*data = value;
	
	/* reset mii_mgmt_command register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_COMMAND_ADDR, 0);
	return 0;
}

int apm_extquadmiiphy_write(struct apm_data_priv *priv,
			   unsigned char reg, u32 data)
{
	u32 value;
	u8 addr = 0x01F;
    
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(addr) | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_ADDRESS_ADDR,value);
	
	/* Write 16 bit data to MII MGMT CONTROL */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_CONTROL_ADDR,value);
	
	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv, BLOCK_ETH_MAC,
			       MII_MGMT_INDICATORS_ADDR, &value);
		if (!(value & BUSY_F14_MASK))
			break;
	}
	return 0;
}

int apm_genericmiiphy_write(struct apm_data_priv *priv, u8 phy_id,
			   unsigned char reg, u32 data)
{
	u32 value;
	
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(phy_id) | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_ADDRESS_ADDR,value);
	
	/* Write 16 bit data to MII MGMT CONTROL */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_CONTROL_ADDR, value);

	/* wait until the write is complete */
	while (1)
	{
		apm_emac_read32(priv, BLOCK_ETH_MAC,
			       MII_MGMT_INDICATORS_ADDR, &value);
		if (!(value & BUSY_F14_MASK))
			break;
	}
	return 0;
}

int apm_genericmiiphy_read(struct apm_data_priv *priv, u8 phy_id,
			  unsigned char reg, u32 *data)
{
	u32 value;
	
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(phy_id) | REG_ADDR_WR(reg);
	
	PHY_PRINT("phy_id: %d, reg: %d \n", phy_id, reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_ADDRESS_ADDR, value);

	ENET_DEBUG_RD("MII_MGMT_CMD: 0X%x \n", READ_CYCLE_MASK);
	PHY_PRINT("MII_MGMT_CMD: 0X%x \n", READ_CYCLE_MASK);
	
	/* Write read command */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_COMMAND_ADDR, 
			READ_CYCLE_MASK);

	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv, BLOCK_ETH_MAC, 
			       MII_MGMT_INDICATORS_ADDR, &value);
		if (!(value & BUSY_F14_MASK))
			break;
	}
	
	apm_emac_read32(priv, BLOCK_ETH_MAC, MII_MGMT_STATUS_ADDR, data);
	ENET_DEBUG_RD("MII_MGMT_STATUS: 0X%x \n", *data);
	PHY_PRINT("MII_MGMT_STATUS: 0X%x \n", *data);
	
	/* reset mii_mgmt_command register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_COMMAND_ADDR, 0);
	return 0;
}

int apm_extmiiphy_write(struct apm_data_priv *priv,
			unsigned char reg, u32 data)
{
	u32 value;
	u8 addr = 0x1F;
	
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(addr) | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MII_MGMT_ADDRESS_ADDR,value);
	
	/* Write 16 bit data to MII MGMT CONTROL */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_CONTROL_ADDR,
			value);
	
	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv, BLOCK_ETH_MAC,
			       MII_MGMT_INDICATORS_ADDR, &value);
		if (!(value & BUSY_F14_MASK))
			break;
	}
	return 0;
}

int apm_miiphy_write(struct apm_data_priv *priv, unsigned int reg, u32 data)
{
	u32 value;
	u32 count = 0;
	u8 addr = 0x1E;

	reg = reg/4;  

	/* Write PHY number and address in MII Mgmt Address Register */
	value = PHY_ADDR_WR(addr)  | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_ADDRESS_ADDR, value);

	/* Write 16 bit data to MII MGMT CONTROL */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_CONTROL_ADDR, value);
	
	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv, BLOCK_ETH_MAC,
			       MII_MGMT_INDICATORS_ADDR, &value);
		
		if (!(value & BUSY_F14_MASK))
			break;

		udelay(ACCESS_DELAY_TIMEMS);
	
		if(count++ > MAX_LOOP_POLL_CNT) {
			ENET_DEBUG("Can not write to RGMII register\n");
			return -1;
		}
	}
	return 0;
}

int apm_miiphy_read(struct apm_data_priv *priv, unsigned int reg, u32 *data)
{
	u32 value;
	u32 count = 0;
	u8 addr = 0x1E;
	
	reg = reg/4;  
	
	/* Write PHY number and address in MII Mgmt Address */
	value = PHY_ADDR_WR(addr) | REG_ADDR_WR(reg);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_ADDRESS_ADDR, value);
	
	/* issue read command */
	value = PHY_CONTROL_WR(data);
	apm_emac_write32(priv, BLOCK_ETH_MAC, 
			MII_MGMT_COMMAND_ADDR, READ_CYCLE_MASK);
	
	/* wait until the write is complete */
	while (1) {
		apm_emac_read32(priv, BLOCK_ETH_MAC, 
			       MII_MGMT_INDICATORS_ADDR, &value);
		
		if (!(value & BUSY_F14_MASK))
			break;
		udelay(ACCESS_DELAY_TIMEMS);
		if(count++ > MAX_LOOP_POLL_CNT) {
			ENET_DEBUG("Can not read from RGMII register\n");
			return -1;
		}
	}
	apm_emac_read32(priv, BLOCK_ETH_MAC, MII_MGMT_STATUS_ADDR, &value);
	*data = value;
	/* reset mii_mgmt_command register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_COMMAND_ADDR, 0);
	return 0;
}


#if 0 
int apm_mii_get_auto_neg(struct apm_data_priv *priv) 
{
	u32 data;
	
	apm_miiphy_read(priv, SGMII_CONTROL_ADDR/4, &data);
	
	return (data & SGMII_CONTROL_R_ENABLE_AN_WR(1)) ? 1 : 0;
}

void apm_mii_auto_neg(struct apm_data_priv *priv, int enable) 
{
	u32 data;
	
	apm_miiphy_read(priv, SGMII_CONTROL_ADDR/4, &data);
	
	if (enable)
		apm_miiphy_write(priv, SGMII_CONTROL_ADDR/4,
				data | SGMII_CONTROL_R_ENABLE_AN_WR(1));
	else 
		apm_miiphy_write(priv, SGMII_CONTROL_ADDR/4,
				data & ~ SGMII_CONTROL_R_ENABLE_AN_WR(1));
}

int apm_mii_start_auto_neg(struct apm_data_priv *priv) 
{
	u32 data;
	u32 status;
	u32 count = 0;
	u32 val = SGMII_CONTROL_R_ENABLE_AN_WR(1) |
		SGMII_CONTROL_R_RESET_AN_WR(1);
	
	apm_miiphy_read(priv, SGMII_CONTROL_ADDR / 4, &data);
	
	apm_miiphy_write(priv, SGMII_CONTROL_ADDR / 4, data | val);
	
	do {
		apm_miiphy_read(priv, SGMII_STATUS_ADDR / 4, &status);
		
		if(count++ > MAX_LOOP_POLL_CNT) {
			ENET_DEBUG("Can not write to SGMII register\n");
			return -1;
		}
		
		udelay(ACCESS_DELAY_TIMEMS);
		
		status &= SGMII_STATUS_R_AUTO_NEGOTIATION_COMPLETE_MASK;
	} while (!status);

	return 0;
}

int apm_miiphy_loopback(struct apm_data_priv *priv) 
{
	/* PHY loopback , TBD, it's not loopback, it's init, change it */
	apm_miiphy_write(priv, SGMII_CONTROL_ADDR,  0x8140);
	return 0;
}

#endif

void apm_rgmii_loopback(struct apm_data_priv *priv, u8 loopback)
{
	u32 data;

	/* Read current value of the RGMII register */
	apm_emac_read32(priv, BLOCK_ETH_MAC_GBL,
		       RGMII_REG_0_ADDR, &data);

	/* Modify value to set or remove RGMII loopback mode */
	if (loopback) 
		data |= CFG_LOOPBACK_TX2RX0_MASK;
	else
		data &= ~ CFG_LOOPBACK_TX2RX0_MASK;
    
	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
			RGMII_REG_0_ADDR, data);
}

int apm_miiphy_init(struct apm_data_priv *priv) 
{
	u32 val;
	
	/* Reset MAC */					
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &val);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, 0);

	/* slow down MDIO clock*/
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_CONFIG_ADDR, 0x6);

	/* Enable SGMII REF clock */
	apm_extquadmiiphy_write(priv, 0x10, 0x68);
	apm_extquadmiiphy_write(priv, 0x00, 0x9140);
	
	/* Select Clock - 2 = 1Gps, 1 = 100Mbps, 0 = 10Mbps */
#ifdef APM_ENET_100BPS
	val = MGMT_CLOCK_SEL_WR(1);
#else
	val = MGMT_CLOCK_SEL_WR(2);
#endif
	apm_emac_write32(priv, BLOCK_ETH_MAC,  MII_MGMT_CONFIG_ADDR, val);
	
	/*Fix Me - Probbably not needed	 */ 
	apm_extmiiphy_write(priv, 0x1B, 0x8320);
	apm_extmiiphy_write(priv, 0x0, 0x9140);
	
	/* Reset internal and external PHY to 1Gbps - no auto nego */
#ifndef APM_ENET_100BPS
	apm_extmiiphy_write(priv, 0x16, 0x01);
	apm_extmiiphy_write(priv, 0x01, 0x8000);
	apm_extmiiphy_write(priv, 0x01, 0x0140);
	udelay(100000);
#endif
	/* Enable Data path port 0 PHY */
	apm_genericmiiphy_write(priv, 0x4, 0x16, 1);
	apm_genericmiiphy_write(priv, 0x4, 0x0, 0x8140);
    
	return 0;
}
/* End of RGMII related functions */

/* Start MAC related functions */
void apm_gmac_set_gmac_addr(struct apm_data_priv *priv,
			unsigned char *dev_addr)
{
	u32 a_hi;
	u32 a_lo;

	a_hi = *(u32 *) &dev_addr[0];
	a_lo = (u32) *(u16 *) &dev_addr[4];

	/* Write higher 4 octects to station register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, STATION_ADDR0_ADDR, a_hi);

	a_lo <<= 16;
	a_lo |= (priv->phy_addr & 0xFFFF);

	/* Write lower 2 octects to station register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, STATION_ADDR1_ADDR, a_lo);
}

int apm_gmac_init(struct apm_data_priv *priv,
		unsigned char *dev_addr, int speed, int mtu, int crc)
{
	u32 value;
	u32 temp;
	u32 addr_hi;
	u32 addr_lo;

	u32 interface_control;
	u32 mac_config_2;
	u32 rgmii;
	u32 icm_config0;
	u32 icm_config2;
#ifdef CONFIG_APM867xx
	u32 ecm_config0;
#endif
	u32 enet_spare_cfg;
	u8 port = priv->port;

	PHY_PRINT("Setting Port%d Link Speed %d Mbps \n", port, speed);
	apm86xxx_read_scu_reg(SCU_SOCDIV4_ADDR, &value);

	switch(speed) {
	case SPEED_10:
		if (port == 0) {
			value = RGMII0_CLK_FREQ_SEL4_SET(value, 0xC8);
		} else {
			value = RGMII1_CLK_FREQ_SEL4_SET(value, 0xC8);
		}
		break;

	case SPEED_100:
		if (port == 0) {
			value = RGMII0_CLK_FREQ_SEL4_SET(value, 0x14);
		} else {
			value = RGMII1_CLK_FREQ_SEL4_SET(value, 0x14);
		}
		break;

	default:
		if (port == 0) {
			value = RGMII0_CLK_FREQ_SEL4_SET(value, 0x04);
		} else {
			value = RGMII1_CLK_FREQ_SEL4_SET(value, 0x04);
		}
	}

	apm86xxx_write_scu_reg(SCU_SOCDIV4_ADDR, value);

	apm_emac_read32(priv, BLOCK_ETH_GBL,
			ENET_SPARE_CFG_REG_ADDR, &enet_spare_cfg);

	if (speed == SPEED_10) {
		interface_control = LHD_MODE_WR(0) |
			GHD_MODE_WR(0);
		mac_config_2 = FULL_DUPLEX2_WR(1)  |
			LENGTH_CHECK2_WR(0)        |
			HUGE_FRAME_EN2_WR(0)       |
			INTERFACE_MODE2_WR(1)      | /* 10Mbps */
			PAD_CRC2_WR(crc)           |
			CRC_EN2_WR(crc)            |
			PREAMBLE_LENGTH2_WR(7);
		rgmii = 0;
		icm_config0 = 0x0000503f;
		icm_config2 = 0x000101f4;
#ifdef CONFIG_APM867xx
		ecm_config0 = 0x600032;
#endif
		if (port == 0)
			enet_spare_cfg = enet_spare_cfg | (0x0000c040);
		else
			enet_spare_cfg = enet_spare_cfg | (0x00030040);
	} else if (speed == SPEED_100) {
		interface_control = LHD_MODE_WR(1);
		mac_config_2 = FULL_DUPLEX2_WR(1)  |
			LENGTH_CHECK2_WR(1)        |
			HUGE_FRAME_EN2_WR(0)       |
			INTERFACE_MODE2_WR(1)      | /* 100Mbps */
			PAD_CRC2_WR(crc)           |
			CRC_EN2_WR(crc)            |
			PREAMBLE_LENGTH2_WR(7);
		rgmii = 0;
		icm_config0 = 0x0004503f;
		icm_config2 = 0x00010050;
#ifdef CONFIG_APM867xx
		ecm_config0 = 0x600032;
#endif
		if (port == 0)
			enet_spare_cfg = enet_spare_cfg | (0x0000c040);
		else
			enet_spare_cfg = enet_spare_cfg | (0x00030040);
	} else {
		interface_control = GHD_MODE_WR(1);
		mac_config_2 = FULL_DUPLEX2_WR(1)  |
			LENGTH_CHECK2_WR(0)        |
			HUGE_FRAME_EN2_WR(0)       |
			INTERFACE_MODE2_WR(2)      | /* 1Gbps */
			PAD_CRC2_WR(crc)           |
			CRC_EN2_WR(crc)            |
			PREAMBLE_LENGTH2_WR(5);

		// Fullerene specific
		rgmii = CFG_TXCLK_MUXSEL0_WR(6) |
			CFG_RXCLK_MUXSEL0_WR(5) |
			CFG_SPEED_1250_WR(1);
		/* rgmii = CFG_TXCLK_MUXSEL0_WR(4)    | */
		/* 	CFG_SPEED_1250_WR(1); */
		icm_config0 = 0x0008503f;
		icm_config2 = 0x0001000f;
#ifdef CONFIG_APM867xx
		ecm_config0 = 0x32;
#endif
		if (port == 0)
			enet_spare_cfg = (enet_spare_cfg & ~0x0000c000) | (0x00000040);
		else
			enet_spare_cfg = (enet_spare_cfg & ~0x00030000) | (0x00000040);
	}

	/* Reset subsystem */
	value = RESET_TX_FUN1_WR(1) |
		RESET_RX_FUN1_WR(1) |
		RESET_TX_MC1_WR(1)  |
		RESET_RX_MC1_WR(1)  |
		SIM_RESET1_WR(1)    |
		SOFT_RESET1_WR(1);

	ENET_DEBUG("MAC base addresses: 0x%X\n", priv->mac_base_addr_v);
	ENET_DEBUG("Stat base addresses: 0x%X\n",priv->stats_base_addr_v);

	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, value);
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &temp);
	udelay(100);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, 0);

	/* Program the station MAC address */
	addr_hi = *(u32 *) &dev_addr[0];
	addr_lo = *(u16 *) &dev_addr[4];
	addr_lo <<= 16;
	addr_lo |= (priv->phy_addr & 0xFFFF);

	ENET_DEBUG("MAC addr hi: %x\n", addr_hi);
	apm_emac_write32(priv, BLOCK_ETH_MAC, STATION_ADDR0_ADDR, addr_hi);
	ENET_DEBUG("MAC addr lo: %x\n", addr_lo);
	apm_emac_write32(priv, BLOCK_ETH_MAC, STATION_ADDR1_ADDR, addr_lo);

	/* Initialize the Interface Control Register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, INTERFACE_CONTROL_ADDR,
			interface_control);

	/* Initialize the Maximum Frame Length register */
	apm_gmac_change_mtu(priv, mtu);

	/* Initialize the MAC configuration #2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, mac_config_2);

	/* Initialize the MAC configuration #1 register */
	value = TX_EN1_WR(1)	  | 
		RX_EN1_WR(1)	  |
			TX_FLOW_EN1_WR(0) |
#ifdef APM_ENET_MAC_LOOPBACK
		LOOP_BACK1_WR(1)  |
#else
		LOOP_BACK1_WR(0)  |
#endif
			RX_FLOW_EN1_WR(0); 

	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, value);
	
	/* Adjust MDC clock frequency */                                
        apm_enet_rd32(priv, BLOCK_ETH_MAC, MII_MGMT_CONFIG_ADDR, &value);
        value |= MGMT_CLOCK_SEL_WR(7);
        apm_enet_wr32(priv, BLOCK_ETH_MAC, MII_MGMT_CONFIG_ADDR, value);

#ifdef CONFIG_APM867xx
	/* Reset SGMII Port  */
	if ((port == 2) || (port == 3)) {
		apm_enet_wr32(priv, BLOCK_ETH_MAC, 
				MII_MGMT_ADDRESS_ADDR, 
				PHY_ADDR_WR(0x1e) | REG_ADDR_WR(0x11));
		apm_enet_wr32(priv, BLOCK_ETH_MAC, 
				MII_MGMT_CONTROL_ADDR, PHY_CONTROL_WR(0x0));
		value = 0x0;
		apm_enet_rd32(priv, BLOCK_ETH_MAC, 
				MII_MGMT_INDICATORS_ADDR, &value);
		while (value) {
			apm_enet_rd32(priv, BLOCK_ETH_MAC, 
				MII_MGMT_INDICATORS_ADDR, &value);
		}
	}
#endif

	if ((port % 2) == 0) { 	/* For port0 and port2 */
		/* Initialize HdrPrs_Config2_reg_0 */
		value = CFG_IPV4_BYTES0_WR(IPV4_HDR_SIZE) |
			CFG_IPV6_BYTES0_WR(IPV6_HDR_SIZE);
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				HDRPRS_CONFIG2_REG_0_ADDR, value);
		/* Rtype should be copied from FP */
		value = 0;
		apm_emac_write32(priv, BLOCK_ETH_GBL,
				RSIF_RAM_DBG_REG0_ADDR, value);
		/* Enable drop if FP not available */
		apm_emac_read32(priv, BLOCK_ETH_GBL,
				RSIF_CONFIG_REG_ADDR, &value);
		value |= CFG_RSIF_FPBUFF_DROP_EN_WR(1);
		apm_emac_write32(priv, BLOCK_ETH_GBL,
				RSIF_CONFIG_REG_ADDR, value);

		if (port == 0) {
			/* Initialize RGMII PHY */
			apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
						RGMII_REG_0_ADDR, rgmii);
		}

		/* Enable HdrParser Timeout */
		value = CFG_PRS_EN_TIMEOUT0_WR(1) |
			CFG_PRS_MAX_HDR_SIZE0_WR(0x80);
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				HDRPRS_CONFIG3_REG_0_ADDR, value);

		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ICM_CONFIG0_REG_0_ADDR, icm_config0);

		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ICM_CONFIG2_REG_0_ADDR, icm_config2);

#ifdef CONFIG_APM867xx
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ECM_CONFIG0_REG_0_ADDR, ecm_config0);
#endif

		apm_emac_write32(priv, BLOCK_ETH_GBL,
				ENET_SPARE_CFG_REG_ADDR, enet_spare_cfg);

		/* TCP MSS  */
		apm_emac_read32(priv, BLOCK_ETH_GBL,
				TSIF_MSS_REG0_0_ADDR, &value);

		apm_emac_write32(priv, BLOCK_ETH_GBL,
				 TSIF_MSS_REG0_0_ADDR,
			    CFG_TSIF_MSS_SZ00_SET(value, DEFAULT_TCP_MSS));

		apm_emac_read32(priv, BLOCK_ETH_GBL,
				TSIF_MSS_REG0_0_ADDR, &value);
		
#ifdef CONFIG_APM867xx
		/* Rx-Tx traffic resume */
		apm_emac_write32(priv, BLOCK_ETH_GBL, 
				CFG_LINK_AGGR_RESUME_0_ADDR, TX_PORT0_WR(0x1));
#endif
	} else if ((port % 2) == 1) { 	/* For port1 and port3 */
		/* Initialize HdrPrs_Config2_reg_1 */
		value = CFG_IPV4_BYTES0_WR(IPV4_HDR_SIZE) |
			CFG_IPV6_BYTES0_WR(IPV6_HDR_SIZE);
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				HDRPRS_CONFIG2_REG_1_ADDR, value);
		/* Rtype should be copied from FP */
		value = 0;
		apm_emac_write32(priv, BLOCK_ETH_GBL,
				RSIF_RAM_DBG_REG0_ADDR, value);
		/* Enable drop if FP not available */
		apm_emac_read32(priv, BLOCK_ETH_GBL,
				RSIF_CONFIG_REG_ADDR, &value);
		value |= CFG_RSIF_FPBUFF_DROP_EN_WR(1);
		apm_emac_write32(priv, BLOCK_ETH_GBL,
				RSIF_CONFIG_REG_ADDR, value);

		if (port == 1) {
			/* Initialize RGMII PHY */
			apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
						RGMII_REG_1_ADDR, rgmii);
		}

		/* Enable HdrParser Timeout */
		value = CFG_PRS_EN_TIMEOUT0_WR(1) |
			CFG_PRS_MAX_HDR_SIZE0_WR(0x80);
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				HDRPRS_CONFIG3_REG_1_ADDR, value);

		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ICM_CONFIG0_REG_1_ADDR, icm_config0);

		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ICM_CONFIG2_REG_1_ADDR, icm_config2);

#ifdef CONFIG_APM867xx
		apm_emac_write32(priv, BLOCK_ETH_MAC_GBL,
				ECM_CONFIG0_REG_1_ADDR, ecm_config0);
#endif

		apm_emac_write32(priv, BLOCK_ETH_GBL,
				ENET_SPARE_CFG_REG_ADDR, enet_spare_cfg);

		/* TCP MSS  */
		apm_emac_read32(priv, BLOCK_ETH_GBL,
				TSIF_MSS_REG1_0_ADDR, &value);

		apm_emac_write32(priv, BLOCK_ETH_GBL,
				TSIF_MSS_REG1_0_ADDR,
				CFG_TSIF_MSS_SZ10_SET(value, DEFAULT_TCP_MSS));

		apm_emac_read32(priv, BLOCK_ETH_GBL,
				TSIF_MSS_REG1_0_ADDR, &value);

#ifdef CONFIG_APM867xx
		/* Rx-Tx traffic resume */
		apm_emac_write32(priv, BLOCK_ETH_GBL, 
				CFG_LINK_AGGR_RESUME_1_ADDR, TX_PORT1_WR(0x1));
#endif
	}

	if (speed != SPEED_10 && speed != SPEED_100) {
		apm_emac_read32(priv, BLOCK_ETH_GBL, DEBUG_REG_ADDR, &value);
		value |= CFG_BYPASS_UNISEC_TX_WR(1) |
			CFG_BYPASS_UNISEC_RX_WR(1);
		apm_emac_write32(priv, BLOCK_ETH_GBL, DEBUG_REG_ADDR, value);
	}

#ifdef PCM_LOOPBACK
	apm_emac_read32(priv, BLOCK_ETH_GBL, DEBUG_REG_ADDR, &value);
	value |= CFG_DEBUG_RXTSOTXBUF_LOOPBACK_WR(1);
	apm_emac_write32(priv, BLOCK_ETH_GBL, DEBUG_REG_ADDR, value);
	apm_gmac_crc_enable(priv, 0);
	apm_gmac_pad_crc_enable(priv, 0);
#endif

	return 0;
}

void apm_gmac_loopback(struct apm_data_priv *priv, u8 loopback)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Modify value to set or remove loopback mode */
	if (loopback) 
		data |= LOOP_BACK1_MASK;
	else
		data &= ~LOOP_BACK1_MASK;

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,MAC_CONFIG_1_ADDR, data);
}

int apm_gmac_is_rx_flow_control(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	return RX_FLOW_EN1_RD(data);
}

void apm_gmac_rx_flow_control(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Modify value to set or reset rx flow control */
	if (enable) 
		data |= RX_FLOW_EN1_MASK;
	else 
		data &= ~RX_FLOW_EN1_MASK;

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,MAC_CONFIG_1_ADDR, data);
}

int apm_gmac_is_tx_flow_control(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	return TX_FLOW_EN1_RD(data);
}

void apm_gmac_tx_flow_control(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Modify value to set or reset tx flow control */
	if (enable) 
		data |= TX_FLOW_EN1_MASK;
	else 
		data &= ~TX_FLOW_EN1_MASK;

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,MAC_CONFIG_1_ADDR, data);
}

int apm_gmac_is_rx_enable(struct apm_data_priv *priv)
{
	u32 data;
	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);
	return RX_EN1_RD(data) ? 1 : 0;
}

void apm_gmac_rx_enable(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC,
		       MAC_CONFIG_1_ADDR, &data);

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MAC_CONFIG_1_ADDR,RX_EN1_SET(data, 1));
}

void apm_gmac_rx_disable(struct apm_data_priv *priv)
{
	u32 data;
	
	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);
	
	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MAC_CONFIG_1_ADDR, RX_EN1_SET(data, 0));
}

void apm_gmac_tx_enable(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MAC_CONFIG_1_ADDR, TX_EN1_SET(data, 1));
}

void apm_gmac_tx_disable(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC,
			MAC_CONFIG_1_ADDR, TX_EN1_SET(data, 0));
}

void apm_gmac_set_preamble_length(struct apm_data_priv *priv, u8 length)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, 
			PREAMBLE_LENGTH2_SET(data, length));
}

void apm_gmac_set_intf_mode(struct apm_data_priv *priv, u8 intf_mode)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, 
			INTERFACE_MODE2_SET(data, intf_mode));
}

int  apm_gmac_get_intf_mode(struct apm_data_priv *priv)
{
	u32 data;
	
	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);
	return INTERFACE_MODE2_RD(data);
}

void apm_gmac_huge_frame_enable(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Modify value to set or reset huge frame enable bit */
	if (enable) 
		data |= HUGE_FRAME_EN2_MASK;
	else 
		data &= ~HUGE_FRAME_EN2_MASK;

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, data);
}

void apm_gmac_len_field_check_enable(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Modify value to set or reset length field check enable bit */
	if (enable)
		data |= LENGTH_CHECK2_MASK;
	else
		data &= ~LENGTH_CHECK2_MASK;

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, data);
}

void apm_gmac_pad_crc_enable(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Modify value to set or reset pad and crc check enable bit */
	if (enable) 
		data |= PAD_CRC2_MASK;
	else 
		data &= ~PAD_CRC2_MASK;

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, data);
}

int  apm_gmac_get_pad_crc_mode(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	return  PAD_CRC2_RD(data);
}

void apm_gmac_crc_enable(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Modify value to set or reset crc check enable bit */
	if (enable) 
		data |= CRC_EN2_MASK;
	else 
		data &= ~CRC_EN2_MASK;

	/* Write value back to config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, data);
}

int  apm_gmac_get_crc_mode(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	return  CRC_EN2_RD(data);
}

int apm_gmac_get_full_duplex_mode(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the MAC Interface Status register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, INTERFACE_STATUS_ADDR, &data);

	return ((data & FULL_DUPLEX_MASK) ? 1 : 0);
}

void apm_gmac_full_duplex_enable(struct apm_data_priv *priv, u8 enable)
{
	u32 data;

	/* Read current value of the MAC config 2 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, &data);

	/* Modify value to set or reset full duplex enable bit */
	if (enable) 
		data |= FULL_DUPLEX2_MASK;
	else 
		data &= ~FULL_DUPLEX2_MASK;

	/* Write value back to MAC config 2 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_2_ADDR, data);
}

void apm_gmac_set_min_ipg(struct apm_data_priv *priv, u16 min_ifg)
{
	u32 data;

	/* Read current value of the IPG IFG register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, IPG_IFG_ADDR, &data);

	/* Write value back to IPG IFG register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, IPG_IFG_ADDR, 
			MIN_IFG_ENFORCE_SET(data, min_ifg));
}

void apm_gmac_set_ipg(struct apm_data_priv *priv, u16 ipg)
{
	u32 data;

	/* Read current value of the IPG IFG register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, IPG_IFG_ADDR, &data);

	/* Write value back to IPG IFG register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, IPG_IFG_ADDR, 
			B2B_IPG_SET(data, ipg));
			
	ENET_DEBUG("Setting IPG to %d bits", ipg);			
}

void apm_gmac_set_mgnt_clock(struct apm_data_priv *priv, u8 clk)
{
	u32 data;

	/* Read current value of the mii_mgmt_config register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MII_MGMT_CONFIG_ADDR, &data);

	/* Write value back to	mii_mgmt_config	 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MII_MGMT_CONFIG_ADDR, 
			MGMT_CLOCK_SEL_SET(data, clk));
}

void apm_gmac_change_mtu(struct apm_data_priv *priv, u32 new_mtu)
{
	u32 data;

	/* Read current value of the Maximum Frame Length register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAX_FRAME_LEN_ADDR, &data);

	/* Write value back to Maximum Frame Length register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAX_FRAME_LEN_ADDR, 
			MAX_FRAME_LEN_SET(data, new_mtu));
	priv->hw_mtu = new_mtu - 4; //don't count 4 byte eth CRC
}

void apm_gmac_full_tx_reset(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Perform MAC TX Reset */
	data |= (RESET_TX_MC1_MASK | RESET_TX_FUN1_MASK);
	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, data);
}

void apm_gmac_full_rx_reset(struct apm_data_priv *priv)
{
	u32 data;

	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);

	/* Perform MAC TX Reset */
	data |= (RESET_RX_MC1_MASK | RESET_RX_FUN1_MASK);

	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, data);
}

void apm_gmac_full_reset(struct apm_data_priv *priv)
{
	u32 data;
	
	/* Read current value of the config 1 register */
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &data);
	
	data |= (RESET_RX_MC1_MASK   |
		 RESET_RX_FUN1_MASK  |
		 RESET_TX_MC1_MASK   |
		 RESET_TX_FUN1_MASK  |
		 SIM_RESET1_MASK     |
		 SOFT_RESET1_MASK);
	
	/* Write value back to config 1 register */
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, data);
}

void apm_gmac_access_phy(struct apm_data_priv *priv)
{
	u32 value;
	u32 mac_base_addr_v = priv->mac_base_addr_v;
	static u32 apm_enet0_mac_init_done = 0;

	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &value);
	if (!(value & SOFT_RESET1_MASK)) {
		return;
	}

	if (priv->port && !apm_enet0_mac_init_done) {
		priv->mac_base_addr_v = priv->enet_mii_base_addr_v;
		apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &value);
		if (!(value & SOFT_RESET1_MASK)) {
			priv->mac_base_addr_v = mac_base_addr_v;
			apm_enet0_mac_init_done = 1;
		}
	}

_reset_gmac_access_phy:
	/* Reset MAC subsystem */
	value = RESET_TX_FUN1_WR(1) |
			RESET_RX_FUN1_WR(1) |
			RESET_TX_MC1_WR(1)  |
			RESET_RX_MC1_WR(1)  |
			SIM_RESET1_WR(1)    |
			SOFT_RESET1_WR(1);
    
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, value);
	apm_emac_read32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, &value);
	udelay(100);
	apm_emac_write32(priv, BLOCK_ETH_MAC, MAC_CONFIG_1_ADDR, 0);

	if (priv->port && !apm_enet0_mac_init_done) {
		priv->mac_base_addr_v = mac_base_addr_v;
		apm_enet0_mac_init_done = 1;
		goto _reset_gmac_access_phy;
	}
}

int apm_gmac_get_link_status(struct apm_data_priv *priv)
{
	u32 data;

	apm_emac_read32(priv, BLOCK_ETH_MAC, INTERFACE_STATUS_ADDR, &data);
	return (data & LINK_OK_MASK) ? 1 : 0;
}

/* Phy realted functions */

int apm_get_phy_link_status(struct apm_data_priv *priv)
{
	return 1;

	u32 data;
	
	apm_genericmiiphy_read(priv, priv->phy_addr,
				RTL_PHYSR_ADR, &data);
	PHY_PRINT("RTL_PHYSR_ADR data: 0x%x \n", data);
	return RTL_PHYSR_LINK_RD(data);
}

int apm_get_link_speed(struct apm_data_priv *priv)
{
	return SPEED_1000;

	u32 data;
	u32 speed = priv->speed;

	if (speed == SPEED_10 || speed == SPEED_100)
		return speed;
	return SPEED_1000;
	apm_genericmiiphy_read(priv, priv->phy_addr,
				RTL_PHYSR_ADR, &data);
	PHY_PRINT("RTL_BMSR_ADR, data: 0x%x \n", data);
	speed = RTL_PHYSR_SPEED_RD(data);
	PHY_PRINT("Phy Speed is :%d \n", speed);

	switch(speed) {
	case PHY_SPEED_10:
		return SPEED_10;

	case PHY_SPEED_100:
		return SPEED_100;

	default:
		return SPEED_1000;
	}
}

/* End of Phy realted functions */

/* Start Statistics related functions */

void apm_gmac_get_brief_stats(struct apm_data_priv *priv,
				struct eth_brief_stats *brief_stats)
{
	struct eth_detailed_stats detailed_stats;

	apm_gmac_get_detailed_stats(priv, &detailed_stats);

	brief_stats->rx_byte_count = detailed_stats.rx_stats.rx_byte_count;
	brief_stats->rx_packet_count = detailed_stats.rx_stats.rx_packet_count;
	brief_stats->rx_drop_pkt_count =
		detailed_stats.rx_stats.rx_drop_pkt_count;
	brief_stats->tx_byte_count = detailed_stats.tx_stats.tx_byte_count;
	brief_stats->tx_pkt_count = detailed_stats.tx_stats.tx_pkt_count;
	brief_stats->tx_drop_frm_count =
		detailed_stats.tx_stats.tx_drop_frm_count;
}

void apm_gmac_get_detailed_stats(struct apm_data_priv *priv,
					struct eth_detailed_stats *stats)
{
	apm_get_tx_rx_combined_stats(priv, &(stats->eth_combined_stats));
	apm_get_rx_stats(priv, &(stats->rx_stats));
	apm_get_tx_stats(priv, &(stats->tx_stats));
}

void apm_get_tx_rx_combined_stats(struct apm_data_priv *priv,
					struct eth_frame_stats *tx_rx_stats)
{
	/* Read Stats */
	apm_emac_read32(priv, BLOCK_ETH_STATS, TR64_ADDR,
				 &tx_rx_stats->c_64B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TR127_ADDR,
				 &tx_rx_stats->c_65_127B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TR255_ADDR,
				 &tx_rx_stats->c_128_255B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TR511_ADDR,
				 &tx_rx_stats->c_256_511B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TR1K_ADDR,
				 &tx_rx_stats->c_512_1023B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TRMAX_ADDR,
				 &tx_rx_stats->c_1024_1518B_frames);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TRMGV_ADDR,
				 &tx_rx_stats->c_1519_1522B_frames);

	/* Mask out unnecessary bits in all the fields */
	tx_rx_stats->c_64B_frames &= TX_RX_64B_FRAME_CNTR4_MASK;
	tx_rx_stats->c_65_127B_frames &= TX_RX_127B_FRAME_CNTR7_MASK;
	tx_rx_stats->c_128_255B_frames &= TX_RX_255B_FRAME_CNTR5_MASK;
	tx_rx_stats->c_256_511B_frames &= TX_RX_511B_FRAME_CNTR1_MASK;
	tx_rx_stats->c_512_1023B_frames &= TX_RX_1KB_FRAME_CNTR_MASK;
	tx_rx_stats->c_1024_1518B_frames &= TX_RX_MAXB_FRAME_CNTR_MASK;
	tx_rx_stats->c_1519_1522B_frames &= TRMGV_MASK;
}

void apm_get_rx_stats(struct apm_data_priv *priv, struct eth_rx_stat *rx_stat)
{
	/* Read Stats */
	apm_emac_read32(priv, BLOCK_ETH_STATS, RBYT_ADDR,
				 &rx_stat->rx_byte_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RPKT_ADDR,
				 &rx_stat->rx_packet_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RFCS_ADDR,
				 &rx_stat->rx_fcs_err_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RMCA_ADDR,
				 &rx_stat->rx_multicast_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RBCA_ADDR,
				 &rx_stat->rx_broadcast_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RXCF_ADDR,
				 &rx_stat->rx_cntrl_frame_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RXPF_ADDR,
				 &rx_stat->rx_pause_frame_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RXUO_ADDR,
				 &rx_stat->rx_unknown_op_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RALN_ADDR,
				 &rx_stat->rx_alignment_err_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RFLR_ADDR,
				 &rx_stat->rx_frm_len_err_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RCDE_ADDR,
				 &rx_stat->rx_code_err_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RCSE_ADDR,
				 &rx_stat->rx_carrier_sense_err_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RUND_ADDR,
				 &rx_stat->rx_undersize_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, ROVR_ADDR,
				 &rx_stat->rx_oversize_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RFRG_ADDR,
				 &rx_stat->rx_fragment_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RJBR_ADDR,
				 &rx_stat->rx_jabber_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, RDRP_ADDR,
				 &rx_stat->rx_drop_pkt_count);

	/* Mask out unnecessary bits in all the fields */
	rx_stat->rx_byte_count &= RX_BYTE_CNTR_MASK;
	rx_stat->rx_packet_count &= RX_PKT_CNTR_MASK;
	rx_stat->rx_fcs_err_count &= RX_FCS_ERROR_CNTR_MASK;
	rx_stat->rx_multicast_pkt_count &= RX_MC_PKT_CNTR_MASK;
	rx_stat->rx_broadcast_pkt_count &= RX_BC_PKT_CNTR_MASK;
	rx_stat->rx_cntrl_frame_pkt_count &= RX_CTRL_PKT_CNTR_MASK;
	rx_stat->rx_pause_frame_pkt_count &= RX_PAUSE_PKT_CNTR_MASK;
	rx_stat->rx_unknown_op_pkt_count &= RX_UNK_OPCODE_CNTR_MASK;
	rx_stat->rx_alignment_err_pkt_count &= RX_ALIGN_ERR_CNTR_MASK;
	rx_stat->rx_frm_len_err_pkt_count &= RX_LEN_ERR_CNTR_MASK;
	rx_stat->rx_code_err_pkt_count &= RX_CODE_ERR_CNTR_MASK;
	rx_stat->rx_carrier_sense_err_pkt_count &= RX_FALSE_CARRIER_CNTR_MASK;
	rx_stat->rx_undersize_pkt_count &= RX_UNDRSIZE_PKT_CNTR_MASK;
	rx_stat->rx_oversize_pkt_count &= RX_OVRSIZE_PKT_CNTR_MASK;
	rx_stat->rx_fragment_count &= RX_FRAG_CNTR_MASK;
	rx_stat->rx_jabber_count &= RX_JABBER_CNTR_MASK;
	rx_stat->rx_drop_pkt_count &= RX_DROPPED_PKT_CNTR_MASK;
}

void apm_get_tx_stats(struct apm_data_priv *priv,
			struct eth_tx_stats *tx_stats)
{
	/* Read Stats */
	apm_emac_read32(priv, BLOCK_ETH_STATS, TBYT_ADDR,
				 &tx_stats->tx_byte_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TPKT_ADDR,
				 &tx_stats->tx_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TMCA_ADDR,
				 &tx_stats->tx_multicast_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TBCA_ADDR,
				 &tx_stats->tx_broadcast_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TXPF_ADDR,
				 &tx_stats->tx_pause_frame_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TDFR_ADDR,
				 &tx_stats->tx_deferral_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TEDF_ADDR,
				 &tx_stats->tx_exesiv_def_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TSCL_ADDR,
				 &tx_stats->tx_single_coll_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TMCL_ADDR,
				 &tx_stats->tx_multi_coll_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TLCL_ADDR,
				 &tx_stats->tx_late_coll_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TXCL_ADDR,
				 &tx_stats->tx_exesiv_coll_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TNCL_ADDR,
				 &tx_stats->tx_toll_coll_pkt_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TPFH_ADDR,
				 &tx_stats->tx_pause_frm_hon_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TDRP_ADDR,
				 &tx_stats->tx_drop_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TJBR_ADDR,
				 &tx_stats->tx_jabber_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TFCS_ADDR,
				 &tx_stats->tx_fcs_err_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TXCF_ADDR,
				 &tx_stats->tx_control_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TOVR_ADDR,
				 &tx_stats->tx_oversize_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TUND_ADDR,
				 &tx_stats->tx_undersize_frm_count);
	apm_emac_read32(priv, BLOCK_ETH_STATS, TFRG_ADDR,
				 &tx_stats->tx_fragments_frm_count);

	/* Mask values with appropriate width of the fields */
	tx_stats->tx_byte_count &= TX_BYTE_CNTR_MASK;
	tx_stats->tx_pkt_count &= TX_PKT_CNTR_MASK;
	tx_stats->tx_multicast_pkt_count &= TX_MC_PKT_CNTR_MASK;
	tx_stats->tx_broadcast_pkt_count &= TX_BC_PKT_CNTR_MASK;
	tx_stats->tx_pause_frame_count &= TX_PAUSE_PKT_CNTR_MASK;
	tx_stats->tx_deferral_pkt_count &= TX_DEFER_PKT_CNTR_MASK;
	tx_stats->tx_exesiv_def_pkt_count &= TX_EXC_DEFER_PKT_CNTR_MASK;
	tx_stats->tx_single_coll_pkt_count &= TX_COL_PKT_CNTR_MASK;
	tx_stats->tx_multi_coll_pkt_count &= TX_MUL_COL_PKT_CNTR_MASK;
	tx_stats->tx_late_coll_pkt_count &= TX_LATE_COL_PKT_CNTR_MASK;
	tx_stats->tx_exesiv_coll_pkt_count &= TX_EXC_COL_PKT_CNTR_MASK;
	tx_stats->tx_toll_coll_pkt_count &= TX_TOTAL_COL_CNTR_MASK;
	tx_stats->tx_pause_frm_hon_count &= TX_PAUSE_FRAME_CNTR_MASK;
	tx_stats->tx_drop_frm_count &= TX_DROP_FRAME_CNTR_MASK;
	tx_stats->tx_jabber_frm_count &= TX_JABBER_FRAME_CNTR_MASK;
	tx_stats->tx_fcs_err_frm_count &= TX_FCS_ERROR_CNTR_MASK;
	tx_stats->tx_control_frm_count &= TX_CTRL_FRAME_CNTR_MASK;
	tx_stats->tx_oversize_frm_count &= TX_OVRSIZE_FRAME_CNTR_MASK;
	tx_stats->tx_undersize_frm_count &= TX_UNDSIZE_FRAME_CNTR_MASK;
	tx_stats->tx_fragments_frm_count &= TX_FRAG_CNTR_MASK;
}

int apm_enet_mss(struct apm_data_priv *priv, int mss)
{
	u32 value;
	
	apm_enet_rd32(priv, BLOCK_ETH_GBL, TSIF_MSS_REG0_0_ADDR, &value);
	apm_enet_wr32(priv, BLOCK_ETH_GBL, TSIF_MSS_REG0_0_ADDR,
			CFG_TSIF_MSS_SZ00_SET(value, mss));
	return 0;
}
