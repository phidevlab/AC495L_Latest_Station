/** \file dvp_api.h
 * dvp_api.h
 *
 * Header file for the API-II c files. This file contains all the declarations 
 * and defintions required to implement the DVP specific VP-API.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef DVP_API_H
#define DVP_API_H

#include "vp_api_types.h"
#include "vp_api_common.h"

#define VP_DVP_MAX_NUM_SLACS       8   /* Maximum Number of SLAC's 
                                        * controlled by the DVP */
#define VP_DVP_MAX_NUM_CHANS       4   /* Maximum Number of channels per SLAC */
#define VP_DVP_MAX_NUM_LINES     (VP_DVP_MAX_NUM_SLACS * VP_DVP_MAX_NUM_CHANS)

#define VP_TEST_BUF_MAX_SAMPLES ((4*1024) + 128)    /* 4K words + some extra */
/******************************************************************************
 *                                ENUMERATIONS                                *
 ******************************************************************************/
/* The following enumeration type is used to identify the DVP product type 
 * (firmware type). */
typedef enum {
    VP_DVP_PRODUCT_VE_790,          /* DVP is controlling Ve790 Series SLACs */
    VP_DVP_PRODUCT_VE_880,          /* DVP is controlling Ve880 Series SLACs */
    VP_DVP_PRODUCT_NUM_TYPES,
    VP_DVP_PRODUCT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDvpProductType;

/******************************************************************************
 *                                STRUCTURES                                  *
 ******************************************************************************/
/* DVP specific Device Object */
typedef struct {
    VpDeviceIdType deviceId;

    /* Static Information */
    uint8 maxChannels;  /**< How many lines supported by this device */

    /* This member is used to store detected SLAC information during 
     * Calibration */
    uint8 slacsFoundDuringCal;

    /* This flag indicates if device is initialized */
    bool deviceInitialized;

    /* Profile table status flags */
    uint16 devicePrfTblSts; 
    uint16 acPrfTblSts; 
    uint16 dcPrfTblSts;
    uint16 ringPrfTblSts;
    uint16 ringCadPrfTblSts;
    uint16 tonePrfTblSts;
    uint16 meterPrfTblSts;
    uint16 cidPrfTblSts;
    uint16 toneCadPrfTblSts;

    uint16 timeStampCntrHB;     /* Time stamp counter higher order bits */

    bool timeStampEventMask;    /* Time stamp rollover event mask; 
                                 * Needed since DVP API can't mask this event
                                 * because of software time stamp counter */

    uint16 recCnt;              /* Records remaining to be downloaded in the
                                 * DVP boot image.  Used by VpBootLoad(). */

#ifdef VP_CC_MULTI_THREAD_PROTECTION
    uint32  lineApiBusy;        /* This flag is used to detect re-entrant state
                                 * for each of the lines (Bit 0->line 0)
                                 * A bit set ==> API is busy for that line */
#endif /* VP_CC_MULTI_THREAD_PROTECTION */

    VpDvpProductType productType; /* Identifies the DVP firmware prouct type */

} VpDvpDeviceObjectType;

/* DVP specific Line Object */
typedef struct {
    uint8 channelId;

    VpTermType termType;

    bool calCompleteEventMask;  /* Calibration complete event is necessary
                                 * to be able to report device init complete 
                                 * event; So, do not make use of mask for this 
                                 * event in the DVP but make use of a software 
                                 * event mask */

    VpLineIdType lineId;	    /* Application provided value for mapping a
                                 * line to a line context */
} VpDvpLineObjectType;

#endif /* DVP_API_H */



