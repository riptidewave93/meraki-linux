/*
 * Micorn - MT29F SPI Nand Flash Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation. 
 * All rights reserved. Tanmay Inamdar <tinamdar@apm.com>
 *
 * Modified by Anup Patel <apatel@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/math64.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/blkdev.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/jffs2.h>

#define MICRON_MAX_PAGE_SIZE                    0x800
#define MICRON_OOB_SIZE				0x40
#define MICRON_MAX_TRY_COUNT                    1000

#define MICRON_CMD_BLOCK_ERASE                  0xD8
#define MICRON_CMD_GET_FEATURE                  0x0F /* Read Feature */
#define MICRON_CMD_PAGE_READ                    0x13 /* Page Read */
#define MICRON_CMD_PROGRAM_EXECUTE              0x10
#define MICRON_CMD_PROGRAM_LOAD                 0x02
#define MICRON_CMD_PROGRAM_LOAD_RANDOM_DATA     0x84
#define MICRON_CMD_READ_FROM_CACHE              0x03 /* Output cache data */
#define MICRON_CMD_READ_ID                      0x9F /* Read ID */
#define MICRON_CMD_RESET                        0xFF /* Reset Device */
#define MICRON_CMD_SET_FEATURE                  0x1F /* Write Feature */
#define MICRON_CMD_WRITE_DISABLE                0x04 /* Write Disable */
#define MICRON_CMD_WRITE_ENABLE                 0x06 /* Write Enable */

#define MICRON_FEATURE_BLOCKLOCK                0xA0
#define MICRON_FEATURE_OTP                      0xB0
#define MICRON_FEATURE_STATUS                   0xC0
#define MICRON_ECC_ENABLE			0x10

#define MICRON_VENDOR_ID			0x2c
#define MICRON_ID_MT29F1G01_1			0x11
#define MICRON_ID_MT29F1G01_2			0x12
#define MICRON_ID_MT29F1G01_3			0x13

#if defined(CONFIG_APM86xxx)
#define constant_cpu_to_je16(x) ((jint16_t){x})
#define constant_cpu_to_je32(x) ((jint32_t){x})
static const struct jffs2_unknown_node oob_cleanmarker =
{
        .magic = constant_cpu_to_je16(JFFS2_MAGIC_BITMASK),
        .nodetype = constant_cpu_to_je16(JFFS2_NODETYPE_CLEANMARKER),
        .totlen = constant_cpu_to_je32(8)
};
#endif

struct mt29f {
	struct spi_device	*spi;
	const struct flash_info	*params;
	struct mutex		lock;
	struct mtd_info		mtd;
	u8			tbuf[MICRON_MAX_PAGE_SIZE + MICRON_OOB_SIZE];
	unsigned		partitioned:1;
};

static inline struct mt29f *mtd_to_mt29f(struct mtd_info *mtd)
{
	return container_of(mtd, struct mt29f, mtd);
}

/**
  OOB area specification layout:  Total 32 available free bytes.
 */
static struct nand_ecclayout spinand_oob_64 = {
	.eccbytes = 24,
	.eccpos = {
		1, 2, 3, 4, 5, 6,
		17, 18, 19, 20, 21, 22,
		33, 34, 35, 36, 37, 38,
		49, 50, 51, 52, 53, 54, },
	.oobavail = 8,
	.oobfree = {
		{	.offset = 8,
			.length = 8},
		},
};

struct flash_info {
	char		*name;
	u8		idcode0;
	u8		idcode1;
	u64             nand_size;
	u64             usable_size;

	u32             block_size;
	u32             block_main_size;
	u16             block_num_per_chip;

	u16             page_size;
	u16             page_main_size;
	u16             page_spare_size;
	u16             page_num_per_block;

	u8              block_shift;
	u32             block_mask;

	u8              page_shift;
	u16             page_mask;

	struct nand_ecclayout *ecclayout;
};

/* NOTE: double check command sets and memory organization when you add
 * more flash chips.  This current list focusses on newer chips, which
 * have been converging on command sets which including JEDEC ID.
 */
