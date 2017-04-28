/*
 * linux/drivers/video/ti81xx/vpss/video.c
 *
 * VPSS video display driver for TI81XX
 *
 * Copyright (C) 2010 TI
 * Author: Yihe Hu <yihehu@ti.com>
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
#define VPSS_SUBMODULE_NAME "VIDEO"

#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/ti81xxscalar.h>
#include <plat/ti81xx-vpss.h>

#include "core.h"
#include "dc.h"

static struct list_head vctrl_list;
static int num_vctrl;
static struct platform_device *video_dev;
static struct vps_payload_info  *video_payload_info;
static struct ti81xxvid_scalarparam scalar_prm;

static inline void video_lock(struct vps_video_ctrl *vctrl)
{
	mutex_lock(&vctrl->vmutex);
}

static inline void video_unlock(struct vps_video_ctrl *vctrl)
{
	mutex_unlock(&vctrl->vmutex);
}


static inline struct vps_video_ctrl *get_video_ctrl_from_handle(void * handle)
{
	struct vps_video_ctrl *vctrl;

	list_for_each_entry(vctrl, &vctrl_list, list) {
		if (handle == vctrl->handle)
			return vctrl;

	}

	return NULL;

}

static inline void video_print_status(struct vps_video_ctrl *vctrl)
{
	int r;
	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return;

	r = vps_fvid2_control(
		 vctrl->handle,
		 IOCTL_VPS_DISP_GET_STATUS,
		 (struct vps_dispstatus *)vctrl->vs_phy,
		 NULL);

	if (!r) {
		VPSSDBG("Number of request queued(%d): %d\n",
			vctrl->idx, vctrl->vstatus->queueCount);
		VPSSDBG("Number of request dequeued(%d): %d\n",
			vctrl->idx, vctrl->vstatus->dequeueCount);
		VPSSDBG("Number of frames displayed(%d): %d\n",
			vctrl->idx, vctrl->vstatus->displayedFrameCount);
		VPSSDBG("Number of frames repeated(%d): %d\n",
			vctrl->idx, vctrl->vstatus->repeatFrameCount);
	}

}

/*determine the input node of the blender*/
static inline int video_get_inputid(struct vps_video_ctrl *vctrl,
					int bid,
					int inputnode)
{
	switch (bid) {
	case VPS_DC_HDMI_BLEND:
		if ((inputnode == VPS_DC_VCOMP) ||
		    (inputnode == VPS_DC_VCOMP_MUX))
			return VPS_DC_CIG_NON_CONSTRAINED_OUTPUT;
		else if (inputnode == VPS_DC_HDCOMP_MUX)
			return VPS_DC_CIG_PIP_OUTPUT;
		else
			return -EINVAL;
		break;
	case VPS_DC_HDCOMP_BLEND:
		if ((inputnode == VPS_DC_VCOMP) ||
		    (inputnode == VPS_DC_VCOMP_MUX)) {
			/*DM813X's HDCOMP is kind of same as TI816X's DVO2*/
			if (v_pdata->cpu == CPU_DM816X)
				return VPS_DC_CIG_CONSTRAINED_OUTPUT;
			else if (v_pdata->cpu == CPU_DM813X)
				return VPS_DC_CIG_NON_CONSTRAINED_OUTPUT;
			else
				return -EINVAL;
		} else if (inputnode == VPS_DC_HDCOMP_MUX)
			return VPS_DC_CIG_PIP_OUTPUT;
		else
			return -EINVAL;
		break;
	case VPS_DC_DVO2_BLEND:
		if ((inputnode == VPS_DC_VCOMP) ||
		    (inputnode == VPS_DC_VCOMP_MUX))
			/*TI816X is different vs TI814X*/
			if (v_pdata->cpu == CPU_DM816X)
				return VPS_DC_CIG_NON_CONSTRAINED_OUTPUT;
			else
				return VPS_DC_CIG_CONSTRAINED_OUTPUT;
		else if (inputnode == VPS_DC_HDCOMP_MUX)
			return VPS_DC_CIG_PIP_OUTPUT;
		else
			return -EINVAL;
		break;
	case VPS_DC_SDVENC_BLEND:
			return VPS_DC_SDVENC_MUX;
		break;
	}
	return -EINVAL;
}

static void video_rt_reset(struct vps_video_ctrl *vctrl)
{
	if (vctrl == NULL)
		return;

	vctrl->framelist->perlistcfg = NULL;
	vctrl->vrtparams->layoutid = NULL;
	vctrl->vrtparams->infrmprms = NULL;
	vctrl->vrtparams->vpdmaposcfg = NULL;

}

/* this will be the call back register to the VPSS m3 for the VSYNC isr*/
static int video_vsync_cb(void *handle, void *appdata, void * reserved)
{
	struct vps_video_ctrl *vctrl = get_video_ctrl_from_handle(handle);
	struct vps_isr_data *isrd;
	if (vctrl == NULL)
		return -EINVAL;

	/*clear all rt flags*/
	video_rt_reset(vctrl);
	/*call all registered function*/
	list_for_each_entry(isrd, &vctrl->cb_list, list) {
		if (isrd->isr)
			isrd->isr(isrd->arg);
	}

	return 0;
}

/*video display create call*/
static int video_create(struct vps_video_ctrl *vctrl,
			enum vps_vpdmamemorytype mtype)
{
	u32				vidinstid;
	int r = 0;
	int i;

	VPSSDBG("create video%d\n", vctrl->idx);

	/*setup the edges by DC*/
	for (i = 0; (i < vctrl->num_edges) && (r == 0); i++) {
		vctrl->nodes[i].isenable = 1;
		r = vps_dc_set_node(vctrl->nodes[i].nodeid,
				vctrl->nodes[i].inputid,
				vctrl->nodes[i].isenable);

	}

	if (r) {
		VPSSERR("Set Video%d Edges failed\n", vctrl->idx);
		return r;
	}
	for (i = 0; (i < vctrl->num_outputs) && (r == 0); i++) {
		vctrl->enodes[i].isenable = 1;
		r = vps_dc_set_node(vctrl->enodes[i].nodeid,
				vctrl->enodes[i].inputid,
				vctrl->enodes[i].isenable);

	}

	if (r) {
		VPSSERR("Set Video%d Edges failed\n", vctrl->idx);
		return r;
	}

	vctrl->cbparams->appdata = NULL;
	vctrl->cbparams->errlist = NULL;
	vctrl->cbparams->errcbfnx = NULL;
	vctrl->cbparams->cbfxn = video_vsync_cb;

	vctrl->vcparams->memtype = mtype;
	/*init the numframe to 1*/
	vctrl->framelist->numframes = 1;

	if (vctrl->idx == 0)
		vidinstid = VPS_DISP_INST_BP0;
	else if (vctrl->idx == 1)
		vidinstid = VPS_DISP_INST_BP1;
	else
		vidinstid = VPS_DISP_INST_SEC1;

	vctrl->handle = vps_fvid2_create(
		FVID2_VPS_DISP_DRV,
		vidinstid,
		(void *)vctrl->vcp_phy,
		(void *)vctrl->vcs_phy,
		(struct fvid2_cbparams *)vctrl->cbp_phy);

