/********************************************************************
 * AudioCodes improved application phone adaptation layer           *
 *                                                                  *
 * File Name:	acl_keypad_adaptation_layer.c                          *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/10/08 - Coding started.                                       *
 *																	*
 ********************************************************************/



#include "acl_keypad_adaptation_layer.h"
#include "acl_dsp_telephony_events.h"
#include "acIPPhoneAPI.h"
#include "acIPPDriversDefs.h"
#ifdef NANOX
#include "nanox_hwLcd.h"
#endif
#include "iphone_paging_pub.h"

#define VOLUME_RESOLUTION_LEVEL 3

static acIPPCodecType audio_device=AC494_CODEC_HANDSET;
static acgEVoiceGain curr_pg_volume=ACG_VOICE_GAIN__0_DB;
static acgEVoiceGain curr_pvt_volume=ACG_VOICE_GAIN__0_DB;
static acgEVoiceGain input_gain=ACG_VOICE_GAIN__0_DB;
int volume_up_handle(int ch);
int volume_down_handle(int ch);
int set_volume(int ch,int level);

static int speaker_handle(int pressed);

void print_volume()
{
	acgEVoiceGain i;
	printf ("volume : %d   ",curr_pg_volume);
	for (i=ACG_VOICE_GAIN__MUTE;i<curr_pg_volume;i++)
		printf("*");
	for (i=curr_pg_volume;i<=ACG_VOICE_GAIN__plus31_DB;i++)
		printf("_");
	printf ("\n");

}

static int paging_event_handle(int pressed,phone_input_event_t event)
{
    if (event >= PHONE_INPUT_PAGING_1 && event < PHONE_INPUT_PAGING_MAX)
    {
    	if (!pressed)
    	{
            if (event < PHONE_INPUT_PAGING_LINE_MAX)
            {
                int line = event - PHONE_INPUT_PAGING_1;
//        		paging_event_received(E_PAGING_OUTGOING_LINE,(void*)line);
            }
            else if (event == PHONE_INPUT_PAGING_IGNORE)
            {
//                paging_event_received(E_PAGING_INCOMING_IGNORE,NULL);
            }
            else if (event == PHONE_INPUT_PAGING_SWITCH)
            {
//                paging_event_received(E_PAGING_INCOMING_SWITCH,NULL);
            }
    	}
     	return 1;
    }
    return 0;
}

int send_to_phone_input_cb(int pressed,int event)
{


	phone_event_t phone_keypad_input;
	voip_event_info_t voip_event_info;
	acl_line_t* line;

    if (paging_event_handle(pressed,event))
        return 0;

	/*there is only one line - therefore can not be "locked" */
	line = (acl_line_t*)get_line(0);
	phone_keypad_input.key=event;
	phone_keypad_input.pressed=pressed;
	voip_event_info.info=(void*)&phone_keypad_input;
	voip_event_info.phone_input_event=KEYPAD_PHONE_INPUT_EVENT;
	phone_input_cb(&voip_event_info,line);
	return 0;

}

static int line_handle(int pressed_line_key)
{
	if (get_hook_state(0)==ON_HOOK)
	{
		if (audio_device!=AC494_CODEC_HEADSET)
		{
			audio_device=AC494_CODEC_SPEAKER;
			return acIPPSetAudioDevice(audio_device);
		}
	}
	return 0;

}

static int off_hook_handle(int pressed)
{
	audio_device=AC494_CODEC_HANDSET;
	printf("in off pressd \n");
	return acIPPSetAudioDevice(audio_device);
}

static int on_hook_handle(int pressed)
{
	if (get_hook_state(0)==OFF_HOOK)
	{
		set_hook_state(0,ON_HOOK);
		if (audio_device==AC494_CODEC_HANDSET)
			send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
	}
	return 0;
}

