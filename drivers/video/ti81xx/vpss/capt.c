/*
 * linux/drivers/cap/ti81xx/vpss/capt.c
 *
 * VPSS graphics driver for TI 81XX
 *
 * Copyright (C) 2011 TI
 * Author: Hardik Shah <hardik.shah@ti.com>
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
#define VPSS_SUBMODULE_NAME "CAPTURE"

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/videodev2.h>

#include "core.h"
#include <mach/capt.h>
#include <linux/vps_capture.h>

static struct list_head cctrl_list;
static int num_captctrl;
static struct platform_device *capture_dev;
static struct vps_payload_info  *capture_payld_info;
static int capture_control(struct vps_capt_ctrl *cctrl,
				u32 cmd, void *cmdargs, void*cmdstatusargs);


static inline struct vps_capt_ctrl *get_capture_ctrl_from_handle(void * handle)
{
	struct vps_capt_ctrl *cctrl;

	list_for_each_entry(cctrl, &cctrl_list, list) {
		if (handle == cctrl->handle)
			return cctrl;

	}
	return NULL;
}
static inline void capture_print_status(struct vps_capt_ctrl *cctrl)
{
	int r;
	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return;

	/* TODO Get Ch Stataus is not working */
	cctrl->cchstatusargs->channelNum = cctrl->capt_num * 64;
	cctrl->cchstatusargs->frameInterval = 16;
	r = vps_fvid2_control(
		 cctrl->handle,
		 IOCTL_VPS_CAPT_GET_CH_STATUS,
		 (struct Vps_CaptChStatus *)cctrl->cchstatus_phy,
		 (struct Vps_CaptChGetStatusArgs *)cctrl->cchstargs_phy);

	if (!r) {
		if (cctrl->cchstatus->isVideoDetected) {
			printk(KERN_INFO "Capture capture detected(%d)",
				cctrl->capt_num);
			printk(KERN_INFO "Output Width detected(%d): %d\n",
				cctrl->capt_num, cctrl->cchstatus->outWidth);
			printk(KERN_INFO "Output height detected(%d): %d\n",
				cctrl->capt_num, cctrl->cchstatus->outHeight);
			printk(KERN_INFO "Dropped frame count(%d): %d\n",
				cctrl->capt_num,
				cctrl->cchstatus->droppedFrameCount);
			printk(KERN_INFO "Caputred frame count(%d): %d\n",
				cctrl->capt_num,
				cctrl->cchstatus->captureFrameCount);
		} else {
			printk(KERN_INFO "Video Not detected(%d)",
				cctrl->capt_num);
		}
	}

}

static int capture_set_storage_fmt(struct vps_capt_ctrl *cctrl,
		struct Vps_CaptStoragePrms *cstorageprms)
{
	int r = 0;

	if (cctrl == NULL)
		return -EINVAL;
	memcpy(cctrl->cstorageprms, cstorageprms,
		sizeof(struct Vps_CaptStoragePrms));
	if ((!cctrl->isstarted) && (cctrl->handle))
		r = vps_fvid2_control(cctrl->handle,
			IOCTL_VPS_CAPT_SET_STORAGE_FMT,
			(struct Vps_CaptStoragePrms *)cctrl->cstorageprms_phy,
			NULL);
	return r;

}

static int capture_reset(struct vps_capt_ctrl *cctrl, u32 cmd)
{
	int r = 0;

	if (cctrl == NULL)
		return -EINVAL;
	if ((!cctrl->isstarted) && (cctrl->handle))
		r = vps_fvid2_control(cctrl->handle, cmd, NULL, NULL);
	return r;
}

static int capture_return_err_frms(struct vps_capt_ctrl *cctrl,
		struct Vps_CaptReturnErrFrms *creturnerrfrms)
{
	int r = 0;

	if (cctrl == NULL)
		return -EINVAL;
	memcpy(cctrl->creturnerrfrms, creturnerrfrms,
		sizeof(struct Vps_CaptReturnErrFrms));
	if ((!cctrl->isstarted) && (cctrl->handle))
		r = vps_fvid2_control(cctrl->handle,
			IOCTL_VPS_CAPT_RETURN_ERR_FRM,
			(struct Vps_CaptReturnErrFrms *)
			cctrl->creturnerrfrms_phy,
			NULL);
	return r;

}

