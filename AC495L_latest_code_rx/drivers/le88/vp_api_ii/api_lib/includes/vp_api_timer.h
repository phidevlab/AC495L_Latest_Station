/** \file vp_api_timer.h
 * vp_api_timer.h
 *
 * Header file for all timer typedefs used in API-II (internal).
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_TIMER_H
#define VP_API_TIMER_H

/* IMT average for Voltage to Frequency (FXO) smoothing */
#define VP_IMT_AVG_LENGTH 2  /* FXO variable, but need this to compile */

#define VP_ACTIVATE_LINE_TIMER  0x8000
#define VP_ACTIVATE_DEV_TIMER   0x8000
#define VP_TIMER_TIME_MASK      0x7FFF

typedef enum {
    VP_LINE_CID_DEBOUNCE,       /**< Debounce for known CID state changes */
    VP_LINE_TIMER_CID_DTMF,     /**< Timer for DTMF generation in CID */
    VP_LINE_TIMER_FAULT,        /**< The fault timer */
    VP_LINE_RING_EXIT_DEBOUNCE, /**< Line Debounce specifically for Ring Exit */    
    VP_LINE_POLREV_DEBOUNCE,    /**< Hook Mask on Polarity Reversals */
    VP_LINE_DISCONNECT_EXIT,    /**< Line Debounce for Disconnect Recovery */
    VP_LINE_TIMER_LAST,         /**< Place holder to indicate number of line timers */
    VP_LINE_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineTimerType;

 /**< Time after disconnect recovery for Hook mask */
#define VP_DISCONNECT_RECOVERY_TIME     100

typedef enum {
    VP_DEV_TIMER_TESTLINE,      /* Test line timer to ensure a more consistant
                                 * MPI PCM collect routine, leave this timer
                                 * as the first in the enumeration type */
    VP_DEV_TIMER_CLKFAIL,       /* Clock fail interrupt timer */
    VP_DEV_TIMER_LAST,          /* Place holder to indicate number of device timers */
    VP_DEV_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDevTimerType;

/** FXO specific timer variables to be used for FXO type lines only */
typedef enum {
    VP_CSLAC_FXS_TIMER,
    VP_CSLAC_FXO_TIMER,
    VP_CSLAC_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCslacTimerType;

typedef struct {
	uint16 highToLowTime;   /**< Device timestamp of last high to low change */
	uint16 prevHighToLowTime;   
    bool noCount;           /**< TRUE when not counting - lack of activity */
    bool lastState;         /**< TRUE if last known high, FALSE if low */
    uint16 timeLastPolRev;  /**< Time in 0.25ms since polrev detected */
    uint16 timePrevPolRev;  /**< Time in 0.25ms since prev polrev detected */
    uint8 maxPeriod;        /**< Time in 0.25ms that ringing is detected */
    uint8 lastStateChange;  /**< Time in 1mS since last state change */
    uint16 lastNotLiu;      /**< Time in 1mS since last Not LIU detected */
} VpFXOTimerType;

/** Union of FXO and FXS timers since only one type can exist for each line */
typedef union {
    VpFXOTimerType fxoTimer;
    uint16 timer[VP_LINE_TIMER_LAST]; /**< FXS Array of timers */
} VpCslacTimers;

typedef struct {
    VpCslacTimerType type;
    VpCslacTimers timers;
} VpCslacTimerStruct;

#endif



