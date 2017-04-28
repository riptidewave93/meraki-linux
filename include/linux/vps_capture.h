/*
 *
 * Capture header for TI 81XX VPSS
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

#ifndef __LINUX_VPS_CAPTURE_H__
#define __LINUX_VPS_CAPTURE_H__

#ifdef __KERNEL__

#include <linux/vps_proxyserver.h>
#include <linux/fvid2.h>
#include <linux/vps.h>
#include <linux/vps_cfgVipParser.h>

/** \brief Capture instance - VIP0 - Port A  */
#define VPS_CAPT_INST_VIP0_PORTA (0u)

/** \brief Capture instance - VIP0 - Port B  */
#define VPS_CAPT_INST_VIP0_PORTB (1u)

/** \brief Capture instance - VIP1 - Port A  */
#define VPS_CAPT_INST_VIP1_PORTA (2u)

/** \brief Capture instance - VIP1 - Port B  */
#define VPS_CAPT_INST_VIP1_PORTB (3u)

/** \brief Capture instance - VIP ALL  */
#define VPS_CAPT_INST_VIP_ALL    (0xFFu)

/** \brief Capture instance - MAX instances  */
#define VPS_CAPT_INST_MAX        (4u)

/** \brief Maximun channels that can be captured per port */
#define VPS_CAPT_CH_PER_PORT_MAX       (16u)

/** \brief Maximun channels that can be captured in the system */
#define VPS_CAPT_CH_MAX       (VPS_CAPT_CH_PER_PORT_MAX*VPS_CAPT_INST_MAX)

/** \brief Maximum frames that can be queued per channel */
#define VPS_CAPT_FRAME_QUE_LEN_PER_CH_MAX  (16u)

/** \brief Maximum stream's that can be captured per channel

  Stream's corresponds to different types of output that are possible
  for a given capture input source.

  The stream output type is setup by user during FVID2_create()

  Example streams are,

  For a 1080P input source,
  Stream 0: YUV420 non-scaled output
  Stream 1: YUV422 scaled output
  Stream 3: RAW VBI output

  Refer to user guide for more details about what valid stream
  combinations are possible.
*/
#define VPS_CAPT_STREAM_ID_MAX (4u)

/** \brief Value to use for 'streamId' when required to do FVID2_queue
 to any stream

 Note, this value cannot be used as 'streamId' with FVID2_dequeue
*/
#define VPS_CAPT_STREAM_ID_ANY (0xFFFFu)

/**
 *  \brief Scalar ID to be used in the set scalar coefficient and set/get
 *  advance scalar params IOCTLS.
 *  Since this driver supports only one scalar per driver instance, the scalar
 *  could be identified by the instance handle. The below macro is used for
 *  initializing the scalar ID to a default value.
 */
#define VPS_CAPT_SCALAR_ID_DEFAULT      (0u)

/**
  * \brief Set scaler frame info
  *
  * This IOCTL is valid only when Scaler is enabled during create time
  * This IOCTL can be used for features like Down scaling, Pan, Crop
  *
  * Note, upscaling is not supported for capture path
  *
  * \param cmdArgs       [IN]  Vps_CaptScParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_VPS_CAPT_SET_SC_PARAMS       (VPS_CAPT_IOCTL_BASE + 0x0000u)

/**
  * \brief Set frame skip info
  *
  * This IOCTL can be used to control capture frame drop sequence,
  * Example, this IOCTL can be used to convert 60fps input source to 30fps
  * by dropping frames.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Vps_CaptFrameSkip *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_VPS_CAPT_SET_FRAME_SKIP      (VPS_CAPT_IOCTL_BASE + 0x0001u)

/**
  * \brief Get channel status
  *
  * This IOCTL can be used to get status about a specific channel, stream
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Vps_CaptChGetStatusArgs *
  * \param cmdArgsStatus [OUT] Vps_CaptChStatus *
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_VPS_CAPT_GET_CH_STATUS       (VPS_CAPT_IOCTL_BASE + 0x0002u)

/**
 * \brief Reset VIP0
 *
 * IMPORTANT: Only applicable to \ref VPS_CAPT_INST_VIP_ALL handle
 *
 * This IOCTL executes the VIP reset sequence for VIP0.
 *
 * This will reset both Port A as well as Port B for that VIP instance.
 *
 * This will reset all the VIP blocks including CSC, SC, CHR_DS
 * for that VIP instance.
 *
 * Make sure no module is being accessed in that VIP instance,
 * either in M2M path or capture path when this API is called.
 *
 * This API MUST be called in the following scenarios
 * - when in non-mux capture mode AND
 * - CHR_DS OR SC or CSC is enabled in capture driver path.
 *
 * If this API is not called in above scenarios then capture driver
 * may get hung up.
 *
 * This API must be called each time before starting capture in above
 * scenario's.
 *
 * This API need not be called in the following scenarios
 * - when in non-mux capture mode or multi-CH capture mode AND
 * - SC is disabled, CSC is disabled, CHR_DS is disabled in capture driver path.
 *
 * \param cmdArgs       [IN]  NULL
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return FVID2_SOK
*/
#define IOCTL_VPS_CAPT_RESET_VIP0          (VPS_CAPT_IOCTL_BASE + 0x0003u)