	if (vctrl->handle == NULL)
		r = -EINVAL;

	return r;

}

/*video display delete call*/
static int video_delete(struct vps_video_ctrl *vctrl)
{
	int r = 0;
	int i;
	VPSSDBG("delete video%d\n", vctrl->idx);

	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return -EINVAL;


	/*remove the notes */
	for (i = 0; i < vctrl->num_edges; i++) {
		vctrl->nodes[i].isenable = 0;
		r = vps_dc_set_node(vctrl->nodes[i].nodeid,
				vctrl->nodes[i].inputid,
				vctrl->nodes[i].isenable);

	}
	if (r) {
		VPSSERR("Remove Video%d Edges failed\n", vctrl->idx);
		return r;
	}
	/*remove outputs*/
	for (i = 0; i < vctrl->num_outputs; i++) {
		vctrl->enodes[i].isenable = 0;
		r = vps_dc_set_node(vctrl->enodes[i].nodeid,
				vctrl->enodes[i].inputid,
				vctrl->enodes[i].isenable);

	}
	if (r) {
		VPSSERR("Remove Video%d Output failed\n", vctrl->idx);
		return r;
	}

	r = vps_fvid2_delete(vctrl->handle, NULL);
	if (!r)
		vctrl->handle = NULL;
	return r;

}

/*video display start call*/
static int video_start(struct vps_video_ctrl *vctrl)
{
	int r = 0;

	VPSSDBG("start video%d\n", vctrl->idx);

	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return -EINVAL;

	video_lock(vctrl);
	if (!vctrl->isstarted) {
		r = vps_fvid2_start(vctrl->handle, NULL);
		if (!r)
			vctrl->isstarted = true;
	}
	video_unlock(vctrl);
	return r;

}

/*video dislay stop call*/
static int video_stop(struct vps_video_ctrl *vctrl)
{
	int r = 0;

	VPSSDBG("stop video%d\n", vctrl->idx);

	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return -EINVAL;

	video_lock(vctrl);
	if (vctrl->isstarted) {
		r = vps_fvid2_stop(vctrl->handle, NULL);
		if (!r)
			vctrl->isstarted = false;
	}
	video_unlock(vctrl);
	return r;

}
/*video display queue buffer call*/
static int video_queue(struct vps_video_ctrl *vctrl)
{
	int r = 0;

	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return -EINVAL;

	VPSSDBG("queue video\n");
	r = vps_fvid2_queue(vctrl->handle,
			    (struct fvid2_framelist *)vctrl->frmls_phy,
			    0);

	return r;

}
/*video display dequeue buffer call*/
static int video_dequeue(struct vps_video_ctrl *vctrl)
{
	int r = 0;

	if ((vctrl == NULL) || (vctrl->handle == NULL))
		return -EINVAL;

	VPSSDBG("dequeue video\n");
	r = vps_fvid2_dequeue(vctrl->handle,
			      (struct fvid2_framelist *)vctrl->frmls_phy,
			      0,
			      FVID2_TIMEOUT_NONE);
	return r;


}
/*get the venc resolution*/
static int video_get_resolution(struct vps_video_ctrl *vctrl,
				u32 *fwidth, u32 *fheight,
				bool *isprogressive)
{
	VPSSDBG("get resolution.\n");


	vps_dc_get_outpfmt(
		vctrl->enodes[vctrl->num_outputs - 1].nodeid,
		&vctrl->framewidth,
		&vctrl->frameheight,
		&vctrl->scformat,
		DC_BLEND_ID);


	*fwidth = vctrl->framewidth;
	*fheight = vctrl->frameheight;
	*isprogressive = 0;
	if (vctrl->scformat == FVID2_SF_PROGRESSIVE)
		*isprogressive = 1;

	return 0;
}

/*video dispaly buffer call*/
static int video_set_buffer(struct vps_video_ctrl *vctrl, u32 addr, u8 idx)
{
	struct fvid2_format *dfmt;
	struct fvid2_frame *frame;
	u32 scfmt, fm, pitch;
	int r = 0;

	VPSSDBG("set buffer\n");
	if (vctrl == NULL)
		return -EINVAL;

	dfmt = vctrl->fmt;
	scfmt = vctrl->fmt->scanformat;
	frame = vctrl->frames[idx];
	/*get the field merged flag*/
	fm = vctrl->fmt->fieldmerged[FVID2_YUV_INT_ADDR_IDX];
	switch (vctrl->fmt->dataformat) {
	case FVID2_DF_YUV422I_YUYV:
		pitch = dfmt->pitch[FVID2_YUV_INT_ADDR_IDX];
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_INT_ADDR_IDX] = (void *)addr;
			if ((vctrl->caps & VPSS_VID_CAPS_SCALING) &&
						scalar_prm.scalar_enable) {
				frame->addr[FVID2_FRAME_ADDR_IDX] \
					[FVID2_YUV_PL_CR_ADDR_IDX]
					= (void *)(scalar_prm.scoutbuf);
			}
		} else {
			/*interlaced display*/
			if (fm) {
				/*field merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] = (void *)addr;
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] = (void *)(addr
					+ pitch);
			} else {
				/*field non-merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] = (void *)addr;
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
				    [FVID2_YUV_INT_ADDR_IDX] = (void *)(addr +
				    (dfmt->height  * pitch >> 1));
			}
		}
	break;
	case FVID2_DF_YUV422SP_UV:
		pitch = dfmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX];
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)addr;

			frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)(addr +
				(dfmt->height * pitch));

		} else {
			if (fm) {
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)(addr);

				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)(addr + pitch);

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
						(void *)(addr +
							dfmt->height * pitch);

				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
						(void *)(addr +
						((dfmt->height  + 1) * pitch));
			} else {
				/*no field merge*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)addr;
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr +
					    ((dfmt->height >> 1) * pitch));
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)
					(addr + (dfmt->height *	pitch));
				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr + ((dfmt->height +
					 (dfmt->height >> 1)) * pitch));

			}
		}

		break;
	case FVID2_DF_YUV420SP_UV:
		pitch = dfmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX];
		if (scfmt == FVID2_SF_PROGRESSIVE) {
			if ((vctrl->caps & VPSS_VID_CAPS_SCALING) &&
					scalar_prm.scalar_enable) {
				frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)(addr
					+ scalar_prm.yframe_offset);

				frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_CBCR_ADDR_IDX] =
				(void *)(addr +	scalar_prm.yframe_size +
					scalar_prm.chroma_offset);
				frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_PL_CR_ADDR_IDX] =
					(void *)(scalar_prm.scoutbuf);
			} else {
				frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)addr;

				frame->addr[FVID2_FRAME_ADDR_IDX] \
				[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)(addr +
					(dfmt->height * pitch));
			}

		} else {
			/*interlaced display*/
			if (fm) {
				/*field merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
						(void *)addr;


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					 [FVID2_YUV_SP_Y_ADDR_IDX] =
						 (void *)(addr + pitch);

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] =
						(void *)(addr +
							dfmt->height * pitch);


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
						(addr +
						(dfmt->height + 1) * pitch);
			} else {
				/*field non-merged*/
				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] =
							(void *)addr;

				frame->addr[FVID2_FIELD_EVEN_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr +
					    ((dfmt->height >> 1) *	pitch));

				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_Y_ADDR_IDX] = (void *)
					(addr +
					    (((dfmt->height * 3) / 4) * pitch));


				frame->addr[FVID2_FIELD_ODD_ADDR_IDX] \
					[FVID2_YUV_SP_CBCR_ADDR_IDX] = (void *)
					(addr +	(((dfmt->height +
						dfmt->height / 4) * pitch)));

			}
		}

	break;
	}
	vctrl->framelist->frames[0] =
		(struct fvid2_frame *)vctrl->frm_phy[idx];

	if (r)
		VPSSERR("failed to update buffer\n");
	return r;
}

