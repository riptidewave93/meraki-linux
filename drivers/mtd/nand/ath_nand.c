/*
 * NAND Chip driver for Atheros on-chip NAND Flash Controller
 *
 * Copyright (C) 2012 Meraki Inc <jdizzle@meraki.net>
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <atheros.h>

#define DRV_NAME	"ath-nand"
#define DRV_VERSION	"0.1"
#define DRV_AUTHOR	"Meraki"
#define DRV_DESC	"Atheros on-chip NAND Flash Controller Driver"

#define TIMEOUT_LOOP(condition, name, ...) do {   \
    int _timeout = 0;                             \
    while (_timeout++ < 1000 && (condition))      \
        udelay(1);                                \
    if (_timeout >= 1000)                         \
        printk(DRV_NAME ": " name ": TIMEOUT EXCEEDED!!\n" __VA_ARGS__);    \
    } while (0)

struct ath_nand {
    uint32_t fifo_buf;
    uint32_t fifo_buf_count;

    struct mtd_info mtd;
    struct mtd_partition *mtd_parts;
    struct nand_chip chip;
};


static int ath_nand_chip_ready(struct mtd_info *mtd)
{
    return ath_reg_rd(ATH_NF_STATUS) == 0xff;
}

static void ath_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
    switch (chipnr) {
        case -1:
        case 0:
            break;
        default:
            BUG();
    }
}

static uint8_t ath_nand_read_byte_swap(struct mtd_info *mtd)
{
    struct ath_nand *ath_nand = container_of(mtd, struct ath_nand, mtd);

    if (ath_nand->fifo_buf_count) {
        ath_nand->fifo_buf >>= 8;
        ath_nand->fifo_buf_count--;
    } else {
        TIMEOUT_LOOP(ath_reg_rd(ATH_NF_FIFO_STATUS) != 1, "read byte swap");
        ath_nand->fifo_buf_count = sizeof(uint32_t) - 1;
        ath_nand->fifo_buf = ath_reg_rd(ATH_NF_FIFO_DATA);
    }

    return (uint8_t)((ath_nand->fifo_buf) & 0xff);
}

static uint8_t ath_nand_read_byte(struct mtd_info *mtd)
{
    struct ath_nand *ath_nand = container_of(mtd, struct ath_nand, mtd);

    if (ath_nand->fifo_buf_count) {
        ath_nand->fifo_buf <<= 8;
        ath_nand->fifo_buf_count--;
    } else {
        TIMEOUT_LOOP(ath_reg_rd(ATH_NF_FIFO_STATUS) != 1, "read byte");
        ath_nand->fifo_buf_count = sizeof(uint32_t) - 1;
        ath_nand->fifo_buf = ath_reg_rd(ATH_NF_FIFO_DATA);
    }

    return (uint8_t)((ath_nand->fifo_buf >> 24) & 0xff);
}

static void ath_nand_read_buf_swap(struct mtd_info *mtd, uint8_t *buf, int left)
{
    uint32_t *buf32 = (uint32_t*)buf;

    //read the bulk of the data
    for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
        TIMEOUT_LOOP(ath_reg_rd(ATH_NF_FIFO_STATUS) != 1, "read buf swap");
#ifdef __LITTLE_ENDIAN
        *(buf32++) = be32_to_cpu(ath_reg_rd(ATH_NF_FIFO_DATA));
#elif __BIG_ENDIAN
        *(buf32++) = le32_to_cpu(ath_reg_rd(ATH_NF_FIFO_DATA));
#else
#error Unknown endianness
#endif
    }

    //any stray bytes at the end
    for (buf = (uint8_t*)buf32; left > 0; left -=1)
        *(buf++) = ath_nand_read_byte_swap(mtd);
}

static void ath_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int left)
{
    uint32_t *buf32 = (uint32_t*)buf;

    //read the bulk of the data
    for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
        TIMEOUT_LOOP(ath_reg_rd(ATH_NF_FIFO_STATUS) == 0, "read buf");
        *(buf32++) = ath_reg_rd(ATH_NF_FIFO_DATA);
    }

    //any stray bytes at the end
    for (buf = (uint8_t*)buf32; left > 0; left -=1)
        *(buf++) = ath_nand_read_byte(mtd);
}

static void ath_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
    uint32_t *buf32 = (uint32_t*)buf;
    int left = len;

    //write the bulk of the data
    for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
        TIMEOUT_LOOP(ath_reg_rd(ATH_NF_FIFO_STATUS) == 0, "write buf");
        ath_reg_wr(ATH_NF_FIFO_DATA, *(buf32++));
    }

    BUG_ON(left > 0);
}

static void ath_nand_cmd_func_lp(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
    struct ath_nand *ath_nand = container_of(mtd, struct ath_nand, mtd);
    struct nand_chip *chip = (struct nand_chip *)mtd->priv;
    uint32_t addr0, addr1;
    uint32_t pg_size = mtd->writesize + mtd->oobsize;

    column = column == -1 ? 0 : column;
    page_addr = page_addr == -1 ? 0 : page_addr;

    if (command == NAND_CMD_READOOB) {
        column += mtd->writesize;
        command = NAND_CMD_READ0;
        pg_size = mtd->oobsize;
    } else {
        //subtract the offset of the page read/write
        pg_size -= column;
    }

    addr0 = column | (page_addr << 16);
    addr1 = page_addr << 24;
    ath_reg_wr(ATH_NF_ADDR0_0, addr0);
    ath_reg_wr(ATH_NF_ADDR0_1, addr1);

    ath_reg_wr(ATH_NF_FIFO_INIT, 1);
    ath_nand->fifo_buf_count = 0; //clear fifo cache
    chip->read_byte = ath_nand_read_byte_swap;
    chip->read_buf = ath_nand_read_buf_swap;
    TIMEOUT_LOOP(ath_reg_rd(ATH_NF_STATUS) & ATH_NF_CTRL_STAT, "cmd_func fifo init");


    switch(command) {
        case NAND_CMD_RESET:
            ath_reg_wr(ATH_NF_COMMAND, (command << 8) | ATH_CMD_SEQ_0);
            break;
        case NAND_CMD_READID:
            ath_reg_wr(ATH_NF_PG_SIZE, 8); // 8 byte nand id
            ath_reg_wr(ATH_NF_COMMAND, (command << 8) | ATH_CMD_SEQ_1);
            break;
        case NAND_CMD_PARAM:
            ath_reg_wr(ATH_NF_PG_SIZE, sizeof(struct nand_onfi_params) * 3); //is repeated three times
            ath_reg_wr(ATH_NF_COMMAND, (command << 8) | ATH_CMD_SEQ_2);
            break;
        case NAND_CMD_READ0:
            ath_reg_wr(ATH_NF_PG_SIZE, pg_size);
            ath_reg_wr(ATH_NF_COMMAND, (command << 8) | (NAND_CMD_READSTART << 16) | ATH_CMD_SEQ_10);
            chip->read_byte = ath_nand_read_byte;
            chip->read_buf = ath_nand_read_buf;
            break;
        case NAND_CMD_STATUS:
            ath_reg_wr(ATH_NF_PG_SIZE, 4);
            ath_reg_wr(ATH_NF_COMMAND, (command << 8) | ATH_CMD_SEQ_4);
            TIMEOUT_LOOP(ath_reg_rd(ATH_NF_STATUS) & ATH_NF_CTRL_STAT, "status cmd");
            {
                /*
                 * the controller puts status output in a different register, so copy
                 * into the fifo buffer for use by read_byte
                 */
                uint32_t tmp = ath_reg_rd(ATH_NF_RD_STATUS);
                ath_nand->fifo_buf = tmp << 8;
                ath_nand->fifo_buf_count = 3;
            }
            break;
        case NAND_CMD_ERASE1:
            ath_reg_wr(ATH_NF_ADDR0_0, addr0 & ~(0x3fffff)); //make sure it's a block address
            ath_reg_wr(ATH_NF_COMMAND, (command << 8 ) | (NAND_CMD_ERASE2 << 16) | ATH_CMD_SEQ_14);
            break;
        case NAND_CMD_ERASE2:
            //handled by ERASE1
            break;
        case NAND_CMD_SEQIN:
            ath_reg_wr(ATH_NF_PG_SIZE, pg_size);
            ath_reg_wr(ATH_NF_COMMAND, (command << 8 ) | (NAND_CMD_PAGEPROG << 16) | ATH_CMD_SEQ_12);
            break;
        case NAND_CMD_PAGEPROG:
            //handled by SEQIN
            break;
        default:
            printk(DRV_NAME ": unknown nand command 0x%x\n", command);
            return;
    }
}

