/*
 ********
 ***??***
 ********
 */

#ifndef __SC_CONFIG_RC4551_H__
#define __SC_CONFIG_RC4551_H__

#define SC_OEM_NAME				"neutral"
#define SC_OEM_FIXED_NAME		"neutral"
#define SC_MODEL_NAME			"RC4551"
#define SC_MODEL_FIXED_NAME	    "RC4551"
#define _OEM_0009_  			/* neutral */
#define OEM_HOSTNAME_PREFIX "SC"
#define _OEM_0009_0032_			/* RC4551 */
#define QSET_INTERNAL_PID		0x0011
#define SC_HW_ID_0		'A'
#define SC_HW_ID_1		'R'
#define SC_HW_ID_2		'P'
#define SC_CUSTOM_ID_0	0x00
#define SC_CUSTOM_ID_1	0x09

#define _ONE_FW_MULTI_MODEL_
#ifdef _ONE_FW_MULTI_MODEL_
#define SC_MODEL_NAME_0			SC_MODEL_NAME
#define SC_MODEL_NAME_1			"RC4551"
#define SC_MODEL_FIXED_NAME_0			SC_MODEL_FIXED_NAME
#define SC_MODEL_FIXED_NAME_1			"RC4551"
#define SC_HW_ID1_0		'A'
#define SC_HW_ID1_1		'R'
#define SC_HW_ID1_2		'P'
#endif

#define RD_FW_VERSION_INDICATOR     "."
//#define _SC_SUPPORT_RESOLUTION_ASPECT_RATION_SETTING_
#define _SC_SUPPORT_16BY6_RATION_
//#define _DUAL_SENSOR_
//#define _SUPPORT_HEATER_
//oc431 have utility downloader
#define _DOWNLOAD_
#define _SUPPORT_HTTP_EVENT_TRIGGER_
#define _SUPPORT_HTTP_POST_
#define _SC_AUTO_UPGRADE_
#ifdef _SC_AUTO_UPGRADE_
#define _SC_AUTO_UPGRADE_NEUTRAL_SPEC_
#endif

#define _SPECIAL_MEDIA_PUSH_SUPPORT_
#define _SC_SUPPORT_RTSP_PUSH_
#define _RTSP_PUSH_SUPPORT_CHANNEL_

#define _AUTO_HTTP_SECOND_PORT_WITH_IP_
#ifdef _AUTO_HTTP_SECOND_PORT_WITH_IP_
#define AUTO_HTTP_SECOND_PORT_BASE	60000
#endif
#define _UPNPCP_AUTO_RETRY_SECOND_PORT_
#ifdef _UPNPCP_AUTO_RETRY_SECOND_PORT_
#define UPNPCP_AUTO_RETRY_HTTP_SECOND_PORT_BASE		60300
#define UPNPCP_AUTO_RETRY_HTTPS_SECOND_PORT_BASE	61300
#define UPNPCP_AUTO_RETRY_SECOND_PORT_RANGE			699		// 0~699
#endif

#define _SC_NEED_FLIP_MIRROR_IMAGE_

#define _NAND_FLASH_
//#define _SPI_NOR_FLASH_
#define _SC_CONSOLE_USE_UART0_

//#define _CMOS_OVAUTO_DETECT_

#ifndef _WEBUPG_IN_LOADER_
#define _WEBUPG_IN_LOADER_
#endif

#define _CHANGE_FRATE_CLOSE_SESSION_
#define _SC_GET_UPTIME_OF_DAY_

//#define _AUDIO_TYPE_AIC23_
#define _AUDIO_HAS_SPEAK_OUT_
#define _AUDIO_HAS_MIC_IN_
//#define _SC_HAS_SERIAL_

#define _HAS_IPFILTER_
//#define _HAS_IOPORT_

#define _SC_HAS_ONE_GROUP_IO_

