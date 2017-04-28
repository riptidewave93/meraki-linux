/*
 * Copyright (C) 2009 Texas Instruments
 * Author: Hardik Shah <hardik.shah@ti.com>
 *
 * This program is free software; you can redistaribute it and/or modify it
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
 */
/*
 * This is a header file for interfacing the external encoders with the
 * TI81xx display.  All the external encoders will register to the Display
 * sub-system of TI81xx SoCs through APIs provided by this file.
 */
#include <linux/platform_device.h>
#include <plat/ti81xx-vpss.h>

#define MAX_EXT_VENCS_PER_DISPLAY 4


/* This status information about the panel is separate from
 * 'status' maintained by respective encoder driver.
 * Power management info is also kept inside those individual drivers.
 */

enum ti81xx_display_status {
	TI81xx_EXT_ENCODER_UNREGISTERED = 0,
	TI81xx_EXT_ENCODER_REGISTERED,
	TI81xx_EXT_ENCODER_ENABLED,
	TI81xx_EXT_ENCODER_DISABLED,
	TI81xx_EXT_ENCODER_RESUMED,
	TI81xx_EXT_ENCODER_SUSPENDED,
	TI81xx_EXT_ENCODER_ACTIVE,
	TI81xx_EXT_ENCODER_DISABLED_FOR_MODECHANGE
};


/* Display encoders of the TI81xx SoCs. All the display will not be valid for
 * all the variants of TI81xx SoCs. This is the super set of all
 */
enum TI81xx_displays {
	TI81xx_DISPLAY_HDMI = 0,
	TI81xx_DISPLAY_DVO2,
	TI81xx_DISPLAY_SD,
	TI81xx_DISPLAY_HDCOMP,
};
/* Type of panel which is interfaced to SoCs. Whether the panel behaves as
 * as master or slave. Based on these timing parameters needs to be configured
 * for the panel */
enum TI81xx_device_type {
	TI81xx_DEVICE_TYPE_MASTER = 0,
	TI81xx_DEVICE_TYPE_SLAVE,
};

/* How the device is interfaced to the display. Like embedded sync or
 * discrete sync, and 8/10/12 bits etc */
 /* Clear the mapping between the displaycontroller and this data */

enum TI81xx_InterfaceFormat {
	TI81xx_DF_16BIT_EMB_SYNC = 0,
	TI81xx_DF_16BIT_DISC_SYNC,
	TI81xx_DF_24BIT_EMB_SYNC,
	TI81xx_DF_24BIT_DISC_SYNC,
	TI81xx_DF_30BIT_EMB_SYNC,
	TI81xx_DF_30BIT_DISC_SYNC,
};

/* What is the expected polarity for each of the sync, data and clock signals
 * for the external encoder connected
   Directly use the display controller rather than defining new.*/
enum TI81xx_polarity {
	TI81xx_POLARITY_HIGH = 0,
	TI81xx_POLARITY_LOW,
};

/* What is the output mode, from HDMI out. DVI for monitors or HDMI for TVs.*/
enum TI81xx_dvi_hmdi {
	TI81xx_MODE_DVI = 0,
	TI81xx_MODE_HDMI,
};

/* What is the output required in the externally connected encoder. Externally
 * connected encoder may required more than one output
   Directly use from display controller */
enum TI81xx_outputs {
	TI81xx_OUTPUT_SVIDEO = 0,
	TI81xx_OUTPUT_COMPOSITE,
	TI81xx_OUTPUT_COMPONENT,
	TI81xx_OUTPUT_HDMI,
	TI81xx_OUTPUT_DVI,
	TI81xx_OUTPUT_VGA,
};

/* Video timings for encoder. Will be mostly used for encoders which works
 * as master
   Change this to FVID2 timing and create one more structure for
   HDMI where video_code and video_mode will be passed */
