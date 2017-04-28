/*
 * linux/drivers/spi/spi_ambarella.c
 *
 * History:
 *	2008/03/03 - [Louis Sun]  created file
 *	2009/06/19 - [Zhenwu Xue] ported from 2.6.22.10

 *
 * Copyright (C) 2004-2012, Ambarella, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <asm/io.h>
#include <mach/io.h>
#include <plat/spi.h>
#include <plat/ambcache.h>

/*============================Global Variables================================*/

struct ambarella_spi {
	u32					regbase;
	struct ambarella_spi_platform_info	*pinfo;

	int					irq;
	struct tasklet_struct			tasklet;

	spinlock_t				lock;
	struct list_head			queue;
	u32					idle;
	u32					shutdown;

	struct spi_device			*c_dev;
	struct spi_message			*c_msg;
	struct spi_transfer			*c_xfer;

	u8					rw_mode, bpw, chip_select;
	u32					ridx, widx, len;
};

struct ambarella_spi_private {
	struct spi_device			*spi;
	struct mutex				mtx;
	spinlock_t				lock;
};

static struct {
	int					cs_num;
	struct ambarella_spi_private		*data;
} ambarella_spi_private_devices[SPI_MASTER_INSTANCES];

static void ambarella_spi_handle_message(struct ambarella_spi *);
static void ambarella_spi_prepare_message(struct ambarella_spi *);
static void ambarella_spi_prepare_transfer(struct ambarella_spi *);
static void ambarella_spi_finish_transfer(struct ambarella_spi *);
static void ambarella_spi_finish_message(struct ambarella_spi *);
static void ambarella_spi_start_transfer(struct ambarella_spi *);

/*============================SPI Bus Driver==================================*/
static int ambarella_spi_setup(struct spi_device *spi)
{
	return 0;
}

static int ambarella_spi_stop(struct ambarella_spi *priv)
{
	amba_readl(priv->regbase + SPI_ICR_OFFSET);
	amba_readl(priv->regbase + SPI_ISR_OFFSET);
	amba_writel(priv->regbase + SPI_SER_OFFSET, 0);
	amba_writel(priv->regbase + SPI_SSIENR_OFFSET, 0);

	return 0;
}

static void ambarella_spi_start_transfer(struct ambarella_spi *priv)
{
	void 				*wbuf;
	u32				widx, ridx, len;
	u32				xfer_len;
	u8				cs_id;
	u16				i, tmp;

	wbuf	= (void *)priv->c_xfer->tx_buf;
	len	= priv->len;
	cs_id	= priv->c_dev->chip_select;
	widx	= priv->widx;
	ridx	= priv->ridx;

	/* Feed data into FIFO */
	switch (priv->rw_mode) {
	case SPI_WRITE_ONLY:
		xfer_len = len - widx;
		if (xfer_len > priv->pinfo->fifo_entries)
			xfer_len = priv->pinfo->fifo_entries;

		amba_writel(priv->regbase + SPI_SER_OFFSET, 0);
		if (priv->bpw <= 8) {
			for(i = 0; i < xfer_len; i++) {
				tmp = ((u8 *)wbuf)[widx++];
				amba_writel(priv->regbase + SPI_DR_OFFSET, tmp);
			}
		} else{
			for(i = 0; i < xfer_len; i++) {
				tmp = ((u16 *)wbuf)[widx++];
				amba_writel(priv->regbase + SPI_DR_OFFSET, tmp);
			}
		}
		amba_writel(priv->regbase + SPI_SER_OFFSET, 1 << cs_id);

		break;

	case SPI_WRITE_READ:
		xfer_len = len - widx;
		if (xfer_len > priv->pinfo->fifo_entries)
			xfer_len = priv->pinfo->fifo_entries;

		amba_writel(priv->regbase + SPI_SER_OFFSET, 0);
		if (priv->bpw <= 8) {
			for(i = 0; i < xfer_len; i++) {
				tmp = ((u8 *)wbuf)[widx++];
				amba_writel(priv->regbase + SPI_DR_OFFSET, tmp);
			}
		} else{
			for(i = 0; i < xfer_len; i++) {
				tmp = ((u16 *)wbuf)[widx++];
				amba_writel(priv->regbase + SPI_DR_OFFSET, tmp);
			}
		}
		amba_writel(priv->regbase + SPI_SER_OFFSET, 1 << cs_id);

		break;

	case SPI_READ_ONLY:
		xfer_len = len - ridx;
		if (xfer_len > priv->pinfo->fifo_entries)
			xfer_len = priv->pinfo->fifo_entries;

		amba_writel(priv->regbase + SPI_SER_OFFSET, 0);
		for(i = 0; i < xfer_len; i++)
			amba_writel(priv->regbase + SPI_DR_OFFSET, SPI_DUMMY_DATA);
		amba_writel(priv->regbase + SPI_SER_OFFSET, 1 << cs_id);

		break;

	default:
		break;
	}

	priv->widx = widx;
	enable_irq(priv->irq);

	return;
}

