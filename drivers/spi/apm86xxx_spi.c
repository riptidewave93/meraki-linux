/*
 * APM86xxx SPI driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Anup Patel <apatel@apm.com>
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
 * @file apm86xxx_spi.c
 *
 * This file contains the SPI driver for APM86xxx SOCs of AppliedMicro Inc.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/workqueue.h>
#include <linux/completion.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/of_spi.h>
#include <linux/clk.h>
#include "apm86xxx_spi.h"

/* driver details */
#define DRIVER_NAME				"apm86xxx_spi"
#define DRIVER_AUTHOR				"Anup Patel <apatel@apm.com>"
#define DRIVER_DESCRIPTION			"APM86xxx SPI driver"
#define DRIVER_VERSION				"1.0"
#define DRIVER_LICENSE				"GPL"
#define DRIVER_OF_COMPATIBLE			"apm,apm-spi"

static inline u32 apm86xxx_spi_read(void *addr)
{
	return in_be32(addr);
}

static inline void apm86xxx_spi_write(void *addr, u32 data)
{
	out_be32(addr, data);
}

static int apm86xxx_spi_initialize(struct apm86xxx_spi_controller *controller)
{
	u32 val;
	struct clk *clk;
	struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* Reset SPI controller (SOC specific) */
	clk = clk_get(NULL, "spi");
	if (IS_ERR(clk)) {
			return -ENODEV;
	}
	clk_enable(clk);

	/* Get input clock for SPI Controller */
	controller->sysclk = clk_get_rate(clk) * 100;

	/* Check peripheral ID for APM86xxx SPI controller */
	if(apm86xxx_spi_read(&regs->idr)!=APM86xxx_SPI_SIGNATURE) {
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		return -ENXIO;
	}

	/* Configure control register with default values */
	val = 0;
	val |= (APM86xxx_CTRL0_CFS_VAL << CFS0_SHIFT) & CFS0_MASK;
	val |= (APM86xxx_CTRL0_SRL_VAL << SRL0_SHIFT) & SRL0_MASK;
	val |= (APM86xxx_CTRL0_SLV_OE_VAL << SLV_OE0_SHIFT) & SLV_OE0_MASK;
	val |= (APM86xxx_CTRL0_TMOD_EEPROM_VAL << TMOD0_SHIFT) & TMOD0_MASK;
	val |= (APM86xxx_CTRL0_SCPOL_VAL << SCPOL0_SHIFT) & SCPOL0_MASK;
	val |= (APM86xxx_CTRL0_SCPH_VAL << SCPH0_SHIFT) & SCPH0_MASK;
	val |= (APM86xxx_CTRL0_FRF_VAL << FRF0_SHIFT) & FRF0_MASK;
	val |= (APM86xxx_CTRL0_DFS_VAL << DFS0_SHIFT) & DFS0_MASK;

	apm86xxx_spi_write(&regs->ctrlr0,val);
	apm86xxx_spi_write(&regs->ctrlr1,0x0);

	/* Diable APM86xxx SPI controller */
	apm86xxx_spi_write(&regs->ssienr, 0);

	/* Configure interrupt mask */
	apm86xxx_spi_write(&regs->imr,APM86xxx_IMR_VAL);
	
	/* Configure TX threshold */
	apm86xxx_spi_write(&regs->txftlr, APM86xxx_TXFTLR_TFT_VAL & TFT_MASK);

	/* Configure RX threshold */
	apm86xxx_spi_write(&regs->rxftlr, APM86xxx_RXFTLR_RFT_VAL & RFT_MASK);

	/* Return success */
	return 0;
}

static void apm86xxx_spi_load_cs(struct apm86xxx_spi_controller *controller,
				 struct apm86xxx_spi_controller_state *cs)
{
	u32 val;
	struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* Diable APM86xxx SPI controller */
	apm86xxx_spi_write(&regs->ssienr, 0);

	/* Configure APM86xxx SPI controller chip select */
	apm86xxx_spi_write(&regs->ser, (0x1 << cs->chip_select));
	