/*format sanity check*/
static int video_check_format(struct vps_video_ctrl *vctrl,
			      struct fvid2_format *fmt)
{
	int r = 0;

	VPSSDBG("check format\n");

	if (fmt->width > vctrl->framewidth) {
		VPSSERR("Width(%d) greater than frame width(%d)\n",
			fmt->width, vctrl->framewidth);
		r = -EINVAL;

	}
	if (fmt->height > vctrl->frameheight) {
		VPSSERR("Height(%d) greater than frame height(%d)\n",
			fmt->height, vctrl->frameheight);
		r = -EINVAL;

	}

	if (FVID2_DF_YUV422I_YUYV != fmt->dataformat &&
		FVID2_DF_YUV420SP_UV != fmt->dataformat &&
		FVID2_DF_YUV422SP_UV != fmt->dataformat) {
		VPSSERR("Buffer format (%d) not supported!!\n",
		    fmt->dataformat);
		r = -EINVAL;
	}
	/*If scaling is enabled from user App */
	if (!(scalar_prm.scalar_enable  &&
		(vctrl->caps & VPSS_VID_CAPS_SCALING))) {
		if (FVID2_DF_YUV420SP_UV == fmt->dataformat ||
			FVID2_DF_YUV422SP_UV == fmt->dataformat) {
			if (((fmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX] <
				(fmt->width))) ||
				(fmt->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX] <
				(fmt->width))) {
				VPSSERR("Y/UV Pitch (%d/%d) less than Width"
					"(%d) in bytes!!\n",
				fmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX],
				fmt->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX],
				(fmt->width));
				r = -EINVAL;
			}
		} else if (FVID2_DF_YUV422I_YUYV == fmt->dataformat) {
			if (fmt->pitch[FVID2_YUV_INT_ADDR_IDX] <
				(fmt->width * 2u)) {
				VPSSERR("pitch (%d) less than width \
						(%d) in bytes!!\n",
				fmt->pitch[FVID2_YUV_INT_ADDR_IDX],
				(fmt->width * 2u));
				r = -EINVAL;
			}
		}
	}
	/* Check whether window width is even */
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
					"format in interlaced display mode!!\n",
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
/*video try format */
static int video_try_format(struct vps_video_ctrl *vctrl, u32 width,
			    u32 height, u32 df, u32 pitch, u8 merged,
			    struct fvid2_format *fmt)
{
	int r = 0;
	bool fieldmerged;

	if (vctrl == NULL)
		return -EINVAL;

	/*setup the fvid2_format*/
	fmt->dataformat = df;
	fmt->channelnum = 0;
	fmt->height = height;
	fmt->width = width;
	fmt->scanformat = vctrl->scformat;

	if (df == FVID2_DF_YUV420SP_UV)
		fmt->bpp = FVID2_BPP_BITS12;
	else
		fmt->bpp = FVID2_BPP_BITS16;

	switch (df) {
	case FVID2_DF_YUV420SP_UV:
	case FVID2_DF_YUV422SP_UV:
		fmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX] = pitch;
		fmt->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX] = pitch;
		if (vctrl->scformat == FVID2_SF_PROGRESSIVE)
			fieldmerged = 0;
		else
			fieldmerged = merged;

		fmt->fieldmerged[FVID2_YUV_SP_Y_ADDR_IDX] = fieldmerged;
		fmt->fieldmerged[FVID2_YUV_SP_CBCR_ADDR_IDX] = fieldmerged;

		VPSSDBG("try format %dx%d df %d pitch %d %d fidmerged %d\n",
			width, height, df,
			fmt->pitch[FVID2_YUV_SP_Y_ADDR_IDX],
			fmt->pitch[FVID2_YUV_SP_CBCR_ADDR_IDX],
			fieldmerged);

		break;
	case FVID2_DF_YUV422I_YUYV:
		fmt->pitch[FVID2_YUV_INT_ADDR_IDX] = pitch;
		if (vctrl->scformat == FVID2_SF_PROGRESSIVE)
			fieldmerged = 0;
		else
			fieldmerged = merged;

		fmt->fieldmerged[FVID2_YUV_INT_ADDR_IDX] = fieldmerged;
		VPSSDBG("try format %dx%d df %d pitch %d fidmerged %d\n",
			width, height, df,
			fmt->pitch[FVID2_YUV_INT_ADDR_IDX],
			fieldmerged);

		break;


	}

	return r;


}

/*video set format call*/
static int video_set_format(struct vps_video_ctrl *vctrl,
			    struct fvid2_format *fmt)
{
	int r = 0;

	if (vctrl == NULL)
		return -EINVAL;

	memcpy(vctrl->fmt, fmt, sizeof(struct fvid2_format));

	if ((!vctrl->isstarted) && (vctrl->handle))
		r = vps_fvid2_setformat(vctrl->handle,
			(struct fvid2_format *)vctrl->fmt_phy);

	if (r)
		VPSSERR("failed to set format\n");

	return r;


}
/*video display get format call*/
static int video_get_format(struct vps_video_ctrl *vctrl,
			    struct fvid2_format *fmt)
{
	int r = 0;

	if (vctrl == NULL)
		return -EINVAL;

