/**
 * AppliedMicro DB9xxx FrameBuffer Driver
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>
 *                      Ravi Patel <rapatel@apm.com>
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
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/clk.h>
#include <asm/ipp.h>
#include "apm_lcd_csr.h"
#include <asm/cacheflush.h>

#define APMLCDPFX		"APMLCD: "
#define RES_SIZE(r)		((r)->end - (r)->start + 1)

/* Must define as LCD does not support coherency */
#define APM_LCDFB_NOT_COHERENT_MEM	

#undef DEBUG
#undef DEBUGX


#ifdef DEBUG
#define DPRINT(x...) printk(x)
#else
#define DPRINT(x...) do{}while(0)
#endif

#ifdef DEBUGX
#define DPRINTX(x...) printk(x)
#else
#define DPRINTX(x...) do{}while(0)
#endif

#define MAX_OFB		16	/* Maximum overlay supported by HW */
#define MAX_FRAME	3	/* Maximum frame buffer support */

#define LCD_INT_CLK_MHZ	25	/* Internal LCD clock */

#define NONSTD_TO_XPOS(x)	(((x) >> 0)  & 0xfff)
#define NONSTD_TO_YPOS(x)	(((x) >> 16) & 0xfff)
#define XYPOS_TO_NONSTD(x,y)	(((y) << 16) | (x))

#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC	_IOW('F', 0x20, __u32) /* wait for vsync */
#endif

struct apm_lcd_info;

/**
 * Descriptor structure for frame link list mode
 *
 */
struct apm_lcd_desc {
	u32 fdna : 31;
	u32 fdnav : 1;
	u32 fdowere : 1;
	u32 fdower : 12;
	u32 flc : 1;
	u32 fnc : 1;
	u32 fdowe : 16;
	u32 fddbare : 1;
	u32 fddbar;
	u32 fddear;
	u32 fdowdbar[MAX_OFB];
		
} __attribute__((packed,aligned(4)));

/*
 * Per-Overlay structure
 * 
 * Each overlay is exposed as an frame buffer device
 *
 */
struct apm_lcd_ofb_info {
	struct fb_info		fb;	/* Must be first */
	int			id;
	atomic_t		usage;

	void __iomem		*video_mem;
	phys_addr_t		video_mem_phys;
	size_t			video_mem_size;
	
	struct apm_lcd_info	*ctx;
};

/* Private data shared between all framebuffer(s) */
struct apm_lcd_info {
	struct device	*dev;
	u8	*csr;
	u8	*diag;
	struct	clk *clk;
	int	irq;	
	struct apm_lcd_ofb_info overlay[MAX_OFB];	 
	
	u8	lcdc_pwred_off;
	
	struct backlight_device *bl;	   /* Backlight */
	unsigned int		bl_period; /* Backlight period in ns */	

	struct fb_info	*fb;		/* HW has only single head */
	wait_queue_head_t wait_vsync;	/* For detection of frame complete */
	u64 vblank_cnt;			/* # of complete frame */
	struct apm_lcd_desc *desc_ptr;	/* Allocated frame descriptor ptr */
	struct apm_lcd_desc *desc;	/* Usable frame descriptor ptr */
	u32 next_dear;			/* Next DEAR/DBAR value to update to */
	u32 next_dbar;
};

/* Per-framebuffer structure */
struct apm_lcd_par {
	u32	pseudo_palette[16];
	u8	cache_pallete[512];	/* cached pallete for hibernation */
	struct apm_lcd_info *ctx;
};

static struct fb_fix_screeninfo apm_lcdfb_fix __devinitdata = {
	.id =		"APM LCD", 
	.type =		FB_TYPE_PACKED_PIXELS,
	.visual =	FB_VISUAL_TRUECOLOR,
	.xpanstep =	8,
	.ypanstep =	1,
	.ywrapstep =	0, 
	.accel =	FB_ACCEL_NONE,
};

static struct fb_var_screeninfo apm_lcdfb_vfb __devinitdata = {
	.xres =		640,
	.yres =		480,
	.xres_virtual =	640,
	.yres_virtual =	480,
	.bits_per_pixel = 24,
	.red =		{  0, 8, 0 },
      	.green =	{  8, 8, 0 },
      	.blue =		{ 16, 8, 0 },
      	.activate =	FB_ACTIVATE_TEST,
      	.height =	-1,
      	.width =	-1,
      	.pixclock =	25000,
      	.left_margin =	0x28,
      	.right_margin =	0x28,
      	.upper_margin =	0x0B,
      	.lower_margin =	0x0B,
      	.hsync_len =	0x4F,
      	.vsync_len =	0x15,
      	.vmode =	FB_VMODE_NONINTERLACED,
};

/* Driver data */
static char *mode_option __devinitdata;
static int apm_lcdfb_init_firsttime = 1;
struct apm_lcd_info *lcdctx = NULL;

static unsigned long ops1 = 1;
module_param(ops1, ulong, 0);
MODULE_PARM_DESC(ops1, "Enable 16-bts RGB output to LCD MSB "
			"(0 = disable; 1 = enable default)");

static unsigned long invert_lvds = 0;
module_param(invert_lvds, ulong, 0);
MODULE_PARM_DESC(invert_lvds, "Enable invert LVDS bus bit "
			"(0 = disable default; 1 = enable)");

static unsigned long bgr = 0;
module_param(bgr, ulong, 0);
MODULE_PARM_DESC(bgr, "Output mode (RGB = 0; BGR = 1)");

static unsigned long dep = 1;
module_param(dep, ulong, 0);
MODULE_PARM_DESC(dep, "Data enable polarity (0 = low; 1 = high default)");

static unsigned long lcd_max_pwm_freq = 28*1024;
module_param(lcd_max_pwm_freq, ulong, 0);
MODULE_PARM_DESC(lcd_max_pwm_freq, "Panel PWM max frequency in hz");

static unsigned long intel24_1_mode = 1;
module_param(intel24_1_mode, ulong, 0);
MODULE_PARM_DESC(intel24_1_mode, "Enable Intel 24.1 mode "
			"(0 = disable; 1 = enable default)");

static unsigned long max_overlay = 3;
module_param(max_overlay, ulong, 0);
MODULE_PARM_DESC(max_overlay, 
		"Specify maximum number of supported overlay "
		"(0 = disable; 1 - 16; 3 = default)");

static unsigned long frame_buffer_no = 2;
module_param(frame_buffer_no, ulong, 0);
MODULE_PARM_DESC(frame_buffer_no, 
		"Specify number of frame buffer (double, tripple, ...)"
		"(1 - 3; 2 = default)");

static unsigned long use_desc = 0;
module_param(use_desc, ulong, 0);
MODULE_PARM_DESC(use_desc, 
		"Enable frame descriptor feature (0 = disable default; "
		"1 = enable)");

static unsigned long ext_clk_khz = 25000;
module_param(ext_clk_khz, ulong, 0);
MODULE_PARM_DESC(ext_clk_khz, 
		"Enable external clock in kHz (0 = disable)");

module_param(mode_option, charp, 0);
MODULE_PARM_DESC(mode_option, "Initial video mode e.g. '648x480-8@60'");

static void apm_lcdfb_update_dbar(struct apm_lcd_info *ctx, 
				struct fb_info	*info, int now);
static int apm_lcd_backlight_update(struct backlight_device *bl);
static int apm_lcdfb_ioctl(struct fb_info *info, unsigned int cmd,
				unsigned long arg);
static int apm_lcdfb_wait_for_vsync(struct fb_info *info, long timeout);

static struct fb_videomode custom_modedb[] __devinitdata = {
	/* AUO G065VN01 Panel - 640x480-60 */
	{ "AUO-G065VN01", 60, 640, 480, 25000, 
		0x28, 0x28, 0x0B, 0x0B, 0x4F, 0x15,
	  	0, FB_VMODE_NONINTERLACED, 0},
	/* NOTE: Add new panel here */	  	
};

/* Helper functions */
static inline u32 INREG(struct apm_lcd_info *ctx, int reg)
{
	u32 val;
	
	val = in_le32((u32 *) (ctx->csr + reg));
	DPRINTX(APMLCDPFX "CSR RD 0x%p val: 0x%08X\n", ctx->csr + reg, val);
	return val;
}

static inline void OUTREG(struct apm_lcd_info *ctx, int reg, u32 val)	
{
	out_le32((u32 *) (ctx->csr + reg), val);
	DPRINTX(APMLCDPFX "CSR WR 0x%p val: 0x%08X\n", ctx->csr + reg, val);
}

static inline u32 INREG16(struct apm_lcd_info *ctx, int reg)
{
	u32 val = in_le16((void *) (ctx->csr + reg));
	DPRINTX(APMLCDPFX "CSR RD 0x%p val: 0x%04X\n", ctx->csr + reg, val);
	return val;
}

static inline void OUTREG16(struct apm_lcd_info *ctx, int reg, u16 val)	
{
	out_le16((u16 *) (ctx->csr + reg), val);
	DPRINTX(APMLCDPFX "CSR WR 0x%p val: 0x%04X\n", ctx->csr + reg, val);
}

static inline int bytes_per_line(struct fb_var_screeninfo *var)
{
	switch (var->bits_per_pixel) {
	case 1:
		return var->xres / 8;
	case 2:
		return var->xres / 4;
	case 4:
		return var->xres / 2;
	case 8:
	case 16:
	case 24:
	case 32:		
	default:	
		return var->xres * var->bits_per_pixel / 8;
	}
}

static inline u32 apm_lcdfb_mmap2hw(void *vaddr)
{
	/* For now, we do one to one translation. The higher nibble is 
	   taken care of by the START_ADDR_REGXX and END_ADDR_REGXX 
	   registers */
	return (u32) virt_to_phys(vaddr);
}

static unsigned int apm_lcdfb_fbsize(struct fb_info *info)
{
	return bytes_per_line(&info->var) * info->var.yres;
}

static int apm_lcdfb_check_var(struct fb_var_screeninfo *var, 
				struct fb_info *info)
{
	unsigned long tmp;

	/* check we can fit these values into the registers */
	if (var->hsync_len > 255 || var->vsync_len > 255) {
		DPRINT(APMLCDPFX "HSync or vsync length invalid\n");		
		return -EINVAL;
	}

	if (var->xres > 4096 || var->yres > 4096) {
		DPRINT(APMLCDPFX "xres or yres length invalid\n");		
		return -EINVAL;
	}
	
	/* Check line length for 128 bit aligned as required by hardware */
	tmp = bytes_per_line(var);
	if ((tmp & 15) != 0) {
		DPRINT(APMLCDPFX "line length invalid\n");		
		return -EINVAL;
	}

