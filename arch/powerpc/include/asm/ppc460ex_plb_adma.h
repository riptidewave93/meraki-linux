

#include <linux/types.h>


#define to_ppc460ex_adma_chan(chan) container_of(chan,ppc460ex_p_ch_t,common)
#define to_ppc460ex_adma_device(dev) container_of(dev,ppc460ex_p_dev_t,common)
#define tx_to_ppc460ex_adma_slot(tx) container_of(tx,ppc460ex_p_desc_t,async_tx)


#define DMA_HEXDUMP(b, l)						\
  print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,			\
		 16, 1, (b), (l), false);

#define PPC460EX_DMA_PROC_ROOT "driver/460ex_plb_dma"

#define MAX_PPC460EX_DMA_CHANNELS 4

#define PPC460EX_ADMA_DMA_MAX_BYTE_COUNT 1024 * 1024

#define PPC460EX_ADMA_THRESHOLD 1

#define DCR_DMA0_BASE     0x200 
#define DCR_DMA1_BASE     0x208
#define DCR_DMA2_BASE     0x210 
#define DCR_DMA3_BASE     0x218
#define DCR_DMASR_BASE    0x220

#define PPC460EX_PDMA0_ID 0
#define PPC460EX_PDMA1_ID 1
#define PPC460EX_PDMA2_ID 2
#define PPC460EX_PDMA3_ID 3


#define DMA_FIFO_SIZE 0x1000

#define DMA_CDB_SIZE 64


/* DMA Registers */
#define DCR_DMA2P40_CR0     (DCR_DMA0_BASE + 0x0) /* DMA Channel Control 0 */
#define DCR_DMA2P40_CTC0    (DCR_DMA0_BASE + 0x1) /* DMA Count 0 */
#define DCR_DMA2P40_SAH0    (DCR_DMA0_BASE + 0x2) /* DMA Src Addr High 0 */
#define DCR_DMA2P40_SAL0    (DCR_DMA0_BASE + 0x3) /* DMA Src Addr Low 0 */  
#define DCR_DMA2P40_DAH0    (DCR_DMA0_BASE + 0x4) /* DMA Dest Addr High 0 */
#define DCR_DMA2P40_DAL0    (DCR_DMA0_BASE + 0x5) /* DMA Dest Addr Low 0 */
#define DCR_DMA2P40_SGH0    (DCR_DMA0_BASE + 0x6) /* DMA SG Desc Addr High 0 */
#define DCR_DMA2P40_SGL0    (DCR_DMA0_BASE + 0x7) /* DMA SG Desc Addr Low 0 */

#define DCR_DMA2P40_CR1	        (DCR_DMA1_BASE + 0x0)	/* DMA Channel Control 1 */
#define DCR_DMA2P40_CTC1	(DCR_DMA1_BASE + 0x1)  /* DMA Count 1 */
#define DCR_DMA2P40_SAH1	(DCR_DMA1_BASE + 0x2)	/* DMA Src Addr High 1 */
#define DCR_DMA2P40_SAL1	(DCR_DMA1_BASE + 0x3)	/* DMA Src Addr Low 1 */
#define DCR_DMA2P40_DAH1	(DCR_DMA1_BASE + 0x4)	/* DMA Dest Addr High 1 */
#define DCR_DMA2P40_DAL1	(DCR_DMA1_BASE + 0x5)	/* DMA Dest Addr Low 1 */
#define DCR_DMA2P40_SGH1	(DCR_DMA1_BASE + 0x6)	/* DMA SG Desc Addr High 1 */
#define DCR_DMA2P40_SGL1	(DCR_DMA1_BASE + 0x7)	/* DMA SG Desc Addr Low 1 */ 

