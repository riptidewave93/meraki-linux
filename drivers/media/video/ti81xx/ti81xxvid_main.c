/*
 * ti81xxvid_main.c
 *
 * Copyright (C) 2005-2010 Texas Instruments.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * Leveraged code from the OMAP2 V4l2 dispaly driver
 *
 *
 * Copyright (C) 2010 Texas Instruments.
 *
 * History:
 * 20-Sep-2010 yihe Creatd
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/videodev2.h>
#include <linux/slab.h>
#include <linux/ti81xxscalar.h>

#include <media/videobuf-dma-contig.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

#include <plat/ti81xx-vpss.h>
#include "ti81xxvid.h"

#include <mach/tiler.h>


/* Driver Configuration macros */
#define VIDOUT_NAME		"t81xx_vidout"

enum ti81xx_vidout_channels {
	TI81XX_VIDEO1 = 0,
	TI81XX_VIDEO2,
	TI81XX_VIDEO3,
};

#define SD_WIDTH		720
#define SD_HEIGHT		576

/* Max Resolution supported by the driver */
#define VID_MAX_WIDTH		1920	/* Largest width */
#define VID_MAX_HEIGHT		1200	/* Largest height */

/* Mimimum requirement is 2x2 for DSS */
#define VID_MIN_WIDTH		2
#define VID_MIN_HEIGHT		2


#define VPSS_VPDMA_BUFFER_ALIGN 0xFFFFFFF0

/* Max buffer size to be allocated during init,
for the 1920x1200 size, the buffer can not be allocated from driver*/
#define TI81XX_VIDOUT_HD_MAX_BUF_SIZE (4 * 1024 * 1024)
#define TI81XX_VIDOUT_SD_MAX_BUF_SIZE (SD_WIDTH * SD_HEIGHT * 2)

static struct videobuf_queue_ops video_vbq_ops;
/* Variables configurable through module params*/
static u32 video1_numbuffers = 3;
static u32 video2_numbuffers = 3;
static u32 video3_numbuffers = 3;
static u32 video1_bufsize = TI81XX_VIDOUT_HD_MAX_BUF_SIZE;
static u32 video2_bufsize = TI81XX_VIDOUT_HD_MAX_BUF_SIZE;
static u32 video3_bufsize = TI81XX_VIDOUT_SD_MAX_BUF_SIZE;
static u32 video3_memtype = VPS_VPDMA_MT_NONTILEDMEM;
static int debug;

/* Module parameters */
module_param(video1_numbuffers, uint, S_IRUGO);
MODULE_PARM_DESC(video1_numbuffers,
	"Number of buffers to be allocated at init time for Video1 device.");

module_param(video2_numbuffers, uint, S_IRUGO);
MODULE_PARM_DESC(video2_numbuffers,
	"Number of buffers to be allocated at init time for Video2 device.");

module_param(video3_numbuffers, uint, S_IRUGO);
MODULE_PARM_DESC(video3_numbuffers,
	"Number of buffers to be allocated at init time for Video3 device.");

module_param(video1_bufsize, uint, S_IRUGO);
MODULE_PARM_DESC(video1_bufsize,
	"Size of the buffer to be allocated for video1 device");

module_param(video2_bufsize, uint, S_IRUGO);
MODULE_PARM_DESC(video2_bufsize,
	"Size of the buffer to be allocated for video2 device");

module_param(video3_bufsize, uint, S_IRUGO);
MODULE_PARM_DESC(video3_bufsize,
	"Size of the buffer to be allocated for video3 device");

module_param(video3_memtype, uint, S_IRUGO);
MODULE_PARM_DESC(video3_memtype,
	"memory type(0:non-tiler or 1:tiler) of video1 device");

module_param(debug, bool, S_IRUGO);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

/* list of data formats supported by TI81XX video pipelines */
static const  struct v4l2_fmtdesc ti81xx_formats[] = {
	{
		.description = "YUYV (YUV 4:2:2), packed",
		.pixelformat = V4L2_PIX_FMT_YUYV,
	},
	{
		.description = "Y/UV(420) semi-planar",
		.pixelformat = V4L2_PIX_FMT_NV12,
	},
	{
		.description = "Y/UV(422) semi-planar",
		.pixelformat = V4L2_PIX_FMT_NV16,
	}
};

#define NUM_OUTPUT_FORMATS (ARRAY_SIZE(ti81xx_formats))


static inline u8 get_numbuffers(struct ti81xx_vidout_dev *vout)
{
	if (vout->vid == TI81XX_VIDEO1)
		return video1_numbuffers;
	else if (vout->vid == TI81XX_VIDEO2)
		return video2_numbuffers;
	else
		return video3_numbuffers;
}

static inline u32 get_buffersize(struct ti81xx_vidout_dev *vout)
{
	u32 size;
	if (vout->vid == TI81XX_VIDEO1)
		size = video1_bufsize ? \
		video1_bufsize : TI81XX_VIDOUT_HD_MAX_BUF_SIZE;
	else if (vout->vid == TI81XX_VIDEO2)
		size = video2_bufsize ? \
		video2_bufsize : TI81XX_VIDOUT_HD_MAX_BUF_SIZE;
	else
		size =  video3_bufsize ? \
		video3_bufsize : TI81XX_VIDOUT_SD_MAX_BUF_SIZE;

	if (size >= TI81XX_VIDOUT_HD_MAX_BUF_SIZE)
		size = TI81XX_VIDOUT_HD_MAX_BUF_SIZE;

	return size;
}

static inline enum vps_vpdmamemorytype get_mtype(struct ti81xx_vidout_dev *vout)
{
	/*only VIDEO3 supports both non-tiler and tiler memory*/
	if (vout->vid == TI81XX_VIDEO3)
		return (enum vps_vpdmamemorytype)video3_memtype;
	else
		return VPS_VPDMA_MT_NONTILEDMEM;

}

static void ti81xx_vidout_setscparams(struct ti81xx_vidout_dev *vout,
				struct ti81xxvid_scalarparam *scprm)
{
struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;
	scalar_prms->scalar_enable = scprm->scalar_enable;
	scalar_prms->inframe_width = scprm->inframe_width;
	scalar_prms->inframe_height = scprm->inframe_height;
	scalar_prms->yframe_offset = scprm->yframe_offset;
	scalar_prms->yframe_size = scprm->yframe_size;
	scalar_prms->yoffset_flag = scprm->yoffset_flag;
	scalar_prms->chroma_offset = scprm->chroma_offset;
	scalar_prms->scoutbuf = NULL;
}

/* Return the default cropping dimension in crop by the image
 * size in pix and the video display size in fbuf.  The default
 * cropping dimension is the largest area no larger than the display size
 * that will fit on the display.  The default cropping is centered in
 * the image.  All vaules are rounded down to even numbers.
 */
static void ti81xx_vidout_set_default_crop(struct v4l2_pix_format *pix,
		  struct v4l2_framebuffer *fbuf, struct v4l2_rect *crop)
{
	crop->width = (pix->width < fbuf->fmt.width) ?
		pix->width : fbuf->fmt.width;
	crop->height = (pix->height < fbuf->fmt.height) ?
		pix->height : fbuf->fmt.height;
	crop->width &= ~1;
	crop->height &= ~1;
	crop->left = ((pix->width - crop->width) >> 1) & ~1;
	crop->top = ((pix->height - crop->height) >> 1) & ~1;
}

/* Adjust the new window to fit the dispaly frame
 * Returns zero if succesful, or -EINVAL if the new windows can not
 * be adjusted.
 */
static int ti81xx_vidout_try_window(struct v4l2_framebuffer *fbuf,
			struct v4l2_window *new_win)
{
	struct v4l2_rect temp_win;

	/* copy of the new window settings  */
	temp_win = new_win->w;

	if (temp_win.left < 0)
		temp_win.left = 0;

	if (temp_win.top < 0)
		temp_win.top = 0;
	/*adjust the window to not over the boundry*/
	temp_win.width = (temp_win.width < fbuf->fmt.width) ?
		temp_win.width : fbuf->fmt.width;
	temp_win.height = (temp_win.height < fbuf->fmt.height) ?
		temp_win.height : fbuf->fmt.height;
	/*adjust left/top instead of width/height*/
	if (temp_win.left + temp_win.width > fbuf->fmt.width)
		temp_win.left = (fbuf->fmt.width - temp_win.width) & ~1;
	if (temp_win.top + temp_win.height > fbuf->fmt.height)
		temp_win.top = (fbuf->fmt.height - temp_win.height) & ~1;
	/*round donw to even number*/
	temp_win.width &= ~1;
	temp_win.height &= ~1;

	if (temp_win.width <= 0 || temp_win.height <= 0)
		return -EINVAL;

	/* update*/
	new_win->w = temp_win;
	new_win->field = V4L2_FIELD_ANY;
	return 0;
}

/* Given a new render window in new_win, adjust the window to the
 * nearest supported configuration.
 * Returns zero if succesful, or -EINVAL if the new windows can not be adjusted
 */
static int ti81xx_vidout_set_window(struct v4l2_rect *crop,
		struct v4l2_window *win, struct v4l2_framebuffer *fbuf,
		struct v4l2_window *new_win)
{
	int err;

	err = ti81xx_vidout_try_window(fbuf, new_win);
	if (err)
		return err;

	/* update*/
	win->w = new_win->w;
	win->field = new_win->field;
	win->chromakey = new_win->chromakey;

