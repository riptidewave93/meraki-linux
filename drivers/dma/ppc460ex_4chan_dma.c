/*
 * Copyright(c) 2008 Applied Micro Circuits Corporation(AMCC). All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/async_tx.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <asm/dcr-regs.h>
#include <asm/dcr.h>
#include "ppc460ex_4chan_dma.h"



#ifdef DEBUG_TEST
#define dma_pr printk
#else
#define dma_pr
#endif 
#define TEST_SIZE 12


ppc460ex_plb_dma_dev_t *adev;



int ppc460ex_get_dma_channel(void)
{
	int i;
	unsigned int status = 0;
	status = mfdcr(DCR_DMA2P40_SR);

	for(i=0; i<MAX_PPC460EX_DMA_CHANNELS; i++) {
		if ((status & (1 >> (20+i))) == 0)
			return i;
	}
	return -ENODEV;
}


int ppc460ex_get_dma_status(void)
{
  return (mfdcr(DCR_DMA2P40_SR));

}


int ppc460ex_set_src_addr(int ch_id, phys_addr_t src_addr)
{

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk("%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }


#ifdef PPC4xx_DMA_64BIT
  mtdcr(DCR_DMA2P40_SAH0 + ch_id*8, src_addr >> 32);
#endif
  mtdcr(DCR_DMA2P40_SAL0 + ch_id*8, (u32)src_addr);

  return DMA_STATUS_GOOD;
}

int ppc460ex_set_dst_addr(int ch_id, phys_addr_t dst_addr)
{

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }

#ifdef PPC4xx_DMA_64BIT
  mtdcr(DCR_DMA2P40_DAH0 + ch_id*8, dst_addr >> 32);
#endif
  mtdcr(DCR_DMA2P40_DAL0 + ch_id*8, (u32)dst_addr);
  
  return DMA_STATUS_GOOD;
}



/*
 * Sets the dma mode for single DMA transfers only.
 * For scatter/gather transfers, the mode is passed to the
 * alloc_dma_handle() function as one of the parameters.
 *
 * The mode is simply saved and used later.  This allows
 * the driver to call set_dma_mode() and set_dma_addr() in
 * any order.
 *
 * Valid mode values are:
 *
 * DMA_MODE_READ          peripheral to memory
 * DMA_MODE_WRITE         memory to peripheral
 * DMA_MODE_MM            memory to memory
 * DMA_MODE_MM_DEVATSRC   device-paced memory to memory, device at src
 * DMA_MODE_MM_DEVATDST   device-paced memory to memory, device at dst
 */
int ppc460ex_set_dma_mode(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, unsigned int mode)
{
  
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id]; 
  
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk("%s: bad channel %d\n", __FUNCTION__, dma_chan->chan_id);
    return DMA_STATUS_BAD_CHANNEL;
  }

  dma_chan->mode = mode;
  return DMA_STATUS_GOOD;
}




/*
 * Sets the DMA Count register. Note that 'count' is in bytes.
 * However, the DMA Count register counts the number of "transfers",
 * where each transfer is equal to the bus width.  Thus, count
 * MUST be a multiple of the bus width.
 */
void ppc460ex_set_dma_count(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, unsigned int count)
{
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];
  
//#ifdef DEBUG_4xxDMA

	{
		int error = 0;
		switch (dma_chan->pwidth) {
		case PW_8:
			break;
		case PW_16:
			if (count & 0x1)
				error = 1;
			break;
		case PW_32:
			if (count & 0x3)
				error = 1;
			break;
		case PW_64:
			if (count & 0x7)
				error = 1;
			break;
		 
		case PW_128:
		        if (count & 0xf)
			        error = 1;
			break;
		default:
			printk("set_dma_count: invalid bus width: 0x%x\n",
			       dma_chan->pwidth);
			return;
		}
		if (error)
			printk
			    ("Warning: set_dma_count count 0x%x bus width %d\n",
			     count, dma_chan->pwidth);
	}
