/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_line_connection.c                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifdef CONFIG_RG_VOIP_RV_SIP
#include "rvsdpenums.h"
#include "acl_rv_sip_api.h"
#endif
#ifdef CONFIG_RG_VOIP_RV_MGCP
#include <rvsdpsymb.h>
#include "acl_rv_mgcp_api.h"
#endif
#include "acl_dsp_telephony_events.h"
#include "acl_ui_commands.h"
#include "acl_dsp_telephony_commands.h"
//#include "dsp.h"
#include "acl_line_connection.h"

#include "voip_main.h"

//#include "ipphone.h"
#include "acl_voip_conf.h"
#include "voip_lcd.h"
#include "led.h"//ER0106 - Indicate ringing by slow LED flashing

/*#include <../mgt/lib/mgt_voip.h>*///Nirp 060709

#include "voip_utils.h"
#include "voip_rtp_utils.h"
#include <arpa/inet.h>

#include "acl_LCD_adaptation_layer.h"

static int voip_stop_call(acl_line_t *line, int callId);
static int voip_start_call(acl_line_t *line, int callId);
static int line_connect_cb(acl_line_t *line, acl_call_t *call);
static int line_hold_cb(acl_line_t *line, acl_call_t *call);
static int line_unhold_cb(acl_line_t *line, acl_call_t *call);
static int line_disconnect_cb(acl_line_t *line, acl_call_t *call);
static void offhook_event_handle(acl_line_t *line);
static void onhook_event_handle(acl_line_t *line);
static void handle_dsp_vmwi(void *data);
static void voip_get_number_of_channels_per_line(int* pNumOfChPerLine);

/*#define  AC_LINE_CONNECTION_DEBUG*/

