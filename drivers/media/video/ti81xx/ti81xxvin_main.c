/*
 * linux/drivers/media/video/ti81xx/ti81xxvin_main.c
 *
 * Framebuffer driver for TI 816X
 *
 * Copyright (C) 2010 Texas Instruments
 * Author: Hardik Shah(hardik.shah@ti.com)
 *
 * Davinci TI81xx HDVPSS capture driver is used as a reference driver.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/videodev2.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <mach/capt.h>
#include <mach/board-ti816x.h>
#include <mach/board-ti814x.h>
#include <linux/ti81xxvin.h>
#include "ti81xxvin_pvt.h"


MODULE_DESCRIPTION("TI 81xx Capture driver");
MODULE_LICENSE("GPL");

#define ti81xxvin_err(fmt, arg...)	\
		v4l2_err(&ti81xxvin_obj.v4l2_dev, fmt, ## arg)
#define ti81xxvin_dbg(level, debug, fmt, arg...)	\
		v4l2_dbg(level, debug, &ti81xxvin_obj.v4l2_dev, fmt, ## arg)
static int debug = 1;
static u32 inst0_numbuffers = 4;
static u32 inst1_numbuffers = 4;
static u32 inst2_numbuffers = 4;
static u32 inst3_numbuffers = 4;

module_param(debug, int, 0644);
module_param(inst0_numbuffers, uint, S_IRUGO);
module_param(inst1_numbuffers, uint, S_IRUGO);
module_param(inst2_numbuffers, uint, S_IRUGO);
module_param(inst3_numbuffers, uint, S_IRUGO);

MODULE_PARM_DESC(debug, "Debug level 0-1");
MODULE_PARM_DESC(inst0_numbuffers, "Instance 0 buf_obj count (default:4)");
MODULE_PARM_DESC(inst1_numbuffers, "Instance 1 buf_obj count (default:4)");
MODULE_PARM_DESC(inst2_numbuffers, "Instance 2 buf_obj count (default:4)");
MODULE_PARM_DESC(inst3_numbuffers, "Instance 3 buf_obj count (default:4)");

static struct ti81xxvin_config_params config_params = {
	.min_numbuffers = 4,
	.numbuffers[0] = 4,
	.numbuffers[1] = 4,
	.numbuffers[2] = 4,
	.numbuffers[3] = 4,
};

/**
 * ch_params: video standard configuration parameters for TI81xx capture
 */
/* global variables */
static struct ti81xxvin_device ti81xxvin_obj = { {NULL} };
static struct device *ti81xxvin_dev;
static void ti81xxvin_check_overflow(struct ti81xxvin_instance_obj *inst,
	struct ti81xxvin_overflow_status *stat, int clear);


static void ti81xxvin_overflow_handler(unsigned long data)
{
	struct ti81xxvin_instance_obj *inst =
		(struct ti81xxvin_instance_obj *)data;
	struct ti81xxvin_buffer_obj *buf_obj = &(inst->buf_obj);
	struct ti81xxvin_overflow_status status;
	struct timeval timevalue;
	struct videobuf_buffer *buf;

	if (0 == buf_obj->started)
		goto restart_timer;
	ti81xxvin_check_overflow(inst, &status, 0);
	if (status.porta_overflow || status.portb_overflow) {
		spin_lock(&buf_obj->buf_irqlock);
		if (list_empty(&inst->dma_queue))
			goto restart_timer;
		if (0 == buf_obj->started)
			goto restart_timer;
		do_gettimeofday(&timevalue);
		buf = list_first_entry(&inst->dma_queue, struct videobuf_buffer,
				queue);
		buf->ts = timevalue;
		buf->state = -VIDEOBUF_ERROR;
		wake_up_interruptible(&buf->done);
		/*remove this buffer from the queue*/
		list_del(&buf->queue);
		spin_unlock(&buf_obj->buf_irqlock);
	}
restart_timer:
	mod_timer(&inst->overflow_timer, jiffies + msecs_to_jiffies(100));
}

static enum fvid2_dataformat ti81xxvin_v4l2_df_to_vps_df(u32 v4l2_df)
{
	switch (v4l2_df) {
	case V4L2_PIX_FMT_YUYV:
		return FVID2_DF_YUV422I_YUYV;
	case V4L2_PIX_FMT_NV12:
		return FVID2_DF_YUV420SP_UV;
	case V4L2_PIX_FMT_RGB24:
		return FVID2_DF_RGB24_888;
	case V4L2_PIX_FMT_NV16:
		return FVID2_DF_YUV422SP_UV;
	default:
		BUG();
	}
}

static int ti81xxvin_vps_config_scalar(struct v4l2_rect *crop,
	struct v4l2_window *win, struct vps_capt_ctrl *cctrl)
{
	struct Vps_CaptOutInfo *out_stream_info;
	struct Vps_CaptScParams *sc_params;
	struct vps_cropconfig *in_cropcfg;


	out_stream_info = &cctrl->ccparams->outStreamInfo[0];
	sc_params = &cctrl->ccparams->scParams;
	in_cropcfg = &cctrl->ccparams->scParams.inCropCfg;
	if ((cctrl->decoder_width == crop->width) &&
			(crop->width == win->w.width) &&
			(cctrl->decoder_height == crop->height) &&
			(crop->height == win->w.height)) {
		out_stream_info->scEnable = 0;
		return 0;
	}
	sc_params->inWidth = cctrl->decoder_width;
	/* Height should be field height in case source is interlaced
	 * Height should be frame height in case source is progressive */
	if (cctrl->ccparams->inScanFormat == FVID2_SF_PROGRESSIVE)
		sc_params->inHeight = cctrl->decoder_height;
	else
		sc_params->inHeight = cctrl->decoder_height >> 1;
	sc_params->outWidth = win->w.width;
	sc_params->outHeight = win->w.height;
	sc_params->inScanFormat = FVID2_SF_PROGRESSIVE;
	sc_params->scConfig = NULL;
	sc_params->scCoeffConfig = NULL;
	/* We dont suport lazy loading as we dont support
	   run-time scaling change
	 */
	sc_params->enableCoeffLoad = 0;
	in_cropcfg->cropstartx = crop->left;
	in_cropcfg->cropstarty = crop->top;
	in_cropcfg->cropwidth = crop->width;
	in_cropcfg->cropheight = crop->height;
	out_stream_info->scEnable = 1;
	return 0;
}