int volume_up_handle(int channel)
{
	int ret=0;


		//if (curr_volume+VOLUME_RESOLUTION_LEVEL<=ACG_VOICE_GAIN__plus13_DB)
			if (curr_pg_volume<ACG_VOICE_GAIN__plus1_DB)
			{
            printf("CUrrent voulme %d is less than higesht %d \n",curr_pg_volume,ACG_VOICE_GAIN__plus1_DB);
			curr_pg_volume+=VOLUME_RESOLUTION_LEVEL;
			}
            else
            {
                 printf("CUrrent voulme %d is greater than %d \n",curr_pg_volume,ACG_VOICE_GAIN__plus1_DB);
                curr_pg_volume=ACG_VOICE_GAIN__plus1_DB;
            }
        printf("VOL is %d\n",curr_pg_volume/VOLUME_RESOLUTION_LEVEL);
		//ret=acIPPShowVolume(curr_volume/VOLUME_RESOLUTION_LEVEL , ACG_VOICE_GAIN__plus13_DB / VOLUME_RESOLUTION_LEVEL);
	//	if (ret<0)
	//		return ret;
		return acIPPSetOutputGain(channel,curr_pg_volume);

	return 0;
}
int volume_down_handle(int channel)
{
	int ret=0;

		if ((int)curr_pg_volume-VOLUME_RESOLUTION_LEVEL>=ACG_VOICE_GAIN__MUTE)
			curr_pg_volume-=VOLUME_RESOLUTION_LEVEL;
		else
			curr_pg_volume=ACG_VOICE_GAIN__MUTE;
         printf("VOL is %d\n",curr_pg_volume/VOLUME_RESOLUTION_LEVEL);
		//ret=acIPPShowVolume(curr_volume/VOLUME_RESOLUTION_LEVEL , ACG_VOICE_GAIN__plus1_DB / VOLUME_RESOLUTION_LEVEL);
	//	if (ret<0)
	//		return ret;
		return acIPPSetOutputGain(channel,curr_pg_volume);

	return 0;
}

int set_volume(int ch,int level)
{
   if(ch==1)
   {
    level=level*VOLUME_RESOLUTION_LEVEL;
    curr_pg_volume = level;
    acIPPSetOutputGain(ch,curr_pg_volume);
    }
    else
    {
    level=level*VOLUME_RESOLUTION_LEVEL;
    curr_pvt_volume=level;
    acIPPSetOutputGain(ch,curr_pvt_volume);
    }




}

static int mute_handle(int pressed)
{
	if (!pressed)
	{
		switch(input_gain)
		{
			case ACG_VOICE_GAIN__0_DB:
				input_gain=ACG_VOICE_GAIN__MUTE;
				break;
			case ACG_VOICE_GAIN__MUTE:
				input_gain=ACG_VOICE_GAIN__0_DB;
				break;
			default:
				printf("[%s,%d] ERROR - unexpected input_gain \n ",__FUNCTION__,__LINE__);
				input_gain=ACG_VOICE_GAIN__0_DB;
				break;
		}
		return acIPPSetInputGain(input_gain);
	}
	else
		return 0;
}

static int audio_device_handle(acIPPCodecType  device)
{
#ifdef NANOX
    message_t message2Send;
    memset((void*)&message2Send, (char)0, sizeof(message_t));

#endif

	switch (audio_device)
	{

		case AC494_CODEC_HANDSET:
			 if (get_hook_state(0)==ON_HOOK)
			 {
				send_to_phone_input_cb(0,PHONE_INPUT_LINE_1);
				send_to_phone_input_cb(1,PHONE_INPUT_LINE_1);
			 }

			audio_device=device;
			break;
		case AC494_CODEC_SPEAKER:
		case AC494_CODEC_HEADSET:
			if (device==audio_device)
			{
				if (get_hook_state(0)==ON_HOOK)
				{
					send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
				}
				audio_device=AC494_CODEC_HANDSET;
			}
			else
			{
				audio_device=device;
			}
			break;
		default:
			printf("[%s,%d] unrecognized codec type %d ",__FUNCTION__,__LINE__,audio_device) ;
			return 0;
	}

#ifdef NANOX
    if(AC494_CODEC_SPEAKER == audio_device)
    {
        message2Send.id = VOIP_DISPLAY_SPK_ON;
        nanox_messageQ_send(&message2Send);
    }
    else
    {
        message2Send.id = VOIP_DISPLAY_SPK_OFF;
        nanox_messageQ_send(&message2Send);
    }
#endif
	return acIPPSetAudioDevice(audio_device);

}


