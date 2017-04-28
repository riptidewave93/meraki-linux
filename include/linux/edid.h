/*
 * linux/drivers/video/omap2/dss/edid.c
 *
 * Copyright (C) 2009 Texas Instruments
 * Author: Mythri P K <mythripk@ti.com>
 *		with some of the ENUM's and structure derived from Yong Zhi's
 *		hdmi.h(Now obsolete)
 *
 * EDID.c to read the EDID content , given the 256 Bytes EDID.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * History:
 */

#ifndef _EDID_H_
#define _EDID_H_


/* HDMI EDID Extension Data Block Tags  */
#define HDMI_EDID_EX_DATABLOCK_TAG_MASK		0xE0
#define HDMI_EDID_EX_DATABLOCK_LEN_MASK		0x1F

#define EDID_TIMING_DESCRIPTOR_SIZE		0x12
#define EDID_DESCRIPTOR_BLOCK0_ADDRESS		0x36
#define EDID_DESCRIPTOR_BLOCK1_ADDRESS		0x80
#define EDID_SIZE_BLOCK0_TIMING_DESCRIPTOR	4
#define EDID_SIZE_BLOCK1_TIMING_DESCRIPTOR	4

/* EDID Detailed Timing	Info 0 begin offset */
#define HDMI_EDID_DETAILED_TIMING_OFFSET	0x36

#define HDMI_EDID_PIX_CLK_OFFSET		0
#define HDMI_EDID_H_ACTIVE_OFFSET		2
#define HDMI_EDID_H_BLANKING_OFFSET		3
#define HDMI_EDID_V_ACTIVE_OFFSET		5
#define HDMI_EDID_V_BLANKING_OFFSET		6
#define HDMI_EDID_H_SYNC_OFFSET			8
#define HDMI_EDID_H_SYNC_PW_OFFSET		9
#define HDMI_EDID_V_SYNC_OFFSET			10
#define HDMI_EDID_V_SYNC_PW_OFFSET		11
#define HDMI_EDID_H_IMAGE_SIZE_OFFSET	12
#define HDMI_EDID_V_IMAGE_SIZE_OFFSET	13
#define HDMI_EDID_H_BORDER_OFFSET		15
#define HDMI_EDID_V_BORDER_OFFSET		16
#define HDMI_EDID_FLAGS_OFFSET			17

/* HDMI Connected States */
#define HDMI_STATE_NOMONITOR	0 /* No HDMI monitor connected*/
#define HDMI_STATE_CONNECTED	1 /* HDMI monitor connected but powered off */
#define HDMI_STATE_ON		2 /* HDMI monitor connected and powered on*/

/* HDMI EDID Length */
#define HDMI_EDID_MAX_LENGTH	512

/* HDMI EDID DTDs */
#define HDMI_EDID_MAX_DTDS		4

/* HDMI EDID DTD Tags */
#define HDMI_EDID_DTD_TAG_MONITOR_NAME			0xFC
#define HDMI_EDID_DTD_TAG_MONITOR_SERIALNUM		0xFF
#define HDMI_EDID_DTD_TAG_MONITOR_LIMITS		0xFD
#define HDMI_EDID_DTD_TAG_STANDARD_TIMING_DATA	0xFA
#define HDMI_EDID_DTD_TAG_COLOR_POINT_DATA		0xFB
#define HDMI_EDID_DTD_TAG_ASCII_STRING			0xFE

/* 5 bits describe the length, which is 32 bytes.
	1 byte for each Video Data Block */
#define HDMI_IMG_FORMAT_MAX_LENGTH			0x20
/* 5 Bits describe the length, which is 32 bytes, we would require 3 bytes per
	audio data block */
#define HDMI_AUDIO_FORMAT_MAX_LENGTH		10

