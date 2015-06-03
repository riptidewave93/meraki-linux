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
#include <linux/delay.h>
#include <asm/cacheflush.h>
#include "ppc460ex_4chan_dma.h"
#include <asm/page.h>
#include <asm/time.h>
#include <linux/pipe_fs_i.h>
#include <linux/splice.h>

#define SGDMA_MAX_POLL_COUNT		100000000
#define SGDMA_POLL_DELAY		5

static phys_addr_t splice_src_dma_addrs[PIPE_BUFFERS];
static dma_addr_t splice_dst_dma_addrs[PIPE_BUFFERS];

//#define DEBUG_SPLICE_DMA	1
//#define SPLICE_DMA_COHERENT	1
//#define DEBUG_SPLICE_DMA_TIMECAL 1

extern int ppc460ex_disable_dma_interrupt(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id);
extern int ppc460ex_disable_burst(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id);
extern int ppc460ex_enable_burst(ppc460ex_plb_dma_dev_t *adev, unsigned int ch_id);
void *dma_mem_page = NULL;





#define dma_pr(x) printk(KERN_DEBUG,x)


int ppc460ex_set_sg_addr(int ch_id, phys_addr_t sg_addr)
{
  if (unlikely(ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
#ifdef PPC4xx_DMA_64BIT
  mtdcr(DCR_DMA2P40_SGH0 + ch_id*8, sg_addr >> 32);
#endif
  mtdcr(DCR_DMA2P40_SGL0 + ch_id*8, (u32)sg_addr);

  return 0;
}

static int
poll_for_sgdma_done(int chan)
{
	int i;
	volatile int status = 0;
#ifdef DEBUG_SPLICE_DMA_TIMECAL
	u64 time1=0, time2=0, timediff=0;
#endif	

#ifdef DEBUG_SPLICE_DMA_TIMECAL
	time1 = get_tbl() | (get_tbu() << 32);
#endif
	for(i = 0; i < SGDMA_MAX_POLL_COUNT; i++) {
#ifdef DEBUG_SPLICE_DMA
		if(i%16 == 0)
		printk("%s:%s:%d - waiting %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#endif
		status = PPC460EX_DMA_CHAN_SGXFR_COMPLETE(chan);
		if(status) {
#ifdef DEBUG_SPLICE_DMA
			printk("%s:%s:%d - Breaking\n", __FILE__, __FUNCTION__, __LINE__);
#endif
			break;
		}

#ifdef DEBUG_SPLICE_DMA
		printk("status = %d dma_status = 0x%08x\n", status, DMA_STATUS(chan));
#endif
		//udelay(SGDMA_POLL_DELAY);
	}
#ifdef DEBUG_SPLICE_DMA_TIMECAL
	time2 = get_tbl() | (get_tbu() << 32);
#endif

#ifdef DEBUG_SPLICE_DMA_TIMECAL
	printk("%s:%s:%d time taken for transfer is %llu\n",
			__FILE__, __FUNCTION__, __LINE__, time2-time1);
#endif
	if(unlikely(i >= SGDMA_MAX_POLL_COUNT)) {
		printk("%s:%s:%d - timeout\n",
				__FILE__, __FUNCTION__, __LINE__);
		return -ETIME;
	}

	return 0;
}

static int
get_transfer_width(u64 align)
{
	if(!(align & 0xF))
		return 128;

	if(!(align & 0x7))
		return 64;

	if(!(align & 0x3))
		return 32;

	if(!(align & 0x1))
		return 16;

	return 8;
}


/*
 *   Add a new sgl descriptor to the end of a scatter/gather list
 *   which was created by alloc_dma_handle().
 *
 *   For a memory to memory transfer, both dma addresses must be
 *   valid. For a peripheral to memory transfer, one of the addresses
 *   must be set to NULL, depending on the direction of the transfer:
 *   memory to peripheral: set dst_addr to NULL,
 *   peripheral to memory: set src_addr to NULL.
 */
int ppc460ex_add_dma_sgl(ppc460ex_plb_dma_dev_t *adev, 
			 sgl_handle_t handle, 
			 phys_addr_t src_addr, 
			 phys_addr_t dst_addr, 
			 unsigned int count)
{
	sgl_list_info_t *psgl = (sgl_list_info_t *)handle;
	ppc460ex_plb_dma_ch_t *p_dma_ch;
	u64 align;
	int tr_width = 8;		/* initial value 8 bits */
#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - Filling in dma sgl list\n", __func__, __FILE__, __LINE__);
#endif

	if (unlikely(!handle)) {
		printk("%s: null handle\n", __FUNCTION__);
		return DMA_STATUS_BAD_HANDLE;
	}
	if (unlikely(psgl->ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
		printk("%s: bad channel %d\n", __FUNCTION__, psgl->ch_id);
		return DMA_STATUS_BAD_CHANNEL;
	}

	p_dma_ch = adev->chan[psgl->ch_id];
	align = src_addr | dst_addr | count;

	tr_width = get_transfer_width(align);
	switch(tr_width) {
		case 128:
			p_dma_ch->pwidth = PW_128;
			break;
		case 64:
			p_dma_ch->pwidth = PW_64;
			break;
		case 32:
			p_dma_ch->pwidth = PW_32;
			break;
		case 16:
			p_dma_ch->pwidth = PW_16;
			break;
		default:
			p_dma_ch->pwidth = PW_8;
			break;
	}

	{
		int error = 0;
		u64 aligned =
			src_addr | dst_addr | count;
		switch (p_dma_ch->pwidth) {
			case PW_8:
				break;
			case PW_16:
				if (aligned & 0x1)
					error = 1;
				break;
			case PW_32:
				if (aligned & 0x3)
					error = 1;
				break;
			case PW_64:
				if (aligned & 0x7)
					error = 1;
				break;
			case PW_128:
				if (aligned & 0xf)
					error = 1;
				break;
			default:
				printk("%s:set_dma_count: invalid bus width: 0x%x\n", __FUNCTION__,
						p_dma_ch->pwidth);
				return DMA_STATUS_GENERAL_ERROR;
		}
		if (unlikely(error))
			printk
				("Warning: set_dma_count count 0x%x bus width %d aligned= 0x%llx\n",
				 count, p_dma_ch->pwidth, aligned);
	}

	p_dma_ch->shift = p_dma_ch->pwidth;


	if (unlikely((unsigned) (psgl->ptail + 1) >= ((unsigned) psgl + SGL_LIST_SIZE))) {
		printk("sgl handle out of memory \n");
		return DMA_STATUS_OUT_OF_MEMORY;
	}

	if (!psgl->ptail) {
		psgl->phead = (ppc_sgl_t *)
			((((unsigned) psgl + sizeof (sgl_list_info_t)))  );
		psgl->phead_dma = (psgl->dma_addr + sizeof(sgl_list_info_t))  ;
		psgl->ptail = psgl->phead;
		psgl->ptail_dma = psgl->phead_dma;
	}
	else {
		if(p_dma_ch->int_on_final_sg) {
			/* mask out all dma interrupts, except error, on tail
			   before adding new tail. */
			psgl->ptail->control_count &=
				~(SG_TCI_ENABLE | SG_ETI_ENABLE);
		}
		psgl->ptail->next = psgl->ptail_dma + sizeof(ppc_sgl_t);
		psgl->ptail++; 
		psgl->ptail_dma += sizeof(ppc_sgl_t);
	}
	psgl->ptail->control = psgl->control | SET_DMA_PW(p_dma_ch->pwidth);
#if !defined(CONFIG_APM82181)
	/* Move to Highband segment to expect higher performance */
	psgl->ptail->src_addr = src_addr | (0x8ULL << 32);
	psgl->ptail->dst_addr = dst_addr | (0x8ULL << 32);
#else /* APM821x1 */
	psgl->ptail->src_addr = src_addr;
        psgl->ptail->dst_addr = dst_addr;
#endif
#ifdef DEBUG_SPLICE_DMA
	psgl->ptail->control_count = (count >> p_dma_ch->shift);
		// | psgl->sgl_control;
#endif
	psgl->ptail->control_count = (count >> p_dma_ch->shift) | psgl->sgl_control;

	psgl->ptail->next = (uint32_t) NULL;

#ifdef DEBUG_SPLICE_DMA
	printk("count=%d control=0x%08x p_dma_ch->pwidth=%d bits=0x%08x\n",
				count, psgl->ptail->control, p_dma_ch->pwidth, SET_DMA_PW(p_dma_ch->pwidth)); 
	printk("src_addr=0x%llx\n", psgl->ptail->src_addr); 
	printk("dst_addr=0x%llx\n", psgl->ptail->dst_addr); 
	printk("control_count=0x%08x\n", psgl->ptail->control_count);
	printk("sgl_control=0x%08x\n", psgl->sgl_control);
#endif

	return DMA_STATUS_GOOD;

}


/*
 * Enable (start) the DMA described by the sgl handle.
 */
int ppc460ex_enable_dma_sgl(ppc460ex_plb_dma_dev_t *adev, sgl_handle_t handle)
{
  sgl_list_info_t *psgl = (sgl_list_info_t *)handle;
  ppc460ex_plb_dma_ch_t *p_dma_ch;
  uint32_t sg_command;
  

  if (unlikely(!handle)) {
    printk("%s: null handle\n", __FUNCTION__);
    return DMA_STATUS_BAD_HANDLE;
  }
  if (unlikely(psgl->ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, psgl->ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  p_dma_ch = adev->chan[psgl->ch_id];
  psgl->ptail->control_count &= ~SG_LINK;	/* make this the last dscrptr */
  sg_command = mfdcr(DCR_DMA2P40_SGC);
  ppc460ex_set_sg_addr(psgl->ch_id, psgl->phead_dma);
  sg_command |= SSG_ENABLE(psgl->ch_id);
//PMB - work around for PLB
  sg_command &= 0xF0FFFFFF;
  mtdcr(DCR_DMA2P40_SGC, sg_command); /* start transfer */

  return 0;
}

/*
 * Halt an active scatter/gather DMA operation.
 */
int ppc460ex_disable_dma_sgl(ppc460ex_plb_dma_dev_t *adev, sgl_handle_t handle)
{
  sgl_list_info_t *psgl = (sgl_list_info_t *) handle;
  uint32_t sg_command;
  
  if (unlikely(!handle)) {
    printk("%s: null handle\n", __FUNCTION__);
    return DMA_STATUS_BAD_HANDLE;
  }
  if (unlikely(psgl->ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, psgl->ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  sg_command = mfdcr(DCR_DMA2P40_SGC);
  sg_command &= ~SSG_ENABLE(psgl->ch_id);
  mtdcr(DCR_DMA2P40_SGC, sg_command); /* stop transfer */
  return 0;
}


/*
 *  Returns number of bytes left to be transferred from the entire sgl list.
 *  *src_addr and *dst_addr get set to the source/destination address of
 *  the sgl descriptor where the DMA stopped.
 *
 *  An sgl transfer must NOT be active when this function is called.
 */
int ppc460ex_get_dma_sgl_residue(ppc460ex_plb_dma_dev_t *adev, sgl_handle_t handle, phys_addr_t * src_addr,
			   phys_addr_t * dst_addr)
{
  sgl_list_info_t *psgl = (sgl_list_info_t *) handle;
  ppc460ex_plb_dma_ch_t *p_dma_ch;
  ppc_sgl_t *pnext, *sgl_addr;
  uint32_t count_left;

  if (unlikely(!handle)) {
    printk("%s: null handle\n", __FUNCTION__);
    return DMA_STATUS_BAD_HANDLE;
  }
  if (unlikely(psgl->ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, psgl->ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }

  sgl_addr = (ppc_sgl_t *) __va(mfdcr(DCR_DMA2P40_SGL0 + (psgl->ch_id * 0x8)));
  count_left = mfdcr(DCR_DMA2P40_CTC0 + (psgl->ch_id * 0x8)) & SG_COUNT_MASK;
  if (unlikely(!sgl_addr)) {
    printk("%s: sgl addr register is null\n", __FUNCTION__);
    goto error;
  }
  pnext = psgl->phead;
  while (pnext &&
	 ((unsigned) pnext < ((unsigned) psgl + SGL_LIST_SIZE) &&
	  (pnext != sgl_addr))
	 ) {
    pnext++;
  }
  if (pnext == sgl_addr) {	/* found the sgl descriptor */
    
    *src_addr = pnext->src_addr;
    *dst_addr = pnext->dst_addr;
    
    /*
     * Now search the remaining descriptors and add their count.
     * We already have the remaining count from this descriptor in
     * count_left.
     */
    pnext++;
    
    while ((pnext != psgl->ptail) &&
	   ((unsigned) pnext < ((unsigned) psgl + SGL_LIST_SIZE))
	   ) {
      count_left += pnext->control_count & SG_COUNT_MASK;
    }
    if (unlikely(pnext != psgl->ptail)) {	/* should never happen */
      printk
	("%s:error (1) psgl->ptail 0x%x handle 0x%x\n", __FUNCTION__,
	 (unsigned int) psgl->ptail, (unsigned int) handle);
      goto error;
    }
    /* success */
    p_dma_ch = adev->chan[psgl->ch_id];
    return (count_left << p_dma_ch->shift);	/* count in bytes */
    
  } else {
    /* this shouldn't happen */
    printk
      ("get_dma_sgl_residue, unable to match current address 0x%x, handle 0x%x\n",
       (unsigned int) sgl_addr, (unsigned int) handle);
    
  }
  
 error:
  src_addr = NULL;
  dst_addr = NULL;
  return 0;

}

/*
 * Returns the address(es) of the buffer(s) contained in the head element of
 * the scatter/gather list.  The element is removed from the scatter/gather
 * list and the next element becomes the head.
 *
 * This function should only be called when the DMA is not active.
 */
int ppc460ex_delete_dma_sgl_element(sgl_handle_t handle, phys_addr_t * src_dma_addr, 
				    phys_addr_t * dst_dma_addr)
{
  sgl_list_info_t *psgl = (sgl_list_info_t *) handle;

  if (unlikely(!handle)) {
    printk("%s: null handle\n", __FUNCTION__);
    return DMA_STATUS_BAD_HANDLE;
  }
  if (unlikely(psgl->ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, psgl->ch_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  
  if (unlikely(!psgl->phead)) {
    printk("%s: sgl list empty\n", __FUNCTION__);
    src_dma_addr = NULL;
    dst_dma_addr = NULL;
    return DMA_STATUS_SGL_LIST_EMPTY;
  }
  
  *src_dma_addr = (phys_addr_t) psgl->phead->src_addr;
  *dst_dma_addr = (phys_addr_t) psgl->phead->dst_addr;
  
  if (psgl->phead == psgl->ptail) {
    /* last descriptor on the list */
    psgl->phead = NULL;
    psgl->ptail = NULL;
  } else {
    psgl->phead++;
    psgl->phead_dma += sizeof(ppc_sgl_t);
  }
  
  return DMA_STATUS_GOOD;
  
}


/*
 *   Create a scatter/gather list handle.  This is simply a structure which
 *   describes a scatter/gather list.
 *
 *   A handle is returned in "handle" which the driver should save in order to
 *   be able to access this list later.  A chunk of memory will be allocated
 *   to be used by the API for internal management purposes, including managing
 *   the sg list and allocating memory for the sgl descriptors.  One page should
 *   be more than enough for that purpose.  Perhaps it's a bit wasteful to use
 *   a whole page for a single sg list, but most likely there will be only one
 *   sg list per channel.
 *
 *   Interrupt notes:
 *   Each sgl descriptor has a copy of the DMA control word which the DMA engine
 *   loads in the control register.  The control word has a "global" interrupt
 *   enable bit for that channel. Interrupts are further qualified by a few bits
 *   in the sgl descriptor count register.  In order to setup an sgl, we have to
 *   know ahead of time whether or not interrupts will be enabled at the completion
 *   of the transfers.  Thus, enable_dma_interrupt()/disable_dma_interrupt() MUST
 *   be called before calling alloc_dma_handle().  If the interrupt mode will never
 *   change after powerup, then enable_dma_interrupt()/disable_dma_interrupt()
 *   do not have to be called -- interrupts will be enabled or disabled based
 *   on how the channel was configured after powerup by the hw_init_dma_channel()
 *   function.  Each sgl descriptor will be setup to interrupt if an error occurs;
 *   however, only the last descriptor will be setup to interrupt. Thus, an
 *   interrupt will occur (if interrupts are enabled) only after the complete
 *   sgl transfer is done.
 */
int ppc460ex_alloc_dma_handle(ppc460ex_plb_dma_dev_t *adev, sgl_handle_t *phandle,
			      unsigned int mode, unsigned int ch_id)
{
  sgl_list_info_t *psgl=NULL;
  static dma_addr_t dma_addr;
  ppc460ex_plb_dma_ch_t *p_dma_ch = adev->chan[ch_id];
  uint32_t sg_command;
#if 0
  void *ret;
#endif
  if (unlikely(ch_id >= MAX_PPC460EX_DMA_CHANNELS)) {
    printk("%s: bad channel %d\n", __FUNCTION__, p_dma_ch->chan_id);
    return DMA_STATUS_BAD_CHANNEL;
  }
  if (unlikely(!phandle)) {
    printk("%s: null handle pointer\n", __FUNCTION__);
    return DMA_STATUS_NULL_POINTER;
  }
#if 0
  /* Get a page of memory, which is zeroed out by consistent_alloc() */
  ret = dma_alloc_coherent(NULL, DMA_PPC4xx_SIZE, &dma_addr, GFP_KERNEL);
  if (ret != NULL) {
    memset(ret, 0, DMA_PPC4xx_SIZE);
    psgl = (sgl_list_info_t *) ret;
  }
#else

 if(!dma_mem_page) {
   dma_mem_page = dma_alloc_coherent(adev->dev, DMA_PPC4xx_SIZE, &dma_addr, GFP_KERNEL);
   if (unlikely(!dma_mem_page)){
      printk("dma_alloc_coherent failed\n");
      return -1;
    }
  }

  psgl = (sgl_list_info_t *) dma_mem_page;
#endif
     
  
  if (unlikely(psgl == NULL)) {
    *phandle = (sgl_handle_t) NULL;
    return DMA_STATUS_OUT_OF_MEMORY;
  }
 
  
  psgl->dma_addr = dma_addr;
  psgl->ch_id = ch_id;
  /*
   * Modify and save the control word. These words will be
   * written to each sgl descriptor.  The DMA engine then
   * loads this control word into the control register
   * every time it reads a new descriptor.
   */
  psgl->control = p_dma_ch->control;
  /* Clear all mode bits */
  psgl->control &= ~(DMA_TM_MASK | DMA_TD);
  /* Save control word and mode */
  psgl->control |= (mode | DMA_CE_ENABLE);
  /* In MM mode, we must set ETD/TCE */
  if (mode == DMA_MODE_MM) {	/* PMB - Workaround */
    psgl->control |= DMA_ETD_OUTPUT | DMA_TCE_ENABLE;
    psgl->control &= 0xFFFCFFFF;
    psgl->control |= 0x00020000;
  }
  
  if (p_dma_ch->int_enable) {
    /* Enable channel interrupt */
    psgl->control |= DMA_CIE_ENABLE;
  } else {
    psgl->control &= ~DMA_CIE_ENABLE;
  }
  psgl->control &= ~DMA_CIE_ENABLE;
  sg_command = mfdcr(DCR_DMA2P40_SGC);
  sg_command |= SSG_MASK_ENABLE(ch_id);


  /*Enable SGL control access */
  mtdcr(DCR_DMA2P40_SGC, sg_command);
  psgl->sgl_control = SG_ERI_ENABLE | SG_LINK;

  
  p_dma_ch->int_enable=0;
  if (p_dma_ch->int_enable) {
    if (p_dma_ch->tce_enable)
      psgl->sgl_control |= SG_TCI_ENABLE | SG_ETI_ENABLE;
    else
      psgl->sgl_control |= SG_ETI_ENABLE | SG_TCI_ENABLE; 
  }
  
  *phandle = (sgl_handle_t) psgl;
  return DMA_STATUS_GOOD;
  
}

/*
 * Destroy a scatter/gather list handle that was created by alloc_dma_handle().
 * The list must be empty (contain no elements).
 */
void
ppc460ex_free_dma_handle(sgl_handle_t handle)
{
	sgl_list_info_t *psgl = (sgl_list_info_t *) handle;

	if (!handle) {
	  printk("%s: got NULL\n", __FUNCTION__);
		return;
	} else if (psgl->phead) {
	  printk("%s: list not empty\n", __FUNCTION__);
		return;
	} else if (!psgl->dma_addr) {	/* should never happen */
	  printk("%s: no dma address\n", __FUNCTION__);
		return;
	}

	//dma_free_coherent(NULL, DMA_PPC4xx_SIZE, (void *) psgl, 0);
}

#if 0

int test_sgdma_memcpy(void *src, void *dst, void *src1, void *dst1, unsigned int length, unsigned int dma_ch_id)
{
  ppc460ex_plb_dma_dev_t *device;
  ppc460ex_plb_dma_ch_t p_init;
  phys_addr_t dma_dest, dma_src;
  phys_addr_t dma_dest1, dma_src1;
  int res = 0;
  ppc460ex_plb_dma_ch_t *new_chan;
  unsigned int control;
  u32 status = 0;
  u32 value = 0;
  sgl_handle_t handle_p;

   /* create a device */
  if ((device = kzalloc(sizeof(*device), GFP_KERNEL)) == NULL) {
    res = -ENOMEM;
  }

  if ((new_chan = kzalloc(sizeof(ppc460ex_plb_dma_ch_t), GFP_KERNEL)) == NULL) {
	   printk("ERROR:No Free memory for allocating dma channels\n");
	   res = -ENOMEM;
  }
  
  dma_src = dma_map_single(p_init.device->dev, src, length,
			   DMA_TO_DEVICE);
  dma_dest = dma_map_single(p_init.device->dev, dst, length,
			    DMA_FROM_DEVICE);

  dma_src1 = dma_map_single(p_init.device->dev, src1, length,
			   DMA_TO_DEVICE);
  dma_dest1 = dma_map_single(p_init.device->dev, dst1, length,
			    DMA_FROM_DEVICE);

  memset(new_chan, 0 , sizeof(ppc460ex_plb_dma_ch_t));
  device->chan[dma_ch_id] = new_chan;

  
  memset((char *)&p_init, sizeof(p_init), 0);  
  p_init.polarity = 0;
  p_init.pwidth = PW_32;
  p_init.in_use = 0;
  p_init.sai = 1;
  p_init.dai = 1;
  p_init.tce_enable = 1;
  //printk("%s:channel id = %d\n", __FUNCTION__, dma_ch_id);

  res = ppc460ex_init_dma_channel(device, dma_ch_id, &p_init);


  ppc460ex_set_dma_count(device, dma_ch_id, length);

  res = ppc460ex_enable_dma_interrupt(device, dma_ch_id);
  if (res) {
    printk("%32s: en/disable_dma_interrupt\n",
	   __FUNCTION__);
  }

  res = ppc460ex_alloc_dma_handle(device, &handle_p, DMA_MODE_MM, dma_ch_id);

  ppc460ex_add_dma_sgl(device, handle_p, dma_src, dma_dest, length);
  ppc460ex_add_dma_sgl(device, handle_p, dma_src1, dma_dest1, length);
  
  ppc460ex_enable_dma_sgl(device, handle_p);
  

  /*do {
    value = mfdcr(DCR_DMA2P40_SR);
    }while ((value & 0x80000000) != 0x80000000);*/
  
#if DEBUG_TEST  
  printk("%s:out:dump src \n", __FUNCTION__);
  DMA_HEXDUMP(src, length);
  printk("%s:out:dump dst\n", __FUNCTION__);
  DMA_HEXDUMP(dst, length);
  printk("%s:out:dump src1 \n", __FUNCTION__);
  DMA_HEXDUMP(src1, length);
  printk("%s:out:dump dst1\n", __FUNCTION__);
  DMA_HEXDUMP(dst1, length);
#endif  

  if (memcmp(src, dst, length) || memcmp(src1, dst1, length)) {
    printk("Self-test copy failed compare, disabling\n");
    res = -ENODEV;
    goto out;
  }
  
  return 0;
 out:
  
  return res;

} 
#endif

#ifdef SPLICE_DMA_COHERENT
int
ppc460ex_sgdma_pipebufs_memcpy(struct pipe_inode_info *pipe, void *dest, dma_addr_t dma_dest, unsigned int length) 
#else
int
ppc460ex_sgdma_pipebufs_memcpy(struct pipe_inode_info *pipe, void *dest, unsigned int length)
#endif
{
	sgl_list_info_t *psgl;
	ppc460ex_plb_dma_dev_t *device;
	ppc460ex_plb_dma_ch_t p_init;
#ifndef SPLICE_DMA_COHERENT
	dma_addr_t dma_dest;
#endif
	//dma_addr_t dma_addrs[32];
	phys_addr_t dma_src;
	ppc460ex_plb_dma_ch_t *new_chan;
	sgl_handle_t handle_p;
	int dma_ch_id;
	void *src;
	int nrbufs = pipe->nrbufs;
	int res = 0;
	int len = 0;
#ifdef DEBUG_SPLICE_DMA
	char *s_vaddr = NULL, *d_vaddr = NULL;
	char strbuf[256];
	int firstbuf=0;
#endif
	int curbuf = pipe->curbuf;	/* stash away pipe->curbuf */

#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - dest = %p, length = %d len = %d\n",
			__FILE__, __FUNCTION__, __LINE__, dest, length, len);
#endif

	if(unlikely(!nrbufs))
		return -EFAULT;

	/* create a device */
	if(unlikely ((device = kzalloc(sizeof(*device), GFP_KERNEL)) == NULL)) {
		res = -ENOMEM;
	}

	if(unlikely ((new_chan = kzalloc(sizeof(ppc460ex_plb_dma_ch_t), GFP_KERNEL)) == NULL)) {
		printk("ERROR:No Free memory for allocating dma channels\n");
		res = -ENOMEM;
	}

	memset(new_chan, 0 , sizeof(ppc460ex_plb_dma_ch_t));
	dma_ch_id = ppc460ex_get_dma_channel();
	if(unlikely(dma_ch_id == -ENODEV))
		return dma_ch_id;

	device->chan[dma_ch_id] = new_chan;
	memset((char *)&p_init, 0, sizeof(ppc460ex_plb_dma_ch_t));
	p_init.polarity = 0;
	p_init.pwidth = PW_8;
	p_init.in_use = 0;
	p_init.sai = 1;
	p_init.dai = 1;
	p_init.tce_enable = 1;


	res = ppc460ex_init_dma_channel(device, dma_ch_id, &p_init);
	if(unlikely(res != DMA_STATUS_GOOD))
		goto out;

	init_waitqueue_head(&device->queue);

	/* ppc460ex_disable_dma_interrupt(device, dma_ch_id);
	ppc460ex_disable_burst(device, dma_ch_id); */
	res = ppc460ex_alloc_dma_handle(device, &handle_p, DMA_MODE_MM, dma_ch_id);
	if(unlikely(res != DMA_STATUS_GOOD))
		goto out;


#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - nrbufs = %d pipe->curbuf = %d\n",
			__FILE__, __FUNCTION__, __LINE__, nrbufs, pipe->curbuf);
#endif
	for(;;) {
		if(nrbufs) {
			struct pipe_buffer *buf = pipe->bufs + curbuf;
#ifdef DEBUG_SPLICE_DMA
			printk("%s:%s:%d - buf[%d] buf->len=%d length=%d len=%d\n",
					__FILE__, __FUNCTION__, __LINE__, curbuf, buf->len, length, len);
#endif
			if(len < length) {

				if(!buf->len)
					continue;

				src = page_address(buf->page);
				dma_src = dma_map_single(p_init.device->dev, src + buf->offset, buf->len, DMA_TO_DEVICE);
#ifndef SPLICE_DMA_COHERENT
				dma_dest = dma_map_single(p_init.device->dev, dest + len, buf->len, DMA_FROM_DEVICE);
#endif

#ifdef DEBUG_SPLICE_DMA
				printk("maping %d src: %p, dest: %p, buf->len=%d dma_dest = 0x%08x\n",
							curbuf, src + buf->offset, dest+len, buf->len, dma_dest);
				printk("ADDING BUF NUMBER %d\n\n\n\n", curbuf);
#endif
				ppc460ex_add_dma_sgl(device, handle_p, dma_src, dma_dest, buf->len);
				len += buf->len;
				curbuf = (curbuf + 1) & (PIPE_BUFFERS - 1);
				--nrbufs;
			}
			else	/* all pipe buf elements mapped to sgl */
				break;
		}
		else
			break;
	}


	__dma_sync(dest, length, DMA_FROM_DEVICE);
	ppc460ex_enable_dma_sgl(device, handle_p);

#if 0
	res = wait_event_interruptible(device->queue, PPC460EX_DMA_CHAN_SGXFR_COMPLETE(dma_ch_id));
#else
	res = poll_for_sgdma_done(dma_ch_id);
#endif

	if(unlikely(res)) {
		printk("%s:%s:%d - Timeout while waiting for SG Xfr to complete\n",
			__FILE__, __FUNCTION__, __LINE__);
		printk("dma_status = 0x%08x\n", DMA_STATUS(dma_ch_id));
	}

	/* Check the error status bits */
	printk("DCR_DMA2P40_SR=0x%x\n",mfdcr(DCR_DMA2P40_SR));
	if(unlikely(mfdcr(DCR_DMA2P40_SR) & (1 << (23 + dma_ch_id)))) {
		printk(KERN_ERR"Error happened in the channel %d\n",dma_ch_id);
		printk("DCR_DMA2P40_SR=0x%x\n",mfdcr(DCR_DMA2P40_SR));
	}

	mtdcr(DCR_DMA2P40_SR, 0xFFFFFFFF);


#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - dma status = 0x%08x\n", __FILE__, __FUNCTION__, __LINE__, DMA_STATUS(dma_ch_id)); 
#endif


	/* Hack */
	psgl = (sgl_list_info_t *) handle_p;
	psgl->phead = NULL;
	ppc460ex_free_dma_handle(handle_p);
	handle_p = 0;

#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - returning res = %d\n", __FILE__, __FUNCTION__, __LINE__, res);

	struct pipe_buffer *buf = pipe->bufs + pipe->curbuf;
	memset(strbuf, 0, 256);
	s_vaddr = page_address(buf->page);
	memcpy(strbuf, s_vaddr+buf->offset, 255);
	*(strbuf+255) = '\0';
	printk("%s:%s:%d - source strbuf is %s\n", __FILE__, __FUNCTION__, __LINE__, strbuf);

	d_vaddr = dest;
	memset(strbuf, 0, 256);
	memcpy(strbuf, d_vaddr, 255);
	*(strbuf+255) = '\0';
	printk("%s:%s:%d - dest strbuf is %s\n", __FILE__, __FUNCTION__, __LINE__, strbuf);
#endif

out:
	
	return res;
}

int splice_dma_memcpy(struct splice_dma_desc *sd_p, unsigned int len)
{
	sgl_list_info_t *psgl;
	//static ppc460ex_plb_dma_dev_t *device;
	ppc460ex_plb_dma_ch_t p_init;
	dma_addr_t dma_dest;
	phys_addr_t dma_src;
	ppc460ex_plb_dma_ch_t *new_chan;
	unsigned int  size = 0;
	sgl_handle_t handle_p;
	int dma_ch_id;
	void *src=NULL, *dst=NULL;
	int res = 0;
	int i = 0;
	int dma_xfr_size=0;
	dma_ch_id = ppc460ex_get_dma_channel();

#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - sd_p->n_elems=%d, len = %d \n",
			__FILE__, __FUNCTION__, __LINE__, sd_p->n_elems, len);
#endif

	if(unlikely(!sd_p->n_elems))
		return -EFAULT;
	new_chan = adev->chan[dma_ch_id];
	p_init.polarity = 0;
	p_init.pwidth = PW_8;
	p_init.in_use = 0;
	p_init.sai = 1;
	p_init.dai = 1;
	p_init.tce_enable = 1;


	res = ppc460ex_init_dma_channel(adev, dma_ch_id, &p_init);
	if(unlikely(res != DMA_STATUS_GOOD))
		goto out;

	init_waitqueue_head(&adev->queue);

	ppc460ex_enable_burst(adev, dma_ch_id);
	res = ppc460ex_alloc_dma_handle(adev, &handle_p, DMA_MODE_MM, dma_ch_id);
	if(unlikely(res != DMA_STATUS_GOOD))
		goto out;

	for(i=0; i<sd_p->n_elems; i++) {
		src = (void *)(sd_p->src_addrs[i]);
		dst = (void *)(sd_p->dst_addrs[i]);
		size = sd_p->xfr_size[i];
#ifdef DEBUG_SPLICE_DMA
		printk(KERN_DEBUG "index=%d src=0x%08x dst=0x%08x size=%d\n", i, src, dst, size);;
#endif
		dma_src = dma_map_single(adev->dev, src, size, DMA_TO_DEVICE);
		dma_dest = dma_map_single(adev->dev, dst, size, DMA_FROM_DEVICE);
		ppc460ex_add_dma_sgl(adev, handle_p, dma_src, dma_dest, size);
		dma_xfr_size += size;
	}

#ifdef DEBUG_SPLICE_DMA
	printk(KERN_DEBUG "%s:%s:%d - dma_xfr_size=%d\n", __FILE__, __FUNCTION__, __LINE__, dma_xfr_size);
#endif

	dst = (void *)(sd_p->dst_addrs[0]);

	ppc460ex_enable_dma_sgl(adev, handle_p);

#if 0
	res = wait_event_interruptible(device->queue, PPC460EX_DMA_CHAN_SGXFR_COMPLETE(dma_ch_id));
#else
	res = poll_for_sgdma_done(dma_ch_id);
#endif

	if(unlikely(res)) {
		printk("%s:%s:%d - Timeout while waiting for SG Xfr to complete\n",
			__FILE__, __FUNCTION__, __LINE__);
		printk("dma_status = 0x%08x\n", DMA_STATUS(dma_ch_id));
	}

	/* Check the error status bits */
	if(unlikely(mfdcr(DCR_DMA2P40_SR) & (1 << (23 + dma_ch_id)))) {
		printk(KERN_ERR"Error happened in the channel %d\n",dma_ch_id);
		printk("DCR_DMA2P40_SR=0x%x\n",mfdcr(DCR_DMA2P40_SR));
	}
	mtdcr(DCR_DMA2P40_SR, 0xFFFFFFFF);



#ifdef DEBUG_SPLICE_DMA
	printk("%s:%s:%d - dma status = 0x%08x\n", __FILE__, __FUNCTION__, __LINE__, DMA_STATUS(dma_ch_id)); 
#endif

	for(i=0; i<sd_p->n_elems; i++) {
		dma_unmap_single(adev->dev, splice_src_dma_addrs[i], size, DMA_TO_DEVICE);
		dma_unmap_single(adev->dev, splice_dst_dma_addrs[i], size, DMA_FROM_DEVICE);
	}

	/* Hack to clean up dma handle without memset */
	psgl = (sgl_list_info_t *) handle_p;
	psgl->phead = NULL;
	psgl->ptail = NULL;
	ppc460ex_free_dma_handle(handle_p);
	handle_p = 0;

out:
	return res;
}



EXPORT_SYMBOL(ppc460ex_alloc_dma_handle);
EXPORT_SYMBOL(ppc460ex_free_dma_handle);
EXPORT_SYMBOL(ppc460ex_add_dma_sgl);
EXPORT_SYMBOL(ppc460ex_delete_dma_sgl_element);
EXPORT_SYMBOL(ppc460ex_enable_dma_sgl);
EXPORT_SYMBOL(ppc460ex_disable_dma_sgl);
EXPORT_SYMBOL(ppc460ex_get_dma_sgl_residue);
EXPORT_SYMBOL(ppc460ex_sgdma_pipebufs_memcpy);
EXPORT_SYMBOL(splice_dma_memcpy);
