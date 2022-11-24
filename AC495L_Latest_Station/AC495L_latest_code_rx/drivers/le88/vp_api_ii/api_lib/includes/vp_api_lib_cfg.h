/** \file vp_api_lib_cfg.h
 * vp_api_lib_cfg.h
 *
 * This file contains the default options for various libraries. In general
 * the default options are same as top level API default options. However
 * VP-API provides a mechanism to define default options that is different
 * compared to top level default options. This file provides such a mechanism.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_LIB_CFG_H
#define VP_API_LIB_CFG_H

/******************************************************************************
 * Library Specific default options                                           *
 *****************************************************************************/
/* The following section provides mechanisms to define default options for 
 * individual VTD families that is different compared to VP-API top level 
 * default settins.
 *
 * NOTE: Users are free to change individual library's default settings as long
 * as it is not needed by the API and the VTD understands it.
 */

#ifdef VP_CC_DVP_SERIES
/* Default Option Settings for DVP */

/* Default pulse options */
#define DVP_OPTION_DEFAULT_DP_BREAK_MIN         VP_OPTION_DEFAULT_DP_BREAK_MIN    
#define DVP_OPTION_DEFAULT_DP_BREAK_MAX         VP_OPTION_DEFAULT_DP_BREAK_MAX    
#define DVP_OPTION_DEFAULT_DP_MAKE_MIN          VP_OPTION_DEFAULT_DP_MAKE_MIN     
#define DVP_OPTION_DEFAULT_DP_MAKE_MAX          VP_OPTION_DEFAULT_DP_MAKE_MAX     
#define DVP_OPTION_DEFAULT_DP_INTER_DIG_MIN     \
                                    VP_OPTION_DEFAULT_DP_INTER_DIG_MIN
#define DVP_OPTION_DEFAULT_DP_FLASH_MIN         VP_OPTION_DEFAULT_DP_FLASH_MIN    
#define DVP_OPTION_DEFAULT_DP_FLASH_MAX         VP_OPTION_DEFAULT_DP_FLASH_MAX    

/* Default setting for Critical fault option */
#define DVP_OPTION_DEFAULT_CF_AC_DIS_EN         VP_OPTION_DEFAULT_CF_AC_DIS_EN     
#define DVP_OPTION_DEFAULT_CF_DC_DIS_EN         VP_OPTION_DEFAULT_CF_DC_DIS_EN     
#define DVP_OPTION_DEFAULT_CF_THERMAL_DIS_EN    \
                                    VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN
#define DVP_OPTION_DEFAULT_CF_AC_DIS_EN_880     \
                                    VP_OPTION_DEFAULT_CF_AC_DIS_EN_880
#define DVP_OPTION_DEFAULT_CF_DC_DIS_EN_880     \
                                    VP_OPTION_DEFAULT_CF_DC_DIS_EN_880

/* Default setting for Zero cross option */
#define DVP_OPTION_DEFAULT_ZERO_CROSS           VP_OPTION_DEFAULT_ZERO_CROSS

/* Default setting for Ramp to standby option */
#define DVP_OPTION_DEFAULT_RAMP2STBY            VP_OPTION_DEFAULT_RAMP2STBY

/* Default setting for Pulse mode option */
#define DVP_OPTION_DEFAULT_PULSE_MODE           VP_OPTION_DEFAULT_PULSE_MODE

/* Default CODEC Mode */
#define DVP_OPTION_DEFAULT_CODEC_MODE           VP_OPTION_DEFAULT_CODEC_MODE

/* Default setting for PCM Highway option */
#define DVP_OPTION_DEFAULT_PCM_HWY              VP_OPTION_DEFAULT_PCM_HWY

/* Default setting for Loop back option */
#define DVP_OPTION_DEFAULT_LOOP_BACK            VP_OPTION_DEFAULT_LOOP_BACK

/* Default setting for Line state option */
#define DVP_OPTION_DEFAULT_LS_BAT_REV           VP_OPTION_DEFAULT_LS_BAT_REV
#define DVP_OPTION_DEFAULT_LS_BAT               VP_OPTION_DEFAULT_LS_BAT

