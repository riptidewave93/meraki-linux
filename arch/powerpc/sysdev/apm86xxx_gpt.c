/**
 * APM86xxx General Purpose Timer
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Author: Vivek Mahajan <vmahajan@apm.com>
 * Author: Loc Ho <lho@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * NOTE: Cascade timer support with 64-bits counter is obsolete as there is 
 *       no real practical usage for this.
 *
 */
#include <asm/mpic.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/clk.h>

#define DEBUG

#ifdef DEBUG
#define DPRINT(x...) printk(x)
#else
#define DPRINT(x...) do{}while(0)
#endif

#define PFX				"APMGPT: "
#define MAX_NUM_GPT			4	/* Per group */

#define MPIC_TIMER_CTRL_ROVR_MSK	0x07000000
#define MPIC_TIMER_CTRL_ROVR(x)		(0x01000000 << (x))
#define MPIC_TIMER_CTRL_CLKR		0x00000300
#define MPIC_TIMER_CTRL_CASC_MSK	0x00000007
#define MPIC_TIMER_CTRL_CSCD_0123	0x00000007
#define MPIC_TIMER_CTRL_CSCD_01		0x00000001
#define MPIC_TIMER_CTRL_CSCD_012	0x00000003
#define MPIC_TIMER_CTRL_CSCD_01_23	0x00000005
#define MPIC_TIMER_CTRL_CSCD_123	0x00000006
#define MPIC_TIMER_CTRL_RTM		0x00010000

#define MPIC_TIMER_CURRENT_CNT_STOP	0x80000000

#define APM_GPT_INFO_SIGN		0x41475054	/* AGPT */

struct apm_gpt {
	u32 	virq;		/* virtual IRQ number */
	u32	idx;		/* Index within group this GPT belong to */
	struct apm_gpt_info *info;
};

struct apm_gpt_info {
	u32	signature;
	spinlock_t lock;
	struct clk *clk;
	u8	   *regs;
	struct apm_gpt gpt[MAX_NUM_GPT];
};

struct apm_gpt *apm_gpt_from_irq(int virq)
{
	struct apm_gpt_info * ctx;
	unsigned long flags;
	int i;
	
	ctx = get_irq_data(virq);
	if (ctx == NULL) {
		DPRINT("GPT: IRQ %d is not GPT\n", virq);
		return NULL;
	}
	if (ctx->signature != APM_GPT_INFO_SIGN) {
		DPRINT("GPT: IRQ %d GPT context is not valid\n", virq);
		return NULL;
	}
	
	spin_lock_irqsave(&ctx->lock, flags);	
	for (i = 0; i < MAX_NUM_GPT; i++) {
		if (ctx->gpt[i].virq == virq) {
			/* We found it */
			spin_unlock_irqrestore(&ctx->lock, flags);
			return &ctx->gpt[i]; 
		}
	}
	spin_unlock_irqrestore(&ctx->lock, flags);		
	
	return NULL;
}
EXPORT_SYMBOL(apm_gpt_from_irq);

u64 apm_gpt_cps(int virq)
{
	struct apm_gpt *gpt;

	gpt = apm_gpt_from_irq(virq);
	if (gpt == NULL)
		return 0;
	return (u64) in_be32((void *) (gpt->info->regs + MPIC_TIMER_FREQ));
}
EXPORT_SYMBOL(apm_gpt_cps);

static u64 apm_gpt_ns2clocks(struct apm_gpt *gpt, u64 periodns)
{
	u64 clocks;
	u32 freq;
	
	/* Determine the number of clocks in the requested period.  Period 
	 * is in nanosecond. Bus frequency is in Hz.
	 *
	 * clocks = (period/1,000,000,000)*frequency 
	 */
	freq   = in_be32((void *) (gpt->info->regs + MPIC_TIMER_FREQ));
	clocks = periodns * freq;
#if BITS_PER_LONG == 32	
	clocks = div64_u64(clocks, 1000000000);
#else
	clocks /= 1000000000;
#endif
	return clocks;
}