	memcpy(fmt, vctrl->fmt, sizeof(struct fvid2_format));
	return r;
}
/*video display set runtime position call*/
static int video_set_rtpos(struct vps_video_ctrl *vctrl, u32 posx, u32 posy)
{

	VPSSDBG("set position\n");
	if ((vctrl == NULL) || (!(vctrl->caps & VPSS_VID_CAPS_POSITIONING)))
		return -EINVAL;

	/*This need be even number*/
	vctrl->vdmaposcfg->startx = (posx & ~1);
	vctrl->vdmaposcfg->starty = (posy & ~1);

	vctrl->vrtparams->vpdmaposcfg = (struct vps_posconfig *)
					vctrl->vdpc_phy;

	vctrl->framelist->perlistcfg = (void *)
					vctrl->vrt_phy;

	return 0;
}
/*video display set runtime crop call*/
static int video_set_rtcrop(struct vps_video_ctrl *vctrl, u32 posx, u32 posy,
			    u32 pitch, u32 w, u32 h)
{
	VPSSDBG("Set crop\n");

	if ((vctrl == NULL) || (!(vctrl->caps & VPSS_VID_CAPS_CROPING)))
		return -EINVAL;

	vctrl->vfrmprm->width = w;
	vctrl->vfrmprm->height = h;
	/*for 420SP format, height need be multiple of 4
		when interlaced output*/
	if ((vctrl->fmt->dataformat == FVID2_DF_YUV420SP_UV) &&
		(vctrl->scformat == FVID2_SF_INTERLACED))
		vctrl->vfrmprm->height &= ~3;
	vctrl->vfrmprm->memtype = vctrl->vcparams->memtype;
	vctrl->vfrmprm->pitch[0] = pitch;
	vctrl->vfrmprm->pitch[1] = pitch;
	vctrl->vfrmprm->pitch[2] = pitch;
	vctrl->vfrmprm->dataFormat = vctrl->fmt->dataformat;

	vctrl->vrtparams->infrmprms = (struct vps_frameparams *)
					vctrl->vfp_phy;

	vctrl->framelist->perlistcfg = (void *)
					vctrl->vrt_phy;

	return 0;
}

static int video_set_scparams(void *scinfo)
{
	struct ti81xxvid_scalarparam *scprm;
	VPSSDBG("Populate scalar params\n");

	scprm = (struct ti81xxvid_scalarparam *)scinfo;
	scalar_prm.yframe_offset =  scprm->yframe_offset;
	scalar_prm.yoffset_flag = scprm->yoffset_flag;
	scalar_prm.yframe_size =  scprm->yframe_size;
	scalar_prm.chroma_offset = scprm->chroma_offset;
	scalar_prm.scoutbuf = scprm->scoutbuf;
	scalar_prm.scalar_enable = scprm->scalar_enable;
	scalar_prm.inframe_width = scprm->inframe_width;
	scalar_prm.inframe_height = scprm->inframe_height;
	VPSSDBG("scalar configs %d\t%d\t%d\t%d\n", scalar_prm.yframe_offset,
	scalar_prm.yoffset_flag, scalar_prm.yframe_size,
	scalar_prm.chroma_offset);
	return 0;
}

static int video_set_deiparams(struct vps_video_ctrl *vctrl,
			struct vps_dei_disp_params *deiparams,
			u32 w, u32 h)
{
	int r;
	VPSSDBG("Set dei/scalar config params\n");

	vctrl->vdeicrpcfg->cropstartx = deiparams->deisccropcfg->cropstartx;
	vctrl->vdeicrpcfg->cropstarty = deiparams->deisccropcfg->cropstarty;
	vctrl->vdeiprm->sctarwidth = w;
	vctrl->vdeiprm->sctarheight = h;
	vctrl->vdeiprm->fmt.height = scalar_prm.inframe_height;
	vctrl->vdeiprm->fmt.width = scalar_prm.inframe_width;
	vctrl->vdeiprm->sccfg.bypass = !scalar_prm.scalar_enable;
	vctrl->vdeiprm->deicfg = NULL;
	vctrl->vdeiprm->deihqcfg = NULL;
	vctrl->vdeiprm->deisccropcfg = (struct vps_cropconfig *)
					vctrl->vdeicr_phy;
	r = vps_fvid2_control(
		vctrl->handle,
		IOCTL_VPS_DEI_DISP_SET_PARAMS,
		(struct vps_dei_disp_params *)vctrl->vdei_phy,
		NULL);
	if (r)
		VPSSERR("set dei/scalar failed\n");
	return r;
}

/*video display get color call*/
static int video_get_color(struct vps_video_ctrl *vctrl,
	struct vps_dccigrtconfig *color)
{
	int r;

	VPSSDBG("get color\n");

	if ((vctrl == NULL) || (!(vctrl->caps & VPSS_VID_CAPS_COLOR)) ||
		(color == NULL))
		return -EINVAL;


	color->nodeid = vctrl->enodes[0].inputid;
	r = vps_dc_get_color(color);
	if (r)
		VPSSERR("Get video%d color failed\n", vctrl->idx);
	return r;
}
/*video display set color call*/
static int video_set_color(struct vps_video_ctrl *vctrl,
	struct vps_dccigrtconfig *color)
{
	int r = 0;
	int i;
	VPSSDBG("set color\n");

	if ((vctrl == NULL) || (!(vctrl->caps & VPSS_VID_CAPS_COLOR)) ||
		(color == NULL))
		return -EINVAL;


	for (i = 0; i < vctrl->num_outputs && !r; i++) {
		color->nodeid = vctrl->enodes[i].inputid;
		/*call DC function to setup the color and alpha value*/
		r = vps_dc_set_color(color);
	}
	if (r)
		VPSSERR("Set video%d color failed\n", vctrl->idx);
	return r;
}
/*reserved call*/
static int video_apply_changes(struct vps_video_ctrl *vctrl)
{
	int r = 0;

	return r;
}
/*S********************SYSFS**************************************************/
/*background call, reserved*/
static ssize_t video_default_color_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	return 0;
}
static ssize_t video_default_color_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int r;
	r = size;
	return r;
}