	/* Can cope with 1,2,4,8,16,18, or 24bpp */
	if (var->bits_per_pixel == 32)
		var->bits_per_pixel = 24;

	/* set r/g/b positions and validate bpp */
	switch(var->bits_per_pixel) {
	case 1:
	case 2:
	case 4:
		var->red.offset    = 0;
		var->red.length    = var->bits_per_pixel;
		var->green         = var->red;
		var->blue          = var->red;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;
	case 8:
		var->red.length		= var->bits_per_pixel;
		var->red.offset		= 0;
		var->green.length	= var->bits_per_pixel;
		var->green.offset	= 0;
		var->blue.length	= var->bits_per_pixel;
		var->blue.offset	= 0;
		var->transp.length	= 0;
		var->transp.offset	= 0;
		break;
	case 16:
		if (var->green.length == 6) {
			/* RGB565 */
			var->blue.offset	= 11;
			var->green.offset	= 5;
			var->red.offset		= 0;
			var->red.length		= 5;
			var->green.length	= 6;
			var->blue.length	= 5;
		} else {
			/* RGB 555 */
			var->blue.offset	= 10;
			var->green.offset	= 5;
			var->red.offset		= 0;
			var->red.length		= 5;
			var->green.length	= 5;
			var->blue.length	= 5;
		}
		break;
	case 24:
		var->red.offset		= 0;
		var->green.offset	= 8;
		var->blue.offset	= 16;
		var->red.length		= 8;
		var->green.length	= 8;
		var->blue.length	= 8;
		var->transp.offset	= 0;
		var->transp.length	= 0;
		break;
	default:
		return -EINVAL;
	}
	/* Fix up for double/triple buffering */
	var->xres_virtual = var->xres;
	if (var->yres_virtual > var->yres * frame_buffer_no)
		var->yres_virtual = var->yres * frame_buffer_no;
	return 0;
}

static int apm_lcd_ofb_check_var(struct fb_var_screeninfo *var, 
				struct fb_info *info)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;
	struct apm_lcd_info *ctx = ofb->ctx;
	int xpos;
	int ypos;

	if (var->xres > 4096 || var->yres > 4096) {
		DPRINT(APMLCDPFX "xres or yres length invalid\n");		
		return -EINVAL;
	}
	/* Can not be larger than the background frame buffer resolution */
	if (var->xres > ctx->fb->var.xres)
		var->xres = ctx->fb->var.xres;
	if (var->yres > ctx->fb->var.yres)
		var->yres = ctx->fb->var.yres;
	/* Must be the same BPP */
	var->bits_per_pixel = ctx->fb->var.bits_per_pixel;

	/* Limitation of hardware for this revision:
	   x position and width must be 32 pixels aligned */
	xpos = NONSTD_TO_XPOS(var->nonstd);
	ypos = NONSTD_TO_YPOS(var->nonstd);
	xpos = (xpos/32) * 32;
	var->xres = (var->xres/32) * 32;
	var->nonstd = XYPOS_TO_NONSTD(xpos, ypos); 

	/* set r/g/b positions and validate bpp */
	switch(var->bits_per_pixel) {
	case 1:
	case 2:
	case 4:
		var->red.offset    = 0;
		var->red.length    = var->bits_per_pixel;
		var->green         = var->red;
		var->blue          = var->red;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;
	case 8:
		var->red.length		= var->bits_per_pixel;
		var->red.offset		= 0;
		var->green.length	= var->bits_per_pixel;
		var->green.offset	= 0;
		var->blue.length	= var->bits_per_pixel;
		var->blue.offset	= 0;
		var->transp.length	= 0;
		var->transp.offset	= 0;
		break;
	case 16:
		if (var->green.length == 6) {
			/* RGB565 */
			var->blue.offset	= 11;
			var->green.offset	= 5;
			var->red.offset		= 0;
			var->red.length		= 5;
			var->green.length	= 6;
			var->blue.length	= 5;
		} else {
			/* RGB 555 */
			var->blue.offset	= 10;
			var->green.offset	= 5;
			var->red.offset		= 0;
			var->red.length		= 5;
			var->green.length	= 5;
			var->blue.length	= 5;
		}
		break;
	case 24:
		var->red.offset		= 0;
		var->green.offset	= 8;
		var->blue.offset	= 16;
		var->red.length		= 8;
		var->green.length	= 8;
		var->blue.length	= 8;
		var->transp.offset	= 0;
		var->transp.length	= 0;
		break;
	default:
		return -EINVAL;
	}
	var->xres_virtual = var->xres;
	if (var->yres_virtual > var->yres * frame_buffer_no)
		var->yres_virtual = var->yres * frame_buffer_no;
	return 0;
}

/**
 * Overlay Support functions 
 *
 */
static void apm_lcd_ofb_setup(struct apm_lcd_ofb_info *ofb)
{
	int i;
	int xpos;
	int ypos;
	struct apm_lcd_info *ctx = ofb->ctx;
	
	if (ofb->id >= max_overlay) {
		dev_err(ctx->dev, "descriptor for overlay %d not available\n", 
			ofb->id);
		return;
	}
	
	/* Load address to each descriptor */
	if (use_desc) { 
		for (i = 0; i < frame_buffer_no; i++) 
			ctx->desc[i].fdowdbar[ofb->id] =  
				apm_lcdfb_mmap2hw(ofb->video_mem);
	} else {
		OUTREG(ctx, LCDC_OWDBAR0_ADDR + ofb->id * 0x10, 
			apm_lcdfb_mmap2hw(ofb->video_mem +
				ofb->fb.var.yoffset * ofb->fb.fix.line_length));		
	}
	
	xpos = NONSTD_TO_XPOS(ofb->fb.var.nonstd);
	ypos = NONSTD_TO_YPOS(ofb->fb.var.nonstd);

	/* Load overlay X/Y width */
	OUTREG(ctx, LCDC_OXSER0_ADDR + ofb->id * 0x10, 
		OW_X_START0_WR(xpos) | 
		OW_X_END0_WR(xpos + ofb->fb.var.xres - 1)); 
	OUTREG(ctx, LCDC_OYSER0_ADDR + ofb->id * 0x10,
		OW_Y_START0_WR(ypos) | 
		OW_Y_END0_WR(ypos + ofb->fb.var.yres - 1));
			
	DPRINT(KERN_INFO APMLCDPFX 
		"overlay %d %d,%d width %d height %d\n",
		ofb->id, xpos, ypos, ofb->fb.var.xres, ofb->fb.var.yres);				
}

static void apm_lcd_ofb_enable(struct apm_lcd_ofb_info *ofb)
{
	int i;
	struct apm_lcd_info *ctx = ofb->ctx;
	
	if (ofb->id >= max_overlay) {
		dev_err(ctx->dev, "descriptor for overlay %d not available\n", 
			ofb->id);
		return;
	}
	
	/* Enable each descriptor overlay */
	if (use_desc) {
		for (i = 0; i < frame_buffer_no; i++)
			ctx->desc[i].fdowe |= 1 << ofb->id;
	} else {
		u32 val = INREG(ctx, LCDC_OWER_ADDR);
		OUTREG(ctx, LCDC_OWER_ADDR, val | (1 << ofb->id));
	}		
}

static void apm_lcd_ofb_disable(struct apm_lcd_ofb_info *ofb)
{
	int i;
	struct apm_lcd_info *ctx = ofb->ctx;
	
	if (ofb->id >= max_overlay) {
		dev_err(ctx->dev, "descriptor for overlay %d not available\n", 
			ofb->id);
		return;
	}
	
	/* Enable each descriptor overlay */
	if (use_desc) {
		for (i = 0; i < frame_buffer_no; i++)
			ctx->desc[i].fdowe &= ~(1 << ofb->id);
	} else {
		u32 val = INREG(ctx, LCDC_OWER_ADDR);
		OUTREG(ctx, LCDC_OWER_ADDR, val & ~(1 << ofb->id));
	}		
}

static int apm_lcd_ofb_open(struct fb_info *info, int user)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;

	/* no support for framebuffer console on overlay */
	if (user == 0)
		return -ENODEV;

	/* allow only one user at a time */
	if (atomic_inc_and_test(&ofb->usage))
		return -EBUSY;

	/* unblank the base framebuffer */
	fb_blank(ofb->ctx->fb, FB_BLANK_UNBLANK);
	return 0;
}

static int apm_lcd_ofb_release(struct fb_info *info, int user)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;

	apm_lcd_ofb_disable(ofb);
	atomic_dec(&ofb->usage);
	return 0;
}

static int apm_lcd_ofb_map_memory(struct apm_lcd_ofb_info *ofb)
{
	struct fb_var_screeninfo *var = &ofb->fb.var;
	int size;
	int bpp;

	bpp = var->bits_per_pixel;
	switch (bpp) {
	case 1:
		ofb->fb.fix.line_length = var->xres_virtual / 8;
		break;
	case 2:
		ofb->fb.fix.line_length = var->xres_virtual / 4;
		break;
	case 4:
		ofb->fb.fix.line_length = var->xres_virtual / 2;
		break;
	case 8:
	case 16:
		ofb->fb.fix.line_length = var->xres_virtual * bpp / 8;
		break;
	case 24:
	case 32: /* Max supported BPP is 24 */	
	default:	
		ofb->fb.fix.line_length = var->xres_virtual * 24 / 8;
		break;
	}

	size  = PAGE_ALIGN(ofb->fb.fix.line_length * var->yres_virtual);

	/* don't re-allocate if the original video memory is enough */
	if (ofb->video_mem && ofb->video_mem_size < size) {
		free_pages_exact(ofb->video_mem, ofb->video_mem_size);
		ofb->video_mem = NULL;
	}
	if (ofb->video_mem == NULL) {
		ofb->video_mem = alloc_pages_exact(size, 
					GFP_KERNEL | GFP_DMA | __GFP_ZERO);
		ofb->video_mem_size = size;
	}
	if (ofb->video_mem == NULL)
		return -ENOMEM;

	ofb->video_mem_phys     = virt_to_phys(ofb->video_mem);
	ofb->fb.fix.smem_start	= ofb->video_mem_phys;
	ofb->fb.fix.smem_len	= ofb->fb.fix.line_length * var->yres_virtual;
	ofb->fb.screen_base	= ofb->video_mem;
	DPRINT(KERN_INFO APMLCDPFX 
		"ofb%d PADDR 0x%02X.%08X VADDR 0x%p bbp %d line %d\n",
		ofb->id, 
		(u32) (ofb->video_mem_phys >> 32),
		(u32) ofb->video_mem_phys, 
		ofb->video_mem, bpp, ofb->fb.fix.line_length);
	
	return 0;
}

