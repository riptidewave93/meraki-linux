/*
 * include/linux//ti81xx_hdmi.c
 *
 * Copyright (C) 2009 Texas Instruments
 *
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
 * Author: Sujith Shivalingappa <sujith.s@ti.com>
 *	: Varada Bellary <varadab@ti.com>, modified for new version (Mar 2011)
 *			of driver.
 *	: Sujith Shivalingappa <sujith.s@ti.com>, added CEC support.
 *	: Sujith Shivalingappa <sujith.s@ti.com>, added HDCP support.
 */

#ifndef __TI81XXHDMI_H__
#define __TI81XXHDMI_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/edid.h>

/* HDCP state
	When application query status of HDCP using IOCTL
	TI81XXHDMI_HDCP_GET_STATUS, depending on state of HDCP, one of the
	following values would be returned to applications. */
#define HDCP_STATE_DISABLED			0
#define HDCP_STATE_INIT				1
#define HDCP_STATE_AUTH_1ST_STEP		2
#define HDCP_STATE_AUTH_2ND_STEP		3
#define HDCP_STATE_AUTH_3RD_STEP		4
#define HDCP_STATE_AUTH_FAIL_RESTARTING		5
#define HDCP_STATE_AUTH_FAILURE			6


/* IOCTL definitions */
#define TI81XXHDMI_MAGIC            'N'
#define TI81XXHDMI_IOW(num, dtype)  _IOW(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IOR(num, dtype)  _IOR(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IOWR(num, dtype) _IOWR(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IO(num)          _IO(TI81XXHDMI_MAGIC, num)

/* IOCLT Supported by this driver */
#define TI81XXHDMI_START		TI81XXHDMI_IO(0)
#define TI81XXHDMI_STOP			TI81XXHDMI_IO(1)
#define TI81XXHDMI_GET_STATUS		TI81XXHDMI_IOR(2, unsigned char *)
#define TI81XXHDMI_READ_EDID		TI81XXHDMI_IOR(3, unsigned char *)

/* Used get the parsed EDID information of the connected sink.
 * Takes in a argument of type struct ti81xxhdmi_sink_edid_parsed.
 *
 * Ensure the sink EDID is explictly read by calling IOCTL TI81XXHDMI_READ_EDID
 */
#define TI81XXHDMI_GET_PARSED_EDID_INFO		TI81XXHDMI_IOWR(4, \
					struct ti81xxhdmi_sink_edid_parsed *)
/*
 * Used to enable the CEC block of HDMI.
 * Before CEC messages could be read / written OR registered, CEC block has to
 * be enabled. This takes no arguments.
 */
#define TI81XXHDMI_CEC_ACTIVATE		TI81XXHDMI_IOW(5, unsigned char *)

/* Used to register the device with the CEC n/w. The device requires to be
 * registered with CEC n/w to be able to send / receive messages.
 * Takes in a argument of type struct ti81xxhdmi_cec_register_dev
 */
#define TI81XXHDMI_CEC_REG_DEV		TI81XXHDMI_IOW(6, unsigned char *)

/* Used to read the received messages, for both unicast or broadcast messages
 * Takes in an argument of type struct ti81xxhdmi_cec_received_msg.
 * An error value of EAGAIN, indicates that are no messages received, try
 * later
 */
#define TI81XXHDMI_CEC_RECEIVE_MSG	TI81XXHDMI_IOR(7, unsigned char *)

/* Used to transmit the message described by ti81xxhdmi_cec_transmit_msg
 * Takes in a argument of type struct ti81xxhdmi_cec_transmit_msg
 */
#define TI81XXHDMI_CEC_TRANSMIT_MSG	TI81XXHDMI_IOWR(8, unsigned char *)

/* Used to enable HDCP.
 * Takes in a argument of type ti81xxhdmi_hdcp_ena_ctrl. Note that all devices
 * need not support HDCP. In case HDCP is not supported, an error value of
 * ENODEV if returned.
 *
 * Use this command only when hdmi is streaming video to a sink
 */
