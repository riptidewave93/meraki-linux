/**
 * @mainpage Ambarella A7S Hardware Abstraction Layer
 *
 * @author Mahendra Lodha <mlodha@ambarella.com>
 * @author Rudi Rughoonundon <rudir@ambarella.com>
 * @date January 2012
 * @version 7195
 *
 * @par Introduction:
 * The Ambarella A7S Hardware Abstraction Layer (ambhal) provides an API between
 * high level software and the low level hardware registers of the A7S chip.
 *
 * @par Objectives:
 * - Ease of Use
 * - Stability
 * - Low Power
 *
 * @par Topics:
 * - @ref init_page
 * - @ref mode_page
 * - @ref pll_page
 * - @ref clocksource_page
 *
 * @par Modules:
 * - @ref init_group
 * - @ref status_group
 * - @ref mode_group
 * - @ref sysconfig_group
 * - @ref clock_group
 * - @ref pll_group
 * - @ref adc_group
 * - @ref arm_group
 * - @ref ahb_group
 * - @ref apb_group
 * - @ref audio_group
 * - @ref au1ch_group
 * - @ref core_group
 * - @ref ddr_group
 * - @ref fdet_group
 * - @ref fio_group
 * - @ref gtx_group
 * - @ref hdmi_group
 * - @ref cortex_group
 * - @ref idsp_group
 * - @ref ir_group
 * - @ref ioctrl_group
 * - @ref lcd_group
 * - @ref lvds_group
 * - @ref motor_group
 * - @ref ms_group
 * - @ref pwm_group
 * - @ref pwmois_group
 * - @ref reset_group
 * - @ref sd_group
 * - @ref sdio_group
 * - @ref sensor_group
 * - @ref ssi_group
 * - @ref ssi2_group
 * - @ref uart_group
 * - @ref vin_group
 * - @ref usb_group
 * - @ref vout_group
 *
 * @defgroup init_group Initialization
 *
 * Set of api calls used to setup/query AMBHAL. See also @ref init_page.
 *
 * @defgroup status_group Command Status
 *
 * Status values returned by AMBHAL api calls.
 *
 * @defgroup mode_group Operating Mode
 *
 * API calls used to change and query the operating mode. See also @ref mode_page.
 *
 * @defgroup sysconfig_group System Configuration
 *
 * These api calls query the system configuration pins and return how the chip is configured.
 *
 * @defgroup clock_group Divided Clocks
 *
 * Defines the type used to specify and return clock frequencies.
 *
 * @defgroup pll_group PLL
 *
 * Various type definitions related to pll programming/query. Also defines an api call to disable observation of pll through xx_clk_si pin.
 *
 * @defgroup adc_group ADC
 *
 * API calls to change/query the frequency of the ADC.
 *
 * @defgroup arm_group ARM
 *
 * API calls to query the frequency of the arm clock.
 * The arm clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup ahb_group AHB
 *
 * API calls to query the frequency of the ahb clock.
 * The ahb clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup apb_group APB
 *
 * API calls to query the frequency of the apb clock.
 * The apb clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup audio_group Audio
 *
 * API calls to change/query frequency of the Audio pll. See also @ref pll_page and @ref clocksource_page.
 *
 * @defgroup au1ch_group Audio 1CH
 *
 * API calls to change/query the frequency of the Audio 1CH block.
 *
 * @defgroup core_group Core
 *
 * API calls to query the frequency of the core clock.
 * The core clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup ddr_group DDR
 *
 * API calls to query the frequency of the ddr clock.
 * The ddr clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup cortex_group Cortex
 *
 * API calls to set/query the frequency of the cortex clock.
 *
 * @defgroup fdet_group Face Detection
 *
 * API calls to change/query the frequency of the face detection unit.
 *
 * @defgroup fio_group Flash IO
 *
 * API calls to reset the various Flash controllers.
 *
 * @defgroup gtx_group Gigabit Ethernet
 *
 * API calls to change/query the frequency of the gigabit ethernet block.
 *
 * @defgroup hdmi_group HDMI
 *
 * API calls to change/query frequency of the HDMI pll. See also @ref pll_page and @ref clocksource_page.
 *
 * @defgroup idsp_group IDSP
 *
 * API calls to query the frequency of the idsp clock.
 * The idsp clock frequency cannot be set directly. It is changed by AMBHAL when the operating mode is changed. See also @ref mode_group.
 *
 * @defgroup ir_group Infrared
 *
 * API calls to change/query the frequency of the IR controller.
 *
 * @defgroup ioctrl_group IO Pads Control
 *
 * API calls to change/query the characteristics of the GPIO pads (such as drive strength/pullup/pulldown/etc).
 *
 * @defgroup lcd_group LCD
 *
 * API calls to change/query frequency of the LCD pll. See also @ref pll_page and @ref clocksource_page.
 *
 * @defgroup lvds_group LVDS
 *
 * API calls to change/query the mode of the LVDS pads.
 *
 * @defgroup motor_group Motor
 *
 * API calls to change/query the frequency of the Motor controller.
 *
 * @defgroup ms_group Memory Stick
 *
 * Various API calls to change/query MS clock frequency and also to control various parameters of the MS IO interface.
 *
 * @defgroup pwm_group PWM
 *
 * API calls to change/query the frequency of the PWM controller.
 *
 * @defgroup pwmois_group PWM
 *
 * API calls to change/query the frequency of the PWM OIS block.
 *
 * @defgroup reset_group Reset
 *
 * @defgroup reset_group Reset
 *
 * @defgroup sd_group SD
 *
 * API calls to change/query the frequency of the SD controller.
 *
 * @defgroup sdio_group SDIO
 *
 * API calls to change/query the frequency of the SDIO block.
 *
 * @defgroup sensor_group Sensor
 *
 * API calls to change/query frequency of the Sensor pll. See also @ref pll_page and @ref clocksource_page.
 *
 * @defgroup ssi_group SSI
 *
 * API calls to change/query the frequency of the SSI controller.
 *
 * @defgroup ssi2_group SSI2
 *
 * API calls to change/query the frequency of the SSI2 controller.
 *
 * @defgroup uart_group UART
 *
 * API calls to change/query the frequency of the UART controller.
 *
 * @defgroup vin_group VIN
 *
 * API calls to change/query the frequency of the VIN controller.
 *
 * @defgroup usb_group USB
 *
 * Set of api calls to change the state of the USB PHYs and subsystem.
 *
 * @defgroup vout_group Video Out
 *
 * API calls to change/query frequency of the Vout pll. See also @ref pll_page and @ref clocksource_page.
 *
 */

/**
 * @page init_page HAL Loading, Initialization & Usage
 *
 * @par Introduction
 * The ambhal image must be loaded into a 4 byte aligned region of memory by
 * the OS or the boot loader. The binary image may be relocated in physical
 * memory (by copying the entire image to a new location) or in virtual memory
 * (by using the mmu) @b before it is initialized.
 * @par Initialization
 * The function amb_hal_init() must be invoked first before any other ambhal API
 * calls are made. This function implements a simple dynamic loader to
 * initialize the ambhal global offset table and to initialize the hardware to
 * a known state. This function must be called after the mmu has been
 * initialized.
 * @warning The hal image must not be relocated in physical or virtual memory
 * after initialization. The function amb_hal_init() should only be called
 * once.
 * @par Usage
 * The api functions may be called by including the ambhal.h header file.
 * @par
 * All ambhal functions are implemented in C using the ARM APCS32 ABI and they
 * use the ARM instruction set only.
 */

/**
 * @page mode_page Changing Operating Mode
 *
 * @par Introduction
 * The hal operating mode represents the current operating status of all the
 * hardware under the control of hal.
 * @par
 * The operating mode is defined using the structure ::amb_operating_mode_t.
 * @par Mode Switch
 * When the operating mode is changed the status of the hardware under the
 * control of hal is changed. For example changing the performance changes
 * the pll settings to increase/decrease clock frequencies to meet the
 * required new performance setting.
 * @par
 * An operating mode switch is performed by using the following sequence.
 *
 * @code
 * amb_hal_success_t success ;
 * amb_operating_mode_t operating_mode ;
 * operating_mode.vidcap_size = AMB_VIDCAP_4000X2250 ;
 * operating_mode.performance = AMB_PERFORMANCE_720P30 ;
 * operating_mode.mode = AMB_OPERATING_MODE_CAPTURE ;
 * success =
 *   amb_set_operating_mode (amb_hal_base_address, &operating_mode) ;
 *
 * K_ASSERT (success == AMB_HAL_SUCCESS) ;
 *
 * @endcode
 *
 * @par
 * See also @subpage operating_mode_settings
 *
 */

/**
 * @page operating_mode_settings Operating Mode Settings.
 *
 * @copydoc operating_mode_parameters.c
 *
 * @include operating_mode_parameters.c
 */

/**
 * @page pll_page Changing PLL Frequency
 *
 * @par Introduction
 * A number of phase locked loops (pll) are present in the device to generate
 * various independent clocks.
 * @par
 * The api allows the frequencies of most of the plls to be set to discrete values.
 * It also allows the frequencies to be changed in such a way that the pll
 * remains locked during the change (this ensures that the clock is stable
 * during the transition).
 * @par
 * A clock frequency change is performed by using the following sequence.
 *
 * @code
 * amb_hal_success_t success ;
 * success = amb_set_sensor_clock_frequency (amb_hal_base_address, 74000000) ;
 *
 * // if this assertion goes off you did something wrong.
 * // either the requested clock frequency is invalid (AMB_HAL_FAIL)
 * // or the pll is not locked and it is not ready to be reprogrammed (AMB_HAL_RETRY)
 * K_ASSERT (success == AMB_HAL_SUCCESS) ;
 *
 * // if you get here the pll has locked and you are good to go
 *
 * @endcode
 */

