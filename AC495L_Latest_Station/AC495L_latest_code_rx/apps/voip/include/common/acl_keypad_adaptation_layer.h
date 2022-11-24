/**************************************************************************
 * AudioCodes improved application - Keypad adaptation layer *
 *                                                                        *
 * Structure, Enum and Constant Type Definitions                          * 	
 *                                                                        *
 * File Name:	acl_keypad_adaptation_layer.h                                  *
 *                                                                        *
 * Copyright (C) 2007 AudioCodes Ltd.                                     *
 *                                                                        *
 * Modifications :                                                        *
 *                                                                        *
 * 23/10/08 - Coding started.                                          *
 *                                                                        *
 **************************************************************************/

#ifndef ACL_KEYPAD_ADAPTATION_LAYER_H
#define ACL_KEYPAD_ADAPTATION_LAYER_H

#include "acl_common.h"
#include "keypad_keys.h"

typedef struct{
	keypad_keys key;
	phone_input_event_t event;
	int (*handle)(); 
}keysToPhoneEvents;


//int keypad_report(keypad_keys key ,int pressed);
void keypad_report(keypad_keys key ,int pressed);


#endif