	return 0;
}

/* Tune the new crop settings   The image render window in win will
 * also be adjusted if necessary.
 * Returns zero if succesful, or -EINVAL if the requested cropping  is
 * impossible and cannot reasonably be adjusted.
 */
static int ti81xx_vidout_set_crop(struct v4l2_pix_format *pix,
	      struct v4l2_rect *crop, struct v4l2_window *win,
	      struct v4l2_framebuffer *fbuf, const struct v4l2_rect *new_crop)
{
	struct v4l2_rect try_crop;

	/* make a local copy */
	try_crop = *new_crop;

	/* adjust the cropping to fit the input */
	if (try_crop.left < 0) {
		try_crop.width += try_crop.left;
		try_crop.left = 0;
	}
	if (try_crop.top < 0) {
		try_crop.height += try_crop.top;
		try_crop.top = 0;
	}
	try_crop.width = (try_crop.width < pix->width) ?
		try_crop.width : pix->width;
	try_crop.height = (try_crop.height < pix->height) ?
		try_crop.height : pix->height;
	if (try_crop.left + try_crop.width > pix->width)
		try_crop.width = pix->width - try_crop.left;
	if (try_crop.top + try_crop.height > pix->height)
		try_crop.height = pix->height - try_crop.top;

	try_crop.width &= ~1;
	try_crop.height &= ~1;

	if (try_crop.width <= 0 || try_crop.height <= 0)
		return -EINVAL;

	*crop = try_crop;
	return 0;
}

/* Init the crop and display window from format and framebuffer infro.
 * All all values are round down to even numbers.
 */
static void ti81xx_vidout_set_format(struct v4l2_pix_format *pix,
		struct v4l2_framebuffer *fbuf, struct v4l2_rect *crop,
		struct v4l2_window *win)
{
	/* update the crop based on the new format
	 */
	ti81xx_vidout_set_default_crop(pix, fbuf, crop);

	/* Set the dispaly window */
	win->w.width = crop->width;
	win->w.height = crop->height;
	win->w.left = ((fbuf->fmt.width - win->w.width) >> 1) & ~1;
	win->w.top = ((fbuf->fmt.height - win->w.height) >> 1) & ~1;
}

/*
 * Allocate buffers
 */
static unsigned long ti81xx_vidout_alloc_buffer(u32 buf_size, u32 *phys_addr)
{
	u32 order, size;
	unsigned long virt_addr, addr;

	size = PAGE_ALIGN(buf_size);
	order = get_order(size);
	virt_addr = __get_free_pages(GFP_KERNEL | GFP_DMA, order);
	addr = virt_addr;
	if (virt_addr) {
		while (size > 0) {
			SetPageReserved(virt_to_page(addr));
			addr += PAGE_SIZE;
			size -= PAGE_SIZE;
		}
	}
	*phys_addr = (u32) virt_to_phys((void *) virt_addr);
	return virt_addr;
}

/*
 * Free buffers
 */
static void ti81xx_vidout_free_buffer(unsigned long virtaddr, u32 buf_size)
{
	u32 order, size;
	unsigned long addr = virtaddr;

	size = PAGE_ALIGN(buf_size);
	order = get_order(size);
	while (size > 0) {
		ClearPageReserved(virt_to_page(addr));
		addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	free_pages((unsigned long) virtaddr, order);
}

/*
 * Try format
 */
static int ti81xx_vidout_try_format(struct ti81xx_vidout_dev *vout,
			struct v4l2_pix_format *pix, int *sbpp)
{
	int ifmt, bpp = 0;

	pix->height = clamp(pix->height, (u32)VID_MIN_HEIGHT,
						(u32)VID_MAX_HEIGHT);
	pix->width = clamp(pix->width, (u32)VID_MIN_WIDTH, (u32)VID_MAX_WIDTH);

	for (ifmt = 0; ifmt < NUM_OUTPUT_FORMATS; ifmt++) {
		if (pix->pixelformat == ti81xx_formats[ifmt].pixelformat)
			break;
	}

	if (ifmt == NUM_OUTPUT_FORMATS)
		ifmt = 0;

	pix->pixelformat = ti81xx_formats[ifmt].pixelformat;

	/*support INTERLACED, INTERLACED TB, SEQ_TB only for interlaced output
	   no filed support for progressive output*/
	if (vout->isprogressive)
		pix->field = V4L2_FIELD_NONE;
	else {
		if ((pix->field == V4L2_FIELD_SEQ_TB) ||
			(pix->field == V4L2_FIELD_SEQ_BT))
				pix->field = V4L2_FIELD_SEQ_TB;

		if (pix->field == V4L2_FIELD_INTERLACED_BT)
			pix->field = V4L2_FIELD_INTERLACED_TB;
		/*not sure field only*/
		if ((pix->field == V4L2_FIELD_ALTERNATE) ||
			(pix->field == V4L2_FIELD_TOP) ||
			(pix->field == V4L2_FIELD_BOTTOM))
			pix->field = V4L2_FIELD_SEQ_TB;

	}
	/*FIXME add luma/chroma buffer offset in priv in future.*/
	pix->priv = 0;

	switch (pix->pixelformat) {
	case V4L2_PIX_FMT_NV12:
		pix->colorspace = V4L2_COLORSPACE_SMPTE170M;
		bpp = 1;
		*sbpp = 12;
		break;
	case V4L2_PIX_FMT_NV16:
		pix->colorspace = V4L2_COLORSPACE_SMPTE170M;
		bpp = 1;
		*sbpp = 16;
		break;
	case V4L2_PIX_FMT_YUYV:
	default:
		pix->colorspace = V4L2_COLORSPACE_SMPTE240M;
		bpp = 2;
		*sbpp = 16;
		break;
	}
	pix->bytesperline = pix->width * bpp;
	/*pitch should be */
	if (pix->bytesperline & 0xF)
		pix->bytesperline += 16 - (pix->bytesperline & 0xF);

	return bpp;
}

/*
 * ti81xx_vidout_uservirt_to_phys: This inline function is used to convert user
 * space virtual address to physical address.
 */
static u32 ti81xx_vidout_uservirt_to_phys(u32 virtp)
{
	unsigned long physp = 0;
	struct vm_area_struct *vma;
	struct mm_struct *mm = current->mm;

	vma = find_vma(mm, virtp);
	/* For kernel direct-mapped memory, take the easy way */
	if (virtp >= PAGE_OFFSET) {
		physp = virt_to_phys((void *) virtp);
	} else if (vma && (vma->vm_flags & VM_IO) && vma->vm_pgoff) {
		/* this will catch, kernel-allocated, mmaped-to-usermode
		   addresses */
		physp = (vma->vm_pgoff << PAGE_SHIFT) + (virtp - vma->vm_start);
	} else {
		/* otherwise, use get_user_pages() for general userland pages */
		int res, nr_pages = 1;
		struct page *pages;
		down_read(&current->mm->mmap_sem);

		res = get_user_pages(current, current->mm, virtp, nr_pages, 1,
				0, &pages, NULL);
		up_read(&current->mm->mmap_sem);

		if (res == nr_pages) {
			physp =  __pa(page_address(&pages[0]) +
					(virtp & ~PAGE_MASK));
		} else {
			printk(KERN_WARNING VIDOUT_NAME
					"get_user_pages failed\n");
			return 0;
		}
	}

	return physp;
}



/*
 * Free the V4L2 buffers
 */
static void ti81xx_vidout_free_buffers(struct ti81xx_vidout_dev *vout)
{
	int i, numbuffers;

	/* free memory for the buffers */
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: free buffers\n", vout->vid);

	numbuffers = get_numbuffers(vout);
	vout->buffer_size = get_buffersize(vout);

	for (i = 0; i < numbuffers; i++) {
		ti81xx_vidout_free_buffer(vout->buf_virt_addr[i],
				vout->buffer_size);
		vout->buf_phy_addr[i] = 0;
		vout->buf_virt_addr[i] = 0;
	}

}

/*
 * Calculate the buffer offsets from which the streaming should
 * start.
 */
static int ti81xx_vidout_calculate_offset(struct ti81xx_vidout_dev *vout)
{
	struct v4l2_rect *crop = &vout->crop;
	struct v4l2_pix_format *pix = &vout->pix;
	int *cropped_offset = &vout->cropped_offset;
	int ps = 2;

	/* otherwise the pixel size is 2 byte */
	if (V4L2_PIX_FMT_YUYV == pix->pixelformat)
		ps = 2;
	 else if ((V4L2_PIX_FMT_NV12 == pix->pixelformat) ||
		(V4L2_PIX_FMT_NV16 == pix->pixelformat))
		ps = 1;


	*cropped_offset = (pix->width * ps) *
		crop->top + crop->left  * ps;

	/*make sure the crop offset is set to 16 byte boundary*/
	*cropped_offset &= VPSS_VPDMA_BUFFER_ALIGN;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: Offset - 0x%x\n",
		vout->vid, *cropped_offset);
	return 0;
}

/*
 * Convert V4L2 pixel format to DSS pixel format
 */
