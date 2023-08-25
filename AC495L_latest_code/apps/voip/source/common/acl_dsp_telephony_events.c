/********************************************************************
 * AudioCodes improved application phone adaptation layer           *
 *                                                                  *
 * File Name:	acl_dsp_telephony_events.c                          *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 31/01/08 - Coding started.                                       *
 *																	*
 ********************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "voip_utils.h"
#include "acl_call_mngr.h"
#include "acl_dsp_telephony_events.h"
#include "acl_dsp_telephony_commands.h"
#include "acl_line_connection.h"
#include "acl_voip_conf.h"

#ifdef CONFIG_RG_IPPHONE
#include "acl_keypad_adaptation_layer.h"
#endif


/*#define AC_DSP_TELEPHONY_EVENTS_DEBUG*/

#ifdef AC_DSP_TELEPHONY_EVENTS_DEBUG
#include <ipplog.h>
/*071016 - EA did this change during work on VI#57767 in order to syncronize our printings with RV*/
//#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2);
#define DBG_PRINT(a1,a2...)		printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif



/*** Definitions, Enumerator and Macros ***/

#define FLASH_INPUT_DELAY_PERIOD_MS 	3000

typedef enum {
    INPUT_STRING_MATCH          = 0,
    INPUT_STRING_NO_MATCH       = 1,
    INPUT_STRING_PARTIAL_MATCH  = 2,
} input_string_match_result_t;


/***  Structure Definitions   ***/



/*** Global Parameters  ***/

static acl_line_t 	*g_lines[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];

static int			hookState[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];

/* Temporary ACL B 070620 */
//static int  call_parcking_en=0;



/*** prototypes declaration ***/
phone_input_event_t translate_acg_events_to_phone_input_events(acl_line_t *pCurrent_phone_line,
								acgTEventInfo *pAcgEvent, phone_event_t *pEvent);
#ifdef CONFIG_RG_ATA
static void flash_input_timeout_cb(void *data);
static void suppl_input_timeout_cb(void *data); /* ACL NB 061113 - call forward */
#endif

#if defined(AC488_CPE_VOIP_TOOLKIT)
static int ignore_phone_inputs(int line);	/* ignore phone inputs on conference line if using AC48802 template */
#endif /*AC488_CPE_VOIP_TOOLKIT*/

/*** General Functions ***/


acl_line_t *get_line(int line)
{
	if((line >= 0) && (line < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES))
	{
//		printf("FILE=%s, LINE=%d g_lines[%d] = 0x%08x\n", __FILE__, __LINE__,
//				line, g_lines[line]);

		return g_lines[line];
	}
	else
	{
		//printf("\n[%s:%d]  line %d is out of range\n", __FUNCTION__, __LINE__, line);
		return NULL;
	}
}

int dsp_select_mode(int n, int *hf)
{
//	acl_line_t *pCurrentLine = get_line(n);

	/* Need to be implemented */
	*hf = 0;
	return hookState[n];
}

int get_hook_state(int n)
{
	int dummy;

	return(dsp_select_mode(n, &dummy));
}

int set_hook_state(int n,int state)
{
	hookState[n]=state;
	return 0;
}



/****************************************************************
********												  ********
********	     Adaptation Layer Functions Implementation		  ********
********												  ********
*****************************************************************/



/******************************************************************************
*	Function:	 phone_input_init
*
*	Description:
*
*	Parameters:	 [I] acl_line_t *line
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
int phone_input_init(acl_line_t *line)
{
//	int keypad_fd;

   	DBG_PRINT(">>\r\n");

#ifdef CONFIG_RG_ATA
	line->last_flash_event = -1;
	line->flash_string_pos = 0;
#endif

	// by itzik - 13072006
	line->conference = 0;

	if(line->num < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES)
	{
		g_lines[line->num] = line;
	}
	else
	{
		printf("[%s:%d]  line %d is out of range\n", __FUNCTION__, __LINE__, line->num);
		return (-1);
	}

	DBG_PRINT("g_lines[%d] = 0x%08x\n", 	line->num, g_lines[line->num]);

#if 0
	if ((keypad_fd = keypad_open(line->num))<0)
		return -1;

	event_fd_set(keypad_fd, EFD_READ, phone_input_cb, line);
#endif

   	DBG_PRINT("<<\r\n");

	return 0;
}


/******************************************************************************
*	Function:	 phone_input_uninit
*
*	Description:
*
*	Parameters:	 [I] acl_line_t *line
*
*	Returns:
*
*******************************************************************************/
void phone_input_uninit(acl_line_t *line)
{
	if(line->num < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES)
	{
		g_lines[line->num] = NULL;
	}
	else
	{
		printf("[%s:%d]  line %d is out of range\n", __FUNCTION__, __LINE__, line->num);
		return;
	}

/*
	int keypad_fd = keypad_close(line->num);

	if (keypad_fd>=0)
		event_fd_clr(keypad_fd, EFD_READ);
*/
}

