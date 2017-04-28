/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_BOOTLOADER_H__
#define __GPIO_BOOTLOADER_H__

 #include "gpio_map.h"
/*
** define the LED which need operate by bootloader
** Currently, we need set power/active/network/privacy to active when power on
*/
typedef struct
{
	int flag; // 0: turn off, 1:turn on
	int pin_num;
	unsigned int pin_type;	// GPIO/PWM
	unsigned int active_value;	// LED on value
	int brightness_rate; // LED brightness rate, valid for PWM type
}BootLoaderLedControl;

//oc431
#if defined(_GPIO_MAP_SPEC_0000_)
static BootLoaderLedControl bootloader_leds[]=
        {
                {1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_RSV_LED1, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_RSV_LED2, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_RSV_LED3, GPIO_PIN_TYPE_LED, 0, -1},
                {1, GPIO_PIN_IDX_OUTPUT_RSV_LED4, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
                //{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_ILLUMINATE_LED, 0, -1},
#endif
                {-1, -1, -1, -1, -1}
        };
//rc8321
#elif defined(_GPIO_MAP_SPEC_0001_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_POWER_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_ILLUMINATE_LED, 0, -1},
#endif
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0002_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_PWM_LED, 0, SC_PWM_BRIGHTNESS_MID},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_PWM_LED, 0, SC_PWM_BRIGHTNESS_MID},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_PWM_LED, 0, SC_PWM_BRIGHTNESS_MID},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_LED, 1, -1},
#endif
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0003_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_PWM_LED, 1, -1},
#endif
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0004_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_PWM_LED, 1, SC_PWM_BRIGHTNESS_MID},
#endif
		{-1, -1, -1, -1, -1}
	};


#elif defined(_GPIO_MAP_SPEC_0005_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_LED, 1, -1},
#endif
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0006_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0007_)
//#define _NO_INPUT_PUSH_BUTTON_

#define GPIO_PIN_IDX_INPUT_PUSH_BUTTON_BOOTLOADER					79

static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_PRIVACY_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{0, GPIO_PIN_IDX_OUTPUT_WLLED_LED, GPIO_PIN_TYPE_LED, 0, -1},
#ifdef _RESET_PAN_TILT_IN_BOOTLOADER_	//Here Loader pin must before reset pin, because it will first config loader pin then reset, other wise the loader pin doesn't work.
		{1, GPIO_PIN_IDX_OUTPUT_PT_LOADER, GPIO_PIN_TYPE_OTHERS, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_PT_RESET, GPIO_PIN_TYPE_OTHERS, 0, -1},
#endif
		{-1, -1, -1, -1, -1}
	};
#elif defined(_GPIO_MAP_SPEC_0008_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_ACTIVE_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED, GPIO_PIN_TYPE_LED, 1, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED, 1, -1},
#ifdef _RESET_PAN_TILT_IN_BOOTLOADER_				//Here Loader pin must before reset pin, because it will first config loader pin then reset, other wise the loader pin doesn't work.
		{1, GPIO_PIN_IDX_OUTPUT_PT_LOADER, GPIO_PIN_TYPE_OTHERS, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_PT_RESET, GPIO_PIN_TYPE_OTHERS, 0, -1},
#endif
		{-1, -1, -1, -1, -1}
	};

#elif defined(_GPIO_MAP_SPEC_0009_)
static BootLoaderLedControl bootloader_leds[]=
	{
		{1, GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED, GPIO_PIN_TYPE_LED, 0, -1},
		{1, GPIO_PIN_IDX_OUTPUT_WIRELESS_AMBER_LED, GPIO_PIN_TYPE_LED,0, -1},
#ifdef _IRLED_TYPE_IS_GPIO_
		//{0, GPIO_PIN_IDX_OUTPUT_IRLED_LED, GPIO_PIN_TYPE_LED, 1, -1},
#endif
		{-1, -1, -1, -1, -1}
	};
#endif

#endif