/*sysfs entry for showing transparency type*/
static ssize_t video_trans_key_type_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	int r;
	struct vps_dccigrtconfig color;

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	video_unlock(vctrl);

	if (!r)
		return snprintf(buf, PAGE_SIZE, "%d\n", color.mask);
	else
		return -EINVAL;
}
/*sysfs entry for storing transparency type*/
static ssize_t video_trans_key_type_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int r = 0;
	u32 mask;
	struct vps_dccigrtconfig color;

	mask = simple_strtoul(buf, NULL, 10);
	if (mask >= VPS_DC_CIG_TM_MASK_3_LSB)
		mask = VPS_DC_CIG_TM_MASK_3_LSB;

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	if (!r) {
		color.mask = mask;
		r = vctrl->set_color(vctrl, &color);
	}
	video_unlock(vctrl);
	if (!r)
		r = size;

	return r;

}
/*sysfs entry for showing color key*/
static ssize_t video_trans_key_value_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	u32 rgb;
	int r;
	struct vps_dccigrtconfig color;
	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	video_unlock(vctrl);

	if (!r) {
		rgb = ((color.transcolor.r & 0xFF) << 16) |
			((color.transcolor.g & 0xFF) << 8) |
			(color.transcolor.b & 0xFF);
		return snprintf(buf, PAGE_SIZE, "%x\n", rgb);
	} else
		return -EINVAL;
}
/*sysfs entry for storing color key*/
static ssize_t video_trans_key_value_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int r = 0;
	u32 rgb;
	struct vps_dccigrtconfig color;

	rgb = simple_strtoul(buf, NULL, 16);


	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	if (!r) {

		color.transcolor.r = (rgb >> 16) & 0xFF;
		color.transcolor.g = (rgb >> 8) & 0xFF;
		color.transcolor.b = (rgb >> 0) & 0xFF;

		r = vctrl->set_color(vctrl, &color);
	}
	video_unlock(vctrl);
	if (!r)
		r = size;
	return r;
}
/*sysfs entry for showing transparency status*/
static ssize_t video_trans_key_enabled_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	int r;
	struct vps_dccigrtconfig color;
	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	video_unlock(vctrl);
	if (r)
		return -EINVAL;
	else
		return snprintf(buf, PAGE_SIZE, "%d\n", color.transparency);
}
/*sysfs entry for storing transparency status*/
static ssize_t video_trans_key_enabled_store(struct vps_video_ctrl *vctrl,
				const char *buf, size_t size)
{
	int r = 0;
	int enable;
	struct vps_dccigrtconfig color;

	if (sscanf(buf, "%d", &enable) != 1)
		return -EINVAL;

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	if (!r) {
		color.transparency = enable ? 1 : 0;
		r = vctrl->set_color(vctrl, &color);
	}
	video_unlock(vctrl);
	if (!r)
		r = size;

	return r;
}
/*sysfs entry for show blending status*/
static ssize_t video_alpha_blending_enabled_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	int r;
	struct vps_dccigrtconfig color;

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	video_unlock(vctrl);

	if (r)
		return -EINVAL;
	else
		return snprintf(buf, PAGE_SIZE, "%d\n", color.alphablending);

}
/*sysfs entry for storing blending status*/
static ssize_t video_alpha_blending_enabled_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int enable;
	int r = 0;
	struct vps_dccigrtconfig color;


	if (sscanf(buf, "%d", &enable) != 1)
		return -EINVAL;

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	if (!r) {
		color.alphablending = enable ? 1 : 0;
		r = vctrl->set_color(vctrl, &color);
	}
	video_unlock(vctrl);
	if (!r)
		r = size;


	return r;
}
/*sysfs entry for showing alpha value*/
static ssize_t video_global_alpha_show(struct vps_video_ctrl *vctrl,
					char *buf)
{
	int r;
	struct vps_dccigrtconfig color;
	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	video_unlock(vctrl);
	if (r)
		return -EINVAL;
	else
		return snprintf(buf, PAGE_SIZE, "%d\n", color.alphavalue);
}
/*sysfs entry for storing alpha value*/
static ssize_t video_global_alpha_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int r = 0;
	u8 alpha;
	struct vps_dccigrtconfig color;

	alpha = simple_strtoul(buf, NULL, 10);

	video_lock(vctrl);
	r = vctrl->get_color(vctrl, &color);
	if (!r) {
		color.alphavalue = alpha;
		r = vctrl->set_color(vctrl, &color);
	}
	video_unlock(vctrl);
	if (!r)
		r = size;
	return r;
}


/*show current video enabled status*/
static ssize_t video_enabled_show(struct vps_video_ctrl *vctrl,
				char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", vctrl->isstarted);
}
/*enable the current video*/
static ssize_t video_enabled_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	bool enabled;
	int r;
	if (vctrl->handle == NULL) {
		VPSSERR("please open /dev/video%d node first.\n",
			vctrl->idx + 1);
		return -EINVAL;
	}

	enabled = simple_strtoul(buf, NULL, 10);
	if (vctrl->isstarted == enabled)
		return size;

	video_lock(vctrl);
	if (enabled)
		r = vctrl->start(vctrl);
	else
		r = vctrl->stop(vctrl);
	video_unlock(vctrl);

	if (r)
		return r;

	return size;
}


