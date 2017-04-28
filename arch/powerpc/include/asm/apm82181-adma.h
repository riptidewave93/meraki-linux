/*
 * 2009-2010 (C) Applied Micro Circuits Corporation.
 *
 * Author: Tai Tri Nguyen<ttnguyen@appliedmicro.com>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of
 * any kind, whether express or implied.
 */

#ifndef APM82181_ADMA_H
#define APM82181_ADMA_H


#include <linux/types.h>


#define to_apm82181_adma_chan(chan) container_of(chan,apm82181_ch_t,common)
#define to_apm82181_adma_device(dev) container_of(dev,apm82181_dev_t,common)
#define tx_to_apm82181_adma_slot(tx) container_of(tx,apm82181_desc_t,async_tx)

#define APM82181_DMA_PROC_ROOT "driver/apm82181_adma"

/* Number of operands supported in the h/w */
#define XOR_MAX_OPS             16
/* this is the XOR_CBBCR width */
#define APM82181_ADMA_XOR_MAX_BYTE_COUNT       (1 << 31)
#define APM82181_ADMA_DMA_MAX_BYTE_COUNT 1024 * 1024
#define MAX_APM82181_DMA_CHANNELS 5
#define APM82181_ADMA_THRESHOLD 1

#define APM82181_PDMA0_ID 0
#define APM82181_PDMA1_ID 1
#define APM82181_PDMA2_ID 2
#define APM82181_PDMA3_ID 3
#define APM82181_XOR_ID	  4 

/* DMA 0/1/2/3 registers */
#define DCR_DMAx_BASE(x)	(0x200 + x*0x8)			/* DMA DCR base */
#define DCR_DMA2P40_CRx(x)     	(DCR_DMAx_BASE(x) + 0x0)   /* DMA Channel Control */
#define DMA_CR_CE		(1 << 31)
#define DMA_CR_CIE		(1 << 30)
#define DMA_CR_PL		(1 << 28)
#define DMA_CR_PW_128		0x08000000
#define DMA_CR_DAI		0x01000000
#define DMA_CR_SAI		0x00800000
#define DMA_CR_BEN		0x00400000
#define DMA_CR_TM_S_MM		0x00300000
#define DMA_CR_ETD		0x00000100
#define DMA_CR_TCE		0x00000080
#define DMA_CR_CP(x)		(x<<5)& 0x00000060
#define DMA_CR_DEC		(1 << 2)
#define DMA_CR_SL		(1 << 1)
#define DCR_DMA2P40_CTCx(x)    	(DCR_DMAx_BASE(x) + 0x1) /* DMA Count 0 */
#define DMA_CTC_ETIE		(1 << 28)
#define DMA_CTC_EIE		(1 << 27)
#define DMA_CTC_PCE		(1 << 20)
#define DMA_CTC_TC_MASK		0x000fffff
#define DCR_DMA2P40_SAHx(x)    	(DCR_DMAx_BASE(x) + 0x2) /* DMA Src Addr High 0 */
#define DCR_DMA2P40_SALx(x)    	(DCR_DMAx_BASE(x) + 0x3) /* DMA Src Addr Low 0 */
#define DCR_DMA2P40_DAHx(x)    	(DCR_DMAx_BASE(x) + 0x4) /* DMA Dest Addr High 0 */
#define DCR_DMA2P40_DALx(x)    	(DCR_DMAx_BASE(x) + 0x5) /* DMA Dest Addr Low 0 */
#define DCR_DMA2P40_SGHx(x)    	(DCR_DMAx_BASE(x) + 0x6) /* DMA SG Desc Addr High 0 */
#define DCR_DMA2P40_SGLx(x)    	(DCR_DMAx_BASE(x) + 0x7) /* DMA SG Desc Addr Low 0 */
/* DMA Status Register */
#define DCR_DMA2P40_SR      	0x220 
#define DMA_SR_CS(x)		(1 << (31 -x))
#define DMA_SR_TS(x)		(1 << (27 -x))
#define DMA_SR_RI(x)		(1 << (23 -x))
#define DMA_SR_IR(x)		(1 << (19 -x))
#define DMA_SR_ER(x)		(1 << (15 -x))
#define DMA_SR_CB(x)		(1 << (11 -x))
#define DMA_SR_SG(x)		(1 << (7 -x))
/* S/G registers */
#define DCR_DMA2P40_SGC		0x223
#define DMA_SGC_SSG(x)		( 1 << (31 - x))
#define DMA_SGC_SGL(x,y)	( y << (27 - x)) /* x: channel; y: 0 PLB, 1 OPB*/
#define DMA_SGC_EM(x)		( 1 << (15 - x))
#define DMA_SGC_EM_ALL		0x0000F000

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
#define XOR_SR_XCP_BIT          (1<<31) /* core processing */
#define XOR_SR_ICB_BIT          (1<<17) /* invalid CB */
#define XOR_SR_IC_BIT           (1<<16) /* invalid command */
#define XOR_SR_IPE_BIT          (1<<15) /* internal parity error */
#define XOR_SR_RNZ_BIT          (1<<2)  /* result not Zero */
#define XOR_SR_CBC_BIT          (1<<1)  /* CB complete */
#define XOR_SR_CBLC_BIT         (1<<0)  /* CB list complete */