static int video_mode_to_vpss_mode(struct ti81xx_vidout_dev *vout, u32 *dfmt)
{
	struct v4l2_pix_format *pix = &vout->pix;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;
	int r = 0;


	switch (pix->pixelformat) {
	case V4L2_PIX_FMT_YUYV:
		*dfmt = FVID2_DF_YUV422I_YUYV;
		break;
	case V4L2_PIX_FMT_NV12:
		if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
				scalar_prms->scalar_enable) {
			*dfmt = FVID2_DF_YUV420SP_UV;
		} else {
			if (vout->vid == TI81XX_VIDEO3)
				*dfmt = FVID2_DF_YUV420SP_UV;
			else
				*dfmt = -EINVAL;
		}
		break;
	case V4L2_PIX_FMT_NV16:
		if (vout->vid == TI81XX_VIDEO3)
			*dfmt = FVID2_DF_YUV422SP_UV;
		else
			*dfmt = -EINVAL;
		break;
	default:
		r = -EINVAL;
	}
	if (!r)
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: data format %d\n",
			vout->vid, *dfmt);
	return r;
}

/*
 * Setup the overlay
 */
int ti81xxvid_setup_video(struct ti81xx_vidout_dev *vout,
		int posx, int posy, int outw, int outh)
{
	int ret = 0;
	struct vps_video_ctrl  *vctrl = vout->vctrl;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;
	struct fvid2_format ffmt;
	u32 dfmt, scbufphy;
	int height, width;
	int left, top;
	bool crop = 0;
	u8 fidmeg;
	struct vps_cropconfig  dei_sc_crop_cfg;
	struct vps_dei_disp_params deiprms;
	if (video_mode_to_vpss_mode(vout, &dfmt) == -EINVAL) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDEO%d: unsupport mode\n", vout->vid);
		ret = -EINVAL;
		goto error;
	}

	/* Setup the input plane parameters according to
	 * crop informations
	 */
	vctrl->get_format(vctrl, &ffmt);
	if ((vout->crop.height != vout->pix.height) ||
	    (vout->crop.width != vout->pix.width)) {
		height = vout->pix.height;
		width = vout->pix.width;
		crop = 1;
	} else {
		height = vout->crop.height;
		width = vout->crop.width;
	}
	ffmt.dataformat = dfmt;
	fidmeg = vout->pix.field == V4L2_FIELD_SEQ_TB ? 0 : 1;

	if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
		scalar_prms->scalar_enable) {
		vout->scoutbuf = ti81xx_vidout_alloc_buffer(vout->buffer_size,
				&scbufphy);
		 if (!vout->scoutbuf) {
			ret = -ENOMEM;
			goto error;
		}
		scalar_prms->scoutbuf = (u8 *)scbufphy ;
		if (vctrl->set_scparams(scalar_prms)) {
			ret = -EINVAL;
			goto error;
		}
	}
	if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
		scalar_prms->scalar_enable) {
		ffmt.height = outh;
		ffmt.width  = outw;
		vctrl->try_format(vctrl, outw, outh, dfmt,
			vout->pix.bytesperline, fidmeg, &ffmt);
	} else {
		/*set the height and width to match crop since scaling
		* not enabled
		*/
		/* ffmt.height = height;
		ffmt.width = width;
		vctrl->try_format(vctrl, width, height, dfmt,
			vout->pix.bytesperline, 0, &ffmt); */
		ffmt.height = vout->crop.height;
		ffmt.width = vout->crop.width;
		vctrl->try_format(vctrl, vout->crop.width,
			vout->crop.height, dfmt,
			vout->pix.bytesperline, fidmeg, &ffmt);
	}
	if (vctrl->check_format(vctrl, &ffmt)) {
		ret = -EINVAL;
		goto error;
	}
	if (crop && (vctrl->caps & VPSS_VID_CAPS_CROPING)) {
		if (vctrl->set_crop(vctrl, vout->crop.left,
		    vout->crop.top, vout->pix.bytesperline,
		    vout->crop.width, vout->crop.height)) {
			ret = -EINVAL;
			goto error;
		}
	}
	if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
					scalar_prms->scalar_enable) {
		/*Setting Dei/scalar params. Currently used for scalar config*/
		deiprms.deisccropcfg = &dei_sc_crop_cfg;
		deiprms.sccfg.bypass = !scalar_prms->scalar_enable;
		deiprms.deisccropcfg->cropstartx = vout->crop.left;
		deiprms.deisccropcfg->cropstarty = vout->crop.top;
		if (vctrl->set_deiparams(vctrl, &deiprms, outw, outh)) {
			ret = -EINVAL;
			goto error;
		}
		/*Necessary because for full screen display and position set
		* to (0,0), set_pos() is not called leading non configuration
		* of vpdmaposconfig leading to display error
		*/
		if (vctrl->caps & VPSS_VID_CAPS_POSITIONING) {
			if (vctrl->set_pos(vctrl, posx, posy)) {
				ret = -EINVAL;
				goto error;
			}
		}
	} else {
		if (vctrl->caps & VPSS_VID_CAPS_POSITIONING) {
			left = ((vout->fbuf.fmt.width -
					vout->crop.width) >> 1) & ~1;
			top = ((vout->fbuf.fmt.height -
					vout->crop.height) >> 1) & ~1;
			if ((left != posy) || (top != posy)) {
				if (vctrl->set_pos(vctrl, posx, posy)) {
					ret = -EINVAL;
					goto error;
				}
			}
		}
	}
	if (vctrl->set_format(vctrl, &ffmt)) {
		ret = -EINVAL;
		goto error;
	}

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: enable=%d width=%d\n height=%d color_mode=%d\n"
		"pos=%dx%d out_width=%d out_height=%d screen_width=%d "
		"crop=%d\n",
		vout->vid, vctrl->isstarted, ffmt.width, ffmt.height,
		ffmt.dataformat, posx, posy,
		outw, outh, ffmt.pitch[0], crop);


	return 0;

error:
	v4l2_err(&vout->vid_dev->v4l2_dev,
		"VIDOUT%d: setup failed enable=%d width=%d\n "
		"height=%d color_mode=%d\n"
		"pix_width = %d pix_height=%d pos=%dx%d "
		"out_width=%d out_height=%d screen_width=%d\n",
		vout->vid, vctrl->isstarted, ffmt.width, ffmt.height,
		ffmt.dataformat, vout->pix.width, vout->pix.height,
		posx, posy, outw, outh, ffmt.pitch[0]);
	return ret;
}

/*
 * Initialize the overlay structure
 */
int ti81xxvid_init(struct ti81xx_vidout_dev *vout)
{
	int ret = 0;
	struct v4l2_window *win;
	int posx, posy, outw, outh;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: init video\n", vout->vid);

	win = &vout->win;
	outw = win->w.width;
	outh = win->w.height;
	posx = win->w.left;
	posy = win->w.top;

	ret = ti81xxvid_setup_video(vout, posx, posy,
			outw, outh);
	if (ret)
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: apply_changes failed\n", vout->vid);
	return ret;
}

/*
 * Apply the changes set the go bit of DSS
 */
int ti81xxvid_apply_changes(struct ti81xx_vidout_dev *vout)
{
	struct vps_video_ctrl *vctrl;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: apply changes\n",
		vout->vid);

	vctrl = vout->vctrl;
	vctrl->apply_changes(vctrl);

	return 0;
}

void ti81xx_vidout_isr(void *handle)
{
	struct timeval timevalue;
	struct ti81xx_vidout_dev *vout = (struct ti81xx_vidout_dev *)handle;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct videobuf_buffer *buf;
	if (!vctrl)
		return;

	if (!vout->streaming)
		return;


	spin_lock(&vout->vbq_lock);
	if ((!vout->dma_queue.next) || list_empty(&vout->dma_queue)) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: list empty\n",
			vout->vid);
			goto error;
	}
	/*set the time stampe for the dequeue buffer*/
	do_gettimeofday(&timevalue);
	buf = list_first_entry(&vout->dma_queue,
				struct videobuf_buffer,
				queue);
	buf->ts = timevalue;
	buf->state = VIDEOBUF_DONE;
	wake_up_interruptible(&buf->done);
	/*remove this buffer from the queue*/
	list_del(&buf->queue);

	/*set the next buffer status to the active*/
	buf = list_entry(vout->dma_queue.next,
			struct videobuf_buffer,
			queue);
	buf->state = VIDEOBUF_ACTIVE;
error:
	spin_unlock(&vout->vbq_lock);

}


/* Video buffer call backs */

/*
 * Buffer setup function is called by videobuf layer when REQBUF ioctl is
 * called. This is used to setup buffers and return size and count of
 * buffers allocated. After the call to this buffer, videobuf layer will
 * setup buffer queue depending on the size and count of buffers
 */