#ifdef CONFIG_RG_VOIP_RV_SIP
#include <ipplog.h>
/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DBG_PRINT_ERR(a1,a2...)	printf("[%s:%4d] ERROR: " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */


#ifdef AC_LINE_CONNECTION_DEBUG
#ifdef CONFIG_RG_VOIP_RV_SIP
/*071016 - EA did this change during work on VI#57767 in order to syncronize our prntings with RV*/
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DBG_PRINT(a1,a2...)	printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */
#else /* AC_LINE_CONNECTION_DEBUG */
#define DBG_PRINT(a1,a2...)
#endif /* AC_LINE_CONNECTION_DEBUG */

extern rtp_session_t *rtp_sessions;	/* ER08 - ORG */
extern int SIPLogLevel; // ER07
static int g_silenceCompression; /* ACL NB 0611105 */
static char g_isSDPIncludePTime; /* ACL NB 070719 */
static codec_t *codecs;

static int giNumOfFXOL;
static int giNumOfFXSL;

int isdigit(int CharIn);
int isprint(int c);

/* ACL NB 061228 - make is static global */

#ifdef CONFIG_RG_ATA
lcd_caller_id_data_t *caller_id;

static void cid_validate(char *name, int len)
{
    while (len--)
    {
	if (!isprint(name[len]))
	    name[len] = '?';
    }
}

static void cnum_validate(char *number)
{
    char *save = number;

    while(*number && isdigit(*number))
	number++;
    /* if the string contains a non-digit character, empty it */
    if (*number)
	*save = 0;
}
#endif

void lcd_set_caller_id(int line, char *number, char *name, char *address, int distRingType)
{
#ifdef CONFIG_RG_ATA

    AC_STRNCPY(caller_id[line].name, name, MAX_CALLER_ID_SIZE - 1);
    cid_validate(caller_id[line].name, strlen(caller_id[line].name));
    AC_STRNCPY(caller_id[line].addr, address, AC_MAX_HOST_NAME_LEN);
    AC_STRNCPY(caller_id[line].number, number, MAX_CALLER_ID_SIZE -1);
    cnum_validate(caller_id[line].number);
    if (distRingType != SLIC_RING_CADENCE_PROFILE_NA)
      caller_id[line].distRingType = distRingType;
#endif
}

static int dialstring_to_addr_map_cb(void *ep_context, char *dial_string,
    char caller_id[], char addr[])
{
    char *user_id, *user_addr;

    voip_conf_map_number((char *)dial_string, &user_id, &user_addr);

    /* 060530 Avner phase 1 indicate the dialed number on the display */
    lcd_set_caller_id(((acl_line_t *)ep_context)->num, dial_string, user_id,
	user_addr, SLIC_RING_CADENCE_PROFILE_1);

    if (!strcmp(user_id, "") && !strcmp(user_addr, ""))
    {
    	free(user_id);
    	free(user_addr);

	return -1;
    }
    AC_STRNCPY(caller_id, user_id, MAX_CALLER_ID_SIZE - 1);
    AC_STRNCPY(addr, user_addr, AC_MAX_HOST_NAME_LEN);

    free(user_id);
    free(user_addr);

    return 0;
}

static void offhook_signal_send(acl_line_t *line)
{
    call_mgt->process_event(line->num, VOIP_INPUT_OFFHOOK, 1);
}

static void onhook_signal_send(acl_line_t *line)
{
   if ( line->keys_sequence == KEY_SEQ_FLASH_ONLY )
   {
      DBG_PRINT("-- \r");
      /* If secondary call is outgoing onhook will transfer the call */
      /* but not if we are during conference call                    */
      if ( line->second_call_orig == CALL_ORIGIN_OUTGOING && !line->conference )
      {
	      DBG_PRINT("onhook_signal_send line->active_call= %p   line->active_call1= %p\r\n",line->active_call,line->active_call1);
         call_mgt->process_event(line->num, VOIP_INPUT_TRANSFER, 1 );

      }
      else
	 call_mgt->process_event(line->num, VOIP_INPUT_ONHOOK, 0);

   }
   else
      	call_mgt->process_event(line->num, VOIP_INPUT_ONHOOK, 0);
}

static voip_dial_match_t dialstring_is_complete_cb(void *ep_context,
    char *dial_string, long *timer_duration)
{
    acl_line_t *line = ep_context;

    return voip_conf_match_number(dial_string, timer_duration,
	call_mgt->get_registration_status(line->num) == SIP_REG_REGISTERED);
}

static void stutter_stop(void *data)
{
   acl_line_t *line = (acl_line_t *)data;

   if ( data  == NULL )
   {
      DBG_PRINT_ERR(":ERROR - no data \r\n");
      return;
   }
   // stop stutter
//   dsp_play(line->num, VOIP_SIGNAL_STUTTER_TONE, 0);
   // start dial tone
//   dsp_play(line->num, VOIP_SIGNAL_DIAL_TONE, 1);

   return;
}

/* AlexChen SDK upgrade 20090415 */
/*  JL 081027 this is a call back for stoping ringing (using a timer)
    on ring splash command, after one ring */
static void stop_ring_splash(void *data)
{
    int line = *(int*)data;
    DBG_PRINT("stop ring splash line(%d)\n", line);
//    dsp_ring(line, 0);
}

static int start_signal_cb(void *context, voip_signal_t s, void *data)
{
    acl_line_t *line = context;
    int           distRingType;

    DBG_PRINT("-->\n");

    if (IS_TONE_SIGNAL(s))
    {

       /* ACL NB 070102 -  Added support of sttuter tone for MWI */
       if ( s == VOIP_SIGNAL_DIAL_TONE )
       {
         if ( line->is_mwi || line->is_cfw_activated || line->is_dnd_activated)
         {
//            dsp_play(line->num, VOIP_SIGNAL_STUTTER_TONE, 1);
            //
           	AC_IPC_TIMER_SET(line->stutter_tone_dur, stutter_stop, line); /* ER08 - MP202 Infrastructure */

         }
         else
            printf("");
//            dsp_play(line->num, s, 1);

       }
	   /* AlexChen SDK upgrade 20090407 */
       else if (s == VOIP_SIGNAL_REORDER_TONE )
       {
           if(  !line->is_dnd_on )
            printf("");
//                dsp_play(line->num, s, 1);

           line->is_dnd_on = 0;
       }
       /* ACL NB 070809 -  Added support of distinctive call waitiing tone  */
       else if (s == VOIP_SIGNAL_CALLER_WAITING_TONE )
       {
          voip_siginfo_dist_ring_t  *distRing_data;
          distRing_data = (voip_siginfo_dist_ring_t *)data;
          if (distRing_data)
          {
/*             DBG_PRINT_ERR("(VOIP_SIGNAL_CALLER_WAITING_TONE) Distinctive ringing %d\r\n", distRing_data->distRing);*/
             distRingType= distRing_data->distRing;
             // TBD if call waiting distinctive tone enable (call waiting priority alerting enabled)
             if (distRingType != SLIC_RING_CADENCE_PROFILE_1)
                s = VOIP_SIGNAL_CALLER_WAITING_TONE_2;
          }
          else
          {
/*             DBG_PRINT_ERR("(VOIP_SIGNAL_CALLER_WAITING_TONE)  NO Distinctive ringing data\r\n");          */
             distRingType = SLIC_RING_CADENCE_PROFILE_1;
          }
          // For now it is debug only ant not implemented yet!!!!! so no use for distRing_data or distRing_data->distRing
//          dsp_play(line->num, s, 1);

       }
       else
        printf("");
//         dsp_play(line->num, s, 1);

    }
    else if (s == VOIP_SIGNAL_RING || s == VOIP_SIGNAL_RING_SPLASH)
    {
      /* ACL NB 070506 - Distinctive ringing */
      voip_siginfo_dist_ring_t  *distRing_data;
      distRing_data = (voip_siginfo_dist_ring_t *)data;
      if (distRing_data)
      {
         //DBG_PRINT_ERR("(VOIP_SIGNAL_RING) Distinctive ringing %d\r\n", distRing_data->distRing);
         distRingType= distRing_data->distRing;
      }
      else
      {
         //DBG_PRINT_ERR("(VOIP_SIGNAL_RING)  NO Distinctive ringing data\r\n");
         distRingType = SLIC_RING_CADENCE_PROFILE_1;
      }

	if (line->is_ringing)
	    return 0;

	if (vcCONFIG_RG_ATA)
	    lcd_set_caller_id(line->num, "", "", "", distRingType);

//	dsp_ring(line->num, 1);

	//ER0106 - Indicate ringing by slow LED flashing
	if(VOIP_SIGNAL_RING == s)
	{
        if(AC_IPC_TIMER_EXISTS(stop_ring_splash, &line->num))
        {/* JL 081027 if the ring splash timer was active while ring signal has arrived,
            delete ring splash timer. change cadence on the fly */
            DBG_PRINT("delete stop_ring_splash timer line(%d)\n", line->num);
            AC_IPC_TIMER_DEL(stop_ring_splash, &line->num);
        }
        line->is_ringing = 1;
		set_phone_led_to_state(line->num, e_GREEN_LED_STATE_SLOW_FLASH);

    }
    if(VOIP_SIGNAL_RING_SPLASH == s)
    {/* ring splash should ring only once for 500ms. */
        DBG_PRINT("set timer to stop ring splash in 4000 ms line(%d)\n", line->num);
     /* this timer stop ringing just before the start of the second ring */
        AC_IPC_TIMER_SET(4000, stop_ring_splash, &line->num);
    }
#if 0 // AlexChen SDK upgrade 20090415
	set_phone_led_to_state(line->num, e_GREEN_LED_STATE_SLOW_FLASH);

	line->is_ringing = 1;
#endif
    }
    /* ACL NB 060720 support DTMF vis SIP */
    else if (s == VOIP_SIGNAL_DTMF_TONE)
    {
        voip_siginfo_gd_t  *gd_data;
        gd_data = (voip_siginfo_gd_t *)data;
        dsp_dtmf(line->num, gd_data->id ,1, gd_data->duration );
    }
    DBG_PRINT("<--\n");

    return 0;
}

static int play_signal_cb(void *context, voip_signal_t s, void *data)
{
#define SIGDATA_IND ((voip_siginfo_ind_t *)data)
#define SIGDATA_CID ((voip_siginfo_cid_t *)data)
#define SIGDATA_MSG ((voip_siginfo_msg_t *)data) /* ACL NB 061125 */

    //DBG_PRINT("--> \r\n");

    DBG_PRINT("s=%d\n",s);

    switch (s)
    {
    case VOIP_SIGNAL_RING:
	start_signal_cb(context, s, data);
	break;

    case VOIP_SIGNAL_INDICATOR:
    DBG_PRINT("-- \r\n");
   /* ACL NB 070506 - Distinctive ringing */
   /* for now debug printing only */
   if (SIGDATA_IND)
   {
      //DBG_PRINT_ERR("(VOIP_SIGNAL_INDICATOR) Distinctive ringing %d\r\n", SIGDATA_IND->distRing);
   }
   else
   {
      //DBG_PRINT_ERR("(VOIP_SIGNAL_INDICATOR) NO Distinctive ringing data\r\n");
   }
	phone_output_ind(SIGDATA_IND->indid, SIGDATA_IND->state_on);
	break;

    case VOIP_SIGNAL_CALLER_ID:
	/* IPPhone shows caller id in DISPLAY_INCOMING signal */
	if (vcCONFIG_RG_ATA)
	{
	    acl_line_t *line = context;
   	    //acl_call_t *call = line->active_call;

	    lcd_set_caller_id(line->num, SIGDATA_CID->number,
		SIGDATA_CID->name, SIGDATA_CID->address, SLIC_RING_CADENCE_PROFILE_NA );

		// ER06 - Support type-II (off-hook) caller-ID
		if(line->is_off_hook)
		{

			/*AlexChen 20090408 SDK upgrade caller id type 2 */
 			char isType2Enabled=0;
            voip_conf_call_id_type2_enable_get( &isType2Enabled );

			if (isType2Enabled)
				dsp_call_waiting_alert(line->num);
			else
			printf("");
//				dsp_play(line->num, VOIP_SIGNAL_CALLER_WAITING_TONE, 1);

#if 0 		//AlexChen 20090408
		        /*NirM, 071008 - Needed for Improved Application, We are in FAX mode - FSK will not be played locally to improve FAX stability*/
			if(!call->isT38 && !call->isVBD)
				dsp_call_waiting_alert(line->num);
#endif
		}
		else
		{
		    /* restart ring with caller id */
//		    dsp_ring(line->num, 0);
//		    dsp_ring(line->num, 1); // TBD
		//    line->is_ringing = 1;
		}

	}
	break;

   /* ACL NB 061008 VMWI*/
   case VOIP_SIGNAL_MWI:
   {
      acl_line_t *line = context;
      //DBG_PRINT("-- VOIP_SIGNAL_MWI SIGDATA_IND->state= %d\r\n",SIGDATA_MSG->state_on );
      if (line)
      {
         /* ACL NB 070102 - for stutter tone*/
         line->is_mwi = SIGDATA_MSG->state_on;
		 handle_dsp_vmwi(line);
      }
      else
      {
         DBG_PRINT_ERR("VOIP_SIGNAL_MWI failure no context- \r\n");
      }
	}
	break;

    default:
	break;
    }
    return 0;
}

/******************************************************************
function name:	handle_dsp_vmwi
parameters:		data - handle to phone connection data structure
description:	execute fsk to message watining indication event
				only when the user is not talking (using polling)
return value:	none
*******************************************************************/
static void handle_dsp_vmwi(void *data)
{
	acl_line_t *line = (acl_line_t *)data;

	if(line->active_call||line->active_call1)
	{//the user is in a middle of active call
		DBG_PRINT("setting timer on handle_dsp_vmwi for 10 SEC\r\n");
		AC_IPC_TIMER_SET(10000, handle_dsp_vmwi, data); /* ER08 - MP202 Infra */
	}
	else
	{//execute fsk only when the user has finished talking
		DBG_PRINT("execute fsk\r\n");
		dsp_vmwi(line->num, line->is_mwi);
	}

}

static int stop_signal_cb(void *context, voip_signal_t s, void *data)
{
    acl_line_t *line = context;


    //DBG_PRINT("-->\r\n");

    if (IS_TONE_SIGNAL(s))
    {
      // Added support of stutter tone
      if ( s != VOIP_SIGNAL_DIAL_TONE )
        printf("");
//         dsp_play(line->num, s, 0);
      else
      {
         if (AC_IPC_TIMER_EXISTS(stutter_stop, line)) /* ER08 - MP202 Infra */
         {
//            dsp_play(line->num, VOIP_SIGNAL_STUTTER_TONE , 0);
            AC_IPC_TIMER_DEL(stutter_stop, line);	/* ER08 - MP202 Infra */
         }
         //else
//         dsp_play(line->num, s, 0);
      }
	  /* AlexChen SDK upgrade 20090407 */
	  line->is_dnd_on = 0;
    }
    else if (s == VOIP_SIGNAL_RING)
    {
		/* AlexChen SDK upgrade 20090415 */
		if(AC_IPC_TIMER_EXISTS(stop_ring_splash, &line->num))
        {/* JL 081027 if the ring splash timer was active while ring signal has arrived,
            delete ring splash timer. change cadence on the fly */
            DBG_PRINT("delete stop_ring_splash timer line(%d)\n", line->num);
            AC_IPC_TIMER_DEL(stop_ring_splash, &line->num);
        }

		line->is_ringing = 0;
//		dsp_ring(line->num, 0);

		//ER0106 - Stop ringing LED indication
		if(line->is_off_hook)
			set_phone_led_to_state(line->num, e_GREEN_LED_STATE_ON);
		else
			onhook_event_handle(line);
    }

    return 0;
}

int is_state_cb(void *conn_context, phone_input_event_t state)
{
    acl_line_t *line = conn_context;

    return (!line->is_off_hook && state==VOIP_INPUT_ONHOOK) ||
	   (line->is_off_hook && state==VOIP_INPUT_OFFHOOK);
}

#ifdef AC488_CPE_VOIP_TOOLKIT
int is_other_line_in_conference_cb(void *conn_context)
{
	acl_line_t *line = conn_context;

	acl_line_t *otherLine = get_line(1 - line->num );

	if(!otherLine)
	{

	}

	return (otherLine->conference);
}
#endif /*AC488_CPE_VOIP_TOOLKIT*/


/* XXX need to understand how to distinguish between G.723 bit rates */
int is_723_low(void)
{
    return 1;
}

static void enum_codecs_cb(enum_codec_cb static_codec_cb, enum_codec_cb
    dyn_codec_cb, void *data)
{
    int i;
    char *codec_name;


    for (i = 0; i<NUMBER_OF_CODECS; i++)
    {
	if (!codecs[i].enabled)
	    continue;
    codec_name = AC_RTP_PT_TO_NAME(codecs[i].payload); /* ER08 - MP202 Infra */
	if (codecs[i].payload < FIRST_DYNAMIC_PAYLOAD)
	    static_codec_cb(codec_name, &codecs[i], data);
	else
	    dyn_codec_cb(codec_name, &codecs[i], data);

    }
}

/* AlexChen SDK upgrade 20090415 */
/*ACL AL 081022 VI# 67399: create a new function for adding annex since the same code is used in more than one place now*/
static void add_sdp_media_special_attr(acl_voip_sdp_media_descr_t *pMedia, rtp_payload_type_t payload)
{
    char fmtp_str[32];

/*================================================================================*/
/* ACL NB 061105 - Added support on annexb(g729) and anneca (g723) - see RFC 3555 */
/*================================================================================*/
   if (payload == RTP_PAYLOAD_G729)
   {
      //console_printf("handle annexb for 729\r\n");
      if (g_silenceCompression)
         sprintf(fmtp_str, "%d annexb=yes", payload);
      else
         sprintf(fmtp_str, "%d annexb=no", payload);;
      acl_voip_sdp_media_descr_attr_add(pMedia, "fmtp", fmtp_str);
   }
   else if (payload == RTP_PAYLOAD_G723)
   {
      //console_printf("handle annexa for 723\r\n");
      if (g_silenceCompression)
         sprintf(fmtp_str, "%d annexa=yes", payload);
      else
         sprintf(fmtp_str, "%d annexa=no", payload);
      acl_voip_sdp_media_descr_attr_add(pMedia, "fmtp", fmtp_str);
   }

}

static int codec_parameters_set(acl_voip_sdp_msg_t *local, int *common_codecs, int active_codec)
{
    acl_voip_sdp_media_descr_t *media;
    int i, j, use_dtmf = 0, ret_val = -1;
    char fmtp_str[32];
	unsigned char already_exist;

    media = acl_voip_sdp_msg_media_descr_get(local, 0);

    for (i = 0; i<NUMBER_OF_CODECS; i++)
    {
	char ptime[LONG_NUM_LEN];

	if (!codecs[i].enabled)
	    continue;

	/* AlexChen SDK upgrade 20090415 */
	/*ACL AL 081022 VI# 67399:SwitchToVoice Re-invite include 3 lines of the same payload*/
	/*the active_codec was already added in set_local_sdp_message_with_currentCodec*/
	if (active_codec == codecs[i].payload )
	{
		continue;
	}

	/* we want to give back an arbitrary enabled codec */
	if (ret_val < 0)
	    ret_val = codecs[i].payload;
	if (common_codecs)
	{
	    int j, is_common = 0;

	    for (j=0; j<NUMBER_OF_CODECS; j++)
	    {
        //DBG_PRINT(" common_codecs[%d]=%d, codecs[%d].payload=%d .\r\n", j, common_codecs[j], i, codecs[i].payload);
		if (common_codecs[j] == codecs[i].payload)
		{
		    is_common = 1;
		    break;
		}
	    }
	    if (!is_common)
		continue;
	}


	/* AlexChen SDK upgrade 20090415 */
	/*ACL AL 081022 VI# 67399:SwitchToVoice Re-invite include 3 lines of the same payload*/
	/* prevent adding the same codec twice or more*/
	for ( j=0, already_exist=0; j<i ; j++)
	{
		if (( codecs[i].payload == codecs[j].payload) &&  codecs[j].enabled )
		{
			already_exist = 1;
			break;
		}
	}
	if (already_exist)
		continue;


	if (codecs[i].payload == AC_PAYLOAD_TYPE_ENUM_TO_ID(RTP_PAYLOAD_DTMF))
	    use_dtmf = 1;

    //DBG_PRINT("codecs[%d].payload=%d .\r\n", i, codecs[i].payload);
	acl_voip_sdp_media_descr_payload_add(media, codecs[i].payload);
#if 0 // AlexChen SDK upgrade 20090415
/*================================================================================*/
/* ACL NB 061105 - Added support on annexb(g729) and anneca (g723) - see RFC 3555 */
/*================================================================================*/
      if (codecs[i].payload == RTP_PAYLOAD_G729)
      {
         //DBG_PRINT_ERR("handle annexb for 729\r\n");
         if (g_silenceCompression)
	         sprintf(fmtp_str, "%d annexb=yes", codecs[i].payload);
         else
            sprintf(fmtp_str, "%d annexb=no", codecs[i].payload);;
	      acl_voip_sdp_media_descr_attr_add(media, "fmtp", fmtp_str);
      }
	   else if (codecs[i].payload == RTP_PAYLOAD_G723)
      {
         //DBG_PRINT_ERR("handle annexa for 723\r\n");
         if (g_silenceCompression)
	         sprintf(fmtp_str, "%d annexa=yes", codecs[i].payload);
         else
	         sprintf(fmtp_str, "%d annexa=no", codecs[i].payload);
	      acl_voip_sdp_media_descr_attr_add(media, "fmtp", fmtp_str);
      }
    //DBG_PRINT("-> \r\n");
#endif
	acl_voip_sdp_media_descr_rtp_map_add(media, codecs[i].payload,
	    AC_RTP_PT_TO_NAME(codecs[i].payload),
	    codecs[i].rate);

	/* AlexChen SDK upgrade 20090415 */
	/*ACL AL 081022 VI# 67399:SwitchToVoice Re-invite include 3 lines of the same payload*/
	add_sdp_media_special_attr(media, codecs[i].payload);

      /* add ptime to the SDP only if isSDPIncludePTime is on-  */
      DBG_PRINT("isSDPIncludePTime=%d\r\n", g_isSDPIncludePTime);
      /* EA - 071226 - VI# 59572 New feature request: need to remove the ptime line of the DTMF (101).
         add the check if its not dtmf in order to remove ptime of DTMF. use_dtmf will be true only
         for RTP_PAYLOAD_DTMF
      */
      if ((g_isSDPIncludePTime) && (! use_dtmf))
      {
         sprintf(ptime, "%d", codecs[i].ptime);
         /*  add ptime attribute(s) */
         acl_voip_sdp_media_descr_attr_add(media, "ptime", ptime);//NIRP 070307 Add ptime to SDP
      }
    }

    if (use_dtmf)
    {
	/* The fmtp attribute describes the DTMF events we support */
	/* Add key events 0 to 15 (mandatory) */
	sprintf(fmtp_str, "%d 0-15", AC_PAYLOAD_TYPE_ENUM_TO_ID(RTP_PAYLOAD_DTMF));
	acl_voip_sdp_media_descr_attr_add(media, "fmtp", fmtp_str);
    }


    return ret_val;
}

static int local_sdp_message_set(acl_voip_sdp_msg_t *local, u16 rtpport,
    int common_codecs[])
{

    /* Set SDP message properties */
    acl_voip_sdp_msg_media_descr_clear(local);
    acl_voip_sdp_msg_media_descr_add(local, rtpport);
    return codec_parameters_set(local, common_codecs, -1);
}

/* ACL NB 060810 Patch for invite with no SDP during a call !!! */
static int local_sdp_message_with_current_codec_set(acl_voip_sdp_msg_t *local, u16 rtpport, int active_codec)
{
    int i;
    acl_voip_sdp_media_descr_t *media;

    int ret_val = -1;

    DBG_PRINT("-> \r\n");

    media = acl_voip_sdp_msg_media_descr_get(local, 0);

    /* Set SDP message properties */
    acl_voip_sdp_msg_media_descr_clear(local);
    acl_voip_sdp_msg_media_descr_add(local, rtpport);

    /* look for current codec and DTMF codec - if exist */
    for (i = 0; i<NUMBER_OF_CODECS; i++)
    {
       /* AlexChen SDK upgrade 20090415 */
		char ptime[LONG_NUM_LEN];
		DBG_PRINT("codecs[i].enabled = %d, codecs[i].payload = %d, active_codec = %d", codecs[i].enabled, codecs[i].payload, active_codec);
		/*ACL AL 081022 VI# 67399:SwitchToVoice Re-invite include 3 lines of the same payload*/
		if ((codecs[i].enabled) && (codecs[i].payload == active_codec))
		{
			DBG_PRINT("-> \r\n");
			//DBG_PRINT("active_codec = %d  \r\n", active_codec  );
			acl_voip_sdp_media_descr_payload_add(media, codecs[i].payload);

			acl_voip_sdp_media_descr_rtp_map_add(media, codecs[i].payload,
			code2str_ex(rtp_payload_type_t_str, codecs[i].payload),  codecs[i].rate);
			/* ACL NB 081019 */
			add_sdp_media_special_attr(media, codecs[i].payload);

			/*ACL AL 081116 VI# 68122  - The MP202 receives 415 (unsupported media) upon SwitchToVoip Re-Invite.
			Solve it by add the missing ptime to the current codec*/
			DBG_PRINT("isSDPIncludePTime=%d\r\n", g_isSDPIncludePTime);
			if ((g_isSDPIncludePTime) /*&& (!use_dtmf)*/)
			{
				sprintf(ptime, "%d", codecs[i].ptime);
				/*  add ptime attribute(s) */
				acl_voip_sdp_media_descr_attr_add(media, "ptime", ptime);//NIRP 070307 Add ptime to SDP
			}


			ret_val = active_codec;
			/*ACL AL 081022 VI# 67399: once we find the active_codec, we can break*/
			break;
		}
    }

    /* Added all other capbilities - the current codec might be added twice */
    codec_parameters_set(local, NULL, active_codec);

    DBG_PRINT("<- \r\n");
    return ret_val;
}


void dtmf_stop(void *data)
{
#ifdef CONFIG_RG_ATA
    acl_call_t *call = (acl_call_t *)data;

    dsp_dtmf(call->phone->num, 0, 0, 0);/* ACL NB 060720 - added duration */
    call->dtmf_select = NO_DTMF;
#endif
}

static void *line_connection_prepare_cb(acl_voip_sdp_msg_t *local, int actual)
{
    acl_call_t *call = NULL;
    char local_ip[AC_DOTTED_IP_LEN];
    char cname[AC_MAX_DOMAIN_NAME_LEN];
   acl_voip_sdp_media_descr_t *media; /* ACL NB 070603 support RFC 3264 Hold VI53372*/

    DBG_PRINT("-> \r\n");
    if (!actual)
    {
	local_sdp_message_set(local, ACL_RTPPORT_ANY, NULL);
	goto Error;
    }

    if (!(call = AC_ZALLOC_L(sizeof(acl_call_t))))
	goto Error;

	/* AlexChen SDK upgrade 20090415 */
	/*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated) - in case of remote conferece
    with parties that support nested re-invite == do not use the workaround - these buffers were never freed - verify that these pointer
    will get thier values only from the workaround and avoid having them hold some garbage value...*/
    /*clear refer info structure*/
#ifdef CONFIG_RG_VOIP_RV_SIP
    call->refer_info.didReferFail = 0;
    call->refer_info.unholdInProgress = 0;
    call->refer_info.referredByHdrBuf = NULL;
    call->refer_info.referToHdrBuf = NULL;
#endif /* CONFIG_RG_VOIP_RV_SIP */

   	/*ACL XY 080917 - VI66797: problems with T.38 fax transmissions*/
   	call->is_fax_relay_detected_wait_for_ok = 0;

   	/*ACL XY 081230 Fax Fallback*/
   	call->is_fax_relay_detected = 0;
    call->isFaxFallback = 0;

    voip_conf_local_address_get(local_ip);
    snprintf(cname, AC_MAX_DOMAIN_NAME_LEN, "%p@%s", call, local_ip);
    call->rtp = rtp_get(cname, codecs, call);
    if (!call->rtp)
	goto Error;

    AC_EVENT_HANDLER_SET(call->rtp->fd, EFD_READ, rtp_rcv, call);
    /* use one of the enabled codecs as default codec */
    call->active_codec = local_sdp_message_set(local, call->rtp->port, NULL);

    //DBG_PRINT("<- port %d \r\n", call->rtp->port);

   /* ACL NB 070603 - Added sendrecv attribute to support RFC 3264 Hold VI53372*/
    media = acl_voip_sdp_msg_media_descr_get(local, 0);
    acl_voip_sdp_media_descr_attr_add(media, "sendrecv", "");


    return call;

Error:
    nfree(call);

    return NULL;
}

static AC_CODE2CODE_MAP supported_codec_get(acl_voip_sdp_media_descr_t *media,
    int common_codecs[], int *common_idx, int rtp_fd, int *pDtmf_method)
{
    int i, j, pl, local_pl;

    AC_CODE2CODE_MAP payload_type = { -1, -1 };
    int voice_payload_found = 0;

    if (*pDtmf_method == DTMF_RFC2833 )
		*pDtmf_method = DTMF_INBAND;

    /* Go over all payloads in the media descriptor and see which ones are
     * supported */
    for (i=0; i<acl_voip_sdp_media_descr_payload_num_get(media); i++)
    {
	pl = acl_voip_sdp_media_descr_payload_get(media, i);
     printf("pl=%d  %s %d \r\n", pl,__FUNCTION__,__LINE__);
	/* Take only first compatible codec */
	if (!voice_payload_found && pl < FIRST_DYNAMIC_PAYLOAD && codec_get_by_payload(pl))
	{
	    voice_payload_found = 1;

	    common_codecs[(*common_idx)++] = pl;

	    if (payload_type.val==-1)
		payload_type.val = payload_type.code = pl;
	}
	/* dynamic codec - must look its name in the RTP map to see if we
	 * support it */
	else if (pl >= FIRST_DYNAMIC_PAYLOAD)
	{
	    for (j=0; j<acl_voip_sdp_media_descr_rtp_map_num_get(media); ++j)
	    {
		acl_voip_sdp_rtp_map_t *rtp_map =
		    acl_voip_sdp_media_descr_rtp_map_get(media, j);
		int element = acl_voip_sdp_rtp_map_payload_get(rtp_map);

		if (element != pl)
		    continue;
		/* on dynamic payload, we need to use the actual name for
		 * identification, and verify it is enabled: some payload
		 * types are not real voice codecs - those we will not allow to
		 * be active codecs */
        if (((local_pl = AC_RTP_NAME_TO_PT(acl_voip_sdp_rtp_map_encoding_name_get(rtp_map))) == -1) ||
		    !codec_get_by_payload(local_pl))
		{
		    continue;
		}
        //DBG_PRINT("pl=%d, local_pl=%d .\r\n", pl, local_pl);
        /* ACL NB 0600724 - changed dtmf_inband handling */
		if ( local_pl == AC_PAYLOAD_TYPE_ENUM_TO_ID(RTP_PAYLOAD_DTMF))
		{
		    	common_codecs[(*common_idx)++] = local_pl;
		   	map_dynamic_payload(rtp_fd, local_pl, pl);
          	   	*pDtmf_method = DTMF_RFC2833;
		}
		else if (!voice_payload_found)
		{
            //DBG_PRINT("----.\r\n");
		    common_codecs[(*common_idx)++] = local_pl;
		    voice_payload_found = 1;
		    payload_type.code = pl;
		    payload_type.val = local_pl;
		    map_dynamic_payload(rtp_fd, local_pl, pl);//Nirp 060625 voice payload types should also be mapped
		}
	    }
	}
    }
    return payload_type;
}

static int remote_params_set(acl_call_t *call, acl_voip_sdp_msg_t *msg,
    int common_codecs[] /*size == NUMBER_OF_CODECS*/, int rtp_fd)
{
    int j, media_desc_num;
    int isT38= 0; /* ACL NB 060409 PATCH!! */
	char faxEnabled = 0;	/*AlexChen SDK upgrade 20090409 */
    int faxpayload = -1;	/*AlexChen SDK upgrade 20090409 */

    /* On every SDP messages there are media descriptors that contain
     * the actual media parameters. For audio only, they are usually
     * only one descriptor */
    media_desc_num = acl_voip_sdp_msg_media_descr_num_get(msg);
    //DBG_PRINT(" > media_desc_num=%d .\r\n", media_desc_num);
    //DBG_PRINT(" ================================.\r\n");
    for (j = 0; j < media_desc_num; j++)
    {
	int k, attr_num, i=0 /*consider only payloads of the relevant media */;
	acl_voip_sdp_media_descr_t *media = acl_voip_sdp_msg_media_descr_get(msg, j);
	acl_voip_sdp_conn_t *connection;
    int port = 0; /* ACL NB 070405 VI51785 */

	AC_CODE2CODE_MAP supported_codec = supported_codec_get(media, common_codecs,
		    &i, rtp_fd, &call->dtmf_method );

   DBG_PRINT(" supported_codec.code=%d, supported_codec.val=%d call->dtmf_method=%d\r\n", supported_codec.code, supported_codec.val,call->dtmf_method);

   /* ACL NB  060409 - PATCH!! */
    /* EA - 071120 - radvision have changed the enum value == check the enum and not the number. */
   /* AlexChen SDK upgrade 20090409 add fax audio startup */
   voip_conf_fax_audio_startup_enable_get( &faxEnabled );
   voip_conf_fax_audio_startup_payload_get( &faxpayload );
   if (acl_voip_sdp_msg_media_descr_type_get(media) == RV_SDPMEDIATYPE_IMAGE/*121*/
   	|| (faxEnabled && acl_voip_sdp_media_descr_payload_get(media,0)==faxpayload))
      isT38 = 1;
    /*
        EA - 071120 - first check if you are t.38: if you are not dont bother to check the codec
        but if are something else, do check the codec.
    */
    if (!isT38 && supported_codec.code == -1)  /* ACL NB 060409 */
	    continue; /* we don't support the codecs of this media */

	for (; i<NUMBER_OF_CODECS; i++)
	    common_codecs[i] = -1;

	/* Look for a packetization attribute */
	attr_num = acl_voip_sdp_media_descr_attr_num_get(media);
	for (k = 0; k < attr_num; k++)
	{
	    /* XXX check support for requested ptime ?
	    attr = acl_voip_sdp_media_descr_attr_get(media, k);
	    if (!strcmp(acl_voip_sdp_attr_name_get(attr), "ptime"))
	    {
		const char *pkt_time = acl_voip_sdp_attr_value_get(attr);
	    }
	    */
	}
	/* This is defined in the Connection field of the SDP Media
	 * Descriptor */
	connection = acl_voip_sdp_media_descr_conn_get(media);
	/* The connection can also be set in the SdpMsg and not in
	 * the descriptor. Check there if it was not found in the
	 * previous step */
	if (!connection)
	    connection = acl_voip_sdp_msg_conn_get(msg);
   port = acl_voip_sdp_media_descr_port_get(media); /* ACL NB 070405 VI#51785*/
   DBG_PRINT("remote IP addr(%s) and remote IP port(%d) \r\n", (char *)acl_voip_sdp_conn_addr_get(connection),  port);
	if (connection && port)                       /* ACL NB 070405 VI#51785*/
	{
	    in_addr_t addr =
		inet_addr((char *)acl_voip_sdp_conn_addr_get(connection));
	    rtp_connect(call, addr,port); /* ACL NB 070405 VI#51785*/
       /* ACL NB 060409 PATCH!!  */
       if (!isT38)
       {
	       call->active_codec = supported_codec.val;
	       call->active_codec_remote_code = supported_codec.code;
       }
       /*
       DBG_PRINT(" >>>>>>>>call->active_codec=%d, call->active_codec_remote_code=%d.\r\n", call->active_codec, call->active_codec_remote_code);
       for (i=0; i<NUMBER_OF_CODECS; i++)
            DBG_PRINT(" common_codecs[%d]=%d .\r\n", i, common_codecs[i]);
       */
	    return 0;
	}
    }
    DBG_PRINT("ERROR EXIT\r\n");
    return -1;
}
/*
 *	 ACL NB 070627 - Similar to remote_params_set, with the following different: there is no loop on
 *	 the media desription list. Instead, we are getting the media description index
 */
static int remote_params_from_media_desc_set(acl_call_t *call, acl_voip_sdp_msg_t *msg,
    int common_codecs[] /*size == NUMBER_OF_CODECS*/, int rtp_fd, int mediaDescIdx)
{
    int isT38= 0; /* ACL NB 060409 PATCH!! */
	int k, attr_num, i=0 /*consider only payloads of the relevant media */;
	acl_voip_sdp_media_descr_t *media = acl_voip_sdp_msg_media_descr_get(msg, mediaDescIdx);
	acl_voip_sdp_conn_t *connection;
    int port = 0; /* ACL NB 070405 VI51785 */

	AC_CODE2CODE_MAP supported_codec = supported_codec_get(media, common_codecs,
		    &i, rtp_fd, &call->dtmf_method);

   printf(" supported_codec.code=%d, supported_codec.val=%d call->dtmf_method=%d\r\n", supported_codec.code, supported_codec.val,call->dtmf_method);

   /* ACL NB  060409 - PATCH!! */
   if (acl_voip_sdp_msg_media_descr_type_get(media) == 121)
      isT38 = 1;

   if (supported_codec.code == -1 && !isT38 )  /* ACL NB 060409 */
	    return -1; /* we don't support the codecs of this media */

	for (; i<NUMBER_OF_CODECS; i++)
	    common_codecs[i] = -1;

	/* Look for a packetization attribute */
	attr_num = acl_voip_sdp_media_descr_attr_num_get(media);
	for (k = 0; k < attr_num; k++)
	{
	    /* XXX check support for requested ptime ?
	    attr = acl_voip_sdp_media_descr_attr_get(media, k);
	    if (!strcmp(acl_voip_sdp_attr_name_get(attr), "ptime"))
	    {
		const char *pkt_time = acl_voip_sdp_attr_value_get(attr);
	    }
	    */
	}
	/* This is defined in the Connection field of the SDP Media
	 * Descriptor */
	connection = acl_voip_sdp_media_descr_conn_get(media);
	/* The connection can also be set in the SdpMsg and not in
	 * the descriptor. Check there if it was not found in the
	 * previous step */
	if (!connection)
	    connection = acl_voip_sdp_msg_conn_get(msg);
   port = acl_voip_sdp_media_descr_port_get(media); /* ACL NB 070405 VI#51785*/
   printf("remote IP addr(%s) and port(%d) \r\n", (char *)acl_voip_sdp_conn_addr_get(connection),  port);
	if (connection && port)                       /* ACL NB 070405 VI#51785*/
	{
	    in_addr_t addr =
		inet_addr((char *)acl_voip_sdp_conn_addr_get(connection));
	    rtp_connect(call, addr,port); /* ACL NB 070405 VI#51785*/
       /* ACL NB 060409 PATCH!!  */
       if (!isT38)
       {
	       call->active_codec = supported_codec.val;
	       call->active_codec_remote_code = supported_codec.code;
       }
       /*
       DBG_PRINT(" >>>>>>>>call->active_codec=%d, call->active_codec_remote_code=%d.\r\n", call->active_codec, call->active_codec_remote_code);
       for (i=0; i<NUMBER_OF_CODECS; i++)
            DBG_PRINT(" common_codecs[%d]=%d .\r\n", i, common_codecs[i]);
       */
	    return 0;
	}

    return -1;
}



/*
 *
 */
static int line_connection_create_cb(void *context, acl_voip_sdp_msg_t *local,
    acl_voip_sdp_msg_t *remote, voip_conn_mode_t mode, int dtmf_method) /* ACL NB 060724 added dtmf_method */
{
    acl_call_t *call = context;
    int common_codecs[NUMBER_OF_CODECS];

    DBG_PRINT("-> \r\n");
    call->mode = mode;
    call->dtmf_method = dtmf_method;
    rtp_mode_set(call, 0);
    if (!remote_params_set(context, remote, common_codecs, call->rtp->fd))
    {
	local_sdp_message_set(local, call->rtp->port, common_codecs);
	return 0;
    }
    DBG_PRINT("ERROR: set remote params failed\r\n");
    return -1;
}

static int ptime_get(acl_call_t *call)
{
    return codec_get_by_payload(call->active_codec)->ptime;
}

/*======================================================================================================*/


static int line_connection_start_cb(void *ep, void *ctx)
{
    acl_line_t *line = ep;
    acl_call_t *call = ctx;
    int ret;

    DBG_PRINT("->\r\n");

    if (line == NULL )
    {
       DBG_PRINT_ERR("line_connection_start_cb: No connection \r\n");
       return -1;
    }
    if (call == NULL)
    {
       DBG_PRINT_ERR("line_connection_start_cb: No call \r\n");
       return -1;
    }

    if (!call->phone)
    {
       call->phone = line; /* at this point we associate the phone with the call */
    }
    ret = line_connect_cb(line, call);
    DBG_PRINT("<- ret(%d)\r\n", ret);
    return ret;

}


static int line_connection_stop_cb(void *ep, void *ctx)
{
    acl_line_t *line = ep;
    acl_call_t *call = ctx;

    /*  ACL NB 060731 make sure - end point and rtp session are really connected */
    if (call->phone != ep)
    	return 0;

	return line_disconnect_cb(line, call);

    return 0;
}



void line_status_print(acl_line_t *line)
{
	DBG_PRINT("line->active_call      = %p\r\n",line->active_call);
	DBG_PRINT("line->active_call1     = %p\r\n",line->active_call1);
}

//NIRP 070527
int send_daa_commands_cb(void *data)
{
	DBG_PRINT("-> Enter.....\r\n");

	if (data == NULL )
	{
		DBG_PRINT("DATA is NULL \r\n");
		return -1;
	}

	dsp_send_daa_commands(data);

	return 0;
}


static int start_3wc_cb(void *conn_context)
{
    acl_line_t *line = conn_context;

    DBG_PRINT("-> line = %p \r\n",line );

    if (line == NULL )
    {
       DBG_PRINT("No connection \r\n");
       return -1;
    }

    if (line->conference)
    {
        DBG_PRINT("Alredy in conference \r\n");
        return 0;
    }

    line->conference = 1;

	dsp_3wc(line->num, 1, 0, 1);

    return 0;
}


/* ACL 060912 set call origin  */
static int call_origin_set_cb(void *conn_context, int origin)
{
	// For now if only update secondary call origin
    acl_line_t *line = conn_context;

    DBG_PRINT(" line = %p  origin=%d\r\n",line,origin );

    if (line == NULL )
    {
       DBG_PRINT_ERR("No connection \r\n");
       return -1;
    }

    if (origin != CALL_ORIGIN_OUTGOING)
    {
      line->second_call_orig = origin;
    }
	/* update in case of secondary call mean if there is 1 call */
   /* the outgoing call is not established yet !!!                  */
	else if ( line->active_call || line->active_call1)
   {
      line->second_call_orig = origin;
      DBG_PRINT("secondary outgoing call \r\n");

   }

	return 0;
}

//NIRP 070610 VI 54097
int adaptation_layer_general_command_send_cb(void *data)
{
    //DBG_PRINT("-> Enter.....\r\n");
    if (data == NULL )
    {
       DBG_PRINT("DATA is NULL \r\n");
       return -1;
    }
	dsp_adaptation_layer_send_general_command(data);
    return 0;
}




/* ACL NB 070411 */
static int cfw_activate_cb(void *conn_context, int on)
{
   // For now if only update secondary call origin
   acl_line_t *line = conn_context;

   DBG_PRINT("-> line = %p  on=%d\r\n",line,on );

   if (line == NULL )
   {
      DBG_PRINT_ERR("No connection \r\n");
      return -1;
   }

   line->is_cfw_activated = on;
   DBG_PRINT("<- on=%d\r\n",line->is_cfw_activated );

   return 0;

}

/****************************************************************************
*
*	Name:		voip_stop_call
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	close the channel and stop the RTP of the callId.
*				Each connection (line) can haves up to two calls, one is the active and the second is the active1
*				The active is always connected to the handset and is assigned with channel=0 and callID=0
*				The active1 is use for holding an hold call and is assigned with channel=1 and callId=1
*				(the channel ID is the line channel and not the DSP channel, to get the DSP channel we are using a \
*				convert table at the ac494 driver)
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				callId - 0 = stop the active call, 1 = stop the active1 (hold) call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/

static int voip_stop_call(acl_line_t *line, int callId)  /* callId - 0=active0 or 1=active1  */
{
    int NumberOfChannelsPerLine;/* NIRP 070813*/

    DBG_PRINT("-> \r\n");

    acl_call_t *call=NULL;
    int channel = callId;

	if(callId == 0)        			/* stop the active call */
    {
    	call = line->active_call;
		channel = 0;
	}
	else if(callId == 1)     			/* stop the active1 (hold) call */
    {
    	call = line->active_call1;
		channel = 1;
	}
	else
	{
		DBG_PRINT_ERR("Illegal callId value %d\r\n", callId);
		return -1;
	}

//NIRP 070813 call1 is not connected to the DSP in case there is only 1 channel per line
	voip_get_number_of_channels_per_line(&NumberOfChannelsPerLine);
	if ((0 == channel) || ((1== channel)&&(NumberOfChannelsPerLine>1)))
	{
		   // need to check this - this fuction stops the dtmf testing by the rtp but we are using the DSP to do it
		   if (AC_TIMER_EXISTS(dtmf_stop, call))
		   {
		      dtmf_stop(call);
		      AC_TIMER_DEL(dtmf_stop, call);
		   }

			/* close the dsp channel and stop the RTP */
			dsp_voice_stop(line->num, channel);

			/* Must stop dtmf after closing rtp( because of a DSP bug ) in the held side	*/
			if ( call->mode == VOIP_CONN_MODE_RECV)
		    {
		        dsp_ibs_stop(line->num, channel);
		    }

	}
	DBG_PRINT("==================\r\n");
	DBG_PRINT("voip_stop_call line = %p callId = %d\r\n",line,callId);
	DBG_PRINT("<- \r\n");


    return 0;

} /* end of voip_stop_call */
/******************************************************************************/



/****************************************************************************
*
*	Name:		voip_start_call
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	open the channel and start the RTP of the callId.
*				Each connection (line) can haves up to two calls, one is the active and the second is the active1
*				The active is always connected to the handset and is assigned with channel=0 and callID=0
*				The active1 is use for holding an hold call and is assigned with channel=1 and callId=1
*				(the channel ID is the line channel and not the DSP channel, to get the DSP channel we are using a \
*				convert table at the ac494 driver)
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				callId - 0 = stop the active call, 1 = stop the active1 (hold) call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/

static int voip_start_call(acl_line_t *line, int callId)
{
    acl_call_t *call=NULL;
    int channel = callId;
    u32 rtp_id;
    int faxDataMethod; /* ACL NB 061213 */
    int g723_high_bitrate;/* ACL NIRP 061224 */
    int active_codec;/* ACL NIRP 061224 */
    int NumberOfChannelsPerLine;/* NIRP 070813*/

    DBG_PRINT("-> \r\n");

	if(callId == 0) { 						/* stop the active call */
    	call = line->active_call;
		channel = 0;
	}
	else if(callId == 1) {     				/* stop the active1 (hold) call */
    	call = line->active_call1;
		channel = 1;
	}
	else
	{
		DBG_PRINT_ERR("Illegal callId value %d\r\n", callId);
		return -1;
	}

	//NIRP 070813 call1 is not connected to the DSP in case there is only 1 channel per line
	voip_get_number_of_channels_per_line(&NumberOfChannelsPerLine);
	if ((0 == channel) || ((1== channel)&&(NumberOfChannelsPerLine>1)))
	{
		/* open the socket and get the ID of the RTP */
		if (!(rtp_id = rtp_id_prepare(call)))
		{
		  DBG_PRINT_ERR("Warning: voip_start_call::rtp_id_prepare failed \r\n");
		  return -1;
		}
		voip_conf_faxDataMethod_get(&faxDataMethod);/* ACL NB 061213 */
		voip_conf_g723_high_bitrate_get(&g723_high_bitrate);/* ACL NIRP 061224 */

		if (g723_high_bitrate)/* ACL NIRP 061224 temporarily "ride" the active_codec for additional information */
		   active_codec =  call->active_codec | ACL_723_HIGH_BITRATE_BIT;
		else
		   active_codec =  call->active_codec | ACL_723_LOW_BITRATE_BIT;

		//DBG_PRINT("call->rtp->port %d \r\n", call->rtp->port);
        active_codec = 9;
		/* open the DSP channel and connect it to the RTP */
		DBG_PRINT("dsp_voice_start dtmf_method = %d \r\n", call->dtmf_method);
		if (dsp_voice_start(line->num, channel, active_codec, ptime_get(call), call->dtmf_method, rtp_id, faxDataMethod))
		{
		  DBG_PRINT_ERR("Warning: voip_start_call::dsp_voice_start failed \r\n");
		   return -1;
		}
	}
	DBG_PRINT("==================\r\n");
	DBG_PRINT("voip_start_call line = %p callId = %d\r\n",line,callId);
	DBG_PRINT("<- \r\n");
    return 0;
} /* end of voip_start_call */
/******************************************************************************/



