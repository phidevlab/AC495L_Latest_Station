/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_ui_commands.c                                   *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include <string.h>

//#include "voip_lang.h"

#include "voip_lcd.h"
#include "led.h"
#include "acl_line_connection.h"//060530 Avner phase 1 - switch places with dsp.h
//#include "dsp.h"
#include "acl_ui_commands.h"
#include "voip_utils.h"
#include "acl_dsp_telephony_commands.h"
//#include "voip_ipc.h"

/* LED indicators definitions */
/* All states but Idle, Indicates offhook */
#ifdef CONFIG_INCAIP_FLEXTRONICS
#define LED_OFFHOOK 9
#else
#define LED_OFFHOOK 8
#endif
/* Dial state, Indicates that user is dialing. */
#ifdef CONFIG_INCAIP_FLEXTRONICS
#define LED_DIAL    8
#else
#define LED_DIAL    9
#endif
/* Set-up state, Indicates that a call is initiated */
#define LED_CALL    10
/* Ring state, Indicates an incoming call (blinking LED) */
#define LED_RING    11
/* For Connected state, Indicates that a voice connection is active */
#define LED_SPEAK   12
/* For all states, LED on only if Handsfree mode is active */
#define LED_HFREE   13
/* Hold state */
#define LED_HOLD    14
/* MWI state ACL NB 060927 */
#define LED_MWI    15

int phone_output_init(void)
{
	return 0;
}

void phone_output_uninit(void)
{

}

void phone_output_show_volume(int setting)
{

}

void phone_output_clear(void)
{

}

void phone_output_print(int c, int r, char *str)
{

}

void phone_output_ind(char *indid, int state_on)
{

#ifdef CONFIG_RG_IPPHONE


     code2str_t led_mapping[] = {
                {LED_OFFHOOK, "l001",(char**)"",0},
                {LED_RING, "ir",(char**)"",0},
                {LED_HOLD, "il",(char**)"",0},
                {LED_HFREE, "hf",(char**)"",0},
                {-1,"",(char**)"",0}
    };

    int led_number = str2code(led_mapping, indid);

    if (led_number < 0)
         return;

   // if (led_number == LED_RING)
//         dsp_ring(0, state_on ? 1 : 0);

   // if (state_on)
        led_set(8);
   // else
   //     led_reset(led_number);

#endif /*CONFIG_RG_IPPHONE*/


}
/* NirM, 080331 - Defined but not used */
#if 0
/* function formats the str to appear centered in the displayed line
 * it returns a pointer to an internal static string since we assume
 * there will not be multi threading calls here (signal LCD in any case) */
static char *phone_output_format_centered_str(char *str)
{
	return NULL;
}
#endif
void phone_output_print_centered(int r, char *str)
{
}



