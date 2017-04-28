/*
 * drivers/ata/sata_dwc.c
 *
 * Synopsys DesignWare Cores (DWC) SATA host driver
 *
 * Author: Mark Miesfeld <mmiesfeld@amcc.com>
 *
 * Ported from 2.6.19.2 to 2.6.25/26 by Stefan Roese <sr@denx.de>
 * Copyright 2008 DENX Software Engineering
 *
 * Based on versions provided by AMCC and Synopsys which are:
 *          Copyright 2006 Applied Micro Circuits Corporation
 *          COPYRIGHT (C) 2005  SYNOPSYS, INC.  ALL RIGHTS RESERVED
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifdef CONFIG_SATA_DWC_DEBUG
#define DEBUG
#endif

#ifdef CONFIG_SATA_DWC_VDEBUG
#define VERBOSE_DEBUG
#define DEBUG_NCQ
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/libata.h>

#include <scsi/scsi_host.h>
#include <scsi/scsi_cmnd.h>

#define DRV_NAME        "sata-dwc"
#define DRV_VERSION     "1.0"

/* SATA DMA driver Globals */
#if defined(CONFIG_APM82181)
#define DMA_NUM_CHANS			2
#else
#define DMA_NUM_CHANS			1
#endif

#define DMA_NUM_CHAN_REGS		8

/* SATA DMA Register definitions */
#if defined(CONFIG_APM82181)
#define AHB_DMA_BRST_DFLT              	64      /* 16 data items burst length */
#else
#define AHB_DMA_BRST_DFLT		64	/* 16 data items burst length */
#endif

struct dmareg {
	u32 low;		/* Low bits 0-31 */
	u32 high;		/* High bits 32-63 */
};

/* DMA Per Channel registers */

struct dma_chan_regs {
	struct dmareg sar;	/* Source Address */
	struct dmareg dar;	/* Destination address */
	struct dmareg llp;	/* Linked List Pointer */
	struct dmareg ctl;	/* Control */
	struct dmareg sstat;	/* Source Status not implemented in core */
	struct dmareg dstat;	/* Destination Status not implemented in core */
	struct dmareg sstatar;	/* Source Status Address not impl in core */
	struct dmareg dstatar;	/* Destination Status Address not implemented */
	struct dmareg cfg;	/* Config */
	struct dmareg sgr;	/* Source Gather */
	struct dmareg dsr;	/* Destination Scatter */
};

/* Generic Interrupt Registers */
struct dma_interrupt_regs {
	struct dmareg tfr;	/* Transfer Interrupt */
	struct dmareg block;	/* Block Interrupt */
	struct dmareg srctran;	/* Source Transfer Interrupt */
	struct dmareg dsttran;	/* Dest Transfer Interrupt */
	struct dmareg error;	/* Error */
};

struct ahb_dma_regs {
	struct dma_chan_regs	chan_regs[DMA_NUM_CHAN_REGS];
	struct dma_interrupt_regs interrupt_raw;	/* Raw Interrupt */
	struct dma_interrupt_regs interrupt_status;	/* Interrupt Status */
	struct dma_interrupt_regs interrupt_mask;	/* Interrupt Mask */
	struct dma_interrupt_regs interrupt_clear;	/* Interrupt Clear */
	struct dmareg		statusInt;		/* Interrupt combined */
	struct dmareg		rq_srcreg;		/* Src Trans Req */
	struct dmareg		rq_dstreg;		/* Dst Trans Req */
	struct dmareg		rq_sgl_srcreg;		/* Sngl Src Trans Req */
	struct dmareg		rq_sgl_dstreg;		/* Sngl Dst Trans Req */
	struct dmareg		rq_lst_srcreg;		/* Last Src Trans Req */
	struct dmareg		rq_lst_dstreg;		/* Last Dst Trans Req */
	struct dmareg		dma_cfg;		/* DMA Config */
	struct dmareg		dma_chan_en;		/* DMA Channel Enable */
	struct dmareg		dma_id;			/* DMA ID */
	struct dmareg		dma_test;		/* DMA Test */
	struct dmareg		res1;			/* reserved */
	struct dmareg		res2;			/* reserved */

	/* DMA Comp Params
	 * Param 6 = dma_param[0], Param 5 = dma_param[1],
	 * Param 4 = dma_param[2] ...
	 */
	struct dmareg		dma_params[6];
};

/* Data structure for linked list item */
struct lli {
	u32		sar;		/* Source Address */
	u32		dar;		/* Destination address */
	u32		llp;		/* Linked List Pointer */
	struct dmareg	ctl;		/* Control */
#if defined(CONFIG_APM82181)
	u32             dstat;          /* Source status is not supported */
#else
	struct dmareg	dstat;		/* Destination Status */
#endif
};

#define SATA_DWC_DMAC_LLI_SZ		(sizeof(struct lli))
#define SATA_DWC_DMAC_LLI_NUM		256
#define SATA_DWC_DMAC_TWIDTH_BYTES	4
#define SATA_DWC_DMAC_LLI_TBL_SZ	\
	(SATA_DWC_DMAC_LLI_SZ * SATA_DWC_DMAC_LLI_NUM)
#if defined(CONFIG_APM82181)
#define SATA_DWC_DMAC_CTRL_TSIZE_MAX    \
        (0x00000800 * SATA_DWC_DMAC_TWIDTH_BYTES)
#else
#define SATA_DWC_DMAC_CTRL_TSIZE_MAX	\
	(0x00000800 * SATA_DWC_DMAC_TWIDTH_BYTES)
#endif
/* DMA Register Operation Bits */
#define DMA_EN			0x00000001		/* Enable AHB DMA */
#define DMA_CHANNEL(ch)		(0x00000001 << (ch))	/* Select channel */
#define DMA_ENABLE_CHAN(ch)	((0x00000001 << (ch)) |			\
				 ((0x000000001 << (ch)) << 8))
#define DMA_DISABLE_CHAN(ch)	(0x00000000 | ((0x000000001 << (ch)) << 8))

/* Channel Control Register */
#define DMA_CTL_BLK_TS(size)	((size) & 0x000000FFF)	/* Blk Transfer size */
#define DMA_CTL_LLP_SRCEN	0x10000000	/* Blk chain enable Src */
#define DMA_CTL_LLP_DSTEN	0x08000000	/* Blk chain enable Dst */
/*
 * This define is used to set block chaining disabled in the control low
 * register.  It is already in little endian format so it can be &'d dirctly.
 * It is essentially: cpu_to_le32(~(DMA_CTL_LLP_SRCEN | DMA_CTL_LLP_DSTEN))
 */
#define DMA_CTL_LLP_DISABLE_LE32 0xffffffe7
#define DMA_CTL_SMS(num)	((num & 0x3) << 25)	/*Src Master Select*/
#define DMA_CTL_DMS(num)	((num & 0x3) << 23)	/*Dst Master Select*/
#define DMA_CTL_TTFC(type)	((type & 0x7) << 20)	/*Type&Flow cntr*/
#define DMA_CTL_TTFC_P2M_DMAC	0x00000002		/*Per mem,DMAC cntr*/
#define DMA_CTL_TTFC_M2P_PER	0x00000003		/*Mem per,peri cntr*/
#define DMA_CTL_SRC_MSIZE(size)	((size & 0x7) << 14)	/*Src Burst Len*/
#define DMA_CTL_DST_MSIZE(size)	((size & 0x7) << 11)	/*Dst Burst Len*/
#define DMA_CTL_SINC_INC	0x00000000		/*Src addr incr*/
#define DMA_CTL_SINC_DEC	0x00000200
#define DMA_CTL_SINC_NOCHANGE	0x00000400
#define DMA_CTL_DINC_INC	0x00000000		/*Dst addr incr*/
#define DMA_CTL_DINC_DEC	0x00000080
#define DMA_CTL_DINC_NOCHANGE	0x00000100
#define DMA_CTL_SRC_TRWID(size)	((size & 0x7) << 4)	/*Src Trnsfr Width*/
#define DMA_CTL_DST_TRWID(size)	((size & 0x7) << 1)	/*Dst Trnsfr Width*/
#define DMA_CTL_INT_EN		0x00000001		/*Interrupt Enable*/

/* Channel Configuration Register high bits */
#define DMA_CFG_FCMOD_REQ	0x00000001		/*Flow cntrl req*/
#define DMA_CFG_PROTCTL		(0x00000003 << 2)	/*Protection cntrl*/

/* Channel Configuration Register low bits */
#define DMA_CFG_RELD_DST	0x80000000		/*Reload Dst/Src Addr*/
#define DMA_CFG_RELD_SRC	0x40000000
#define DMA_CFG_HS_SELSRC	0x00000800		/*SW hndshk Src/Dst*/
#define DMA_CFG_HS_SELDST	0x00000400
#define DMA_CFG_FIFOEMPTY       (0x00000001 << 9)	/*FIFO Empty bit*/

/* Assign hardware handshaking interface (x) to dst / sre peripheral */
#define DMA_CFG_HW_HS_DEST(int_num)	((int_num & 0xF) << 11)
#define DMA_CFG_HW_HS_SRC(int_num)	((int_num & 0xF) << 7)

/* Channel Linked List Pointer Register */
#define DMA_LLP_LMS(addr, master)	(((addr) & 0xfffffffc) | (master))
#define DMA_LLP_AHBMASTER1		0	/* List Master Select */
#define DMA_LLP_AHBMASTER2		1

#define SATA_DWC_MAX_PORTS	1

#define SATA_DWC_SCR_OFFSET	0x24
#define SATA_DWC_REG_OFFSET	0x64

/* DWC SATA Registers */
struct sata_dwc_regs {
	u32 fptagr;		/* 1st party DMA tag */
	u32 fpbor;		/* 1st party DMA buffer offset */
	u32 fptcr;		/* 1st party DMA Xfr count */
	u32 dmacr;		/* DMA Control */
	u32 dbtsr;		/* DMA Burst Transac size */
	u32 intpr;		/* Interrupt Pending */
	u32 intmr;		/* Interrupt Mask */
	u32 errmr;		/* Error Mask */
	u32 llcr;		/* Link Layer Control */
	u32 phycr;		/* PHY Control */
	u32 physr;		/* PHY Status */
	u32 rxbistpd;		/* Recvd BIST pattern def register */
	u32 rxbistpd1;		/* Recvd BIST data dword1 */
	u32 rxbistpd2;		/* Recvd BIST pattern data dword2 */
	u32 txbistpd;		/* Trans BIST pattern def register */
	u32 txbistpd1;		/* Trans BIST data dword1 */
	u32 txbistpd2;		/* Trans BIST data dword2 */
	u32 bistcr;		/* BIST Control Register */
	u32 bistfctr;		/* BIST FIS Count Register */
	u32 bistsr;		/* BIST Status Register */
	u32 bistdecr;		/* BIST Dword Error count register */
	u32 res[15];		/* Reserved locations */
	u32 testr;		/* Test Register */
	u32 versionr;		/* Version Register */
	u32 idr;		/* ID Register */
	u32 unimpl[192];	/* Unimplemented */
	u32 dmadr[256];	/* FIFO Locations in DMA Mode */
};

#define SCR_SCONTROL_DET_ENABLE		0x00000001
#define SCR_SSTATUS_DET_PRESENT		0x00000001
#define SCR_SERROR_DIAG_X		0x04000000

/* DWC SATA Register Operations */
#define	SATA_DWC_TXFIFO_DEPTH		0x01FF
#define	SATA_DWC_RXFIFO_DEPTH		0x01FF

#define SATA_DWC_DMACR_TMOD_TXCHEN	0x00000004
#define	SATA_DWC_DMACR_TXCHEN		(0x00000001 | \
						SATA_DWC_DMACR_TMOD_TXCHEN)