/****************************************************************************
*
*	Name:		line_connect_cb
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	connect the call to a connection (line).
*				if the call already exist (hold at active1) then move it to the active object
*				if a new call then put it on active
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				call - the new/old call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/

static int line_connect_cb(acl_line_t *line, acl_call_t *call)
{
    DBG_PRINT("-> line = %p call = %p\r\n",line,call);
/* ACL NB  060731 */
    if ( line->active_call == call )
    {
        DBG_PRINT("Warning: already in connected  \r\n");
    }
    else
    {
        /* Switch calls - connected call always placed on active_call
        if there is call in active_call it moves to active_call1 and might be re-connected */

        if ( !line->active_call)
        {
          line->active_call = call;

          /* a singal call that was stopped */
          if ( line->active_call1 == call )
          {
              	/* Stop active call1 !!!!*/
	        if(voip_stop_call(line,1) != 0)
		{
			DBG_PRINT("voip_start_call failed\r\n");
			return -1;
		}
              	line->active_call1 = NULL;
          }
        }
        /* switch*/
        else
        {
          	/* Stop active call !!!!*/
	      	if(voip_stop_call(line,0) != 0)
		{
			DBG_PRINT("voip_start_call failed\r\n");
			return -1;
		}
	        /* switch calls */
	        line->active_call1 = line->active_call;
	        line->active_call  = call;

	        if(voip_start_call(line,1) != 0)
	        {
			DBG_PRINT("voip_start_call failed\r\n");
			return -1;
	        }
        }

        /* start active_call*/
	    if(voip_start_call(line,0) != 0)
	    {
		DBG_PRINT("voip_start_call failed\r\n");
		return -1;
	    }
    }


   	DBG_PRINT("<- line %p \r\n",line);

	return 0;

} /* end of line_connect_cb */
/******************************************************************************/



