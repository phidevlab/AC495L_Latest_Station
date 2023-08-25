/** \file boot_common.h
 * boot_common.h
 *
 * Header file for the VP-API-II c files.
 *
 * This file contains all of the VP-API-II declarations that are Voice 
 * Termination Device (VTD) family independent.	The implementation in this file
 * is applicable to DVP and VPP classes of devices.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef BOOT_COMMON_H
#define BOOT_COMMON_H

#include "vp_api_types.h"
#include "hbi_common.h"

/******************************************************************************
 *                        			FUNCTION PROTOTYPES                       *
 ******************************************************************************/
/*
 * Initialization functions
 */
VpStatusType 
BootLoadImage(
	VpDevCtxType *pDevCtx, 
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation);



#endif /* BOOT_COMMON_H */