static void ambarella_spi_tasklet(unsigned long data)
{
	struct ambarella_spi		*priv	= (struct ambarella_spi *)data;
	void 				*rbuf;
	u32				widx, ridx, len;
	u32				rxflr, xfer_len;
	u32				status;
	u16				i, tmp;
	u32				finish_transfer;

	/* Wait until SPI idle */
	status = amba_readl(priv->regbase + SPI_SR_OFFSET);
	if (status & 0x1) {
		/* Transfer is still in progress */
		for (i = 0; i < MAX_QUERY_TIMES; i++) {
			status = amba_readl(priv->regbase + SPI_SR_OFFSET);
			if (!(status & 0x1))
				break;
		}
		if (status & 0x1) {
			tasklet_schedule(&priv->tasklet);
			return;
		}
	}

	rbuf	= (void *)priv->c_xfer->rx_buf;
	len	= priv->len;
	widx	= priv->widx;
	ridx	= priv->ridx;

	/* Fetch data from FIFO */
	switch (priv->rw_mode) {
	case SPI_READ_ONLY:
	case SPI_WRITE_READ:
		xfer_len	= len - ridx;
		rxflr		= amba_readl(priv->regbase + SPI_RXFLR_OFFSET);
		if (xfer_len > rxflr)
			xfer_len = rxflr;

		if (priv->bpw <= 8) {
			for(i = 0; i < xfer_len; i++) {
				tmp	= amba_readl(priv->regbase + SPI_DR_OFFSET);
				((u8 *)rbuf)[ridx++]	= tmp & 0xff;
			}
		} else {
			for(i = 0; i < xfer_len; i++){
				tmp	= amba_readl(priv->regbase + SPI_DR_OFFSET);
				((u16 *)rbuf)[ridx++]	= tmp;
			}
		}

		priv->ridx	= ridx;
		break;

	default:
		break;
	}

	/* Check whether the current transfer ends */
	finish_transfer = 0;
	switch (priv->rw_mode) {
	case SPI_WRITE_ONLY:
		if (widx == len) {
			finish_transfer = 1;
		}
		break;

	case SPI_READ_ONLY:
		if (ridx == len) {
			finish_transfer = 1;
		}
		break;

	case SPI_WRITE_READ:
		if (ridx == len && widx == len) {
			finish_transfer = 1;
		}
		break;

	default:
		break;
	}

	/* End transfer or continue filling FIFO */
	if (finish_transfer) {
		ambarella_spi_finish_transfer(priv);
		enable_irq(priv->irq);
	} else {
		ambarella_spi_start_transfer(priv);
	}
}

static void ambarella_spi_prepare_transfer(struct ambarella_spi *priv)
{
	struct spi_message		*msg;
	struct spi_transfer		*xfer;
	struct ambarella_spi_cs_config  cs_config;
	u32				ctrlr0;
	void				*wbuf, *rbuf;

	msg		= priv->c_msg;
	xfer		= list_entry(msg->transfers.next, struct spi_transfer, transfer_list);
	priv->c_xfer	= xfer;
	list_del(msg->transfers.next);

	wbuf	= (void *)xfer->tx_buf;
	rbuf	= (void *)xfer->rx_buf;
	if (priv->bpw <= 8)
		priv->len	= xfer->len;
	else
		priv->len	= xfer->len >> 1;
	priv->widx	= 0;
	priv->ridx	= 0;
	if (wbuf && !rbuf)
		priv->rw_mode = SPI_WRITE_ONLY;
	if ( !wbuf && rbuf)
		priv->rw_mode = SPI_READ_ONLY;
	if (wbuf && rbuf)
		priv->rw_mode = SPI_WRITE_READ;

	ctrlr0	= amba_readl(priv->regbase + SPI_CTRLR0_OFFSET);
	ctrlr0	&= 0xfffff4ff;
	/* Always use write & read mode due to I1 changes */
	ctrlr0	|= (SPI_WRITE_READ << 8);
	if (priv->c_dev->mode & SPI_LOOP)
		ctrlr0 |= (0x1 << 11);

	amba_writel(priv->regbase + SPI_CTRLR0_OFFSET, ctrlr0);

	if (!priv->chip_select) {
		cs_config.bus_id	= priv->c_dev->master->bus_num;
		cs_config.cs_id		= priv->c_dev->chip_select;
		cs_config.cs_num	= priv->c_dev->master->num_chipselect;
		cs_config.cs_pins	= priv->pinfo->cs_pins;
		cs_config.cs_high	= priv->pinfo->cs_high;
		priv->pinfo->cs_activate(&cs_config);
		priv->chip_select	= 1;
	}

	disable_irq_nosync(priv->irq);
	amba_writel(priv->regbase + SPI_IMR_OFFSET, SPI_TXEIS_MASK);
	amba_writel(priv->regbase + SPI_SSIENR_OFFSET, 1);
	amba_writel(priv->regbase + SPI_SER_OFFSET, 0);
}