static int speaker_handle(int pressed)
{
	if (!pressed)
	{
		return audio_device_handle(AC494_CODEC_SPEAKER);
	}
	else
	{
		return 0;
	}
}

static int headset_handle(int pressed)
{
	if (!pressed)
	{
		return audio_device_handle(AC494_CODEC_HEADSET);
	}
	else
	{
		return 0;
	}

}




static keysToPhoneEvents keysToEventsArr[] =
{
	{KEYPAD_KEY_INVALID,ILLEGAL_PHONE_INPUT,NULL},
	{KEYPAD_KEY_DIGIT_0,PHONE_INPUT_0,NULL},
	{KEYPAD_KEY_DIGIT_1,PHONE_INPUT_1,NULL},
	{KEYPAD_KEY_DIGIT_2,PHONE_INPUT_2,NULL},
	{KEYPAD_KEY_DIGIT_3,PHONE_INPUT_3,NULL},
	{KEYPAD_KEY_DIGIT_4,PHONE_INPUT_4,NULL},
	{KEYPAD_KEY_DIGIT_5,PHONE_INPUT_5,NULL},
	{KEYPAD_KEY_DIGIT_6,PHONE_INPUT_6,NULL},
	{KEYPAD_KEY_DIGIT_7,PHONE_INPUT_7,NULL},
	{KEYPAD_KEY_DIGIT_8,PHONE_INPUT_8,NULL},
	{KEYPAD_KEY_DIGIT_9,PHONE_INPUT_9,NULL},
	{KEYPAD_KEY_ASTERIKS,PHONE_INPUT_ASTERISK,NULL},
	{KEYPAD_KEY_POUND,PHONE_INPUT_POUND,NULL},
	{KEYPAD_KEY_TRANSFER,PHONE_INPUT_TRANSFER,NULL},
	{KEYPAD_KEY_CONFERENCE,PHONE_INPUT_CONF,NULL},
	{KEYPAD_KEY_HOLD,PHONE_INPUT_HOLD,NULL},
	{KEYPAD_KEY_ON_HOOK,HANDLE_EVENT,&on_hook_handle},
	{KEYPAD_KEY_OFF_HOOK,PHONE_INPUT_HOOK_OFF,&off_hook_handle},
	{KEYPAD_KEY_LINE_1,PHONE_INPUT_LINE_1,&line_handle},
	{KEYPAD_KEY_LINE_2,PHONE_INPUT_LINE_2,&line_handle},
	{KEYPAD_KEY_VOLUME_UP,HANDLE_EVENT,&volume_up_handle},
	{KEYPAD_KEY_VOLUME_DOWN,HANDLE_EVENT,&volume_down_handle},
	{KEYPAD_KEY_MUTE,HANDLE_EVENT,&mute_handle},
	{KEYPAD_KEY_SPEAKER,PHONE_INPUT_SPEAKER,&speaker_handle},
	{KEYPAD_KEY_HEADSET,PHONE_INPUT_HEADSET,&headset_handle},
    {KEYPAD_KEY_PAGING_1,PHONE_INPUT_PAGING_1,NULL},//NULL here instead handle
    {KEYPAD_KEY_PAGING_2,PHONE_INPUT_PAGING_2,NULL}, //NULL  //paging event handle commneted
    {KEYPAD_KEY_PAGING_SKIP,PHONE_INPUT_PAGING_IGNORE,NULL},
    {KEYPAD_KEY_PAGING_SWITCH,PHONE_INPUT_PAGING_SWITCH,NULL}
};


/*int keypad_report(keypad_keys key ,int  pressed)
{

	int i=0,ret=0;

	while (i<NUM_OF_KEYPAD_KEYS && keysToEventsArr[i].key!=key)
		i++;

	if (i>NUM_OF_KEYPAD_KEYS)
	{
		printf("Error - unrecognized key\n ");
		return -1;
	}
	else
	{
		if (keysToEventsArr[i].event!=HANDLE_EVENT)
		{
			ret=send_to_phone_input_cb(pressed,keysToEventsArr[i].event);
		}
		if (keysToEventsArr[i].handle!=NULL)
			ret = keysToEventsArr[i].handle(pressed);

		return ret;
	}
}*/