#define	SATA_DWC_DMACR_RXCHEN		(0x00000002 | \
						SATA_DWC_DMACR_TMOD_TXCHEN)
#define SATA_DWC_DMACR_TX_CLEAR(v)	(((v) & ~SATA_DWC_DMACR_TXCHEN) | \
						SATA_DWC_DMACR_TMOD_TXCHEN)
#define SATA_DWC_DMACR_RX_CLEAR(v)	(((v) & ~SATA_DWC_DMACR_RXCHEN) | \
						SATA_DWC_DMACR_TMOD_TXCHEN)
#define SATA_DWC_DMACR_TXRXCH_CLEAR	SATA_DWC_DMACR_TMOD_TXCHEN

#define SATA_DWC_DBTSR_MWR(size)	((size/4) & \
						SATA_DWC_TXFIFO_DEPTH)
#define SATA_DWC_DBTSR_MRD(size)	(((size/4) & \
						SATA_DWC_RXFIFO_DEPTH) << 16)
#define	SATA_DWC_INTPR_DMAT		0x00000001
#define SATA_DWC_INTPR_NEWFP		0x00000002
#define SATA_DWC_INTPR_PMABRT		0x00000004
#define SATA_DWC_INTPR_ERR		0x00000008
#define SATA_DWC_INTPR_NEWBIST		0x00000010
#define SATA_DWC_INTPR_IPF		0x10000000
#define	SATA_DWC_INTMR_DMATM		0x00000001
#define SATA_DWC_INTMR_NEWFPM		0x00000002
#define SATA_DWC_INTMR_PMABRTM		0x00000004
#define SATA_DWC_INTMR_ERRM		0x00000008
#define SATA_DWC_INTMR_NEWBISTM		0x00000010
#define SATA_DWC_LLCR_SCRAMEN		0x00000001
#define SATA_DWC_LLCR_DESCRAMEN		0x00000002
#define SATA_DWC_LLCR_RPDEN		0x00000004

/* This is all error bits, zero's are reserved fields. */
#define SATA_DWC_SERROR_ERR_BITS	0x0FFF0F03

#define SATA_DWC_SCR0_SPD_GET(v)	((v >> 4) & 0x0000000F)

struct sata_dwc_device {
	struct resource reg;            /* Resource for register */
	struct device		*dev;		/* generic device struct */
	struct ata_probe_ent	*pe;		/* ptr to probe-ent */
	struct ata_host		*host;
	u8			*reg_base;
	struct sata_dwc_regs	*sata_dwc_regs;	/* DW Synopsys SATA specific */
	u8     			*scr_base;
	int    			dma_channel;	/* DWC SATA DMA channel  */
	int			irq_dma;
};

#define SATA_DWC_QCMD_MAX	32

struct sata_dwc_device_port {
	struct sata_dwc_device	*hsdev;
	int			cmd_issued[SATA_DWC_QCMD_MAX];
	struct lli		*llit[SATA_DWC_QCMD_MAX];
	dma_addr_t		llit_dma[SATA_DWC_QCMD_MAX];
	u32			dma_chan[SATA_DWC_QCMD_MAX];
	int			dma_pending[SATA_DWC_QCMD_MAX];
	u32 			sata_dwc_sactive_issued;	/* issued queued ops */
	u32 			sata_dwc_sactive_queued;	/* queued ops */
	u32 			dma_interrupt_count;

};

static struct sata_dwc_device* dwc_dev_list[2];
static int dma_intr_registered = 0;

/*
 * Commonly used DWC SATA driver Macros
 */
#define HSDEV_FROM_HOST(host)	((struct sata_dwc_device *) \
					(host)->private_data)
#define HSDEV_FROM_AP(ap)	((struct sata_dwc_device *) \
					(ap)->host->private_data)
#define HSDEVP_FROM_AP(ap)	((struct sata_dwc_device_port *) \
					(ap)->private_data)
#define HSDEV_FROM_QC(qc)	((struct sata_dwc_device *) \
					(qc)->ap->host->private_data)
#define HSDEV_FROM_HSDEVP(p)	((struct sata_dwc_device *) \
					(hsdevp)->hsdev)

enum {
	SATA_DWC_CMD_ISSUED_NOT		= 0,
	SATA_DWC_CMD_ISSUED_PENDING	= 1,
	SATA_DWC_CMD_ISSUED_EXEC	= 2,
	SATA_DWC_CMD_ISSUED_NODATA	= 3,

	SATA_DWC_DMA_PENDING_NONE	= 0,
	SATA_DWC_DMA_PENDING_TX		= 1,
	SATA_DWC_DMA_PENDING_RX		= 2,
};

/*
 * Globals
 */
static struct ahb_dma_regs *sata_dma_regs = 0;
/*
* Duc: Moving to device port field to make it private per each port
*/
//static u32 sata_dwc_sactive_issued = 0;	/* issued queued ops */
//static u32 sata_dwc_sactive_queued = 0;	/* queued ops */
//static u32 dma_interrupt_count;

/*
 * Prototypes
 */
static void sata_dwc_bmdma_start_by_tag(struct ata_queued_cmd *qc, u8 tag);
static int sata_dwc_qc_complete(struct ata_port *ap, struct ata_queued_cmd *qc,
				u32 check_status);
static void sata_dwc_dma_xfer_complete(struct ata_port *ap, u32 check_status);
static void sata_dwc_port_stop(struct ata_port *ap);
static void sata_dwc_clear_dmacr(struct sata_dwc_device_port *hsdevp, u8 tag);

static int dma_dwc_init(struct sata_dwc_device *hsdev, int irq);
static void dma_dwc_exit(struct sata_dwc_device *hsdev);
static int dma_dwc_xfer_setup(struct ata_queued_cmd *qc,
			      struct lli *lli, dma_addr_t dma_lli,
			      void __iomem *addr);
static void dma_dwc_xfer_start(int dma_ch);
static void dma_dwc_terminate_dma(struct ata_port *ap, int dma_ch);

static const char *dir_2_txt(enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_BIDIRECTIONAL:
		return "bi";
	case DMA_FROM_DEVICE:
		return "from";
	case DMA_TO_DEVICE:
		return "to";
	case DMA_NONE:
		return "none";
	default:
		return "err";
	}
}

static const char *prot_2_txt(enum ata_tf_protocols protocol)
{
	switch (protocol) {
	case ATA_PROT_UNKNOWN:
		return "unknown";
	case ATA_PROT_NODATA:
		return "nodata";
	case ATA_PROT_PIO:
		return "pio";
	case ATA_PROT_DMA:
		return "dma";
	case ATA_PROT_NCQ:
		return "ncq";
	case ATAPI_PROT_PIO:
		return "atapi pio";
	case ATAPI_PROT_NODATA:
		return "atapi nodata";
	case ATAPI_PROT_DMA:
		return "atapi dma";
	default:
		return "err";
	}
}

inline const char *ata_cmd_2_txt(const struct ata_taskfile *tf)
{
	switch (tf->command) {
	case ATA_CMD_CHK_POWER:
		return "ATA_CMD_CHK_POWER";
	case ATA_CMD_EDD:
		return "ATA_CMD_EDD";
	case ATA_CMD_FLUSH:
		return "ATA_CMD_FLUSH";
	case ATA_CMD_FLUSH_EXT:
		return "ATA_CMD_FLUSH_EXT";
	case ATA_CMD_ID_ATA:
		return "ATA_CMD_ID_ATA";
	case ATA_CMD_ID_ATAPI:
		return "ATA_CMD_ID_ATAPI";
	case ATA_CMD_FPDMA_READ:
		return "ATA_CMD_FPDMA_READ";
	case ATA_CMD_FPDMA_WRITE:
		return "ATA_CMD_FPDMA_WRITE";
	case ATA_CMD_READ:
		return "ATA_CMD_READ";
	case ATA_CMD_READ_EXT:
		return "ATA_CMD_READ_EXT";
	case ATA_CMD_WRITE:
		return "ATA_CMD_WRITE";
	case ATA_CMD_WRITE_EXT:
		return "ATA_CMD_WRITE_EXT";
	case ATA_CMD_PIO_READ:
		return "ATA_CMD_PIO_READ";
	case ATA_CMD_PIO_READ_EXT:
		return "ATA_CMD_PIO_READ_EXT";
	case ATA_CMD_PIO_WRITE:
		return "ATA_CMD_PIO_WRITE";
	case ATA_CMD_PIO_WRITE_EXT:
		return "ATA_CMD_PIO_WRITE_EXT";
	case ATA_CMD_SET_FEATURES:
		return "ATA_CMD_SET_FEATURES";
	case ATA_CMD_PACKET:
		return "ATA_CMD_PACKET";
	default:
		return "ATA_CMD_???";
	}
}

static void sata_dwc_tf_dump(struct device *dwc_dev, struct ata_taskfile *tf)
{
	dev_vdbg(dwc_dev, "taskfile cmd: 0x%02x protocol: %s flags: 0x%lx"
			"device: %x\n", tf->command, prot_2_txt(tf->protocol),
			tf->flags, tf->device);
	dev_vdbg(dwc_dev, "feature: 0x%02x nsect: 0x%x lbal: 0x%x lbam:"
			"0x%x lbah: 0x%x\n", tf->feature, tf->nsect, tf->lbal,
			tf->lbam, tf->lbah);
	dev_vdbg(dwc_dev, "hob_feature: 0x%02x hob_nsect: 0x%x hob_lbal: 0x%x "
			"hob_lbam: 0x%x hob_lbah: 0x%x\n", tf->hob_feature,
			tf->hob_nsect, tf->hob_lbal, tf->hob_lbam,
			tf->hob_lbah);
}

/*
 * Function: get_burst_length_encode
 * arguments: datalength: length in bytes of data
 * returns value to be programmed in register corrresponding to data length
 * This value is effectively the log(base 2) of the length
 */
static inline int get_burst_length_encode(int datalength)
{
	int items = datalength >> 2;	/* div by 4 to get lword count */

	if (items >= 64)
		return 5;

	if (items >= 32)
		return 4;

	if (items >= 16)
		return 3;

	if (items >= 8)
		return 2;

	if (items >= 4)
		return 1;

	return 0;
}

static inline void clear_chan_interrupts(int c)
{
	out_le32(&(sata_dma_regs->interrupt_clear.tfr.low), DMA_CHANNEL(c));
	out_le32(&(sata_dma_regs->interrupt_clear.block.low), DMA_CHANNEL(c));
	out_le32(&(sata_dma_regs->interrupt_clear.srctran.low), DMA_CHANNEL(c));
	out_le32(&(sata_dma_regs->interrupt_clear.dsttran.low), DMA_CHANNEL(c));
	out_le32(&(sata_dma_regs->interrupt_clear.error.low), DMA_CHANNEL(c));
}

/*
 * Function: dma_request_channel
 * arguments: None
 * returns channel number if available else -1
 * This function assigns the next available DMA channel from the list to the
 * requester
 */
static int dma_request_channel(struct ata_port *ap)
{
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	
	if (!(in_le32(&(sata_dma_regs->dma_chan_en.low)) & DMA_CHANNEL(hsdev->dma_channel))) {
	#if defined(VERBOSE_DEBUG)
		dev_notice(ap->dev, "%s Successfully requested DMA channel %d\n", 
				__func__, hsdev->dma_channel);
	#endif
		return (hsdev->dma_channel);
	}
	
	return -1;
}



/*
 * Function: dma_dwc_interrupt
 * arguments: irq, dev_id, pt_regs
 * returns channel number if available else -1
 * Interrupt Handler for DW AHB SATA DMA
 */
