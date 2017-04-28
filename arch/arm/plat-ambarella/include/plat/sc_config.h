/*
 ********
 ***??***
 ********
 */

#ifndef __SC_CONFIG_H__
#define __SC_CONFIG_H__

/* OEM-Model MACRO GLOBAL Define 
 * Same as defined in the shell: makeall
 */
#include "sc_model.h"

//#define _AUTO_SWITCH_LAN_WLAN_

#define _SC_GETHOSTBYNAME_MEM_ALIGN_TO_4_

#ifdef _SC_HAS_MT7620_ONBOARD_ 
#define _SC_SUPPORT_THROUGHPUT_MT_CGI_
#endif
#ifdef _SC_SUPPORT_THROUGHPUT_MT_CGI_
// Support if_bridge mode, only for WiFi certificate testing.
#define _SC_SUPPORT_IF_BRIDGE_
#endif

#define _SC_SUPPORT_2_RTC_AUTO_

#define _ONE_FW_MULTI_LOCATION_

// Has a status flag in configuration file to indicate whether the camera is set?
#define _HAS_CONF_STATUS_CONTROL_

// Only for WiFi certificate testing.
//#define _WIFI_CERT_FW_
#ifdef _WLAN_
#define _WLAN_HAS_WSC_
#endif

#ifdef _MULTI_WLAN_SUPPORT_
#define MAX_WLAN_NUMBER	5
#else
#define MAX_WLAN_NUMBER	1
#endif

#ifndef  _NO_PUSHBUTTON_3_10_
#ifndef _PUSHBUTTON_3_10_
#define _PUSHBUTTON_3_10_
#endif
#endif

// Fully support MP4S or M4S2, Suzhou update at 12/25/2007
/*
 * Currently:
 *		The is-object-layer-identifire of all MPEG4 is 0
 *		ASF streaming, all of trigger file(asf/avi/mp4/3gp) use MP4S format without VOS header
 *		RTP use VOS header
 */
#define _SC_MPEG4_IS_MP4S_	
//#define _SC_MPEG4_IS_OBJECT_LAYER_IDENTIFIER_
// but RTP packets have vos header
#define _SC_RTP_MPEG4_HAS_VOS_

// Display the default name, and can config the device name
#define _SC_EDIT_DEVICE_NAME_

#define _SZ_REDUCE_MEM_

#define _SC_GROUP_CGI_SOLUTION_NEW_

// Fixed the bug can't crypt the string whose length is bigger than 8
#define _SC_FIX_CRYPT_BUG_
#define _SC_SUPPORT_DUAL_IMAGE_

#define UPG_FILE		"/tmp/sc1100.bin"

#define _CHECK_VIDEO_ACESS_ENABLE_

// From Taipei's function of new query CGI
#define BOA_REMOTE_USER	

#define _HIDDEN_SESSION_FOR_ALERTOCX_ADMIN_

#define SC_RESVRVED_FW_INFO_FILE		"/tmp/sc_resv_info"

#define ARPCHECK

#define _RTSP_
#define _RTCP_
#define _FTP_
#define _FTP_2_
#define _SMTP_AUTH_

#define _MOBILE_SETTING_
#define _OPT_BITRATE_QUALITY_EN_

//#define _AUDIO_MIC_BOOST_MANUAL_
#define _MT_AUDIO_
#define _ALTNATIVE_PORT_

#define _HAVE_UPNP_
#define _DHCPCLIENT_
#define HAVE_ATTACHED
#define _WINDOW_NAME_
#define _MOTION_

//#define _SC_UPNP_HAS_PNPX_		// Suzhou add
#ifndef _SUPPORT_UPNPIGD_CP_
#define _SC_UPNPIGD_DEL_PORT_IN_FACTORY_DEF_
#ifdef _SC_UPNPIGD_DEL_PORT_IN_FACTORY_DEF_
//Notice: Don't set UPNPIGD_WAIT_TIMES_MAX as a big value.
//Because camera may wait 2*UPNPIGD_WAIT_TIMES_MAX seconds when doing factory default 
//in some case(camera bootes with running UpnPPartMapping and before this UpnPPartMapping quit.
#define UPNPIGD_WAIT_TIMES_MAX		2
#endif
#endif

