/** \file vp_api.c
 * vp_api.c
 *
 *  This file contains the implementation of top level VoicePath API-II.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */


/* INCLUDES */
#include "vp_api.h"     /* Typedefs and function prototypes for API */

#if defined (VP_CC_790_SERIES) || defined (VP_CC_880_SERIES) \
 || defined (VP_CC_580_SERIES) || defined (VP_CC_DVP_SERIES) \
 || defined (VP_CC_VPP_SERIES)

#include "vp_hal.h"
#include "vp_api_int.h" /* Device specific typedefs and function prototypes */

typedef void (*VpTempFuncPtrType) (void);

/* Helper function prototypes */
#ifdef VP_CC_MULTI_THREAD_PROTECTION
static VpStatusType VpApiFunctionEntry(VpLineCtxType *pLineCtx,
                                VpDevCtxType *pDevCtx, bool *pFuncNotBlcked);
static bool VpApiFunctionExit(VpLineCtxType *pLineCtx, VpDevCtxType *pDevCtx);
#endif /* VP_CC_MULTI_THREAD_PROTECTION */



/******************************************************************************
 *                     SYSTEM CONFIGURATION FUNCTIONS                         *
 ******************************************************************************/
/**
 * VpMakeDeviceObject()
 *  This function creates a device context using the information that is
 * provided. This funciton should be the first API function that should be
 * called. This function is like C++ constructor. If the passed device type is
 * not valid or the code for the device type is not compiled in, this function
 * returns error. Please see VP-API documentation for more information.
 *
 * Preconditions:
 *  The device context, device object pointers must be non zero and device type
 * must be valid. The type of device object should match with device type. The
 * deviceId must uniquely determine a chipselect for the device of interest in
 * HAL layer.
 *
 * Postconditions:
 *  The device context and device object are initialized and this function
 * returns success if context is created properly.
 */