/**
 * @page clocksource_page Changing Clock Sources
 *
 * @par Introduction
 * Some of the plls (@ref vout_group, @ref audio_group & @ref lcd_group) in the design allow the reference clock source to be changed.
 * The api to change the clock source takes the new clock source name and the new clock source frequency.
 * @par External PLL Reference Clocks
 * When the new clock source is 
 * AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI or AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK
 * the reference clock source of the pll is being changed.
 * The api needs that reference clock frequency to be able to calculate the correct pll settings
 * that will generate the output clock of the pll.
 * @par Internal PLL Reference Clock
 * When the new clock source is AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_REF the api selects the reference clock frequency
 * based on the system configuration pins (it is either 24 MHz or 27 MHz). In this
 * case the application does not need to provide anything as the api will figure it out on its
 * own and do the pll settings calculations accordingly.
 * @par External Clock (No PLL) 
 * When the new clock source is AMB_EXTERNAL_CLOCK_SOURCE the pll is not used and so the api does not
 * care what the reference clock frequency is. In fact the api will power down
 * that pll when the application selects that option to save power.
 */

/**
 * @file ambhal.h
 * @brief Ambarella A7S Hardware Abstraction Layer API.
 *
 */

#ifndef _AMBHAL_H_INCLUDED_
#define _AMBHAL_H_INCLUDED_

#define AMBHALMAGIC "Ambarella"

#if defined __GNUC__ && __GNUC__ >= 2

#define INLINE inline
#define AMBHALUNUSED(var) unsigned int var __attribute__((unused))

#else

#define INLINE __inline
#define AMBHALUNUSED(var) unsigned int var

#endif

/*
 * indexes used to access functioninfo array in hal
 */

typedef enum {
AMB_HAL_FUNCTION_INFO_HAL_INIT,
AMB_HAL_FUNCTION_INFO_GET_CHIP_NAME,
AMB_HAL_FUNCTION_INFO_GET_VERSION,
AMB_HAL_FUNCTION_INFO_SET_OPERATING_MODE,
AMB_HAL_FUNCTION_INFO_GET_OPERATING_MODE,
AMB_HAL_FUNCTION_INFO_GET_REFERENCE_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SYSTEM_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_BOOT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_HIF_TYPE,
AMB_HAL_FUNCTION_INFO_DISABLE_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_GET_IDSP_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_IDSP_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_IDSP_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_IDSP_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_GET_CORE_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_CORE_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_CORE_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_CORE_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_GET_DDR_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_DDR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_DDR_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_DDR_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_SET_CORTEX_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_CORTEX_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_CORTEX_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_CORTEX_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_CORTEX_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_SET_SENSOR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SENSOR_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_SENSOR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SENSOR_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_SENSOR_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_SET_LCD_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_LCD_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_LCD_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_LCD_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_LCD_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_LCD_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_SET_VOUT_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_VOUT_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_VOUT_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_VOUT_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_VOUT_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_VOUT_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_SET_AUDIO_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_AUDIO_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_AUDIO_PLL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_AUDIO_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_AUDIO_PLL_LOCK_STATUS,
AMB_HAL_FUNCTION_INFO_ENABLE_AUDIO_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_USB_DEVICE_SOFT_RESET,
AMB_HAL_FUNCTION_INFO_USB_HOST_SOFT_RESET,
AMB_HAL_FUNCTION_INFO_SET_USB_PORT_STATE,
AMB_HAL_FUNCTION_INFO_GET_USB_PORT_STATE,
AMB_HAL_FUNCTION_INFO_SET_UART_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_UART_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_UART_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_UART_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_UART_INIT,
AMB_HAL_FUNCTION_INFO_SET_VIN_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_VIN_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_VIN_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_PIP_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PIP_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PIP_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_MOTOR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_MOTOR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_MOTOR_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_IR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_IR_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_IR_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SSI_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SSI_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SSI_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SSI_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_SSI2_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SSI2_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SSI2_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SSI2_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_PWM_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PWM_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PWM_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_PWM_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_PWMOIS_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PWMOIS_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_PWMOIS_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_PWMOIS_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_ADC_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_ADC_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_ADC_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SD_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SD_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SD_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SDIO_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SDIO_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_SDIO_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_GTX_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_FDET_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_FDET_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_FDET_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_AU1CH_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_AU1CH_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_AU1CH_CLOCK_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_MS_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_MS_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_SET_MS_SCLK_DELAY,
AMB_HAL_FUNCTION_INFO_SET_MS_SD_INPUT_DELAY,
AMB_HAL_FUNCTION_INFO_SET_MS_SD_OUTPUT_DELAY,
AMB_HAL_FUNCTION_INFO_SET_MS_READ_DELAY,
AMB_HAL_FUNCTION_INFO_GET_MS_DELAY_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_MS_STATUS,
AMB_HAL_FUNCTION_INFO_GET_MS_STATUS,
AMB_HAL_FUNCTION_INFO_RESET_FLASH,
AMB_HAL_FUNCTION_INFO_RESET_XD,
AMB_HAL_FUNCTION_INFO_RESET_CF,
AMB_HAL_FUNCTION_INFO_RESET_FIO,
AMB_HAL_FUNCTION_INFO_RESET_ALL,
AMB_HAL_FUNCTION_INFO_RESET_WATCHDOG,
AMB_HAL_FUNCTION_INFO_RESET_CHIP,
AMB_HAL_FUNCTION_INFO_SET_LVDS_PAD_MODE,
AMB_HAL_FUNCTION_INFO_GET_LVDS_PAD_MODE,
AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_MISC1_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_MISC2_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_SMIOA_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_SMIOB_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_SMIOC_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_SMIOD_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_VD1_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_DRIVE_STRENGTH,
AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_PULLUPDOWN,
AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_INPUT_TYPE,
AMB_HAL_FUNCTION_INFO_GET_SENSOR_IOCTRL_CONFIGURATION,
AMB_HAL_FUNCTION_INFO_GET_ARM_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_HDMI_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_ENABLE_HDMI_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_GET_HDMI4K_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_ENABLE_HDMI4K_CLOCK_OBSERVATION,
AMB_HAL_FUNCTION_INFO_AUTO_SELECT_HDMI4K_RANGE,
AMB_HAL_FUNCTION_INFO_SET_SENSOR_CLOCK_PAD_MODE,
AMB_HAL_FUNCTION_INFO_GET_SENSOR_CLOCK_PAD_MODE,
AMB_HAL_FUNCTION_INFO_SET_PERIPHERALS_BASE_ADDRESS,
AMB_HAL_FUNCTION_INFO_GET_AHB_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_APB_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_GET_AXI_CLOCK_FREQUENCY,
AMB_HAL_FUNCTION_INFO_SET_GTX_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_SOURCE,
AMB_HAL_FUNCTION_INFO_SET_DRAM_ARBITER_PRIORITY,
AMB_HAL_FUNCTION_INFO_NULL
} amb_hal_function_info_index_t ;

/**
 * Status returned by ambhal functions.
 *
 * @ingroup status_group
 */

typedef enum {
/** function succeeded. */
AMB_HAL_SUCCESS = 0x00000000UL,
/** function failed - check arguments. */
AMB_HAL_FAIL = 0xffffffffUL,
/** function cannot complete right now - try again. */
AMB_HAL_RETRY = 0xfffffffeUL
} amb_hal_success_t ;

/**
 * PLL fractional frequency setting.
 *
 * @note This value is limited so that only a fractional change of up to ~50 KHz may be
 * requested.
 *
 * @ingroup pll_group
 */

typedef unsigned int amb_pll_fractional_divisor_t ;

/**
 * PLL/Divider Reference Clock Source
 *
 * @ingroup pll_group
 * @ingroup divider_group
 */

typedef enum {
/** Reference clock from crystal oscillator - either 24 MHz or 27 MHz */
AMB_REFERENCE_CLOCK_SOURCE_CLK_REF,
/* Use spclk_c as reference for pll */
AMB_REFERENCE_CLOCK_SOURCE_SPCLK_C,
/** Use clk_si as reference for the pll */
AMB_REFERENCE_CLOCK_SOURCE_CLK_SI,
/** Use lvds_idsp_sclk as reference for the pll/divider */
AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK,
/** Use external clock source - no pll */
AMB_EXTERNAL_CLOCK_SOURCE,
/** Use the vout pll clock for lcd/hdmi pll */
AMB_SHARE_VOUT_CLOCK,
/** Use the core pll vco output for divider */
AMB_REFERENCE_CLOCK_SOURCE_CORE_PLL_VCO,
/** Use the core pll output for divider */
AMB_REFERENCE_CLOCK_SOURCE_CORE_PLL,
/** Use the audio pll vco output for divider */
AMB_REFERENCE_CLOCK_SOURCE_AUDIO_PLL_VCO,
/** Use the audio pll output for divider */
AMB_REFERENCE_CLOCK_SOURCE_AUDIO_PLL,
/** Use the idsp pll vco output for divider */
AMB_REFERENCE_CLOCK_SOURCE_IDSP_PLL_VCO,
/** Use the cortex pll vco output for divider */
AMB_REFERENCE_CLOCK_SOURCE_CORTEX_PLL_VCO,
/** Use the apb clock for divider */
AMB_REFERENCE_CLOCK_SOURCE_APB,
/** Use the core clock for divider */
AMB_REFERENCE_CLOCK_SOURCE_CORE,
/** Use the arm clock for divider */
AMB_REFERENCE_CLOCK_SOURCE_ARM,
/** Use the idsp clock for divider */
AMB_REFERENCE_CLOCK_SOURCE_IDSP,
/* Reserved */
AMB_REFERENCE_CLOCK_SOURCE_RESERVED = 0xffffffffUL
} amb_clock_source_t ;