#define DCR_DMA2P40_CR2	        (DCR_DMA2_BASE + 0x0)	/* DMA Channel Control 2 */
#define DCR_DMA2P40_CTC2	(DCR_DMA2_BASE + 0x1)  /* DMA Count 2 */
#define DCR_DMA2P40_SAH2	(DCR_DMA2_BASE + 0x2)	/* DMA Src Addr High 2 */
#define DCR_DMA2P40_SAL2	(DCR_DMA2_BASE + 0x3)	/* DMA Src Addr Low 2 */
#define DCR_DMA2P40_DAH2	(DCR_DMA2_BASE + 0x4)	/* DMA Dest Addr High 2 */
#define DCR_DMA2P40_DAL2	(DCR_DMA2_BASE + 0x5)	/* DMA Dest Addr Low 2 */
#define DCR_DMA2P40_SGH2	(DCR_DMA2_BASE + 0x6)	/* DMA SG Desc Addr High 2 */
#define DCR_DMA2P40_SGL2	(DCR_DMA2_BASE + 0x7)	/* DMA SG Desc Addr Low 2 */ 

#define DCR_DMA2P40_CR3	        (DCR_DMA3_BASE + 0x0)	/* DMA Channel Control 3 */
#define DCR_DMA2P40_CTC3	(DCR_DMA3_BASE + 0x1)  /* DMA Count 3 */
#define DCR_DMA2P40_SAH3	(DCR_DMA3_BASE + 0x2)	/* DMA Src Addr High 3 */
#define DCR_DMA2P40_SAL3	(DCR_DMA3_BASE + 0x3)	/* DMA Src Addr Low 3 */
#define DCR_DMA2P40_DAH3	(DCR_DMA3_BASE + 0x4)	/* DMA Dest Addr High 3 */
#define DCR_DMA2P40_DAL3	(DCR_DMA3_BASE + 0x5)	/* DMA Dest Addr Low 3 */
#define DCR_DMA2P40_SGH3	(DCR_DMA3_BASE + 0x6)	/* DMA SG Desc Addr High 3 */
#define DCR_DMA2P40_SGL3	(DCR_DMA3_BASE + 0x7)	/* DMA SG Desc Addr Low 3 */ 

#define DCR_DMA2P40_SR      (DCR_DMASR_BASE + 0x0) /* DMA Status Register */
#define DCR_DMA2P40_SGC     (DCR_DMASR_BASE + 0x3) /* DMA Scatter/Gather Command */
#define DCR_DMA2P40_SLP     (DCR_DMASR_BASE + 0x5) /* DMA Sleep Register */
#define DCR_DMA2P40_POL     (DCR_DMASR_BASE + 0x6) /* DMA Polarity Register */  



/*
 * Function return status codes
 * These values are used to indicate whether or not the function
 * call was successful, or a bad/invalid parameter was passed.
 */
#define DMA_STATUS_GOOD			0
#define DMA_STATUS_BAD_CHANNEL		1
#define DMA_STATUS_BAD_HANDLE		2
#define DMA_STATUS_BAD_MODE		3
#define DMA_STATUS_NULL_POINTER		4
#define DMA_STATUS_OUT_OF_MEMORY	5
#define DMA_STATUS_SGL_LIST_EMPTY	6
#define DMA_STATUS_GENERAL_ERROR	7
#define DMA_STATUS_CHANNEL_NOTFREE	8

#define DMA_CHANNEL_BUSY		0x80000000

/*
 * These indicate status as returned from the DMA Status Register.
 */
#define DMA_STATUS_NO_ERROR	0
#define DMA_STATUS_CS		1	/* Count Status        */
#define DMA_STATUS_TS		2	/* Transfer Status     */
#define DMA_STATUS_DMA_ERROR	3	/* DMA Error Occurred  */
#define DMA_STATUS_DMA_BUSY	4	/* The channel is busy */

/*
 * DMA Channel Control Registers
 */
#ifdef CONFIG_44x
#define	PPC4xx_DMA_64BIT
#define DMA_CR_OFFSET 1
#else
#define DMA_CR_OFFSET 0
#endif

#define DMA_CE_ENABLE        (1<<31)	/* DMA Channel Enable */
#define SET_DMA_CE_ENABLE(x) (((x)&0x1)<<31)
#define GET_DMA_CE_ENABLE(x) (((x)&DMA_CE_ENABLE)>>31)