VpStatusType
VpMakeDeviceObject(
    VpDeviceType deviceType,    /**< Device Type */
    VpDeviceIdType deviceId,    /**< Hardware chip select for this device */

    VpDevCtxType *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
    void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8 i;
    VpStatusType status;

    /* Pointer to a function pointer array (of same function type) */
    VpTempFuncPtrType *funcPtr;

    /* Basic error checking */
    if ((pDevObj == VP_NULL) || (pDevCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    if ((deviceType != VP_DEV_DVP_SERIES) && 
        (deviceType != VP_DEV_VPP_SERIES) &&
        (deviceType != VP_DEV_880_SERIES) &&
        (deviceType != VP_DEV_790_SERIES) &&
        (deviceType != VP_DEV_580_SERIES)) {
        return VP_STATUS_ERR_VTD_CODE;
    }        

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    
    if(pDevCtx != VP_NULL) {
        /* User wants to create device context as well; First clear it */
        /* Initialize All the funciton pointers to zero */
        funcPtr = (VpTempFuncPtrType *)&pDevCtx->funPtrsToApiFuncs;
        for(i = 0; i < (sizeof(ApiFunctions) / sizeof(VpTempFuncPtrType)); i++){
            *funcPtr = VP_NULL;
            funcPtr++;
        }

        /* Initialize all the line context pointers to null */
        for(i = 0; i < VP_MAX_LINES_PER_DEVICE; i++) {
            pDevCtx->pLineCtx[i] = VP_NULL;
        }
    }

    pDevCtx->deviceType = deviceType;
    
    switch (deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            ((VpDvpDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VpMakeDvpDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            ((VpVppDeviceObjectType *)pDevObj)->deviceId = deviceId;
            status = VpMakeVppDeviceObject(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            status = VpMakeVp880DeviceObject(pDevCtx, pDevObj);
            ((Vp880DeviceObjectType *)pDevObj)->deviceId = deviceId;
            ((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0] = 0;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            status = VpMakeVp790DeviceObject(pDevCtx, pDevObj);
            ((Vp790DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            status = VpMakeVp580DeviceObject(pDevCtx, pDevObj);
            ((Vp580DeviceObjectType *)pDevObj)->deviceId = deviceId;
            break;
#endif
        default:
            /* 
             * This error check should be performed pointers are initialized, so
             * this code should never be executed -- error checking above
             */
            status = VP_STATUS_ERR_VTD_CODE;
            break;
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* VpMakeDeviceObject() */

/**
 * VpMakeDeviceCtx()
 *  This function links a device object to a device context and initializes
 * the function pointers.  This function must be called if VpMakeDeviceObject
 * has not been called with a valid device context. This function enables more
 * than one device context to be created for a shared device object referring to
 * to a given device. This feature is useful when more than one process is
 * controlling a given device.
 *
 * Preconditions:
 *  The device context and device object pointers must be non zero. Device
 * Object must be created before calling this function.
 *
 * Postconditions:
 *  The device context and device object are linked and the device context
 * function pointers are initialized.
 */
VpStatusType
VpMakeDeviceCtx(
    VpDeviceType deviceType,    /**< Device Type - must match type in pDevObj */
    VpDevCtxType *pDevCtx,      /**< Device Context to be initialized by other
                                 * input
                                 */
    void *pDevObj)              /**< Device Object to be pointed to by device
                                 * context
                                 */
{
    uint8 i;
    VpDeviceIdType deviceId;    /**< Hardware chip select for this device */
    VpStatusType status;
    
    /* Pointer to a function pointer array (of same function type) */
    VpTempFuncPtrType *funcPtr;

    /* Basic error checking */
    if((pDevCtx == VP_NULL) || (pDevObj == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    switch (deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

        default:
            return VP_STATUS_ERR_VTD_CODE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* First Initialize all the funciton pointers to zero */
    funcPtr = (VpTempFuncPtrType *)&pDevCtx->funPtrsToApiFuncs;
    for(i = 0; i < (sizeof(ApiFunctions) / sizeof(VpTempFuncPtrType)); i++) {
        *funcPtr = VP_NULL;
        funcPtr++;
    }

    /* Initialize all the line context pointers to null */
    for(i = 0; i < VP_MAX_LINES_PER_DEVICE; i++) {
        pDevCtx->pLineCtx[i] = VP_NULL;
    }

    pDevCtx->deviceType = deviceType;

    switch (deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            status = VpMakeDvpDeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            status = VpMakeVppDeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            status = VpMakeVp880DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            status = VpMakeVp790DeviceCtx(pDevCtx, pDevObj);
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            status = VpMakeVp580DeviceCtx(pDevCtx, pDevObj);
            break;
#endif
        default:
            status = VP_STATUS_ERR_VTD_CODE;
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* VpMakeDeviceCtx() */

/**
 * VpMakeLineObject()
 *  This function initializes a line context using the information that is
 * passed. This function is like a C++ constructor. It initializes the passed
 * line context and line object based on the paramters provided. The passed line
 * object type should match with the type of device object type. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  This function assumes device context has already been created and
 * initialized. This function should only be called after downloading the boot
 * image the device when applicable (like for DVP and VPP class of devices).
 *
 * Postconditions:
 *  This function initializes the line context/line object. Line related VP-API
 * functions can be called after calling this function.
 */
VpStatusType
VpMakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx)
{
    VpDeviceIdType deviceId;    /**< Hardware chip select for this device */
    VpStatusType status;
    void *pDevObj = pDevCtx->pDevObj;

    /* Basic error checking */
    if((pLineObj == VP_NULL) || (pDevCtx == VP_NULL) || (pLineCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    switch (pDevCtx->deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            status = VpMakeDvpLineObject(termType, channelId, pLineCtx, pLineObj,
                pDevCtx);
            break;    
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            status = VpMakeVppLineObject(termType, channelId, pLineCtx, pLineObj,
                pDevCtx);
            break;    
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            status = VpMakeVp880LineObject(termType, channelId, pLineCtx, pLineObj,
                pDevCtx);
            break;    
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            status = VpMake790LineObject(termType, channelId, pLineCtx, pLineObj,
                pDevCtx);
            break;    
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            status = VpMakeVp580LineObject(termType, channelId, pLineCtx, pLineObj,
                pDevCtx);
            break;    
#endif

        default:
            return VP_STATUS_ERR_VTD_CODE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* VpMakeLineObject() */

/**
 * VpMakeLineCtx()
 *  This function makes the association from line context to line object and
 * device context. It must be called if VpMakeLineObject() is called with line
 * context set to VP_NULL. This function allows line objects and line contexts
 * to be created in separate steps. In multiprocess environment this function
 * allows associating more than one line contexts with one shared line object
 * for a given line.
 *
 * Preconditions:
 *  None of the arguments can be VP_NULL. Line object must be created before
 * calling this function.
 *
 * Postconditions:
 *  The line context is associated with the line object and device context.
 */
VpStatusType
VpMakeLineCtx(
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx)
{
    uint8 channelId;
    VpDeviceIdType deviceId;    /**< Hardware chip select for this device */
    void *pDevObj = pDevCtx->pDevObj;

    /* Basic error checking */
    if((pLineCtx == VP_NULL) || (pLineObj == VP_NULL) || (pDevCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    switch (pDevCtx->deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            channelId = ((VpDvpLineObjectType *)pLineObj)->channelId;
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            channelId = ((VpVppLineObjectType *)pLineObj)->channelId;
            deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            channelId = ((Vp580LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

        default:
            return VP_STATUS_ERR_VTD_CODE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * Make the device context member of the line context point to the
     * device context passed. This links this line context with a specific
     * device context.
     */
    pLineCtx->pDevCtx = pDevCtx;

    /*
     * Make the line object member of this line context point to the passed
     * line object pointer
     */
    pLineCtx->pLineObj = pLineObj;

    /*
     * Make the indexed line context array in the device context point to the
     * line context passed.
     */
    pDevCtx->pLineCtx[channelId] = pLineCtx;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* VpMakeLineCtx() */

/**
 * VpFreeLineCtx()
 *  This function frees the association from line context to device context. It
 * must be called if the application is freeing the memory associated with the
 * line context or object.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  All areas of the API-II where this line context may be used is set to
 * VP_NULL.
 */
VpStatusType
VpFreeLineCtx(
    VpLineCtxType *pLineCtx)
{
    uint8 channelId;
    VpDevCtxType *pDevCtx;
    void *pLineObj, *pDevObj;
    VpDeviceIdType deviceId;    /**< Hardware chip select for this device */

    if (pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    pDevObj = pDevCtx->pDevObj;
    pLineObj = pLineCtx->pLineObj;

    /*
     * Get the channel ID in the device context that is associated with this
     * line context.
     */
     
    switch(pDevCtx->deviceType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            channelId = ((VpDvpLineObjectType *)pLineObj)->channelId;
            deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            channelId = ((VpVppLineObjectType *)pLineObj)->channelId;
            deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            channelId = ((Vp580LineObjectType *)pLineObj)->channelId;
            deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Free the line context of this device */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevCtx->pLineCtx[channelId] = VP_NULL;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* VpFreeLineCtx() */

/*
 * VpGetDeviceInfo()
 * This function is used to obtain information about a device. Please see VP-API
 * documentation for more information.
 *
 * Preconditions:
 * This function assumes the passed line/device contexts are created and
 * initialized.
 *
 * Postconditions:
 * Returns the requested information.
 */
VpStatusType
VpGetDeviceInfo(
    VpDeviceInfoType *pDeviceInfo)
{
    VpStatusType retVal = VP_STATUS_SUCCESS;
    VpLineCtxType *pLineCtx;
    VpDevCtxType *pDevCtx;
    VpDeviceType devType;
    void *pDevObj;

    /* Basic error checking */
    if(pDeviceInfo == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    pLineCtx = pDeviceInfo->pLineCtx;
    pDevCtx = pDeviceInfo->pDevCtx;

    if((pLineCtx == VP_NULL) && (pDevCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Extract device info from...*/
    if(pLineCtx != VP_NULL) {
        /* Line context */
        pDevCtx = pLineCtx->pDevCtx;
        pDeviceInfo->pDevCtx = pDevCtx;
    } else {
        /* Device context */
        pDevCtx = pDeviceInfo->pDevCtx;
        pDeviceInfo->pLineCtx = VP_NULL;
    }

    pDevObj = pDevCtx->pDevObj;
    devType = pDevCtx->deviceType;
    pDeviceInfo->deviceType = devType;
    pDeviceInfo->revCode = 0;

    switch(devType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            pDeviceInfo->numLines =
                ((VpDvpDeviceObjectType *)pDevObj)->maxChannels;
            pDeviceInfo->deviceId =
                ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
            pDeviceInfo->numLines =
                ((VpVppDeviceObjectType *)pDevObj)->maxChannels;
            pDeviceInfo->deviceId =
                ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pDeviceInfo->numLines =
                ((Vp880DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pDeviceInfo->revCode =
                ((Vp880DeviceObjectType *)pDevObj)->staticInfo.rcnPcn[0];
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDeviceInfo->numLines =
                ((Vp790DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pDeviceInfo->numLines =
                ((Vp580DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
            pDeviceInfo->deviceId =
                ((Vp580DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

        default:
            pDeviceInfo->numLines = 0;
            retVal = VP_STATUS_FAILURE;
            break;
    }

    return retVal;
} /* VpGetDeviceInfo() */

/**
 * VpGetLineInfo()
 *  This function is used to obtain information about a line. Please see VP-API
 * documentation for more information.
 *
 * Preconditions:
 *  This function assumes the line context and device context are already
 * created.
 *
 * Postconditions:
 *  Returns the requested information.
 */
VpStatusType
VpGetLineInfo(
    VpLineInfoType *pLineInfo)
{
    VpDevCtxType *pDevCtx;
    VpLineCtxType *pLineCtx;
    VpDeviceType deviceType;
    uint8 maxChan, channelId;
    void *pLineObj, *pDevObj;

    if (pLineInfo == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If the device context is not known, get it from the line context if that
     * is known.
     *
     * If the device context is known, and the line context is not known, we can
     * extract the line context from the device context, type, and channel ID.
     *
     * If the line context and device context are either both known, or both not
     * known, this function returns error
     */
    if ((pLineInfo->pDevCtx == VP_NULL) && (pLineInfo->pLineCtx != VP_NULL)) {
        pLineCtx = pLineInfo->pLineCtx;
        pDevCtx = pLineCtx->pDevCtx;
        pLineInfo->pDevCtx = pDevCtx;
        deviceType = pDevCtx->deviceType;
        pLineObj = pLineCtx->pLineObj;

        switch(deviceType) {
#if defined (VP_CC_880_SERIES)
            case VP_DEV_880_SERIES:
                pLineInfo->channelId =
                    ((Vp880LineObjectType *)pLineObj)->channelId;
                pLineInfo->termType =
                    ((Vp880LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp880LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_880_SERIES */

#if defined (VP_CC_790_SERIES)
            case VP_DEV_790_SERIES:
                pLineInfo->channelId =
                    ((Vp790LineObjectType *)pLineObj)->channelId;
                pLineInfo->termType =
                    ((Vp790LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp790LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_790_SERIES */

#if defined (VP_CC_580_SERIES)
            case VP_DEV_580_SERIES:
                pLineInfo->channelId =
                    ((Vp580LineObjectType *)pLineObj)->channelId;
                pLineInfo->termType =
                    ((Vp580LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp580LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_580_SERIES */

#if defined (VP_CC_DVP_SERIES)
            case VP_DEV_DVP_SERIES:
                pLineInfo->channelId =
                    ((VpDvpLineObjectType *)pLineObj)->channelId;
                pLineInfo->termType =
                    ((VpDvpLineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((VpDvpLineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_DVP_SERIES */

#if defined (VP_CC_VPP_SERIES)
            case VP_DEV_VPP_SERIES:
                pLineInfo->channelId =
                    ((VpVppLineObjectType *)pLineObj)->channelId;
                pLineInfo->termType =
                    ((VpVppLineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((VpVppLineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_VPP_SERIES */
            default:
                return VP_STATUS_INVALID_ARG;
        }
    } else if (pLineInfo->pDevCtx != VP_NULL) {
        pDevCtx = pLineInfo->pDevCtx;
        deviceType = pDevCtx->deviceType;
        channelId = pLineInfo->channelId;
        pDevObj = pDevCtx->pDevObj;

        switch(deviceType) {
#if defined (VP_CC_790_SERIES)
            case VP_DEV_790_SERIES:
                maxChan =
                    ((Vp790DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
                if(channelId >= maxChan) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineInfo->pLineCtx = pLineCtx;
                pLineInfo->termType =
                    ((Vp790LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp790LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_790_SERIES */

#if defined (VP_CC_880_SERIES)
            case VP_DEV_880_SERIES:
                maxChan =
                    ((Vp880DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
                if(channelId >= maxChan) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineInfo->pLineCtx = pLineCtx;
                pLineInfo->termType =
                    ((Vp880LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp880LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_880_SERIES */

#if defined (VP_CC_580_SERIES)
            case VP_DEV_580_SERIES:
                maxChan =
                    ((Vp580DeviceObjectType *)pDevObj)->staticInfo.maxChannels;
                if(channelId >= maxChan) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineInfo->pLineCtx = pLineCtx;
                pLineInfo->termType =
                    ((Vp580LineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((Vp580LineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_580_SERIES */

#if defined (VP_CC_DVP_SERIES)
            case VP_DEV_DVP_SERIES:
                maxChan =
                    ((VpDvpDeviceObjectType *)pDevObj)->maxChannels;
                if(channelId >= maxChan) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineInfo->pLineCtx = pLineCtx;
                pLineInfo->termType =
                    ((VpDvpLineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((VpDvpLineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_DVP_SERIES */

#if defined (VP_CC_VPP_SERIES)
            case VP_DEV_VPP_SERIES:
                maxChan = ((VpVppDeviceObjectType *)pDevObj)->maxChannels;
                if(channelId >= maxChan) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx == VP_NULL) {
                    return VP_STATUS_INVALID_ARG;
                }
                pLineObj = pLineCtx->pLineObj;
                pLineInfo->pLineCtx = pLineCtx;
                pLineInfo->termType =
                    ((VpVppLineObjectType *)pLineObj)->termType;
                pLineInfo->lineId =
                    ((VpVppLineObjectType *)pLineObj)->lineId;
                break;
#endif /* VP_CC_VPP_SERIES */
            default:
                return VP_STATUS_INVALID_ARG;
        }
    } else {
        return VP_STATUS_INVALID_ARG;
    }
    return VP_STATUS_SUCCESS;
} /* VpGetLineInfo() */

/******************************************************************************
 *                        INITIALIZATION FUNCTIONS                            *
 ******************************************************************************/
/*
 * VpBootLoad()
 * This function is used to download the boot image to the device.
 * This function must be called upon power on reset for DVP and VPP classes
 * of devices to download boot image for these devices. This function is
 * not applicable for CSLAC class of devices. See VP-API-II documentation
 * for more information about this function.
 *
 * Preconditions:
 * Device context must be created before calling this function.
 *
 * Postconditions:
 * This function downloads the boot image to the part and configures the part
 * to start excecuting the image that was downloaded.
 */
VpStatusType
VpBootLoad(
    VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation)
{
    VpBootLoadFuncPtrType bootLoadFuncPtr = pDevCtx->funPtrsToApiFuncs.BootLoad;
    if (bootLoadFuncPtr == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return bootLoadFuncPtr(pDevCtx, state, pImageBuffer, bufferSize,
            pScratchMem, validation);
    }
} /* VpBootLoad() */

/*
 * VpInitDevice()
 *  This function calibrates the device and initializes all lines (for which
 * line context has been created and intialized) on the device with the AC, DC,
 * and Ringing parameters passed. See VP-API reference guide for more
 * information.
 *
 * Preconditions:
 *  This function should be called only after creating and initializing the
 * device context and line context (atleast for those lines which need service).
 *
 * Postconditions:
 *  Device is calibrated and all lines (for whom line context has been created
 * and initialized) associated with this device are initialized with the AC, DC,
 * and Ringing Paramaters passed (DC and Ringing apply to FXS type lines only).
 */
VpStatusType
VpInitDevice(
    VpDevCtxType *pDevCtx,          /**< Pointer to device context for the
                                     * device that will be initialized
                                     */
    VpProfilePtrType pDevProfile,   /**< Pointer to Device Profile */
    VpProfilePtrType pAcProfile,    /**< Pointer to AC Profile that is applied
                                     * to all FXS lines on this device
                                     */
    VpProfilePtrType pDcProfile,    /**< Pointer to DC Profile that is applied
                                     * to all FXS lines on this device
                                     */
    VpProfilePtrType pRingProfile,  /**< Pointer to Ringing Profile that is
                                     * applied to all FXS lines on this device
                                     */
    VpProfilePtrType pFxoAcProfile, /**< Pointer to AC Profile that is applied
                                     * to all FXO lines on this device
                                     */
    VpProfilePtrType pFxoCfgProfile)/**< Pointer to Config Profile that is
                                     * applied to all FXO lines on this device
                                     */
{
    VpInitDeviceFuncPtrType initDeviceFuncPtr;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
         return VP_STATUS_INVALID_ARG;
    }

    initDeviceFuncPtr = pDevCtx->funPtrsToApiFuncs.InitDevice;
    if (initDeviceFuncPtr == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return initDeviceFuncPtr(pDevCtx, pDevProfile, pAcProfile, pDcProfile,
            pRingProfile, pFxoAcProfile, pFxoCfgProfile);
    }
} /* VpInitDevice() */

/**
 * VpInitLine()
 *  This function initializes the line with the AC, DC, and Ringing parameters
 * passed. It resets the line, loads the default options, performs calibration
 * (if applicable) and it also results in an event being generated.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized also device must
 * be initialized (via VpInitDevice).
 *
 * Postconditions:
 *  The line is initialized with the AC, DC, and Ringing parameters passed.
 * DC and Ringing parameters apply to FXS lines only.
 */
VpStatusType
VpInitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile)
{
    VpInitLineFuncPtrType InitLine;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    InitLine = pDevCtx->funPtrsToApiFuncs.InitLine;

    if (InitLine == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitLine(pLineCtx, pAcProfile, pDcFeedOrFxoCfgProfile,
            pRingProfile);
    }
} /* VpInitLine() */

/**
 * VpConfigLine()
 *  This function initializes the line with the AC, DC, and Ringing parameters
 * passed. See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized also device must
 * be initialized (via VpInitDevice).
 *
 * Postconditions:
 *  The line is initialized with the AC, DC, and Ringing parameters passed.
 * DC and Ringing parameters apply to FXS lines only.
 */
VpStatusType
VpConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile)
{
    VpConfigLineFuncPtrType ConfigLine;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ConfigLine = pDevCtx->funPtrsToApiFuncs.ConfigLine;

    if (ConfigLine == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ConfigLine(pLineCtx, pAcProfile, pDcFeedOrFxoCfgProfile,
            pRingProfile);
    }
} /* VpConfigLine() */

/**
 * VpCalCodec()
 *  This function initiates a calibration operation for analog circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
VpCalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode)
{
    VpCalCodecFuncPtrType CalCodec;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    if((mode != VP_DEV_CAL_NOW) && (mode != VP_DEV_CAL_NBUSY)) {
        return VP_STATUS_INVALID_ARG;
    }

    CalCodec = pDevCtx->funPtrsToApiFuncs.CalCodec;

    if (CalCodec == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return CalCodec(pLineCtx, mode);
    }
} /* VpCalCodec() */


/**
 * VpCalLine()
 *  This function initiates a calibration operation for analog circuits
 * associated with a given line. See VP-API reference guide for more
 * information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
VpCalLine(
    VpLineCtxType *pLineCtx)
{
    VpCalLineFuncPtrType CalLine;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    CalLine = pDevCtx->funPtrsToApiFuncs.CalLine;

    if (CalLine == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return CalLine(pLineCtx);
    }
} /* VpCalLine() */

/**
 * VpInitRing()
 *  This function is used to initialize Ringing and CID profile to a given line.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The line context and device context should be created initialized. The boot
 * image should be downloaded before calling this function (for applicable
 * devices).
 *
 * Postconditions:
 *  Applies the Caller ID and Ringing profile.
 */
VpStatusType
VpInitRing(
    VpLineCtxType *pLineCtx,        /**< Line context */
    VpProfilePtrType pCadProfile,   /**< Ringing cadence profile */
    VpProfilePtrType pCidProfile)   /**< Caller ID profile */
{
    VpInitRingFuncPtrType InitRing;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    InitRing = pDevCtx->funPtrsToApiFuncs.InitRing;

    if (InitRing == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitRing(pLineCtx, pCadProfile, pCidProfile);
    }
} /* VpInitRing() */

/**
 * VpInitCid()
 *  This function is used to send caller ID information. See VP-API reference
 * guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. This function needs to be called before placing the line in to
 * ringing state.
 *
 * Postconditions:
 *  This function transmits the given CID information on the line (when the line
 * is placed in the ringing state).
 */
VpStatusType
VpInitCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData)
{
    VpInitCidFuncPtrType InitCid;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    InitCid = pDevCtx->funPtrsToApiFuncs.InitCid;

    if (InitCid == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitCid(pLineCtx, length, pCidData);
    }
} /* VpInitCid() */

/**
 * VpInitMeter()
 *  This function is used to initialize metering parameters. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. This function needs to be called before placing the line in to
 * ringing state.
 *
 * Postconditions:
 *  This function initializes metering parameters as per given profile.
 */
VpStatusType
VpInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile)
{
    VpInitMeterFuncPtrType InitMeter;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    /* If metering profile is null, there is nothing to do. */
    if (pMeterProfile == VP_NULL) {
        return VP_STATUS_SUCCESS;
    }

    InitMeter = pDevCtx->funPtrsToApiFuncs.InitMeter;

    if (InitMeter == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitMeter(pLineCtx, pMeterProfile);
    }
} /* VpInitMeter() */

/**
 * VpInitCustomTerm()
 *  This function is used to initialize the I/O control for a custom termination
 * type. 
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function. It is required to be called for term type CUSTOM before calling
 * VpInitDevice()/VpInitLine().
 *
 * Postconditions:
 *  This function initializes I/O control parameters as per given profile. When
 * called with device context only, all custom lines on the device are affected
 * by the profile passed. When a line context only is passed, then only that
 * line context is affected.
 */
VpStatusType
VpInitCustomTermType (
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile)
{
    VpInitCustomTermTypeFuncPtrType InitCustomTerm = VP_NULL;
    VpDevCtxType *pDevCtxLocal = VP_NULL;
    
    /* Basic error checking */
    if((pLineCtx == VP_NULL) && (pDevCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    if((pLineCtx != VP_NULL) && (pDevCtx == VP_NULL)) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        InitCustomTerm = pDevCtxLocal->funPtrsToApiFuncs.InitCustomTerm;
    } else if ((pDevCtx != VP_NULL) && (pLineCtx == VP_NULL)) {
        InitCustomTerm = pDevCtx->funPtrsToApiFuncs.InitCustomTerm;
    } else {
        return VP_STATUS_INVALID_ARG;
    }
    
    if (InitCustomTerm == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitCustomTerm(pDevCtx, pLineCtx, pCustomTermProfile);
    }
    return VP_STATUS_SUCCESS;
} /* VpInitCustomTermType() */

/**
 * VpInitProfile()
 *  This function is used to initialize profile tables in DVP and VPP. See
 * VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Stores the given profile at the specified index of the profile table.
 */
VpStatusType
VpInitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile)
{
    VpInitProfileFuncPtrType InitProfile;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    InitProfile = pDevCtx->funPtrsToApiFuncs.InitProfile;

    if (InitProfile == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return InitProfile(pDevCtx, type, pProfileIndex, pProfile);
    }
} /* VpInitProfile() */

/**
 * VpSoftReset()
 *  This function resets DVP or VPP without requiring another image load.
 * See VP-API reference guide for more information.
 *
 * Preconditions:
 *  The device and must be created and initialized before calling this function.
 *
 * Postconditions:
 *  The part is reset.
 */
VpStatusType
VpSoftReset(
    VpDevCtxType *pDevCtx)
{
    VpSoftResetFuncPtrType SoftReset;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    SoftReset = pDevCtx->funPtrsToApiFuncs.SoftReset;

    if (SoftReset == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SoftReset(pDevCtx);
    }
} /* VpSoftReset() */

/******************************************************************************
 *                        CONTROL FUNCTIONS                                   *
 ******************************************************************************/
/**
 * VpSetLineState()
 *  This function sets a given line to indicated state. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The indicated line is set to indicated line state.
 */
VpStatusType
VpSetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    VpSetLineStateFuncPtrType SetLineState;
    VpDevCtxType *pDevCtx;
#ifdef VP_CC_MULTI_THREAD_PROTECTION
    bool lineApiNotBusy;
    VpStatusType status;
#endif /* VP_CC_MULTI_THREAD_PROTECTION */

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SetLineState = pDevCtx->funPtrsToApiFuncs.SetLineState;

    if (SetLineState == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
#ifdef VP_CC_MULTI_THREAD_PROTECTION
        status = VpApiFunctionEntry(pLineCtx, VP_NULL, &lineApiNotBusy);
        if(status == VP_STATUS_SUCCESS) {
            if(lineApiNotBusy == TRUE) {
                status = SetLineState(pLineCtx, state);
                if(VpApiFunctionExit(pLineCtx, VP_NULL) == TRUE) {
                    return status;
                } else {
                    return VP_STATUS_FAILURE; /* Some internal error */
                }
            } else {
                return VP_STATUS_IN_CRTCL_SECTN;
            }
        } else {
            return status;
        }
#else
        return SetLineState(pLineCtx, state);
#endif /* VP_CC_MULTI_THREAD_PROTECTION */
    }
} /* VpSetLineState() */


/**
 * VpSetLineTone()
 *  This function is used to set a tone on a given line. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Starts/Stops tone generation for a given line.
 */
VpStatusType
VpSetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl)
{
    VpSetLineToneFuncPtrType SetLineTone;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SetLineTone = pDevCtx->funPtrsToApiFuncs.SetLineTone;

    if (SetLineTone == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SetLineTone(pLineCtx, pToneProfile, pCadProfile, pDtmfControl);
    }
} /* VpSetLineTone() */

/**
 * VpSetRelayState()
 *  This function controls the state of VTD controlled relays. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The indicated relay state is set for the given line.
 */
VpStatusType
VpSetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState)
{
    VpSetRelayStateFuncPtrType SetRelayState;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SetRelayState = pDevCtx->funPtrsToApiFuncs.SetRelayState;

    if (SetRelayState == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SetRelayState(pLineCtx, rState);
    }
} /* VpSetRelayState() */

/**
 * VpSetRelGain()
 *  This function adjusts the transmit and receive path relative gains. See
 * VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The requested relative gains will be applied.
 */
VpStatusType
VpSetRelGain(
    VpLineCtxType *pLineCtx,
    uint16 txLevel,
    uint16 rxLevel,
    uint16 handle)
{
    VpSetRelGainFuncPtrType SetRelGain;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SetRelGain = pDevCtx->funPtrsToApiFuncs.SetRelGain;

    if (SetRelGain == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SetRelGain(pLineCtx, txLevel, rxLevel, handle);
    }
} /* VpSetRelGain() */

/**
 * VpSendSignal()
 *  This function is used to send a signal on a line. The signal type is
 * specified by the type parameter and the parameters associated with the signal
 * type are specified by the structure pointer passed.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Applies a signal to the line.
 */
VpStatusType
VpSendSignal(
    VpLineCtxType *pLineCtx,
    VpSendSignalType signalType,
    void *pSignalData)
{
    VpSendSignalFuncPtrType SendSignal;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SendSignal = pDevCtx->funPtrsToApiFuncs.SendSignal;

    if (SendSignal == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SendSignal(pLineCtx, signalType, pSignalData);
    }
} /* VpSendSignal() */

/**
 * VpSendCid()
 *  This function may be used to send Caller ID information on-demand. See
 * VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 * Caller ID information is transmitted on the line.
 */
VpStatusType
VpSendCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    VpProfilePtrType pCidProfile,
    uint8p pCidData)
{
    VpSendCidFuncPtrType SendCid;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    SendCid = pDevCtx->funPtrsToApiFuncs.SendCid;

    if (SendCid == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SendCid(pLineCtx, length, pCidProfile, pCidData);
    }
} /* VpSendCid() */

/**
 * VpContinueCid()
 *  This function is called to provide more caller ID data (in response to
 * Caller ID data event from the VP-API). See VP-API-II  documentation
 * for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Continues to transmit Caller ID information on the line.
 */
VpStatusType
VpContinueCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData)
{
    VpContinueCidFuncPtrType ContinueCid;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ContinueCid = pDevCtx->funPtrsToApiFuncs.ContinueCid;

    if (ContinueCid == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ContinueCid(pLineCtx, length, pCidData);
    }
} /* VpContinueCid() */


/**
 * VpStartMeter()
 *  This function starts(can also abort) metering pulses on the line. See
 * VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Metering pulses are transmitted on the line.
 */
VpStatusType
VpStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters)
{
    VpStartMeterFuncPtrType StartMeter;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    StartMeter = pDevCtx->funPtrsToApiFuncs.StartMeter;

    if (StartMeter == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return StartMeter(pLineCtx, onTime, offTime, numMeters);
    }
} /* VpStartMeter() */

/**
 * VpSetOption()
 *  This function is used to set various options that are supported by VP-API.
 * For a detailed description of how this function can be used to set device,
 * line specific, device specific options and to various types of options
 * please see VP-API user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  This function sets the requested option.
 */
VpStatusType
VpSetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    VpSetOptionFuncPtrType SetOption;
    VpDevCtxType *pDevCtxLocal;

    /* Basic error checking */
    if((pLineCtx == VP_NULL) && (pDevCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }
    if((pLineCtx != VP_NULL) && (pDevCtx != VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    if(pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        SetOption = pDevCtxLocal->funPtrsToApiFuncs.SetOption;
    } else {
        SetOption = pDevCtx->funPtrsToApiFuncs.SetOption;
    }

    if (SetOption == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SetOption(pLineCtx,
            pDevCtx, option, value);
    }
} /* VpSetOption() */

/**
 * VpToneDetectionControl()
 *  This function controls the tone detection in VTD. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Configures the tone detection algorithms(blocks) as requested.
 */
VpStatusType
VpToneDetectionControl(
    VpLineCtxType *pLineCtx,
    VpToneDetectionType *pToneDetection)
{
    VpToneDetectionControlFuncPtrType ToneDetectionControl;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ToneDetectionControl = pDevCtx->funPtrsToApiFuncs.ToneDetectionControl;

    if (ToneDetectionControl == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ToneDetectionControl(pLineCtx, pToneDetection);
    }
} /* VpToneDetectionControl() */

/**
 * VpDeviceIoAccess()
 *  This function is used to access device IO pins of the VTD. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Reads/Writes from device IO pins.
 */
VpStatusType
VpDeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData)
{
    VpDeviceIoAccessFuncPtrType DeviceIoAccess;

    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    DeviceIoAccess = pDevCtx->funPtrsToApiFuncs.DeviceIoAccess;

    if (DeviceIoAccess == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return DeviceIoAccess(pDevCtx, pDeviceIoData);
    }
} /* VpDeviceIoAccess() */

/**
 * VpVirtualISR()
 *  This function should be called from the interrupt context for CSLAC devices
 * upon a CSLAC device interrupt. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Updates API internal variables to indicate an interrupt has occured.
 */
VpStatusType
VpVirtualISR(
    VpDevCtxType *pDevCtx)
{
    VpVirtualISRFuncPtrType VirtualISR;

    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    VirtualISR = pDevCtx->funPtrsToApiFuncs.VirtualISR;

    if(VirtualISR == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return VirtualISR(pDevCtx);
    }
} /* VpVirtualISR() */

/**
 * VpApiTick()
 *  This function should be called at regular intervals of time for CSLAC
 * devices. This function call is used to update timing related aspects of
 * VP-API. See VP-API-II documentation for more information.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  Updates necessary internal variables.
 */
VpStatusType
VpApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus)
{
    VpApiTickFuncPtrType ApiTick;

    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    ApiTick = pDevCtx->funPtrsToApiFuncs.ApiTick;

    if (ApiTick == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ApiTick(pDevCtx, pEventStatus);
    }
} /* VpApiTick() */

/**
 * VpLowLevelCmd()
 *  This function provides a by-pass mechanism for the VP-API. THE USE
 * OF THIS FUNCTION BY THE APPLICATION CODE IS STRONGLY DISCOURAGED. THIS
 * FUNCTION CALL BREAKS THE SYNCHRONIZATION BETWEEN THE VP-API AND THE
 * VTD. See VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Performs the indicated low-level command access.
 */
VpStatusType
VpLowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle)
{
    VpLowLevelCmdFuncPtrType LowLevelCmd;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    LowLevelCmd = pDevCtx->funPtrsToApiFuncs.LowLevelCmd;

    if (LowLevelCmd == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return LowLevelCmd(pLineCtx, pCmdData, len, handle);
    }
} /* VpLowLevelCmd() */

/******************************************************************************
 *                        STATUS AND QUERY FUNCTIONS                          *
 ******************************************************************************/
/**
 * VpGetEvent()
 *  This function is used to get a event from a given device. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  This function can be called upon detecting an interrupt from interrupt
 * context or this function could be called to poll events. This function
 * assumes passed device context has been initialized.
 *
 * Postconditions:
 *  Returns TRUE is there is a new event. Information about this event is
 * filled in to the event pointer (if not zero). If either of its arguments is
 * zero this function returns FALSE.
 *
 */
bool
VpGetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent)
{
    VpGetEventFuncPtrType GetEvent;

    /* Basic error checking */
    if((pDevCtx == VP_NULL) || (pEvent == VP_NULL)) {
        return FALSE;
    }

    GetEvent = pDevCtx->funPtrsToApiFuncs.GetEvent;

    if (GetEvent == VP_NULL) {
        return FALSE;
    } else {
        return GetEvent(pDevCtx, pEvent);
    }
} /* VpGetEvent() */

/**
 * VpGetLineStatus()
 *  This function is used to obtain status of a given line with respect to a
 * certain type of line condition. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 * Returns status information about the line condition that was asked for.
 */
VpStatusType
VpGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus)
{
    VpGetLineStatusFuncPtrType GetLineStatus;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    GetLineStatus = pDevCtx->funPtrsToApiFuncs.GetLineStatus;

    if (GetLineStatus == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetLineStatus(pLineCtx, input, pStatus);
    }
} /* VpGetLineStatus() */

/**
 * VpGetDeviceStatus()
 *  This function is used to obtain status of all lines with respect to a
 * certain type of line condition. See VP-API-II documentation for more
 * information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Returns status information about the line condition that was asked for.
 */
VpStatusType
VpGetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus)
{
    VpGetDeviceStatusFuncPtrType GetDeviceStatus;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    GetDeviceStatus = pDevCtx->funPtrsToApiFuncs.GetDeviceStatus;

    if (GetDeviceStatus == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetDeviceStatus(pDevCtx, input, pDeviceStatus);
    }
} /* VpGetDeviceStatus() */

/**
 * VpGetLoopCond()
 *  This function is used to obtain the telephone loop condition for a given
 * line. See VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  This function starts the necessary actions to obtain the loop condition.
 */
VpStatusType
VpGetLoopCond(
    VpLineCtxType *pLineCtx,
    uint16 handle)
{
    VpGetLoopCondFuncPtrType GetLoopCond;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    GetLoopCond = pDevCtx->funPtrsToApiFuncs.GetLoopCond;

    if (GetLoopCond == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetLoopCond(pLineCtx, handle);
    }
} /* VpGetLoopCond() */

/**
 * VpGetOption()
 *  This function is used to read an option. See VP-API-II documentation for
 * more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Starts the necessary action to be able to read the requested option.
 */
VpStatusType
VpGetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle)
{
    VpGetOptionFuncPtrType GetOption;
    VpDevCtxType *pDevCtxLocal;

    /* Basic error checking */
    if((pLineCtx == VP_NULL) &&  (pDevCtx == VP_NULL)){
        return VP_STATUS_INVALID_ARG;
    }
    if((pLineCtx != VP_NULL) &&  (pDevCtx != VP_NULL)){
        return VP_STATUS_INVALID_ARG;
    }

    if (pDevCtx != VP_NULL) {
        GetOption = pDevCtx->funPtrsToApiFuncs.GetOption;
    } else {
        pDevCtxLocal = pLineCtx->pDevCtx;
        GetOption = pDevCtxLocal->funPtrsToApiFuncs.GetOption;
    }

    if (GetOption == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetOption(pLineCtx,
            pDevCtx, option, handle);
    }
} /* VpGetOption() */

VpStatusType
VpGetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState)
{
    void *pLineObj = VP_NULL;
    VpDevCtxType *pDevCtx;

#if defined (VP_CC_DVP_SERIES)
    VpGetLineStateFuncPtrType  GetLineState;
#endif

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pLineObj = pLineCtx->pLineObj;
        pDevCtx = pLineCtx->pDevCtx;
    }

    switch(pDevCtx->deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            *pCurrentState =
                ((Vp790LineObjectType *)pLineObj)->lineState.usrCurrent;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            *pCurrentState =
                ((Vp880LineObjectType *)pLineObj)->lineState.usrCurrent;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            *pCurrentState =
                ((Vp580LineObjectType *)pLineObj)->lineState.usrCurrent;
            break;
#endif

#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
            GetLineState = pDevCtx->funPtrsToApiFuncs.GetLineState;
            if(GetLineState == VP_NULL) {
                return VP_STATUS_FUNC_NOT_SUPPORTED;
            } else {
                return GetLineState(pLineCtx, pCurrentState);
            }
#endif
        default:
            return VP_STATUS_FUNC_NOT_SUPPORTED;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * VpFlushEvents()
 *  This function is used to flush all outstanding events from the VTD's event
 * queue. See VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Clears all outstanding events from event queue.
 */
VpStatusType
VpFlushEvents(
    VpDevCtxType *pDevCtx)
{
    VpFlushEventsFuncPtrType FlushEvents;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    FlushEvents = pDevCtx->funPtrsToApiFuncs.FlushEvents;

    if (FlushEvents == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return FlushEvents(pDevCtx);
    }
} /* VpFlushEvents() */

/**
 * VpGetResults()
 *  This function retrives the result based on a previous operation. For more
 * information see VP-API-II user guide.
 *
 * Preconditions:
 *  This function can be called upon detecting an event for which there could
 * be associated data. The results pointer must provide enough storage space
 * the type of result that is anticipated.
 *
 * Postconditions:
 *  It fills in the results based on the event to location provided by results
 * pointer. This function returns an error if either of its arguments is zero.
 */
VpStatusType
VpGetResults(
    VpEventType *pEvent,
    void *pResults)
{
    VpGetResultsFuncPtrType GetResults;

    /* Basic error checking */
    if(pEvent == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    GetResults = pEvent->pDevCtx->funPtrsToApiFuncs.GetResults;

    if (GetResults == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetResults(pEvent, pResults);
    }
} /* VpGetResults() */

/**
 * VpClearResults()
 *  This function is used to clear results associated with an event.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Clears results entry (only one at the top of the results queue) that is
 * outstanding and waiting to be read.
 */
VpStatusType
VpClearResults(
    VpDevCtxType *pDevCtx)
{
    VpClearResultsFuncPtrType ClearResults;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    ClearResults = pDevCtx->funPtrsToApiFuncs.ClearResults;

    if (ClearResults == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ClearResults(pDevCtx);
    }
} /* VpClearResults() */

/**
 * VpDtmfDigitDetected()
 *  This function is used to set a value in the API-II indicating that a DTMF
 * digit was detected in an external application/process.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  A value in the API-II is set which indicates the digit detected. The most
 * recent value is stored.
 */
VpStatusType
VpDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense)
{
    VpDtmfDigitDetectedFuncPtrType DtmfDigitDetected;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    if (VpIsDigit(digit) == FALSE) {
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtx = pLineCtx->pDevCtx;
    DtmfDigitDetected = pDevCtx->funPtrsToApiFuncs.DtmfDigitDetected;

    if (DtmfDigitDetected == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return DtmfDigitDetected(pLineCtx, digit, sense);
    }
}

/******************************************************************************
 *                        PACKET INTERFACE FUNCTIONS                          *
 ******************************************************************************/
/**
 * VpControlVoiceStream()
 *  This function controls the 3 way conference start/stop. See VP-API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. Two phones have
 *  been talking. The 3rd phone can be added / removed from the party.
 *
 * Postconditions:
 *  Start or Stop the 3 way conference as requested.
 */
VpStatusType
VpControlVoiceStream(
    VpLineCtxType *pLineCtx,
    uint8 streamId,
    VpVSConfControlType streamControl, /* Start, Stop the conference */
    VpVSConfModeType confMode)         /* Conference Mode */
{
    VpControlVoiceStreamFuncPtrType ControlVoiceStream;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ControlVoiceStream = pDevCtx->funPtrsToApiFuncs.ControlVoiceStream;

    if (ControlVoiceStream == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ControlVoiceStream(pLineCtx, streamId, streamControl, confMode);
    }
} /* VpControlVoiceStream() */

/**
 * VpReadUpStreamPacket()
 *  This function is used to read upstream (transmit) packet. For more
 * information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Reads the packet that is to be transmitted (Once read, the same packet is
 * no longer available to be read again).
 */
VpStatusType
VpReadUpStreamPacket(
    VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId)
{
    VpReadUpStreamPacketFuncPtrType ReadUpStreamPacket;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if((pLineCtx == VP_NULL) || (pPacketData == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ReadUpStreamPacket = pDevCtx->funPtrsToApiFuncs.ReadUpStreamPacket;

    if (ReadUpStreamPacket == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
         return ReadUpStreamPacket((struct VpLineCtxType *)pLineCtx,
                                    pPacketData, streamId);
    }
} /* VpReadUpStreamPacket() */

/**
 * VpWriteDownStreamPacket()
 *  This function is used to write a downstream (or receive) packet. For more
 * information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 * Writes the downstream packet to be played out. Note that only one packet
 * could be written during one packet interval for a given channel.
 */
VpStatusType
VpWriteDownStreamPacket(
    VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId)
{
    VpWriteDownStreamPacketFuncPtrType WriteDownStreamPacket;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    WriteDownStreamPacket = pDevCtx->funPtrsToApiFuncs.WriteDownStreamPacket;

    if (WriteDownStreamPacket == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return WriteDownStreamPacket(pLineCtx,
            pPacketData, streamId);
    }
} /* VpWriteDownStreamPacket() */

/**
 * VpGetTimeStamp()
 *  This function is used to obtain timestamp from DVP. For more information see
 * VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 * Reads timestamp information from DVP.
 */
VpStatusType
VpGetTimeStamp(
    VpDevCtxType *pDevCtx,
    uint16 *pTimeStamp)
{
    VpGetTimeStampFuncPtrType GetTimeStamp;

    /* Basic error checking */
    if((pDevCtx == VP_NULL) || (pTimeStamp == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    GetTimeStamp = pDevCtx->funPtrsToApiFuncs.GetTimeStamp;

    if (GetTimeStamp == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return GetTimeStamp(pDevCtx, pTimeStamp);
    }
} /* VpGetTimeStamp() */

/**
 * VpReadPacketStatistics()
 *  This function is used to obtain packet statistics. For more information see
 * VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Initiates action to read packet statistics.
 */
VpStatusType
VpReadPacketStatistics(
    VpLineCtxType *pLineCtx,
    uint16 handle)
{
    VpReadPacketStatisticsFuncPtrType ReadPacketStatistics;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    ReadPacketStatistics =  pDevCtx->funPtrsToApiFuncs.ReadPacketStatistics;

    if (ReadPacketStatistics == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return ReadPacketStatistics(pLineCtx, handle);
    }
} /* VpReadPacketStatistics() */

/******************************************************************************
 *                            TEST FUNCTIONS                                  *
 ******************************************************************************/
/**
 * VpTestLine()
 *  This function performs the requested test. For more information see
 * VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Starts/Stops requested test actions.
 */
VpStatusType
VpTestLine(
    VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle)
{
    VpTestLineFuncPtrType TestLine;
    VpDevCtxType *pDevCtx;    

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    TestLine = pDevCtx->funPtrsToApiFuncs.TestLine;

    if (TestLine == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return TestLine(pLineCtx, test, pArgs, handle);
    }
} /* VpTestLine() */

/**
 * VpCodeCheckSum()
 *  This function performs a checksum calculation over the loaded code (for
 * program and data memory). For more information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Initiates a checksum verification action.
 */
VpStatusType
VpCodeCheckSum(
    VpDevCtxType *pDevCtx,
    uint16 handle)
{
    VpCodeCheckSumFuncPtrType CodeCheckSum;

    /* Basic error checking */
    if(pDevCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    CodeCheckSum = pDevCtx->funPtrsToApiFuncs.CodeCheckSum;

    if (CodeCheckSum == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return CodeCheckSum(pDevCtx, handle);
    }
} /* VpCodeCheckSum() */

/**
 * VpSelfTest()
 *  This function performs a self test on a given line or for all lines in the
 * device. For more information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Initiates self test.
 */
VpStatusType
VpSelfTest(
    VpLineCtxType *pLineCtx)
{
    VpSelfTestFuncPtrType SelfTest;
    VpDevCtxType *pDevCtxLocal;

    /* Basic error checking */
    if(pLineCtx == VP_NULL){
        return VP_STATUS_INVALID_ARG;
    }

    pDevCtxLocal = pLineCtx->pDevCtx;
    SelfTest = pDevCtxLocal->funPtrsToApiFuncs.SelfTest;

    if (SelfTest == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return SelfTest(pLineCtx);
    }
} /* VpSelfTest() */

/**
 * VpFillTestBuf()
 *  This function is used to load test vector data in to the device internal
 * memory. For more information see VP-API-II user guide.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Loads the given test data in to the device.
 */
VpStatusType
VpFillTestBuf(
    VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData)
{
    VpFillTestBufFuncPtrType FillTestBuf;
    VpDevCtxType *pDevCtx;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    } else {
        pDevCtx = pLineCtx->pDevCtx;
    }

    FillTestBuf = pDevCtx->funPtrsToApiFuncs.FillTestBuf;

    if (FillTestBuf == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    } else {
        return FillTestBuf(pLineCtx, length, pData);
    }
} /* VpFillTestBuf() */

/******************************************************************************
 *                              HELPER FUNCTIONS                              *
 ******************************************************************************/
#ifdef VP_CC_MULTI_THREAD_PROTECTION

/**
 * VpApiFunctionEntry()
 * This function returns TRUE if re-enterency restrictions are met by the
 * device/line API function it is called. The following table captures the
 * bahaviour of this function.
 *
 * Line Ctx     Device Ctx      Checks for
 * Valid        Null            Line not busy in another API function
 * Null         Valid           Device not busy in another API function
 * Valid        Valid           Both device/none of the lines are processing
 *                              any API function
 * Null         Null            Returns False (not expected to be called)
 *
 * Preconditions:
 * None
 *
 * Postconditions:
 * Updates the contents in device object if re-entrency requirements are met.
 */
static VpStatusType
VpApiFunctionEntry(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    bool *pFuncNotBlcked)
{
    VpDeviceType devType;
    void *pDevObj = VP_NULL;
    void *pLineObj = VP_NULL;
    VpDeviceIdType deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;
    uint8 line;
    uint32 *pApiReEnterStatus;
    uint32 lineMask;

    if(pDevCtx != VP_NULL) {
        devType = pDevCtx->deviceType;
        pDevObj = pDevCtx->pDevObj;
    } else if(pLineCtx != VP_NULL) {
        devType = pLineCtx->pDevCtx->deviceType;
        pDevObj = pLineCtx->pDevCtx->pDevObj;
    } else {
        return VP_STATUS_FAILURE;
    }

    /* Just to be sure */
    *pFuncNotBlcked = FALSE;

    /* From now on we just care about device context */
    switch (devType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES: {
            if(pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;
                line = ((VpDvpLineObjectType *)pLineObj)->channelId;
                pApiReEnterStatus = &((VpDvpDeviceObjectType *)pDevObj)->
                                        lineApiBusy;
                lineMask = 1<<line;
                deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
                VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
                /* This function should never get called if any of the API
                 * is within either MPI/HBI/Code Critical sections. This is
                 * because the interrupts which would acted as a source of
                 * trigger for this function call would have been disabled.
                 *
                 * Even if this function were to be called while inside a
                 * critical section (either HBI/MPI/Code), this function is
                 * one extra call and should not make any difference because the
                 * interrupts should not be enabled until the overall nesting of
                 * critical section becomes zero.
                 */
                if((*pApiReEnterStatus & lineMask) == 0) {
                    /* Resource is available; Grab it */
                    *pApiReEnterStatus = *pApiReEnterStatus | lineMask;
                    *pFuncNotBlcked = TRUE;
                }
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            } else {
                /* Does not handle this yet */
                status = VP_STATUS_FAILURE;
            }
            break;
        }
#endif /* VP_CC_DVP_SERIES */
        default: {
            return VP_STATUS_FAILURE;
        }
    }

    return VP_STATUS_SUCCESS;
} /* VpApiFunctionEntry() */


/**
 * VpApiFunctionExit()
 * This function releases the resource that was captured to excecute an API
 * function. This function can release two types of resources
 * 1)line API resource and 2)Device specific API resource.
 *
 * Preconditions:
 * This function should be called when exiting a function that captured the
 * applicable resource.
 *
 * Postconditions:
 * Releases the resource if it was captured at the first place.
 */
static bool
VpApiFunctionExit(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx)
{
    VpDeviceType devType;
    void *pDevObj = VP_NULL;
    void *pLineObj = VP_NULL;
    VpDeviceIdType deviceId;
    uint8 line;
    uint32 *pApiReEnterStatus;
    uint32 lineMask;
    bool retVal = TRUE;

    if(pDevCtx != VP_NULL) {
        devType = pDevCtx->deviceType;
        pDevObj = pDevCtx->pDevObj;
    } else if(pLineCtx != VP_NULL) {
        devType = pLineCtx->pDevCtx->deviceType;
        pDevObj = pLineCtx->pDevCtx->pDevObj;
    } else {
        return VP_STATUS_FAILURE;
    }

    /* From now on we just care about device context */
    switch (devType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES: {
            if(pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;
                line = ((VpDvpLineObjectType *)pLineObj)->channelId;
                pApiReEnterStatus = &((VpDvpDeviceObjectType *)pDevObj)->
                                        lineApiBusy;
                lineMask = 1<<line;
                deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
                VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

                if((*pApiReEnterStatus & lineMask) == 0) {
                    /* Not inside a function at the first place */
                    retVal = FALSE;
                } else {
                    /* Release the resource */
                    *pApiReEnterStatus = *pApiReEnterStatus & (~lineMask);
                }
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            } else {
                /* Does not handle this yet */
                retVal = FALSE;
            }
            break;
        }
#endif /* VP_CC_DVP_SERIES */
        default: {
            retVal = FALSE;
            break;
        }
    }

    return retVal;

} /* VpApiFunctionExit() */
#endif /* VP_CC_MULTI_THREAD_PROTECTION */

/**
 * VpMapLineId()
 *  This function assignes a lineId to a line object. It is used by the
 * application when mapping a device and channel Id to a specific line
 * context is not convenient (or easy). The value of lineId is reported
 * in the event structure.
 *
 * Preconditions:
 *  The line context and device object pointers must be non zero. Device
 * Object must be created before calling this function.
 *
 * Postconditions:
 *  The device context and device object are linked and the device context
 * function pointers are initialized.
 */
VpStatusType
VpMapLineId(
    VpLineCtxType *pLineCtx,   /**< Line Context to map to lineId */
	VpLineIdType lineId)       /**< Value assigned as line Id */
{
    VpDevCtxType *pDevCtx;
    VpDeviceIdType deviceId;
    void *pLineObj, *pDevObj;
    VpDeviceType devType;

    /* Basic error checking */
    if(pLineCtx == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }
    pDevCtx = pLineCtx->pDevCtx;
    pLineObj = pLineCtx->pLineObj;

    devType = pDevCtx->deviceType;
    pDevObj = pDevCtx->pDevObj;

    switch (devType) {
#if defined (VP_CC_DVP_SERIES)
        case VP_DEV_DVP_SERIES:
			deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
        	VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((VpDvpLineObjectType *)pLineObj)->lineId = lineId;
        	VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_VPP_SERIES)
        case VP_DEV_VPP_SERIES:
			deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
        	VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((VpVppLineObjectType *)pLineObj)->lineId = lineId;
        	VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
			deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
        	VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp880LineObjectType *)pLineObj)->lineId = lineId;
        	VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
			deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
        	VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp790LineObjectType *)pLineObj)->lineId = lineId;
        	VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
			deviceId = ((Vp580DeviceObjectType *)pDevObj)->deviceId;
        	VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
            ((Vp580LineObjectType *)pLineObj)->lineId = lineId;
        	VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            break;
#endif

        default:
            return VP_STATUS_ERR_VTD_CODE;
    }

    return VP_STATUS_SUCCESS;
} /* VpMapLineId() */

#endif




