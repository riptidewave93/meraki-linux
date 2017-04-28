/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP0000_H__
#define __GPIO_MAP0000_H__

#include "sc_config.h"

/* 
 * GPIO map
 */
#ifdef _GPIO_MAP_SPEC_0000_ 

/* Output */
#define 	NETWORK_LED_ACTIVE_VALUE 	0
#define 	NETWORK_LED_INACTIVE_VALUE	1
#define 	DAY_SENSOR_ACTIVE_VALUE 1
#define 	NIGHT_SENSOR_ACTIVE_VALUE 0

#define 	GPIO_PIN_IDX_OUTPUT_RST_RTL8201		(54)
#define 	GPIO_PIN_IDX_OUTPUT_RST_AK4642		(51)
#define 	GPIO_PIN_IDX_OUTPUT_RST_MT7620N		(-1)
#define 	GPIO_PIN_IDX_OUTPUT_RST_OV2715		(9)

#define 	GPIO_PIN_IDX_OUTPUT_IRCUT		(53)
#define 	GPIO_PIN_IDX_OUTPUT_WLLED_LED		(-1)

#define 	GPIO_PIN_IDX_OUTPUT_IRLED_LED		(50)
#define		GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED			(10)
#define		GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED			(18)
#define		GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED		(10)
#define		GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_ACTIVE_AMBER_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_PRIVACY_AMBER_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED		(-1)
#define		GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED		(-1)
#define 	GPIO_PIN_IDX_OUTPUT_PORT1					(-1)
#define 	GPIO_PIN_IDX_OUTPUT_PORT2					(-1)
#define 	GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL1			(-1)
#define 	GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL2			(-1)
#define 	GPIO_PIN_IDX_OUTPUT_DUAL_LENS_SWITCH	(-1)
#define 	GPIO_PIN_IDX_OUTPUT_HEATER (-1)
#define 	GPIO_PIN_IDX_OUTPUT_RSV_LED1	(7)
#define 	GPIO_PIN_IDX_OUTPUT_RSV_LED2	(8)
#define 	GPIO_PIN_IDX_OUTPUT_RSV_LED3	(23)
#define 	GPIO_PIN_IDX_OUTPUT_RSV_LED4	(20)

#define GPIO_OUTPUT_NUM	10

/* Input */
#define GPIO_PIN_IDX_INPUT_MIN					(-1)
#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON				(52) 
#define GPIO_PIN_IDX_INPUT_LS					(-1)
#define GPIO_PIN_IDX_INPUT_PIR					(-1)
#define GPIO_PIN_IDX_INPUT_PB					(-1)
#define GPIO_PIN_IDX_INPUT_WPS_BUTTON				(-1)
#define GPIO_PIN_IDX_INPUT_PORT1				(-1)
#define GPIO_PIN_IDX_INPUT_PORT2				(-1)
#define GPIO_PIN_IDX_INPUT_VIDEO_MODE				(-1)
#define GPIO_PIN_IDX_INPUT_DAY_SENSOR_VSYNC			(-1)
#define GPIO_PIN_IDX_INPUT_NIGHT_SENSOR_VSYNC			(-1)

#define GPIO_INPUT_NUM	1	

/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
// moved into gpio_bootloader.h file

#endif

#endif