#if defined CONFIG_RG_ATA
/******************************************************************************
*	Function:	 end_flash_input_state
*
*	Description:
*
*	Parameters:	 [I] acl_line_t *line
*
*	Returns:
*
*******************************************************************************/
static void end_flash_input_state(acl_line_t *line)
{
   	DBG_PRINT(">>\r\n");

	/* Exits from flash-input state, without canceling the hold state
	 * of the corresponding phone-conn */

	if (!line->is_flash_input_state)
		return;

	if (AC_TIMER_EXISTS(flash_input_timeout_cb, line))
	{
		AC_TIMER_DEL(flash_input_timeout_cb, line);
	}

	line->is_flash_input_state = 0;

	if (line->active_call)
	{
		int dtmf_method = acl_line_get_dtmf_method(line->active_call);
		int rtp_id = acl_line_get_rtp_id(line->active_call);
		int codec = acl_line_get_active_codec(line->active_call);
		int ptime = acl_line_get_active_codec_ptime(line->active_call);

		if (dsp_voice_update(line->num, 0, codec,
			ptime, dtmf_method, rtp_id,-1))
		{
			DBG_PRINT("%s %d ERROR: dsp voice update failed\r\n", __FUNCTION__, __LINE__);
		}
	}

   	DBG_PRINT("<<\r\n");

}


/******************************************************************************
*	Function:	 cancel_flash_input_state
*
*	Description:
*
*	Parameters:	 [I] acl_line_t *line
*
*	Returns:
*
*******************************************************************************/
static void cancel_flash_input_state(acl_line_t *line)
{
   	DBG_PRINT(">>\r\n");

	if (!line->is_flash_input_state)
		return;

	end_flash_input_state(line);

	line->last_flash_event = -1;

   	DBG_PRINT("<<\r\n");
}


/******************************************************************************
*	Function:	 flash_input_timeout_cb
*
*	Description:
*
*	Parameters:	 [I] void *data
*
*	Returns:
*
*******************************************************************************/
static void flash_input_timeout_cb(void *data)
{
	phone_event_t event;
	acl_line_t *line;

   	DBG_PRINT(">>\r\n");

	/* After timeout, we get out of flash-input state */
	cancel_flash_input_state(data);

	line = (acl_line_t *)data;

	if ( line->blindTransferEnabled )
	{
		memset(&event, 0, sizeof(phone_event_t));
		event.pressed = 1; /*other structre's fields are not relevant*/
		acl_line_connection_input_event_handle(data, PHONE_INPUT_FLASH, &event);
	}

	DBG_PRINT("<<\r\n");

}


/******************************************************************************
*	Function:	 match_flash_string
*
*	Description:
*
*	Parameters:	 [I] acl_line_t *line
*				 [O] phone_input_event_t *out_event
*
*	Returns:
*
*******************************************************************************/
static input_string_match_result_t match_flash_string(acl_line_t *line,
    phone_input_event_t *out_event)
{
    static code2str_t flash_strs_events[] = {
	{PHONE_INPUT_HOLD, "1", 0, 0},
	{PHONE_INPUT_TRANSFER, "2",0 , 0},
   /*{PHONE_INPUT_BLINDT,   "98" , 0, 0  },*/

	/*
	 * commented out until comference is implemented for AudioCodes: 	*/
	// by itzik - 11072006 - 3wc
/*A.L - disable 3-way conf for WB version because it is only 2 channels firmware*/
#ifndef CONFIG_AC494_FIRMWARE_WB
	{PHONE_INPUT_CONF, "3", 0, 0},
#endif
	{-1, 0, 0, 0}
    };
    int i;
    phone_input_event_t event = *out_event;
    input_string_match_result_t res = INPUT_STRING_NO_MATCH;


   	DBG_PRINT(">>\r\n");

    /* Collect the new digit */
    line->flash_string[line->flash_string_pos++] =
	phone_input_get_event_desc(event)[0];
    if (line->flash_string_pos > MAX_KEY_SEQ_STRING_LEN)/* ACL NB 061210 changed MAX_FLASH_STRING_LEN to MAX_KEY_SEQ_STRING_LEN */
	goto Exit;
    line->flash_string[line->flash_string_pos] = 0;
    /* Compare the collected string with any one of the flash & digits
     * combinations strings */
    for (i=0; flash_strs_events[i].code!=-1; i++)
    {
	int j, match = 1;
	char *matched_str = flash_strs_events[i].str;

	for (j=0; matched_str[j] && j<line->flash_string_pos; j++)
	{
	    if (matched_str[j] != line->flash_string[j])
	    {
		match = 0;
		break;
	    }
	}
	if (match)
	{
	    /* No mismatch digits found */
	    if (!matched_str[j])
	    {
		/* If we reached the end of the matched string we have a
		 * complete match, so we assign the event associated with this
		 * digits combination */
		res = INPUT_STRING_MATCH;
		*out_event = flash_strs_events[i].code; /* faster than
		*out_event = str2code(flash_strs_events, flash_string); */
		break;
	    }
	    else
		res = INPUT_STRING_PARTIAL_MATCH;
	}
    }
Exit:
    if (res != INPUT_STRING_PARTIAL_MATCH)
	line->flash_string_pos = 0;

   	DBG_PRINT("<<\r\n");

    return res;
}



