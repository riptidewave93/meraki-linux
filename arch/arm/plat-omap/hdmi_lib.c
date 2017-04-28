/*
 * hdmi_lib.c
 *
 * HDMI library support functions for TI OMAP processors.
 *
 * Copyright (C) 2010 Texas Instruments
 * Author: Yong Zhi <y-zhi@ti.com>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Rev history:
 * Yong Zhi <y-zhi@ti.com>	changed SiVal macros
 *				added PLL/PHY code
 *				added EDID code
 *				moved PLL/PHY code to hdmi panel driver
 *				cleanup 2/08/10
 * MythriPk <mythripk@ti.com>	Apr 2010 Modified to read extended EDID
 *				partition and handle checksum with and without
 *				extension
 *				May 2010 Added support for Hot Plug Detect.
 * Varada Bellary <varadab@ti.com>
 *				Retrofit the OMAP driver for DM81xx SOCs
 * Sujith Shivalingappa <sujith.s@ti.com>
 *				Added CEC support
 *
 */

#define DSS_SUBSYS_NAME "HDMI"

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <plat/hdmi_lib.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <sound/pcm.h>
#include <linux/hrtimer.h>
#include <linux/ti81xxhdmi.h>

/* HDMI PHY */
#define HDMI_TXPHY_TX_CTRL			0x0ul
#define HDMI_TXPHY_DIGITAL_CTRL			0x4ul
#define HDMI_TXPHY_POWER_CTRL			0x8ul

/* HDMI Wrapper */
#define HDMI_WP_REVISION			0x0ul
#define HDMI_WP_SYSCONFIG			0x10ul
#define HDMI_WP_IRQSTATUS_RAW		0x24ul
#define HDMI_WP_IRQSTATUS			0x28ul
#define HDMI_WP_IRQWAKEEN			0x34ul
#define HDMI_WP_PWR_CTRL			0x40ul
#define HDMI_WP_IRQENABLE_SET		0x2Cul
#define HDMI_WP_VIDEO_CFG			0x50ul
#define HDMI_WP_VIDEO_SIZE			0x60ul
#define HDMI_WP_VIDEO_TIMING_H		0x68ul
#define HDMI_WP_VIDEO_TIMING_V		0x6Cul
#define HDMI_WP_WP_CLK				0x70ul

/* HDMI IP Core System */
#define HDMI_CORE_SYS__VND_IDL		0x0ul
#define HDMI_CORE_SYS__DEV_IDL		0x8ul
#define HDMI_CORE_SYS__DEV_IDH		0xCul
#define HDMI_CORE_SYS__DEV_REV		0x10ul
#define HDMI_CORE_SYS__SRST			0x14ul
#define HDMI_CORE_CTRL1				0x20ul
#define HDMI_CORE_SYS__SYS_STAT		0x24ul
#define HDMI_CORE_SYS__VID_ACEN			0x124ul
#define HDMI_CORE_SYS__VID_MODE			0x128ul
#define HDMI_CORE_SYS__VID_CTRL			0x120ul
#define HDMI_CORE_SYS__INTR_STATE		0x1C0ul
#define HDMI_CORE_SYS__INTR1			0x1C4ul
#define HDMI_CORE_SYS__INTR2			0x1C8ul
#define HDMI_CORE_SYS__INTR3			0x1CCul
#define HDMI_CORE_SYS__INTR4			0x1D0ul
#define HDMI_CORE_SYS__UMASK1			0x1D4ul
#define HDMI_CORE_SYS__TMDS_CTRL		0x208ul
#define HDMI_CORE_CTRL1_VEN__FOLLOWVSYNC	0x1ul
#define HDMI_CORE_CTRL1_HEN__FOLLOWHSYNC	0x1ul
#define HDMI_CORE_CTRL1_BSEL__24BITBUS		0x1ul
#define HDMI_CORE_CTRL1_EDGE__RISINGEDGE	0x1ul

#define HDMI_CORE_SYS__DE_DLY			0xC8ul
#define HDMI_CORE_SYS__DE_CTRL			0xCCul
#define HDMI_CORE_SYS__DE_TOP			0xD0ul
#define HDMI_CORE_SYS__DE_CNTL			0xD8ul
#define HDMI_CORE_SYS__DE_CNTH			0xDCul
#define HDMI_CORE_SYS__DE_LINL			0xE0ul
#define HDMI_CORE_SYS__DE_LINH__1		0xE4ul

#define HDMI_CORE_SYS__HBIT_2HSYNC1		0x100ul
#define HDMI_CORE_SYS__HBIT_2HSYNC2		0x104ul
#define HDMI_CORE_SYS__FLD2_HS_OFSTL	0x108ul
#define HDMI_CORE_SYS__FLD2_HS_OFSTH	0x10cul
#define HDMI_CORE_SYS__HWIDTH1			0x110ul
#define HDMI_CORE_SYS__HWIDTH2			0x114ul
#define HDMI_CORE_SYS__VBIT_TO_VSYNC	0x118ul
#define HDMI_CORE_SYS__VWIDTH			0x11cul



/* HDMI IP Core Audio Video */
#define HDMI_CORE_AV_HDMI_CTRL			0xBCul
#define HDMI_CORE_AV_DPD				0xF4ul
#define HDMI_CORE_AV_PB_CTRL1			0xF8ul
#define HDMI_CORE_AV_PB_CTRL2			0xFCul
#define HDMI_CORE_AV_AVI_TYPE			0x100ul
#define HDMI_CORE_AV_AVI_VERS			0x104ul
#define HDMI_CORE_AV_AVI_LEN			0x108ul
#define HDMI_CORE_AV_AVI_CHSUM			0x10Cul
#define HDMI_CORE_AV_AVI_DBYTE			0x110ul
#define HDMI_CORE_AV_AVI_DBYTE__ELSIZE	0x4ul

/* HDMI DDC E-DID */
#define HDMI_CORE_DDC_CMD				0x3CCul
#define HDMI_CORE_DDC_STATUS			0x3C8ul
#define HDMI_CORE_DDC_ADDR				0x3B4ul
#define HDMI_CORE_DDC_OFFSET			0x3BCul
#define HDMI_CORE_DDC_COUNT1			0x3C0ul
#define HDMI_CORE_DDC_COUNT2			0x3C4ul
#define HDMI_CORE_DDC_DATA				0x3D0ul
#define HDMI_CORE_DDC_SEGM				0x3B8ul

#define HDMI_WP_AUDIO_CFG			0x80ul
#define HDMI_WP_AUDIO_CFG2			0x84ul
#define HDMI_WP_AUDIO_CTRL			0x88ul
#define HDMI_WP_AUDIO_DATA			0x8Cul

#define HDMI_CORE_AV__AVI_DBYTE				0x110ul
#define HDMI_CORE_AV__AVI_DBYTE__ELSIZE		0x4ul
#define HDMI_IP_CORE_AV__AVI_DBYTE__NELEMS	15
#define HDMI_CORE_AV__SPD_DBYTE				0x190ul
#define HDMI_CORE_AV__SPD_DBYTE__ELSIZE		0x4ul
#define HDMI_CORE_AV__SPD_DBYTE__NELEMS		27
#define HDMI_CORE_AV__AUDIO_DBYTE			0x210ul
#define HDMI_CORE_AV__AUDIO_DBYTE__ELSIZE	0x4ul
#define HDMI_CORE_AV__AUDIO_DBYTE__NELEMS	10
#define HDMI_CORE_AV__MPEG_DBYTE			0x290ul
#define HDMI_CORE_AV__MPEG_DBYTE__ELSIZE	0x4ul
#define HDMI_CORE_AV__MPEG_DBYTE__NELEMS	27
#define HDMI_CORE_AV__GEN_DBYTE				0x300ul
#define HDMI_CORE_AV__GEN_DBYTE__ELSIZE		0x4ul
#define HDMI_CORE_AV__GEN_DBYTE__NELEMS		31
#define HDMI_CORE_AV__GEN2_DBYTE			0x380ul
#define HDMI_CORE_AV__GEN2_DBYTE__ELSIZE	0x4ul
#define HDMI_CORE_AV__GEN2_DBYTE__NELEMS	31
#define HDMI_CORE_AV__ACR_CTRL			0x4ul
#define HDMI_CORE_AV__FREQ_SVAL			0x8ul
#define HDMI_CORE_AV__N_SVAL1			0xCul
#define HDMI_CORE_AV__N_SVAL2			0x10ul
#define HDMI_CORE_AV__N_SVAL3			0x14ul
#define HDMI_CORE_AV__CTS_SVAL1			0x18ul
#define HDMI_CORE_AV__CTS_SVAL2			0x1Cul
#define HDMI_CORE_AV__CTS_SVAL3			0x20ul
#define HDMI_CORE_AV__CTS_HVAL1			0x24ul
#define HDMI_CORE_AV__CTS_HVAL2			0x28ul
#define HDMI_CORE_AV__CTS_HVAL3			0x2Cul
#define HDMI_CORE_AV__AUD_MODE			0x50ul
#define HDMI_CORE_AV__SPDIF_CTRL		0x54ul
#define HDMI_CORE_AV__HW_SPDIF_FS		0x60ul
#define HDMI_CORE_AV__SWAP_I2S			0x64ul
#define HDMI_CORE_AV__SPDIF_ERTH		0x6Cul
#define HDMI_CORE_AV__I2S_IN_MAP		0x70ul
#define HDMI_CORE_AV__I2S_IN_CTRL		0x74ul
#define HDMI_CORE_AV__I2S_CHST0			0x78ul
#define HDMI_CORE_AV__I2S_CHST1			0x7Cul
#define HDMI_CORE_AV__I2S_CHST2			0x80ul
#define HDMI_CORE_AV__I2S_CHST4			0x84ul
#define HDMI_CORE_AV__I2S_CHST5			0x88ul
#define HDMI_CORE_AV__ASRC				0x8Cul
#define HDMI_CORE_AV__I2S_IN_LEN		0x90ul
#define HDMI_CORE_AV__HDMI_CTRL			0xBCul
#define HDMI_CORE_AV__AUDO_TXSTAT		0xC0ul
#define HDMI_CORE_AV__AUD_PAR_BUSCLK_1		0xCCul
#define HDMI_CORE_AV__AUD_PAR_BUSCLK_2		0xD0ul
#define HDMI_CORE_AV__AUD_PAR_BUSCLK_3		0xD4ul
#define HDMI_CORE_AV__TEST_TXCTRL			0xF0ul
#define HDMI_CORE_AV__DPD				0xF4ul
#define HDMI_CORE_AV__PB_CTRL1			0xF8ul
#define HDMI_CORE_AV__PB_CTRL2			0xFCul
#define HDMI_CORE_AV__AVI_TYPE			0x100ul
#define HDMI_CORE_AV__AVI_VERS			0x104ul
#define HDMI_CORE_AV__AVI_LEN			0x108ul
#define HDMI_CORE_AV__AVI_CHSUM			0x10Cul
#define HDMI_CORE_AV__SPD_TYPE			0x180ul
#define HDMI_CORE_AV__SPD_VERS			0x184ul
#define HDMI_CORE_AV__SPD_LEN			0x188ul
#define HDMI_CORE_AV__SPD_CHSUM			0x18Cul
#define HDMI_CORE_AV__AUDIO_TYPE		0x200ul
#define HDMI_CORE_AV__AUDIO_VERS		0x204ul
#define HDMI_CORE_AV__AUDIO_LEN			0x208ul
#define HDMI_CORE_AV__AUDIO_CHSUM		0x20Cul
#define HDMI_CORE_AV__MPEG_TYPE			0x280ul
#define HDMI_CORE_AV__MPEG_VERS			0x284ul
#define HDMI_CORE_AV__MPEG_LEN			0x288ul
#define HDMI_CORE_AV__MPEG_CHSUM		0x28Cul
#define HDMI_CORE_AV__CP_BYTE1			0x37Cul
#define HDMI_CORE_AV__CEC_ADDR_ID		0x3FCul

/* CEC */
#define HDMI_CORE_CEC_DEV_ID			0x00ul
#define HDMI_CORE_CEC_SPEC				0x04ul
#define HDMI_CORE_CEC_SUFF				0x08ul
#define HDMI_CORE_CEC_FW				0x0Cul
#define HDMI_CORE_CEC_DBG_3				0x1Cul
#define HDMI_CORE_CEC_TX_INIT			0x20ul
#define HDMI_CORE_CEC_TX_DEST			0x24ul
#define HDMI_CORE_CEC_SET_UP			0x38ul
#define HDMI_CORE_CEC_TX_COMMAND		0x3Cul
#define HDMI_CORE_CEC_TX_OPERAND		0x40ul
#define HDMI_CORE_CEC_TRANSMIT_DATA		0x7Cul
#define HDMI_CORE_CEC_CA_7_0			0x88ul
#define HDMI_CORE_CEC_CA_15_8			0x8Cul
#define HDMI_CORE_CEC_INT_STATUS_0		0x98ul
#define HDMI_CORE_CEC_INT_STATUS_1		0x9Cul
#define HDMI_CORE_CEC_RX_CONTROL		0xB0ul
#define HDMI_CORE_CEC_RX_COUNT			0xB4ul
#define HDMI_CORE_CEC_RX_CMD_HEAD		0xB8ul
#define HDMI_CORE_CEC_RX_COMMAND		0xBCul
#define HDMI_CORE_CEC_RX_OPERAND		0xC0ul