static struct flash_info __devinitdata mt29f_data [] = {
	/* Micron */
	{ 	.name = "mt29f",
		.idcode0 = MICRON_VENDOR_ID,
		.idcode1 = MICRON_ID_MT29F1G01_1,
		.nand_size = 1024 * 64 * 2112,
		.usable_size = 1024 * 64 * 2048,
		.block_size = 64 * 2112,
		.block_main_size = 64 * 2048,
		.block_num_per_chip = 1024,
		.page_size = 2112,
		.page_main_size = 2048,
		.page_spare_size = 64,
		.page_num_per_block = 64,
		.block_shift = 17,
		.block_mask = 0x1ffff,
		.page_shift = 11,
		.page_mask = 0x7ff,
		.ecclayout = &spinand_oob_64,
	},
	{ 	.name = "mt29f",
		.idcode0 = MICRON_VENDOR_ID,
		.idcode1 = MICRON_ID_MT29F1G01_2,
		.nand_size = 1024 * 64 * 2112,
		.usable_size = 1024 * 64 * 2048,
		.block_size = 64 * 2112,
		.block_main_size = 64 * 2048,
		.block_num_per_chip = 1024,
		.page_size = 2112,
		.page_main_size = 2048,
		.page_spare_size = 64,
		.page_num_per_block = 64,
		.block_shift = 17,
		.block_mask = 0x1ffff,
		.page_shift = 11,
		.page_mask = 0x7ff,
		.ecclayout = &spinand_oob_64,
	},
	{ 	.name = "mt29f",
		.idcode0 = MICRON_VENDOR_ID,
		.idcode1 = MICRON_ID_MT29F1G01_3,
		.nand_size = 1024 * 64 * 2112,
		.usable_size = 1024 * 64 * 2048,
		.block_size = 64 * 2112,
		.block_main_size = 64 * 2048,
		.block_num_per_chip = 1024,
		.page_size = 2112,
		.page_main_size = 2048,
		.page_spare_size = 64,
		.page_num_per_block = 64,
		.block_shift = 17,
		.block_mask = 0x1ffff,
		.page_shift = 11,
		.page_mask = 0x7ff,
		.ecclayout = &spinand_oob_64,
	},
};

static int micron_spi_flash_cmd_read(struct spi_device *spi,
				const u8 *tx, unsigned n_tx,
				u8 *rx, unsigned n_rx)
{
	struct spi_message      message;
	struct spi_transfer     x[2];

	spi_message_init(&message);
	memset(x, 0, sizeof x);
	if (n_tx) {
		x[0].len = n_tx;
		x[0].tx_buf = tx;
		spi_message_add_tail(&x[0], &message);
	}
	if (n_rx) {
		x[1].len = n_rx;
		x[1].rx_buf = rx;
		spi_message_add_tail(&x[1], &message);
	}

	return spi_sync(spi, &message);
}
 
static int micron_spi_flash_cmd_write(struct spi_device *spi,
				const u8 *tx1, unsigned n_tx1,
				u8 *tx2, unsigned n_tx2)
{
	struct spi_message      message;
	struct spi_transfer     x[2];

	spi_message_init(&message);
	memset(x, 0, sizeof x);
	if (n_tx1) {
		x[0].len = n_tx1;
		x[0].tx_buf = tx1;
		spi_message_add_tail(&x[0], &message);
	}
	if (n_tx2) {
		x[1].len = n_tx2;
		x[1].tx_buf = tx2;
		spi_message_add_tail(&x[1], &message);
	}

	return spi_sync(spi, &message);
}
 
static int micron_reset(struct mt29f *flash)
{
        int ret;
	u8 cmd = MICRON_CMD_RESET;
        ret = spi_write(flash->spi, &cmd, 1);
        udelay(1000);
        return ret;
}

static int micron_get_features(struct mt29f *flash, u8 reg, u8 *val)
{
        u8 cmd[2];
        cmd[0] = MICRON_CMD_GET_FEATURE;
        cmd[1] = reg;
        return micron_spi_flash_cmd_read(flash->spi, cmd, 2, val, 1);
}

static int micron_set_features(struct mt29f *flash, u8 reg, u8 val)
{
        u8 cmd[3];
        cmd[0] = MICRON_CMD_SET_FEATURE;
        cmd[1] = reg;
        cmd[2] = val;
        return spi_write(flash->spi, cmd, 3);
}

static int micron_unlock_all(struct mt29f *flash)
{
        int ret;
        u8 stat;

        ret = micron_get_features(flash, MICRON_FEATURE_BLOCKLOCK, &stat);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        ret = micron_set_features(flash, MICRON_FEATURE_BLOCKLOCK, 0);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        return 0;
}