/****************************************************************************
*
*	Name:		line_hold_cb
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	connect the call to a connection (line).
*				if the call already exist (hold at active1) then move it to the active object
*				if a new call then put it on active
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				call - the new/old call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/

static int line_hold_cb(acl_line_t *line, acl_call_t *call)
{
/* if hold is on the active call and if    */
/* there are 2 calls we need to switch them */
   	DBG_PRINT("-> \r\n");

	if(line->active_call == call)
    {
        /* Stop active call */
		if(voip_stop_call(line,0) != 0)
			return -1;

		line->active_call = NULL;

        /* if there is a call on hold  - stop it and switch calls  */
		if(line->active_call1 != NULL)
        {
			if(voip_stop_call(line,1) != 0)
				return -1;

            line->active_call = line->active_call1;

		    if(voip_start_call(line,0) != 0)
			    return -1;
       	}
		line->active_call1 = call;

		if(voip_start_call(line,1) != 0)
			return -1;
	}
    else if (line->active_call1 == call )
    {
        //DBG_PRINT("Warning: already in hold \r\n");

    }
    //else
    //    DBG_PRINT_ERR("Warning: Undefined call  \r\n");

   	DBG_PRINT("<- \r\n");

	return 0;

} /* end of line_hold_cb */
/******************************************************************************/