/**
 * All the fields that make up the pll frequency programming.
 *
 * The effective pll frequency may be calculated as follows:
 * @f[
 * f_{out} = (\frac{reference}{prescaler})*(intprog+1+fraction)*(\frac{sdiv+1}{sout+1})*(\frac{1}{postscaler})
 * @f]
 *
 * @f{equation*}{
 * fraction = \begin{cases}
 * 0& \text{if fractional\_mode is 0}\\
 * (-0.5*frac[31])+(\frac{frac[30:0]}{2^{32}})& \text{if fractional\_mode is 1}
 * \end{cases}
 * @f}
 *
 * Guidelines:
 * @f[
 * f_{jdiv} = \frac{f_{vco}}{sdiv+1} <= 800 MHz
 * @f]
 *
 * @f[
 * f_{vco} = f_{out}*(sout+1)*postscaler <= 2.2 GHz
 * @f]
 *
 * @ingroup pll_group
 */

typedef struct {
/** 1 bit fractional mode/integer mode */
unsigned int fractional_mode ;
/** 7 bit integer multiplier */
unsigned int intprog ;
/** 4 bit feedback fast multiplier */
unsigned int sdiv ;
/** 4 bit output fast divider */
unsigned int sout ;
/** 32 bit signed fractional multiplier */
int frac ;
/** 16 bit reference clock divider */
unsigned int prescaler ;
/** 16 bit output divider */
unsigned int postscaler ;
/** clock source for the block ::amb_clock_source_t */
unsigned int clock_source ;
/* Reserved */
unsigned int delay ;
} amb_pll_configuration_t ;

/**
 * Clock frequency in Hz
 *
 * @ingroup clock_group
 */

typedef unsigned int amb_clock_frequency_t ;

/**
 * Clock Divider Configuration.
 *
 * ALl the fields that make up the clock divider programming.
 *
 * @ingroup clock_group
 */

typedef struct {
/** Clock source frequency */
amb_clock_frequency_t clock_source_frequency ;
/** Clock source */
amb_clock_source_t clock_source ;
/** Divider value */
unsigned int divider ;
} amb_divider_configuration_t ;

/**
 * Video Capture Window Size
 *
 * @ingroup mode_group
 */

typedef enum {
/** 4K p30 */
AMB_VIDCAP_4KP30,
/** 4K p60 */
AMB_VIDCAP_4096X2176_60FPS,
/** IMX 083: 1080p30 */
AMB_VIDCAP_4096X3575,
/** IMX 078: 1080p30, 720p30 */
AMB_VIDCAP_4000X2250,
/** Aptina 3135: 1080p60, 720p60, 1080p30, 720p30 */
AMB_VIDCAP_2304X1296,
/** IMX 078: 1080p60, 720p60 */
AMB_VIDCAP_1984X1116,
/** Aptina 3135: 4:3 photo preview */
AMB_VIDCAP_2048X1536,
/** IMX 078: 4:3 photo preview */
AMB_VIDCAP_1312X984,
/** IMX 083: 720p30 */
AMB_VIDCAP_1536X384,
/** IMX 083: photo preview */
AMB_VIDCAP_1536X384_SMALL_VB,
/* Reserved */
AMB_VIDCAP_RESERVED = 0xffffffffUL
} amb_vidcap_window_size_t ;

/**
 * Performance.
 *
 * @ingroup mode_group
 */

typedef enum {
/** 480p30 */
AMB_PERFORMANCE_480P30,
/** 720p30 */
AMB_PERFORMANCE_720P30,
/** 720p60 */
AMB_PERFORMANCE_720P60,
/** 1080i */
AMB_PERFORMANCE_1080I60,
/** 1080p30 */
AMB_PERFORMANCE_1080P30,
/** 1080p60 */
AMB_PERFORMANCE_1080P60,
/** 4K p30 */
AMB_PERFORMANCE_4KP30,
/* Reserved */
AMB_PERFORMANCE_RESERVED = 0xffffffffUL
} amb_performance_t ;

/**
 * Operating Mode.
 *
 * @ingroup mode_group
 */

typedef enum {
/** Camera is on but no video being captured */
AMB_OPERATING_MODE_PREVIEW,
/** Still picture capture */
AMB_OPERATING_MODE_STILL_CAPTURE,
/** Camera is on and video is being captured */
AMB_OPERATING_MODE_CAPTURE,
/** Video playback */
AMB_OPERATING_MODE_PLAYBACK,
/** GUI only */
AMB_OPERATING_MODE_DISPLAY_AND_ARM,
/** Low power mode */
AMB_OPERATING_MODE_STANDBY,
/** LCD off */
AMB_OPERATING_MODE_LCD_BYPASS,
/** Still picture preview */
AMB_OPERATING_MODE_STILL_PREVIEW,
/** Low power */
AMB_OPERATING_MODE_LOW_POWER,
/** IP Cam */
AMB_OPERATING_MODE_IP_CAM,
/* Reserved */
AMB_OPERATING_MODE_RESERVED = 0xffffffffUL
} amb_mode_t ;

/**
 * USB Port State Settings.
 *
 * @ingroup usb_group
 */

typedef enum {
/** Disable USB port */
AMB_USB_OFF,
/** Enable USB port */
AMB_USB_ON,
/** Force USB port into suspend state */
AMB_USB_SUSPEND,
/** Enable USB port  & force USB to never suspend */
AMB_USB_ALWAYS_ON,
/* Reserved */
AMB_USB_RESERVED = 0xffffffffUL
} amb_usb_port_state_t ;

/**
 * State of HDMI Interface
 * 
 * @ingroup hdmi_group
 */

typedef enum {
/** HDMI phy is off */
AMB_HDMI_OFF,
/** HDMI phy is on */
AMB_HDMI_ON,
/* Reserved */
AMB_HDMI_RESERVED = 0xffffffffUL
} amb_hdmi_interface_state_t ;

/**
 * Dual Stream state
 * 
 * @ingroup mode_group
 */

typedef enum {
/** Dual Stream is off */
AMB_DUAL_STREAM_OFF,
/** Dual Stream is on */
AMB_DUAL_STREAM_ON,
/* Reserved */
AMB_DUAL_STREAM_RESERVED = 0xffffffffUL
} amb_dual_stream_state_t ;

/**
 * Digital Gamma Mode
 *
 * Turning this on forces the core clock frequency to be multiple of 36 MHz.
 * 
 * @ingroup mode_group
 */

typedef enum {
/** Digital Gamma Mode is off */
AMB_DIGITAL_GAMMA_MODE_OFF,
/** Digital Gamma Mode is on */
AMB_DIGITAL_GAMMA_MODE_ON,
/* Reserved */
AMB_DIGITAL_GAMMA_MODE_RESERVED = 0xffffffffUL
} amb_digital_gamma_mode_t ;

/**
 * Operating mode
 *
 * @ingroup mode_group
 */

typedef struct {
/** Sensor resolution */
amb_vidcap_window_size_t vidcap_size ;
/** Output resolution */
amb_performance_t performance ;
/** Operating mode ::amb_mode_t */
amb_mode_t mode ;
/** USB state */
amb_usb_port_state_t usb_state ;
/** HDMI state */
amb_hdmi_interface_state_t hdmi_state ;
/** Dual Stream state */
amb_dual_stream_state_t dual_stream_state ;
/** Digital Gamma Mode */
amb_digital_gamma_mode_t amb_digital_gamma_mode ;
} amb_operating_mode_t ;

/**
 * Memory stick delay configuration
 *
 * @ingroup ms_group
 */

typedef struct {
/** Delay on output path of xx_ms_sclk */
unsigned int sclk_delay ;
/** Delay on input path of xx_ms_sd */
unsigned int sd_input_delay ;
/** Delay on output path of xx_ms_sd and xx_ms_bs */
unsigned int sd_output_delay ;
/** Read timing adjustment */
unsigned int read_delay ;
} amb_ms_delay_configuration_t ;

/**
 * Memory stick controller status
 *
 * @ingroup ms_group
 */

typedef enum {
/** Memory Stick controller disabled */
AMB_MS_DISABLE,
/** Memory Stick controller enabled */
AMB_MS_ENABLE,
/* Reserved */
AMB_MS_RESERVED = 0xffffffffUL
} amb_ms_status_t ;

/**
 * The mode for lvds pads
 *
 * @ingroup lvds_mode
 */

typedef enum {
/** LVDS pads configured for LVDS */
AMB_LVDS_PAD_MODE_LVDS,
/** LVDS pads configured for SLVS */
AMB_LVDS_PAD_MODE_SLVS,
/** LVDS pads configured for LVCMOS */
AMB_LVDS_PAD_MODE_LVCMOS,
/* Reserved */
AMB_LVDS_PAD_MODE_RESERVED = 0xffffffffUL
} amb_lvds_pad_mode_t ;

/**
 * Memory stick delay
 *
 * @ingroup ms_group
 */

typedef unsigned int amb_ms_delay_t ;

/**
 * System configuration settings
 *
 * @ingroup sysconfig_group
 */