#define TI81XXHDMI_HDCP_ENABLE		TI81XXHDMI_IOW(9, \
					struct ti81xxhdmi_hdcp_ena_ctrl)

/* Used to disable HDCP process.
 * Applicable only when HDCP_ENABLE was called / HDCP autentication was in
 * progress.
 */
#define TI81XXHDMI_HDCP_DISABLE		TI81XXHDMI_IOW(10, unsigned char *)

/* Used to query the current step / status of HDCP process.
 * Returns one of the values defined below under "HDCP State". Takes in a
 * argument of type __u32
 */
#define TI81XXHDMI_HDCP_GET_STATUS	TI81XXHDMI_IOWR(11, __u32)

/* Used by application to wait for a state of HDCP process.
 *
 * Used to wait until Step 1/2 is reached. The application are expected
 * perform check BKSV / V` computation respectively and let the driver know,
 * if it can proceed with the autentication.
 *
 * This is a blocking call, the caller will be blocked until step 2 is reached.
 *
 */
#define TI81XXHDMI_HDCP_WAIT_EVENT	TI81XXHDMI_IOWR(12, \
					struct ti81xxhdmi_hdcp_wait_ctrl)

/* Used by application to let the driver know if it can proceed with the
 * autentication process.
 *
 * The application let the driver know, when BKSV has been verified, as a
 * to event TI81XXHDMI_HDCP_EVENT_STEP1 and V` has been computed for
 * TI81XXHDMI_HDCP_EVENT_STEP2. This will enable the driver to proceed with
 * autentication or not.
 *
 * The argument should be
 * In case of success 0x0 | TI81XXHDMI_HDCP_EVENT_STEP2
 * In case of failures -1 | TI81XXHDMI_HDCP_EVENT_STEP2
 * In the case of error, the driver would try to re-autenticate. If the number
 * of re-trys is exceeded. Autentication failure state would be entered.
 *
 * In case of success 0x0 | TI81XXHDMI_HDCP_EVENT_STEP1
 * In case of failures -1 | TI81XXHDMI_HDCP_EVENT_STEP1
 * In the case of error, the driver would try to re-autenticate. If the number
 * of re-trys is exceeded. Autentication failure state would be entered.
 */
#define TI81XXHDMI_HDCP_EVENT_DONE	TI81XXHDMI_IOWR(14, __u32)

/* TODO Not supported for now */
#if 0
#define TI81XX_HDMI_SET_MODE         	TI81XX_HDMI_IOW(6, enum ti81xxhdmi_mode)
#define TI81XX_HDMI_GET_MODE         	TI81XX_HDMI_IOR(7, enum ti81xxhdmi_mode)
#endif

/* HDCP events
 * Events that application could wait on and let the driver know if driver
 * could proceed to next step of autentication.
 */
#define TI81XXHDMI_HDCP_EVENT_STEP1		(1 << 0x0)
#define TI81XXHDMI_HDCP_EVENT_STEP2		(1 << 0x1)
#define TI81XXHDMI_HDCP_EVENT_EXIT		(1 << 0x2)

/*Local Macros */
#define HDMI_MAX_DETAILED_TIMINGS	(EDID_SIZE_BLOCK0_TIMING_DESCRIPTOR + \
					EDID_SIZE_BLOCK1_TIMING_DESCRIPTOR)
struct ti81xxhdmi_status {
	unsigned int is_hpd_detected;
	unsigned int is_hdmi_streaming;
};

struct ti81xxhdmi_sink_edid_parsed {

	/* Specifies the sinks EDID version */
	__u8 version;
	/* Specifies the sinks EDID revision */
	__u8 revision;

	/* Specifies if the connected sink supports HDMI or DVI interface.
		0 - Indicates DVI interface
		1 - Indicates HDMI interface
		In case of DVI mode num_dt, supported_cea_vic, audio_support &
			is_yuv_supported of this strucutre is in-valid. */
	__u8 is_hdmi_supported;

	/* Estabilished Video Timings.
			VESA ENHANCED EXTENDED DISPLAY
			IDENTIFICATION DATA STANDARD
			(Defines EDID Structure Version 1, Revision 4)
			To decode values present in timing_1, timing_2 &
			timing_3
		Table 3.18 - Established Timings I & II
	 */
	__u8 established_timings[3];

