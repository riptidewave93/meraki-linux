/* **************************************************************************
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
 * echo "ad" > /proc/driver/scp_dev
 *
 * **************************************************************************/
#include <linux/mtd/mtd.h>  /* required for mtd support */

/* SDR read/write helper macros */
#define DCRN_SDR_CONFIG_ADDR    0x000E 
#define DCRN_SDR_CONFIG_DATA    0x000F 

#define SDR_READ(offset) ({                     \
        mtdcr(DCRN_SDR_CONFIG_ADDR, offset);    \
        mfdcr(DCRN_SDR_CONFIG_DATA);})
#define SDR_WRITE(offset, data) ({              \
        mtdcr(DCRN_SDR_CONFIG_ADDR, offset);    \
        mtdcr(DCRN_SDR_CONFIG_DATA, data);})

// Physical address
#define SCP_PHYS_ADD 0x4EF600900ULL 
/* GPIO address */
#define GPIO_BASE           (0x4EF600B00ULL)
#define GPIO0_OR        (GPIO_BASE+0x0)
#define GPIO0_TCR       (GPIO_BASE+0x4)
#define GPIO0_OSRL	(GPIO_BASE+0x8)
#define GPIO0_OSRH      (GPIO_BASE+0xC)
#define GPIO0_TSRL      (GPIO_BASE+0x10)
#define GPIO0_TSRH      (GPIO_BASE+0x14)
#define GPIO0_ODR	(GPIO_BASE+0x18)
#define GPIO0_ISR1L     (GPIO_BASE+0x30)
#define GPIO0_ISR1H     (GPIO_BASE+0x34)
#define GPIO0_ISR2L     (GPIO_BASE+0x38)
#define GPIO0_ISR2H     (GPIO_BASE+0x3C)
#define GPIO0_ISR3L     (GPIO_BASE+0x40)
#define GPIO0_ISR3H     (GPIO_BASE+0x44)
/* Function Selection register */
#define DCRN_SDR_PFC0            0x4100 
#define DCRN_SDR_PFC1            0x4101 

#define SCP_IRQ 34
#define SCP_NAME "Serial Communications Port"
#define SCP_PROC_NAME  "driver/scp_dev"

/*-------------------------------------------------------------------------*/
/* Device Driver Control Flags (combinable flags parameters)               */
/*-------------------------------------------------------------------------*/
/* Flags used in SCPD_Set_Configuration API */ 
#define  SCPD_SCP              0x10  /* Serial Clock Phase */
#define  SCPD_EN               0x08  /* Serial Port Enabled */
#define	 SCPD_RD	       0x04  /* Data bit 7 transfer first */
#define  SCPD_CI	       0x02  /* Clock inverse */
#define  SCPD_LOOP             0x01  /* Internal Loopback */

/*-------------------------------------------------------------------------*/
/* Device Driver Control values                                            */
/*-------------------------------------------------------------------------*/
/* SCPD_Tx_Char values */
#define  SCPD_STR              0x01    /* start of xfer */ 

/* SCPD_RBR values */
#define  SCPD_RBR              0x01    /* Rx Byte Ready */ 

/*----------------------------------------------------------------------*/
/* Core Register Address Map                                            */
/*----------------------------------------------------------------------*/
#define SCPD_SPMODE 0x0000 
#define SCPD_RXDATA 0x0001
#define SCPD_TXDATA 0x0002
#define SCPD_CR     0x0003
#define SCPD_SR     0x0004
#define SCPD_CDM    0x0006

#define SCPD_SR_BUSY	0x2
#define SCPD_SR_RBR	0x1

#define MAF_NAME_STR_LEN	256
#define MEM_TYPE_STR_LEN	256

#define DEVICE_NAME		"scp"
#define DEVICE_REGS		"ssi_regs"
#define	NUM_SPI_SLAVES		1

/* ATMEL 250x0 instructions */
#define	ATMEL_WREN	0x06
#define	ATMEL_WRDI	0x04
#define ATMEL_RDSR	0x05
#define ATMEL_WRSR	0x01
#define	ATMEL_READ	0x03
#define	ATMEL_WRITE	0x02
#define ATMEL_WRITE_SIZE	32

#define ATMEL_SR_BSY	0x01
#define ATMEL_SR_WEN	0x02
#define ATMEL_SR_BP0	0x04
#define ATMEL_SR_BP1	0x08



/*******************************************************************************
 * struct spi_chip
 * Data structure for SPI Flash chip
 @read_buf :        read data from the chip into the buffer
 @write_buf :       write data from the buffer to the chip
 @select_chip :     select chip number
 @hwcontrol :       hw specific function for accessing control-lines
 @dev_ready :       hw specific function for device ready/busy ?
 @chip_lock :       lock to protect access
 @wq :          wait queue to sleep if SPI operation in progress
 @state :       current state of SPI device
 @options :     various chip options
 @numchips :        number of physical chips
 @maf_name :        Manufacturer name, obtained from READ_ID call
 @mem_type :        Memory Type, obtained from READ_ID call
 @chipsize :        Memory Capacity, obtained from READ_ID call
 @numsectors :      number of sectors on flash chip
 @sectorsize :      size of each sector
 @numpages :        number of pages on flash chip
 @pagesize :        size of each page
 @slaveid :     slave/flash chip number
******************************************************************************/
typedef struct spi_chip
{
    /* SPI devices list */
    struct 	list_head    list;

    void    (*select_chip)(int chip, int on_off);

    int     (*single_command)(struct mtd_info *mtd,
                            const int command);

    int     (*read_buf)(struct mtd_info *mtd, ulong addr,
                        u_char *data, int data_len);

    int     (*write_buf)(struct mtd_info *mtd, ulong addr,
                    const u_char *data, int data_len);

    spinlock_t  chip_lock;
    wait_queue_head_t wq;
    uint    options;
    int     manuf_id;
    int     id_mem_type;
    int     chipsize;
    int     numsectors;
    int     sectorsize;
    int     numpages;
    int     chip_delay;
    int     slaveid;
    void    *priv;
} SPI_DEV;

/* SPI */
struct spi_dev_desc {
	unsigned int baud;
	unsigned int  tcss, tcsh, tcsr; /* CS setup/hold/recovery time */
	unsigned int byteorder:1;	/* 0:LSB-First, 1:MSB-First */
	unsigned int polarity:1;	/* 0:High-Active */
	unsigned int phase:1;		/* 0:Sample-Then-Shift */
};


extern unsigned char scp_rx_data(void);
extern void scp_tx_data(unsigned char buffer);
extern int scp_io(int chipid, struct spi_dev_desc *desc,
        const unsigned char **inbufs, unsigned int *incounts,
        unsigned char **outbufs, unsigned int *outcounts,
        int cansleep);
extern int beech_scp_init(SPI_DEV * s);
extern void scp_cs(int chipid, int on);

#ifdef SCP_DEBUG
extern int scp_verbose;
#endif

