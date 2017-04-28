#ifndef _ATH79_NAND_H_
#define _ATH79_NAND_H_

#define ATH79_NF_COMMAND		(0x000u)
#define ATH79_NF_CTRL			(0x004u)
#define ATH79_NF_STATUS			(0x008u)
#define ATH79_NF_INT_MASK		(0x00cu)
#define ATH79_NF_INT_STATUS		(0x010u)
#define ATH79_NF_ECC_CTRL		(0x014u)
#define ATH79_NF_ECC_OFFSET		(0x018u)
#define ATH79_NF_ADDR0_0		(0x01cu)
#define ATH79_NF_ADDR1_0		(0x020u)
#define ATH79_NF_ADDR0_1		(0x024u)
#define ATH79_NF_ADDR1_1		(0x028u)
#define ATH79_NF_SPARE_SIZE		(0x030u)
#define ATH79_NF_PROTECT		(0x038u)
#define ATH79_NF_LOOKUP_EN		(0x040u)
#define ATH79_NF_LOOKUP0		(0x044u)
#define ATH79_NF_LOOKUP1		(0x048u)
#define ATH79_NF_LOOKUP2		(0x04cu)
#define ATH79_NF_LOOKUP3		(0x050u)
#define ATH79_NF_LOOKUP4		(0x054u)
#define ATH79_NF_LOOKUP5		(0x058u)
#define ATH79_NF_LOOKUP6		(0x05cu)
#define ATH79_NF_LOOKUP7		(0x060u)
#define ATH79_NF_DMA_ADDR		(0x064u)
#define ATH79_NF_DMA_COUNT		(0x068u)
#define ATH79_NF_DMA_CTRL		(0x06cu)
#define ATH79_NF_MEM_CTRL		(0x080u)
#define ATH79_NF_PG_SIZE		(0x084u)
#define ATH79_NF_RD_STATUS		(0x088u)
#define ATH79_NF_TIME_SEQ		(0x08cu)
#define ATH79_NF_TIMINGS_ASYN		(0x090u)
#define ATH79_NF_TIMINGS_SYN		(0x094u)
#define ATH79_NF_FIFO_DATA		(0x098u)
#define ATH79_NF_TIME_MODE		(0x09cu)
#define ATH79_NF_DMA_ADDR_OFFSET	(0x0a0u)
#define ATH79_NF_FIFO_INIT		(0x0b0u)
#define ATH79_NF_GENERIC_SEQ_CTRL	(0x0b4u)
#define ATH79_NF_FIFO_STATUS		(0x0b8u)

#define ATH79_NF_CTRL_STAT        (1 << 8)

#define ATH79_NF_TIMING_ASYN	0x11
#define ATH79_NF_STATUS_OK	0x40	//0xc0
#define ATH79_NF_RD_STATUS_MASK	0x47	//0xc7

#define ATH79_NF_CTRL_SMALL_BLOCK_EN	(1 << 21)

#define ATH79_NF_CTRL_ADDR_CYCLE1_0	(0 << 18)
#define ATH79_NF_CTRL_ADDR_CYCLE1_1	(1 << 18)
#define ATH79_NF_CTRL_ADDR_CYCLE1_2	(2 << 18)
#define ATH79_NF_CTRL_ADDR_CYCLE1_3	(3 << 18)
#define ATH79_NF_CTRL_ADDR_CYCLE1_4	(4 << 18)
#define ATH79_NF_CTRL_ADDR_CYCLE1_5	(5 << 18)

#define ATH79_NF_CTRL_ADDR1_AUTO_INC_EN	(1 << 17)
#define ATH79_NF_CTRL_ADDR0_AUTO_INC_EN	(1 << 16)
#define ATH79_NF_CTRL_WORK_MODE_SYNC	(1 << 15)
#define ATH79_NF_CTRL_PROT_EN		(1 << 14)
#define ATH79_NF_CTRL_LOOKUP_EN		(1 << 13)
#define ATH79_NF_CTRL_IO_WIDTH_16BIT	(1 << 12)
#define ATH79_NF_CTRL_CUSTOM_SIZE_EN	(1 << 11)

#define ATH79_NF_CTRL_PAGE_SIZE_256	(0 <<  8)	/* bytes */
#define ATH79_NF_CTRL_PAGE_SIZE_512	(1 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_1024	(2 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_2048	(3 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_4096	(4 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_8192	(5 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_16384	(6 <<  8)
#define ATH79_NF_CTRL_PAGE_SIZE_0		(7 <<  8)

