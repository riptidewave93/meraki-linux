/*
 * spi_eeprom.c
 *
 * 2003-2005 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/mtd/partitions.h>	/* for mtd partitions */

//#undef SCP_DEBUG
#define SCP_DEBUG
#include "scp-dev.h"
DEFINE_SPINLOCK(spi_eeprom_lock);

static struct spi_dev_desc eeprom_dev_desc = { 
    .baud = 1500000,		/* 1.5Mbps */
    .tcss = 10,
    .tcsh = 10,
    .tcsr = 1,
    .byteorder = 1,		/* MSB-First */
    .polarity = 0,		/* High-Active */
    .phase = 0,			/* Sample-Then-Shift */

};

static struct mtd_info beech_spi_mtd[NUM_SPI_SLAVES];

static struct mtd_partition spi_partition_allmem[] = {
    {
     .name = "scp",
     .offset = 0x0,
     .size = 0x400,
     },
};

static int
spi_eeprom_io(int chipid,
	      const unsigned char **inbufs, unsigned int *incounts,
	      unsigned char **outbufs, unsigned int *outcounts)
{
    return scp_io(chipid, &eeprom_dev_desc,
		  inbufs, incounts, outbufs, outcounts, 0);
}

int spi_eeprom_write_enable(int chipid, int enable)
{
    unsigned char inbuf[1];
    const unsigned char *inbufs[2];
    unsigned int incounts[2], outcounts[1];
    unsigned long flags;
    int stat;
    
    inbuf[0] = enable ? ATMEL_WREN : ATMEL_WRDI;
    inbufs[0] = inbuf;
    incounts[0] = sizeof(inbuf);
	inbufs[1] = NULL;
	incounts[1] = 0;
	outcounts[0] = 0;
    spin_lock_irqsave(&spi_eeprom_lock, flags);
    stat = spi_eeprom_io(chipid, inbufs, incounts, NULL, outcounts);
    spin_unlock_irqrestore(&spi_eeprom_lock, flags);
    return stat;
}

static int spi_eeprom_read_status_nolock(int chipid)
{
    unsigned char inbuf[1], outbuf[1];
    const unsigned char *inbufs[2];
    unsigned char *outbufs[1];
    unsigned int incounts[2], outcounts[1];
    int stat;

    inbuf[0] = ATMEL_RDSR;
    inbufs[0] = inbuf;
    incounts[0] = sizeof(inbuf);
	incounts[1] = 0;
    outbufs[0] = outbuf;
    outcounts[0] = sizeof(outbuf);
    stat = spi_eeprom_io(chipid, inbufs, incounts, outbufs, outcounts);
    if (stat < 0) {
		printk("KERNEL_DEBUG %s bail\n", __FUNCTION__);
		return stat;
	}
    return outbuf[0];
}

int spi_eeprom_read_status(int chipid)
{
    unsigned long flags;
    int stat;

    spin_lock_irqsave(&spi_eeprom_lock, flags);
    stat = spi_eeprom_read_status_nolock(chipid);
    spin_unlock_irqrestore(&spi_eeprom_lock, flags);
    return stat;
}


int spi_eeprom_read(struct mtd_info *mtd, ulong address, u_char * buf,
    int len)
{
    unsigned char inbuf[3];
    const unsigned char *inbufs[2];
    unsigned char *outbufs[1];
    unsigned int incounts[2], outcounts[1];
    unsigned long flags;
    int stat;

	SPI_DEV *spi_dev = mtd->priv;

	address &= 0xffff;
    inbuf[0] = ATMEL_READ;
    inbuf[1] = address >> 8;
	inbuf[2] = address & 0xff;

    inbufs[0] = inbuf;
    incounts[0] = sizeof(inbuf);
	inbufs[1] = NULL;
	incounts[1] = 0;
    outbufs[0] = buf;
    outcounts[0] = len;
    spin_lock_irqsave(&spi_eeprom_lock, flags);
    stat = spi_eeprom_io(spi_dev->slaveid, inbufs, incounts,
		 outbufs, outcounts);
    spin_unlock_irqrestore(&spi_eeprom_lock, flags);
    return stat;
}

int spi_eeprom_write(struct mtd_info *mtd, ulong address,
		     const u_char * buf, int len)
{
    unsigned char inbuf[3];
    const unsigned char *inbufs[3];
    unsigned int incounts[3], outcounts[1];
    unsigned long flags;
    int i, stat, w_len, remain;

	SPI_DEV *spi_dev = mtd->priv;

	remain = len;
	while (remain > 0)  {
	    stat = spi_eeprom_write_enable(spi_dev->slaveid, 1);
    	if (stat < 0)
			return stat;
	    stat = spi_eeprom_read_status(spi_dev->slaveid);
	    if (stat < 0)
			return stat;
	    if (!(stat & ATMEL_SR_WEN))
			return -EPERM;

	    inbuf[0] = ATMEL_WRITE;
		address &= 0xffff;
	    inbuf[1] = address >> 8;
		inbuf[2] = address & 0xff;

	    inbufs[0] = inbuf;
	    inbufs[1] = buf;
		inbufs[2] = NULL;

   		incounts[0] = sizeof(inbuf);
		if (address & (ATMEL_WRITE_SIZE-1)) {
			w_len = ATMEL_WRITE_SIZE - (address & (ATMEL_WRITE_SIZE-1));
			if (remain <= w_len) {
				w_len = remain;
			}
		}
		else {
			w_len = (remain>= ATMEL_WRITE_SIZE) ?  ATMEL_WRITE_SIZE:remain;
		}
		remain -= w_len;
		address += w_len;
		buf += w_len;

	    incounts[1] = w_len;
		incounts[2] = 0;

		outcounts[0] = 0;
    	spin_lock_irqsave(&spi_eeprom_lock, flags);
	    stat = spi_eeprom_io(spi_dev->slaveid, inbufs, incounts, 
			NULL, outcounts);
    	if (stat < 0)
			goto unlock_return;
    	/* write start.  max 10ms */
	    for (i = 5; i > 0; i--) {
			mdelay(2);
			stat = spi_eeprom_read_status_nolock(spi_dev->slaveid);
#ifdef SCP_DEBUG
			printk("w%d 0x%x\n",i, stat);
#endif
			if (stat < 0)
	    		goto unlock_return;
			if (!(stat & ATMEL_SR_BSY)) {
				/* Should bail out here */
				//printk("bail ATMEL_SR_BSY\n");
		    	break;
			}
    	}
	    spin_unlock_irqrestore(&spi_eeprom_lock, flags);
    	if (i == 0) {
			printk(KERN_DEBUG  "bail -EIO\n");
			break;
			//return -EIO;
		}
	}
    return (len-remain);
unlock_return:
	printk("bail unlock_return\n");
    spin_unlock_irqrestore(&spi_eeprom_lock, flags);
    return stat;
}


