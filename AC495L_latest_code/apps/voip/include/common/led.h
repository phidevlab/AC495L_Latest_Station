/********************************************************************
 * AudioCodes improved application led api                          *
 *                                                                  *
 * File Name:	led.h                                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _LED_H_
#define _LED_H_

/* Indicator number must be between 0 and 19 */
#define LED_MAX_INDICATOR 19

enum _ledsType
{
	e_INVALID_LED = -1,
	e_LED_PHONE0,
	e_LED_PHONE1,
	e_LED_PHONE2,
	e_LED_PHONE3,
	e_LED_FXO,
	e_LED_STATUS,
	e_MAX_LED_NUM
};

enum _LedStates//ER0106 LED
{
	e_LED_STATE_OFF,
	e_GREEN_LED_STATE_ON,
	e_GREEN_LED_STATE_SLOW_FLASH,
	e_GREEN_LED_STATE_FAST_FLASH,
	e_GREEN_LED_STATE_ONESHOT_ON,
	e_GREEN_LED_STATE_VERY_SLOW_FLASH,
#ifdef CONFIG_AC494_LE88
	e_RED_LED_STATE_ON = e_LED_STATE_OFF,
	e_RED_LED_STATE_SLOW_FLASH = e_LED_STATE_OFF,
	e_RED_LED_STATE_FAST_FLASH = e_LED_STATE_OFF,
	e_RED_LED_STATE_ONESHOT_ON = e_LED_STATE_OFF,
	e_RED_LED_STATE_VERY_SLOW_FLASH = e_LED_STATE_OFF,
#else
	e_RED_LED_STATE_ON,
	e_RED_LED_STATE_SLOW_FLASH,
	e_RED_LED_STATE_FAST_FLASH,
	e_RED_LED_STATE_ONESHOT_ON,
	e_RED_LED_STATE_VERY_SLOW_FLASH,
#endif
	e_NUM_OF_LED_STATES,
	e_INVALID_STATE = e_NUM_OF_LED_STATES

};

/* Prepare the LED indicators to be used
 * Returns: 0 on success, -1 on failure */
int led_init(void);

/* CLI function prototypes*/
int led_handle_debug_cmd(void *arg);

/* Free LED related resources */
void led_uninit(void);

/* Resets the write vector */
void led_clear(void);

int set_phone_led_to_state(int led_num, int state);//ER0106 - LED

//ER06 - Allow reading the current LED state
int get_phone_led_state(int led_num);

/* Set the specified LED indicator. 
 * Returns: 0 on success, -1 if write fails for any reason
 */
int led_set(int led_number);

/* Reset the specified LED indicator. 
 * Returns: 0 on success, -1 if write fails for any reason
 */
int led_reset(int led_number);

int set_all_led_to_state(int state);

#endif