/* HDMI EDID Extenion Data Block Values: Video */
#define HDMI_EDID_EX_VIDEO_NATIVE		0x80
#define HDMI_EDID_EX_VIDEO_MASK			0x7F
#define HDMI_EDID_EX_VIDEO_MAX			35

#define HDMI_EDID_EX_VIDEO_640x480p_60Hz_4_3	1
#define HDMI_EDID_EX_VIDEO_720x480p_60Hz_4_3	2
#define HDMI_EDID_EX_VIDEO_720x480p_60Hz_16_9	3
#define HDMI_EDID_EX_VIDEO_1280x720p_60Hz_16_9	4
#define HDMI_EDID_EX_VIDEO_1920x1080i_60Hz_16_9	5
#define HDMI_EDID_EX_VIDEO_720x480i_60Hz_4_3	6
#define HDMI_EDID_EX_VIDEO_720x480i_60Hz_16_9	7
#define HDMI_EDID_EX_VIDEO_720x240p_60Hz_4_3	8
#define HDMI_EDID_EX_VIDEO_720x240p_60Hz_16_9	9
#define HDMI_EDID_EX_VIDEO_2880x480i_60Hz_4_3	10
#define HDMI_EDID_EX_VIDEO_2880x480i_60Hz_16_9	11
#define HDMI_EDID_EX_VIDEO_2880x480p_60Hz_4_3	12
#define HDMI_EDID_EX_VIDEO_2880x480p_60Hz_16_9	13
#define HDMI_EDID_EX_VIDEO_1440x480p_60Hz_4_3	14
#define HDMI_EDID_EX_VIDEO_1440x480p_60Hz_16_9	15
#define HDMI_EDID_EX_VIDEO_1920x1080p_60Hz_16_9	16
#define HDMI_EDID_EX_VIDEO_720x576p_50Hz_4_3	17
#define HDMI_EDID_EX_VIDEO_720x576p_50Hz_16_9	18
#define HDMI_EDID_EX_VIDEO_1280x720p_50Hz_16_9	19
#define HDMI_EDID_EX_VIDEO_1920x1080i_50Hz_16_9	20
#define HDMI_EDID_EX_VIDEO_720x576i_50Hz_4_3	21
#define HDMI_EDID_EX_VIDEO_720x576i_50Hz_16_9	22
#define HDMI_EDID_EX_VIDEO_720x288p_50Hz_4_3	23
#define HDMI_EDID_EX_VIDEO_720x288p_50Hz_16_9	24
#define HDMI_EDID_EX_VIDEO_2880x576i_50Hz_4_3	25
#define HDMI_EDID_EX_VIDEO_2880x576i_50Hz_16_9	26
#define HDMI_EDID_EX_VIDEO_2880x288p_50Hz_4_3	27
#define HDMI_EDID_EX_VIDEO_2880x288p_50Hz_16_9	28
#define HDMI_EDID_EX_VIDEO_1440x576p_50Hz_4_3	29
#define HDMI_EDID_EX_VIDEO_1440x576p_50Hz_16_9	30
#define HDMI_EDID_EX_VIDEO_1920x1080p_50Hz_16_9	31
#define HDMI_EDID_EX_VIDEO_1920x1080p_24Hz_16_9	32
#define HDMI_EDID_EX_VIDEO_1920x1080p_25Hz_16_9	33
#define HDMI_EDID_EX_VIDEO_1920x1080p_30Hz_16_9	34

#define VIDEO_TIMINGS_NB			34
#define VIDEO_TIMINGS_VESA_START		15


struct video_timings {
	/* Unit: pixels */
	__u16 x_res;
	/* Unit: pixels */
	__u16 y_res;
	/* Unit: KHz */
	__u32 pixel_clock;
	/* Unit: pixel clocks */
	__u16 hsw;	/* Horizontal synchronization pulse width */
	/* Unit: pixel clocks */
	__u16 hfp;	/* Horizontal front porch */
	/* Unit: pixel clocks */
	__u16 hbp;	/* Horizontal back porch */
	/* Unit: line clocks */
	__u16 vsw;	/* Vertical synchronization pulse width */
	/* Unit: line clocks */
	__u16 vfp;	/* Vertical front porch */
	/* Unit: line clocks */
	__u16 vbp;	/* Vertical back porch */
};