//Suzhou update.For control hydra memory using.
#define _SC_HYDRA_MEM_CAL_SOL_

#ifdef _SC_HYDRA_MEM_CAL_SOL_
//#define _SC_CHANGE_FPS_WHEN_IMAGE_LARGE_
#endif

//#define	_CGI_SCHEDULE_USE_ACTION_
#define _CGI_SCHEDULE_USE_SAVE_ALL_SOLUTION_

//For control schedule,when schedule is enable,but list is empty,we will think schdule is disable
#define _SC_SCHEDULE_NULL_IS_ALL_

//#include "conf_sec.h"
#define _SC_SET_GET_WEP_KEYS_STRING_

/*
#ifndef _WEBUPG_IN_LOADER_
#define _WEBUPG_IN_LOADER_
#endif
*/

//#define _SUPPORT_MAC_SECTOR_SETTING_
//#define _SUPPORT_MAC_SECTOR_SETTING_AUTO_

#define UPGRADE_TFLASH_LOCK_FILE                                        "/tmp/tflash.lck"

#ifdef _NAND_FLASH_
#define MTD_MAC					"/dev/mtd3"			
#define MTD_LOGO				"/dev/mtd?"	
#define MTD_CONF0				"/dev/mtd1"			
#define MTD_CONF1				"/dev/mtd2"			
#define MTD_ALLFLASH			"/dev/mtd?"
#define MTD_IMG_CURRENT				"/dev/mtd4"
#define MTD_ALL					"/dev/mtd0"
#define MTD_PINFO				"/dev/mtd6"
#define MTD_IMG_UPGRADE				"/dev/mtd7"
#define MTD_DEBUG                               "/dev/mtd8"
#define MTD_UPGRADE_LOG                 	"/dev/mtd9"
#define MTD_RESERVE				"/dev/mtd10"
#define MTD_CALIBRATION				"/dev/mtd11"
#define MTD_FLASH_DUMP				"/dev/mtd12"
#ifndef _WEBUPG_IN_LOADER_
#define MTD_HTTPS_CA			"/dev/mtd?"		
#endif

#else
#define MTD_MAC					"/dev/mtdblock1"			
#define MTD_LOGO				"/dev/mtdblock?"	
#define MTD_FLASH_CONF			"/dev/mtdblock2"
#define MTD_ALLFLASH			"/dev/mtdblock0"
#define MTD_RESERVE				"/dev/mtdblock3"
#define MTD_FLASH_DEBUG			"/dev/mtdblock6"
#ifndef _WEBUPG_IN_LOADER_
#define MTD_HTTPS_CA			"/dev/mtdblock?"		
#endif
#endif

#ifdef _DEF_CONF_FILE_IN_FLASH_
#define MTD_FLASH_CONF_DEF		"/dev/mtdblock7"	//	// Make sure maximal is less than 128K
#endif

#define _CHECK_MODELS_WHEN_RESTORE_CONF_

#ifdef _OEM_0007_
#define SC_ALLOW_UPG_FW_FILENAME		"/tmp/sc_allow_upg"
#endif

#ifdef _WLAN_
#define _SC_DHCP_SEND_MORE_PACKETS_FOR_WIRELESS_ // Because wireless loss packets in noisy office
												// It is easy time out for DHCP, so sending DHCP packets frequently 
												// to enhance the chance of success when DHCP 
#define _SC_SUPPORT_64_HEX_CHAR_AS_PSK_			// WPA - PSK , 8-63 characters, or 64 HEX key

#define _SC_WLAN_REINSTALL_	//Wireless driver will be reinstall when not link over 2 minutes
#ifdef _SC_WLAN_REINSTALL_
#ifdef _SCNP_CONF_AP_
#define SC_WLAN_REINSTALL_TIMEOUT		240 // 4 minutes
#else
#define SC_WLAN_REINSTALL_TIMEOUT		120 // 2 minutes
#endif
#endif

#endif
//#define _WPA_WPA2_ENTERPRISE_

#define SYS_DATE_MIN		2010
#define SYS_DATE_MAX		2037

#define _SC_SMTP_CLIENT_CONNECT_TIMEOUT_SOLUTION_
#define _SUPPORT_RECEIVE_GABAGE_DATA_IN_CGI_