static void ambarella_spi_finish_transfer(struct ambarella_spi *priv)
{
	if (priv->c_xfer->cs_change) {
		struct ambarella_spi_cs_config  cs_config;

		cs_config.bus_id	= priv->c_dev->master->bus_num;
		cs_config.cs_id		= priv->c_msg->spi->chip_select;
		cs_config.cs_num	= priv->c_dev->master->num_chipselect;
		cs_config.cs_pins	= priv->pinfo->cs_pins;
		cs_config.cs_high	= priv->pinfo->cs_high;

		priv->pinfo->cs_deactivate(&cs_config);
		priv->chip_select	= 0;
	}
	ambarella_spi_stop(priv);

	if (list_empty(&priv->c_msg->transfers)) {
		ambarella_spi_finish_message(priv);
	} else {
		ambarella_spi_prepare_transfer(priv);
		ambarella_spi_start_transfer(priv);
	}
}

static void ambarella_spi_finish_message(struct ambarella_spi *priv)
{
	struct spi_message		*msg;
	unsigned long			flags;
	u32				message_pending;

	if (priv->chip_select) {
		struct ambarella_spi_cs_config  cs_config;

		cs_config.bus_id	= priv->c_dev->master->bus_num;
		cs_config.cs_id		= priv->c_msg->spi->chip_select;
		cs_config.cs_num	= priv->c_dev->master->num_chipselect;
		cs_config.cs_pins	= priv->pinfo->cs_pins;
		cs_config.cs_high	= priv->pinfo->cs_high;

		priv->pinfo->cs_deactivate(&cs_config);
		priv->chip_select	= 0;
	}

	msg			= priv->c_msg;
	msg->actual_length	= priv->c_xfer->len;
	msg->status		= 0;

	/* Next Message */
	spin_lock_irqsave(&priv->lock, flags);
	list_del_init(&msg->queue);
	if (!list_empty(&priv->queue)) {
		message_pending	= 1;
	} else {
		message_pending	= 0;
		priv->idle	= 1;
		priv->c_msg	= NULL;
		priv->c_xfer	= NULL;
	}
	spin_unlock_irqrestore(&priv->lock, flags);

	msg->complete(msg->context);
	if (message_pending) {
		ambarella_spi_handle_message(priv);
	}
}

static void ambarella_spi_handle_message(struct ambarella_spi *priv)
{
	ambarella_spi_prepare_message(priv);
	ambarella_spi_prepare_transfer(priv);
	ambarella_spi_start_transfer(priv);
}

