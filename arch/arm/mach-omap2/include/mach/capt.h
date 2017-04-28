/*
 *
 * Capture internal header file for TI 81XX VPSS
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

#ifndef __LINUX_CAPT_H__
#define __LINUX_CAPT_H__

#include <linux/vps_capture.h>

typedef void (*capt_complete_callback_t)(void *arg);

struct vps_capt_ctrl {
	/* Kobject for registering with kernel */
	struct kobject                  kobj;
	/* To protect the interrupts */
	spinlock_t			buf_irqlock;
	u32                             capt_num;
	bool                            isfmtset;
	bool				isstdset;
	bool				isinputset;
	bool                            isstarted;
	struct mutex                    cmutex;
	struct list_head                list;
	/*list of the all callback for capture complete*/
	struct list_head                cb_list;
	/*Handle returned by the create call*/
	void                            *handle;
	/*create params*/
	struct Vps_CaptCreateParams     *ccparams;
	u32                             ccp_phy;
	/* VIP  params */
	struct Vps_VipConfig		*vipcfg;
	u32				vipcfg_phy;
	/* VIP port configuration */
	struct Vps_VipPortConfig	*vipportcfg;
	u32				vipportcfg_phy;
	/*create status*/
	struct Vps_CaptCreateStatus     *ccstatus;
	u32                             ccs_phy;
	/*scaling coeff*/
	struct vps_captsccoeff          *csccoeff;
	u32                             csccoff_phy;
	/*runtime change parameters*/
	struct Vps_CaptRtParams         *crtparam;
	u32                             crtp_phy;
	/* Channel status arguments */
	struct Vps_CaptChGetStatusArgs  *cchstatusargs;
	u32				cchstargs_phy;
	/* Channel status */
	struct Vps_CaptChStatus		*cchstatus;
	u32				cchstatus_phy;
	/* Frameskip */
	struct Vps_CaptFrameSkip	*cfrmskip;
	u32				cfrmskip_phy;
	/* OverFlow staus */
	struct Vps_CaptOverFlowStatus	*covlflowstat;
	u32				covlflowstat_phy;

	/* Ioctl to return err frames to driver so that Firmware does not
	 * give out of sync frames */
	struct Vps_CaptReturnErrFrms	*creturnerrfrms;
	u32				creturnerrfrms_phy;

	/* IOCTL to inform capture to have one callback per frame else
	   firmware keeps on givig callback till frame is dequeued*/
	struct Vps_CaptOneCallBackPerFrm	*conecbperfrm;
	u32					conecbperfrm_phy;

	/* Ioctl to inform firmware to return both fields are merged for
	 * interlaced capture else firmware gives fiels separate */
	struct Vps_CaptStoragePrms	*cstorageprms;
	u32				cstorageprms_phy;
	/*last buffer address*/
	u32                             buffer_addr;
	/* The followings  are the FVID2 varables*/
	/*fvid2 create params*/
	struct fvid2_cbparams           *cbparams;
	u32                             cbp_phy;
	/* set format params*/
	struct fvid2_format             *inputf;
	u32                             inputf_phy;
	/*frame list*/
	struct fvid2_framelist          *framelist[VIDEO_MAX_FRAME];
	u32                             frmls_phy[VIDEO_MAX_FRAME];
	/*frames*/
	struct fvid2_frame              *frames[VIDEO_MAX_FRAME];
	u32                             frm_phy[VIDEO_MAX_FRAME];

	capt_complete_callback_t                vsync_cb;
	void                            *vcb_arg;
	/* minimum width and height requried by decoder standard */
	u32                             decoder_width;
	u32                             decoder_height;
	u8                              scformat;
	struct fvid2_format             fmt;
	/*function pointer*/
	/*function pointer*/
	int (*create)(struct vps_capt_ctrl *cctrl);
	int (*delete)(struct vps_capt_ctrl *cctrl);
	int (*start)(struct vps_capt_ctrl *cctrl);
	int (*stop)(struct vps_capt_ctrl *cctrl);
	int (*queue)(struct vps_capt_ctrl *queue, int index);
	int (*dequeue)(struct vps_capt_ctrl *dequeue, int index);

	int (*apply_changes)(struct vps_capt_ctrl *cctrl);
	int (*check_format)(struct vps_capt_ctrl *cctrl,
			struct fvid2_format *fmt);
	int (*try_format)(struct vps_capt_ctrl *cctrl,
			u32 width, u32 height, u32 df,
			u32 pitch, u8 merged,
			struct fvid2_format *fmt);
	int (*set_format)(struct vps_capt_ctrl *cctrl,
			struct fvid2_format *fmt);
	int (*get_format) (struct vps_capt_ctrl *cctrl,
			struct fvid2_format *fmt);
	int (*set_buffer)(struct vps_capt_ctrl *cctrl,
			u32 buffer_paddr, u8 idx, u32 offset);
	int (*control) (struct vps_capt_ctrl *cctrl,
			u32 cmd, void *cmdargs, void *cmdstatusargs);
};

static inline void capt_lock(struct vps_capt_ctrl *gctrl)
{
	mutex_lock(&gctrl->cmutex);
}

static inline void capt_unlock(struct vps_capt_ctrl *gctrl)
{
	mutex_unlock(&gctrl->cmutex);
}

int vps_capt_get_num_cap(void);
struct vps_capt_ctrl *vps_capt_get_ctrl(int num);
int vps_capture_unregister_isr(capt_complete_callback_t cb, void *arg, int idx);
int vps_capture_register_isr(capt_complete_callback_t cb, void *arg, int idx);

#endif