/*show how many blender are connecting to this video*/
static ssize_t video_nodes_show(struct vps_video_ctrl *vctrl,
				char *buf)
{
	int i, r = 0;
	int l = 0;
	char name[10];

	vps_dc_get_node_name(vctrl->nodes[0].nodeid, name);
	if (vctrl->num_edges)
		l += snprintf(buf + l, PAGE_SIZE - l, "%s", name);
	else
		return snprintf(buf, PAGE_SIZE, " ");

	for (i = 0; i < vctrl->num_outputs; i++)  {
		r = vps_dc_get_node_name(vctrl->enodes[i].nodeid, name);
		if (i == 0)
			l += snprintf(buf + l, PAGE_SIZE - l, ":%s", name);
		else
			l += snprintf(buf + l, PAGE_SIZE - l, ",%s", name);
	}
	l += snprintf(buf + l, PAGE_SIZE - l, "\n");

	return l;

}
/*set the enodes for video in the format edge,venc:edge,venc*/
static ssize_t video_nodes_store(struct vps_video_ctrl *vctrl,
				      const char *buf, size_t size)
{
	int r = 0;
	int total = 0;
	int i;
	int idx = 0;
	u8 tiedvenc = 0;
	char *input = (char *)buf, *this_opt;
	/*this is store the edege without blender lever*/
	u8 numedges = 0;
	int nodeid;
	struct vps_dcnodeinput nodes[VPS_DISPLAY_MAX_EDGES];
	/*this is for the blender*/
	struct vps_dcnodeinput enodes[VPS_DC_MAX_VENC - 1];
	struct vps_dcvencinfo vinfo;

	if (vctrl->isstarted) {
		VPSSERR("please stop video%d before continue.\n",
			vctrl->idx);
		return -EINVAL;
	}

	video_lock(vctrl);

	/*get the edge path first*/
	this_opt = strsep(&input, ":");
	if (vps_dc_get_id(this_opt, &nodeid, DC_NODE_ID)) {
		VPSSERR("(%d): wrong paths\n", vctrl->idx);
		r = -EINVAL;
		goto exit;
	}
	if ((vctrl->idx == 2) && (nodeid != VPS_DC_SDVENC_MUX)) {
		VPSSERR("(%d): can not connect other nodes except"
				" sdvencmux only\n", vctrl->idx);
		r = -EINVAL;
		goto exit;
	}


	switch (nodeid) {
	case VPS_DC_VCOMP:
		nodes[0].nodeid = VPS_DC_VCOMP;
		nodes[0].inputid = VPS_DC_MAIN_INPUT_PATH;
		numedges = 1;
		break;
	case VPS_DC_VCOMP_MUX:
		nodes[0].nodeid = VPS_DC_VCOMP_MUX;
		if (vctrl->idx == 0)
			nodes[0].inputid = VPS_DC_BP0_INPUT_PATH;
		else if (vctrl->idx == 1)
			nodes[0].inputid = VPS_DC_BP1_INPUT_PATH;

		nodes[1].nodeid = VPS_DC_VCOMP;
		nodes[1].inputid = VPS_DC_VCOMP_MUX;
		numedges = 2;
		break;
	case VPS_DC_HDCOMP_MUX:
		nodes[0].nodeid = VPS_DC_HDCOMP_MUX;
		if (vctrl->idx == 0)
			nodes[0].inputid = VPS_DC_BP0_INPUT_PATH;
		else if (vctrl->idx == 1)
			nodes[0].inputid = VPS_DC_BP1_INPUT_PATH;

		nodes[1].nodeid = VPS_DC_CIG_PIP_INPUT;
		nodes[1].inputid = VPS_DC_HDCOMP_MUX;
		numedges = 2;
		break;
	case VPS_DC_SDVENC_MUX:
		nodes[0].nodeid = VPS_DC_SDVENC_MUX;
		if (vctrl->idx == 0)
			nodes[0].inputid = VPS_DC_BP0_INPUT_PATH;
		else if (vctrl->idx == 1)
			nodes[0].inputid = VPS_DC_BP1_INPUT_PATH;
		else if (vctrl->idx == 2)
			nodes[0].inputid = VPS_DC_SEC1_INPUT_PATH;

		numedges = 1;
	}

	/*get the output venc information, could be multiple output*/
	if ((input == NULL) || (!(strcmp(input, "\0")))) {
		r = -EINVAL;
		VPSSERR("(%d)- wrong node information\n", vctrl->idx);
		goto exit;
	}
	/*parse each end note*/
	while (!r && (this_opt = strsep(&input, ",")) != NULL) {
		int nid, inputid;

		if (vps_dc_get_id(this_opt, &nid, DC_NODE_ID)) {
			VPSSERR("(%d)- failed to parse node name %s\n",
				vctrl->idx, this_opt);
			r = -EINVAL;
			goto exit;

		}

		vps_dc_get_id(this_opt,
			      &vinfo.modeinfo[idx].vencid,
			      DC_VENC_ID);
		enodes[idx].nodeid = nid;
		inputid = video_get_inputid(vctrl, nid, nodeid);
		if (inputid < 0) {
			VPSSERR("(%d) - invalid node %s",
				vctrl->idx,
				this_opt);
			r = -EINVAL;
			goto exit;
		}
		enodes[idx].inputid = inputid;
		VPSSDBG("(%d)- s: %d e:%d vid:%d\n",
			vctrl->idx,
			enodes[idx].inputid,
			enodes[idx].nodeid,
			vinfo.modeinfo[idx].vencid);
		idx++;
		if (input == NULL)
			break;
	}
	total = idx;
	/*the total is not match what parsed in the string*/
	if (idx != total) {
		r = -EINVAL;
		VPSSERR("(%d)- node number not match.\n",
			vctrl->idx);
		goto exit;
	}

	vinfo.numvencs = total;
	for (i = 0; i < total; i++)
		tiedvenc |= vinfo.modeinfo[i].vencid;


	/*tied venc check*/
	if (total == 1)
		tiedvenc = 0;
	else {
		u8 tvencs;
		vps_dc_get_tiedvenc(&tvencs);
		if (tiedvenc & (~tvencs)) {
			VPSSERR("(%d)- nodes not match tied vencs\n",
				vctrl->idx);
			r = -EINVAL;
			goto exit;
		}
	}

	/*make sure the VENCs are already running before continue*/
	vinfo.numvencs = total;
	r = vps_dc_get_vencinfo(&vinfo);
	if (r)
		goto exit;

	for (i = 0; i < vinfo.numvencs; i++) {
		if (vinfo.modeinfo[i].isvencrunning == 0) {
			r = -EINVAL;
			VPSSERR("(%d)- venc %d not running.\n",
				vctrl->idx,
				vinfo.modeinfo[i].vencid);
			goto exit;
		}
	}
	/*should we reconfig the display path or not*/
/*	if (nodeid != vctrl->nodes[0].nodeid) */
	{
		/*disable old first*/
		for (i = 0; (i < vctrl->num_edges) && (r == 0); i++) {
			vctrl->nodes[i].isenable = 0;
			r = vps_dc_set_node(vctrl->nodes[i].nodeid,
					vctrl->nodes[i].inputid,
					vctrl->nodes[i].isenable);
		}

		if (r) {
			r = -EINVAL;
			VPSSERR("(%d): failed to disable old path\n",
				vctrl->idx);
			goto exit;
		}

		/*enable the new path*/
		for (i = 0; (i < numedges) && (r == 0); i++) {
			nodes[i].isenable = 1;
			r = vps_dc_set_node(nodes[i].nodeid,
					nodes[i].inputid,
					nodes[i].isenable);
		}
		if (r) {
			r = -EINVAL;
			VPSSERR("(%d): failed to enable new path\n",
				vctrl->idx);
			goto exit;
		}
		/*store back to the local structure*/
		vctrl->num_edges = numedges;
		for (i = 0; i < vctrl->num_edges; i++)
			vctrl->nodes[i] = nodes[i];
	}

	/*disable the exit nodes*/
	for (i = 0; (i < vctrl->num_outputs) && (r == 0); i++) {
		vctrl->enodes[i].isenable = 0;
		r =  vps_dc_set_node(vctrl->enodes[i].nodeid,
				     vctrl->enodes[i].inputid,
				     vctrl->enodes[i].isenable);
	}
	if (r) {
		r = -EINVAL;
		VPSSERR("(%d)- failed to disable old output nodes\n",
			vctrl->idx);
		goto exit;
	}
	/*enable the new nodes*/
	for (i = 0; (i < total) && (r == 0); i++) {
		enodes[i].isenable = 1;
		r = vps_dc_set_node(enodes[i].nodeid,
				    enodes[i].inputid,
				    enodes[i].isenable);
	}
	if (r) {
		r = -EINVAL;
		VPSSERR("(%d)- failed to enable new output nodes\n",
			vctrl->idx);
		goto exit;
	}
	/*update new nodes into local database*/
	vctrl->num_outputs = total;
	for (i = 0; i < vctrl->num_outputs; i++)
		vctrl->enodes[i] = enodes[i];
	VPSSDBG("(%d)- numedge :%d\n",
		vctrl->idx, vctrl->num_outputs);
	r = size;

exit:
	video_unlock(vctrl);
	return r;
}

struct video_attribute {
	struct attribute attr;
	ssize_t (*show)(struct vps_video_ctrl *, char *);
	ssize_t (*store)(struct vps_video_ctrl *, const char *, size_t);
};
#define VIDEO_ATTR(_name, _mode, _show, _store) \
	struct video_attribute video_attr_##_name = \
	__ATTR(_name, _mode, _show, _store)

static VIDEO_ATTR(default_color, S_IRUGO | S_IWUSR,
	video_default_color_show, video_default_color_store);


static VIDEO_ATTR(trans_key_type, S_IRUGO | S_IWUSR,
	video_trans_key_type_show, video_trans_key_type_store);

static VIDEO_ATTR(trans_key_value, S_IRUGO | S_IWUSR,
	video_trans_key_value_show, video_trans_key_value_store);

static VIDEO_ATTR(trans_key_enabled, S_IRUGO | S_IWUSR,
	video_trans_key_enabled_show, video_trans_key_enabled_store);


static VIDEO_ATTR(alpha_blending_enabled, S_IRUGO | S_IWUSR,
	video_alpha_blending_enabled_show, video_alpha_blending_enabled_store);

static VIDEO_ATTR(global_alpha, S_IRUGO | S_IWUSR,
	video_global_alpha_show, video_global_alpha_store);

static VIDEO_ATTR(enabled, S_IRUGO | S_IWUSR,
	video_enabled_show, video_enabled_store);

static VIDEO_ATTR(nodes, S_IRUGO | S_IWUSR,
	video_nodes_show, video_nodes_store);

