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

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/nand.h>
#include <linux/spi/flash.h>

#define DRV_NAME	"ath79-nand"
#define DRV_VERSION	"0.1"
#define DRV_AUTHOR	"Meraki"
#define DRV_DESC	"Atheros on-chip NAND Flash Controller Driver"

#define TIMEOUT_LOOP(condition, name, ...) do {   \
		int _timeout = 0;		  \
		while (_timeout++ < 1000 && (condition))	\
			udelay(1);				\
		if (_timeout >= 1000)					\
			printk(DRV_NAME ": " name ": TIMEOUT EXCEEDED!!\n" __VA_ARGS__); \
	} while (0)

struct ath79_nand {
	void __iomem *controller;
	uint32_t fifo_buf;
	uint32_t fifo_buf_count;

	struct mtd_info mtd;
	struct mtd_partition *mtd_parts;
	struct nand_chip chip;
};

static int ath79_nand_chip_ready(struct mtd_info *mtd)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);

	return __raw_readl(ath79_nand->controller + ATH79_NF_STATUS) == 0xff;
}

static void ath79_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	switch (chipnr) {
        case -1:
        case 0:
		break;
        default:
		BUG();
	}
}

static uint8_t ath79_nand_read_byte_swap(struct mtd_info *mtd)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);

	if (ath79_nand->fifo_buf_count) {
		ath79_nand->fifo_buf >>= 8;
		ath79_nand->fifo_buf_count--;
	} else {
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_STATUS) != 1, "read byte swap");
		ath79_nand->fifo_buf_count = sizeof(uint32_t) - 1;
		ath79_nand->fifo_buf = __raw_readl(ath79_nand->controller + ATH79_NF_FIFO_DATA);
	}
	return (uint8_t)((ath79_nand->fifo_buf) & 0xff);
}

static uint8_t ath79_nand_read_byte(struct mtd_info *mtd)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);

	if (ath79_nand->fifo_buf_count) {
		ath79_nand->fifo_buf <<= 8;
		ath79_nand->fifo_buf_count--;
	} else {
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_STATUS) != 1, "read byte");
		ath79_nand->fifo_buf_count = sizeof(uint32_t) - 1;
		ath79_nand->fifo_buf = __raw_readl(ath79_nand->controller + ATH79_NF_FIFO_DATA);
	}

	return (uint8_t)((ath79_nand->fifo_buf >> 24) & 0xff);
}

static void ath79_nand_read_buf_swap(struct mtd_info *mtd, uint8_t *buf, int left)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);
	uint32_t *buf32 = (uint32_t*)buf;

	//read the bulk of the data
	for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_STATUS) != 1, "read buf swap");
#ifdef __LITTLE_ENDIAN
		*(buf32++) = be32_to_cpu(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_DATA));
#elif __BIG_ENDIAN
		*(buf32++) = le32_to_cpu(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_DATA));
#else
#error Unknown endianness
#endif
	}

	//any stray bytes at the end
	for (buf = (uint8_t*)buf32; left > 0; left -=1)
		*(buf++) = ath79_nand_read_byte_swap(mtd);
}

static void ath79_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int left)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);
	uint32_t *buf32 = (uint32_t*)buf;

	//read the bulk of the data
	for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_STATUS) == 0, "read buf");
		*(buf32++) = __raw_readl(ath79_nand->controller + ATH79_NF_FIFO_DATA);
	}

	//any stray bytes at the end
	for (buf = (uint8_t*)buf32; left > 0; left -=1)
		*(buf++) = ath79_nand_read_byte(mtd);
}

static void ath79_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);
	uint32_t *buf32 = (uint32_t*)buf;
	int left = len;

	//write the bulk of the data
	for (buf32 = (uint32_t*)buf; left > 3; left -=4) {
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_FIFO_STATUS) == 0, "write buf");
		__raw_writel(*(buf32++), ath79_nand->controller + ATH79_NF_FIFO_DATA);
	}
	BUG_ON(left > 0);
}

