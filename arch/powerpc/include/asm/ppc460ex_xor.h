/*
 * include/asm/ppc440spe_xor.h
 *
 * 440SPe's XOR engines support header file
 *
 * 2006 (c) DENX Software Engineering
 *
 * Author: Yuri Tikhonov <yur@emcraft.com>
 *
 * This file is licensed under the term of  the GNU General Public License
 * version 2. The program licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef PPC440SPE_XOR_H
#define PPC440SPE_XOR_H

#include <asm/types.h>

/* Number of operands supported in the h/w */
#define XOR_MAX_OPS		16

/* XOR Memory Mapped Registers base address is different
 * for ppc440sp and ppc440spe processors
 */
#ifdef CONFIG_440SP
#define XOR_MMAP_BASE		0x100200000ULL
#else
#define XOR_MMAP_BASE		0x400200000ULL
#endif
#define XOR_MMAP_SIZE		0x224ULL

/* XOR Interrupt Source, UIC1[31] */
#define XOR_IRQ			63

/*
 * XOR Command Block Control Register bits
 */
#define XOR_CBCR_LNK_BIT        (1<<31) /* link present */
#define XOR_CBCR_TGT_BIT        (1<<30) /* target present */
#define XOR_CBCR_CBCE_BIT       (1<<29) /* command block compete enable */
#define XOR_CBCR_RNZE_BIT       (1<<28) /* result not zero enable */
#define XOR_CBCR_XNOR_BIT       (1<<15) /* XOR/XNOR */
#define XOR_CDCR_OAC_MSK        (0x7F)  /* operand address count */

/*
 * XORCore Status Register bits
 */
#define XOR_SR_XCP_BIT		(1<<31)	/* core processing */
#define XOR_SR_ICB_BIT		(1<<17)	/* invalid CB */
#define XOR_SR_IC_BIT		(1<<16)	/* invalid command */
#define XOR_SR_IPE_BIT		(1<<15)	/* internal parity error */
#define XOR_SR_RNZ_BIT		(1<<2)	/* result not Zero */
#define XOR_SR_CBC_BIT		(1<<1)	/* CB complete */
#define XOR_SR_CBLC_BIT		(1<<0)	/* CB list complete */

/*
 * XORCore Control Set and Reset Register bits
 */
#define XOR_CRSR_XASR_BIT	(1<<31)	/* soft reset */
#define XOR_CRSR_XAE_BIT	(1<<30)	/* enable */
#define XOR_CRSR_RCBE_BIT	(1<<29)	/* refetch CB enable */
#define XOR_CRSR_PAUS_BIT	(1<<28)	/* pause */
#define XOR_CRSR_64BA_BIT	(1<<27) /* 64/32 CB format */
#define XOR_CRSR_CLP_BIT	(1<<25)	/* continue list processing */

/*
 * XORCore Interrupt Enable Register
 */
#define XOR_IE_ICBIE_BIT	(1<<17)	/* Invalid Command Block Interrupt Enable */
#define XOR_IE_ICIE_BIT		(1<<16)	/* Invalid Command Interrupt Enable */
#define XOR_IE_RPTIE_BIT	(1<<14)	/* Read PLB Timeout Error Interrupt Enable */
#define XOR_IE_CBCIE_BIT	(1<<1)	/* CB complete interrupt enable */
#define XOR_IE_CBLCI_BIT	(1<<0)	/* CB list complete interrupt enable */

/*
 * XOR Accelerator engine Command Block Type
 */
typedef struct {
	/*
	 * Basic 64-bit format XOR CB (Table 19-1, p.463, 440spe_um_1_22.pdf)
	 */
	u32	cbc;		/* control */
	u32	cbbc;		/* byte count */
	u32	cbs;		/* status */
	u8	pad0[4];	/* reserved */
	u32	cbtah;		/* target address high */
	u32	cbtal;		/* target address low */
	u32	cblah;		/* link address high */
	u32	cblal;		/* link address low */
	struct {
		u32 h;
		u32 l;
	} __attribute__ ((packed)) ops [16];
} __attribute__ ((packed)) xor_cb_t;

typedef struct {
	xor_cb_t		*vaddr;
	dma_addr_t		paddr;

	/*
	 * Additional fields
	 */
	struct list_head	link;	/* link to processing CBs */
	u32			status;	/* status of the CB */
	/* status bits: */
	#define XOR_CB_DONE	(1<<0)	/* CB processing competed */
	#define XOR_CB_CANCEL	(1<<1)	/* waiting thread was interrupted */
#if 0
	#define XOR_CB_STALLOC	(1<<2)	/* CB allocated statically */
#endif
} xor_cbd_t;


/*
 * XOR hardware registers Table 19-3, UM 1.22
 */
typedef struct {
	u32	op_ar[16][2];	/* operand address[0]-high,[1]-low registers */
	u8	pad0[352];	/* reserved */
	u32	cbcr;		/* CB control register */
	u32	cbbcr;		/* CB byte count register */
	u32	cbsr;		/* CB status register */
	u8	pad1[4];	/* reserved */
	u32	cbtahr;		/* operand target address high register */
	u32	cbtalr;		/* operand target address low register */
	u32	cblahr;		/* CB link address high register */
	u32	cblalr;		/* CB link address low register */
	u32	crsr;		/* control set register */
	u32	crrr;		/* control reset register */
	u32	ccbahr;		/* current CB address high register */
	u32	ccbalr;		/* current CB address low register */
	u32	plbr;		/* PLB configuration register */
	u32	ier;		/* interrupt enable register */
	u32	pecr;		/* parity error count register */
	u32	sr;		/* status register */
	u32	revidr;		/* revision ID register */
} xor_regs_t;

/*
 * Prototypes
 */
int init_xor_eng(void);
int spe440_xor_block (unsigned int ops_count, unsigned int op_len, void **ops);

#endif /* PPC440SPE_XOR_H */

