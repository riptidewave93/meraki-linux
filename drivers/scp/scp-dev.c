/*
 **************************************************************************
 * drivers/spi/scp-dev.c -- Serial Communications Port
 *
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *
 * (C) Copyright IBM Corp. 2004 Christophe Lombard <christophe_lombard@fr.ibm.com>
 *
 *
 * Usage:
 * Transmit data to scp
 * echo -n "..." > /proc/driver/scp_dev
 * Example: Write and read the value 0x19 at the address 0x1EE

   echo "Status Read "

   echo -n "CSENA" > /proc/driver/scp_dev   # Enable FRAM
   echo -n "RDSR" > /proc/driver/scp_dev    # RDSR: 0x05 to read FRAM status
   echo -n "0" > /proc/driver/scp_dev
   echo -n "RXDATA" > /proc/driver/scp_dev  # read data
   echo -n "CSDIS" > /proc/driver/scp_dev   # Disable FRAM


   #echo "write data "

   echo -n "CSENA" > /proc/driver/scp_dev # Enable FRAM
   echo -n "WREN" > /proc/driver/scp_dev  # WREN: 0x06
   echo -n "CSDIS" > /proc/driver/scp_dev # Disable FRAM

   echo -n "CSENA" > /proc/driver/scp_dev  # Enable FRAM
   echo  -n "WRITE" > /proc/driver/scp_dev  # WRITE: 0x02
   echo -n "01" > /proc/driver/scp_dev     # write address on 10 bits. Ex: 0x17EE (max : 3FF)
   echo -n "EE" > /proc/driver/scp_dev
   echo -n "19" > /proc/driver/scp_dev     # write data on 8 bits. Ex: 19
   echo -n "CSDIS" > /proc/driver/scp_dev  # Disable FRAM


   #echo "read data "

   echo -n "CSENA" > /proc/driver/scp_dev    # Enable FRAM
   echo -n "READ" > /proc/driver/scp_dev     # READ: 0x03
   echo -n "1" > /proc/driver/scp_dev	     # read address on 10 bits. Ex: 0x1EE
   echo -n "EE" > /proc/driver/scp_dev
   echo -n "0" > /proc/driver/scp_dev        # write dummy data: 0 (Mandatory)
   echo -n "RXDATA" > /proc/driver/scp_dev   # read data
   echo -n "CSDIS" > /proc/driver/scp_dev    # Disable FRAM
 *
 * It is impotant to know the example to know how the SCP works.
 * On Beech board the SPI EEPROM is AT25080A. This affects the max address.
 * ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/platform_device.h>
#include <asm/dcr-native.h>
#include <linux/of_platform.h>

#undef SCP_DEBUG
#define SCP_DEBUG
#include "scp-dev.h"

/*
 * If you want debugging uncomment: 
 */
#ifdef SCP_DEBUG
int scp_verbose = 1;
#endif

#define DRIVER_VERSION "0.1"

MODULE_DESCRIPTION("AMCC scp driver v" DRIVER_VERSION);
MODULE_LICENSE("GPL");
static int major;		/* default is dynamic major device number */
module_param(major, int, 0);
MODULE_PARM_DESC(major, "Major device number");

#define MASK_SEL_SCP 0xFFFDFFFF
#define CS_LOW 		0xFFFDFFFF
#define CS_HIGH 	0x00020000
#define GPIO_MASK	0xFFFFFFF3

static int
scp_ioctl(struct inode *inode, struct file *filep, unsigned int cmd,
	  unsigned long arg)
{
    return 0;
}

static struct file_operations scp_device_fops = {
	owner:THIS_MODULE,
	ioctl:scp_ioctl,
};

static int scpdev_initialized;
static void *virtual_scp_add;	/* Virtual address to access to the scp core */
static void *gpio_base;


#define scp_readb(addr) in_8((volatile u8 *)(virtual_scp_add + (addr)))
#define scp_writeb(addr,b) out_8((volatile u8 *)(virtual_scp_add + (addr)), (b))

//static DECLARE_WAIT_QUEUE_HEAD(scp_wait);

/* ***********************************************************************
 * FUNCTIONS
 *  
 */

