/*
 * ESS reset
 *
 * extracted from qca-ssdk/src/init/ssdk_init.c, which is
 *   Copyright (c) 2014 - 2016, The Linux Foundation. All rights reserved.
 */

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/phy.h>
#include <linux/timer.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/string.h>
#include <linux/reset.h>
#include "edma.h"

extern u8  __iomem      *ess_hw_addr;
extern u8  __iomem      *psgmii_hw_addr;
extern struct mii_bus   *miibus_gb;

#define QCA8035_PHY_DEBUG_PORT_ADDR          0x1D
#define QCA8035_PHY_DEBUG_PORT_DATA          0x1E
#define QCA8035_PHY_DEBUG_REG_ANALOG_CTRL    0x00
#define QCA8035_PHY_DEBUG_REG_SERDES_MODE    0x05
#define QCA8035_PHY_DEBUG_REG_HIBERNATE_CTRL 0x0B

static void
qca_ar8327_phy_write(u32 phy_addr, u32 reg, u16 data)
{
	mdiobus_write(miibus_gb, phy_addr, reg, data);
}

static void ssdk_ess_reset(struct platform_device *pdev)
{
	struct reset_control *ess_rst;
	u32 regval;
	struct device_node *np = pdev->dev.of_node;

	ess_rst = devm_reset_control_get(&pdev->dev, "ess_rst");
	if (IS_ERR(ess_rst)) {
		printk("ess rst fail!\n");
		return;
	}

	reset_control_assert(ess_rst);
	mdelay(10);
	reset_control_deassert(ess_rst);
	mdelay(100);
	printk("reset ok in probe!\n");

	if (of_property_read_bool(np, "qcom,extra_phy_reset")) {
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_ADDR, QCA8035_PHY_DEBUG_REG_SERDES_MODE);
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_DATA, 0x2d47);
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_ADDR, QCA8035_PHY_DEBUG_REG_HIBERNATE_CTRL);
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_DATA, 0xbc40);
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_ADDR, QCA8035_PHY_DEBUG_REG_ANALOG_CTRL);
		qca_ar8327_phy_write(0, QCA8035_PHY_DEBUG_PORT_DATA, 0x82ee);
	}

	/* turn off all forwarding */
#define SET_PORT_BMP(r,v)				\
	ess_read_reg(r, &regval);			\
	ess_write_reg(r, ((regval & ~0x3F) | v));

	SET_PORT_BMP(ESS_PORT0_LOOKUP_CTRL,0x00);
	SET_PORT_BMP(ESS_PORT1_LOOKUP_CTRL,0x00);
	SET_PORT_BMP(ESS_PORT2_LOOKUP_CTRL,0x00);
	SET_PORT_BMP(ESS_PORT3_LOOKUP_CTRL,0x00);
	SET_PORT_BMP(ESS_PORT4_LOOKUP_CTRL,0x00);
	SET_PORT_BMP(ESS_PORT5_LOOKUP_CTRL,0x00);

#undef SET_PORT_BMP
}

static void
qca_ar8327_phy_read(u32 phy_addr, u32 reg, u16* data)
{
	*data = mdiobus_read(miibus_gb, phy_addr, reg);
}

static u16
qca_phy_mmd_read(u32 phy_id, u16 mmd_num, u16 reg_id)
{
	u16 value = 0;
	qca_ar8327_phy_write(phy_id, MII_MMD_CTRL, mmd_num);
	qca_ar8327_phy_write(phy_id, MII_MMD_DATA, reg_id);
	qca_ar8327_phy_write(phy_id, MII_MMD_CTRL, 0x4000 | mmd_num);
	qca_ar8327_phy_read(phy_id, MII_MMD_DATA, &value);
	return value;
}

static void
qca_phy_mmd_write(u32 phy_id, u16 mmd_num, u16 reg_id, u16 reg_val)
{
	qca_ar8327_phy_write(phy_id, MII_MMD_CTRL, mmd_num);
	qca_ar8327_phy_write(phy_id, MII_MMD_DATA, reg_id);
	qca_ar8327_phy_write(phy_id, MII_MMD_CTRL, 0x4000 | mmd_num);
	qca_ar8327_phy_write(phy_id, MII_MMD_DATA, reg_val);
}