#define _SUPPORT_PPPOE_
#define _SC_HTTPS_SUPPORT_
#ifdef _SC_HTTPS_SUPPORT_  
#define _SC_HTTPS_CA_SUPPORT_  
//#define _SC_GET_GROUP_ONLY_SHOW_PASSWORD_VIA_HTTPS_
#endif
#define _VIDEO_IN_IS_CMOS_
//#define _ANALOG_OUT_SUPP_

//#define _SC_QSET1_ENCRYPT_DATA_ //_NC_QSET_VER_1_		// Use the TEA to encrypt the Data

// user can set ntp update schedule 
#define _USER_SETNTP_

#define _SUPPORT_UPNPIGD_CP_

#define _SC_BACKDOOR_FOR_FACTORY_

#define _S35390A_RTC_
//#define _RTC_UNIVERSAL_INTERFACE_
//#define _HAS_IRIS_
#define _DN_ALWAYSDN_SUPP_
//#define _LAN_TYPE_TI_EMAC
/*
// 7-bit to control the dst, others is the time-zone index
#define _SC_QSET_UNION_TIMEZONE_AND_DAYLIGHT_
#define _SC_QSET_CHANGE_TIMEZONE_TRIGGER_NTP_
*/
#define _SC_SUPPORT_AUTH_INFO_IN_URL_WITH_BASE64_STRING_

#define _SC_GET_UPG_TIMER_FROM_CGI_
#define _SC_AUTHORIZATION_VARIABLE_

// Can config the SMTP port by UI
#define _SC_EMAIL_SMTP_PORT_

#define MAXCONNECTIONS			10

//#define _SC_DEVICE_NAME_DEFAULT_IS_NULL_

// Keep the original ip address when dhcpc is restarted
#define _SC_DHCP_KEEP_OLD_IP_
//#define _SC_DHCPC_AUTO_IP_		// Need we add the AUTO ip feature
#define _SC_DDNS_SUPPORT_1_2_3_5_
#define _SC_DDNS_MUST_UPDATE_AFTER_CGI_CHG_

#define _SC_IPADDRESS_IN_4FIELDS_

#define _SC_DST_CHANGE_AUTO_
#define _SC_DDNS_UPDATE_WHEN_BOOT_
#define _SC_DDNS_UPDATE_WHEN_CHANGE_IP_

// No the DDNS update-start time input feature
//#define _SC_DDNS_NO_START_UPDATE_TIME_

// To display more information about the wireless
#define _SC_WIRELESS_MORE_INFO_

//#define _SC_WIRELESS_CAN_SET_BG_MODE_
#define _SC_WIRELESS_WEPKEY_HAS_ASCII_MODE_

// Make sure that only one administrator can config the server at one time
// and There has a idle timeout
#define _SC_WEB_SERVER_ADM_TIMEOUT_

// Can control the alert mail whether has the attachment
#define _SC_ALERT_MAIL_SWITCH_ATTCHMENT_

//#define _SC_ALERT_SCHEDULE_DIS_ENABLE_
//#define _SC_ALERT_HAS_DIFF_LED_
#define _SC_ALERT_NO_FTP_CGI_
#define _SC_ALERT_NO_FTP_

// Only support one user ...
//#define _SC_ONLY_ONE_USER_
// User must login even access any page.
//#define _SC_USER_MUST_LOGIN_

// another solution in the CGI alerts.htm->schedule_mt/access/per.htm
//#define _SC_SCHEDULE_SAVEALL_SOLUTION_
#define _SC_SCHEDULE_CAN_MODIFY_

//#define _SC_SUPPORT_PERIODIC_

//#define _SC_TIME_FORMAT_AMPM_

#define _SC_DISPLAY_DNS_IP_IN_UI_

#define _SC_PROBE_IP_BY_ARP_IN_UI_

#define _SC_ENHANCE_VIDEO_640_PERFORMANCE_

#define _SC_COLOR_BALANCE_HAS_BW_

//#define _SC_CHECK_PORT_USED_OR_NOT_


//Only support one administrator at same time and support administrator timeout
//#define	_SC_SUPPORT_AUTO_LOGOUT_

