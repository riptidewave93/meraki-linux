/*
 * linux/drivers/spi/spi-ambarella-polling.c
 *
 * History: 
 *	2013/09/17 - [Zhenwu Xue] Initial Version

 *
 * Copyright (C) 2012-2016, Ambarella, Inc.
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
	spinlock_t				lock;
	
	struct ambarella_spi_platform_info	*pinfo;

	u32					regbase;
	u32					ctrlr0;
	u32					baudr;
	u32					dr;
	u32					ssienr;
	u32					ser;
	u32					sr;
	u32					rxflr;

	u32					cs_ser;
	u32					cs_gpio;
	u32					cs_high;
	u32					cs_enable;

	u32					byte_entry;
};

struct spi_master *masters[SPI_MASTER_INSTANCES] = {NULL};

static int ambarella_spi_inithw(struct ambarella_spi *bus)
{
	struct ambarella_spi_platform_info	*pinfo;

	pinfo = bus->pinfo;
	pinfo->rct_set_ssi_pll();

	amba_writel(bus->ser, 0);
	amba_writel(bus->ssienr, 0);

	return 0;
}

static int ambarella_spi_setup(struct spi_device *spi, 
	struct ambarella_spi *bus)
{
	spi_ctrl0_reg_t			ctrlr0;
	u32 				ssi_clk, sckdv;
	
	ctrlr0.w	= amba_readl(bus->ctrlr0);	
	ctrlr0.s.dfs	= spi->bits_per_word - 1;
	if (spi->mode & SPI_CPHA) {
		ctrlr0.s.scph	= 1;
	} else {
		ctrlr0.s.scph	= 0;
	}
	if (spi->mode & SPI_CPOL) {
		ctrlr0.s.scpol	= 1;
	} else {
		ctrlr0.s.scpol	= 0;
	}
	if (spi->mode & SPI_LOOP) {
		ctrlr0.s.srl	= 1;
	} else {
		ctrlr0.s.srl	= 0;
	}
	ctrlr0.s.tmod		= 0;
	amba_writel(bus->ctrlr0, ctrlr0.w);

	ssi_clk	= bus->pinfo->get_ssi_freq_hz();
	if(spi->max_speed_hz > ssi_clk / 2) {
	    spi->max_speed_hz = ssi_clk / 2;
	}
	sckdv = (ssi_clk / spi->max_speed_hz + 1) & 0xfffe;
	amba_writel(bus->baudr, sckdv);
	
	return 0;
}

static int ambarella_spi_polling(struct ambarella_spi *bus)
{
	spi_status_reg_t	status;
	
	do {
		status.w = amba_readl(bus->sr);
	} while(status.s.busy || !status.s.tfe);

	return 0;
}

static int __ambarella_spi_transfer(struct ambarella_spi *bus, 
	struct spi_transfer *xfer)
{
	const void		*wbuf;
	void			*rbuf;
	const u8		*pw8;
	u8			*pr8;
	const u16		*pw16;
	u16			*pr16;
	u32			widx, ridx, len, total_len;
	u32			entries;
	u32			i;
	
	wbuf	= xfer->tx_buf;
	rbuf	= xfer->rx_buf;
	pw8	= wbuf;
	pr8	= rbuf;
	pw16	= wbuf;
	pr16	= rbuf;	

	amba_writel(bus->ssienr, 1);
	amba_writel(bus->ser, 0);

	entries		= bus->pinfo->fifo_entries;
	total_len	= xfer->len;
	if (!bus->byte_entry) {
		total_len >>= 1;
	}
	if (total_len <= entries) {
		len = total_len;
	} else {
		len = entries;
	}
	if (wbuf) {
		for (i = 0; i < len; i++) {
			if (bus->byte_entry) {
				amba_writel(bus->dr, pw8[i]);
			} else {
				amba_writel(bus->dr, pw16[i]);
			}			
		}
	} else {
		for (i = 0; i < len; i++) {
			amba_writel(bus->dr, SPI_DUMMY_DATA);
		}
	}
	widx	= len;
	ridx	= 0;
	
	if (!bus->cs_enable) {
		if (!bus->cs_high) {
			ambarella_gpio_set(bus->cs_gpio, GPIO_LOW);
		} else {
			ambarella_gpio_set(bus->cs_gpio, GPIO_HIGH);
		}
		bus->cs_enable = 1;
	}

	amba_writel(bus->ser, bus->cs_ser);
	
	while (1) {
		ambarella_spi_polling(bus);
		
		if (rbuf) {
			len = amba_readl(bus->rxflr);
			for (i = 0; i < len; i++) {
				if (bus->byte_entry) {
					pr8[ridx + i] = amba_readl(bus->dr);
				} else {
					pr16[ridx + i] = amba_readl(bus->dr);
				}
			}
			ridx += len;

			if (ridx >= total_len) {
				break;
			}
		} else {
			if (widx >= total_len) {
				break;
			}
		}
		
		amba_writel(bus->ser, 0);

		len = total_len - widx;
		if (len > entries) {
			len = entries;
		}
		if (wbuf) {
			for (i = 0; i < len; i++) {
				if (bus->byte_entry) {
					amba_writel(bus->dr, pw8[widx + i]);
				} else {
					amba_writel(bus->dr, pw16[widx + i]);
				}			
			}
		} else {
			for (i = 0; i < len; i++) {
				amba_writel(bus->dr, SPI_DUMMY_DATA);
			}
		}
		widx	+= len;
		
		amba_writel(bus->ser, bus->cs_ser);
	}

	amba_writel(bus->ser, 0);
	amba_writel(bus->ssienr, 0);

	if (xfer->cs_change) {
		if (!bus->cs_high) {
			ambarella_gpio_set(bus->cs_gpio, GPIO_HIGH);
		} else {
			ambarella_gpio_set(bus->cs_gpio, GPIO_LOW);
		}
		bus->cs_enable = 0;
	}
	
	return 0;
}

static int ambarella_spi_transfer(struct spi_master *master, 
	struct spi_message *msg)
{
	int				err = 0;
	struct spi_device		*spi;
	struct ambarella_spi		*bus;
	struct spi_transfer		*xfer;

	spi = msg->spi;
	
	/* Check Argument */
	if (!spi->max_speed_hz) {
		err = -EINVAL;
		goto ambarella_spi_transfer_exit;
	}

	if (spi->bits_per_word < 4 || spi->bits_per_word > 16) {
		err = -EINVAL;
		goto ambarella_spi_transfer_exit;
	}

	if (spi->chip_select >= spi->master->num_chipselect) {
		err = -EINVAL;
		goto ambarella_spi_transfer_exit;
	}

	if (list_empty(&msg->transfers)) {
		err = -EINVAL;
		goto ambarella_spi_transfer_exit;
	}

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (!xfer->tx_buf && !xfer->rx_buf) {
			err = -EINVAL;
			goto ambarella_spi_transfer_exit;
		}

		if (spi->bits_per_word > 8 && (xfer->len & 0x1)) {
			err = -EINVAL;
			goto ambarella_spi_transfer_exit;
		}
	}
	
	bus = spi_master_get_devdata(master);
	
	spin_lock_irq(&bus->lock);

	bus->cs_ser	= 1 << spi->chip_select;
	bus->cs_gpio	= bus->pinfo->cs_pins[spi->chip_select];
	bus->cs_high	= spi->mode & SPI_CS_HIGH;
	bus->cs_enable	= 0;
	ambarella_gpio_config(bus->cs_gpio, GPIO_FUNC_SW_OUTPUT);

	if (spi->bits_per_word <= 8) {
		bus->byte_entry = 1;
	} else {
		bus->byte_entry = 0;
	}

	ambarella_spi_setup(spi, bus);

	msg->status		= 0;
	msg->actual_length	= 0;
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		__ambarella_spi_transfer(bus, xfer);
		msg->actual_length	+= xfer->len;		
	}

	if (bus->cs_enable) {
		if (!bus->cs_high) {
			ambarella_gpio_set(bus->cs_gpio, GPIO_HIGH);
		} else {
			ambarella_gpio_set(bus->cs_gpio, GPIO_LOW);
		}
		bus->cs_enable = 0;
	}
	
	spin_unlock_irq(&bus->lock);	
	
	spi_finalize_current_message(master);

