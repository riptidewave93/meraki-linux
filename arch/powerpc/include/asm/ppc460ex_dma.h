/*
 * include/asm-ppc/ppc440spe_dma.h
 *
 * 440SPe's DMA engines support header file
 *
 * 2006 (c) DENX Software Engineering
 *
 * Author: Yuri Tikhonov <yur@emcraft.com>
 *
 * This file is licensed under the term of  the GNU General Public License
 * version 2. The program licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef	PPC440SPE_DMA_H
#define PPC440SPE_DMA_H

#include <asm/types.h>

/* Number of elements in the array with statical CDBs */
#define	MAX_STAT_DMA_CDBS	16
/* Number of DMA engines available on the contoller */
#define DMA_ENGINES_NUM		1

/* Maximum h/w supported number of destinations */
#define DMA_DEST_MAX_NUM	2

/* FIFO's params */
#define DMA1_FIFO_SIZE		0x1000
#define DMA_FIFO_ENABLE		(1<<12)

/* DMA Configuration Register. Prefetch disable */
#define DMA_CFG_DXEPD 		(1 << 31)
/* DMA Configuration Register. Prefetch disable */
#define DMA_CFG_DXERO 		(1 << 30)
/* DMA Configuration Register. Data Transfer Engine PLB Priority: */
#define DMA_CFG_DXEPR_LP	(0<<26)
#define DMA_CFG_DXEPR_HP	(3<<26)
#define DMA_CFG_DXEPR_HHP	(2<<26)
#define DMA_CFG_DXEPR_HHHP	(1<<26)

/* DMA Configuration Register. DMA FIFO Manager PLB Priority: */
#define DMA_CFG_DFMPP_LP	(0<<23)
#define DMA_CFG_DFMPP_HP	(3<<23)
#define DMA_CFG_DFMPP_HHP	(2<<23)
#define DMA_CFG_DFMPP_HHHP	(1<<23)

/* DMA Configuration Register. Force 64-byte Alignment */
#define DMA_CFG_FALGN		(1 << 19)

/* I2O Memory Mapped Registers base address */
#define I2O_MMAP_BASE		0x400100000ULL
#define I2O_REG_ENABLE		0x1
#define I2O_MMAP_SIZE		0xF4ULL

/* DMA Memory Mapped Registers base address */
#define DMA0_MMAP_BASE		0x400100100ULL
#define DMA1_MMAP_BASE		0x400100200ULL
#define DMA_MMAP_SIZE		0x80

/* DMA Interrupt Sources, UIC0[20],[22] */
//#define DMA0_CP_FIFO_FULL_IRQ		19
//#define DMA0_CS_FIFO_NEED_SERVICE_IRQ	20
#define DMA1_CP_FIFO_FULL_IRQ		21
#define DMA1_CS_FIFO_NEED_SERVICE_IRQ	22
#define DMA_ERROR_IRQ			54

/*UIC0:*/
#define D0CPF_INT		(1<<12)
#define D0CSF_INT		(1<<11)
#define D1CPF_INT		(1<<10)
#define D1CSF_INT		(1<<9)
/*UIC1:*/
#define DMAE_INT		(1<<9)

/* I2O IOP Interrupt Mask Register */
#define I2O_IOPIM_P0SNE		(1<<3)
#define I2O_IOPIM_P0EM		(1<<5)
#define I2O_IOPIM_P1SNE		(1<<6)
#define I2O_IOPIM_P1EM		(1<<8)

/* DMA CDB fields */
#define DMA_CDB_MSK		(0xF)
#define DMA_CDB_64B_ADDR	(1<<2)
#define DMA_CDB_NO_INT		(1<<3)
#define DMA_CDB_STATUS_MSK	(0x3)
#define DMA_CDB_ADDR_MSK	(0xFFFFFFF0)

/* DMA CDB OpCodes */
#define DMA_CDB_OPC_NO_OP	(0x00)
#define DMA_CDB_OPC_MV_SG1_SG2	(0x01)
#define DMA_CDB_OPC_MULTICAST	(0x05)
#define DMA_CDB_OPC_DFILL128	(0x24)
#define DMA_CDB_OPC_DCHECK128	(0x23)

#define DMA_CUED_XOR_BASE	(0x10000000)
#define DMA_CUED_XOR_HB		(0x00000008)

#ifdef CONFIG_440SP
#define DMA_CUED_MULT1_OFF	0
#define DMA_CUED_MULT2_OFF	8
#define DMA_CUED_MULT3_OFF	16
#define DMA_CUED_REGION_OFF	24
#define DMA_CUED_XOR_WIN_MSK	(0xFC000000)
#else
#define DMA_CUED_MULT1_OFF	2
#define DMA_CUED_MULT2_OFF	10
#define DMA_CUED_MULT3_OFF	18
#define DMA_CUED_REGION_OFF	26
#define DMA_CUED_XOR_WIN_MSK	(0xF0000000)
#endif