	/* Estabilished Video Timings.
			VESA ENHANCED EXTENDED DISPLAY
			IDENTIFICATION DATA STANDARD
			(Defines EDID Structure Version 1, Revision 4)
			To decode values present in timing_1, timing_2 &
			timing_3
		Table 3.18 - Established Timings I & II
	 */
	__u8 standard_timings[8];

	/* Detailed Video Timings
	 * 0 location is the preffered timing as declared by sink in block 0
	 * 1 location is the next timing as declared by sink in block 0
	 * 2 - 10 location - defined by detailed timings descriptor - defined in
	 *			extended blocks
	 *	EDID_SIZE_BLOCK0_TIMING_DESCRIPTOR + 8,
	 *  EDID_SIZE_BLOCK0_TIMING_DESCRIPTOR = 4 detailed timings in block 0,
	 *	2 are mandatory.
	 *
	 *	EDID_SIZE_BLOCK1_TIMING_DESCRIPTOR = maximum timings descriptors
	 *	in block 1
	 *
	 *	video_timings is defined in \include\linux\edid.h
	 */
	struct video_timings detailed_timings[HDMI_MAX_DETAILED_TIMINGS];

	/* Number of detailed timings descriptors parsed and number of valid
		detailed_timings entries */
	__u8 num_dt;

	/* Specifies the video resolutions supported, identified by
		Video Identification Code as defined in CEA 861 spec.

		struct image_format is defined in \include\linux\edid.h
	*/
	struct image_format supported_cea_vic;

	/* Specifies the various audio configurations supported
		struct audio_format is defined in \include\linux\edid.h */
	struct audio_format audio_support;

	/* YCbCr is supported is this is non-zero.
		A value of 1 - indicates YCbCr 422
		A value of 2 - indicates YCbCr 444 */
	__u32 is_yuv_supported;
};


/* The following lists the valid device types & logical address on the CEC n/w
	0 = Logical Address for TV
	1 = Logical Address for Recorder Device 1
	2 = Logical Address for Recorder Device 2
	3 = Logical Address for tuner 1
	4 = Logical Address for PLAY BACK DEVICE 1
	5 = Logical Address for AUDIO SYSTEM
	6 = Logical Address for tuner 2
	7 = Logical Address for tuner 3
	8 = Logical Address for PLAY BACK DEVICE 2
	09 = Logical Address for Recorder Device 3
	10 = Logical Address for tuner 4
	11 = Logical Address for PLAY BACK DEVICE 3
	14 = Logical Address for FREE USUAGE
	15 = Logical Address for Unregistered Broad Cast
*/