/******************************************************************************
*	Function:	 handle_flash_input
*
*	Description:
*
*	Parameters:	 [I] phone_input_event_t event
*				 [i] acl_line_t *line
*				 [i] int down
*
*	Returns: Enum phone_input_event_t
*
*******************************************************************************/
static phone_input_event_t handle_flash_input(phone_input_event_t event,
    acl_line_t *line, int down)
{
	phone_input_event_t ret_event = ILLEGAL_PHONE_INPUT;

   	DBG_PRINT(">>\r\n");

	DBG_PRINT("event %d\r\n", event);

	if (call_mgt->get_active_protocol() == VOIP_PROT_MGCP)
		return event;

	if (event == PHONE_INPUT_HOOK_ON)
	{
		/* In case of on-hook event we just cancel everything associated with
		 * the flash key, and continue with its normal handling */
		end_flash_input_state(line);
		line->last_flash_event = -1;
		ret_event = event;
	}
	else if (event == PHONE_INPUT_FLASH)
	{
       if (((line->keys_sequence == KEY_SEQ_FLASH_ONLY) && !line->blindTransferEnabled) ||  /* ACL NB 070516 - AC#53280 Send Hook-flash */
		   	(line->keys_sequence == KEY_SEQ_REMOTE_FLASH) ||
		   	(line->keys_sequence == KEY_SEQ_FLASH_FOR_MGCP) )
		{
			ret_event = event;
		}
		else
		{
			if (!line->is_flash_input_state)
			{
				if (line->last_flash_event == -1)
				{
					/* Get into the flash-input state only when in active call */
					if (acl_line_is_in_active_call(line))
					{
						/* start collecting the digits */
						AC_TIMER_SET(FLASH_INPUT_DELAY_PERIOD_MS, flash_input_timeout_cb, line);

						line->is_flash_input_state = 1;

						if (line->active_call)
						{
							int rtp_id = acl_line_get_rtp_id(line->active_call);
							int codec = acl_line_get_active_codec(line->active_call);
							int ptime = acl_line_get_active_codec_ptime(line->active_call);
							if (dsp_voice_update(line->num, 0, codec,
								ptime, 2/*IBS_TRANSFER_MODE__RELAY_DISABLE_VOICE_MUTE*/, rtp_id,-1))
							{
								DBG_PRINT("ERROR: dsp voice update failed\r\n");
							}
						}
					}
			}
				else
				{
					/* Every flash-event need to be pressed twice (at least this is
					 * the case now with hold/transfer/conference), so having a
					 * last_flash_event means that this should be the second press
					 * and we don't to wait for the digits: just flash key press */
					ret_event = line->last_flash_event;
					line->last_flash_event = -1;
				}
			}
			else /* pressing flash inside flash-input state, cancels the state */
				cancel_flash_input_state(line);
		}
	}

	else if (line->is_flash_input_state && event<=PHONE_INPUT_9 && !down)/* AlexChen SDK upgrade 20090415 - handle key when up*/
	{
		switch (match_flash_string(line, &event))
		{
			case INPUT_STRING_MATCH:
			    /* In case of match, we activate the corresponding event and store
			     * it for the next flash press */
			    ret_event = event;
			    line->last_flash_event = event;


				// by itzik - 17072006 - always use flash+key
			    // end_flash_input_state(line);
			    cancel_flash_input_state(line);

				/* XXX transfer/conference should * cancel hold here */
			    break;
			case INPUT_STRING_NO_MATCH:
			    cancel_flash_input_state(line);
			    break;
			case INPUT_STRING_PARTIAL_MATCH:
			    /* Restart the timeout timer for the next digit press */
				if (AC_TIMER_EXISTS(flash_input_timeout_cb, line))
				{
					AC_TIMER_DEL(flash_input_timeout_cb, line);
				}

			    AC_TIMER_SET(FLASH_INPUT_DELAY_PERIOD_MS, flash_input_timeout_cb, line);
			    break;
		}
	}
	else if (!line->is_flash_input_state)
		ret_event = event; /* deliver the input event normally */

	DBG_PRINT("ret_event %d\r\n", ret_event);

   	DBG_PRINT(">>\r\n");

	return ret_event;
}


/******************************************************************************
*	Function:	 end_suppl_input_state
*
*	Description:
*
*	Parameters:	 [i] acl_line_t *line
*
*	Returns:
*
*******************************************************************************/
static void end_suppl_input_state(acl_line_t *line)
{
   	DBG_PRINT(">>\r\n");

	/* Exits from suppl-input state */
	if (!line->is_suppl_input_state)
		return;

	if (AC_TIMER_EXISTS(suppl_input_timeout_cb, line))
	{
		AC_TIMER_DEL(suppl_input_timeout_cb, line);
	}

	line->is_suppl_input_state = 0;

	DBG_PRINT("--\r\n");

	if (line->active_call)
		rtp_mode_set(line->active_call, 0);

	if (line->active_call1)
		rtp_mode_set(line->active_call1, 0);

   	DBG_PRINT("<<\r\n");

}