static int ti81xxvin_vps_config_format(struct ti81xxvin_instance_obj *inst)
{
	struct vps_capt_ctrl *cctrl;
	struct Vps_CaptCreateParams *cparams;
	struct Vps_CaptOutInfo *out_stream_info;
	struct v4l2_pix_format *v4l2_fmt;
	enum fvid2_dataformat df;
	struct fvid2_format fvid2_fmt;
	struct ti81xxvin_subdev_info *sdev_info;

	cctrl = inst->captctrl;
	v4l2_fmt = &inst->buf_obj.fmt.fmt.pix;

	cparams = cctrl->ccparams;
	out_stream_info = &cctrl->ccparams->outStreamInfo[0];
	df = ti81xxvin_v4l2_df_to_vps_df(inst->buf_obj.fmt.fmt.pix.pixelformat);

	/* TODO Set the capture mode according to the decoder selected */
	cparams->videoCaptureMode =
		VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
	/* TODO Set the video interface mode according to the
	 * decoder selected */
	cparams->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_16BIT;
	/* Select the inDataformat according the input selected in decoder */
	cparams->inDataFormat = FVID2_DF_YUV422P;
	cparams->periodicCallbackEnable = 0;
	cparams->numCh = 1;
	cparams->numStream = 1;
	/* Don't care for single channel capture */
	cparams->muxModeStartChId = 0;

	sdev_info = inst->curr_subdev_info;

	cparams->videoCaptureMode = sdev_info->video_capture_mode;
	cparams->videoIfMode = sdev_info->video_if_mode;
	cparams->inDataFormat = sdev_info->input_data_format;
	memcpy(cctrl->vipcfg, &sdev_info->vip_cfg,
			sizeof(struct Vps_VipConfig));
	memcpy(cctrl->vipportcfg, &sdev_info->vip_port_cfg,
			sizeof(struct Vps_VipPortConfig));
	cparams->vipParserInstConfig =
		(struct Vps_VipConfig *)cctrl->vipcfg_phy;
	cparams->vipParserPortConfig =
		(struct Vps_VipPortConfig *)cctrl->vipportcfg_phy;
	cparams->cscConfig = NULL;

	if (inst->video.buf_params.interlaced)
		cparams->inScanFormat = FVID2_SF_INTERLACED;
	else
		cparams->inScanFormat = FVID2_SF_PROGRESSIVE;
	cparams->channelNumMap[0][0] =
		Vps_captMakeChannelNum(inst->instance_id, 0, 0);

	out_stream_info->dataFormat = df;
	out_stream_info->memType = VPS_VPDMA_MT_NONTILEDMEM;
	switch (df) {
	case FVID2_DF_YUV422I_YUYV:
		out_stream_info->pitch[FVID2_YUV_INT_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		break;
	case FVID2_DF_YUV422SP_UV:
		/* For YUV semiplanar data pitch is equal to the width
		 * of the  image
		 */
		out_stream_info->pitch[FVID2_YUV_SP_Y_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		out_stream_info->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		break;
	case  FVID2_DF_YUV420SP_UV:
		/* For YUV semiplanar data pitch is equal to the
		 * width of the image
		 */
		out_stream_info->pitch[FVID2_YUV_SP_Y_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		out_stream_info->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		break;
	case  FVID2_DF_RGB24_888:
		/* For YUV semiplanar data pitch is equal to the
		 * width of the  image
		 */
		out_stream_info->pitch[FVID2_RGB_ADDR_IDX] =
			inst->buf_obj.fmt.fmt.pix.bytesperline;
		break;
	default:
		BUG();
	}
	out_stream_info->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;
	out_stream_info->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_UNLIMITED;

	out_stream_info->scEnable = 0;

	/* Don't care for capture driver as we dont support
	   sub-frame based scaling
	 */
	out_stream_info->subFrameModeEnable = 0;
	out_stream_info->numLinesInSubFrame = 0;
	out_stream_info->subFrameCb = NULL;


	/* Don't care for caputre drivers */
	fvid2_fmt.channelnum = 0;
	fvid2_fmt.width = inst->buf_obj.fmt.fmt.pix.width;
	fvid2_fmt.height = inst->buf_obj.fmt.fmt.pix.height;
	fvid2_fmt.pitch[0] = inst->buf_obj.fmt.fmt.pix.bytesperline;
	fvid2_fmt.pitch[1] = inst->buf_obj.fmt.fmt.pix.bytesperline;
	fvid2_fmt.pitch[2] = inst->buf_obj.fmt.fmt.pix.bytesperline;
	if (inst->buf_obj.fmt.fmt.pix.field == V4L2_FIELD_ANY ||
		inst->buf_obj.fmt.fmt.pix.field == V4L2_FIELD_NONE ||
		inst->buf_obj.fmt.fmt.pix.field == V4L2_FIELD_INTERLACED) {
		fvid2_fmt.fieldmerged[0] =  1;
		fvid2_fmt.fieldmerged[0] =  1;
		fvid2_fmt.fieldmerged[0] =  1;
	} else {
		fvid2_fmt.fieldmerged[0] =  0;
		fvid2_fmt.fieldmerged[0] =  0;
		fvid2_fmt.fieldmerged[0] =  0;
	}
	fvid2_fmt.dataformat = df;
	fvid2_fmt.scanformat = cparams->inScanFormat;
	fvid2_fmt.bpp = 16;
	fvid2_fmt.reserved = NULL;
	cctrl->decoder_width = inst->video.buf_params.width;
	cctrl->decoder_height = inst->video.buf_params.height;
	ti81xxvin_vps_config_scalar(&inst->crop, &inst->win, cctrl);
	cctrl->set_format(cctrl, &fvid2_fmt);
	return 0;
}
/**
 * ti81xxvin_uservirt_to_phys : translate user/virtual address to phy address
 * @virtp: user/virtual address
 *
 * This inline function is used to convert user space virtual address to
 * physical address.
 */
static inline u32 ti81xxvin_uservirt_to_phys(u32 virtp)
{
	unsigned long physp = 0;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	vma = find_vma(mm, virtp);

	/* For kernel direct-mapped memory, take the easy way */
	if (virtp >= PAGE_OFFSET)
		physp = virt_to_phys((void *)virtp);
	else if (vma && (vma->vm_flags & VM_IO) && (vma->vm_pgoff))
		/**
		 * this will catch, kernel-allocated, mmaped-to-usermode
		 * addresses
		 */
		physp = (vma->vm_pgoff << PAGE_SHIFT) + (virtp - vma->vm_start);
	else {
		/* otherwise, use get_user_pages() for general userland pages */
		int res, nr_pages = 1;
		struct page *pages;

		down_read(&current->mm->mmap_sem);

		res = get_user_pages(current, current->mm,
				virtp, nr_pages, 1, 0, &pages, NULL);
		up_read(&current->mm->mmap_sem);

		if (res == nr_pages)
			physp = __pa(page_address(&pages[0]) +
					(virtp & ~PAGE_MASK));
		else {
			ti81xxvin_err("get_user_pages failed\n");
			return 0;
		}
	}
	return physp;
}

/**
 * ti81xxvin_config_format: configure default frame format in the device
 * based on standard selected
 * inst : ptr to instance object
 */
static void ti81xxvin_config_format(struct ti81xxvin_instance_obj *inst)
{
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;

	ti81xxvin_dbg(2, debug, "ti81xxvin_config_format\n");

	if (V4L2_DV_INVALID != inst->video.cur_dv_preset.preset) {
		switch (inst->video.cur_dv_preset.preset) {
		case V4L2_DV_1080I30:
		case V4L2_DV_1080I25:
		case V4L2_DV_1080I50:
		case V4L2_DV_1080I60:
			buf_obj->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
			break;
		default:
			buf_obj->fmt.fmt.pix.field = V4L2_FIELD_NONE;
		}
	} else if (V4L2_STD_UNKNOWN != inst->video.cur_std_id)
		buf_obj->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	else
		buf_obj->fmt.fmt.pix.field = V4L2_FIELD_NONE;
	buf_obj->memory = V4L2_MEMORY_MMAP;

	buf_obj->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	buf_obj->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf_obj->fmt.fmt.pix.sizeimage = buf_obj->fmt.fmt.pix.height *
		buf_obj->fmt.fmt.pix.width * 2 ;
	buf_obj->fmt.fmt.pix.bytesperline = buf_obj->fmt.fmt.pix.width * 2;
	buf_obj->fmt.fmt.pix.colorspace = V4L2_COLORSPACE_REC709;

	inst->crop.left = 0;
	inst->crop.top = 0;
	inst->crop.width = buf_obj->fmt.fmt.pix.width;
	inst->crop.height = buf_obj->fmt.fmt.pix.height;

	inst->win.w.left = 0;
	inst->win.w.top = 0;
	inst->win.w.width = inst->crop.width;
	inst->win.w.height = inst->crop.height;
}

/**
 * ti81xxvin_update_std_info() - update standard related info
 * @inst: ptr to instance object
 *
 * For a given standard selected by application, update values
 * in the device data structures
 */
static int ti81xxvin_update_std_info(struct ti81xxvin_instance_obj *inst,
	struct v4l2_mbus_framefmt *mbus_framefmt)
{
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct ti81xxvin_vid_obj *vid_ch = &inst->video;
	struct ti81xxvin_buffer_params *buf_params_cfg;

	ti81xxvin_dbg(2, debug, "ti81xxvin_update_std_info\n");

	buf_params_cfg = &vid_ch->buf_params;

	vid_ch->buf_params.width = mbus_framefmt->width;
	vid_ch->buf_params.height = mbus_framefmt->height;
	if (V4L2_FIELD_NONE == mbus_framefmt->field)
		vid_ch->buf_params.interlaced = 0;
	else
		vid_ch->buf_params.interlaced = 1;
	vid_ch->buf_params.bytesperline =
		mbus_framefmt->width * mbus_framefmt->height * 2;
	vid_ch->buf_params.dv_preset = vid_ch->cur_dv_preset.preset;

	buf_obj->fmt.fmt.pix.width = buf_params_cfg->width;
	buf_obj->fmt.fmt.pix.height = buf_params_cfg->height;
	buf_obj->fmt.fmt.pix.bytesperline = buf_params_cfg->bytesperline;

	inst->captctrl->isstdset = 1;
	return 0;
}

/**
 * hdvpss_map_sub_device_to_input() - Maps sub device to input
 * @inst - ptr to instance
 * @config - ptr to capture configuration
 * @input_index - Given input index from application
 * @sub_device_index - index into sd table
 *
 * lookup the sub device information for a given input index.
 * we report all the inputs to application. inputs table also
 * has sub device name for the each input
 */
static struct ti81xxvin_subdev_info *hdvpss_map_sub_device_to_input(
				struct ti81xxvin_instance_obj *inst,
				struct ti81xxvin_config *hdvpss_cfg,
				int input_index,
				int *sub_device_index)
{
	struct ti81xxvin_inst_config *inst_cfg;
	struct ti81xxvin_subdev_info *subdev_info = NULL;
	const char *subdev_name = NULL;
	int i;

	ti81xxvin_dbg(2, debug, "hdvpss_map_sub_device_to_input\n");

	inst_cfg = &hdvpss_cfg->inst_config[inst->instance_id];

	/**
	 * search through the inputs to find the sub device supporting
	 * the input
	 */
	for (i = 0; i < inst_cfg->input_count; i++) {
		/* For each sub device, loop through input */
		if (i == input_index) {
			subdev_name = inst_cfg->inputs[i].subdev_name;
			break;
		}
	}

	/* if reached maximum. return null */
	if (i == inst_cfg->input_count || (NULL == subdev_name))
		return subdev_info;

	/* loop through the sub device list to get the sub device info */
	for (i = 0; i < hdvpss_cfg->subdev_count; i++) {
		subdev_info = &hdvpss_cfg->subdev_info[i];
		if (!strcmp(subdev_info->name, subdev_name))
			break;
	}

	if (i == hdvpss_cfg->subdev_count)
		return subdev_info;

	/* check if the sub device is registered */
	if (NULL == ti81xxvin_obj.sd[i])
		return NULL;

	*sub_device_index = i;
	return subdev_info;
}

static int ti81xxvin_get_valid_field(struct ti81xxvin_instance_obj *inst,
		int update, enum v4l2_field *field)
{
	struct ti81xxvin_buffer_params *buf_params = &inst->video.buf_params;

	if (buf_params->interlaced) {
		if (*field != V4L2_FIELD_INTERLACED) {
			if (update)
				*field = V4L2_FIELD_INTERLACED;
			else
				return -EINVAL;
		}
	} else if (*field != V4L2_FIELD_NONE) {
		if (update)
			*field = V4L2_FIELD_NONE;
		else
			return -EINVAL;
	}
	return 0;
}


/**
 * ti81xxvin_check_format()  - check given pixel format for compatibility
 * @inst - instance  ptr
 * @pixfmt - Given pixel format
 * @update - update the values as per hardware requirement
 *
 * Check the application pixel format for S_FMT and update the input
 * values as per hardware limits for TRY_FMT. The default pixel and
 * field format is selected based on interface type.
 */
static int ti81xxvin_check_format(struct ti81xxvin_instance_obj *inst,
		struct v4l2_pix_format *pixfmt,
		int update)
{
	struct ti81xxvin_buffer_obj *buf_obj = &(inst->buf_obj);
	enum v4l2_field field = pixfmt->field;
	u32 sizeimage, hpitch, width, height, min_height, min_width, numlines;
	u32 min_hpitch, min_numlines;
	int ret = -EINVAL;


	ti81xxvin_dbg(2, debug, "ti81xxvin_check_format\n");
	/**
	 * first check for the pixel format. V4L2_PIX_FMT_YUYV is supported
	 */

	if (pixfmt->pixelformat != V4L2_PIX_FMT_YUYV &&
			pixfmt->pixelformat != V4L2_PIX_FMT_NV12 &&
			pixfmt->pixelformat != V4L2_PIX_FMT_NV16 &&
			pixfmt->pixelformat != V4L2_PIX_FMT_RGB24) {
		if (!update) {
			ti81xxvin_dbg(2, debug, "invalid pixel format\n");
			goto exit;
		}
		pixfmt->pixelformat = V4L2_PIX_FMT_YUYV;
	}

	if (V4L2_COLORSPACE_REC709 != pixfmt->colorspace) {
		if (!update) {
			ti81xxvin_dbg(2, debug, "Invalid colorspace\n");
			goto exit;
		}
		pixfmt->colorspace = V4L2_COLORSPACE_REC709;

	}
	ret = ti81xxvin_get_valid_field(inst, update, &field);
	if (update) {
		pixfmt->field = field;
	} else if (ret) {
		ti81xxvin_dbg(2, debug, "Invalid field format\n");
		goto exit;
	}
	min_width = inst->win.w.left + inst->win.w.width;
	width = pixfmt->width;
	if (width < min_width)  {
		if (!update) {
			ti81xxvin_dbg(2, debug, "Invalid width\n");
			goto exit;
		}
		width = min_width;
	}

	min_height = inst->win.w.top + inst->win.w.height;
	height = pixfmt->height;
	if (height < min_height)  {
		if (!update) {
			ti81xxvin_dbg(2, debug, "Invalid width\n");
			goto exit;
		}
		height = min_height;
	}

	/* validate the hpitch */
	hpitch = pixfmt->bytesperline;

	switch (pixfmt->pixelformat) {
	case V4L2_PIX_FMT_YUYV:
		min_hpitch = (inst->win.w.width + inst->win.w.left) * 2;
		min_numlines = height;
		break;
	case V4L2_PIX_FMT_NV12:
		min_hpitch = inst->win.w.width + inst->win.w.left;
		min_numlines = (height * 3) / 2;
		break;
	case V4L2_PIX_FMT_NV16:
		min_hpitch = inst->win.w.width + inst->win.w.left;
		min_numlines = height * 2;
		break;
	case V4L2_PIX_FMT_RGB24:
		min_hpitch = (inst->win.w.width + inst->win.w.left) * 3;
		min_numlines = height;
	default:
		min_hpitch = (inst->win.w.width + inst->win.w.left) * 2;
		min_numlines = height;
	}

	if (hpitch < min_hpitch) {
		if (!update) {
			ti81xxvin_dbg(2, debug, "invalid hpitch\n");
			goto exit;
		}
		hpitch = min_hpitch;
	}
	sizeimage = pixfmt->sizeimage;

	numlines = sizeimage / (hpitch);

	/* validate the numlines */
	if (numlines < min_numlines) {
		if (!update) {
			ti81xxvin_dbg(2, debug, "Invalid numlines\n");
			goto exit;
		}
		numlines = min_numlines;
	}

	/* Check for 8 byte alignment */
	if (!ALIGN(hpitch, TI81XXVIN_BUFFER_ALIGN)) {
		if (!update) {
			ti81xxvin_dbg(2, debug, "invalid pitch alignment\n");
			goto exit;
		}
		/* adjust to next 8 byte boundary */
		hpitch = (((hpitch + (TI81XXVIN_BUFFER_ALIGN - 1)) /
			TI81XXVIN_BUFFER_ALIGN) * TI81XXVIN_BUFFER_ALIGN);
	}
	if (update) {
		/* if update is set, modify the bytesperline and sizeimage */
		buf_obj->fmt.fmt.pix.width = width;
		buf_obj->fmt.fmt.pix.height = height;
		buf_obj->fmt.fmt.pix.bytesperline = hpitch;
		buf_obj->fmt.fmt.pix.sizeimage = hpitch * numlines;
		buf_obj->fmt.fmt.pix.pixelformat = pixfmt->pixelformat;

		pixfmt->bytesperline = buf_obj->fmt.fmt.pix.bytesperline;
		pixfmt->sizeimage = buf_obj->fmt.fmt.pix.sizeimage;
		/**
		 * Image width and height is always based on current
		 * window width and height
		 */
		pixfmt->width = buf_obj->fmt.fmt.pix.width;
		pixfmt->height = buf_obj->fmt.fmt.pix.height;
	}
	if (update) {
		ti81xxvin_vps_config_format(inst);
		ret = inst->captctrl->check_format(inst->captctrl,
				&inst->captctrl->fmt);


	}
exit:
	return ret;
}

void  ti81xxvin_instance_isr(void *dev_id)
{
	struct ti81xxvin_device *dev = &ti81xxvin_obj;
	struct ti81xxvin_buffer_obj *buf_obj;
	struct ti81xxvin_instance_obj *inst;
	enum v4l2_field field;
	int instance_id = 0;
	struct videobuf_buffer *buf;
	struct timeval timevalue;

	instance_id = (int)(dev_id);
	inst = dev->dev[instance_id];
	field = inst->buf_obj.fmt.fmt.pix.field;

	buf_obj = &inst->buf_obj;
	spin_lock(&buf_obj->buf_irqlock);
	if ((!inst->dma_queue.next) || list_empty(&inst->dma_queue)) {
		v4l2_err(ti81xxvin_dev->driver, "list empty\n");
		goto isr_error;
	}
	if (0 == buf_obj->started) {
		v4l2_err(ti81xxvin_dev->driver, "Instance not started\n");
		goto isr_error;
	}
	do_gettimeofday(&timevalue);
	buf = list_first_entry(&inst->dma_queue, struct videobuf_buffer,
			queue);
	buf->ts = timevalue;
	buf->state = VIDEOBUF_DONE;
	wake_up_interruptible(&buf->done);
	/*remove this buffer from the queue*/
	list_del(&buf->queue);
	/*set the next buffer status to the active*/
	buf = list_entry(inst->dma_queue.next,
			struct videobuf_buffer,
			queue);
	buf->state = VIDEOBUF_ACTIVE;
isr_error:
	spin_unlock(&buf_obj->buf_irqlock);
}
/**
 * ti81xxvin_buffer_setup : Callback function for buffer setup.
 * @q: buffer queue ptr
 * @count: number of buffers
 * @size: size of the buffer
 *
 * This callback function is called when reqbuf() is called to adjust
 * the buffer count and buffer size
 */
static int ti81xxvin_buffer_setup(struct videobuf_queue *q, unsigned int *count,
		unsigned int *size)
{
	/* Get the file handle object and instance object */
	struct ti81xxvin_fh *fh = q->priv_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;

	buf_obj = &inst->buf_obj;

	ti81xxvin_dbg(2, debug, "ti81xxvin_buffer_setup\n");

	/* If memory type is not mmap, return */
	if (V4L2_MEMORY_MMAP != buf_obj->memory)
		return 0;

	/* Calculate the size of the buffer
	 * We will not force the minimum size on user */

	if (*count < config_params.min_numbuffers)
		*count = config_params.min_numbuffers;

	*size = buf_obj->fmt.fmt.pix.sizeimage;

	return 0;
}

/**
 * buffer_prepare :  callback function for buffer prepare
 * @q : buffer queue ptr
 * @vb: ptr to video buffer
 * @field: field info
 *
 * This is the callback function for buffer prepare when videobuf_qbuf()
 * function is called. The buffer is prepared and user space virtual address
 * or user address is converted into  physical address
 */
static int ti81xxvin_buffer_prepare(struct videobuf_queue *q,
			       struct videobuf_buffer *vb,
			       enum v4l2_field field)
{
	/* Get the file handle object and instance object */
	struct ti81xxvin_fh *fh = q->priv_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;


	ti81xxvin_dbg(2, debug, "ti81xxvin_buffer_prepare\n");

	buf_obj = &inst->buf_obj;

	/* If buffer is not initialized, initialize it */
	if (VIDEOBUF_NEEDS_INIT == vb->state) {
		vb->width = buf_obj->fmt.fmt.pix.width;
		vb->height = buf_obj->fmt.fmt.pix.height;
		vb->size = vb->height * buf_obj->fmt.fmt.pix.bytesperline;
		vb->field = field;
	}
	vb->state = VIDEOBUF_PREPARED;
	/**
	 * if user pointer memory mechanism is used, get the physical
	 * address of the buffer
	 */
	if (V4L2_MEMORY_USERPTR == buf_obj->memory) {
		if (0 == vb->baddr) {
			ti81xxvin_dbg(1, debug, "buffer address is 0\n");
			return -EINVAL;

		}
		vb->boff = ti81xxvin_uservirt_to_phys(vb->baddr);
		if (!IS_ALIGNED(vb->boff, 8))
			goto exit;
	}
	return 0;
exit:
	ti81xxvin_dbg(1, debug, "buffer_prepare:offset is not"
	" aligned to 16 bytes\n");
	return -EINVAL;
}

/**
 * ti81xxvin_buffer_queue : Callback function to add buffer to DMA queue
 * @q: ptr to videobuf_queue
 * @vb: ptr to videobuf_buffer
 */
static void ti81xxvin_buffer_queue(struct videobuf_queue *q,
		struct videobuf_buffer *vb)
{
	/* Get the file handle object and instance object */
	struct ti81xxvin_fh *fh = q->priv_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;

	buf_obj = &inst->buf_obj;

	ti81xxvin_dbg(2, debug, "ti81xxvin_buffer_queue\n");

	if (!buf_obj->last_done_buffer)
		buf_obj->last_done_buffer = list_first_entry(
				&buf_obj->buffer_queue.stream,
				struct videobuf_buffer,
				stream);
#if 0
	/* Make FVID2_Framelist here and queue the buffers here */
	/* By the time function comes here FVID2 create is complete and capture
	 * driver is opened and all the parameters are set
	 */
	if (V4L2_MEMORY_USERPTR == buf_obj->memory)
		addr = vb->boff;
	else
		addr = videobuf_to_dma_contig(vb);
	offset = (inst->win.w.left) +
		(inst->win.w.top * buf_obj->fmt.fmt.pix.bytesperline);
	inst->captctrl->set_buffer(inst->captctrl, addr, vb->i, offset);

	/* TODO Check all the software conditions which are checked in
	 * FVID2_queue so that queue call should never return error
	 */
	ret = inst->captctrl->queue(inst->captctrl, vb->i);
	BUG_ON(ret);
#endif
	/* Queue the buffer to driver maintained queueu */
	list_add_tail(&vb->queue, &inst->dma_queue);
	/* Change state of the buffer */
	vb->state = VIDEOBUF_QUEUED;
}

/**
 * ti81xxvin_buffer_release : Callback function to free buffer
 * @q: buffer queue ptr
 * @vb: ptr to video buffer
 *
 * This function is called from the videobuf layer to free memory
 * allocated to  the buffers
 */
static void ti81xxvin_buffer_release(struct videobuf_queue *q,
		struct videobuf_buffer *vb)
{
	/* Get the file handle object and instance object */
	struct ti81xxvin_fh *fh = q->priv_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;

	buf_obj = &inst->buf_obj;
	videobuf_dma_contig_free(q, vb);
	vb->state = VIDEOBUF_NEEDS_INIT;
}

static int ti81xxvin_vps_create(struct ti81xxvin_instance_obj *inst)
{
	int ret = 0;
	struct Vps_CaptReturnErrFrms returnerrfrm;
	struct Vps_CaptOneCallBackPerFrm onecallback;
	struct Vps_CaptStoragePrms storageparams;

	ret = (inst->captctrl->create(inst->captctrl));
	if (ret) {
		ti81xxvin_err("ti81xxvin_vps_create failed\n");
		return ret;
	}
	returnerrfrm.enable = 1;
	ret = (inst->captctrl->control(inst->captctrl,
				IOCTL_VPS_CAPT_RETURN_ERR_FRM,
				(void *)&returnerrfrm, NULL));
	BUG_ON(ret);

	onecallback.enable = 1;
	ret = (inst->captctrl->control(inst->captctrl,
				IOCTL_VPS_CAPT_ONE_CB_PER_FRM,
				(void *)&onecallback, NULL));
	BUG_ON(ret);
	if (inst->video.buf_params.interlaced) {
		storageparams.chNum = 0;
		storageparams.bufferFmt = FVID2_BUF_FMT_FRAME;
		storageparams.fieldMerged = 1;
		ret = (inst->captctrl->control(inst->captctrl,
					IOCTL_VPS_CAPT_SET_STORAGE_FMT,
					(void *)&storageparams, NULL));

	}
	return ret;
}

static void ti81xxvin_vps_delete(struct ti81xxvin_instance_obj *inst)
{
	int ret = 0;
	ret = (inst->captctrl->delete(inst->captctrl));
	BUG_ON(ret);
}

static int ti81xxvin_vps_start(struct ti81xxvin_instance_obj *inst)
{
	int ret = 0;
	ret = (inst->captctrl->start(inst->captctrl));
	return ret;
}

static int ti81xxvin_vps_stop(struct ti81xxvin_instance_obj *inst)
{
	int ret = 0;
	ret = (inst->captctrl->stop(inst->captctrl));
	return ret;
}
/* This function is protected under mutex */
static int ti81xxvin_hdvpss_stop(struct ti81xxvin_instance_obj *inst)
{
	struct videobuf_buffer *buf;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret;

	buf_obj->started = 0;

	/* Disable interrupt first */
	vps_capture_unregister_isr(ti81xxvin_instance_isr,
			(void *)inst->instance_id, inst->instance_id);
	list_for_each_entry(buf, &buf_obj->buffer_queue.stream, stream) {
		buf->state = VIDEOBUF_DONE;
	}

	/* Stop the FVID2_driver */
	ti81xxvin_vps_stop(inst);

	ret = videobuf_streamoff(&buf_obj->buffer_queue);
	if (ret)
		ti81xxvin_dbg(1, debug, "videobuf streamoff failed\n");

	ti81xxvin_vps_delete(inst);
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			s_stream, 0);

	if (ret && (ret != -ENOIOCTLCMD))
		ti81xxvin_dbg(1, debug, "stream off failed in subdev\n");
	return ret;
}

static void ti81xxvin_check_overflow(struct ti81xxvin_instance_obj *inst,
		struct ti81xxvin_overflow_status *stat, int clear)
{
	u32 *addr;
	u32 reg_val;

	addr = (u32 *)inst->addr;
	stat->porta_overflow = 0;
	stat->portb_overflow = 0;

	reg_val = addr[VIP_REG_FIQ_STATUS_OFFSET];

	if (reg_val & VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTA_LUMA_STATUS_MASK) {
		stat->porta_overflow = 1;
		if (clear)
			addr[VIP_REG_FIQ_CLEAR_OFFSET] =
		VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTA_LUMA_STATUS_MASK;
	}
	if (reg_val & VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTB_LUMA_STATUS_MASK) {
		stat->portb_overflow = 1;
		if (clear)
			addr[VIP_REG_FIQ_CLEAR_OFFSET] =
		VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTB_LUMA_STATUS_MASK;
	}
}

static struct videobuf_queue_ops video_qops = {
	.buf_setup = ti81xxvin_buffer_setup,
	.buf_prepare = ti81xxvin_buffer_prepare,
	.buf_queue = ti81xxvin_buffer_queue,
	.buf_release = ti81xxvin_buffer_release,
};

/**
 * vidioc_querycap() - QUERYCAP handler
 * @file: file ptr
 * @priv: file handle
 * @cap: ptr to v4l2_capability structure
 */
static int vidioc_querycap(struct file *file, void  *priv,
		struct v4l2_capability *cap)
{
	struct ti81xxvin_config *config = ti81xxvin_dev->platform_data;

	cap->version = TI81XXVIN_VERSION_CODE;
	cap->capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING |
		V4L2_BUF_TYPE_VIDEO_OVERLAY;
	strlcpy(cap->driver, "ti81xxvin", sizeof(cap->driver));
	strlcpy(cap->bus_info, "TI81xx Platform", sizeof(cap->bus_info));
	strlcpy(cap->card, config->card_name, sizeof(cap->card));

	return 0;
}

/**
 * vidioc_g_priority() - get priority handler
 * @file: file ptr
 * @priv: file handle
 * @prio: ptr to v4l2_priority structure
 */
static int vidioc_g_priority(struct file *file, void *priv,
		enum v4l2_priority *prio)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *ints = fh->instance;

	*prio = v4l2_prio_max(&ints->prio);

	return 0;
}

/**
 * vidioc_s_priority() - set priority handler
 * @file: file ptr
 * @priv: file handle
 * @prio: ptr to v4l2_priority structure
 */
static int vidioc_s_priority(struct file *file, void *priv,
	enum v4l2_priority p)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *ints = fh->instance;

	return v4l2_prio_change(&ints->prio, &fh->prio, p);
}