static int micron_wait_for_completion(struct mt29f *flash)
{
        int ret;
        u8 stat;
        u32 try;

        udelay(100);
        ret = micron_get_features(flash, MICRON_FEATURE_STATUS, &stat);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}
        try = 0;
        while ((stat & 0x01) && (try < MICRON_MAX_TRY_COUNT)) {
                ret = micron_get_features(flash, MICRON_FEATURE_STATUS, &stat);
                if (ret) {
			DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                        return ret;
		}
                if (stat & 0x2c) {
			DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed! Stat : 0x%02x\n",
					__func__, __LINE__,stat);
                        return -1;
		}
                udelay(100);
                try++;
        }
        if (try == MICRON_MAX_TRY_COUNT) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed\n",
				__func__, __LINE__);
                return -1;
	}

        return 0;
}

static int micron_write_enable(struct mt29f *flash)
{
	u8 cmd = MICRON_CMD_WRITE_ENABLE;
        return spi_write(flash->spi, &cmd, 1);
}

static int micron_read_page(struct mt29f *flash, u32 pnum, u16 page_offset,
				u8 *pbuf, u16 len)
{
        int ret;
        u8 cmd[4];

	DEBUG(MTD_DEBUG_LEVEL3, "%s: pnum : 0x%x ,pbuf : 0x%x, len : 0x%x\n", 
				__func__, pnum, (u32)pbuf, len);

        cmd[0] = MICRON_CMD_PAGE_READ;
        cmd[1] = 0x00;
        cmd[2] = pnum >> 8;
        cmd[3] = pnum & 0xFF;
        ret = spi_write(flash->spi, cmd, 4);
        if (ret) {
		printk(KERN_ERR "%s:%d spi_write failed with err %d\n",
				__func__,__LINE__,ret);
                return ret;
	}

        ret = micron_wait_for_completion(flash);
        if (ret) {
		printk(KERN_ERR "%s:%d micron_completion failed with err %d\n",
				__func__,__LINE__,ret);

                return ret;
	}

        cmd[0] = MICRON_CMD_READ_FROM_CACHE;
        cmd[1] = (u8)((page_offset & 0xFF00) >> 8);; /* Cache Offset MSB */
        cmd[2] = (u8)(page_offset & 0xFF); /* Cache Offset LSB */
        cmd[3] = 0x00;
        ret = micron_spi_flash_cmd_read(flash->spi, cmd, 4, pbuf, len);
        if (ret) {
		printk(KERN_ERR "%s:%d micron_spi_flash_cmd_read failed with err %d\n",
				__func__,__LINE__,ret);
                return ret;
	}

        return 0;
}

static int micron_write_page(struct mt29f *flash, u32 pnum, u16 page_offset,
				u8 *pbuf, u16 len)
{
        int ret;
        u8 cmd[4];

	DEBUG(MTD_DEBUG_LEVEL3, "%s: pnum : 0x%x, pbuf : 0x%x, len : 0x%x\n",
				__func__, pnum, (u32)pbuf, len);

        ret = micron_write_enable(flash);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        cmd[0] = MICRON_CMD_PROGRAM_LOAD;
        cmd[1] = (u8)((page_offset & 0xFF00) >> 8);
        cmd[2] = (u8)(page_offset & 0x00FF);
        ret = micron_spi_flash_cmd_write(flash->spi, cmd, 3, pbuf, len);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        cmd[0] = MICRON_CMD_PROGRAM_EXECUTE;
        cmd[1] = 0x00;
        cmd[2] = (pnum >> 8) & 0xFF;
        cmd[3] = pnum & 0xFF;
        ret = spi_write(flash->spi, cmd, 4);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        ret = micron_wait_for_completion(flash);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        return 0;
}