	/* Configure APM86xxx SPI control register 0 */
	val = apm86xxx_spi_read(&regs->ctrlr0);	
	val |= (cs->xfer_mode << TMOD0_SHIFT) & TMOD0_MASK;
	if(cs->mode & SPI_CPOL) {
		val |= (0x1 << SCPOL0_SHIFT) & SCPOL0_MASK;
	} else {
		val &= ~SCPOL0_MASK;
	}
	if(cs->mode & SPI_CPHA) {
		val |= (0x1 << SCPH0_SHIFT) & SCPH0_MASK;
	} else {
		val &= ~SCPH0_MASK;
	}
	val |= ((cs->bits_per_word-1) << DFS0_SHIFT) & DFS0_MASK;
	apm86xxx_spi_write(&regs->ctrlr0, val);

	/* Configure baude rate */
	val = controller->sysclk / cs->speed_hz;
	if(val == 0) {
		val = 1;
	}
	apm86xxx_spi_write(&regs->baudr,val & SCKDV_MASK);

	/* Enable APM86xxx SPI controller if required */
	if(cs->activated) {
		apm86xxx_spi_write(&regs->ssienr, 1);
	}
} 

static void apm86xxx_spi_activate_cs(struct spi_device *spi)
{
	struct apm86xxx_spi_controller_state *cs = spi->controller_state;
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);

	/* Update controller state */
	cs->activated=1;

	/* Load registers based on controller state */
	apm86xxx_spi_load_cs(controller, cs);

	/* Backup the controller state */
	spin_lock(&controller->state_lock);
	memcpy(&controller->state, cs, sizeof(controller->state));
	spin_unlock(&controller->state_lock);
}

static void apm86xxx_spi_deactivate_cs(struct spi_device *spi)
{
	struct apm86xxx_spi_controller_state *cs = spi->controller_state;
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);
	struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* Update controller state */
	cs->activated = 0;

	/* Diable APM86xxx SPI controller */
	apm86xxx_spi_write(&regs->ssienr, 0);

	/* Backup the controller state */
	spin_lock(&controller->state_lock);
	memcpy(&controller->state, cs, sizeof(controller->state));
	spin_unlock(&controller->state_lock);
}

static int apm86xxx_spi_setup_cs(struct spi_device *spi,
					struct spi_transfer *t)
{
	struct apm86xxx_spi_controller_state *cs = spi->controller_state;

	/* Update controller state */
	if(t != NULL) {
		/* Do sanity checks before trasfer */
		if ((!t->tx_buf && !t->rx_buf && t->len) || !t->len) {
			printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
			return -EINVAL;
		}
		/* Setup controller state for SPI transfer */
		cs->activated = 0;
		cs->mode = spi->mode;
		cs->chip_select = spi->chip_select;
		cs->bits_per_word = (t && t->bits_per_word) ? 
					t->bits_per_word : spi->bits_per_word;
		cs->bits_per_word = ((cs->bits_per_word + 7) / 8) * 8;
		if(t->tx_buf && t->rx_buf && t->len) {
			cs->xfer_mode = APM86xxx_CTRL0_TMOD_TXRX_VAL;
		} else if(!t->tx_buf && t->rx_buf && t->len) {
			cs->xfer_mode = APM86xxx_CTRL0_TMOD_RX_VAL;
		} else if(t->tx_buf && !t->rx_buf && t->len) {
			cs->xfer_mode = APM86xxx_CTRL0_TMOD_TX_VAL;
		}
		if(t->speed_hz==0) {
			cs->speed_hz = spi->max_speed_hz;
		} else {
			cs->speed_hz = (t && t->speed_hz) ? 
					t->speed_hz : spi->max_speed_hz;
		}
	} else {
		/* Default controller state for given SPI device */
		cs->activated = 0;
		cs->mode = spi->mode;
		cs->chip_select = spi->chip_select;
		cs->bits_per_word = spi->bits_per_word;
		cs->xfer_mode = APM86xxx_CTRL0_TMOD_TXRX_VAL;
		cs->speed_hz = spi->max_speed_hz;
	}

	return 0;
}