/*
 **************************************************************************
 * scp_set_configuration
 *
 * Change the configuration of the SCP.
 * Enable the serial port and the serial clock Phase
 *
 * Return nothing.
 * ************************************************************************/
void scp_set_configuration(void)
{
    /*
     * SCP Clock Divisor Modulus register
     */
    scp_writeb(SCPD_CDM, 0x18);

    /*
     * SCP Mode register => Serial port Enabled + loopback (for test) 
     */
    /*
     * scp_writeb(SCPD_SPMODE, SCPD_EN | SCPD_LOOP | SCPD_SCP); 
     */

    /*
     * SCP Mode register => Serial port Enabled + Serial Clock Phase 
     */
    scp_writeb(SCPD_SPMODE, SCPD_EN | SCPD_SCP);
}


/*
 **************************************************************************
 + * scp_rx_data
 + *
 + * Read the Receive Data Register. Read Only register.
 + *
 + * Return unsigned char read
 + * **********************************************************************/
unsigned char scp_rx_data(void)
{
    unsigned char buffer = 0x00;

    /*
     * In case of loopback the data are inverted 
     */
    /*
     * buffer = ~scp_readb(SCPD_RXDATA); 
     */
    /*
     * start the xfer 
     */
    //scp_writeb(SCPD_CR, SCPD_STR);	/* fscz */
    buffer = (unsigned char) scp_readb(SCPD_RXDATA);
#ifdef SCP_DEBUG
	if (scp_verbose)
    	printk("scp_rx_data: receive data: %#X \n", buffer);
#endif

    return buffer;
}


/*
 **************************************************************************
 * scp_tx_data
 *
 * Write data from command on the Transmit Data Register.
 *
 * Return nothing.
 * ************************************************************************/
void scp_tx_data(unsigned char buffer)
{
    /*
     * write data to txdata 
     */
#ifdef SCP_DEBUG
	if (scp_verbose)
	    printk("scp_tx_data: Transmit data: %#X \n", buffer);
#endif
    scp_writeb(SCPD_TXDATA, buffer);

    /*
     * start the xfer 
     */
    scp_writeb(SCPD_CR, SCPD_STR);
}

/*
 **************************************************************************
 * scp_cs
 *
 * Enable/Disable Chip Select in the EPLD register (Selection_2_reg).
 *
 * Return nothing.
 * ************************************************************************/
void scp_cs(int chipid, int high)
{
	ulong val;
#if 1
	val = in_be32((gpio_base+(GPIO0_OR-GPIO_BASE)));
	if (!high) {
		out_be32((gpio_base+(GPIO0_OR-GPIO_BASE)), val && CS_LOW);
	}
	else {
		out_be32((gpio_base+(GPIO0_OR-GPIO_BASE)), val | CS_HIGH );
	}
#endif
/* CPU 32bit not 64bit
	u64 val;

	val = in_be64((gpio_base+(GPIO0_OR-GPIO_BASE)));
        if (!high) {
                out_be64((gpio_base+(GPIO0_OR-GPIO_BASE)), val && CS_LOW);
        }
        else {
                out_be64((gpio_base+(GPIO0_OR-GPIO_BASE)), val | CS_HIGH );
        }
*/

}

/*
 **************************************************************************
 * scp_write_proc: proc interface
 *
 * This function is called when the user writes in the interface /proc/driver/scp_dev
 *
 * Commands: works on chip 0
 * CSENA:  Enable Chip Select in the EPLD
 * CSDIS:  Disable Chip Select in the EPLD
 * RXDATA:     Read Receive Data register
 *
 * WREN:   Set Write Enable Latch          0x06
 * WRDI:   Write Disable                   0x04
 * RDSR:   Read Status                     0x05
 * WRSR:   Write Status                    0x01
 * WRITE:  Write Memory Data               0x02
 * READ:   Read Memory Data                0x03
 *
 * Return the number of data
 *
 * ************************************************************************/