static int dma_dwc_interrupt(int irq, void *hsdev_instance)
{
	int chan;
	volatile u32 tfr_reg, err_reg;
	unsigned long flags;
	struct sata_dwc_device *hsdev =
		(struct sata_dwc_device *)hsdev_instance;
	struct ata_host *host = (struct ata_host *)hsdev->host;
	struct ata_port *ap;
	struct sata_dwc_device_port *hsdevp;
	u8 tag = 0;
	unsigned int port = 0;
	spin_lock_irqsave(&host->lock, flags);


	ap = host->ports[port];
	hsdevp = HSDEVP_FROM_AP(ap);
	tag = ap->link.active_tag;
#if defined(VERBOSE_DEBUG)
	dev_notice(ap->dev, "%s: DMA interrupt in channel %d\n", __func__, hsdevp->hsdev->dma_channel);
#endif
	tfr_reg = in_le32(&(sata_dma_regs->interrupt_status.tfr.low));
	err_reg = in_le32(&(sata_dma_regs->interrupt_status.error.low));

	dev_dbg(ap->dev, "eot=0x%08x err=0x%08x pending=%d active port=%d\n",
		tfr_reg, err_reg, hsdevp->dma_pending[tag], port);
#if 0
	for (chan = 0; chan < DMA_NUM_CHANS; chan++) {
		/* Check for end-of-transfer interrupt. */
		
		/* Do nothing if this interrupt does not happen in hsdev DMA channel */
		if (hsdev->dma_channel != chan) {
			continue;
		}
#endif				
		chan = hsdev->dma_channel;				
	
		if (tfr_reg & DMA_CHANNEL(chan)) {
			/*
			 *Each DMA command produces 2 interrupts.  Only
			 * complete the command after both interrupts have been
			 * seen. (See sata_dwc_isr())
			 */
			hsdevp->dma_interrupt_count++;

			sata_dwc_clear_dmacr(hsdevp, tag);

			if (unlikely(hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_NONE)) {
				dev_err(ap->dev, "DMA not pending eot=0x%08x "
					"err=0x%08x tag=0x%02x pending=%d\n",
					tfr_reg, err_reg, tag,
					hsdevp->dma_pending[tag]);
			}

			if ((hsdevp->dma_interrupt_count % 2) == 0)
				sata_dwc_dma_xfer_complete(ap, 1);

			/* Clear the interrupt */
			out_le32(&(sata_dma_regs->interrupt_clear.tfr.low),
				  DMA_CHANNEL(chan));
		}

		/* Check for error interrupt. */
		/* We do not expect error happens */
		if (unlikely(err_reg & DMA_CHANNEL(chan))) {
			/* TODO Need error handler ! */
			dev_err(ap->dev, "error interrupt err_reg=0x%08x\n",
					err_reg);

			/* Clear the interrupt. */
			out_le32(&(sata_dma_regs->interrupt_clear.error.low),
				  DMA_CHANNEL(chan));
		}
#if 0
	}
#endif	
	spin_unlock_irqrestore(&host->lock, flags);
	return IRQ_HANDLED;
}

static irqreturn_t dma_dwc_handler(int irq, void *hsdev_instance)
{
	volatile u32 tfr_reg, err_reg;
	int chan;
	
	tfr_reg = in_le32(&(sata_dma_regs->interrupt_status.tfr.low));	
	err_reg = in_le32(&(sata_dma_regs->interrupt_status.error.low));

	for (chan = 0; chan < DMA_NUM_CHANS; chan++) {
		/* Check for end-of-transfer interrupt. */
		
		if (tfr_reg & DMA_CHANNEL(chan)) {
			dma_dwc_interrupt(0, dwc_dev_list[chan]);		
		}

		/* Check for error interrupt. */
		if (err_reg & DMA_CHANNEL(chan)) {
			dma_dwc_interrupt(0, dwc_dev_list[chan]);
		}
	}

	return IRQ_HANDLED;
}


/*
 * Function: dma_request_interrupts
 * arguments: hsdev
 * returns status
 * This function registers ISR for a particular DMA channel interrupt
 */
static int dma_request_interrupts(struct sata_dwc_device *hsdev, int irq)
{
	int retval = 0;
	int chan;

	for (chan = 0; chan < DMA_NUM_CHANS; chan++) {
		/* Unmask error interrupt */
		out_le32(&sata_dma_regs->interrupt_mask.error.low,
			 in_le32(&sata_dma_regs->interrupt_mask.error.low) | DMA_ENABLE_CHAN(chan));

		/* Unmask end-of-transfer interrupt */
		out_le32(&sata_dma_regs->interrupt_mask.tfr.low,
			in_le32(&sata_dma_regs->interrupt_mask.tfr.low) | DMA_ENABLE_CHAN(chan));
#if 0
	        out_le32(&sata_dma_regs->interrupt_mask.block.low,
                         DMA_ENABLE_CHAN(chan));

                out_le32(&sata_dma_regs->interrupt_mask.srctran.low,
                         DMA_ENABLE_CHAN(chan));

                out_le32(&sata_dma_regs->interrupt_mask.dsttran.low,
                         DMA_ENABLE_CHAN(chan));
#endif
	}

	/* 
	 * FIXME: 2 SATA controllers share the same DMA engine so
	 * currently, they also share same DMA interrupt
	 */
	if (!dma_intr_registered) {
		//retval = request_irq(irq, dma_dwc_interrupt, IRQF_SHARED, "SATA DMA", hsdev);
		retval = request_irq(irq, dma_dwc_handler, IRQF_DISABLED, "SATA DMA", NULL);
		if (retval) {
			dev_err(hsdev->dev, "%s: could not get IRQ %d\n", __func__, irq);
			return -ENODEV;
		}
		dma_intr_registered = 1;
	}

	
	/* Mark this interrupt as requested */
	hsdev->irq_dma = irq;

	return 0;
}

/*
 * Function: map_sg_to_lli
 * arguments: sg: scatter/gather list(sg)
 *	      num_elems: no of elements in sg list
 *	      dma_lli: LLI table
 *	      dest: destination address
 *	      read: whether the transfer is read or write
 * returns array of AHB DMA Linked List Items
 * This function creates a list of LLIs for DMA Xfr and returns the number
 * of elements in the DMA linked list.
 *
 * Note that the Synopsis driver has a comment proposing that better performance
 * is possible by only enabling interrupts on the last item in the linked list.
 * However, it seems that could be a problem if an error happened on one of the
 * first items.  The transfer would halt, but no error interrupt would occur.
 *
 * Currently this function sets interrupts enabled for each linked list item:
 * DMA_CTL_INT_EN.
 */