int apm86xxx_spi_eeprom_read(struct spi_device *spi,
				struct spi_transfer *t1,
				struct spi_transfer *t2)
{
	int retval = 0;
	unsigned long flags;
	u8 *tx, *rx;
	u32 old_ctrl0, old_ctrl1, old_ser, sr, tx_pos, tx_cnt, rx_pos, rx_cnt;
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);
        struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* Save stuff for doing transfer */
	tx_pos = 0;
	tx_cnt = t1->len;
	tx = (u8*)t1->tx_buf;
	rx_pos = 0;
	rx_cnt = t2->len;
	rx = (u8*)t2->rx_buf;

	/* Save old control registers */
	old_ctrl0 = apm86xxx_spi_read(&regs->ctrlr0);
	old_ctrl1 = apm86xxx_spi_read(&regs->ctrlr1);
	old_ser = apm86xxx_spi_read(&regs->ser);

	/* Change control registers */
	apm86xxx_spi_write(&regs->ssienr, 0);
	apm86xxx_spi_write(&regs->ctrlr0, (old_ctrl0 & ~TMOD0_MASK) |
			(APM86xxx_CTRL0_TMOD_EEPROM_VAL << TMOD0_SHIFT));
	apm86xxx_spi_write(&regs->ctrlr1, (rx_cnt) ? (rx_cnt - 1) : 0);
	apm86xxx_spi_write(&regs->ser, 0x0);
	apm86xxx_spi_write(&regs->ssienr, 1);

	/* Write to TX FIFO */
	while (tx_cnt) {
		sr = apm86xxx_spi_read(&regs->sr);
		while (sr & TFNF_MASK) {
			apm86xxx_spi_write(&regs->dr[0], tx[tx_pos]);
			tx_pos++;
			tx_cnt--;
			if (!tx_cnt) break;
			sr = apm86xxx_spi_read(&regs->sr);
		}
	}

	/* Save & disable Local IRQ */
	local_irq_save(flags);

	/* Start the transfer */
	apm86xxx_spi_write(&regs->ser, old_ser);

	/* Read from RX FIFO */
	while (rx_cnt > 0) {
		sr = apm86xxx_spi_read(&regs->sr);
		while (sr & RFNE_MASK) {
			rx[rx_pos] = apm86xxx_spi_read(&regs->dr[0]);
			rx_pos++;
			rx_cnt--;
			if(!rx_cnt) break;
			sr = apm86xxx_spi_read(&regs->sr);
		}
	}

	/* Restore & enable local irq */
	local_irq_restore(flags);

	/* Execute delays */
	if (t1->delay_usecs) {
		udelay(t1->delay_usecs);
	}
	if (t2->delay_usecs) {
		udelay(t2->delay_usecs);
	}

	/* Wait for controller to be ready */
	while (apm86xxx_spi_read(&regs->sr) & BUSY_MASK) ;

	/* Restore old control register */
	apm86xxx_spi_write(&regs->ssienr, 0);
	apm86xxx_spi_write(&regs->ctrlr0, old_ctrl0);
	apm86xxx_spi_write(&regs->ctrlr1, old_ctrl1);
	apm86xxx_spi_write(&regs->ssienr, 1);

	/* Return */
	return retval;
}