/* CEC wait timeouts configurations */
#define HDMI_CORE_CEC_TIMEOUT			200


#define HDMI_WP_IRQSTATUS_CORE 0x00000001
#define HDMI_WP_IRQSTATUS_PHYCONNECT 0x02000000
#define HDMI_WP_IRQSTATUS_PHYDISCONNECT 0x04000000
#define HDMI_WP_IRQSTATUS_OCPTIMEOUT 0x00000010

#define HDMI_CORE_SYS__SYS_STAT_HPD 0x02

#define HDMI_IP_CORE_SYSTEM__INTR2__BCAP	0x80
#define HDMI_IP_CORE_SYSTEM__INTR3__RI_ERR	0xF0

bool first_hpd, dirty;

static struct {
	void __iomem *base_core;	/* 0 */
	void __iomem *base_core_av;	/* 1 */
	void __iomem *base_wp;		/* 2 */
	void __iomem *base_core_cec;	/* 3 */
	struct hdmi_core_infoframe_avi avi_param;
	struct hdmi_config hdmi_cfg;
	struct mutex mutex;
	struct list_head notifier_head;
} hdmi;

static inline void hdmi_write_reg(u32 base, u16 idx, u32 val)
{
	void __iomem *b;

	switch (base) {
	case HDMI_CORE_SYS:
		b = hdmi.base_core;
		break;
	case HDMI_CORE_AV:
		b = hdmi.base_core_av;
		break;
	case HDMI_WP:
		b = hdmi.base_wp;
		break;
	case HDMI_CORE_CEC:
		b = hdmi.base_core_cec;
		break;
	default:
		BUG();
	}
	__raw_writel(val, b + idx);
	/* DBG("write = 0x%x idx =0x%x\r\n", val, idx); */
}

static inline u32 hdmi_read_reg(u32 base, u16 idx)
{
	void __iomem *b;
	u32 l;

	switch (base) {
	case HDMI_CORE_SYS:
		b = hdmi.base_core;
		break;
	case HDMI_CORE_AV:
		b = hdmi.base_core_av;
		break;
	case HDMI_WP:
		b = hdmi.base_wp;
		break;
	case HDMI_CORE_CEC:
		b = hdmi.base_core_cec;
		break;
	default:
		BUG();
	}

	l = __raw_readl(b + idx);

	/* DBG("addr = 0x%p rd = 0x%x idx = 0x%x\r\n", (b+idx), l, idx); */
	return l;
}

void hdmi_dump_regs(struct seq_file *s)
{
#define DUMPREG(g, r) seq_printf(s, "%-35s %08x\n", #r, hdmi_read_reg(g, r))

	/* wrapper registers */
	DUMPREG(HDMI_WP, HDMI_WP_REVISION);
	DUMPREG(HDMI_WP, HDMI_WP_SYSCONFIG);
	DUMPREG(HDMI_WP, HDMI_WP_IRQSTATUS_RAW);
	DUMPREG(HDMI_WP, HDMI_WP_IRQSTATUS);
	DUMPREG(HDMI_WP, HDMI_WP_PWR_CTRL);
	DUMPREG(HDMI_WP, HDMI_WP_IRQENABLE_SET);
	DUMPREG(HDMI_WP, HDMI_WP_VIDEO_SIZE);
	DUMPREG(HDMI_WP, HDMI_WP_VIDEO_TIMING_H);
	DUMPREG(HDMI_WP, HDMI_WP_VIDEO_TIMING_V);
	DUMPREG(HDMI_WP, HDMI_WP_WP_CLK);

	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__VND_IDL);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DEV_IDL);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DEV_IDH);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DEV_REV);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__SRST);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__SYS_STAT);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_ACEN);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_MODE);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR_STATE);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR1);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR2);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR3);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR4);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__UMASK1);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__TMDS_CTRL);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_CTRL1_VEN__FOLLOWVSYNC);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_CTRL1_HEN__FOLLOWHSYNC);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_CTRL1_BSEL__24BITBUS);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_CTRL1_EDGE__RISINGEDGE);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_CTRL);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_TOP);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_CNTH);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_LINL);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_LINH__1);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_SYS__DE_DLY);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_CMD);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_STATUS);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_ADDR);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_OFFSET);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_COUNT1);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_COUNT2);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_DATA);
	DUMPREG(HDMI_CORE_SYS, HDMI_CORE_DDC_SEGM);

	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_DBYTE);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__ACR_CTRL);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_TYPE);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_VERS);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_LEN);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_CHSUM);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__HDMI_CTRL);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_DBYTE);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_DBYTE);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_DBYTE);
	DUMPREG(HDMI_CORE_AV, HDMI_CORE_AV__AVI_DBYTE);
}

#define FLD_MASK(start, end)	(((1 << ((start) - (end) + 1)) - 1) << (end))
#define FLD_VAL(val, start, end) (((val) << (end)) & FLD_MASK(start, end))
#define FLD_GET(val, start, end) (((val) & FLD_MASK(start, end)) >> (end))
#define FLD_MOD(orig, val, start, end) \
	(((orig) & ~FLD_MASK(start, end)) | FLD_VAL(val, start, end))

#define REG_FLD_MOD(base, idx, val, start, end) \
	hdmi_write_reg(base, idx, \
		FLD_MOD(hdmi_read_reg(base, idx), val, start, end))

#define RD_REG_32(COMP, REG)            hdmi_read_reg(COMP, REG)
#define WR_REG_32(COMP, REG, VAL)       hdmi_write_reg(COMP, REG, (u32)(VAL))

int hdmi_core_ddc_edid(u8 *pEDID, int ext)
{
	u32 i, j, l = 0;
	char checksum = 0;
	u32 sts = HDMI_CORE_DDC_STATUS;
	u32 ins = HDMI_CORE_SYS;
	u32 offset = 0;

	/* Turn on CLK for DDC */
	REG_FLD_MOD(HDMI_CORE_AV, HDMI_CORE_AV_DPD, 0x7, 2, 0);

	/* Wait */
	msleep(10);
	if (!ext) {
		/* Clk SCL Devices */
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0xA, 3, 0);

		/* HDMI_CORE_DDC_STATUS__IN_PROG */
		while (FLD_GET(hdmi_read_reg(ins, sts), 4, 4) == 1);

		/* Clear FIFO */
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0x9, 3, 0);

		/* HDMI_CORE_DDC_STATUS__IN_PROG */
		while (FLD_GET(hdmi_read_reg(ins, sts), 4, 4) == 1)
			;
	} else {
		if (ext%2 != 0)
			offset = 0x80;
	}


	/* Load Segment Address Register */
	REG_FLD_MOD(ins, HDMI_CORE_DDC_SEGM, ext/2, 7, 0);

	/* Load Slave Address Register */
	REG_FLD_MOD(ins, HDMI_CORE_DDC_ADDR, 0xA0 >> 1, 7, 1);

	/* Load Offset Address Register */
	REG_FLD_MOD(ins, HDMI_CORE_DDC_OFFSET, offset, 7, 0);
	/* Load Byte Count */
	REG_FLD_MOD(ins, HDMI_CORE_DDC_COUNT1, 0x80, 7, 0);
	REG_FLD_MOD(ins, HDMI_CORE_DDC_COUNT2, 0x0, 1, 0);
	/* Set DDC_CMD */
	if (ext)
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0x4, 3, 0);
	else
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0x2, 3, 0);

	/*
	 * Yong: do not optimize this part of the code, seems
	 * DDC bus needs some time to get stabilized
	 */

	/* I2C bus slow. So need some additional wait : Varada */
	for(i=0;i<2;i++){
	l = hdmi_read_reg(ins, sts);
	msleep(102);
	}

	/* HDMI_CORE_DDC_STATUS__BUS_LOW */
	if (FLD_GET(l, 6, 6) == 1) {
		printk("I2C Bus Low?\n\r");
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0xf, 3, 0);
		return -1;
	}
	/* HDMI_CORE_DDC_STATUS__NO_ACK */
	if (FLD_GET(l, 5, 5) == 1) {
		printk("I2C No Ack\n\r");
		/* Abort transaction */
		REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0xf, 3, 0);
		while (FLD_GET(hdmi_read_reg(ins, sts), 4, 4) == 1);
		hdmi_read_reg(ins, sts);
		return -1;
	}

	i = ext * 128;
	j = 0;
	while (((FLD_GET(hdmi_read_reg(ins, sts), 4, 4) == 1) ||
		(FLD_GET(hdmi_read_reg(ins, sts), 2, 2) == 0)) && j < 128) {
		if (FLD_GET(hdmi_read_reg(ins, sts), 2, 2) == 0) {
			/* FIFO not empty */
			pEDID[i++] = FLD_GET(
				hdmi_read_reg(ins, HDMI_CORE_DDC_DATA), 7, 0);
			j++;
		}
	}

	for (j = 0; j < 128; j++)
		checksum += pEDID[j];

	if (checksum != 0) {
		printk("E-EDID checksum failed!!");
		return -1;
	}

	/* Abort before exit, Varada : Check it this can be removed */
	REG_FLD_MOD(ins, HDMI_CORE_DDC_CMD, 0xf, 3, 0);
	/* Abort transaction */
	while (FLD_GET(hdmi_read_reg(ins, sts), 4, 4) == 1);
	l= hdmi_read_reg(ins, sts);

	return 0;
}

int read_edid(u8 *pEDID, u16 max_length)
{
	int r = 0, n = 0, i = 0;
	int max_ext_blocks = (max_length / 128) - 1;


	r = hdmi_core_ddc_edid(pEDID, 0);
	if (r == -1) {
		return -1;
	} else {
		n = pEDID[0x7e];

		/*
		 * README: need to comply with max_length set by the caller.
		 * Better implementation should be to allocate necessary
		 * memory to store EDID according to nb_block field found
		 * in first block
		 */

		if (n > max_ext_blocks)
			n = max_ext_blocks;

		for (i = 1; i <= n; i++) {
			r = hdmi_core_ddc_edid(pEDID, i);
			if (r)
				return -1;
		}
	}
	return 0;
}

static void hdmi_core_init(enum hdmi_deep_mode deep_color,
	enum hdmi_data_format input_df,
	enum hdmi_data_format output_df,
	struct hdmi_core_video_config_t *v_cfg,
	struct hdmi_core_audio_config *audio_cfg,
	struct hdmi_core_infoframe_avi *avi,
	struct hdmi_core_packet_enable_repeat *r_p)
{
	DBG("Enter HDMI_Core_GlobalInitVars()\n");

	/* video core */
	switch (deep_color) {
	case HDMI_DEEP_COLOR_30BIT:
		v_cfg->CoreInputBusWide = HDMI_INPUT_10BIT;
		v_cfg->CoreOutputDitherTruncation = HDMI_OUTPUTTRUNCATION_10BIT;
		v_cfg->CoreDeepColorPacketED = HDMI_DEEPCOLORPACKECTENABLE;
		v_cfg->CorePacketMode = HDMI_PACKETMODE30BITPERPIXEL;
		break;
	case HDMI_DEEP_COLOR_36BIT:
		v_cfg->CoreInputBusWide = HDMI_INPUT_12BIT;
		v_cfg->CoreOutputDitherTruncation = HDMI_OUTPUTTRUNCATION_12BIT;
		v_cfg->CoreDeepColorPacketED = HDMI_DEEPCOLORPACKECTENABLE;
		v_cfg->CorePacketMode = HDMI_PACKETMODE36BITPERPIXEL;
		break;
	case HDMI_DEEP_COLOR_24BIT:
	default:
		v_cfg->CoreInputBusWide = HDMI_INPUT_8BIT;
		v_cfg->CoreOutputDitherTruncation = HDMI_OUTPUTTRUNCATION_8BIT;
		v_cfg->CoreDeepColorPacketED = HDMI_DEEPCOLORPACKECTDISABLE;
		/* Originally : HDMI_PACKETMODERESERVEDVALUE;*/
		v_cfg->CorePacketMode = HDMI_PACKETMODE24BITPERPIXEL;
		/* ToDo Varada : is this OMAP bug ?*/
		break;
	}