static int apm_lcd_ofb_set_par(struct fb_info *info)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;
	int ret;

	ret = apm_lcd_ofb_map_memory(ofb);
	if (ret)
		return ret;

	apm_lcd_ofb_setup(ofb);
	apm_lcd_ofb_enable(ofb);
	return 0;
}

static int apm_lcd_ofb_pan_display(struct fb_var_screeninfo *var,
                             	struct fb_info *info)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;
#if defined(APM_LCDFB_NOT_COHERENT_MEM)
	char __iomem *base_ptr;
#endif
        if (var->xoffset != 0 || 
            var->yoffset > (info->var.yres_virtual - info->var.yres))
        	return -EINVAL;
	info->var.yoffset = var->yoffset;
#if defined(APM_LCDFB_NOT_COHERENT_MEM)
	base_ptr = ofb->video_mem + info->var.yoffset * info->fix.line_length;				 
	flush_dcache_range((u32) base_ptr, 
			(u32) base_ptr + apm_lcdfb_fbsize(info));
#endif	
	/* Load address to each descriptor */
	if (!use_desc) { 
		OUTREG(ofb->ctx, LCDC_OWDBAR0_ADDR + ofb->id * 0x10, 
			apm_lcdfb_mmap2hw(ofb->video_mem +
				ofb->fb.var.yoffset * ofb->fb.fix.line_length));		
	}
	
	DPRINT(KERN_INFO APMLCDPFX 
		"ofb%d pan display %dx%d xoffset %d,%d\n",
		ofb->id, var->xres, var->yres, var->xoffset, var->yoffset);
	
	return 0;
}      

static int apm_lcd_ofb_ioctl(struct fb_info *info, unsigned int cmd,
				unsigned long arg)
{
	struct apm_lcd_ofb_info *ofb = (struct apm_lcd_ofb_info *) info;
	
	return apm_lcdfb_ioctl(ofb->ctx->fb, cmd, arg);				
}

static struct fb_ops apm_lcd_ofb_ops = {
	.owner		= THIS_MODULE,
	.fb_open	= apm_lcd_ofb_open,
	.fb_release	= apm_lcd_ofb_release,
	.fb_check_var 	= apm_lcd_ofb_check_var,
	.fb_set_par	= apm_lcd_ofb_set_par,
	.fb_pan_display = apm_lcd_ofb_pan_display,
	.fb_ioctl	= apm_lcd_ofb_ioctl,
};

static void __devinit apm_lcd_ofb_init_one(struct apm_lcd_info *ctx,
				struct apm_lcd_ofb_info *ofb, int id)
{
	sprintf(ofb->fb.fix.id, "overlay%d", id + 1);

	ofb->fb.fix.type		= FB_TYPE_PACKED_PIXELS;
	ofb->fb.fix.xpanstep		= 0;
	ofb->fb.fix.ypanstep		= 0;
	ofb->fb.fix.ywrapstep		= 0;

	ofb->fb.var.activate		= FB_ACTIVATE_NOW;
	ofb->fb.var.height		= -1;
	ofb->fb.var.width		= -1;
	ofb->fb.var.vmode		= FB_VMODE_NONINTERLACED;

	ofb->fb.fbops			= &apm_lcd_ofb_ops;
	ofb->fb.flags			= FBINFO_FLAG_DEFAULT;
	ofb->fb.node			= -1;
	ofb->fb.pseudo_palette		= NULL;

	ofb->id = id;
	atomic_set(&ofb->usage, 0);
	
	ofb->video_mem = NULL;
	ofb->video_mem_size = 0;
	ofb->ctx = ctx;
}

static int __devinit apm_lcd_ofb_init(struct apm_lcd_info *ctx)
{
	int i, ret;

	if (max_overlay <= 0)
		return 0;
		 
	for (i = 0; i < max_overlay; i++) {
		apm_lcd_ofb_init_one(ctx, &ctx->overlay[i], i);
		ret = register_framebuffer(&ctx->overlay[i].fb);
		if (ret) {
			dev_err(ctx->dev, "failed to register overlay %d\n", i);
			return ret;
		}
	}

	pr_info("APM LCD %ld overlay driver loaded successfully!\n", 
		max_overlay);
	return 0;
}

static void __devexit apm_lcd_ofb_exit(struct apm_lcd_info *ctx)
{
	int i;

	for (i = 0; i < max_overlay; i++)
		unregister_framebuffer(&ctx->overlay[i].fb);
}

static int apm_lcd_map2hwbpp(int bits_per_pixel)
{
	switch (bits_per_pixel) {
	case 1:
		return 0;
	case 2:
		return 1;
	case 4: 
		return 2;
	default:
	case 8:
		return 3;
	case 15:
	case 16:
		return 4;
	case 18:
		return 5;
	case 24:
		return 6;  
	}
}

static void apm_lcdfb_enable(struct apm_lcd_info *ctx, int enable)
{
	u32 cr1;
	
	/* Enable/disable the controller and LCD power */
	cr1 = INREG(ctx, LCDC_CR1_ADDR);
	if (enable) {
		cr1 |= LCE1_MASK;
		cr1 |= LPE1_MASK;	 
	} else {
		cr1 &= ~LCE1_MASK;
		cr1 &= ~LPE1_MASK;	 
	}
	OUTREG(ctx, LCDC_CR1_ADDR, cr1);
}

static void apm_lcdfb_wait_disable_done(struct apm_lcd_info *ctx)
{
	u32 status;
	unsigned long start;
	unsigned long end;
	long diff;
	
	/* Wait for 100ms */
	start = jiffies;
	do {
		status = INREG(ctx, LCDC_ISR_ADDR);	
		if (status & LDD_MASK) {
			OUTREG(ctx, LCDC_ISR_ADDR, LDD_MASK);
			break;
		}
		end = jiffies;
		diff = (long) end - (long) start;
	} while ((diff * 1000 / HZ) < 100);
}

static int apm_lcdfb_setup_clock(struct apm_lcd_info *ctx,
				struct fb_info *info)
{
	struct apm86xxx_pll_ctrl pll_ctr;
	int lcd_ser_freq_sel;
	int lcd_clk_freq_sel;
	u32 val;
	int rc = -1;

	/* LCD Clock Formula: 				
	   Input:       X = ?
	   Input:       Y = ?
	   Input:       REFRESH_RATE (Hz) = ?
	   LCD_PCLK_IN Required = PCLK = X * 1.31 * Y * 1.04 * REFRESH_RATE
	   
	   Input:	LCD_PCLK_IN = 25000000		
	   Input:	OD = 2	NOTE: Various this value to find proper 
	                              output clock
	   Input Fixed:	NR = 1		
			VCO = LCD_PCLK_IN * 7 * OD		
			NF = (VCO * NR) / LCD_PCLK_IN		
			LCD_PLL_OUT = VCO / OD		
	   Input:	LCD_SER_FREQ_SEL = 1		
	   Input:	LCD_CLK_FREQ_SEL = 7		
			LCD_PCLK_OUT = LCD_PLL_OUT / LCD_CLK_FREQ_SEL		
			LCD_CLK = LCD_PLL_OUT / LCD_SER_FREQ_SEL		
	*/
	
	if (info->var.xres == 320 && info->var.yres == 240) {
		pll_ctr.clkf_val = 105 - 1;
		pll_ctr.clkod_val = 15 - 1;
		pll_ctr.clkr_val = 1 - 1;
		pll_ctr.bwadj_val = 105 / 2 - 1;
		lcd_ser_freq_sel = 1;
		lcd_clk_freq_sel = 7;	
	} else if (info->var.xres == 480 && info->var.yres == 272) {
		pll_ctr.clkf_val = 56 - 1;
		pll_ctr.clkod_val = 8 - 1;
		pll_ctr.clkr_val = 1 - 1;
		pll_ctr.bwadj_val = 56 / 2 - 1;		
		lcd_ser_freq_sel = 1;
		lcd_clk_freq_sel = 7;	
	} else if ((info->var.xres == 640 && info->var.yres == 480) ||
		   (info->var.xres == 800 && info->var.yres == 600)) {
		pll_ctr.clkf_val = 28 - 1;
		pll_ctr.clkod_val = 4 - 1;
		pll_ctr.clkr_val = 1 - 1;
		pll_ctr.bwadj_val = 28 / 2 - 1;			
		lcd_ser_freq_sel = 1;
		lcd_clk_freq_sel = 7;	
	} else if ((info->var.xres == 1024 && info->var.yres == 768) ||
		   (info->var.xres == 1280 && info->var.yres == 768) ||
		   (info->var.xres == 1280 && info->var.yres == 1024) ||
		   (info->var.xres == 1920 && info->var.yres == 1080)) {		   
		pll_ctr.clkf_val = 14 - 1;
		pll_ctr.clkod_val = 2 - 1;
		pll_ctr.clkr_val = 1 - 1;
		pll_ctr.bwadj_val = 14 / 2 - 1;		
		lcd_ser_freq_sel = 1;
		lcd_clk_freq_sel = 7;	
	} else {
		/* Need to compute all values - FIXME */
		return rc;
	}

	DPRINT(KERN_INFO APMLCDPFX 
		"CLKF %d CLKOD %d CLKR %d BWAdj %d "
		"Ser Freq Sel %d CLK Freq Sel %d\n",
		pll_ctr.clkf_val, pll_ctr.clkod_val, pll_ctr.clkr_val,
		pll_ctr.bwadj_val, lcd_ser_freq_sel, lcd_clk_freq_sel);

	/* Disable LCDC clock before configuring LCD PLL */
	clk_disable(ctx->clk);

	/* Disable the PLL in order to program it */
	disable_lcd_pll();

	if (apm_lcdfb_init_firsttime) {
	/* Select internal or external LCD input clock */
		apm86xxx_read_scu_reg(SCU_SOCDIV3_ADDR, &val);
		if (ext_clk_khz == 0)
			val |= LCD_PLL_MUX_SEL3_MASK;
		else
			val &= ~LCD_PLL_MUX_SEL3_MASK;
		apm86xxx_write_scu_reg(SCU_SOCDIV3_ADDR, val);
		apm86xxx_read_scu_reg(SCU_SOCDIV3_ADDR, &val);
		DPRINT(KERN_INFO APMLCDPFX "SOCDIV3 0x%08X\n", val);
	}