/******************************************************************************
*	Function:	 cancel_suppl_input_state
*
*	Description:
*
*	Parameters:	 [i] acl_line_t *line
*
*	Returns:
*
*******************************************************************************/
static void cancel_suppl_input_state(acl_line_t *line)
{
   	DBG_PRINT(">>\r\n");

	if (!line->is_suppl_input_state)
		return;

	end_suppl_input_state(line);

	DBG_PRINT("<<\r\n");
}


/******************************************************************************
*	Function:	 suppl_input_timeout_cb
*
*	Description:
*
*	Parameters:	 [i] void *data
*
*	Returns:
*
*******************************************************************************/
static void suppl_input_timeout_cb(void *data)
{
	/* After timeout, we get out of flash-input state */
   	DBG_PRINT(">>\r\n");

	cancel_suppl_input_state(data);

   	DBG_PRINT("<<\r\n");
}



/******************************************************************************
*	Function:	 suppl_input_timeout_cb
*
*	Description:
*
*	Parameters:	 [i] acl_line_t *line
*				 [o] phone_input_event_t *out_event
*
*	Returns: Enum input_string_match_result_t
*
*******************************************************************************/
static input_string_match_result_t match_suppl_string(acl_line_t *line, phone_input_event_t *out_event)
{
	/* ACL NB 070507 */
	static code2str_t suppl_strs_events[] = {
		{PHONE_INPUT_CFW,     "dummy" , 0, 0},
		{PHONE_INPUT_BLINDT,   "98"  , 0, 0 },
		{-1, 0, 0, 0}
	};

	int i;
	int j, match = 1;

	/* end of ACL NB 070507 */
	phone_input_event_t event = *out_event;
	input_string_match_result_t res = INPUT_STRING_NO_MATCH;

   	DBG_PRINT(">>\r\n");

	/* Collect the new digit */
	line->suppl_string[line->suppl_string_pos++] = phone_input_get_event_desc(event)[0];

	if (line->suppl_string_pos > MAX_KEY_SEQ_STRING_LEN) /* ACL NB 061210 changed MAX_FLASH_STRING_LEN to MAX_KEY_SEQ_STRING_LEN */
		goto Exit;

	line->suppl_string[line->suppl_string_pos] = 0;
	/* Compare the collected string with any one of * & digits
	* combinations strings */
	char *matched_str = line->user_cfw_KeySeq;

	if (!matched_str[0])
		match = 0;

	for (j=0; matched_str[j] && j<line->suppl_string_pos; j++)
	{
		if (matched_str[j] != line->suppl_string[j])
		{
		   	match = 0;
		   	break;
		}
	}

    	DBG_PRINT("-- match = %d\r\n", match);

	/* ACL NB 070507 */
	if (match)
	{
		// NO CFW activation/deactivation while an active call
		if (acl_line_is_in_active_call(line))
		{
			res = INPUT_STRING_NO_MATCH;
		}
		else
		{
			 /* No mismatch digits found */
			 if (!matched_str[j])
			 {
				 /* If we reached the end of the matched string we have a
				 * complete match, so we assign the event associated with this
				    * digits combination */
				    res = INPUT_STRING_MATCH;
				    *out_event = PHONE_INPUT_CFW;
		 	}
		 	else
		    		res = INPUT_STRING_PARTIAL_MATCH;
		}
	}
	else
	{
      /* This loop is actually for feature use for now only blind transfer is handle through this */
		for (i=0; suppl_strs_events[i].code!=-1; i++)
		{
			 int j, match = 1;
			 char *matched_str = suppl_strs_events[i].str;

         /* ACL NB 071118 - VI58788 */
         if (( suppl_strs_events[i].code == PHONE_INPUT_BLINDT ) &&
             ( !line->blindTransferEnabled)                          )
         {
            DBG_PRINT("blind transfer is disabled\r\n");
            match = 0;
            break;
         }
			 for (j=0; matched_str[j] && j<line->suppl_string_pos; j++)
			 {
				if (matched_str[j] != line->suppl_string[j])
				{
				   	match = 0;
				   	break;
				}
			 }
			 /* end of ACL NB 070507 */
			 if (match)
			 {
				/* No mismatch digits found */
				if (!matched_str[j])
				{
					/* If we reached the end of the matched string we have a
					* complete match, so we assign the event associated with this
					   * digits combination */
					   res = INPUT_STRING_MATCH;
					   *out_event = suppl_strs_events[i].code;
					   break;
				}
				else
				   	res = INPUT_STRING_PARTIAL_MATCH;
			 }
		}
		/* AlexChen 20090407 SDK upgrade */
		if( !match )
		{
			char *matched_str = line->user_dnd_KeySeq;

			match=1;

			if (!matched_str[0])
				match = 0;

			for (j=0; matched_str[j] && j<line->suppl_string_pos; j++)
			{
				if (matched_str[j] != line->suppl_string[j])
				{
					match = 0;
					break;
				}
			}

			if( match )
			{
				if (acl_line_is_in_active_call(line))
				{
					res = INPUT_STRING_NO_MATCH;
				}
				else
				{
				/* No mismatch digits found */
					if (!matched_str[j])
					{
					/* If we reached the end of the matched string we have a
					* complete match, so we assign the event associated with this
					    * digits combination */
					    res = INPUT_STRING_MATCH;
					    *out_event = PHONE_INPUT_DND;
					}
					else
					    res = INPUT_STRING_PARTIAL_MATCH;
				}
			}
		}
	}
	Exit:
    DBG_PRINT("suppl_string = %s out_event=%d\r\n", line->suppl_string, *out_event);
	if (res != INPUT_STRING_PARTIAL_MATCH)
		line->suppl_string_pos = 0;
	DBG_PRINT("suppl_string = %s res=%d\r\n", line->suppl_string,res);

   	DBG_PRINT("<<\r\n");

	return res;

}