/**
 * vidioc_enum_fmt_vid_cap() - ENUM_FMT handler
 * @file: file ptr
 * @priv: file handle
 * @index: input index
 */
static int vidioc_enum_fmt_vid_cap(struct file *file, void  *priv,
		struct v4l2_fmtdesc *fmt)
{
	switch (fmt->index) {
	case (0):
		strcpy(fmt->description, "YCbCr4:2:2 YC Interleaved");
		fmt->pixelformat = V4L2_PIX_FMT_YUYV;
		break;
	case (1):
		strcpy(fmt->description, "YCbCr4:2:0 YC Semiplaner");
		fmt->pixelformat = V4L2_PIX_FMT_NV12;
		break;
	case (2):
		strcpy(fmt->description, "YCbCr4:2:2 YC Semiplaner");
		fmt->pixelformat = V4L2_PIX_FMT_NV16;
		break;
	case (3):
		strcpy(fmt->description, "RGB Packed");
		fmt->pixelformat = V4L2_PIX_FMT_RGB24;
		break;
	default:
		ti81xxvin_dbg(1, debug, "Invalid format index\n");
		return -EINVAL;
	}
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	return 0;
}

/**
 * vidioc_s_fmt_vid_cap() - Set FMT handler
 * @file: file ptr
 * @priv: file handle
 * @fmt: ptr to v4l2 format structure
 */