static int capture_one_cb_per_frm(struct vps_capt_ctrl *cctrl,
		struct Vps_CaptReturnErrFrms *conecbperfrm)
{
	int r = 0;

	if (cctrl == NULL)
		return -EINVAL;
	memcpy(cctrl->conecbperfrm, conecbperfrm,
		sizeof(struct Vps_CaptReturnErrFrms));
	if ((!cctrl->isstarted) && (cctrl->handle))
		r = vps_fvid2_control(cctrl->handle,
			IOCTL_VPS_CAPT_ONE_CB_PER_FRM,
			(struct Vps_CaptOneCallBackPerFrm *)
			cctrl->conecbperfrm_phy,
			NULL);
	return r;

}

/* this will be the call back register to the VPSS m3 for the VSYNC isr*/
static int capture_complete_cb(void *handle, void *appdata, void * reserved)
{
	struct vps_capt_ctrl *cctrl = get_capture_ctrl_from_handle(handle);
	struct vps_isr_data *isrd;
	unsigned long flags = 0;


	if (cctrl == NULL)
		return -EINVAL;
	spin_lock_irqsave(&cctrl->buf_irqlock, flags);
	list_for_each_entry(isrd, &cctrl->cb_list, list) {
		if (isrd->isr)
			isrd->isr(isrd->arg);
	}
	spin_unlock_irqrestore(&cctrl->buf_irqlock, flags);
	return 0;
}

static int capture_create(struct vps_capt_ctrl *cctrl)
{
	u32	capinstid, cmd;
	int r = 0, i;

	VPSSDBG("create capture%d\n", cctrl->capt_num);

	cctrl->cbparams->appdata = NULL;
	cctrl->cbparams->errlist = NULL;
	cctrl->cbparams->errcbfnx = NULL;
	cctrl->cbparams->cbfxn = capture_complete_cb;

	for (i = 0; i < 32; i++)
		cctrl->framelist[i]->numframes = 1;
	switch (cctrl->capt_num) {
	case 0:
		capinstid = VPS_CAPT_INST_VIP0_PORTA;
		break;
	case 1:
		capinstid = VPS_CAPT_INST_VIP0_PORTB;
		break;
	case 2:
		capinstid = VPS_CAPT_INST_VIP1_PORTA;
		break;
	case 3:
		capinstid = VPS_CAPT_INST_VIP1_PORTB;
		break;
	default:
		BUG();
	}
	cctrl->handle = vps_fvid2_create(FVID2_VPS_CAPT_VIP_DRV, capinstid,
		(void *)cctrl->ccp_phy, (void *)cctrl->ccs_phy,
		(struct fvid2_cbparams *)cctrl->cbp_phy);

	if (cctrl->handle == NULL) {
		r = -EINVAL;
		return r;
	}
	if (VPS_CAPT_INST_VIP0_PORTA == capinstid ||
		VPS_CAPT_INST_VIP0_PORTB == capinstid)
		cmd = IOCTL_VPS_CAPT_RESET_VIP0;
	else
		cmd = IOCTL_VPS_CAPT_RESET_VIP1;
	r = capture_control(cctrl, cmd, NULL, NULL);
	return r;
}

static int capture_delete(struct vps_capt_ctrl *cctrl)
{
	int r = 0;

	VPSSDBG("delete capture%d\n", cctrl->capt_num);

	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return -EINVAL;

	r = vps_fvid2_delete(cctrl->handle, NULL);
	if (!r)
		cctrl->handle = NULL;
	return r;

}

static int capture_start(struct vps_capt_ctrl *cctrl)
{
	int r = 0;

	VPSSDBG("start capture%d\n", cctrl->capt_num);

	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return -EINVAL;
	if (!cctrl->isstarted) {
		r = vps_fvid2_start(cctrl->handle, NULL);
		if (!r)
			cctrl->isstarted = true;
	}
	return r;

}
static int capture_stop(struct vps_capt_ctrl *cctrl)
{
	int r = 0;

	VPSSDBG("stop capture%d\n", cctrl->capt_num);

	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return -EINVAL;
	if (cctrl->isstarted) {
		r = vps_fvid2_stop(cctrl->handle, NULL);
		if (!r) {
			cctrl->isstarted = false;
			/*capture_print_status(cctrl);*/
		}
	}
	return r;

}