enum ti81xxhdmi_cec_command {
	ti81xxhdmi_cec_cmd_u_feature_abort = 0x00,
	ti81xxhdmi_cec_cmd_u_image_view_on = 0x04,
	ti81xxhdmi_cec_cmd_u_record_on = 0x09,
	ti81xxhdmi_cec_cmd_u_record_status = 0x0A,
	ti81xxhdmi_cec_cmd_u_record_off = 0x0B,
	ti81xxhdmi_cec_cmd_u_text_view_on = 0x0D,
	ti81xxhdmi_cec_cmd_u_record_tv_screen = 0x0F,
	ti81xxhdmi_cec_cmd_b_set_menu_language = 0x32,
	ti81xxhdmi_cec_cmd_b_u_standby = 0x36,
	ti81xxhdmi_cec_cmd_b_active_source = 0x82,
	ti81xxhdmi_cec_cmd_u_give_physical_address = 0x83,
	ti81xxhdmi_cec_cmd_b_report_physical_address = 0x84,
	ti81xxhdmi_cec_cmd_b_request_active_source = 0x85,
	ti81xxhdmi_cec_cmd_u_give_device_power_status = 0x8F,
	ti81xxhdmi_cec_cmd_u_report_power_status = 0x90,
	ti81xxhdmi_cec_cmd_u_cec_version = 0x9E,
	ti81xxhdmi_cec_cmd_u_get_cec_version = 0x9F,
	ti81xxhdmi_cec_cmd_u_abort = 0xFF,
	ti81xxhdmi_cec_cmd_b_routing_change = 0x80,
	ti81xxhdmi_cec_cmd_b_routing_info = 0x81,
	ti81xxhdmi_cec_cmd_b_set_stream_path = 0x86,
	ti81xxhdmi_cec_cmd_u_inactive_source = 0x9D,
	ti81xxhdmi_cec_cmd_u_tuner_step_increment = 0x05,
	ti81xxhdmi_cec_cmd_u_tuner_step_decrement = 0x06,
	ti81xxhdmi_cec_cmd_u_tuner_device_status = 0x07,
	ti81xxhdmi_cec_cmd_u_give_tuner_device_status = 0x08,
	ti81xxhdmi_cec_cmd_u_give_deck_status = 0x1A,
	ti81xxhdmi_cec_cmd_u_deck_status = 0x1B,
	ti81xxhdmi_cec_cmd_u_clear_external_timer = 0xA1,
	ti81xxhdmi_cec_cmd_u_set_external_timer = 0xA2,
	ti81xxhdmi_cec_cmd_u_clear_analog_timer = 0x33,
	ti81xxhdmi_cec_cmd_u_set_analog_timer = 0x34,
	ti81xxhdmi_cec_cmd_u_timer_status = 0x35,
	ti81xxhdmi_cec_cmd_u_play = 0x41,
	ti81xxhdmi_cec_cmd_u_deck_control = 0x42,
	ti81xxhdmi_cec_cmd_u_timer_cleared_status = 0x43,
	ti81xxhdmi_cec_cmd_u_user_control_pressed = 0x44,
	ti81xxhdmi_cec_cmd_u_user_control_released = 0x45,
	ti81xxhdmi_cec_cmd_u_give_osd_name = 0x46,
	ti81xxhdmi_cec_cmd_u_set_osd_name = 0x47,
	ti81xxhdmi_cec_cmd_u_set_osd_string = 0x64,
	ti81xxhdmi_cec_cmd_u_set_timer_program_title = 0x67,
	ti81xxhdmi_cec_cmd_u_system_audio_mode_request = 0x70,
	ti81xxhdmi_cec_cmd_u_give_audio_status = 0x71,
	ti81xxhdmi_cec_cmd_b_u_set_system_audio_mode = 0x72,
	ti81xxhdmi_cec_cmd_u_report_audio_status = 0x7A,
	ti81xxhdmi_cec_cmd_u_give_system_audio_mode_status = 0x7D,
	ti81xxhdmi_cec_cmd_u_system_audio_mode_status = 0x7E,
	ti81xxhdmi_cec_cmd_b_device_verndor_id = 0x87,
	ti81xxhdmi_cec_cmd_u_vendor_command = 0x89,
	ti81xxhdmi_cec_cmd_u_B_vendor_remote_button_down = 0x8A,
	ti81xxhdmi_cec_cmd_u_B_vendor_remote_button_up = 0x8B,
	ti81xxhdmi_cec_cmd_u_give_device_vendor_id = 0x8C,
	ti81xxhdmi_cec_cmd_u_menu_request = 0x8D,
	ti81xxhdmi_cec_cmd_u_menu_status = 0x8E,
	ti81xxhdmi_cec_cmd_u_get_menu_language = 0x91,
	ti81xxhdmi_cec_cmd_u_select_analogue_service = 0x92,
	ti81xxhdmi_cec_cmd_u_select_digital_service = 0x93,
	ti81xxhdmi_cec_cmd_u_set_digital_timer = 0x97,
	ti81xxhdmi_cec_cmd_u_clear_digital_timer = 0x99,
	ti81xxhdmi_cec_cmd_u_set_audio_rate = 0x9A,
	ti81xxhdmi_cec_cmd_u_B_vendor_comamnd_with_id = 0xA0
};