static struct attribute *video_sysfs_attrs[] = {
	&video_attr_enabled.attr,
	&video_attr_nodes.attr,
	&video_attr_default_color.attr,
	&video_attr_alpha_blending_enabled.attr,
	&video_attr_global_alpha.attr,
	&video_attr_trans_key_enabled.attr,
	&video_attr_trans_key_value.attr,
	&video_attr_trans_key_type.attr,
	NULL
};

static ssize_t video_attr_show(struct kobject *kobj,
				  struct attribute *attr,
				  char *buf)
{
	struct vps_video_ctrl *vctrl;
	struct video_attribute *vid_attr;

	vctrl = container_of(kobj, struct vps_video_ctrl, kobj);
	vid_attr = container_of(attr, struct video_attribute, attr);

	if (!vid_attr->show)
		return -ENOENT;

	return vid_attr->show(vctrl, buf);
}

static ssize_t video_attr_store(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t size)
{
	struct vps_video_ctrl *vctrl;
	struct video_attribute *vid_attr;

	vctrl = container_of(kobj, struct vps_video_ctrl, kobj);
	vid_attr = container_of(attr, struct video_attribute, attr);

	if (!vid_attr->store)
		return -ENOENT;

	return vid_attr->store(vctrl, buf, size);
}


static const struct sysfs_ops video_sysfs_ops = {
	.show = video_attr_show,
	.store = video_attr_store,
};

static struct kobj_type video_ktype = {
	.sysfs_ops = &video_sysfs_ops,
	.default_attrs = video_sysfs_attrs,
};

static int video_create_sysfs(struct vps_video_ctrl *vctrl)
{
	int r;
	if (vctrl == NULL)
		return -EINVAL;

	r = kobject_init_and_add(&vctrl->kobj,
				 &video_ktype,
				 &video_dev->dev.kobj,
				 "video%d",
				 vctrl->idx);
	if (r)
		VPSSERR("failed to create video%d sysfs file.\n",
			vctrl->idx);
	else
		vctrl->sysfs = true;
	return r;

}

static void video_remove_sysfs(struct vps_video_ctrl *vctrl)
{
	if (vctrl->sysfs) {
		kobject_del(&vctrl->kobj);
		kobject_put(&vctrl->kobj);
		vctrl->sysfs = false;
	}
}
/*E*******************SYSFS Function*********************************/


int vps_video_register_isr(vsync_callback_t cb, void *arg, int idx)
{
	int r = 0;
	struct vps_video_ctrl *vctrl;
	struct vps_isr_data *isrd, *new;

	vctrl = vps_video_get_ctrl(idx);
	if (vctrl == NULL)
		return -EINVAL;

	video_lock(vctrl);
	/*make sure not duplicate */
	list_for_each_entry(isrd, &vctrl->cb_list, list) {
		if ((isrd->isr == cb) && (isrd->arg == arg)) {
			r = -EINVAL;
			goto exit;
		}
	}
	/*add to the list*/
	new = kzalloc(sizeof(*isrd), GFP_KERNEL);
	new->isr = cb;
	new->arg = arg;
	list_add_tail(&new->list, &vctrl->cb_list);

exit:
	video_unlock(vctrl);
	return r;

}
EXPORT_SYMBOL(vps_video_register_isr);

int vps_video_unregister_isr(vsync_callback_t cb , void *arg, int idx)
{
	int r = 0;
	struct vps_video_ctrl *vctrl;
	struct vps_isr_data  *isrd,  *next;

	vctrl = vps_video_get_ctrl(idx);
	if (vctrl == NULL)
		return -EINVAL;

	video_lock(vctrl);
	list_for_each_entry_safe(isrd,
				 next,
				 &vctrl->cb_list,
				 list) {
		if ((isrd->arg == arg) && (isrd->isr == cb)) {
			list_del(&isrd->list);
			kfree(isrd);
			break;
		}
	}

	video_unlock(vctrl);

	return r;

}
EXPORT_SYMBOL(vps_video_unregister_isr);
struct vps_video_ctrl *vps_video_get_ctrl(int num)
{
	struct vps_video_ctrl *vctrl;

	list_for_each_entry(vctrl, &vctrl_list, list) {
		if (vctrl->idx == num)
			return vctrl;
	}

	return NULL;
}
EXPORT_SYMBOL(vps_video_get_ctrl);

int vps_video_get_num_video(void)
{
	return num_vctrl;
}
EXPORT_SYMBOL(vps_video_get_num_video);


static void vps_video_add_ctrl(struct vps_video_ctrl *vctrl)
{
	++num_vctrl;
	list_add_tail(&vctrl->list, &vctrl_list);
}


void __init vps_fvid2_video_ctrl_init(struct vps_video_ctrl *vctrl)
{
	int i;
	/*create paramters*/
	vctrl->vcparams->memtype = VPS_VPDMA_MT_NONTILEDMEM;
	vctrl->vcparams->percben = 0; /*do not need periodically callback*/

	vctrl->fmt->channelnum = 0;
	vctrl->framelist->numframes = 1;
	for (i = 0 ; i < MAX_BUFFER_NUM; i++)
		vctrl->framelist->frames[i] =
			(struct fvid2_frame *)vctrl->frm_phy[i];

	/*assign the function pointer*/
	vctrl->create = video_create;
	vctrl->delete = video_delete;
	vctrl->start = video_start;
	vctrl->stop = video_stop;
	vctrl->queue = video_queue;
	vctrl->dequeue = video_dequeue;
	vctrl->get_resolution = video_get_resolution;
	vctrl->set_buffer = video_set_buffer;
	vctrl->check_format = video_check_format;
	vctrl->try_format = video_try_format;
	vctrl->set_format = video_set_format;
	vctrl->get_format = video_get_format;
	vctrl->set_pos = video_set_rtpos;
	vctrl->set_crop = video_set_rtcrop;
	vctrl->set_color = video_set_color;
	vctrl->get_color = video_get_color;
	vctrl->apply_changes = video_apply_changes;
	vctrl->set_scparams = video_set_scparams;
	vctrl->set_deiparams = video_set_deiparams;
}

static inline int get_alloc_size(void)
{
	int size = 0;

	size += sizeof(struct vps_dispcreateparams);
	size += sizeof(struct vps_dispcreatestatus);
	size += sizeof(struct vps_disprtparams);
	size += sizeof(struct vps_posconfig);
	size += sizeof(struct vps_frameparams);
	size += sizeof(struct vps_dispstatus);
	size += sizeof(struct vps_dei_disp_params);
	size += sizeof(struct vps_cropconfig);

	size += sizeof(struct fvid2_cbparams);
	size += sizeof(struct fvid2_format);
	size += sizeof(struct fvid2_framelist);
	size += sizeof(struct fvid2_frame) *MAX_BUFFER_NUM;

	return size * VPS_DISPLAY_INST_MAX;
}

