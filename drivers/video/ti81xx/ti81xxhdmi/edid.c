/*
 * drivers/video/ti81xx/ti81xxhdmi/edid.c
 *
 * Copyright (C) 2009 Texas Instruments
 * Author: Mythri P K <mythripk@ti.com>
 *         With EDID parsing for DVI Monitor from Rob Clark <rob@ti.com>
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
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/platform_device.h>

#include <plat/cpu.h>

#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/edid.h>

/* All of the supported timing values */
const struct video_timings video_timings[VIDEO_TIMINGS_NB] = {
	{640, 480, 25200, 96, 16, 48, 2, 10, 33},
	{1280, 720, 74250, 40, 440, 220, 5, 5, 20},
	{1280, 720, 74250, 40, 110, 220, 5, 5, 20},
	{720, 480, 27027, 62, 16, 60, 6, 9, 30},
	{2880, 576, 108000, 256, 48, 272, 5, 5, 39},
	{1440, 240, 27027, 124, 38, 114, 3, 4, 15},
	{1440, 288, 27000, 126, 24, 138, 3, 2, 19},
	{1920, 540, 74250, 44, 528, 148, 5, 2, 15},
	{1920, 540, 74250, 44, 88, 148, 5, 2, 15},
	{1920, 1080, 148500, 44, 88, 148, 5, 4, 36},
	{720, 576, 27000, 64, 12, 68, 5, 5, 39},
	{1440, 576, 54000, 128, 24, 136, 5, 5, 39},
	{1920, 1080, 148500, 44, 528, 148, 5, 4, 36},
	{2880, 480, 108108, 248, 64, 240, 6, 9, 30},
	{1920, 1080, 74250, 44, 638, 148, 5, 4, 36},
	/* Vesa frome here */
	{640, 480, 25175, 96, 16, 48, 2 , 11, 31},
	{800, 600, 40000, 128, 40, 88, 4 , 1, 23},
	{848, 480, 33750, 112, 16, 112, 8 , 6, 23},
	{1280, 768, 79500, 128, 64, 192, 7 , 3, 20},
	{1280, 800, 83500, 128, 72, 200, 6 , 3, 22},
	{1360, 768, 85500, 112, 64, 256, 6 , 3, 18},
	{1280, 960, 108000, 112, 96, 312, 3 , 1, 36},
	{1280, 1024, 108000, 112, 48, 248, 3 , 1, 38},
	{1024, 768, 65000, 136, 24, 160, 6, 3, 29},
	{1400, 1050, 121750, 144, 88, 232, 4, 3, 32},
	{1440, 900, 106500, 152, 80, 232, 6, 3, 25},
	{1680, 1050, 146250, 176 , 104, 280, 6, 3, 30},
	{1366, 768, 85500, 143, 70, 213, 3, 3, 24},
	{1920, 1080, 148500, 44, 88, 80, 5, 4, 36},
	{1280, 768, 68250, 32, 48, 80, 7, 3, 12},
	{1400, 1050, 101000, 32, 48, 80, 4, 3, 23},
	{1680, 1050, 119000, 32, 48, 80, 6, 3, 21},
	{1280, 800, 79500, 32, 48, 80, 6, 3, 14},
	{1280, 720, 74250, 40, 110, 220, 5, 5, 20}
};

__u32 get_edid_timing_info(union HDMI_EDID_DTD *edid_dtd,
					struct video_timings *timings)
{
	struct HDMI_EDID_DTD_VIDEO *vid = &edid_dtd->video;
	if (edid_dtd->video.pixel_clock) {

		timings->pixel_clock = 10 * vid->pixel_clock;
		timings->x_res = vid->horiz_active |
				(((u16)vid->horiz_high & 0xf0) << 4);
		timings->y_res = vid->vert_active |
				(((u16)vid->vert_high & 0xf0) << 4);
		timings->hfp = vid->horiz_sync_offset |
				(((u16)vid->sync_pulse_high & 0xc0) << 2);
		timings->hsw = vid->horiz_sync_pulse |
				(((u16)vid->sync_pulse_high & 0x30) << 4);
		timings->hbp = (vid->horiz_blanking |
				(((u16)vid->horiz_high & 0x0f) << 8)) -
				(timings->hfp + timings->hsw);
		timings->vfp = ((vid->vert_sync_pulse & 0xf0) >> 4) |
				((vid->sync_pulse_high & 0x0f) << 2);
		timings->vsw = (vid->vert_sync_pulse & 0x0f) |
				((vid->sync_pulse_high & 0x03) << 4);
		timings->vbp = (vid->vert_blanking |
				(((u16)vid->vert_high & 0x0f) << 8)) -
				(timings->vfp + timings->vsw);
		return 0;
	}

