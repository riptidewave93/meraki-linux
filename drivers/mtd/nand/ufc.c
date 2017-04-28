/*
 *  drivers/mtd/ufc.c
 *
 *  Overview:
 *   Platform independent driver for UFC (NanD Flash Controller)
 *   integrated into APM8018x cores. This driver has been ported
 *   from NDFC driver.
 *
 *   Ported to an OF platform driver
 *
 *  Author: Manikandan Pillai
 *
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/ufc.h>
#include <linux/mtd/mtd.h>
#include <linux/of_platform.h>
#include <linux/bch.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/ipp.h>

/*#define UFC_DEBUG*/
#ifdef UFC_DEBUG
#define ufc_dprint(fmt, ...) printk(KERN_INFO "UFC_DEBUG: %s: " fmt, __func__, \
								##__VA_ARGS__);
#else
#define ufc_dprint(fmt, ...)
#endif

#define	UFC_TIMEOUT_LOOP_CNTR	0x100
#define	UFC_HW_DELAY		5
#define CONFIG_SYS_HZ		1000

#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_5_ADDR_CYCLE    1

int ufc_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int page);
void ufc_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf);

struct ufc_controller {
	struct platform_device *pdev;
	void __iomem *ufcbase;
	struct mtd_info mtd;
	struct nand_chip chip;
	int chip_select;
	struct nand_hw_control ufc_control;
	struct mtd_part_parser_data ppdata;
};

static struct ufc_controller ufc_ctrl;

enum {
	BOOTDEV_NONE = 0,
	BOOTDEV_LEGACY_NAND = 2,
	BOOTDEV_LEGACY_NOR = 3,
	_BOOTDEV_LAST,

};
static int bootdev = BOOTDEV_NONE;
static int nand_cs = BOOTDEV_NONE;
static int nor_cs = BOOTDEV_NONE;

/*#define UFC_DEBG*/
/*#define DEBUG_CSR*/

#ifdef UFC_DEBG
#define UFC_DBG(format, arg...) printk(format, ##arg)
#else
#define UFC_DBG(format, arg...) do {} while (0)
#endif

#ifdef DEBUG_CSR
#define UFC_CSRDBG(format, arg...) printk(format, ##arg)
#else
#define UFC_CSRDBG(format, arg...) do {} while (0)
#endif

#define	NAND_PKT_SIZE	256
#define	NAND_PKT_CNT	4
#define	NAND_UFC_READ	0
#define	NAND_UFC_WR	1

#if 1
static struct nand_ecclayout nand_swecc_4 = {
        .eccbytes = 28,
        .eccpos = { 	32, 33, 34, 35, 36, 37, 38,
			40, 41, 42, 43, 44, 45, 46,
			48, 49, 50, 51, 52, 53, 54,
			56, 57, 58, 59, 60, 61, 62 },
        .oobfree = {
                {.offset = 2,
                 .length = 30},
        }
};
#endif

struct ufc_state {
	/* UFC interface is 32-bit access. Need shadow to support 8-bit access */
	int shadow_rd_offset;
	unsigned int shadow_rd_val;
};

/**
 * struct nand_bch_control - private NAND BCH control structure
 * @bch:       BCH control structure
 * @ecclayout: private ecc layout for this BCH configuration
 * @errloc:    error location array
 * @eccmask:   XOR ecc mask, allows erased pages to be decoded as valid
 */
struct nand_bch_control {
	struct bch_control   *bch;
	struct nand_ecclayout ecclayout;
	unsigned int         *errloc;
	unsigned char        *eccmask;
};

static struct ufc_state ufc_chip_state[CONFIG_SYS_MAX_NAND_DEVICE];

static inline void OUTLE32(void *addr, unsigned int val)
{
	out_le32(addr, val);
	UFC_CSRDBG("UFC CSR WR %p val 0x%08X\n", addr, val);
}

static inline unsigned int INLE32(void *addr)
{
	unsigned int val = in_le32(addr);
	UFC_CSRDBG("UFC CSR RD %p val 0x%08X\n", addr, val);
	return val;
}