/****************************************************************************
*
*	Name:		line_unhold_cb
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				call - the new/old call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/
static int line_unhold_cb(acl_line_t *line, acl_call_t *call)
{
    DBG_PRINT("-> line = %p call = %p\r\n",line,call);

/* during conference unhold should is just for  */
/* open rtp but not start or stop voice         */

    if (!line->conference)
    {
        return( line_connect_cb(line,call) );
    }
    else
    {
        DBG_PRINT("unhold during 3wc.  line = %p call = %p\r\n",line,call);
    }

   	DBG_PRINT("<- \r\n");


	return 0;

} /* end of line_unhold_cb */
/******************************************************************************/




/****************************************************************************
*
*	Name:		line_disconnect_cb
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	disconnect the call, if no conference then normal stop, if conference then make sure
*				that the remaining call will be at active0
*----------------------------------------------------------------------------
*	Input:		line - this is the connection (line being use)
*				call - the new/old call
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 is case of error
******************************************************************************/

static int line_disconnect_cb(acl_line_t *line, acl_call_t *call)
{
    DBG_PRINT("-> line = %x call = %x\r\n",line,call);

	if(line->conference)     						/* check if conference */
    {
		if(line->active_call1 == call)           	/* if disconnect active1 - no problem, just stop it */
         {
			/* stop conference */
			dsp_3wc(line->num, 1, 0, 0);
			line->conference = 0;

			if(voip_stop_call(line,1) != 0)         /* stop the call */
				return -1;

			/* stop the conference */
            		line->active_call1 = NULL; /* ACL 060802 call; */
     	}
		else if(line->active_call == call)         /* if disconnect active0 - swap between active0 and active1 */
        {
            		/* stop conference */
			dsp_3wc(line->num, 1, 0, 0);
			line->conference   = 0;

			if(voip_stop_call(line,1) != 0)          /* stop active1 */
				return -1;
			if(voip_stop_call(line,0) != 0)          /* stop active0 */
				return -1;

			/* stop conference */
			line->active_call  = line->active_call1;        /* copy active1 to active0 */
            line->active_call1 = NULL; /* ACL 060802 call; */
			if(voip_start_call(line,0) != 0)              	/* start active0 */
				return -1;


		}
	}
	else    /* ! conference */
    {
		if(line->active_call == call)
        {
			if(voip_stop_call(line,0) != 0)
				return -1;

           line->active_call = NULL; /* ACL 060802 */

        }

		else if(line->active_call1 == call)
        {
			if(voip_stop_call(line,1) != 0)
				return -1;

            line->active_call1 = NULL; /* ACL 060802 */

        }
		else
        {
	        DBG_PRINT("already : disconected call %p \r\n", call );
        }
	}

  	 DBG_PRINT("<- \r\n");

	return 0;

} /* end of line_disconnect_cb */
/******************************************************************************/


/*======================================================================================================*/




static int line_connection_destroy_cb(void *ctx)
{
    acl_call_t *call = ctx;

    DBG_PRINT("-> \r\n");

    if ( !call )
    {
        DBG_PRINT("Already destroyed\r\n");
        return 0;
    }
    if ( call->rtp )
      rtp_free(call->rtp);
    else
       DBG_PRINT("call->rtp already free\r\n");

// ACL NB 060614
    if (call->phone)
    {
        /* check if call was not disconnected  yet !!! */
       if ( call->phone->active_call == call )
       {
       	    if(voip_stop_call(call->phone,0) != 0)
		return -1;

            call->phone->active_call = NULL;
            DBG_PRINT("clear active_call %p\r\n",call);
       }
       else if ( call->phone->active_call1 == call )
       {
       	    if(voip_stop_call(call->phone,1) != 0)
		return -1;

            call->phone->active_call1 = NULL;
            DBG_PRINT("clear active_call1 %p \r\n",call);
       }

      /* ACL 060912 */
      /*  if there is no secondary call */
      /* if one of the calls is null there are no 2 calls */
      if ( !call->phone->active_call || !call->phone->active_call1 )
        call->phone->second_call_orig = CALL_ORIGIN_UNKNOWN;

    }

	/* AlexChen SDK upgrade 20090415 */
	/*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated) - in case of remote conferece
    with parties that support nested re-invite == do not use the workaround - these buffers were never freed*/
    /*free and clear refer info structure*/
#ifdef CONFIG_RG_VOIP_RV_SIP
    if (NULL != call->refer_info.referredByHdrBuf)
    {
        free(call->refer_info.referredByHdrBuf);
        call->refer_info.referredByHdrBuf = NULL;
    }
    if (NULL != call->refer_info.referToHdrBuf)
    {
        free(call->refer_info.referToHdrBuf);
        call->refer_info.referToHdrBuf = NULL;
    }
    call->refer_info.didReferFail = 0;
    call->refer_info.unholdInProgress = 0;
#endif /* CONFIG_RG_VOIP_RV_SIP */

    free(call);

    DBG_PRINT("<- \r\n");

    return 0;
}

#ifdef CONFIG_RG_ATA

/* ACL NB 0604  */
static int fax_t38_start_cb(void *ep,acl_voip_sdp_msg_t *local,acl_voip_sdp_msg_t *remote,voip_conn_mode_t mode,int fax_rate)
{
   acl_line_t *line = ep;
   int channel;
   acl_call_t *call;
   int isT38 = 1;
   int common_codecs[NUMBER_OF_CODECS];
   int faxDataMethod;

   fax_transport_mode_t 	fax_transport_mode;

   DBG_PRINT("->\r\n");
   /* If disable should not start fax  */
   voip_conf_faxDataMethod_get(&faxDataMethod);
//	if ( faxDataMethod != 1 ) /*FAX_MODEM_TRANSFER_METHOD__RELAY*/
	if ( ( faxDataMethod & FAX_METHOD_MASK ) != T38_TRANSPORT_RELAY ) /*ACL NB 061214 the 4 LSBs are fax method */
   {
      DBG_PRINT("fax method is not T38!!!\r\n");
      return -1;
   }

   call = line->active_call;

   if ( call->isT38 )
      return 0;

   call->isT38 = 1;

   fax_transport_mode = FAX_TRANSPORT_RELAY;

    // Patch - Need to update remote params in case of changes caused by the re-inite !!!!!
   if (remote && remote_params_set(call, remote, common_codecs, call->rtp->fd))
	   return -1;

   if ( local )
   {
       /* Set SDP message properties */
       acl_voip_sdp_msg_media_descr_clear(local);
       acl_voip_sdp_msg_media_descr_add(local, call->rtp->port);

      call->mode = VOIP_CONN_MODE_SEND_RECV /* ACL NB 060420 mode*/;
      rtp_mode_set(call, 0);
   }

   channel = 0; //  ???
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
   //DBG_PRINT("fax rate parameter is(%d)\r\n", fax_rate);
   dsp_fax_start(line->num, channel, fax_transport_mode, fax_rate);
   rtp_t38_set(call, isT38);

   DBG_PRINT("<-\r\n");
   return 0;
}

/* ACL NM 080403  */
static int stop_fax_cb(void *ep)
{
	int channel;

   	acl_line_t *line = ep;
   	acl_call_t *call = line->active_call;

	fax_transport_mode_t 	fax_transport_mode;
   	channel = 0; //  ???

	DBG_PRINT("->\r\n");

	if(call->isT38)	/* T38*/
		fax_transport_mode = FAX_TRANSPORT_RELAY;
	else  /* VBD */
		fax_transport_mode = FAX_VOICE_BAND_DATA;

	/* Stop T38 or VBD */
   	dsp_fax_stop(line->num, channel, fax_transport_mode);

	DBG_PRINT("<-\r\n");

	return 0;
}


/* ACL NB 061217 */
static int fax_bypass_start_cb(void *ep)
{
   acl_line_t *line = ep;
   acl_call_t *call;
   int faxDataMethod;
   int channel;

   fax_transport_mode_t 	fax_transport_mode;

   DBG_PRINT("->\r\n");
   /* If disable should not start fax  */
   voip_conf_faxDataMethod_get(&faxDataMethod);
//	if ( faxDataMethod != 1 ) /*FAX_MODEM_TRANSFER_METHOD__RELAY*/
	if ((( faxDataMethod & FAX_METHOD_MASK )  != T38_VOICE_BAND_DATA ) && /*ACL NB 061214 the 4 LSBs are fax method */
       (( faxDataMethod & DATA_METHOD_MASK ) != DATA_VOICE_BAND_DATA ))
   {
      DBG_PRINT("fax/data method is not Bypass!!!\r\n");
      return -1;
   }

   call = line->active_call;

   channel = 0; //  ???

   fax_transport_mode = FAX_VOICE_BAND_DATA;

   dsp_fax_start(line->num, channel, fax_transport_mode, -1);

   if ( call->isVBD )
      return 0;

   call->isVBD = 1;

   DBG_PRINT("<-\r\n");

   return 0;
}

/* AlexChen SDK upgrade 20090415 */
/*ACL XY 081218 Fax Fallback*/
static int start_fax_fallback_cb(void *conn_context)
{
    acl_line_t *conn = (acl_line_t *)conn_context;
    acl_call_t *call = conn->active_call;
    int n, ch;
    int      bypassCoder;

    n = conn->num;
    ch = 0;

    if ( call->isFaxFallback )
      return 0;

    call->isFaxFallback = 1;

    voip_conf_bypassCoder_get(&bypassCoder);
    /*configure the dsp for transparent*/
    return dsp_voice_update(n, ch,
                                   bypassCoder,
                                   codec_get_by_payload(bypassCoder)->ptime,
                                   acl_line_get_dtmf_method(call),
                                   acl_line_get_rtp_id(call),
                                   0);
}

#endif