ambarella_spi_transfer_exit:
	return err;
}

static int ambarella_spi_transfer2(struct spi_device *spi, 
	struct spi_transfer *xfer)
{
	int				err = 0;
	struct ambarella_spi		*bus;
	
	/* Check Argument */
	if (!spi->max_speed_hz) {
		err = -EINVAL;
		goto ambarella_spi_transfer2_exit;
	}

	if (spi->bits_per_word < 4 || spi->bits_per_word > 16) {
		err = -EINVAL;
		goto ambarella_spi_transfer2_exit;
	}

	if (spi->chip_select >= spi->master->num_chipselect) {
		err = -EINVAL;
		goto ambarella_spi_transfer2_exit;
	}
	
	if (!xfer->tx_buf && !xfer->rx_buf) {
		err = -EINVAL;
		goto ambarella_spi_transfer2_exit;
	}

	if (spi->bits_per_word > 8 && (xfer->len & 0x1)) {
		err = -EINVAL;
		goto ambarella_spi_transfer2_exit;
	}	
	
	bus = spi_master_get_devdata(spi->master);
	
	spin_lock_irq(&bus->lock);

	bus->cs_ser	= 1 << spi->chip_select;
	bus->cs_gpio	= bus->pinfo->cs_pins[spi->chip_select];
	bus->cs_high	= spi->mode & SPI_CS_HIGH;
	bus->cs_enable	= 0;
	ambarella_gpio_config(bus->cs_gpio, GPIO_FUNC_SW_OUTPUT);