	v_cfg->CoreHdmiDvi = HDMI_DVI;
	v_cfg->CoreTclkSelClkMult = FPLL10IDCK;

	/* info frame */
	avi->db1y_rgb_yuv422_yuv444 = 0;
	avi->db1a_active_format_off_on = 0;
	avi->db1b_no_vert_hori_verthori = 0;
	avi->db1s_0_1_2 = 0;
	avi->db2c_no_itu601_itu709_extented = 0;
	avi->db2m_no_43_169 = 0;
	avi->db2r_same_43_169_149 = 0;
	avi->db3itc_no_yes = 0;
	avi->db3ec_xvyuv601_xvyuv709 = 0;
	avi->db3q_default_lr_fr = 0;
	avi->db3sc_no_hori_vert_horivert = 0;
	avi->db4vic_videocode = 0;
	avi->db5pr_no_2_3_4_5_6_7_8_9_10 = 0;
	avi->db6_7_lineendoftop = 0 ;
	avi->db8_9_linestartofbottom = 0;
	avi->db10_11_pixelendofleft = 0;
	avi->db12_13_pixelstartofright = 0;

	/* packet enable and repeat */
	r_p->AudioPacketED = 0;
	r_p->AudioPacketRepeat = 0;
	r_p->AVIInfoFrameED = 0;
	r_p->AVIInfoFrameRepeat = 0;
	r_p->GeneralcontrolPacketED = 0;
	r_p->GeneralcontrolPacketRepeat = 0;
	r_p->GenericPacketED = 0;
	r_p->GenericPacketRepeat = 0;
	r_p->MPEGInfoFrameED = 0;
	r_p->MPEGInfoFrameRepeat = 0;
	r_p->SPDInfoFrameED = 0;
	r_p->SPDInfoFrameRepeat = 0;
	v_cfg->enable_downsampler = 0;
	if (input_df == HDMI_DF_YUV444 && output_df == HDMI_DF_YUV422)
		v_cfg->enable_downsampler = 1;
}

void hdmi_core_powerdown_disable(void)
{
	DBG("Enter DSS_HDMI_CORE_POWER_DOWN_DISABLE()\n");
	REG_FLD_MOD(HDMI_CORE_SYS, HDMI_CORE_CTRL1, 0x0, 0, 0);
}

/* todo: power off the core */
static __attribute__ ((unused)) void hdmi_core_powerdown_enable(void)
{
	REG_FLD_MOD(HDMI_CORE_SYS, HDMI_CORE_CTRL1, 0x1, 0, 0);
}

void hdmi_core_swreset_release(void)
{
	DBG("Enter DSS_HDMI_CORE_SW_RESET_RELEASE()\n");
	REG_FLD_MOD(HDMI_CORE_SYS, HDMI_CORE_SYS__SRST, 0x0, 0, 0);
}

void hdmi_core_swreset_assert(void)
{
	DBG("Enter DSS_HDMI_CORE_SW_RESET_ASSERT ()\n");
	REG_FLD_MOD(HDMI_CORE_SYS, HDMI_CORE_SYS__SRST, 0x1, 0, 0);
}

/* DSS_HDMI_CORE_VIDEO_CONFIG */
static int hdmi_core_video_config(struct hdmi_core_video_config_t *cfg)
{
	u32 name = HDMI_CORE_SYS;
	u32 av_name = HDMI_CORE_AV;
	u32 r = 0;

	/* sys_ctrl1 default configuration not tunable */
	u32 ven;
	u32 hen;
	u32 bsel;
	u32 edge;

	/* sys_ctrl1 default configuration not tunable */
	ven = HDMI_CORE_CTRL1_VEN__FOLLOWVSYNC;
	hen = HDMI_CORE_CTRL1_HEN__FOLLOWHSYNC;
	bsel = HDMI_CORE_CTRL1_BSEL__24BITBUS;
	edge = HDMI_CORE_CTRL1_EDGE__RISINGEDGE;

	/* sys_ctrl1 default configuration not tunable */
	r = hdmi_read_reg(name, HDMI_CORE_CTRL1);
	r = FLD_MOD(r, ven, 5, 5);
	r = FLD_MOD(r, hen, 4, 4);
	r = FLD_MOD(r, bsel, 2, 2);
	r = FLD_MOD(r, edge, 1, 1);
	/* PD bit has to be written to recieve the interrupts */
	r = FLD_MOD(r, 1, 0, 0);
	hdmi_write_reg(name, HDMI_CORE_CTRL1, r);

	REG_FLD_MOD(name, HDMI_CORE_SYS__VID_ACEN, cfg->CoreInputBusWide, 7, 6);
	REG_FLD_MOD(name, HDMI_CORE_SYS__VID_ACEN,
			cfg->enable_downsampler, 0, 0);


	/* Vid_Mode */
	r = hdmi_read_reg(name, HDMI_CORE_SYS__VID_MODE);
	/* dither truncation configuration */
	if (cfg->CoreOutputDitherTruncation > HDMI_OUTPUTTRUNCATION_12BIT) {
		r = FLD_MOD(r, cfg->CoreOutputDitherTruncation - 3, 7, 6);
		r = FLD_MOD(r, 1, 5, 5);
	} else {
		r = FLD_MOD(r, cfg->CoreOutputDitherTruncation, 7, 6);
		r = FLD_MOD(r, 0, 5, 5);
	}
	/* For TI816x and TI814x, set the SYNC based on embedded or discrete
	 */
	if (cpu_is_ti814x() || cpu_is_ti816x()) {
		if (cfg->CoreSyncFormat == HDMI_EMBEDDED_SYNC)
			r = FLD_MOD(r, 1, 0, 0);
		else
			r = FLD_MOD(r, 0, 0, 0);

	}
	hdmi_write_reg(name, HDMI_CORE_SYS__VID_MODE, r);

	/* Configure the polarity of output syncs in case HDMI is
	   receiving Embedded Syncs.
	 */
	if (cpu_is_ti814x() || cpu_is_ti816x()) {
		if (cfg->CoreSyncFormat == HDMI_EMBEDDED_SYNC) {
			r = hdmi_read_reg(name, HDMI_CORE_SYS__DE_CTRL);
			r = FLD_MOD(r, cfg->vsync_polarity, 5, 5);
			r = FLD_MOD(r, cfg->hsync_polarity, 4, 4);
			hdmi_write_reg(name, HDMI_CORE_SYS__DE_CTRL, r);
		}
	}
	/* HDMI_CTRL */
	r = hdmi_read_reg(av_name, HDMI_CORE_AV_HDMI_CTRL);
	r = FLD_MOD(r, cfg->CoreDeepColorPacketED, 6, 6);
	r = FLD_MOD(r, cfg->CorePacketMode, 5, 3);
	r = FLD_MOD(r, cfg->CoreHdmiDvi, 0, 0);
	hdmi_write_reg(av_name, HDMI_CORE_AV_HDMI_CTRL, r);


	/* TMDS_CTRL */
	REG_FLD_MOD(name, HDMI_CORE_SYS__TMDS_CTRL,
			cfg->CoreTclkSelClkMult, 6, 5);

	return 0;
}

int hdmi_core_audio_mode_enable(u32  instanceName)
{
	REG_FLD_MOD(instanceName, HDMI_CORE_AV__AUD_MODE, 1, 0, 0);
	return 0;
}

int hdmi_core_audio_config(u32 name,
		struct hdmi_core_audio_config *audio_cfg)
{
	int ret = 0;
	u32 SD3_EN = 0, SD2_EN = 0, SD1_EN = 0 , SD0_EN = 0;
	u8 DBYTE1, DBYTE2, DBYTE4, CHSUM;
	u8 size1;
	u16 size0;
	u32 r;

	/* Parameters for audio clock recovery packets */
	REG_FLD_MOD(name, HDMI_CORE_AV__N_SVAL1, audio_cfg->n, 7, 0);
	REG_FLD_MOD(name, HDMI_CORE_AV__N_SVAL2, audio_cfg->n >> 8, 7, 0);
	REG_FLD_MOD(name, HDMI_CORE_AV__N_SVAL3, audio_cfg->n >> 16, 7, 0);

	/*program the CTS values only in SW mode 1:SW, 0:HW */
	if (CTS_MODE_HW == audio_cfg->cts_mode) {
		REG_FLD_MOD(name, HDMI_CORE_AV__AUD_PAR_BUSCLK_1,
					audio_cfg->aud_par_busclk, 7, 0);
		REG_FLD_MOD(name, HDMI_CORE_AV__AUD_PAR_BUSCLK_2,
					(audio_cfg->aud_par_busclk >> 8), 7, 0);
		REG_FLD_MOD(name, HDMI_CORE_AV__AUD_PAR_BUSCLK_3,
					(audio_cfg->aud_par_busclk >> 16), 7, 0);
	} else {
		/* Software mode */
		REG_FLD_MOD(name, HDMI_CORE_AV__CTS_SVAL1,
					 audio_cfg->cts, 7, 0);
		REG_FLD_MOD(name, HDMI_CORE_AV__CTS_SVAL2,
					 audio_cfg->cts >> 8, 7, 0);
		REG_FLD_MOD(name, HDMI_CORE_AV__CTS_SVAL3,
					 audio_cfg->cts >> 16, 7, 0);
	}
	/* Update ACR clock divider - 128 FS */
	REG_FLD_MOD(name, HDMI_CORE_AV__FREQ_SVAL, 0, 2, 0);

	/* CTS_MODE */
	r = RD_REG_32(name, HDMI_CORE_AV__ACR_CTRL);

	/*
	 * Use TMDS clock for ACR packets. For devices that use
	 * the MCLK, this is the first part of the MCLK initialization.
	 */
	r = FLD_MOD(r, 0, 2, 2);

	/* Enable the CTS request enable always */
	r = FLD_MOD(r, 1, 1, 1);
	if (CTS_MODE_HW == audio_cfg->cts_mode) {
		r = FLD_MOD(r, 0, 0, 0);
	} else {
		r = FLD_MOD(r, 1, 0, 0);
	}
	WR_REG_32(name, HDMI_CORE_AV__ACR_CTRL, r);

	/* For devices using MCLK, this completes its initialization. */
	/* Please that this enable of MCLK should be done sperately */
	if (CTS_MODE_HW == audio_cfg->cts_mode)
		REG_FLD_MOD(name, HDMI_CORE_AV__ACR_CTRL, 1, 2, 2);

	/* Use sample frequency from channel status word  */
	WR_REG_32(name, HDMI_CORE_AV__SPDIF_CTRL, 0x1);

	/* Update IEC-60958-3 Channel status bits - hard coded for now */
	r = 0;
	r = 0 |			/* Bit 0 - consumer */
		(0 << 1) |	/* Bit 1 - Audio data */
		(1 << 2);	/* Bit 2 - No copyright asserted ? */

	/* Bit 3, 4,  5 - No pre-emphasis 2 channel audio  */
	WR_REG_32(name, HDMI_CORE_AV__I2S_CHST0, r);
	/* Category code - not specified */
	WR_REG_32(name, HDMI_CORE_AV__I2S_CHST1, 0);
	/* Source & channel numbers are zero ? */
	WR_REG_32(name, HDMI_CORE_AV__I2S_CHST2, 0);
	/* Refer IEC 60958 - 3 */
	switch (audio_cfg->if_fs) {
	case IF_FS_32000:
		r = 3;
		break;
	case IF_FS_44100:
		r = 0;
		break;
	case IF_FS_48000:
		r = 2;
		break;
	case IF_FS_96000:
		r = 10;
		break;
	case IF_FS_192000:
		r = 14;
		break;
	default:
		r = 3;
		break;
	}

	/* r should also have parts per million PPM, its 0, which is 1000 */
	WR_REG_32(name, HDMI_CORE_AV__I2S_CHST4, r);

	r = 0;
	if (IF_16BIT_PER_SAMPLE == audio_cfg->if_sample_size) {
		r = 1 << 1;
	} else {
		r = (1 << 1) | (1 << 0);
	}
	WR_REG_32(name, HDMI_CORE_AV__I2S_CHST5, r);

	/*
	 * audio config is mainly due to wrapper hardware connection
	 * and so are fixe (hardware) I2S deserializer is by-pass
	 * so I2S configuration is not needed (I2S don't care).
	 * Wrapper are directly connected at the I2S deserialiser
	 * output level so some register call I2S... need to be
	 * programm to configure this parallel bus, there configuration
	 * is also fixe and due to the hardware connection (I2S hardware)
	 */
	WR_REG_32(name, HDMI_CORE_AV__I2S_IN_CTRL,
		(0 << 7) |	/* HBRA_ON */
		(1 << 6) |	/* SCK_EDGE Sample clock is rising */
		(0 << 5) |	/* CBIT_ORDER */
		(0 << 4) |	/* VBit, 0x0=PCM, 0x1=compressed */
		(0 << 3) |	/* I2S_WS, Left channel on WS low */
		(audio_cfg->justify << 2) |	/* I2S_JUST, 0=left- 1=right */
		(0 << 1) |	/* I2S_DIR, MSB first */
		(0));		/* I2S_SHIFT, First bit shift */