static inline void OUTBE32(void *addr, unsigned int val)
{
	out_be32((volatile unsigned int *)addr, val);
	UFC_CSRDBG("SCU CSR WR %p val 0x%08X\n", addr, val);
}

static inline unsigned int INBE32(void *addr)
{
	unsigned int val = in_be32(addr);
	UFC_CSRDBG("SCU CSR RD %p val 0x%08X\n", addr, val);
	return val;
}

static void ufc_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	ufc_dprint("Inside\n");
	return;
}

static int ufc_dev_ready(struct mtd_info *mtd)
{
#if 0
	struct ufc_controller *ufc = &ufc_ctrl;
        volatile u32 timeout = UFC_TIMEOUT_LOOP_CNTR;
        u32 reg_val;

	reg_val = in_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL));
        while(((reg_val & UFC_CTRL_BUSY)) && (timeout--) ) {
		udelay(UFC_HW_DELAY);
                reg_val = in_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL));
        }
	if(timeout==0) printk("READY expired\n");
        return timeout;
#else
	struct ufc_controller *ufc = &ufc_ctrl;
	volatile u32 timeout = 0x10000;
	u32 reg_val;

	do {
		reg_val = INLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL));
		udelay(1);
	} while ((reg_val & UFC_CTRL_BUSY) && (timeout--));

	return timeout;
#endif
}

/* Returns a positive value, if success */
int ufc_dev_done(void)
{
	struct ufc_controller *ufc = &ufc_ctrl;
	volatile u32 timeout = 0x20;
	u32 reg_val;

	do {
		reg_val = INLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL));
/*                udelay(1);*/
	} while ((reg_val & UFC_CTRL_START) && timeout--);

	return timeout;
}

#if 0
static void ufc_enable_hwecc(struct mtd_info *mtd, int mode)
{
	struct ufc_controller *ufc = &ufc_ctrl;
        u32 ccr;

	ufc_dprint("Inside\n");
        ccr = in_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_CMD_REG));
	ccr |= (UFC_ECC_ON_OFF);
        out_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_CMD_REG), ccr);
}
#endif

#if 0
static int ufc_calculate_ecc(struct mtd_info *mtd,
			      const u_char *dat, u_char *ecc_code)
{
	printk("Calculate ECC\n");
	return 0;
}
#endif

unsigned char ufc_read_byte(struct mtd_info *mtd)
{
        register struct nand_chip *chip = mtd->priv;
	struct ufc_state *state = (struct ufc_state *) chip->priv;
	struct ufc_controller *ufc = &ufc_ctrl;
	u32 val;
	int off;

	if (state == NULL || state->shadow_rd_offset == -1) {

		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
				UFC_CTRL_FIFO_RST);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 0x00000000);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
			UFC_CTRL_START | UFC_CTRL_READ | UFC_CTRL_DATACNT);
		ufc_dev_done();
		off = 0;
		val = INLE32((u32 *)(ufc->ufcbase + UFC_RX_CMDDATA));
		if (state) {
			state->shadow_rd_offset = 0;
			state->shadow_rd_val = val;
		}
	} else {
		off = state->shadow_rd_offset;
		val = state->shadow_rd_val;
	}

	switch (off % 4) {
	case 0:
	default:
		if (state)
			state->shadow_rd_offset++;
		return val & 0xFF;
	case 1:
		if (state)
			state->shadow_rd_offset++;
		return (val >> 8) & 0xFF;
	case 2:
		if (state)
			state->shadow_rd_offset++;
		return (val >> 16) & 0xFF;
	case 3:
		if (state)
			state->shadow_rd_offset = -1;
		return (val >> 24) & 0xFF;
	}
}