//#endif
	count = count >> dma_chan->shift;
	//count = 10;
	mtdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8), count);

}




/*
 * Enables the channel interrupt.
 *
 * If performing a scatter/gatter transfer, this function
 * MUST be called before calling alloc_dma_handle() and building
 * the sgl list.  Otherwise, interrupts will not be enabled, if
 * they were previously disabled.
 */
int ppc460ex_enable_dma_interrupt(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int control;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];
  
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  dma_chan->int_enable = 1;


  control = mfdcr(DCR_DMA2P40_CR0);
  control |= DMA_CIE_ENABLE;	/* Channel Interrupt Enable */
  mtdcr(DCR_DMA2P40_CR0, control);
  


#if 1
  control = mfdcr(DCR_DMA2P40_CTC0);
  control |= DMA_CTC_TCIE | DMA_CTC_ETIE| DMA_CTC_EIE;
  mtdcr(DCR_DMA2P40_CTC0, control);

#endif

  
  return DMA_STATUS_GOOD;

}


/*
 * Disables the channel interrupt.
 *
 * If performing a scatter/gatter transfer, this function
 * MUST be called before calling alloc_dma_handle() and building
 * the sgl list.  Otherwise, interrupts will not be disabled, if
 * they were previously enabled.
 */
int ppc460ex_disable_dma_interrupt(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int control;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  dma_chan->int_enable = 0;
  control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
  control &= ~DMA_CIE_ENABLE;	/* Channel Interrupt Enable */
  mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);

  return DMA_STATUS_GOOD;
}


/*
 * This function returns the channel configuration.
 */
int ppc460ex_get_channel_config(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, 
				ppc460ex_plb_dma_ch_t *p_dma_ch)
{
  unsigned int polarity;
  unsigned int control;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];
  
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }

  memcpy(p_dma_ch, dma_chan, sizeof(ppc460ex_plb_dma_ch_t));

  polarity = mfdcr(DCR_DMA2P40_POL);

  p_dma_ch->polarity = polarity & GET_DMA_POLARITY(ch_id);
  control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
  
  p_dma_ch->cp = GET_DMA_PRIORITY(control);
  p_dma_ch->pwidth = GET_DMA_PW(control);
  p_dma_ch->psc = GET_DMA_PSC(control);
  p_dma_ch->pwc = GET_DMA_PWC(control);
  p_dma_ch->phc = GET_DMA_PHC(control);
  p_dma_ch->ce = GET_DMA_CE_ENABLE(control);
  p_dma_ch->int_enable = GET_DMA_CIE_ENABLE(control);
  p_dma_ch->shift = GET_DMA_PW(control);
  p_dma_ch->pf = GET_DMA_PREFETCH(control);

  return DMA_STATUS_GOOD;

}

/*
 * Sets the priority for the DMA channel dmanr.
 * Since this is setup by the hardware init function, this function
 * can be used to dynamically change the priority of a channel.
 *
 * Acceptable priorities:
 *
 * PRIORITY_LOW
 * PRIORITY_MID_LOW
 * PRIORITY_MID_HIGH
 * PRIORITY_HIGH
 *
 */
int ppc460ex_set_channel_priority(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, 
				  unsigned int priority)
{
  unsigned int control;

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  if ((priority != PRIORITY_LOW) &&
      (priority != PRIORITY_MID_LOW) &&
      (priority != PRIORITY_MID_HIGH) && (priority != PRIORITY_HIGH)) {
    printk("%s:bad priority: 0x%x\n", __FUNCTION__, priority);
  }
  
  control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
  control |= SET_DMA_PRIORITY(priority);
  mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);
  
  return DMA_STATUS_GOOD;
}

/*
 * Returns the width of the peripheral attached to this channel. This assumes
 * that someone who knows the hardware configuration, boot code or some other
 * init code, already set the width.
 *
 * The return value is one of:
 *   PW_8
 *   PW_16
 *   PW_32
 *   PW_64
 *
 *   The function returns 0 on error.
 */