static inline void assign_payload_addr(struct vps_video_ctrl *vctrl,
				 struct vps_payload_info *pinfo,
				 u32  *buf_offset)
{
	int i;

	vctrl->vcparams = (struct vps_dispcreateparams *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vcp_phy,
				sizeof(struct vps_dispcreateparams));
	vctrl->vcstatus = (struct vps_dispcreatestatus *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vcs_phy,
				sizeof(struct vps_dispcreatestatus));

	vctrl->vrtparams = (struct vps_disprtparams *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vrt_phy,
				sizeof(struct vps_disprtparams));

	vctrl->vdmaposcfg = (struct vps_posconfig *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vdpc_phy,
				sizeof(struct vps_posconfig));

	vctrl->vfrmprm  = (struct vps_frameparams *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vfp_phy,
				sizeof(struct vps_frameparams));

	vctrl->vstatus = (struct vps_dispstatus *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vs_phy,
				sizeof(struct vps_dispstatus));

	vctrl->vdeiprm = (struct vps_dei_disp_params *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vdei_phy,
				sizeof(struct vps_dei_disp_params));

	vctrl->vdeicrpcfg = (struct vps_cropconfig *)
				setaddr(pinfo,
				buf_offset,
				&vctrl->vdeicr_phy,
				sizeof(struct vps_cropconfig));

	vctrl->cbparams = (struct fvid2_cbparams *)
				setaddr(pinfo,
					buf_offset,
					&vctrl->cbp_phy,
					sizeof(struct fvid2_cbparams));
	vctrl->fmt =     (struct fvid2_format *)
				setaddr(pinfo,
					buf_offset,
					&vctrl->fmt_phy,
					sizeof(struct fvid2_format));
	vctrl->framelist = (struct fvid2_framelist *)
				setaddr(pinfo,
					buf_offset,
					&vctrl->frmls_phy,
					sizeof(struct fvid2_framelist));

	for (i = 0 ; i < MAX_BUFFER_NUM; i++)
		vctrl->frames[i]  = (struct fvid2_frame *)
					setaddr(pinfo,
						buf_offset,
						&vctrl->frm_phy[i],
						sizeof(struct fvid2_frame));

}

int __init vps_video_init(struct platform_device *pdev)
{
	int i, r;
	struct vps_payload_info *pinfo;
	u32 size = 0;
	u32 offset = 0;
	int numvid;
	VPSSDBG("video init\n");
	INIT_LIST_HEAD(&vctrl_list);

	num_vctrl = 0;
	video_dev = pdev;
	/*allocate payload info*/
	video_payload_info = kzalloc(sizeof(struct vps_payload_info),
				GFP_KERNEL);

	if (!video_payload_info) {
		VPSSERR("allocate payload info failed\n");
		return -ENOMEM;
	}
	pinfo = video_payload_info;
	/*calcuate size to allocate*/
	size = get_alloc_size();
	pinfo->vaddr = vps_sbuf_alloc(size, &pinfo->paddr);

	if (pinfo->vaddr == NULL) {
		VPSSERR("alloc video dma buffer failed\n");
		pinfo->paddr = 0;
		r = -ENOMEM;
		goto cleanup;
	}

	pinfo->size = PAGE_ALIGN(size);
	memset(pinfo->vaddr, 0, pinfo->size);
	numvid = v_pdata->numvideo > VPS_DISPLAY_INST_MAX ? \
		VPS_DISPLAY_INST_MAX : v_pdata->numvideo;
	for (i = 0; i < numvid; i++) {
		struct vps_video_ctrl *vctrl;
		int j;
		vctrl = kzalloc(sizeof(*vctrl), GFP_KERNEL);

		if (vctrl == NULL) {
			VPSSERR("failed to allocate video%d\n", i);
			r = -ENOMEM;
			goto cleanup;
		}
		/*assign the dma buffer*/
		assign_payload_addr(vctrl,
				    pinfo,
				    &offset);
		/*init video control*/
		vps_fvid2_video_ctrl_init(vctrl);
		vctrl->idx = v_pdata->vdata[i].idx;
		vps_video_add_ctrl(vctrl);
		mutex_init(&vctrl->vmutex);

		INIT_LIST_HEAD(&vctrl->cb_list);
		/*setup the nodes*/
		vctrl->num_edges = v_pdata->vdata[i].numedges;
		vctrl->num_outputs = v_pdata->vdata[i].numoutput;
		for (j = 0; j < vctrl->num_edges; j++) {
			vctrl->nodes[j].inputid =
				v_pdata->vdata[i].snodes_inputid[j];
			vctrl->nodes[j].nodeid = v_pdata->vdata[i].snodes[j];
		}

		for (j = 0; j < vctrl->num_outputs; j++) {
			vctrl->enodes[j].inputid =
				v_pdata->vdata[i].enodes_inputid[j];
			vctrl->enodes[j].nodeid = v_pdata->vdata[i].enodes[j];

		}
		switch (i) {
		case 0:
			vctrl->caps = VPSS_VID_CAPS_POSITIONING |
				VPSS_VID_CAPS_COLOR | VPSS_VID_CAPS_CROPING |
				VPSS_VID_CAPS_SCALING;
			break;
		case 1:
			vctrl->caps = VPSS_VID_CAPS_POSITIONING |
				VPSS_VID_CAPS_COLOR | VPSS_VID_CAPS_CROPING |
				VPSS_VID_CAPS_SCALING;
			break;
		case 2:
			vctrl->caps = VPSS_VID_CAPS_POSITIONING |
					 VPSS_VID_CAPS_CROPING;
			break;
		}
		r = video_create_sysfs(vctrl);
		if (r)
			goto cleanup;

	}
	return 0;
cleanup:
	vps_video_deinit(pdev);
	return r;

}
void __exit vps_video_deinit(struct platform_device *pdev)
{
	struct vps_video_ctrl *vctrl;

	/*remove all ndoes*/
	list_for_each_entry(vctrl, &vctrl_list, list) {
		int i;
		for (i = 0; i < vctrl->num_edges; i++) {
			vctrl->nodes[i].isenable = 0;
			vps_dc_set_node(vctrl->nodes[i].nodeid,
					vctrl->nodes[i].inputid,
					vctrl->nodes[i].isenable);

		}
		for (i = 0; i < vctrl->num_outputs; i++) {
			vctrl->enodes[i].isenable = 0;
			vps_dc_set_node(vctrl->enodes[i].nodeid,
					vctrl->enodes[i].inputid,
					vctrl->enodes[i].isenable);

		}

	}

	/*free payload memory*/
	if (video_payload_info) {
		if (video_payload_info->vaddr) {
			vps_sbuf_free(video_payload_info->paddr,
				      video_payload_info->vaddr,
				      video_payload_info->size);
		}
		kfree(video_payload_info);
		video_payload_info = NULL;
	}
	/*free video ctrl */
	while (!list_empty(&vctrl_list)) {
		struct vps_isr_data *isrd;

		vctrl = list_first_entry(&vctrl_list,
					 struct vps_video_ctrl, list);
		/*free all cb list*/
		while (!list_empty(&vctrl->cb_list)) {
			isrd = list_first_entry(&vctrl->cb_list,
						struct vps_isr_data,
						list);
			list_del(&isrd->list);
			kfree(isrd);
		}
		video_remove_sysfs(vctrl);
		list_del(&vctrl->list);
		kfree(vctrl);
	}
	num_vctrl = 0;
	video_dev = NULL;
}