/**
 * \brief Reset VIP1
 *
 * Same as \ref IOCTL_VPS_CAPT_RESET_VIP0 except that it operates on VIP1
 *
 * \param cmdArgs       [IN]  NULL
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return FVID2_SOK
*/
#define IOCTL_VPS_CAPT_RESET_VIP1          (VPS_CAPT_IOCTL_BASE + 0x0004u)

/**
 * \brief Print detailed capture information
 *
 * IMPORTANT: Only applicable to \ref VPS_CAPT_INST_VIP_ALL handle
 *
 * This is meant to be used by driver developer for internal debugging purposes
 * It should not be used by user.
 *
 * \param cmdArgs    [IN] u32 Execution time in msecs
 *
 * \return FVID2_SOK
*/
#define IOCTL_VPS_CAPT_PRINT_ADV_STATISTICS (VPS_CAPT_IOCTL_BASE + 0x0005u)

/**
 *  \brief Check VIP0, VIP1 overflow status and return the status
 *
 *  \param cmdArgs       [IN]  NULL
 *  \param cmdArgsStatus [OUT] Vps_CaptOverFlowStatus *
 *
 *  \return FVID2_SOK
 */
#define IOCTL_VPS_CAPT_CHECK_OVERFLOW       (VPS_CAPT_IOCTL_BASE + 0x0006u)

/**
 *  \brief If the overflow flag is set, resets VIP
 *
 *  This API does the following
 *  If VIP output FIFO is overflowed
 *    - then its stops the driver on PortA, PortB
 *    - reset the VIP instance
 *    - starts driver of PortA, PortB
 *
 *  \param cmdArgs       [IN]  const Vps_CaptOverFlowStatus *
 *  \param cmdArgsStatus [OUT] NULL
 *
 *  \return FVID2_SOK
 */
#define IOCTL_VPS_CAPT_RESET_AND_RESTART    (VPS_CAPT_IOCTL_BASE + 0x0007u)

/**
 *  \brief Flush capture driver and dequeue all frames including those that are not captured
 *
 *  This API can be called only when driver is in stopped state.
 *  Driver is in stopped state when,
 *  - FIVD2_stop() is called
 *  - FVID2_create() is called and FVID2_start() is not called.
 *
 *  This will return frames from all streams for a given capture handle.
 *  If this IOCTL is called with global handle then this will return
 *  frames from all capture handles.
 *
 *  Since maximum FVID2_MAX_FVID_FRAME_PTR frames can be returned at a time.
 *  This function should be called in a loop until FVID2_FrameList.numFrames = 0
 *  is returned in order to get back all the frames from the capture driver.
 *
 *  In case capture is in running state this function will return error.
 *
 *  This IOCTL will return capture frames, non-captured frames, as well
 *  as all frames held inside the hardware.
 *
 *  \param cmdArgs       [IN]  NULL
 *  \param cmdArgsStatus [OUT] FVID2_FrameList
 *
 *  \return FVID2_SOK
 */
#define IOCTL_VPS_CAPT_FLUSH                 (VPS_CAPT_IOCTL_BASE + 0x0008u)

/**
 * \brief User provided buffer to be used by driver when dropping data
 *
 * WARNING: Experimental IOCTL for test. Users should NOT call this IOCTL.
 *
 *  \param cmdArgs       [IN]  Void * pointer to data buffer
 *
 *  \return FVID2_SOK
*/
#define IOCTL_VPS_CAPT_DROP_DATA_BUFFER      (VPS_CAPT_IOCTL_BASE + 0x0009u)