	/*
	 * The I2S input word length is twice the lenght given in
	 * the IEC-60958 status word. If the word size is greater
	 * than 20 bits, increment by one.
	 */
	if (IF_16BIT_PER_SAMPLE == audio_cfg->if_sample_size) {
		r = (1<<1);	/* 20-bit or 16-bit */
	} else {
		r = (5<<1);	/* 24-bit or 20-bit */
		r++;		/* increment by one */
	}
	WR_REG_32(name, HDMI_CORE_AV__I2S_IN_LEN, r);

	/* Audio channel parameters */
	REG_FLD_MOD(name, HDMI_CORE_AV__HDMI_CTRL, audio_cfg->layout, 2, 1);

	/* channel enable depend of the layout */
	if (audio_cfg->layout == LAYOUT_2CH) {
		SD3_EN = 0x0;
		SD2_EN = 0x0;
		SD1_EN = 0x0;
		SD0_EN = 0x1;
	} else if (audio_cfg->layout == LAYOUT_8CH) {
		SD3_EN = 0x1;
		SD2_EN = 0x1;
		SD1_EN = 0x1;
		SD0_EN = 0x1;
	}

	/*
	 * When ever audio FIFO is used, we would require to
	 * use parallel interface only. Furthre serial
	 * interface is not available on TI814x, TI813x, etc.
	 */
	WR_REG_32(name, HDMI_CORE_AV__AUD_MODE,
		(SD3_EN << 7) |	/* SD3_EN */
		(SD2_EN << 6) |	/* SD2_EN */
		(SD1_EN << 5) |	/* SD1_EN */
		(SD0_EN << 4) |	/* SD0_EN */
		(0 << 3) |	/* DSD_EN */
		(1 << 2) |	/* AUD_PAR_EN */
		(0 << 1) |	/* SPDIF_EN - disabled not supported by HW.*/
		(0));		/* AUD_EN - Required only in case of SPDIF input. */

	/* Number of channels */
	DBYTE1 = audio_cfg->if_channel_number - 1;
	/* We support L-PCM 60958 format only */
	DBYTE1 |= 0x10;

	/*
	 * Audio info frame setting refer to CEA-861-d spec
	 * Table 18 Audio InfoFrame Data Byte 2
	 */
	DBYTE2 = (audio_cfg->if_sample_size == IF_16BIT_PER_SAMPLE) ? 0x1 : 0x3;
	DBYTE2 |= (audio_cfg->if_fs << 2);

	/* channel location according to CEA spec */
	DBYTE4 = audio_cfg->if_audio_channel_location;

	/* HDMI Spec 1.3a, 0x80+InfoFrame Type = 0x04 */
	WR_REG_32(name, HDMI_CORE_AV__AUDIO_TYPE, 0x084);
	WR_REG_32(name, HDMI_CORE_AV__AUDIO_VERS, 0x001);
	WR_REG_32(name, HDMI_CORE_AV__AUDIO_LEN, 0x00A);

	size0 = HDMI_CORE_AV__AUDIO_DBYTE;
	size1 = HDMI_CORE_AV__AUDIO_DBYTE__ELSIZE;
	hdmi_write_reg(name, (size0 + 0 * size1), DBYTE1);
	hdmi_write_reg(name, (size0 + 1 * size1), DBYTE2);
	hdmi_write_reg(name, (size0 + 2 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 3 * size1), DBYTE4);
	hdmi_write_reg(name, (size0 + 4 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 5 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 6 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 7 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 8 * size1), 0x000);
	hdmi_write_reg(name, (size0 + 9 * size1), 0x000);

	/* write the check sum last, no perticular reason. Just convention */
	CHSUM = 0x100 - (0x84 + 0x01 + 0x0A + DBYTE1 + DBYTE2 + DBYTE4);
	WR_REG_32(name, HDMI_CORE_AV__AUDIO_CHSUM, CHSUM);

	return ret;
}

int hdmi_core_read_avi_infoframe(struct hdmi_core_infoframe_avi *info_avi)
{
	info_avi->db1y_rgb_yuv422_yuv444 =
		hdmi.avi_param.db1y_rgb_yuv422_yuv444;
	info_avi->db1a_active_format_off_on =
		hdmi.avi_param.db1a_active_format_off_on;
	info_avi->db1b_no_vert_hori_verthori =
		hdmi.avi_param.db1b_no_vert_hori_verthori;
	info_avi->db1s_0_1_2 = hdmi.avi_param.db1s_0_1_2;
	info_avi->db2c_no_itu601_itu709_extented =
		hdmi.avi_param.db2c_no_itu601_itu709_extented;
	info_avi->db2m_no_43_169 = hdmi.avi_param.db2m_no_43_169;
	info_avi->db2r_same_43_169_149 = hdmi.avi_param.db2r_same_43_169_149;
	info_avi->db3itc_no_yes = hdmi.avi_param.db3itc_no_yes;
	info_avi->db3ec_xvyuv601_xvyuv709 =
		hdmi.avi_param.db3ec_xvyuv601_xvyuv709;
	info_avi->db3q_default_lr_fr = hdmi.avi_param.db3q_default_lr_fr;
	info_avi->db3sc_no_hori_vert_horivert =
		hdmi.avi_param.db3sc_no_hori_vert_horivert;
	info_avi->db4vic_videocode = hdmi.avi_param.db4vic_videocode;
	info_avi->db5pr_no_2_3_4_5_6_7_8_9_10 =
		hdmi.avi_param.db5pr_no_2_3_4_5_6_7_8_9_10;
	info_avi->db6_7_lineendoftop = hdmi.avi_param.db6_7_lineendoftop;
	info_avi->db8_9_linestartofbottom =
		hdmi.avi_param.db8_9_linestartofbottom;
	info_avi->db10_11_pixelendofleft =
		hdmi.avi_param.db10_11_pixelendofleft;
	info_avi->db12_13_pixelstartofright =
		hdmi.avi_param.db12_13_pixelstartofright;

	return 0;
}

static int hdmi_core_audio_infoframe_avi(
				struct hdmi_core_infoframe_avi info_avi)
{
	u16 offset;
	int dbyte, dbyte_size;
	u32 val;
	char sum = 0, checksum = 0;

	dbyte = HDMI_CORE_AV_AVI_DBYTE;
	dbyte_size = HDMI_CORE_AV_AVI_DBYTE__ELSIZE;
	/* info frame video */
	sum += 0x82 + 0x002 + 0x00D;
	hdmi_write_reg(HDMI_CORE_AV, HDMI_CORE_AV_AVI_TYPE, 0x082);
	hdmi_write_reg(HDMI_CORE_AV, HDMI_CORE_AV_AVI_VERS, 0x002);
	hdmi_write_reg(HDMI_CORE_AV, HDMI_CORE_AV_AVI_LEN, 0x00D);

	offset = dbyte + (0 * dbyte_size);
	val = (info_avi.db1y_rgb_yuv422_yuv444 << 5) |
		(info_avi.db1a_active_format_off_on << 4) |
		(info_avi.db1b_no_vert_hori_verthori << 2) |
		(info_avi.db1s_0_1_2);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (1 * dbyte_size);
	val = (info_avi.db2c_no_itu601_itu709_extented << 6) |
		(info_avi.db2m_no_43_169 << 4) |
		(info_avi.db2r_same_43_169_149);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (2 * dbyte_size);
	val = (info_avi.db3itc_no_yes << 7) |
		(info_avi.db3ec_xvyuv601_xvyuv709 << 4) |
		(info_avi.db3q_default_lr_fr << 2) |
		(info_avi.db3sc_no_hori_vert_horivert);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (3 * dbyte_size);
	hdmi_write_reg(HDMI_CORE_AV, offset, info_avi.db4vic_videocode);
	sum += info_avi.db4vic_videocode;

	offset = dbyte + (4 * dbyte_size);
	val = info_avi.db5pr_no_2_3_4_5_6_7_8_9_10;
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (5 * dbyte_size);
	val = info_avi.db6_7_lineendoftop & 0x00FF;
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (6 * dbyte_size);
	val = ((info_avi.db6_7_lineendoftop >> 8) & 0x00FF);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (7 * dbyte_size);
	val = info_avi.db8_9_linestartofbottom & 0x00FF;
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (8 * dbyte_size);
	val = ((info_avi.db8_9_linestartofbottom >> 8) & 0x00FF);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (9 * dbyte_size);
	val = info_avi.db10_11_pixelendofleft & 0x00FF;
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (10 * dbyte_size);
	val = ((info_avi.db10_11_pixelendofleft >> 8) & 0x00FF);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	offset = dbyte + (11 * dbyte_size);
	val = info_avi.db12_13_pixelstartofright & 0x00FF;
	hdmi_write_reg(HDMI_CORE_AV, offset , val);
	sum += val;

	offset = dbyte + (12 * dbyte_size);
	val = ((info_avi.db12_13_pixelstartofright >> 8) & 0x00FF);
	hdmi_write_reg(HDMI_CORE_AV, offset, val);
	sum += val;

	checksum = 0x100 - sum;
	hdmi_write_reg(HDMI_CORE_AV, HDMI_CORE_AV_AVI_CHSUM, checksum);

	return 0;
}

int hdmi_configure_csc(enum hdmi_core_av_csc csc)
{
	int var;
	switch (csc) {
	/*
	 * Setting the AVI infroframe to respective color mode
	 * As the quantization is in default mode ie it selects
	 * full range for RGB (except for VGA ) and limited range
	 * for YUV we dont have to make any changes for this
	 */
	case RGB:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 = INFOFRAME_AVI_DB1Y_RGB;
		hdmi_core_audio_infoframe_avi(hdmi.avi_param);
		var = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_ACEN);
		var = FLD_MOD(var, 0, 2, 2);
		hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_ACEN, var);
		break;
	case RGB_TO_YUV:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 =
						INFOFRAME_AVI_DB1Y_YUV422;
		hdmi_core_audio_infoframe_avi(hdmi.avi_param);
		var = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_ACEN);
		var = FLD_MOD(var, 1, 2, 2);
		hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_ACEN, var);
		break;
	case YUV_TO_RGB:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 = INFOFRAME_AVI_DB1Y_RGB;
		hdmi_core_audio_infoframe_avi(hdmi.avi_param);
		var = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_MODE);
		var = FLD_MOD(var, 1, 3, 3);
		hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VID_MODE, var);
		break;
	default:
		break;
	}
	return 0;
}

int hdmi_configure_lrfr(enum hdmi_range lr_fr, int force_set)
{
	int var;
	switch (lr_fr) {
	/*
	 * Setting the AVI infroframe to respective limited range
	 * 0 if for limited range 1 for full range
	 */
	case HDMI_LIMITED_RANGE:
		hdmi.avi_param.db3q_default_lr_fr = INFOFRAME_AVI_DB3Q_LR;
		hdmi_core_audio_infoframe_avi(hdmi.avi_param);
		if (force_set) {
			var = hdmi_read_reg(HDMI_CORE_SYS,
						HDMI_CORE_SYS__VID_ACEN);
			var = FLD_MOD(var, 1, 1, 1);
			hdmi_write_reg(HDMI_CORE_SYS,
						HDMI_CORE_SYS__VID_ACEN, var);
		}
		break;
	case HDMI_FULL_RANGE:
		if (hdmi.avi_param.db1y_rgb_yuv422_yuv444 ==
						INFOFRAME_AVI_DB1Y_YUV422) {
			printk(KERN_ERR "It is only limited range for YUV");
			return -1;
		}
		hdmi.avi_param.db3q_default_lr_fr = INFOFRAME_AVI_DB3Q_FR;
		hdmi_core_audio_infoframe_avi(hdmi.avi_param);
		if (force_set) {
			var = hdmi_read_reg(HDMI_CORE_SYS,
						HDMI_CORE_SYS__VID_MODE);
			var = FLD_MOD(var, 1, 4, 4);
			hdmi_write_reg(HDMI_CORE_SYS,
						HDMI_CORE_SYS__VID_MODE, var);
		}
		break;
	default:
		break;
	}
	return 0;
}

static int hdmi_core_av_packet_config(u32 name,
	struct hdmi_core_packet_enable_repeat r_p)
{

	/* enable/repeat the infoframe */
	hdmi_write_reg(name, HDMI_CORE_AV_PB_CTRL1,
		(r_p.MPEGInfoFrameED << 7) |
		(r_p.MPEGInfoFrameRepeat << 6) |
		(r_p.AudioPacketED << 5) |
		(r_p.AudioPacketRepeat << 4) |
		(r_p.SPDInfoFrameED << 3) |
		(r_p.SPDInfoFrameRepeat << 2) |
		(r_p.AVIInfoFrameED << 1) |
		(r_p.AVIInfoFrameRepeat));