static int capture_queue(struct vps_capt_ctrl *cctrl, int index)
{
	int r = 0;

	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return -EINVAL;

	VPSSDBG("queue capture\n");
#if 0
{
	u32 addr = (u32)cctrl->framelist[i]->frames[0];
	int index;

if (cctrl->scformat == FVID2_SF_PROGRESSIVE)
	VPSSDBG("queue %d capture addr 0x%p",
		index, cctrl->frames[index]->addr[0][0]);
else {
	VPSSDBG("queue %d capture addr 0x%p",
		index, cctrl->frames[index]->addr[0][0]);
	VPSSDBG("queue %d capture addr 0x%p",
		index, cctrl->frames[index]->addr[1][0]);
}
}
#endif
	r = vps_fvid2_queue(cctrl->handle,
			    (struct fvid2_framelist *)cctrl->frmls_phy[index],
			    0);
	return r;


}

static int capture_dequeue(struct vps_capt_ctrl *cctrl, int index)
{
	int r = 0;

	if ((cctrl == NULL) || (cctrl->handle == NULL))
		return -EINVAL;

	VPSSDBG("dequeue video\n");
	r = vps_fvid2_dequeue(cctrl->handle,
			      (struct fvid2_framelist *)cctrl->frmls_phy[index],
			      0, FVID2_TIMEOUT_NONE);
	return r;



}
static int capture_control(struct vps_capt_ctrl *cctrl,
				u32 cmd, void *cmdargs, void*cmdstatusargs)

{
	if (cctrl == NULL)
		return -EINVAL;
	switch (cmd) {
	case IOCTL_VPS_CAPT_SET_STORAGE_FMT:
		return capture_set_storage_fmt(cctrl,
			(struct Vps_CaptStoragePrms *)cmdargs);
	case IOCTL_VPS_CAPT_RETURN_ERR_FRM:
		return capture_return_err_frms(cctrl,
			(struct Vps_CaptReturnErrFrms *)cmdargs);
	case IOCTL_VPS_CAPT_ONE_CB_PER_FRM:
		return capture_one_cb_per_frm(cctrl,
			(struct Vps_CaptReturnErrFrms *)cmdargs);
	case  IOCTL_VPS_CAPT_RESET_VIP0:
	case  IOCTL_VPS_CAPT_RESET_VIP1:
		return capture_reset(cctrl, cmd);
	default:
		BUG();
	}


}