u16 ufc_read_word(struct mtd_info *mtd)
{
	struct ufc_controller *ufc = &ufc_ctrl;
	register struct nand_chip *chip = mtd->priv;
	struct ufc_state *state = (struct ufc_state *) chip->priv;

	if (state->shadow_rd_offset == -1) {
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 
				UFC_CTRL_FIFO_RST);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 0x00000000);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 
			UFC_CTRL_START | UFC_CTRL_READ | UFC_CTRL_DATACNT);
		ufc_dev_done();
		state->shadow_rd_val = INLE32((u32 *)(ufc->ufcbase + 
							UFC_RX_CMDDATA));
		state->shadow_rd_offset = 0;
	}

	switch (state->shadow_rd_offset % 2) {
	case 0:
	default:
		state->shadow_rd_offset += 2;
		return state->shadow_rd_val & 0xFFFF;
	case 2:
		state->shadow_rd_offset = -1;
		return (state->shadow_rd_val >> 16) & 0xFFFF;
	}
}

/*
 * Speedups for buffer read/write/verify
 *
 * UFC allows 32bit read/write of data. So we can speed up the buffer
 * functions. No further checking, as nand_base will always read/write
 * page aligned.
 */
static void ufc_read_buf(struct mtd_info *mtdinfo, uint8_t *buf, int len)
{
	uint32_t *p = (uint32_t *) buf;
	struct ufc_controller *ufc = &ufc_ctrl;
	int tmp = len;

	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), UFC_CTRL_FIFO_RST);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 0x00000000);
	for (; tmp > 0; tmp -= 4) {
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 
			UFC_CTRL_START | UFC_CTRL_READ | UFC_CTRL_DATACNT);
		ufc_dev_done();
		*p++ = INLE32((u32 *)(ufc->ufcbase + UFC_RX_CMDDATA));
	}
}

static void ufc_write_buf(struct mtd_info *mtdinfo, const uint8_t *buf, int len)
{
	uint32_t *p = (uint32_t *) buf;
	struct ufc_controller *ufc = &ufc_ctrl;
	unsigned int tmp;

	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), UFC_CTRL_FIFO_RST);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 0x00000000);
	tmp = len;
	ufc_dev_done();
	for (tmp = len; tmp > 0; tmp -= 4) {
		OUTLE32((u32 *)(ufc->ufcbase + UFC_TX_CMDDATA), *p++);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 
		UFC_CTRL_START | UFC_CTRL_WRITE |  UFC_CTRL_DATACNT);
		ufc_dev_done();
	}
}

static int ufc_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	ufc_dprint("Inside\n");
	return 0;
}

void ufc_reset(void)
{
#if 0
	u32 val;

	val = INBE32((void *)CONFIG_SYS_SCU_BASE + SCU_SRST_ADDR);
	OUTBE32((void *)CONFIG_SYS_SCU_BASE + SCU_SRST_ADDR, val & ~NDFC_MASK);
	OUTBE32((void *)CONFIG_SYS_SCU_BASE + SCU_SRST_ADDR, val);
#else /* !0 */
	struct apm86xxx_reset_ctrl reset_ctrl;

	/* Reset UFC */
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reg_group = REG_GROUP_SRST_CLKEN;
	reset_ctrl.csr_reset_mask = 0;
	reset_ctrl.reset_mask = NDFC_MASK;
	__reset_apm86xxx_block(&reset_ctrl);
#endif /* !0 */
}

#if 0
/**
 * ufc_correct_data - [NAND Interface] Detect and correct bit error(s)
 * @mtd:        MTD block structure
 * @dat:        raw data read from the chip
 * @read_ecc:   ECC from the chip
 * @calc_ecc:   the ECC calculated from raw data
 *
 * Detect and correct a 1 bit error for 256 byte block
 */
int ufc_correct_data(struct mtd_info *mtd, u_char *dat,
                      u_char *read_ecc, u_char *calc_ecc)
{
        u32 reg_val;
	struct ufc_controller *ufc = &ufc_ctrl;

	ufc_dprint("Inside\n");
        reg_val = in_le32((u32 *)(ufc->ufcbase + UFC_INT_STAT));
	if(reg_val & UFC_MUL_BIT_ERR_INT)
	{
		printk("UFC ERR data\n");
		return 1;
	}
	else
		return 0;
        //return(reg_val & UFC_MUL_BIT_ERR_INT);
}
#endif

/**
 * ufc_read_page_hwecc - [REPLACABLE] hardware ecc based page read function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        buffer to store read data
 *
 * Not for syndrome calculating ecc controllers which need a special oob layout
 */