	/* Program the LCD_SER and LCD_PCLK divisor */
	apm86xxx_read_scu_reg(SCU_SOCDIV1_ADDR, &val);
	val &= ~(LCD_SER_FREQ_SEL1_MASK | LCD_CLK_FREQ_SEL1_MASK);
	val |= LCD_SER_FREQ_SEL1_WR(lcd_ser_freq_sel) 
		| LCD_CLK_FREQ_SEL1_WR(lcd_clk_freq_sel); 
	apm86xxx_write_scu_reg(SCU_SOCDIV1_ADDR, val);
	apm86xxx_read_scu_reg(SCU_SOCDIV1_ADDR, &val);
	DPRINT(KERN_INFO APMLCDPFX "SOCDIV1 0x%08X\n", val);

	/* Program the PLL and wait for lock */
	enable_lcd_pll(&pll_ctr);

	/* Enable LCDC clock and take IP out of reset */
	clk_enable(ctx->clk);

	rc = apm86xxx_disable_mem_shutdown(
	     	(u32 __iomem *)(ctx->diag +
		    	LCD_CFG_MEM_RAM_SHUTDOWN_ADDR),
		     	MPIC_F1_MASK);
	if (rc != 0) {
		printk("Failed to remove LCDC CSR/palette memory "
			"out of shutdown\n");
	}

	return rc;
}

static void apm_lcdfb_hwcore_init(struct apm_lcd_info *ctx,
				struct fb_info *info)
{
	int i;
	u32 val;
	u32 cr1;

	/* Remove LCDC CSR/palette memory out of shutdown */
	if (apm_lcdfb_init_firsttime) {
		val = INREG(ctx, LCDC_CIR_ADDR);
		printk("APM LCD model %d %d-bits Bus version 1.%02d\n",
			CIR_MN_RD(val),
			CIR_BI_RD(val) == 5 ? 128 : 
				(CIR_BI_RD(val) == 4 ? 64 : 32),
			CIR_REV_RD(val));

		apm_lcdfb_init_firsttime = 0;
	}

	/* Disable the controller and remove power to panel */
	cr1 = INREG(ctx, LCDC_CR1_ADDR) & ~(LCE1_MASK | LPE1_MASK);
	OUTREG(ctx, LCDC_CR1_ADDR, cr1);

	/* Program Horizontal Timing */
	val = (u32) info->var.hsync_len << 24 |
	      (u32) info->var.left_margin << 16 |
	      ((u32) info->var.xres/16) << 8 |
	      (u32) info->var.right_margin;
	OUTREG(ctx, LCDC_HTR_ADDR, val);
	
		/* Program Vertical Timing */	
	val = (u32) info->var.upper_margin << 16 |
	      (u32) info->var.lower_margin << 8 |
	      (u32) info->var.vsync_len;
	OUTREG(ctx, LCDC_VTR1_ADDR, val);
	OUTREG(ctx, LCDC_VTR2_ADDR, info->var.yres);

	/* Set Pixel clock Polarity rising */
	cr1 &= ~PCP1_MASK; 
	/* Set LCD enable polarity */
	if (dep)	
		cr1 |= DEP1_MASK;	/* High data polarity */
	else
		cr1 &= ~DEP1_MASK;	/* Low data polarity */	

	/* Select between RGB or BGR output format */
	if (bgr)
		cr1 |= RGB1_MASK;
	else
		cr1 &= ~RGB1_MASK;

	/* Set LVDS Invert and Intel 24.1 mode */
	val = INREG(ctx, DEBUG_TEST_REG_ADDR);
	if (invert_lvds)
		val &= ~0x10;
	else
		val |= 0x10;
	if (intel24_1_mode)
		val |= 0x04;
	else			
		val &= ~0x04;
	OUTREG(ctx, DEBUG_TEST_REG_ADDR, val); 

	/* Program Pixel clock timing 
	   1. Pixel clock from PCLK_IN 
	   2. No divider bypass 
	   3. Divisor of 1 */	  			
	OUTREG(ctx, LCDC_PCTR_ADDR, 0x00000201);       
	
	/* Program Frame Buffer address and mapper accordingly */
	if (use_desc) {
		for (i = 0; i < frame_buffer_no; i++) {
			int j;
			if (i + 1 < frame_buffer_no) {
				ctx->desc[i].fdna   = 
					apm_lcdfb_mmap2hw(&ctx->desc[i+1]) >> 1;
				ctx->desc[i].fdnav  = 1;
			} else {
				ctx->desc[i].fdna   = 
					apm_lcdfb_mmap2hw(&ctx->desc[0]) >> 1;
				ctx->desc[i].fdnav  = 0;
			}
			ctx->desc[i].fdowere  = 0; 
			ctx->desc[i].fdower   = 0;
			ctx->desc[i].flc      = 0; 
			ctx->desc[i].fnc      = 0;
			ctx->desc[i].fdowe    = 0;
			ctx->desc[i].fddbare  = 1;
			ctx->desc[i].fddbar   = 
				apm_lcdfb_mmap2hw(info->screen_base +
						i * apm_lcdfb_fbsize(info));
			ctx->desc[i].fddear   = 
				apm_lcdfb_mmap2hw(info->screen_base +
						i * apm_lcdfb_fbsize(info) - 1);
			for (j = 0; j < MAX_OFB; j++) 
				ctx->desc[i].fdowdbar[j] = 0;  	
		}
		/* Program Frame descriptor basic address */
		OUTREG(ctx, LCDC_DFBAR_ADDR, apm_lcdfb_mmap2hw(ctx->desc));
		cr1 |= DFE1_MASK;
		cr1 |= DFB1_MASK;	
	} else {
		OUTREG(ctx, LCDC_DBAR_ADDR, 
			apm_lcdfb_mmap2hw(info->screen_base));
		/* End address is only required if 24-bpp and packed */		
		OUTREG(ctx, LCDC_DEAR_ADDR, 
			apm_lcdfb_mmap2hw(info->screen_base + 
					apm_lcdfb_fbsize(info) - 1));		
		cr1 &= ~DFE1_MASK;
		cr1 &= ~DFB1_MASK;
	}

	/* Clear all LCD address translate as we will use default which
	   map them directly to lower 32 address space */
	for (i = 0; i <= 16; i++) {
		OUTREG(ctx, START_ADDR0_ADDR + i * 8, 0x00000000);
		OUTREG(ctx, END_ADDR0_ADDR + i * 8, 0x00000000);
	}
	
	/* Enable all error interrupts */
	val  = INREG(ctx, LCDC_IMR_ADDR);
	/* NOTE: MBE_MASK and ARI_MASK can not be enabled when overlay is 
	         enabled. You will get these errors if enabled and overlay
	         also enabled. This is an errata of the LCD controller 
	         which will be resovled in future revision. This error
	         does not affect LCD functionality. */
	val |= ABL_MASK	
		| ARS_MASK 
		| FBE_MASK
		| OFO_MASK
		| OFU_MASK
		| IFO_MASK
		| IFU_MASK;
	OUTREG(ctx, LCDC_IMR_ADDR, val);
	
	/* Enable VSYNC interrupt */
	val = INREG(ctx, LCDC_IMR_ADDR);
	val |= VCT_MASK; 	
	OUTREG(ctx, LCDC_IMR_ADDR, val);

	/* Enable interrupt compare of VSYNC */
	OUTREG(ctx, LCDC_ISCR_ADDR, 0x7);		
	
	/* Configure cr1 register */
	OUTREG(ctx, LCDC_CR1_ADDR, cr1);
}

static int apm_lcd_hwframe_init(struct apm_lcd_info *ctx, 
				struct fb_info *info)
{
	int i;
 	u32 cr1_tmp;
	u32 cr1;
 	u32 ops;

	cr1_tmp = cr1 = INREG(ctx, LCDC_CR1_ADDR);

	cr1 &= ~FBP1_MASK;
	cr1 &= ~DEE1_MASK;
	cr1 &= ~LPS1_MASK;	/* Output to first port */
	cr1 &= ~FDW1_MASK;	/* Select 8 beat burst request word */
	cr1 &= ~PSS1_MASK;	/* Load palette from slave bus instead frame 
				   buffer memeory */
	cr1 &= ~EBO1_MASK;	/* Little endian byte ordering */
	cr1 &= ~EPO1_MASK;	/* Little endian pixel ordering within byte 
	                           if and only if EBO=0*/
	cr1 &= ~RGB1_MASK;	/* RGB format */
	ops = 0;

   	switch (info->var.bits_per_pixel) {
	case 8:
		ops |= 0x1;
		cr1 |= RGB1_MASK;	/* RGB format */
		break;
	/* For 16bpp, set out to 5:6:5 or 5:5:5 and for 18-bit/24-bit LCD,
	   output to LCD panel MSB */
	case 16:
		ops |= 0x1;
		cr1 |= FDW1_WR(0x01);
		break;
	case 24:
		cr1 |= FBP1_MASK;	/* 24 bpp packed pixel */
	default:
		ops |= 0x1;
		cr1 |= DEE1_MASK;	/* Enable DEAR */
		cr1 |= FDW1_WR(0x01);
		break;
	}

	if (ops1)
		ops |= 0x02; 	
	cr1 &= ~OPS1_MASK;
	cr1 |= OPS1_WR(ops);	
	
	/* Set LCD_VSYNC Polarity high */
	if (info->var.sync & FB_SYNC_VERT_HIGH_ACT)
		cr1 &= ~VSP1_MASK;
	else 
		cr1 |= VSP1_MASK;
	/* Set LCD_HSYNC Polarity high */
	if (info->var.sync & FB_SYNC_HOR_HIGH_ACT)
		cr1 &= ~HSP1_MASK;
	else		 
		cr1 |= HSP1_MASK;
	/* Set bpp */
	cr1 &= ~BPP1_MASK;
	cr1 |= BPP1_WR(apm_lcd_map2hwbpp(info->var.bits_per_pixel));

	if (INREG(ctx, LCDC_CR1_ADDR) != cr1)
		goto update; 
	
	DPRINT(KERN_INFO APMLCDPFX 
		"Skip update par CR1 0x%08X\n", cr1);
	return 0;

update:
	
	cr1_tmp &= ~LPE1_MASK;	/* Disable LCD power */
	cr1_tmp &= ~LCE1_MASK;	/* Disable LCD controller until timing 
				   and etc are programmed */
	OUTREG(ctx, LCDC_CR1_ADDR, cr1_tmp);
	/* Need to wait disable completed */
	if (cr1 & LCE1_MASK)
		apm_lcdfb_wait_disable_done(ctx);
	
