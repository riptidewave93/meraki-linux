/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP0001_H__
#define __GPIO_MAP0001_H__

#include "sc_config.h"

/* 
 * GPIO map
 */
#ifdef _GPIO_MAP_SPEC_0001_ 

/* Output */
#define 	NETWORK_LED_ACTIVE_VALUE 	0
#define 	NETWORK_LED_INACTIVE_VALUE	1

#define 	GPIO_PIN_IDX_OUTPUT_RST_RTL8201		(7)
#define 	GPIO_PIN_IDX_OUTPUT_RST_AK4642		(12)
#define 	GPIO_PIN_IDX_OUTPUT_RST_MT7620N		(83)
#define 	GPIO_PIN_IDX_OUTPUT_RST_OV9715		(93)
//for sensor select: day or night sensor
#define GPIO_PIN_IDX_OUTPUT_SENSOR_SELECT	(94)

#define 	GPIO_PIN_IDX_OUTPUT_IRCUT		(-1)
#define 	GPIO_PIN_IDX_OUTPUT_WLLED_LED		(-1)

#ifdef _IRLED_TYPE_IS_GPIO_
#define 	GPIO_PIN_IDX_OUTPUT_IRLED_LED		(85)
#endif
#define		GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED			(28)
#define		GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED			(29)
#define		GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED		(30)
#define		GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED		(31)
#define		GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_ACTIVE_AMBER_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_PRIVACY_AMBER_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED		(30)
#define		GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED		(31)
#define 	GPIO_PIN_IDX_OUTPUT_PORT1					(-1)
#define 	GPIO_PIN_IDX_OUTPUT_PORT2					(-1)
#define 	GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL1			(46)
#define 	GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL2			(45)

#define 	GPIO_PIN_IDX_OUTPUT_LED1		(28)
#define 	GPIO_PIN_IDX_OUTPUT_LED2		(29)
#define 	GPIO_PIN_IDX_OUTPUT_LED3		(30)
#define 	GPIO_PIN_IDX_OUTPUT_LED4		(31)
#define         GPIO_PIN_IDX_OUTPUT_DUAL_LENS_SWITCH    (-1)

#define GPIO_OUTPUT_NUM	11

/* Input */
#define GPIO_PIN_IDX_INPUT_MIN					(11)
#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON			(GPIO_PIN_IDX_INPUT_MIN) 
#define GPIO_PIN_IDX_INPUT_LS						(84)
#define GPIO_PIN_IDX_INPUT_PIR						(-1)
#define GPIO_PIN_IDX_INPUT_PB						(-1)
#define GPIO_PIN_IDX_INPUT_WPS_BUTTON				(GPIO_PIN_IDX_INPUT_MIN)
#define GPIO_PIN_IDX_INPUT_PORT1					(-1)
#define GPIO_PIN_IDX_INPUT_PORT2					(-1)
#define GPIO_PIN_IDX_INPUT_VIDEO_MODE				(-1)

#define GPIO_INPUT_NUM		2

/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
// moved into gpio_bootloader.h file

#endif

#endif