#define _SC_BEFORE_RESTART_WEB_SERVER_REFUSE_SET_

#define _SC_SUPPORT_MJPEG_OVER_RTP_
#define _SC_SUPPORT_RTSP_FROM_LOCAL_RELAY_	// Tracy Add for support RTP over UDP/TCP by HTTP port
#define _SC_SUPPORT_SWF_
#define _SC_SUPPORT_TS_
#ifdef _SUPPORT_H264_
#define _SC_SUPPORT_FLV_
#endif
//#define _SC_SUPPORT_POLICY_SERVICES_ON_SPECIAL_PORT_	// serve crossdomain.xml on 843 port

#ifdef _PT_HW_ID_00_01_
#define _SC_SUPPORT_CALIBRATION_HOME_
#endif

// "_BITRATE_CONTROL_0_" control the oldest algorithm for adjust FPS base on the slow session
// "_BITRATE_CONTROL_1_" control adjust the FPS base on FASTEST session bps and encode bps
#ifndef _BITRATE_CONTROL_0_
#define _BITRATE_CONTROL_1_
#define _FAST_SESSION_HAS_HIGHER_PRIORITY_
#endif

#define _SUPPORT_RATIO_16_6_						//16:6

#ifdef _HAS_IRCUTTER_
//#define _IR_CUT_SWITCHING_LONG_RUN_TEST_   //Note: this function is only for IR cut long run test and will bring many side effects
#endif
#define _SUPPORT_USE_DEFAULT_MAC_WHEN_MAC_INVALID_
#ifdef _SUPPORT_USE_DEFAULT_MAC_WHEN_MAC_INVALID_
#define DEFAULT_MAC_ADDR_PART0	0x00
#define DEFAULT_MAC_ADDR_PART1	0xc0
#define DEFAULT_MAC_ADDR_PART2	0x02
#define DEFAULT_MAC_ADDR_PART3	0x12
#define DEFAULT_MAC_ADDR_PART4	0x35
#define DEFAULT_MAC_ADDR_PART5	0x88
#endif

//#define _WLAN_SPECIAL_STATUS_CGI_FOR_TEST_  //If open this MACRO, camera will log the wifi signal every second till the query CGI, then log a new file again till the query CGI again.

//#define _MT_CALIBRATION_
//#define _MT_ONLY_
//#define _FCC_TEST_FW_  //To open this MACRO to build EMI FW
//#define _SC_ONLY4_EMI_TEST_FW_ //Open this MACRO only for EMI FW, otherwise can't open this MACRO
#ifdef _MT_CALIBRATION_
#ifndef _MT_ONLY_
#define _MT_ONLY_
#endif
#endif
#ifdef _MT_ONLY_
#ifndef _FCC_TEST_FW_
#define _FCC_TEST_FW_
#endif
#ifndef _SUPPORT_TELNETD_
#define _SUPPORT_TELNETD_
#endif
#ifdef _SUPPORT_TELNETD_
#define _OPEN_TELNETD_
#endif
#endif

#define SC_URI_EXTENSINO_TAG		"-sc-"

#ifndef SC_HTTPC_VERSION_ITEM
#define SC_HTTPC_VERSION_ITEM		"HTTP/1.1"
#endif

#ifndef _CMS_CHANNEL_NUM_MAX
#define _CMS_CHANNEL_NUM_MAX	1
#endif

#define _SUPPORT_CALIBRATION_CHECK_

#define _SC_READ_RELEASE_DATE_FROM_BIN_

#ifndef _WLAN_SPECIAL_STATUS_CGI_FOR_TEST_
#define _WLAN_SPECIAL_STATUS_CGI_FOR_TEST_
#endif

#ifdef _SUPPORT_HTTP_POST_
#ifndef HTTP_POST_NUM
#define HTTP_POST_NUM	1
#endif
#endif


#define EXTRA_INFO_SIZE_IN_FLASH		2052
#define _SC_USE_FILE_LOCK_FOR_SET_CONFIG_

#ifndef RD_FW_VERSION_INDICATOR 
#define RD_FW_VERSION_INDICATOR     "R" 
#endif 

#endif