/* Defines the details required for registration of device to CEC n/w.
 * The HDMI driver would have discovered physical address, registering with
 * undiscovered / arbit physical address is illegal.
 *
 * CAUTION : The device should be registered on CEC n/w before messages could
 * be transmitted. On sink disconnect / HPD toggle, the registered device would
 * automatically be un-registered, the device has to be re-registered.
 */

struct ti81xxhdmi_cec_register_dev {
	/* Valid rage is from 0 - 15. Refer above for the relation between
	 * devices functionality and device number.
	 * If the device ID is 15, its treated as un-registered device. which
	 * is OK.
	 */
	__u32 dev_id;

	/* Flag to indicate if previously registered devices should be
	 * unregistered from the CEC n/w.
	 * == 0x1, clears previously registered devices.
	 * == 0x0, Do not clears previously registered devices.
	 *
	 * Could be used, when required to register the device for more than
	 * one functionality. i.e. if a device is capable of recording /
	 * playback, you might want to register as playback and recording
	 * device
	 */
	int clear_registered_dev;

	/* This variable would be updated by the driver to indicate the
	 * discovered physical address assigned to port, to which this device
	 * is connected to.
	 */
	__u8 physical_addr[2];
};


/* Defines the details that would be provided for every received message on the
 * CEC n/w. On error, the message is to be discarded. */
struct ti81xxhdmi_cec_received_msg {

	/* Defines who initiated this command*/
	__u8 initiator;
	/* Defines the command destined to device */
	__u8 destination;
	/* Defines the command */
	enum ti81xxhdmi_cec_command cmd;
	/* Defines the number of arguments associated with this command */
	__u8 no_args;
	/* Defines the arguments for the command */
	__u8 args[15];
	/* In case there were error while receiving this messages.
		0 for no error, error otherwise */
	__u32 errors;
};


/* Defines the required parameter for transmission of a message on CEC n/w and
 * provide the status of transmission attempt. On error, the transmission of
 * message to be assumed as un-successful.
 */
struct ti81xxhdmi_cec_transmit_msg {
	/* Defines the Device own ID, could also broad cast */
	__u8 initiator;
	/* Defines the command is intended to be received by,
	 * valid rage is 0 - 14
	 * Refer above for the relation between devices functionality and
	 * device number
	 */
	__u8 destination;
	/* Defines the command to be transmitted */
	enum ti81xxhdmi_cec_command cmd;
	/* Defines the number of arguments associated with the command */
	__u8 no_args;
	/* Defines the arguments for the command */
	__u8 args[15];
	/* Defines the whether the command is acked or nacked by the device
	 * specified in the destination.
	 * 1 indicates that command is acked by the device.
	 */
	__u8 acked_nacked;
	/* Defines how many times should a re-try be attempted, before giving
	 * up transmission
	 */
	__u8 no_retransmits;
	/* Defines the Flag to indicate ping command, in this case cmd,
	 * no_args, args are ignored
	 */
	__u8 send_ping;
	/* Defines whether any errors associated with the transmitting a
	 * command, 0x0 for normal operation
	 */
	__u32 errors;
};

/* HDCP Specifics */

/* HDCP key size in 32-bit words */
#define DESHDCP_KEY_SIZE 		160
#define MAX_SHA_DATA_SIZE		645
#define MAX_SHA_VPRIME_SIZE		20

/* Passed as an argument when enabling HDCP (TI81XXHDMI_HDCP_ENABLE) */
struct ti81xxhdmi_hdcp_ena_ctrl {
	/* Specify the number of times autentication should be attempted,
		before giving up autentication */
	int nb_retry;
};

/* Used to receive data from driver, required for V` computation */
struct hdcp_sha_in {
	__u8 data[MAX_SHA_DATA_SIZE];
	__u32 byte_counter;
	__u8 vprime[MAX_SHA_VPRIME_SIZE];
};

/* Control strucutre that would be used by application to wait for events.
	its expected that application initializes the event and
	member "data" would point to valid memory */
struct ti81xxhdmi_hdcp_wait_ctrl {
	__u32 event;
	struct hdcp_sha_in *data;
};

#endif /* End of #ifndef __TI81XX_HDMI_H__ */