static void ambarella_spi_prepare_message(struct ambarella_spi *priv)
{
	u32 				ctrlr0, ssi_clk, sckdv, cs_pin, cs_high;
	struct spi_message		*msg;
	unsigned long			flags;

	spin_lock_irqsave(&priv->lock, flags);
	msg	= list_entry(priv->queue.next, struct spi_message, queue);
	spin_unlock_irqrestore(&priv->lock, flags);
	ctrlr0	= amba_readl(priv->regbase + SPI_CTRLR0_OFFSET);

	if (msg->spi->bits_per_word < 4)
		msg->spi->bits_per_word		= 4;
	if (msg->spi->bits_per_word > 16)
		msg->spi->bits_per_word		= 16;
	priv->bpw	= msg->spi->bits_per_word;

	ctrlr0	&= 0xfffffff0;
	ctrlr0	|= (priv->bpw - 1);

	ctrlr0	&= (~((1 << 6) | (1 << 7)));
	ctrlr0	|= ((msg->spi->mode & (SPI_CPHA | SPI_CPOL)) << 6);
	if (msg->spi->mode & SPI_LOOP) {
		ctrlr0 |= 0x00000800;
	}
	amba_writel(priv->regbase + SPI_CTRLR0_OFFSET, ctrlr0);

	ssi_clk	= priv->pinfo->get_ssi_freq_hz();
	if(msg->spi->max_speed_hz == 0 || msg->spi->max_speed_hz > ssi_clk / 2)
	    msg->spi->max_speed_hz = ssi_clk / 2;
	sckdv = (u16)(((ssi_clk / msg->spi->max_speed_hz) + 0x01) & 0xfffe);
	amba_writel(priv->regbase + SPI_BAUDR_OFFSET, sckdv);

	priv->chip_select	= 0;
	priv->c_dev		= msg->spi;
	priv->c_msg		= msg;

	cs_pin	= priv->pinfo->cs_pins[priv->c_dev->chip_select];
	cs_high	= priv->pinfo->cs_high[priv->c_dev->chip_select];
	ambarella_gpio_config(cs_pin, GPIO_FUNC_SW_OUTPUT);
	if (!cs_high) {
		ambarella_gpio_set(cs_pin, GPIO_HIGH);
	} else {
		ambarella_gpio_set(cs_pin, GPIO_LOW);
	}
}

static int ambarella_spi_main_entry(struct spi_device *spi, struct spi_message *msg)
{
	struct ambarella_spi		*priv;
	struct spi_transfer		*xfer;
	unsigned long			flags;
	u32				shut_down, bus_idle;

	priv		= spi_master_get_devdata(spi->master);
	spin_lock_irqsave(&priv->lock, flags);
	shut_down	= priv->shutdown;
	spin_unlock_irqrestore(&priv->lock, flags);
	if (shut_down) {
		return -ESHUTDOWN;
	}

	/* Validation */
	if (list_empty(&msg->transfers) || !spi->max_speed_hz) {
		return -EINVAL;
	}

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (!xfer->tx_buf && !xfer->rx_buf) {
			return -EINVAL;
		}

		if (spi->bits_per_word > 8 && (xfer->len & 0x1)) {
			return -EINVAL;
		}
	}

	/* Queue Message */
	msg->status		= -EINPROGRESS;
	msg->actual_length	= 0;
	spin_lock_irqsave(&priv->lock, flags);
	list_add_tail(&msg->queue, &priv->queue);
	if (priv->idle) {
		priv->idle	= 0;
		bus_idle	= 1;
	} else {
		bus_idle	= 0;
	}
	spin_unlock_irqrestore(&priv->lock, flags);

	/* Handle message right away if bus is idle */
	if (bus_idle) {
		ambarella_spi_handle_message(priv);
	}

	return 0;
}

static void ambarella_spi_cleanup(struct spi_device *spi)
{
	return;
}

static int ambarella_spi_inithw(struct ambarella_spi *priv)
{
	u16 				sckdv;
	u32 				ctrlr0, ssi_freq;

	/* Set PLL */
	if (priv->pinfo->rct_set_ssi_pll)
		priv->pinfo->rct_set_ssi_pll();

	/* Disable SPI */
	ambarella_spi_stop(priv);

#if 0
	/* Config Chip Select Pins */
	for (i = 0; i < priv->pinfo->cs_num; i++) {
		if (priv->pinfo->cs_pins[i] < 0) {
			continue;
		}

		ambarella_gpio_config(priv->pinfo->cs_pins[i], GPIO_FUNC_SW_OUTPUT);
		ambarella_gpio_set(priv->pinfo->cs_pins[i], GPIO_HIGH);
	}
#endif

	/* Initial Register Settings */
	ctrlr0 = ( ( SPI_CFS << 12) | (SPI_WRITE_ONLY << 8) | (SPI_SCPOL << 7) |
		(SPI_SCPH << 6)	| (SPI_FRF << 4) | (SPI_DFS)
	      );
	amba_writel(priv->regbase + SPI_CTRLR0_OFFSET, ctrlr0);

	ssi_freq = 54000000;
	if (priv->pinfo->get_ssi_freq_hz)
		ssi_freq = priv->pinfo->get_ssi_freq_hz();
	sckdv =	(u16)(((ssi_freq / SPI_BAUD_RATE) + 0x01) & 0xfffe);
	amba_writel(priv->regbase + SPI_BAUDR_OFFSET, sckdv);

	amba_writel(priv->regbase + SPI_TXFTLR_OFFSET, 0);
	amba_writel(priv->regbase + SPI_RXFTLR_OFFSET, 1);

	return 0;
}

