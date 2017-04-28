/*
 * Common DCR / SDR / CPR register definitions used on various IBM/AMCC
 * 4xx processors
 *
 *    Copyright 2007 Benjamin Herrenschmidt, IBM Corp
 *                   <benh@kernel.crashing.org>
 *
 * Mostly lifted from asm-ppc/ibm4xx.h by
 *
 *    Copyright (c) 1999 Grant Erickson <grant@lcse.umn.edu>
 *
 */

#ifndef __DCR_REGS_H__
#define __DCR_REGS_H__

/*
 * Most DCRs used for controlling devices such as the MAL, DMA engine,
 * etc... are obtained for the device tree.
 *
 * The definitions in this files are fixed DCRs and indirect DCRs that
 * are commonly used outside of specific drivers or refer to core
 * common registers that may occasionally have to be tweaked outside
 * of the driver main register set
 */

/* CPRs (440GX and 440SP/440SPe) */
#define DCRN_CPR0_CONFIG_ADDR	0xc
#define DCRN_CPR0_CONFIG_DATA	0xd

/* SDRs (440GX and 440SP/440SPe) */
#define DCRN_SDR0_CONFIG_ADDR 	0xe
#define DCRN_SDR0_CONFIG_DATA	0xf

#define SDR0_PFC0		0x4100
#define SDR0_PFC1		0x4101
#define SDR0_PFC1_EPS		0x1c00000
#define SDR0_PFC1_EPS_SHIFT	22
#define SDR0_PFC1_RMII		0x02000000
#define SDR0_MFR		0x4300
#define SDR0_MFR_TAH0 		0x80000000  	/* TAHOE0 Enable */
#define SDR0_MFR_TAH1 		0x40000000  	/* TAHOE1 Enable */
#define SDR0_MFR_PCM  		0x10000000  	/* PPC440GP irq compat mode */
#define SDR0_MFR_ECS  		0x08000000  	/* EMAC int clk */
#define SDR0_MFR_T0TXFL		0x00080000
#define SDR0_MFR_T0TXFH		0x00040000
#define SDR0_MFR_T1TXFL		0x00020000
#define SDR0_MFR_T1TXFH		0x00010000
#define SDR0_MFR_E0TXFL		0x00008000
#define SDR0_MFR_E0TXFH		0x00004000
#define SDR0_MFR_E0RXFL		0x00002000
#define SDR0_MFR_E0RXFH		0x00001000
#define SDR0_MFR_E1TXFL		0x00000800
#define SDR0_MFR_E1TXFH		0x00000400
#define SDR0_MFR_E1RXFL		0x00000200
#define SDR0_MFR_E1RXFH		0x00000100
#define SDR0_MFR_E2TXFL		0x00000080
#define SDR0_MFR_E2TXFH		0x00000040
#define SDR0_MFR_E2RXFL		0x00000020
#define SDR0_MFR_E2RXFH		0x00000010
#define SDR0_MFR_E3TXFL		0x00000008
#define SDR0_MFR_E3TXFH		0x00000004
#define SDR0_MFR_E3RXFL		0x00000002
#define SDR0_MFR_E3RXFH		0x00000001
#define SDR0_UART0		0x0120
#define SDR0_UART1		0x0121
#define SDR0_UART2		0x0122
#define SDR0_UART3		0x0123
#define SDR0_CUST0		0x4000

/* SDR for 405EZ */
#define DCRN_SDR_ICINTSTAT	0x4510
#define ICINTSTAT_ICRX	0x80000000
#define ICINTSTAT_ICTX0	0x40000000
#define ICINTSTAT_ICTX1 0x20000000
#define ICINTSTAT_ICTX	0x60000000

/* SDRs (460EX/460GT) */
#define SDR0_ETH_CFG		0x4103
#define SDR0_ETH_CFG_ECS	0x00000100	/* EMAC int clk source */

/*
 * All those DCR register addresses are offsets from the base address
 * for the SRAM0 controller (e.g. 0x20 on 440GX). The base address is
 * excluded here and configured in the device tree.
 */
#define DCRN_SRAM0_SB0CR	0x00
#define DCRN_SRAM0_SB1CR	0x01
#define DCRN_SRAM0_SB2CR	0x02
#define DCRN_SRAM0_SB3CR	0x03
#define  SRAM_SBCR_BU_MASK	0x00000180
#define  SRAM_SBCR_BS_64KB	0x00000800
#define  SRAM_SBCR_BU_RO	0x00000080
#define  SRAM_SBCR_BU_RW	0x00000180
#define DCRN_SRAM0_BEAR		0x04
#define DCRN_SRAM0_BESR0	0x05
#define DCRN_SRAM0_BESR1	0x06
#define DCRN_SRAM0_PMEG		0x07
#define DCRN_SRAM0_CID		0x08
#define DCRN_SRAM0_REVID	0x09
#define DCRN_SRAM0_DPC		0x0a
#define  SRAM_DPC_ENABLE	0x80000000