int ufc_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
                                uint8_t *buf, int page)
{
        uint8_t *p = buf;
	//struct ufc_controller *ufc = &ufc_ctrl;

	ufc_dprint("Inside\n");
        chip->read_buf(mtd, p, mtd->writesize);
	//out_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_COL_ADDR), mtd->writesize);
	//chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
        return 0;
}

/**
 * ufc_write_page_hwecc - UFC hardware ecc based page write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        data buffer
 */
void ufc_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
                                  const uint8_t *buf)
{
        const uint8_t *p = buf;
	//struct ufc_controller *ufc = &ufc_ctrl;
	//uint32_t tmp;

	ufc_dprint("Inside\n");
        //chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
        chip->write_buf(mtd, p, mtd->writesize);
	
	//out_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_COL_ADDR), mtd->writesize);
	//tmp = in_le32((u32 *)(ufc->ufcbase + UFC_NFLASH_ROW_ADDR));

	//printk("OOB wr size = 0x%x page = 0x%x\n", mtd->oobsize, tmp<<11);
	//chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
}

/**
 * ufc_write_oob_std - [REPLACABLE] the most common OOB data write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @page:       page number to write
 */
static int ufc_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip,
                              int page)
{
	int status = 0;
	const uint8_t *buf = chip->oob_poi;
	int length = mtd->oobsize;

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize, page);
	chip->write_buf(mtd, buf, length);
	/* Send command to program the OOB data */
	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

	status = chip->waitfunc(mtd, chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

/**
 * nand_cmd - [DEFAULT] Send command to NAND large page device
 * @mtd:        MTD device structure
 * @command:    the command to be sent
 * @column:     the column address for this command, -1 if none
 * @page_addr:  the page address for this command, -1 if none
 *
 * Send command to NAND device. This is the version for the new large page
 * devices We dont have the separate regions as we have in the small page
 * devices.  We must emulate NAND_CMD_READOOB to keep the code compatible.
 */
void ufc_nand_command(struct mtd_info *mtd, unsigned int command,
                            int column, int page_addr)
{
	register struct nand_chip *chip = mtd->priv;
	unsigned int ctrl = 0;
	struct ufc_state *state;
	struct ufc_controller *ufc = &ufc_ctrl;

	/* Emulate NAND_CMD_READOOB */
	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
	}

	/* Reset the FIFO */
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), UFC_CTRL_FIFO_RST);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 0x00000000);
	ctrl |= UFC_CTRL_START;
	ctrl |= (command & 0xFF) | UFC_CTRL_CMD0_PRE;

	if ((column != -1 || page_addr != -1) && command != NAND_CMD_READID) {
		/* Serially input address */
		if (column != -1) {
			OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_COL_ADDR),
					column);
			ctrl |= UFC_CTRL_COL_ADDR_PRE;
		}
		if (page_addr != -1) {
			OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_ROW_ADDR),
					page_addr);
			ctrl |= UFC_CTRL_ROW_ADDR_PRE;
		}
	}

	state = (struct ufc_state *) chip->priv;
	if (state)
		state->shadow_rd_offset = -1;	/* Reset offset state */

	/*
	 * program and erase have their own busy handlers
	 * status, sequential in, and deplete1 need no delay
	 */
	switch (command) {
		case NAND_CMD_CACHEDPROG:
		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE1:
		case NAND_CMD_SEQIN:
		case NAND_CMD_RNDIN:
		case NAND_CMD_STATUS:
		case NAND_CMD_DEPLETE1:
		case NAND_CMD_RESET:
			break;
		case NAND_CMD_READID:
			ctrl |= UFC_CTRL_CMD1_PRE | UFC_CTRL_ADDR_SPL;
			break;
		case NAND_CMD_ERASE2:
			ctrl |= ((command & 0xFF) << UFC_CTRL_CMD1_SHIFT)
				| UFC_CTRL_CMD1_PRE;
			break;

			/*
			 * read error status commands require only a short delay
			 */
		case NAND_CMD_STATUS_ERROR:
		case NAND_CMD_STATUS_ERROR0:
			OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), ctrl);
			ufc_dev_done();
			break;

		case NAND_CMD_RNDOUT:
			ctrl &= ~UFC_CTRL_START;
			ctrl |= ((NAND_CMD_RNDOUTSTART & 0xFF) << UFC_CTRL_CMD1_SHIFT)
				| UFC_CTRL_CMD1_PRE;
			break;

		case NAND_CMD_READOOB:
			ctrl &= ~0xFF;
			ctrl |= (NAND_CMD_READ0 | UFC_CTRL_CMD0_PRE | UFC_CTRL_START);
			ctrl |= ((NAND_CMD_READSTART & 0xFF) << UFC_CTRL_CMD1_SHIFT)
				| UFC_CTRL_CMD1_PRE;
			break;

		case NAND_CMD_READ0:
			ctrl |= ((NAND_CMD_READSTART & 0xFF) << UFC_CTRL_CMD1_SHIFT)
				| UFC_CTRL_CMD1_PRE;
			break;
			/* This applies to read commands */
		default:
			/*
			 * If we don't have access to the busy pin, we apply the given
			 * command delay
			 */
			if (!chip->dev_ready) {
				udelay(chip->chip_delay);
				return;
			}
	}
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), ctrl);
	ufc_dev_done();

	if (command == NAND_CMD_STATUS) {
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
				(UFC_CTRL_START | UFC_CTRL_READ));
		ufc_dev_done();
	}

	/* Apply this short delay always to ensure that we do wait tWB in
	 * any case on any machine.
	 * FIXME - why 100us
	 */
	udelay(100);

	switch (command) {
	case NAND_CMD_RESET:
		udelay(100);
		break;
	}
}

