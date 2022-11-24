/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_dsp_telephony_events.h                          *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _PHONE_INPUT_H_
#define _PHONE_INPUT_H_

#include "acl_line_connection.h"

typedef void (*phone_input_handler_t)(acl_line_t *line, phone_input_event_t e,
    int down);

char *phone_input_get_event_desc(phone_input_event_t input_event);

int phone_input_init(acl_line_t *line);

void phone_input_uninit(acl_line_t *line);

acl_line_t *get_line(int line);

void phone_input_cb(voip_event_info_t *voip_event_info, acl_line_t *line);
void keypad_phone_input_cb(phone_event_t* pAcgEvent,acl_line_t *line);

void dsp_slic_daa_phone_input_cb(acgTEventInfo *pAcgEvent, acl_line_t *line);

void misc_phone_input_cb(voip_misc_phone_event_info *pAcgEvent, acl_line_t *line);


int dsp_select_mode(int n, int *hf);

int get_hook_state(int n);

int set_hook_state(int n,int state);

#endif