static irqreturn_t ambarella_spi_isr(int irq, void *dev_data)
{
	struct ambarella_spi		*priv	= dev_data;

	if (amba_readl(priv->regbase + SPI_ISR_OFFSET)) {
		disable_irq_nosync(priv->irq);
		amba_writel(priv->regbase + SPI_ISR_OFFSET, 0);

		ambarella_spi_tasklet((unsigned long)priv);
	}

	return IRQ_HANDLED;
}

static int ambarella_spi_probe(struct platform_device *pdev)
{
	struct ambarella_spi			*priv;
	struct ambarella_spi_private		*ps;
	struct spi_master			*master;
	struct spi_device 			*spidev;
	struct resource 			*res;
	struct ambarella_spi_platform_info	*pinfo;
	int					i, irq, errorCode;

	/* Get IRQ NO. */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}

	/* Get Platform Info */
	pinfo = (struct ambarella_spi_platform_info *)pdev->dev.platform_data;
	if (!pinfo) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}
	if (pinfo->cs_num && !pinfo->cs_pins) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}
	if (!pinfo->cs_activate || !pinfo->cs_deactivate) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}
	if (!pinfo->rct_set_ssi_pll || !pinfo->get_ssi_freq_hz) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}

	/* Get Base Address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		errorCode = -EINVAL;
		goto ambarella_spi_probe_exit3;
	}

	/* Alocate Master */
	master = spi_alloc_master(&pdev->dev, sizeof *priv);
	if (!master) {
		errorCode = -ENOMEM;
		goto ambarella_spi_probe_exit3;
	}

	/* Initalize Device Data */
	master->bus_num		= pdev->id;
	master->num_chipselect	= pinfo->cs_num;
	master->mode_bits	= SPI_CPHA | SPI_CPOL | SPI_LSB_FIRST | SPI_LOOP;
	master->setup		= ambarella_spi_setup;
	master->transfer	= ambarella_spi_main_entry;
	master->cleanup		= ambarella_spi_cleanup;
	platform_set_drvdata(pdev, master);
	priv			= spi_master_get_devdata(master);
	priv->regbase		= (u32)res->start;
	priv->irq		= irq;
	priv->pinfo		= pinfo;
	tasklet_init(&priv->tasklet, ambarella_spi_tasklet, (unsigned long)priv);
	INIT_LIST_HEAD(&priv->queue);
	priv->idle		= 1;
	priv->c_dev		= NULL;
	priv->c_msg		= NULL;
	priv->c_xfer		= NULL;
	priv->shutdown		= 0;
	spin_lock_init(&priv->lock);
	priv->bpw		= 16;

	/* Inittialize Hardware*/
	ambarella_spi_inithw(priv);

	/* Request IRQ */
	errorCode = request_irq(irq, ambarella_spi_isr, IRQF_TRIGGER_HIGH,
			dev_name(&pdev->dev), priv);
	if (errorCode)
		goto ambarella_spi_probe_exit2;
	else
		dev_info(&pdev->dev, "ambarella SPI Controller %d created \n", pdev->id);

	/* Register Master */
	errorCode = spi_register_master(master);
	if (errorCode)
		goto ambarella_spi_probe_exit1;

	/* Allocate Private Devices */
	ps = (struct ambarella_spi_private *)kmalloc(master->num_chipselect * \
		sizeof(struct ambarella_spi_private), GFP_KERNEL);
	if (!ps) {
		errorCode = -ENOMEM;
		goto ambarella_spi_probe_exit3;
	}
	spidev = (struct spi_device *)kmalloc(master->num_chipselect * \
		sizeof(struct spi_device), GFP_KERNEL);
	if (!spidev) {
		errorCode = -ENOMEM;
		kfree(ps);
		goto ambarella_spi_probe_exit3;
	}

	for (i = 0; i < master->num_chipselect; i++) {
		ps[i].spi		= spidev + i;
		ps[i].spi->master	= master;
		mutex_init(&ps[i].mtx);
		spin_lock_init(&ps[i].lock);
	}
	ambarella_spi_private_devices[master->bus_num].cs_num	= master->num_chipselect;
	ambarella_spi_private_devices[master->bus_num].data	= ps;

	goto ambarella_spi_probe_exit3;

ambarella_spi_probe_exit1:
	free_irq(irq, priv);

ambarella_spi_probe_exit2:
	tasklet_kill(&priv->tasklet);
	spi_master_put(master);

ambarella_spi_probe_exit3:
	return errorCode;
}