static int ti81xx_vidout_buffer_setup(struct videobuf_queue *q,
					      unsigned int *count,
					      unsigned int *size)
{
	int startindex = 0, i;
	u32 phy_addr = 0, virt_addr = 0;
	struct ti81xx_vidout_dev *vout = q->priv_data;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;

	if (!vout)
		return -EINVAL;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: setup %d buffers\n",
		vout->vid, *count);

	if (V4L2_BUF_TYPE_VIDEO_OUTPUT != q->type)
		return -EINVAL;

	startindex = get_numbuffers(vout);
	if (V4L2_MEMORY_MMAP == vout->memory && *count < startindex)
		*count = startindex;

	/*if it is user point, do nothing*/
	if (V4L2_MEMORY_MMAP != vout->memory)
		return 0;

	/* Now allocated the V4L2 buffers */
	if ((vout->pix.pixelformat == V4L2_PIX_FMT_NV12)
		&& (vout->vid != TI81XX_VIDEO3)) {
		*size = PAGE_ALIGN(scalar_prms->yframe_size * vout->sbpp >> 3);
	} else {
		*size = PAGE_ALIGN(vout->pix.width * vout->pix.height
						* vout->sbpp >> 3);
	}
	/*check size to avoid any problems*/
	if (*size > vout->buffer_size) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: buffer size wrong %u : %u\n",
			vout->vid, *size, vout->buffer_size);
		return -EINVAL;
	}
	/*allocate more buffers if required*/
	for (i = startindex; i < *count; i++) {
		vout->buffer_size = *size;

		virt_addr = ti81xx_vidout_alloc_buffer(vout->buffer_size,
				&phy_addr);
		if (!virt_addr)
			break;
		vout->buf_virt_addr[i] = virt_addr;
		vout->buf_phy_addr[i] = phy_addr;
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: buf%d: va: 0x%x pa: 0x%x size 0x%x\n",
			vout->vid, i, virt_addr, phy_addr, *size);
	}
	*count = vout->buffer_allocated = i;

	return 0;
}

/*
 * Free the V4L2 buffers additionally allocated than default
 * number of buffers.
 */
static void ti81xx_vidout_free_allbuffers(struct ti81xx_vidout_dev *vout)
{
	int num_buffers = 0, i;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: free additional buffers\n", vout->vid);
	num_buffers = get_numbuffers(vout);

	/*free all additional buffer other than default buffer*/
	for (i = num_buffers; i < vout->buffer_allocated; i++) {
		if (vout->buf_virt_addr[i])
			ti81xx_vidout_free_buffer(vout->buf_virt_addr[i],
					vout->buffer_size);

		vout->buf_virt_addr[i] = 0;
		vout->buf_phy_addr[i] = 0;
	}
	vout->buffer_allocated = num_buffers;
}

/*
 * This function will be called when VIDIOC_QBUF ioctl is called.
 * It prepare buffers before give out for the display. This function
 * converts user space virtual address into physical address if userptr memory
 * exchange mechanism is used.
 */
static int ti81xx_vidout_buffer_prepare(struct videobuf_queue *q,
			    struct videobuf_buffer *vb,
			    enum v4l2_field field)
{
	struct ti81xx_vidout_dev *vout = q->priv_data;

	if (VIDEOBUF_NEEDS_INIT == vb->state) {
		vb->width = vout->pix.width;
		vb->height = vout->pix.height;
		vb->size = vb->width * vb->height * vout->bpp;
		vb->field = field;
	}
	vb->state = VIDEOBUF_PREPARED;
	/* if user pointer memory mechanism is used, get the physical
	 * address of the buffer
	 */
	if (V4L2_MEMORY_USERPTR == vb->memory) {
		if (0 == vb->baddr)
			return -EINVAL;
		/*physical address*/
		vout->queued_buf_addr[vb->i] = (u8 *)
			ti81xx_vidout_uservirt_to_phys(vb->baddr);
	} else
		vout->queued_buf_addr[vb->i] = (u8 *)vout->buf_phy_addr[vb->i];


	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: prepare buffer 0x%p with field %d\n",
		vout->vid, vout->queued_buf_addr[vb->i], field);
	return 0;

}

/*
 * Buffer queue funtion will be called from the videobuf layer when _QBUF
 * ioctl is called. It is used to enqueue buffer, which is ready to be
 * displayed.
 */
static void ti81xx_vidout_buffer_queue(struct videobuf_queue *q,
			  struct videobuf_buffer *vb)
{
	struct ti81xx_vidout_dev *vout = q->priv_data;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: add buffer%d\n", vout->vid, vb->i);

	/* Driver is also maintainig a queue. So enqueue buffer in the driver
	 * queue */
	 if (!vout->dma_queue.next) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: dma_queue is not init\n",
			vout->vid);
		return;
	 }
	list_add_tail(&vb->queue, &vout->dma_queue);

	vb->state = VIDEOBUF_QUEUED;
}

/*
 * Buffer release function is called from videobuf layer to release buffer
 * which are already allocated
 */
static void ti81xx_vidout_buffer_release(struct videobuf_queue *q,
			    struct videobuf_buffer *vb)
{
	struct ti81xx_vidout_dev *vout = q->priv_data;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: buffer release\n", vout->vid);

	vb->state = VIDEOBUF_NEEDS_INIT;

	if (V4L2_MEMORY_MMAP != vout->memory)
		return;
}

/*
 *  File operations
 */
static void ti81xx_vidout_vm_open(struct vm_area_struct *vma)
{
	struct ti81xx_vidout_dev *vout = vma->vm_private_data;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: vm_open [vma=%08lx-%08lx]\n",
		vout->vid, vma->vm_start, vma->vm_end);
	vout->mmap_count++;
}

static void ti81xx_vidout_vm_close(struct vm_area_struct *vma)
{
	struct ti81xx_vidout_dev *vout = vma->vm_private_data;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: vm_close [vma=%08lx-%08lx]\n",
		vout->vid, vma->vm_start, vma->vm_end);
	vout->mmap_count--;
}

static struct vm_operations_struct ti81xx_vidout_vm_ops = {
	.open	= ti81xx_vidout_vm_open,
	.close	= ti81xx_vidout_vm_close,
};

static int ti81xx_vidout_mmap(struct file *file, struct vm_area_struct *vma)
{
	int i;
	void *pos;
	unsigned long start = vma->vm_start;
	unsigned long size = (vma->vm_end - vma->vm_start);
	struct ti81xxvideo_fh *fh = file->private_data;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct videobuf_queue *q = &vout->vbq;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: pgoff=0x%lx, start=0x%lx, end=0x%lx\n",
			vout->vid, vma->vm_pgoff, vma->vm_start, vma->vm_end);

	/* look for the buffer to map */
	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		if (NULL == q->bufs[i])
			continue;
		if (V4L2_MEMORY_MMAP != q->bufs[i]->memory)
			continue;
		if (q->bufs[i]->boff == (vma->vm_pgoff << PAGE_SHIFT))
			break;
	}

	if (VIDEO_MAX_FRAME == i) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"VIDOUT%d: offset invalid [offset=0x%lx]\n",
				vout->vid, (vma->vm_pgoff << PAGE_SHIFT));
		return -EINVAL;
	}

	if (size > vout->buffer_size) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: buffer size wrong %lu : %u\n",
			vout->vid, size, vout->buffer_size);
		return -EINVAL;
	}

	q->bufs[i]->baddr = vma->vm_start;

	vma->vm_flags |= VM_RESERVED;
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	vma->vm_ops = &ti81xx_vidout_vm_ops;
	vma->vm_private_data = (void *) vout;
	pos = (void *)vout->buf_virt_addr[i];
	vma->vm_pgoff = virt_to_phys((void *)pos) >> PAGE_SHIFT;
	while (size > 0) {
		unsigned long pfn;
		pfn = virt_to_phys((void *) pos) >> PAGE_SHIFT;
		if (remap_pfn_range(vma, start, pfn, PAGE_SIZE, PAGE_SHARED)) {
			v4l2_err(&vout->vid_dev->v4l2_dev,
				"VIDOUT%d: failed to map\n",
				vout->vid);
			return -EAGAIN;
		}
		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	vma->vm_flags &= ~VM_IO;
	vout->mmap_count++;

	return 0;
}

static int ti81xx_vidout_vrelease(struct file *file)
{
	unsigned int ret = 0;
	struct videobuf_queue *q;
	struct ti81xxvideo_fh *fh = file->private_data;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;
	struct vps_video_ctrl *vctrl;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Entering %s for"
		" VIDOUT%d\n",
		 __func__, vout->vid);

	if (!vout)
		return 0;

	vctrl = vout->vctrl;
	if (!vctrl)
		return 0;

	if (vout->users > 1) {
		kfree(file->private_data);
		vout->users--;
		return 0;
	}
	q = &vout->vbq;
	mutex_lock(&vout->lock);
	if (vout->streaming)
		ret = vctrl->stop(vctrl);

	if (ret)
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"failed to stop VIDOUT%d\n",
				vout->vid);

	/* Free all buffers */
	ti81xx_vidout_free_allbuffers(vout);
	if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
			scalar_prms->scalar_enable) {
		ti81xx_vidout_free_buffer(vout->scoutbuf, vout->buffer_size);
		scalar_prms->scalar_enable = 0;
	}
	videobuf_mmap_free(q);

	/* Even if apply changes fails we should continue
	   freeing allocated memeory */
	if (vout->streaming) {
		vps_video_unregister_isr(ti81xx_vidout_isr, vout, vout->vid);
		vout->streaming = 0;

		videobuf_streamoff(q);
		videobuf_queue_cancel(q);
	}

	if (vout->mmap_count != 0)
		vout->mmap_count = 0;

	vout->users = 0;
	file->private_data = NULL;

	if (vout->buffer_allocated)
		videobuf_mmap_free(q);

	vctrl->delete(vctrl);
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Exiting %s for"
		" vidout%d\n",
		 __func__, vout->vid);
	kfree(file->private_data);
	mutex_unlock(&vout->lock);
	return ret;
}