/**
 * \brief Reset VIP Port
 *
 * IMPORTANT: Cannot be used with ref \ref VPS_CAPT_INST_VIP_ALL handle
 *
 * This IOCTL executes the VIP reset sequence for VIP Port associated
 * with the given handle.
 *
 * This will reset all the VIP blocks including CSC, SC, CHR_DS used
 * for that VIP instance.
 *
 * \param cmdArgs       [IN]  NULL
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return FVID2_SOK
*/
#define IOCTL_VPS_CAPT_RESET_VIP           (VPS_CAPT_IOCTL_BASE + 0x000Au)
/**
 * \brief Field or Frame capture for interlaced input.
 * This IOCTL allows user to capture fields or frames for interlaced input.
 * For frame capture fields can be line interleaved or it can be
 * separate. This IOCTL is only valid
 *  \param cmdArgs          [IN]  const Vps_CaptStoragePrms *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return FVID_SOK        return FVID_SOK on success else a proper failure
 *                          FVID2 failure code.
 */
#define IOCTL_VPS_CAPT_SET_STORAGE_FMT      (VPS_CAPT_IOCTL_BASE + 0x000Bu)

/**
 * \brief Field or Frame capture for interlaced input.
 * This IOCTL allows user to capture fields or frames for interlaced input.
 * For frame capture fields can be line interleaved or it can be
 * separate. This IOCTL is only valid
 *  \param cmdArgs          [IN]  const Vps_CaptStoragePrms *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return FVID_SOK        return FVID_SOK on success else a proper failure
 *                          FVID2 failure code.
 */
#define IOCTL_VPS_CAPT_GET_STORAGE_FMT      (VPS_CAPT_IOCTL_BASE + 0x000Cu)

/**
 * \brief Caputre driver queues back error frame to driver empty queue and
 * does not return err frame to application. With this IOCTL capture driver
 * will return err frame to application with error status flagged. Instead of
 * driver queing it back to empty queue
 *  \param cmdArgs          [IN]  const enable *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return FVID_SOK        return FVID_SOK on success else a proper failure
 *                          FVID2 failure code.
 */
#define IOCTL_VPS_CAPT_RETURN_ERR_FRM      (VPS_CAPT_IOCTL_BASE + 0x000Du)

/**
 * \brief Caputre driver gives call back continously till the frame is dequeued
 * from the output queue. This ioctl allows application to have callback once
 * per completed frame. This will ensure that application will receive N call
 * backs for N queued frames and not more than that.  Application will have
 * to call N time dequeue for N call backs This functionality is
 * required by high level operation system like V4L2 capture.
 *  \param cmdArgs          [IN]  const Vps_CaptOneCallBackPerFrm *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return FVID_SOK        return FVID_SOK on success else a proper FVID2
 *                          failure code.
 */
#define IOCTL_VPS_CAPT_ONE_CB_PER_FRM      (VPS_CAPT_IOCTL_BASE + 0x000Eu)

/**
 *  \brief Sets the VIP parser crop configuration for a channel.
 *
 *  This IOCTL allows the user to configure the VIP parser crop feature for a
 *  port.
 *  This is only available for TI816x ES 2.0 and TI814x ES2.1 versions or above.
 *
 *  \param cmdArgs          [IN]  const Vps_CaptVipCropParams *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return Returns FVID_SOK on success else a proper failure FVID2
 *  failure code.
 */
#define IOCTL_VPS_CAPT_SET_VIP_CROP_CFG     (VPS_CAPT_IOCTL_BASE + 0x000Fu)

/**
 *  \brief Gets the VIP parser crop configuration for a channel.
 *
 *  This IOCTL allows the user to get the already set VIP parser crop
 *  parameters of a port.
 *  This is only available for TI816x ES 2.0 and TI814x ES2.1 versions or above.
 *
 *  \param cmdArgs          [IN]  Vps_CaptVipCropParams *
 *  \param cmdArgsStatus    [OUT] NULL
 *
 *  \return Returns FVID_SOK on success else a proper failure FVID2
 *  failure code.
 */
#define IOCTL_VPS_CAPT_GET_VIP_CROP_CFG     (VPS_CAPT_IOCTL_BASE + 0x0010u)

/* @} */