unsigned int ppc460ex_get_peripheral_width(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int control;

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
   control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
   return (GET_DMA_PW(control));
}

/*
 * Enables the burst on the channel (BTEN bit in the control/count register)
 * Note:
 * For scatter/gather dma, this function MUST be called before the
 * ppc4xx_alloc_dma_handle() func as the chan count register is copied into the
 * sgl list and used as each sgl element is added.
 */
int ppc460ex_enable_burst(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int ctc;
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  ctc = mfdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8)) | DMA_CTC_BTEN;
  mtdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8), ctc);
  return DMA_STATUS_GOOD;
}


/*
 * Disables the burst on the channel (BTEN bit in the control/count register)
 * Note:
 * For scatter/gather dma, this function MUST be called before the
 * ppc4xx_alloc_dma_handle() func as the chan count register is copied into the
 * sgl list and used as each sgl element is added.
 */
int ppc460ex_disable_burst(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int ctc;
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  ctc = mfdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8)) &~ DMA_CTC_BTEN;
  mtdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8), ctc);
  return DMA_STATUS_GOOD;
}


/*
 * Sets the burst size (number of peripheral widths) for the channel
 * (BSIZ bits in the control/count register))
 * must be one of:
 *    DMA_CTC_BSIZ_2
 *    DMA_CTC_BSIZ_4
 *    DMA_CTC_BSIZ_8
 *    DMA_CTC_BSIZ_16
 * Note:
 * For scatter/gather dma, this function MUST be called before the
 * ppc4xx_alloc_dma_handle() func as the chan count register is copied into the
 * sgl list and used as each sgl element is added.
 */
int ppc460ex_set_burst_size(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, 
			    unsigned int bsize)
{
  unsigned int ctc;
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  ctc = mfdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8)) &~ DMA_CTC_BSIZ_MSK;
  ctc |= (bsize & DMA_CTC_BSIZ_MSK);
  mtdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8), ctc);
  return DMA_STATUS_GOOD;
}

/*
 *   Returns the number of bytes left to be transferred.
 *   After a DMA transfer, this should return zero.
 *   Reading this while a DMA transfer is still in progress will return
 *   unpredictable results.
 */
int ppc460ex_get_dma_residue(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int count;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  count = mfdcr(DCR_DMA2P40_CTC0 + (ch_id * 0x8));
  count &= DMA_CTC_TC_MASK ;

  return (count << dma_chan->shift);

}


/*
 * Configures a DMA channel, including the peripheral bus width, if a
 * peripheral is attached to the channel, the polarity of the DMAReq and
 * DMAAck signals, etc.  This information should really be setup by the boot
 * code, since most likely the configuration won't change dynamically.
 * If the kernel has to call this function, it's recommended that it's
 * called from platform specific init code.  The driver should not need to
 * call this function.
 */
int ppc460ex_init_dma_channel(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id, 
			      ppc460ex_plb_dma_ch_t *p_init)
{
  unsigned int polarity;
  uint32_t control = 0;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];
  

  DMA_MODE_READ = (unsigned long) DMA_TD;	/* Peripheral to Memory */
  DMA_MODE_WRITE = 0;	/* Memory to Peripheral */

  if (!p_init) {
    printk("%s: NULL p_init\n", __FUNCTION__);
    return DMA_STATUS_NULL_POINTER;
  }
  
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
#if DCR_DMA2P40_POL > 0
  polarity = mfdcr(DCR_DMA2P40_POL);
#else
  polarity = 0;
#endif

   p_init->int_enable = 0;
   p_init->buffer_enable = 1;
   p_init->etd_output = 1;
   p_init->tce_enable = 1;
   p_init->pl = 0;
   p_init->dai = 1;
   p_init->sai = 1;
   /* Duc Dang: make channel priority to 2, original is 3 */
   p_init->cp = 2; 
   p_init->pwidth = PW_8;
   p_init->psc = 0;
   p_init->pwc = 0;
   p_init->phc = 0;
   p_init->pf = 1;


  /* Setup the control register based on the values passed to
   * us in p_init.  Then, over-write the control register with this
   * new value.
   */