static int
scp_write_proc(struct file *file, const char *buffer,
	       unsigned long count, void *data)
{
    char str[10];
    int val;
#ifdef SCP_DEBUG
	scp_verbose = 1;
#endif

    memset(str, 0, sizeof(str));
    if (copy_from_user(str, buffer, min_t(unsigned long, count, sizeof(str))))
		return -EFAULT;

    /*
     * Enable Chip Select in the EPLD 
     */
    if (!strncmp(str, "CSENA", 5))
		scp_cs(0, 0); 

    /*
     * Disable Chip Select in the EPLD 
     */
    else if (!strncmp(str, "CSDIS", 5))
		scp_cs(0, 1); 

    /*
     * Read RX DATA 
     */
    else if (!strncmp(str, "RXDATA", 6)) {
		scp_rx_data();
	}
    /*
     * Set Write Enable Latch: 0x06 
     */
    else if (!strncmp(str, "WREN", 4))
		scp_tx_data(0x06);

    /*
     * Write Disable: 0x04 
     */
    else if (!strncmp(str, "WRDI", 4))
		scp_tx_data(0x04);

    /*
     * Read Status Command: 0x05 
     */
    else if (!strncmp(str, "RDSR", 4))
		scp_tx_data(0x05);
    /*
     * Write Status Command: 0x01 
     */
    else if (!strncmp(str, "WRSR", 4))
		scp_tx_data(0x01);

    /*
     * Write Memory Data: 0x02 
     */
    else if (!strncmp(str, "WRITE", 5))
		scp_tx_data(0x02);

    /*
     * Read Memory Data: 0x03 
     */
    else if (!strncmp(str, "READ", 4))
		scp_tx_data(0x03);

    else {
		val = simple_strtol(str, NULL, 16);
		scp_tx_data((unsigned char) val);
    }
    return count;
}

/*
 **************************************************************************
 * scp_int_handler:  Handle general SCP interrupts.
 *
 *
 * ************************************************************************/
static irqreturn_t scp_int_handler(int irq, void *dev_id)
{
	unsigned char status_register;

	status_register = scp_readb(SCPD_SR);
    //wake_up(&scp_wait);
	// Serial Data receive is complete and RxD is available
	if (status_register & SCPD_RBR)  {
#ifdef SCP_DEBUG
		if (scp_verbose) 
			printk("Int-> ");
#endif
		scp_rx_data();
	}

    return IRQ_HANDLED;
}

int scp_io(int chipid, struct spi_dev_desc *desc,
       const unsigned char **inbufs, unsigned int *incounts,
       unsigned char **outbufs, unsigned int *outcounts, int cansleep)
{
    unsigned int incount, outcount, tries;
    const unsigned char *inp;
	unsigned char *outp;
	int extra_clock=0;

    /* CS 'L' */
   	scp_cs(chipid,0);
    udelay(desc->tcss);
#ifdef SCP_DEBUG 
	scp_verbose = 0;
#endif
    /* do scatter IO */
    inp = inbufs ? *inbufs : NULL;
	extra_clock =  (*inp == ATMEL_RDSR)? 1:0;
    incount = *incounts;
	while (incount && inp) { 
		tries = 0;
		while (scp_readb(SCPD_SR) & SCPD_SR_BUSY) {
			udelay(desc->tcss);
			if (tries++ > 20) {
				scp_cs(chipid,1);
				return -1;
			}
		}
		scp_tx_data(*inp++);
		incount --;
		if (!incount && *(++inbufs)) {
			incount = *(++incounts);
			inp = *(inbufs);
		} 
	}
#ifdef SCP_DEBUG 
	scp_verbose = 0;
#endif
	if (extra_clock) {
		scp_tx_data(0);
		scp_tx_data(0);
	}
    outp = outbufs ? *outbufs : NULL;
    outcount = *outcounts;
	while (outcount && outp) {
		scp_tx_data(0); /* dummy, for clock */
		//wait_event(scp_wait, scp_readb(SCPD_SR) & SCPD_SR_RBR);
		tries = 0;
		while (!(scp_readb(SCPD_SR) & SCPD_SR_RBR)) {
			udelay(desc->tcss);
			if (tries++ > 5) {
				return -1;
			}
		}
		*outp++ = scp_rx_data();
		outcount --;  
	}
	udelay(desc->tcss);
    /* CS 'H' */
   	scp_cs(chipid,1);

    return 0;
}