/**
 * \brief Video capture operation mode
*/
enum Vps_CaptVideoCaptureMode {

	VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC = 0,
	/**< Single Channel non multiplexed mode */

	VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_LINE_MUX_EMBEDDED_SYNC,
	/**< Multi-channel line-multiplexed mode */

	VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_PIXEL_MUX_EMBEDDED_SYNC,
	/**< Multi-channel pixel muxed */
	VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_HSYNC_VBLK,
	/**< Single Channel non multiplexed discrete sync mode with HSYNC and
	VBLK as control signals. */
	VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_HSYNC_VSYNC,
	/**< Single Channel non multiplexed discrete sync mode with HSYNC and
	VSYNC as control signals. */
	VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VBLK,
	/**< Single Channel non multiplexed discrete sync mode with ACTVID and
	VBLK as control signals. */
	VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VSYNC,
	/**< Single Channel non multiplexed discrete sync mode with ACTVID and
	VBLK as control signals. */
	VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_LINE_MUX_SPLIT_LINE_EMBEDDED_SYNC,
	/**< Multi-channel line-multiplexed mode - split line mode */

	VPS_CAPT_VIDEO_CAPTURE_MODE_MAX
	/**< Maximum modes */
};

/**
 * \brief Video interface mode
*/
enum Vps_CaptVideoIfMode {

	VPS_CAPT_VIDEO_IF_MODE_8BIT = 0,
	/**< Embedded sync mode:  8bit - BT656 standard  */

	VPS_CAPT_VIDEO_IF_MODE_16BIT,
	/**< Embedded sync mode:  16bit - BT1120 standard  */

	VPS_CAPT_VIDEO_IF_MODE_24BIT,
	/**< Embedded sync mode:  24bit */

	VPS_CAPT_VIDEO_IF_MODE_MAX
	/**< Maximum modes */
};

/**
	\brief Output data max width
	This enum can be used to limit the width of the output
	data that is captured in to the user buffer.
	Any additional pixels coming from the source are discarded.
  */
enum  Vps_CaptMaxOutWidth {
	VPS_CAPT_MAX_OUT_WIDTH_UNLIMITED = 0,
	/**< Do not limit the output width, captured
	  whatever is coming from the source */
	VPS_CAPT_MAX_OUT_WIDTH_352_PIXELS = 4,
	/**< Limit height to 352 pixels */
	VPS_CAPT_MAX_OUT_WIDTH_768_PIXELS = 5,
	/**< Limit height to 768 pixels */
	VPS_CAPT_MAX_OUT_WIDTH_1280_PIXELS = 6,
	/**< Limit height to 1280 pixels */
	VPS_CAPT_MAX_OUT_WIDTH_1920_PIXELS = 7
	/**< Limit height to 1920 pixels */
};

/**
  \brief Output data max height

  This enum can be used to limit the height of the output data that is
  captured in to the user buffer.

  By default HW will capture whatever active data comes from the external
  video source. The width typically remains fixed from the external source.
  However, in some cases  external video sources may generate spurious frames
  of active data which have random height. This can cause user buffers to
  overflow if the height of such a frame is more that the allocated buffer
  height.

  To avoid such buffer overflow, users should limit the maximum output data
  height. This a HW feature and limits that can be set are a few fixed values
  as shown below.

  Any additional lines coming from the source are discarded.
 */
enum Vps_CaptMaxOutHeight {
	VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED = 0,
	/**< Do not limit the output width, captured whatever
	  is coming from the source */

	VPS_CAPT_MAX_OUT_HEIGHT_288_LINES = 4,
	/**< Limit height to 288 lines */

	VPS_CAPT_MAX_OUT_HEIGHT_576_LINES = 5,
	/**< Limit height to 576 lines */

	VPS_CAPT_MAX_OUT_HEIGHT_720_LINES = 6,
	/**< Limit height to 720 lines */

	VPS_CAPT_MAX_OUT_HEIGHT_1080_LINES = 7
	/**< Limit height to 1080 lines */

};

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Capture window information
 *
 * Only valid when Vps_CaptOutInfo.scEnable is or can be TRUE
 *
 * Note, upscaling is not supported for capture path
 */
struct Vps_CaptScParams {
	u32 inWidth;
	/**< Input source width, MUST be >= actual or expected
	 * video source input width
	 */

