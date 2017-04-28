/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP0006_H__
#define __GPIO_MAP0006_H__

#include "sc_config.h"

/* 
 * GPIO map
 */
#ifdef _GPIO_MAP_SPEC_0006_ 

/* Output */
#define 	NETWORK_LED_ACTIVE_VALUE 	1
#define 	NETWORK_LED_INACTIVE_VALUE	0

#define	GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED (-1)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED   (-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED    (-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_AMBER_LED    (-1)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED   (-1)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_AMBER_LED   (-1)
#define GPIO_PIN_IDX_OUTPUT_WLLED_LED   (-1)
#define	GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED			(79)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED		(81)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED		(81)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED		(80)
#define GPIO_PIN_IDX_OUTPUT_PORT1					(89)
#define GPIO_PIN_IDX_OUTPUT_PORT2					(90)
#define GPIO_PIN_IDX_OUTPUT_PORT3					(91)
#define GPIO_PIN_IDX_OUTPUT_PORT4					(92)
    
#define GPIO_OUTPUT_NUM		(8)

/* Input */
#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON				(26)
#define GPIO_PIN_IDX_INPUT_WPS_BUTTON				(94)
#define GPIO_PIN_IDX_INPUT_PORT1					(85)
#define GPIO_PIN_IDX_INPUT_PORT2					(86)
#define GPIO_PIN_IDX_INPUT_PORT3					(87)
#define GPIO_PIN_IDX_INPUT_PORT4					(88)
#define GPIO_PIN_IDX_VIDEO_OUT_CH_SEL			(82)

#define GPIO_INPUT_NUM			(7)

//#define GPIO_PIN_IDX_GIO_RTC_SCL					(85)
//#define GPIO_PIN_IDX_GIO_RTC_SDA					(86)
/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
// moved into gpio_bootloader.h file

#endif

#endif

