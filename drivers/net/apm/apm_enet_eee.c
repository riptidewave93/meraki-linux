/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
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
 * @file apm_enet_eee.c
 *
 * This file implements sysfs interface for Energy Efficient Ethernet
 * functionality of APM862xx SoC Ethernet subsystem
 *
 */

#ifdef CONFIG_SYSFS
#include "apm_enet_eee.h"

extern struct net_device *devices[MAX_PORTS];

#define CONFIG_SYS_MPA_GPIO_CSR_BASE		0xddd8b0000ULL	/* MPA_GPIO_CS */
#define CONFIG_SYS_MPA_GPIO_BASE		CONFIG_SYS_MPA_GPIO_CSR_BASE 

#define DEBUG_MPA_REGMOD 1
#define mpa_gpio_wr(reg, d)	\
  do { *(volatile unsigned int *)ioremap(CONFIG_SYS_MPA_GPIO_BASE + reg, sizeof(unsigned int)) = d; } while (0)
#define mpa_gpio_rd(reg, d)	\
  do { d = *(volatile unsigned int *)ioremap(CONFIG_SYS_MPA_GPIO_BASE + reg, sizeof(unsigned int)); } while (0)
 
static u32 mpa_gpio_regmod(u32 addr, u32 data, u8 bl, u8 bh)
{
	u32 reg, mask;

	mask = ~(0xffffffff << (32 - bl));
	data &= mask;
	data = data << (31 - bh);
	
	/*Preparing register mask*/
        mask = ~(0xffffffff << (32 - bl));
        mask &= (0xffffffff << (31 - bh));
        mask = ~(mask);

	 mpa_gpio_rd(addr, reg);
#if defined(DEBUG_MPA_REGMOD)
	printk("%s: reg 0x%x is 0x%x set 0x%x from %d to %d\n", __func__, addr, reg, data, bl, bh);
#endif
	reg &= mask;
	reg |= data;
	 mpa_gpio_wr(addr, reg);

#if defined(DEBUG_MPA_REGMOD)
	printk("%s: setting 0x%x to 0x%x\n", __func__, addr, reg);
#endif
	return 0;
}

static u32 mpa_gpio_regread (u32 addr, int bl, int bh) 
{
	u32 mask = 0;
	int i;
  u32 regval = 0;    
#if defined(DEBUG_MPA_REGMOD)
	printk("%s: @ 0x%08x,%d,%d(", __func__, addr,bl,bh);
#endif

         mpa_gpio_rd(addr, regval);
#if defined(DEBUG_MPA_REGMOD)
       printk("@ 0x%08x: 0x%08x)",addr,regval);
#endif
        for(i=0; i<(bh - bl + 1); i++) {
                mask |= (1 << i) ;
        }
        regval = regval >> (31 - bh);
        regval &= mask;
#if defined(DEBUG_MPA_REGMOD)
        printk(" = %x (mask = %08x)",regval,mask);
#endif
        return regval;
}

static ssize_t apm_enet_eee_enable_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, 
		"echo <port> <phy> <disable_clock> to go into eee mode\n");
}

static ssize_t apm_enet_eee_enable_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port, phy;
	u32 data;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	int disable_clock;

	ret = sscanf(buf, "%d %d %d", &port, &phy, &disable_clock);
	
	if (ret != 3) {
		printk(KERN_ERR "echo <port> <phy> <disable_clock> \n");
		return count;
	}
	
	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;
#if 0 
	reg = 31; data = 0x0000;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 13; data = 0x0003;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 14; data = 0x0000;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 13; data = 0x4003;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 14; data = 0x0400;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
