/** \file vp_api.h
 * vp_api.h
 *
 * Header file for the API-II c files.
 *
 * This file contains the all of the VoicePath API-II function prototypes. This 
 * file should be used to bring in the VP-API-II library modules that are 
 * necessary for a given application.
 *
 *****************************************************************************
 * NOTE: Inclusion of only this file is sufficient to bring in the all the	 *
 * necessary aspects of VP_API.												 *
 *****************************************************************************
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_H
#define VP_API_H

/******************************************************************************
 * VP-API Version                                                             *
 *****************************************************************************/
/*
 * The following version number tag is updated at every release of the VP-API.
 * Since the VP-API is a common interface that supports more than one device,
 * version number change might occur when any aspect of the VP-API gets
 * released.
 * The process is to determine the amount of change made since the release
 * shown (by your latest changes), and update the number accordingly. Then
 * release and update CVS. Then, CVS and everyone sync to it is at the same
 * release level.
 */
#define VP_API_VERSION_TAG (0x020800)

#define VP_API_VERSION_MAJOR_NUM ((VP_API_VERSION_TAG) >> 16)
#define VP_API_VERSION_MINOR_NUM (((VP_API_VERSION_TAG) & 0x00FF00) >> 8)
#define VP_API_VERSION_MINI_NUM (((VP_API_VERSION_TAG) & 0x0FF))

/* First include various basic data types used in the API */
#include "vp_api_types.h"

/******************************************************************************
 * Defines the configuration of VP-API library that needs to be built.        *
 * Please modify the following include file as per your VP-API library		  *
 * requirement(s).         								                      *
 ******************************************************************************/
#include "vp_api_cfg.h"

/******************************************************************************
 *                  PLEASE DO NOT MODIFY BELOW THIS LINE                      *
 ******************************************************************************/
/* Include the main VP-API-Common file */
#include "vp_api_common.h"

/*
** Defines for the specific LE88221 board 
*/
#undef VP_CC_790_SERIES
#undef VP_CC_580_SERIES
#undef VP_CC_DVP_SERIES
#undef VP_CC_VPP_SERIES
#define	VP_CC_880_SERIES
/*
** End of Defines for the specific LE88221 board 
*/


/* Include the necessary files depending on the requirement of the project */
#if defined (VP_CC_790_SERIES)
#include "vp790_api.h"    /* Vp790 device specific API functions and typedefs */
#endif

#if defined (VP_CC_880_SERIES)
#include "vp880_api.h"    /* Vp880 device specific API functions and typedefs */

/*
 * For 0823 LM, the FXO CID line is controlled by I/O3. For 0803 LM, it's
 * controlled by I/O2. Control with I/O3 is better since I/O2 has more features
 *
 *  #define VP880_FXO_CID_LINE      VP880_IODATA_IO2    Use with 0803 ref
 *  #define VP880_FXO_CID_LINE      VP880_IODATA_IO3    Use with 0823 ref
 */
#define VP880_FXO_CID_LINE      VP880_IODATA_IO3
#endif

#if defined (VP_CC_580_SERIES)
#include "vp580_api.h"    /* Vp580 device specific API functions and typedefs */
#endif

#if defined (VP_CC_DVP_SERIES)
#include "dvp_api.h"     /* DVP device specific API functions and typedefs */
#endif

#if defined (VP_CC_VPP_SERIES)
#include "vpp_api.h"     /* VPP device specific API functions and typedefs */
#endif


/* 
 * Based on the existing structure of the include files the following def
 * have been moved here.
 */
 
#if defined(VP_CC_880_SERIES) || defined(VP_CC_790_SERIES) 
typedef union {
    uint8 dummy; /* preventing an empty union */
#if defined(VP_CC_880_SERIES)
#ifdef VP880_INCLUDE_TESTLINE_CODE
    Vp880TestHeapType vpTestHeap880;
#endif  
#endif

#if defined (VP_CC_790_SERIES) 
    /* for future use 
    Vp790TestHeapType vpTestHeap790;
    */
#endif    
} VpTestHeapType;
#endif 
#endif /* VP_API_H */