static int map_sg_to_lli(struct ata_queued_cmd *qc, struct lli *lli,
			 dma_addr_t dma_lli, void __iomem *dmadr_addr)
{
	struct scatterlist *sg = qc->sg;
	struct device *dwc_dev = qc->ap->dev;
	int num_elems = qc->n_elem;
	int dir = qc->dma_dir;
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(qc->ap);
	
	int i, idx = 0;
	int fis_len = 0;
	dma_addr_t next_llp;
	int bl;
	unsigned int dma_ts = 0;

	dev_dbg(dwc_dev, "%s: sg=%p nelem=%d lli=%p dma_lli=0x%08x "
		"dmadr=0x%08x\n", __func__, sg, num_elems, lli, (u32)dma_lli,
		(u32)dmadr_addr);

	bl = get_burst_length_encode(AHB_DMA_BRST_DFLT);

	for (i = 0; i < num_elems; i++, sg++) {
		u32 addr, offset;
		u32 sg_len, len;

		addr = (u32) sg_dma_address(sg);
		sg_len = sg_dma_len(sg);

		dev_dbg(dwc_dev, "%s: elem=%d sg_addr=0x%x sg_len=%d\n",
			__func__, i, addr, sg_len);

		while (sg_len) {

			if (unlikely(idx >= SATA_DWC_DMAC_LLI_NUM)) {
				/* The LLI table is not large enough. */
				dev_err(dwc_dev, "LLI table overrun (idx=%d)\n",
						idx);
				break;
			}
			len = (sg_len > SATA_DWC_DMAC_CTRL_TSIZE_MAX) ?
				SATA_DWC_DMAC_CTRL_TSIZE_MAX : sg_len;

			offset = addr & 0xffff;
			if ((offset + sg_len) > 0x10000)
				len = 0x10000 - offset;

			/*
			 * Make sure a LLI block is not created that will span a
			 * 8K max FIS boundary.  If the block spans such a FIS
			 * boundary, there is a chance that a DMA burst will
			 * cross that boundary -- this results in an error in
			 * the host controller.
			 */
			if (unlikely(fis_len + len > 8192)) {
				dev_dbg(dwc_dev, "SPLITTING: fis_len=%d(0x%x) "
					"len=%d(0x%x)\n", fis_len, fis_len,
					len, len);
				len = 8192 - fis_len;
				fis_len = 0;
			} else {
				fis_len += len;
			}
			if (fis_len == 8192)
				fis_len = 0;

			/*
			 * Set DMA addresses and lower half of control register
			 * based on direction.
			 */
#if defined(VERBOSE_DEBUG)
                        dev_notice(qc->ap->dev, "%s: sg_len = %d, len = %d\n", __func__, sg_len, len);
#endif

#if defined(CONFIG_APM82181)
			 if (dir == DMA_FROM_DEVICE) {
                                lli[idx].dar = cpu_to_le32(addr);
                                lli[idx].sar = cpu_to_le32((u32)dmadr_addr);
				if (hsdevp->hsdev->dma_channel == 0) {/* DMA channel 0 */
		                        lli[idx].ctl.low = cpu_to_le32(
                                        DMA_CTL_TTFC(DMA_CTL_TTFC_P2M_DMAC) |
                                        DMA_CTL_SMS(1) |	/* Source: Master 2 */
                                        DMA_CTL_DMS(0) |	/* Dest: Master 1 */
                                        DMA_CTL_SRC_MSIZE(bl) |
                                        DMA_CTL_DST_MSIZE(bl) |
                                        DMA_CTL_SINC_NOCHANGE |
                                        DMA_CTL_SRC_TRWID(2) |
                                        DMA_CTL_DST_TRWID(2) |
                                        DMA_CTL_INT_EN |
                                        DMA_CTL_LLP_SRCEN |
                                        DMA_CTL_LLP_DSTEN);
				} else if (hsdevp->hsdev->dma_channel == 1) {/* DMA channel 1 */
					lli[idx].ctl.low = cpu_to_le32(
                                        DMA_CTL_TTFC(DMA_CTL_TTFC_P2M_DMAC) |
                                        DMA_CTL_SMS(2) |	/* Source: Master 3 */
                                        DMA_CTL_DMS(0) |	/* Dest: Master 1 */
                                        DMA_CTL_SRC_MSIZE(bl) |
                                        DMA_CTL_DST_MSIZE(bl) |
                                        DMA_CTL_SINC_NOCHANGE |
                                        DMA_CTL_SRC_TRWID(2) |
                                        DMA_CTL_DST_TRWID(2) |
                                        DMA_CTL_INT_EN |
                                        DMA_CTL_LLP_SRCEN |
                                        DMA_CTL_LLP_DSTEN);
				}
                        } else {        /* DMA_TO_DEVICE */
                                lli[idx].sar = cpu_to_le32(addr);
                                lli[idx].dar = cpu_to_le32((u32)dmadr_addr);
				if (hsdevp->hsdev->dma_channel == 0) {/* DMA channel 0 */
	                                lli[idx].ctl.low = cpu_to_le32(
                                        DMA_CTL_TTFC(DMA_CTL_TTFC_M2P_PER) |
                                        DMA_CTL_SMS(0) |
                                        DMA_CTL_DMS(1) |
                                        DMA_CTL_SRC_MSIZE(bl) |
                                        DMA_CTL_DST_MSIZE(bl) |
                                        DMA_CTL_DINC_NOCHANGE |
                                        DMA_CTL_SRC_TRWID(2) |
                                        DMA_CTL_DST_TRWID(2) |
                                        DMA_CTL_INT_EN |
                                        DMA_CTL_LLP_SRCEN |
                                        DMA_CTL_LLP_DSTEN);
				} else if (hsdevp->hsdev->dma_channel == 1) {/* DMA channel 1 */
	                                lli[idx].ctl.low = cpu_to_le32(
                                        DMA_CTL_TTFC(DMA_CTL_TTFC_M2P_PER) |
                                        DMA_CTL_SMS(0) |
                                        DMA_CTL_DMS(2) |
                                        DMA_CTL_SRC_MSIZE(bl) |
                                        DMA_CTL_DST_MSIZE(bl) |
                                        DMA_CTL_DINC_NOCHANGE |
                                        DMA_CTL_SRC_TRWID(2) |
                                        DMA_CTL_DST_TRWID(2) |
                                        DMA_CTL_INT_EN |
                                        DMA_CTL_LLP_SRCEN |
                                        DMA_CTL_LLP_DSTEN);
				}
                        }
#else
			if (dir == DMA_FROM_DEVICE) {
				lli[idx].dar = cpu_to_le32(addr);
				lli[idx].sar = cpu_to_le32((u32)dmadr_addr);

				lli[idx].ctl.low = cpu_to_le32(
					DMA_CTL_TTFC(DMA_CTL_TTFC_P2M_DMAC) |
					DMA_CTL_SMS(0) |
					DMA_CTL_DMS(1) |
					DMA_CTL_SRC_MSIZE(bl) |
					DMA_CTL_DST_MSIZE(bl) |
					DMA_CTL_SINC_NOCHANGE |
					DMA_CTL_SRC_TRWID(2) |
					DMA_CTL_DST_TRWID(2) |
					DMA_CTL_INT_EN |
					DMA_CTL_LLP_SRCEN |
					DMA_CTL_LLP_DSTEN);
			} else {	/* DMA_TO_DEVICE */
				lli[idx].sar = cpu_to_le32(addr);
				lli[idx].dar = cpu_to_le32((u32)dmadr_addr);

				lli[idx].ctl.low = cpu_to_le32(
					DMA_CTL_TTFC(DMA_CTL_TTFC_M2P_PER) |
					DMA_CTL_SMS(1) |
					DMA_CTL_DMS(0) |
					DMA_CTL_SRC_MSIZE(bl) |
					DMA_CTL_DST_MSIZE(bl) |
					DMA_CTL_DINC_NOCHANGE |
					DMA_CTL_SRC_TRWID(2) |
					DMA_CTL_DST_TRWID(2) |
					DMA_CTL_INT_EN |
					DMA_CTL_LLP_SRCEN |
					DMA_CTL_LLP_DSTEN);
			}
#endif
			dev_dbg(dwc_dev, "%s setting ctl.high len: 0x%08x val: "
					"0x%08x\n", __func__, len,
					DMA_CTL_BLK_TS(len / 4));

			/* Program the LLI CTL high register */
			dma_ts = DMA_CTL_BLK_TS(len / 4);
			lli[idx].ctl.high = cpu_to_le32(dma_ts);

			/*
			 *Program the next pointer.  The next pointer must be
			 * the physical address, not the virtual address.
			 */
			next_llp = (dma_lli + ((idx + 1) * sizeof(struct lli)));

			/* The last 2 bits encode the list master select. */
#if defined(CONFIG_APM82181)
			next_llp = DMA_LLP_LMS(next_llp, DMA_LLP_AHBMASTER1);
#else
			next_llp = DMA_LLP_LMS(next_llp, DMA_LLP_AHBMASTER2);
#endif

			lli[idx].llp = cpu_to_le32(next_llp);
#if defined(VERBOSE_DEBUG)
                        dev_notice(qc->ap->dev, "%s: index %d\n", __func__, idx);
                        dev_notice(qc->ap->dev, "%s setting ctl.high with val: 0x%08x\n", __func__, lli[idx].ctl.high);
                        dev_notice(qc->ap->dev, "%s setting ctl.low with val: 0x%08x\n", __func__, lli[idx].ctl.low);
                        dev_notice(qc->ap->dev, "%s setting lli.dar with val: 0x%08x\n", __func__, lli[idx].dar);
                        dev_notice(qc->ap->dev, "%s setting lli.sar with val: 0x%08x\n", __func__, lli[idx].sar);
                        dev_notice(qc->ap->dev, "%s setting next_llp with val: 0x%08x\n", __func__, lli[idx].llp);
#endif

			idx++;
			sg_len -= len;
			addr += len;
		}
	}

	/*
	 * The last next ptr has to be zero and the last control low register
	 * has to have LLP_SRC_EN and LLP_DST_EN (linked list pointer source
	 * and destination enable) set back to 0 (disabled.)  This is what tells
	 * the core that this is the last item in the linked list.
	 */
	if (likely(idx)) {
		lli[idx-1].llp = 0x00000000;
		lli[idx-1].ctl.low &= DMA_CTL_LLP_DISABLE_LE32;

		/* Flush cache to memory */
		dma_cache_sync(NULL, lli, (sizeof(struct lli) * idx),
			       DMA_BIDIRECTIONAL);
	}

#if defined(VERBOSE_DEBUG)
        dev_notice(qc->ap->dev, "%s: Final index %d\n", __func__, idx-1);
        dev_notice(qc->ap->dev, "%s setting ctl.high with val: 0x%08x\n", __func__, lli[idx-1].ctl.high);
        dev_notice(qc->ap->dev, "%s setting ctl.low with val: 0x%08x\n", __func__, lli[idx-1].ctl.low);
        dev_notice(qc->ap->dev, "%s setting lli.dar with val: 0x%08x\n", __func__, lli[idx-1].dar);
        dev_notice(qc->ap->dev, "%s setting lli.sar with val: 0x%08x\n", __func__, lli[idx-1].sar);
        dev_notice(qc->ap->dev, "%s setting next_llp with val: 0x%08x\n", __func__, lli[idx-1].llp);
#endif

	return idx;
}

/*
 * Function: dma_dwc_xfer_start
 * arguments: Channel number
 * Return : None
 * Enables the DMA channel
 */
static void dma_dwc_xfer_start(int dma_ch)
{
	/* Enable the DMA channel */
	out_le32(&(sata_dma_regs->dma_chan_en.low),
		 in_le32(&(sata_dma_regs->dma_chan_en.low)) |
		 DMA_ENABLE_CHAN(dma_ch));
#if defined(VERBOSE_DEBUG)
	printk("%s: setting sata_dma_regs->dma_chan_en.low with val: 0x%08x\n",
                __func__, in_le32(&(sata_dma_regs->dma_chan_en.low)));
#endif
}

static int dma_dwc_channel_enabled(int ch)
{
	if (in_le32(&(sata_dma_regs->dma_chan_en.low)) & DMA_CHANNEL(ch))
		return 1;

	return 0;
}

static void dma_dwc_terminate_dma(struct ata_port *ap, int dma_ch)
{
	int enabled = dma_dwc_channel_enabled(dma_ch);

	dev_info(ap->dev, "%s terminate DMA on channel=%d enabled=%d\n",
		 __func__, dma_ch, enabled);

	if (enabled)  {
		out_le32(&(sata_dma_regs->dma_chan_en.low),
			 in_le32(&(sata_dma_regs->dma_chan_en.low)) | DMA_DISABLE_CHAN(dma_ch));

		do {
			enabled = dma_dwc_channel_enabled(dma_ch);
		} while (enabled);
	}
}

static int dma_dwc_xfer_setup(struct ata_queued_cmd *qc,
			      struct lli *lli, dma_addr_t dma_lli,
			      void __iomem *addr)
{
	int dma_ch;
	int num_lli;

	/* Acquire DMA channel */
	dma_ch = dma_request_channel(qc->ap);
	if (unlikely(dma_ch == -1)) {
		dev_err(qc->ap->dev, "%s: dma channel unavailable\n", __func__);
		return -EAGAIN;
	}
#if defined(VERBOSE_DEBUG)
	dev_notice(qc->ap->dev, "%s: Got channel %d\n", __func__, dma_ch);
#endif
	/* Convert SG list to linked list of items (LLIs) for AHB DMA */
	num_lli = map_sg_to_lli(qc, lli, dma_lli, addr);

	dev_dbg(qc->ap->dev, "%s sg: 0x%p, count: %d lli: %p dma_lli: 0x%0xlx addr:"
		" %p lli count: %d\n", __func__, qc->sg, qc->n_elem, lli,
		(u32)dma_lli, addr, num_lli);

	/* Clear channel interrupts */
	clear_chan_interrupts(dma_ch);
	
	/* Program the CFG register. */
#if defined(CONFIG_APM82181)
	if (dma_ch == 0) {
		/* Buffer mode enabled, FIFO_MODE  = 0 */
		out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.high), 0x000000d);
		/* Channel 0 bit [7:5]*/
		out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.low), 0x00000000); 
	} else if (dma_ch == 1) {
		/* Buffer mode enabled, FIFO_MODE  = 0 */
		out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.high), 0x0000088d); 
		/* Channel 1 - bit [7:5] */
		out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.low), 0x00000020); 
	}
#else
	out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.high),
		 DMA_CFG_PROTCTL | DMA_CFG_FCMOD_REQ);
	out_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.low), 0);
#endif
	/* Program the address of the linked list */
#if defined(CONFIG_APM82181)
	out_le32(&(sata_dma_regs->chan_regs[dma_ch].llp.low),
                 DMA_LLP_LMS(dma_lli, DMA_LLP_AHBMASTER1));
#else
	out_le32(&(sata_dma_regs->chan_regs[dma_ch].llp.low),
		 DMA_LLP_LMS(dma_lli, DMA_LLP_AHBMASTER2));
#endif

	/* Program the CTL register with src enable / dst enable */
	//out_le32(&(sata_dma_regs->chan_regs[dma_ch].ctl.low),
	//	 DMA_CTL_LLP_SRCEN | DMA_CTL_LLP_DSTEN);
	out_le32(&(sata_dma_regs->chan_regs[dma_ch].ctl.low), 0x18000000);

#if defined(VERBOSE_DEBUG)
	dev_notice(qc->ap->dev, "%s DMA channel %d is ready\n", __func__, dma_ch);
        dev_notice(qc->ap->dev, "%s setting cfg.high of channel %d with val: 0x%08x\n", __func__, dma_ch, in_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.high)));
        dev_notice(qc->ap->dev, "%s setting cfg.low of channel %d with val: 0x%08x\n", __func__, dma_ch, in_le32(&(sata_dma_regs->chan_regs[dma_ch].cfg.low)));
        dev_notice(qc->ap->dev, "%s setting llp.low of channel %d with val: 0x%08x\n", __func__, dma_ch, in_le32(&(sata_dma_regs->chan_regs[dma_ch].llp.low)));
        dev_notice(qc->ap->dev, "%s setting ctl.low of channel %d with val: 0x%08x\n", __func__, dma_ch, in_le32(&(sata_dma_regs->chan_regs[dma_ch].ctl.low)));
#endif

	return dma_ch;
}

/*
 * Function: dma_dwc_exit
 * arguments: None
 * returns status
 * This function exits the SATA DMA driver
 */
static void dma_dwc_exit(struct sata_dwc_device *hsdev)
{
	dev_dbg(hsdev->dev, "%s:\n", __func__);
	
	if (sata_dma_regs)
		iounmap(sata_dma_regs);

	if (hsdev->irq_dma)
		free_irq(hsdev->irq_dma, hsdev);
}