typedef enum {
/** 1.8V NAND Flash Selected (1) or 3.3V NAND Flash Selected (0) */
AMB_SYSTEM_CONFIGURATION_NAND_FLASH_TYPE = 0x1UL,
/** 2048 Bytes Flash Page Size (1) or 512 Bytes Flash Page Size (0) */
AMB_SYSTEM_CONFIGURATION_NAND_FLASH_2048_PAGE_SIZE = 0x10UL,
/** NAND Read Confirm */
AMB_SYSTEM_CONFIGURATION_NAND_FLASH_READ_CONFIRM = 0x20UL,
/** NAND ECC */
AMB_SYSTEM_CONFIGURATION_NAND_FLASH_ECC = 0x400UL,
/** NAND Spare Cell */
AMB_SYSTEM_CONFIGURATION_NAND_FLASH_SPARE_CELL = 0x800UL,
/** Ethernet Selected */
AMB_SYSTEM_CONFIGURATION_ETHERNET_SELECTED = 0x00800000UL,
/** RMII Selected */
AMB_SYSTEM_CONFIGURATION_RMII_SELECTED = 0x8000UL,
/** Host Interface Secure Mode */
AMB_SYSTEM_CONFIGURATION_HIF_SECURE_MODE = 0x200000UL,
/* Reserved */
AMB_SYSTEM_CONFIGURATION_RESERVED = 0xffffffffUL
} amb_system_configuration_t ;

/**
 * Boot type select
 *
 * @ingroup sysconfig_group
 */

typedef enum {
/** USB Boot */
AMB_USB_BOOT,
/** SD Boot */
AMB_SD_BOOT,
/** Flash Boot */
AMB_NAND_BOOT,
/** Flash Boot */
AMB_NOR_BOOT,
/** SSI Boot */
AMB_SSI_BOOT,
/** Host Interface Boot */
AMB_HIF_BOOT,
/** XIP Boot */
AMB_XIP_BOOT,
/* Reserved */
AMB_RESERVED_BOOT = 0xffffffffUL
} amb_boot_type_t ;

/**
 * Host interface type select
 *
 * @ingroup sysconfig_group
 */

typedef enum {
/** Host Interface Disabled */
AMB_HIF_DISABLE,
/** Intel Ready Asserted High */
AMB_INTEL_READY_ACTIVE_HIGH,
/** Intel Ready Asserted Low */
AMB_INTEL_READY_ACTIVE_LOW,
/** Motorola Data Acknowledge Asserted High */
AMB_MOTOROLA_DACK_ACTIVE_HIGH,
/** Motorola Data Acknowledge Asserted Low */
AMB_MOTOROLA_DACK_ACTIVE_LOW,
/* Reserved */
AMB_HIF_RESERVED = 0xffffffffUL
} amb_hif_type_t ;

/**
 * IO pad drive strength
 *
 * @ingroup ioctrl_group
 */

typedef enum {
/** 2 mA Driver */
AMB_IOCTRL_DRIVE_STRENGTH_2MA,
/** 8 mA Driver */
AMB_IOCTRL_DRIVE_STRENGTH_8MA,
/** 4 mA Driver */
AMB_IOCTRL_DRIVE_STRENGTH_4MA,
/** 12 mA Driver */
AMB_IOCTRL_DRIVE_STRENGTH_12MA,
/* Reserved */
AMB_IOCTRL_RESERVED = 0xffffffffUL
} amb_ioctrl_drive_strength_t ;

/**
 * IO pad pull up/pull down
 *
 * @ingroup ioctrl_group
 */

typedef enum {
/** Pullup/Pulldown disabled */
AMB_IOCTRL_PULLUPDOWN_DISABLED,
/** Pullup enabled */
AMB_IOCTRL_PULLUP_ENABLED,
/** Pulldown enabled */
AMB_IOCTRL_PULLDOWN_ENABLED,
/* Reserved */
AMB_IOCTRL_PULLUPDOWN_RESERVED = 0xffffffffUL
} amb_ioctrl_pullupdown_t ;

/**
 * IO pad type
 *
 * @ingroup ioctrl_group
 */

typedef enum {
/** cmos input pad */
AMB_IOCTRL_CMOS_INPUT_TYPE,
/** schmitt trigger input pad */
AMB_IOCTRL_SCHMITT_INPUT_TYPE,
/* Reserved */
AMB_IOCTRL_RESERVED_INPUT_TYPE = 0xffffffffUL
} amb_ioctrl_input_type_t ;

/**
 * IO pad slew rate
 *
 * @ingroup ioctrl_group
 */

typedef enum {
/** fast slew rate */
AMB_IOCTRL_FAST_SLEW_RATE,
/** slow slew rate */
AMB_IOCTRL_SLOW_SLEW_RATE,
/* Reserved */
AMB_IOCTRL_RESERVED_SLEW_RATE = 0xffffffffUL
} amb_ioctrl_slew_rate_t ;

/**
 * IO pad configuration
 *
 * @ingroup ioctrl_group
 */

typedef struct {
/** pad drive strength ::amb_ioctrl_drive_strength_t */
amb_ioctrl_drive_strength_t drive_strength ;
/** pad pullup/pulldown enabled ::amb_ioctrl_pullupdown_t */
amb_ioctrl_pullupdown_t pullupdown ;
/** type of input pad ::amb_ioctrl_input_type_t */
amb_ioctrl_input_type_t input_type ;
/** slew rate ::amb_ioctrl_slew_rate_t */
amb_ioctrl_slew_rate_t slew_rate ;
} amb_ioctrl_configuration_t ;

/**
 * Sensor clock pad mode
 *
 * @ingroup sensor_group
 */

typedef enum {
/** Sensor clock pad is an output */
AMB_SENSOR_CLOCK_PAD_OUTPUT_MODE,
/** Sensor clock pad is an input */
AMB_SENSOR_CLOCK_PAD_INPUT_MODE,
/* Reserved */
AMB_SENSOR_CLOCK_PAD_RESERVED_MODE = 0xffffffffUL
} amb_sensor_clock_pad_mode_t ;

/**
 * DRAM arbiter priority
 *
 * @ingroup init_group
 */

typedef enum {
/** Low priority for dsp clients (75 of total bandwidth) */
AMB_DRAM_ARBITER_DSP_VERY_LOW_PRIORITY,
/** Low priority for dsp clients (81.25% of total bandwidth) */
AMB_DRAM_ARBITER_DSP_LOW_PRIORITY,
/** Normal priority for dsp clients (87.5% of total bandwidth) */
AMB_DRAM_ARBITER_DSP_NORMAL_PRIORITY,
/** High priority for dsp clients (93.75% of total bandwidth - large arbiter throttle period) */
AMB_DRAM_ARBITER_DSP_HIGH_PRIORITY_HIGH_THROTTLE,
/** High priority for dsp clients (93.75% of total bandwidth) */
AMB_DRAM_ARBITER_DSP_HIGH_PRIORITY,
/** High priority for dsp clients (96.8% of total bandwidth) */
AMB_DRAM_ARBITER_DSP_VERY_HIGH_PRIORITY,
/** High priority for dsp clients (100% of total bandwidth) */
AMB_DRAM_ARBITER_DSP_HIGHEST_PRIORITY,
/* Reserved */
AMB_DRAM_ARBITER_DSP_RESERVED = 0xffffffffUL
} amb_dram_arbiter_priority_t ;

/*
 *
 */

typedef unsigned int amb_hal_function_index_t ;
typedef unsigned int (*amb_hal_function_t) (unsigned int, unsigned int, unsigned int, unsigned int) ;

/*
 * Common inline function to call the ambhal functions
 * Do not call this function directly !!
 * Use the inline wrappers below instead
 */

static INLINE unsigned int amb_hal_function_call (void *amb_hal_base_address, amb_hal_function_info_index_t amb_hal_function_index, unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
  unsigned int rval;
  amb_hal_function_t amb_hal_function = (amb_hal_function_t) ((*((unsigned int*) (((unsigned int*) amb_hal_base_address) + 32 + (amb_hal_function_index*2)))) + ((unsigned int) amb_hal_base_address)) ;
  AMBARELLA_HAL_OS_LOCK();
  rval = amb_hal_function (arg0, arg1, arg2, arg3) ;
  AMBARELLA_HAL_OS_UNLOCK();

  return rval;
}

/**
 * Initialize the ambhal.
 *
 * @note This must be called before any other ambhal functions are invoked.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_apb_peripherals_base_address Virtual address of peripherals (corresponding to
 * physical address 0x70000000)
 * @param[in] amb_ahb_peripherals_base_address Virtual address of peripherals (corresponding to
 * physical address 0x60000000)
 *
 * @retval ::AMB_HAL_SUCCESS ambhal initialization was successful
 *
 * @retval ::AMB_HAL_FAIL ambhal system failure
 *
 * @ingroup init_group
 */