	switch (edid_dtd->monitor_name.block_type) {
	case HDMI_EDID_DTD_TAG_STANDARD_TIMING_DATA:
		/* printk(KERN_INFO "standard timing data\n"); */
		return 1;
	case HDMI_EDID_DTD_TAG_COLOR_POINT_DATA:
		/* printk(KERN_INFO "color point data\n"); */
		return 1;
	case HDMI_EDID_DTD_TAG_MONITOR_NAME:
		/* printk(KERN_INFO "monitor name: %s\n",
						edid_dtd->monitor_name.text); */
		return 1;
	case HDMI_EDID_DTD_TAG_MONITOR_LIMITS:
	{
		int i, max_area = 0, best_idx = -1;
		struct HDMI_EDID_DTD_MONITOR *limits =
						&edid_dtd->monitor_limits;

/*		printk(KERN_INFO "monitor limits\n");
		printk(KERN_INFO "  min_vert_freq=%d\n", limits->min_vert_freq);
		printk(KERN_INFO "  max_vert_freq=%d\n", limits->max_vert_freq);
		printk(KERN_INFO "  min_horiz_freq=%d\n",
						limits->min_horiz_freq);
		printk(KERN_INFO "  max_horiz_freq=%d\n",
						limits->max_horiz_freq);
		printk(KERN_INFO "  pixel_clock_mhz=%d\n",
						limits->pixel_clock_mhz * 10);
*/
		/* find the highest matching resolution (w*h) */

		/*
		 * XXX since this is mainly for DVI monitors, should we only
		 * support VESA timings?  My monitor at home would pick
		 * 1920x1080 otherwise, but that seems to not work well (monitor
		 * blanks out and comes back, and picture doesn't fill full
		 * screen, but leaves a black bar on left (native res is
		 * 2048x1152). However if I only consider VESA timings, it picks
		 * 1680x1050 and the picture is stable and fills whole screen
		 */
		for (i = VIDEO_TIMINGS_VESA_START;
					i < VIDEO_TIMINGS_NB; i++) {
			const struct video_timings *timings =
						 &video_timings[i];
			int hz, hscan, pixclock;
			int vtotal, htotal;
			htotal = timings->hbp + timings->hfp +
					timings->hsw + timings->x_res;
			vtotal = timings->vbp + timings->vfp +
					timings->vsw + timings->y_res;

			/* NOTE: We don't support interlaced mode for VESA */
			pixclock = timings->pixel_clock * 1000;
			hscan = (pixclock + htotal / 2) / htotal;
			hscan = (hscan + 500) / 1000 * 1000;
			hz = (hscan + vtotal / 2) / vtotal;
			hscan /= 1000;
			pixclock /= 1000000;
			/* printk(KERN_DEBUG "debug only pixclock=%d, hscan=%d, hz=%d\n",
					pixclock, hscan, hz); */
			if ((pixclock < (limits->pixel_clock_mhz * 10)) &&
				(limits->min_horiz_freq <= hscan) &&
				(hscan <= limits->max_horiz_freq) &&
				(limits->min_vert_freq <= hz) &&
				(hz <= limits->max_vert_freq)) {
				int area = timings->x_res * timings->y_res;
				/* printk(KERN_INFO " -> %d: %dx%d\n", i,
					timings->x_res, timings->y_res); */
				if (area > max_area) {
					max_area = area;
					best_idx = i;
				}
			}
		}
		if (best_idx > 0) {
			*timings = video_timings[best_idx];
			printk(KERN_DEBUG "found best resolution: %dx%d (%d)\n",
				timings->x_res, timings->y_res, best_idx);
		}
		return 0;
	}
	case HDMI_EDID_DTD_TAG_ASCII_STRING:
		/* printk(KERN_INFO "ascii string: %s\n", edid_dtd->ascii.text); */
		return 1;
	case HDMI_EDID_DTD_TAG_MONITOR_SERIALNUM:
		/* printk(KERN_INFO "monitor serialnum: %s\n",
			edid_dtd->monitor_serial_number.text);  */
		return 1;
	default:
		printk(KERN_INFO "unsupported EDID descriptor block format\n");
		return 1;
	}
}