static int capture_set_buffer(struct vps_capt_ctrl *cctrl, u32 addr, u8 idx,
				u32 offset)
{
	struct fvid2_format *dfmt;
	struct fvid2_frame *frame;
	struct fvid2_framelist *framelist;
	u32 scfmt, fm;
	int r = 0;

	VPSSDBG("set buffer\n");
	if (cctrl == NULL)
		return -EINVAL;

	dfmt = &cctrl->fmt;
	scfmt = cctrl->fmt.scanformat;
	frame = cctrl->frames[idx];
	framelist = cctrl->framelist[idx];
	framelist->frames[0] = (struct fvid2_frame *)cctrl->frm_phy[idx];
	framelist->numframes = 1;
	frame->channelnum = cctrl->ccparams->channelNumMap[0][0];
	/*get the field merged flag*/
	fm = cctrl->fmt.fieldmerged[FVID2_YUV_INT_ADDR_IDX];
	switch (cctrl->fmt.dataformat) {
	case FVID2_DF_YUV422I_YUYV:
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_INT_ADDR_IDX] =
					(void *)addr + offset;
		} else {
			/*interlaced display*/
			if (fm) {
				/*field merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] =
					(void *)addr + offset;
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] = (void *)addr
					+ offset +
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX];
			} else {
				/*field non-merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] =
					(void *)addr + offset;
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] =
					(void *)addr + offset + (dfmt->height *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);
			}
		}
	break;
	case FVID2_DF_YUV422SP_UV:
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				[FVID2_YUV_SP_Y_ADDR_IDX] =
					(void *)addr + offset;
			frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)addr +
				offset + (dfmt->height *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);

		} else {
			if (fm) {
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)addr + offset;

				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)addr + offset +
					(dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
					(void *)addr + offset + (dfmt->height *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
					(void *)(addr +	offset +
					((dfmt->height + 1) *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]));
			} else {
			}
		}

		break;
	case FVID2_DF_YUV420SP_UV:
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)addr +
				offset;

			frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)addr +
					offset +
					(dfmt->height *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);

		} else {
			/*interlaced display*/
			if (fm) {
				/*field merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)addr + offset;


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					 [FVID2_YUV_SP_Y_ADDR_IDX] =
						 (void *)addr + offset +
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX];

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
						(void *)addr + offset +
						(dfmt->height *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
						addr +	offset +
						((dfmt->height + 1) *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]);
			} else {
				/*field non-merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
							(void *)addr + offset;

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr +	offset + ((dfmt->height >> 1) *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]));

				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)
					(addr +	offset +
					(((dfmt->height * 3) / 4) *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX]));


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr + offset +
					(((dfmt->height + dfmt->height / 4) *
					dfmt->pitch[FVID2_YUV_INT_ADDR_IDX])));

			}
		}

	break;
	case FVID2_DF_RGB24_888:
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_RGB_ADDR_IDX] = (void *)addr + offset;
		} else {
			/*interlaced display*/
			if (fm) {
				/*field merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_RGB_ADDR_IDX] = (void *)addr
				    + offset;
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_RGB_ADDR_IDX] = (void *)(addr
					+ offset
					+ dfmt->pitch[FVID2_RGB_ADDR_IDX]);
			} else {
				/*field non-merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_RGB_ADDR_IDX] = (void *)addr
				    + offset;
				    frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_RGB_ADDR_IDX] = (void *)(addr
					+ dfmt->height *
					dfmt->pitch[FVID2_RGB_ADDR_IDX]);
			}
		}
	break;
	default:
		BUG();
	}
	if (r)
		VPSSERR("failed to update buffer\n");
	return r;
}

static int capture_check_format(struct vps_capt_ctrl *cctrl,
				      struct fvid2_format *fmt)
{
	int r = 0;

	VPSSDBG("check format\n");
	if (FVID2_DF_YUV422I_YUYV != fmt->dataformat &&
		FVID2_DF_YUV420SP_UV != fmt->dataformat &&
		FVID2_DF_YUV422SP_UV != fmt->dataformat &&
		FVID2_DF_RGB24_888 != fmt->dataformat) {
		VPSSERR("Buffer format (%d) not supported!!\n",
		    fmt->dataformat);
		r = -EINVAL;
	}



	/* Check whether window width/startX is even */
	if (fmt->width & 0x01u)	{
		VPSSERR("width(%d) can't be odd!!\n", fmt->width);
		r = -EINVAL;
	}

	if (FVID2_DF_YUV420SP_UV == fmt->dataformat) {
		if (FVID2_SF_INTERLACED == fmt->scanformat) {
			/* Check whether window height is multiple of 4
			* for YUV420 format in interlaced display mode */
			if (fmt->height & 0x03u) {
				VPSSERR("height(%d)should be multiple"
					"of 4 for YUV420 "
					"format in display mode!!\n",
					fmt->height);
				r = -EINVAL;
		    }
		} else {
			/* Check whether window height/startY is
			even for YUV420 format */
			if (fmt->height & 0x01u) {
				VPSSERR("height(%d) can't be odd for "
					"YUV420 format!!\n",
					fmt->height);
				r = -EINVAL;
		    }
		}
	}
	return r;

}

static int capture_set_format(struct vps_capt_ctrl *cctrl,
				  struct fvid2_format *fmt)
{
	int r = 0;

	if (cctrl == NULL || cctrl->isstarted)
		r = -EINVAL;

	if (!r) {
		memcpy(&cctrl->fmt, fmt, sizeof(struct fvid2_format));
		cctrl->isfmtset = 1;
	}

	if (r)
		VPSSERR("failed to set format\n");

	return r;


}

static int capture_get_format(struct vps_capt_ctrl *cctrl,
				  struct fvid2_format *fmt)
{
	int r = 0;

	if (cctrl == NULL)
		return -EINVAL;

	memcpy(fmt, &cctrl->fmt, sizeof(struct fvid2_format));
	return r;
}

static int capture_apply_changes(struct vps_capt_ctrl *cctrl)
{
	int r = 0;

	return r;
}

static int capture_try_format(struct vps_capt_ctrl *cctrl, u32 width, u32
				  height, u32 df, u32 pitch, u8 merged,
				  struct fvid2_format *fmt)
{
	int r = 0;
	bool fieldmerged;

	if (cctrl == NULL)
		return -EINVAL;

	/*setup the fvid2_format*/
	fmt->dataformat = df;
	fmt->channelnum = 0;
	fmt->height = height;
	fmt->width = width;
	fmt->scanformat = cctrl->scformat;

	if (df == FVID2_DF_YUV420SP_UV)
		fmt->bpp = FVID2_BPP_BITS12;
	else
		fmt->bpp = FVID2_BPP_BITS16;

	switch (df) {
	case FVID2_DF_YUV420SP_UV:
	case FVID2_DF_YUV422SP_UV:
		fmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX] = pitch;
		fmt->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX] = pitch;
		if (cctrl->scformat == FVID2_SF_PROGRESSIVE)
			fieldmerged = 0;
		else
			fieldmerged = 1;

		fmt->fieldmerged[FVID2_YUV_SP_Y_ADDR_IDX] = fieldmerged;
		fmt->fieldmerged[FVID2_YUV_SP_CBCR_ADDR_IDX] = fieldmerged;

		break;
	case FVID2_DF_YUV422I_YUYV:
		fmt->pitch[FVID2_YUV_INT_ADDR_IDX] = pitch;
		if (cctrl->scformat == FVID2_SF_PROGRESSIVE)
			fieldmerged = 0;
		else
			fieldmerged = 1;

		fmt->fieldmerged[FVID2_YUV_INT_ADDR_IDX] = fieldmerged;
		break;


	}
	VPSSDBG("try format %dx%d df %d pitch %d\n",
		width, height, df, fmt->pitch[FVID2_YUV_INT_ADDR_IDX]);

	return r;


}

static inline int frame_index(struct vps_capt_ctrl *cctrl, u32 addr)
{
	int i;
	for (i = 0; i < 32; i++) {
		if (addr == cctrl->frm_phy[i])
			return i;
	}
	return -1;
}


static inline int get_alloc_size(void)
{
	int size = 0;

	size += sizeof(struct Vps_CaptCreateParams);
	size += sizeof(struct Vps_VipPortConfig);
	size += sizeof(struct Vps_VipConfig);
	size += sizeof(struct Vps_CaptCreateStatus);
	size += sizeof(struct Vps_CaptRtParams);

	size += sizeof(struct Vps_CaptChGetStatusArgs);
	size += sizeof(struct Vps_CaptChStatus);
	size += sizeof(struct Vps_CaptFrameSkip);
	size += sizeof(struct Vps_CaptOverFlowStatus);
	size += sizeof(struct Vps_CaptReturnErrFrms);
	size += sizeof(struct Vps_CaptOneCallBackPerFrm);
	size += sizeof(struct Vps_CaptStoragePrms);

	size += sizeof(struct fvid2_cbparams);
	size += sizeof(struct fvid2_framelist) *VIDEO_MAX_FRAME;
	size += sizeof(struct fvid2_frame) *VIDEO_MAX_FRAME;
	size *= VPS_CAPT_INST_MAX;

	return size;
}

static inline void assign_payload_addr(struct vps_capt_ctrl *cctrl,
		struct vps_payload_info *pinfo, u32 *buf_offset)
{
	int i;
	cctrl->ccparams = (struct Vps_CaptCreateParams *)
			setaddr(pinfo, buf_offset, &cctrl->ccp_phy,
				sizeof(struct Vps_CaptCreateParams));
	cctrl->vipcfg = (struct Vps_VipConfig *)
			setaddr(pinfo, buf_offset, &cctrl->vipcfg_phy,
				sizeof(struct Vps_VipConfig));
	cctrl->vipportcfg = (struct Vps_VipPortConfig *)
			setaddr(pinfo, buf_offset, &cctrl->vipportcfg_phy,
				sizeof(struct Vps_VipPortConfig));

	cctrl->ccstatus = (struct Vps_CaptCreateStatus *)
			setaddr(pinfo, buf_offset, &cctrl->ccs_phy,
				sizeof(struct Vps_CaptCreateStatus));

	cctrl->crtparam = (struct Vps_CaptRtParams *)
			setaddr(pinfo, buf_offset, &cctrl->crtp_phy,
				sizeof(struct Vps_CaptRtParams));

	cctrl->cchstatusargs = (struct Vps_CaptChGetStatusArgs *)
			setaddr(pinfo, buf_offset, &cctrl->cchstargs_phy,
				sizeof(struct Vps_CaptChGetStatusArgs));

	cctrl->cchstatus = (struct Vps_CaptChStatus *)
			setaddr(pinfo, buf_offset, &cctrl->cchstatus_phy,
				sizeof(struct Vps_CaptChStatus));

	cctrl->cfrmskip = (struct Vps_CaptFrameSkip *)
			setaddr(pinfo, buf_offset, &cctrl->cfrmskip_phy,
				sizeof(struct Vps_CaptFrameSkip));

	cctrl->covlflowstat = (struct Vps_CaptOverFlowStatus *)
			setaddr(pinfo, buf_offset, &cctrl->covlflowstat_phy,
				sizeof(struct Vps_CaptOverFlowStatus));

	cctrl->creturnerrfrms = (struct Vps_CaptReturnErrFrms *)
			setaddr(pinfo, buf_offset, &cctrl->creturnerrfrms_phy,
				sizeof(struct Vps_CaptReturnErrFrms));

	cctrl->conecbperfrm = (struct Vps_CaptOneCallBackPerFrm *)
			setaddr(pinfo, buf_offset, &cctrl->conecbperfrm_phy,
				sizeof(struct Vps_CaptOneCallBackPerFrm));

	cctrl->cstorageprms = (struct Vps_CaptStoragePrms *)
			setaddr(pinfo, buf_offset, &cctrl->cstorageprms_phy,
				sizeof(struct Vps_CaptStoragePrms));

	cctrl->cbparams = (struct fvid2_cbparams *)
			setaddr(pinfo, buf_offset, &cctrl->cbp_phy,
				sizeof(struct fvid2_cbparams));


	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		cctrl->framelist[i] = (struct fvid2_framelist *)
				setaddr(pinfo, buf_offset, &cctrl->frmls_phy[i],
					((sizeof(struct fvid2_framelist)) *
					VIDEO_MAX_FRAME));
	}
	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		cctrl->frames[i] = (struct fvid2_frame *)
				setaddr(pinfo, buf_offset, &cctrl->frm_phy[i],
					((sizeof(struct fvid2_frame)) *
					VIDEO_MAX_FRAME));
	}
}