static int vidioc_s_fmt_vid_cap(struct file *file, void *priv,
		struct v4l2_format *fmt)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct v4l2_pix_format *pixfmt;
	int ret = 0;

	ti81xxvin_dbg(2, debug, "VIDIOC_S_FMT\n");

	/* If streaming is started, return error */
	if (buf_obj->started) {
		ti81xxvin_dbg(1, debug, "Streaming is started\n");
		return -EBUSY;
	}

	if (!fh->initialized) {
		ti81xxvin_dbg(1, debug, "Channel Busy\n");
		return -EBUSY;
	}

	ret = v4l2_prio_check(&inst->prio, fh->prio);
	if (0 != ret)
		return ret;

	fh->initialized = 1;

	pixfmt = &fmt->fmt.pix;
	/* Check for valid field format */
	ret = ti81xxvin_check_format(inst, pixfmt, 1);

	if (ret)
		return ret;
	/* store the format in the instance object */
	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;
	buf_obj->fmt = *fmt;
	mutex_unlock(&buf_obj->buf_lock);

	return 0;
}

/**
 * vidioc_try_fmt_vid_cap() - TRY_FMT handler
 * @file: file ptr
 * @priv: file handle
 * @fmt: ptr to v4l2 format structure
 */
static int vidioc_try_fmt_vid_cap(struct file *file, void *priv,
		struct v4l2_format *fmt)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct v4l2_pix_format *pixfmt = &fmt->fmt.pix;

	return ti81xxvin_check_format(inst, pixfmt, 1);
}

/**
 * vidioc_g_fmt_vid_cap() - Set INPUT handler
 * @file: file ptr
 * @priv: file handle
 * @fmt: ptr to v4l2 format structure
 */
static int vidioc_g_fmt_vid_cap(struct file *file, void *priv,
		struct v4l2_format *fmt)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;

	/* Check the validity of the buf_obj type */
	if (buf_obj->fmt.type != fmt->type)
		return -EINVAL;

	/* Fill in the information about format */
	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	*fmt = buf_obj->fmt;
	mutex_unlock(&buf_obj->buf_lock);
	return 0;
}

/**
 * vidioc_enum_input() - ENUMINPUT handler
 * @file: file ptr
 * @priv: file handle
 * @input: ptr to input structure
 */
static int vidioc_enum_input(struct file *file, void *priv,
		struct v4l2_input *input)
{

	struct ti81xxvin_config *config = ti81xxvin_dev->platform_data;
	struct ti81xxvin_inst_config *inst_cfg;
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;

	inst_cfg = &config->inst_config[inst->instance_id];

	if (input->index >= inst_cfg->input_count) {
		ti81xxvin_dbg(1, debug, "Invalid input index\n");
		return -EINVAL;
	}

	memcpy(input, &inst_cfg->inputs[input->index].input,
			sizeof(*input));
	return 0;
}

/**
 * vidioc_g_input() - Get INPUT handler
 * @file: file ptr
 * @priv: file handle
 * @index: ptr to input index
 */
static int vidioc_g_input(struct file *file, void *priv, unsigned int *index)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_vid_obj *vid_ch = &inst->video;

	*index = vid_ch->input_idx;

	return 0;
}

/**
 * vidioc_s_input() - Set INPUT handler
 * @file: file ptr
 * @priv: file handle
 * @index: input index
 */
static int vidioc_s_input(struct file *file, void *priv, unsigned int index)
{
	struct ti81xxvin_config *config = ti81xxvin_dev->platform_data;
	struct ti81xxvin_inst_config *inst_cfg;
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct ti81xxvin_vid_obj *vid_ch = &inst->video;
	struct ti81xxvin_subdev_info *subdev_info;
	int ret = 0, sd_index = 0;

	inst_cfg = &config->inst_config[inst->instance_id];

	if (buf_obj->started) {
		ti81xxvin_err("Streaming in progress\n");
		return -EBUSY;
	}

	if (!fh->initialized) {
		ti81xxvin_dbg(1, debug, "Channel Busy\n");
		return -EBUSY;
	}

	ret = v4l2_prio_check(&inst->prio, fh->prio);
	if (0 != ret)
		return ret;

	fh->initialized = 1;
	subdev_info = hdvpss_map_sub_device_to_input(inst, config, index,
			&sd_index);
	if (NULL == subdev_info) {
		ti81xxvin_dbg(1, debug,
			"couldn't lookup sub device for the input index\n");
		return -EINVAL;
	}

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	vid_ch->input_idx = index;
	inst->curr_subdev_info = subdev_info;
	inst->curr_sd_index = sd_index;

	/* update tvnorms from the sub device input info */
	inst->video_dev->tvnorms = inst_cfg->inputs[index].input.std;

	mutex_unlock(&buf_obj->buf_lock);

	inst->captctrl->isinputset = 1;
	return ret;
}

/**
 * vidioc_query_dv_preset() - querystd handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 *
 * This function is called to detect standard at the selected input
 */