static void ssdk_malibu_psgmii_and_dakota_dess_reset(struct platform_device *pdev)
{
	int m = 0, n = 0;

	/*reset Malibu PSGMII and Dakota ESS start*/
	qca_ar8327_phy_write(5, 0x0, 0x005b);/*fix phy psgmii RX 20bit*/
	qca_ar8327_phy_write(5, 0x0, 0x001b);/*reset phy psgmii*/
	qca_ar8327_phy_write(5, 0x0, 0x005b);/*release reset phy psgmii*/
	/* mdelay(100); this 100ms be replaced with below malibu psgmii calibration process*/
	/*check malibu psgmii calibration done start*/
	n = 0;
	while (n < 100) {
		u16 status;
		status = qca_phy_mmd_read(5, 1, 0x28);
		if (status & BIT(0))
			break;
		mdelay(10);
		n++;
	}

	if (n >= 100)
		printk("MALIBU PSGMII PLL_VCO_CALIB NOT READY\n");

	mdelay(50);
	/*check malibu psgmii calibration done end..*/
	qca_ar8327_phy_write(5, 0x1a, 0x2230);/*freeze phy psgmii RX CDR*/

	ssdk_ess_reset(pdev);
	/*check dakota psgmii calibration done start*/
	m = 0;
	while (m < 100) {
		u32 status = readl(psgmii_hw_addr + 0xa0);
		if (status & BIT(0))
			break;
		mdelay(10);
		m++;
	}

	if (m >= 100)
		printk("DAKOTA PSGMII PLL_VCO_CALIB NOT READY\n");

	mdelay(50);
	/*check dakota psgmii calibration done end..*/
	qca_ar8327_phy_write(5, 0x1a, 0x3230); /*release phy psgmii RX CDR*/
	qca_ar8327_phy_write(5, 0x0, 0x005f);  /*release phy psgmii RX 20bit*/
	mdelay(200);

	/*reset Malibu PSGMII and Dakota ESS end*/
	return;
}

static u32 phy_t_status = 0;
void ssdk_psgmii_single_phy_testing(int phy)
{
	int j = 0;

	u32 tx_counter_ok, tx_counter_error;
	u32 rx_counter_ok, rx_counter_error;
	u32 tx_counter_ok_high16;
	u32 rx_counter_ok_high16;
	u32 tx_ok, rx_ok;

	qca_ar8327_phy_write(phy, 0x0, 0x9000);
	qca_ar8327_phy_write(phy, 0x0, 0x4140);
	for (j = 0; j < 100; j++) {
		u16 status;
		qca_ar8327_phy_read(phy, 0x11, &status);
		if (status & (1 << 10))
			break;
		mdelay(10);
	}

	/*enable check*/
	qca_phy_mmd_write(phy, 7, 0x8029, 0x0000);
	qca_phy_mmd_write(phy, 7, 0x8029, 0x0003);

	/*start traffic*/
	qca_phy_mmd_write(phy, 7, 0x8020, 0xa000);
	mdelay(200);

	/*check counter*/
	tx_counter_ok = qca_phy_mmd_read(phy, 7, 0x802e);
	tx_counter_ok_high16 = qca_phy_mmd_read(phy, 7, 0x802d);
	tx_counter_error = qca_phy_mmd_read(phy, 7, 0x802f);
	rx_counter_ok = qca_phy_mmd_read(phy, 7, 0x802b);
	rx_counter_ok_high16 = qca_phy_mmd_read(phy, 7, 0x802a);
	rx_counter_error = qca_phy_mmd_read(phy, 7, 0x802c);
	tx_ok = tx_counter_ok + (tx_counter_ok_high16<<16);
	rx_ok = rx_counter_ok + (rx_counter_ok_high16<<16);
	if (tx_ok == 0x3000 && tx_counter_error == 0) {
		/*success*/
		phy_t_status &= (~(1<<phy));
	} else {
		phy_t_status |= (1<<phy);
	}

	qca_ar8327_phy_write(phy, 0x0, 0x1840);
}