#define DMA_CIE_ENABLE        (1<<30)	/* DMA Channel Interrupt Enable */
#define SET_DMA_CIE_ENABLE(x) (((x)&0x1)<<30)
#define GET_DMA_CIE_ENABLE(x) (((x)&DMA_CIE_ENABLE)>>30)

#define DMA_TD                (1<<29)
#define SET_DMA_TD(x)         (((x)&0x1)<<29)
#define GET_DMA_TD(x)         (((x)&DMA_TD)>>29)

#define DMA_PL                (1<<28)	/* Peripheral Location */
#define SET_DMA_PL(x)         (((x)&0x1)<<28)
#define GET_DMA_PL(x)         (((x)&DMA_PL)>>28)

#define EXTERNAL_PERIPHERAL    0
#define INTERNAL_PERIPHERAL    1

#define SET_DMA_PW(x)     (((x)&0x7)<<(26-DMA_CR_OFFSET))	/* Peripheral Width */
#define DMA_PW_MASK       SET_DMA_PW(7)
#define   PW_8                 0
#define   PW_16                1
#define   PW_32                2
#define   PW_64                3
#define   PW_128               4


#define GET_DMA_PW(x)     (((x)&DMA_PW_MASK)>>(26-DMA_CR_OFFSET))

#define DMA_DAI           (1<<(25-DMA_CR_OFFSET))	/* Destination Address Increment */
#define SET_DMA_DAI(x)    (((x)&0x1)<<(25-DMA_CR_OFFSET))

#define DMA_SAI           (1<<(24-DMA_CR_OFFSET))	/* Source Address Increment */
#define SET_DMA_SAI(x)    (((x)&0x1)<<(24-DMA_CR_OFFSET))

#define DMA_BEN           (1<<(23-DMA_CR_OFFSET))	/* Buffer Enable */
#define SET_DMA_BEN(x)    (((x)&0x1)<<(23-DMA_CR_OFFSET))

#define SET_DMA_TM(x)     (((x)&0x3)<<(21-DMA_CR_OFFSET))	/* Transfer Mode */
#define DMA_TM_MASK       SET_DMA_TM(3)
#define   TM_PERIPHERAL        0	/* Peripheral */
#define   TM_RESERVED          1	/* Reserved */
#define   TM_S_MM              2	/* Memory to Memory */
#define   TM_D_MM              3	/* Device Paced Memory to Memory */
#define GET_DMA_TM(x)     (((x)&DMA_TM_MASK)>>(21-DMA_CR_OFFSET))

#define SET_DMA_PSC(x)    (((x)&0x3)<<(19-DMA_CR_OFFSET))	/* Peripheral Setup Cycles */
#define DMA_PSC_MASK      SET_DMA_PSC(3)
#define GET_DMA_PSC(x)    (((x)&DMA_PSC_MASK)>>(19-DMA_CR_OFFSET))

#define SET_DMA_PWC(x)    (((x)&0x3F)<<(13-DMA_CR_OFFSET))	/* Peripheral Wait Cycles */
#define DMA_PWC_MASK      SET_DMA_PWC(0x3F)
#define GET_DMA_PWC(x)    (((x)&DMA_PWC_MASK)>>(13-DMA_CR_OFFSET))

#define SET_DMA_PHC(x)    (((x)&0x7)<<(10-DMA_CR_OFFSET))	/* Peripheral Hold Cycles */
#define DMA_PHC_MASK      SET_DMA_PHC(0x7)
#define GET_DMA_PHC(x)    (((x)&DMA_PHC_MASK)>>(10-DMA_CR_OFFSET))

#define DMA_ETD_OUTPUT     (1<<(9-DMA_CR_OFFSET))	/* EOT pin is a TC output */
#define SET_DMA_ETD(x)     (((x)&0x1)<<(9-DMA_CR_OFFSET))

#define DMA_TCE_ENABLE     (1<<(8-DMA_CR_OFFSET))
#define SET_DMA_TCE(x)     (((x)&0x1)<<(8-DMA_CR_OFFSET))