static int vidioc_query_dv_preset(struct file *file,
		void *priv, struct v4l2_dv_preset *qpreset)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;
	struct v4l2_mbus_framefmt mbus_framefmt;
	struct ti81xxvin_config *config = ti81xxvin_dev->platform_data;
	struct ti81xxvin_subdev_info *subdev_info;
	int sd_index;


	ti81xxvin_dbg(2, debug, "vidioc_query_dv_preset\n");

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	/* Call querystd function of decoder device */
	sd_index = inst->curr_sd_index;
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[sd_index], video,
			query_dv_preset, qpreset);
	if (ret < 0) {
		ti81xxvin_dbg(1, debug, "Failed to set dv format"
		" for sub devices\n");
		goto vidioc_query_dv_preset_exit;
	}
	inst->video.cur_dv_preset.preset = V4L2_DV_INVALID;
	inst->video.cur_std_id = V4L2_STD_UNKNOWN;
	inst->video.cur_dv_preset = *qpreset;

	ret = v4l2_subdev_call(ti81xxvin_obj.sd[sd_index], video,
			g_mbus_fmt, &mbus_framefmt);
	/* Get the information about the standard */
	if (ti81xxvin_update_std_info(inst, &mbus_framefmt)) {
		ret = -EINVAL;
		ti81xxvin_err("Error getting the standard info\n");
		goto vidioc_query_dv_preset_exit;
	}
	subdev_info = config->subdev_info;
	if (subdev_info[sd_index].ti81xxvin_set_mode) {
		switch (inst->video.cur_dv_preset.preset) {
		case V4L2_DV_1080P60:
		case V4L2_DV_1080P50:
			ret = subdev_info[sd_index].ti81xxvin_set_mode(
					FVID2_STD_1080P_60);
		break;
		case V4L2_DV_720P60:
		case V4L2_DV_720P50:
			ret = subdev_info[sd_index].ti81xxvin_set_mode(
					FVID2_STD_720P_60);
		break;
		case V4L2_DV_1080I60:
		case V4L2_DV_1080I50:
			ret = subdev_info[sd_index].ti81xxvin_set_mode(
					FVID2_STD_1080I_60);
		break;
		default:
			ret = subdev_info[sd_index].ti81xxvin_set_mode(
					FVID2_STD_1080P_60);
		}
	}
	/* Configure the default format information according to the std
	 * selected
	 */
	ti81xxvin_config_format(inst);

	ti81xxvin_vps_config_format(inst);
vidioc_query_dv_preset_exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_query_dv_preset() - querystd handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 *
 * This function is called to detect standard at the selected input
 */
static int vidioc_querystd(struct file *file, void *priv, v4l2_std_id *norm)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;
	struct v4l2_mbus_framefmt mbus_framefmt;

	ti81xxvin_dbg(2, debug, "vidioc_query_dv_preset\n");

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	/* Call querystd function of decoder device */
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			querystd, norm);
	if (ret < 0) {
		ti81xxvin_dbg(1, debug, "Failed to set dv format"
		" for sub devices\n");
		goto querystd_exit;
	}
	inst->video.cur_dv_preset.preset = V4L2_DV_INVALID;
	inst->video.cur_std_id = V4L2_STD_UNKNOWN;
	inst->video.cur_std_id = *norm;

	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			g_mbus_fmt, &mbus_framefmt);
	/* Get the information about the standard */
	if (ti81xxvin_update_std_info(inst, &mbus_framefmt)) {
		ret = -EINVAL;
		ti81xxvin_err("Error getting the standard info\n");
		goto querystd_exit;
	}
	/* Configure the default format information according to the std
	 * selected
	 */
	ti81xxvin_config_format(inst);

	ti81xxvin_vps_config_format(inst);
querystd_exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_s_dv_preset() - set STD handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 */
static int vidioc_s_dv_preset(struct file *file,
		void *priv, struct v4l2_dv_preset *dv_preset)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;
	struct v4l2_mbus_framefmt mbus_framefmt;

	ti81xxvin_dbg(2, debug, "vidioc_s_dv_preset\n");

	if (buf_obj->started) {
		ti81xxvin_err("streaming in progress\n");
		return -EBUSY;
	}

	ret = v4l2_prio_check(&inst->prio, fh->prio);
	if (0 != ret)
		return ret;

	fh->initialized = 1;

	/* Call encoder subdevice function to set the standard */
	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;
	/* Call the V4l2 subdev to set the preset */
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			s_dv_preset, dv_preset);
	if (ret)
		goto s_dv_preset_exit;
	inst->video.cur_dv_preset.preset = V4L2_DV_INVALID;
	inst->video.cur_std_id = V4L2_STD_UNKNOWN;
	inst->video.cur_dv_preset = *dv_preset;

	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			g_mbus_fmt, &mbus_framefmt);
	/* Get the information about the standard */
	if (ti81xxvin_update_std_info(inst, &mbus_framefmt)) {
		ret = -EINVAL;
		ti81xxvin_err("Error getting the standard info\n");
		goto s_dv_preset_exit;
	}

	/* Configure the default format information according to the std
	 * selected
	 */
	ti81xxvin_config_format(inst);
s_dv_preset_exit:
	if (ret < 0)
		ti81xxvin_dbg(1, debug, "Failed to set standard"
		" for sub devices\n");

	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_s_std() - set STD handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 */
static int vidioc_s_std(struct file *file,
		void *priv, v4l2_std_id *norm)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;
	struct v4l2_mbus_framefmt mbus_framefmt;

	ti81xxvin_dbg(2, debug, "vidioc_s_std\n");

	if (buf_obj->started) {
		ti81xxvin_err("streaming in progress\n");
		return -EBUSY;
	}

	ret = v4l2_prio_check(&inst->prio, fh->prio);
	if (0 != ret)
		return ret;

	fh->initialized = 1;

	/* Call encoder subdevice function to set the standard */
	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;
	/* Call the V4l2 subdev to set the preset */
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], core,
			s_std, *norm);
	if (ret)
		goto s_std_exit;
	inst->video.cur_dv_preset.preset = V4L2_DV_INVALID;
	inst->video.cur_std_id = V4L2_STD_UNKNOWN;
	inst->video.cur_std_id = *norm;

	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			g_mbus_fmt, &mbus_framefmt);
	/* Get the information about the standard */
	if (ti81xxvin_update_std_info(inst, &mbus_framefmt)) {
		ret = -EINVAL;
		ti81xxvin_err("Error getting the standard info\n");
		goto s_std_exit;
	}

	/* Configure the default format information according to the std
	 * selected
	 */
	ti81xxvin_config_format(inst);
s_std_exit:
	if (ret < 0)
		ti81xxvin_dbg(1, debug, "Failed to set standard"
		" for sub devices\n");

	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_g_dv_preset() - get STD handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 */
static int vidioc_g_dv_preset(struct file *file, void *priv,
		struct v4l2_dv_preset *preset)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;

	ti81xxvin_dbg(2, debug, "vidioc_g_dv_preset\n");

	preset->preset = inst->video.cur_dv_preset.preset;
	return 0;
}

/**
 * vidioc_g_std() - get STD handler
 * @file: file ptr
 * @priv: file handle
 * @std_id: ptr to std id
 */
static int vidioc_g_std(struct file *file, void *priv,
		v4l2_std_id *norm)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;

	ti81xxvin_dbg(2, debug, "vidioc_g_std\n");

	*norm = inst->video.cur_std_id;
	return 0;
}

/**
 * vidioc_enum_dv_presets() - get STD handler
 * @file: file ptr
 * @priv: file handle
 * @preset: ptr to v4l2_dv_enum_preset
 */
static int vidioc_enum_dv_presets(struct file *file, void *priv,
		struct v4l2_dv_enum_preset *preset)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;

	ti81xxvin_dbg(2, debug, "vidioc_enum_dv_presets\n");
	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;
	/* Call the V4l2 subdev to set the preset */
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			enum_dv_presets, preset);
	if (ret)
		goto enum_dv_presets_exit;
	mutex_unlock(&buf_obj->buf_lock);

	return 0;
enum_dv_presets_exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_reqbufs() - request buffer handler
 * @file: file ptr
 * @priv: file handle
 * @reqbuf: request buffer structure ptr
 */
static int vidioc_reqbufs(struct file *file, void *priv,
		struct v4l2_requestbuffers *reqbuf)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;
	int ret = 0;

	ti81xxvin_dbg(2, debug, "vidioc_reqbufs\n");

	/**
	 * This file handle has not initialized the instance,
	 * It is not allowed to do settings
	 */
	if (!fh->initialized) {
		ti81xxvin_dbg(1, debug, "Channel Busy\n");
		return -EBUSY;
	}
	if (V4L2_BUF_TYPE_VIDEO_CAPTURE != reqbuf->type)
		return -EINVAL;

	buf_obj = &inst->buf_obj;
	mutex_lock(&buf_obj->buf_lock);

	if (0 != buf_obj->io_usrs) {
		ret = -EBUSY;
		goto reqbuf_exit;
	}

	/* Initialize videobuf queue as per the buffer type */
	videobuf_queue_dma_contig_init(&buf_obj->buffer_queue,
			&video_qops, NULL,
			&buf_obj->buf_irqlock,
			reqbuf->type,
			buf_obj->fmt.fmt.pix.field,
			sizeof(struct videobuf_buffer), fh,
			&buf_obj->buf_lock);

	/* Set io allowed member of file handle to TRUE */
	fh->io_allowed = 1;
	/* Increment io usrs member of instance object to 1 */
	buf_obj->io_usrs = 1;
	/* Store type of memory requested in instance object */
	buf_obj->memory = reqbuf->memory;

	/* Initialize driver queue head */
	INIT_LIST_HEAD(&inst->dma_queue);
	/* Allocate buffers */
	ret = videobuf_reqbufs(&buf_obj->buffer_queue, reqbuf);

reqbuf_exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_querybuf() - query buffer handler
 * @file: file ptr
 * @priv: file handle
 * @buf: v4l2 buffer structure ptr
 */
static int vidioc_querybuf(struct file *file, void *priv,
		struct v4l2_buffer *buf)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int r;

	mutex_lock(&buf_obj->buf_lock);
	ti81xxvin_dbg(2, debug, "vidioc_querybuf\n");

	if (buf_obj->fmt.type != buf->type)
		return -EINVAL;

	if (buf_obj->memory != V4L2_MEMORY_MMAP) {
		ti81xxvin_dbg(1, debug, "Invalid memory\n");
		return -EINVAL;
	}
	r = videobuf_querybuf(&buf_obj->buffer_queue, buf);
	mutex_unlock(&buf_obj->buf_lock);
	return r;
}

/**
 * vidioc_qbuf() - query buffer handler
 * @file: file ptr
 * @priv: file handle
 * @buf: v4l2 buffer structure ptr
 */