/*
 * Function: dma_dwc_init
 * arguments: hsdev
 * returns status
 * This function initializes the SATA DMA driver
 */
static int dma_dwc_init(struct sata_dwc_device *hsdev, int irq)
{
	int err;

	err = dma_request_interrupts(hsdev, irq);
	if (err) {
		dev_err(hsdev->dev, "%s: dma_request_interrupts returns %d\n",
			__func__, err);
		goto error_out;
	}

	/* Enabe DMA */
	out_le32(&(sata_dma_regs->dma_cfg.low), DMA_EN);

	dev_notice(hsdev->dev, "DMA initialized\n");
	dev_notice(hsdev->dev, "DMA CFG = 0x%08x\n", in_le32(&(sata_dma_regs->dma_cfg.low)));
	dev_dbg(hsdev->dev, "SATA DMA registers=0x%p\n", sata_dma_regs);

	return 0;

error_out:
	dma_dwc_exit(hsdev);

	return err;
}

static int sata_dwc_scr_read(struct ata_link *link, unsigned int scr, u32 *val)
{
	if (unlikely(scr > SCR_NOTIFICATION)) {
		dev_err(link->ap->dev, "%s: Incorrect SCR offset 0x%02x\n",
				__func__, scr);
		return -EINVAL;
	}

	*val = in_le32((void *)link->ap->ioaddr.scr_addr + (scr * 4));
	dev_dbg(link->ap->dev, "%s: id=%d reg=%d val=val=0x%08x\n",
		__func__, link->ap->print_id, scr, *val);

	return 0;
}

static int sata_dwc_scr_write(struct ata_link *link, unsigned int scr, u32 val)
{
	dev_dbg(link->ap->dev, "%s: id=%d reg=%d val=val=0x%08x\n",
		__func__, link->ap->print_id, scr, val);
	if (unlikely(scr > SCR_NOTIFICATION)) {
		dev_err(link->ap->dev, "%s: Incorrect SCR offset 0x%02x\n",
				__func__, scr);
		return -EINVAL;
	}
	out_le32((void *)link->ap->ioaddr.scr_addr + (scr * 4), val);

	return 0;
}

static inline u32 core_scr_read ( struct ata_port *ap, unsigned int scr)
{
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	return in_le32((void __iomem *)hsdev->scr_base + (scr * 4));
}


static inline void core_scr_write ( struct ata_port *ap, unsigned int scr, u32 val)
{
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	out_le32((void __iomem *)hsdev->scr_base + (scr * 4), val);
}

static inline void clear_serror(struct ata_port *ap)
{
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	out_le32( (void __iomem *)hsdev->scr_base + 4,
		in_le32((void __iomem *)hsdev->scr_base + 4));
}

static inline void clear_intpr(struct sata_dwc_device *hsdev)
{
	out_le32(&hsdev->sata_dwc_regs->intpr,
		 in_le32(&hsdev->sata_dwc_regs->intpr));
}

static inline void clear_interrupt_bit(struct sata_dwc_device *hsdev, u32 bit)
{
	out_le32(&hsdev->sata_dwc_regs->intpr,
		 in_le32(&hsdev->sata_dwc_regs->intpr));
}

static inline void disable_err_irq(struct sata_dwc_device *hsdev)
{
	out_le32(&hsdev->sata_dwc_regs->intmr,
		 in_le32(&hsdev->sata_dwc_regs->intmr) & ~SATA_DWC_INTMR_ERRM);
	out_le32(&hsdev->sata_dwc_regs->errmr, ~SATA_DWC_SERROR_ERR_BITS);
}

static inline void enable_err_irq(struct sata_dwc_device *hsdev)
{
	out_le32(&hsdev->sata_dwc_regs->intmr,
		 in_le32(&hsdev->sata_dwc_regs->intmr) | SATA_DWC_INTMR_ERRM);
	out_le32(&hsdev->sata_dwc_regs->errmr, SATA_DWC_SERROR_ERR_BITS);
}

static inline u32 qcmd_tag_to_mask(u8 tag)
{
	return 0x00000001 << (tag & 0x1f);
}

/* See ahci.c */
static void sata_dwc_error_intr(struct ata_port *ap,
				struct sata_dwc_device *hsdev, uint intpr)
{
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	struct ata_eh_info *ehi = &ap->link.eh_info;
	unsigned int err_mask = 0, action = 0;
	struct ata_queued_cmd *qc;
	u32 serror;
	u8 status, tag;
	volatile u32 err_reg;
#if defined(VERBOSE_DEBUG)
	int chan = hsdev->dma_channel;
#endif
	ata_ehi_clear_desc(ehi);

	serror = core_scr_read(ap, SCR_ERROR);
	status = ap->ops->sff_check_status(ap);

	err_reg = in_le32(&(sata_dma_regs->interrupt_status.error.low));
	tag = ap->link.active_tag;