static int micron_erase_block(struct mt29f *flash, u32 blknum)
{
        int ret;
        u8 cmd[4];
        u32 psz, blksz;

        psz = flash->params->page_main_size;
        blksz = psz * flash->params->page_num_per_block;

	DEBUG(MTD_DEBUG_LEVEL3, "%s: blknum : 0x%x, blksz : 0x%x, psz : 0x%x\n",
		__func__, blknum, blksz, psz);

        ret = micron_write_enable(flash);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        cmd[0] = MICRON_CMD_BLOCK_ERASE;
        cmd[1] = 0x00;
        cmd[2] = (((blknum * blksz) / psz) >> 8) & 0xFF;
        cmd[3] = ((blknum * blksz) / psz) & 0xFF;
        ret = spi_write(flash->spi, cmd, 4);
        if (ret){
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        ret = micron_wait_for_completion(flash);
        if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
                return ret;
	}

        return 0;
}

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int micron_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u32 blksz, addr, len;
	size_t actual;
	int ret;
	u32 blkbits;

        DEBUG(MTD_DEBUG_LEVEL3, "%s: %s %s 0x%llx, len %lld\n",
              dev_name(&flash->spi->dev), __func__, "at",
              (long long)instr->addr, (long long)instr->len);

	/*
	 * This function currently uses block erase only.
	 * probably speed things up by using bulk erase
	 * when possible.
	 */
        if ((instr->addr + instr->len) > flash->mtd.size) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s:%d flash->mtd.size : 0x%llx\n",
				__func__, __LINE__, flash->mtd.size);
                return -EINVAL;
	}

	blksz = mtd->erasesize;
	blkbits = blksize_bits(blksz);	

	DEBUG(MTD_DEBUG_LEVEL3, "%s:%d blksz : %x , blkbits : %d\n",
				__func__, __LINE__, blksz, blkbits);
	if ((instr->addr & (blksz - 1)) || (instr->len & (blksz - 1))) {
		DEBUG(MTD_DEBUG_LEVEL0,
		"Erase offset/length not multiple of sector size\n");
		return -EINVAL;
	}

	addr = instr->addr;
	len = instr->len >> blkbits;

	mutex_lock(&flash->lock);

	ret = micron_unlock_all(flash);
	if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0, "Fail to unlock chip\n");
		instr->state = MTD_ERASE_FAILED;
		mutex_unlock(&flash->lock);
		return ret;
	}

	ret = 0;
	for (actual = 0; actual < len; actual++) {
		ret = micron_erase_block(flash,
				(addr >> blkbits) + actual);
		if (ret) {
			DEBUG(MTD_DEBUG_LEVEL0, "Erasing %d block failed\n",
					(addr >> blkbits) + actual);
			instr->state = MTD_ERASE_FAILED;
			mutex_unlock(&flash->lock);
			return ret;
		}
	}
	mutex_unlock(&flash->lock);

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);

	return ret;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
static int micron_read(struct mtd_info *mtd, loff_t offset, size_t len,
			size_t *retlen, u_char *buf)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	int ret = 0;
	u8 *dest = NULL;
	u32 ite=0, page_sz, buf_bytes, buf_off, dest_off=0;
	u32 pgbits = blksize_bits(flash->params->page_main_size);

        DEBUG(MTD_DEBUG_LEVEL3, "%s: %s %s offset 0x%llx, len %lld dest 0x%p\n",
              dev_name(&flash->spi->dev), __func__, "at",
              (long long)offset, (long long)len, buf);

	page_sz = flash->params->page_main_size;
	dest = buf;

	mutex_lock(&flash->lock);
	while(len) {
		if (ite == 0 && 0 < (offset & (page_sz - 1))) {
			buf_off = (offset & (page_sz - 1));
			buf_bytes = (len > (page_sz - (offset & (page_sz - 1)))) ?
				page_sz - (offset & (page_sz - 1)) : len;
		} else {
			buf_off = 0;
			buf_bytes = (len < page_sz) ? len : page_sz;
		}
		DEBUG(MTD_DEBUG_LEVEL3, "buf_off 0x%08x, buf_bytes %u\n",
							buf_off, buf_bytes);
		DEBUG(MTD_DEBUG_LEVEL3, "len %u, dest_off 0x%08x, ite %u\n", 
							len, dest_off, ite);
		if(buf_bytes % page_sz == 0) {
			if ((ret = micron_read_page(flash,
						(offset >> pgbits) + ite,0,
						&dest[dest_off], page_sz)))
				break;
		} else {
			if ((ret = micron_read_page(flash,
						(offset >> pgbits) + ite,0,
						flash->tbuf, page_sz)))
				break;
			memcpy(&dest[dest_off], &flash->tbuf[buf_off], buf_bytes);
		}
		len -= buf_bytes;
		dest_off += buf_bytes;
		ite += ((buf_off + buf_bytes) / page_sz);
	}
	*retlen = dest_off;
	mutex_unlock(&flash->lock);

	return ret;
}