static int vidioc_qbuf(struct file *file, void *priv, struct v4l2_buffer *buf)
{

	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct v4l2_buffer tbuf = *buf;
	u32 addr, offset;
	int ret = 0;


	if (buf_obj->fmt.type != tbuf.type) {
		ti81xxvin_err("invalid buffer type\n");
		ret =  -EINVAL;
		goto qbuf_exit;
	}
	if (!fh->io_allowed) {
		ti81xxvin_err("fh io not allowed\n");
		ret = -EACCES;
		goto qbuf_exit;
	}
	mutex_lock(&buf_obj->buf_lock);
	ti81xxvin_dbg(2, debug, "vidioc_qbuf\n");

	ret = videobuf_qbuf(&buf_obj->buffer_queue, buf);
	if (buf_obj->started && !ret) {
		struct videobuf_buffer *vb;
		vb = list_entry(inst->dma_queue.prev,
				struct videobuf_buffer, queue);

		if (V4L2_MEMORY_USERPTR == buf_obj->memory)
			addr = vb->boff;
		else
			addr = videobuf_to_dma_contig(vb);
		offset = (inst->win.w.left) +
			(inst->win.w.top * buf_obj->fmt.fmt.pix.bytesperline);
		inst->captctrl->set_buffer(inst->captctrl, addr, vb->i, offset);

		/* TODO Check all the software conditions which are checked in
		 * FVID2_queue so that queue call should never return error
		 */
		ret = inst->captctrl->queue(inst->captctrl, vb->i);
		BUG_ON(ret);
	}
qbuf_exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * vidioc_dqbuf() - query buffer handler
 * @file: file ptr
 * @priv: file handle
 * @buf: v4l2 buffer structure ptr
 */
static int vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *buf)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret = 0;
	int num_frms_dequeued;

	mutex_lock(&buf_obj->buf_lock);
	ti81xxvin_dbg(2, debug, "vidioc_dqbuf\n");

	ret = inst->captctrl->dequeue(inst->captctrl, 0);
	num_frms_dequeued = inst->captctrl->framelist[0]->numframes;
	if (num_frms_dequeued > 1)
		BUG_ON(num_frms_dequeued);

	ret = videobuf_dqbuf(&buf_obj->buffer_queue, buf,
			file->f_flags & O_NONBLOCK);
	mutex_unlock(&buf_obj->buf_lock);
	return ret;

}

/**
 * vidioc_streamon() - streamon handler
 * @file: file ptr
 * @priv: file handle
 * @buftype: v4l2 buffer type
 */
static int vidioc_streamon(struct file *file, void *priv,
		enum v4l2_buf_type buftype)
{

	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct ti81xxvin_vid_obj *vid_ch;
	int ret = 0;
	struct videobuf_buffer *vb;
	u32 addr, offset;

	ti81xxvin_dbg(2, debug, "vidioc_streamon\n");
	mutex_lock(&buf_obj->buf_lock);
	vid_ch = &inst->video;
	if (buftype != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		ti81xxvin_dbg(1, debug, "buffer type not supported\n");
		ret = -EINVAL;
		goto vidioc_streamon_failed;
	}
	/* If file handle is not allowed IO, return error */
	if (!fh->io_allowed) {
		ti81xxvin_dbg(1, debug, "io not allowed\n");
		ret = -EACCES;
		goto vidioc_streamon_failed;
	}
	/* If Streaming is already started, return error */
	if (buf_obj->started) {
		ti81xxvin_dbg(1, debug, "instance->started\n");
		ret = -EBUSY;
		goto vidioc_streamon_failed;
	}
	ret = ti81xxvin_check_format(inst, &buf_obj->fmt.fmt.pix, 0);
	if (ret) {
		ti81xxvin_dbg(1, debug, "io not allowed\n");
		goto vidioc_streamon_failed;

	}
	/* After checking the buffer format. Call the FVID2 Create and
	 * Set format for starting the driver
	 */
	/* Enable streamon on the sub device */
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			s_stream, 1);

	if (ret && (ret != -ENOIOCTLCMD)) {
		ti81xxvin_dbg(1, debug, "stream on failed in subdev\n");
		goto vidioc_streamon_failed;
	}
	ret = ti81xxvin_vps_create(inst);
	if (ret) {
		ti81xxvin_err("Vps create failed\n");
		goto ti81xxvin_vps_create_failed;

	}
	/* Register interrupt handler */
	vps_capture_register_isr(ti81xxvin_instance_isr,
			(void *)inst->instance_id, inst->instance_id);
	/* Call videobuf_streamon to start streaming in videobuf */
	ret = videobuf_streamon(&buf_obj->buffer_queue);
	if (ret) {
		ti81xxvin_dbg(1, debug, "videobuf_streamon\n");
		goto streamon_failed;
	}
	if (list_empty(&inst->dma_queue)) {
		mutex_unlock(&buf_obj->buf_lock);
		ret = -EIO;
		goto streamon_failed;
	}

	list_for_each_entry(vb, &inst->dma_queue, queue) {
		/*though queue all prime buffer, but only the first
		buffer will be displayed. other still int the QUEUE state*/
		if (vb->i == 0)
			vb->state = VIDEOBUF_ACTIVE;
		if (V4L2_MEMORY_USERPTR == buf_obj->memory)
			addr = vb->boff;
		else
			addr = videobuf_to_dma_contig(vb);
		offset = (inst->win.w.left) +
			(inst->win.w.top * buf_obj->fmt.fmt.pix.bytesperline);
		inst->captctrl->set_buffer(inst->captctrl, addr, vb->i, offset);

		/* TODO Check all the software conditions which are checked in
		* FVID2_queue so that queue call should never return error
		*/
		ret = inst->captctrl->queue(inst->captctrl, vb->i);
		BUG_ON(ret);

	}

	/* Initialize field_id and started member */
	inst->field_id = 0;
	buf_obj->started = 1;

	if ((vid_ch->buf_params.interlaced &&
				(buf_obj->fmt.fmt.pix.field !=
				V4L2_FIELD_INTERLACED)) ||
			(!vid_ch->buf_params.interlaced &&
			 (buf_obj->fmt.fmt.pix.field != V4L2_FIELD_NONE))) {
		ti81xxvin_dbg(1, debug, "conflict in field format"
			" and std format\n");
		ret = -EINVAL;
		goto vps_start_failed;
	}

	ret = ti81xxvin_vps_start(inst);
	if (ret) {
		ti81xxvin_err("vps start failed\n");
		goto vps_start_failed;
	}
	ret = mod_timer(&inst->overflow_timer,
		jiffies + msecs_to_jiffies(100));
	if (ret)
		ti81xxvin_err("Setting up of timer failed\n");

	mutex_unlock(&buf_obj->buf_lock);
	return 0;

vps_start_failed:
	videobuf_streamoff(&buf_obj->buffer_queue);
streamon_failed:
	vps_capture_unregister_isr(ti81xxvin_instance_isr,
			(void *)inst->instance_id, inst->instance_id);
ti81xxvin_vps_create_failed:
	ret = v4l2_subdev_call(ti81xxvin_obj.sd[inst->curr_sd_index], video,
			s_stream, 0);
vidioc_streamon_failed:
	mutex_unlock(&buf_obj->buf_lock);

	return ret;
}

/**
 * vidioc_cropcap() - cropcap handler
 * @file: file ptr
 * @priv: file handle
 * @crop: ptr to v4l2_cropcap structure
 */
static int vidioc_cropcap(struct file *file, void *priv,
		struct v4l2_cropcap *crop)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct ti81xxvin_buffer_params *params = &inst->video.buf_params;

	if (V4L2_BUF_TYPE_VIDEO_CAPTURE != crop->type)
		return -EINVAL;
	if (params->interlaced)
		return -EINVAL;
	crop->bounds.left = 0;
	crop->bounds.top = 0;
	crop->bounds.height = buf_obj->fmt.fmt.pix.height;
	crop->bounds.width = buf_obj->fmt.fmt.pix.width;
	crop->defrect = crop->bounds;
	return 0;
}

static int vidioc_g_crop(struct file *file, void *priv,
		struct v4l2_crop *crop)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;

	if (crop->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		crop->c = inst->crop;
		return 0;
	} else
		return -EINVAL;
}

static int vidioc_s_crop(struct file *file, void *priv,
		struct v4l2_crop *crop)
{
	int err = -EINVAL;
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	u32 decoder_width = inst->video.buf_params.width;
	u32 decoder_height = inst->video.buf_params.height;
	struct ti81xxvin_buffer_params *params = &inst->video.buf_params;

	if (buf_obj->started)
		return -EBUSY;

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	if (params->interlaced) {
		mutex_unlock(&buf_obj->buf_lock);
		return -EINVAL;
	}
	if (crop->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		err = ti81xxvin_new_crop(decoder_width, decoder_height,
				&inst->crop, &crop->c);
		inst->win.w.left = 0;
		inst->win.w.top = 0;
		inst->win.w.width = inst->crop.width;
		inst->win.w.height = inst->crop.height;
		mutex_unlock(&buf_obj->buf_lock);
		return err;
	} else {
		mutex_unlock(&buf_obj->buf_lock);
		return -EINVAL;
	}
}

static int vidioc_try_fmt_vid_overlay(struct file *file, void *priv,
		struct v4l2_format *f)
{
	int err = -EINVAL;
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct v4l2_window *win = &f->fmt.win;

	err = ti81xxvin_try_window(&inst->crop, win);

	if (err)
		return err;
	return 0;
}

static int vidioc_s_fmt_vid_overlay(struct file *file, void *priv,
		struct v4l2_format *f)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	struct v4l2_window *win = &f->fmt.win;
	int err = -EINVAL;
	struct ti81xxvin_buffer_params *params = &inst->video.buf_params;

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	if (params->interlaced) {
		mutex_unlock(&buf_obj->buf_lock);
		return -EINVAL;
	}
	err = ti81xxvin_new_window(&inst->crop, &inst->win, win);
	if (err) {
		mutex_unlock(&buf_obj->buf_lock);
		return err;
	}
	mutex_unlock(&buf_obj->buf_lock);
	return 0;
}

static int vidioc_g_fmt_vid_overlay(struct file *file, void *priv,
		struct v4l2_format *f)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct v4l2_window *win = &f->fmt.win;

	memset(win, 0, sizeof(*win));

	/*
	 * The API has a bit of a problem here.  We're returning a v4l2_window
	 * structure, but that structure contains pointers to variable-sized
	 * objects for clipping rectangles and clipping bitmaps.  We will just
	 * return NULLs for those pointers.
	 */
	win->w = inst->win.w;
	win->field = inst->win.field;
	win->global_alpha = 255;
	win->chromakey = 0;
	return 0;
}

#if 0
static int vidioc_enum_fmt_vid_overlay(struct file *file, void *fh,
		struct v4l2_fmtdesc *fmt)
{
	int index = fmt->index;
	enum v4l2_buf_type type = fmt->type;

	memset(fmt, 0, sizeof(*fmt));
	fmt->index = index;
	fmt->type = type;
	if (index >= NUM_OUTPUT_FORMATS)
		return -EINVAL;