	/* enable/repeat the packet */
	hdmi_write_reg(name, HDMI_CORE_AV_PB_CTRL2,
		(r_p.GeneralcontrolPacketED << 3) |
		(r_p.GeneralcontrolPacketRepeat << 2) |
		(r_p.GenericPacketED << 1) |
		(r_p.GenericPacketRepeat));
	return 0;
}

static void hdmi_w1_init(struct hdmi_video_timing *t_p,
	struct hdmi_video_format *f_p,
	struct hdmi_video_interface *i_p,
	struct hdmi_irq_vector *pIrqVectorEnable,
	struct hdmi_audio_format *audio_fmt,
	struct hdmi_audio_dma *audio_dma)
{
	DBG("Enter HDMI_W1_GlobalInitVars()\n");

	t_p->horizontalBackPorch = 0;
	t_p->horizontalFrontPorch = 0;
	t_p->horizontalSyncPulse = 0;
	t_p->verticalBackPorch = 0;
	t_p->verticalFrontPorch = 0;
	t_p->verticalSyncPulse = 0;

	/*HDMI_PACK_24b_RGB_YUV444_YUV422;*/
	/* ToDo : revert back to HDMI_PACK_10b_RGB_YUV444,
	   or put conditional code*/
	f_p->packingMode = HDMI_PACK_10b_RGB_YUV444;
	msleep(340);

	f_p->linePerPanel = 0;
	f_p->pixelPerLine = 0;

	i_p->vSyncPolarity = 0;
	i_p->hSyncPolarity = 0;

	i_p->interlacing = 0;
	i_p->timingMode = 0; /* HDMI_TIMING_SLAVE */

	memset(pIrqVectorEnable, 0, sizeof(*pIrqVectorEnable));
	pIrqVectorEnable->phyDisconnect = 0;
	pIrqVectorEnable->phyConnect = 0;
	pIrqVectorEnable->ocpTimeOut = 1;
	pIrqVectorEnable->core = 1;

	audio_fmt->stereo_channel_enable = HDMI_STEREO_ONECHANNELS;
	audio_fmt->audio_channel_location = HDMI_CEA_CODE_03;
	audio_fmt->iec = HDMI_AUDIO_FORMAT_LPCM;
	audio_fmt->justify = HDMI_AUDIO_JUSTIFY_LEFT;
	audio_fmt->left_before = HDMI_SAMPLE_LEFT_FIRST;
	audio_fmt->sample_number = HDMI_ONEWORD_TWO_SAMPLES;
	audio_fmt->sample_size = HDMI_SAMPLE_16BITS;

	audio_dma->dma_transfer = 0x20;
	audio_dma->block_size = 0xC0;
	audio_dma->dma_or_irq = HDMI_THRESHOLD_DMA;
	audio_dma->threshold_value = 0x10;
	audio_dma->block_start_end = HDMI_BLOCK_STARTEND_ON;
}


static void hdmi_w1_irq_enable(struct hdmi_irq_vector *pIrqVectorEnable)
{
	u32 r = 0;

	r = ((pIrqVectorEnable->pllRecal << 31) |
		(pIrqVectorEnable->pllUnlock << 30) |
		(pIrqVectorEnable->pllLock << 29) |
		(pIrqVectorEnable->phyDisconnect << 26) |
		(pIrqVectorEnable->phyConnect << 25) |
		(pIrqVectorEnable->phyShort5v << 24) |
		(pIrqVectorEnable->videoEndFrame << 17) |
		(pIrqVectorEnable->videoVsync << 16) |
		(pIrqVectorEnable->fifoSampleRequest << 10) |
		(pIrqVectorEnable->fifoOverflow << 9) |
		(pIrqVectorEnable->fifoUnderflow << 8) |
		(pIrqVectorEnable->ocpTimeOut << 4) |
		(pIrqVectorEnable->core << 0));

	hdmi_write_reg(HDMI_WP, HDMI_WP_IRQENABLE_SET, r);
}

static void hdmi_w1_irq_wakeup_enable(struct hdmi_irq_vector *pIrqVectorEnable)
{
	u32 r = 0;
	/* make the irq wakeup enable */
	r = ((pIrqVectorEnable->phyDisconnect << 26) |
			(pIrqVectorEnable->phyConnect << 25) |
			(pIrqVectorEnable->core << 0));
	hdmi_write_reg(HDMI_WP, HDMI_WP_IRQWAKEEN, r);
}

static inline int hdmi_w1_wait_for_bit_change(const u32 ins,
	u32 idx, int b2, int b1, int val)
{
	int t = 0;
	while (val != FLD_GET(hdmi_read_reg(ins, idx), b2, b1)) {
		udelay(1);
		if (t++ > 1000)
			return !val;
	}
	return val;
}

/* todo: add timeout value */
int hdmi_w1_set_wait_srest(void)
{
	/* reset W1 */
	REG_FLD_MOD(HDMI_WP, HDMI_WP_SYSCONFIG, 0x1, 0, 0);

	/* wait till SOFTRESET == 0 */
	while (FLD_GET(hdmi_read_reg(HDMI_WP, HDMI_WP_SYSCONFIG), 0, 0))
		;
	return 0;
}

/* PHY_PWR_CMD */
int hdmi_w1_set_wait_phy_pwr(HDMI_PhyPwr_t val)
{
	DBG("*** Set PHY power mode to %d\n", val);
	REG_FLD_MOD(HDMI_WP, HDMI_WP_PWR_CTRL, val, 7, 6);

	if (hdmi_w1_wait_for_bit_change(HDMI_WP, HDMI_WP_PWR_CTRL, 5, 4, val)
	    != val) {
		ERR("Failed to set PHY power mode to %d\n", val);
		return -ENODEV;
	}
	return 0;
}

/* PLL_PWR_CMD */
int hdmi_w1_set_wait_pll_pwr(HDMI_PllPwr_t val)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_PWR_CTRL, val, 3, 2);

	/* wait till PHY_PWR_STATUS=ON */
	if (hdmi_w1_wait_for_bit_change(HDMI_WP, HDMI_WP_PWR_CTRL, 1, 0, val)
	    != val) {
		ERR("Failed to set PHY_PWR_STATUS to ON\n");
		return -ENODEV;
	}

	return 0;
}

void hdmi_w1_video_stop(void)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_VIDEO_CFG, 0, 31, 31);
}
void hdmi_w1_video_start(void)
{
	/* For TI814x some secret bit needs to be set */
	if (cpu_is_ti814x())
		REG_FLD_MOD(HDMI_WP, HDMI_WP_VIDEO_CFG,
					(u32)0x1, 2, 2);
	REG_FLD_MOD(HDMI_WP, HDMI_WP_VIDEO_CFG, (u32)0x1, 31, 31);
}

unsigned int hdmi_w1_video_status(void)
{
	return 	(hdmi_read_reg(HDMI_WP, HDMI_WP_VIDEO_CFG) & 0x80000000);
}

static void hdmi_w1_video_init_format(struct hdmi_video_format *f_p,
	struct hdmi_video_timing *t_p, struct hdmi_config *param)
{
	DBG("Enter HDMI_W1_ConfigVideoResolutionTiming()\n");

	f_p->linePerPanel = param->lpp;
	f_p->pixelPerLine = param->ppl;

	t_p->horizontalBackPorch = param->hbp;
	t_p->horizontalFrontPorch = param->hfp;
	t_p->horizontalSyncPulse = param->hsw;
	t_p->verticalBackPorch = param->vbp;
	t_p->verticalFrontPorch = param->vfp;
	t_p->verticalSyncPulse = param->vsw;
}

static void hdmi_w1_video_config_format(
	struct hdmi_video_format *f_p)
{
	u32 l = 0;

	REG_FLD_MOD(HDMI_WP, HDMI_WP_VIDEO_CFG, f_p->packingMode, 10, 8);

	l |= FLD_VAL(f_p->linePerPanel, 31, 16);
	l |= FLD_VAL(f_p->pixelPerLine, 15, 0);
	hdmi_write_reg(HDMI_WP, HDMI_WP_VIDEO_SIZE, l);
}


static void hdmi_w1_video_config_interface(
	struct hdmi_video_interface *i_p)
{
	u32 r;
	DBG("Enter HDMI_W1_ConfigVideoInterface()\n");

	r = hdmi_read_reg(HDMI_WP, HDMI_WP_VIDEO_CFG);
	r = FLD_MOD(r, i_p->vSyncPolarity, 7, 7);
	r = FLD_MOD(r, i_p->hSyncPolarity, 6, 6);
	r = FLD_MOD(r, i_p->interlacing, 3, 3);
	r = FLD_MOD(r, i_p->timingMode, 1, 0);
	hdmi_write_reg(HDMI_WP, HDMI_WP_VIDEO_CFG, r);
}

static void hdmi_w1_video_config_timing(
	struct hdmi_video_timing *t_p)
{
	u32 timing_h = 0;
	u32 timing_v = 0;

	DBG("Enter HDMI_W1_ConfigVideoTiming ()\n");

	timing_h |= FLD_VAL(t_p->horizontalBackPorch, 31, 20);
	timing_h |= FLD_VAL(t_p->horizontalFrontPorch, 19, 8);
	timing_h |= FLD_VAL(t_p->horizontalSyncPulse, 7, 0);
	hdmi_write_reg(HDMI_WP, HDMI_WP_VIDEO_TIMING_H, timing_h);

	timing_v |= FLD_VAL(t_p->verticalBackPorch, 31, 20);
	timing_v |= FLD_VAL(t_p->verticalFrontPorch, 19, 8);
	timing_v |= FLD_VAL(t_p->verticalSyncPulse, 7, 0);
	hdmi_write_reg(HDMI_WP, HDMI_WP_VIDEO_TIMING_V, timing_v);
}

int hdmi_w1_audio_config_format(u32 name,
			struct hdmi_audio_format *audio_fmt)
{
	int ret = 0;
	u32 value = 0;
	/* Differs from OMAP code, this function explictly disable audio wrapper.
		In our case, we ensure caller of this function disables audio wrapper */
	value = hdmi_read_reg(name, HDMI_WP_AUDIO_CFG);
	value &= 0xffffffef;
	value |= ((audio_fmt->iec) << 4);
	value &= 0xfffffff7;
	value |= ((audio_fmt->justify) << 3);;
	value &= 0xfffffffb;
	value |= ((audio_fmt->left_before) << 2);
	value &= 0xfffffffd;
	value |= ((audio_fmt->sample_number) << 1);
	value &= 0xfffffffe;
	value |= ((audio_fmt->sample_size));
	value &= 0xf8ffffff;
	value |= ((audio_fmt->stereo_channel_enable) << 24);
	value &= 0xff00ffff;
	value |= ((audio_fmt->audio_channel_location) << 16);
	value &= 0xffffffdf;
	value |= (audio_fmt->block_start_end << 5);

	/* Wakeup value = 0x1030022; */

	hdmi_write_reg(name, HDMI_WP_AUDIO_CFG, value);
	DBG("HDMI_WP_AUDIO_CFG = 0x%x\n", value);

	return ret;
}

int hdmi_w1_audio_config_dma(u32 name, struct hdmi_audio_dma *audio_dma)
{
	int ret = 0;
	u32 value = 0;

	value = hdmi_read_reg(name, HDMI_WP_AUDIO_CFG2);
	value &= 0xffffff00;
	value |= audio_dma->block_size;
	value &= 0xffff00ff;
	value |= audio_dma->dma_transfer << 8;

	/*  Wakeup value = 0x20C0; */

	hdmi_write_reg(name, HDMI_WP_AUDIO_CFG2, value);
	DBG("HDMI_WP_AUDIO_CFG2 = 0x%x\n", value);

	value = hdmi_read_reg(name, HDMI_WP_AUDIO_CTRL);
	value &= 0xfffffdff;
	value |= audio_dma->dma_or_irq << 9;
	value &= 0xfffffe00;
	value |= audio_dma->threshold_value;

	/*  Wakeup value = 0x020; */

	hdmi_write_reg(name, HDMI_WP_AUDIO_CTRL, value);
	DBG("HDMI_WP_AUDIO_CTRL = 0x%x\n", value);

	return ret;
}

static void hdmi_w1_audio_enable(void)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_AUDIO_CTRL, 1, 31, 31);
}

static void hdmi_w1_audio_disable(void)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_AUDIO_CTRL, 0, 31, 31);
}

static void hdmi_w1_audio_start(void)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_AUDIO_CTRL, 1, 30, 30);
}