static void *line_connection_context_create(int epn, char *name, int keys_sequence)
{
    int hf;

    acl_line_t *line = AC_ZALLOC_L(sizeof(acl_line_t));
   char              *destCfw=NULL; /*ACL NB 070411 */
   voip_cfw_params_t cfwParams;      /*ACL NB 070417 */
	char isDndEnabled;				/* AlexChen SDK upgrade 20090407 */

    if (!line)
	return NULL;

    line->num = epn;
   line->keys_sequence    = keys_sequence;      /* ACL NB 060913 */
	line->second_call_orig = CALL_ORIGIN_UNKNOWN;/* ACL NB 060912 */
   voip_conf_cfw_keySequence_get( &line->user_cfw_KeySeq[0] ); /* ACL NB 070103 */

    /* AlexChen SDK upgrade 04072009 dnd feature */
   voip_conf_dnd_keySequence_get( &line->user_dnd_KeySeq[0] );

   voip_conf_dnd_enable_get( &isDndEnabled);
   if( isDndEnabled )
      voip_conf_dnd_line_get(line->num,&line->is_dnd_activated);
   else
      line->is_dnd_activated = 0;
   line->is_dnd_on = 0;

   voip_conf_blind_transfer_enable_get( &line->blindTransferEnabled );

   /* ACL NB - */
   if ( voip_conf_ext_cfw_dest_get(line->num, &destCfw) != 0 )
   {
      DBG_PRINT_ERR("Warning:voip_conf_ext_cfw_dest_get Failed \r\n") ;
   }
   else
   {
      voip_conf_cfw_type_get(&cfwParams);

      if ( strlen(destCfw) && cfwParams.isEnabled )
      {
         line->is_cfw_activated = 1;
         line->is_cfw_on        = 1;
      }
      else
      {
         line->is_cfw_activated = 0;
         line->is_cfw_on        = 0;
      }
   }

   phone_input_init(line);

    /* Check initial hook state */
    line->is_off_hook = dsp_select_mode(epn, &hf);
    //ipc_hook_state_update(epn, cont_mn->is_off_hook);//060530 Avner phase 1 - ipc only from main to voip not vice versa

    voip_conf_stutter_tone_dur_get(&line->stutter_tone_dur);
    if (line->stutter_tone_dur < 0)
      line->stutter_tone_dur = 1000; // TBD
    DBG_PRINT(">>> line->is_off_hook=%d, line->num=%d, \n", line->is_off_hook, line->num);
    //NIRP VI 51493 in case of off hook when the voip task is initialized calling the process signal so
    //that a pre-configured tone is played
    if (1 == line->is_off_hook)
    {
	    offhook_event_handle(line);
        phone_output_ind("hf", hf);
    }

#if !defined(CONFIG_RG_VOIP_RV_SIP)&&!defined(CONFIG_RG_VOIP_RV_MGCP)

    /* ACL NB 080218 - VI 61324 workaround*/
    /* in case the voip_task is down while ringing we should stop ringing */
    if(!call_mgt->is_fxo_line(line->num))
    {
    	/*JL should not ring if FXO line!!!*/
	DBG_PRINT("line->num=%d, - stop ringing\n",line->num );
//	dsp_ring(line->num, 0);
    }

#endif

    return line;

}

static void line_connection_context_destruct(void *line)
{
    if (!line)
	return;
    phone_input_uninit(line);
    free(line);
}

static void dialed_digit_process(acl_line_t *line, int digit, int down)
{
    call_mgt->process_event(line->num, digit, down);
}

static void offhook_event_handle(acl_line_t *line)
{
	//ER0106 - Set LED indication according to hook state
	set_phone_led_to_state(line->num, e_GREEN_LED_STATE_ON);
    	offhook_signal_send(line);
}

static void line_onhook_event_handle_cb(int LineIndex)
{
	int status = call_mgt->get_registration_status(LineIndex);

	if(SIP_REG_REGISTERED == status)
	{
		//registration to sip proxy succedded
		set_phone_led_to_state(LineIndex, e_GREEN_LED_STATE_VERY_SLOW_FLASH);
	}
	else if((SIP_REG_ERROR == status) || (SIP_REG_UNAUTHENTICATED == status))
	{
		//registration to sip proxy failed
		set_phone_led_to_state(LineIndex, e_RED_LED_STATE_ON);
	}
	else
	{
		//user is not using sip proxy
		set_phone_led_to_state(LineIndex, e_LED_STATE_OFF);
	}
}

/*080818 ACL MC VI65268 return busy when receive fax call during fax session*/
static int is_during_fax_call_cb(void *conn_context)
{
    acl_line_t *conn = (acl_line_t *)conn_context;
    acl_call_t *call = conn->active_call;

      DBG_PRINT("-> conn = %x \r\n",conn );
     if((conn==NULL)||(call==NULL))
     {
            DBG_PRINT("No connection or call \r\n");
            return 0;
     }

      if(call->isT38 ||call->isVBD)
     {
            DBG_PRINT("Fax session is actiaved \r\n");
            return 1;
      }
      else
      {
            DBG_PRINT("Fax session isn't actiaved \r\n");
            return 0;
      }

}

static void onhook_event_handle(acl_line_t *line)
{
	DBG_PRINT ("onhook_event_handle(%p) \n ",line);
	line_onhook_event_handle_cb(line->num);
	//if (line->is_off_hook)
    	onhook_signal_send(line);
	DBG_PRINT ("returned from onhook_event_handle() \n ");
}

int acl_line_is_in_active_call(acl_line_t *line)
{
    return line->active_call || line->active_call1;
}


// NIRP 070527
static void handle_daa_ring_detect_start_event(acl_line_t *line)
{
	DBG_PRINT("handle_daa_ring_detect_start_event !!!\r\n");

	call_mgt->process_event(line->num, VOIP_INPUT_DAA_RING_DETECT_START, 0 /*down*/);
	/* the down has no meaning in ring detection */
	return;
}

// NIRP 070527
static void handle_daa_ring_detect_stop_event(acl_line_t *line)
{
	DBG_PRINT("handle_daa_ring_detect_stop_event line =%d !!!\r\n", line->num);

	call_mgt->process_event(line->num, VOIP_INPUT_DAA_RING_DETECT_STOP, 0 /*down*/);

	/* the down has no meaning in ring detection */
	return;
}


#define DTMF_DEFAULT_DURATION 90

// ACL NB 060424
static void fax_t38_detect_event_handle(acl_line_t *line)
{
	int faxDataMethod;

	voip_conf_faxDataMethod_get(&faxDataMethod);
	//	if (faxDataMethod != 1 ) /*FAX_MODEM_TRANSFER_METHOD__RELAY*/
	if ( ( faxDataMethod & FAX_METHOD_MASK ) != T38_TRANSPORT_RELAY ) /*ACL NB 061214 the 4 LSBs are fax method */
	{
		DBG_PRINT("fax method is not T38!!!\r\n");
		return;
	}

	if (line->active_call && ( line->active_call->isT38 == 0) )
	{
		line->active_call->is_fax_relay_detected = 1; /*ACL XY 081230 Fax Fallback*/
		call_mgt->process_event(line->num, VOIP_INPUT_FAX_DETECT, 0 /*down*/); /* the down has no meaning in fax detection */
	}
	else
	{
		DBG_PRINT(" Fax event already detedted!!!\r\n");
	}

	return;
}


// NIRP 070607
static void handle_daa_hook_flash_hook_detect_event(acl_line_t *line)
{
	call_mgt->process_event(line->num, PHONE_INPUT_DAA_FLASH_HOOK_RELAY, 0 /*down*/);
	return;
}

// ACL NB 060704
void fax_bypass_detect_event_handle(acl_line_t *line)
{
   int faxDataMethod;

   voip_conf_faxDataMethod_get(&faxDataMethod);

   /* only in case of vbd , in case of a "real" bypass every thing is done by the DSP */
//	if (faxDataMethod != 2 ) /* vbd */
	if ((( faxDataMethod & FAX_METHOD_MASK )  != T38_VOICE_BAND_DATA ) && /*ACL NB 061214 the 4 LSBs are fax method */
       (( faxDataMethod & DATA_METHOD_MASK ) != DATA_VOICE_BAND_DATA ))
   {
      DBG_PRINT("fax/data method is not VBD!!!\r\n");
      return;
   }

   if (line->active_call  && ( line->active_call->isVBD == 0) )  /* ACL NB 061217 - if not already detected !!!! */
   {
	   call_mgt->process_event(line->num, VOIP_INPUT_FAX_BYPASS_DETECT, 0 /*down*/); /* the down has no meaning in fax detection */
   }
   else
   {
	   DBG_PRINT(" Bypass event already detedted!!!\r\n");
   }

   return;
}

// ACL NB 070806
static void switch_to_voice_detect_event_handle(acl_line_t *line)
{
	int faxDataMethod;

	voip_conf_faxDataMethod_get(&faxDataMethod);

	if ( ( faxDataMethod & FAX_METHOD_MASK ) != T38_TRANSPORT_RELAY ) /*ACL NB 061214 the 4 LSBs are fax method */
	{
		DBG_PRINT("fax method is not T38!!!\r\n");
		return;
	}

	if (line->active_call && ( line->active_call->isT38 == 1) )
	{
		line->active_call->isT38 = 0;
		line->active_call->is_fax_relay_detected = 0;  /*ACL XY 081230 Fax Fallback*/
		/*ACL XY 080917 - VI66797: problems with T.38 fax transmissions*/
		line->active_call->is_fax_relay_detected_wait_for_ok = 0;

		call_mgt->process_event(line->num, VOIP_INPUT_SWITCH2VOICE_DETECT, 0 /*down*/); /* the down has no meaning in fax detection */
	}
	else
	{
		DBG_PRINT(" already in voice mode !!!\r\n");
	}

	return;
}

/* state machine handling of input events */
void acl_line_connection_input_event_handle(acl_line_t *line,
    phone_input_event_t input_event, phone_event_t *pEvent)
{
   int down=pEvent->pressed;

   DBG_PRINT ("acl_line_connection_input_event_handle event=%d pressed=%d\n ",input_event,pEvent->pressed);

    /* XXX */
   /* DBG_PRINT("Got input event: %s on %d\n\n",
	phone_input_get_event_desc(input_event), line->num);*/

	 //DBG_PRINT(" Got input event\r\n");
    switch (input_event)
    {
    case PHONE_INPUT_HOOK_ON:
	 DBG_PRINT(" ON HOOK \r\n");
	if (!down)
	    break;
	/* AlexChen SDK upgrade 20090407 */
	line->is_dnd_on = 0;
	onhook_event_handle(line);
	line->is_off_hook = 0;
	//ipc_hook_state_update(line->num, line->is_off_hook);//060530 Avner phase 1 - ipc only from main to voip not vice versa
	phone_output_ind("hf", 0);
	DBG_PRINT("break acl_line_connection_input_event_handle() \n");
	break;
    case PHONE_INPUT_HOOK_OFF:
	if (!down)
	    break;
	{
	    int hf;

	    line->is_off_hook = 1;
	    offhook_event_handle(line);
	    dsp_select_mode(line->num, &hf);
	    //ipc_hook_state_update(line->num, line->is_off_hook);//060530 Avner phase 1 - ipc only from main to voip not vice versa
	    phone_output_ind("hf", hf);
	}
	break;
    case PHONE_INPUT_0:
    case PHONE_INPUT_1:
    case PHONE_INPUT_2:
    case PHONE_INPUT_3:
    case PHONE_INPUT_4:
    case PHONE_INPUT_5:
    case PHONE_INPUT_6:
    case PHONE_INPUT_7:
    case PHONE_INPUT_8:
    case PHONE_INPUT_9:
    case PHONE_INPUT_ASTERISK:
    case PHONE_INPUT_POUND:
	/* Eitan1207 - Add support for DTMF A-D */
	case PHONE_INPUT_A:
	case PHONE_INPUT_B:
	case PHONE_INPUT_C:
	case PHONE_INPUT_D:
    //DBG_PRINT("acl_line_connection_input_event_handle %d \r\n", input_event );

    /* DBG_PRINT_ERR("[%s:%d]:calling dialed_digit_process with %d\r\n" , __FUNCTION__, __LINE__,input_event - PHONE_INPUT_0); */
	dialed_digit_process(line, input_event - PHONE_INPUT_0, down);
	if (!vcCONFIG_RG_ATA) /* On ATA the tone is played by the hardware */
	    dsp_dtmf(line->num, input_event - PHONE_INPUT_0, down, 0);/* ACL NB 060720 - added duration */
	break;
#ifdef CONFIG_RG_IPPHONE
    case PHONE_INPUT_SETTINGS:
	{
	    int hf;
	    voip_volume_params_t vol;

	    if (down)
		break;
#if 0
	    vol = dsp_volume_get();
#endif
	    if (!dsp_select_mode(line->num, &hf))
		phone_output_show_volume(vol.ring_vol);
	    else if (hf)
		phone_output_show_volume(vol.loudspeaker_vol);
	    else
		phone_output_show_volume(vol.handset_vol);
	    AC_IPC_VOLUME_SET(vol);
	}
	break;

/* ACL NirS 081126 */
	case PHONE_INPUT_LINE_1:
		call_mgt->process_event(line->num,VOIP_INPUT_LINE_1,down);
		break;
	case PHONE_INPUT_LINE_2:
		call_mgt->process_event(line->num,VOIP_INPUT_LINE_2,down);
		break;
	case PHONE_INPUT_SPEAKER:
		call_mgt->process_event(line->num,VOIP_INPUT_SPEAKER,down);
		break;
	case PHONE_INPUT_HEADSET:
		call_mgt->process_event(line->num,VOIP_INPUT_HEADSET,down);
		break;


#endif
    case PHONE_INPUT_FLASH:
   if ( line->keys_sequence == KEY_SEQ_FLASH_ONLY )
   {
	  /* ACL NB 070904 - workaround! Hold event can cause a crash of voip task! if the hold */
      /* event arrives while Modify Media is called (RE-invite) for switch to voice event.  */
      if ( ( line->active_call) &&
           ( line->active_call->isT38 || line->active_call->isVBD ))
      {
         DBG_PRINT_ERR("onhook_signal_send - not a voice call- Flash is not allowed \r\n");
      }

      /* if there is secondary call that is outgoing call and call  already  */
      /* connected should send Conf event else it should send hold/toggle    */
      else if (line->active_call && line->second_call_orig == CALL_ORIGIN_OUTGOING )
	      call_mgt->process_event(line->num, VOIP_INPUT_CONF, down);
      else
      	call_mgt->process_event(line->num, VOIP_INPUT_HOLD, down);
   }
   else
	   dialed_digit_process(line, VOIP_INPUT_FLASH, down);
	break;
    case PHONE_INPUT_HOLD:
	call_mgt->process_event(line->num, VOIP_INPUT_HOLD, down);
	break;
    case PHONE_INPUT_TRANSFER:
	call_mgt->process_event(line->num, VOIP_INPUT_TRANSFER, down);
	break;
    case PHONE_INPUT_CONF:
	call_mgt->process_event(line->num, VOIP_INPUT_CONF, down);
	break;
    case PHONE_INPUT_FAX_DETECT:
   fax_t38_detect_event_handle(line);
	break;
    case PHONE_INPUT_FAX_BYPASS_DETECT:
   fax_bypass_detect_event_handle(line);
	break;
    case PHONE_INPUT_CFW: /* ACL NB 061113 call forward*/
       if ( line->is_cfw_on )
	      call_mgt->process_event(line->num, VOIP_INPUT_CFW_ON, down);
       else
	      call_mgt->process_event(line->num, VOIP_INPUT_CFW_OFF, down);
	break;
    case PHONE_INPUT_DAA_RING_START:
		//NIRP 070527
		DBG_PRINT("PHONE_INPUT_DAA_RING_START .\r\n");
		handle_daa_ring_detect_start_event(line);
		break;

    case PHONE_INPUT_DAA_RING_STOP:
		//NIRP 070527
		DBG_PRINT("PHONE_INPUT_DAA_RING_STOP .\r\n");
		handle_daa_ring_detect_stop_event(line);
		break;

    case PHONE_INPUT_DAA_FLASH_HOOK_RELAY:
		//NIRP 070607
		DBG_PRINT("PHONE_INPUT_DAA_FLASH_HOOK_RELAY.\r\n");
		handle_daa_hook_flash_hook_detect_event(line);
		break;
    /* ACL NB 070507 Blind Transfer */
    case PHONE_INPUT_BLINDT:
	   call_mgt->process_event(line->num, VOIP_INPUT_BLINDT, down);
	   break;
    case PHONE_INPUT_SWITCH2VOICE_DETECT:
       switch_to_voice_detect_event_handle(line);
       break;

    case PHONE_INPUT_STUN_RES:
       stun_response_handle(line, pEvent);
       break;
	/* AlexChen SDK upgrade 20090407 dnd feature */
    case PHONE_INPUT_DND:
       DBG_PRINT("[%s:%d] PHONE_INPUT_DND\r\n", __FUNCTION__, __LINE__);
       voip_conf_dnd_line_set( line->num,line->is_dnd_activated );
//       dsp_play(line->num, VOIP_SIGNAL_STUTTER_TONE, 1);
       call_mgt->process_event(line->num, VOIP_INPUT_DND, down);

	   break;
    default:
	break;
    }


	DBG_PRINT ("returned from acl_line_connection_input_event_handle() \n\n ");
}