/*
 * Write an address range to the flash chip.  Data must be written in
 * FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
static int micron_write(struct mtd_info *mtd, loff_t offset, size_t len,
			size_t *retlen, const u_char *buf)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u8 *src = NULL;
	int ret = 0;
	u32 ite = 0, page_sz, buf_bytes, buf_off, src_off = 0;
	u32 pgbits = blksize_bits(flash->params->page_main_size);

        DEBUG(MTD_DEBUG_LEVEL3, "%s: %s %s offset 0x%llx, len %lld dest 0x%p\n",
              dev_name(&flash->spi->dev), __func__, "at",
              (long long)offset, (long long)len, buf);

	page_sz = flash->params->page_main_size;
	src = (u8 *)buf;

	mutex_lock(&flash->lock);
	while(len) {
		if (ite == 0 && 0 < (offset & (page_sz -1 ))) {
			buf_off = (offset & (page_sz - 1));
			buf_bytes = (len > (page_sz - (offset & (page_sz - 1)))) ?
				page_sz - (offset & (page_sz - 1)) : len;
		} else {
			buf_off = 0;
			buf_bytes = (len < page_sz) ? len : page_sz;
		}
		if(buf_bytes % page_sz == 0) {
			if ((ret = micron_write_page(flash,
						(offset >> pgbits) + ite, 0,
						&src[src_off], page_sz)))
				break;
		} else {
			if ((ret = micron_read_page(flash,
						(offset >> pgbits) + ite, 0,
						flash->tbuf, page_sz)))
				break;
			memcpy(&flash->tbuf[buf_off], &src[src_off], buf_bytes);
			if ((ret = micron_write_page(flash,
						(offset >> pgbits) + ite, 0,
						flash->tbuf, page_sz)))
				break;
		}
		len -= buf_bytes;
		src_off += buf_bytes;
		ite += ((buf_off + buf_bytes) / page_sz);
		
	}
	*retlen = src_off;
	mutex_unlock(&flash->lock);

	return ret;
}

/* The commented OOB apis are theoroticaly correct way to implement OOB
 * management. However few issues are observed :-
 * 1. Partial(from offset != 0 in page) reads/writes in the pages fail. We
 *    have to read or program full page.
 * 2. if we program the erased page at the time of writing OOB, then further
 *    write opeations on that page fail with ECC error. Hence we cannot write
 *    OOB.
 */
#if !defined(CONFIG_APM86xxx)
static int micron_read_oob(struct mtd_info *mtd, loff_t from,
				struct mtd_oob_ops *ops)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u32 page_id = from >> flash->params->page_shift;
	u16 oob_area = flash->params->ecclayout->oobfree[0].offset;
	u16 oob_offset = flash->params->page_main_size + oob_area;
	u32 ret = 0;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx, len %d, ooblen %d, mode %d\n",
		__func__,(long long unsigned int) from, ops->len, ops->ooblen,
		ops->mode);
	DEBUG(MTD_DEBUG_LEVEL3, "%s: oob_offset : %d , page_id : %d\n",__func__,
		oob_offset, page_id);

	mutex_lock(&flash->lock);
	ret = micron_read_page(flash, page_id, 0,
				flash->tbuf, flash->params->page_size);
	mutex_unlock(&flash->lock);

	memcpy(ops->oobbuf, flash->tbuf + oob_offset, ops->ooblen);
#ifdef MICRON_DEBUG
	print_hex_dump(KERN_WARNING, "OOB_BUF:", DUMP_PREFIX_OFFSET, 16, 1,
			ops->oobbuf, ops->ooblen, 0);
#endif
	if(!ret)
		ops->oobretlen = ops->ooblen;

	return ret;
}

static int micron_write_oob(struct mtd_info *mtd, loff_t to,
				struct mtd_oob_ops *ops)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u32 page_id = to >> flash->params->page_shift;
	u16 oob_area = flash->params->ecclayout->oobfree[0].offset;
	u16 oob_offset = flash->params->page_main_size + oob_area;
	u32 ret = 0;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx, len %d, ooblen %d, mode %d\n",
		__func__,(long long unsigned int) to, ops->len, ops->ooblen,
		ops->mode);
	DEBUG(MTD_DEBUG_LEVEL3, "%s: oob_offset : %d , page_id : %d\n",__func__,
		oob_offset, page_id);

	memset(flash->tbuf, 0xFF, flash->params->page_size);
	memcpy(flash->tbuf + oob_offset, ops->oobbuf, ops->ooblen);