/*
 * All those DCR register addresses are offsets from the base address
 * for the SRAM0 controller (e.g. 0x30 on 440GX). The base address is
 * excluded here and configured in the device tree.
 */
#define DCRN_L2C0_CFG		0x00
#define  L2C_CFG_L2M		0x80000000
#define  L2C_CFG_ICU		0x40000000
#define  L2C_CFG_DCU		0x20000000
#define  L2C_CFG_DCW_MASK	0x1e000000
#define  L2C_CFG_TPC		0x01000000
#define  L2C_CFG_CPC		0x00800000
#define  L2C_CFG_FRAN		0x00200000
#define  L2C_CFG_SS_MASK	0x00180000
#define  L2C_CFG_SS_256		0x00000000
#define  L2C_CFG_CPIM		0x00040000
#define  L2C_CFG_TPIM		0x00020000
#define  L2C_CFG_LIM		0x00010000
#define  L2C_CFG_PMUX_MASK	0x00007000
#define  L2C_CFG_PMUX_SNP	0x00000000
#define  L2C_CFG_PMUX_IF	0x00001000
#define  L2C_CFG_PMUX_DF	0x00002000
#define  L2C_CFG_PMUX_DS	0x00003000
#define  L2C_CFG_PMIM		0x00000800
#define  L2C_CFG_TPEI		0x00000400
#define  L2C_CFG_CPEI		0x00000200
#define  L2C_CFG_NAM		0x00000100
#define  L2C_CFG_SMCM		0x00000080
#define  L2C_CFG_NBRM		0x00000040
#define  L2C_CFG_RDBW		0x00000008	/* only 460EX/GT */
#define DCRN_L2C0_CMD		0x01
#define  L2C_CMD_CLR		0x80000000
#define  L2C_CMD_DIAG		0x40000000
#define  L2C_CMD_INV		0x20000000
#define  L2C_CMD_CCP		0x10000000
#define  L2C_CMD_CTE		0x08000000
#define  L2C_CMD_STRC		0x04000000
#define  L2C_CMD_STPC		0x02000000
#define  L2C_CMD_RPMC		0x01000000
#define  L2C_CMD_HCC		0x00800000
#define DCRN_L2C0_ADDR		0x02
#define DCRN_L2C0_DATA		0x03
#define DCRN_L2C0_SR		0x04
#define  L2C_SR_CC		0x80000000
#define  L2C_SR_CPE		0x40000000
#define  L2C_SR_TPE		0x20000000
#define  L2C_SR_LRU		0x10000000
#define  L2C_SR_PCS		0x08000000
#define DCRN_L2C0_REVID		0x05
#define DCRN_L2C0_SNP0		0x06
#define DCRN_L2C0_SNP1		0x07
#define  L2C_SNP_BA_MASK	0xffff0000
#define  L2C_SNP_SSR_MASK	0x0000f000
#define  L2C_SNP_SSR_32G	0x0000f000
#define  L2C_SNP_ESR		0x00000800

/*
 * DCR register offsets for 440SP/440SPe I2O/DMA controller.
 * The base address is configured in the device tree.
 */
#define DCRN_I2O0_IBAL		0x006
#define DCRN_I2O0_IBAH		0x007
#define I2O_REG_ENABLE		0x00000001	/* Enable I2O/DMA access */

/* 440SP/440SPe Software Reset DCR */
#define DCRN_SDR0_SRST		0x0200
#define DCRN_SDR0_SRST_I2ODMA	(0x80000000 >> 15)	/* Reset I2O/DMA */

/* 440SP/440SPe Memory Queue DCR offsets */
#define DCRN_MQ0_XORBA		0x04
#define DCRN_MQ0_CF2H		0x06
#define DCRN_MQ0_CFBHL		0x0f
#define DCRN_MQ0_BAUH		0x10

/* HB/LL Paths Configuration Register */
#define MQ0_CFBHL_TPLM		28
#define MQ0_CFBHL_HBCL		23
#define MQ0_CFBHL_POLY		15

#ifdef CONFIG_APM86xxx
/*
 * DCR registers for APM86xxx
 */

#define DCRN_L2C_ADDR	0x00
#define DCRN_L2C_DATA	0x01