static int __devinit scp_request_irq(struct of_device *ofdev,
                                struct platform_device *dev)
{
        struct device_node *np = ofdev->node;
        int irq;

        irq = irq_of_parse_and_map(np, 0);
        if (irq == NO_IRQ) {
                dev_err(&ofdev->dev, "irq_of_parse_and_map failed\n");
                return NO_IRQ;
        }

        /* request interrupt */
        if (request_irq(irq, scp_int_handler , 0, SCP_NAME, dev)) {
                dev_err(&ofdev->dev, "request_irq %d failed\n", irq);
                return NO_IRQ;
        }

        return irq;
}

/*
 **************************************************************************
 * scpdev_cleanup: Clean the module
 *
 *
 * Return nothing.
 * ************************************************************************/
static int __devexit scp_remove(struct of_device *ofdev)
{
    if (scpdev_initialized > 0) {
		free_irq(SCP_IRQ, NULL);
	    iounmap(virtual_scp_add);
    	virtual_scp_add = NULL;
		iounmap(gpio_base);
		gpio_base = NULL;
	    release_mem_region(SCP_PHYS_ADD, 24);
    	remove_proc_entry("AMCC-SCP", NULL);
	    unregister_chrdev(major, "AMCC-SCP");
    	scpdev_initialized--;
	}
    return 0;
}