#if 0
  control |= SET_DMA_CONTROL;
#endif
  control = SET_DMA_CONTROL;
  /* clear all polarity signals and then "or" in new signal levels */

//PMB - Workaround
  //control = 0x81A2CD80;
  //control = 0x81A00180;


  polarity &= ~GET_DMA_POLARITY(ch_id);
  polarity |= p_init->polarity;

#if DCR_DMA2P40_POL > 0
  mtdcr(DCR_DMA2P40_POL, polarity);
#endif
  mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);
  
  /* save these values in our dma channel structure */
  //memcpy(dma_chan, p_init, sizeof(ppc460ex_plb_dma_ch_t));
  /*
   * The peripheral width values written in the control register are:
   *   PW_8                 0
   *   PW_16                1
   *   PW_32                2
   *   PW_64                3
   *   PW_128               4
   *
   *   Since the DMA count register takes the number of "transfers",
   *   we need to divide the count sent to us in certain
   *   functions by the appropriate number.  It so happens that our
   *   right shift value is equal to the peripheral width value.
   */
   dma_chan->shift = p_init->pwidth;
   dma_chan->sai = p_init->sai;
   dma_chan->dai = p_init->dai;
   dma_chan->tce_enable = p_init->tce_enable;
   dma_chan->mode = DMA_MODE_MM;
   /*
    * Save the control word for easy access.
    */
   dma_chan->control = control;
   mtdcr(DCR_DMA2P40_SR, 0xffffffff);


   return DMA_STATUS_GOOD;
}


int ppc460ex_enable_dma(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
  unsigned int control;
  ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];
  unsigned int status_bits[] = { DMA_CS0 | DMA_TS0 | DMA_CH0_ERR,
				 DMA_CS1 | DMA_TS1 | DMA_CH1_ERR};

  if (dma_chan->in_use) {
    printk("%s:enable_dma: channel %d in use\n", __FUNCTION__, ch_id);
    return DMA_STATUS_CHANNEL_NOTFREE;
  }

  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk(KERN_ERR "%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }

#if 0
  if (dma_chan->mode == DMA_MODE_READ) {
    /* peripheral to memory */
    ppc460ex_set_src_addr(ch_id, 0);
    ppc460ex_set_dst_addr(ch_id, dma_chan->addr);
  } else if (dma_chan->mode == DMA_MODE_WRITE) {
                                                            /* memory to peripheral */
    ppc460ex_set_src_addr(ch_id, dma_chan->addr);
    ppc460ex_set_dst_addr(ch_id, 0);
  }
#endif
  /* for other xfer modes, the addresses are already set */
  control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
  control &= ~(DMA_TM_MASK | DMA_TD);	/* clear all mode bits */
  if (dma_chan->mode == DMA_MODE_MM) {
    /* software initiated memory to memory */
    control |= DMA_ETD_OUTPUT | DMA_TCE_ENABLE;
    control |= DMA_MODE_MM;
    if (dma_chan->dai) {
      control |= DMA_DAI;
    }
    if (dma_chan->sai) {
      control |= DMA_SAI;
    }
  }

  mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);
  /*
   * Clear the CS, TS, RI bits for the channel from DMASR.  This
   * has been observed to happen correctly only after the mode and
   * ETD/DCE bits in DMACRx are set above.  Must do this before
   * enabling the channel.
   */
  mtdcr(DCR_DMA2P40_SR, status_bits[ch_id]);
  /*
   * For device-paced transfers, Terminal Count Enable apparently
   * must be on, and this must be turned on after the mode, etc.
   * bits are cleared above (at least on Redwood-6).
   */
  
  if ((dma_chan->mode == DMA_MODE_MM_DEVATDST) ||
      (dma_chan->mode == DMA_MODE_MM_DEVATSRC))
    control |= DMA_TCE_ENABLE;

  /*
   * Now enable the channel.
   */
  
  control |= (dma_chan->mode | DMA_CE_ENABLE);
  control |= DMA_BEN;
  //control = 0xc4effec0;

  mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);
  dma_chan->in_use = 1;
  return 0;
  
}


