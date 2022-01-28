/*
 * B53 register access through Switch Register Access Bridge Registers
 *
 * Copyright (C) 2013 Hauke Mehrtens <hauke@hauke-m.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <asm/io.h>

/* command and status register of the SRAB */
#define B53_SRAB_CMDSTAT		0x2c
#define  B53_SRAB_CMDSTAT_RST		BIT(2)
#define  B53_SRAB_CMDSTAT_WRITE		BIT(1)
#define  B53_SRAB_CMDSTAT_GORDYN	BIT(0)
#define  B53_SRAB_CMDSTAT_PAGE		24
#define  B53_SRAB_CMDSTAT_REG		16

/* high order word of write data to switch registe */
#define B53_SRAB_WD_H			0x30

/* low order word of write data to switch registe */
#define B53_SRAB_WD_L			0x34

/* high order word of read data from switch register */
#define B53_SRAB_RD_H			0x38

/* low order word of read data from switch register */
#define B53_SRAB_RD_L			0x3c

/* command and status register of the SRAB */
#define B53_SRAB_CTRLS			0x40
#define  B53_SRAB_CTRLS_RCAREQ		BIT(3)
#define  B53_SRAB_CTRLS_RCAGNT		BIT(4)
#define  B53_SRAB_CTRLS_SW_INIT_DONE	BIT(6)

/* the register captures interrupt pulses from the switch */
#define B53_SRAB_INTR			0x44
#define  B53_SRAB_INTR_P(x)		BIT(x)
#define  B53_SRAB_SWITCH_PHY		BIT(8)
#define  B53_SRAB_1588_SYNC		BIT(9)
#define  B53_SRAB_IMP1_SLEEP_TIMER	BIT(10)
#define  B53_SRAB_P7_SLEEP_TIMER	BIT(11)
#define  B53_SRAB_IMP0_SLEEP_TIMER	BIT(12)

struct b53_srab_priv {
	void __iomem *regs;
};

static int b53_srab_request_grant(u8 __iomem *regs)
{
	u32 ctrls;
	int i;

	ctrls = readl(regs + B53_SRAB_CTRLS);
	ctrls |= B53_SRAB_CTRLS_RCAREQ;
	writel(ctrls, regs + B53_SRAB_CTRLS);

	for (i = 0; i < 20; i++) {
		ctrls = readl(regs + B53_SRAB_CTRLS);
		if (ctrls & B53_SRAB_CTRLS_RCAGNT)
			break;
		usleep_range(10, 100);
	}
	if (WARN_ON(i == 5))
		return -EIO;

	return 0;
}

static void b53_srab_release_grant(u8 __iomem *regs)
{
	u32 ctrls;

	ctrls = readl(regs + B53_SRAB_CTRLS);
	ctrls &= ~B53_SRAB_CTRLS_RCAREQ;
	writel(ctrls, regs + B53_SRAB_CTRLS);
}

static int b53_srab_op(u8 __iomem *regs, u8 page, u8 reg, u32 op)
{
	int i;
	u32 cmdstat;

	/* set register address */
	cmdstat = (page << B53_SRAB_CMDSTAT_PAGE) |
		  (reg << B53_SRAB_CMDSTAT_REG) |
		  B53_SRAB_CMDSTAT_GORDYN |
		  op;
	writel(cmdstat, regs + B53_SRAB_CMDSTAT);

	/* check if operation completed */
	for (i = 0; i < 5; ++i) {
		cmdstat = readl(regs + B53_SRAB_CMDSTAT);
		if (!(cmdstat & B53_SRAB_CMDSTAT_GORDYN))
			break;
		usleep_range(10, 100);
	}

	if (WARN_ON(i == 5))
		return -EIO;

	return 0;
}

int b53_srab_read64(u8 __iomem *regs, u8 page, u8 reg, u64 *val)
{
	int ret = 0;

	ret = b53_srab_request_grant(regs);
	if (ret)
		goto err;

	ret = b53_srab_op(regs, page, reg, 0);
	if (ret)
		goto err;

	*val = readl(regs + B53_SRAB_RD_L);
	*val += (u64)readl(regs + B53_SRAB_RD_H) << 32;

err:
	b53_srab_release_grant(regs);

	return ret;
}

int b53_srab_read32(u8 __iomem *regs, u8 page, u8 reg, u32 *val)
{
	int ret = 0;

	ret = b53_srab_request_grant(regs);
	if (ret)
		goto err;

	ret = b53_srab_op(regs, page, reg, 0);
	if (ret)
		goto err;

	*val = readl(regs + B53_SRAB_RD_L);

err:
	b53_srab_release_grant(regs);

	return ret;
}


int b53_srab_write64(u8 __iomem *regs, u8 page, u8 reg,
			    u64 value)
{
	int ret = 0;

	ret = b53_srab_request_grant(regs);
	if (ret)
		goto err;

	writel((u32)value, regs + B53_SRAB_WD_L);
	writel((u32)(value >> 32), regs + B53_SRAB_WD_H);

	ret = b53_srab_op(regs, page, reg, B53_SRAB_CMDSTAT_WRITE);

err:
	b53_srab_release_grant(regs);

	return ret;
}