static void ath79_nand_cmd_func_lp(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
	struct ath79_nand *ath79_nand = container_of(mtd, struct ath79_nand, mtd);
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
	__raw_writel(addr0, ath79_nand->controller + ATH79_NF_ADDR0_0);
	__raw_writel(addr1, ath79_nand->controller + ATH79_NF_ADDR0_1);


	__raw_writel(1, ath79_nand->controller + ATH79_NF_FIFO_INIT);
	__raw_writel(0, ath79_nand->controller + ATH79_NF_FIFO_INIT);
	ath79_nand->fifo_buf_count = 0; //clear fifo cache
	chip->read_byte = ath79_nand_read_byte_swap;
	chip->read_buf = ath79_nand_read_buf_swap;
	TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_STATUS) & ATH79_NF_CTRL_STAT, "cmd_func fifo init");

	switch(command) {
        case NAND_CMD_RESET:
		__raw_writel((command << 8) | ATH79_CMD_SEQ_0, ath79_nand->controller + ATH79_NF_COMMAND);
		break;
        case NAND_CMD_READID:
		__raw_writel(8, ath79_nand->controller + ATH79_NF_PG_SIZE); // 8 byte nand id
		__raw_writel((command << 8) | ATH79_CMD_SEQ_1,
			     ath79_nand->controller + ATH79_NF_COMMAND);
		break;
        case NAND_CMD_PARAM:
		__raw_writel(sizeof(struct nand_onfi_params) * 3, ath79_nand->controller + ATH79_NF_PG_SIZE); //is repeated three times
		__raw_writel((command << 8) | ATH79_CMD_SEQ_2, ath79_nand->controller + ATH79_NF_COMMAND);
            break;
        case NAND_CMD_READ0:
		__raw_writel(pg_size, ath79_nand->controller + ATH79_NF_PG_SIZE);
		__raw_writel((command << 8) | (NAND_CMD_READSTART << 16) | ATH79_CMD_SEQ_10, ath79_nand->controller + ATH79_NF_COMMAND);
		chip->read_byte = ath79_nand_read_byte;
		chip->read_buf = ath79_nand_read_buf;
            break;
        case NAND_CMD_STATUS:
		__raw_writel(4, ath79_nand->controller + ATH79_NF_PG_SIZE);
		__raw_writel((command << 8) | ATH79_CMD_SEQ_4, ath79_nand->controller + ATH79_NF_COMMAND);
		TIMEOUT_LOOP(__raw_readl(ath79_nand->controller + ATH79_NF_STATUS) & ATH79_NF_CTRL_STAT, "status cmd");
		{
			/*
			 * the controller puts status output in a different register, so copy
			 * into the fifo buffer for use by read_byte
			 */
			uint32_t tmp = __raw_readl(ath79_nand->controller + ATH79_NF_RD_STATUS);
			ath79_nand->fifo_buf = tmp << 8;
			ath79_nand->fifo_buf_count = 3;
		}
		break;
        case NAND_CMD_ERASE1:
		__raw_writel(addr0 & ~(0x3fffff), ath79_nand->controller + ATH79_NF_ADDR0_0); //make sure it's a block address
		__raw_writel((command << 8 ) | (NAND_CMD_ERASE2 << 16) | ATH79_CMD_SEQ_14, ath79_nand->controller + ATH79_NF_COMMAND);
		break;
        case NAND_CMD_ERASE2:
		//handled by ERASE1
		break;
        case NAND_CMD_SEQIN:
		__raw_writel(pg_size, ath79_nand->controller + ATH79_NF_PG_SIZE);
		__raw_writel((command << 8 ) | (NAND_CMD_PAGEPROG << 16) | ATH79_CMD_SEQ_12, ath79_nand->controller + ATH79_NF_COMMAND);
		break;
        case NAND_CMD_PAGEPROG:
		//handled by SEQIN
		break;
        default:
		printk(DRV_NAME ": unknown nand command 0x%x\n", command);
		return;
	}
}

static int __init ath79_nand_probe(struct platform_device *pdev)
{
	static const char *part_types[] = { "cmdlinepart", NULL };
	struct flash_platform_data *plat = pdev->dev.platform_data;

	int err = 0;
	struct nand_chip *chip;
	struct mtd_info  *mtd;
	struct ath79_nand *ath79_nand;
	int nr_parts = 0;
	struct mtd_partition *mtd_parts=NULL;

	if (!soc_is_ar934x() && !soc_is_qca955x())
		return -ENODEV;

	ath79_nand = kzalloc(sizeof(struct ath79_nand), GFP_KERNEL);

	if (!ath79_nand)
		return -ENOMEM;

	ath79_nand->controller = (void __iomem *)
		KSEG1ADDR((soc_is_ar934x() ?
			   AR934X_NAND_BASE :
			   QCA955X_NAND_BASE));

	ath79_device_reset_set(AR934X_RESET_NAND);
	udelay(100);
	ath79_device_reset_clear(AR934X_RESET_NAND);
	udelay(100);

	mtd = &ath79_nand->mtd;
	chip = &ath79_nand->chip;

	mtd->owner = THIS_MODULE;
	mtd->priv = chip;
	mtd->dev.parent = &pdev->dev;
	mtd->name = DRV_NAME;

	chip->priv = ath79_nand;
	chip->dev_ready = ath79_nand_chip_ready;
	chip->cmdfunc = ath79_nand_cmd_func_lp;
	chip->read_byte = ath79_nand_read_byte_swap;
	chip->read_buf = ath79_nand_read_buf_swap;
	chip->write_buf = ath79_nand_write_buf;
	chip->select_chip = ath79_nand_select_chip;
	chip->ecc.mode = NAND_ECC_SOFT_BCH;

	//disable all write protection lines
	__raw_writel(0xff00, ath79_nand->controller + ATH79_NF_MEM_CTRL);

	if (nand_scan(mtd, 1)) {
		err = -ENXIO;
		goto fail;
	}

	/* Use MTD partition table specified in platform data if available */
	if (plat) {
		nr_parts  = plat->nr_parts;
		mtd_parts = plat->parts;
	}

	err = mtd_device_parse_register(mtd, part_types, NULL,
				    mtd_parts, nr_parts);
	if (err < 0)
		goto fail_out;

	platform_set_drvdata(pdev, ath79_nand);

	return err;

fail:
	err = -EIO;
fail_out:
	kfree(ath79_nand);
	return err;

}

static int __exit ath79_nand_remove(struct platform_device *pdev)
{
	struct ath79_nand *ath79_nand = platform_get_drvdata(pdev);

	nand_release(&ath79_nand->mtd);
	kfree(ath79_nand);

	return 0;
}

static struct platform_driver ath79_nand_driver = {
	.remove = __devexit_p(ath79_nand_remove),
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
	},
};


static int __init ath79_nand_init(void)
{
	return platform_driver_probe(&ath79_nand_driver, ath79_nand_probe);
}

static void __exit ath79_nand_exit(void)
{
	platform_driver_unregister(&ath79_nand_driver);
}

module_init(ath79_nand_init);
module_exit(ath79_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRV_AUTHOR);
MODULE_DESCRIPTION(DRV_DESC);
MODULE_ALIAS("platform:" DRV_NAME);