static int apm86xxx_spi_tx_dual(struct spi_device *spi,
				struct spi_transfer *t1,
				struct spi_transfer *t2)
{
	unsigned long flags;
	u8 *tx1, *tx2;
	u32 old_ctrl0, old_ser, sr, tx_pos, tx1_cnt, tx2_cnt; 
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);
        struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* Sanity check */
	if (!t1) {
		return -1;
	}

	/* Save stuff for doing transfer */
	tx_pos = 0;
	tx1 = (u8 *)t1->tx_buf;
	tx2 = (t2) ? (u8 *)t2->tx_buf : NULL;
	tx1_cnt = t1->len;
	tx2_cnt = (t2) ? t2->len : 0;

	/* Save old control registers */
	old_ctrl0 = apm86xxx_spi_read(&regs->ctrlr0);
	old_ser = apm86xxx_spi_read(&regs->ser);

	/* Change control registers */
	apm86xxx_spi_write(&regs->ssienr, 0);
	apm86xxx_spi_write(&regs->ctrlr0, (old_ctrl0 & ~TMOD0_MASK) |
			(APM86xxx_CTRL0_TMOD_TXRX_VAL << TMOD0_SHIFT));
	apm86xxx_spi_write(&regs->ser, 0x0);
	apm86xxx_spi_write(&regs->ssienr, 1);

	/* Fill the TX FIFO */ 
	tx_pos = 0;
	while (tx_pos < (tx1_cnt + tx2_cnt)) {
		sr = apm86xxx_spi_read(&regs->sr);
		if (sr & TFNF_MASK) {
			if (tx_pos < tx1_cnt) {
				apm86xxx_spi_write(&regs->dr[0], tx1[tx_pos]);
			} else {
				apm86xxx_spi_write(&regs->dr[0], 
							tx2[tx_pos - tx1_cnt]);
			} 
			tx_pos++;
		} else {
			break;
		}
	}
	
	/* Save & disable Local IRQ */
	local_irq_save(flags);

	/* Enable slaves to start transfer */ 
	apm86xxx_spi_write(&regs->ser, old_ser);
	
	/* Write remaing data to TX FIFO */ 
	while (tx_pos < (tx1_cnt + tx2_cnt)) {
		sr = apm86xxx_spi_read(&regs->sr);
		if (sr & TFNF_MASK) {
			if (tx_pos < tx1_cnt) {
				apm86xxx_spi_write(&regs->dr[0], tx1[tx_pos]);
			} else {
				apm86xxx_spi_write(&regs->dr[0], 
							tx2[tx_pos - tx1_cnt]);
			}
			tx_pos++;
		}
	}

	/* Wait till TX FIFO is not empty */ 
	while (!(apm86xxx_spi_read(&regs->sr) & TFE_MASK)) ;
	
	/* Wait for controller to be ready */
	while (apm86xxx_spi_read(&regs->sr) & BUSY_MASK) ;

 	/* Restore & enable local irq */
	local_irq_restore(flags);

	/* Execute delays */
	if (t1->delay_usecs) {
		udelay(t1->delay_usecs);
	}
	if (t2 && t2->delay_usecs) {
		udelay(t2->delay_usecs);
	}

	/* Restore old control register */
	apm86xxx_spi_write(&regs->ssienr, 0);
	apm86xxx_spi_write(&regs->ctrlr0, old_ctrl0);
	apm86xxx_spi_write(&regs->ssienr, 1);

	return 0;
}

static irqreturn_t apm86xxx_spi_isr(int irq, void *devdata)
{
	/* ISR not used at this moment */
	return IRQ_NONE;
}

int apm86xxx_spi_xfer(struct spi_device *spi,
		      struct spi_transfer *t1,
		      struct spi_transfer *t2)
{
	/* Sanity check of parameters */
	if(t1==NULL && t2==NULL) {
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		return -EINVAL;
	}

	/* Detect and process combined transfers whenever possible */
	if(t1!=NULL && t2!=NULL) {
		if(t1->tx_buf && 
		   t2->rx_buf && 
		   t1->len <= APM86xxx_MAX_FIFO_DEPTH && 
		   t2->len > 0) {
			return apm86xxx_spi_eeprom_read(spi, t1, t2);
		} else if(t1->tx_buf && 
			  t2->tx_buf && 
			  t1->len <= APM86xxx_MAX_FIFO_DEPTH) {
			return apm86xxx_spi_tx_dual(spi, t1, t2);
		}
	} 
	
	/* Do transfer for t1 */
	if(t1!=NULL) {
		if(t1->tx_buf!=NULL) {
			return apm86xxx_spi_tx_dual(spi, t1, NULL);
		}
	}

	/* Do transfer for t2 */
	if(t2!=NULL) {
		if(t2->tx_buf!=NULL) {
			return apm86xxx_spi_tx_dual(spi, t2, NULL);
		}
	}

	/* Return success */
	return 0;
}