#define DMA_DEC            (1<<(2))	/* Address Decrement */
#define SET_DMA_DEC(x)     (((x)&0x1)<<2)
#define GET_DMA_DEC(x)     (((x)&DMA_DEC)>>2)


/*
 * Transfer Modes
 * These modes are defined in a way that makes it possible to
 * simply "or" in the value in the control register.
 */

#define DMA_MODE_MM		(SET_DMA_TM(TM_S_MM))	/* memory to memory */

				/* Device-paced memory to memory, */
				/* device is at source address    */
#define DMA_MODE_MM_DEVATSRC	(DMA_TD | SET_DMA_TM(TM_D_MM))

				/* Device-paced memory to memory,      */
				/* device is at destination address    */
#define DMA_MODE_MM_DEVATDST	(SET_DMA_TM(TM_D_MM))

#define SGL_LIST_SIZE	16384
#define DMA_PPC4xx_SIZE SGL_LIST_SIZE

#define SET_DMA_PRIORITY(x)   (((x)&0x3)<<(6-DMA_CR_OFFSET))	/* DMA Channel Priority */
#define DMA_PRIORITY_MASK SET_DMA_PRIORITY(3)
#define PRIORITY_LOW           0
#define PRIORITY_MID_LOW       1
#define PRIORITY_MID_HIGH      2
#define PRIORITY_HIGH          3
#define GET_DMA_PRIORITY(x) (((x)&DMA_PRIORITY_MASK)>>(6-DMA_CR_OFFSET))


#define SET_DMA_PREFETCH(x)   (((x)&0x3)<<(4-DMA_CR_OFFSET))	/* Memory Read Prefetch */
#define DMA_PREFETCH_MASK      SET_DMA_PREFETCH(3)
#define   PREFETCH_1           0	/* Prefetch 1 Double Word */
#define   PREFETCH_2           1
#define   PREFETCH_4           2
#define GET_DMA_PREFETCH(x) (((x)&DMA_PREFETCH_MASK)>>(4-DMA_CR_OFFSET))

#define DMA_PCE            (1<<(3-DMA_CR_OFFSET))	/* Parity Check Enable */
#define SET_DMA_PCE(x)     (((x)&0x1)<<(3-DMA_CR_OFFSET))
#define GET_DMA_PCE(x)     (((x)&DMA_PCE)>>(3-DMA_CR_OFFSET))

/*
 * DMA Polarity Configuration Register
 */
#define DMAReq_ActiveLow(chan) (1<<(31-(chan*3)))
#define DMAAck_ActiveLow(chan) (1<<(30-(chan*3)))
#define EOT_ActiveLow(chan)    (1<<(29-(chan*3)))	/* End of Transfer */

/*
 * DMA Sleep Mode Register
 */
#define SLEEP_MODE_ENABLE (1<<21)

/*
 * DMA Status Register
 */
#define DMA_CS0           (1<<31)	/* Terminal Count has been reached */
#define DMA_CS1           (1<<30)
#define DMA_CS2           (1<<29)
#define DMA_CS3           (1<<28)

#define DMA_TS0           (1<<27)	/* End of Transfer has been requested */
#define DMA_TS1           (1<<26)
#define DMA_TS2           (1<<25)
#define DMA_TS3           (1<<24)

#define DMA_CH0_ERR       (1<<23)	/* DMA Chanel 0 Error */
#define DMA_CH1_ERR       (1<<22)
#define DMA_CH2_ERR       (1<<21)
#define DMA_CH3_ERR       (1<<20)

#define DMA_IN_DMA_REQ0   (1<<19)	/* Internal DMA Request is pending */
#define DMA_IN_DMA_REQ1   (1<<18)
#define DMA_IN_DMA_REQ2   (1<<17)
#define DMA_IN_DMA_REQ3   (1<<16)

#define DMA_EXT_DMA_REQ0  (1<<15)	/* External DMA Request is pending */
#define DMA_EXT_DMA_REQ1  (1<<14)
#define DMA_EXT_DMA_REQ2  (1<<13)
#define DMA_EXT_DMA_REQ3  (1<<12)