	if (spi->bits_per_word <= 8) {
		bus->byte_entry = 1;
	} else {
		bus->byte_entry = 0;
	}

	ambarella_spi_setup(spi, bus);	
	
	__ambarella_spi_transfer(bus, xfer);	

	if (bus->cs_enable) {
		if (!bus->cs_high) {
			ambarella_gpio_set(bus->cs_gpio, GPIO_HIGH);
		} else {
			ambarella_gpio_set(bus->cs_gpio, GPIO_LOW);
		}
		bus->cs_enable = 0;
	}
	
	spin_unlock_irq(&bus->lock);

ambarella_spi_transfer2_exit:
	return err;
}

int ambarella_spi_write(amba_spi_cfg_t *cfg, amba_spi_write_t *write)
{
	int				err = 0;
	struct spi_device		spi;
	struct spi_transfer		xfer;
	struct spi_master		*master;
	
	if (!cfg || !write) {
		err = -EINVAL;
		goto ambarella_spi_write_exit;
	}

	if (write->bus_id >= SPI_MASTER_INSTANCES) {
		err = -EINVAL;
		goto ambarella_spi_write_exit;
	}

	master = masters[write->bus_id];
	if (!master) {
		err = -EINVAL;
		goto ambarella_spi_write_exit;
	}

	spi.master		= master;
	
	spi.chip_select		= write->cs_id;
	spi.mode		= cfg->spi_mode;
	spi.bits_per_word	= cfg->cfs_dfs;
	spi.max_speed_hz	= cfg->baud_rate;
	
	memset(&xfer, 0, sizeof(xfer));
	xfer.tx_buf	= write->buffer;
	xfer.len	= write->n_size;
	xfer.cs_change	= cfg->cs_change;

	err = ambarella_spi_transfer2(&spi, &xfer);
	
ambarella_spi_write_exit:
	return err;
}
EXPORT_SYMBOL(ambarella_spi_write);

