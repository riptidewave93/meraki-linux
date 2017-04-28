/*
 * Copyright (C) 2010 Texas Instruments Inc
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TI81XXVIN_H
#define TI81XXVIN_H

#ifdef __KERNEL__

/* Header files */
#include <linux/videodev2.h>
#include <linux/version.h>
#include <media/v4l2-device.h>
#include <media/videobuf-core.h>
#include <media/videobuf-dma-contig.h>
#include <linux/ti81xx.h>
#include "ti81xxvin_lib.h"
/* Macros */

#define TI81XXVIN_NUM_INSTANCES	4
#define TI81XXVIN_BUFFER_ALIGN	16


#define TI81XXVIN_MAJOR_RELEASE		0
#define TI81XXVIN_MINOR_RELEASE		0
#define TI81XXVIN_BUILD			1
#define TI81XXVIN_VERSION_CODE	((TI81XXVIN_MAJOR_RELEASE << 16) | \
		(TI81XXVIN_MINOR_RELEASE << 8) | TI81XXVIN_BUILD)

#define TI81XXVIN_VALID_FIELD(field)		(((V4L2_FIELD_ANY == field) || \
	(V4L2_FIELD_NONE == field)) || \
	(((V4L2_FIELD_INTERLACED == field) || \
	(V4L2_FIELD_SEQ_TB == field)) || \
	(V4L2_FIELD_SEQ_BT == field)))

#define TI81XXVIN_CAPTURE_MAX_DEVICES	4
#define TI81XXVIN_VIDEO_INDEX		0
#define TI81XXVIN_NUMBER_OF_OBJECTS	1

#define VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTA_LUMA_STATUS_MASK (0x00000010u)
#define VIP_PARSER_FIQ_STATUS_OUTPUT_FIFO_PRTB_LUMA_STATUS_MASK (0x00000080u)
#define VIP_REG_MAIN_OFFSET		0
#define VIP_REG_PORT_A_OFFSET		1
#define VIP_REG_XTRA_PORT_A_OFFSET	2
#define VIP_REG_PORT_B_OFFSET		3
#define VIP_REG_XTRA_PORT_B_OFFSET	4
#define VIP_REG_FIQ_MASK_OFFSET		5
#define VIP_REG_FIQ_CLEAR_OFFSET	6
#define VIP_REG_FIQ_STATUS_OFFSET	7

/* Enumerated data type to give id to each instance */
enum ti81xxvin_instance_id {
	TI81XXVIN_INSTANCE0_VIDEO = 0,
	TI81XXVIN_INSTANCE1_VIDEO,
	TI81XXVIN_INSTANCE2_VIDEO,
	TI81XXVIN_INSTANCE3_VIDEO,
};

/* This structure will store size parameters as per the mode selected by user */
struct ti81xxvin_buffer_params {
	char name[32];			/* Name of the dv standard */
	u16 width;			/* Indicates width of the image */
	u16 height;			/* Indicates height of the image */
	u8 fps;				/* Indicates the fps of the image */
	u8 interlaced;			/* Indicates whether this is interlaced
					 * or progressive format */
	u8 ycmux_mode;			/* Indicates whether this mode requires
					 * single or two channels */
	u16 vsize;			/* Vertical size of the image */
	u32 dv_preset;			/* dv preset */
	u32 bytesperline;		/* required bytes per line */
};

struct ti81xxvin_vid_obj {
	enum v4l2_field buf_field;
	/* Currently selected or default dv standard */
	struct v4l2_dv_preset cur_dv_preset;
	/* Currently selected or default dv standard */
	v4l2_std_id cur_std_id;
	/* This is to track the last input that is passed to application */
	u32 input_idx;
	/* Pointer for holding the buffer params */
	struct ti81xxvin_buffer_params buf_params;

};

struct ti81xxvin_buffer_obj {
	/* Pointer pointing to current v4l2_buffer */
	struct videobuf_buffer *cur_frm;
	/* Pointer pointing to current v4l2_buffer */
	struct videobuf_buffer *next_frm;
	/*
	 * This field keeps track of type of buffer exchange mechanism
	 * user has selected
	 */
	enum v4l2_memory memory;
	/* Used to store pixel format */
	struct v4l2_format fmt;
	/* Buffer queue used in video-buf */
	struct videobuf_queue buffer_queue;
	/* Queue of filled frames */
	/* Used in video-buf */
	spinlock_t buf_irqlock;
	/* lock used to access this structure */
	struct mutex buf_lock;
	/* number of users performing IO */
	u32 io_usrs;
	/* Indicates whether streaming started */
	u8 started;
	/* Pointer pointing to the last DONE buffer in buffer_queue */
	struct videobuf_buffer *last_done_buffer;
	/* Serialize queueing from multiple threads.
	 * Still de-queue and queue are not serialized
	 */
};

struct ti81xxvin_instance_obj {
	/* Identifies video device for this instance */
	struct video_device *video_dev;
	/* Used to keep track of state of the priority */
	struct v4l2_prio_state prio;
	/* number of open instances of the instance */
	int usrs;
	/* Indicates id of the field which is being displayed */
	u32 field_id;
	/* flag to indicate whether decoder is initialized */
	u8 initialized;
	/* Identifies instance */
	enum ti81xxvin_instance_id instance_id;
	/* index into sd table */
	int curr_sd_index;
	/* ptr to current sub device information */
	struct ti81xxvin_subdev_info *curr_subdev_info;
	/* buf_obj object array */
	struct ti81xxvin_buffer_obj buf_obj;
	/* video object */
	struct ti81xxvin_vid_obj video;
	struct vps_capt_ctrl *captctrl;
	/* Flag to indicate whether FVID2_dequeue call can be made to
	 * firware
	 */
	u32 fvid2_dq_can_be_done;
	/* placeholder for cropping parameters*/
	struct v4l2_rect crop;
	/* Place holder for buffer parameters */
	struct v4l2_pix_format pix;
	/* Place holder for scaling parameter */
	struct v4l2_window win;
	/* Driver queue head */
	struct list_head dma_queue;
	/* Address for the VIP overflow register */
	void __iomem *addr;
	/* Timer function to periodically check for overflow */
	struct timer_list overflow_timer;
};

/* File handle structure */
struct ti81xxvin_fh {
	/* pointer to instance object for opened device */
	struct ti81xxvin_instance_obj *instance;
	/* Indicates whether this file handle is doing IO */
	u8 io_allowed;
	/* Used to keep track priority of this instance */
	enum v4l2_priority prio;
	/* Used to indicate instance is initialize or not */
	u8 initialized;
};

struct ti81xxvin_device {
	struct v4l2_device v4l2_dev;
	struct ti81xxvin_instance_obj *dev[TI81XXVIN_NUM_INSTANCES];
	struct v4l2_subdev **sd;
	struct vps_capt_ctrl *captctrl[TI81XXVIN_NUM_INSTANCES];
	u32 act_num_capt_ctrl;
	void __iomem *addr[TI81XXVIN_NUM_INSTANCES];
};

struct ti81xxvin_config_params {
	u8 min_numbuffers;
	u8 numbuffers[TI81XXVIN_NUM_INSTANCES];
	u32 min_bufsize[TI81XXVIN_NUM_INSTANCES];
	u8 default_device[TI81XXVIN_NUM_INSTANCES];
};
#endif				/* End of __KERNEL__ */
#endif				/* TI81XXVIN_H */