#define DMA_CUED_REGION_MSK	0x3
#define DMA_RXOR123		0x0
#define DMA_RXOR124		0x1
#define DMA_RXOR125		0x2
#define DMA_RXOR12		0x3

/* S/G addresses */
#define DMA_CDB_SG_SRC		1
#define DMA_CDB_SG_DST1		2
#define DMA_CDB_SG_DST2		3

/*
 * DMAx engines Command Descriptor Block Type
 */
typedef struct dma_cdb {
	/*
	 * Basic CDB structure (Table 20-17, p.499, 440spe_um_1_22.pdf) 
	 */
	u8	pad0[2];        /* reserved */
	u8	attr;		/* attributes */
	u8	opc;		/* opcode */
	u32	sg1u;		/* upper SG1 address */
	u32	sg1l;		/* lower SG1 address */
	u32	cnt;		/* SG count, 3B used */
	u32	sg2u;		/* upper SG2 address */
	u32	sg2l;		/* lower SG2 address */
	u32	sg3u;		/* upper SG3 address */
	u32	sg3l;		/* lower SG3 address */
} dma_cdb_t;

/*
 * Descriptor of allocated CDB
 */
typedef struct {
	dma_cdb_t		*vaddr;	/* virtual address of CDB */
	dma_addr_t		paddr;	/* physical address of CDB */
	/*
	 * Additional fields
	 */
	struct list_head 	link;	/* link in processing list */
	u32			status;	/* status of the CDB */
	/* status bits:  */
	#define	DMA_CDB_DONE	(1<<0)	/* CDB processing competed */
	#define DMA_CDB_CANCEL	(1<<1)	/* waiting thread was interrupted */
} dma_cdbd_t;

/*
 * DMAx hardware registers (p.515 in 440SPe UM 1.22)
 */
typedef struct {
	u32	cpfpl;
	u32	cpfph;
	u32	csfpl;
	u32	csfph;
	u32	dsts;
	u32	cfg;
	u8	pad0[0x8];
	u16	cpfhp;
	u16	cpftp;
	u16	csfhp;
	u16	csftp;
	u8	pad1[0x8];
	u32	acpl;
	u32	acph;
	u32	s1bpl;
	u32	s1bph;
	u32	s2bpl;
	u32	s2bph;
	u32	s3bpl;
	u32	s3bph;
	u8	pad2[0x10];
	u32	earl;
	u32	earh;
	u8	pad3[0x8];
	u32	seat;
	u32	sead;
	u32	op;
	u32	fsiz;
} dma_regs_t;

/*
 * I2O hardware registers (p.528 in 440SPe UM 1.22)
 */
typedef struct {
	u32	ists;
	u32	iseat;
	u32	isead;
	u8	pad0[0x14];
	u32	idbel;
	u8	pad1[0xc];
	u32	ihis;
	u32	ihim;
	u8	pad2[0x8];
	u32	ihiq;
	u32	ihoq;
	u8	pad3[0x8];
	u32	iopis;
	u32	iopim;
	u32	iopiq;
	u8	iopoq;
	u8	pad4[3];
	u16	iiflh;
	u16	iiflt;
	u16	iiplh;
	u16	iiplt;
	u16	ioflh;
	u16	ioflt;
	u16	ioplh;
	u16	ioplt;
	u32	iidc;
	u32	ictl;
	u32	ifcpp;
	u8	pad5[0x4];
	u16	mfac0;
	u16	mfac1;
	u16	mfac2;
	u16	mfac3;
	u16	mfac4;
	u16	mfac5;
	u16	mfac6;
	u16	mfac7;
	u16	ifcfh;
	u16	ifcht;
	u8	pad6[0x4];
	u32	iifmc;
	u32	iodb;
	u32	iodbc;
	u32	ifbal;
	u32	ifbah;
	u32	ifsiz;
	u32	ispd0;
	u32	ispd1;
	u32	ispd2;
	u32	ispd3;
	u32	ihipl;
	u32	ihiph;
	u32	ihopl;
	u32	ihoph;
	u32	iiipl;
	u32	iiiph;
	u32	iiopl;
	u32	iioph;
	u32	ifcpl;
	u32	ifcph;
	u8	pad7[0x8];
	u32	iopt;
} i2o_regs_t;

#endif /* PPC440SPE_DMA_H */