static void apm86xxx_spi_worker(struct work_struct *work)
{
	struct apm86xxx_spi_controller *controller = 
		container_of(work, struct apm86xxx_spi_controller, xfer_work);

	/* Lock the controller data and update busy status */
	spin_lock_irq(&controller->lock);
	controller->busy = 1;

	while (!list_empty(&controller->xfer_queue)) {
		struct spi_message *m;
		struct spi_device *spi;
		struct spi_transfer *t = NULL;
		struct spi_transfer *tarr[2] = { NULL, NULL };
		int tcnt=0;
		unsigned cs_change;
		int status;

		/* Retrive SPI message */
		m = container_of(controller->xfer_queue.next, 
				 struct spi_message, queue);
		list_del_init(&m->queue);

		/* Unlock the controller data, since 
		 * we won't access it any further in this iteration */
		spin_unlock_irq(&controller->lock);

		/* Retrive the spi_device */
		spi = m->spi;

		/* Assume controller state has changed */
		cs_change = 1;

		/* Set error status to zero (no error) */
		status = 0;

		/* For each transfer request */
		list_for_each_entry (t, &m->transfers, transfer_list) {
			/* Setup controller state for transfer */
			status = apm86xxx_spi_setup_cs(spi, t);
			if (status < 0) 
				break;

			/* Do the transfer */
			if(cs_change) {
				if(tcnt>0) {
					status = apm86xxx_spi_xfer(spi, 
							tarr[0], tarr[1]);
					if(status)
						break;
					if(tarr[0]) {
						m->actual_length += tarr[0]->len;
					}
					if(tarr[1]) {
						m->actual_length += tarr[1]->len;
					}
				}
				apm86xxx_spi_activate_cs(spi);
				tcnt = 1;
				tarr[0] = t;
				tarr[1] = NULL;
			} else {
				if(tcnt==2) {
					status = apm86xxx_spi_xfer(spi, 
							tarr[0], tarr[1]);
					if(status)
						break;
					if(tarr[0]) {
						m->actual_length += tarr[0]->len;
					}
					if(tarr[1]) {
						m->actual_length += tarr[1]->len;
					}
					tcnt = 0;
					tarr[0] = NULL;
					tarr[1] = NULL;
				} else {
					tarr[tcnt] = t;
					tcnt++;
				}
			}

			/* Determine if we want to change controller state 
			 * for next SPI transfer */
			cs_change = t->cs_change;

			/* Deactivate controller state if required */
			if (cs_change) {
				apm86xxx_spi_deactivate_cs(spi);
			}
		}

		/* Complete any pending saved transfers */
		if(tcnt>0) {
			status = apm86xxx_spi_xfer(spi, tarr[0], tarr[1]);
			if(tarr[0]) {
				m->actual_length += tarr[0]->len;
			}
			if(tarr[1]) {
				m->actual_length += tarr[1]->len;
			}
		}

		/* Save the error status for SPI message */
		m->status = status;
		m->complete(m->context);

		/* Deactivate controller state if required */
		if (status || !cs_change) {
			apm86xxx_spi_deactivate_cs(spi);
		}

		/* Setup controller for no transfer */
		apm86xxx_spi_setup_cs(spi, NULL);

		/* Lock the controller data for next loop iteration */
		spin_lock_irq(&controller->lock);
	}

	/* Unlock the controller data and update busy status */
	controller->busy = 0;
	spin_unlock_irq(&controller->lock);
}

static int apm86xxx_spi_setup(struct spi_device *spi)
{
	unsigned long flags;
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);
	struct apm86xxx_spi_controller_state *cs = spi->controller_state;

	/* Do sainity check of bits per word */
	if (spi->bits_per_word%8) {
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		return -EINVAL;
	}

	/* Do sainity check of MODE bits */
	if (spi->mode & ~APM86xxx_COMPATIBLE_SPI_MODEBITS) {
		dev_dbg(&spi->dev, "setup: unsupported mode bits %x\n",
				spi->mode & ~APM86xxx_COMPATIBLE_SPI_MODEBITS);
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		return -EINVAL;
	}

	/* Allocate controller state for SPI device if required */
	if (!cs) {
		cs = kzalloc(sizeof *cs, GFP_KERNEL);
		if (!cs) return -ENOMEM;
		spi->controller_state = cs;
	}

	/* Initialize controller state for SPI device */
	apm86xxx_spi_setup_cs(spi, NULL);

	/* Deactivate the controller state if controller is not busy */
	spin_lock_irqsave(&controller->lock, flags);
	if (!controller->busy) {
		apm86xxx_spi_deactivate_cs(spi);
	}
	spin_unlock_irqrestore(&controller->lock, flags);

	return 0;
}