#endif 
	if (port == 0) {
		/* use defaults for now */
		apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_TW_TIMER_0, &data);
		printk("EEE_TW_TIMER_0 = 0x%08x\n", data);

		/* use defaults for now */
		apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL,
			      EEE_LPI_WAIT_PATTERNS_0, &data);
		printk("EEE_LPI_WAIT_PATTERNS_0 = 0x%08x\n", data);

		apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, &data);
		/* use default cfg_lpi_clk_stop_after_ncycles for now */
		data  = EEE_REG_CFG_LPI_CLK_STOPPABLE |
			EEE_REG_CFG_LPI_MODE |
			0x804;  /* 16.5us */
		apm_enet_wr32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, data);
		apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, &data);
		printk("EEE_REG_0 = 0x%08x\n", data);
		if (disable_clock) {
			apm86xxx_write_scu_reg(SCU_SOCDIV4_ADDR, 0x0004007F);
			apm86xxx_read_scu_reg(SCU_SOCDIV4_ADDR, &data);
			printk("SCU_SOCDIV4_ADDR = 0x%08x\n", data);
		}
	} else {
		apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_1, &data);
		/* use default cfg_lpi_clk_stop_after_ncycles for now */
		data |= EEE_REG_CFG_LPI_CLK_STOPPABLE;
		data |= EEE_REG_CFG_LPI_MODE;
		apm_enet_wr32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_1, data);
	}
        return count;
}

static ssize_t apm_enet_eee_disable_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, 
		"echo <port> <phy> <enable_clock>  to get out of eee mode\n");
}

static ssize_t apm_enet_eee_disable_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port, phy;
	u32 data;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	int enable_clock;

	ret = sscanf(buf, "%d %d %d", &port, &phy, &enable_clock);
	if (ret != 3) {
		printk(KERN_ERR "echo <port> <phy> <enable_clock> \n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;
#if 0 
	reg = 31; data = 0x0000;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 13; data = 0x0003;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 14; data = 0x0000;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 13; data = 0x4003;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
	reg = 14; data = 0x0000;
	printk("set phy=0x%02x reg=0x%02x data=0x%04x\n", phy, reg, data);
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);
#endif
	if (enable_clock) {
		apm86xxx_write_scu_reg(SCU_SOCDIV4_ADDR, 0x00040004);
		apm86xxx_read_scu_reg(SCU_SOCDIV4_ADDR, &data);
		printk("SCU_SOCDIV4_ADDR = 0x%08x\n", data);
	}
	apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, &data);
	data &= ~EEE_REG_CFG_LPI_CLK_STOPPABLE;
	data &= ~EEE_REG_CFG_LPI_MODE;
	apm_enet_wr32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, data);
	apm_enet_rd32(priv, BLOCK_ETH_MAC_GBL, EEE_REG_0, &data);
	printk("EEE_REG_0 = 0x%08x\n", data);
        return count;
}
/************************************************************************************/
static ssize_t apm_enet_gpio14_set_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, 
		"echo <port> <gpio> <enable|disable> to enable GPIO14\n");
}

static ssize_t apm_enet_gpio14_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;

	char status[10];
	ret = sscanf(buf, "%d  %s",&port,&status[0]);
	if (ret != 2) {
		printk(KERN_ERR "echo <port> <enable|disable> \n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}
	priv = &priv_dev->priv;
	
	if (strcmp(status, "enable") == 0)
	{
	 mpa_gpio_regmod(0xa94, 1,17,17); /* To enable GPIO14 to switch MDC clock to U26 */
	 printk("\nTo enable GPIO14 to switch MDC clock to U26\n\n");
	}else	if (strcmp(status, "disable") == 0){
		mpa_gpio_regmod(0xa94, 0,17,17);/* To enable GPIO14 to switch MDC clock to U28 */
	  printk("\nTo disable GPIO14 to switch MDC clock to U28\n\n");
	}else{
		printk(KERN_ERR "Invalid State\n");
	}			
	return count;
}	
/*
	AR8033(U30) SMI address 0x5
	AR8033(U31) SMI address 0x6
	REG TYPE:  
	reg - general MII register
	dbg - debug register
	mmd -
	MMD3 ¡V MDIO Manageable Device Address 3 for PCS
  MMD7 ¡V MDIO Manageable Device Address 7 for Auto-Negotiation
*/
static ssize_t apm_enet_phy_read_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <reg_type> <phy_addr> <reg_addr>\n");
}