int ambarella_spi_read(amba_spi_cfg_t *cfg, amba_spi_read_t *read)
{
	int				err = 0;
	struct spi_device		spi;
	struct spi_transfer		xfer;
	struct spi_master		*master;
	
	if (!cfg || !read) {
		err = -EINVAL;
		goto ambarella_spi_read_exit;
	}

	if (read->bus_id >= SPI_MASTER_INSTANCES) {
		err = -EINVAL;
		goto ambarella_spi_read_exit;
	}

	master = masters[read->bus_id];
	if (!master) {
		err = -EINVAL;
		goto ambarella_spi_read_exit;
	}

	spi.master		= master;
	
	spi.chip_select		= read->cs_id;
	spi.mode		= cfg->spi_mode;
	spi.bits_per_word	= cfg->cfs_dfs;
	spi.max_speed_hz	= cfg->baud_rate;
	
	memset(&xfer, 0, sizeof(xfer));
	xfer.rx_buf	= read->buffer;
	xfer.len	= read->n_size;
	xfer.cs_change	= cfg->cs_change;

	err = ambarella_spi_transfer2(&spi, &xfer);
	
ambarella_spi_read_exit:
	return err;
}
EXPORT_SYMBOL(ambarella_spi_read);

int ambarella_spi_write_and_read(amba_spi_cfg_t *cfg, 
	amba_spi_write_and_read_t *write_and_read)
{
	int				err = 0;
	struct spi_device		spi;
	struct spi_transfer		xfer;
	struct spi_master		*master;
	
	if (!cfg || !write_and_read) {
		err = -EINVAL;
		goto ambarella_spi_write_and_read_exit;
	}

	if (write_and_read->bus_id >= SPI_MASTER_INSTANCES) {
		err = -EINVAL;
		goto ambarella_spi_write_and_read_exit;
	}

	master = masters[write_and_read->bus_id];
	if (!master) {
		err = -EINVAL;
		goto ambarella_spi_write_and_read_exit;
	}

	spi.master		= master;
	
	spi.chip_select		= write_and_read->cs_id;
	spi.mode		= cfg->spi_mode;
	spi.bits_per_word	= cfg->cfs_dfs;
	spi.max_speed_hz	= cfg->baud_rate;
	
	memset(&xfer, 0, sizeof(xfer));
	xfer.tx_buf	= write_and_read->w_buffer;
	xfer.rx_buf	= write_and_read->r_buffer;
	xfer.len	= write_and_read->n_size;
	xfer.cs_change	= cfg->cs_change;

	err = ambarella_spi_transfer2(&spi, &xfer);
	
ambarella_spi_write_and_read_exit:
	return err;
}
EXPORT_SYMBOL(ambarella_spi_write_and_read);

int ambarella_spi_write_then_read(amba_spi_cfg_t *cfg, 
	amba_spi_write_then_read_t *write_then_read)
{
	int				err = 0;
	struct spi_device		spi;
	struct spi_transfer		xfer;
	struct spi_master		*master;
	int				sz;
	char				*buf;
	
	if (!cfg || !write_then_read) {
		err = -EINVAL;
		goto ambarella_spi_write_then_read_exit;
	}

	if (write_then_read->bus_id >= SPI_MASTER_INSTANCES) {
		err = -EINVAL;
		goto ambarella_spi_write_then_read_exit;
	}

	master = masters[write_then_read->bus_id];
	if (!master) {
		err = -EINVAL;
		goto ambarella_spi_write_then_read_exit;
	}

	sz = write_then_read->w_size + write_then_read->r_size;
	buf = kzalloc(sz, GFP_KERNEL);
	if (!buf) {
		err = -EINVAL;
		goto ambarella_spi_write_then_read_exit;
	} else {
		memcpy(buf, write_then_read->w_buffer, write_then_read->w_size);
	}

	spi.master		= master;
	
	spi.chip_select		= write_then_read->cs_id;
	spi.mode		= cfg->spi_mode;
	spi.bits_per_word	= cfg->cfs_dfs;
	spi.max_speed_hz	= cfg->baud_rate;
	
	memset(&xfer, 0, sizeof(xfer));
	xfer.tx_buf	= buf;
	xfer.rx_buf	= buf;
	xfer.len	= sz;
	xfer.cs_change	= cfg->cs_change;

	err = ambarella_spi_transfer2(&spi, &xfer);

	memcpy(write_then_read->r_buffer, buf + write_then_read->w_size,
		write_then_read->r_size);
	kfree(buf);
	
ambarella_spi_write_then_read_exit:
	return err;
}
EXPORT_SYMBOL(ambarella_spi_write_then_read);

