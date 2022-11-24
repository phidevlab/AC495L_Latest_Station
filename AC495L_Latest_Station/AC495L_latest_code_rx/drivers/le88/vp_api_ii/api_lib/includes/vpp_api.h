/** \file vpp_api.h
 * vpp_api.h
 *
 * Header file for the API-II c files. This file contains all the declarations 
 * and defintions required to implement the VP-API library specific to VPP
 * class of devices.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VPP_API_H
#define VPP_API_H

#include "vp_api.h"

#define VPP_MAX_NUM_LINES		(2)		/* Maximum number of lines supported 
                                         * by VPP class of devices. */ 
/******************************************************************************
 *                                DEFINES  	                                  *
 ******************************************************************************/
#define VPP_RING_PROF_MAX_CNT		(2)	/* Number of entries in the software 
										 * ring profile table */
#define VPP_RINGCAD_PROF_MAX_CNT 	(VPP_RING_PROF_MAX_CNT)	/* Number of entries 
    * in the software ring profile table */

/******************************************************************************
 *                                ENUMERATIONS                                *
 ******************************************************************************/


/******************************************************************************
 *                                STRUCTURES	                              *
 ******************************************************************************/
/* VPP specific Device Object */
typedef struct {
	VpDeviceIdType deviceId;  /**< Device chip select ID defined by user */

	/* Static Information */
    uint8 maxChannels;	/**< How many lines supported by this device */

	/* VPP class of devices do not provide seperate Ring profile and ring 
	 * cadence profile. This presents a tricky situation for the API to provide 
	 * a consistant view across all platforms. For this reason, ring profile 
	 * present in the device is not used and seperate ring and cadence profiles 
	 * are stored in the device context itself. 
	 * NOTE: Pointers are being stored instead of the whole profile itself. So
	 *       make sure pointers are valid as long as their application is 
	 *       needed. */
	VpProfilePtrType pRingingProfileTable[VPP_RING_PROF_MAX_CNT];
	VpProfilePtrType pRingingCadProfileTable[VPP_RINGCAD_PROF_MAX_CNT];
	
	bool deviceInitOngoing;     /* Used to determine InitDevice Function 
	                             * is started */
	bool bootCompleteProcssing; /* Used to determine if further processing 
	                             * after boot is ongoing */

    uint16 recCnt;              /* Records remaining to be downloaded in the
                                 * VPP boot image.  Used by VpBootLoad(). */
} VpVppDeviceObjectType;

/* VPP specific Line Object */
typedef struct {
    uint8 channelId;		    /**< Channel # associated with "this" line 
                                 * on the device */
	VpTermType termType;	    /**< Termination type */

	/* Ring and Ring cadence profiles are simulated in the software */
	int8 activeRingPrfIndex;	/* Indicates the active ring profile 
	                             * (previously selected) */
	int8 activeRingCadPrfIndex;	/* Indicates the active ring cadence profile 
	                             * (previously selected) */

    VpLineIdType lineId;	/**< Application provided value for mapping a line to
                             * a line context
                             */
} VpVppLineObjectType;

#endif /* VPP_API_H */