#ifdef MICRON_DEBUG
	print_hex_dump(KERN_WARNING, "OOB_BUF:", DUMP_PREFIX_OFFSET, 16, 1,
			flash->tbuf + oob_offset, ops->ooblen, 0);
#endif
	mutex_lock(&flash->lock);
	ret = micron_write_page(flash, page_id, 0, flash->tbuf,
				flash->params->page_size);
	mutex_unlock(&flash->lock);

	if(!ret)
		ops->oobretlen = ops->ooblen;

	return ret;
}

#else
/* XXX  : Extremely wrong method to implement OOB calls */
/* FIXME: Above mentioned issues with read and program  */
static int micron_read_oob(struct mtd_info *mtd, loff_t from,
				struct mtd_oob_ops *ops)
{
	u32 ret = 0;
	int i;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx, len %d, ooblen %d, mode %d\n",
		__func__,(long long unsigned int) from, ops->len, ops->ooblen,
		ops->mode);
	memcpy(ops->oobbuf,(void *)&oob_cleanmarker, ops->ooblen);
	if(ops->ooblen > 8) {
		for(i = 8; i < ops->ooblen; i++)
			ops->oobbuf[i] = 0xff;
	}
	ops->oobretlen = ops->ooblen;
	return ret;
}

static int micron_write_oob(struct mtd_info *mtd, loff_t to,
				struct mtd_oob_ops *ops)
{
	u32 ret = 0;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx, len %d, ooblen %d, mode %d\n",
		__func__,(long long unsigned int)to, ops->len, ops->ooblen,
		ops->mode);

	ops->oobretlen = ops->ooblen;
	return ret;
}
#endif

static int micron_block_markbad(struct mtd_info *mtd, loff_t offset)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u16 bb_offset = flash->params->page_main_size;
        u16 page_id = offset >> flash->params->page_shift;
        u8 ret = 0;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx\n",
		__func__, (long long unsigned int)offset);

	memset(flash->tbuf, 0xFF, flash->params->page_size);
	flash->tbuf[bb_offset] = 0x00;
#ifdef MICRON_DEBUG
	print_hex_dump(KERN_WARNING, "OOB_BUF:", DUMP_PREFIX_OFFSET, 16, 1,
			flash->tbuf + oob_offset, ops->ooblen, 0);
#endif
	mutex_lock(&flash->lock);
	ret = micron_write_page(flash, page_id, 0, flash->tbuf,
				flash->params->page_size);
	mutex_unlock(&flash->lock);

	return ret;
}

static int micron_block_isbad(struct mtd_info *mtd, loff_t offset)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
        u16 page_id = offset >> flash->params->page_shift;
        u16 block_id = offset >> flash->params->block_shift;
        u16 bb_offset = flash->params->page_main_size;
        u8 ret = 0;

	DEBUG(MTD_DEBUG_LEVEL3,"%s: offset 0x%llx\n",
		__func__, (long long unsigned int)offset);

	page_id = offset >> flash->params->page_shift;
	mutex_lock(&flash->lock);
	ret = micron_read_page(flash, page_id, 0, flash->tbuf,
				flash->params->page_size);
	mutex_unlock(&flash->lock);

	if(flash->tbuf[bb_offset] != 0xFF) {
		printk(KERN_DEBUG "%s: Bad Block Found at block %u\n",
						__func__, block_id);	
		ret = 1;
	}

	return ret;
}

/**
 * micron_sync - [MTD Interface] sync
 * <at> param mtd          MTD device structure
 *
 * Sync is actually a wait for chip ready function
 */
static void micron_sync(struct mtd_info *mtd)
{
	struct mt29f *flash = mtd_to_mt29f(mtd);
	u32 ret = 0;

        DEBUG(MTD_DEBUG_LEVEL3, "%s: called\n",__func__);

	mutex_lock(&flash->lock);
	ret = micron_wait_for_completion(flash);
	if (ret) {
		DEBUG(MTD_DEBUG_LEVEL0,"%s:%d failed with error %d\n",
				__func__, __LINE__, ret);
	}
	mutex_unlock(&flash->lock);


}

static struct flash_info *__devinit mt29f_read_id(struct spi_device *spi)
{
	int			tmp;
	u8			code[2];
	u8			id[5];
	u8			vendor_id;
	u8	                chip_id;
	struct flash_info	*info;