static void hdmi_w1_audio_stop(void)
{
	REG_FLD_MOD(HDMI_WP, HDMI_WP_AUDIO_CTRL, 0, 30, 30);
}

static void hdmi_core_extract_sync_config(struct hdmi_video_format *f_p,
		struct hdmi_video_timing *t_p)
{
	u32 hbit2_hsync1, hbit2_hsync2, fld2_hs_ofstl, fld2_hs_ofsth;
	u32 hwidth1, hwidth2, vbit2_vsync, vwidth, total_pixels;

	total_pixels = f_p->pixelPerLine + t_p->horizontalBackPorch +
		t_p->horizontalFrontPorch + t_p->horizontalSyncPulse;
	hbit2_hsync1 = t_p->horizontalFrontPorch & 0xFF;
	hbit2_hsync2 = ((t_p->horizontalFrontPorch & 0x300) >> 8);

	fld2_hs_ofstl = ((total_pixels >> 1) & 0xFF);
	fld2_hs_ofsth = (((total_pixels >> 1) & 0xF00) >> 8);

	hwidth1 = t_p->horizontalSyncPulse & 0xFF;
	hwidth2 = ((t_p->horizontalSyncPulse & 0x300) >> 8);

	vbit2_vsync = t_p->verticalFrontPorch & 0x3F;

	vwidth = t_p->verticalSyncPulse & 0x3F;
#if 0
	printk("total_pixels = %d\n", total_pixels);
	printk("hbit2_hsync1 = %d\n", hbit2_hsync1);
	printk("hbit2_hsync2 = %d\n", hbit2_hsync2);
	printk("fld2_hs_ofstl  = %d\n",fld2_hs_ofstl );
	printk("fld2_hs_ofsth  = %d\n",fld2_hs_ofsth );
	printk("hwidth1  = %d\n",hwidth1 );
	printk("hwidth2  = %d\n",hwidth2 );
	printk("vbit2_vsync  = %d\n",vbit2_vsync );
	printk(" vwidth = %d\n",vwidth );
#endif
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__HBIT_2HSYNC1, hbit2_hsync1);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__HBIT_2HSYNC2, hbit2_hsync2);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__FLD2_HS_OFSTL, fld2_hs_ofstl);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__FLD2_HS_OFSTH, fld2_hs_ofsth);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__HWIDTH1, hwidth1);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__HWIDTH2, hwidth2);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VBIT_TO_VSYNC, vbit2_vsync);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__VWIDTH, vwidth);
}

int hdmi_lib_enable(struct hdmi_config *cfg)
{
	u32 r;

	u32 av_name = HDMI_CORE_AV;

	/* HDMI */
	struct hdmi_video_timing VideoTimingParam;
	struct hdmi_video_format VideoFormatParam;
	struct hdmi_video_interface VideoInterfaceParam;
	struct hdmi_irq_vector IrqHdmiVectorEnable;
	struct hdmi_audio_format audio_fmt;
	struct hdmi_audio_dma audio_dma;

	/* HDMI core */
	struct hdmi_core_video_config_t v_core_cfg;
	struct hdmi_core_audio_config audio_cfg;
	struct hdmi_core_packet_enable_repeat repeat_param;

	hdmi_w1_init(&VideoTimingParam, &VideoFormatParam,
			&VideoInterfaceParam, &IrqHdmiVectorEnable,
			&audio_fmt, &audio_dma);

	if ( cpu_is_ti816x() || cpu_is_ti814x() )
	{
		/* ToDo : TI81xx , verify support for other modes*/
		cfg->deep_color = HDMI_DEEP_COLOR_24BIT;
	}
	hdmi_core_init(cfg->deep_color, cfg->input_df,
			cfg->output_df, &v_core_cfg,
			&audio_cfg, &hdmi.avi_param,
			&repeat_param);

	/*
	   Currently we are seeing OCP timeout interrupt.
	   It happens if l3 is idle. we need to set the SCP
	   clock to proper value to make sure good performance
	   is met.
	 */
	hdmi_write_reg(HDMI_WP, HDMI_WP_WP_CLK, 0x100);

	/* Enable PLL Lock and UnLock intrerrupts */
	IrqHdmiVectorEnable.pllUnlock = 1;
	IrqHdmiVectorEnable.pllLock = 1;
	IrqHdmiVectorEnable.core = 1;

	/***************** init DSS register **********************/

	hdmi_w1_irq_enable(&IrqHdmiVectorEnable);

	msleep(100);
	IrqHdmiVectorEnable.phyDisconnect = 0;
	hdmi_w1_irq_wakeup_enable(&IrqHdmiVectorEnable);

	hdmi_w1_video_init_format(&VideoFormatParam, &VideoTimingParam, cfg);
	if ( cpu_is_omap44xx())
		hdmi_w1_video_config_timing(&VideoTimingParam);

	/* TI816x and TI814x works in both discrete and embedded sync mode.
	   So set the timing based on the sync format
	 */
	if (cpu_is_ti814x() || cpu_is_ti816x())
		if (cfg->sync == HDMI_EMBEDDED_SYNC)
			hdmi_core_extract_sync_config(&VideoFormatParam,
					&VideoTimingParam);

	/* video config */
	switch (cfg->deep_color) {
	case 0:
		VideoFormatParam.packingMode = HDMI_PACK_24b_RGB_YUV444_YUV422;
		VideoInterfaceParam.timingMode = HDMI_TIMING_MASTER_24BIT;
		break;
	case 1:
		VideoFormatParam.packingMode = HDMI_PACK_10b_RGB_YUV444;
		VideoInterfaceParam.timingMode = HDMI_TIMING_MASTER_30BIT;
		break;
	case 2:
		VideoFormatParam.packingMode = HDMI_PACK_ALREADYPACKED;
		VideoInterfaceParam.timingMode = HDMI_TIMING_MASTER_36BIT;
		break;
	}

	/* TI81xx , HDMI wrapper only slave mode. */
	hdmi_w1_video_config_format(&VideoFormatParam);

	/* FIXME */
	if ( cpu_is_omap44xx()){
		VideoInterfaceParam.vSyncPolarity = cfg->v_pol;
		VideoInterfaceParam.hSyncPolarity = cfg->h_pol;
		VideoInterfaceParam.interlacing = cfg->interlace;
	}

	if (( cpu_is_ti816x() || cpu_is_ti814x())){
		VideoInterfaceParam.vSyncPolarity = 0;
		VideoInterfaceParam.hSyncPolarity = 0;
		VideoInterfaceParam.interlacing = cfg->interlace;
	}

	/*TI81xx , HDMI wrapper only slave mode, so h_pol, v_pol is ignored*/
	if ( cpu_is_ti816x())
		VideoInterfaceParam.timingMode = 0 ;
	/* TI814x needs to generate pixel clock for Venc
	   So it works in master mode. This is not true master mode.
	   In true master mode timing is also generated by HDMI
	 */
	if ( cpu_is_ti814x())
		VideoInterfaceParam.timingMode = 1;
	hdmi_w1_video_config_interface(&VideoInterfaceParam);

	/****************************** CORE *******************************/
	/************* configure core video part ********************************/
	/* set software reset in the core */
	hdmi_core_swreset_assert();

	/* power down off */
	hdmi_core_powerdown_disable();

	v_core_cfg.CoreHdmiDvi = cfg->hdmi_dvi;
	/*store the sync format*/
	v_core_cfg.CoreSyncFormat = cfg->sync;
	v_core_cfg.vsync_polarity = 0;
	v_core_cfg.hsync_polarity = 0;
	if (HDMI_EMBEDDED_SYNC == v_core_cfg.CoreSyncFormat) {
		switch (cfg->video_format) {
		/* According to HDMI spec. polarity for 480p and 576 should be
		   reversed
		 */
		case 2:
		case 17:
			v_core_cfg.vsync_polarity = 1;
			v_core_cfg.hsync_polarity = 1;
			break;
		default:
			v_core_cfg.vsync_polarity = 0;
			v_core_cfg.hsync_polarity = 0;
		}
	}
	r = hdmi_core_video_config(&v_core_cfg);
	/* release software reset in the core */
	hdmi_core_swreset_release();

	/* configure packet */
	/* info frame video see doc CEA861-D page 65 */
	switch (cfg->output_df) {
	case HDMI_DF_YUV422:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 =
			INFOFRAME_AVI_DB1Y_YUV422;
		break;
	case HDMI_DF_YUV444:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 =
			INFOFRAME_AVI_DB1Y_YUV444;
		break;
	case HDMI_DF_RGB:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 =
			INFOFRAME_AVI_DB1Y_RGB;
		break;
	default:
		hdmi.avi_param.db1y_rgb_yuv422_yuv444 = INFOFRAME_AVI_DB1Y_RGB;

	}
	hdmi.avi_param.db1a_active_format_off_on =
		INFOFRAME_AVI_DB1A_ACTIVE_FORMAT_OFF;
	hdmi.avi_param.db1b_no_vert_hori_verthori = INFOFRAME_AVI_DB1B_NO;
	hdmi.avi_param.db1s_0_1_2 = INFOFRAME_AVI_DB1S_2;
	hdmi.avi_param.db2c_no_itu601_itu709_extented = INFOFRAME_AVI_DB2C_NO;

	/* Support AR in AVI infoframe */
	switch (cfg->video_format) {
	/* 16:9 */
	case 4:
	case 5:
	case 16:
	case 19:
	case 20:
	case 31:
	case 32:
	case 39:
		hdmi.avi_param.db2m_no_43_169 = INFOFRAME_AVI_DB2M_169;
		break;
		/* 4:3 */
	case 1:
	case 2:
	case 6:
	case 17:
	case 21:
	case 29:
	case 35:
	case 37:
		hdmi.avi_param.db2m_no_43_169 = INFOFRAME_AVI_DB2M_43;
		break;
	}

	hdmi.avi_param.db2r_same_43_169_149 = INFOFRAME_AVI_DB2R_SAME;
	hdmi.avi_param.db3itc_no_yes = INFOFRAME_AVI_DB3ITC_NO;
	hdmi.avi_param.db3ec_xvyuv601_xvyuv709 = INFOFRAME_AVI_DB3EC_XVYUV601;
	hdmi.avi_param.db3q_default_lr_fr = INFOFRAME_AVI_DB3Q_DEFAULT;
	hdmi.avi_param.db3sc_no_hori_vert_horivert = INFOFRAME_AVI_DB3SC_NO;
	/*check this value is passed correctly from the NeCe HDMI driver */
	hdmi.avi_param.db4vic_videocode = cfg->video_format;
	hdmi.avi_param.db5pr_no_2_3_4_5_6_7_8_9_10 = INFOFRAME_AVI_DB5PR_NO;
	hdmi.avi_param.db6_7_lineendoftop = 0;
	hdmi.avi_param.db8_9_linestartofbottom = 0;
	hdmi.avi_param.db10_11_pixelendofleft = 0;
	hdmi.avi_param.db12_13_pixelstartofright = 0;

	r = hdmi_core_audio_infoframe_avi(hdmi.avi_param);

	/* enable/repeat the infoframe */
	repeat_param.AVIInfoFrameED = PACKETENABLE;
	repeat_param.AVIInfoFrameRepeat = PACKETREPEATON;
	/* wakeup */
	repeat_param.AudioPacketED = PACKETENABLE;
	repeat_param.AudioPacketRepeat = PACKETREPEATON;

	/* ISCR1 transmission - Disabled */
	repeat_param.MPEGInfoFrameED = PACKETDISABLE;
	repeat_param.MPEGInfoFrameRepeat = PACKETREPEATOFF;
	/* ACP transmission - Disabled */
	repeat_param.SPDInfoFrameED = PACKETDISABLE;
	repeat_param.SPDInfoFrameRepeat = PACKETREPEATOFF;

	r = hdmi_core_av_packet_config(av_name, repeat_param);

	REG_FLD_MOD(av_name, HDMI_CORE_AV__HDMI_CTRL,
			v_core_cfg.CoreHdmiDvi, 0, 0);
	memcpy(&hdmi.hdmi_cfg, cfg, sizeof(struct hdmi_config));
	return r;
}



int hdmi_lib_init(void){
	u32 rev;

	if(cpu_is_ti816x()){
		/* OMAP first, then overwrite it */
		hdmi.base_wp =  ioremap(TI81xx_HDMI_WP, 4096);
	}else if(cpu_is_ti814x()){
		hdmi.base_wp =  ioremap(TI81xx_HDMI_WP, 4096);
	} else {
		hdmi.base_wp = ioremap(HDMI_WP, (HDMI_HDCP - HDMI_WP));
	}

	if (!hdmi.base_wp) {
		ERR("can't ioremap WP\n");
		return -ENOMEM;
	}

	hdmi.base_core = hdmi.base_wp + 0x400;
	hdmi.base_core_av = hdmi.base_wp + 0x900;
	hdmi.base_core_cec = hdmi.base_wp + 0xD00;

	mutex_init(&hdmi.mutex);
	INIT_LIST_HEAD(&hdmi.notifier_head);

	rev = hdmi_read_reg(HDMI_WP, HDMI_WP_REVISION);
	printk(KERN_INFO "HDMI W1 rev %d.%d\n",
		FLD_GET(rev, 10, 8), FLD_GET(rev, 5, 0));
	rev = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_SPEC);
	printk(KERN_INFO "HDMI CEC Spec version %x.%x\n",
		FLD_GET(rev, 7, 4), (FLD_GET(rev, 3, 0) - 1));
	return 0;
}