void
ppc460ex_disable_dma(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id)
{
	unsigned int control;
	ppc460ex_plb_dma_ch_t *dma_chan = adev->chan[ch_id];

	if (!dma_chan->in_use) {
		printk("disable_dma: channel %d not in use\n", ch_id);
		return;
	}

	if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
		printk("disable_dma: bad channel: %d\n", ch_id);
		return;
	}

	control = mfdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8));
	control &= ~DMA_CE_ENABLE;
	mtdcr(DCR_DMA2P40_CR0 + (ch_id * 0x8), control);

        dma_chan->in_use = 0;
}




/*
 * Clears the channel status bits
 */
int ppc460ex_clear_dma_status(unsigned int ch_id)
{
  if (ch_id >= MAX_PPC460EX_DMA_CHANNELS) {
    printk("KERN_ERR %s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  mtdcr(DCR_DMA2P40_SR, ((u32)DMA_CH0_ERR | (u32)DMA_CS0 | (u32)DMA_TS0) >> ch_id);
  return DMA_STATUS_GOOD;

}


/**
 * ppc460ex_dma_eot_handler - end of transfer interrupt handler
 */
irqreturn_t ppc460ex_4chan_dma_eot_handler(int irq, void *data)
{
	unsigned int data_read = 0;
	unsigned int try_cnt = 0;
	
	//printk("transfer complete\n");
	data_read = mfdcr(DCR_DMA2P40_SR);
	//printk("%s: status 0x%08x\n", __FUNCTION__, data_read);

	do{
	  //while bit 3 TC done is 0
	  data_read = mfdcr(DCR_DMA2P40_SR);
	  if (data_read & 0x00800000 ) {printk("test FAIL\n"); } //see if error bit is set
	}while(((data_read & 0x80000000) != 0x80000000) && ++try_cnt <= 10);// TC is now 0
	
	data_read = mfdcr(DCR_DMA2P40_SR);
	while (data_read & 0x00000800){ //while channel is busy
	  data_read = mfdcr(DCR_DMA2P40_SR);
	  printk("%s: status for busy 0x%08x\n", __FUNCTION__, data_read);
	}
	mtdcr(DCR_DMA2P40_SR, 0xffffffff);
	
	
	
	return IRQ_HANDLED;
}



static struct of_device_id dma_per_chan_match[] = {
	{
		.compatible = "amcc,dma-4channel",
	},
	{},
};




#if 0
/*** test code ***/
static int ppc460ex_dma_memcpy_self_test(ppc460ex_plb_dma_dev_t *device, unsigned int dma_ch_id)
{
  ppc460ex_plb_dma_ch_t p_init;
  int res = 0, i;
  unsigned int control;
  phys_addr_t  *src;
  phys_addr_t *dest;

  phys_addr_t *gap;

  phys_addr_t dma_dest, dma_src;

  src = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!src)
    return -ENOMEM;
  gap = kzalloc(200,  GFP_KERNEL);
  if (!gap)
    return -ENOMEM;



  dest = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!dest) {
    kfree(src);
    return -ENOMEM;
  }

  printk("src = 0x%08x\n", (unsigned int)src);
  printk("gap = 0x%08x\n", (unsigned int)gap);
  printk("dest = 0x%08x\n", (unsigned int)dest);

  /* Fill in src buffer */
  for (i = 0; i < TEST_SIZE; i++)
    ((u8*)src)[i] = (u8)i;

  printk("dump src\n");
  DMA_HEXDUMP(src, TEST_SIZE);
  DMA_HEXDUMP(dest, TEST_SIZE);
#if 1
  dma_src = dma_map_single(p_init.device->dev, src, TEST_SIZE,
			   DMA_TO_DEVICE);
  dma_dest = dma_map_single(p_init.device->dev, dest, TEST_SIZE,
			    DMA_FROM_DEVICE);
#endif  
  printk("%s:channel = %d chan 0x%08x\n", __FUNCTION__, device->chan[dma_ch_id]->chan_id, 
	 (unsigned int)(device->chan));
  
  p_init.polarity = 0;
  p_init.pwidth = PW_32;
  p_init.in_use = 0;
  p_init.sai = 1;
  p_init.dai = 1;
  res = ppc460ex_init_dma_channel(device, dma_ch_id, &p_init);

  if (res) {
    printk("%32s: init_dma_channel return %d\n",
	   __FUNCTION__, res);
  }
  ppc460ex_clear_dma_status(dma_ch_id);

  ppc460ex_set_src_addr(dma_ch_id, dma_src);
  ppc460ex_set_dst_addr(dma_ch_id, dma_dest);

  ppc460ex_set_dma_mode(device, dma_ch_id, DMA_MODE_MM);
  ppc460ex_set_dma_count(device, dma_ch_id, TEST_SIZE);

  res = ppc460ex_enable_dma_interrupt(device, dma_ch_id);
  if (res) {
    printk("%32s: en/disable_dma_interrupt\n",
	   __FUNCTION__);
  }


  if (dma_ch_id == 0)
    control = mfdcr(DCR_DMA2P40_CR0);
  else if (dma_ch_id == 1)
    control = mfdcr(DCR_DMA2P40_CR1);


  control &= ~(SET_DMA_BEN(1));
  control &= ~(SET_DMA_PSC(3));
  control &= ~(SET_DMA_PWC(0x3f));
  control &= ~(SET_DMA_PHC(0x7));
  control &= ~(SET_DMA_PL(1));



  if (dma_ch_id == 0)
    mtdcr(DCR_DMA2P40_CR0, control);
  else if (dma_ch_id == 1)
    mtdcr(DCR_DMA2P40_CR1, control);


  ppc460ex_enable_dma(device, dma_ch_id);
  

  if (memcmp(src, dest, TEST_SIZE)) {
    printk("Self-test copy failed compare, disabling\n");
    res = -ENODEV;
    goto out;
  }


  return 0;

 out: kfree(src);
  kfree(dest);
  return res;
  
}