struct TI81xx_video_timings {
	enum fvid2_standard	standard;
	/**< [IN] Standard for which to get the info.
	 For valid values see #fvid2_Standard. */
	unsigned int                    width;
	/**< Active video frame width in pixels. */
	unsigned int                    height;
	/**< Active video frame height in lines. */
	unsigned int                    scanformat;
	/**< Scan format of standard.
		For valid values see #TI81xx_ScanFormat. */
	unsigned int                    pixel_clock;
	/**< Pixel clock of standard in KHz. */
	unsigned int                    hfp;
	/**< Horizontal front porch. Same for both fields in case
	    of interlaced display */
	unsigned int                    hbp;
	/**< Horizontal back porch */
	unsigned int                    hsw;
	/**< Horizontal sync length. Same for both fields in case
	    of interlaced display */
	unsigned int                    vfp;
	/**< Vertical front porch for each field or frame */
	unsigned int                    vbp;
	/**< Vertical back porch for each field or frame */
	unsigned int                   vsw;
	/**< Vertical sync length for each field */
	/* unsigned int                  video_code;*/
	/**< video code. Typicall used for HDMI encoders
	Remove this from this structure.*/
	enum TI81xx_dvi_hmdi	      dvi_hdmi;
	/**< video mode. DVI  mode or HDMI mode. Typically all external
	 HDMI encoders support sending data in DVI/HDMI mode
	 Find a better name for this*/
	unsigned int                   reserved[4u];
	/**< For future use. Not used currently. */
};

/*this structure contain the information of the VENC to be registered
 external devcie can use this*/
struct ti81xx_venc_info {
	/*venc timing*/
	struct TI81xx_video_timings vtimings;
	/*venc enable or disable*/
	u32 enabled;
	/*current venc output info*/
	struct vps_dcoutputinfo outinfo;
};

/* This is the data structure with which all the external encoders will be
 * registered to the display sub-system of TI81xx
 */
struct TI81xx_display_driver {
	/* Display to which this panel is connected */
	enum TI81xx_displays 		display;
	/* Whether display works in master mode or slave mode.
	 * This will decide whether display needs the timing parameters to
	 * be programmed or not
	 */
	enum TI81xx_device_type		type;
	/* What is the data format that external encoder expects */
	enum TI81xx_InterfaceFormat		format;
	/* What is the polarity for the sync signals. Applicable to only
	 * Except clock and data other signals applicable to only discrete sync
	 */
	enum TI81xx_polarity 		clk;
	enum TI81xx_polarity		hsync;
	enum TI81xx_polarity 		vsync;
	enum TI81xx_polarity 		dataenable;
	enum TI81xx_polarity 		data;
	/* Keep one reserved paramters for all this function pointers for
	 * future enhancement if any */
	/* Enable the display panel */
	int (*enable) (void *);
	/* Disable the display panel */
	int (*disable) (void *);
	/* Suspend the display panel */
	int (*suspend) (void *);
	/* Resume the display panel */
	int (*resume) (void *);
	/* Set the timing on the panel */
	int (*set_timing) (struct TI81xx_video_timings *, void *);
	/* Get the timing on the panel */
	int (*get_timing) (struct  TI81xx_video_timings *, void *);
	/* Read EDID */
	int (*get_edid) (char *, void *);
	/* To set differnt outputs like HDMI, DVI, Component, Composite etc. */
	int (*set_output) (enum TI81xx_outputs,
			struct vps_dcoutputinfo *outinfo, void *);
	/* To get the current output set */
	int (*get_output) (enum TI81xx_outputs,
			struct ti81xx_venc_info *vencinfo, void *);
};


struct ti81xx_external_encoder{
	struct  TI81xx_display_driver *panel_driver;
	/* status information is maintained by the external encoder drivers.
	 * For the middle layer the status information is 'Read Only'.
	 */
	enum   ti81xx_display_status status;
	struct list_head list;
};


int TI81xx_register_display_panel(struct TI81xx_display_driver *panel_driver,
				struct ti81xx_venc_info *vencinfo);
int TI81xx_un_register_display_panel(
			struct TI81xx_display_driver *panel_driver);