void hdmi_lib_exit(void){
	iounmap(hdmi.base_wp);
}

int hdmi_set_irqs(int i)
{
	u32 r = 0 , hpd = 0;
	struct hdmi_irq_vector pIrqVectorEnable = {
		.pllUnlock = 1,
		.pllLock = 1,
		.phyDisconnect = 0,
		.phyConnect = 0,
		.core = 1,
	};

	if (!i) {
		hdmi_w1_irq_enable(&pIrqVectorEnable);

		r = hdmi_read_reg(HDMI_WP, HDMI_WP_IRQENABLE_SET);
		DBG("Irqenable %x\n", r);
	}
	r = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_CTRL1);
	/* PD bit has to be written to recieve the interrupts */
	r = FLD_MOD(r, !i, 0, 0);
	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_CTRL1, r);

	hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__UMASK1, i ? 0x00 : 0x40);
	hpd = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__UMASK1);
	DBG("%x hpd\n", hpd);

	return 0;
}

/* Interrupt handler */
void HDMI_W1_HPD_handler(int *r)
{
	u32 val, set = 0, hpd_intr = 0, core_state = 0;
	u32 time_in_ms, intr2 = 0, intr3 = 0;
	static ktime_t ts_hpd_low, ts_hpd_high;

	DBG("-------------DEBUG-------------------");
	DBG("%x hdmi_wp_irqstatus\n", \
		hdmi_read_reg(HDMI_WP, HDMI_WP_IRQSTATUS));
	DBG("%x hdmi_core_intr_state\n", \
		hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR_STATE));
	DBG("%x hdmi_core_intr1\n", \
		hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR1));
	DBG("%x hdmi_core_intr2\n", \
		hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR2));
	DBG("%x hdmi_core_intr3\n", \
		hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR3));
	DBG("%x hdmi_core_sys_sys_stat\n", \
		hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__SYS_STAT));
	DBG("-------------DEBUG-------------------\n");

	val = hdmi_read_reg(HDMI_WP, HDMI_WP_IRQSTATUS);
	if (val & HDMI_WP_IRQSTATUS_CORE) {
		core_state =
			hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR_STATE);
		if (core_state & 0x1) {
			set =
			hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__SYS_STAT);
			hpd_intr = hdmi_read_reg(HDMI_CORE_SYS,
					HDMI_CORE_SYS__INTR1);
			intr2 = hdmi_read_reg(HDMI_CORE_SYS,
					HDMI_CORE_SYS__INTR2);
			intr3 = hdmi_read_reg(HDMI_CORE_SYS,
					HDMI_CORE_SYS__INTR3);

			hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR1,
					hpd_intr);
			hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR2,
					intr2);
			hdmi_write_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR3,
					intr3);

			/* Read to flush */
			hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR1);
			hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR2);
			hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__INTR3);
		}
	}


	if ((val & HDMI_WP_IRQSTATUS_CORE) && (core_state & 0x1)) {
		if (hpd_intr & 0x40) {
			if  (set & HDMI_CORE_SYS__SYS_STAT_HPD) {
				if ((first_hpd == 0) && (dirty == 0)) {
					*r |= HDMI_FIRST_HPD;
					first_hpd++;
					DBG("first hpd");
				} else {
					ts_hpd_high = ktime_get();
					if (dirty) {
						time_in_ms =
							(int)ktime_to_us(ktime_sub\
									(ts_hpd_high, ts_hpd_low)) / 1000;
						if (time_in_ms >= 80)
							*r |= HDMI_HPD_MODIFY;
						else
							*r |= HDMI_HPD_HIGH;
						dirty = 0;
					}
				}
			} else {
				ts_hpd_low = ktime_get();
				dirty = 1;
				*r |= HDMI_HPD_LOW;
			}
		}
	}

	/*
	   Handle ocp timeout in correct way. SCP clock will reset the divder
	   to 7 automatically. We need to speed up the clock to avoid
	   performance degradation.
	 */
	if (val & HDMI_WP_IRQSTATUS_OCPTIMEOUT) {
		hdmi_write_reg(HDMI_WP, HDMI_WP_WP_CLK, 0x100);
		DBG("OCP timeout");
	}

	if (intr2 & HDMI_IP_CORE_SYSTEM__INTR2__BCAP)
		*r |= HDMI_BCAP;

	if (intr3 & HDMI_IP_CORE_SYSTEM__INTR3__RI_ERR)
		*r |= HDMI_RI_ERR;

	/* Ack other interrupts if any */
	hdmi_write_reg(HDMI_WP, HDMI_WP_IRQSTATUS, val);
	/* flush posted write */
	hdmi_read_reg(HDMI_WP, HDMI_WP_IRQSTATUS);
}
#if 0
int hdmi_rxdet(void)
{
	int state = 0;
	int loop = 0, val1, val2, val3, val4;
	struct hdmi_irq_vector IrqHdmiVectorEnable;

	hdmi_write_reg(HDMI_WP, HDMI_WP_WP_DEBUG_CFG, 4);

	do {
		val1 = hdmi_read_reg(HDMI_WP, HDMI_WP_WP_DEBUG_DATA);
		udelay(5);
		val2 = hdmi_read_reg(HDMI_WP, HDMI_WP_WP_DEBUG_DATA);
		udelay(5);
		val3 = hdmi_read_reg(HDMI_WP, HDMI_WP_WP_DEBUG_DATA);
		udelay(5);
		val4 = hdmi_read_reg(HDMI_WP, HDMI_WP_WP_DEBUG_DATA);
	} while ((val1 != val2 || val2 != val3 || val3 != val4)
		&& (loop < 100));

	hdmi_write_reg(HDMI_WP, HDMI_WP_WP_DEBUG_CFG, 0);

	if (loop == 100)
		state = -1;
	else
		state = (val1 & 1);

	/* Turn on the wakeup capability of the interrupts
	It is recommended to turn on opposite interrupt wake
	up capability in connected and disconnected state.
	This is to avoid race condition in interrupts.
	*/
	IrqHdmiVectorEnable.core = 1;
	if (state) {
		IrqHdmiVectorEnable.phyDisconnect = 1;
		IrqHdmiVectorEnable.phyConnect = 0;
		hdmi_w1_irq_wakeup_enable(&IrqHdmiVectorEnable);
	} else {
		IrqHdmiVectorEnable.phyDisconnect = 0;
		IrqHdmiVectorEnable.phyConnect = 1;
		hdmi_w1_irq_wakeup_enable(&IrqHdmiVectorEnable);
	}

	return state;
}
#endif

/* wrapper functions to be used until L24.5 release */
int HDMI_CORE_DDC_READEDID(u32 name, u8 *p, u16 max_length)
{
	int r = read_edid(p, max_length);
	return r;
}

int HDMI_W1_StopVideoFrame(u32 name)
{
	DBG("Enter HDMI_W1_StopVideoFrame()\n");
	hdmi_w1_video_stop();
	return 0;
}

int HDMI_W1_StartVideoFrame(u32 name)
{
	DBG("Enter HDMI_W1_StartVideoFrame  ()\n");

#if 0
	/*Only for debug purpose */
	temp  = __raw_readl(hdmi.base_core + HDMI_CORE_SYS__SYS_STAT);
	temp &= HDMI_SYS_STAT_HPD_MASK;
	if (!temp)
	{
		printk("Sink not detected\n");
	}
	else
		printk("Sink detected\n");

	mdelay(1000);

	temp = __raw_readl(hdmi.base_core + HDMI_CORE_CTRL1);
	temp |= HDMI_SYS_CTRL1_PD_MASK;
	__raw_writel(temp,(hdmi.base_core + HDMI_CORE_CTRL1));

	temp = __raw_readl(hdmi.base_core + HDMI_CORE_SYS__SRST);
	temp &= (~(HDMI_SRST_SWRST_MASK));
	__raw_writel(temp,(hdmi.base_core + HDMI_CORE_SYS__SRST));
#endif

	hdmi_w1_video_start();
	return 0;
}

/* PHY_PWR_CMD */
int HDMI_W1_SetWaitPhyPwrState(u32 name,
	HDMI_PhyPwr_t param)
{
	int r = hdmi_w1_set_wait_phy_pwr(param);
	return r;
}

/* PLL_PWR_CMD */
int HDMI_W1_SetWaitPllPwrState(u32 name,
		HDMI_PllPwr_t param)
{
	int r = hdmi_w1_set_wait_pll_pwr(param);
	return r;
}

int HDMI_W1_SetWaitSoftReset(void)
{
	/* reset W1 */
	REG_FLD_MOD(HDMI_WP, HDMI_WP_SYSCONFIG, 0x1, 0, 0);

	/* wait till SOFTRESET == 0 */
	while (FLD_GET(hdmi_read_reg(HDMI_WP, HDMI_WP_SYSCONFIG), 0, 0))
		;
	return 0;
}

int hdmi_w1_wrapper_enable(u32 instanceName)
{
	printk(KERN_INFO "Wrapper Enabled...\n");
	hdmi_w1_audio_enable();
	return 0;
}

int hdmi_w1_wrapper_disable(u32 instanceName)
{
	hdmi_w1_audio_disable();
	printk(KERN_INFO "Wrapper disabled...\n");
	return 0;
}

int hdmi_w1_stop_audio_transfer(u32 instanceName)
{
	hdmi_w1_audio_stop();
	return 0;
}

int hdmi_w1_start_audio_transfer(u32 instanceName)
{
	hdmi_w1_audio_start();
	printk(KERN_INFO "Start audio transfer...\n");
	return 0;
}

void hdmi_add_notifier(struct hdmi_notifier *notifier)
{
	mutex_lock(&hdmi.mutex);
	list_add_tail(&notifier->list, &hdmi.notifier_head);
	mutex_unlock(&hdmi.mutex);
}

void hdmi_remove_notifier(struct hdmi_notifier *notifier)
{
	struct hdmi_notifier *cur, *next;

	list_for_each_entry_safe(cur, next, &hdmi.notifier_head, list) {
		if (cur == notifier) {
			mutex_lock(&hdmi.mutex);
			list_del(&cur->list);
			mutex_unlock(&hdmi.mutex);
		}
	}
}

void hdmi_notify_hpd(int state)
{
	struct hdmi_notifier *cur, *next;

	list_for_each_entry_safe(cur, next, &hdmi.notifier_head, list) {
		if (cur->hpd_notifier)
			cur->hpd_notifier(state, cur->private_data);
	}
}


int hdmi_get_hpd_pin_state(void)
{
	int state = hdmi_read_reg(HDMI_CORE_SYS, HDMI_CORE_SYS__SYS_STAT);
	return (state & 0x00000002);
}

void hdmi_core_software_reset(void)
{
	hdmi_core_swreset_assert();
	return;
}

/* This function will return the current video clock  */
int hdmi_lib_get_pixel_clock()
{
	return hdmi.hdmi_cfg.pixel_clock;

}

/* TODO - Check if this function can be deprecated */
int hdmi_get_video_timing()
 {
       int ret = 0;

       DBG("TDMS: %s %d\n", __func__, hdmi.avi_param.db4vic_videocode);

       switch (hdmi.avi_param.db4vic_videocode) {
       case 16:        /* 1080P-60 */
               ret = 148500;
               break;
       case 5:         /* 1080I-60 */
       case 4:         /* 720P-60  */
       case 34:        /* 1080P-30 */
               ret = 74250;
               break;
       default:
               ret = -1;
       }

       return ret;
}


/*******************************************************************************
 *	CEC Specifics
 ******************************************************************************/

int hdmi_lib_cec_activate(void)
{
	u32 temp;

	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_SET_UP);
	if (FLD_GET(temp, 4, 4) != 0) {
		temp = FLD_MOD(temp, 0, 4, 4);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_SET_UP, temp);

		/* If we enabled CEC in middle of a CEC messages on CEC n/w,
			we will could have start bit irregularity and/or short
			pulse event. Clear them now */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_1);
		temp = FLD_MOD(0x0, 0x5, 2, 0);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_1, temp);
	}
	return 0x0;
}