/******************************************************************************
*	Function:	 handle_suppl_input
*
*	Description:
*
*	Parameters:	 [i] phone_input_event_t 	event
*				 [i] acl_line_t 			*line
*				 [i] int 					down
*
*	Returns: Enum input_string_match_result_t
*
*******************************************************************************/
static phone_input_event_t handle_suppl_input(phone_input_event_t event, acl_line_t *line, int down)
{
    phone_input_event_t ret_event = ILLEGAL_PHONE_INPUT;

   	DBG_PRINT(">>\r\n");

	/* AlexChen SDK upgrade 20090415 handle key when up */
	if (down)
		return event;

    //console_printf("handle_suppl_input::Event %d-%s down=%d \r\n", event, phone_input_get_event_desc(event), down);
    /* ACL NB 071010 */
    /* using the down value is a little bit tricky and it enable to get the * if there is no match */
    if ( event == PHONE_INPUT_ASTERISK && !down )
    {
      if (!line->is_suppl_input_state)
	   {
 /* Eitan1207 - enable * relay during call except *98 for call park */
         /* Get into the service key state only when no active call */
		   if (acl_line_is_in_active_call(line)&& !line->blindTransferEnabled)
		   {
 		           /* ignore */
		   }
         else
         {
          	/* start collecting the digits */
		    AC_TIMER_SET(FLASH_INPUT_DELAY_PERIOD_MS, suppl_input_timeout_cb, line);
		    line->is_suppl_input_state = 1;

          		DBG_PRINT("--\r\n");

		    if (line->active_call)
   			rtp_mode_set(line->active_call, 1);

		    if (line->active_call1)
	   		rtp_mode_set(line->active_call1, 1);

			return ret_event; /* AlexChen SDK upgrade 20090415 handle key when up */
         }

	   }

    }

    if (line->is_suppl_input_state && event<=PHONE_INPUT_POUND)
    {
	    switch (match_suppl_string(line, &event))
       {
	      case INPUT_STRING_MATCH:
		  	DBG_PRINT("-- event = %d .\r\n", event);
         /* In case of match, we activate the corresponding event and store
         * it for the next time */
         if (event == PHONE_INPUT_CFW )
         {
            if ( line->is_cfw_on )
               line->is_cfw_on = 0;
            else
               line->is_cfw_on = 1;
         }
		 if (event == PHONE_INPUT_DAA_FLASH_HOOK_RELAY)/* NIRP 070607 */
		 {
		 	DBG_PRINT("Got PHONE_INPUT_DAA_FLASH_HOOK_RELAY .\r\n");
		 }
		/* In case of match, we activate the corresponding event and store
		* it for the next time  ACL AlexChen 20090407 */
		if (event == PHONE_INPUT_DND )
		{
			if ( line->is_dnd_activated )
			line->is_dnd_activated = 0;
			else
			line->is_dnd_activated = 1;

			line->is_dnd_on = 1;
		}
         cancel_suppl_input_state(line);
         break;

         case INPUT_STRING_NO_MATCH:
	      cancel_suppl_input_state(line);
	      break;

         case INPUT_STRING_PARTIAL_MATCH:
	       /* Restart the timeout timer for the next digit press */
		if (AC_TIMER_EXISTS(suppl_input_timeout_cb, line))
		{
	       		AC_TIMER_DEL(suppl_input_timeout_cb, line);
		}

	       AC_TIMER_SET(FLASH_INPUT_DELAY_PERIOD_MS, suppl_input_timeout_cb, line);
	      break;
      }

      ret_event = event;

    }
    else if (!line->is_suppl_input_state)
		ret_event = event; /* deliver the input event normally */

   	DBG_PRINT("<<\r\n");

    return ret_event;
}

#endif /*CONFIG_RG_ATA*/


/******************************************************************************
*	Function:	 phone_input_cb
*
*	Description:
*
*	Parameters:	 [i] acgTEventInfo 	*pAcgEvent
*
*	Returns:
*
*******************************************************************************/
/*NirS 081028 : new phone_input_cb */
void phone_input_cb(voip_event_info_t *voip_event_info, acl_line_t *line)
{
    switch(voip_event_info->phone_input_event)
   {
       case  DSP_PHONE_INPUT_EVENT:
       case  SLIC_PHONE_INPUT_EVENT:
       case  DAA_PHONE_INPUT_EVENT:
       {
          acgTEventInfo *pAcgEvent;
          pAcgEvent = (acgTEventInfo *)voip_event_info->info;
          dsp_slic_daa_phone_input_cb(pAcgEvent, line);
       }
        break;

       case  MISC_PHONE_INPUT_EVENT:
       {
          voip_misc_phone_event_info *pAcgMiscEvent;
          pAcgMiscEvent = (voip_misc_phone_event_info *)voip_event_info->info;
          misc_phone_input_cb(pAcgMiscEvent,line);
       }
        break;
	case KEYPAD_PHONE_INPUT_EVENT:
	{
		phone_event_t *pKeypadEvent;
		pKeypadEvent=(phone_event_t*)voip_event_info->info;
		keypad_phone_input_cb(pKeypadEvent, line);
	}

	break;

       default:
          printf("\nUnrecognized phone_input_event \n");
        break;

    }
}