int vps_capt_get_num_cap(void)
{
	return num_captctrl;
}
EXPORT_SYMBOL(vps_capt_get_num_cap);

struct vps_capt_ctrl *vps_capt_get_ctrl(int num)
{
	struct vps_capt_ctrl *cctrl;

	list_for_each_entry(cctrl, &cctrl_list, list) {
		if (cctrl->capt_num == num)
			return cctrl;
	}

	return NULL;
}
EXPORT_SYMBOL(vps_capt_get_ctrl);

int vps_capture_register_isr(capt_complete_callback_t cb, void *arg, int idx)
{
	int r = 0;
	struct vps_capt_ctrl *cctrl;
	struct vps_isr_data *isrd, *new;

	cctrl = vps_capt_get_ctrl(idx);
	if (cctrl == NULL)
		return -EINVAL;
	capt_lock(cctrl);
	/*make sure not duplicate */
	list_for_each_entry(isrd, &cctrl->cb_list, list) {
		if ((isrd->isr == cb) && (isrd->arg == arg)) {
			r = -EINVAL;
			goto exit;
		}
	}
	/*add to the list*/
	new = kzalloc(sizeof(*isrd), GFP_KERNEL);
	new->isr = cb;
	new->arg = arg;
	list_add_tail(&new->list, &cctrl->cb_list);
exit:
	capt_unlock(cctrl);
	return r;

}
EXPORT_SYMBOL(vps_capture_register_isr);