static int test1(void)
{
  void *src, *dest;
  void *src1, *dest1;
  int i;
  unsigned int chan;

  src = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!src)
    return -ENOMEM;

  dest = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!dest) {
    kfree(src);
    return -ENOMEM;
  }
  
  src1 = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!src1)
    return -ENOMEM;

  dest1 = kzalloc(TEST_SIZE, GFP_KERNEL);
  if (!dest1) {
    kfree(src1);
    return -ENOMEM;
  }
  
  /* Fill in src buffer */
  for (i = 0; i < TEST_SIZE; i++)
    ((u8*)src)[i] = (u8)i;

  /* Fill in src buffer */
  for (i = 0; i < TEST_SIZE; i++)
    ((u8*)src1)[i] = (u8)0xaa;

#ifdef DEBUG_TEST
  DMA_HEXDUMP(src, TEST_SIZE);
  DMA_HEXDUMP(dest, TEST_SIZE);
  DMA_HEXDUMP(src1, TEST_SIZE);
  DMA_HEXDUMP(dest1, TEST_SIZE);
#endif  
  chan = ppc460ex_get_dma_channel();
  
#ifdef ENABLE_SGL
  test_sgdma_memcpy(src, dest, src1, dest1, TEST_SIZE, chan);
#endif
  test_dma_memcpy(src, dest, TEST_SIZE, chan);


 out: kfree(src);
  kfree(dest);
  kfree(src1);
  kfree(dest1);

  return 0;

}
#endif