void update_phone_input_event(phone_input_event_t key,acl_line_t *line)
{
	switch(key)
	{
		case PHONE_INPUT_HOOK_OFF:
			hookState[line->num]=OFF_HOOK;
			break;
		case PHONE_INPUT_HOOK_ON:
			hookState[line->num]=ON_HOOK;
			break;
		default:
			break;

	}
}

/******************************************************************************
*	Function:	keypad_phone_input_cb
*
*	Description:
*
*	Parameters:	 [i] phone_keypad_input_t* pAcgEvent
*
*	Returns:
*
*******************************************************************************/
void keypad_phone_input_cb(phone_event_t* pKeypadEvent,acl_line_t *line)
{
	update_phone_input_event(pKeypadEvent->key,line);
	acl_line_connection_input_event_handle(line, pKeypadEvent->key, pKeypadEvent);
}


/******************************************************************************
*	Function:	 dsp_slic_daa_phone_input_cb
*
*	Description:
*
*	Parameters:	 [i] acgTEventInfo 	*pAcgEvent
*
*	Returns:
*
*******************************************************************************/
void dsp_slic_daa_phone_input_cb(acgTEventInfo *pAcgEvent, acl_line_t *line)
{
	phone_event_t 		event;
	phone_input_event_t 	keyEvent=0;

   	DBG_PRINT(">>\r\n");

#ifdef AC488_CPE_VOIP_TOOLKIT
	if(ignore_phone_inputs(line->num))
		return;
#endif /*AC488_CPE_VOIP_TOOLKIT*/

	memset(&event, 0, sizeof(event));

	keyEvent = translate_acg_events_to_phone_input_events(line, pAcgEvent, &event);
	if(keyEvent == ILLEGAL_PHONE_INPUT)
		return;


        DBG_PRINT("keyEvent = %d\r\n", keyEvent);


#ifdef CONFIG_RG_ATA
	/* Note: we know that data here is actually phone_conn_t_ptr */
	/* XXX we may want to deliver a VOIP_INPUT_FLASH event to the the
	* signaling protocol (like in MGCP) */
	keyEvent = handle_flash_input(keyEvent, line, event.pressed);
	if(keyEvent == ILLEGAL_PHONE_INPUT)
		return;

        DBG_PRINT("\r\n");

	/* ACL Nb 061113 */
	keyEvent = handle_suppl_input(keyEvent, line, event.pressed);
	if(keyEvent == ILLEGAL_PHONE_INPUT)
		return;

	DBG_PRINT("\r\n");

#endif

        DBG_PRINT("\r\n");

	acl_line_connection_input_event_handle(line, keyEvent, &event);

   	DBG_PRINT("<<\r\n");

}


/******************************************************************************
*	Function:	 misc_phone_input_cb
*
*	Description: handle miscellaneous inputs

*	Parameters:
*
*	Returns:
*
*******************************************************************************/
void misc_phone_input_cb(voip_misc_phone_event_info *pAcgEvent, acl_line_t *line)
{
    phone_input_event_t     keyEvent=0;
    phone_event_t       event;

    printf("misc_phone_input_cb \r\n");
    switch(pAcgEvent->EventType)
    {
        case MISC_PHONE_EVENT_TYPE_STUN:
            keyEvent = PHONE_INPUT_STUN_RES;
            event.intData = pAcgEvent->EventInfo.Stun.Sfd;
            break;

        default:
            printf("\nUnrecognize EventType\n");
            return;
    }

    acl_line_connection_input_event_handle(line, keyEvent, &event);

}