static int ufc_wait(struct mtd_info *mtd, struct nand_chip *this)
{
	volatile unsigned long   timeo;
	int state = this->state;
	struct ufc_controller *ufc = &ufc_ctrl;

	if (state == FL_ERASING)
		timeo = (CONFIG_SYS_HZ * 400) / 1000;
	else
		timeo = (CONFIG_SYS_HZ * 20) / 1000;

	if ((state == FL_ERASING) && (this->options & NAND_IS_AND)) {
		this->cmdfunc(mtd, NAND_CMD_STATUS_MULTI, -1, -1);
	} else {
		this->cmdfunc(mtd, NAND_CMD_STATUS, -1, -1);
	}

	timeo = 0x200000;
	while (timeo--) {
		if (this->dev_ready) {
			if (this->dev_ready(mtd))
				break;
		}
	}

	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL), 
		UFC_CTRL_START | UFC_CTRL_READ);
	/* wait until command done */
	ufc_dev_done();

	timeo = INLE32((u32 *)(ufc->ufcbase + UFC_STAT_REG));

	return timeo & 0xFF;
}

static void ufc_set_ctrl_timing(void)
{
	u32 val;
	struct ufc_controller *ufc = &ufc_ctrl;

	/* Program 2-byte column and 3-byte row adress size for CSx */
	val  = 0x1 << 2;		/* 2-bytes column */
#if defined(CONFIG_SYS_NAND_5_ADDR_CYCLE)
	val |= 0x2 << 4;		/* 3-bytes row */
#else
	val |= 0x1 << 4;		/* 2-bytes row */
#endif
	switch (nand_cs) {
	case 0:
	default:
		/* FIXME - Need to tune the timing interval */
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_TIM_INTER_0),
			0x00000002);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_TIMING1_INTER_0),
			0x00000004);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_RDYBSYLOW_CNT_0),
			0x00000014);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL2_0), val);
		break;
	case 1:
		/* FIXME - Need to tune the timing interval */
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_TIM_INTER_1),
			0x00000002);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_TIMING1_INTER_1),
			0x00000004);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_RDYBSYLOW_CNT_1),
			0x00000014);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL2_1), val);
		break;
	case 2:
		/* FIXME - Need to tune the timing interval */
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_TIM_INTER_2),
			0x00000002);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_TIMING1_INTER_2),
			0x00000004);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_RDYBSYLOW_CNT_2),
			0x00000014);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL2_2), val);
		break;
	case 3:
		/* FIXME - Need to tune the timing interval */
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_TIM_INTER_3),
			0x00000002);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_TIMING1_INTER_3),
			0x00000004);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_RDYBSYLOW_CNT_3),
			0x00000014);
		OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL2_3), val);
		break;
	}
}