/*
 * XORCore Control Set and Reset Register bits
 */
#define XOR_CRSR_XASR_BIT       (1<<31) /* soft reset */
#define XOR_CRSR_XAE_BIT        (1<<30) /* enable */
#define XOR_CRSR_RCBE_BIT       (1<<29) /* refetch CB enable */
#define XOR_CRSR_PAUS_BIT       (1<<28) /* pause */
#define XOR_CRSR_64BA_BIT       (1<<27) /* 64/32 CB format */
#define XOR_CRSR_CLP_BIT        (1<<25) /* continue list processing */

/*
 * XORCore Interrupt Enable Register
 */
#define XOR_IE_ICBIE_BIT        (1<<17) /* Invalid Command Block Interrupt Enable */
#define XOR_IE_ICIE_BIT         (1<<16) /* Invalid Command Interrupt Enable */
#define XOR_IE_RPTIE_BIT        (1<<14) /* Read PLB Timeout Error Interrupt Enable */
#define XOR_IE_CBCIE_BIT        (1<<1)  /* CB complete interrupt enable */
#define XOR_IE_CBLCI_BIT        (1<<0)  /* CB list complete interrupt enable */

typedef struct apm82181_plb_dma4_device {
        struct resource reg;    /* Resource for register */
        void __iomem *reg_base;
        struct of_device *ofdev;
        struct device *dev;
} apm82181_plb_dma_t;

/**
 * struct apm82181_dma_device - internal representation of an DMA device
 * @id: HW DMA Device selector
 * @ofdev: OF device
 * @dcr_base: dcr base of HW PLB DMA channels
 * @reg_base: base of ADMA XOR channel
 * @dma_desc_pool: base of DMA descriptor region (DMA address)
 * @dma_desc_pool_virt: base of DMA descriptor region (CPU address)
 * @pool_size: memory pool size for the channel device
 * @common: embedded struct dma_device
 * @cap_mask: capabilities of ADMA channels
 */
typedef struct apm82181_plb_dma_device {
	int id;
	struct of_device *ofdev;
	u32 dcr_base;
	void __iomem *xor_base;
	struct device *dev;
	struct dma_device common;
	struct apm82181_plb_dma4_device *pdma;
	void *dma_desc_pool_virt;
	u32 pool_size;
	dma_addr_t dma_desc_pool;
	dma_cap_mask_t cap_mask;
} apm82181_dev_t;

/**
 * struct apm82181_dma_chan - internal representation of an ADMA channel
 * @lock: serializes enqueue/dequeue operations to the slot pool
 * @device: parent device
 * @chain: device chain view of the descriptors
 * @common: common dmaengine channel object members
 * @all_slots: complete domain of slots usable by the channel
 * @reg: Resource for register 
 * @pending: allows batching of hardware operations
 * @completed_cookie: identifier for the most recently completed operation
 * @slots_allocated: records the actual size of the descriptor slot pool
 * @hw_chain_inited: h/w descriptor chain initialization flag
 * @irq_tasklet: bottom half where apm82181_adma_slot_cleanup runs
 * @needs_unmap: if buffers should not be unmapped upon final processing
 */