/******************************************************************************
*	Function:	 phone_input_get_event_desc
*
*	Description:
*
*	Parameters:	 [i] phone_input_event_t 	input_event
*
*	Returns:	pointer to a null terminated string
*
*******************************************************************************/
char *phone_input_get_event_desc(phone_input_event_t input_event)
{
    switch (input_event)
    {
	    case PHONE_INPUT_0:
		return "0";
	    case PHONE_INPUT_1:
		return "1";
	    case PHONE_INPUT_2:
		return "2";
	    case PHONE_INPUT_3:
		return "3";
	    case PHONE_INPUT_4:
		return "4";
	    case PHONE_INPUT_5:
		return "5";
	    case PHONE_INPUT_6:
		return "6";
	    case PHONE_INPUT_7:
		return "7";
	    case PHONE_INPUT_8:
		return "8";
	    case PHONE_INPUT_9:
		return "9";
	    case PHONE_INPUT_ASTERISK:
		return "ASTERISK";
	    case PHONE_INPUT_POUND:
		return "POUND";
	/* Eitan1207 - Add support for DTMF A-D */
    case PHONE_INPUT_A:
	return "A";
    case PHONE_INPUT_B:
	return "B";
    case PHONE_INPUT_C:
	return "C";
    case PHONE_INPUT_D:
	return "D";
	    case PHONE_INPUT_HOOK_ON:
		return "HOOK ON";
	    case PHONE_INPUT_HOOK_OFF:
		return "HOOK OFF";
	    case PHONE_INPUT_SETTINGS:
		return "SETTINGS (internal)";
	    case PHONE_INPUT_HOLD:
		return "HOLD";
	    case PHONE_INPUT_TRANSFER:
		return "TRANSFER";
	    case PHONE_INPUT_CONF:
		return "CONFERENCE";
	    case PHONE_INPUT_FLASH:
		return "FLASH";
	    case PHONE_INPUT_CFW:/* ACL NB 061113 call forward*/
		return "CALL FORWARD";
	    case PHONE_INPUT_BLINDT:/* ACL NB 070507  Blind Transfer*/
		return "BLIND TRANSFER";
    	    case PHONE_INPUT_DAA_FLASH_HOOK_RELAY:/* NIRP 070607 */
		return "DAA FLASH HOOK RELAY";

	    default:
	       break;
    }

    return "Unknown input type";

}