void board_nand_select_device(struct nand_chip *nand, int chip)
{
	u32 reg_val;
	struct ufc_controller *ufc = &ufc_ctrl;

	switch (nor_cs) {
	case 1:
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_BANKCFG_1, 0x00000007);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CTRL2_1, 0x00725010);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CFG_1, 0x0000037e);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CTRL1_1, 0x00000040);
		break;

	default:
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_BANKCFG_0, 0x00000007);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CTRL2_0, 0x00725010);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CFG_0, 0x0000037e);
		OUTLE32((void *)ufc->ufcbase + UFC_SRAM_CTRL1_0, 0x00000040);
		break;
	}

	/* Program NAND device type on CSx */
	reg_val  = INLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL3));

	/* Prepare value to be set for NAND in UFC_NFLASH_CTRL3 */
	reg_val &= ~(0x3 << (nand_cs * 2));
	reg_val |= UFC_FLASH_TYPE_NAND << (nand_cs * 2);

	/* Prepare value to be set for NOR in UFC_NFLASH_CTRL3 */
	reg_val &= ~(0x3 << (nor_cs * 2));
	reg_val |= UFC_FLASH_TYPE_NOR << (nor_cs * 2);

	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL3), reg_val);

	/* Relocate CS0-CS3 to 0xFF00.0000 to 0xFC00.0000 */
	OUTLE32((u32 *)(ufc->ufcbase + UFC_SRAM_CS), 0x0000cdef);

	/* Disable all interrupt and clear all interrupt status */
	OUTLE32((u32 *)(ufc->ufcbase + UFC_INT_EN), 0x00000000);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_INT_STAT), 0xFFFFFFFF);

	OUTLE32((void *)(ufc->ufcbase + UFC_FLAG), UFC_FLAG_EN(nand_cs));
}

void ufc_select_chip(struct mtd_info *mtd, int chip)
{
	struct ufc_controller *ufc = &ufc_ctrl;

	ufc_dprint("Inside\n");

	switch (chip) {
	/* Disable NAND */
	case -1:
		OUTLE32((u32 *)(ufc->ufcbase + UFC_FLAG),
				UFC_FLAG_EN(nor_cs));

		break;

	/* Enable NAND */
	default:
		OUTLE32((u32 *)(ufc->ufcbase + UFC_FLAG),
				UFC_FLAG_EN(nand_cs));
		break;
	}
}
EXPORT_SYMBOL(ufc_select_chip);

static int ufc_sw_bch_init(struct nand_chip *nand)
{
	unsigned char *erased_page;
	struct nand_bch_control *nbc;
	int m = fls(1+8*nand->ecc.size);
	int t = (nand->ecc.bytes*8)/m;
	int i;

	ufc_dprint("Inside\n");
	UFC_DBG("UFC BCH initalize m %d t %d eccsize %d eccbyte %d\n", 
		m, t, nand->ecc.size, nand->ecc.bytes);
	nbc = kmalloc(sizeof(*nbc), GFP_KERNEL);
	if (nbc == NULL)
		return -1;
	nbc->bch = init_bch(m, t, 0);
	if (nbc->bch == NULL) {
		kfree(nbc);
		return -1;
	}
	nbc->eccmask = kmalloc(nand->ecc.bytes, GFP_KERNEL);
	nbc->errloc = kmalloc(t*sizeof(*nbc->errloc), GFP_KERNEL);
	/*
	 * compute and store the inverted ecc of an erased ecc block
	 */
	erased_page = kmalloc(nand->ecc.size, GFP_KERNEL);
	if (!erased_page || !nbc->eccmask || !nbc->errloc)
		goto fail;

	memset(erased_page, 0xff, nand->ecc.size);
	memset(nbc->eccmask, 0, nand->ecc.bytes);
	encode_bch(nbc->bch, erased_page, nand->ecc.size, nbc->eccmask);
	kfree(erased_page);

	for (i = 0; i < nand->ecc.bytes; i++)
		nbc->eccmask[i] ^= 0xff;

	nand->ecc.priv = nbc;
	return 0;

fail:
	if (nbc->eccmask)
		kfree(nbc->eccmask);
	if (nbc->eccmask)
		kfree(nbc->errloc);
	if (erased_page)
		kfree(erased_page);
	return -1;
}