	/* JEDEC also defines an optional "extended device information"
	 * string for after vendor-specific data, after the three bytes
	 * we use here.  Supporting some chips might require using it.
	 */
	code[0] = MICRON_CMD_READ_ID;
	code[1] = 0;
	tmp = spi_write_then_read(spi, code, sizeof(code), id, sizeof(id));
	if (tmp < 0) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: error %d reading JEDEC ID\n",
			dev_name(&spi->dev), tmp);
		return NULL;
	}
	DEBUG(MTD_DEBUG_LEVEL0, "%s: %x %x %x %x %x\n", __func__, 
				id[0], id[1], id[2], id[3], id[4]);
	vendor_id = id[0];
	chip_id = id[1];

	DEBUG(MTD_DEBUG_LEVEL0, "MT29F VENDOR ID: %02x ", vendor_id);
	DEBUG(MTD_DEBUG_LEVEL0, "MT29F CHIP ID: %02x\n", chip_id);
	for (tmp = 0, info = mt29f_data;
			tmp < ARRAY_SIZE(mt29f_data);
			tmp++, info++) {
		if (info->idcode0 == vendor_id) {
			if (info->idcode1 != 0 && info->idcode1 != chip_id)
				continue;
			return info;
		}
	}
	dev_err(&spi->dev, "unrecognized id %x%x\n", vendor_id,chip_id);
	return NULL;
}

static void enable_hw_ecc(struct mt29f *flash)
{
        u8 stat;

        micron_get_features(flash, MICRON_FEATURE_OTP, &stat);

	if(!(stat & MICRON_ECC_ENABLE)) {
	        micron_set_features(flash, MICRON_FEATURE_OTP,
					(stat | MICRON_ECC_ENABLE));
	}	
}

/*
 * board specific setup should have ensured the SPI clock used here
 * matches what the READ command supports, at least until this driver
 * understands FAST_READ (for clocks over 25 MHz).
 */