static void acl_line_connection_codecs_init(codec_t *conf_codecs, int out_of_band_dtmf)
{
    int i;

    codecs = conf_codecs;

    /* find the place for the DTMF codec in the array. there MUST be at least
     * one free place */
    for (i = 0; codecs[i].payload != -1; i++);
    /* add DTMF codec */
    codecs[i].payload = AC_PAYLOAD_TYPE_ENUM_TO_ID(RTP_PAYLOAD_DTMF);
    codecs[i].enabled = out_of_band_dtmf;
    codecs[i].ptime = 0;
    codecs[i].rate = AUDIO_CODEC_RATE;
}

void acl_line_connection_init(codec_t *conf_codecs, int out_of_band_dtmf)
{
   /* ACL NB 061105 - added advanced_channel_configuration*/
 	voip_dsp_advanced_channel_configuration_t advanced_channel_configuration;

   giNumOfFXOL = voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO);
   giNumOfFXSL = voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXS);

 	voip_conf_get_advanced_channel_configuration(&advanced_channel_configuration);

    aclVoipRtpUtils_advancedChConf(&advanced_channel_configuration);
   g_silenceCompression = advanced_channel_configuration.SilenceCompressionMode;/* ACL NB 061105 */

   voip_conf_get_sdp_include_ptime(&g_isSDPIncludePTime); /* ACL NB 070719*/

    acl_line_connection_codecs_init(conf_codecs, out_of_band_dtmf);
#ifdef CONFIG_RG_ATA
    caller_id = AC_ZALLOC_E(sizeof(lcd_caller_id_data_t) *(giNumOfFXOL+giNumOfFXSL));
#endif
}

void acl_line_connection_uninit(void)
{
#ifdef CONFIG_RG_ATA
	free(caller_id);
	caller_id = NULL;
#endif
}

int acl_line_get_dtmf_method(acl_call_t * call)	/*JL VI # 64097*/
{
	return call->dtmf_method;
}

int acl_line_get_rtp_id(acl_call_t * call)/*JL VI # 64097*/
{
	return rtp_id_prepare(call);
}

int acl_line_get_active_codec(acl_call_t * call)/*JL VI # 64097*/
{
	return call->active_codec;
}

int acl_line_get_active_codec_ptime(acl_call_t * call)/*JL VI # 64097*/
{
	return ptime_get(call);
}

/* AlexChen SDK upgrade 20090408 Fax */
int acl_line_fax_relay_deteceted_get(acl_line_t *phoneConn)
{
    acl_call_t *call = phoneConn->active_call;

    if((phoneConn==NULL)||(call==NULL))
    {
           DBG_PRINT("No connection or call \r\n");
           return 0;
    }

    return call->is_fax_relay_detected;
}

void acl_line_fax_relay_detected_wait_for_ok_set(acl_line_t *phoneConn,int val)
{
    acl_call_t *call = phoneConn->active_call;

    if((phoneConn==NULL)||(call==NULL))
    {
           DBG_PRINT("No connection or call \r\n");
           return ;
    }

    call->is_fax_relay_detected_wait_for_ok = val;
}

int acl_line_fax_relay_detected_wait_for_ok_get(acl_line_t *phoneConn)
{
    acl_call_t *call = phoneConn->active_call;

    if((phoneConn==NULL)||(call==NULL))
    {
           DBG_PRINT("No connection or call \r\n");
           return 0;
    }

    return call->is_fax_relay_detected_wait_for_ok;
}

char *active_codec_name_get(acl_line_t *phone)
{
    if (!phone || !phone->active_call)
	return "";
    return AC_RTP_PT_TO_NAME(phone->active_call->active_codec); /* ER08 - MP202 Infra */
}

codec_t *codec_get_by_payload(int payload)
{
    int i;

    for (i=0; i<NUMBER_OF_CODECS && !(payload==codecs[i].payload &&
	codecs[i].enabled); i++);
    return (i < NUMBER_OF_CODECS) ? &codecs[i] : NULL;
}

/* translate dial string accepted, as the '*' and '#' keys are translated to 'e'
 * and 'f', respectively, by the MDM - we need to reverse this translation */
void dtmf_convert(char *dialed)
{
    /* find the next appearance of E or F (we may reach the string's end) */
    while (*(dialed += strcspn(dialed, "efEF")))
    {
// ACL NB 070604 	*dialed = strcspn(dialed, "eE") ? 0 : '*';
 	*dialed = strcspn(dialed, "eE") ? '#' : '*';
	dialed++; /* continue search after the current match */
    }
}


static void display_update_cb(void *context, voip_display_state_t display_state, void *pInfo)
{
#ifdef CONFIG_RG_IPPHONE
#ifndef CONFIG_GTEK

    if (!pInfo)
	    {printf("\r\nERROR: pInfo is NULL\r\ndisplay_state = [%d]\r\n", display_state);return;}
    //printf("\r\ndisplay_state = [%d]\r\n", display_state);

#ifdef NANOX
    LCDDisplayDatum((int)display_state, pInfo);
#else

    switch (display_state)
    {
	case VOIP_DISPLAY_IDLE:
	    LCDDisplayIdle(pInfo);
	    break;

	case VOIP_DISPLAY_DIALING:
	    LCDDisplayDialing(pInfo);
	    break;

	case VOIP_DISPLAY_RINGING_PARTY:
	    LCDDisplayRingingParty(pInfo);
	    break;

	case VOIP_DISPLAY_CALL_IN_PROGRESS:
		LCDDisplayCallInProgress(pInfo);
	    break;

	case VOIP_DISPLAY_BUSY:
		LCDDisplayBusy(pInfo);
		break;

	case VOIP_DISPLAY_HOLD:
		LCDDisplayHold(pInfo);
		break;

	case VOIP_DISPLAY_CALL_END:
		LCDDisplayCallEnd(pInfo);
		break;

	case VOIP_DISPLAY_PARTY_HUNG_UP:
		LCDDisplayOtherPartyDisconnected(pInfo);
	    break;

	case VOIP_DISPLAY_WRONG_NUMBER:
		LCDDisplayWrongNumber(pInfo);
	    break;

	case VOIP_DISPLAY_INCOMING_CALL:
		LCDDisplayInomingCall(pInfo);
	    break;

	default:
	    break;
    }
#endif
#endif
#endif
}