	cr1 |= LPE1_MASK;	/* Enable LCD power */
	cr1 |= LCE1_MASK;	/* Enable LCD controller */
	OUTREG(ctx, LCDC_CR1_ADDR, cr1);
	
	/* Need to update line length in case bpp changed */
	ctx->fb->fix.line_length = bytes_per_line(&ctx->fb->var);
	/* Also update overlay bpp */
	for (i = 0; i < max_overlay; i++) {
		ctx->overlay[i].fb.var.bits_per_pixel = 
			ctx->fb->var.bits_per_pixel;
		ctx->overlay[i].fb.fix.line_length = 
			bytes_per_line(&ctx->overlay[i].fb.var); 
	}

	DPRINT(KERN_INFO APMLCDPFX 
		"Update par CR1 0x%08X\n", cr1);

	return 0;				
}

static int apm_lcdfb_set_par(struct fb_info *info)
{
	struct apm_lcd_par  *par = info->par;
	struct apm_lcd_info *ctx = par->ctx;
	u32 htr;
	u32 vtr;
	int rc;
	int backlight_update = 0;

	htr = (u32) info->var.hsync_len << 24 |
	      (u32) info->var.left_margin << 16 |
	      ((u32) info->var.xres/16) << 8 |
	      (u32) info->var.right_margin;
	
	vtr = (u32) info->var.upper_margin << 16 |
	      (u32) info->var.lower_margin << 8 |
	      (u32) info->var.vsync_len;

	if (apm_lcdfb_init_firsttime ||
	    INREG(ctx, LCDC_HTR_ADDR) != htr ||
	    INREG(ctx, LCDC_VTR1_ADDR) != vtr ||
	    INREG(ctx, LCDC_VTR2_ADDR) != info->var.yres) {
		/* Setup up clock timings for new display size */
		if ((rc = apm_lcdfb_setup_clock(ctx, info)) < 0) { 
			printk(KERN_ERR APMLCDPFX 
				"apm_lcdfb_setup_clock: error %d for "
				"requested %dx%d resoluton\n", rc,
				info->var.xres, info->var.yres);				
			return rc;
		}

		/* Initial Core */
		apm_lcdfb_hwcore_init(ctx, info);

		DPRINT(KERN_INFO APMLCDPFX "Update "
			"par HTR 0x%08X VTR1 0x%08X "
			"VTR2 0x%08X PCTR 0x%08X\n", htr, vtr, info->var.yres,
			INREG(ctx, LCDC_PCTR_ADDR));

		backlight_update = 1;

	} else {
		DPRINT(KERN_INFO APMLCDPFX "Skip update "
			"par HTR 0x%08X VTR1 0x%08X "
			"VTR2 0x%08X PCTR 0x%08X\n", htr, vtr, info->var.yres,
			INREG(ctx, LCDC_PCTR_ADDR));
		return 0;
	}

	/* Initialize the HW frame buffer */
	rc = apm_lcd_hwframe_init(ctx, info);

	if (backlight_update && ctx->bl) {
		apm_lcdfb_update_dbar(ctx, info, 1);
		apm_lcd_backlight_update(ctx->bl);
	}

	return rc;
}

static void apm_lcdfb_update_dbar(struct apm_lcd_info *ctx, 
				struct fb_info	*info, int now)
{
	char __iomem *base_ptr = info->screen_base + 
				 info->var.yoffset * info->fix.line_length;				 
	
	if (!use_desc) {
		if (now) {
			OUTREG(ctx, LCDC_DEAR_ADDR,
				apm_lcdfb_mmap2hw(base_ptr +
						apm_lcdfb_fbsize(info) - 1));
			OUTREG(ctx, LCDC_DBAR_ADDR, apm_lcdfb_mmap2hw(base_ptr));
		} else {
			/* Update Frame Buffer address accordingly
			On VSYNC interrupt, DBAR and DEAR are updated */
			ctx->next_dear = apm_lcdfb_mmap2hw(base_ptr + 
						apm_lcdfb_fbsize(info) - 1);
			ctx->next_dbar = apm_lcdfb_mmap2hw(base_ptr);
		}		
	}
}

static int apm_lcdfb_pan_display(struct fb_var_screeninfo *var,
                             struct fb_info *info)
{
	struct apm_lcd_par  *par = info->par;
	struct apm_lcd_info *ctx = par->ctx;
#if defined(APM_LCDFB_NOT_COHERENT_MEM)
	char __iomem *base_ptr;
#endif
        
        if (var->xoffset != 0 || 
            var->yoffset > (info->var.yres_virtual - info->var.yres))
        	return -EINVAL;
	info->var.yoffset = var->yoffset;
#if defined(APM_LCDFB_NOT_COHERENT_MEM)
	base_ptr = info->screen_base + 
			info->var.yoffset * info->fix.line_length;				 
	flush_dcache_range((u32) base_ptr, 
			(u32) base_ptr + apm_lcdfb_fbsize(info));
#endif	
	apm_lcdfb_update_dbar(ctx, info, 0);
	DPRINT(KERN_INFO APMLCDPFX 
		"pan display %dx%d xoffset %d,%d\n",
		var->xres, var->yres, var->xoffset, var->yoffset);
	apm_lcdfb_wait_for_vsync(info, HZ/50);
	return 0;
}      

static void apm_lcd_set_lut_cache(struct apm_lcd_par *par, u32 addr, 
				u16 value)
{
	u16 *pallete = (u16 *) &par->cache_pallete[addr];
	*pallete = value;
}

static void apm_lcd_load_lut_cache2hw(struct apm_lcd_par *par)
{
	struct fb_info	*info = par->ctx->fb;
	u16 *pallete = (u16 *) &par->cache_pallete[0];
	int index;
		
	for (index = 0; index < 256; index++, pallete++) {
		/* Set the color pallete for 1, 2, 4, and 8 bit */
		switch (info->var.red.offset) {
		case 0:
			if (info->var.blue.offset == 0) {
				/* BGR with 5:5:5 Format */
				OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
					*pallete);  
			} else {
				/* BGR with 5:6:5 Format */
				OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
					*pallete);  
			}
			break;			
		case 10:
			/* RGB with 5:5:5 Format */
			OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
				*pallete);  
			break;
		case 11:
			/* RGB with 5:6:5 Format */
			OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
				*pallete);  
			break;
		}
	}		
}

void apm_lcd_set_lut(struct apm_lcd_par *par, unsigned int index, 
			unsigned char r, unsigned char g, unsigned char b)
{
	struct fb_info	*info = par->ctx->fb;
		
	/* Set the color pallete for 1, 2, 4, and 8 bit */
	switch (info->var.red.offset) {
	case 0:
		if (info->var.blue.offset == 0) {
			/* BGR with 5:5:5 Format */
			OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
				(u16) b << 10 | (u16) g << 5 | (u16) r);  
			apm_lcd_set_lut_cache(par, index * 2, 
				(u16) b << 10 | (u16) g << 5 | (u16) r);
		} else {
			/* BGR with 5:6:5 Format */
			OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
				(u16) b << 11 | (u16) g << 5 | (u16) r);  
			apm_lcd_set_lut_cache(par, index * 2, 
				(u16) b << 11 | (u16) g << 5 | (u16) r);
		}
		break;			
	case 10:
		/* RGB with 5:5:5 Format */
		OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
			(u16) r << 10 | (u16) g << 5 | (u16) b);  
		apm_lcd_set_lut_cache(par, index * 2, 
			(u16) r << 10 | (u16) g << 5 | (u16) b);
		break;
	case 11:
		/* RGB with 5:6:5 Format */
		OUTREG16(par->ctx, LCDC_PALT_ADDR + index * 2, 
			(u16) r << 11 | (u16) g << 5 | (u16) b);  
		apm_lcd_set_lut_cache(par, index * 2, 
			(u16) r << 11 | (u16) g << 5 | (u16) b);
		break;
	}		
}

static int apm_lcdfb_setcolreg(unsigned regno, unsigned red, unsigned green,
			   unsigned blue, unsigned transp,
			   struct fb_info *info)
{
	int err = 0;
	struct apm_lcd_par *par = info->par;
	
	if (regno >= 256)  /* no. of hw registers */
		return -EINVAL;
	/* grayscale works only partially under directcolor */
	if (info->var.grayscale) {
		/* grayscale = 0.30*R + 0.59*G + 0.11*B */
		red = green = blue = (red * 77 + green * 151 + blue * 28) >> 8;
	}
	/* Directcolor:
	 *   var->{color}.offset contains start of bitfield
	 *   var->{color}.length contains length of bitfield
	 *   {hardwarespecific} contains width of DAC
	 *   pseudo_palette[X] is programmed to (X << red.offset) |
	 *                                      (X << green.offset) |
	 *                                      (X << blue.offset)
	 *   RAMDAC[X] is programmed to (red, green, blue)
	 *   color depth = SUM(var->{color}.length)
	 *
	 * Pseudocolor:
	 *    var->{color}.offset is 0 unless the palette index takes less than
	 *                        bits_per_pixel bits and is stored in the upper
	 *                        bits of the pixel value
	 *    var->{color}.length is set so that 1 << length is the number of
	 *                        available palette entries
	 *    pseudo_palette is not used
	 *    RAMDAC[X] is programmed to (red, green, blue)
	 *    color depth = var->{color}.length
 	 *
	 * Static pseudocolor:
	 *    same as Pseudocolor, but the RAMDAC is not programmed (read-only)
	 *
	 * Mono01/Mono10:
	 *    Has only 2 values, black on white or white on black (fg on bg),
	 *    var->{color}.offset is 0
	 *    white = (1 << var->{color}.length) - 1, black = 0
	 *    pseudo_palette is not used
	 *    RAMDAC does not exist
	 *    color depth is always 2
	 *
	 * Truecolor:
	 *    does not use RAMDAC (usually has 3 of them).
	 *    var->{color}.offset contains start of bitfield
	 *    var->{color}.length contains length of bitfield
	 *    pseudo_palette is programmed to (red << red.offset) |
	 *                                    (green << green.offset) |
	 *                                    (blue << blue.offset) |
	 *                                    (transp << transp.offset)
	 *    RAMDAC does not exist
 	 *    color depth = SUM(var->{color}.length})
	 *
	 *  The color depth is used by fbcon for choosing the logo and also
	 *  for color palette transformation if color depth < 4
	 *
	 *  As can be seen from the above, the field bits_per_pixel is _NOT_
	 *  a criteria for describing the color visual.
	 *
	 *  A common mistake is assuming that bits_per_pixel <= 8 is pseudocolor,
	 *  and higher than that, true/directcolor.  This is incorrect, one needs
	 *  to look at the fix->visual.
	 *
	 *  Another common mistake is using bits_per_pixel to calculate the color
	 *  depth.  The bits_per_pixel field does not directly translate to color
	 *  depth. You have to compute for the color depth (using the color
	 *  bitfields) and fix->visual as seen above.
	 */