static int ambarella_spi_remove(struct platform_device *pdev)
{

	struct spi_master		*master = platform_get_drvdata(pdev);
	struct ambarella_spi		*priv = spi_master_get_devdata(master);
	struct spi_message		*msg;
	unsigned long			flags;

	spin_lock_irqsave(&priv->lock, flags);
	priv->shutdown	= 1;
	spin_unlock_irqrestore(&priv->lock, flags);
	tasklet_kill(&priv->tasklet);
	free_irq(priv->irq, priv);
	ambarella_spi_stop(priv);

	spin_lock_irqsave(&priv->lock, flags);
	list_for_each_entry(msg, &priv->queue, queue) {
		msg->status	= -ESHUTDOWN;
		msg->complete(msg->context);
	}
	spin_unlock_irqrestore(&priv->lock, flags);

	spi_unregister_master(master);

	return 0;
}

#ifdef CONFIG_PM
static int ambarella_spi_suspend_noirq(struct device *dev)
{
	int				errorCode = 0;
	struct spi_master		*master;
	struct ambarella_spi		*priv;
	struct platform_device		*pdev;

	pdev = to_platform_device(dev);
	master = platform_get_drvdata(pdev);
	priv = spi_master_get_devdata(master);

	if (priv) {
		disable_irq(priv->irq);
		ambarella_spi_stop(priv);
	} else {
		dev_err(&pdev->dev, "Cannot find valid pinfo\n");
		errorCode = -ENXIO;
	}

	dev_dbg(&pdev->dev, "%s\n", __func__);

	return errorCode;
}

static int ambarella_spi_resume_noirq(struct device *dev)
{
	int				errorCode = 0;
	struct spi_master		*master;
	struct ambarella_spi		*priv;
	struct platform_device		*pdev;

	pdev = to_platform_device(dev);
	master = platform_get_drvdata(pdev);
	priv = spi_master_get_devdata(master);
	if (priv) {
		ambarella_spi_inithw(priv);
		enable_irq(priv->irq);
	} else {
		dev_err(&pdev->dev, "Cannot find valid pinfo\n");
		errorCode = -ENXIO;
	}

	dev_dbg(&pdev->dev, "%s\n", __func__);

	return errorCode;
}

static const struct dev_pm_ops ambarella_spi_dev_pm_ops = {
	.suspend_noirq = ambarella_spi_suspend_noirq,
	.resume_noirq = ambarella_spi_resume_noirq,
};
#endif

static struct platform_driver ambarella_spi_driver = {
	.probe		= ambarella_spi_probe,
	.remove		= ambarella_spi_remove,
	.driver		= {
		.name	= "ambarella-spi",
		.owner	= THIS_MODULE,
#ifdef CONFIG_PM
		.pm	= &ambarella_spi_dev_pm_ops,
#endif
	},
};

static int __init ambarella_spi_init(void)
{
	return platform_driver_register(&ambarella_spi_driver);
}

static void __exit ambarella_spi_exit(void)
{
	platform_driver_unregister(&ambarella_spi_driver);
}

subsys_initcall(ambarella_spi_init);
module_exit(ambarella_spi_exit);

MODULE_DESCRIPTION("Ambarella Media Processor SPI Bus Controller");
MODULE_AUTHOR("Louis Sun, <louis.sun@ambarella.com>");
MODULE_LICENSE("GPL");


/*=================Utilities for Non-GPL Use==================================*/
static void ambarella_spi_complete(void *arg)
{
	complete(arg);
}