static int line_connection_modify_cb(void *ctx, acl_voip_sdp_msg_t *local,
    acl_voip_sdp_msg_t *remote, voip_conn_mode_t mode, int remoteMediaDesIdx)
{
    acl_call_t *call = ctx;
    int common_codecs[NUMBER_OF_CODECS];
    int faxDataMethod; /* ACL NB 060424 */
    rtp_payload_type_t curr_active_codec; /* ACL NB 080226*/
	acl_voip_sdp_media_descr_t *media; /* AlexChen SDK upgrade 20090415 ACL AL 081116 VI67566 - MOH issue: re- INVITE without sendonly upon 407: Added sendonly attribute */

    printf("-> \r\n");

    if(NULL == call)
            return -1;

    printf("-> line %p call %p \r\n", call->phone, call);

	/*AlexChen SDK upgrade 20090415 ACL XY 081218 - Fax Fallback
     after we got 488 error message from remote side,we send re-invite with audio+711,
     this will set local sdp and start dsp again.we must prevent it bye using this flag.
     */
    if ( call->isFaxFallback )
    {
        printf("FaxFallback failed\n");
        return 0;
    }


    /* ACL NB 080226 - save current codec, for case that the function is  called to change codec and mode at the same call*/
    curr_active_codec = call->active_codec;

    if (remote && remote_params_from_media_desc_set(call, remote, common_codecs, call->rtp->fd, remoteMediaDesIdx))
    {
        printf("remote remote_params_from_media_desc_set failed\n");
        return -1;
    }


    /* ACL NB 060809  Need to add port -  PATCH  !!!*/
    if (!remote )
    {
       /*  build SDP with  current codec as first priority */

        if (call->active_codec != local_sdp_message_with_current_codec_set(local, call->rtp->port, call->active_codec ))
        {
            printf("No active codec.\r\n");
            return -1;
        }
        else
            return 0;
    }
    else
        local_sdp_message_set(local, call->rtp->port, common_codecs);

    /* ACL NB 060521 PATCH  for RV2.0*/
/* In RV version 2.0
 *	The media stream is made unidirectional by calling the callback
 * RvMdmTermModifyMediaCB. and not by calling the callback RvMdmTermMgrDisconnectCB.
 */
   //DBG_PRINT("call->mode %d mode %d \r\n",call->mode, mode );

   if (call->mode != mode)
   {
        printf("change mode call->mode %d to mode %d \r\n",call->mode, mode );
      /* in case of unhold */
      if ( mode == VOIP_CONN_MODE_SEND_RECV )
      {
         /* ACL NB 070517 */
         //DBG_PRINT_ERR("**unhold_  mode= %d  call->mode = %d \r\n", mode, call->mode);
         /* Remote unhold - only stop the Tx RTP (not areal hold)*/
         if (call->mode ==  VOIP_CONN_MODE_RECV)
         {
               	printf("Remote unhold \r\n");
            	call->mode = mode;
            	rtp_mode_set(call, 0);
         }
         else
         {
		/* Local unhold */
		printf("Local unhold \r\n");
		call->mode = mode;
		rtp_mode_set(call, 0);
		if ( call->phone )
			line_unhold_cb(call->phone, call);
         }
      }
      /* in case of remote hold */
      else if ( mode == VOIP_CONN_MODE_RECV )
      {
		/* ACL NB 070517 */
		printf("Remote hold \r\n");
		call->mode = mode;
		rtp_mode_set(call, 0);

 	}
      /* Local hold */
      else if ( mode == VOIP_CONN_MODE_SEND )
      {
		char pMuteRtp = 0;
		/* ACL NB 070517 */
		printf("Local hold \r\n");


		/*ACL AL 081026 VI65329 cannot play MOH after Re-INVITE, Swyx/Topaz, Germany -
		This modification is temporary and should be configurable!!!*/
		/*EA - 090107 - AC68794 - Make the 'Mute RTP' option (upon hold) configurable*/
		voip_conf_rtp_mute_on_hold_get(&pMuteRtp);
		if (pMuteRtp)
		{
			printf("setting call->mode = 0 for not sending RTP on local hold");
			call->mode = 0;
		}
		else
		{
			call->mode = mode;
		}

		/* ACL AL 081116 VI67566 - MOH issue: re- INVITE without sendonly upon 407: Added sendonly attribute */
		media = acl_voip_sdp_msg_media_descr_get(local, 0);
		/*printSdp((char *)local,"modify_media_cb: \n ********* local SDP *********"); */
		acl_voip_sdp_media_descr_attr_add(media, "sendonly", "");
		/*printSdp((char *)local,"modify_media_cb: \n ********* local SDP + sendonly *********"); */

		rtp_mode_set(call, 0);
		if ( call->phone )
			line_hold_cb(call->phone, call);
#if 0 //AlexChen SDK upgrade 20090415
		/* ACL NB 070517 */
		DBG_PRINT("Local hold \r\n");
		call->mode = mode;
		rtp_mode_set(call, 0);
		if ( call->phone )
			line_hold_cb(call->phone, call);
#endif
      }
	/* ACL NB 080213 - function is called to change codec and mode at the same call*/
	if (remote && (curr_active_codec != call->active_codec) )
	{
		printf("codec was changed from %d to %d\r\n", curr_active_codec, call->active_codec);
	}
	else
	       return 0;

   }

/* ACL NB 080226 - redundant code
	call->mode = mode;
    	rtp_mode_set(call, 0);
*/

   if (call->phone)
   {
	   int channel;
	   u32 rtp_id;
	   char fax_to_transparent_Enabled = 0; /* AlexChen SDK upgrade 20090415 */

      if (call->phone->active_call == call)
         channel = 0;
      else
         channel = 1;

      //DBG_PRINT("channel %d \r\n",channel);/*sleep(1);*/

      if ( channel == 0)
      {
         	printf("need to update the new codec \r\n");
		 /* need to restart the DSP with the new codec */
	      //dsp_voice_stop(call->phone->num, channel);  ACL NB 080402 - use update instead of close and open

	      if (!(rtp_id = rtp_id_prepare(call)))
	          goto Error;

         voip_conf_faxDataMethod_get(&faxDataMethod); /* ACL NB 061213 */

         printf("dsp_voice_start dtmf_method = %d \r\n", call->dtmf_method);
/* ACL NB 061213
         if (dsp_voice_start(call->phone->num, channel, call->active_codec,
	          ptime_get(call), call->dtmf_method, rtp_id,faxDataMethod))
*/
			/* AlexChen SDK upgrade 20090415 */
			/*ACL XY 090106 - Fax Fallback
           There are times that the remote G/W detects the fax first and sends re-invite with Audio
           (since it is configured to Transparent).In this case, we need to update the channel with
           fax mode =  transparent and update the RTP with the chosen codec*/
         voip_conf_update_fax_to_transparent_enable_get( &fax_to_transparent_Enabled );
         if( fax_to_transparent_Enabled == 1 && remote && (common_codecs[0] == 0 || common_codecs[0] == 8))
            faxDataMethod = 0;

			if (dsp_voice_update(call->phone->num, channel, call->active_codec,
				ptime_get(call), call->dtmf_method, rtp_id,faxDataMethod))
	      {
	          goto Error;
	      }
      }

	   /* change the displayed codec */
	   //display_update_cb(call->phone, VOIP_DISPLAY_CALL_IN_PROGRESS, NULL);
    }

    printf("->\r\n");/*sleep(1);*/

    return 0;

Error:
    printf("-> ERROR \r\n");/*sleep(1);*/
    /* Break off association between connection and call */
    if (call->phone)
    {
	if (call->phone->active_call == call )
	    call->phone->active_call = NULL;
	if (call->phone->active_call1 == call)
	    call->phone->active_call1 = NULL;
    }

    return -1;
}


//060530 Avner phase 1
/* get current line state for a polling display. note that this should be
 * extended to reflect the possibility of several channels and held calls over
 * this line, and to indicate past calls statistics */
void acl_line_voip_state_get(int lineNum, voip_state_t *state)
{
#ifdef CONFIG_RG_ATA


    acl_line_t *line = call_mgt->get_conn_by_line(lineNum);
    rtp_statistics_t rtpstat;

    memset(&rtpstat, 0, sizeof(rtpstat));

    memset(state, 0, sizeof(*state));
    if (!is_line_enabled(lineNum) || !line)
    {
	state->phone = PHONE_STATE_DISABLED;
	return;
    }

    /* app data */
    AC_STRNCPY(state->remote_number, caller_id[lineNum].number,
	sizeof(state->remote_number));

    AC_STRNCPY(state->remote_id, caller_id[lineNum].name, sizeof(state->remote_id));

    AC_STRNCPY(state->remote_addr, caller_id[lineNum].addr,
	sizeof(state->remote_addr));

    /* driver data: phone, jitter, round_trip, packet loss, loss percentage */
    dsp_get_info(lineNum, state);

    /* RTP/driver data */
    if (line->active_call && line->active_call->rtp &&
	line->active_call->rtp->fd >= 0)
    {
        rtp_statistics_get(line, &rtpstat);
    }
    state->packets_sent = rtpstat.packets_sent;
    state->packets_received = rtpstat.packets_received;
    state->bytes_sent = rtpstat.bytes_sent;
    state->bytes_received = rtpstat.bytes_received;
	// ER06 - Take these valus from the DSP instead.
//    state->packets_lost = rtpstat.packets_lost;
//    state->packet_loss_percentage = rtpstat.packet_loss_percentage;
    state->enc = rtpstat.enc;
    state->dec = rtpstat.dec;
    state->duration = rtpstat.duration;

    /* RV data */
    state->sip_reg = is_using_proxy() ?
	call_mgt->get_registration_status(lineNum) : SIP_REG_UNREGISTERED;
    state->next_sip_reg = 30;
    call_mgt->get_call_state(lineNum, &state->call, &state->origin);

    /* ER06 - Set voice/fax indication. ACL NB 061217  - Add support for fax bypass */
	if(line->active_call)
		state->type = ( line->active_call->isT38 || line->active_call->isVBD ) ? CALL_TYPE_FAX : CALL_TYPE_VOICE;
	else
		state->type = CALL_TYPE_VOICE;

#endif


}

// ER07 - Logging Infrastructure - Set the log level
void acl_line_log_level_set(int level)
{
	SIPLogLevel = level;
}


static void voip_get_number_of_channels_per_line(int* pNumOfChPerLine)
{
	#define MAX_FXS_NUM	 8
	#define MAX_FXO_NUM  4

	/* AlexChen fixed for tulip2_40 and tulip2_21 */
	static int NumOfChannelsPerLine[MAX_FXO_NUM+1][MAX_FXS_NUM+1] = {
						/*0fxs*/ /*1fxs*/ /*2fxs*/ /*3fxs*/ /*4fxs*/ /*5fxs*/ /*6fxs*/ /*7fxs*/ /*8fxs*/
				/*0fxo*/{	0,		2,		 2,		  2,		2,		2,		 2,		   2,		2},
				/*1fxo*/{	0,		2,		 2,		  0,		1,		0,		 0,		   0,		0},
				/*2fxo*/{	0,		2,		 2,		  0,		1,		0,		 0,		   0,		0},
				/*3fxo*/{	0,		2,		 2,		  0,		1,		0,		 0,		   0,		0},
				/*4fxo*/{	0,		0,		 0,		  0,		2,		0,		 0,		   0,		0}
				};

	if ((MAX_FXS_NUM<giNumOfFXSL) || (MAX_FXO_NUM<giNumOfFXOL))
	{
		DBG_PRINT("************ ERROR invalid FXOS configuration ******** giNumOfFXSL=%d, giNumOfFXOL=%d \n", giNumOfFXSL, giNumOfFXOL);
		return;
	}

	*pNumOfChPerLine = NumOfChannelsPerLine[giNumOfFXOL][giNumOfFXSL];

	if(!(*pNumOfChPerLine))
	{
		DBG_PRINT("************ ERROR invalid FXOS configuration ******** giNumOfFXSL=%d, giNumOfFXOL=%d \n", giNumOfFXSL, giNumOfFXOL);
	}
}

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
#ifdef CONFIG_RG_VOIP_RV_SIP
void acl_line_refer_info_headers_str_set(acl_line_t *phoneConn,
                                           char* strReferToHeader,
                                           char* strReferredByHeader)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return;
    }

    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return;
        }
    }

    /*if a pointer is NULL imply clear the pointer*/
    phoneCall->refer_info.referToHdrBuf = strReferToHeader;
    phoneCall->refer_info.referredByHdrBuf = strReferredByHeader;
    /*  clear flag - bacause when you create you start a new story you definitely dont have any failure
        therefore clear any possible historicle failure*/
    phoneCall->refer_info.didReferFail = 0;

}

void acl_line_refer_info_headers_str_get(acl_line_t *phoneConn,
                                           char** pStrReferToHeader,
                                           char** pStrReferredByHeader)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return;
    }

    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return;
        }
    }

    *pStrReferToHeader     = phoneCall->refer_info.referToHdrBuf;
    *pStrReferredByHeader  = phoneCall->refer_info.referredByHdrBuf;

}

void acl_line_refer_info_failure_flag_set(acl_line_t *phoneConn, int val)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return;
    }
    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return;
        }
    }

    phoneCall->refer_info.didReferFail = val;
}

int acl_line_refer_info_failure_flag_get(acl_line_t *phoneConn)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return -1;
    }
    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return -1;
        }
    }

    return phoneCall->refer_info.didReferFail;
}

void acl_line_refer_info_unhold_flag_set(acl_line_t *phoneConn, int val)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return;
    }
    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return;
        }
    }

    phoneCall->refer_info.unholdInProgress = val;
}

int acl_line_refer_info_unhold_flag_get(acl_line_t *phoneConn)
{
    acl_call_t    *phoneCall  = NULL;

    if (phoneConn == NULL)
    {
        DBG_PRINT("phoneConn is NULL\n");
        return -1;
    }
    phoneCall = phoneConn->active_call;
    if (phoneCall == NULL)
    {
        DBG_PRINT("phoneCall is NULL\n");
        phoneCall = phoneConn->active_call1;
        if (phoneCall == NULL)
        {
            DBG_PRINT("phoneCall1 is NULL\n");
            return -1;
        }
    }

    return phoneCall->refer_info.unholdInProgress;
}

#endif /* CONFIG_RG_VOIP_RV_SIP */

acl_call_mngr_cbs_t call_mngr_cbs = {
    play_signal: play_signal_cb,
    start_signal: start_signal_cb,
    stop_signal: stop_signal_cb,
    update_display: display_update_cb,
    dialstring_is_complete: dialstring_is_complete_cb,
    dialstring_to_addr: dialstring_to_addr_map_cb,
    prepare_connection: line_connection_prepare_cb,
    create_connection: line_connection_create_cb,
    start_connection: line_connection_start_cb,
    stop_connection: line_connection_stop_cb,
    modify_connection: line_connection_modify_cb,
    destroy_connection: line_connection_destroy_cb,
    set_endpoint_context: line_connection_context_create,
    free_endpoint_context: line_connection_context_destruct,
    is_state: is_state_cb,
    enum_codecs: enum_codecs_cb,
#ifdef CONFIG_RG_ATA
    start_fax:fax_t38_start_cb,                   /* ACL NB 060404 */
    start_bypass:fax_bypass_start_cb,                /* ACL NB 061217 */
    start_fax_fallback:start_fax_fallback_cb,                /* ACL NB 061217 */
    stop_fax:stop_fax_cb,                   /* ACL NM 080403 */
#endif
    start_3wc:start_3wc_cb,                   /* ACL NB 060801 */
    set_call_origin:call_origin_set_cb,       /* ACL NB 060912 */
    activate_cfw:cfw_activate_cb,             /* ACL NB 070411 */
    adaptation_layer_send_general_command:adaptation_layer_general_command_send_cb,   /* NIRP 070610 VI 54097 */
    send_daa_commands:send_daa_commands_cb,       /* NIRP 070527 */
    onhook_event_handle:line_onhook_event_handle_cb,

#ifdef AC488_CPE_VOIP_TOOLKIT
    is_other_line_in_conference: is_other_line_in_conference_cb,
#endif /*AC488_CPE_VOIP_TOOLKIT*/
	is_during_fax_call:	is_during_fax_call_cb, /* AlexChen SDK upgrade 20090414 */
};