	switch (info->var.bits_per_pixel) {
	case 1:
	case 2:
	case 4:
	case 8:
		if (regno >= 256)
			return -EINVAL;
	/*
	 * This is the point where the color is converted to something that
	 * is acceptable by the hardware.
	 */
		#define CNVT_TOHW(val,width) ((((val)<<(width))+0x7FFF-(val))>>16)
		red = CNVT_TOHW(red, info->var.red.length);
		green = CNVT_TOHW(green, info->var.green.length);
		blue = CNVT_TOHW(blue, info->var.blue.length);
		transp = CNVT_TOHW(transp, info->var.transp.length);
		#undef CNVT_TOHW
	/*
	* This is the point where the function feeds the color to the hardware
	* palette after converting the colors to something acceptable by
	* the hardware. Note, only FB_VISUAL_DIRECTCOLOR and
	* FB_VISUAL_PSEUDOCOLOR visuals need to write to the hardware palette.
	* If you have code that writes to the hardware CLUT, and it's not
	* any of the above visuals, then you are doing something wrong.
	*/
		apm_lcd_set_lut(par, regno, red, green, blue);
		break;

	/* This is the point were you need to fill up the contents of
	 * info->pseudo_palette. This structure is used _only_ by fbcon, thus
	 * it only contains 16 entries to match the number of colors supported
	 * by the console. The pseudo_palette is used only if the visual is
	 * in directcolor or truecolor mode.  With other visuals, the
	 * pseudo_palette is not used. (This might change in the future.)
	 *
	 * The contents of the pseudo_palette is in raw pixel format.  Ie, each
	 * entry can be written directly to the framebuffer without any conversion.
	 * The pseudo_palette is (void *).  However, if using the generic
	 * drawing functions (cfb_imageblit, cfb_fillrect), the pseudo_palette
	 * must be casted to (u32 *) _regardless_ of the bits per pixel. If the
	 * driver is using its own drawing functions, then it can use whatever
	 * size it wants.
	 */
	case 16:
		if (regno >= 16)
			return 0;

		if (info->var.red.offset == 10) {
			/* ARGB with 1:5:5:5 Format */
			((u32*) (info->pseudo_palette))[regno] =
				((red   & 0xf800) >>  1) |
				((green & 0xf800) >>  6) |
				((blue  & 0xf800) >> 11);
		} else {
			/* ARGB with 0:5:6:5 Format */
			((u32*) (info->pseudo_palette))[regno] =
				((red   & 0xf800)      ) |
				((green & 0xfc00) >>  5) |
				((blue  & 0xf800) >> 11);
		}
		break;

	case 24:
	case 32:
		if (regno >= 16)
			return 0;

		/* ARGB with 0:8:8:8 Format */
		((u32*) (info->pseudo_palette))[regno] =
			((red   & 0xff00)      ) |
			((green & 0xff00) <<  8) |
			((blue  & 0xff00) << 16);
		break;

	default:
		err = -1;
	}

	return err;
}

/* Backlight functions */
static inline void apm_lcd_backlight_power(struct apm_lcd_info *ctx, int on)
{
	u32 pwmfr;
	
	/* Enable/disable PWM (backlight) */
	pwmfr = INREG(ctx, LCDC_PWMFR_ADDR);
	if (on)
		pwmfr |= PWMFCE_MASK;
	else 
		pwmfr &= ~PWMFCE_MASK;
	OUTREG(ctx, LCDC_PWMFR_ADDR, pwmfr);
}

static int apm_lcd_backlight_config(struct apm_lcd_info *ctx,
				int duty_ns, int period_ns)
{
	unsigned long period_cycles, duty_cycles, prescale;
	
	if (ctx == NULL || period_ns == 0 || duty_ns > period_ns) {
		DPRINT(KERN_INFO APMLCDPFX
			"Invalid parameter for backlight configuration "
			"ctx %p period %d duty %d\n",
			ctx, period_ns, duty_ns); 
		return -EINVAL;
	}
	DPRINT(KERN_INFO APMLCDPFX
		"backlight configuration ctx %p period %d duty %d\n",
		ctx, period_ns, duty_ns); 
		
	/* Find prescale such that scaled frequency is below 30KHz as per
	   panel spec */
	if (ext_clk_khz > 0)
		period_cycles = ext_clk_khz * 1000;
	else 
		period_cycles = LCD_INT_CLK_MHZ * 1000 * 1000;	
	prescale = period_cycles / lcd_max_pwm_freq + 1;
	/* NOTE: 255 is MAX of LCDC_PWMDCR */
	duty_cycles = 255 * duty_ns / period_ns;
	/* For PWM, use PCLK_IN as input frequency */	
	OUTREG(ctx, LCDC_PWMFR_ADDR, prescale | PWMFCE_MASK | PWMFCI_MASK);	
	OUTREG(ctx, LCDC_PWMDCR_ADDR, duty_cycles);
	return 0;
}

static int apm_lcd_backlight_update(struct backlight_device *bl)
{
	struct apm_lcd_info *ctx = dev_get_drvdata(&bl->dev);
	int brightness = bl->props.brightness;
	int max = bl->props.max_brightness;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

	if (brightness == 0) {
		apm_lcd_backlight_config(ctx, 0, ctx->bl_period);
		apm_lcd_backlight_power(ctx, 0);
	} else {
		apm_lcd_backlight_config(ctx, 
					brightness * ctx->bl_period / max, 
					ctx->bl_period);
		apm_lcd_backlight_power(ctx, 1);
	}
	return 0;
}

static int apm_lcd_backlight_get_brightness(struct backlight_device *bl)
{
	return bl->props.brightness;
}

static struct backlight_ops apm_lcd_backlight_ops = {
	.update_status	= apm_lcd_backlight_update,
	.get_brightness	= apm_lcd_backlight_get_brightness,
};

static int apm_lcdfb_blank(int blank_mode, struct fb_info *info)
{
	struct apm_lcd_par *par = info->par;
	u32 val;
	
	switch (blank_mode) {
	case FB_BLANK_UNBLANK:   
		/* Turn on LCD */
		val = INREG(par->ctx, LCDC_CR1_ADDR);
		val |= LPE1_MASK;
		OUTREG(par->ctx, LCDC_CR1_ADDR, val);
		/* Turn on backlight */
		apm_lcd_backlight_power(par->ctx, 0);
		return 0;
	case FB_BLANK_POWERDOWN: /* Turn off the LCD */
		val = INREG(par->ctx, LCDC_CR1_ADDR);
		val &= ~LPE1_MASK;
		OUTREG(par->ctx, LCDC_CR1_ADDR, val);
		/* Turn off backlight */
		apm_lcd_backlight_power(par->ctx, 1);
		return 0;
	case FB_BLANK_NORMAL:
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	default:
		return !0;
	}
}

#if 0
static int apm_lcdfb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	unsigned long start = vma->vm_start;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long page, pos;
	struct apm_lcd_info *ctx = (struct apm_lcd_info *) info;

	if (offset + size > info->fix.smem_len) {
		if (!use_desc)
			return -EINVAL;
		/* Return the mmap of frame descriptor */
		pos = (unsigned long) ctx4->desc;
		size = PAGE_SIZE;			
	} else {
		/* Return the mmap of the frame buffer */
		pos = (unsigned long) info->fix.smem_start + offset;
	}

	while (size > 0) {
		page = vmalloc_to_pfn((void *) pos);
		if (remap_pfn_range(vma, start, page, PAGE_SIZE, PAGE_SHARED)) {
			return -EAGAIN;
		}
		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		if (size > PAGE_SIZE)
			size -= PAGE_SIZE;
		else
			size = 0;
	}

	vma->vm_flags |= VM_RESERVED;	/* avoid to swap out this VMA */
	return 0;
}
#endif

static struct fb_ops apm_lcdfb_ops = {
	.owner		= THIS_MODULE,
	.fb_read        = fb_sys_read,
	.fb_write       = fb_sys_write,
	.fb_check_var	= apm_lcdfb_check_var,
	.fb_set_par	= apm_lcdfb_set_par,
	.fb_pan_display = apm_lcdfb_pan_display,
	.fb_setcolreg	= apm_lcdfb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_blank	= apm_lcdfb_blank,
	.fb_cursor	= NULL,
	.fb_rotate	= NULL,
	.fb_sync	= NULL,
	.fb_ioctl	= apm_lcdfb_ioctl,
	/* .fb_mmap	= apm_lcdfb_mmap, */
};

static irqreturn_t apm_lcd_irq(int irqno, void *ctx_ptr)
{
	struct apm_lcd_info *ctx = (struct apm_lcd_info *) ctx_ptr;
	u32 status;
	
	status = INREG(ctx, LCDC_ISR_ADDR);	
	if (!status)
		return IRQ_NONE;
	
	/* Handle error condition */
	if (status & (ARS_MASK | ARI_MASK | MBE_MASK | IFO_MASK 
			| IFU_MASK | OFO_MASK | OFU_MASK | FBE_MASK)) {
		if (status & ARS_MASK)
			printk(KERN_ERR APMLCDPFX 
				"AXI master response signal error\n");		
		if (status & ARI_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"AXI master response ID error\n");		
		if (status & ABL_MASK)
			printk(KERN_ERR APMLCDPFX 
				"AXI master read burst length error\n");		
		if (status & MBE_MASK) 
			printk(KERN_ERR APMLCDPFX "master bus error\n");		
		if (status & IFO_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"input fifo overrun error\n");		
		if (status & IFU_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"input fifo underrun error\n");		
		if (status & OFO_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"output fifo overrun error\n");		
		if (status & OFU_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"output fifo underrun error\n");		
		if (status & FBE_MASK) 
			printk(KERN_ERR APMLCDPFX 
				"frame descriptor bus error\n");		
	}	
	/* Handle operation condition */
	if (status & LDD_MASK) {
		/* LCD controller disabled - no operation */
	}
	if (status & BAU_MASK) {
		/* DMA base address register update to CAR */
	}
	if (status & VCT_MASK) {
		/* Vertical compare triggered */
		if (ctx->next_dbar != 0) { 
			OUTREG(ctx, LCDC_DBAR_ADDR, ctx->next_dbar);
			OUTREG(ctx, LCDC_DEAR_ADDR, ctx->next_dear);
			ctx->next_dbar = 0;
		}
		wake_up_interruptible(&ctx->wait_vsync);
		++ctx->vblank_cnt;
	}
	if (status & FNC_MASK) {
		/* Frame descriptor node completed */
		wake_up_interruptible(&ctx->wait_vsync);
		++ctx->vblank_cnt;
	}
	if (status & FLC_MASK) {
		/* Frame descriptor entire list completed */
	}

	/* Clear IRQ */
	OUTREG(ctx, LCDC_ISR_ADDR, status);
	
	return IRQ_HANDLED;	
}