#define DMA_CH0_BUSY      (1<<11)	/* DMA Channel 0 Busy */
#define DMA_CH1_BUSY      (1<<10)
#define DMA_CH2_BUSY       (1<<9)
#define DMA_CH3_BUSY       (1<<8)

#define DMA_SG0            (1<<7)	/* DMA Channel 0 Scatter/Gather in progress */
#define DMA_SG1            (1<<6)
#define DMA_SG2            (1<<5)
#define DMA_SG3            (1<<4)

/* DMA Channel Count Register */
#define DMA_CTC_TCIE	 (1<<29)	/* Terminal Count Interrupt Enable */
#define DMA_CTC_ETIE     (1<<28)	/* EOT Interupt Enable */
#define DMA_CTC_EIE	 (1<<27)	/* Error Interrupt Enable */
#define DMA_CTC_BTEN     (1<<23)    /* Burst Enable/Disable bit */
#define DMA_CTC_BSIZ_MSK (3<<21)    /* Mask of the Burst size bits */
#define DMA_CTC_BSIZ_2   (0)
#define DMA_CTC_BSIZ_4   (1<<21)
#define DMA_CTC_BSIZ_8   (2<<21)
#define DMA_CTC_BSIZ_16  (3<<21)
#define DMA_CTC_TC_MASK  0xFFFFF

/*
 * DMA SG Command Register
 */
#define SSG_ENABLE(chan)   	(1<<(31-chan))	/* Start Scatter Gather */
#define SSG_MASK_ENABLE(chan)	(1<<(15-chan))	/* Enable writing to SSG0 bit */


/*
 * DMA Scatter/Gather Descriptor Bit fields
 */
#define SG_LINK            (1<<31)	/* Link */
#define SG_TCI_ENABLE      (1<<29)	/* Enable Terminal Count Interrupt */
#define SG_ETI_ENABLE      (1<<28)	/* Enable End of Transfer Interrupt */
#define SG_ERI_ENABLE      (1<<27)	/* Enable Error Interrupt */
#define SG_COUNT_MASK       0xFFFF	/* Count Field */

#define SET_DMA_CONTROL \
 		(SET_DMA_CIE_ENABLE(p_init->int_enable) | /* interrupt enable         */ \
 		SET_DMA_BEN(p_init->buffer_enable)     | /* buffer enable            */\
		SET_DMA_ETD(p_init->etd_output)        | /* end of transfer pin      */ \
	       	SET_DMA_TCE(p_init->tce_enable)        | /* terminal count enable    */ \
                SET_DMA_PL(p_init->pl)                 | /* peripheral location      */ \
                SET_DMA_DAI(p_init->dai)               | /* dest addr increment      */ \
                SET_DMA_SAI(p_init->sai)               | /* src addr increment       */ \
                SET_DMA_PRIORITY(p_init->cp)           |  /* channel priority        */ \
                SET_DMA_PW(p_init->pwidth)             |  /* peripheral/bus width    */ \
                SET_DMA_PSC(p_init->psc)               |  /* peripheral setup cycles */ \
                SET_DMA_PWC(p_init->pwc)               |  /* peripheral wait cycles  */ \
                SET_DMA_PHC(p_init->phc)               |  /* peripheral hold cycles  */ \
                SET_DMA_PREFETCH(p_init->pf)              /* read prefetch           */)

#define GET_DMA_POLARITY(chan) (DMAReq_ActiveLow(chan) | DMAAck_ActiveLow(chan) | EOT_ActiveLow(chan))


/**
 * struct ppc460ex_dma_device - internal representation of an DMA device
 * @pdev: Platform device
 * @id: HW DMA Device selector
 * @dma_desc_pool: base of DMA descriptor region (DMA address)
 * @dma_desc_pool_virt: base of DMA descriptor region (CPU address)
 * @common: embedded struct dma_device
 */