static int ti81xx_vidout_vopen(struct file *file)
{
	struct videobuf_queue *q;
	struct ti81xx_vidout_dev *vout = NULL;
	struct vps_video_ctrl *vctrl;
	struct ti81xxvideo_fh  *fh = NULL;

	vout = video_drvdata(file);
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Entering %s for"
		" vidout%d\n",
		 __func__, vout->vid);

	if (vout == NULL)
		return -ENODEV;

	/*get the vpss control*/
	vctrl = vout->vctrl;
	if (vctrl == NULL)
		return -ENODEV;

	fh = kzalloc(sizeof(struct ti81xxvideo_fh), GFP_KERNEL);
	if (!fh) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"VIDOUT%d: no memory\n", vout->vid);
		return -ENOMEM;
	}
	fh->voutdev = vout;
	file->private_data = fh;

	/* if it is not the first open call,
	 do thing but increment counter*/
	if (vout->users) {
		vout->users++;
		return 0;
	}

	/*create fvid2 handle*/
	if (vctrl->create(vctrl, get_mtype(vout))) {
		kfree(fh);
		return -ENODEV;
	}
	vout->users = 1;
	vout->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	q = &vout->vbq;
	video_vbq_ops.buf_setup = ti81xx_vidout_buffer_setup;
	video_vbq_ops.buf_prepare = ti81xx_vidout_buffer_prepare;
	video_vbq_ops.buf_release = ti81xx_vidout_buffer_release;
	video_vbq_ops.buf_queue = ti81xx_vidout_buffer_queue;
	spin_lock_init(&vout->vbq_lock);

	videobuf_queue_dma_contig_init(q, &video_vbq_ops, q->dev,
			&vout->vbq_lock, vout->type, V4L2_FIELD_NONE,
			sizeof(struct videobuf_buffer), vout, &vout->lock);

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Exiting %s for"
		" VIDOUT%d\n",
		__func__, vout->vid);
	return 0;
}

/*
 * V4L2 ioctls
 */
static int vidioc_querycap(struct file *file, void *priv,
		struct v4l2_capability *cap)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: query capacities\n",
		vout->vid);

	strlcpy(cap->driver, VIDOUT_NAME, sizeof(cap->driver));
	strlcpy(cap->card, vout->vfd->name, sizeof(cap->card));
	cap->bus_info[0] = '\0';
	cap->capabilities = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_OUTPUT;

	return 0;
}

static int vidioc_enum_fmt_vid_out(struct file *file, void *priv,
			struct v4l2_fmtdesc *fmt)
{
	int index = fmt->index;
	enum v4l2_buf_type type = fmt->type;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: enum format ioctl\n",
		vout->vid);

	if (type != V4L2_BUF_TYPE_VIDEO_OUTPUT) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d\n",
			vout->vid, type);
		return -EINVAL;
	}

	fmt->index = index;
	fmt->type = type;
	if (index >= NUM_OUTPUT_FORMATS)
		return -EINVAL;

	fmt->flags = ti81xx_formats[index].flags;
	strlcpy(fmt->description, ti81xx_formats[index].description,
			sizeof(fmt->description));
	fmt->pixelformat = ti81xx_formats[index].pixelformat;

	return 0;
}

static int vidioc_g_fmt_vid_out(struct file *file, void *priv,
			struct v4l2_format *f)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: Get format ioctl\n",
		vout->vid);

	if (f->type != V4L2_BUF_TYPE_VIDEO_OUTPUT) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d\n",
			vout->vid, f->type);
		return -EINVAL;
	}

	f->fmt.pix = vout->pix;
	return 0;

}

static int vidioc_try_fmt_vid_out(struct file *file, void *priv,
			struct v4l2_format *f)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl  *vctrl = vout->vctrl;
	u32 width, height;
	int sbpp;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: try format ioctl\n",
		vout->vid);


	if (!vctrl)
		return -EINVAL;

	if (f->type != V4L2_BUF_TYPE_VIDEO_OUTPUT) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d\n",
			vout->vid, f->type);
		return -EINVAL;
	}

	/* get the display device attached to the overlay */
	vctrl->get_resolution(vctrl, &width, &height, &vout->isprogressive);

	vout->fbuf.fmt.height = height;
	vout->fbuf.fmt.width = width;

	ti81xx_vidout_try_format(vout, &f->fmt.pix, &sbpp);
	return 0;
}

static int vidioc_s_fmt_vid_out(struct file *file, void *priv,
			struct v4l2_format *f)
{
	int ret, bpp, sbpp;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl *vctrl = vout->vctrl;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: set format ioctl\n", vout->vid);


	if (f->type != V4L2_BUF_TYPE_VIDEO_OUTPUT) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d\n",
			vout->vid, f->type);
		return -EINVAL;
	}


	if (vout->streaming) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: stop streaming before seting format\n",
			vout->vid);
		return -EBUSY;
	}

	mutex_lock(&vout->lock);

	ret = vctrl->get_resolution(vctrl,
			      &vout->fbuf.fmt.width,
			      &vout->fbuf.fmt.height,
			      &vout->isprogressive);

	if (ret)
		goto error;
	/* change to samller size is OK */

	bpp = ti81xx_vidout_try_format(vout, &f->fmt.pix, &sbpp);
	f->fmt.pix.sizeimage = (f->fmt.pix.width * f->fmt.pix.height
					* sbpp) >> 3;

	/* try & set the new output format */
	vout->bpp = bpp;
	vout->sbpp = sbpp;
	vout->pix = f->fmt.pix;

	/* set default crop and win */
	ti81xx_vidout_set_format(&vout->pix,
				 &vout->fbuf,
				 &vout->crop,
				 &vout->win);

	/*ret = ti81xxvid_init(vout);
	if (ret) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: failed to change mode\n",
			vout->vid);
		goto s_fmt_vid_out_exit;
	}*/
	ret = 0;

error:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_try_fmt_vid_overlay(struct file *file, void *priv,
			struct v4l2_format *f)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct v4l2_window *win = &f->fmt.win;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: try fmt overlay ioctl\n",
		vout->vid);


	if (f->type != V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d",
			vout->vid, f->type);
		return -EINVAL;
	}


	ret = ti81xx_vidout_try_window(&vout->fbuf, win);

	if (!ret)
		win->global_alpha = f->fmt.win.global_alpha;
	else
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: failed to try overlay format\n",
			vout->vid);
	return ret;
}

static int vidioc_s_fmt_vid_overlay(struct file *file, void *priv,
			struct v4l2_format *f)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct v4l2_window *win = &f->fmt.win;
	struct vps_dccigrtconfig  color;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: set fmt overlay ioctl\n",
		vout->vid);

	if (f->type != V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d",
			vout->vid, f->type);
		return -EINVAL;
	}

	/*overlay fmt should be the same as crop for the non-scaling dispaly*/
	if (((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) == 0) &&
	    (win->w.width != vout->crop.width ||
	    win->w.height != vout->crop.height)) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDEO%d: not support scaling win %dx%d, crop %dx%d\n",
			vout->vid, win->w.width, win->w.height,
			vout->crop.width, vout->crop.height);

		return -EINVAL;
	}

	mutex_lock(&vout->lock);

	ret = ti81xx_vidout_set_window(&vout->crop,
				       &vout->win,
				       &vout->fbuf,
				       win);
	if (!ret) {

		/*runtime support position*/
		if (vout->streaming) {
			if (vout->vctrl->caps & VPSS_VID_CAPS_POSITIONING)
				if (vout->vctrl->set_pos(vout->vctrl,
				    vout->win.w.left, vout->win.w.top))
					return -EINVAL;
		}

		if (vout->vctrl->caps & VPSS_VID_CAPS_COLOR) {
			if (vout->vctrl->get_color)
				if (vout->vctrl->get_color(vout->vctrl, &color))
					return -EINVAL;

			color.alphavalue = f->fmt.win.global_alpha;
			color.transcolor.r =
					(f->fmt.win.chromakey >> 16) & 0xff;
			color.transcolor.g =
					(f->fmt.win.chromakey >> 8) & 0xff;
			color.transcolor.b =
					(f->fmt.win.chromakey & 0xff);

			vout->win.global_alpha = f->fmt.win.global_alpha;
			vout->win.chromakey = f->fmt.win.chromakey;

			if (vout->vctrl->set_color)
				if (vout->vctrl->set_color(vout->vctrl, &color))
					return -EINVAL;
		}
	} else
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: failed to set overlay format\n",
			vout->vid);

	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_enum_fmt_vid_overlay(struct file *file, void *priv,
			struct v4l2_fmtdesc *fmt)
{
	int index = fmt->index;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	enum v4l2_buf_type type = fmt->type;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: enum overlay format ioctl\n",
		vout->vid);

	if (fmt->type != V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d",
			vout->vid, fmt->type);
		return -EINVAL;
	}

	fmt->index = index;
	fmt->type = type;
	if (index >= NUM_OUTPUT_FORMATS)
		return -EINVAL;

	fmt->flags = ti81xx_formats[index].flags;
	strlcpy(fmt->description, ti81xx_formats[index].description,
			sizeof(fmt->description));
	fmt->pixelformat = ti81xx_formats[index].pixelformat;
	return 0;
}

static int vidioc_g_fmt_vid_overlay(struct file *file, void *priv,
			struct v4l2_format *f)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct v4l2_window *win = &f->fmt.win;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct vps_dccigrtconfig color;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: get overlay format ioctl\n",
		vout->vid);

	if (f->type != V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: wrong buf type %d",
			vout->vid, f->type);
		return -EINVAL;
	}

	win->w = vout->win.w;
	win->field = vout->win.field;
	if (vctrl->caps & VPSS_VID_CAPS_COLOR) {
		if (vctrl->get_color)
			if (vctrl->get_color(vctrl, &color))
				return -EINVAL;

		win->global_alpha = color.alphavalue;
		win->chromakey = ((color.transcolor.r & 0xFF) << 16) |
			((color.transcolor.g & 0xFF) << 8) |
			(color.transcolor.b  & 0xff);

	} else {
		win->global_alpha = vout->win.global_alpha;
		win->chromakey = vout->win.chromakey;
	}
	return 0;
}

