/*
 ********
 ***??***
 ********
 */

#ifndef __GPIO_MAP_H__
#define __GPIO_MAP_H__

#include "sc_config.h"

/* Angelo: for a5s driver test*/
#define  _SUPPORT_GPIO_INTERRUPT_ON_DM36X_

#include "gpio_map0000.h"
#include "gpio_map0001.h"
#include "gpio_map0002.h"
#include "gpio_map0003.h"
#include "gpio_map0004.h"
#include "gpio_map0005.h"
#include "gpio_map0006.h"
#include "gpio_map0007.h"
#include "gpio_map0008.h"
#include "gpio_map0009.h"
#include "gpio_mapxxxx.h"

#define GPIO_DEV_FILE	"/dev/gpio_drv"


/*
 * IO Expander control
*/
#define IOEX_WRITE_CMD_BUF	3
#define IOEX_READ_CMD_BUF	2
typedef enum IOEXControl
{
	IOEX_CTRL_INACTIVE,
	IOEX_CTRL_ACTIVE,
	IOEX_CTRL_WHILE_ACTIVE    //this command need timer
} IOEXControl;

/*
 * GPIO Set control
 */
typedef enum GPIOControl
{
	GPIO_CTRL_DISABLE,
	GPIO_CTRL_ENABLE,
	GPIO_CTRL_INACTIVE,
	GPIO_CTRL_ACTIVE,
	GPIO_CTRL_BLINK,
	GPIO_CTRL_BLINK_SHARP,  // Not used in driver, only for application
	GPIO_CTRL_WHILE_ACTIVE  //not support in gpio driver, need update it later 
} GPIOControl;

// GPIO type
typedef enum GPIOPinType
{
	GPIO_PIN_TYPE_INVALID,
	GPIO_PIN_TYPE_BUTTON,
	GPIO_PIN_TYPE_LS,
	GPIO_PIN_TYPE_PIR,
	GPIO_PIN_TYPE_IO,
	GPIO_PIN_TYPE_LED,
	GPIO_PIN_TYPE_ILLUMINATE_LED,
	GPIO_PIN_TYPE_IRCUTER,
	GPIO_PIN_TYPE_PWM_LED,
	GPIO_PIN_TYPE_OTHERS
} GPIOPinType;

#define GPIO_LED_TIMER1	400	//400 is normal
#define GPIO_LED_TIMER2	100
#define GPIO_LED_TIMER3	2500 //400 is too fast for RC4551, using 2500 for normal
#define PWM_LED_BRIGHTNESS_RATE	50	// 50%

#define PIN_NUM_ARRAY_SIZE		5
#define PIN_NUM_ONE_ARRAY		32
#define GPIO_PIN_CHECK(pin, pin_array)  (((pin_array)[(pin)>>5]) & (1<<((pin)%32)))
#define GPIO_PIN_ADD(output, pin) 		(output)[(pin)>>5] |= 1<<((pin)%32)
#define GPIO_PIN_CLEAR(output, pin) 		(output)[(pin)>>5] &= ~(1<<((pin)%32))
/*
** Attention, Currently, GPIO read only support read one pin at one time
*/
typedef struct GPIORead
{
	char op_flag;	// 0 non-block; 1 block
	unsigned int pin_nums[PIN_NUM_ARRAY_SIZE]; // True value of Bits to indicate the PIN
	unsigned int pin_stat; // status of bits for each PIN, 0->disable, 1->enable, only used for output pins.
	unsigned int pin_vals[PIN_NUM_ARRAY_SIZE]; // Value of bits for each PIN, 0->inactive, 1->active.
	unsigned int steady_time; // only valid for block control, and don't support multiple PIN control currently
} GPIORead;

typedef struct GPIOWrite
{
	GPIOControl state; 	// Control for all set PINs, 0->Disable, 1->Enable, 2->Low, 3->High, 4->flash between Low and High, need a enum
	GPIOPinType type;	// Control all pins whose type is, and pin_num will not be valid if type is set.
	unsigned int pin_nums[PIN_NUM_ARRAY_SIZE]; // True value of Bits to indicate the PIN
	unsigned short blink_num;	// only valid for flash control
	unsigned short blink_interval;
	unsigned int blink_role;	//0->Normal,1->Listener,2->Learner
	unsigned short brightness_rate;	// only for PWM brightness control
} GPIOWrite;

//#ifdef _SC_PWM_IN_GPIO_DRV_
// Instance of the private WDT device structure
#define DM3XX_PWM_MINORS	3 // 4
#define DAVINCI_PWM_MINORS	DM3XX_PWM_MINORS /* MAX of all PWM_MINORS */
// pinnum
#define SC_PWM_0_PINNUM				92 //GPIO_PIN_IDX_OUTPUT_POWER_GREEN_LED //GIO92 Led D3
#define SC_PWM_1_PINNUM				91 //GPIO_PIN_IDX_OUTPUT_NETWORK_GREEN_LED //GIO91 Led D5
#define SC_PWM_2_PINNUM				88 //GPIO_PIN_IDX_OUTPUT_WIRELESS_GREEN_LED //GIO88 Led D4
typedef struct {
	unsigned int pid;
	unsigned int pcr;
	unsigned int cfg;
	unsigned int start;
	unsigned int rpt;
	unsigned int per;
	unsigned int ph1d;
} davinci_pwmregs;
typedef volatile davinci_pwmregs *davinci_pwmregsovly;
//#endif

// brightness
#define SC_PWM_BRIGHTNESS_MAX		100 //0x20
#ifdef _OEM_0009_0009_
#define SC_PWM_BRIGHTNESS_HIGH		65 //0x03
#define SC_PWM_BRIGHTNESS_MID			35 //0x12
#define SC_PWM_BRIGHTNESS_LOW		5 //0x1b
#else
#define SC_PWM_BRIGHTNESS_HIGH		90 //0x03
#define SC_PWM_BRIGHTNESS_MID			50 //0x12
#define SC_PWM_BRIGHTNESS_LOW		10 //0x1b
#endif

#endif