//support view video schedule.
#define	_SC_SUPPORT_SCHEDULE_CHECK_

//#define _SC_NO_USER_TYPE_

//#define _SUPPORT_PIR_MODE_
#ifdef _SUPPORT_PIR_MODE_
//#define _SUPPORT_PIR_RANGE_
#endif
#define _SC_ONLY_TWO_LED_
#define _SC_POE_WIRED_LED_

//#define _GPIO_LS_
//#define _ADC_LS_
#define _I2C_LS_
#define _HAS_IRCUTTER_
#ifdef _HAS_IRCUTTER_
#define _IRCUTTER_IS_GPIO_
#endif
#define _SUPPORT_IR_LED_
#ifdef _SUPPORT_IR_LED_
#define _SUPPORT_IR_LED_LUM_
#define _IRLED_TYPE_IS_PWM_
//#define _IRLED_TYPE_IS_GPIO_
//#define _SUPPORT_IR_LED_MULTI_PIN_
#define _SUPPORT_IRLED_ATTCH_DNSWITCH_
#endif

#define _SC_2A_ENABLE_
//#define _HAS_PRIVACY_MODE_
#ifdef _HAS_PRIVACY_MODE_
#define _SUPPORT_PB_BUTTON_
#define _SUPPORT_EXCLUDE_PRIVACY_LED_
#define _PRIVACY_LED_IS_GPIO_
#endif

#define _AUTO_NIGHT_VISION_BY_LS_
//#define _SC_PWM_LED_
#define _GPIO_MAP_SPEC_0000_

//#define _SET_AUDIO_AMP_ENABLE_

#define _AUTO_CHK_LINK_AND_DHCP_  //Note:If defined _AUTO_SWITCH_LAN_WLAN_, only check link and dhcp for wlan 

#define _SC_HTTP_NOTIFY_SUPPORT_  //When open the MACRO _EVENT_ACTION_NEW_FORMAT_, it's best to control this MACRO by _EVENT_ACTION_NEW_FORMAT_ (event action new format)

//#define _SUPPORT_SMB_NOTIFY_CLIENT_
#define _G726_BITRATE_IS_32KBPS_

//#define _SUPPORT_SMB_SURVEY_
//#define _SC_SAMBA_SURVEY_GET_AUTH_FROM_CONF_WHEN_USER_CAN_NOT_INPUT_AUTH_

//#define _SUPPORT_SMBC_RECORD_STREAM_
#ifdef _SUPPORT_SMBC_RECORD_STREAM_
#define _SUPPORT_SMBC_RECORD_AVI_	//can record avi video and put them into smb server, else will record asf file
#define _SUPPORT_SMBC_RECORD_FILENAME_PREFIX_	// user can input samba record file name prefix
#define _SUPPORT_SMBC_SCHEDULE_
#endif

#define _SUPPORT_MEMORY_ALIGN_TO_4_

#define _DAVINCI_EMAC_USE_LED_

//#define _SUPPORT_OLDHYDRA_2_STREAM_SERVER_

#define _SUPPORT_TRIG_ACT_NUM_TO_32_

#define _SUPPORT_MMCSD_
#ifdef _SUPPORT_MMCSD_
#define _SUPPORT_QUERY_BY_PAGE_NUM_
#define _SUPPORT_MMCSD_TRIGGER_STREAM_
#define _SUPPORT_MMCSD_RECORD_STREAM_
#ifdef _SUPPORT_MMCSD_RECORD_STREAM_
#define _SUPPORT_MMCSD_RECORD_AVI_
#define _SUPPORT_MMCSD_RECORD_AVI_ENCODE_ALL_
#define _SUPPORT_MMCSD_RECORD_SCHEDULE_
#define _SUPPORT_MMCSD_RECORD_FILE_CTRL_
//#define _SUPPORT_MMCSD_RECORD_FILENAME_PREFIX_
#ifdef _SUPPORT_MMCSD_RECORD_FILENAME_PREFIX_
#define _SUPPORT_MMCSD_RECORD_EVENT_PREFIX_
#endif
#endif
#endif

