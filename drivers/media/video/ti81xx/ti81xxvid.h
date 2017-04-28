/*
 * ti81xxvid.h
 *
 * Copyright (C) 2010 Texas Instruments.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef __TI81XXVID_H
#define __TI81XXVID_H

#define MAX_CID		3

/* per-device data structure */
struct ti81xx_vidout_dev {

	struct vps_video_ctrl *vctrl;
	struct video_device *vfd;
	struct ti81xxvideo_device *vid_dev;
	int vid;
	int users;
	bool isprogressive;

	/* we don't allow to change image fmt/size once buffer has
	 * been allocated
	 */
	int buffer_allocated;
	/* allow to reuse previously allocated buffer which is big enough */
	int buffer_size;
	/* keep buffer info across opens */
	unsigned long buf_virt_addr[VIDEO_MAX_FRAME];
	unsigned long buf_phy_addr[VIDEO_MAX_FRAME];

	/* we don't allow to request new buffer when old buffers are
	 * still mmaped
	 */
	int mmap_count;

	spinlock_t vbq_lock;		/* spinlock for videobuf queues */
	unsigned long field_count;	/* field counter for videobuf_buffer */

	/* non-NULL means streaming is in progress. */
	bool streaming;

	/*OUTPUT TYPE ATTRIBUTE*/
	struct v4l2_pix_format pix;
	struct v4l2_rect crop;
	/*OVERLAY TYPE ATTRIBUTE*/
	struct v4l2_window win;
	struct v4l2_framebuffer fbuf;

	/* Lock to protect the shared data structures in ioctl */
	struct mutex lock;

	/* V4L2 control structure for different control id */
	struct v4l2_control control[MAX_CID];
	/* V4L2 control structure for different control id */

	int bpp; /* bytes per pixel */
	int sbpp; /*bytes per pixle for size */
	int  first_int, field_id;
	enum v4l2_memory memory;
	struct list_head dma_queue;
	u8 *queued_buf_addr[VIDEO_MAX_FRAME];
	u32 cropped_offset;

	/* Buffer queue variables */
	struct ti81xx_vidout_dev *vout;
	enum v4l2_buf_type type;
	struct videobuf_queue vbq;

	/*Scalar parameters*/
	struct ti81xxvid_scalarparam scalar_prms;
	unsigned long scoutbuf; /* temporary buffer for scalar o/p*/

};

struct ti81xxvideo_device {
	struct mutex  mtx;

	struct v4l2_device v4l2_dev;
	int num_vout;
	struct ti81xx_vidout_dev *vouts[VPS_DISPLAY_INST_MAX];

	int num_videos;
	struct vps_video_ctrl *vctrl[VPS_DISPLAY_INST_MAX];
};

/*file handle to support multiple open*/
struct ti81xxvideo_fh {
	struct ti81xx_vidout_dev  *voutdev;
};
#endif	/* ifndef TI81XX_VID_H */