int hdmi_lib_cec_reg_dev(struct ti81xxhdmi_cec_register_dev *dev, __u8 *pa)
{
	u32 temp, regis_reg, shift_cnt, old_ca_7_0, old_ca_15_8;
	int r;
	struct ti81xxhdmi_cec_transmit_msg wr_msg;
	/*
	1. Send an Ping command - if required
	2. If acked, return error
	3. Register to receive for registered initiator
	4. send report physical address
	5. check for nacks
	6. set flag is_pa_set to TRUE indicating we are registered.
	*/
	/* In case device requires to be registered as un-registered device,
	skip the ping */
	if (dev->dev_id == 0xF) {
		wr_msg.acked_nacked = 1;
	} else {
		wr_msg.initiator = 0xF;
		wr_msg.destination = dev->dev_id;
		/* cmd and no of arguments are not used for ping command */
		wr_msg.cmd = 0x0;
		wr_msg.no_args = 0x0;

		wr_msg.acked_nacked = 0xFF;
		wr_msg.errors = 0xFFFFFFFF;
		wr_msg.send_ping = 0x1;
		wr_msg.no_retransmits = 5;

		r = hdmi_lib_cec_write_msg(&wr_msg);

		if (r != 0) {
			printk(KERN_ERR "\nCould not Ping device\n");
			return -1;
		}
	}
	old_ca_15_8 = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_CA_15_8);
	old_ca_7_0 = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_CA_7_0);

	/* Check if device is already present */
	if (wr_msg.acked_nacked != 1) {

		/* Register to receive messages intended for this device
			and broad cast messages */
		regis_reg = HDMI_CORE_CEC_CA_7_0;
		shift_cnt = dev->dev_id;
		temp = 0;
		if (dev->dev_id > 0x7) {
			regis_reg = HDMI_CORE_CEC_CA_15_8;
			shift_cnt -= 0x7;
		}
		if (dev->clear_registered_dev == 0x1) {
			hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_CA_7_0, 0);
			hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_CA_15_8, 0);
		} else {
			temp = hdmi_read_reg(HDMI_CORE_CEC, regis_reg);
		}
		temp |= 0x1 << shift_cnt;
		hdmi_write_reg(HDMI_CORE_CEC, regis_reg, temp);

		/* Report Association between physical address and device id */
		/* Report Physical address broadcast message */
		wr_msg.initiator = dev->dev_id;
		wr_msg.destination = 0xF;
		wr_msg.cmd = ti81xxhdmi_cec_cmd_b_report_physical_address;
		wr_msg.no_args = 0x3;
		wr_msg.args[0] = pa[0];
		wr_msg.args[1] = pa[1];
		wr_msg.args[2] = dev->dev_id;
		wr_msg.acked_nacked = 0xFF;
		wr_msg.no_retransmits = 5;
		wr_msg.send_ping = 0;
		wr_msg.errors = 0xFFFFFFFF;
		r = hdmi_lib_cec_write_msg(&wr_msg);
		if ((wr_msg.acked_nacked != 1) || (r != 0x0)) {
			/* Restore previous registration */
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_CA_15_8, old_ca_15_8);
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_CA_7_0, old_ca_7_0);
		}
	} else {
		/* Device present */
		r = -EEXIST;
	}

	return r;
}

int hdmi_lib_cec_write_msg(struct ti81xxhdmi_cec_transmit_msg *msg)
{
	int r = -EINVAL;
	u32 timeout = HDMI_CORE_CEC_TIMEOUT;
	u32 temp, i = 0;

	if (msg == NULL)
		goto error_exit;
	/* 1. Flush TX FIFO - required as change of initiator ID / destination
		ID while TX is in progress could result in courrupted message.
	2. Clear interrupt status registers for TX.
	3. Set initiator Address, set retry count
	4. Set Destination Address
	5. Clear TX interrupt flags - if required
	6. Set the command
	7. Transmit
	8. Check for NACK / ACK - report the same. */

	/* Clear TX FIFO */
	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_DBG_3);
	temp = FLD_MOD(temp, 1, 7, 7);
	hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_DBG_3, temp);

	while (timeout) {
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_DBG_3);
		if (FLD_GET(temp, 7, 7) == 0)
			break;
		/* FIX ME - use msleep instead, as this wait is not a
		 * constant for all platforms */
		timeout--;
	}
	if (timeout == 0x0) {
		printk(KERN_ERR "Could not clear TX FIFO");
		printk(KERN_ERR "\n FIFO Reset - timeouts  : %d - was %d\n",
			timeout, HDMI_CORE_CEC_TIMEOUT);
		goto error_exit;
	}

	/* Clear out TX change event if any */
	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_0);
	if (FLD_GET(temp, 5, 5) == 1) {
		temp = FLD_MOD(0x0, 1, 5, 5);
		printk(KERN_INFO "HDMI CECDBG TX INT STAT0 value - %x\n", temp);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_0, temp);
	}

	/* Set the initiator detination addresses */
	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_INIT);
	temp = FLD_MOD(temp, msg->initiator, 3, 0);
	hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_INIT, temp);

	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_DEST);
	temp = FLD_MOD(temp, msg->destination, 3, 0);
	hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_DEST, temp);

	/* Set the retry count */
	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_DBG_3);
	temp = FLD_MOD(temp, msg->no_retransmits, 6, 4);
	hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_DBG_3, temp);

	if (msg->send_ping)
	{
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_DEST);
		temp = FLD_MOD(temp, 1, 7, 7);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_DEST, temp);
	} else {
		/* Setup command and arguments for the command */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_COMMAND);
		temp = FLD_MOD(temp, msg->cmd, 7, 0);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TX_COMMAND, temp);


		for (i = 0; i < msg->no_args; i++) {
			temp = hdmi_read_reg(HDMI_CORE_CEC,
				(HDMI_CORE_CEC_TX_OPERAND + (i * 4)));
			temp = FLD_MOD(temp, msg->args[i], 7, 0);
			hdmi_write_reg(HDMI_CORE_CEC,
				(HDMI_CORE_CEC_TX_OPERAND + (i * 4)), temp);
		}

		/* Operand count */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TRANSMIT_DATA);
		temp = FLD_MOD(temp, msg->no_args, 3, 0);
		/* Transmit */
		temp = FLD_MOD(temp, 0x1, 4, 4);
		hdmi_write_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_TRANSMIT_DATA, temp);
	}

	timeout = HDMI_CORE_CEC_TIMEOUT + 200;
	i = 0;
	do {
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_0);
		/* Look for TX change event */
		if (FLD_GET(temp, 5, 5) == 1) {
			i = 1;
			/* Clear interrupt status */
			temp = FLD_MOD(0x0, 1, 5, 5);
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_INT_STATUS_0, temp);
			r = 0;
			break;
		}
		/* Wait for re-transmits to expire */
		temp = hdmi_read_reg(HDMI_CORE_CEC,
			HDMI_CORE_CEC_INT_STATUS_1);
		if (FLD_GET(temp, 1, 1) == 0) {
			/* Wait for 7 mSecs - As per CEC protocol
				nominal bit period is ~3 msec
				delay of >= 3 bit period before next attempt
			*/
			mdelay(10);
			timeout--;
		} else {
			/* Nacked ensure to clear the status */
			temp = FLD_MOD(0x0, 1, 1, 1);
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_INT_STATUS_1, temp);
			r = 0;
			break;
		}
	} while (timeout);

	if (timeout == 0x0) {
		printk(KERN_ERR "\nCould not send\n");
		printk(KERN_ERR "\nNo ack / nack sensed\n");
		printk(KERN_ERR "\nResend did not complete in : %d\n",
			((HDMI_CORE_CEC_TIMEOUT - timeout) * 10));
	}

error_exit:
	msg->acked_nacked = i;
	return r;
}

int hdmi_lib_cec_read_msg(struct ti81xxhdmi_cec_received_msg *msg)
{
	int r = 0;
	u32 temp, cmd_cnt, rx_byte_cnt, i;

	/*
	1. Check for RX FIFO non-empty pending pending in INT_STATUS0
	2. Ensure RX_CMD_CNT is non zero in RX_COUNT, no of data bytes & errors
	3. Read the received command from RX_COMMAD
	4. Extract the initiator and destination ID from CMD_HEADER
	5. Read in the operands if any
	6. Clear the current received cmd
	7. Check for RX FIFO overflow, if so let the app know
	8. if RX_CMD_CNT is 0x0, clear the event status in INT_STATUS0
	*/
	r = -EAGAIN;
	temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_0);
	if (FLD_GET(temp, 1, 1)) {
		cmd_cnt = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_RX_COUNT);
		if (FLD_GET(cmd_cnt, 6, 4) == 0x0) {
			printk(KERN_INFO "HDMI CECDBG - flase notification\n");
			/* Stale status, but why, clear them now */
			temp = FLD_MOD(0x0, 1, 1, 1);
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_INT_STATUS_0, temp);
			goto exit_this_func;
		}

		r = 0;
		/* Even in case error, go ahead and recevive command, let the
			apps figure to what to do with this */
		if (FLD_GET(cmd_cnt, 7, 7))
			msg->errors = -1;

		rx_byte_cnt = FLD_GET(cmd_cnt, 3, 0);
		cmd_cnt = FLD_GET(cmd_cnt, 6, 4);

		/* Get initiator / destination */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_RX_CMD_HEAD);
		msg->initiator = FLD_GET(temp, 7, 4);
		msg->destination = FLD_GET(temp, 3, 0);

		/* Get the command and operands */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_RX_COMMAND);
		msg->cmd = FLD_GET(temp, 7, 0);
		msg->no_args = rx_byte_cnt;
		for (i = 0; i < rx_byte_cnt; i++) {
			temp = hdmi_read_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_RX_OPERAND + (i * 4));
			msg->args[i] = FLD_GET(temp, 7, 0);
		}

		/* Clear the just read command */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_RX_CONTROL);
		temp = FLD_MOD(temp, 1, 0, 0);
		hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_RX_CONTROL, temp);

		/* Clear up status registers */
		temp = hdmi_read_reg(HDMI_CORE_CEC, HDMI_CORE_CEC_INT_STATUS_0);
		if (cmd_cnt == 1) {
			temp = FLD_MOD(0x0, 1, 1, 1);
			hdmi_write_reg(HDMI_CORE_CEC,
				HDMI_CORE_CEC_INT_STATUS_0, temp);
		}
	}

exit_this_func:
	return r;
}


EXPORT_SYMBOL(hdmi_lib_enable);
EXPORT_SYMBOL(hdmi_lib_init);
EXPORT_SYMBOL(hdmi_lib_exit);
EXPORT_SYMBOL(hdmi_set_irqs);
EXPORT_SYMBOL(HDMI_W1_HPD_handler);
EXPORT_SYMBOL(HDMI_CORE_DDC_READEDID);
EXPORT_SYMBOL(HDMI_W1_StopVideoFrame);
EXPORT_SYMBOL(HDMI_W1_StartVideoFrame);
EXPORT_SYMBOL(HDMI_W1_SetWaitPhyPwrState);
EXPORT_SYMBOL(HDMI_W1_SetWaitPllPwrState);
EXPORT_SYMBOL(HDMI_W1_SetWaitSoftReset);
EXPORT_SYMBOL(hdmi_w1_wrapper_enable);
EXPORT_SYMBOL(hdmi_w1_wrapper_disable);
EXPORT_SYMBOL(hdmi_w1_stop_audio_transfer);
EXPORT_SYMBOL(hdmi_w1_start_audio_transfer);
EXPORT_SYMBOL(hdmi_add_notifier);
EXPORT_SYMBOL(hdmi_remove_notifier);
EXPORT_SYMBOL(hdmi_notify_hpd);
EXPORT_SYMBOL(hdmi_get_hpd_pin_state);
EXPORT_SYMBOL(hdmi_core_software_reset);
EXPORT_SYMBOL(hdmi_w1_video_start);
EXPORT_SYMBOL(hdmi_w1_video_status);
EXPORT_SYMBOL(hdmi_core_audio_config);
EXPORT_SYMBOL(hdmi_core_audio_mode_enable);
EXPORT_SYMBOL(hdmi_lib_get_pixel_clock);
EXPORT_SYMBOL(hdmi_get_video_timing);
EXPORT_SYMBOL(hdmi_w1_audio_config_format);
EXPORT_SYMBOL(hdmi_core_powerdown_disable);
EXPORT_SYMBOL(hdmi_core_swreset_assert);
EXPORT_SYMBOL(hdmi_core_swreset_release);
EXPORT_SYMBOL(hdmi_w1_audio_config_dma);
EXPORT_SYMBOL(hdmi_lib_cec_reg_dev);
EXPORT_SYMBOL(hdmi_lib_cec_activate);
EXPORT_SYMBOL(hdmi_lib_cec_write_msg);
EXPORT_SYMBOL(hdmi_lib_cec_read_msg);