static INLINE amb_hal_success_t amb_hal_init (void *amb_hal_base_address, void *amb_apb_peripherals_base_address, void *amb_ahb_peripherals_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_HAL_INIT, (unsigned int) amb_apb_peripherals_base_address, (unsigned int) amb_ahb_peripherals_base_address, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the chip name.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_hal_chip_name Pointer to the name of the device.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup init_group
 */

static INLINE amb_hal_success_t amb_get_chip_name (void *amb_hal_base_address, char **amb_hal_chip_name)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CHIP_NAME, (unsigned int) amb_hal_chip_name, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the hal version.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_hal_version Pointer to the version of hal.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup init_group
 */

static INLINE amb_hal_success_t amb_get_version (void *amb_hal_base_address, unsigned int *amb_hal_version)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VERSION, (unsigned int) amb_hal_version, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the current operating mode for the system
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_operating_mode New operating mode.
 *
 * @retval ::AMB_HAL_SUCCESS The new operating mode has been programmed.
 *
 * @par
 * amb_get_operating_mode_status() must be called after this to check
 * whether the new operating mode has taken effect.
 *
 * @retval ::AMB_HAL_RETRY Another operation is in progress. Try later
 * @retval ::AMB_HAL_FAIL The new operating mode was not set because of invalid arguments.
 *
 * @ingroup mode_group
 */

static INLINE amb_hal_success_t amb_set_operating_mode (void *amb_hal_base_address, amb_operating_mode_t *amb_operating_mode)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_OPERATING_MODE, (unsigned int) amb_operating_mode, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current operating mode for the system
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_operating_mode Current operating mode.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup mode_group
 */

static INLINE amb_hal_success_t amb_get_operating_mode (void *amb_hal_base_address, amb_operating_mode_t *amb_operating_mode)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_OPERATING_MODE, (unsigned int) amb_operating_mode, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Check whether a previous amb_set_operating_mode() call has completed.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS The new operating mode has been set.
 * @retval ::AMB_HAL_RETRY The new operating mode has not been set yet.
 * @retval ::AMB_HAL_FAIL The new operating mode has failed.
 *
 * @ingroup mode_group
 */

static INLINE amb_hal_success_t amb_get_operating_mode_status (void *amb_hal_base_address)
{
  // AMBHALUNUSED(amb_hal_unused) = 0 ;
  // return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_OPERATING_MODE_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
  return AMB_HAL_FAIL ;
}

/**
 * Get the reference clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The reference clock frequency
 * from the system configuration pins.
 *
 * @ingroup sysconfig_group
 */

static INLINE amb_clock_frequency_t amb_get_reference_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_REFERENCE_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the system configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @note Use the flags defined in ::amb_system_configuration_t to determine
 * what system configuration was set.
 *
 * @retval ::amb_system_configuration_t The system configuration.
 *
 * @ingroup sysconfig_group
 */

static INLINE amb_system_configuration_t amb_get_system_configuration (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_system_configuration_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SYSTEM_CONFIGURATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the boot type selection
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_boot_type_t The boot type selected.
 *
 * @ingroup sysconfig_group
 */

static INLINE amb_boot_type_t amb_get_boot_type (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_boot_type_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_BOOT_TYPE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the host interface type 
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_hif_type_t The host interface type selected.
 *
 * @ingroup sysconfig_group
 */

static INLINE amb_hif_type_t amb_get_hif_type (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hif_type_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_HIF_TYPE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Disable clock observation.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup pll_group
 */

static INLINE amb_hal_success_t amb_disable_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_DISABLE_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * IDSP
 *
 */

/**
 * Get the current idsp pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_idsp_pll_configuration pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup idsp_group
 */

static INLINE amb_hal_success_t amb_get_idsp_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_idsp_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_IDSP_PLL_CONFIGURATION, (unsigned int) amb_idsp_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current idsp pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup idsp_group
 */

static INLINE amb_clock_frequency_t amb_get_idsp_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_IDSP_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested idsp pll frequency change
 *
 * @note A new idsp pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup idsp_group
 */

static INLINE amb_hal_success_t amb_get_idsp_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_IDSP_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of idsp clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup idsp_group
 */

static INLINE amb_hal_success_t amb_enable_idsp_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_IDSP_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Core
 *
 */

/**
 * Get the current core pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_core_pll_configuration pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup core_group
 */

static INLINE amb_hal_success_t amb_get_core_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_core_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORE_PLL_CONFIGURATION, (unsigned int) amb_core_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current core pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup core_group
 */

static INLINE amb_clock_frequency_t amb_get_core_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORE_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested core pll frequency change
 *
 * @note A new core pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup core_group
 */

static INLINE amb_hal_success_t amb_get_core_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORE_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of core clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup core_group
 */

static INLINE amb_hal_success_t amb_enable_core_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_CORE_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * DDR
 *
 */

/**
 * Get the current ddr pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ddr_pll_configuration pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ddr_group
 */

static INLINE amb_hal_success_t amb_get_ddr_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_ddr_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_DDR_PLL_CONFIGURATION, (unsigned int) amb_ddr_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current ddr pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup ddr_group
 */

static INLINE amb_clock_frequency_t amb_get_ddr_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_DDR_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested ddr pll frequency change
 *
 * @note A new ddr pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup ddr_group
 */

static INLINE amb_hal_success_t amb_get_ddr_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_DDR_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of ddr clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup ddr_group
 */

static INLINE amb_hal_success_t amb_enable_ddr_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_DDR_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * CORTEX
 *
 */

/**
 * Set the cortex pll frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_cortex_clock_frequency The requested frequency.
 *
 * @retval ::AMB_HAL_SUCCESS the new requested pll frequency is valid and it has
 * been programmed.
 *
 * @retval ::AMB_HAL_FAIL the new pll frequency requested is not supported.
 * @retval ::AMB_HAL_RETRY a previous pll frequency change request is still
 * outstanding.
 *
 * @ingroup cortex_group
 */

static INLINE amb_hal_success_t amb_set_cortex_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_cortex_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_CORTEX_CLOCK_FREQUENCY, amb_cortex_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current cortex pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_cortex_pll_configuration pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup cortex_group
 */

static INLINE amb_hal_success_t amb_get_cortex_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_cortex_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORTEX_PLL_CONFIGURATION, (unsigned int) amb_cortex_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current cortex pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup cortex_group
 */

static INLINE amb_clock_frequency_t amb_get_cortex_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORTEX_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested cortex pll frequency change
 *
 * @note A new cortex pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup cortex_group
 */

static INLINE amb_hal_success_t amb_get_cortex_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_CORTEX_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of cortex clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup cortex_group
 */

static INLINE amb_hal_success_t amb_enable_cortex_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_CORTEX_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Sensor
 *
 */

/**
 * Set the sensor pll frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_sensor_clock_frequency The requested frequency.
 *
 * @retval ::AMB_HAL_SUCCESS the new requested pll frequency is valid and it has
 * been programmed.
 *
 * @retval ::AMB_HAL_FAIL the new pll frequency requested is not supported.
 * @retval ::AMB_HAL_RETRY a previous pll frequency change request is still
 * outstanding.
 *
 * @ingroup sensor_group
 */

static INLINE amb_hal_success_t amb_set_sensor_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_sensor_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SENSOR_CLOCK_FREQUENCY, amb_sensor_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current sensor pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_sensor_pll_configuration Sensor pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup sensor_group
 */

static INLINE amb_hal_success_t amb_get_sensor_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_sensor_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SENSOR_PLL_CONFIGURATION, (unsigned int) amb_sensor_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current sensor pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup sensor_group
 */

static INLINE amb_clock_frequency_t amb_get_sensor_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SENSOR_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested sensor pll frequency change
 *
 * @note A new sensor pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup sensor_group
 */

static INLINE amb_hal_success_t amb_get_sensor_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SENSOR_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of sensor clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup sensor_group
 */

static INLINE amb_hal_success_t amb_enable_sensor_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_SENSOR_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * USB
 *
 */

/**
 * Apply the usb device controller soft reset
 *
 * @note This function triggers a soft reset for the usb device controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS reset sequence has completed
 *
 * @ingroup usb_group
 */

static INLINE amb_hal_success_t amb_usb_device_soft_reset (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_USB_DEVICE_SOFT_RESET, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Apply the usb host controller soft reset
 *
 * @note This function triggers a soft reset for the usb host controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS reset sequence has completed
 *
 * @ingroup usb_group
 */

static INLINE amb_hal_success_t amb_usb_host_soft_reset (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_USB_HOST_SOFT_RESET, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Suspend/un-suspend USB Port
 *
 * @note This function suspends the USB Port if ::AMB_USB_SUSPEND is specified. 
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] usb_port_state Requested State of the USB Device
 *
 * @retval ::AMB_HAL_SUCCESS Port state has been set
 * @retval ::AMB_HAL_FAIL Port state is not valid
 *
 * @ingroup usb_group
 */

static INLINE amb_hal_success_t amb_set_usb_port_state (void *amb_hal_base_address, amb_usb_port_state_t usb_port_state)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_USB_PORT_STATE, usb_port_state, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the state of the USB Port
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_USB_ON if USB Device is enabled
 * @retval ::AMB_USB_SUSPEND if USB Device is suspended
 *
 * @ingroup usb_group
 */

static INLINE amb_usb_port_state_t amb_get_usb_port_state (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_usb_port_state_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_USB_PORT_STATE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * LCD
 *
 */

/**
 * Set the clock source for lcd.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source the new clock source.
 * @param[in] amb_clock_frequency the clock frequency of the new source.
 *
 * @note The amb_clock_frequency only needs to be specified for the
 * clock sources AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI and
 * AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. Specify
 * an amb_clock_frequency of 0 for all other clock sources.
 * The topic @ref clocksource_page covers this in more details.
 *
 * @retval ::AMB_HAL_SUCCESS a new clock source has been set.
 * @retval ::AMB_HAL_FAIL the new clock source is not supported.
 *
 * @ingroup lcd_group
 */

static INLINE amb_hal_success_t amb_set_lcd_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_LCD_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the lcd pll frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_lcd_clock_frequency The requested frequency.
 *
 * @retval ::AMB_HAL_SUCCESS the new requested pll frequency is valid and it has
 * been programmed.
 *
 * @retval ::AMB_HAL_FAIL the new pll frequency requested is not supported.
 * @retval ::AMB_HAL_RETRY a previous pll frequency change request is still
 * outstanding.
 *
 * @ingroup lcd_group
 */

static INLINE amb_hal_success_t amb_set_lcd_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_lcd_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_LCD_CLOCK_FREQUENCY, amb_lcd_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current lcd pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_lcd_pll_configuration Sensor pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup lcd_group
 */

static INLINE amb_hal_success_t amb_get_lcd_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_lcd_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_LCD_PLL_CONFIGURATION, (unsigned int) amb_lcd_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current lcd pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup lcd_group
 */

static INLINE amb_clock_frequency_t amb_get_lcd_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_LCD_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested lcd pll frequency change
 *
 * @note A new lcd pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup lcd_group
 */

static INLINE amb_hal_success_t amb_get_lcd_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_LCD_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of lcd clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup lcd_group
 */

static INLINE amb_hal_success_t amb_enable_lcd_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_LCD_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * VOUT
 *
 */

/**
 * Set the clock source for vout.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source the new clock source.
 * @param[in] amb_clock_frequency the clock frequency of the new source.
 *
 * @note The amb_clock_frequency only needs to be specified for the
 * clock sources AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI and
 * AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. Specify
 * an amb_clock_frequency of 0 for all other clock sources.
 * The topic @ref clocksource_page covers this in more details.
 *
 * @retval ::AMB_HAL_SUCCESS a new clock source has been set.
 * @retval ::AMB_HAL_FAIL the new clock source is not supported.
 *
 * @ingroup vout_group
 */

static INLINE amb_hal_success_t amb_set_vout_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VOUT_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the vout pll frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_vout_clock_frequency The requested frequency.
 *
 * @retval ::AMB_HAL_SUCCESS the new requested pll frequency is valid and it has
 * been programmed.
 *
 * @retval ::AMB_HAL_FAIL the new pll frequency requested is not supported.
 * @retval ::AMB_HAL_RETRY a previous pll frequency change request is still
 * outstanding.
 *
 * @ingroup vout_group
 */

static INLINE amb_hal_success_t amb_set_vout_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_vout_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VOUT_CLOCK_FREQUENCY, amb_vout_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current vout pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_vout_pll_configuration Sensor pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup vout_group
 */

static INLINE amb_hal_success_t amb_get_vout_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_vout_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VOUT_PLL_CONFIGURATION, (unsigned int) amb_vout_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current vout pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup idsp_group
 */

static INLINE amb_clock_frequency_t amb_get_vout_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VOUT_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested vout pll frequency change
 *
 * @note A new vout pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup vout_group
 */

static INLINE amb_hal_success_t amb_get_vout_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VOUT_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of vout clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup vout_group
 */

static INLINE amb_hal_success_t amb_enable_vout_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_VOUT_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Audio
 *
 */

/**
 * Set the clock source for audio.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source the new clock source.
 * @param[in] amb_clock_frequency the clock frequency of the new source.
 *
 * @note The amb_clock_frequency only needs to be specified for the
 * clock sources AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI and
 * AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. Specify
 * an amb_clock_frequency of 0 for all other clock sources.
 * The topic @ref clocksource_page covers this in more details.
 *
 * @retval ::AMB_HAL_SUCCESS a new clock source has been set.
 * @retval ::AMB_HAL_FAIL the clock source is not supported.
 *
 * @ingroup audio_group
 */

static INLINE amb_hal_success_t amb_set_audio_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_AUDIO_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the audio pll frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_audio_clock_frequency The requested frequency.
 *
 * @retval ::AMB_HAL_SUCCESS the new requested pll frequency is valid and it has
 * been programmed.
 * @retval ::AMB_HAL_FAIL the new pll frequency requested is not supported.
 * @retval ::AMB_HAL_RETRY a previous pll frequency change request is still
 * outstanding.
 *
 * @ingroup audio_group
 */

static INLINE amb_hal_success_t amb_set_audio_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_audio_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_AUDIO_CLOCK_FREQUENCY, amb_audio_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current audio pll configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_audio_pll_configuration Sensor pll configuration information read
 * from pll registers.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup audio_group
 */

static INLINE amb_hal_success_t amb_get_audio_pll_configuration (void *amb_hal_base_address, amb_pll_configuration_t* amb_audio_pll_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AUDIO_PLL_CONFIGURATION, (unsigned int) amb_audio_pll_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the current audio pll frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup audio_group
 */

static INLINE amb_clock_frequency_t amb_get_audio_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AUDIO_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the previous requested audio pll frequency change
 *
 * @note A new audio pll frequency change may be requested after this function
 * returns ::AMB_HAL_SUCCESS.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS the pll has locked to the new frequency.
 * @retval ::AMB_HAL_FAIL the pll lock has failed to lock in a reasonable amount of time. something is wrong.
 * @retval ::AMB_HAL_RETRY the pll has not locked yet. try again.
 *
 * @ingroup audio_group
 */

static INLINE amb_hal_success_t amb_get_audio_pll_lock_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AUDIO_PLL_LOCK_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of audio clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup audio_group
 */

static INLINE amb_hal_success_t amb_enable_audio_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_AUDIO_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * UART
 *
 */

/**
 * Set the uart clock frequency.
 *
 * @note This is not the baud rate.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New uart clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup uart_group
 */

static INLINE amb_hal_success_t amb_set_uart_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_UART_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the uart clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup uart_group
 */

static INLINE amb_clock_frequency_t amb_get_uart_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_UART_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Initialize the uart.
 *
 * This api call sets up the uart for 115200 bauds, 8 bit data, no parity,
 * 1 stop bit.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup uart_group
 */

static INLINE amb_hal_success_t amb_uart_init (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_UART_INIT, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the uart clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_uart_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup uart_group
 */

static INLINE amb_clock_frequency_t amb_get_uart_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_uart_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_UART_CLOCK_CONFIGURATION, (unsigned int) amb_uart_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the clock source of the uart clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source Clock source for the uart divider.
 *
 * @note: Valid clock sources are: ::AMB_REFERENCE_CLOCK_SOURCE_IDSP, ::AMB_REFERENCE_CLOCK_SOURCE_ARM, ::AMB_REFERENCE_CLOCK_SOURCE_CORE or ::AMB_REFERENCE_CLOCK_SOURCE_CLK_REF.
 * 
 * @retval ::AMB_HAL_SUCCESS The function always returns success.
 *
 * @ingroup uart_group
 */

static INLINE amb_hal_success_t amb_set_uart_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_UART_CLOCK_SOURCE, amb_clock_source, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * VIN
 *
 */

/**
 * Set the vin clock frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New vin clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup vin_group
 */

static INLINE amb_hal_success_t amb_set_vin_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VIN_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the vin clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup vin_group
 */

static INLINE amb_clock_frequency_t amb_get_vin_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VIN_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the vin clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_vin_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup vin_group
 */

static INLINE amb_clock_frequency_t amb_get_vin_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_vin_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VIN_CLOCK_CONFIGURATION, (unsigned int) amb_vin_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * PIP
 *
 */

/**
 * Set the picture in picture clock frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New pip clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup pip_group
 */

static INLINE amb_hal_success_t amb_set_pip_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PIP_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the pip clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup pip_group
 */

static INLINE amb_clock_frequency_t amb_get_pip_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PIP_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the pip clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_pip_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup pip_group
 */

static INLINE amb_clock_frequency_t amb_get_pip_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_pip_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PIP_CLOCK_CONFIGURATION, (unsigned int) amb_pip_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Motor
 *
 */

/**
 * Set the motor clock frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New motor clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup motor_group
 */

static INLINE amb_hal_success_t amb_set_motor_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MOTOR_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the motor clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup motor_group
 */

static INLINE amb_clock_frequency_t amb_get_motor_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MOTOR_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the motor clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_motor_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup motor_group
 */

static INLINE amb_clock_frequency_t amb_get_motor_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_motor_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MOTOR_CLOCK_CONFIGURATION, (unsigned int) amb_motor_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Infrared
 *
 */

/**
 * Set the infrared clock frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New infrared clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup ir_group
 */

static INLINE amb_hal_success_t amb_set_ir_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_IR_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the ir clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency.
 *
 * @ingroup ir_group
 */

static INLINE amb_clock_frequency_t amb_get_ir_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_IR_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the ir clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ir_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup ir_group
 */

static INLINE amb_clock_frequency_t amb_get_ir_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_ir_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_IR_CLOCK_CONFIGURATION, (unsigned int) amb_ir_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * SSI
 *
 */

/**
 * Set the clock frequency of the ssi.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New ssi frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup ssi_group
 */

static INLINE amb_hal_success_t amb_set_ssi_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SSI_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the ssi clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup ssi_group
 */

static INLINE amb_clock_frequency_t amb_get_ssi_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SSI_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the ssi clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ssi_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup ssi_group
 */

static INLINE amb_clock_frequency_t amb_get_ssi_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_ssi_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SSI_CLOCK_CONFIGURATION, (unsigned int) amb_ssi_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the clock source of the ssi AND ssi2 clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source Clock source for the ssi divider.
 * @param[in] amb_clock_frequency Clock frequency of the selected source.
 *
 * @note: Valid clock sources are: AMB_REFERENCE_CLOCK_SOURCE_APB or AMB_REFERENCE_CLOCK_SOURCE_SPCLK_C. The amb_clock_frequency must be specified for AMB_REFERENCE_CLOCK_SOURCE_SPCLK_C. Otherwise it should be set to 0.
 * 
 * @retval ::AMB_HAL_SUCCESS The function always returns success.
 *
 * @ingroup ssi_group
 */

static INLINE amb_hal_success_t amb_set_ssi_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SSI_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}


/*
 *
 * SSI2
 *
 */

/**
 * Set the clock frequency of the ssi2.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New ssi2 frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup ssi2_group
 */

static INLINE amb_hal_success_t amb_set_ssi2_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SSI2_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the ssi2 clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup ssi2_group
 */

static INLINE amb_clock_frequency_t amb_get_ssi2_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SSI2_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the clock source of the ssi AND ssi2 clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source Clock source for the ssi divider.
 * @param[in] amb_clock_frequency Clock frequency of the selected source.
 *
 * @note: Valid clock sources are: AMB_REFERENCE_CLOCK_SOURCE_APB or AMB_REFERENCE_CLOCK_SOURCE_SPCLK_C. The amb_clock_frequency must be specified for AMB_REFERENCE_CLOCK_SOURCE_SPCLK_C. Otherwise it should be set to 0.
 * 
 * @retval ::AMB_HAL_SUCCESS The function always returns success.
 *
 * @ingroup ssi2_group
 */

static INLINE amb_hal_success_t amb_set_ssi2_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SSI2_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * PWM
 *
 */

/**
 * Set the clock frequency of the pwm.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New pwm frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup pwm_group
 */

static INLINE amb_hal_success_t amb_set_pwm_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PWM_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the pwm clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup pwm_group
 */

static INLINE amb_clock_frequency_t amb_get_pwm_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PWM_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the pwm clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_pwm_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup pwm_group
 */

static INLINE amb_clock_frequency_t amb_get_pwm_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_pwm_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PWM_CLOCK_CONFIGURATION, (unsigned int) amb_pwm_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the clock source of the pwm clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source Clock source for the pwm divider.
 * @param[in] amb_clock_frequency Clock frequency of the selected source.
 *
 * @note: Valid clock sources are: AMB_REFERENCE_CLOCK_SOURCE_APB or AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. The amb_clock_frequency must be specified for AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. Otherwise it should be set to 0.
 * 
 * @retval ::AMB_HAL_SUCCESS The function always returns success.
 *
 * @ingroup pwm_group
 */

static INLINE amb_hal_success_t amb_set_pwm_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PWM_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * PWMOIS
 *
 */

/**
 * Set the clock frequency of the pwmois.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New pwmois frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup pwmois_group
 */

static INLINE amb_hal_success_t amb_set_pwmois_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PWMOIS_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the pwmois clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup pwmois_group
 */

static INLINE amb_clock_frequency_t amb_get_pwmois_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PWMOIS_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the pwmois clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_pwmois_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup pwmois_group
 */

static INLINE amb_clock_frequency_t amb_get_pwmois_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_pwmois_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_PWMOIS_CLOCK_CONFIGURATION, (unsigned int) amb_pwmois_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the clock source of the pwmois clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_source Clock source for the pwmois divider.
 * @param[in] amb_clock_frequency Clock frequency of the selected source.
 *
 * @note: Valid clock sources are: ::AMB_REFERENCE_CLOCK_SOURCE_IDSP_PLL_VCO, ::AMB_REFERENCE_CLOCK_SOURCE_AUDIO_PLL_VCO, ::AMB_REFERENCE_CLOCK_SOURCE_CORE_PLL_VCO or ::AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. The amb_clock_frequency must be specified for ::AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK. Otherwise it should be set to 0.
 * 
 * @retval ::AMB_HAL_SUCCESS The function always returns success.
 *
 * @ingroup pwmois_group
 */

static INLINE amb_hal_success_t amb_set_pwmois_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_clock_source, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PWMOIS_CLOCK_SOURCE, amb_clock_source, amb_clock_frequency, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * ADC
 *
 */

/**
 * Set the adc clock frequency.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New adc clock frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL    The new frequency is invalid and has not been set
 *
 * @ingroup adc_group
 */

static INLINE amb_hal_success_t amb_set_adc_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_ADC_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the adc clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup adc_group
 */

static INLINE amb_clock_frequency_t amb_get_adc_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_ADC_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the adc clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_adc_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup adc_group
 */

static INLINE amb_clock_frequency_t amb_get_adc_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_adc_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_ADC_CLOCK_CONFIGURATION, (unsigned int) amb_adc_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * SD
 *
 */

/**
 * Set the clock frequency of the sd controller.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New smiod frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL The new requested frequency is not valid
 *
 * @ingroup sd_group
 */

static INLINE amb_hal_success_t amb_set_sd_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SD_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the sd clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup sd_group
 */

static INLINE amb_clock_frequency_t amb_get_sd_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SD_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the sd clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_sd_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup sd_group
 */

static INLINE amb_clock_frequency_t amb_get_sd_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_sd_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SD_CLOCK_CONFIGURATION, (unsigned int) amb_sd_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * SDIO
 *
 */

/**
 * Set the clock frequency of the sdio clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New sdio frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL The new requested frequency is not valid
 *
 * @ingroup sdio_group
 */

static INLINE amb_hal_success_t amb_set_sdio_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SDIO_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the sdio clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup sdio_group
 */

static INLINE amb_clock_frequency_t amb_get_sdio_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SDIO_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the sdio clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_sdio_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup sdio_group
 */

static INLINE amb_clock_frequency_t amb_get_sdio_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_sdio_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SDIO_CLOCK_CONFIGURATION, (unsigned int) amb_sdio_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * GTX
 *
 */

/**
 * Set the clock frequency of the gigabit ethernet controller.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New gtx frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup gtx_group
 */

static INLINE amb_hal_success_t amb_set_gtx_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_GTX_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the gtx clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup gtx_group
 */

static INLINE amb_clock_frequency_t amb_get_gtx_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the gtx clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_gtx_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup gtx_group
 */

static INLINE amb_clock_frequency_t amb_get_gtx_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_gtx_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_CONFIGURATION, (unsigned int) amb_gtx_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * FDET
 *
 */

/**
 * Set the clock frequency of the face detection unit
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New fdet frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup fdet_group
 */

static INLINE amb_hal_success_t amb_set_fdet_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_FDET_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the fdet clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup fdet_group
 */

static INLINE amb_clock_frequency_t amb_get_fdet_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_FDET_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the fdet clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_fdet_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup fdet_group
 */

static INLINE amb_clock_frequency_t amb_get_fdet_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_fdet_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_FDET_CLOCK_CONFIGURATION, (unsigned int) amb_fdet_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * AU1CH
 *
 */

/**
 * Set the clock frequency of the single channel audio controller.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New au1ch frequency
 *
 * @retval ::AMB_HAL_SUCCESS the new frequency has been set
 * @retval ::AMB_HAL_FAIL the new requested frequency is not valid
 *
 * @ingroup au1ch_group
 */

static INLINE amb_hal_success_t amb_set_au1ch_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_AU1CH_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency of the au1ch clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup au1ch_group
 */

static INLINE amb_clock_frequency_t amb_get_au1ch_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AU1CH_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the configuration of the au1ch clock divider.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_au1ch_divider_configuration Clock configuration information read from divider.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success.
 *
 * @ingroup au1ch_group
 */

static INLINE amb_clock_frequency_t amb_get_au1ch_clock_configuration (void *amb_hal_base_address, amb_divider_configuration_t* amb_au1ch_divider_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AU1CH_CLOCK_CONFIGURATION, (unsigned int) amb_au1ch_divider_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Memory Stick
 *
 */

/**
 * Set the clock frequency of the memory stick controller.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_clock_frequency New memory stick frequency
 *
 * @retval ::AMB_HAL_SUCCESS The new frequency has been set
 * @retval ::AMB_HAL_FAIL The new requested frequency is not valid
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_clock_frequency (void *amb_hal_base_address, amb_clock_frequency_t amb_clock_frequency)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_CLOCK_FREQUENCY, amb_clock_frequency, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the frequency the memory stick controller clock.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t Requested clock frequency
 *
 * @ingroup ms_group
 */

static INLINE amb_clock_frequency_t amb_get_ms_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return  (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MS_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the memory stick sclk delay
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ms_delay Requested delay.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_sclk_delay (void *amb_hal_base_address, amb_ms_delay_t amb_ms_delay)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_SCLK_DELAY, amb_ms_delay, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the memory stick sd input delay
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ms_delay Requested delay.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_sd_input_delay (void *amb_hal_base_address, amb_ms_delay_t amb_ms_delay)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_SD_INPUT_DELAY, amb_ms_delay, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the memory stick sd output delay
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ms_delay Requested delay.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_sd_output_delay (void *amb_hal_base_address, amb_ms_delay_t amb_ms_delay)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_SD_OUTPUT_DELAY, amb_ms_delay, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the memory stick read delay
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ms_delay Requested delay.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_read_delay (void *amb_hal_base_address, amb_ms_delay_t amb_ms_delay)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_READ_DELAY, amb_ms_delay, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}


/**
 * Get the current memory stick delay configuration.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ms_delay_configuration Memory stick delays read from the delay
 * configuration register.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_get_ms_delay_configuration (void *amb_hal_base_address, amb_ms_delay_configuration_t* amb_ms_delay_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MS_DELAY_CONFIGURATION, (unsigned int) amb_ms_delay_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable/Disable the memory stick controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ms_status Status of the memory stick controller
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup ms_group
 */

static INLINE amb_hal_success_t amb_set_ms_status (void *amb_hal_base_address, amb_ms_status_t amb_ms_status)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MS_STATUS, amb_ms_status, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the status of the memory stick controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_ms_status_t Status of the memory stick controller
 *
 * @ingroup ms_group
 */

static INLINE amb_ms_status_t amb_get_ms_status (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_ms_status_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MS_STATUS, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Flash IO
 *
 */

/**
 * Reset the flash controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup fio_group
 */

static INLINE amb_hal_success_t amb_reset_flash (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_FLASH, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Reset the xd controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup fio_group
 */

static INLINE amb_hal_success_t amb_reset_xd (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_XD, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Reset the cf controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup fio_group
 */

static INLINE amb_hal_success_t amb_reset_cf (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_CF, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Reset the fio controller
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup fio_group
 */

static INLINE amb_hal_success_t amb_reset_fio (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_FIO, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Reset the fio, cf, xd & flash controller all at once
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup fio_group
 */

static INLINE amb_hal_success_t amb_reset_all (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_ALL, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * Miscellaneous
 *
 */

/*
 * Reset the watchdog timer
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup watchdog_group

static INLINE amb_hal_success_t amb_reset_watchdog (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_WATCHDOG, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}
*/


/**
 * Reset the chip.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @note This api call is implemented so that it returns ::AMB_HAL_SUCCESS but a warm reset will
 * restart the entire system and so do not expect to do anything else after this call is made.
 *
 * @retval ::AMB_HAL_SUCCESS always returns success.
 *
 * @ingroup reset_group
 */

static INLINE amb_hal_success_t amb_reset_chip (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_RESET_CHIP, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the lvds pad mode
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_lvds_pad_mode The mode for lvds pads
 *
 * @retval ::AMB_HAL_SUCCESS The new pad mode was set.
 * @retval ::AMB_HAL_FAIL The requested pad mode is not valid.
 *
 * @ingroup lvds_group
 */

static INLINE amb_hal_success_t amb_set_lvds_pad_mode (void *amb_hal_base_address, amb_lvds_pad_mode_t amb_lvds_pad_mode)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_LVDS_PAD_MODE, amb_lvds_pad_mode, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the lvds pad mode
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval amb_lvds_pad_mode_t The current pad mode setting.
 *
 * @ingroup lvds_group
 */

static INLINE amb_lvds_pad_mode_t amb_get_lvds_pad_mode (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_lvds_pad_mode_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_LVDS_PAD_MODE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/*
 *
 * IO Pad Control
 *
 */

/**
 * Set the misc1 io pad drive strength
 *
 * @note These are the pads not covered by the other api calls.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc1_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the misc1 io pad pullup or pulldown
 *
 * @note These are the pads not covered by the other api calls.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc1_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the misc1 io pad input type
 *
 * @note These are the pads not covered by the other api calls.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc1_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC1_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the misc1 io pad configuration
 *
 * @note These are the pads not covered by the other api calls.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_misc1_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MISC1_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the misc2 io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc2_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the misc2 io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc2_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the misc2 io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_misc2_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_MISC2_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the misc2 io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_misc2_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_MISC2_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioa io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioa_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioa io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioa_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioa io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioa_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOA_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the smioa io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_smioa_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SMIOA_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiob io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiob_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiob io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiob_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiob io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiob_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOB_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the smiob io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_smiob_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SMIOB_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioc io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioc_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioc io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioc_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smioc io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smioc_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOC_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the smioc io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_smioc_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SMIOC_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiod io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiod_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiod io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiod_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the smiod io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_smiod_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SMIOD_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the smiod io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_smiod_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SMIOD_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the vd1 io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_vd1_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the vd1 io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_vd1_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the vd1 io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_vd1_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_VD1_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the vd1 io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_vd1_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_VD1_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the sensor io pad drive strength
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_drive_strength The drive strength of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new drive strength was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_sensor_ioctrl_drive_strength (void *amb_hal_base_address, amb_ioctrl_drive_strength_t amb_ioctrl_drive_strength)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_DRIVE_STRENGTH, amb_ioctrl_drive_strength, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the sensor io pad pullup or pulldown
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_pullupdown The pullup/pulldown of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new pullup/pulldown was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_sensor_ioctrl_pullupdown (void *amb_hal_base_address, amb_ioctrl_pullupdown_t amb_ioctrl_pullupdown)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_PULLUPDOWN, amb_ioctrl_pullupdown, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the sensor io pad input type
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_ioctrl_input_type The input type of the io pad
 *
 * @retval ::AMB_HAL_SUCCESS The new input type was set.
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_set_sensor_ioctrl_input_type (void *amb_hal_base_address, amb_ioctrl_input_type_t amb_ioctrl_input_type)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SENSOR_IOCTRL_INPUT_TYPE, amb_ioctrl_input_type, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the sensor io pad configuration
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[out] amb_ioctrl_configuration The current configuration of the io pad
 *
 * @ingroup ioctrl_group
 */

static INLINE amb_hal_success_t amb_get_sensor_ioctrl_configuration (void *amb_hal_base_address, amb_ioctrl_configuration_t *amb_ioctrl_configuration)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SENSOR_IOCTRL_CONFIGURATION, (unsigned int) amb_ioctrl_configuration, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the arm clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The arm clock frequency.
 *
 * @ingroup arm_group
 */

static INLINE amb_clock_frequency_t amb_get_arm_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_ARM_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the hdmi clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The hdmi clock frequency.
 *
 * @ingroup hdmi_group
 */

static INLINE amb_clock_frequency_t amb_get_hdmi_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_HDMI_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of hdmi clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup hdmi_group
 */

static INLINE amb_hal_success_t amb_enable_hdmi_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_HDMI_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the hdmi4k clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The hdmi4k clock frequency.
 *
 * @ingroup hdmi_group
 */

static INLINE amb_clock_frequency_t amb_get_hdmi4k_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_HDMI4K_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Enable observation of hdmi4k clock
 *
 * @note A divided by 16 version of the clock may be observed on the xx_clk_si
 * pin.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Always returns success
 *
 * @ingroup hdmi_group
 */

static INLINE amb_hal_success_t amb_enable_hdmi4k_clock_observation (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_ENABLE_HDMI4K_CLOCK_OBSERVATION, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Auto select range of hdmi4k pll
 *
 * Automatically set the hdmi4k pll range.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::AMB_HAL_SUCCESS Returns success if a valid range was found
 * @retval ::AMB_HAL_FAIL Returns fail if a valid range was not found
 *
 * @ingroup hdmi_group
 */

static INLINE amb_hal_success_t amb_auto_select_hdmi4k_range (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_AUTO_SELECT_HDMI4K_RANGE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the direction of the sensor clock pad.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_sensor_clock_pad_mode The sensor clock pad mode.
 *
 * @ingroup sensor_group
 */

static INLINE amb_hal_success_t amb_set_sensor_clock_pad_mode (void *amb_hal_base_address, amb_sensor_clock_pad_mode_t amb_sensor_clock_pad_mode)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_SENSOR_CLOCK_PAD_MODE, amb_sensor_clock_pad_mode, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the direction of the sensor clock pad.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_sensor_clock_pad_mode_t The sensor clock pad direction (input or output).
 *
 * @ingroup sensor_group
 */

static INLINE amb_sensor_clock_pad_mode_t amb_get_sensor_clock_pad_mode (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_sensor_clock_pad_mode_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_SENSOR_CLOCK_PAD_MODE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Change the base address of the apb and ahb peripherals.
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_apb_peripherals_base_address Virtual address of peripherals (corresponding to
 * physical address 0x70000000)
 * @param[in] amb_ahb_peripherals_base_address Virtual address of peripherals (corresponding to
 * physical address 0x60000000)
 *
 * @retval ::AMB_HAL_SUCCESS ambhal initialization was successful
 *
 * @retval ::AMB_HAL_FAIL ambhal system failure
 *
 * @ingroup init_group
 */

static INLINE amb_hal_success_t amb_set_peripherals_base_address (void *amb_hal_base_address, void *amb_apb_peripherals_base_address, void *amb_ahb_peripherals_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_PERIPHERALS_BASE_ADDRESS, (unsigned int) amb_apb_peripherals_base_address, (unsigned int) amb_ahb_peripherals_base_address, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the AHB clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The AHB clock frequency.
 *
 * @ingroup ahb_group
 */

static INLINE amb_clock_frequency_t amb_get_ahb_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AHB_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the APB clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The APB clock frequency.
 *
 * @ingroup apb_group
 */

static INLINE amb_clock_frequency_t amb_get_apb_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_APB_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the AXI clock frequency
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval ::amb_clock_frequency_t The AXI clock frequency.
 *
 * @ingroup cortex_group
 */

static INLINE amb_clock_frequency_t amb_get_axi_clock_frequency (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_frequency_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_AXI_CLOCK_FREQUENCY, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Set the GTX clock source
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_gtx_clock_source The gtx clock source.
 *
 * @note: Valid clock sources are: AMB_REFERENCE_CLOCK_SOURCE_CORTEX_PLL_VCO and AMB_EXTERNAL_CLOCK_SOURCE.
 *
 * @retval ::AMB_HAL_SUCCESS The gtx clock source was set properly.
 * @retval ::AMB_HAL_FAIL The requested gtx clock source is not valid.
 *
 * @ingroup gtx_group
 */

static INLINE amb_hal_success_t amb_set_gtx_clock_source (void *amb_hal_base_address, amb_clock_source_t amb_gtx_clock_source)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_GTX_CLOCK_SOURCE, amb_gtx_clock_source, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Get the GTX clock source
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 *
 * @retval amb_clock_source_t The gtx clock source.
 *
 * @ingroup gtx_group
 */

static INLINE amb_clock_source_t amb_get_gtx_clock_source (void *amb_hal_base_address)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_clock_source_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_GET_GTX_CLOCK_SOURCE, amb_hal_unused, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

/**
 * Change the priority of dsp clients in DRAM arbiter
 *
 * @param[in] amb_hal_base_address Virtual address where ambhal is loaded by OS.
 * @param[in] amb_dram_arbiter_priority Priority given to dsp clients by DRAM arbiter.
 *
 * @retval ::AMB_HAL_SUCCESS ambhal initialization was successful
 *
 * @retval ::AMB_HAL_FAIL The amb_dram_arbiter_priority is not defined
 *
 * @ingroup init_group
 */

static INLINE amb_hal_success_t amb_set_dram_arbiter_priority (void *amb_hal_base_address, amb_dram_arbiter_priority_t amb_dram_arbiter_priority)
{
  AMBHALUNUSED(amb_hal_unused) = 0 ;
  return (amb_hal_success_t) amb_hal_function_call (amb_hal_base_address, AMB_HAL_FUNCTION_INFO_SET_DRAM_ARBITER_PRIORITY, (unsigned int) amb_dram_arbiter_priority, amb_hal_unused, amb_hal_unused, amb_hal_unused) ;
}

#endif // ifndef _AMBHAL_H_INCLUDED_