enum extension_edid_db {
	DATABLOCK_AUDIO	= 1,
	DATABLOCK_VIDEO	= 2,
	DATABLOCK_VENDOR = 3,
	DATABLOCK_SPEAKERS = 4,
};

/* Short Video Descriptor - Refer CEA 861 Spec
 *	code indicates the Video Identification Code, that defines the
 *	resolution.
 *	pref - when set indicates resolution specified in code is native to the
 *		sink. A native format is one, that a sink can support with any
 *		processing. 
 */
struct img_edid {
	__u8 pref;
	__u32 code;
};

struct image_format {
	__u32 length;
	struct img_edid fmt[HDMI_IMG_FORMAT_MAX_LENGTH];
};

struct audio_edid {
	__u32 num_of_ch; /* Number of channels */
	__u32 format;	/* 1 - Linear PCM (IEC 60985) */
	__u32 freq; /* Frequency / Sample rate
			1 - 32 KHz, 2 - 44.1 KHz, 4 - 48 KHz,
			8 - 88.2 KHz, 16 - 96 KHz, 32 - 176.4 KHz,
			64 - 192 KHz */
	__u32 width; /* Sample width 1 - 16 bits, 2 - 20 bits, 4 - 24 bits */
};

struct audio_format {
	__u32 length;
	struct audio_edid fmt[HDMI_AUDIO_FORMAT_MAX_LENGTH];
};

struct latency {
	/* vid: if indicated, value=1+ms/2 with a max of 251 meaning 500ms */
	__u32 vid_latency;

	__u32 aud_latency;
	__u32 int_vid_latency;
	__u32 int_aud_latency;
};

struct deep_color {
	__u8 bit_30;
	__u8 bit_36;
	__u32 max_tmds_freq;
};

/*  Video Descriptor Block  */
struct HDMI_EDID_DTD_VIDEO {
	__u16	pixel_clock;		/* 54-55 */
	__u8	horiz_active;		/* 56 */
	__u8	horiz_blanking;		/* 57 */
	__u8	horiz_high;		/* 58 */
	__u8	vert_active;		/* 59 */
	__u8	vert_blanking;		/* 60 */
	__u8	vert_high;		/* 61 */
	__u8	horiz_sync_offset;	/* 62 */
	__u8	horiz_sync_pulse;	/* 63 */
	__u8	vert_sync_pulse;	/* 64 */
	__u8	sync_pulse_high;	/* 65 */
	__u8	horiz_image_size;	/* 66 */
	__u8	vert_image_size;	/* 67 */
	__u8	image_size_high;	/* 68 */
	__u8	horiz_border;		/* 69 */
	__u8	vert_border;		/* 70 */
	__u8	misc_settings;		/* 71 */
};

/*	Monitor Limits Descriptor Block	*/
struct HDMI_EDID_DTD_MONITOR {
	__u16	pixel_clock;		/* 54-55*/
	__u8	_reserved1;		/* 56 */
	__u8	block_type;		/* 57 */
	__u8	_reserved2;		/* 58 */
	__u8	min_vert_freq;		/* 59 */
	__u8	max_vert_freq;		/* 60 */
	__u8	min_horiz_freq;		/* 61 */
	__u8	max_horiz_freq;		/* 62 */
	__u8	pixel_clock_mhz;	/* 63 */
	__u8	GTF[2];			/* 64 -65 */
	__u8	start_horiz_freq;	/* 66	*/
	__u8	C;			/* 67 */
	__u8	M[2];			/* 68-69 */
	__u8	K;			/* 70 */
	__u8	J;			/* 71 */

} __attribute__ ((packed));