#define _SUPPORT_NEWEST_TIMEZONE_AS_VISTA_

#define _EVENT_ACTION_NEW_FORMAT_  // Note: if the mode support IM, need add a MACRO of " _SC_IM_SUPPORT_ "
#ifdef _EVENT_ACTION_NEW_FORMAT_
#define _SC_HTTP_NOTIFY_SUPPORT_
#endif

//#define _SC_PROTECT_PASSWORD_
#ifdef _SC_PROTECT_PASSWORD_
#define _SC_PROTECT_WIRELESS_KEY_
#define _SC_DO_NOT_PROTECT_ACCESS_CODE_
#endif

#define _SC_CFG_STREAMING_PARA_EXT_	//Add bit rate and resolution new format as udpated Spec

#define _AUDIO_AIC23_ADCHP_OFF_

#define _SC_SUPPORT_BITE_RATE_2M_

#define _ENLARGE_UI_REFRESH_TIMER_

#define _SUPPORT_BLANK_CONF_FOR_ACTIONS_	//Blank is valid in event actions configuration.(In UI, SMB client/E-mail/FTP/HTTP Notification/... action pages that the configuration can be set blank if Event action is disabled.)

#define _SC_SMB_TEST_
#define _SC_ALERT_FTP_TEST_
#define _SC_ALERT_EMAIL_TEST_
#define _SC_IP_TEST_

#define _SC_SINGLE_MOTION_BUFFER_SOLUTION_
#define _MOTION_DETECTION_NEW_

#define _SUPPORT_H264_

//#define _SUPPORT_MPEG2_
#ifdef _SUPPORT_MPEG2_
#define MPEG2_CHANNEL_NUM_MAX		3
//#define _SC_SUPPORT_MPEG2_MOBILE_
#else
#define MPEG2_CHANNEL_NUM_MAX		0
#endif
#define MPEG4_CHANNEL_NUM_MAX		3
//#define _SC_SUPPORT_MPEG4_MOBILE_
#define MJPEG_CHANNEL_NUM_MAX		3
#define AUDIO_CHANNEL_NUM_MAX		2	// channel 1 is used for Flash LCPM support by default
#ifdef _SUPPORT_H264_
#define H264_CHANNEL_NUM_MAX		3
//#define _SC_SUPPORT_H264_MOBILE_
#else
#define H264_CHANNEL_NUM_MAX		0
#endif

#define STREAM_INDEX_MAX	3
#define AV_INPUT_MAX_NUM   1

#define _SC_ONLY_SUPPORT_ONE_STREAM_FOR_MULTICAST_BY_CHANNEL_	
//#define _SC_USER_CAN_SET_STREAM_CHANNEL_FOR_MULTICAST_
//#define _SC_SUPPORT_MJPEG_MULTICAST_

//SC_STREAM_MULTICAST_DEFAULT_CHANNEL <= MJPEG_CHANNEL_NUM_MAX
//SC_STREAM_MULTICAST_DEFAULT_CHANNEL <= MPEG4_CHANNEL_NUM_MAX
//SC_STREAM_MULTICAST_DEFAULT_CHANNEL <= H264_CHANNEL_NUM_MAX
//#define SC_STREAM_MULTICAST_DEFAULT_CHANNEL	2 // 1 ~ CHANNEL_NUM_MAX

#define _SC_SUPPORT_CHANNEL_AS_HIGHER_PRIORITY_

#define _HTTP_NOTIFY_SUPPORT_APPENDIX_

#define _FIXED_GOV_LENGTH_

#define _AUDIO_VOLUME_TRIGGER_
#define AUDIO_TRIG_CHANNEL_NUM_MAX	1
#define _AUDIO_VOLUME_TRIGGER_NEW_ARITHMETIC_

#define _SC_SUPPORT_NEW_EVENT_ATTCH_FORMAT_

//#define _HAS_MASK_OVERLAY_

#define _SC_ONLY_SUPPORT_3_STREAMING_AT_ONE_TIME_