static void ssdk_psgmii_all_phy_testing(void)
{
	int phy = 0, j = 0;

	qca_ar8327_phy_write(0x1f, 0x0, 0x9000);
	qca_ar8327_phy_write(0x1f, 0x0, 0x4140);
	j = 0;
	for (j = 0; j < 100; j++) {
		for (phy = 0; phy < 5; phy++) {
			u16 status;
			qca_ar8327_phy_read(phy, 0x11, &status);
			if (!(status & (1 << 10)))
				break;
		}

		if (phy >= 5)
			break;
		mdelay(10);
	}

	/*enable check*/
	qca_phy_mmd_write(0x1f, 7, 0x8029, 0x0000);
	qca_phy_mmd_write(0x1f, 7, 0x8029, 0x0003);

	/*start traffic*/
	qca_phy_mmd_write(0x1f, 7, 0x8020, 0xa000);
	mdelay(200);
	for (phy = 0; phy < 5; phy++) {
		u32 tx_counter_ok, tx_counter_error;
		u32 rx_counter_ok, rx_counter_error;
		u32 tx_counter_ok_high16;
		u32 rx_counter_ok_high16;
		u32 tx_ok, rx_ok;
		/*check counter*/
		tx_counter_ok        = qca_phy_mmd_read(phy, 7, 0x802e);
		tx_counter_ok_high16 = qca_phy_mmd_read(phy, 7, 0x802d);
		tx_counter_error     = qca_phy_mmd_read(phy, 7, 0x802f);
		rx_counter_ok        = qca_phy_mmd_read(phy, 7, 0x802b);
		rx_counter_ok_high16 = qca_phy_mmd_read(phy, 7, 0x802a);
		rx_counter_error     = qca_phy_mmd_read(phy, 7, 0x802c);
		tx_ok = tx_counter_ok + (tx_counter_ok_high16<<16);
		rx_ok = rx_counter_ok + (rx_counter_ok_high16<<16);
		if (tx_ok== 0x3000 && tx_counter_error == 0) {
			/*success*/
			phy_t_status &= (~(1<<(phy+8)));
		} else {
			phy_t_status |= (1<<(phy+8));
		}
	}
}

static void ssdk_psgmii_self_test(struct platform_device *pdev)
{
	int i = 0, phy = 0, times = 100;
	u32 value = 0;

	ssdk_malibu_psgmii_and_dakota_dess_reset(pdev);

	qca_ar8327_phy_write(4, 0x1f, 0x8500);/*switch to access MII reg for copper*/
	for(phy = 0; phy < 5; phy++) {
		/*enable phy mdio broadcast write*/
		qca_phy_mmd_write(phy, 7, 0x8028, 0x801f);
	}

	/* force no link by power down */
	qca_ar8327_phy_write(0x1f, 0x0, 0x1840);

	/*packet number*/
	qca_phy_mmd_write(0x1f, 7, 0x8021, 0x3000);
	qca_phy_mmd_write(0x1f, 7, 0x8062, 0x05e0);

	/*fix mdi status */
	qca_ar8327_phy_write(0x1f, 0x10, 0x6800);

	for(i = 0; i < times; i++) {
		phy_t_status = 0;

		for(phy = 0; phy < 5; phy++) {
			value = readl(ess_hw_addr + 0x66c + phy * 0xc);
			writel((value|(1<<21)), (ess_hw_addr + 0x66c + phy * 0xc));
		}

		for (phy = 0; phy < 5; phy++)
			ssdk_psgmii_single_phy_testing(phy);

		ssdk_psgmii_all_phy_testing();
		if (!phy_t_status)
			break;

		ssdk_malibu_psgmii_and_dakota_dess_reset(pdev);
	}

	if (i>=100)
		printk("PSGMII cannot recover\n");
	else
		printk("PSGMII recovered after %d times reset\n", i);

	/*configuration recover*/
	/*packet number*/
	qca_phy_mmd_write(0x1f, 7, 0x8021, 0x0);
	/*disable check*/
	qca_phy_mmd_write(0x1f, 7, 0x8029, 0x0);
	/*disable traffic*/
	qca_phy_mmd_write(0x1f, 7, 0x8020, 0x0);
}

static void clear_self_test_config(void)
{
	int phy = 0;
	u32 value = 0;
	/* disable EEE */
/*	qca_phy_mmd_write(0, 0x1f, 0x7,  0x3c, 0x0); */

	/*disable phy internal loopback*/
	qca_ar8327_phy_write(0x1f, 0x10, 0x6860);
	qca_ar8327_phy_write(0x1f, 0x0, 0x9040);

	for (phy = 0; phy < 5; phy++) {
		/*disable mac loop back*/
		value = readl(ess_hw_addr+0x66c+phy*0xc);
		writel((value&(~(1<<21))), (ess_hw_addr+0x66c+phy*0xc));
		/*disable phy mdio broadcast write*/
		qca_phy_mmd_write(phy, 7, 0x8028, 0x001f);
	}

	/* clear fdb entry */
	/* fal_fdb_del_all(0,1); */
}

void ess_reset(struct platform_device *pdev)
{
	struct device_node* np = pdev->dev.of_node;

	if (of_property_read_bool(np, "qcom,self_test")) {
		ssdk_psgmii_self_test(pdev);
		clear_self_test_config();
	} else {
		ssdk_ess_reset(pdev);
	}
}