static int __init ath_nand_probe(struct platform_device *pdev)
{
    static const char *part_types[] = { "cmdlinepart", NULL };

    int err, num_parts;
    struct nand_chip *chip;
    struct mtd_info  *mtd;
    struct ath_nand *ath_nand;

    ath_nand = kzalloc(sizeof(struct ath_nand), GFP_KERNEL);

    if (!ath_nand)
        return -ENOMEM;

    ath_reg_rmw_set(ATH_RESET, RST_RESET_NANDF_RESET_MASK);
    udelay(100);
    ath_reg_rmw_clear(ATH_RESET, RST_RESET_NANDF_RESET_MASK);
    udelay(100);

    mtd = &ath_nand->mtd;
    chip = &ath_nand->chip;

    mtd->owner = THIS_MODULE;
    mtd->priv = chip;
    mtd->dev.parent = &pdev->dev;
    mtd->name = DRV_NAME;

    chip->priv = ath_nand;
    chip->dev_ready = ath_nand_chip_ready;
    chip->cmdfunc = ath_nand_cmd_func_lp;
    chip->read_byte = ath_nand_read_byte_swap;
    chip->read_buf = ath_nand_read_buf_swap;
    chip->write_buf = ath_nand_write_buf;
    chip->select_chip = ath_nand_select_chip;
    chip->ecc.mode = NAND_ECC_SOFT_BCH;

    //disable all write protection lines
    ath_reg_wr(ATH_NF_MEM_CTRL, 0xff00);

    if (nand_scan(mtd, 1)) {
        err = -ENXIO;
        goto fail;
    }

    num_parts = parse_mtd_partitions(mtd, part_types, &ath_nand->mtd_parts, 0);
    if (num_parts < 0)
        goto fail;

    if (mtd_device_register(mtd, ath_nand->mtd_parts, num_parts) < 0)
        goto fail;

    platform_set_drvdata(pdev, ath_nand);

    return 0;

 fail:
    kfree(ath_nand);
    return -1;

}

static int __exit ath_nand_remove(struct platform_device *pdev)
{
    struct ath_nand *ath_nand = platform_get_drvdata(pdev);

    nand_release(&ath_nand->mtd);
    kfree(ath_nand);

    return 0;
}

static struct platform_driver ath_nand_driver = {
    .probe  = ath_nand_probe,
    .remove = ath_nand_remove,
    .driver = {
        .name = DRV_NAME,
        .owner = THIS_MODULE,
    },
};


static int __init ath_nand_init(void)
{
    return platform_driver_register(&ath_nand_driver);
}

static void __exit ath_nand_exit(void)
{
    platform_driver_unregister(&ath_nand_driver);
}

module_init(ath_nand_init);
module_exit(ath_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRV_AUTHOR);
MODULE_DESCRIPTION(DRV_DESC);
MODULE_ALIAS("platform:" DRV_NAME);