int ambarella_spi_write(amba_spi_cfg_t *spi_cfg, amba_spi_write_t *spi_write)
{
	u8				bus_id, cs_id, cs_num;
	int				errorCode;
	struct ambarella_spi_private	*ps;
	struct spi_device		*spi;
	struct spi_message		msg;
	struct spi_transfer		xfer;

	DECLARE_COMPLETION_ONSTACK(done);

	/* Validate Input Args */
	if (!spi_cfg || !spi_write)
		return -EINVAL;

	bus_id	= spi_write->bus_id;
	cs_id	= spi_write->cs_id;
	cs_num	= ambarella_spi_private_devices[bus_id].cs_num;
	ps	= ambarella_spi_private_devices[bus_id].data;

	if (bus_id >= SPI_MASTER_INSTANCES	|| cs_id >= cs_num
		|| !spi_write->buffer	|| !spi_write->n_size)
		return -EINVAL;

	/* Transfer */
	memset(&xfer, 0, sizeof(struct spi_transfer));
	xfer.tx_buf	= spi_write->buffer;
	xfer.len	= spi_write->n_size;
	xfer.cs_change	= spi_cfg->cs_change;

	/* Message */
	memset(&msg, 0, sizeof(struct spi_message));
	INIT_LIST_HEAD(&msg.transfers);
	list_add_tail(&xfer.transfer_list, &msg.transfers);
	msg.complete	= ambarella_spi_complete;
	msg.context	= &done;
	spi		= ps[cs_id].spi;
	msg.spi		= spi;

	mutex_lock(&ps[cs_id].mtx);

	/* Config */
	spi->chip_select	= cs_id;
	spi->mode		= spi_cfg->spi_mode;
	spi->mode		&= ~SPI_LOOP;
	spi->bits_per_word	= spi_cfg->cfs_dfs;
	spi->max_speed_hz	= spi_cfg->baud_rate;

	/* Wait */
	spin_lock_irq(&ps[cs_id].lock);
	errorCode = spi->master->transfer(spi, &msg);
	spin_unlock_irq(&ps[cs_id].lock);
	if (!errorCode)
		wait_for_completion(&done);

	mutex_unlock(&ps[cs_id].mtx);

	return errorCode;
}
EXPORT_SYMBOL(ambarella_spi_write);

int ambarella_spi_read(amba_spi_cfg_t *spi_cfg, amba_spi_read_t *spi_read)
{
	u8				bus_id, cs_id, cs_num;
	int				errorCode;
	struct ambarella_spi_private	*ps;
	struct spi_device		*spi;
	struct spi_message		msg;
	struct spi_transfer		xfer;

	DECLARE_COMPLETION_ONSTACK(done);

	/* Validate Input Args */
	if (!spi_cfg || !spi_read)
		return -EINVAL;

	bus_id	= spi_read->bus_id;
	cs_id	= spi_read->cs_id;
	cs_num	= ambarella_spi_private_devices[bus_id].cs_num;
	ps	= ambarella_spi_private_devices[bus_id].data;

	if (bus_id >= SPI_MASTER_INSTANCES	|| cs_id >= cs_num
		|| !spi_read->buffer	|| !spi_read->n_size)
		return -EINVAL;

	/* Transfer */
	memset(&xfer, 0, sizeof(struct spi_transfer));
	xfer.rx_buf	= spi_read->buffer;
	xfer.len	= spi_read->n_size;
	xfer.cs_change	= spi_cfg->cs_change;

	/* Message */
	memset(&msg, 0, sizeof(struct spi_message));
	INIT_LIST_HEAD(&msg.transfers);
	list_add_tail(&xfer.transfer_list, &msg.transfers);
	msg.complete	= ambarella_spi_complete;
	msg.context	= &done;
	spi		= ps[cs_id].spi;
	msg.spi		= spi;

	mutex_lock(&ps[cs_id].mtx);

	/* Config */
	spi->chip_select	= cs_id;
	spi->mode		= spi_cfg->spi_mode;
	spi->mode		&= ~SPI_LOOP;
	spi->bits_per_word	= spi_cfg->cfs_dfs;
	spi->max_speed_hz	= spi_cfg->baud_rate;

	/* Wait */
	spin_lock_irq(&ps[cs_id].lock);
	errorCode = spi->master->transfer(spi, &msg);
	spin_unlock_irq(&ps[cs_id].lock);
	if (!errorCode)
		wait_for_completion(&done);

	mutex_unlock(&ps[cs_id].mtx);

	return errorCode;
}
EXPORT_SYMBOL(ambarella_spi_read);