void get_eedid_timing_info(__u32 current_descriptor_addrs, __u8 *edid ,
			struct video_timings *timings)
{
	timings->x_res = (((edid[current_descriptor_addrs + 4] & 0xF0) << 4)
				| edid[current_descriptor_addrs + 2]);
	timings->y_res = (((edid[current_descriptor_addrs + 7] & 0xF0) << 4)
				| edid[current_descriptor_addrs + 5]);
	timings->pixel_clock = ((edid[current_descriptor_addrs + 1] << 8)
				| edid[current_descriptor_addrs]);
	timings->pixel_clock = 10 * timings->pixel_clock;
	timings->hfp = edid[current_descriptor_addrs + 8];
	timings->hsw = edid[current_descriptor_addrs + 9];
	timings->hbp = (((edid[current_descriptor_addrs + 4] & 0x0F) << 8)
				| edid[current_descriptor_addrs + 3]) -
				(timings->hfp + timings->hsw);
	timings->vfp = ((edid[current_descriptor_addrs + 10] & 0xF0) >> 4);
	timings->vsw = (edid[current_descriptor_addrs + 10] & 0x0F);
	timings->vbp = (((edid[current_descriptor_addrs + 7] & 0x0F) << 8)
				| edid[current_descriptor_addrs + 6]) -
				(timings->vfp + timings->vsw);
}

__u32 hdmi_get_datablock_offset(__u8 *edid, enum extension_edid_db datablock,
								__u32 *offset)
{
	int current_byte, disp, i = 0, length = 0;

	if (edid[0x7e] == 0x00)
		return 1;

	disp = edid[(0x80) + 2];
	printk(KERN_DEBUG "Extension block present db %d %x\n",
		datablock, disp);
	if (disp == 0x4)
		return 1;

	i = 0x80 + 0x4;
	printk(KERN_DEBUG "%x\n", i);
	while (i < (0x80 + disp)) {
		current_byte = edid[i];
		if ((current_byte >> 5)	== datablock) {
			*offset = i;
			return 0;
		} else {
			length = (current_byte &
					HDMI_EDID_EX_DATABLOCK_LEN_MASK) + 1;
			i += length;
		}
	}
	return 1;
}

__u32 hdmi_get_image_format(__u8 *edid, struct image_format *format)
{
	int offset, current_byte, j = 0, length = 0;
	enum extension_edid_db vsdb =  DATABLOCK_VIDEO;
	format->length = 0;

	memset(format->fmt, 0, sizeof(format->fmt));
	if (!hdmi_get_datablock_offset(edid, vsdb, &offset)) {
		current_byte = edid[offset];
		length = current_byte & HDMI_EDID_EX_DATABLOCK_LEN_MASK;

		if (length >= HDMI_IMG_FORMAT_MAX_LENGTH)
			format->length = HDMI_IMG_FORMAT_MAX_LENGTH;
		else
			format->length = length;

		for (j = 1 ; j <= format->length ; j++) {
			current_byte = edid[offset+j];
			format->fmt[j-1].code = current_byte & 0x7F;
			format->fmt[j-1].pref = current_byte & 0x80;
		}
	}
	return 0;
}

__u32 hdmi_get_audio_format(__u8 *edid, struct audio_format *format)
{
	int offset, current_byte, i, j = 0, length = 0;
	enum extension_edid_db vsdb =  DATABLOCK_AUDIO;

	format->length = 0;
	memset(format->fmt, 0, sizeof(format->fmt));

	if (!hdmi_get_datablock_offset(edid, vsdb, &offset)) {
		current_byte = edid[offset];
		length = current_byte & HDMI_EDID_EX_DATABLOCK_LEN_MASK;

		if (length >= HDMI_AUDIO_FORMAT_MAX_LENGTH * 3)
			format->length = HDMI_AUDIO_FORMAT_MAX_LENGTH * 3;
		else
			format->length = length;

		i = 0;
		for (j = 1 ; j <= format->length ; j++) {
			if (j%3 == 1) {
				current_byte = edid[offset + j];
				format->fmt[i].format =
						(current_byte & 0x78) >> 3;
				format->fmt[i].num_of_ch =
						(current_byte & 0x07) + 1;
			} else if (j%3 == 2) {
				/* Get the Frequency -
					CEA861 - PG 154 - Table 84 / 34 */
				current_byte = edid[offset + j];
				format->fmt[i].freq = current_byte;
			} else if (j%3 == 0) {
				/* Get the Bit Rate -
					CEA861 - PG 154 - Table 84 / 34 */
				current_byte = edid[offset + j];
				format->fmt[i].width = current_byte & 0x7;
				i++;
			}
		}
	}
	return 0;
}