static int vidioc_cropcap(struct file *file, void *priv,
		struct v4l2_cropcap *cropcap)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct v4l2_pix_format *pix = &vout->pix;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: crop capability ioctl\n",
		vout->vid);

	if (cropcap->type != V4L2_BUF_TYPE_VIDEO_OUTPUT)
		return -EINVAL;

	if (!(vout->vctrl->caps & VPSS_VID_CAPS_CROPING)) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: does not support crop\n",
			vout->vid);
		return -EINVAL;
	}

	/* Width and height are always even */
	cropcap->bounds.width = pix->width & ~1;
	cropcap->bounds.height = pix->height & ~1;

	ti81xx_vidout_set_default_crop(&vout->pix,
				       &vout->fbuf,
				       &cropcap->defrect);
	cropcap->pixelaspect.numerator = 1;
	cropcap->pixelaspect.denominator = 1;
	return 0;
}

static int vidioc_g_crop(struct file *file, void *priv, struct v4l2_crop *crop)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: Get crop ioctl\n",
		vout->vid);

	if (!(vout->vctrl->caps & VPSS_VID_CAPS_CROPING)) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: does not support crop\n",
			vout->vid);
		return -EINVAL;
	}

	if (crop->type != V4L2_BUF_TYPE_VIDEO_OUTPUT)
		return -EINVAL;
	crop->c = vout->crop;
	return 0;
}

static int vidioc_s_crop(struct file *file, void *priv, struct v4l2_crop *crop)
{
	int ret = -EINVAL;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: set crop ioctl\n",
		vout->vid);

	if (!(vctrl->caps & VPSS_VID_CAPS_CROPING)) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: does not support crop\n",
			vout->vid);
		return -EINVAL;
	}
	mutex_lock(&vout->lock);
	/* get the display device attached to the overlay */

	ret = vctrl->get_resolution(vctrl, &vout->fbuf.fmt.width,
				&vout->fbuf.fmt.height, &vout->isprogressive);

	if (ret)
		goto error;

	if (crop->type == V4L2_BUF_TYPE_VIDEO_OUTPUT)
		ret = ti81xx_vidout_set_crop(&vout->pix, &vout->crop,
					&vout->win, &vout->fbuf, &crop->c);
	if ((vout->streaming) && (!ret)) {
		if (vctrl->caps & VPSS_VID_CAPS_CROPING) {
			if (vctrl->set_crop) {
				ret = vctrl->set_crop(vctrl, vout->crop.left,
				    vout->crop.top, vout->pix.bytesperline,
				    vout->crop.width, vout->crop.height);
			}
		}
	}

error:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_queryctrl(struct file *file, void *priv,
		struct v4l2_queryctrl *ctrl)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: query control ioctl %d\n",
		vout->vid, ctrl->id);

	switch (ctrl->id) {
	case V4L2_CID_BG_COLOR:
		ret = v4l2_ctrl_query_fill(ctrl, 0, 0xFFFFFF, 1, 0);
		break;
	default:
		ctrl->name[0] = '\0';
		ret = -EINVAL;
	}
	return ret;
}

static int vidioc_g_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: Get control ioctl %d\n",
		vout->vid, ctrl->id);

	switch (ctrl->id) {
	case V4L2_CID_BG_COLOR:
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int vidioc_s_ctrl(struct file *file, void *priv, struct v4l2_control *a)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: Set control ioctl %d\n",
		vout->vid, a->id);

	switch (a->id) {
	case V4L2_CID_BG_COLOR:
	{
		unsigned int  color = a->value;


		mutex_lock(&vout->lock);
		vout->control[1].value = color;
		mutex_unlock(&vout->lock);
		break;
	}
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int vidioc_reqbufs(struct file *file, void *priv,
			struct v4l2_requestbuffers *req)
{
	int ret = 0;
	unsigned int i, num_buffers = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct videobuf_queue *q = &vout->vbq;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: request buffer ",
		vout->vid);

	if ((req->type != V4L2_BUF_TYPE_VIDEO_OUTPUT) || (req->count < 0))
		return -EINVAL;
	/* if memory is not mmp or userptr
	   return error */
	if ((V4L2_MEMORY_MMAP != req->memory) &&
			(V4L2_MEMORY_USERPTR != req->memory)) {
		v4l2_err(&vout->vid_dev->v4l2_dev, "VIDOUT%d: wrong memory\n",
			vout->vid);
		return -EINVAL;
	}

	mutex_lock(&vout->lock);
	/* Cannot be requested when streaming is on */
	if (vout->streaming) {
		ret = -EBUSY;
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: stop streaming before requesting buffer\n",
			vout->vid);
		goto error;
	}

	/* If buffers are already allocated, free them */
	if (q->bufs[0] && (V4L2_MEMORY_MMAP == q->bufs[0]->memory)) {
		if (vout->mmap_count) {
			ret = -EBUSY;
			v4l2_err(&vout->vid_dev->v4l2_dev,
				"VIDOUT%d: buffer not unmapped\n",
				vout->vid);
			goto error;
		}
		num_buffers = get_numbuffers(vout);
		for (i = num_buffers; i < vout->buffer_allocated; i++) {
			ti81xx_vidout_free_buffer(vout->buf_virt_addr[i],
					vout->buffer_size);
			vout->buf_virt_addr[i] = 0;
			vout->buf_phy_addr[i] = 0;
		}
		vout->buffer_allocated = num_buffers;
		videobuf_mmap_free(q);
	} else if (q->bufs[0] && (V4L2_MEMORY_USERPTR == q->bufs[0]->memory)) {
		if (vout->buffer_allocated) {
			videobuf_mmap_free(q);
			for (i = 0; i < vout->buffer_allocated; i++) {
				kfree(q->bufs[i]);
				q->bufs[i] = NULL;
			}
			vout->buffer_allocated = 0;
		}
	}

	/*store the memory type in data structure */
	vout->memory = req->memory;

	INIT_LIST_HEAD(&vout->dma_queue);

	/* call videobuf_reqbufs api */
	ret = videobuf_reqbufs(q, req);
	if (ret < 0)
		goto error;

	vout->buffer_allocated = req->count;
error:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_querybuf(struct file *file, void *priv,
			struct v4l2_buffer *b)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: query buffer\n",
		vout->vid);
	mutex_lock(&vout->lock);
	ret = videobuf_querybuf(&vout->vbq, b);
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_qbuf(struct file *file, void *priv,
			struct v4l2_buffer *buffer)
{
	int r;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct videobuf_queue *q = &vout->vbq;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;
	u32 addr;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: queue buffer %d\n",
		vout->vid, buffer->index);

	if ((V4L2_BUF_TYPE_VIDEO_OUTPUT != buffer->type) ||
			(buffer->index >= vout->buffer_allocated) ||
			(q->bufs[buffer->index]->memory != buffer->memory)) {
		return -EINVAL;
	}
	if (V4L2_MEMORY_USERPTR == buffer->memory) {
		if ((buffer->length < vout->pix.sizeimage) ||
				(0 == buffer->m.userptr)) {
			return -EINVAL;
		}
	}
	mutex_lock(&vout->lock);
	r = videobuf_qbuf(q, buffer);
	if (vout->streaming) {
		struct videobuf_buffer *buf;
		/*get the last buffer from the queue, which is
			the one just queued*/
		buf = list_entry(vout->dma_queue.prev,
				struct videobuf_buffer, queue);
		if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
						scalar_prms->scalar_enable) {
			addr = (unsigned long) vout->queued_buf_addr[buf->i];
		} else {
			if (ti81xx_vidout_calculate_offset(vout)) {
				mutex_unlock(&vout->lock);
				return -EINVAL;
			}

			addr = (unsigned long) vout->queued_buf_addr[buf->i]
				+ vout->cropped_offset;
		}
		r = vctrl->set_buffer(vctrl, addr, buf->i);
		if (!r)
			r = vctrl->queue(vctrl);

		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: queue %d  %d addr 0x%x",
			vout->vid, buffer->index, buf->i,
			addr);
	}
	mutex_unlock(&vout->lock);
	return r;

}

static int vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *b)
{
	int r;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct videobuf_queue *q = &vout->vbq;
	struct vps_video_ctrl *vctrl = vout->vctrl;

	mutex_lock(&vout->lock);
	if (!vout->streaming) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: invalid dequeue\n",
			vout->vid);
		mutex_unlock(&vout->lock);
		return -EINVAL;
	}

	if (file->f_flags & O_NONBLOCK)
		/* Call videobuf_dqbuf for  nonblocking mode */
		r = videobuf_dqbuf(q, (struct v4l2_buffer *)b, 1);
	else
		/* Call videobuf_dqbuf for  blocking mode */
		r = videobuf_dqbuf(q, (struct v4l2_buffer *)b, 0);

	if (!r) {
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: dequeue buffer %d\n",
			vout->vid, b->index);

		r = vctrl->dequeue(vctrl);
	}
	mutex_unlock(&vout->lock);
	return r;
}