static int __init scp_probe(struct of_device *ofdev,
                        const struct of_device_id *match)
{
    SPI_DEV *spi_dev[NUM_SPI_SLAVES];
    struct proc_dir_entry *pentry;
    struct platform_device *scp_dev;
    int retval = 0, i;
    ulong val;
    if (scpdev_initialized) {
    	return -1;
    }

    scp_dev = kzalloc(sizeof(*scp_dev), GFP_KERNEL);
    if (!scp_dev) {
        dev_err(&ofdev->dev, "failed to allocate device data\n");
        return -ENOMEM;
    }
    dev_set_drvdata(&ofdev->dev, scp_dev);

    if (request_mem_region(SCP_PHYS_ADD, 24, "AMCC-SCP") == NULL) {
		retval = -EBUSY;
		goto err0;
    }
    if ((virtual_scp_add = ioremap(SCP_PHYS_ADD, 24)) == NULL) {
		retval = -ENOMEM;
		goto err1;
    }
	//printk("virtual_scp_add=0x%p\n", virtual_scp_add);
	if ((gpio_base = ioremap(GPIO_BASE, 0x100)) == NULL) {
		retval = -ENOMEM;
		goto err2;
	}
#if 1
	/* select SCP */
	val = SDR_READ(DCRN_SDR_PFC1);
	SDR_WRITE(DCRN_SDR_PFC1, val & MASK_SEL_SCP);

	/* enable GPIO output for pin 9, 10, 11 and 14*/
        val = SDR_READ(DCRN_SDR_PFC0); 
        SDR_WRITE(DCRN_SDR_PFC0, (val & 0xFF8FFFFF) | 0x00700000);

        val = SDR_READ(DCRN_SDR_PFC0);
        SDR_WRITE(DCRN_SDR_PFC0, (val & 0xFFFDFFFF) | 0x00020000);

	/* configure GPIO 9 and 11 as SPIClkOut and SPIDO */
        val = in_be32((gpio_base+(GPIO0_OSRL-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_OSRL-GPIO_BASE)), (val & 0xFFFFCCFF) | 0x00001100);

        val = in_be32((gpio_base+(GPIO0_TSRL-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_TSRL-GPIO_BASE)), (val & 0xFFFFCCFF) | 0x00001100);

        val = in_be32((gpio_base+(GPIO0_ISR1L-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_ISR1L-GPIO_BASE)), (val & 0xFFFFF3FF) | 0x00000400);

	/* configure GPIO14 as chip select */
	val = in_be32((gpio_base+(GPIO0_TCR-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_TCR-GPIO_BASE)), (val & 0xFFFDFFFF) | 0x00020000); 

        val = in_be32((gpio_base+(GPIO0_TSRL-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_TSRL-GPIO_BASE)), (val & 0xFFFFFFF3) | 0x00000000);

        val = in_be32((gpio_base+(GPIO0_OSRL-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_OSRL-GPIO_BASE)), (val & 0xFFFFFFF3) | 0x00000000);

        val = in_be32((gpio_base+(GPIO0_ODR-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_ODR-GPIO_BASE)), (val & 0xFFFDFFFF) | 0x00000000);
        
        val = in_be32((gpio_base+(GPIO0_ISR1L-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_ISR1L-GPIO_BASE)), val & GPIO_MASK);
        
	val = in_be32((gpio_base+(GPIO0_ISR2L-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_ISR2L-GPIO_BASE)), val & GPIO_MASK);
        
	val = in_be32((gpio_base+(GPIO0_ISR3L-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_ISR3L-GPIO_BASE)), val & GPIO_MASK);

        /* CS high, b14 <= '1' */
        val = in_be32((gpio_base+(GPIO0_OR-GPIO_BASE)));
        out_be32((gpio_base+(GPIO0_OR-GPIO_BASE)), val | CS_HIGH );
#endif

    if ((retval = register_chrdev(major, "AMCC-SCP", &scp_device_fops)) < 0) {	
		retval = -ENODEV;
		goto err3;
    }
    /* Init the SCP Core */
    scp_set_configuration();
    /* Create proc entry */
    if ((pentry = create_proc_entry(SCP_PROC_NAME, 0200, NULL)) == NULL) {
		retval = -ENODEV;
		goto err4;
    }
	pentry->write_proc = scp_write_proc;
    /* SCP interrupt registration to Linux. */
    printk(KERN_INFO "SCP: Requesting SCP irq %d ...\n", SCP_IRQ);
    /* Request SCP interrupt */
    if ((scp_request_irq(ofdev, scp_dev)) == NO_IRQ)
    {
        printk(KERN_INFO "SCP: Requesting SCP irq %d ...fail\n", SCP_IRQ);
        retval = -EINVAL;
        goto err5;
    }

    for (i = 0; i < NUM_SPI_SLAVES; i++) {
    	spi_dev[i] = NULL;
	/* Allocate SPI Flash device */
    	if ( (spi_dev[i] = (SPI_DEV *)kmalloc(sizeof(SPI_DEV), GFP_KERNEL))
		 == NULL) {
		retval = -ENOMEM;
		goto err6;
    	}
    	spi_dev[i]->slaveid = i;
    	if ((retval = beech_scp_init(spi_dev[i])) != 0) {
		printk(KERN_ERR	
			"beech_scp_init(): %s beech_scp_init err 0x%08x\n",
	       		DEVICE_NAME, retval);
		goto err7;
   	}
    }
    scpdev_initialized++;

	return 0;
err7:
err6:
	for (i = 0; i < NUM_SPI_SLAVES; i++) {
		if (spi_dev[i]) {
			kfree(spi_dev[i]);
		}
	}
	free_irq(SCP_IRQ, NULL);
err5:
	remove_proc_entry("AMCC-SCP", NULL);
    unregister_chrdev(major, "AMCC-SCP");
err4:
err3:
	iounmap(gpio_base);
	gpio_base = NULL;
err2:
    iounmap(virtual_scp_add);
    virtual_scp_add = NULL;

err1:
    release_mem_region(SCP_PHYS_ADD, 24);
err0:
    return retval;
}

static const struct of_device_id amcc_scp_match[] = {
        { .compatible = "amcc,scp-405ex", },
        {}
};

static struct of_platform_driver amcc_scp_driver = {
        .name   = "AMCC-SCP",
        .match_table = amcc_scp_match,
        .probe  = scp_probe,
        .remove = __devexit_p(scp_remove),
};

static int __init scp_init(void)
{
        return of_register_platform_driver(&amcc_scp_driver);
}

static void __exit scp_exit(void)
{
        of_unregister_platform_driver(&amcc_scp_driver);
}

module_init(scp_init);
module_exit(scp_exit);

MODULE_AUTHOR("christophe Lombard <christophe_lombard@fr.ibm.com>");
MODULE_DESCRIPTION("SCP /dev entries driver");
MODULE_LICENSE("GPL");