#define _SC_USER_CAN_SWITCH_HTTP_AND_HTTPS_FEATURE_

#define _SC_USE_CHANNEL_AS_HIGH_PRIORITY_THAN_CODEC_FOR_STREAM_

#define _SUPPORT_LPCM_


/*
 * Define the MACROs here to control the UPNPIGD punched ports
 */
#ifdef _SUPPORT_UPNPIGD_CP_
#define _SUPPORT_UPNP_TRAV_RTSP_
#ifdef _SUPPORT_UPNP_TRAV_RTSP_
#define _SUPPORT_UPNP_TRAV_RTP_RTCP_
#endif
#ifdef _SC_HTTPS_SUPPORT_
#define _SUPPORT_UPNP_TRAV_HTTPS_
#endif
#define _SC_UPNPIGD_DISCOVERY_PACKET_TIMES_		//send out the discovery packet three times with the interval 10ms
#endif

#define _SUPPORT_BONJOUR_
#ifdef _SUPPORT_BONJOUR_
#ifdef _SC_HTTPS_SUPPORT_
#define _SUPPORT_HTTPS_SERVICE_IN_BONJOUR_
#endif
#define _SUPPORT_FIX_DEFAULT_BONJOUR_NAME_TO_SPECIAL_
#endif

#define TIME_WITH_SYS_TIME

//#define _SC_SUPPORT_DIGEST_AUTH_

#define _SC_NOT_SUPPORT_ASF_VIDEO_ 

#define _SC_SUPPORT_CROPPING_IMAGE_
//#define _SC_SUPPORT_SNAPSHOT_WITH_ALL_RESOLUTION_

//#define _SC_SUPPORT_VIDEO_ADVANCE_FEATURE_VNF_
//#define _SC_SUPPORT_VIDEO_ADVANCE_FEATURE_LDC_
//#define _SC_SUPPORT_VIDEO_ADVANCE_FEATURE_VS_

#define _SC_DO_NOT_USE_GETAVDATA_

#define _SC_HAS_MT7620_EXTERNAL_
#ifdef _SC_HAS_MT7620_EXTERNAL_
#define _MT7620_EXT_LNA_PA_
#define _WIFI_BUNDLE_AUTO_DETECTION_
#endif
#if defined(_SC_HAS_MT7620_ONBOARD_) || defined(_SC_HAS_MT7620_EXTERNAL_)
//commonly use vlan, if for test MT7620, can hide this macro, then MT7620 will as a switchboard.
//#define _MT7620_USE_VLAN_
#ifdef _MT7620_USE_VLAN_
//use bridge mode to solve two MAC issue 
#define _MT7620_USE_BRIDGE_
#endif
#endif

#define _WLAN_
#define _WLAN_SITE_SURVEY_
#define _WLAN_DOMAIN_
#ifdef _WLAN_DOMAIN_
#define _WLAN_DOMAIN_MAP_RALINK_REGION_
#endif
#define _WEPKEY128_4ENTRY_
#ifndef _WLAN_SPECIAL_STATUS_CGI_FOR_TEST_
#define _WLAN_SPECIAL_STATUS_CGI_FOR_TEST_
#endif
#define _SUPPORT_WPA_MIXM_MONITOR_
//#define _SUPPORT_WLAN_TEST_WHEN_CABLED_
#define _NETWORK_LED_WIRELESS_AMBER_BLINK_
#define _SUPPORT_WPS_BUTTON_
#define _SC_READ_PIN_CODE_FFROM_FLASH_
#define _SC_SUPPORT_DIFF_LED_BEHAVIOR_FOR_WPS_MODE_
#ifdef _SUPPORT_WPS_BUTTON_
#define _PUSHBUTTON_3_10_
#define _WPS_WITHOUT_PUSHBUTTON_
#ifdef _SC_HAS_MT7620_EXTERNAL_
#define _SUPPORT_INIC_WPS_
#endif
#endif

#define _SC_DRAW_OSD_FOR_CROP_STREAMING_