static int ambarella_spi_probe(struct platform_device *pdev)
{
	struct ambarella_spi			*bus;
	struct spi_master			*master;
	struct resource 			*res;
	struct ambarella_spi_platform_info	*pinfo;
	int					err = 0;	

	/* Get Platform Info */
	pinfo = (struct ambarella_spi_platform_info *)pdev->dev.platform_data;
	if (!pinfo) {
		err = -EINVAL;
		goto ambarella_spi_probe_exit;
	}
	if (pinfo->cs_num && !pinfo->cs_pins) {
		err = -EINVAL;
		goto ambarella_spi_probe_exit;
	}
	if (!pinfo->cs_activate || !pinfo->cs_deactivate) {
		err = -EINVAL;
		goto ambarella_spi_probe_exit;
	}
	if (!pinfo->rct_set_ssi_pll || !pinfo->get_ssi_freq_hz) {
		err = -EINVAL;
		goto ambarella_spi_probe_exit;
	}

	/* Get Base Address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		err = -EINVAL;
		goto ambarella_spi_probe_exit;
	}

	/* Alocate Master */
	master = spi_alloc_master(&pdev->dev, sizeof(*bus));
	if (!master) {
		err = -ENOMEM;
		goto ambarella_spi_probe_exit;
	}

	/* Initalize Device Data */
	master->bus_num			= pdev->id;
	master->num_chipselect		= pinfo->cs_num;
	master->mode_bits		= SPI_CPHA | SPI_CPOL | SPI_LOOP;
	master->transfer_one_message	= ambarella_spi_transfer;
	
	bus			= spi_master_get_devdata(master);	
	bus->pinfo		= pinfo;
	spin_lock_init(&bus->lock);
	bus->regbase		= (u32)res->start;
	bus->ctrlr0		= bus->regbase + SPI_CTRLR0_OFFSET;
	bus->baudr		= bus->regbase + SPI_BAUDR_OFFSET;
	bus->dr			= bus->regbase + SPI_DR_OFFSET;
	bus->ssienr		= bus->regbase + SPI_SSIENR_OFFSET;
	bus->ser		= bus->regbase + SPI_SER_OFFSET;
	bus->sr			= bus->regbase + SPI_SR_OFFSET;
	bus->rxflr		= bus->regbase + SPI_RXFLR_OFFSET;

	ambarella_spi_inithw(bus);

	/* Register Master */
	err = spi_register_master(master);
	if (err) {
		goto ambarella_spi_probe_exit;
	}

	if (master->bus_num < SPI_MASTER_INSTANCES) {
		masters[master->bus_num] = master;
	}

ambarella_spi_probe_exit:
	return err;
}

static int ambarella_spi_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver ambarella_spi_driver = {
	.probe		= ambarella_spi_probe,
	.remove		= ambarella_spi_remove,
	.driver		= {
		.name	= "ambarella-spi",
		.owner	= THIS_MODULE,
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
MODULE_AUTHOR("Zhenwu Xue, <zwxue@ambarella.com>");
MODULE_LICENSE("GPL");