	u32 inHeight;
	/**< Input source height, MUST be >= actual or expected
	  video source input width

	  Height should be field height in case source is interlaced \n
	  Height should be frame height in case source is progressive \n
	 */

	u32 outWidth;
	/**< Scaler output width, in pixels */

	u32 outHeight;
	/**< Scaler output height, in lines */

	struct vps_cropconfig inCropCfg;
	/**< Scalar input crop config
	 */

	u32 inScanFormat;
	/**< Input source scan format - interlaced or progressive.
	  For valid values see #FVID2_ScanFormat. */

	void *scConfig;
	/**< Scaler config, set NULL to setup default parameters */

	void *scCoeffConfig;
	/**< Scaler co-eff config, set NULL to setup default co-effs */
	u32                  enableCoeffLoad;
	/**< Enable scaler coefficient load during IOCTL_VPS_CAPT_SET_SC_PARAMS
	  If this is set to TRUE, it may result in the VIP instance getting
	  stopped, reset, and restarted to load new coefficients as per the
	  provided new scaling factor. This may result in some frame loss.
	  The scaler coefficients are loaded only if there is a change in either
	  the horizontal or vertical scaling set. The best scaler coefficients
	  to  be used are determined internally when scCoeffConfig in this
	  structure is set to NULL.
	  If the user has provided scaler coefficients or coefficient sets to be
	  used, these are used instead of internally calculating the best scaler
	  coefficients. */


};

/**
 * \brief Capture output format information
 */
struct Vps_CaptOutInfo {

	u32 dataFormat;
	/**< Output Data format, valid options are
	  FVID2_DF_YUV422I_YUYV,
	  FVID2_DF_YUV420SP_UV,
	  FVID2_DF_YUV422SP_UV,
	  FVID2_DF_RGB24_888,
	  FVID2_DF_RAW_VBI.
	  For valid values see #FVID2_DataFormat.
	 */

	u32 memType;
	/**< Tiled or non-tiled memory selection for output.
	  For valid values see #Vps_VpdmaMemoryType.

	  For FVID2_DF_YUV422I_YUYV, FVID2_DF_RGB24_888, FVID2_DF_RAW_VBI
	  output, always VPS_VPDMA_MT_NONTILEDMEM will be used internally.

	  User can control tiled or non-tiled for FVID2_DF_YUV420SP_UV,
	  FVID2_DF_YUV422SP_UV
	 */

	u32 pitch[FVID2_MAX_PLANES];
	/**< Pitch in bytes between two lines.
	  Pitch can be specified separately for every plane.
	 */
	u32                  maxOutWidth;
	/**< Set limit on the max possible width of the output frame
	  For valid values see #Vps_CaptMaxOutWidth */
	u32 maxOutHeight;
	/**< Set limit on the max possible height of the output frame
	  For valid values see #Vps_CaptMaxOutHeight
	 */

	u32 scEnable;
	/**< TRUE: Use scaler before writing video data to memory,
	 * FALSE: Scaler is not used in capture path
	 * MUST be FALSE for line-multiplexed, pixel multiplexed modes  */

	u32 subFrameModeEnable;
	/**< TRUE: subframe based capture is enabled
FALSE: subframe based capture is disabled

Must be FALSE for multi-channel modes
	 */

	u32 numLinesInSubFrame;
	/**< Number of lines per subframes for this output stream

	  MUST be multiple of the stream output size.
	  Not valid, ignored for ancillary data stream

	  SubFrame callback gets called after every numLinesInSubFrame
	  for every output stream, except ancillary data stream

	  Ignored when subFrameModeEnable = FALSE
	 */

	FVID2_SubFrameCbFxn subFrameCb;
	/**< SubFrame callback, is called for every subframe of this
	  output stream

	  Ignored when subFrameModeEnable = FALSE
	 */

};

/**
 * \brief Capture driver create arguments, used when calling FVID2_create()
*/
struct Vps_CaptCreateParams {

	u32 videoCaptureMode;
	/**< Video capture mode. For valid values see #Vps_CaptVideoCaptureMode
	 */

	u32 videoIfMode;
	/**< Video interface mode. For valid values see #Vps_CaptVideoIfMode */

	u32 inDataFormat;
	/**< Input source color data format, valid values are given below \n
	  FVID2_DF_YUV422P, ( 'P' is not relavent for input data format) \n
	  FVID2_DF_YUV444P, ( 'P' is not relavent for input data format) \n
	  FVID2_DF_RGB24_888.
	  For valid values see #FVID2_DataFormat.
	 */