static int __devinit mt29f_probe(struct spi_device *spi)
{
	struct flash_platform_data *data;
	struct mt29f		*flash;
	struct flash_info	*info;
	unsigned		i;

	/* Platform data helps sort out which chip type we have, as
	 * well as how this board partitions it.  If we don't have
	 * a chip ID, try the JEDEC id commands; they'll work for most
	 * newer chips, even if we don't recognize the particular chip.
	 */
	data = spi->dev.platform_data;
	if (data && data->type) {
		for (i = 0, info = mt29f_data;
				i < ARRAY_SIZE(mt29f_data);
				i++, info++) {
			if (strcmp(data->type, info->name) == 0)
				break;
		}

		/* unrecognized chip? */
		if (i == ARRAY_SIZE(mt29f_data)) {
			DEBUG(MTD_DEBUG_LEVEL0, "%s: unrecognized id %s\n",
					dev_name(&spi->dev), data->type);
			info = NULL;

		/* recognized; is that chip really what's there? */
		} else if (info->idcode0) {
			struct flash_info *chip = mt29f_read_id(spi);

			if (!chip || chip != info) {
				dev_warn(&spi->dev, "found %s, expected %s\n",
						chip ? chip->name : "UNKNOWN",
						info->name);
				info = NULL;
			}
		}
	} else {
		info = mt29f_read_id(spi);
	}

	if (!info)
		return -ENODEV;

	flash = kzalloc(sizeof *flash, GFP_KERNEL);
	if (!flash)
		return -ENOMEM;

	flash->spi = spi;
	flash->params = info;
	mutex_init(&flash->lock);
	dev_set_drvdata(&spi->dev, flash);

	if (data && data->name)
		flash->mtd.name = data->name;
	else
		flash->mtd.name = dev_name(&spi->dev);

        flash->mtd.owner = THIS_MODULE;
	flash->mtd.type = MTD_NANDFLASH;
	flash->mtd.flags = MTD_CAP_NANDFLASH;
	flash->mtd.size = info->usable_size;
	flash->mtd.writesize = info->page_main_size;
	flash->mtd.erasesize = info->block_main_size;
	flash->mtd.oobsize = info->ecclayout->oobavail;
	flash->mtd.ecclayout = info->ecclayout;

	flash->mtd.erase = micron_erase;
	flash->mtd.point = NULL;
	flash->mtd.unpoint = NULL;
	flash->mtd.read = micron_read;
	flash->mtd.write = micron_write;
        flash->mtd.sync = micron_sync;
        flash->mtd.lock = NULL;
        flash->mtd.unlock = NULL;
	flash->mtd.read_oob = micron_read_oob;
	flash->mtd.write_oob = micron_write_oob;
	flash->mtd.block_isbad = micron_block_isbad;
	flash->mtd.block_markbad = micron_block_markbad;
	flash->mtd.block_markbad = NULL;

	flash->mtd.dev.parent = &spi->dev;

	dev_info(&spi->dev, "%s (%lld Kbytes)\n", info->name,
			(long long)flash->mtd.size >> 10);

	micron_reset(flash);
	enable_hw_ecc(flash);

	DEBUG(MTD_DEBUG_LEVEL3,
		"mtd .name = %s, .size = 0x%llx (%lldMiB) "
			".erasesize = 0x%.8x (%uKiB) .numeraseregions = %d\n",
		flash->mtd.name,
		(long long)flash->mtd.size, (long long)(flash->mtd.size >> 20),
		flash->mtd.erasesize, flash->mtd.erasesize / 1024,
		flash->mtd.numeraseregions);

	if (flash->mtd.numeraseregions) {
		for (i = 0; i < flash->mtd.numeraseregions; i++) {
			DEBUG(MTD_DEBUG_LEVEL3,
				"mtd.eraseregions[%d] = { .offset = 0x%llx, "
				".erasesize = 0x%.8x (%uKiB), "
				".numblocks = %d }\n",
				i, (long long)flash->mtd.eraseregions[i].offset,
				flash->mtd.eraseregions[i].erasesize,
				flash->mtd.eraseregions[i].erasesize / 1024,
				flash->mtd.eraseregions[i].numblocks);
		}
	}

	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */
	if (mtd_has_partitions()) {
		struct mtd_partition	*parts = NULL;
		int			nr_parts = 0;

		if (mtd_has_cmdlinepart()) {
			static const char *part_probes[]
					= { "cmdlinepart", NULL, };

			nr_parts = parse_mtd_partitions(&flash->mtd,
					part_probes, &parts, 0);
		}

#ifdef CONFIG_MTD_OF_PARTS
		if (nr_parts == 0) {
			nr_parts = of_mtd_parse_partitions(&spi->dev,
					spi->dev.archdata.of_node, &parts);
		}
#endif

		if (nr_parts <= 0 && data && data->parts) {
			parts = data->parts;
			nr_parts = data->nr_parts;
		}

		if (nr_parts > 0) {
			for (i = 0; i < nr_parts; i++) {
				DEBUG(MTD_DEBUG_LEVEL3, "partitions[%d] = "
					"{.name = %s, .offset = 0x%llx, "
						".size = 0x%llx (%lldKiB) }\n",
					i, parts[i].name,
					(long long)parts[i].offset,
					(long long)parts[i].size,
					(long long)(parts[i].size >> 10));
			}
			flash->partitioned = 1;
			return add_mtd_partitions(&flash->mtd, parts, nr_parts);
		}
	} else if (data && data->nr_parts)
		dev_warn(&spi->dev, "ignoring %d default partitions on %s\n",
				data->nr_parts, data->name);


	return add_mtd_device(&flash->mtd) == 1 ? -ENODEV : 0;
}

static int __devexit mt29f_remove(struct spi_device *spi)
{
	struct mt29f *flash = dev_get_drvdata(&spi->dev);
	int status;

	/* Clean up MTD stuff. */
	if (mtd_has_partitions() && flash->partitioned)
		status = del_mtd_partitions(&flash->mtd);
	else
		status = del_mtd_device(&flash->mtd);
	if (status == 0)
		kfree(flash);
	return 0;
}

static struct spi_driver mt29f_driver = {
	.driver = {
		.name	= "mt29f",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe	= mt29f_probe,
	.remove	= __devexit_p(mt29f_remove),

	/* TODO: many of these chips have deep power-down modes, which
	 * should clearly be entered on suspend() to minimize power use.
	 * And also when they're otherwise idle...
	 */
};

static int __init mt29f_init(void)
{
	return spi_register_driver(&mt29f_driver);
}

static void __exit mt29f_exit(void)
{
	spi_unregister_driver(&mt29f_driver);
}

module_init(mt29f_init);
module_exit(mt29f_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay Inamdar");
MODULE_DESCRIPTION("MTD driver for Micron MT29F SPI Nand flash");