typedef struct ppc460ex_plb_dma_device {
	//struct platform_device *pdev;
	struct of_device *odev;
	void __iomem *reg_base;
	struct device *dev;
	struct resource reg;    /* Resource for register */
	int id;
	struct dma_device common;
        struct ppc460ex_plb_dma_chan *chan[MAX_PPC460EX_DMA_CHANNELS];
	wait_queue_head_t queue;
} ppc460ex_p_dev_t;

typedef uint32_t sgl_handle_t;
/**
 * struct ppc460ex_dma_chan - internal representation of an ADMA channel
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
 * @irq_tasklet: bottom half where ppc460ex_adma_slot_cleanup runs
 * @needs_unmap: if buffers should not be unmapped upon final processing
 */
typedef struct ppc460ex_plb_dma_chan {
	spinlock_t lock;
	void __iomem *reg_base;
	struct ppc460ex_plb_dma_device *device;
	struct timer_list cleanup_watchdog;
	struct list_head chain;
	struct dma_chan common;
	struct list_head all_slots;
	struct ppc460ex_adma_desc_slot *last_used;
	int pending;
	dma_cookie_t completed_cookie;
	int slots_allocated;
	int hw_chain_inited;
	void *dma_desc_pool_virt;
	dma_addr_t dma_desc_pool;

	struct resource reg;            /* Resource for register */
	unsigned int chan_id;
        struct tasklet_struct irq_tasklet;
	u8 needs_unmap;
        sgl_handle_t *phandle;
        unsigned short in_use;	/* set when channel is being used, clr when
				 * available.
				 */
	/*
	 * Valid polarity settings:
	 *   DMAReq_ActiveLow(n)
	 *   DMAAck_ActiveLow(n)
	 *   EOT_ActiveLow(n)
	 *
	 *   n is 0 to max dma chans
	 */
	unsigned int polarity;

	char buffer_enable;	/* Boolean: buffer enable            */
	char tce_enable;	/* Boolean: terminal count enable    */
	char etd_output;	/* Boolean: eot pin is a tc output   */
	char pce;		/* Boolean: parity check enable      */

	/*
	 * Peripheral location:
	 * INTERNAL_PERIPHERAL (UART0 on the 405GP)
	 * EXTERNAL_PERIPHERAL
	 */
	char pl;		/* internal/external peripheral      */

	/*
	 * Valid pwidth settings:
	 *   PW_8
	 *   PW_16
	 *   PW_32
	 *   PW_64
	 */
	unsigned int pwidth;

	char dai;		/* Boolean: dst address increment   */
	char sai;		/* Boolean: src address increment   */

	/*
	 * Valid psc settings: 0-3
	 */
	unsigned int psc;	/* Peripheral Setup Cycles         */

	/*
	 * Valid pwc settings:
	 * 0-63
	 */
	unsigned int pwc;	/* Peripheral Wait Cycles          */

	/*
	 * Valid phc settings:
	 * 0-7
	 */
	unsigned int phc;	/* Peripheral Hold Cycles          */

	/*
	 * Valid cp (channel priority) settings:
	 *   PRIORITY_LOW
	 *   PRIORITY_MID_LOW
	 *   PRIORITY_MID_HIGH
	 *   PRIORITY_HIGH
	 */
	unsigned int cp;	/* channel priority                */

	/*
	 * Valid pf (memory read prefetch) settings:
	 *
	 *   PREFETCH_1
	 *   PREFETCH_2
	 *   PREFETCH_4
	 */
	unsigned int pf;	/* memory read prefetch            */

	/*
	 * Boolean: channel interrupt enable
	 * NOTE: for sgl transfers, only the last descriptor will be setup to
	 * interrupt.
	 */
	char int_enable;

	char shift;		/* easy access to byte_count shift, based on */
	/* the width of the channel                  */

	uint32_t control;	/* channel control word                      */

	/* These variabled are used ONLY in single dma transfers              */
	unsigned int mode;	/* transfer mode                     */
	phys_addr_t addr;
	char ce;		/* channel enable */
        char int_on_final_sg;/* for scatter/gather - only interrupt on last sg */

} ppc460ex_p_ch_t;
typedef struct ppc460ex_adma_plb_desc_slot {
	dma_addr_t phys;
	struct ppc460ex_adma_plb_desc_slot *group_head;
	struct ppc460ex_adma_plb_desc_slot *hw_next;
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
#define PPC460EX_DESC_INT       0       /* generate interrupt on complete */

}ppc460ex_p_desc_t;