/*
 * Initialize chip structure
 */
static int ufc_chip_init(struct ufc_controller *ufc,
			  struct device_node *node)
{

	struct device_node *flash_np;
	struct nand_chip *chip = &ufc->chip;
	int pagesize;
	u32 val;
	int ret;

	ufc_dprint("Inside\n");
	board_nand_select_device(chip, 0);

	/*
	 * Settings same as u-boot
	 */
	chip->IO_ADDR_R = (void __iomem *)(ufc->ufcbase + UFC_RX_CMDDATA);
	chip->IO_ADDR_W = (void __iomem *)(ufc->ufcbase + UFC_TX_CMDDATA);
	chip->chip_delay = 50;
	chip->read_byte = ufc_read_byte;
	chip->read_buf = ufc_read_buf;
	chip->dev_ready = ufc_dev_ready;

	chip->cmdfunc = ufc_nand_command;
	chip->cmd_ctrl = ufc_hwcontrol;
/*        nand->read_word = ufc_read_word;*/
	chip->write_buf = ufc_write_buf;
	chip->verify_buf = ufc_verify_buf;
	chip->waitfunc = ufc_wait;
/*        chip->read_status = ufc_read_status;*/
/*        chip->priv = &ufc_chip_state[chip];*/

	apm86xxx_read_scu_reg(SCU_NFBOOT0_ADDR, &val);
	val  = 0x00200000; 		/* Enable Auto-Read */
	/* val |= 0x00040000; */	/* Enable UFC HW ECC */
	apm86xxx_write_scu_reg(SCU_NFBOOT0_ADDR, val);

	OUTLE32((void *)ufc->ufcbase + UFC_SRAM_BOOT_CFG, 0xFFFF0000);

	ufc_set_ctrl_timing();

	/* Reset the NAND device */
	OUTLE32((u32*)(ufc->ufcbase + UFC_NFLASH_CTRL),
			UFC_CTRL_START | UFC_CTRL_CMD0_PRE | NAND_CMD_RESET);
	ufc_dev_ready(NULL);

	/* Read the NAND ID to extract page size */
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
			UFC_CTRL_START | UFC_CTRL_CMD0_PRE | NAND_CMD_READID
			| UFC_CTRL_CMD1_PRE | UFC_CTRL_ADDR_SPL);
	ufc_dev_ready(NULL);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
			UFC_CTRL_DATACNT | UFC_CTRL_START| UFC_CTRL_READ);
	ufc_dev_ready(NULL);
	val = INLE32((u32 *)(ufc->ufcbase + UFC_RX_CMDDATA));
	pagesize = 1024 << ((val >> 16) & 0x3);
	OUTLE32((u32 *)(ufc->ufcbase + UFC_NFLASH_CTRL),
			UFC_CTRL_DATACNT | UFC_CTRL_START| UFC_CTRL_READ);
	ufc_dev_ready(NULL);
	val = INLE32((u32 *)(ufc->ufcbase + UFC_RX_CMDDATA));

	/*
	 * Software ECC settings
	 */