	u32 periodicCallbackEnable;
	/**< TRUE: User callback passed during FVID2 create is called
	  periodically at a fixed duration of about 8msecs
	FALSE: User callback passed during FVID2 create is called only
	if one or more frames are captured in any of the streams,
	channels belonging to this handle
	 */

	u32 numCh;
	/**< Number of channel for multi-channel modes,

	  Must be 1 for
	  VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC
	 */

	u32 numStream;
	/**< Number of streams to capture, MUST be <= VPS_CAPT_STREAM_ID_MAX  */

	struct Vps_CaptOutInfo outStreamInfo[VPS_CAPT_STREAM_ID_MAX];
	/**< Output format for each stream */

	struct Vps_CaptScParams scParams;
	/**< Scaler parameters to use when
	  Vps_CaptCreateParams.outStreamInfo[x].scEnable = TRUE

	  Parameters are ignored when outStreamInfo[x].scEnable = FALSE
	 */

	struct Vps_VipConfig *vipParserInstConfig;
	/**< VIP Parser instance config, set NULL to ignore these parameters */

	struct Vps_VipPortConfig *vipParserPortConfig;
	/**< VIP Parser port config, set NULL to ignore these parameters */

	struct vps_cscconfig *cscConfig;
	/**< CSC config, set NULL to ignore these parameters */

	u32 channelNumMap[VPS_CAPT_STREAM_ID_MAX][VPS_CAPT_CH_PER_PORT_MAX];
	/**< Channel Number to assign to each channel and stream of this handle.
	  This is used during FVID2_queue(), FVID2_dequeue().
	  Channel number must be unique across the whole system.
	  Users can use Vps_captMakeChannelNum() to generate a
	  system unique channel number
	 */

	u32 inScanFormat;
	/**< Input source scan format - interlaced or progressive.
	  For valid values see #FVID2_ScanFormat. */
	u32                  muxModeStartChId;
	/**< Start channel ID in pixel or line mux mode. Used to add an
	  offset to  start channel mapping. This will be used when the
	  decoder start channel ID is other than 0.
	  For example some decoder's channel 0 CHID starts from 4 instead of 0.
	  This is valid only in multi-channel mode and is ignored in single
	  channel or other modes. */

};

/**
  \brief Capture driver create status. Returned after calling FVID2_create()
*/
struct Vps_CaptCreateStatus {
	int retVal;
	/**< Create status, FVID2_SOK on success, else failure */

};

/**
  \brief Capture driver run-time parameters

  - This structure is returned by capture driver when FVID2_dequeue()
    is called by application
  - This structure is returned as part of FVID2_Frame.perFrameCfg
  - Application should make sure FVID2_Frame.perFrameCfg is set to a valid
    Vps_CaptRtParams pointer when queue-ing the frame back to capture driver
    \code
    Vps_CaptRtParams rtParams;

    pFrame->perFrameCfg = &rtParams;
    \endcode
  - Alternatively, user can pass back the same FVID2_Frame pointer without
    modifying FVID2_Frame.perFrameCfg
*/
struct Vps_CaptRtParams {

	u32 captureOutWidth;
	/**< Capture data width in pixels */

	u32 captureOutHeight;
	/**< Capture data height in lines */

};

/**
  \brief Capture Channel Get Status IOCTL input arguments
*/
struct Vps_CaptChGetStatusArgs {

	u32 channelNum;
	/**< [IN] Channel number for which status is requested
	 */

	u32 frameInterval;
	/**< [IN] Expected interval in units of timer ticks between frames.

	  Vps_CaptChStatus.isVideoDetected is FALSE if no frame is captured
	  for a duration of 'frameInterval x 2', else
	  Vps_CaptChStatus.isVideoDetected is TRUE
	 */

};

/**
  \brief Capture Channel Get Status IOCTL result
*/
struct Vps_CaptChStatus {

	u32 isVideoDetected;
	/**< [OUT] TRUE: Video detected at this channel,
FALSE: Video not detected at this channel
	 */

	u32 outWidth;
	/**< [OUT] Capture data output width in pixels */

	u32 outHeight;
	/**< [OUT] Capture data output height in lines */