phone_input_event_t translate_acg_events_to_phone_input_events(acl_line_t *line,
								acgTEventInfo *pAcgEvent, phone_event_t *pEvent)
{
  	int faxDataMethod;

   	DBG_PRINT(">>\r\n");

	pEvent->key = ILLEGAL_PHONE_INPUT; /* In case we don't want to deliver the event to upper layer */
	pEvent->pressed = 1;

	switch(pAcgEvent->EventType)
	{
		case ACG_EVENT_TYPE__ON_HOOK:

			/* Channel in SLIC event is the actual line */
			pEvent->key = PHONE_INPUT_HOOK_ON;
			hookState[line->num] = ON_HOOK;

			printf("\nACG_EVENT_TYPE__ON_HOOK on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__OFF_HOOK:

			/* Channel in SLIC event is the actual line */
			pEvent->key = PHONE_INPUT_HOOK_OFF;
			hookState[line->num] = OFF_HOOK;

			printf("\nACG_EVENT_TYPE__OFF_HOOK on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__FLASH_HOOK:

			/* Channel in SLIC event is the actual line */
			pEvent->key = PHONE_INPUT_FLASH;

			printf("\nACG_EVENT_TYPE__FLASH_HOOK on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__DTMF_START:

			if((pAcgEvent->EventInfo.Tone.Type == ACG_TONE_TYPE__DTMF) &&
				(pAcgEvent->EventInfo.Tone.Direction == ACG_DIRECTION__TO_LOCAL_TDM))
			{
				pEvent->key = (phone_input_event_t)pAcgEvent->EventInfo.Tone.ToneIndex;

				pEvent->pressed = 1;
			}

			break;

		case ACG_EVENT_TYPE__DTMF_END:

			if((pAcgEvent->EventInfo.Tone.Type == ACG_TONE_TYPE__DTMF) &&
				(pAcgEvent->EventInfo.Tone.Direction == ACG_DIRECTION__TO_LOCAL_TDM))
			{
				pEvent->key = (phone_input_event_t)pAcgEvent->EventInfo.Tone.ToneIndex;

				pEvent->pressed = 0;
			}

			break;

		case ACG_EVENT_TYPE__FAX_START:

			if(pAcgEvent->EventInfo.FaxModem.InitiatingFlag == ACG_INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_DSP)
			{
				voip_conf_faxDataMethod_get(&faxDataMethod);

				if((!line->active_call->isT38) &&
					((faxDataMethod & FAX_METHOD_MASK) == T38_TRANSPORT_RELAY ))  /*ACL NB 061214 the 4 LSBs are fax method */
				{
					pEvent->key = PHONE_INPUT_FAX_DETECT;
				}
			}

			printf("\nACG_EVENT_TYPE__FAX_START on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__FAX_OR_MODEM_START:

			if(pAcgEvent->EventInfo.FaxModem.InitiatingFlag == ACG_INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_DSP)
			{
				voip_conf_faxDataMethod_get(&faxDataMethod);

				if((!line->active_call->isVBD) &&
					((faxDataMethod & FAX_METHOD_MASK) == T38_VOICE_BAND_DATA ))  /*ACL NB 061214 the 4 LSBs are fax method */
				{
					pEvent->key = PHONE_INPUT_FAX_BYPASS_DETECT;
				}
			}

			printf("\nACG_EVENT_TYPE__FAX_OR_MODEM_START on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__FAX_OR_MODEM_END:

			pEvent->key = PHONE_INPUT_SWITCH2VOICE_DETECT;//need to stop fax
		#ifdef CONFIG_RG_ATA
			dsp_fax_stop(line->num, 0, ACG_FAX_MODEM_TRANSPORT__RELAY);
		#endif
			printf("\nACG_EVENT_TYPE__FAX_OR_MODEM_END on line %d\n", line->num);

			break;

		case ACG_EVENT_TYPE__MODEM_START:
			break;

		case ACG_EVENT_TYPE__CALL_PROGRESS_TONE_START:
			break;

		case ACG_EVENT_TYPE__CALL_PROGRESS_TONE_END:
			break;

		case ACG_EVENT_TYPE__CALLER_ID_END:
//			printf("\nACG_EVENT_TYPE__CALLER_ID_END on line %d\n", line->num);
#ifdef AC48_CPE_VOIP_TOOLKIT
			/*NirM, 082005, in the VP call progress tone must be generated after caller id was ended */
			dsp_play(line->num, VOIP_SIGNAL_CALLER_WAITING_TONE, 1);
#endif
			break;

		case ACG_EVENT_TYPE__RTCP_INFO:
/*
			printf("\nACG_EVENT_TYPE__RTCP_INFO on line %d\n", line->num);

			switch(pAcgEvent->EventInfo.RTCPInfo.RTCP.ReportType)
			{
				case ACG_RTCP_RR_Rx:
					printf("ReportType=ACG_RTCP_RR_Rx ");
					break;
				case ACG_RTCP_SR_Rx:
					printf("ReportType=ACG_RTCP_SR_Rx ");
					break;
				case ACG_RTCP_RR_Tx:
					printf("ReportType=ACG_RTCP_RR_Tx ");
					break;
				case ACG_RTCP_SR_Tx:
					printf("ReportType=ACG_RTCP_SR_Tx ");
					break;
				default:
					printf("ReportType unrecognized ");

					break;
			}

			printf("PacketCount=%d OctetCount=%d Jitter=%d RoundTrip=%d FractionLost=%2.1f%c CumLost=%d ExtHighSeq=%d SsrcSender=%d RtpTimeStamp=%d PacketsReceived=%d OctetsReceived=%d\n",
				pAcgEvent->EventInfo.RTCPInfo.RTCP.PacketCount, pAcgEvent->EventInfo.RTCPInfo.RTCP.OctetCount,
				pAcgEvent->EventInfo.RTCPInfo.RTCP.Jitter, pAcgEvent->EventInfo.RTCPInfo.RTCP.RoundTrip, pAcgEvent->EventInfo.RTCPInfo.RTCP.FractionLost/256.*100.,'%',
				pAcgEvent->EventInfo.RTCPInfo.RTCP.CumLost, pAcgEvent->EventInfo.RTCPInfo.RTCP.ExtHighSeq, pAcgEvent->EventInfo.RTCPInfo.RTCP.SsrcSender,
				pAcgEvent->EventInfo.RTCPInfo.RTCP.RtpTimeStamp, pAcgEvent->EventInfo.RTCPInfo.RTCP.PacketsReceived, pAcgEvent->EventInfo.RTCPInfo.RTCP.OctetsReceived);
*/
			break;

		case ACG_EVENT_TYPE__RTCP_XR_INFO:
/*
			printf("\nACG_EVENT_TYPE__RTCP_XR_INFO on line %d\n", line->num);

			switch(pAcgEvent->EventInfo.RTCPInfo.RTCP_XR.ReportType)
			{
				case ACG_RTCPXR_INFO:
					printf("ReportType=ACG_RTCP_XR_INFO ");
					break;
				case ACG_RTCPXR_REMOTE_INFO:
					printf("ReportType=ACG_RTCP_XR_REMOTE_INFO ");
					break;
				default:
					printf("ReportType unrecognized ");

					break;
			}

			printf("RFactor=%d MOS_CQ=%d MOS_LQ=%d\n",
				pAcgEvent->EventInfo.RTCPInfo.RTCP_XR.RFactor, pAcgEvent->EventInfo.RTCPInfo.RTCP_XR.MOS_CQ,
				pAcgEvent->EventInfo.RTCPInfo.RTCP_XR.MOS_LQ);
*/
			break;

		case ACG_EVENT_TYPE__RING_START:
                     pEvent->key = PHONE_INPUT_DAA_RING_START;
			break;

		case ACG_EVENT_TYPE__RING_END:
                    pEvent->key = PHONE_INPUT_DAA_RING_STOP;
			break;

		case ACG_EVENT_TYPE__POLARITY_REVERSAL:
			break;

		case ACG_EVENT_TYPE__NO_EVENT:

			pEvent->key = ILLEGAL_PHONE_INPUT;

			break;

		default:
			printf("\nUnrecognize EventType\n");
			break;
	}

   	DBG_PRINT("<<\r\n");

	return pEvent->key;

}

#ifdef AC488_CPE_VOIP_TOOLKIT
/* When using AC48802 template three way conference can only be
	achieved on one line, the conference line should be blocked */
static int ignore_phone_inputs(int line)
{
	acl_line_t *pAc488ConferenceLine = get_line(1 - line);	/* get the conference line */

	if (pAc488ConferenceLine == NULL) return 0; /* AlexChen */
	if(pAc488ConferenceLine->conference)
	{
		return 1;	/* inputs should be ignored */
	}
	else
	{
		return 0;
	}
}
#endif /*AC488_CPE_VOIP_TOOLKIT*/

