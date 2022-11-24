/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	IPPMessageHandler.c                              *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 9/12/08 - Coding started.										*
 *																	*
 ********************************************************************/
/*****************************************************************************
*                                                                            
* 		DESCRIPTION:  	
*                                        
*		implemantation of the function message_handler() defined on IPPMessageQueue.h.
******************************************************************************/


#include "IPPMessageQueue.h"
#include "acl_keypad_adaptation_layer.h"


void message_handler(message_t *msg)
{
	int  pressed;
	keypad_keys key;

	key =(keypad_keys) msg->id;
	pressed = (int)(msg->data[0]);
	
	keypad_report(key, pressed);
	
}