static int apm86xxx_spi_transfer(struct spi_device *spi,struct spi_message *m)
{
	unsigned long flags;
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(spi->master);

	/* Update count of data transfered in SPI message */
	m->actual_length = 0;

	/* Update error status in SPI message */
	m->status = -EINPROGRESS;

	/* Update transfer queue and schedule worker thread */
	spin_lock_irqsave(&controller->lock, flags);
	list_add_tail(&m->queue, &controller->xfer_queue);
	queue_work(controller->xfer_workqueue, &controller->xfer_work);
	spin_unlock_irqrestore(&controller->lock, flags);

	return 0;
}

static void apm86xxx_spi_cleanup(struct spi_device *spi)
{
	/* Free the controller state */
	kfree(spi->controller_state);
}

static int __init apm86xxx_spi_of_probe(struct of_device *ofdev,
		const struct of_device_id *match)
{
	struct apm86xxx_spi_controller *controller;
	struct spi_master *master;
	struct resource	res;
	int *bus_num = NULL;
	int retval;

	/* Allocate SPI master from Linux SPI subsystem */
	master = spi_alloc_master(&ofdev->dev, sizeof *controller);
	if (master == NULL) return -ENOMEM;

	/* Set SPI master as driver data of platform device 
	 * in open firmware device */
	dev_set_drvdata(&ofdev->dev, master);

	/* Get controller data from SPI master */
	controller = spi_master_get_devdata(master);

	/* Save pointer to open firmware device for later usage */
	controller->ofdev = ofdev;

	/* Map the SPI registers using open firmware node */
	if (of_address_to_resource(ofdev->node, 0, &res)) {
		dev_err(&ofdev->dev, "%s: Can't get register base address\n",
				ofdev->node->full_name);
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		return -ENODEV;
	}
	controller->regs = ioremap_nocache(res.start, res.end - res.start + 1);
	if (controller->regs == NULL) {
		dev_err(&ofdev->dev, "%s: Can't map device registers!\n",
				ofdev->node->full_name);
		return -ENOMEM;
	}

	/* Allocate irq based on open firmware node */
	controller->irq = irq_of_parse_and_map(ofdev->node, 0);
	retval = request_irq(controller->irq, 
				apm86xxx_spi_isr, 
				IRQF_DISABLED, DRIVER_NAME, 
				controller);
	if (retval) goto free_registers;

	/* Initialize controller spin locks */
	spin_lock_init(&controller->lock);
	spin_lock_init(&controller->state_lock);

	/* Initialize transfer workqueue */
	INIT_WORK(&controller->xfer_work, apm86xxx_spi_worker);
	controller->xfer_workqueue = 
			create_singlethread_workqueue(dev_name(&ofdev->dev));
	if (controller->xfer_workqueue == NULL) {
		printk(KERN_ERR "%s:%d SPI Error\n",__func__,__LINE__);
		retval = -EBUSY;
		goto free_irq;
	}

	/* Initialize transfer queue */
	INIT_LIST_HEAD(&controller->xfer_queue);

	/* Load defaults in backed up controller state */
	controller->state.activated = 0;
	controller->state.mode = SPI_CPOL | SPI_CPHA;
	controller->state.chip_select = 0;
	controller->state.bits_per_word = controller->bits_per_word;
	controller->state.xfer_mode = APM86xxx_CTRL0_TMOD_EEPROM_VAL;
	controller->state.speed_hz = controller->sysclk / 2;

	/* Update information required by SPI master */
	bus_num = (int *)of_get_property(ofdev->node, "bus_num", NULL);
	master->bus_num = (bus_num!=NULL) ? *bus_num : -1;
	master->num_chipselect = APM86xxx_CONFIG_MAX_SLAVE;
	master->mode_bits = APM86xxx_COMPATIBLE_SPI_MODEBITS;
	master->setup = apm86xxx_spi_setup;
	master->transfer = apm86xxx_spi_transfer;
	master->cleanup = apm86xxx_spi_cleanup;
	controller->bus_num = master->bus_num;

	/* Initialize APM86xxx SPI contoller */
	retval = apm86xxx_spi_initialize(controller);
	if (retval < 0) goto free_workqueue;

	/* Register SPI master with Linux SPI subsystem */
	retval = spi_register_master(master);
	if (retval < 0) goto free_workqueue;

	/* Print banner */
	printk("%s: %s\n", ofdev->node->name, DRIVER_NAME);

	/* Probe slave devices */
	of_register_spi_devices(master, ofdev->node);

	return retval;

	/* Free the transfer workqueue */
free_workqueue:
	destroy_workqueue(controller->xfer_workqueue);

	/* Free the transfer irq */
free_irq:
	free_irq(controller->irq, controller);

	/* Free the memory mapped registers */
free_registers:
	if (controller->regs) iounmap(controller->regs);

	/* Free the SPI master to Linux SPI subsystem */
	spi_master_put(master);

	return retval;
}