int vps_capture_unregister_isr(capt_complete_callback_t cb , void *arg, int idx)
{
	int r = 0;
	struct vps_capt_ctrl *cctrl;
	struct vps_isr_data  *isrd,  *next;

	cctrl = vps_capt_get_ctrl(idx);
	if (cctrl == NULL)
		return -EINVAL;

	capt_lock(cctrl);
	list_for_each_entry_safe(isrd, next, &cctrl->cb_list, list) {
		if ((isrd->arg == arg) && (isrd->isr == cb)) {
			list_del(&isrd->list);
			kfree(isrd);
			break;
		}
	}

	capt_unlock(cctrl);

	return r;

}
EXPORT_SYMBOL(vps_capture_unregister_isr);

void __init vps_fvid2_capt_ctrl_init(struct vps_capt_ctrl *cctrl)
{
	int i;
	/*create paramters*/

	cctrl->ccparams->videoCaptureMode =
		VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
	cctrl->ccparams->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_16BIT;
	cctrl->ccparams->inDataFormat = FVID2_DF_YUV422P;
	cctrl->ccparams->periodicCallbackEnable = 0;
	cctrl->ccparams->numCh = 1;
	cctrl->ccparams->numStream = 1;
	cctrl->ccparams->vipParserInstConfig = NULL;
	cctrl->ccparams->cscConfig = NULL;
	cctrl->ccparams->vipParserPortConfig  = NULL;
	cctrl->ccparams->inScanFormat = FVID2_SF_PROGRESSIVE;


	cctrl->ccparams->outStreamInfo[0].dataFormat  = FVID2_DF_YUV422I_YUYV;
	cctrl->ccparams->outStreamInfo[0].memType = VPS_VPDMA_MT_NONTILEDMEM;
	cctrl->ccparams->outStreamInfo[0].pitch[0] = 1920*2;
	cctrl->ccparams->outStreamInfo[0].maxOutHeight =
		VPS_CAPT_MAX_OUT_HEIGHT_1080_LINES;
	cctrl->ccparams->outStreamInfo[0].scEnable = 0;
	cctrl->ccparams->outStreamInfo[0].subFrameModeEnable = 0;
	/* Don't care as sub-frame mode is not enabled */
	cctrl->ccparams->outStreamInfo[0].numLinesInSubFrame = 0;
	cctrl->ccparams->outStreamInfo[0].subFrameCb = NULL;

	cctrl->ccparams->channelNumMap[0][0] = 64 * cctrl->capt_num;

	for (i = 0 ; i < 32; i++) {
		cctrl->framelist[i]->frames[0] =
			(struct fvid2_frame *)cctrl->frm_phy[i];
		cctrl->frames[i]->fid = 0;
		cctrl->frames[i]->channelnum = 0;
		cctrl->frames[i]->timestamp = 0;
		cctrl->frames[i]->appdata = NULL;
		cctrl->frames[i]->perframecfg = NULL;
		cctrl->frames[i]->blankdata = NULL;
		cctrl->frames[i]->drvdata = NULL;
		cctrl->frames[i]->subframeinfo = NULL;
		cctrl->frames[i]->reserved = NULL;
	}


	/*assign the function pointer*/
	cctrl->create = capture_create;
	cctrl->delete = capture_delete;
	cctrl->start = capture_start;
	cctrl->stop = capture_stop;
	cctrl->queue = capture_queue;
	cctrl->dequeue = capture_dequeue;
	cctrl->set_buffer = capture_set_buffer;
	cctrl->check_format = capture_check_format;
	cctrl->try_format = capture_try_format;
	cctrl->set_format = capture_set_format;
	cctrl->get_format = capture_get_format;
	cctrl->apply_changes = capture_apply_changes;
	cctrl->control = capture_control;

	cctrl->decoder_width = 1920;
	cctrl->decoder_width = 1080;
	cctrl->scformat = FVID2_SF_PROGRESSIVE;

	cctrl->fmt.channelnum = 0;
	cctrl->fmt.width = 1920;
	cctrl->fmt.height = 1080;
	cctrl->fmt.pitch[0] = 1920 * 2;
	cctrl->fmt.pitch[1] = 1920 * 2;
	cctrl->fmt.pitch[2] = 1920 * 2;

	cctrl->fmt.fieldmerged[0] = 0;
	cctrl->fmt.fieldmerged[1] = 0;
	cctrl->fmt.fieldmerged[2] = 0;

	cctrl->fmt.dataformat = FVID2_DF_YUV422P;
	cctrl->fmt.scanformat = FVID2_SF_PROGRESSIVE;
	cctrl->fmt.bpp = 16;
}