#define ATH79_NF_CTRL_BLOCK_SIZE_32	(0 <<  6)	/* pages */
#define ATH79_NF_CTRL_BLOCK_SIZE_64	(1 <<  6)
#define ATH79_NF_CTRL_BLOCK_SIZE_128	(2 <<  6)
#define ATH79_NF_CTRL_BLOCK_SIZE_256	(3 <<  6)

#define ATH79_NF_CTRL_ECC_EN		(1 <<  5)
#define ATH79_NF_CTRL_INT_EN		(1 <<  4)
#define ATH79_NF_CTRL_SPARE_EN		(1 <<  3)

#define ATH79_NF_CTRL_ADDR_CYCLE0_0	(0 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0_1	(1 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0_2	(2 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0_3	(3 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0_4	(4 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0_5	(5 <<  0)
#define ATH79_NF_CTRL_ADDR_CYCLE0(c)	((c) << 0)


#define ATH79_NF_DMA_CTRL_DMA_START	(1 << 7)
#define ATH79_NF_DMA_CTRL_DMA_DIR_WRITE	(0 << 6)
#define ATH79_NF_DMA_CTRL_DMA_DIR_READ	(1 << 6)
#define ATH79_NF_DMA_CTRL_DMA_MODE_SG	(1 << 5)
/*
 * 000 - incrementing precise burst of precisely four transfers
 * 001 - stream burst (address const)
 * 010 - single transfer (address increment)
 * 011 - burst of unspecified length (address increment)
 * 100 - incrementing precise burst of precisely eight transfers
 * 101 - incrementing precise burst of precisely sixteen transfers
 */
#define ATH79_NF_DMA_CTRL_DMA_BURST_0	(0 << 2)
#define ATH79_NF_DMA_CTRL_DMA_BURST_1	(1 << 2)
#define ATH79_NF_DMA_CTRL_DMA_BURST_2	(2 << 2)
#define ATH79_NF_DMA_CTRL_DMA_BURST_3	(3 << 2)
#define ATH79_NF_DMA_CTRL_DMA_BURST_4	(4 << 2)
#define ATH79_NF_DMA_CTRL_DMA_BURST_5	(5 << 2)
#define ATH79_NF_DMA_CTRL_ERR_FLAG	(1 << 1)
#define ATH79_NF_DMA_CTRL_DMA_READY	(1 << 0)

#define ATH79_NF_ECC_CTRL_ERR_THRESH(x)	((x << 8) & (0x1fu << 8))
#define ATH79_NF_ECC_CTRL_ECC_CAP(x)	((x << 5) & (0x07u << 5))
#define ATH79_NF_ECC_CTRL_ECC_2_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(0)
#define ATH79_NF_ECC_CTRL_ECC_4_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(1)
#define ATH79_NF_ECC_CTRL_ECC_6_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(2)
#define ATH79_NF_ECC_CTRL_ECC_8_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(3)
#define ATH79_NF_ECC_CTRL_ECC_10_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(4)
#define ATH79_NF_ECC_CTRL_ECC_12_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(5)
#define ATH79_NF_ECC_CTRL_ECC_14_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(6)
#define ATH79_NF_ECC_CTRL_ECC_16_BITS	ATH79_NF_ECC_CTRL_ECC_CAP(7)

#define ATH79_NF_ECC_CTRL_ERR_OVER	(1 << 2)
#define ATH79_NF_ECC_CTRL_ERR_UNCORR	(1 << 1)
#define ATH79_NF_ECC_CTRL_ERR_CORR	(1 << 0)
#	define ATH79_NF_ECC_ERROR	(ATH79_NF_ECC_CTRL_ERR_UNCORR | \
					 ATH79_NF_ECC_CTRL_ERR_OVER)

#define ATH79_NF_CMD_END_INT		(1 << 1)

#define ATH79_NF_HW_ECC		1
#define ATH79_NF_STATUS_RETRY	1000

#define ATH79_CMD_SEQ_0  0x0
#define ATH79_CMD_SEQ_1  0x21
#define ATH79_CMD_SEQ_2  0x22
#define ATH79_CMD_SEQ_4  0x24
#define ATH79_CMD_SEQ_7  0x27
#define ATH79_CMD_SEQ_10 0x2a
#define ATH79_CMD_SEQ_12 0x0c
#define ATH79_CMD_SEQ_14 0x0e

#endif //_ATH79_NAND_H_