typedef struct ppc460ex_adma_plb_platform_data {
	int hw_id;
	dma_cap_mask_t cap_mask;
	size_t pool_size;
} ppc460ex_p_aplat_t;

typedef struct dma_cdb {
	u32 ctrl;
	u32 cnt;
	u32 src_hi;
	u32 src_lo;
	u32 dest_hi;
	u32 dest_lo;
	u32 sg_hi;
	u32 sg_lo;
}dma_cdb_t;
/*
 * PPC44x DMA implementations have a slightly different
 * descriptor layout.  Probably moved about due to the
 * change to 64-bit addresses and link pointer. I don't
 * know why they didn't just leave control_count after
 * the dst_addr.
 */
#ifdef PPC4xx_DMA_64BIT
typedef struct {
	uint32_t control;
	uint32_t control_count;
	phys_addr_t src_addr;
	phys_addr_t dst_addr;
	phys_addr_t next;
} ppc_sgl_t;
#else
typedef struct {
	uint32_t control;
	phys_addr_t src_addr;
	phys_addr_t dst_addr;
	uint32_t control_count;
	uint32_t next;
} ppc_sgl_t;
#endif



typedef struct {
	unsigned int ch_id;
	uint32_t control;	/* channel ctrl word; loaded from each descrptr */
	uint32_t sgl_control;	/* LK, TCI, ETI, and ERI bits in sgl descriptor */
	dma_addr_t dma_addr;	/* dma (physical) address of this list          */
	ppc_sgl_t *phead;
	dma_addr_t phead_dma;
	ppc_sgl_t *ptail;
	dma_addr_t ptail_dma;
} sgl_list_info_t;

typedef struct {
	phys_addr_t *src_addr;
	phys_addr_t *dst_addr;
	phys_addr_t dma_src_addr;
	phys_addr_t dma_dst_addr;
} pci_alloc_desc_t;

#define PPC460EX_DMA_SGXFR_COMPLETE(id)		(!((1 << (11-id)) & mfdcr(DCR_DMA2P40_SR)))
#define PPC460EX_DMA_CHAN_BUSY(id)		( (1 << (11-id)) & mfdcr(DCR_DMA2P40_SR) )		
#define DMA_STATUS(id)				(mfdcr(DCR_DMA2P40_SR))
#define CLEAR_DMA_STATUS(id)			(mtdcr(DCR_DMA2P40_SR, 0xFFFFFFFF))
#define PPC460EX_DMA_SGSTAT_FREE(id)			(!((1 << (7-id)) & mfdcr(DCR_DMA2P40_SR)) )
#define PPC460EX_DMA_TC_REACHED(id)		( (1 << (31-id)) & mfdcr(DCR_DMA2P40_SR) )
#define PPC460EX_DMA_CHAN_XFR_COMPLETE(id)	( (!PPC460EX_DMA_CHAN_BUSY(id)) && (PPC460EX_DMA_TC_REACHED(id)) )
#define PPC460EX_DMA_CHAN_SGXFR_COMPLETE(id)	( (!PPC460EX_DMA_CHAN_BUSY(id)) && PPC460EX_DMA_SGSTAT_FREE(id) )
#define PPC460EX_DMA_SG_IN_PROGRESS(id)		( (1 << (7-id)) | (1 << (11-id)) )
#define PPC460EX_DMA_SG_OP_COMPLETE(id)		( (PPC460EX_DMA_SG_IN_PROGRESS(id) & DMA_STATUS(id) ) == 0)



#if 0
extern int test_dma_memcpy(void *src, void *dst, unsigned int length, unsigned int dma_ch);

extern int test_sgdma_memcpy(void *src, void *dst, void *src1, void *dst1, 
			     unsigned int length, unsigned int dma_ch);
#endif