int ambarella_spi_write_then_read(amba_spi_cfg_t *spi_cfg,
	amba_spi_write_then_read_t *spi_write_then_read)
{
	u8				bus_id, cs_id, cs_num, *buf;
	u16				size;
	int				errorCode;
	struct ambarella_spi_private	*ps;
	struct spi_device		*spi;
	struct spi_message		msg;
	struct spi_transfer		xfer;

	DECLARE_COMPLETION_ONSTACK(done);

	/* Validate Input Args */
	if (!spi_cfg || !spi_write_then_read)
		return -EINVAL;

	bus_id	= spi_write_then_read->bus_id;
	cs_id	= spi_write_then_read->cs_id;
	cs_num	= ambarella_spi_private_devices[bus_id].cs_num;
	ps	= ambarella_spi_private_devices[bus_id].data;

	if (bus_id >= SPI_MASTER_INSTANCES		  || cs_id >= cs_num
		|| !spi_write_then_read->w_buffer || !spi_write_then_read->w_size
		|| !spi_write_then_read->r_buffer || !spi_write_then_read->r_size)
		return -EINVAL;

	/* Prepare Buffer */
	size	= spi_write_then_read->w_size + spi_write_then_read->r_size;
	buf	= (u8 *)kmalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	memcpy(buf, spi_write_then_read->w_buffer, spi_write_then_read->w_size);
	memset(buf + spi_write_then_read->w_size, SPI_DUMMY_DATA,
		spi_write_then_read->r_size);

	/* Transfer */
	memset(&xfer, 0, sizeof(struct spi_transfer));
	xfer.tx_buf	= buf;
	xfer.rx_buf	= buf;
	xfer.len	= size;
	xfer.cs_change	= spi_cfg->cs_change;

	/* Message */
	memset(&msg, 0, sizeof(struct spi_message));
	INIT_LIST_HEAD(&msg.transfers);
	list_add_tail(&xfer.transfer_list, &msg.transfers);
	msg.complete	= ambarella_spi_complete;
	msg.context	= &done;
	spi		= ps[cs_id].spi;
	msg.spi		= spi;

	mutex_lock(&ps[cs_id].mtx);

	/* Config */
	spi->chip_select	= cs_id;
	spi->mode		= spi_cfg->spi_mode;
	spi->mode		&= ~SPI_LOOP;
	spi->bits_per_word	= spi_cfg->cfs_dfs;
	spi->max_speed_hz	= spi_cfg->baud_rate;

	/* Wait */
	spin_lock_irq(&ps[cs_id].lock);
	errorCode = spi->master->transfer(spi, &msg);
	spin_unlock_irq(&ps[cs_id].lock);
	if (!errorCode)
		wait_for_completion(&done);

	mutex_unlock(&ps[cs_id].mtx);

	/* Free Buffer */
	memcpy(spi_write_then_read->r_buffer, buf + spi_write_then_read->w_size,
		spi_write_then_read->r_size);
	kfree(buf);

	return errorCode;
}
EXPORT_SYMBOL(ambarella_spi_write_then_read);

int ambarella_spi_write_and_read(amba_spi_cfg_t *spi_cfg,
	amba_spi_write_and_read_t *spi_write_and_read)
{
	u8				bus_id, cs_id, cs_num;
	int				errorCode;
	struct ambarella_spi_private	*ps;
	struct spi_device		*spi;
	struct spi_message		msg;
	struct spi_transfer		xfer;

	DECLARE_COMPLETION_ONSTACK(done);

	/* Validate Input Args */
	if (!spi_cfg || !spi_write_and_read)
		return -EINVAL;

	bus_id	= spi_write_and_read->bus_id;
	cs_id	= spi_write_and_read->cs_id;
	cs_num	= ambarella_spi_private_devices[bus_id].cs_num;
	ps	= ambarella_spi_private_devices[bus_id].data;

	if (bus_id >= SPI_MASTER_INSTANCES		|| cs_id >= cs_num
		|| !spi_write_and_read->w_buffer|| !spi_write_and_read->r_buffer
		|| !spi_write_and_read->n_size)
		return -EINVAL;

	/* Transfer */
	memset(&xfer, 0, sizeof(struct spi_transfer));
	xfer.tx_buf	= spi_write_and_read->w_buffer;
	xfer.rx_buf	= spi_write_and_read->r_buffer;
	xfer.len	= spi_write_and_read->n_size;
	xfer.cs_change	= spi_cfg->cs_change;

	/* Message */
	memset(&msg, 0, sizeof(struct spi_message));
	INIT_LIST_HEAD(&msg.transfers);
	list_add_tail(&xfer.transfer_list, &msg.transfers);
	msg.complete	= ambarella_spi_complete;
	msg.context	= &done;
	spi		= ps[cs_id].spi;
	msg.spi		= spi;

	mutex_lock(&ps[cs_id].mtx);

	/* Config */
	spi->chip_select	= cs_id;
	spi->mode		= spi_cfg->spi_mode;
	spi->mode		&= ~SPI_LOOP;
	spi->bits_per_word	= spi_cfg->cfs_dfs;
	spi->max_speed_hz	= spi_cfg->baud_rate;

	/* Wait */
	spin_lock_irq(&ps[cs_id].lock);
	errorCode = spi->master->transfer(spi, &msg);
	spin_unlock_irq(&ps[cs_id].lock);
	if (!errorCode)
		wait_for_completion(&done);

	mutex_unlock(&ps[cs_id].mtx);

	return errorCode;
}
EXPORT_SYMBOL(ambarella_spi_write_and_read);