/* Default Event Masks for DVP */
/* First, define the events that DVP does not understand or not needed */
#define DVP_EVCAT_FAULT_NOT_NEEDED \
        (VP_DEV_EVID_EVQ_OFL_FLT    | /* DVP does not understand */ \
         VP_DEV_EVID_WDT_FLT)        /* DVP does not understand */
/* Second, define the events that are specially needed for the DVP API */
#define DVP_EVCAT_FAULT_NEEDED (~0x0000) 

/* Third, Define the composite events */
#define DVP_OPTION_DEFAULT_FAULT_EVENT_MASK  \
    ((VP_OPTION_DEFAULT_FAULT_EVENT_MASK  |  /* Top level API default */ \
     DVP_EVCAT_FAULT_NOT_NEEDED) &         /* Events not needed for DVP*/ \
     DVP_EVCAT_FAULT_NEEDED)                /* Events needed for DVP */

/* First, define the events that DVP does not understand or not needed */
#define DVP_EVCAT_SIGNALLING_NOT_NEEDED \
        (VP_LINE_EVID_US_TONE_DETECT | /* DVP does not understand */  \
         VP_LINE_EVID_DS_TONE_DETECT | /* DVP does not understand */  \
         VP_DEV_EVID_SEQUENCER)       /* DVP does not understand */
/* Second, define the events that are specially needed for the DVP API */
#define DVP_EVCAT_SIGNALLING_NEEDED (~0x0000)

/* Third, Define the composite events */
#define DVP_OPTION_DEFAULT_SIGNALING_EVENT_MASK  \
    ((VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK  |  /* Top level API default */ \
     DVP_EVCAT_SIGNALLING_NOT_NEEDED) &         /* Events not needed for DVP*/ \
     DVP_EVCAT_SIGNALLING_NEEDED)                /* Events needed for DVP */

/* First, define the events that DVP does not understand or not needed */
#define DVP_EVCAT_RESPONSE_NOT_NEEDED \
        (VP_DEV_EVID_DNSTR_MBOX     | /* Seems redundant for DVP */ \
         VP_DEV_EVID_DEV_INIT_CMP ) /* DVP does not understand */
/* Second, define the events that are specially needed for the DVP API */
#define DVP_EVCAT_RESPONSE_NEEDED (~0x0000)

/* Third, Define the composite events */
#define DVP_OPTION_DEFAULT_RESPONSE_EVENT_MASK  \
    ((VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK  |  /* Top level API default */ \
     DVP_EVCAT_RESPONSE_NOT_NEEDED) &         /* Events not needed for DVP*/ \
     DVP_EVCAT_RESPONSE_NEEDED)                /* Events needed for DVP */

#define DVP_OPTION_DEFAULT_TEST_EVENT_MASK     VP_OPTION_DEFAULT_TEST_EVENT_MASK

#define DVP_OPTION_DEFAULT_PROCESS_EVENT_MASK   \
                                    VP_OPTION_DEFAULT_PROCESS_EVENT_MASK

#define DVP_OPTION_DEFAULT_FXO_EVENT_MASK    (0xffff)/* DVP does not 
                                                      * understand */

#define DVP_OPTION_DEFAULT_PACKET_EVENT_MASK (0xffff)/* DVP does not 
                                                      * understand */

/* Default setting for Ring Control option */
#define DVP_OPTION_DEFAULT_RC_ZERO_CROSS         VP_OPTION_DEFAULT_RC_ZERO_CROSS        
#define DVP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL \
                                    VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL
#define DVP_OPTION_DEFAULT_RC_RING_EXIT_STATE    \
                                    VP_OPTION_DEFAULT_RC_RING_EXIT_STATE   

/* Default setting for DTMF Mode option */
#define DVP_OPTION_DEFAULT_DM_DTMF_CONTROL_MODE  \
                                    VP_OPTION_DEFAULT_DM_DTMF_CONTROL_MODE
#define DVP_OPTION_DEFAULT_DM_DIRECTION          VP_OPTION_DEFAULT_DM_DIRECTION        

/* Default setting for PCM TX/RX path control */
#define DVP_OPTION_DEFAULT_PCM_TXRX_CNTRL VP_OPTION_DEFAULT_PCM_TXRX_CNTRL

#endif /* VP_CC_DVP_SERIES */


#endif /* VP_API_LIB_CFG_H */