void hdmi_get_av_delay(u8 *edid, struct latency *lat)
{
	int offset, current_byte, length = 0;
	enum extension_edid_db vsdb =  DATABLOCK_VENDOR;

	if (!hdmi_get_datablock_offset(edid, vsdb, &offset)) {
		current_byte = edid[offset];
		length = current_byte & HDMI_EDID_EX_DATABLOCK_LEN_MASK;
		if (length >= 8 && ((current_byte + 8) & 0x80)) {
			lat->vid_latency = ((current_byte + 8) - 1) * 2;
			lat->aud_latency = ((current_byte + 9) - 1) * 2;
		}
		if (length >= 8 && ((current_byte + 8) & 0xC0)) {
			lat->int_vid_latency = ((current_byte + 10) - 1) * 2;
			lat->int_aud_latency = ((current_byte + 11) - 1) * 2;
		}
	}
}

void hdmi_deep_color_support_info(u8 *edid, struct deep_color *format)
{
	int offset, current_byte, length = 0;
	enum extension_edid_db vsdb = DATABLOCK_VENDOR;
	memset(format, 0, sizeof(*format));

	if (!hdmi_get_datablock_offset(edid, vsdb, &offset)) {
		current_byte = edid[offset];
		length = current_byte & HDMI_EDID_EX_DATABLOCK_LEN_MASK;
		if (length >= 6) {
			format->bit_30 = ((current_byte + 6) & 0x10);
			format->bit_36 = ((current_byte + 6) & 0x20);
		}
		if (length >= 7)
			format->max_tmds_freq = ((current_byte + 7)) * 5;
	}
}

__u32 hdmi_tv_yuv_supported(__u8 *edid)
{
	if (edid[0x7e] != 0x00)
		return (edid[0x83] & 0x30) >> 4;
	return 0;
}

_Bool hdmi_tv_hdmi_supported(__u8 *edid)
{
	/*check with TV suppport HDMI or not
	if no externsion block, HDMI is not supported*/
	if (edid[0x7e] == 0x00)
		return false;
	else {
		enum extension_edid_db db = DATABLOCK_VENDOR;
		int offset, i, blocks, index = 0;
		blocks = edid[0x7e];
		for (i = 1; i <= blocks; i++) {
			/*only version 3 or up and
				CEA EDID is possible to support EDID*/
			if ((edid[128 * i] == 0x02) &&
			    (edid[128 * i + 1] >= 0x03)) {
				/*if IEEE code(first three bytes)
				  in Vender specific data block
				  are 00-0C-00, it is HDMI*/
				if (!hdmi_get_datablock_offset(
				    &edid[index], db, &offset)) {
					if ((edid[index + offset + 1] == 0x3) &&
					(edid[index + offset + 2] == 0xc) &&
					(edid[index + offset + 3] == 0x0))
						return true;
				}
			}
			index = i*128;
		}

	}
	return false;

}

_Bool hdmi_tv_cec_get_pa(__u8 *edid, __u8 *pa)
{
	/*check with TV suppport HDMI or not
	if no externsion block, HDMI is not supported*/
	if (edid[0x7e] == 0x00)
		return false;
	else {
		enum extension_edid_db db = DATABLOCK_VENDOR;
		int offset, i, blocks, idx = 0;
		blocks = edid[0x7e];
		for (i = 1; i <= blocks; i++) {
			/*only version 3 or up and
				CEA EDID is possible to support EDID*/
			if ((edid[128 * i] == 0x02) &&
			    (edid[128 * i + 1] >= 0x03)) {
				/*if IEEE code(first three bytes)
				  in Vender specific data block
				  are 00-0C-00, it is HDMI*/
				if (!hdmi_get_datablock_offset(
				    &edid[idx], db, &offset)) {
					if ((edid[idx + offset + 1] == 0x3) &&
					(edid[idx + offset + 2] == 0xc) &&
					(edid[idx + offset + 3] == 0x0)) {

						/* Get CEC PA now
						 * Bytes 4 & 5 specify the sam
						 * HDMI 1.3a spec,
						 * Table 8-6 VSDB
						 */
						pa[0] = edid[idx + offset + 4];
						pa[1] = edid[idx + offset + 5];
						return true;
					}
				}
			}
			idx = i*128;
		}

	}
	return false;

}