/*        chip->ecc.hwctl = ufc_enable_hwecc;*/
/*        chip->ecc.correct = ufc_correct_data;*/
/*        chip->ecc.calculate = ufc_calculate_ecc;*/
	chip->ecc.mode = NAND_ECC_SOFT_BCH;
	chip->ecc.size = 512;
	chip->ecc.steps = pagesize / chip->ecc.size;
	chip->ecc.bytes = 7;
	chip->ecc.layout = &nand_swecc_4;

	if (chip->ecc.mode == NAND_ECC_SOFT_BCH)
		ufc_sw_bch_init(chip);

	/*
	 * Extra Linux specific settings
	 */
	chip->select_chip = ufc_select_chip;
	chip->controller = &ufc->ufc_control;

        chip->ecc.write_oob = ufc_write_oob_std;
	chip->priv = &ufc_chip_state[0];

	chip->ecc.read_page = ufc_read_page_hwecc;
	chip->ecc.write_page = ufc_write_page_hwecc;

	ufc->mtd.priv = chip;
	ufc->mtd.owner = THIS_MODULE;

	flash_np = of_get_next_child(node, NULL);
	if (!flash_np)
		return -ENODEV;

	ufc->ppdata.of_node = flash_np;
	ufc->mtd.name = kasprintf(GFP_KERNEL, "%s.%s",
			dev_name(&ufc->pdev->dev), flash_np->name);

	if (!ufc->mtd.name) {
		ret = -ENOMEM;
		goto err;
	}

	ret = nand_scan(&ufc->mtd, 1);
	if (ret)
		goto err;

	ret = mtd_device_parse_register(&ufc->mtd, NULL, &ufc->ppdata, NULL, ret);

err:
	of_node_put(flash_np);
	if (ret)
		kfree(ufc->mtd.name);
	return ret;
}

static int __devinit ufc_probe(struct platform_device *pdev)
{
	struct ufc_controller *ufc = &ufc_ctrl;
/*        struct device_node *np = ofdev->node;*/
	const u32 *reg;
	int err, len;

	ufc_dprint("Inside\n");

	apm86xxx_read_scu_reg(SCU_PPCBOOTDEV_ADDR, &bootdev);
	ufc_dprint("SCU_PPCBOOTDEV_ADDR = 0x%08x\n", bootdev);

	switch (bootdev) {
	case BOOTDEV_LEGACY_NOR:
		ufc_dprint("BOOTDEV_LEGACY_NOR\n");
		nor_cs  = 0;
		nand_cs = 2;
		break;

	case BOOTDEV_LEGACY_NAND:
		ufc_dprint("BOOTDEV_LEGACY_NAND\n");
		nor_cs  = 1;
		nand_cs = 0;
		break;

	default:
		printk(KERN_INFO "Unsuported bootdev specified\n");
		return -ENODEV;
		break;
	}

	spin_lock_init(&ufc->ufc_control.lock);
	init_waitqueue_head(&ufc->ufc_control.wq);
	ufc->pdev = pdev;
	dev_set_drvdata(&pdev->dev, ufc);

	/* Read the reg property to get the chip select */
	reg = of_get_property(pdev->dev.of_node, "reg", &len);
	ufc->chip_select = 0;
	ufc->ufcbase = of_iomap(pdev->dev.of_node, 0);
	if (!ufc->ufcbase) {
		dev_err(&pdev->dev, "failed to get memory\n");
		return -EIO;
	}

	/* Set the bank settings if given */

	err = ufc_chip_init(ufc, pdev->dev.of_node);
	if (err) {
		iounmap(ufc->ufcbase);
		return err;
	}
	return 0;
}

static int __devexit ufc_remove(struct platform_device *pdev)
{
	struct ufc_controller *ufc = dev_get_drvdata(&pdev->dev);

	nand_release(&ufc->mtd);

	return 0;
}

static const struct of_device_id ufc_match[] = {
	{ .compatible = "ibm,ufc", },
	{}
};
MODULE_DEVICE_TABLE(of, ufc_match);

static struct platform_driver ufc_driver = {
	.driver = {
		.name	= "ufc",
		.of_match_table = ufc_match,
	},
	.probe = ufc_probe,
	.remove = __devexit_p(ufc_remove),
};

static int __init ufc_nand_init(void)
{
	return platform_driver_register(&ufc_driver);
}

static void __exit ufc_nand_exit(void)
{
	platform_driver_unregister(&ufc_driver);
}

module_init(ufc_nand_init);
module_exit(ufc_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manikandan Pillai <mpillai@apm.com>");
MODULE_DESCRIPTION("OF Platform driver for UFC");