static int __devinit apm_lcdfb_init_one(struct apm_lcd_info *ctx, 
					int fb_idx)
{
	struct fb_info *fbi;
	struct apm_lcd_par *par = NULL;
	int retval;	
	
	/* Dynamically allocate info and par */
	fbi = framebuffer_alloc(sizeof(struct apm_lcd_par), ctx->dev);	
	if (!fbi) {
		printk(KERN_ERR APMLCDPFX 
			"Unable to allocate frame buffer memory\n");
		return -ENOMEM;
	}
		
	ctx->fb = fbi;
	ctx->desc_ptr = NULL;	
	par = fbi->par;
	par->ctx = ctx;

	fbi->fbops = &apm_lcdfb_ops;
    	fbi->fix = apm_lcdfb_fix; 
    	fbi->var = apm_lcdfb_vfb; 
    	fbi->pseudo_palette = par->pseudo_palette;
	fbi->flags = FBINFO_DEFAULT;

#if 0
	#define PIXMAP_SIZE	((4*1024) * 32/8)
	fbi->pixmap.addr = kmalloc(PIXMAP_SIZE, GFP_KERNEL);
	if (!fbi->pixmap.addr) {
		printk(KERN_ERR APMLCDPFX 
			"Unable to allocate memory for pixmap\n");
		retval = -ENOMEM;
		goto err_pixmap;
	}	
	fbi->pixmap.size = PIXMAP_SIZE;
	fbi->pixmap.flags = FB_PIXMAP_SYSTEM;
	fbi->pixmap.scan_align = 4;
	fbi->pixmap.buf_align = 4;
	fbi->pixmap.access_align = 32;
#endif
	/* Default video mode */
	if (!mode_option)
		mode_option = "640x480-24@60";	 	

	retval = fb_find_mode(&fbi->var, fbi, mode_option, custom_modedb, 
			ARRAY_SIZE(custom_modedb), NULL, 8);
	if (!retval || retval == 4) {
		printk(KERN_ERR APMLCDPFX 
			"unable to found video mode %s\n", mode_option);
		retval = -EINVAL;
		goto err_mode;
	}
	
	/* This has to be done! */
	if (fb_alloc_cmap(&fbi->cmap, 256, 0)) {
		printk(KERN_ERR APMLCDPFX "unable to allocate cmap\n");
		retval = -ENOMEM;
		goto err_cmap;
	}

	apm_lcdfb_check_var(&fbi->var, fbi);

	/* Allocate maximum frame buffer memory width x height x 3 
	   NOTE: 3 bytes as support up to 24 bpp */
	fbi->screen_size = fbi->var.xres * fbi->var.yres * 3 * frame_buffer_no;
	fbi->screen_base = alloc_pages_exact(fbi->screen_size, 
					GFP_KERNEL | GFP_DMA | __GFP_ZERO);	  	    	
	if (fbi->screen_base == NULL) {
		printk(KERN_ERR APMLCDPFX 
			"unable to allocate frame buffer memory\n");
		retval = -ENOMEM;
		goto err_fbmem;
	}
	fbi->fix.smem_start  = virt_to_phys(fbi->screen_base);	
	fbi->fix.smem_len    = fbi->screen_size;
	fbi->fix.line_length = bytes_per_line(&fbi->var);
	if (use_desc) {
		fbi->fix.mmio_start  = virt_to_phys(ctx->desc);
		fbi->fix.mmio_len    = sizeof(struct apm_lcd_desc) * MAX_FRAME;
	} else {
		fbi->fix.mmio_start  = virt_to_phys(ctx->csr);
		fbi->fix.mmio_len    = 0x2000;
	}
	
#if defined(APM_LCDFB_NOT_COHERENT_MEM)
	flush_dcache_range((u32) fbi->screen_base, 
			(u32) fbi->screen_base + apm_lcdfb_fbsize(fbi));
#endif

	/* Register with frame work which will trigger a call to 
	   apm_lcdfb_set_par(fbi) */	
	if (register_framebuffer(fbi) < 0) {
		printk(KERN_ERR APMLCDPFX "Unable to register framebuffer\n");
		retval = -EINVAL;
		goto err_reg;
	}
	
	DPRINT("fb%d: CSR 0x%p frame buffer VAddr 0x%p PAddr 0x%02X.%08X\n",
		fbi->node, ctx->csr, fbi->screen_base, 
		(u32) ((u64) virt_to_phys(fbi->screen_base) >> 32),
		(u32) virt_to_phys(fbi->screen_base));		 
	
    	printk(KERN_INFO 
    		"fb%d: %s frame buffer %dx%d %d bpp\n", 
    		fbi->node, fbi->fix.id, fbi->var.xres, fbi->var.yres, 
    		fbi->var.bits_per_pixel);
	return 0;
	
err_reg:
	free_pages_exact(fbi->screen_base, fbi->screen_size);

err_fbmem:
	fb_dealloc_cmap(&fbi->cmap);

err_cmap:
err_mode:
	if (fbi->pixmap.addr != NULL)
		kfree(fbi->pixmap.addr);
		
	if (ctx->desc_ptr != NULL)
		kfree(ctx->desc_ptr);
				
	if (ctx->fb != NULL)
		 framebuffer_release(ctx->fb);
	
	return retval;
}

static int apm_lcdfb_deinit_one(struct apm_lcd_info *ctx, int fb_idx)
{
	struct fb_info *info = ctx->fb;
	
	/* Clean up main frame buffer */
	unregister_framebuffer(info);
	fb_dealloc_cmap(&info->cmap);
	if (info->pixmap.addr != NULL)
		kfree(info->pixmap.addr);
	framebuffer_release(info);
	free_pages_exact(info->screen_base, info->screen_size);
	ctx->fb = NULL;
	return 0;
}

static int __devinit apm_lcdfb_probe(struct of_device* dev,
					const struct of_device_id *match)
{
	struct device_node *np = dev->node;
	struct device *device = &dev->dev;
	struct apm_lcd_info *ctx = NULL;
	int retval;	
        const u32 *prop;
	struct resource	res;
	int max_brightness = 0xFF;
	int brightness = max_brightness / 2;
	u32 clk_hz;
	
	if (is_apm863xx()) {
		/* To protect the innocent, there is not LCD in APM863xx */
		return -ENODEV;
	}	
		
        prop = of_get_property(np, "overlay", NULL);
        if (prop != NULL) {
	        int temp = prop[0];
	        if (temp >= 0 && temp <= 16) 
	        	max_overlay = temp;	
	}

        prop = of_get_property(np, "frame", NULL);
        if (prop != NULL) {
	        int temp = prop[0];
	        if (temp >= 1 && temp <= MAX_FRAME)
	        	frame_buffer_no = temp;	        	
	}

        prop = of_get_property(np, "desc", NULL);
        if (prop != NULL) {
	        int temp = prop[0];
	        use_desc = temp ? 1 : 0; 
	}
	
        prop = of_get_property(np, "maxbrightness", NULL);
        if (prop != NULL)
	        max_brightness = prop[0];
	prop = of_get_property(np, "brightness", NULL);
        if (prop != NULL)
	        brightness = prop[0];

	ctx = kzalloc(sizeof(struct apm_lcd_info), GFP_KERNEL);
	if (ctx == NULL) {
		printk(KERN_ERR APMLCDPFX "unable to allocate context\n");
		retval = -ENOMEM;
		goto err_ctx; 
	}	
	ctx->fb  = NULL;   	
	ctx->dev = device;
	ctx->irq = NO_IRQ;
	
	init_waitqueue_head(&ctx->wait_vsync);
	ctx->vblank_cnt = 0;
	ctx->next_dear = 0;
	ctx->next_dbar = 0;

	/* Get CSR addres */
	retval = of_address_to_resource(np, 0, &res);
	if (retval) {
		printk(KERN_ERR APMLCDPFX "no CSR entry\n");
		goto err_ctx;
	}
	ctx->csr = ioremap(res.start, RES_SIZE(&res));
	if (ctx->csr == NULL) {
		printk(KERN_ERR APMLCDPFX "can't map CSR space\n");
		retval = -EINVAL;
		goto err_ctx;
	}

	/* Get GBL_DIAG addres */
	retval = of_address_to_resource(np, 1, &res);
	if (retval) {
		printk(KERN_ERR APMLCDPFX "no GBL_DIAG entry\n");
		goto err_ctx;
	}
	ctx->diag = ioremap(res.start, RES_SIZE(&res));
	if (ctx->csr == NULL) {
		printk(KERN_ERR APMLCDPFX "can't map GBL_DIAG space\n");
		retval = -EINVAL;
		goto err_ctx;
	}

	/* Get LCDC clock structure */
	ctx->clk = clk_get(NULL, "lcd");
	if (IS_ERR(ctx->clk)) {
		printk(KERN_ERR APMLCDPFX "No clock\n");				
		return -ENODEV;
	}

	/* Get IRQ */
	ctx->irq = of_irq_to_resource(np, 0, NULL);
        if (ctx->irq == NO_IRQ) {
                printk(KERN_ERR APMLCDPFX "no IRQ entry\n");
		goto err_ctx;
	}
	retval = request_irq(ctx->irq, apm_lcd_irq, 0, "LCD", ctx);
	if (retval != 0) {
		printk(KERN_ERR APMLCDPFX "unable to register IRQ\n");
		goto err_ctx;
	}
	
	/* Allocate buffer for descriptor link aligned to 16 bytes */
	ctx->desc_ptr = kmalloc(128/8-1 + 
				sizeof(struct apm_lcd_desc) * MAX_FRAME, 
				GFP_KERNEL | GFP_DMA | __GFP_ZERO);	
	ctx->desc = (void *) (((unsigned long) ctx->desc_ptr+128/8-1) & ~0xF);					
	
	/* Setup main frame buffer */
	retval = apm_lcdfb_init_one(ctx, 0);
	if (retval != 0) {
		goto err_ctx;
	}

    	/* Setup overlay */
	apm_lcd_ofb_init(ctx);
	  			
	/* Enable the controoler */
	apm_lcdfb_enable(ctx, 1);
	
	/* Enable backlight */
	apm_lcd_backlight_power(ctx, 1);

	dev_set_drvdata(&dev->dev, ctx);
		
	/* Register backlight */
	ctx->bl = backlight_device_register(dev_name(device), device, ctx, 
					&apm_lcd_backlight_ops);
	if (IS_ERR(ctx->bl)) {
		dev_err(device, "failed to register backlight\n");
		retval = PTR_ERR(ctx->bl);
		goto err_ctx;
	}
	if (ext_clk_khz == 0)
		clk_hz = LCD_INT_CLK_MHZ*1000*1000;
	else
		clk_hz = ext_clk_khz * 1000;
	/* period = 1000000000 / (ext_clk_khz * 1000)
	   NOTE: For 25MHz, period is 40 ns */
	ctx->bl_period = 1000000000 / clk_hz;
	ctx->bl->props.max_brightness = max_brightness;
	ctx->bl->props.brightness = brightness;
	apm_lcd_backlight_update(ctx->bl);
		
	return 0;

err_ctx:
	if (ctx != NULL) {
		if (ctx->fb)
			apm_lcdfb_deinit_one(ctx, 0);
		if (ctx->desc_ptr)
			kfree(ctx->desc_ptr);
		if (ctx->csr != NULL) 
			iounmap(ctx->csr);
		if (ctx->irq != NO_IRQ)
			free_irq(ctx->irq, NULL);
		kfree(ctx);	
	}
	return retval;
}