static ssize_t apm_enet_phy_read_set(struct device_driver *drv,
				    const char *buf, size_t count)
{
	int ret;
	int port;
	u32	phy_addr = 0, reg_addr = 0, mmd_addr = 3;
	u32	reg_data = 0;
	char reg_type[10];	
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	
	ret = sscanf(buf, "%x  %s %x %x", &port,&reg_type[0],&phy_addr,&reg_addr);
	
	if (ret != 4) {
		printk(KERN_ERR "echo <port> <reg_type> <phy_addr> <reg_addr>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}
	priv = &priv_dev->priv;
	
	if (strcmp(reg_type, "reg") == 0){
		apm_genericmiiphy_read(priv, (u8)phy_addr, (u8)reg_addr, &reg_data);	
		printk("\n\nRead From AR8033 PHY Register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, (reg_data&0xffff));	
	}else if (strcmp(reg_type, "dbg") == 0){
		apm_genericmiiphy_write(priv, (u8)phy_addr, 0x1d,reg_addr);
		apm_genericmiiphy_read(priv, (u8)phy_addr, 0x1e,&reg_data);			
		printk("\n\nRead From AR8033 PHY Register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, (reg_data&0xffff));
	}else if ((strcmp(reg_type, "mmd3") == 0)||(strcmp(reg_type, "mmd7") == 0)){
		
		if(strcmp(reg_type, "mmd7")==0){
			 mmd_addr = 0x07;
		}	 
		else	if(strcmp(reg_type, "mmd3")==0){
			 mmd_addr = 0x03;
		}
		apm_genericmiiphy_write(priv, (u8)phy_addr,0x0d,mmd_addr);
		apm_genericmiiphy_write(priv, (u8)phy_addr,0x0e,reg_addr);
		apm_genericmiiphy_write(priv, (u8)phy_addr,0x0d,(mmd_addr|0X4000));	
		apm_genericmiiphy_read(priv, (u8)phy_addr, 0x0e, &reg_data);		
		printk("\n\nRead From AR8033 MMD PHY Register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, (reg_data&0xffff));
	}else{
			printk("\n>>>>>>>> Invalid Reg Type <<<<<<<<<<<<<<<<\n");
	}	
	return count;
	
}
static ssize_t apm_enet_phy_write_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <reg_type> <phy_addr> <reg_addr> <reg_data>\n");
}

static ssize_t apm_enet_phy_write_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port;
	u32	phy_addr = 0, reg_addr = 0, mmd_addr = 3;
	u32	reg_data = 0;
	char reg_type[5];	
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;

	ret = sscanf(buf, "%x  %s %x  %x %x", &port,&reg_type[0],&phy_addr,&reg_addr,&reg_data);
	if (ret != 5) {
			printk(KERN_ERR "echo <port> <reg_type> <phy_addr> <reg_addr> <reg_data>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;
	if (strcmp(reg_type, "reg") == 0){	
		apm_genericmiiphy_write(priv, (u8)phy_addr,reg_addr, reg_data);	
		printk("\n\nWrite to  AR8033 PHY register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, reg_data);				
	}else if (strcmp(reg_type, "dbg") == 0){
		apm_genericmiiphy_write(priv, (u8)phy_addr, 0x1d,reg_addr);
  	apm_genericmiiphy_write(priv, (u8)phy_addr, 0x1e,reg_data);
		printk("\n\nWrite to AR8033 PHY register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, reg_data);	
	}else if ((strcmp(reg_type, "mmd3") == 0)||(strcmp(reg_type, "mmd7") == 0)){
		
		if(strcmp(reg_type, "mmd7")==0){
			 mmd_addr = 0x07;
		}	 
		else	if(strcmp(reg_type, "mmd3")==0){
			 mmd_addr = 0x03;
		}
		apm_genericmiiphy_write(priv, (u8) phy_addr,0x0d,mmd_addr);
		apm_genericmiiphy_write(priv, (u8) phy_addr,0x0e,reg_addr);
		apm_genericmiiphy_write(priv, (u8) phy_addr,0x0d,(mmd_addr|0X4000));																	
		apm_genericmiiphy_write(priv, (u8) phy_addr,0x0e,reg_data);
		printk("\n\nWrite to AR8033 MMD PHY register:PHY Address = 0x%x Reg Address = 0x%x ,Reg Data = %04X\n",phy_addr,reg_addr, reg_data);		
		
	}else{
			printk("\n>>>>>>>> Invalid Reg Type <<<<<<<<<<<<<<<<\n");
	}	
	
	return count;
	 
	 
}
		
static ssize_t apm_enet_switch_read_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <reg>\n");
}

static ssize_t apm_enet_switch_read_set(struct device_driver *drv,
				    const char *buf, size_t count)
{
	int ret;
	int port;
	u32 reg_addr;
	u32 reg_word_addr;
	u32 phy_addr, tmp_val, reg_val;
	u16 phy_val;
	u8 phy_reg;
	
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	
	ret = sscanf(buf, "%x %x", &port, &reg_addr);
	
	if (ret != 2) {
		printk(KERN_ERR "echo <port> <reg>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;
	
	
	
		/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr& 0xfffffffc) >> 1;

	/* configure register high address */
	phy_addr = 0x18;
	phy_reg = 0x0;
	phy_val = (u16) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */
	apm_genericmiiphy_write(priv, (u8)phy_addr, (u8)phy_reg, phy_val);
	
	/* For some registers such as MIBs, since it is read/clear, we should */
	/* read the lower 16-bit register then the higher one */

	/* read register in lower address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (u8) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */	
	apm_genericmiiphy_read(priv, (u8)phy_addr, (u8)phy_reg, &reg_val);
	
	/* read register in higher address */
	reg_word_addr++;
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (u8) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
	apm_genericmiiphy_read(priv, (u8)phy_addr, (u8)phy_reg, &tmp_val);	
	reg_val |= (tmp_val << 16);
	printk("Read Switch Port %d, Reg 0x%x , Reg value 0x%x\n",port,reg_addr,reg_val);

        return count;
}

static ssize_t apm_enet_switch_write_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <reg> <data>\n");
}

static ssize_t apm_enet_switch_write_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port, reg_addr;
	u32 reg_word_addr;
	u32 reg_val;
	u32 phy_addr;
	u16 phy_val;
	u8 phy_reg;
	
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;

	ret = sscanf(buf, "%d %x %x", &port, &reg_addr, &reg_val);
	if (ret != 3) {
		printk(KERN_ERR "echo <port> <reg_addr> <reg_val>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;


	/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

	/* configure register high address */
	phy_addr = 0x18;
	phy_reg = 0x0;
	phy_val = (u16) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */

	apm_genericmiiphy_write(priv, (u8)phy_addr, (u8)phy_reg, phy_val);
	/* For S17 registers such as ARL and VLAN, since they include BUSY bit */
	/* in higher address, we should write the lower 16-bit register then the */
	/* higher one */

	/* write register in lower address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (u8) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
	phy_val = (u16) (reg_val & 0xffff);	
	apm_genericmiiphy_write(priv, (u8)phy_addr, (u8)phy_reg, phy_val);
	/* read register in higher address */
	reg_word_addr++;
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (u8) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
	phy_val = (u16) ((reg_val >> 16) & 0xffff);
	apm_genericmiiphy_write(priv, (u8)phy_addr, (u8)phy_reg, phy_val);
 printk("Write Switch Port = %d, Reg Addr = 0x%x , Reg value = 0x%x\n",port,reg_addr,reg_val);
        return count;
}

/*******************************************************************************************/
static ssize_t apm_enet_miiwrite_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <phy> <reg> <data>\n");
}

static ssize_t apm_enet_miiwrite_set(struct device_driver *drv,
				     const char *buf, size_t count)
{
	int ret;
	int port, phy, reg;
	u32 data;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;

	ret = sscanf(buf, "%d %d %d %d", &port, &phy, &reg, &data);
	if (ret != 4) {
		printk(KERN_ERR "echo <port> <phy> <reg> <data>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;

	printk("phy=0x%02x reg=0x%02x data=0x%04x\n",
		phy, reg, (data & 0x0000FFFF));
	apm_genericmiiphy_write(priv, (u8)phy, (u8)reg, data);

        return count;
}

static ssize_t apm_enet_miiread_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <phy> <reg>\n");
}

static ssize_t apm_enet_miiread_set(struct device_driver *drv,
				    const char *buf, size_t count)
{
	int ret;
	int port, phy, reg;
	u32 data;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	
	ret = sscanf(buf, "%d %d %d", &port, &phy, &reg);
	
	if (ret != 3) {
		printk(KERN_ERR "echo <port> <phy> <reg>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;

	apm_genericmiiphy_read(priv, (u8)phy, (u8)reg, &data);
	printk("phy=0x%02x reg=0x%02x data=0x%04x\n",
		phy, reg, (data & 0x0000FFFF));

        return count;
}

static ssize_t apm_enet_miidump_show(struct device_driver *drv, char *buf)
{
	return sprintf(buf, "echo <port> <phy>\n");
}

static ssize_t apm_enet_miidump_set(struct device_driver *drv,
				    const char *buf, size_t count)
{
	int ret;
	int port, phy, reg;
	u32 data;
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;

	ret = sscanf(buf, "%d %d", &port, &phy);
	if (ret != 2) {
		printk(KERN_ERR "echo <port> <phy>\n");
		return count;
	}

	if (port > MAX_PORTS) {
		printk(KERN_ERR "Invalid port\n");
		return count;
	}

	ndev = devices[port];
	if (ndev == NULL) {
		printk(KERN_ERR "Couldn't get net device\n");
		return count;
	}

	priv_dev = netdev_priv(ndev);
	if (priv_dev == NULL) {
		printk(KERN_ERR "Couldn't get device\n");
		return count;
	}

	priv = &priv_dev->priv;

	for (reg = 0; reg < 32; reg++) {
		apm_genericmiiphy_read(priv, (u8)phy, (u8)reg, &data);
		printk("phy=0x%02x reg=0x%02x data=0x%04x\n",
			phy, reg, (data & 0x0000FFFF));
	}

        return count;
}

static struct driver_attribute apm_enet_attrs[] = {
	__ATTR(eee_enable, S_IWUGO | S_IRUGO,
		apm_enet_eee_enable_show, apm_enet_eee_enable_set),
	__ATTR(eee_disable, S_IWUGO | S_IRUGO,
		apm_enet_eee_disable_show, apm_enet_eee_disable_set),
	__ATTR(mii_write, S_IWUGO | S_IRUGO,
		apm_enet_miiwrite_show, apm_enet_miiwrite_set),
	__ATTR(mii_read, S_IWUGO | S_IRUGO,
		apm_enet_miiread_show, apm_enet_miiread_set),
	__ATTR(mii_dump, S_IWUGO | S_IRUGO,
		apm_enet_miidump_show, apm_enet_miidump_set),		
	__ATTR(switch_read, S_IWUGO | S_IRUGO,
		apm_enet_switch_read_show, apm_enet_switch_read_set),		
	__ATTR(switch_write, S_IWUGO | S_IRUGO,
		apm_enet_switch_write_show, apm_enet_switch_write_set),	
	__ATTR(phy_read, S_IWUGO | S_IRUGO,
		apm_enet_phy_read_show, apm_enet_phy_read_set),		
	__ATTR(phy_write, S_IWUGO | S_IRUGO,
		apm_enet_phy_write_show, apm_enet_phy_write_set),	
	__ATTR(gpio14, S_IWUGO | S_IRUGO,
		apm_enet_gpio14_set_show, apm_enet_gpio14_set),		

};

int apm_enet_add_sysfs(struct device_driver *driver)
{
	int i;
	int err;
	static int added_done = 0;

	if (added_done)
		return 0;
	
	for (i = 0; i < ARRAY_SIZE(apm_enet_attrs); i++) {
		err = driver_create_file(driver, &apm_enet_attrs[i]);
		if (err)
			goto fail;
	}

	added_done = 1;

	return 0;
fail:
	while (--i >= 0)
		driver_remove_file(driver, &apm_enet_attrs[i]);
	return err;
}

void apm_enet_remove_sysfs(struct device_driver *driver)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(apm_enet_attrs); i++)
		driver_remove_file(driver, &apm_enet_attrs[i]);
}

#endif /* CONFIG_SYSFS */
