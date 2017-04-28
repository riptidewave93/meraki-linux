/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP0003_H__
#define __GPIO_MAP0003_H__

#include "sc_config.h"

/* 
 * GPIO map
 */
#ifdef _GPIO_MAP_SPEC_0003_ 
#define 	NETWORK_LED_ACTIVE_VALUE 	0
#define 	NETWORK_LED_INACTIVE_VALUE	1

/* Output */
#define GPIO_PIN_IDX_OUTPUT_IRCUT					(80)
#define GPIO_PIN_IDX_OUTPUT_WLLED_LED				(-1)

#ifdef _IRLED_TYPE_IS_GPIO_
#define GPIO_PIN_IDX_OUTPUT_IRLED_LED				(92)
#endif

#define	GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED			(47)
#define	GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED			(-1)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED		(49)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED		(89)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED		(49)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED		(46)
#define GPIO_PIN_IDX_OUTPUT_PORT1					(36)
#define GPIO_PIN_IDX_OUTPUT_PORT2					(-1)

#ifdef _SUPPORT_PIR_RANGE_
#define GPIO_PIN_IDX_OUTPUT_PIR_RANGE_PORT0			(83)
#define GPIO_PIN_IDX_OUTPUT_PIR_RANGE_PORT1			(82)
#define GPIO_PIN_IDX_OUTPUT_PIR_RANGE_PORT2			(81)
#endif

#ifdef _SET_AUDIO_AMP_ENABLE_
#define GPIO_PIN_IDX_OUTPUT_AUDIO_AMP           (88)
#endif

#ifdef _SET_AUDIO_AMP_ENABLE_
#define GPIO_OUTPUT_NUM		12
#else
#define GPIO_OUTPUT_NUM		11
#endif

/* Input */
#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON				(45)//(79)
#define GPIO_PIN_IDX_INPUT_LS						(87)
#define GPIO_PIN_IDX_INPUT_PIR						(84)
#define GPIO_PIN_IDX_INPUT_PB						(79)//(48)
#define GPIO_PIN_IDX_INPUT_WPS_BUTTON				(48)//(45)
#define GPIO_PIN_IDX_INPUT_PORT1					(35)
#define GPIO_PIN_IDX_INPUT_PORT2					(-1)
#define GPIO_PIN_IDX_INPUT_VIDEO_MODE				(-1)

#define GPIO_INPUT_NUM			6

#define GPIO_PIN_IDX_GIO_RTC_SCL					(85)
#define GPIO_PIN_IDX_GIO_RTC_SDA					(86)
/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
// moved into gpio_bootloader.h file

#endif

#endif