#define _SEND_GARP_AFTER_ROAMING_

#define _SUPPORT_MOTION_LENGTH_TO_30_SECS_
#ifdef _SUPPORT_MOTION_LENGTH_TO_30_SECS_
#define _SC_SINGLE_MOTION_BUFFER_SOLUTION_
#endif
//#define _SC_AUTO_UPGRADE_
//#define _AUTO_SWITCH_LAN_WLAN_
#define _SC_HTTP_POST_
#define _SUPPORT_HTTP_EVENT_TRIGGER_
#define _SUPPORT_HTTP_POST_
#ifdef _SC_HTTP_POST_
#define _SC_HTTPS_CLIENT_
#define _SC_HTTPS_DYNAMIC_PORT_
#endif
#ifdef _SC_HTTPS_CLIENT_
#define _SUPPORT_SSL_CLIENT_HANDSHAKE_ROLLBACK_
#endif
#define _HTTPS_NOTIFY_SUPPORT_
#ifdef _SUPPORT_HTTP_POST_
#define _SUPPORT_HTTP_POST_SHOW_TRIGGER_TYPE_
#define _SUPPORT_HTTP_POST_SHOW_CAPTURE_TIME_
#define _SUPPORT_DATE_PARAMETER_IN_HTTP_HEADER_FOR_HTTPPOST_
#endif

#ifdef _SUPPORT_HTTP_EVENT_TRIGGER_
#define _SC_SUPPORT_HTTPPOST_URL_ //for support http_trigger.cgi?http_post_url=...
#define _SUPPORT_BLOCK_HTTP_TRIGGER_
#define _SUPPORT_HTTP_TRIGGER_TIMEOUT_ //for support http_trigger.cgi?timeout=...
#endif
/*
#define _LINKTYPE_CHG_AUTO_
#define _DHCP_MORE_LOG_
#define _DHCP_RENEW_DURATION_1S_
#define _AUTO_DHCP_ALWAYS_SEND_MSG_
*/
//#define _WIRELESS_FLASH_REPLACE_EEPROM_

#define _SUPPORT_SET_SPEAKER_VOL_

#define _SC_HTTP_POSTSNAPSHOT_TIMEOUT_SUPPORT_
#define _SUPPORT_TELNETD_
//#define _ONVIF_
#define _SC_SUPPORT_NEW_EVENT_FOR_MULTI_EVENT_SAME_TIME_
#define _SC_IGNORE_HTTP_CGI_TRIGGER_DURING_EVENT_INTERVAL_

#define SC_HTTPC_VERSION_ITEM		"HTTP/1.0"
#define _SC_FLASHMAP_CARE_REAL_KERNEL_FS_SIZE_	// please refer "Nor Flash Map Extension" flash map design

#define _SC_SUPPORT_EVENT_ATT_MP4_FORMAT_
#define _NEW_ASSIGN_PROTOCOL_
//#define _SUPPORT_IPV6_
#define _SC_NEW_UI_WITH_AJAX_SOLUTION_
#define _SC_USE_XML_SOLUTION_FOR_EVENT_
//#define _SC_PROTECT_PASSWORD_

#define _SC_NOT_CHECK_RTCP_TIMEOUT_WEHN_RTP_OVER_UDP_
#define _SC_DHCP_LEASE_REBOOT_
#define _SUPPORT_KEEP_HTTP_EVENT_WHEN_IT_DIFFER_WITH_BEFORE_
#define _SUPPORT_PSIA_
#define _SUPPORT_OPENHOME_PSIA_
#ifdef _SUPPORT_OPENHOME_PSIA_
#define _SUPPORT_TO_TEST_WIRELESS_SETTINGS_BEFORE_WRITE_INTO_FLASH_
#define _OEM_REGISTER_CONF_ 
#define _HAS_PRIVACY_MODE_
#define _SC_SUPPORT_RTSP_PUSH_
#define _RTSP_PUSH_SUPPORT_CHANNEL_
#define _SPECIAL_MEDIA_PUSH_SUPPORT_
#define _SC_SUPPORT_MEDIA_TUNNEL_ //
#define _SC_SUPPORT_UPDATE_HTTPS_ROOT_CA_
#define _SC_SUPPORT_AUTO_UPDATE_HTTPS_CA_ //
//#define _SC_CORRECT_FLASH_MAP_BY_SEPC_DM365_NOR_ //only nor flash
#define _SC_HTTPS_CA_MANAGEMENT_ //only for nflash
#define _SC_MERGE_OPENHOME_NETWORK_INTERFACE_TO_ONE_
#define _SC_SUPPORT_WIFI_ROAMING_
#endif
#define _SUPPORT_DEBUG_SYSTEM_
#ifdef _SUPPORT_DEBUG_SYSTEM_
#define _SUPPORT_SET_SYSLOG_MODULE_LEVEL_
#endif