	dev_err(ap->dev, "%s SCR_ERROR=0x%08x intpr=0x%08x status=0x%08x "
		"dma_intp=%d pending=%d issued=%d dma_err_status=0x%08x\n",
		__func__, serror, intpr, status, hsdevp->dma_interrupt_count,
		hsdevp->dma_pending[tag], hsdevp->cmd_issued[tag], err_reg);
#if defined(VERBOSE_DEBUG)
        printk("%s reading cfg.high of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].cfg.high)));
        printk("%s reading cfg.low of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].cfg.low)));
        printk("%s reading llp.low of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].llp.low)));
        printk("%s reading ctl.low of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].ctl.low)));
        printk("%s reading sar.low of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].sar.low)));
        printk("%s reading sar.high of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].sar.high)));
        printk("%s reading dar.low of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].dar.low)));
        printk("%s reading dar.high of channel %d with val: 0x%08x\n", __func__, chan, in_le32(&(sata_dma_regs->chan_regs[chan].dar.high)));
#endif
	/* Clear error register and interrupt bit */
	clear_serror(ap);
	clear_interrupt_bit(hsdev, SATA_DWC_INTPR_ERR);

	/* This is the only error happening now. */
	/* TODO check for exact error */
	err_mask |= AC_ERR_HOST_BUS;
	action |= ATA_EH_RESET;

	/* Pass this on to EH */
	ehi->serror |= serror;
	ehi->action |= action;

	qc = ata_qc_from_tag(ap, tag);
	if (qc)
		qc->err_mask |= err_mask;
	else
		ehi->err_mask |= err_mask;

	ata_port_abort(ap);

	/*
	  if (irq_stat & PORT_IRQ_FREEZE)
	  ata_port_freeze(ap);
	  else
	  ata_port_abort(ap);
	*/
}

/*
 * Function : sata_dwc_isr
 * arguments : irq, void *dev_instance, struct pt_regs *regs
 * Return value : irqreturn_t - status of IRQ
 * This Interrupt handler called via port ops registered function.
 * .irq_handler = sata_dwc_isr
 */
static irqreturn_t sata_dwc_isr(int irq, void *dev_instance)
{
	struct ata_host *host = (struct ata_host *)dev_instance;
	struct sata_dwc_device *hsdev = HSDEV_FROM_HOST(host);
	struct ata_port *ap;
	struct ata_queued_cmd *qc;
	unsigned long flags;
	u8 status, tag;
	int handled, num_processed, port = 0;
	uint intpr, sactive, sactive2, tag_mask;
	struct sata_dwc_device_port *hsdevp;


	spin_lock_irqsave(&host->lock, flags);

	/* Read the interrupt register */
	intpr = in_le32(&hsdev->sata_dwc_regs->intpr);

	ap = host->ports[port];
	hsdevp = HSDEVP_FROM_AP(ap);
#if defined(VERBOSE_DEBUG)
	dev_notice(ap->dev, "%s: INTERRUPT in HSDEV with DMA channel %d\n", __func__, hsdevp->hsdev->dma_channel);
#endif
	dev_dbg(ap->dev, "%s intpr=0x%08x active_tag=%d\n", __func__, intpr,
		ap->link.active_tag);

	/* Check for error interrupt */
	if (intpr & SATA_DWC_INTPR_ERR) {
		sata_dwc_error_intr(ap, hsdev, intpr);
		handled = 1;
		goto done_irqrestore;
	}

	/* Check for DMA SETUP FIS (FP DMA) interrupt */
	if (intpr & SATA_DWC_INTPR_NEWFP) {
		clear_interrupt_bit(hsdev, SATA_DWC_INTPR_NEWFP);

		tag = (u8)(in_le32(&hsdev->sata_dwc_regs->fptagr));
		dev_dbg(ap->dev, "%s: NEWFP tag=%d\n", __func__, tag);
		if (hsdevp->cmd_issued[tag] != SATA_DWC_CMD_ISSUED_PENDING)
			dev_warn(ap->dev, "CMD tag=%d not pending?\n", tag);

		hsdevp->sata_dwc_sactive_issued |= qcmd_tag_to_mask(tag);

		qc = ata_qc_from_tag(ap, tag);
		/*
		 * Start FP DMA for NCQ command.  At this point the tag is the
		 * active tag.  It is the tag that matches the command about to
		 * be completed.
		 */
		qc->ap->link.active_tag = tag;
		sata_dwc_bmdma_start_by_tag(qc, tag);

		handled = 1;
		goto done_irqrestore;
	}

	sactive = core_scr_read(ap, SCR_ACTIVE);
	tag_mask = (hsdevp->sata_dwc_sactive_issued | sactive) ^ sactive;

	/* If no sactive issued and tag_mask is zero then this is not NCQ */
	if (hsdevp->sata_dwc_sactive_issued == 0 && tag_mask == 0) {
#if 0
		if (ap->link.active_tag == ATA_TAG_POISON)
                        tag = 0;
                else
#endif
		tag = ap->link.active_tag;
		qc = ata_qc_from_tag(ap, tag);

		/* DEV interrupt w/ no active qc? */
		if (unlikely(!qc || (qc->tf.flags & ATA_TFLAG_POLLING))) {
			dev_err(ap->dev, "%s intr with no active qc qc=%p\n",
				__func__, qc);
			ata_sff_check_status(ap);
			handled = 1;
			goto done_irqrestore;
		}

		status = ap->ops->sff_check_status(ap);

		qc->ap->link.active_tag = tag;
		hsdevp->cmd_issued[tag] = SATA_DWC_CMD_ISSUED_NOT;

		if (status & ATA_ERR) {
			dev_dbg(ap->dev, "interrupt ATA_ERR (0x%x)\n", status);
			sata_dwc_qc_complete(ap, qc, 1);
			handled = 1;
			goto done_irqrestore;
		}

		dev_dbg(ap->dev, "%s non-NCQ cmd interrupt, protocol: %s\n",
			__func__, prot_2_txt(qc->tf.protocol));
drv_still_busy:
		if (ata_is_dma(qc->tf.protocol)) {
			int dma_flag = hsdevp->dma_pending[tag];
			/*
			 * Each DMA transaction produces 2 interrupts.  The DMAC
			 * transfer complete interrupt and the SATA controller
			 * operation done interrupt. The command should be
			 * completed only after both interrupts are seen.
			 */
			hsdevp->dma_interrupt_count++;
			if (unlikely(dma_flag == SATA_DWC_DMA_PENDING_NONE)) {
				dev_err(ap->dev, "%s: DMA not pending "
					"intpr=0x%08x status=0x%08x pend=%d\n",
					__func__, intpr, status, dma_flag);
			}

			if ((hsdevp->dma_interrupt_count % 2) == 0)
				sata_dwc_dma_xfer_complete(ap, 1);
		} else if (ata_is_pio(qc->tf.protocol)) {
			ata_sff_hsm_move(ap, qc, status, 0);
			handled = 1;
			goto done_irqrestore;
		} else {
			if (unlikely(sata_dwc_qc_complete(ap, qc, 1)))
				goto drv_still_busy;
		}

		handled = 1;
		goto done_irqrestore;
	}

	/*
	 * This is a NCQ command.  At this point we need to figure out for which
	 * tags we have gotten a completion interrupt.  One interrupt may serve
	 * as completion for more than one operation when commands are queued
	 * (NCQ).  We need to process each completed command.
	 */

process_cmd:  /* process completed commands */
	sactive = core_scr_read(ap, SCR_ACTIVE);
	tag_mask = (hsdevp->sata_dwc_sactive_issued | sactive) ^ sactive;

	if (sactive != 0 || hsdevp->sata_dwc_sactive_issued > 1 || tag_mask > 1) {
		dev_dbg(ap->dev, "%s NCQ: sactive=0x%08x  sactive_issued=0x%08x"
			" tag_mask=0x%08x\n", __func__, sactive,
			hsdevp->sata_dwc_sactive_issued, tag_mask);
	}

	if (unlikely((tag_mask | hsdevp->sata_dwc_sactive_issued) != hsdevp->sata_dwc_sactive_issued)) {
		dev_warn(ap->dev, "Bad tag mask?  sactive=0x%08x "
			 "sata_dwc_sactive_issued=0x%08x  tag_mask=0x%08x\n",
			 sactive, hsdevp->sata_dwc_sactive_issued, tag_mask);
	}

	/* read just to clear ... not bad if currently still busy */
	status = ap->ops->sff_check_status(ap);
	dev_dbg(ap->dev, "%s ATA status register=0x%x\n", __func__, status);

	tag = 0;
	num_processed = 0;
	while (tag_mask) {
		num_processed++;
		while (!(tag_mask & 0x00000001)) {
			tag++;
			tag_mask <<= 1;
		}
		tag_mask &= (~0x00000001);
		qc = ata_qc_from_tag(ap, tag);

		/* To be picked up by completion functions */
		qc->ap->link.active_tag = tag;
		hsdevp->cmd_issued[tag] = SATA_DWC_CMD_ISSUED_NOT;

		/* Let libata/scsi layers handle error */
		if (unlikely(status & ATA_ERR)) {
			dev_dbg(ap->dev, "%s ATA_ERR (0x%x)\n",
					 __func__, status);

			sata_dwc_qc_complete(ap, qc, 1);
			handled = 1;
			goto done_irqrestore;
		}

		/* Process completed command */
		dev_dbg(ap->dev, "%s NCQ command, protocol: %s\n", __func__,
			prot_2_txt(qc->tf.protocol));
		if (ata_is_dma(qc->tf.protocol)) {
			hsdevp->dma_interrupt_count++;
			if (hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_NONE)
				dev_warn(ap->dev,
					"%s: DMA not pending?\n", __func__);
			if ((hsdevp->dma_interrupt_count % 2) == 0)
				sata_dwc_dma_xfer_complete(ap, 1);
		} else {
			if (unlikely(sata_dwc_qc_complete(ap, qc, 1)))
				goto still_busy;
		}
		continue;

still_busy:
		ap->stats.idle_irq++;
		dev_warn(ap->dev, "STILL BUSY IRQ ata%d: irq trap\n",
				ap->print_id);
	} /* while tag_mask */

	/*
	 * Check to see if any commands completed while we were processing our
	 * initial set of completed commands (reading of status clears
	 * interrupts, so we might miss a completed command interrupt if one
	 * came in while we were processing:
	 * we read status as part of processing a completed command).
	 */
	sactive2 = core_scr_read(ap, SCR_ACTIVE);
	if (sactive2 != sactive) {
		dev_dbg(ap->dev, "More finished - sactive=0x%x sactive2=0x%x\n",
			sactive, sactive2);
		goto process_cmd;
	}
	handled = 1;

done_irqrestore:
	spin_unlock_irqrestore(&host->lock, flags);
	return IRQ_RETVAL(handled);
}

static void sata_dwc_clear_dmacr(struct sata_dwc_device_port *hsdevp, u8 tag)
{
	struct sata_dwc_device *hsdev = HSDEV_FROM_HSDEVP(hsdevp);

	if (hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_RX) {
		out_le32(&(hsdev->sata_dwc_regs->dmacr),
			 SATA_DWC_DMACR_RX_CLEAR(
				 in_le32(&(hsdev->sata_dwc_regs->dmacr))));
	} else if (hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_TX) {
		out_le32(&(hsdev->sata_dwc_regs->dmacr),
			 SATA_DWC_DMACR_TX_CLEAR(
				 in_le32(&(hsdev->sata_dwc_regs->dmacr))));
	} else {
		/*
		 * This should not happen, it indicates the driver is out of
		 * sync.  If it does happen, clear dmacr anyway.
		 */
		dev_err(hsdev->dev, "%s DMA protocol RX and TX DMA not pending "
			"tag=0x%02x pending=%d dmacr: 0x%08x\n",
			__func__, tag, hsdevp->dma_pending[tag],
			in_le32(&(hsdev->sata_dwc_regs->dmacr)));
		out_le32(&(hsdev->sata_dwc_regs->dmacr),
				SATA_DWC_DMACR_TXRXCH_CLEAR);
	}
}

static void sata_dwc_dma_xfer_complete(struct ata_port *ap, u32 check_status)
{
	struct ata_queued_cmd *qc;
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	u8 tag = 0;

	tag = ap->link.active_tag;
	qc = ata_qc_from_tag(ap, tag);

#ifdef DEBUG_NCQ
	if (tag > 0) {
		dev_info(ap->dev, "%s tag=%u cmd=0x%02x dma dir=%s proto=%s "
			 "dmacr=0x%08x\n", __func__, qc->tag, qc->tf.command,
			 dir_2_txt(qc->dma_dir), prot_2_txt(qc->tf.protocol),
			 in_le32(&(hsdev->sata_dwc_regs->dmacr)));
	}
#endif

	if (ata_is_dma(qc->tf.protocol)) {
		if (unlikely(hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_NONE)) {
			dev_err(ap->dev, "%s DMA protocol RX and TX DMA not "
				"pending dmacr: 0x%08x\n", __func__,
				in_le32(&(hsdev->sata_dwc_regs->dmacr)));
		}

		hsdevp->dma_pending[tag] = SATA_DWC_DMA_PENDING_NONE;
		sata_dwc_qc_complete(ap, qc, check_status);
		ap->link.active_tag = ATA_TAG_POISON;
	} else {
		sata_dwc_qc_complete(ap, qc, check_status);
	}
}

static int sata_dwc_qc_complete(struct ata_port *ap, struct ata_queued_cmd *qc,
				u32 check_status)
{
	u8 status = 0;
	int i = 0;
	u32 mask = 0x0;
	u8 tag = qc->tag;
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	u32 serror;

	dev_dbg(ap->dev, "%s checkstatus? %x\n", __func__, check_status);

	if (unlikely(hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_TX))
		dev_err(ap->dev, "TX DMA PENDINGING\n");
	else if (unlikely(hsdevp->dma_pending[tag] == SATA_DWC_DMA_PENDING_RX))
		dev_err(ap->dev, "RX DMA PENDINGING\n");

	if (check_status) {
		i = 0;
		do {
			/* check main status, clearing INTRQ */
			status = ap->ops->sff_check_status(ap);
			if (status & ATA_BUSY) {
				dev_dbg(ap->dev, "STATUS BUSY (0x%02x) [%d]\n",
						status, i);
			}
			if (++i > 10)
				break;
		} while (status & ATA_BUSY);

		status = ap->ops->sff_check_status(ap);
		if (unlikely(status & ATA_BUSY))
			dev_err(ap->dev, "QC complete cmd=0x%02x STATUS BUSY "
				"(0x%02x) [%d]\n", qc->tf.command, status, i);
		serror = core_scr_read(ap, SCR_ERROR);
		if (unlikely(serror & SATA_DWC_SERROR_ERR_BITS))
			dev_err(ap->dev, "****** SERROR=0x%08x ******\n",
				serror);
	}
	dev_dbg(ap->dev, "QC complete cmd=0x%02x status=0x%02x ata%u: "
		"protocol=%d\n", qc->tf.command, status, ap->print_id,
		qc->tf.protocol);

	/* clear active bit */
	mask = (~(qcmd_tag_to_mask(tag)));
	hsdevp->sata_dwc_sactive_queued = hsdevp->sata_dwc_sactive_queued & mask;
	hsdevp->sata_dwc_sactive_issued = hsdevp->sata_dwc_sactive_issued & mask;

	/* Complete taskfile transaction (does not read SCR registers) */
	ata_qc_complete(qc);

	return 0;
}

static void sata_dwc_enable_interrupts(struct sata_dwc_device *hsdev)
{
	/* Enable selective interrupts by setting the interrupt mask register */
	out_le32(&hsdev->sata_dwc_regs->intmr,
		 SATA_DWC_INTMR_ERRM |
		 SATA_DWC_INTMR_NEWFPM |
		 SATA_DWC_INTMR_PMABRTM |
		 SATA_DWC_INTMR_DMATM);

	/*
	 * Unmask the error bits that should trigger an error interrupt by
	 * setting the error mask register.
	 */
	out_le32(&hsdev->sata_dwc_regs->errmr, SATA_DWC_SERROR_ERR_BITS);

	dev_dbg(hsdev->dev, "%s: INTMR = 0x%08x, ERRMR = 0x%08x\n", __func__,
		in_le32(&hsdev->sata_dwc_regs->intmr),
		in_le32(&hsdev->sata_dwc_regs->errmr));
}

static void sata_dwc_setup_port(struct ata_ioports *port, unsigned long base)
{
	port->cmd_addr = (void *)base + 0x00;
	port->data_addr = (void *)base + 0x00;

	port->error_addr = (void *)base + 0x04;
	port->feature_addr = (void *)base + 0x04;

	port->nsect_addr = (void *)base + 0x08;

	port->lbal_addr = (void *)base + 0x0c;
	port->lbam_addr = (void *)base + 0x10;
	port->lbah_addr = (void *)base + 0x14;

	port->device_addr = (void *)base + 0x18;
	port->command_addr = (void *)base + 0x1c;
	port->status_addr = (void *)base + 0x1c;

	port->altstatus_addr = (void *)base + 0x20;
	port->ctl_addr = (void *)base + 0x20;
}

/*
 * Function : sata_dwc_port_start
 * arguments : struct ata_ioports *port
 * Return value : returns 0 if success, error code otherwise
 * This function allocates the scatter gather LLI table for AHB DMA
 */
static int sata_dwc_port_start(struct ata_port *ap)
{
	int err = 0;
	struct sata_dwc_device *hsdev;
	struct sata_dwc_device_port *hsdevp = NULL;
	struct device *pdev;
	u32 sstatus;
	int i;

	hsdev = HSDEV_FROM_AP(ap);

	dev_dbg(ap->dev, "%s: port_no=%d\n", __func__, ap->port_no);

	hsdev->host = ap->host;
	pdev = ap->host->dev;
	if (!pdev) {
		dev_err(ap->dev, "%s: no ap->host->dev\n", __func__);
		err = -ENODEV;
		goto cleanup_exit;
	}

	/* Allocate Port Struct */
	hsdevp = kmalloc(sizeof(*hsdevp), GFP_KERNEL);
	if (!hsdevp) {
		dev_err(ap->dev, "%s: kmalloc failed for hsdevp\n", __func__);
		err = -ENOMEM;
		goto cleanup_exit;
	}
	memset(hsdevp, 0, sizeof(*hsdevp));
	hsdevp->hsdev = hsdev;

	for (i = 0; i < SATA_DWC_QCMD_MAX; i++)
		hsdevp->cmd_issued[i] = SATA_DWC_CMD_ISSUED_NOT;

	ap->prd = 0;	/* set these so libata doesn't use them */
	ap->prd_dma = 0;

	/*
	 * DMA - Assign scatter gather LLI table. We can't use the libata
	 * version since it's PRD is IDE PCI specific.
	 */
	for (i = 0; i < SATA_DWC_QCMD_MAX; i++) {
		hsdevp->llit[i] = dma_alloc_coherent(pdev,
						     SATA_DWC_DMAC_LLI_TBL_SZ,
						     &(hsdevp->llit_dma[i]),
						     GFP_ATOMIC);
		if (!hsdevp->llit[i]) {
			dev_err(ap->dev, "%s: dma_alloc_coherent failed size "
				"0x%x\n", __func__, SATA_DWC_DMAC_LLI_TBL_SZ);
			err = -ENOMEM;
			goto cleanup_exit;
		}
	}

	if (ap->port_no == 0)  {
		dev_dbg(ap->dev, "%s: clearing TXCHEN, RXCHEN in DMAC\n",
				__func__);

		out_le32(&hsdev->sata_dwc_regs->dmacr,
			 SATA_DWC_DMACR_TXRXCH_CLEAR);

		dev_dbg(ap->dev, "%s: setting burst size in DBTSR\n", __func__);
		out_le32(&hsdev->sata_dwc_regs->dbtsr,
			 (SATA_DWC_DBTSR_MWR(AHB_DMA_BRST_DFLT) |
			  SATA_DWC_DBTSR_MRD(AHB_DMA_BRST_DFLT)));
		 dev_notice(ap->dev, "%s: setting burst size in DBTSR: 0x%08x\n", 
			__func__, in_le32(&hsdev->sata_dwc_regs->dbtsr));
	}

	/* Clear any error bits before libata starts issuing commands */
	clear_serror(ap);

	ap->private_data = hsdevp;

	/* Are we in Gen I or II */
	sstatus = core_scr_read(ap, SCR_STATUS);
	switch (SATA_DWC_SCR0_SPD_GET(sstatus)) {
	case 0x0:
		dev_info(ap->dev, "**** No neg speed (nothing attached?) \n");
		break;
	case 0x1:
		dev_info(ap->dev, "**** GEN I speed rate negotiated \n");
		break;
	case 0x2:
		dev_info(ap->dev, "**** GEN II speed rate negotiated \n");
		break;
	}

cleanup_exit:
	if (err) {
		kfree(hsdevp);
		sata_dwc_port_stop(ap);
		dev_dbg(ap->dev, "%s: fail\n", __func__);
	} else {
		dev_dbg(ap->dev, "%s: done\n", __func__);
	}

	return err;
}

static void sata_dwc_port_stop(struct ata_port *ap)
{
	int i;
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);

	dev_dbg(ap->dev, "%s: ap->id = %d\n", __func__, ap->print_id);

	if (hsdevp && hsdev) {
		/* deallocate LLI table */
		for (i = 0; i < SATA_DWC_QCMD_MAX; i++) {
			dma_free_coherent(ap->host->dev,
					  SATA_DWC_DMAC_LLI_TBL_SZ,
					  hsdevp->llit[i], hsdevp->llit_dma[i]);
		}

		kfree(hsdevp);
	}
	ap->private_data = NULL;
}

/*
 * Function : sata_dwc_exec_command_by_tag
 * arguments : ata_port *ap, ata_taskfile *tf, u8 tag, u32 cmd_issued
 * Return value : None
 * This function keeps track of individual command tag ids and calls
 * ata_exec_command in libata
 */
static void sata_dwc_exec_command_by_tag(struct ata_port *ap,
					 struct ata_taskfile *tf,
					 u8 tag, u32 cmd_issued)
{
	unsigned long flags;
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);

	dev_dbg(ap->dev, "%s cmd(0x%02x): %s tag=%d\n", __func__, tf->command,
		ata_cmd_2_txt(tf), tag);

	spin_lock_irqsave(&ap->host->lock, flags);
	hsdevp->cmd_issued[tag] = cmd_issued;
	spin_unlock_irqrestore(&ap->host->lock, flags);

	/*
	 * Clear SError before executing a new command.
	 *
	 * TODO if we read a PM's registers now, we will throw away the task
	 * file values loaded into the shadow registers for this command.
	 *
	 * sata_dwc_scr_write and read can not be used here. Clearing the PM
	 * managed SError register for the disk needs to be done before the
	 * task file is loaded.
	 */
	clear_serror(ap);
	ata_sff_exec_command(ap, tf);
}

static void sata_dwc_bmdma_setup_by_tag(struct ata_queued_cmd *qc, u8 tag)
{
	sata_dwc_exec_command_by_tag(qc->ap, &qc->tf, tag,
				     SATA_DWC_CMD_ISSUED_PENDING);
}

static void sata_dwc_bmdma_setup(struct ata_queued_cmd *qc)
{
	u8 tag = qc->tag;

	if (ata_is_ncq(qc->tf.protocol)) {
		dev_dbg(qc->ap->dev, "%s: ap->link.sactive=0x%08x tag=%d\n",
			__func__, qc->ap->link.sactive, tag);
	} else {
		tag = 0;
	}

	sata_dwc_bmdma_setup_by_tag(qc, tag);
}

static void sata_dwc_bmdma_start_by_tag(struct ata_queued_cmd *qc, u8 tag)
{
	volatile int start_dma;
	u32 reg, dma_chan;
	struct sata_dwc_device *hsdev = HSDEV_FROM_QC(qc);
	struct ata_port *ap = qc->ap;
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	int dir = qc->dma_dir;
	dma_chan = hsdevp->dma_chan[tag];

	/* Used for ata_bmdma_start(qc) -- we are not BMDMA compatible */

	if (hsdevp->cmd_issued[tag] != SATA_DWC_CMD_ISSUED_NOT) {
		start_dma = 1;
		if (dir == DMA_TO_DEVICE)
			hsdevp->dma_pending[tag] = SATA_DWC_DMA_PENDING_TX;
		else
			hsdevp->dma_pending[tag] = SATA_DWC_DMA_PENDING_RX;
	} else {
		dev_err(ap->dev, "%s: Command not pending cmd_issued=%d "
			"(tag=%d) - DMA NOT started\n", __func__,
			hsdevp->cmd_issued[tag], tag);
		start_dma = 0;
	}

	dev_dbg(ap->dev, "%s qc=%p tag: %x cmd: 0x%02x dma_dir: %s "
			"start_dma? %x\n", __func__, qc, tag, qc->tf.command,
			dir_2_txt(qc->dma_dir), start_dma);
	sata_dwc_tf_dump(hsdev->dev, &(qc->tf));

	if (start_dma) {
		reg = core_scr_read(ap, SCR_ERROR);
		if (unlikely(reg & SATA_DWC_SERROR_ERR_BITS)) {
			dev_err(ap->dev, "%s: ****** SError=0x%08x ******\n",
				__func__, reg);
		}

		if (dir == DMA_TO_DEVICE)
			out_le32(&hsdev->sata_dwc_regs->dmacr,
					SATA_DWC_DMACR_TXCHEN);
		else
			out_le32(&hsdev->sata_dwc_regs->dmacr,
					SATA_DWC_DMACR_RXCHEN);
#if defined(VERBOSE_DEBUG)                
		dev_notice(ap->dev, "%s: setting DMACR: 0x%08x\n", __func__, in_le32(&hsdev->sata_dwc_regs->dmacr));
#endif
		/* Enable AHB DMA transfer on the specified channel */
		dma_dwc_xfer_start(dma_chan);
	}
}

static void sata_dwc_bmdma_start(struct ata_queued_cmd *qc)
{
	u8 tag = qc->tag;
	if (ata_is_ncq(qc->tf.protocol)) {
		dev_dbg(qc->ap->dev, "%s: ap->link.sactive=0x%08x tag=%d\n",
			__func__, qc->ap->link.sactive, tag);
	} else {
		tag = 0;
	}

	dev_dbg(qc->ap->dev, "%s\n", __func__);
	sata_dwc_bmdma_start_by_tag(qc, tag);
}

/*
 * Function : sata_dwc_qc_prep_by_tag
 * arguments : ata_queued_cmd *qc, u8 tag
 * Return value : None
 * qc_prep for a particular queued command based on tag
 */
static void sata_dwc_qc_prep_by_tag(struct ata_queued_cmd *qc, u8 tag)
{
	struct ata_port *ap = qc->ap;
	u32 dma_chan;
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	int dir;
	int err;

	dir = qc->dma_dir;

	if ((dir == DMA_NONE) || (qc->tf.protocol == ATA_PROT_PIO))
		return;

	dev_dbg(ap->dev, "%s: port=%d dma dir=%s n_elem=%d\n",
		__func__, ap->port_no, dir_2_txt(dir), qc->n_elem);

	dma_chan = dma_dwc_xfer_setup(qc, hsdevp->llit[tag],
			hsdevp->llit_dma[tag],
			(void *__iomem)(&hsdev->sata_dwc_regs->dmadr));
	if (unlikely(dma_chan < 0)) {
		dev_err(ap->dev, "%s: dma_dwc_xfer_setup returns err %d\n",
			__func__, err);
		return;
	}

	hsdevp->dma_chan[tag] = dma_chan;
}

static unsigned int sata_dwc_qc_issue(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;

#ifdef DEBUG_NCQ
	if (qc->tag > 0 || ap->link.sactive > 1) {
		dev_info(ap->dev, "%s ap id=%d cmd(0x%02x)=%s qc tag=%d prot=%s"
			" ap active_tag=0x%08x ap sactive=0x%08x\n",
			__func__, ap->print_id, qc->tf.command,
			ata_cmd_2_txt(&qc->tf), qc->tag,
			prot_2_txt(qc->tf.protocol), ap->link.active_tag,
			ap->link.sactive);
	}
#endif

	if (ata_is_ncq(qc->tf.protocol)) {
		ap->ops->sff_tf_load(ap, &qc->tf);
		sata_dwc_exec_command_by_tag(ap, &qc->tf, qc->tag,
					     SATA_DWC_CMD_ISSUED_PENDING);
	} else {
		ata_sff_qc_issue(qc);
	}

	return 0;
}

/*
 * Function : sata_dwc_eng_timeout
 * arguments : ata_port *ap
 * Return value : None
 * error handler for DMA time out
 * ata_eng_timeout(ap) -- this does bmdma stuff which can not be done by this
 * driver. SEE ALSO ata_qc_timeout(ap)
 */
static void sata_dwc_eng_timeout(struct ata_port *ap)
{
	struct ata_queued_cmd *qc;
	struct sata_dwc_device *hsdev = HSDEV_FROM_AP(ap);
	struct sata_dwc_device_port *hsdevp = HSDEVP_FROM_AP(ap);
	u8  tag;
	uint mask = 0x0;
	unsigned long flags;
	u32 serror, intpr, dma_ch;

	tag = ap->link.active_tag;
	dma_ch = hsdevp->dma_chan[tag];
	qc = ata_qc_from_tag(ap, tag);

	dev_err(ap->dev, "%s: id=%d active_tag=%d qc=%p dma_chan=%d\n",
		__func__, ap->print_id, tag, qc, dma_ch);

	if (unlikely(!qc)) {
		dev_err(ap->dev,
			"%s: timeout without queued command\n", __func__);
		return;
	}

	intpr = in_le32(&hsdev->sata_dwc_regs->intpr);
	serror = core_scr_read(ap, SCR_ERROR);

	dev_err(ap->dev, "intpr=0x%08x serror=0x%08x\n", intpr, serror);

	/* If there are no error bits set, can we just pass this on to eh? */
	if (!(serror & SATA_DWC_SERROR_ERR_BITS) &&
	    !(intpr & SATA_DWC_INTPR_ERR)) {

		spin_lock_irqsave(&ap->host->lock, flags);
		if (dma_dwc_channel_enabled(dma_ch))
			dma_dwc_terminate_dma(ap, dma_ch);

		hsdevp->dma_pending[tag] = SATA_DWC_DMA_PENDING_NONE;

		/* clear active bit */
		mask = (~(qcmd_tag_to_mask(tag)));
		hsdevp->sata_dwc_sactive_queued = hsdevp->sata_dwc_sactive_queued & mask;
		hsdevp->sata_dwc_sactive_issued = hsdevp->sata_dwc_sactive_issued & mask;

		spin_unlock_irqrestore(&ap->host->lock, flags);
	} else {
		/* This is wrong, what really needs to be done is a reset. */

		spin_lock_irqsave(ap->lock, flags);

		if (ata_is_dma(qc->tf.protocol)) {
			/* disable DMAC */
			dma_dwc_terminate_dma(ap, dma_ch);
		}

		spin_unlock_irqrestore(ap->lock, flags);
	}

	WARN_ON(!(qc->flags & ATA_QCFLAG_ACTIVE));
	if (qc->flags & ATA_QCFLAG_ACTIVE) {
		qc->err_mask |= AC_ERR_TIMEOUT;
		/*
		 * test-only: The original code (AMCC: 2.6.19) called
		 * ata_eng_timeout(ap) here. This function is not available
		 * anymore. So what to do now?
		 */
	}
}

/*
 * Function : sata_dwc_qc_prep
 * arguments : ata_queued_cmd *qc
 * Return value : None
 * qc_prep for a particular queued command
 */
static void sata_dwc_qc_prep(struct ata_queued_cmd *qc)
{
	u32 sactive;
	u8 tag = qc->tag;

	 if ((qc->dma_dir == DMA_NONE) || (qc->tf.protocol == ATA_PROT_PIO))
                return;

#ifdef DEBUG_NCQ
	if (qc->tag > 0) {
		dev_info(qc->ap->dev, "%s: qc->tag=%d ap->active_tag=0x%08x\n",
			 __func__, tag, qc->ap->link.active_tag);
	}
#endif

	if (qc->tf.protocol == ATA_PROT_NCQ) {
		sactive = core_scr_read(qc->ap, SCR_ACTIVE);
		sactive |= (0x00000001 << tag);
		core_scr_write(qc->ap, SCR_ACTIVE, sactive);
		dev_dbg(qc->ap->dev, "%s: tag=%d ap->link.sactive = 0x%08x "
			"sactive=0x%08x\n", __func__, tag, qc->ap->link.sactive,
			sactive);
	} else {
		tag = 0;
	}

	sata_dwc_qc_prep_by_tag(qc, tag);
	
}

/*
 * test-only: Needed when no drive is connected upon driver startup.
 * Otherwise, by using the default prereset routine, the driver crashes
 * upon loading.
 * Not sure if this could be handled differently.
 */
static int sata_dwc_prereset(struct ata_link *link, unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	struct ata_eh_context *ehc = &ap->link.eh_context;
	int rc = 0;

	if (ata_link_online(link)) {
		rc = ata_sff_wait_ready(link, deadline);
	} else {
		/* tell EH to bail */
		ehc->i.action &= ~ATA_EH_RESET;
	}

	return rc;
}

/*
 * scsi mid-layer and libata interface structures
 */
static struct scsi_host_template sata_dwc_sht = {
	ATA_NCQ_SHT(DRV_NAME),
	/*
	 * test-only: Currently this driver doesn't handle NCQ
	 * correctly. We enable NCQ but set the queue depth to a
	 * max of 1. This will get fixed in in a future release.
	 */
//        .sg_tablesize           = LIBATA_MAX_PRD,
	.can_queue 		= ATA_DEF_QUEUE,	/* ATA_MAX_QUEUE */
	.dma_boundary 		= ATA_DMA_BOUNDARY,
};

static void sata_dwc_error_handler(struct ata_port *ap)
{
        ap->link.flags |= ATA_LFLAG_NO_HRST;
        ata_sff_error_handler(ap);
        ap->link.flags &= ~ATA_LFLAG_NO_HRST;
}


static struct ata_port_operations sata_dwc_ops = {
	.inherits		= &ata_sff_port_ops,
	
	.error_handler          = sata_dwc_error_handler,

	.qc_prep		= sata_dwc_qc_prep,
	.qc_issue		= sata_dwc_qc_issue,

	.scr_read		= sata_dwc_scr_read,
	.scr_write		= sata_dwc_scr_write,

	.port_start		= sata_dwc_port_start,
	.port_stop		= sata_dwc_port_stop,

	.bmdma_setup		= sata_dwc_bmdma_setup,
	.bmdma_start		= sata_dwc_bmdma_start,

	.prereset		= sata_dwc_prereset,

	/* test-only: really needed? */
	.eng_timeout		= sata_dwc_eng_timeout,
};

static const struct ata_port_info sata_dwc_port_info[] = {
	{
		/*
		 * test-only: Currently this driver doesn't handle NCQ
		 * correctly. So we disable NCQ here for now. To enable
		 * it ATA_FLAG_NCQ needs to be added to the flags below.
		 */
		.flags		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
				  ATA_FLAG_MMIO | ATA_FLAG_NCQ,
		.pio_mask	= 0x1f,	/* pio 0-4 */
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &sata_dwc_ops,
	},
};

static int sata_dwc_probe(struct of_device *ofdev,
			const struct of_device_id *match)
{
	struct sata_dwc_device *hsdev;
	u32 idr, versionr;
	char *ver = (char *)&versionr;
	u8 *base = NULL;
	int err = 0;
	int irq;
	struct ata_host *host;
	struct ata_port_info pi = sata_dwc_port_info[0];
	const struct ata_port_info *ppi[] = { &pi, NULL };

	const unsigned int *dma_channel;

        /*
         * Check if device is enabled
         */
        if (!of_device_is_available(ofdev->node)) {
                printk(KERN_INFO "%s: Port disabled via device-tree\n", 
			ofdev->node->full_name);
                return 0;
        }


	/* Allocate DWC SATA device */
	hsdev = kmalloc(sizeof(*hsdev), GFP_KERNEL);
	if (hsdev == NULL) {
		dev_err(&ofdev->dev, "kmalloc failed for hsdev\n");
		err = -ENOMEM;
		goto error_out;
	}
	memset(hsdev, 0, sizeof(*hsdev));


	// Identify SATA controller index from the cell-index property
	dma_channel = of_get_property(ofdev->node, "dma-channel", NULL);
	if ( dma_channel ) {
		dev_notice(&ofdev->dev, "Gettting DMA channel %d\n", *dma_channel);
		hsdev->dma_channel = *dma_channel;
	} else
		hsdev->dma_channel = 0;

	/* Ioremap SATA registers */
	base = of_iomap(ofdev->node, 0);
	if (!base) {
		dev_err(&ofdev->dev, "ioremap failed for SATA register address\n");
		err = -ENODEV;
		goto error_out;
	}
	hsdev->reg_base = base;
	dev_dbg(&ofdev->dev, "ioremap done for SATA register address\n");

	/* Synopsys DWC SATA specific Registers */
	hsdev->sata_dwc_regs = (void *__iomem)(base + SATA_DWC_REG_OFFSET);

	/* Allocate and fill host */
	host = ata_host_alloc_pinfo(&ofdev->dev, ppi, SATA_DWC_MAX_PORTS);
	if (!host) {
		dev_err(&ofdev->dev, "ata_host_alloc_pinfo failed\n");
		err = -ENOMEM;
		goto error_out;
	}

	host->private_data = hsdev;

	/* Setup port */
	host->ports[0]->ioaddr.cmd_addr = base;
	host->ports[0]->ioaddr.scr_addr = base + SATA_DWC_SCR_OFFSET;
	hsdev->scr_base = (u8 *)(base + SATA_DWC_SCR_OFFSET);
	//scr_addr_sstatus = base + SATA_DWC_SCR_OFFSET;
	sata_dwc_setup_port(&host->ports[0]->ioaddr, (unsigned long)base);

	/* Read the ID and Version Registers */
	idr = in_le32(&hsdev->sata_dwc_regs->idr);
	versionr = in_le32(&hsdev->sata_dwc_regs->versionr);
	dev_notice(&ofdev->dev, "id %d, controller version %c.%c%c\n",
		   idr, ver[0], ver[1], ver[2]);

	/* Get SATA DMA interrupt number */
	irq = irq_of_parse_and_map(ofdev->node, 1);
	if (irq == NO_IRQ) {
		dev_err(&ofdev->dev, "no SATA DMA irq\n");
		err = -ENODEV;
		goto error_out;
	}

	/* Get physical SATA DMA register base address */
	if (!sata_dma_regs) {
	sata_dma_regs = of_iomap(ofdev->node, 1);
	if (!sata_dma_regs) {
		dev_err(&ofdev->dev, "ioremap failed for AHBDMA register address\n");
		err = -ENODEV;
		goto error_out;
	}
	}
	/* Save dev for later use in dev_xxx() routines */
	hsdev->dev = &ofdev->dev;

	/* Init glovbal dev list */
	dwc_dev_list[hsdev->dma_channel] = hsdev;

	/* Initialize AHB DMAC */
	dma_dwc_init(hsdev, irq);

	/* Enable SATA Interrupts */
	sata_dwc_enable_interrupts(hsdev);

	/* Get SATA interrupt number */
	irq = irq_of_parse_and_map(ofdev->node, 0);
	if (irq == NO_IRQ) {
		dev_err(&ofdev->dev, "no SATA irq\n");
		err = -ENODEV;
		goto error_out;
	}

	/*
	 * Now, register with libATA core, this will also initiate the
	 * device discovery process, invoking our port_start() handler &
	 * error_handler() to execute a dummy Softreset EH session
	 */
	ata_host_activate(host, irq, sata_dwc_isr, 0, &sata_dwc_sht);

	dev_set_drvdata(&ofdev->dev, host);

	/* Everything is fine */
	return 0;

error_out:
	/* Free SATA DMA resources */
	dma_dwc_exit(hsdev);

	if (base)
		iounmap(base);

	if (hsdev)
		kfree(hsdev);

	return err;
}

static int sata_dwc_remove(struct of_device *ofdev)
{
	struct device *dev = &ofdev->dev;
	struct ata_host *host = dev_get_drvdata(dev);
	struct sata_dwc_device *hsdev = host->private_data;

	ata_host_detach(host);

	dev_set_drvdata(dev, NULL);

	/* Free SATA DMA resources */
	dma_dwc_exit(hsdev);

	iounmap(hsdev->reg_base);
	kfree(hsdev);
	kfree(host);

	dev_dbg(&ofdev->dev, "done\n");

	return 0;
}

static const struct of_device_id sata_dwc_match[] = {
	{ .compatible = "amcc,sata-460ex", },
	{ .compatible = "amcc,sata-apm82181", },
	{}
};
MODULE_DEVICE_TABLE(of, sata_dwc_match);

static struct of_platform_driver sata_dwc_driver = {
	.name = "sata-dwc",
	.match_table = sata_dwc_match,
	.probe = sata_dwc_probe,
	.remove = sata_dwc_remove,
};

static int __init sata_dwc_init(void)
{
	return of_register_platform_driver(&sata_dwc_driver);
}

static void __exit sata_dwc_exit(void)
{
	of_unregister_platform_driver(&sata_dwc_driver);
}

module_init(sata_dwc_init);
module_exit(sata_dwc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Miesfeld <mmiesfeld@amcc.com>");
MODULE_DESCRIPTION("DesignWare Cores SATA controller driver");
MODULE_VERSION(DRV_VERSION);
