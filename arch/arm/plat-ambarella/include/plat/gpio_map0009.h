/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP0009_H__
#define __GPIO_MAP0009_H__

#include "sc_config.h"

/* 
 * GPIO map
 */
#ifdef _GPIO_MAP_SPEC_0009_ 

/* Output */
#define 	NETWORK_LED_ACTIVE_VALUE 	0
#define 	NETWORK_LED_INACTIVE_VALUE	1

#define GPIO_PIN_IDX_OUTPUT_IRCUT					(-1)
#define GPIO_PIN_IDX_OUTPUT_WLLED_LED				(-1)

#ifdef _IRLED_TYPE_IS_GPIO_
#define GPIO_PIN_IDX_OUTPUT_IRLED_LED				(87)
#endif
#define	GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED			(92)
#define	GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED			(-1)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED		(91)
#define	GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_ACTIVE_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_PRIVACY_AMBER_LED		(-1)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED		(91)
#define	GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED		(90)
#define GPIO_PIN_IDX_OUTPUT_DUAL_LENS_SWITCH		(38)
#define GPIO_PIN_IDX_OUTPUT_PORT1					(-1)
#define GPIO_PIN_IDX_OUTPUT_PORT2					(-1)
#define GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL1			(48)
#define GPIO_PIN_IDX_OUTPUT_IRLED_CURR_CTL2			(86)

#define GPIO_OUTPUT_NUM		7

/* Input */
#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON				(83)
#define GPIO_PIN_IDX_INPUT_LS						(45)
#define GPIO_PIN_IDX_INPUT_PIR						(-1)
#define GPIO_PIN_IDX_INPUT_PB						(-1)
#define GPIO_PIN_IDX_INPUT_WPS_BUTTON				(-1)
#define GPIO_PIN_IDX_INPUT_PORT1					(-1)
#define GPIO_PIN_IDX_INPUT_PORT2					(-1)
#define GPIO_PIN_IDX_INPUT_VIDEO_MODE				(-1)

#define GPIO_INPUT_NUM			2

/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
// moved into gpio_bootloader.h file

#endif

#endif