static int vidioc_streamon(struct file *file, void *priv, enum v4l2_buf_type i)
{
	int ret = 0, j = 0;
	u32 addr = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct videobuf_queue *q = &vout->vbq;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct videobuf_buffer *buf;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: stream on\n",
		vout->vid);

	mutex_lock(&vout->lock);

	if (vout->streaming) {
		ret = -EBUSY;
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: stream is already on\n",
			vout->vid);
		goto error0;
	}
	if (!vout->dma_queue.next) {
		ret = -EIO;
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: please call VIDIOC_REQBUFS first\n",
			vout->vid);
		goto error0;
	}

	ret = ti81xxvid_init(vout);
	if (ret)
		goto error0;
	ret = videobuf_streamon(q);
	if (ret)
		goto error0;

	if (list_empty(&vout->dma_queue)) {
		ret = -EIO;
		goto error1;
	}

	/* queue all prime buffers to the M3 */
	list_for_each_entry(buf, &vout->dma_queue, queue) {

		/*though queue all prime buffer, but only the first
		buffer will be displayed. other still int the QUEUE state*/
		if (j == 0)
			buf->state = VIDEOBUF_ACTIVE;
		/* Initialize field_id and started member */

		if ((vout->vctrl->caps & VPSS_VID_CAPS_SCALING) &&
						scalar_prms->scalar_enable) {
			addr = (unsigned long) vout->queued_buf_addr[buf->i];
		} else {
			if (ti81xx_vidout_calculate_offset(vout)) {
				ret  = -EINVAL;
				goto error1;
			}
			addr = (unsigned long) vout->queued_buf_addr[buf->i]
				+ vout->cropped_offset;
		}
		ret = vctrl->set_buffer(vctrl, addr, j++);
		if (ret)
			goto error1;
		ret = vctrl->queue(vctrl);
		if (ret)
			goto error1;
	}
	vout->field_id = 0;

	/* set flag here. Next QBUF will start DMA */
	vout->streaming = 1;

	vout->first_int = 1;

	vps_video_register_isr(ti81xx_vidout_isr, vout, vout->vid);
	ret = vctrl->start(vctrl);
	if (ret)
		vps_video_unregister_isr(ti81xx_vidout_isr, vout, vout->vid);
error1:
	if (ret)
		ret = videobuf_streamoff(q);
error0:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_streamoff(struct file *file, void *priv, enum v4l2_buf_type i)
{
	int ret = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl *vctrl = vout->vctrl;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: stream off\n",
		vout->vid);

	mutex_lock(&vout->lock);

	if (!vout->streaming) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
			"VIDOUT%d: stream is already off\n",
			vout->vid);
		ret = -EINVAL;
		goto error;
	}
	vout->streaming = 0;
	vps_video_unregister_isr(ti81xx_vidout_isr, vout, vout->vid);

	/* Turn of the pipeline */
	ret = vctrl->stop(vctrl);
	if (ret)
		goto error;
	INIT_LIST_HEAD(&vout->dma_queue);
	ret = videobuf_streamoff(&vout->vbq);
error:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_s_fbuf(struct file *file, void *priv,
				struct v4l2_framebuffer *a)
{
	int r = 0;
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct vps_dccigrtconfig color;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: set framebuffer\n",
		vout->vid);

	if (!(vctrl->caps & VPSS_VID_CAPS_COLOR)) {
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: does not support color feature\n",
			vout->vid);
		return 0;
	}
	if (vctrl->get_color) {
		r = vctrl->get_color(vctrl, &color);
		if (r)
			return r;
	}
	/*transparecy keying*/
	if ((a->flags & V4L2_FBUF_FLAG_SRC_CHROMAKEY))
		vout->fbuf.flags |= V4L2_FBUF_FLAG_SRC_CHROMAKEY;
	else
		vout->fbuf.flags &= ~V4L2_FBUF_FLAG_SRC_CHROMAKEY;

	/*alpha blending*/
	if (a->flags & V4L2_FBUF_FLAG_LOCAL_ALPHA)
		vout->fbuf.flags |= V4L2_FBUF_FLAG_LOCAL_ALPHA;
	else
		vout->fbuf.flags &= ~V4L2_FBUF_FLAG_LOCAL_ALPHA;


	color.alphavalue = vout->win.global_alpha;
	color.alphablending = 0;
	if (vout->fbuf.flags & V4L2_FBUF_FLAG_LOCAL_ALPHA)
		color.alphablending = 1;

	color.transcolor.r = (vout->win.chromakey >> 16) & 0xFF;
	color.transcolor.g = (vout->win.chromakey >> 8) & 0xFF;
	color.transcolor.b = vout->win.chromakey & 0xFF;
	color.transparency = 0;
	if (vout->fbuf.flags & V4L2_FBUF_FLAG_SRC_CHROMAKEY)
		color.transparency = 1;

	if (vctrl->set_color)
		r = vctrl->set_color(vctrl, &color);

	return r;
}

static int vidioc_g_fbuf(struct file *file, void *priv,
		struct v4l2_framebuffer *a)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct vps_video_ctrl *vctrl = vout->vctrl;
	struct vps_dccigrtconfig color;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: get framebuffer\n",
		vout->vid);

	a->flags = 0x0;
	if (!(vctrl->caps & VPSS_VID_CAPS_COLOR)) {
		a->capability = 0;
		return 0;
	} else
		a->capability = V4L2_FBUF_CAP_LOCAL_ALPHA |
			V4L2_FBUF_CAP_SRC_CHROMAKEY;

	if (vctrl->get_color) {
		if (vctrl->get_color(vctrl, &color))
			return -EINVAL;

		if (color.alphablending)
			a->flags = V4L2_FBUF_FLAG_LOCAL_ALPHA;

		if (color.transparency)
			a->flags = V4L2_FBUF_FLAG_SRC_CHROMAKEY;
	}
	return 0;
}

static long vidioc_default(struct file *file, void *priv, int cmd, void *arg)
{
	struct ti81xxvideo_fh *fh = priv;
	struct ti81xx_vidout_dev *vout = fh->voutdev;
	struct ti81xxvid_scalarparam *scalar_prms = &vout->scalar_prms;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"VIDOUT%d: vidioc Default\n", vout->vid);
	switch (cmd) {
	case TISET_SCALAR_PARAMS:
		if (scalar_prms->scalar_enable) {
			return -EINVAL;
		} else {
			ti81xx_vidout_setscparams(vout,
				(struct ti81xxvid_scalarparam *)arg);
			return 0;
		}
		break;
	default:
	return -EINVAL;
	}
}

static const struct v4l2_ioctl_ops vout_ioctl_ops = {
	.vidioc_querycap                        = vidioc_querycap,
	.vidioc_enum_fmt_vid_out                = vidioc_enum_fmt_vid_out,
	.vidioc_g_fmt_vid_out                   = vidioc_g_fmt_vid_out,
	.vidioc_try_fmt_vid_out                 = vidioc_try_fmt_vid_out,
	.vidioc_s_fmt_vid_out                   = vidioc_s_fmt_vid_out,
	.vidioc_queryctrl                       = vidioc_queryctrl,
	.vidioc_g_ctrl                          = vidioc_g_ctrl,
	.vidioc_s_fbuf	                        = vidioc_s_fbuf,
	.vidioc_g_fbuf	                        = vidioc_g_fbuf,
	.vidioc_s_ctrl                          = vidioc_s_ctrl,
	.vidioc_try_fmt_vid_overlay             = vidioc_try_fmt_vid_overlay,
	.vidioc_s_fmt_vid_overlay               = vidioc_s_fmt_vid_overlay,
	.vidioc_enum_fmt_vid_overlay            = vidioc_enum_fmt_vid_overlay,
	.vidioc_g_fmt_vid_overlay               = vidioc_g_fmt_vid_overlay,
	.vidioc_cropcap	                        = vidioc_cropcap,
	.vidioc_g_crop                          = vidioc_g_crop,
	.vidioc_s_crop	                        = vidioc_s_crop,
	.vidioc_reqbufs	                        = vidioc_reqbufs,
	.vidioc_querybuf                        = vidioc_querybuf,
	.vidioc_qbuf                            = vidioc_qbuf,
	.vidioc_dqbuf                           = vidioc_dqbuf,
	.vidioc_streamon                        = vidioc_streamon,
	.vidioc_streamoff                       = vidioc_streamoff,
	.vidioc_default                         = vidioc_default,
};

static const struct v4l2_file_operations ti81xx_vidout_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl	= video_ioctl2,
	.mmap           = ti81xx_vidout_mmap,
	.open           = ti81xx_vidout_vopen,
	.release        = ti81xx_vidout_vrelease,
};