/*******************************************************************************
* Function spi_read
* This function provides the mtd interface to read the buffer from the SPI flash
* at the offset
*******************************************************************************/
int
spi_read(struct mtd_info *mtd, loff_t from, size_t len,
	 size_t * retlen, u_char * buf)
{
    int err = 0;
    SPI_DEV *this = mtd->priv;
#ifdef SCP_DEBUG
    printk(KERN_DEBUG "spi_read: from= 0x%08x, len= %d, buf= %p\n",
	   (uint) from, len, buf);
#endif
    err = this->read_buf(mtd, from, buf, len);
    if (err) {
		printk(KERN_DEBUG "spi_read: read_data failed with err %d\n", err);
		goto out;
    }

    /* Tell the MTD device how many bytes have been read */
    *retlen = len;

out:
    return err;
}

/*******************************************************************************
* Function spi_write
* This function provides the mtd interface to write the buffer to the SPI flash
* at the offset
*******************************************************************************/
int
spi_write(struct mtd_info *mtd, loff_t to, size_t len,
	  size_t * retlen, const u_char * buf)
{
    int err = 0;
    SPI_DEV *this = mtd->priv;

    printk(KERN_DEBUG "spi_write: to= 0x%08x, len= %d, buf= %p\n",
	   (uint) to, len, buf);
    err = this->write_buf(mtd, to, buf, len);
#ifdef SCP_DEBUG
    printk(KERN_DEBUG "spi_write: write_buf returned %d\n", err);
#endif
    return err;
}


/*******************************************************************************
* Function spi_scan
* This function scans for the existence of the slave SPI Flash device
* by reading the Manufacturer ID, Memory Type and Chip Size.
*******************************************************************************/
int spi_scan(struct mtd_info *mtd)
{
#ifdef SCP_DEBUG
    printk(KERN_DEBUG "spi_scan()\n");
#endif
    /* Fill in MTD driver data */
	mtd->type = MTD_DATAFLASH;
    mtd->read = spi_read;
    mtd->write = spi_write;
    mtd->lock = NULL;
    mtd->unlock = NULL;
	mtd->writesize=0x00100000;
	mtd->size = 0x00100000;
	mtd->name = "AMCC-SCP";
	mtd->flags |= MTD_WRITEABLE;
    mtd->owner = THIS_MODULE;
	mtd->erasesize = 32;
    return 0;
}


/*******************************************************************************
 * Function: pine_scp_init
 * This function is called by the LiveOak SSI device layer
 * to initialize the device.
*******************************************************************************/
int beech_scp_init(SPI_DEV * this)
{
    int id = this->slaveid;
    int err = 0;

#ifdef SCP_DEBUG
    printk(KERN_DEBUG "beech_spi_init() : %s slave id : %d\n",
	   DEVICE_NAME, id);
#endif
    memset(&(beech_spi_mtd[id]), 0, sizeof(beech_spi_mtd[id]));

    /* Link the private data with the MTD structure */
    beech_spi_mtd[id].priv = (void *) this;

    this->chip_delay = 100;

    this->read_buf = spi_eeprom_read;	
    this->write_buf = spi_eeprom_write;	

    /* Init wait queue */
    //init_waitqueue_head(&this->wq);

    /* Scan to find existence of SPI Flash device */
    if ( (err = spi_scan(&beech_spi_mtd[id])) != 0)	{
		goto err;
	}

	/* Adr MTD partition onto SPI device */
    mtd_device_register(&beech_spi_mtd[id], spi_partition_allmem,
			sizeof(spi_partition_allmem) /
			sizeof(spi_partition_allmem[0]));
err:
    return err;
}

/****************************************************************************
 * Function: pine_spi_cleanup
 * This function is called by the Pine SCP device layer
 * on exit.
*****************************************************************************/
void beech_spi_cleanup(void)
{
    int i;
#ifdef SCP_DEBUG
    printk(KERN_DEBUG "beech_spi_cleanup()\n");
#endif
	for(i = 0; i < NUM_SPI_SLAVES; i++) {
		/* Unregister partitions */
		mtd_device_unregister(&beech_spi_mtd[i]);
	}
}