int apm_gpt_start_timer(struct apm_gpt *gpt, u64 period, int continuous)
{
	struct apm_gpt_info *ctx;
	unsigned long flags;
	u64 clocks;
	u32 val;
	
	BUG_ON(!gpt);
	
	if (continuous != 0) {
		/* Do not support continuous running */
		return -EINVAL;
	}
	
	ctx = gpt->info;
	
	clocks = apm_gpt_ns2clocks(gpt, period);

	spin_lock_irqsave(&ctx->lock, flags);
	
	/* Check if timer already running */
	val = in_be32((void *) (ctx->regs + MPIC_TIMER_BASE_OFFS + 
			MPIC_TIMER_STRIDE * gpt->idx + 
			MPIC_TIMER_CURRENT_HI_CNT));
	if (!(val & MPIC_TIMER_CURRENT_CNT_STOP)) {
		spin_unlock_irqrestore(&ctx->lock, flags);
		/* Timer already active, stop timer first */
		return -EAGAIN;
	}
	/* Load new value and start it */
	out_be32((void *) (ctx->regs + MPIC_TIMER_BASE_OFFS + 
			MPIC_TIMER_STRIDE * gpt->idx + 
			MPIC_TIMER_CURRENT_CNT), 
			(u32) clocks);
	out_be32((void *) (ctx->regs + MPIC_TIMER_BASE_OFFS + 
			MPIC_TIMER_STRIDE * gpt->idx + 
			MPIC_TIMER_CURRENT_HI_CNT),
			(u32) (clocks >> 32) & ~MPIC_TIMER_CURRENT_CNT_STOP);
			
	spin_unlock_irqrestore(&ctx->lock, flags);
	
	return 0;
}
EXPORT_SYMBOL(apm_gpt_start_timer);

void apm_gpt_stop_timer(struct apm_gpt *gpt)
{
	struct apm_gpt_info *ctx;
	unsigned long flags;
	u32 tmp;

	BUG_ON(!gpt);
	
	ctx = gpt->info;

	spin_lock_irqsave(&ctx->lock, flags);
	tmp = in_be32((void *) (ctx->regs + MPIC_TIMER_BASE_OFFS + 
			MPIC_TIMER_BASE_HI_CNT));
	tmp |= MPIC_TIMER_CURRENT_CNT_STOP; 
	out_be32((void *) (ctx->regs + MPIC_TIMER_BASE_OFFS + 
			MPIC_TIMER_BASE_HI_CNT), tmp);
	spin_unlock_irqrestore(&ctx->lock, flags);
}
EXPORT_SYMBOL(apm_gpt_stop_timer);

static unsigned int mpic_timer_divisor(unsigned int val)
{
	return 1 << (((val & MPIC_TIMER_CTRL_CLKR) >> 8) + 3);	 
}