static int __exit apm86xxx_spi_of_remove(struct of_device *op)
{
	/* Retrive the SPI master */
	struct spi_master *master = dev_get_drvdata(&op->dev);

	/* Retrive the SPI controller from device data */
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(master);

	/* Complete any pending work */
	flush_workqueue(controller->xfer_workqueue);

	/* Free the transfer workqueue */
	destroy_workqueue(controller->xfer_workqueue);

	/* Unregister SPI master to Linux SPI subsystem */
	spi_unregister_master(master);

	/* Free the transfer irq */
	free_irq(controller->irq, controller);

	/* Free the memory mapped registers */
	if (controller->regs) iounmap(controller->regs);

	return 0;
}

#ifdef CONFIG_PM

static int apm86xxx_spi_suspend(struct of_device * op, pm_message_t state)
{
	/* Retrive the SPI master */
	struct spi_master *master = dev_get_drvdata(&op->dev);

	/* Retrive the SPI controller from device data */
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(master);

	/* Retrive SPI registers from SPI controller */
	struct apm86xxx_spi_regs __iomem *regs = controller->regs;

	/* We don't need to do any thing here */
	/* The controller state is always backed up when it changes */

	/* Just diable APM86xxx SPI controller and return */
	apm86xxx_spi_write(&regs->ssienr, 0);

	return 0;
}

static int apm86xxx_spi_resume(struct of_device* op)
{
	/* Retrive the SPI master */
	struct spi_master *master = dev_get_drvdata(&op->dev);

	/* Retrive the SPI controller from device data */
	struct apm86xxx_spi_controller *controller = 
					spi_master_get_devdata(master);

	/* Initialize APM86xxx SPI controller */
	apm86xxx_spi_initialize(controller);

	/* Load registers based on controller state */
	apm86xxx_spi_load_cs(controller, &controller->state);

	return 0;
}

#else

#define apm86xxx_spi_suspend NULL
#define apm86xxx_spi_resume NULL

#endif /* CONFIG_PM */

static struct of_device_id apm86xxx_spi_of_match[] = {
	{ .compatible = DRIVER_OF_COMPATIBLE },
	{}
};

MODULE_DEVICE_TABLE(of, apm86xxx_spi_of_match);

static struct of_platform_driver apm86xxx_spi_of_driver = {
	.owner = THIS_MODULE,
	.name = DRIVER_NAME,
	.match_table = apm86xxx_spi_of_match,
	.probe = apm86xxx_spi_of_probe,
	.remove = __exit_p(apm86xxx_spi_of_remove),
	.suspend = apm86xxx_spi_suspend,
	.resume	= apm86xxx_spi_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init apm86xxx_spi_init(void)
{
	return of_register_platform_driver(&apm86xxx_spi_of_driver);
}

static void __exit apm86xxx_spi_exit(void)
{
	of_unregister_platform_driver(&apm86xxx_spi_of_driver);
}

module_init(apm86xxx_spi_init);
module_exit(apm86xxx_spi_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_LICENSE(DRIVER_LICENSE);