/* Init functions used during driver initialization */
/* Initial setup of video_data */
static int __init ti81xx_vidout_setup_video_data(struct ti81xx_vidout_dev *vout)
{
	struct video_device *vfd;
	struct v4l2_pix_format *pix;
	struct v4l2_control *control;
	struct vps_video_ctrl *vctrl =
		vout->vctrl;

	/* set the default pix */
	pix = &vout->pix;
	vctrl->get_resolution(vctrl,
			      &vout->fbuf.fmt.width,
			      &vout->fbuf.fmt.height,
			      &vout->isprogressive);

	pix->width = vout->fbuf.fmt.width;
	pix->height = vout->fbuf.fmt.height;
	pix->pixelformat = V4L2_PIX_FMT_YUYV;
	if (vout->vid == TI81XX_VIDEO3)
		pix->colorspace = V4L2_COLORSPACE_SMPTE170M;
	else
		pix->colorspace = V4L2_COLORSPACE_SMPTE240M;
	vout->bpp = 2;
	vout->sbpp = 16;

	/*pitch should be 16 byte boundary*/
	pix->bytesperline = pix->width * vout->bpp;
	if (pix->bytesperline & 0xF)
		pix->bytesperline += 16 - (pix->bytesperline & 0xF);

	/* Default pixel format is YUV422 */
	pix->field = V4L2_FIELD_ANY;

	pix->sizeimage = (pix->bytesperline * pix->height * vout->sbpp) >> 3;
	pix->priv = 0;

	/* Set the data structures for the overlay parameters*/
	vout->win.global_alpha = 255;
	vout->fbuf.flags = 0;
	if (!(vctrl->caps & VPSS_VID_CAPS_COLOR))
		vout->fbuf.capability = 0;
	else
		vout->fbuf.capability =
			V4L2_FBUF_CAP_SRC_CHROMAKEY | V4L2_FBUF_CAP_LOCAL_ALPHA;
	vout->win.chromakey = 0;

	ti81xx_vidout_set_format(pix, &vout->fbuf, &vout->crop, &vout->win);

	/*Initialize the control variables for
	  rotation, flipping and background color. */
	control = vout->control;
	control[1].id = V4L2_CID_BG_COLOR;
	control[1].value = 0;

	/* initialize the video_device struct */
	vfd = vout->vfd = video_device_alloc();

	if (!vfd) {
		printk(KERN_ERR VIDOUT_NAME ": could not allocate"
				" video device struct\n");
		return -ENOMEM;
	}
	vfd->release = video_device_release;
	vfd->ioctl_ops = &vout_ioctl_ops;

	strlcpy(vfd->name, VIDOUT_NAME, sizeof(vfd->name));

	/* need to register for a VID_HARDWARE_* ID in videodev.h */
	vfd->fops = &ti81xx_vidout_fops;
	vfd->v4l2_dev = &vout->vid_dev->v4l2_dev;
	mutex_init(&vout->lock);

	vfd->minor = -1;
	return 0;

}

/* Setup video buffers */
static int __init ti81xx_vidout_setup_video_bufs(struct platform_device *pdev,
		int vid_num)
{
	u32 numbuffers;
	int ret = 0, i;
	struct video_device *vfd;
	struct ti81xx_vidout_dev *vout;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct ti81xxvideo_device *vid_dev =
		container_of(v4l2_dev, struct ti81xxvideo_device, v4l2_dev);

	vout = vid_dev->vouts[vid_num];
	vfd = vout->vfd;

	numbuffers = get_numbuffers(vout);
	vout->buffer_size = get_buffersize(vout);

	for (i = 0; i < numbuffers; i++) {
		vout->buf_virt_addr[i] =
			ti81xx_vidout_alloc_buffer(vout->buffer_size,
					(u32 *) &vout->buf_phy_addr[i]);
		if (!vout->buf_virt_addr[i]) {
			numbuffers = i;
			ret = -ENOMEM;
			goto free;
		}
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			"VIDOUT%d: buf%d: va: 0x%lx pa: 0x%lx size 0x%x\n",
			vout->vid,
			i,
			vout->buf_virt_addr[i],
			vout->buf_phy_addr[i],
			vout->buffer_size);

	}
	vout->cropped_offset = 0;
	return 0;

free:
	for (i = 0; i < numbuffers; i++) {
		ti81xx_vidout_free_buffer(vout->buf_virt_addr[i],
					  vout->buffer_size);
		vout->buf_virt_addr[i] = 0;
		vout->buf_phy_addr[i] = 0;
	}
	return ret;

}

/* Create video out devices */
static int __init ti81xx_vout_create_video_devices(struct platform_device *pdev)
{
	int ret = 0, i;
	struct ti81xx_vidout_dev *vout;
	struct video_device *vfd = NULL;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct ti81xxvideo_device *vid_dev = container_of(v4l2_dev,
			struct ti81xxvideo_device, v4l2_dev);

	for (i = 0; i < pdev->num_resources; i++) {

		vout = kzalloc(sizeof(struct ti81xx_vidout_dev), GFP_KERNEL);
		if (!vout) {
			dev_err(&pdev->dev, ": could not allocate memory\n");
			return -ENOMEM;
		}

		vout->vid = i;
		vid_dev->vouts[i] = vout;
		vout->vid_dev = vid_dev;
		vout->vctrl = vid_dev->vctrl[i];

		/* Setup the default configuration for the video devices
		 */
		if (ti81xx_vidout_setup_video_data(vout) != 0) {
			ret = -ENOMEM;
			goto error0;
		}

		/* Allocate default number of buffers for the video streaming
		 */
		if (ti81xx_vidout_setup_video_bufs(pdev, i) != 0) {
			ret = -ENOMEM;
			goto error1;
		}

		/* Register the Video device with V4L2
		 */
		vfd = vout->vfd;
		if (video_register_device(vfd, VFL_TYPE_GRABBER, i + 1) < 0) {
			dev_err(&pdev->dev, ": Could not register "
					"Video for Linux device\n");
			vfd->minor = -1;
			ret = -ENODEV;
			goto error2;
		}
		video_set_drvdata(vfd, vout);

		/* Configure the overlay structure */
		ret = ti81xxvid_init(vid_dev->vouts[i]);
		if (!ret)
			goto success;

error2:
		ti81xx_vidout_free_buffers(vout);
error1:
		video_device_release(vfd);
error0:
		kfree(vout);
		return ret;

success:
		dev_dbg(&pdev->dev, "registered and initialized"
				" vidout%d\n", vfd->minor);
		if (i == (pdev->num_resources - 1)) {
			vid_dev->num_vout = pdev->num_resources;
			return 0;
		}
	}

	return -ENODEV;
}
/* Driver functions */
static void ti81xx_vidout_cleanup_device(struct ti81xx_vidout_dev *vout)
{
	struct video_device *vfd;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"cleanup vidout%d\n", vout->vid);
	if (!vout)
		return;

	vfd = vout->vfd;
	if (vfd) {
		if (!video_is_registered(vfd))
			/*
			 * The device was never registered, so release the
			 * video_device struct directly.
			 */
			video_device_release(vfd);
		else
			/*
			 * The unregister function will release the video_device
			 * struct as well as unregistering it.
			 */
			video_unregister_device(vfd);
	}

	ti81xx_vidout_free_buffers(vout);
	kfree(vout);
}

static int ti81xx_vidout_remove(struct platform_device *pdev)
{
	int i;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct ti81xxvideo_device *vid_dev = container_of(v4l2_dev, struct
			ti81xxvideo_device, v4l2_dev);

	dev_dbg(&pdev->dev, "remove all video out devices\n");
	v4l2_device_unregister(v4l2_dev);
	for (i = 0; i < pdev->num_resources; i++)
		ti81xx_vidout_cleanup_device(vid_dev->vouts[i]);

	kfree(vid_dev);
	return 0;
}

static int ti81xx_vidout_probe(struct platform_device *pdev)
{
	int ret = 0, i;
	struct ti81xxvideo_device *vid_dev = NULL;

	if (pdev->num_resources == 0 ||
		(pdev->num_resources > VPS_DISPLAY_INST_MAX)) {
		dev_err(&pdev->dev, "probed for an unknown device\n");
		return -ENODEV;
	}

	vid_dev = kzalloc(sizeof(struct ti81xxvideo_device), GFP_KERNEL);
	if (vid_dev == NULL)
		return -ENOMEM;


	vid_dev->num_videos = vps_video_get_num_video();
	if (vid_dev->num_videos == 0) {
		dev_err(&pdev->dev, "no vpss video nodes\n");
		ret = -ENODEV;
		goto error0;
	}
	for (i = 0; i < vid_dev->num_videos; i++)
		vid_dev->vctrl[i] = vps_video_get_ctrl(i);


	if (v4l2_device_register(&pdev->dev, &vid_dev->v4l2_dev) < 0) {
		dev_err(&pdev->dev, "v4l2_device_register failed\n");
		ret = -ENODEV;
		goto error0;
	}

	ret = ti81xx_vout_create_video_devices(pdev);
	if (ret)
		goto error1;

	return 0;

error1:
	v4l2_device_unregister(&vid_dev->v4l2_dev);
error0:
	kfree(vid_dev);
	return ret;
}

static struct platform_driver ti81xx_vidout_driver = {
	.driver = {
		.name = VIDOUT_NAME,
	},
	.probe = ti81xx_vidout_probe,
	.remove = ti81xx_vidout_remove,
};

static int __init ti81xx_vidout_init(void)
{
	if ((platform_driver_probe(&ti81xx_vidout_driver,
				ti81xx_vidout_probe)) != 0) {
		printk(KERN_ERR VIDOUT_NAME
			":Could not register Video driver\n");
		return -EINVAL;
	}
	return 0;
}

static void ti81xx_vidout_cleanup(void)
{
	platform_driver_unregister(&ti81xx_vidout_driver);
}

late_initcall(ti81xx_vidout_init);
module_exit(ti81xx_vidout_cleanup);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("TI81XX Video for Linux Video Out driver");
MODULE_LICENSE("GPL v2");