static void vps_capt_add_ctrl(struct vps_capt_ctrl *cctrl)
{
	++num_captctrl;
	list_add_tail(&cctrl->list, &cctrl_list);
}

void __exit vps_capt_deinit(struct platform_device *pdev)
{
	struct vps_capt_ctrl *cctrl;


	/*free payload memory*/
	if (capture_payld_info) {
		if (capture_payld_info->vaddr) {
			vps_sbuf_free(capture_payld_info->paddr,
				      capture_payld_info->vaddr,
				      capture_payld_info->size);
		}
		kfree(capture_payld_info);
		capture_payld_info = NULL;
	}
	/*free capture ctrl */
	while (!list_empty(&cctrl_list)) {
		struct vps_isr_data *isrd;

		cctrl = list_first_entry(&cctrl_list,
					 struct vps_capt_ctrl, list);
		/*free all cb list*/
		while (!list_empty(&cctrl->cb_list)) {
			isrd = list_first_entry(&cctrl->cb_list,
						struct vps_isr_data,
						list);
			list_del(&isrd->list);
			kfree(isrd);
		}
		list_del(&cctrl->list);
		kfree(cctrl);
	}
	num_captctrl = 0;
	capture_dev = NULL;
}

int __init vps_capt_init(struct platform_device *pdev)
{
	int i, r;
	struct vps_payload_info *pinfo;
	u32 size = 0;
	u32 offset = 0;
	VPSSDBG("cap init\n");

	INIT_LIST_HEAD(&cctrl_list);

	num_captctrl = 0;
	capture_dev = pdev;
	/*allocate payload info*/
	capture_payld_info = kzalloc(sizeof(struct vps_payload_info),
				GFP_KERNEL);

	if (!capture_payld_info) {
		VPSSERR("allocate payload info failed\n");
		return -ENOMEM;
	}
	pinfo = capture_payld_info;
	/*calcuate size to allocate*/
	size = get_alloc_size();
	pinfo->vaddr = vps_sbuf_alloc(size, &pinfo->paddr);

	if (pinfo->vaddr == NULL) {
		VPSSERR("alloc cap dma buffer failed\n");
		pinfo->paddr = 0;
		r = -ENOMEM;
		goto cleanup;
	}

	pinfo->size = PAGE_ALIGN(size);
	memset(pinfo->vaddr, 0, pinfo->size);

	for (i = 0; i < VPS_CAPT_INST_MAX; i++) {
		struct vps_capt_ctrl *capctrl;
		capctrl = kzalloc(sizeof(*capctrl), GFP_KERNEL);

		if (capctrl == NULL) {
			VPSSERR("failed to allocate cap%d\n", i);
			r = -ENOMEM;
			goto cleanup;
		}
		/*assign the dma buffer*/
		assign_payload_addr(capctrl, pinfo, &offset);
		/*init cap control*/
		capctrl->capt_num = i;
		vps_fvid2_capt_ctrl_init(capctrl);

		vps_capt_add_ctrl(capctrl);
		mutex_init(&capctrl->cmutex);
		spin_lock_init(&capctrl->buf_irqlock);
		INIT_LIST_HEAD(&capctrl->cb_list);
	}
	return 0;
cleanup:
	vps_capt_deinit(pdev);
	return r;

}