static int __devexit apm_lcdfb_remove(struct of_device* dev)
{
	struct apm_lcd_info *ctx = dev_get_drvdata(&dev->dev);
	
	/* Clean up overlay first */
	apm_lcd_ofb_exit(ctx);
	
	/* Clean up backlight */
	if (ctx->bl)
		backlight_device_unregister(ctx->bl);
	
	/* Clean up main frame buffer */
	if (ctx->fb)
		apm_lcdfb_deinit_one(ctx, 0);
	if (ctx->csr != NULL) 
		iounmap(ctx->csr);
	if (ctx->irq != NO_IRQ)
		free_irq(ctx->irq, NULL);
	if (ctx->desc_ptr)
		kfree(ctx->desc_ptr);
	kfree(ctx);
	
	return 0;
}

int apm_lcdfb_wait_for_vsync(struct fb_info *info, long timeout)
{
	struct apm_lcd_par  *par = info->par;
	struct apm_lcd_info *ctx = par->ctx;
	int ret;
	u64 count;
	
	count = ctx->vblank_cnt;		
	
	ret = wait_event_interruptible_timeout(ctx->wait_vsync,
					       count != ctx->vblank_cnt,
						timeout);
	if (!ret)
		return -ETIMEDOUT;
	return 0;
}

static int apm_lcdfb_get_vblank(struct fb_vblank *vblank)
{
	memset(vblank, 0, sizeof(*vblank));
	vblank->flags = FB_VBLANK_HAVE_VSYNC;
	return 0;
}

static int apm_lcdfb_ioctl(struct fb_info *info, unsigned int cmd,
				unsigned long arg)
{
	void __user *argp = (void __user *) arg;
	struct fb_vblank vblank;
	struct fb_ops *fb;
	int ret;
	
	switch (cmd) {
	case FBIOGET_VBLANK:
		ret = apm_lcdfb_get_vblank(&vblank);
		if (ret)
			break;
		if (copy_to_user(argp, &vblank, sizeof(vblank)))
			ret = -EFAULT;
		break;
	case FBIO_WAITFORVSYNC:
		ret = apm_lcdfb_wait_for_vsync(info, HZ/10);
		break;
	default:
		if (!lock_fb_info(info))
			return -ENODEV;
		fb = info->fbops;
		if (fb->fb_ioctl)
			ret = fb->fb_ioctl(info, cmd, arg);
		else
			ret = -ENOTTY;
		unlock_fb_info(info);
		break;
	}
	return ret;
}

#ifdef CONFIG_PM
static void apm_lcdfb_poweroff(struct apm_lcd_info *ctx)
{
	u32 val;
	 
	/* Disable backlight */
	apm_lcd_backlight_power(ctx, 0);
	
	val = INREG(ctx, LCDC_CR1_ADDR);
	val &= ~LPE1_MASK;	/* Turn off LCD */
	val &= ~LCE1_MASK;	/* Disable controller */
	OUTREG(ctx, LCDC_CR1_ADDR, val);	
}
 
static void apm_lcdfb_poweron(struct apm_lcd_info *ctx)
{
	u32 val;
	 
	val = INREG(ctx, LCDC_CR1_ADDR);
	val |= LPE1_MASK;	/* Turn on LCD */
	val |= LCE1_MASK;	/* Enable controller */
	OUTREG(ctx, LCDC_CR1_ADDR, val);	
	
	/* Enable backlight */
	apm_lcd_backlight_power(ctx, 1);
}

static int apm_lcdfb_suspend(struct of_device * dev, pm_message_t state)
{
	struct apm_lcd_info *ctx = dev_get_drvdata(&dev->dev); 
	struct fb_info *info = ctx->fb;
	struct apm_lcd_par *par = info->par;
	
	if (state.event & PM_EVENT_FREEZE ||
	    state.event & PM_EVENT_SUSPEND) {
		/* To hibernate */
		apm_lcdfb_poweroff(par->ctx);
		par->ctx->lcdc_pwred_off = 1;
	} else if (state.event & PM_EVENT_RESUME) {
		/* To resume */ 
		apm_lcdfb_poweron(par->ctx);
	} else if (state.event & PM_EVENT_RECOVER) {
		/* To recover from enter suspend failure */	 
		apm_lcdfb_poweron(par->ctx);
		par->ctx->lcdc_pwred_off = 0;
	} 
	DPRINT(APMLCDPFX "suspend state 0x%08X power off %d "
		"DBAR 0x%08X DEAR 0x%08X\n", 
		state.event, par->ctx->lcdc_pwred_off,
		INREG(par->ctx, LCDC_DBAR_ADDR),
		INREG(par->ctx, LCDC_DEAR_ADDR));
	return 0;
}

static int apm_lcdfb_resume(struct of_device* dev)
{
	struct apm_lcd_info *ctx = dev_get_drvdata(&dev->dev); 
	struct fb_info *info = ctx->fb;
	struct apm_lcd_par *par = info->par;
	int retval = 0;

	if (par->ctx->lcdc_pwred_off) {
		/* LCD controller completely powered off, re-program all */		
		apm_lcdfb_hwcore_init(par->ctx, info);
		apm_lcd_load_lut_cache2hw(par);
		retval = apm_lcd_hwframe_init(par->ctx, info);
		apm_lcdfb_update_dbar(ctx, info, 1);
		apm_lcd_backlight_update(ctx->bl);
		par->ctx->lcdc_pwred_off = 0;
	}
	DPRINT(APMLCDPFX "resume power off %d DBAR 0x%08X DEAR 0x%08X\n", 
		par->ctx->lcdc_pwred_off, 
		INREG(par->ctx, LCDC_DBAR_ADDR),
		INREG(par->ctx, LCDC_DEAR_ADDR));
	
	apm_lcdfb_poweron(par->ctx);
			
	return retval;
}
#else
#define apm_lcdfb_suspend NULL
#define apm_lcdfb_resume NULL
#endif /* CONFIG_PM */

#ifndef MODULE
int __init apm_lcdfb_setup(char *options)
{
	char *this_opt;

	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ",")) != NULL) {
		if (!*this_opt)
			continue;
 		if (!strncmp(this_opt, "ops1:", 5)) {
			ops1 = simple_strtoul(this_opt+5, NULL, 0);
		} else if (!strncmp(this_opt, "overlay:", 8))  {
			max_overlay = simple_strtoul(this_opt+8, NULL, 0);
			if (max_overlay < 0 || max_overlay > 16)
				max_overlay = 3;
		} else if (!strncmp(this_opt, "frame:", 6))  {
			frame_buffer_no = simple_strtoul(this_opt+6, NULL, 0);
			if (frame_buffer_no <= 0 || 
			    frame_buffer_no > MAX_FRAME)
				frame_buffer_no = 3;
		} else if (!strncmp(this_opt, "desc:", 5))  {
			use_desc = simple_strtoul(this_opt+5, NULL, 0) ? 1 : 0;
		} else if (!strncmp(this_opt, "extclk:", 7))  {
			ext_clk_khz = simple_strtoul(this_opt+7, NULL, 0);
		} else if (!strncmp(this_opt, "invertlvds:", 11))  {
			invert_lvds = simple_strtoul(this_opt+11, NULL, 0);
		} else if (!strncmp(this_opt, "dep:", 4))  {
			dep = simple_strtoul(this_opt+4, NULL, 0);
		} else if (!strncmp(this_opt, "pwmfreq:", 8))  {
			lcd_max_pwm_freq = simple_strtoul(this_opt+8, NULL, 0);
		} else if (!strncmp(this_opt, "intel241:", 9))  {
			intel24_1_mode = simple_strtoul(this_opt+9, NULL, 0);
		} else if (!strncmp(this_opt, "bgr:", 4))  {
			bgr = simple_strtoul(this_opt+4, NULL, 0);
		} else {
			mode_option = this_opt;
		}
	}
	return 0;
}
#endif /* MODULE */

static struct of_device_id apm_lcdfb_match[] = {
	{ .compatible	= "apm,apm-lcd", },
	{ },
};

static struct of_platform_driver apm_lcdfb_driver = {
	.name		= "APM-LCD",
	.match_table	= apm_lcdfb_match,
	.probe		= apm_lcdfb_probe,
	.remove		= apm_lcdfb_remove,
	.suspend	= apm_lcdfb_suspend,
        .resume		= apm_lcdfb_resume,
};

static int __init apm_lcdfb_init(void)
{
	/*
	 *  For kernel boot options (in 'video=xxxfb:<options>' format)
	 */
#ifndef MODULE
	char *option = NULL;

	if (fb_get_options("apmlcdfb", &option))
		return -ENODEV;
	apm_lcdfb_setup(option);
#endif
	return of_register_platform_driver(&apm_lcdfb_driver);
}

static void __exit apm_lcdfb_exit(void)
{
	of_unregister_platform_driver(&apm_lcdfb_driver);
}

module_init(apm_lcdfb_init);
module_exit(apm_lcdfb_exit);

MODULE_AUTHOR("Loc Ho <lho@apm.com>");
MODULE_DESCRIPTION("APM LCD FrameBuffer Driver");
MODULE_LICENSE("GPL");