/* Text Descriptor Block */
struct HDMI_EDID_DTD_TEXT {
	__u16	pixel_clock;		/* 54-55 */
	__u8	_reserved1;		/* 56 */
	__u8	block_type;		/* 57 */
	__u8	_reserved2;		/* 58 */
	__u8	text[13];		/* 59-71 */
} __attribute__ ((packed));

/* DTD Union */
union HDMI_EDID_DTD {
	struct HDMI_EDID_DTD_VIDEO	video;
	struct HDMI_EDID_DTD_TEXT	monitor_name;
	struct HDMI_EDID_DTD_TEXT	monitor_serial_number;
	struct HDMI_EDID_DTD_TEXT	ascii;
	struct HDMI_EDID_DTD_MONITOR	monitor_limits;
} __attribute__ ((packed));

/*	EDID struct	*/
struct HDMI_EDID {
	__u8	header[8];		/* 00-07 */
	__u16	manufacturerID;		/* 08-09 */
	__u16	product_id;		/* 10-11 */
	__u32	serial_number;		/* 12-15 */
	__u8	week_manufactured;	/* 16 */
	__u8	year_manufactured;	/* 17 */
	__u8	edid_version;		/* 18 */
	__u8	edid_revision;		/* 19 */
	__u8	video_in_definition;	/* 20 */
	__u8	max_horiz_image_size;	/* 21 */
	__u8	max_vert_image_size;	/* 22 */
	__u8	display_gamma;		/* 23 */
	__u8	power_features;		/* 24 */
	__u8	chroma_info[10];	/* 25-34 */
	__u8	timing_1;		/* 35 */
	__u8	timing_2;		/* 36 */
	__u8	timing_3;		/* 37 */
/* Refer
 *	VESA ENHANCED EXTENDED DISPLAY
 *	IDENTIFICATION DATA STANDARD
 *	(Defines EDID Structure Version 1, Revision 4)
 *	To decode values present in timing_1, timing_2 & timing_3
 *	Table 3.18 - Established Timings I & II
 */

	__u8	std_timings[16];	/* 38-53 */
/* Refer
 *	VESA ENHANCED EXTENDED DISPLAY
 *	IDENTIFICATION DATA STANDARD
 *	(Defines EDID Structure Version 1, Revision 4)
 *	To decode values present in std_timings
 *	Table 3.19 - Standard Timings 
 */

	union HDMI_EDID_DTD DTD[4];	/* 54-125 */

	__u8	extension_edid;		/* 126 */
	__u8	checksum;		/* 127 */
	__u8	extension_tag;		/* 00 (extensions follow EDID) */
	__u8	extention_rev;		/* 01 */
	__u8	offset_dtd;		/* 02 */
	__u8	num_dtd;		/* 03 */

	__u8	data_block[123];	/* 04 - 126 */
	__u8	extension_checksum;	/* 127 */

	__u8	ext_datablock[256];
} __attribute__ ((packed));

__u32 get_edid_timing_info(union HDMI_EDID_DTD *edid_dtd,
			struct video_timings *timings);
void get_eedid_timing_info(__u32 current_descriptor_addrs, __u8 *edid ,
			struct video_timings *timings);
__u32 hdmi_get_datablock_offset(__u8 *edid, enum extension_edid_db datablock,
								__u32 *offset);
__u32 hdmi_get_image_format(__u8 *edid, struct image_format *format);
__u32 hdmi_get_audio_format(__u8 *edid, struct audio_format *format);
void hdmi_get_av_delay(__u8 *edid, struct latency *lat);
void hdmi_deep_color_support_info(__u8 *edid, struct deep_color *format);
__u32 hdmi_tv_yuv_supported(__u8 *edid);
_Bool hdmi_tv_hdmi_supported(__u8 *edid);
_Bool hdmi_tv_cec_get_pa(__u8 *edid, __u8 *pa);
#endif