/*******************************************************************************
 * Module Initialization Routine
 *******************************************************************************
 */
int __devinit ppc460ex_dma_per_chan_probe(struct of_device *ofdev,
		const struct of_device_id *match)
{
	int ret=0;
	//ppc460ex_plb_dma_dev_t *adev;
	ppc460ex_plb_dma_ch_t *new_chan;
	int err;
	 


	 adev = dev_get_drvdata(ofdev->dev.parent);
	 BUG_ON(!adev);
	 /* create a device */
	 if ((new_chan = kzalloc(sizeof(*new_chan), GFP_KERNEL)) == NULL) {
	   printk("ERROR:No Free memory for allocating dma channels\n");
	   ret = -ENOMEM;
	   goto err;
	 }

	 err = of_address_to_resource(ofdev->node,0,&new_chan->reg);
	 if (err) {
	   printk("ERROR:Can't get %s property reg\n", __FUNCTION__);
	   goto err;
	 }
	 new_chan->device = adev;
	 new_chan->reg_base = ioremap(new_chan->reg.start,new_chan->reg.end - new_chan->reg.start + 1);
#if 1
	 printk("PPC460ex PLB DMA engine @0x%02X_%08X size %d\n",
		(u32)(new_chan->reg.start >> 32),
		(u32)new_chan->reg.start,
		(u32)(new_chan->reg.end - new_chan->reg.start + 1));
#endif

	 switch(new_chan->reg.start) {
	case 0x100:
		new_chan->chan_id = 0;
		break;
	case 0x108:
		new_chan->chan_id = 1;
		break;
	case 0x110:
		new_chan->chan_id = 2;
		break;
	case 0x118:
		new_chan->chan_id = 3;
		break;
	 }
	 new_chan->chan_id = ((new_chan->reg.start - 0x100)& 0xfff) >> 3;
	 printk("new_chan->chan_id 0x%x\n",new_chan->chan_id);
	 adev->chan[new_chan->chan_id] = new_chan;
	 printk("new_chan->chan->chan_id 0x%x\n",adev->chan[new_chan->chan_id]->chan_id);
	 //adev->chan[new_chan->chan_id]->reg_base = new_chan->reg_base;

	 return 0;
	 
 err:
	 return ret;
	 
}

int __devinit ppc460ex_dma_4chan_probe(struct of_device *ofdev,
				      const struct of_device_id *match)
{
  int ret=0, irq = 0;
  //ppc460ex_plb_dma_dev_t *adev;
  ppc460ex_plb_dma_ch_t *chan = NULL;
  

  /* create a device */
  if ((adev = kzalloc(sizeof(*adev), GFP_KERNEL)) == NULL) {
      ret = -ENOMEM;
    goto err_adev_alloc;
  }
  adev->dev = &ofdev->dev;
#if !defined(CONFIG_APM82181)
  err = of_address_to_resource(ofdev->node,0,&adev->reg);
  if(err) {
    printk(KERN_ERR"Can't get %s property 'reg'\n",ofdev->node->full_name);
  }
#endif  
  printk(KERN_INFO"Probing AMCC DMA driver\n");
#if !defined(CONFIG_APM82181)
  adev->reg_base = ioremap(adev->reg.start, adev->reg.end - adev->reg.start + 1);
#endif

#if 1
  irq = of_irq_to_resource(ofdev->node, 0, NULL);
  if (irq >= 0) {
    ret = request_irq(irq, ppc460ex_4chan_dma_eot_handler,
		      IRQF_DISABLED, "Peripheral DMA0/1", chan);
    if (ret) {
      ret = -EIO;
      goto err_irq;
    }
    //irq = platform_get_irq(adev, 0);
    /* only DMA engines have a separate err IRQ
     * so it's Ok if irq < 0 in XOR case
     */
  } else
    ret = -ENXIO;

#if !defined(CONFIG_APM82181)
  printk("PPC4xx PLB DMA engine @0x%02X_%08X size %d IRQ %d \n",
	  (u32)(adev->reg.start >> 32),
	  (u32)adev->reg.start,
	  (u32)(adev->reg.end - adev->reg.start + 1),
	  irq);
#else
	printk("PPC4xx PLB DMA engine IRQ %d\n", irq);
#endif
#endif
  dev_set_drvdata(&(ofdev->dev),adev);
  of_platform_bus_probe(ofdev->node,dma_per_chan_match,&ofdev->dev);


  //ppc460ex_dma_memcpy_self_test(adev, 0);
  //test1();

   
  return 0;


err_adev_alloc:
  //release_mem_region(adev->reg.start, adev->reg.end - adev->reg.start);
err_irq:
	kfree(chan);

	return ret;
}