#define DCRN_L2CR0	0x00
#define  L2CR0_AS_MASK	0x30000000
#define  L2CR0_AS_NO	0x00000000
#define  L2CR0_AS_256K	0x10000000
#define  L2CR0_TAA_MASK	0x03000000
#define  L2CR0_TAA_1	0x01000000
#define  L2CR0_TAA_2	0x02000000
#define  L2CR0_TAA_3	0x03000000
#define  L2CR0_DAA_MASK	0x00300000
#define  L2CR0_DAA_1	0x00100000
#define  L2CR0_DAA_2	0x00200000
#define  L2CR0_DAA_3	0x00300000
#define  L2CR0_TTWEN	0x00004000
#define  L2CR0_TTW_0	0x00000000
#define  L2CR0_TTW_1	0x00001000
#define  L2CR0_TTW_2	0x00002000
#define  L2CR0_TTW_3	0x00003000
#define  L2CR0_DECC	0x00000008
#define  L2CR0_DECA	0x00000004
#define  L2CR0_TAI	0x00000002
#define DCRN_L2CR1	0x01
#define  L2CR1_FAM_OFF	0x00000000
#define  L2CR1_FAM_HALF	0x40000000
#define  L2CR1_FAM_ALL	0x80000000
#define  L2CR1_PUBFAM	0x20000000
#define  L2CR1_FDIOD	0x10000000
#define  L2CR1_SL1LU	0x08000000
#define  L2CR1_SL1DLU	0x04000000
#define  L2CR1_DSG	0x01000000
#define  L2CR1_TAS	0x00800000
#define  L2CR1_RAS	0x00400000
#define DCRN_L2CR2	0x02
#define  L2CR2_SNPME	0x80000000
#define  L2CR2_MEI	0x10000000
#define  L2CR2_CSNPKF	0x08000000
#define  L2CR2_CSNPEP	0x04000000
#define  L2CR2_CSNPPF	0x02000000
#define  L2CR2_FRS	0x01000000
#define  L2CR2_SNPRQPD3	0x00300000
#define  L2CR2_L1CE	0x00008000
#define DCRN_L2CR3	0x03
#define  L2CR3_PLBPE	0x80000000
#define  L2CR3_MSTOORE	0x40000000
#define DCRN_L2ERAPR	0x08
#define DCRN_L2SLVERAPR	0x09
#define DCRN_L2FAMAR	0x0A
#define DCRN_L2REVID	0x0B
#define DCRN_L2COPR	0x0C
#define DCRN_L2THRR	0x0D
#define DCRN_L2FER0	0x0E
#define DCRN_L2FER1	0x0F
#define DCRN_L2MCSR	0x10
#define  L2MCSR_MCS	0x80000000
#define  L2MCSR_ITSBE	0x40000000
#define  L2MCSR_ITDBE	0x20000000
#define  L2MCSR_IDSBE	0x10000000
#define  L2MCSR_IDDBE	0x08000000
#define  L2MCSR_DTSBE	0x04000000
#define  L2MCSR_DTDBE	0x02000000
#define  L2MCSR_DDSBE	0x01000000
#define  L2MCSR_DDDBE	0x00800000
#define  L2MCSR_SNPTSBE	0x00400000
#define  L2MCSR_SNPTDBE	0x00200000
#define  L2MCSR_SNPDSBE	0x00100000
#define  L2MCSR_SNPDDBE	0x00080000
#define  L2MCSR_FARM	0x00040000
#define  L2MCSR_MSTRQE	0x00008000
#define  L2MCSR_MSTRDE	0x00004000
#define  L2MCSR_MSTRDPE	0x00002000
#define  L2MCSR_SNPAPE	0x00001000
#define  L2MCSR_SLVWDE	0x00000400
#define  L2MCSR_SLVWDPE	0x00000200
#define  L2MCSR_SLVAPE	0x00000100
#define  L2MCSR_SLVBEPE	0x00000080
#define  L2MCSR_SLVDSBE	0x00000040
#define  L2MCSR_SLVDDBE	0x00000020
#define  L2MCSR_SLVIAE	0x00000010
#define  L2MCSR_SLVICE	0x00000008
#define  L2MCSR_DCRTO	0x00000001
#define DCRN_L2MCRER	0x11
#define DCRN_L2ECCIDX	0x12
#define DCRN_L2SLVEAR0	0x14
#define DCRN_L2SLVEAR1	0x15
#define DCRN_L2SLVMIDR	0x16
#define DCRN_L2DBACMR	0x17
#define DCRN_L2DBSR	0x18
#define DCRN_L2DBCR	0x19
#define DCRN_L2DBACSR	0x1A
#define DCRN_L2DBACCR	0x1B
#define DCRN_L2SLVAC0	0x1C
#define DCRN_L2DBACR	0x1D
#define DCRN_L2SNPAC0	0x1E
#define DCRN_L2SNPAC1	0x1F
#define DCRN_L2DBDR0	0x20
#define DCRN_L2DBDR1	0x21
#define DCRN_L2DBDR2	0x22
#define DCRN_L2DBDR3	0x23
#define DCRN_L2DBDR4	0x24
#define DCRN_L2DBTR0	0x25
#define DCRN_L2DBTR1	0x26
#define DCRN_L2DBTR2	0x27
#define DCRN_L2MCSRS	0x30
#define DCRN_L2DBSRS	0x38
#define DCRN_L2DBACSRS	0x3A

#endif /* CONFIG_APM86xxx */

#endif /* __DCR_REGS_H__ */