	fmt->flags = omap_formats[index].flags;
	strncpy(fmt->description, omap_formats[index].description,
			sizeof(fmt->description));
	fmt->pixelformat = omap_formats[index].pixelformat;
	return 0;
}
#endif

/**
 * vidioc_streamoff() - streamoff handler
 * @file: file ptr
 * @priv: file handle
 * @buftype: v4l2 buffer type
 */
static int vidioc_streamoff(struct file *file, void *priv,
		enum v4l2_buf_type buftype)
{

	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &inst->buf_obj;
	int ret;

	mutex_lock(&buf_obj->buf_lock);
	ti81xxvin_dbg(2, debug, "vidioc_streamoff\n");

	if (buftype != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		ti81xxvin_dbg(1, debug, "buffer type not supported\n");
		return -EINVAL;
	}

	/* If io is allowed for this file handle, return error */
	if (!fh->io_allowed) {
		ti81xxvin_dbg(1, debug, "io not allowed\n");
		return -EACCES;
	}

	/* If streaming is not started, return error */
	if (!buf_obj->started) {
		ti81xxvin_dbg(1, debug, "instance->started\n");
		return -EINVAL;
	}
	ret = ti81xxvin_hdvpss_stop(inst);

	/* Set io_allowed member to false */
	fh->io_allowed = 0;
	/* Reset io_usrs member of instance object */
	buf_obj->io_usrs = 0;
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

static long vidioc_default(struct file *file, void *priv, int cmd, void *arg)
{
	struct ti81xxvin_fh *fh = priv;
	struct ti81xxvin_instance_obj *inst = fh->instance;

	switch (cmd) {
	case TICAPT_CHECK_OVERFLOW:
		ti81xxvin_check_overflow(inst,
			(struct ti81xxvin_overflow_status *)arg, 1);
		return 0;
		break;
	default:
		return -EINVAL;
	}
}
/* TI81xx capture ioctl operations */
static const struct v4l2_ioctl_ops hdvpss_ioctl_ops = {
	.vidioc_querycap		= vidioc_querycap,
	.vidioc_g_priority		= vidioc_g_priority,
	.vidioc_s_priority		= vidioc_s_priority,
	.vidioc_enum_fmt_vid_cap	= vidioc_enum_fmt_vid_cap,
	.vidioc_g_fmt_vid_cap		= vidioc_g_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap		= vidioc_s_fmt_vid_cap,
	.vidioc_try_fmt_vid_cap		= vidioc_try_fmt_vid_cap,
	.vidioc_enum_input		= vidioc_enum_input,
	.vidioc_s_input			= vidioc_s_input,
	.vidioc_g_input			= vidioc_g_input,
	.vidioc_query_dv_preset		= vidioc_query_dv_preset,
	.vidioc_s_dv_preset		= vidioc_s_dv_preset,
	.vidioc_g_dv_preset		= vidioc_g_dv_preset,
	.vidioc_enum_dv_presets		= vidioc_enum_dv_presets,
	.vidioc_querystd		= vidioc_querystd,
	.vidioc_g_std			= vidioc_g_std,
	.vidioc_s_std			= vidioc_s_std,
	.vidioc_reqbufs			= vidioc_reqbufs,
	.vidioc_querybuf		= vidioc_querybuf,
	.vidioc_qbuf			= vidioc_qbuf,
	.vidioc_dqbuf			= vidioc_dqbuf,
	.vidioc_streamon		= vidioc_streamon,
	.vidioc_streamoff		= vidioc_streamoff,
	.vidioc_cropcap			= vidioc_cropcap,
	.vidioc_g_crop			= vidioc_g_crop,
	.vidioc_s_crop			= vidioc_s_crop,
	.vidioc_try_fmt_vid_overlay	= vidioc_try_fmt_vid_overlay,
	.vidioc_s_fmt_vid_overlay	= vidioc_s_fmt_vid_overlay,
	/*.vidioc_enum_fmt_vid_overlay	= vidioc_enum_fmt_vid_overlay,*/
	.vidioc_g_fmt_vid_overlay	= vidioc_g_fmt_vid_overlay,
	.vidioc_default			= vidioc_default,
};

/**
 * ti81xxvin_open : TI81xx capture open handler
 * @filep: file ptr
 *
 * It creates object of file handle structure and stores it in private_data
 * member of filepointer
 */
static int ti81xxvin_open(struct file *filep)
{
	struct ti81xxvin_config *config = ti81xxvin_dev->platform_data;
	struct video_device *vdev = video_devdata(filep);
	struct ti81xxvin_buffer_obj *buf_obj;
	struct ti81xxvin_vid_obj *vid_inst;
	struct ti81xxvin_instance_obj *inst;
	struct ti81xxvin_fh *fh;
	int i, ret = 0;

	ti81xxvin_dbg(2, debug, "ti81xxvin_open\n");

	inst = video_get_drvdata(vdev);
	setup_timer(&inst->overflow_timer, ti81xxvin_overflow_handler,
		(unsigned long)inst);


	vid_inst = &inst->video;
	buf_obj = &inst->buf_obj;

	if (mutex_lock_interruptible(&buf_obj->buf_lock))
		return -ERESTARTSYS;

	if (NULL == inst->curr_subdev_info) {
		/**
		 * searinst through the sub device to see a registered
		 * sub device and make it as current sub device
		 */
		for (i = 0; i < config->subdev_count; i++) {
			if (ti81xxvin_obj.sd[i]) {
				/* the sub device is registered */
				inst->curr_subdev_info =
					&config->subdev_info[i];
				/* make first input as the current input */
				vid_inst->input_idx = 0;
				break;
			}
		}
		if (i == config->subdev_count) {
			ti81xxvin_err("No sub device registered\n");
			ret = -ENOENT;
			goto exit;
		}
	}

	/* Allocate memory for the file handle object */
	fh = kmalloc(sizeof(struct ti81xxvin_fh), GFP_KERNEL);
	if (NULL == fh) {
		ti81xxvin_err("unable to allocate memory for file"
			"handle object\n");
		ret = -ENOMEM;
		goto exit;
	}

	/* store pointer to fh in private_data member of filep */
	filep->private_data = fh;
	fh->instance = inst;
	fh->initialized = 0;
	/* If decoder is not initialized. initialize it */
	if (!inst->initialized) {
		fh->initialized = 1;
		inst->initialized = 1;
		/*memset(&(inst->hdvpssparams),
			0, sizeof(struct hdvpss_params));*/
	}
	/* Increment instance usrs counter */
	inst->usrs++;
	/* Set io_allowed member to false */
	fh->io_allowed = 0;
	/* Initialize priority of this instance to default priority */
	fh->prio = V4L2_PRIORITY_UNSET;
	v4l2_prio_open(&inst->prio, &fh->prio);
exit:
	mutex_unlock(&buf_obj->buf_lock);
	return ret;
}

/**
 * ti81xxvin_release : function to clean up file close
 * @filep: file pointer
 *
 * This function deletes buf_obj queue, frees the buffers and the vpfe file
 * handle
 */
static int ti81xxvin_release(struct file *filep)
{
	struct ti81xxvin_fh *fh = filep->private_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj;
	int retry_count;
	int ret = 0;

	ti81xxvin_dbg(2, debug, "ti81xxvin_release\n");

	buf_obj = &inst->buf_obj;

	mutex_lock(&buf_obj->buf_lock);

	/* Reset io_usrs member of instance object */
	buf_obj->io_usrs = 0;
	/* If streaming is not started, return error */
	if (buf_obj->started)
		ti81xxvin_hdvpss_stop(inst);
	/* if this instance is doing IO */
	if (fh->io_allowed) {
		buf_obj->started = 0;
		/* Free buffers allocated */
		videobuf_queue_cancel(&buf_obj->buffer_queue);
		videobuf_mmap_free(&buf_obj->buffer_queue);
	}
	/* Set io_allowed member to false */
	fh->io_allowed = 0;
	retry_count = 0;
	do {
		ret = del_timer(&inst->overflow_timer);
		if (ret) {
			retry_count++;
			mdelay(16);
		} else
			break;
	} while (retry_count < 10);
	if (ret)
		ti81xxvin_err("Failed to del timer\n");
	/* Decrement instance usrs counter */
	inst->usrs--;
	/* unlock mutex on instance object */
	mutex_unlock(&buf_obj->buf_lock);
	/* Close the priority */
	v4l2_prio_close(&inst->prio, fh->prio);
	if (fh->initialized)
		inst->initialized = 0;
	filep->private_data = NULL;
	kfree(fh);
	return 0;
}

/**
 * ti81xxvin_poll: It is used for select/poll system call
 * @filep: file pointer
 * @wait: poll table to wait
 */
static unsigned int ti81xxvin_poll(struct file *filep, poll_table * wait)
{
	int err = 0;
	struct ti81xxvin_fh *fh = filep->private_data;
	struct ti81xxvin_instance_obj *instance = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &(instance->buf_obj);

	ti81xxvin_dbg(2, debug, "ti81xxvin_poll\n");
	mutex_lock(&buf_obj->buf_lock);
	if (buf_obj->started)
		err = videobuf_poll_stream(filep, &buf_obj->buffer_queue, wait);

	mutex_unlock(&buf_obj->buf_lock);
	return 0;
}

/**
 * ti81xxvin_mmap : It is used to map kernel space buffers into user spaces
 * @filep: file pointer
 * @vma: ptr to vm_area_struct
 */
static int ti81xxvin_mmap(struct file *filep, struct vm_area_struct *vma)
{
	/* Get the instance object and file handle object */
	struct ti81xxvin_fh *fh = filep->private_data;
	struct ti81xxvin_instance_obj *inst = fh->instance;
	struct ti81xxvin_buffer_obj *buf_obj = &(inst->buf_obj);

	ti81xxvin_dbg(2, debug, "ti81xxvin_mmap\n");
	mutex_lock(&buf_obj->buf_lock);
	return videobuf_mmap_mapper(&buf_obj->buffer_queue, vma);
	mutex_unlock(&buf_obj->buf_lock);
}
/* TI81xx capture file operations */
static struct v4l2_file_operations hdvpss_fops = {
	.owner = THIS_MODULE,
	.open = ti81xxvin_open,
	.release = ti81xxvin_release,
	.ioctl = video_ioctl2,
	.mmap = ti81xxvin_mmap,
	.poll = ti81xxvin_poll
};

static struct video_device hdvpss_video_template = {
	.name		= "ti81xxvin",
	.fops		= &hdvpss_fops,
	.minor		= -1,
	.ioctl_ops	= &hdvpss_ioctl_ops,
};


/**
 * ti81xxvin_initialize() - Initialize TI81xx capture data structures
 *
 * Allocate memory for data structures and initialize them
 */
static int ti81xxvin_initialize(void)
{
	int ret = 0, i, j;
	int free_instance_objects_index;

	/* Default number of buffers should be 3 */
	if ((inst0_numbuffers > 0) &&
	    (inst0_numbuffers < config_params.min_numbuffers))
		inst0_numbuffers = config_params.min_numbuffers;
	if ((inst1_numbuffers > 0) &&
	    (inst1_numbuffers < config_params.min_numbuffers))
		inst1_numbuffers = config_params.min_numbuffers;
	if ((inst2_numbuffers > 0) &&
	    (inst2_numbuffers < config_params.min_numbuffers))
		inst2_numbuffers = config_params.min_numbuffers;
	if ((inst3_numbuffers > 0) &&
	    (inst3_numbuffers < config_params.min_numbuffers))
		inst3_numbuffers = config_params.min_numbuffers;

	config_params.numbuffers[TI81XXVIN_INSTANCE0_VIDEO] = inst0_numbuffers;
	config_params.numbuffers[TI81XXVIN_INSTANCE1_VIDEO] = inst1_numbuffers;
	config_params.numbuffers[TI81XXVIN_INSTANCE2_VIDEO] = inst2_numbuffers;
	config_params.numbuffers[TI81XXVIN_INSTANCE3_VIDEO] = inst3_numbuffers;


	/* Allocate memory for 4 instance objects */
	for (i = 0; i < TI81XXVIN_CAPTURE_MAX_DEVICES; i++) {
		ti81xxvin_obj.dev[i] =
		    kzalloc(sizeof(*ti81xxvin_obj.dev[i]), GFP_KERNEL);
		/* If memory allocation fails, return error */
		if (!ti81xxvin_obj.dev[i]) {
			free_instance_objects_index = i;
			ret = -ENOMEM;
			goto hdvpss_init_free_instance_objects;
		}
	}
	return ret;

hdvpss_init_free_instance_objects:
	for (j = 0; j < free_instance_objects_index; j++)
		kfree(ti81xxvin_obj.dev[j]);
	return ret;
}

/**
 * ti81xxvin_probe : This function probes the TI81xx capture driver
 * @pdev: platform device pointer
 *
 * This creates device entries by register itself to the V4L2 driver and
 * initializes fields of eainst instance objects
 */
static int ti81xxvin_probe(struct platform_device *pdev)
{
	struct ti81xxvin_subdev_info *subdevdata;
	struct ti81xxvin_config *config;
	int i, j, k, err, t;
	struct i2c_adapter *i2c_adap;
	struct ti81xxvin_instance_obj *inst;
	struct ti81xxvin_buffer_obj *buf_obj;
	struct video_device *vfd;
	int subdev_count;
	int num_capt;
	struct resource *hdvpss_resource;

	ti81xxvin_dev = &pdev->dev;
	err = ti81xxvin_initialize();
	if (err) {
		v4l2_err(ti81xxvin_dev->driver, "Error initializing TI81xx\n");
		/* TODO Deinitialize HDVPSS */
		return err;
	}
	for (i = 0; i < pdev->num_resources; i++) {
		hdvpss_resource = &pdev->resource[i];
		if (hdvpss_resource->flags == IORESOURCE_MEM) {
			ti81xxvin_obj.addr[i] = ioremap_nocache(
				hdvpss_resource->start,
				hdvpss_resource->end - hdvpss_resource->start);
		}
	}
	for (i = 0; i < TI81XXVIN_CAPTURE_MAX_DEVICES; i++) {
		/* Get the pointer to the instance object */
		inst = ti81xxvin_obj.dev[i];
		if (TI81XXVIN_INSTANCE0_VIDEO == i ||
			TI81XXVIN_INSTANCE1_VIDEO == i)
			inst->addr = ti81xxvin_obj.addr[0];
		else
			inst->addr = ti81xxvin_obj.addr[1];
		/* Allocate memory for video device */
		vfd = video_device_alloc();
		if (NULL == vfd) {
			for (j = 0; j < i; j++) {
				inst = ti81xxvin_obj.dev[j];
				video_device_release(inst->video_dev);
			}
			err = -ENOMEM;
			goto hdvpss_dev_alloc_err;
		}

		/* Initialize field of video device */
		*vfd = hdvpss_video_template;
		vfd->v4l2_dev = &ti81xxvin_obj.v4l2_dev;
		vfd->release = video_device_release;
		snprintf(vfd->name, sizeof(vfd->name),
			 "TI81xx_HDVPSSCapture_DRIVER_V%d.%d.%d",
			 (TI81XXVIN_VERSION_CODE >> 16) & 0xff,
			 (TI81XXVIN_VERSION_CODE >> 8) & 0xff,
			 (TI81XXVIN_VERSION_CODE) & 0xff);
		/* Set video_dev to the video device */
		inst->video_dev = vfd;
	}
	t = vps_capt_get_num_cap();
	num_capt = 0;
	for (i = 0; i < t; i++) {
		struct vps_capt_ctrl *cctrl;
		cctrl = vps_capt_get_ctrl(i);
		if (cctrl == NULL) {
			ti81xxvin_dbg(1, debug, "can't get cctrl %d\n", i);
			err = -EINVAL;
			goto hdvpss_dev_alloc_err;
		}
		ti81xxvin_obj.captctrl[ti81xxvin_obj.act_num_capt_ctrl++] =
			cctrl;
	}
	for (j = 0; j < TI81XXVIN_CAPTURE_MAX_DEVICES; j++) {
		inst = ti81xxvin_obj.dev[j];
		inst->instance_id = j;
		inst->captctrl = ti81xxvin_obj.captctrl[j];
		buf_obj = &(inst->buf_obj);
		spin_lock_init(&buf_obj->buf_irqlock);
		mutex_init(&buf_obj->buf_lock);
		/* Initialize prio member of instance object */
		v4l2_prio_init(&inst->prio);
		err = video_register_device(inst->video_dev,
					    VFL_TYPE_GRABBER, j);
		if (err)
			goto probe_out;

		video_set_drvdata(inst->video_dev, inst);

	}


	config = pdev->dev.platform_data;
	subdev_count = config->subdev_count;
	ti81xxvin_obj.sd = kmalloc(sizeof(struct v4l2_subdev *) *subdev_count,
				GFP_KERNEL);
	if (ti81xxvin_obj.sd == NULL) {
		ti81xxvin_err("unable to allocate memory for subdevice"
			" pointers\n");
		err = -ENOMEM;
		goto probe_out;
	}

	err = v4l2_device_register(ti81xxvin_dev, &ti81xxvin_obj.v4l2_dev);
	if (err) {
		v4l2_err(ti81xxvin_dev->driver, "Error registering"
			" v4l2 device\n");
		goto probe_subdev_out;
	}
	for (i = 0; i < 1; i++) {
		subdevdata = &config->subdev_info[i];
		i2c_adap = i2c_get_adapter(subdevdata->i2c_adapter_id);
		ti81xxvin_obj.sd[i] =
			v4l2_i2c_new_subdev_board(&ti81xxvin_obj.v4l2_dev,
						  i2c_adap,
						  &subdevdata->board_info,
						  NULL, 0);

		if (!ti81xxvin_obj.sd[i]) {
			ti81xxvin_err("Error registering v4l2 subdevice\n");
			err = -ENODEV;
			goto probe_subdev_out;
		}
		if (subdevdata->ti81xxvin_select_decoder)
			err = subdevdata->ti81xxvin_select_decoder(
				subdevdata->decoder_id);
		if (err < 0) {
			ti81xxvin_err("Error selecting decoder\n");
			err = -ENODEV;
			goto probe_subdev_out;
		}
		if (subdevdata->ti81xxvin_set_mode)
			err = subdevdata->ti81xxvin_set_mode(
					FVID2_STD_1080P_60);
		if (err < 0) {
			ti81xxvin_err("Error setting mode on decoder\n");
			goto probe_subdev_out;
		}

		v4l2_info(&ti81xxvin_obj.v4l2_dev, "registered sub device %s\n",
			  subdevdata->name);

		if (ti81xxvin_obj.sd[i])
			ti81xxvin_obj.sd[i]->grp_id = 1 << i;
	}
	v4l2_info(&ti81xxvin_obj.v4l2_dev, "TI81xx HDVPSS Capture driver"
		  " initialized\n");

	return 0;

probe_subdev_out:
	/* free sub devices memory */
	kfree(ti81xxvin_obj.sd);

	j = TI81XXVIN_CAPTURE_MAX_DEVICES;
probe_out:
	v4l2_device_unregister(&ti81xxvin_obj.v4l2_dev);
	for (k = 0; k < j; k++) {
		/* Get the pointer to the instance object */
		inst = ti81xxvin_obj.dev[k];
		/* Unregister video device */
		video_unregister_device(inst->video_dev);
	}

hdvpss_dev_alloc_err:
	return err;
}

/**
 * ti81xxvin_remove() - driver remove handler
 * @device: ptr to platform device structure
 *
 * The vidoe device is unregistered
 */
static int ti81xxvin_remove(struct platform_device *device)
{
	int i;
	struct ti81xxvin_instance_obj *inst;

	for (i = 0; i < device->num_resources; i++) {
		if (device->resource[i].flags == IORESOURCE_MEM)
			iounmap(ti81xxvin_obj.addr[i]);
	}
	v4l2_device_unregister(&ti81xxvin_obj.v4l2_dev);

	/* un-register device */
	for (i = 0; i < TI81XXVIN_CAPTURE_MAX_DEVICES; i++) {
		/* Get the pointer to the instance object */
		inst = ti81xxvin_obj.dev[i];
		/* Unregister video device */
		video_unregister_device(inst->video_dev);
	}
	return 0;
}

static struct platform_driver hdvpss_driver = {
	.driver	= {
		.name	= "ti81xxvin",
		.owner	= THIS_MODULE,
		.pm	= NULL,
	},
	.probe	= ti81xxvin_probe,
	.remove = ti81xxvin_remove,
};

/**
 * ti81xxvin_init: initialize the TI81xx capture driver
 *
 * This function registers device and driver to the kernel, requests irq
 * handler and allocates memory
 * for instance objects
 */
static __init int ti81xxvin_init(void)
{
	return platform_driver_probe(&hdvpss_driver, ti81xxvin_probe);
}

/**
 * ti81xxvin_cleanup : This function clean up the TI81xx capture resources
 *
 * This will un-registers device and driver to the kernel, frees
 * requested irq handler and de-allocates memory allocated for instance
 * objects.
 */
static void ti81xxvin_cleanup(void)
{
	int i = 0;

	platform_driver_unregister(&hdvpss_driver);

	kfree(ti81xxvin_obj.sd);
	for (i = 0; i < TI81XXVIN_CAPTURE_MAX_DEVICES; i++)
		kfree(ti81xxvin_obj.dev[i]);
}

/* Function for module initialization and cleanup */
module_init(ti81xxvin_init);
module_exit(ti81xxvin_cleanup);

MODULE_AUTHOR("Hardik Shah(hardik.shah@ti.com");
MODULE_DESCRIPTION("DM81xx Vin Capture driver");
MODULE_LICENSE("GPL");