static struct of_device_id dma_4chan_match[] = {
	{
		.compatible = "amcc,dma",
	},
	{},
};

struct of_platform_driver ppc460ex_dma_4chan_driver = {
	.name = "plb_dma",
	.match_table = dma_4chan_match,
	.probe = ppc460ex_dma_4chan_probe,
};

struct of_platform_driver ppc460ex_dma_per_chan_driver = {
	.name = "dma-4channel",
	.match_table = dma_per_chan_match,
	.probe = ppc460ex_dma_per_chan_probe,
};


static int __init mod_init (void)
{
  printk("%s:%d\n", __FUNCTION__, __LINE__);
  return of_register_platform_driver(&ppc460ex_dma_4chan_driver);
  printk("here 2\n");
}

static void __exit mod_exit(void)
{
	of_unregister_platform_driver(&ppc460ex_dma_4chan_driver);
}

static int __init ppc460ex_dma_per_chan_init (void)
{
  printk("%s:%d\n", __FUNCTION__, __LINE__);
  return of_register_platform_driver(&ppc460ex_dma_per_chan_driver);
  printk("here 3\n");
}

static void __exit ppc460ex_dma_per_chan_exit(void)
{
	of_unregister_platform_driver(&ppc460ex_dma_per_chan_driver);
}

subsys_initcall(ppc460ex_dma_per_chan_init);
subsys_initcall(mod_init);

//module_exit(mod_exit);

//module_exit(ppc460ex_dma_per_chan_exit);

MODULE_DESCRIPTION("AMCC PPC460EX 4 channel Engine Driver");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL_GPL(ppc460ex_get_dma_status);
EXPORT_SYMBOL_GPL(ppc460ex_set_src_addr);
EXPORT_SYMBOL_GPL(ppc460ex_set_dst_addr);
EXPORT_SYMBOL_GPL(ppc460ex_set_dma_mode);
EXPORT_SYMBOL_GPL(ppc460ex_set_dma_count);
EXPORT_SYMBOL_GPL(ppc460ex_enable_dma_interrupt);
EXPORT_SYMBOL_GPL(ppc460ex_init_dma_channel);
EXPORT_SYMBOL_GPL(ppc460ex_enable_dma);
EXPORT_SYMBOL_GPL(ppc460ex_disable_dma);
EXPORT_SYMBOL_GPL(ppc460ex_clear_dma_status);
EXPORT_SYMBOL_GPL(ppc460ex_get_dma_residue);
EXPORT_SYMBOL_GPL(ppc460ex_disable_dma_interrupt);
EXPORT_SYMBOL_GPL(ppc460ex_get_channel_config);
EXPORT_SYMBOL_GPL(ppc460ex_set_channel_priority);
EXPORT_SYMBOL_GPL(ppc460ex_get_peripheral_width);
EXPORT_SYMBOL_GPL(ppc460ex_enable_burst);
EXPORT_SYMBOL_GPL(ppc460ex_disable_burst);
EXPORT_SYMBOL_GPL(ppc460ex_set_burst_size);

/************************************************************************/