typedef struct apm82181_plb_dma_chan {
	spinlock_t lock;
	struct apm82181_plb_dma_device *device;
	struct timer_list cleanup_watchdog;
	struct list_head chain;
	struct dma_chan common;
	struct list_head all_slots;
	struct apm82181_adma_plb_desc_slot *last_used;
	int pending;
	dma_cookie_t completed_cookie;
	int slots_allocated;
	int hw_chain_inited;
        struct tasklet_struct irq_tasklet;
	u8 needs_unmap;
	phys_addr_t current_cdb_addr;	
} apm82181_ch_t;

typedef struct apm82181_adma_plb_desc_slot {
	dma_addr_t phys;
	struct apm82181_adma_plb_desc_slot *group_head;
	struct apm82181_adma_plb_desc_slot *hw_next;
	struct dma_async_tx_descriptor async_tx;
	struct list_head slot_node;
	struct list_head chain_node;
	struct list_head group_list;
	unsigned int unmap_len;
	void *hw_desc;
	u16 stride;
	u16 idx;
	u16 slot_cnt;
	u8 src_cnt;
	u8 dst_cnt;
	u8 slots_per_op;
	u8 descs_per_op;
	unsigned long flags;
	unsigned long reverse_flags[8];
#define APM82181_DESC_INT       0       /* generate interrupt on complete */

}apm82181_desc_t;

typedef struct {
	u32 ce:1;
	u32 cie:1;
	u32 td:1;
	u32 pl:1;
	u32 pw:3;
	u32 dai:1;
	u32 sai:1;
	u32 ben:1;
	u32 tm:2;
	u32 psc:2;
	u32 pwc:6;
	u32 phc:3;
	u32 etd:1;
	u32 tce:1;
	u32 cp:2;
	u32 pf:2;
	u32 dec:1;
	u32 sl:1;
	u32 reserved:1;
} __attribute__((packed)) dma_cdb_ctrl_t;

typedef struct {
	u32 link:1;
	u32 sgl:1;
	u32 tcie:1;
	u32 etie:1;
	u32 eie:1;
	u32 sid:3;
	u32 bten:1;
	u32 bsiz:2;
	u32 pce:1;
	u32 tc:20;
} __attribute__((packed)) dma_cdb_count_t;
/* scatter/gather descriptor struct */
typedef struct dma_cdb {
	dma_cdb_ctrl_t ctrl;
	dma_cdb_count_t cnt;
	u32 src_hi;
	u32 src_lo;
	u32 dest_hi;
	u32 dest_lo;
	u32 sg_hi;
	u32 sg_lo;
}dma_cdb_t;

typedef struct {
        uint32_t control;
        phys_addr_t src_addr;
        phys_addr_t dst_addr;
        uint32_t control_count;
        uint32_t next;
} ppc_sgl_t;

/*
 * XOR Accelerator engine Command Block Type
 */
typedef struct {
        /*
         * Basic 64-bit format XOR CB
         */
        u32     cbc;            /* control */
        u32     cbbc;           /* byte count */
        u32     cbs;            /* status */
        u8      pad0[4];        /* reserved */
        u32     cbtah;          /* target address high */
        u32     cbtal;          /* target address low */
        u32     cblah;          /* link address high */
        u32     cblal;          /* link address low */
        struct {
                u32 h;
                u32 l;
        } __attribute__ ((packed)) ops [16];
} __attribute__ ((packed)) xor_cb_t;

/*
 * XOR hardware registers
 */
typedef struct {
        u32     op_ar[16][2];   /* operand address[0]-high,[1]-low registers */
        u8      pad0[352];      /* reserved */
        u32     cbcr;           /* CB control register */
        u32     cbbcr;          /* CB byte count register */
        u32     cbsr;           /* CB status register */
        u8      pad1[4];        /* reserved */
        u32     cbtahr;         /* operand target address high register */
        u32     cbtalr;         /* operand target address low register */
        u32     cblahr;         /* CB link address high register */
        u32     cblalr;         /* CB link address low register */
        u32     crsr;           /* control set register */
        u32     crrr;           /* control reset register */
        u32     ccbahr;         /* current CB address high register */
        u32     ccbalr;         /* current CB address low register */
        u32     plbr;           /* PLB configuration register */
        u32     ier;            /* interrupt enable register */
        u32     pecr;           /* parity error count register */
        u32     sr;             /* status register */
        u32     revidr;         /* revision ID register */
} xor_regs_t;

#endif