static int __devinit apm_gpt_of_probe(struct of_device *ofdev,
				    const struct of_device_id *match)
{
	struct device_node  *np = ofdev->node;
	struct resource	    res;
	struct apm_gpt_info *ctx;
	int rc;
	int i;
	u32 val;
	
	ctx = kzalloc(sizeof(struct apm_gpt_info), GFP_KERNEL);
	if (!ctx) {
		pr_err(PFX "%s: unable to allocate memory\n",
			np->full_name);
		return -ENOMEM;
	}
	ctx->signature = APM_GPT_INFO_SIGN;
	
	/* Get the HBF clock in which MPIC attached to */
	ctx->clk = clk_get(NULL, "hbf");
	if (IS_ERR(ctx->clk)) { 
		pr_err(PFX "%s: no clock implementation\n",
			np->full_name);
		rc = -EINVAL;
		goto err;
	}
	
	/* Setup CSR */
	rc = of_address_to_resource(np, 0, &res);
	if (rc) {
		rc = -EINVAL;
		pr_err(PFX "%s: no CSR entry\n", np->full_name);
		goto err;
	}
	ctx->regs = of_iomap(np, 0);
	if (!ctx->regs) {
		pr_err(PFX "%s: unable to iomap registers\n", np->full_name);
		rc = -EINVAL;
		goto err;			       		
	}
	/* Configure this group to standard timer (no cascade) and 
	   use HBF clock instead RTC */
	val = in_be32((void *) (ctx->regs + MPIC_TIMER_CTRL));
	val &= ~MPIC_TIMER_CTRL_CASC_MSK;
	val &= ~MPIC_TIMER_CTRL_RTM;
	val &= ~MPIC_TIMER_CTRL_CLKR;
	out_be32((void *) (ctx->regs + MPIC_TIMER_CTRL), val);
	/* Set the timer frequency (tick/second) */		
	out_be32((void *) (ctx->regs + MPIC_TIMER_FREQ), 
		clk_get_rate(ctx->clk) / mpic_timer_divisor(val));
	
	/* Configure each IRQ for each timer */
	for (i = 0; i < MAX_NUM_GPT; i++) {
		struct resource irq;
		unsigned int virq;

		if (of_irq_to_resource(np, i, &irq) == NO_IRQ) {
			if (i == 0) {
				pr_err(PFX 
					"%s: not enough interrupts specified\n",
			       		np->full_name);
			       	rc = -EINVAL;
				goto err;
			}
			break;
		}
		virq = irq_create_mapping(NULL, (u32) irq.start);
		if (virq == NO_IRQ) {
			pr_err(PFX "Map irq %d failed\n", (u32) irq.start);
			rc = -EFAULT;
			goto err;
		}
		
		set_irq_data(virq, ctx);
		
		ctx->gpt[i].virq = virq;
		ctx->gpt[i].idx  = i;
		ctx->gpt[i].info = ctx;
	}
	for ( ; i < MAX_NUM_GPT; i++) {
		ctx->gpt[i].virq = NO_IRQ;
		ctx->gpt[i].idx = i;
		ctx->gpt[i].info = ctx;
	}
	
	spin_lock_init(&ctx->lock);
				      
	dev_set_drvdata(&ofdev->dev, ctx);

	printk(KERN_INFO "APM86xxx GPT @0x%02X.%08X initialized\n",
		(u32) (res.start >> 32), (u32) res.start);
	return 0;

err:
	for (i = 0; i < MAX_NUM_GPT; i++) {
		irq_dispose_mapping(ctx->gpt[i].virq);
	}
	iounmap(ctx->regs);
	kfree(ctx);
	return rc;
}

static int __devexit apm_gpt_of_remove(struct of_device *dev)
{
	unsigned int i;
	struct apm_gpt_info *ctx;
	struct apm_gpt *gpt;
	
	ctx = dev_get_drvdata(&dev->dev);
	if (ctx == NULL)
		return -EINVAL;
	
	/* Disable all timers */
	for (i = 0; i < MAX_NUM_GPT; i++) {
		gpt = &ctx->gpt[i];
		if (gpt->virq != NO_IRQ) {
			apm_gpt_stop_timer(gpt);
			set_irq_data(gpt->virq, NULL);
			irq_dispose_mapping(gpt->virq);
		}
	}
	iounmap(ctx->regs);
	kfree(ctx);
	dev_set_drvdata(&dev->dev, NULL);
	
	return 0;
}

static struct of_device_id apm_gpt_of_match[] = {
	{
		.compatible      = "apm,apm86xxx-gpt",
	},
	{},
};

static struct of_platform_driver apm_gpt_driver = {
	.name		= "apm86xxx-gpt",
	.match_table	= apm_gpt_of_match,
	.probe		= apm_gpt_of_probe,
	.remove		= __devexit_p(apm_gpt_of_remove),
};

static int __init apm_gpt_init(void)
{
	return of_register_platform_driver(&apm_gpt_driver);
}

static void __exit apm_gpt_cleanup(void)
{
	of_unregister_platform_driver(&apm_gpt_driver);
}

module_init(apm_gpt_init);
module_exit(apm_gpt_cleanup);

MODULE_DESCRIPTION("APM86xxx General Purpose Timer");
MODULE_AUTHOR("Vivek Mahajan <vmahajan@apm.com>");
MODULE_LICENSE("GPL v2");