	u32 droppedFrameCount;
	/**< [OUT] Number of frame's dropped by driver due to unavailability
	  of buffer from application  */

	u32 captureFrameCount;
	/**< [OUT] Number of frame's captured by driver into the buffer
	  provided by application
	 */

};

/**
  \brief Frame Skip parameters
*/
struct Vps_CaptFrameSkip {

	u32 channelNum;
	/**< Channel number of which frame skip will be applied
	 */

	u32 frameSkipMask;
	/**< Frame Skip Mask (bit0..bit29) bitN = 1 Skip frame,
	  bitN = 0 DO NOT skip frame

	  Example, 0x0 for no frame skip, 0x2AAAAAAA for skipping alternate
	  frames
	 */

} ;

/**
  \brief VIP overflow status structure.
*/
struct Vps_CaptOverFlowStatus {
	u32          isPortOverFlowed[VPS_CAPT_INST_MAX];
	/**< Flag to indicate whether a port overflowed or not. */
};

/**
 \brief Structure for VIP Frame based capture.
*/
struct Vps_CaptStoragePrms {
	u32          chNum;
	/**< Channel number for which this configuration holds good. ChNum is
	 *   ignored for now. Configuration will take effect for all the channels
	 *   of the object for now
	 */
	u32          bufferFmt;
	/**< Captured interlaced input as either fields or frame. For valid
	  values see #FVID2_BUFFER_FMT
	 */
	u32          fieldMerged;
	/**< Whether to merged the captured fields or to be in seperate planes.
	 *   If this variable is True both the fields of the captured frames
	 *   will be merged. If its false EvenField will be in top plane and odd
	 *   field will be bottom plane. User has to populate address for both
	 *   the fields in #FVID2_Frame structure for frame based capture.
	 */
};
/**
 *  \brief Structure for VIP crop configuration.
 */
struct Vps_CaptVipCropParams {
	u32                  channelNum;
	/**< Channel number for which this configuration is be applied. This is
	 *  not used in the current implementation and should be set to zero by
	 *  the application. */
	u32                  vipCropEnable;
	/**< Enable or disable VIP crop feature. */
	struct vps_cropconfig          vipCropCfg;
	/**< VIP crop configuration - crop width, crop height,
	  startX and startY. */
} ;

struct Vps_CaptReturnErrFrms {
	u32          enable;
	/**< Enable/disable returning of error frames to application. */
};

struct Vps_CaptOneCallBackPerFrm {
	u32          enable;
	/**< Enable single callback for the completed frames. */
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Get capture instance ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param channelNum [IN] channelNum
 *
 * \return instance ID ( 0 .. VPS_CAPT_INST_MAX-1 )
*/
static inline u32 Vps_captGetInstId (u32 channelNum)
{
	return channelNum / (VPS_CAPT_CH_PER_PORT_MAX * VPS_CAPT_STREAM_ID_MAX);
}

/**
 * \brief Get capture stream ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param channelNum [IN] channelNum
 *
 * \return stream ID ( 0 .. VPS_CAPT_STREAM_ID_MAX-1 )
*/
static inline u32 Vps_captGetStreamId (u32 channelNum)
{
	u32 value;

	value = channelNum % (VPS_CAPT_CH_PER_PORT_MAX * VPS_CAPT_STREAM_ID_MAX);
	value /= VPS_CAPT_CH_PER_PORT_MAX;

	return value;
}

/**
 * \brief Get capture channel ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
* \param channelNum [IN] channelNum
 *
 * \return channel ID (0 .. VPS_CAPT_CH_PER_PORT_MAX-1 )
*/
static inline u32 Vps_captGetChId (u32 channelNum)
{
	return channelNum % (VPS_CAPT_CH_PER_PORT_MAX);
}

/**
 * \brief Make a system unique channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param instId    [IN] VIP Instance ID
 * \param streamId  [IN] Stream ID
 * \param chId      [IN] Channel ID
 *
 * \return channelNum
*/
static inline u32 Vps_captMakeChannelNum (u32 instId, u32 streamId,
		u32 chId)
{
	return instId * VPS_CAPT_CH_PER_PORT_MAX * VPS_CAPT_STREAM_ID_MAX
		+ streamId * VPS_CAPT_CH_PER_PORT_MAX + chId;
}
#endif /*#ifdef __KERNEL__ */
#endif /* #ifdef __LINUX_VPS_CAPTURE_H__ */