#define _SUPPORT_WIFI_STATISTICS_

//#define _SC_SUPPORT_FACE_DETECTION_
#define _SUPPORT_QUERY_FW_UPGRADE_STATUS_

//#define _SUPPORT_CUSTOM_QUALITY_

#define _ECHO_CANCEL_SUPPORT_AUTO_DETECTION_
#define _SUPPORT_LENGTHEN_MEDIA_UPLOAD_URL_

#define _DIGITAL_PT_SUPPORT_		// Confirmed that VGA/QHD always cropping from 720P
#ifdef _DIGITAL_PT_SUPPORT_
#define _ENABLE_ALL_POSITION_MOTION_OR_RESET_CAMERA_POSITION_
#endif

#define _SUPPORT_SYSLOG_NEUTRAL_SECTION_
#define _SUPPORT_HTTP_WARNING_HEADER_

#define _SC_DISABLE_MOTION_WHEN_DAY_NIGHT_SWITCH_
#define _SUPPORT_1080P_RESOLUTION_
#define _SC_CHECK_FUNCTION_ID_WHEN_UPGRADE_
#ifdef _SC_CHECK_FUNCTION_ID_WHEN_UPGRADE_
#define _SC_NEW_FUNCTION_ID_
#endif

#define _SUPPORT_DEWARP_
#define _SUPPORT_TIME_ZONE_SET_IN_SYSTEM_TIME
#define _SC_SUPPORT_AUDIO_ENABLE_FOR_EVENT_STREAMING_CHANNEL_

#define _SUPPORT_S2_NEUTAL_STREAM_SPEC_			//stream 1/2: 1080P, 720P, VGA; stream 3: ePTZ and VGA only.
#define _SUPPORT_SUBREGION_EPTZ_					//use VGA subRegion in main buffer to do ePT
#define _SC_SUPPORT_SET_AUTO_SWITCH_LAN_WLAN_

#define _SUPPORT_SSL_SWITCH_BY_SET_GROUP_
#define _SC_ENHANCE_WEB_SERVER_RELOAD_
#define _SC_UPNP_WAIT_FOR_WEB_SERVER_READY_AFTER_BOOTUP_
#define _SC_SUPPORT_POLL_METHOD_
#define _SC_SUPPORT_COMBIE_MULTI_WINDOWS_AS_ONE
#define _SC_SUPPORT_SPECIAL_THROUGHPUT_TOOL_
//#define _SC_SUPPORT_START_SPECIAL_THROUGHPUT_TOOL_AT_BOOTUP_  //only when you need to build testing FW, open it. 
#define _OEM_0009_DIFF_SENSITIVITY_FOR_MOTION_ //support different motion setting for low/medium/high level 

#define _SC_SUPPORT_SPECIFY_ONE_CHANNEL_FOR_EPTZ_
#define _SC_KEEP_VIEW_POINT_AFTER_ZOOM_
#define _SC_SUPPORT_EPTZ_ZOOM_COMMAND_
#ifdef _SC_SUPPORT_EPTZ_ZOOM_COMMAND_
#define EPTZ_ZOOM_UNIT_NUMBER    100
#endif

#endif

