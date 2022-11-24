/** \file apiInit.c
 * apiInit.c
 *
 * This file contains the line and device initialization functions for
 * the Vp880 device API.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_880_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_api.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"
#include "vp_hal.h"
#include "sys_service.h"
#include <linux/kernel.h>

/**< Vp880 Initalization Function Prototypes */
static VpStatusType
Vp880InitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

static VpStatusType
Vp880InitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

static VpStatusType
Vp880ConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

static VpStatusType
Vp880InitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

static VpStatusType
Vp880InitRing(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCadProfile,
    VpProfilePtrType pCidProfile);

static VpStatusType
Vp880InitCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);
/*
 * Auto Battery Detection is done to automatically detect the battery
 * configuration on the circuit, generally used only during initial bringup of
 * new hardware. The algorithm uses system wait functions that may consume all
 * processor resources, so it is recommended to remove this option for a
 * production environment. However, do so only when the parameters of the
 * switcher are determined and will not damage the device or circuitry.
 */
#ifdef VP880_AUTO_BAT_DETECT
/*#include <stdio.h>*/
VpStatusType
Vp880AutoBatDetect(
    Vp880DeviceObjectType *pDevObj,
    VpProfilePtrType pSwitcherParam);
static int16
Vp880AdcSettling(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 adcConfig,
    bool decay);
#endif

static void
Vp880InitDeviceObject(
    Vp880DeviceObjectType *pDevObj);

static void
Vp880InitLineObject(
    Vp880LineObjectType *pLineObj);

typedef enum
{
    VP880_DEV_PROFILE_PCLK_MSB = 6,
    VP880_DEV_PROFILE_PCLK_LSB = 7,
    VP880_DEV_PROFILE_DEVCFG1 = 8,
    VP880_DEV_PROFILE_MCLK_CORR = 9,
    VP880_DEV_PROFILE_CLOCK_SLOT = 10,
    VP880_DEV_PROFILE_MAX_EVENTS = 11,
    VP880_DEV_PROFILE_TICKRATE_MSB = 12,
    VP880_DEV_PROFILE_TICKRATE_LSB = 13,
    VP880_DEV_PROFILE_SWITCHER_CMD = 14,
    VP880_DEV_PROFILE_SWITCHER_DATA0 = 15,
    VP880_DEV_PROFILE_SWITCHER_DATA1 = 16,
    VP880_DEV_PROFILE_SWITCHER_DATA2 = 17,
    VP880_DEV_PROFILE_VBH_REG = 18,
    VP880_DEV_PROFILE_NOT_USED = 19,
    VP880_DEV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp880_deviceProfileParams;

/**
 * VpMakeVp880DeviceObject()
 *  This function performs the main tasks of VpMakeDeviceObject() for Vp880 type
 * of devices.
 *
 * Preconditions:
 *  Same as VpMakeDeviceObject(), and in addition the deviceType pointed to by
 * pDevCtx should be Vp880 series type.
 *
 * Postconditions:
 *  VpAPI Function pointers for pDevCtx are initialized to Vp880 specific
 * functions.  This completes the function abstraction for "this" device.
 */
VpStatusType
VpMakeVp880DeviceObject(
    VpDevCtxType *pDevCtx,  /**< Device context to be initialized with function
                             * pointers
                             */
    Vp880DeviceObjectType *pDevObj) /**< Device object containing information
                                     * for the device pointed to by pDevCtx
                                     */
{
    Vp880InitDeviceObject(pDevObj);

    /* Initialize other elements in the device object */
    return VpMakeVp880DeviceCtx(pDevCtx, pDevObj);
    }

/**
 * Vp880InitDeviceObject()
 *  This function initializes the Vp880 Device object data structure. It is
 * called only in this file .
 */
static void
Vp880InitDeviceObject(
    Vp880DeviceObjectType *pDevObj)
{
    uint16 objSize;
    uint8 *objPtr = (uint8 *)pDevObj;

    for (objSize = 0;
         objSize < (sizeof(Vp880DeviceObjectType) / sizeof(uint8));
         objSize++) {

        *objPtr = 0;
        objPtr++;
    }

    pDevObj->staticInfo.maxChannels = VP880_MAX_NUM_CHANNELS;
    }

/**
 * Vp880InitLineObject()
 *  This function initializes the Vp880 Line Object data structure. It is
 * called only in this file .
 */
static void
Vp880InitLineObject(
    Vp880LineObjectType *pLineObj)
{
    uint16 objSize;
    uint8 *objPtr = (uint8 *)pLineObj;

    for (objSize = 0;
         objSize < (sizeof(Vp880LineObjectType) / sizeof(uint8));
         objSize++) {

        *objPtr = 0;
        objPtr++;
    }
}

/**
 * VpMakeVp880DeviceCtx()
 *  This function initializes the device context to handle Vp880 functionality.
 *
 * Preconditions:
 *  This function should be called after initializing the device object. This
 * function can be called more than once since it does not modify the contents
 * of the device object.
 *
 * Postconditions:
 *  Initializes device context to be able to handle Vp780 functionality.
 */
VpStatusType
VpMakeVp880DeviceCtx(
    VpDevCtxType *pDevCtx,          /**< Device Context to be initialized */
    Vp880DeviceObjectType *pDevObj) /**< Device Object that has been already
                                     * initialized
                                     */
{
    uint8 channelCount, maxChan;

    if((pDevCtx == VP_NULL) || (pDevObj == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Initialize Device context */
    pDevCtx->pDevObj = pDevObj;
    pDevCtx->deviceType = VP_DEV_880_SERIES;

    /*
     * Initialize all of the line context pointers to null in the device context
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (channelCount = 0; channelCount < maxChan; channelCount++) {
        pDevCtx->pLineCtx[channelCount] = VP_NULL;
    }

    /* Functions in apiInit.c */
    pDevCtx->funPtrsToApiFuncs.InitDevice = Vp880InitDevice;
    pDevCtx->funPtrsToApiFuncs.InitLine = Vp880InitLine;
    pDevCtx->funPtrsToApiFuncs.ConfigLine = Vp880ConfigLine;
    pDevCtx->funPtrsToApiFuncs.InitRing = Vp880InitRing;
    pDevCtx->funPtrsToApiFuncs.InitProfile = Vp880InitProfile;
    pDevCtx->funPtrsToApiFuncs.InitCid = Vp880InitCid;
    pDevCtx->funPtrsToApiFuncs.InitMeter = VpCSLACInitMeter;
    pDevCtx->funPtrsToApiFuncs.ClearResults = VpCSLACClearResults;
    pDevCtx->funPtrsToApiFuncs.DtmfDigitDetected = VpCSLACDtmfDigitDetected;

    /* Functions in apicnt.c */
    pDevCtx->funPtrsToApiFuncs.SetLineState = Vp880SetLineState;
    pDevCtx->funPtrsToApiFuncs.SetLineTone = Vp880SetLineTone;
    pDevCtx->funPtrsToApiFuncs.SetRelGain = Vp880SetRelGain;
    pDevCtx->funPtrsToApiFuncs.SetRelayState = Vp880SetRelayState;
    pDevCtx->funPtrsToApiFuncs.SendSignal = Vp880SendSignal;
    pDevCtx->funPtrsToApiFuncs.SendCid = Vp880SendCid;
    pDevCtx->funPtrsToApiFuncs.ContinueCid = Vp880ContinueCid;
    pDevCtx->funPtrsToApiFuncs.SetOption = Vp880SetOption;
    pDevCtx->funPtrsToApiFuncs.VirtualISR = Vp880VirtualISR;
    pDevCtx->funPtrsToApiFuncs.ApiTick = Vp880ApiTick;
    pDevCtx->funPtrsToApiFuncs.LowLevelCmd = Vp880LowLevelCmd;
    pDevCtx->funPtrsToApiFuncs.StartMeter = VpCSLACStartMeter;
    pDevCtx->funPtrsToApiFuncs.DeviceIoAccess = Vp880DeviceIoAccess;

    /* Functions in apiQuery.c */
    pDevCtx->funPtrsToApiFuncs.GetEvent = Vp880GetEvent;
    pDevCtx->funPtrsToApiFuncs.GetLineStatus = VpCSLACGetLineStatus;
    pDevCtx->funPtrsToApiFuncs.GetDeviceStatus = Vp880GetDeviceStatus;
    pDevCtx->funPtrsToApiFuncs.FlushEvents = Vp880FlushEvents;
    pDevCtx->funPtrsToApiFuncs.GetResults = Vp880GetResults;
    pDevCtx->funPtrsToApiFuncs.GetOption = Vp880GetOption;

    /* Functions in apiTestLine.c */                          
#ifdef VP880_INCLUDE_TESTLINE_CODE    
    pDevCtx->funPtrsToApiFuncs.TestLineInt = Vp880TestLineInt;
    pDevCtx->funPtrsToApiFuncs.TestLine = Vp880TestLine;      
#endif

    return VP_STATUS_SUCCESS;
}

/**
 * VpMakeVp880LineObject()
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
VpMakeVp880LineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    Vp880LineObjectType *pLineObj,
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880DeviceStateIntType chanMap[] = {VP880_LINE0_IS_FXO, VP880_LINE1_IS_FXO};

    if (channelId >= pDevObj->staticInfo.maxChannels) {
        return VP_STATUS_INVALID_ARG;
    }

    Vp880InitLineObject(pLineObj);

    switch (termType) {
        case VP_TERM_FXO_GENERIC:
            /*
             * At this point, it is only a recommendation. We'll adjust this
             * when we determine the device type found in VpInitDevice()
             */
            if (pDevObj->stateInt & VP880_DEVICE_DETECTED) {
                if (chanMap[channelId] & pDevObj->stateInt) {
                    pLineObj->status = VP880_IS_FXO;
                } else {
                    return VP_STATUS_ERR_VTD_CODE;
                }
            } else {
            pLineObj->status = VP880_IS_FXO;
            }
            break;

        case VP_TERM_FXS_GENERIC:
        case VP_TERM_FXS_ISOLATE:
        case VP_TERM_FXS_SPLITTER:
            pLineObj->status = VP880_INIT_STATUS;
            break;

        default:
            return VP_STATUS_ERR_VTD_CODE;
    }

    pLineCtx->pLineObj = pLineObj;
    pLineCtx->pDevCtx = pDevCtx;

    pDevCtx->pLineCtx[channelId] = pLineCtx;
    pLineObj->channelId = channelId;
    pLineObj->termType = termType;

    /* Everything else done by device/line specific functions */
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880Init
 *  This function initializes the device, and (contrary to InitDevice) does
 * not initialize any channels. This function should be called internal to the
 * API only.
 *
 * Preconditions:
 *  The device context must be of a Vp880 device type.
 *
 * Postconditions:
 *  This function returns a failure code if the clock configuration is not set
 * correctly based on the device data set in InitDevice.
 */
VpStatusType
Vp880Init(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 data, chanId, maxChan;
    uint8 intMaskData[] = {0x7F, 0xFF};
    uint8 clkNotStable;
    uint8 clkTestCount;

    /*
     * When the device timing is cleared, so are the lines associated with
     * this device
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (chanId = 0; chanId < maxChan; chanId++) {
        /*
         * It is possible that any channel on a device is not used and therefore
         * points to a NULL pointer. Check it and jump over if this is the case
         * to the next line until all possible line numbers for this device have
         * been checked.
         */
        pLineCtx = pDevCtx->pLineCtx[chanId];
        if (pLineCtx != VP_NULL) {
            /*
             * If the device is "dumping" it's timers, the lines associated with
             * this device better not be relying on timers.
             */
            InitTimerVars(pLineCtx);
        }
    }

    /*
     * If the MPI Bus gets out of sequence for any reason, a HW reset command
     * will not work and this function may fail. To be sure a reset occurs, the
     * following sequence is required.
     */
    /* Cause a 16-bit Chip Select Low reset */
//    VpMpiReset(deviceId, pDevCtx->deviceType);

    /* Wait at least 1.25mS for internal reset to go away */
    VpSysWait(20);  /* 20 * 125uS = 2.5mS */

    /* Perform a hardware reset */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_HW_RESET_CMD, NO_DATA, &data);

    /* Wait 3 Frames after hardware reset before sending any more commands */
    VpSysWait(3);

    /*
     * Setup mclk. The MCLK mask set the mclk frequency, sets the mclk source
     * (the MCLK pin or the PCLK pin), and sets the interrupt pin output drive
     * mode (TTL or open collector)
     */
    data = pDevObj->devProfileData.devCfg1;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_MCLK_CNT_WRT, VP880_MCLK_CNT_LEN,
        &data);

    /* Setup the Clock Fail Interrupt */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_MASK_WRT, VP880_INT_MASK_LEN,
        intMaskData);

    /*
     * Wait for the CFAIL bit to clear before proceding. If the CFAIL bit does
     * not clear after several trys, give up and return an error condition. Wait
     * between each read of the status register.
     */

    clkNotStable = VP880_CFAIL_MASK;
    clkTestCount = MAX_CFAIL_TEST;
    while(clkNotStable && (--clkTestCount) != 0) {
        VpSysWait(CFAIL_TEST_INTERVAL*10);
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_UL_SIGREG_RD,
            VP880_UL_SIGREG_LEN, pDevObj->intReg);
        clkNotStable = pDevObj->intReg[0] & VP880_CFAIL_MASK;
    }

    /*
     * The CFAIL bit did not clear so the part will not complete initialization.
     * Return error status to indicate failure.
     */
    if(clkNotStable) {
        pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
        return VP_STATUS_FAILURE;
    }

    /* Setup interrupts back to default */
    intMaskData[0] = 0xFF;  /* Clear all Device Interrupt Masks */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_MASK_WRT, VP880_INT_MASK_LEN,
        intMaskData);

    /*
     * The PCM mask tells the device which clock edge to grab and xmit the
     * PCM data on and also which clock period LSB of the PCM data starts on
     */
    data = pDevObj->devProfileData.clockSlot;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_XR_CS_WRT, VP880_XR_CS_LEN, &data);

    return VP_STATUS_SUCCESS;
} /* Vp880Init */

/**
 * Vp880InitDevice
 *  This function initializes the device and all lines associated with this
 * device (if line profiles are passed to this function). The device profile
 * passed must be valid otherwise an error code is returned and the device
 * remains in it's previously initialized state.
 *
 * Preconditions:
 *  None (device context is not NULL and is of Vp880 type, which is handled in
 * higher level software)
 *
 * Postconditions:
 *  This device is initialized to the configuration specified in the device
 * profile, and the FXS lines associated with this device are initialized by the
 * FXS specific AC, DC, and Ringing profiles passed, and the FXO lines
 * associated with this device are initialized by the FXO specific AC and Config
 * profiles passed.  If the FXO/FXS profiles are all NULL, then only the device
 * initialization occurs. This function returns an error code if the device
 * profile trying to be used for initialization is VP_PTABLE_NULL (either
 * passed or by a non-initialized index).
 */
VpStatusType
Vp880InitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,   /**< The profile pointer for the device
                                     * configuration parameters
                                     */
    VpProfilePtrType pAcProfile,    /**< The profile pointer (or index) for
                                     * the AC characteristic to apply to the
                                     * FXS lines
                                     */
    VpProfilePtrType pDcProfile,    /**< The profile pointer (or index) for
                                     * the DC characteristic to apply to the
                                     * FXS lines
                                     */
    VpProfilePtrType pRingProfile,  /**< The profile pointer (or index) for
                                     * the Ringing characteristic to apply to
                                     * the FXS lines
                                       */
    VpProfilePtrType pFxoAcProfile, /**< The profile pointer (or index) for
                                     * the AC characteristic to apply to the
                                     * FXO lines
                                       */
    VpProfilePtrType pFxoCfgProfile)/**< The profile pointer for the FXO
                                     * specific supervision paramaters.
                                     */
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    Vp880LineObjectType *pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    VpProfilePtrType pDevProf;
    uint8 intSwParam[] = {0x5C, 0x4B, 0xC4, 0x4B, 0xC4, 0x4B};
    uint8 deviceRcn;

    uint8 chan, data;
    VpStatusType status = VP_STATUS_SUCCESS;
    Vp880PcnType devicePcn;

    int profIndex = GetProfileIndex(pDevProfile);

    /*
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pDevProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_DEVICE, pDevProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pDevProf = pDevProfile;
    } else if (profIndex < VP_CSLAC_DEV_PROF_TABLE_SIZE) {
        pDevProf = pDevObj->devProfileTable.pDevProfileTable[profIndex];
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Initialize the API's device status variables */
    pDevObj->status.state = VP_DEV_INIT_IN_PROGRESS;
    pDevObj->timeStamp = 0;

    /* Initialize the API's device dynamic variables */
    pDevObj->dynamicInfo.lastChan = 0;
    pDevObj->dynamicInfo.bat1Fault = FALSE;
    pDevObj->dynamicInfo.bat2Fault = FALSE;
    pDevObj->dynamicInfo.bat3Fault = FALSE;
    pDevObj->dynamicInfo.clkFault = FALSE;

    pDevObj->stateInt = 0;  /* Reset the internal state information */

    if (pDevProf != VP_PTABLE_NULL) {
        pDevObj->devProfileData.pcmClkRate =
            (uint16)(((pDevProf[VP880_DEV_PROFILE_PCLK_MSB] << 8) & 0xFF00)
                    | (pDevProf[VP880_DEV_PROFILE_PCLK_LSB] & 0x00FF));

        pDevObj->devProfileData.devCfg1 =
            (uint8)(pDevProf[VP880_DEV_PROFILE_DEVCFG1]);
        pDevObj->devProfileData.clockSlot =
            (uint8)(pDevProf[VP880_DEV_PROFILE_CLOCK_SLOT]);

        pDevObj->devProfileData.maxNumInterrupts =
            pDevProf[VP880_DEV_PROFILE_MAX_EVENTS];

        pDevObj->devProfileData.tickRate =
            (uint16)(((pDevProf[VP880_DEV_PROFILE_TICKRATE_MSB] << 8) & 0xFF00)
                    | (pDevProf[VP880_DEV_PROFILE_TICKRATE_LSB] & 0x00FF));
    } else {
        pDevObj->status.state &= ~VP_DEV_INIT_IN_PROGRESS;
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }

    /* Initialize device */
    /*
     * If not successful, the Clock Fail bit did not clear so return error code
     */
    if ((status = Vp880Init(pDevCtx)) != VP_STATUS_SUCCESS) {
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return status;
    }

    /*
     * Read in the PCN so we will know what type of device specifically we
     * are working with.  This affects the max number of lines supported by the
     * device as well as the type of lines (and may affect the line init)
     *
     * Note: EC value is not important for this command
     */
    VpMpiCmd(deviceId, ecVal[0], VP880_RCN_PCN_RD, VP880_RCN_PCN_LEN,
        pDevObj->staticInfo.rcnPcn);
    devicePcn = (Vp880PcnType)(pDevObj->staticInfo.rcnPcn[VP880_PCN_LOCATION]);
    deviceRcn = (uint8)(pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]);

    /*
     * Verify that we recognize the device by limiting to the range of those
     * supported in the Vp880PcnType table. If not recognized (although may
     * be a valid Legerity PN) return an error because the API-II does not
     * know how to handle it. More often, the error is occuring because the
     * hardware cannot talk to the device.
     */
    if ((devicePcn > VP880_FIRST_OF_ONE_CHANNEL_DEVICES)
     && (devicePcn < VP880_LAST_OF_ONE_CHANNEL_DEVICES)) {
        pDevObj->staticInfo.maxChannels = 1;
        maxChan = 1;
        pDevObj->stateInt |= VP880_IS_SINGLE_CHANNEL;
    } else if ((devicePcn > VP880_FIRST_OF_TWO_CHANNEL_DEVICES)
            && (devicePcn < VP880_LAST_OF_TWO_CHANNEL_DEVICES)) {
        pDevObj->staticInfo.maxChannels = 2;
        maxChan = 2;
        pDevObj->stateInt &= ~VP880_IS_SINGLE_CHANNEL;
    } else {
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_FAILURE;
    }

    pDevObj->stateInt |= VP880_DEVICE_DETECTED;

    /* Check if device is ABS or Not */
    switch (devicePcn) {
        case VP880_DEV_PCN_88320:
        case VP880_DEV_PCN_88321:
        case VP880_DEV_PCN_88326:
        case VP880_DEV_PCN_88340:
        case VP880_DEV_PCN_88341:
        case VP880_DEV_PCN_88346:
        case VP880_DEV_PCN_88220:
        case VP880_DEV_PCN_88221:
        case VP880_DEV_PCN_88226:
        case VP880_DEV_PCN_88240:
        case VP880_DEV_PCN_88241:
        case VP880_DEV_PCN_88246:
            pDevObj->stateInt |= VP880_IS_ABS; 

            /* Initialize ABS devices sensitive items */
            data = VP880_SS_DISCONNECT;
            for (chan = 0; chan < maxChan; chan++) {
                VpMpiCmd(deviceId, ecVal[chan], VP880_SYS_STATE_WRT,
                    VP880_SYS_STATE_LEN, &data);
            }

            data = VP880_SWY_OFF;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);

#ifdef VP880_AUTO_BAT_DETECT
            if ((status = Vp880AutoBatDetect(pDevObj,
                &pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1])) != VP_STATUS_SUCCESS) {
                pDevObj->status.state &= 
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
                
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return status;
            }
#else
            if (pDevProf[VP_PROFILE_VERSION] >= VP_CSLAC_DEV_PROFILE_VERSION_SW_CONFIG) {
                VpMpiCmd(deviceId, ecVal[0],
                    pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD],
                    VP880_REGULATOR_PARAM_LEN,
                    (VpProfileDataType *)(&pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1]));
            } else {
                pDevObj->status.state &= 
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);

                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_FAILURE;
            }

            data = VP880_SWY_HP | VP880_SWZ_LP;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);
#endif
            break;

        default:
            pDevObj->stateInt &= ~VP880_IS_ABS; 

            /* Initialize Non-ABS device sensitve items */
            /* 
             * Configure the Switcher for Flyback or Buckboost per device 
             * profile. If the device is in Buckboost mode, config the internal 
             * switcher
             */
            if (pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1] & 0x20) {
                VpMpiCmd(deviceId, ecVal[0], 0xF6, 0x06,  intSwParam);
            }

            VpMpiCmd(deviceId, ecVal[0], pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD],
                VP880_REGULATOR_PARAM_LEN,
                (VpProfileDataType *)(&pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1]));

            /* Enable the switchers in med power */
            data = VP880_SWY_MP | VP880_SWZ_MP;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);
            break;
    }

    /* 
     * Check for High Voltage Device and line test switch 
     * Currently high voltage and test switch go hand in hand but may not in
     * the future that is why there are two bits but only a test for one.
     */    
    if ((devicePcn & VP880_HV_MASK) == VP880_HV_MASK) {
        pDevObj->stateInt |= VP880_IS_HIGH_VOLTAGE;
        pDevObj->stateInt |= VP880_HAS_TEST_LOAD_SWITCH;
    } else {
        pDevObj->stateInt &= ~VP880_IS_HIGH_VOLTAGE;
        pDevObj->stateInt &= ~VP880_HAS_TEST_LOAD_SWITCH;
    }

    /* Check for devices that have an FXO on line 0 and/or line 1*/
    switch (devicePcn) {
        case VP880_DEV_PCN_88020:
        case VP880_DEV_PCN_88026:
            pDevObj->stateInt |= VP880_LINE1_IS_FXO;

        case VP880_DEV_PCN_88010:
        case VP880_DEV_PCN_88016:
        case VP880_DEV_PCN_88320:
        case VP880_DEV_PCN_88321:
        case VP880_DEV_PCN_88326:
        case VP880_DEV_PCN_88340:
        case VP880_DEV_PCN_88341:
        case VP880_DEV_PCN_88346:

        case VP880_DEV_PCN_88310:
        case VP880_DEV_PCN_88311:
        case VP880_DEV_PCN_88316:
        case VP880_DEV_PCN_88330:
        case VP880_DEV_PCN_88331:
        case VP880_DEV_PCN_88336:
            pDevObj->stateInt |= VP880_LINE0_IS_FXO;
            break;

        default:
            break;
    }  
                  
    /* Check for Wideband Mode support */
    if ((devicePcn & (VP880_CODEC_MASK | VP880_WIDEBAND_MASK))
        == (VP880_CODEC_MASK | VP880_WIDEBAND_MASK)) {
        pDevObj->stateInt |= VP880_WIDEBAND;
    }

    /* Check for Cal Circuit */
    if (VP880_REV_VA == deviceRcn) {
        /* none of the rev 1 devices have cal circuit */
        pDevObj->stateInt &= ~VP880_HAS_CALIBRATE_CIRCUIT;
        
    } else if (VP880_REV_VC == deviceRcn) {
        if (pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL) {
            /* none of the single channel rev 2 devices have a cal circuit */
                pDevObj->stateInt &= ~VP880_HAS_CALIBRATE_CIRCUIT;            
        } else {
                pDevObj->stateInt |= VP880_HAS_CALIBRATE_CIRCUIT;            
        }    
    } else {
        /* all other revs should have cal circuit */    
        pDevObj->stateInt |= VP880_HAS_CALIBRATE_CIRCUIT;
    }                

    /* Initialize each channel */
    for (chan = 0; chan < maxChan; chan++) {
        /*
         * For Init Line to work, the device cannot be non-initialized because
         * the init line function tries to set the line state.  Therefore,
         * temporarily set the device init flag to TRUE then immediately after
         * line init, set back to FALSE until device init is complete
         */
        pLineCtx = pDevCtx->pLineCtx[chan];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            if (pLineObj->status & VP880_IS_FXO) {
                status = Vp880InitLine(pLineCtx, pFxoAcProfile, pFxoCfgProfile,
                    VP_PTABLE_NULL);
            } else {
                status = Vp880InitLine(pLineCtx, pAcProfile, pDcProfile,
                    pRingProfile);
#ifdef VP880_INCLUDE_TESTLINE_CODE
                /* initialize the calibration coeffs */
                pDevObj->calOffsets[chan].nullOffset = 0;
                pDevObj->calOffsets[chan].vabOffset = 0;
                pDevObj->calOffsets[chan].vahOffset = 0;
                pDevObj->calOffsets[chan].vbhOffset = 0;
#endif /* VP880_INCLUDE_TESTLINE_CODE */                    
            }
            if (status != VP_STATUS_SUCCESS) {
                pDevObj->status.state &= 
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
            
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return status;
            }        
        }        
    }

    status = VpImplementDefaultSettings(pDevCtx, VP_NULL);

    /*
     * This clears the Init Line Events and any other erroneous event that
     * may have been created due to initialization
     */
    Vp880FlushEvents(pDevCtx);

#ifdef VP880_INCLUDE_TESTLINE_CODE
    /* 
     * This clears the Test structure
     */
    pDevObj->currentTest.prepared = FALSE;
    pDevObj->currentTest.testState = -1;
    pDevObj->currentTest.testId = VP_NUM_TEST_IDS;
#endif /* VP880_INCLUDE_TESTLINE_CODE */
                                           
    /*
     * Set a Device Init Complete Event if status is succesfull and it's
     * either a non-ABS device or the silicon revision is < VC. For ABS rev
     * VC and higher, start device calibration.
     */
    if (status == VP_STATUS_SUCCESS) {
        if ((!(pDevObj->stateInt & VP880_IS_ABS)) ||
            (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
            VP_DOUT(DBG_INFO, ("\n\rCal Not Required: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));

            pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
        pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
            pDevObj->status.state &= ~VP_DEV_IN_CAL;
        } else {
#ifdef VP880_CAL_ENABLE
            VP_DOUT(DBG_INFO, ("\n\rCal Required: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));
            pDevObj->status.state |= VP_DEV_IN_CAL;
#else
            VP_DOUT(DBG_INFO, ("\n\rCal Required, Disabled at Compile: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));
            pDevObj->status.state &= ~VP_DEV_IN_CAL;
            pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
#endif
        }
    }

    /*
     * Success, Failure, or Calibration started -- we're not in "InitDevice"
     * function anymore. So normal rules apply.
     */
    pDevObj->status.state |= VP_DEV_INIT_CMP;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp880InitDevice */

/**
 * Vp880AutoBatDetect()
 *  This function configures the battery configuration on the device if it can
 * be detected.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  The device battery configuration is set. Note: A possible setting is there
 * are no batteries present, so the switcher parameters are set to minimal
 * values.
 */
#ifdef VP880_AUTO_BAT_DETECT
VpStatusType
Vp880AutoBatDetect(
    Vp880DeviceObjectType *pDevObj,
    VpProfilePtrType pSwitcherParam)
{
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 swyInit[VP880_REGULATOR_PARAM_LEN] = {0x02, 0x04, 0x84};
    uint8 swyVerify[VP880_REGULATOR_PARAM_LEN];

    uint8 swLowReg[VP880_REGULATOR_PARAM_LEN] = {0x00, 0x05, 0x00};
    uint8 swMedReg[VP880_REGULATOR_PARAM_LEN] = {0x80, 0x11, 0x80};

    uint8 data; /* Used lots for different 1-byte MPI actions */
    bool failure = TRUE;

#define VP880_AUTODET_ATMPT 5
    uint8 autoDetAtmpt;

    uint8 chan;
    int16 vblVolt = 0;
    int16 vbhVolt = 0;
    int16 vbmVolt = 0;
    int16 vbOffVolt = 0;
    int16 v25 = 0;

    /*
     * Scale the comparison voltages by MAX_BIT_SCALE / MAX_VOLT = 32768 / 240
     * = 137 (approx).
     */
    const int16 scaleFactor = 137;
    const int16 vblRegHigh = 32 * scaleFactor;  /* Scale 25V +25% tolerance */
    const int16 vblRegLow = 19 * scaleFactor;   /* Scale 25V -25% tolerance */

    const int16 offsetHigh = 5 * scaleFactor;
    const int16 offsetLow = -5 * scaleFactor;

    /*
     * This loop makes several attempts to detect the supply configuration
     * in conditions when the supply may have been initially powered up, or
     * when reset from a previously powered condition. If the supply
     * configuration cannot reasonably be determined, they are disabled and
     * an error is returned.
     */
    VP_DOUT(DBG_INFO, ("\n\rAttempting to Determine ABS Supply Configuration on DevId 0x%02X ", deviceId));
    
    for (autoDetAtmpt = 0;
        ((autoDetAtmpt < VP880_AUTODET_ATMPT) && (failure == TRUE));
        autoDetAtmpt++) {

        failure = FALSE;
        /* disable both switching regulators */
        data = VP880_SWY_OFF;
        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, &data);

        /* Disable Auto-System State Control and Set all lines to Disconnect */
        for (chan = 0; chan < maxChan; chan++) {
            /* Disable auto system state control */
            data = VP880_AUTO_SSC_DIS | VP880_AUTO_BAT_SHUTDOWN_EN;
            VpMpiCmd(deviceId, ecVal[chan], VP880_SS_CONFIG_WRT,
                VP880_SS_CONFIG_LEN, &data);

            data = VP880_SS_DISCONNECT;
            VpMpiCmd(deviceId, ecVal[chan], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, &data);
        }

        /*
         * Prepare channel 1 to take measurements using
         * ADC in a non-disconnect state
         */
        /* Set line one to Active-Mid-Battery */
        data = VP880_SS_ACTIVE_MID_BAT;
        VpMpiCmd(deviceId, ecVal[0], VP880_SYS_STATE_WRT,
            VP880_SYS_STATE_LEN, &data);

        /* Set compression to Linear Mode */
        data = VP880_LINEAR_CODEC;
        VpMpiCmd(deviceId, ecVal[0], VP880_CODEC_REG_WRT,
            VP880_CODEC_REG_LEN, &data);

        /* Cut TX/RX PCM and disable HPF */
        data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
        VpMpiCmd(deviceId, ecVal[0], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &data);
        VpSysWait(255);   /* allow circuits to settle */

        /* wait for switcher y to settle (decay to 0V) */
        Vp880AdcSettling(deviceId, ecVal[0], VP880_SWITCHER_Y, TRUE);

        /* Set SWVSY and SWVSZ to 25V -- a harmless setting */
        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
            VP880_REGULATOR_PARAM_LEN, swyInit);

        /* Set device for Low Power */
        data = VP880_SWY_LP;
        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, &data);

        /* Connect to a known DC source to make sure NO Connect is settled */
        data = VP880_CALIBRATION_I;
        VpMpiCmd(deviceId, ecVal[0], VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
            &data);
        VpSysWait(24);   /* 125uS * 24 = 3mS */

        /* wait for offset to settle and get value*/
        vbOffVolt = Vp880AdcSettling(deviceId, ecVal[0], VP880_NO_CONNECT, FALSE);

        /* If offset is too much -- failure */
        if ((vbOffVolt < offsetLow) || (vbOffVolt > offsetHigh)) {
            failure = TRUE;
            VP_DOUT(DBG_ERROR, ("\n\rOffset Error %d. Retrying on Device 0x%02X\n\r",
                (vbOffVolt * 240 / 32768), deviceId));
            continue; /* Go back and try detection routine again */
        }

        /* wait for Y to settle and get value*/
        vblVolt =
            Vp880AdcSettling(deviceId, ecVal[0], VP880_SWITCHER_Y, FALSE);
        vblVolt -= vbOffVolt;

        /* wait for Z to settle */
        vbhVolt =
            Vp880AdcSettling(deviceId, ecVal[0], VP880_SWITCHER_Z, FALSE);
        vbhVolt -= vbOffVolt;

        /* wait for XB to settle */
        vbmVolt = Vp880AdcSettling(deviceId, ecVal[0], VP880_XBR, FALSE);
        vbmVolt -= vbOffVolt;

            /* 
             * One of the voltages are the regulated voltage and must be at 
             * 25V. Find out which
             */
            if ((vblVolt <= vblRegHigh) && (vblVolt >= vblRegLow)) {
                v25 = vblVolt;
            } else if ((vbmVolt <= vblRegHigh) && (vbmVolt >= vblRegLow)) {
                v25 = vbmVolt;
            } else if ((vbhVolt <= vblRegHigh) && (vbhVolt >= vblRegLow)) {
                v25 = vbhVolt;
            } else {
                /* Battery configuration not determined -- disable */
                data = VP880_SWY_OFF;
                VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                    VP880_REGULATOR_CTRL_LEN, &data);
                failure = TRUE;
            VP_DOUT(DBG_ERROR, ("\n\rERROR: Voltages do not meet regulation criteria on Device 0x%02X\n\r",
                deviceId));

                VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_RD,
                    VP880_REGULATOR_PARAM_LEN, swyVerify);
            VP_DOUT(DBG_INFO, ("\n\rSW: 0x%02X 0x%02X 0x%02X\n\r",
                swyVerify[0], swyVerify[1], swyVerify[2]));
            continue; /* Go back and try detection routine again */
            }
            
            /* 
             * One of the other voltages must be either 2 to 4 times the
             * regulated voltage, or 2 to 4 times lower than the regulated
             * voltage. Test for 2 to 4 times higher first (VBL Regulated).
             */
            if (((vblVolt <= (v25 * 4)) && (vblVolt >= (v25 * 2)))
             || ((vbmVolt <= (v25 * 4)) && (vbmVolt >= (v25 * 2)))
             || ((vbhVolt <= (v25 * 4)) && (vbhVolt >= (v25 * 2)))) {
                VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
                    VP880_REGULATOR_PARAM_LEN, swLowReg);
            } else {
                /* VBL Regulation Check failed. Check for VBH regulation */
                if (((vblVolt <= (v25 / 2)) && (vblVolt >= (v25 / 4)))
                 || ((vbmVolt <= (v25 / 2)) && (vbmVolt >= (v25 / 4)))
                 || ((vbhVolt <= (v25 / 2)) && (vbhVolt >= (v25 / 4)))) {
                    VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
                        VP880_REGULATOR_PARAM_LEN, swMedReg);

                } else {
                    /* Battery configuration not determined -- disable */
                    data = VP880_SWY_OFF;
                    VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                        VP880_REGULATOR_CTRL_LEN, &data);
                    failure = TRUE;
                VP_DOUT(DBG_ERROR, ("\n\rRegulation Unknown on Device 0x%02X\n\r", deviceId));
                }
            }
        }

    if (failure == TRUE) {
        /*
         * Supply configuration not determined or does not match the device
         * profile passed. Disable the supplies and return an error.
         */
        data = VP880_SWY_OFF;
        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, &data);
        VP_DOUT(DBG_ERROR, ("\n\rERROR!! Supply Configuration on Device 0x%02X Not Determined.\n\r",
            deviceId));
        return VP_STATUS_FAILURE;
    } else {
        VP_DOUT(DBG_INFO, ("\n\rSupply Configuration in Device 0x%02X Complete.\n\r",
            deviceId));
        data = VP880_SWY_HP | VP880_SWZ_HP;
        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, &data);
    }

    return VP_STATUS_SUCCESS;
}

static int16
Vp880AdcSettling(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 adcConfig,
    bool decay)
{
    #define ADC_PERCENT_ERROR 1 /* 1% */
    #define NUM_ADC_READS 5
    #define MAX_SYS_TIME 32000 /* 4000ms */
    #define SYS_WAIT_INTERVAL 160 /* 20 */
    uint8 xdataTemp[VP880_TX_PCM_DATA_LEN];
    int16 xdataAvgOld, xdataAvgNew, temp;
    int32 delta;
    uint16 sysCntr;
    uint8 i;
    int32 sum = 0;
    uint16 x = 1;

    xdataAvgOld = 1;

    /*
     * Get average value of the ? swither until the delta of the current
     * measurement and the previous measurement is within 4 units (30mv),
     * or until time expires or until both measurements have decayed below ~3.65V.
     * (if decay flag is set)
     */
    for (sysCntr = 0; sysCntr < MAX_SYS_TIME; sysCntr+=(NUM_ADC_READS*SYS_WAIT_INTERVAL)) {
        sum = 0;

        /* enable the proper adc configuration */
        VpMpiCmd(deviceId, ecVal, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
            &adcConfig);

        /*
         * Read the adc x number of times and get calculate the average.
         */
        for (i = 0; i < NUM_ADC_READS; i++) {
            VpMpiCmd(deviceId, ecVal, VP880_TX_PCM_DATA_RD, VP880_TX_PCM_DATA_LEN,
                xdataTemp);
            temp = ( (xdataTemp[0] << 8) | xdataTemp[1]);
            sum += (int32)temp;

            VpSysWait(SYS_WAIT_INTERVAL);   /* wait 20ms before next measurment */
        }
        xdataAvgNew = (int16)(sum / NUM_ADC_READS);

        /* find delta and scale to 1 percent granularity*/
        if (0 != xdataAvgOld) {
            delta = (int32)abs((xdataAvgOld - xdataAvgNew) * 100 / xdataAvgOld);
    } else {
            delta = 0;
        }

        VP_DOUT(DBG_INFO, ("%i:%i:%i:%li  ", decay, xdataAvgOld, xdataAvgNew, delta));

        /*
         * if the delta is less than percent error | both old and new xdata
         * are less than ~3.65V (voltage decay) then get out
         */
        if ((delta <= ADC_PERCENT_ERROR) ||
            ((500 > abs(xdataAvgOld)) &&
             (500 > abs(xdataAvgNew)) &&
             (decay == TRUE)) ) {
            /* break out if the switcher has settled long enough */
            break;
    }

        /* provide feedback for next measurment */
        xdataAvgOld = xdataAvgNew;
        x++;
    }
    return xdataAvgNew;
}
#endif

/**
 * Vp880InitLine
 *  This function initializes a line of a device with the specified parameters
 * and API default values. It is a "Line Reset".
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to be the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 */
VpStatusType
Vp880InitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,    /**< Pointer to AC coefficient data or
                                     * profile index to be applied to this line.
                                     */

    VpProfilePtrType pDcOrFxoProfile,   /**< Pointer to DC Feed (FXS) or Cfg
                                         * (FX0) profile or profile index to be
                                         * applied to this line.
                                         */

    VpProfilePtrType pRingProfile)  /**< Pointer to Ringing profile or profile
                                     * index to apply to this line
                                     */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 alwaysOn[VP880_CADENCE_TIMER_LEN] = {0x3F, 0xFF, 0x00, 0x00};
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x00, 0x02};

    uint8 seqByte, ringingParamCount;
    uint8 defaultRingParams[VP880_RINGER_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8 defaultLoopSup[VP880_LOOP_SUP_LEN] = {0x1B, 0x84, 0xB3, 0x05};
    uint8 defaultDcFeed[VP880_DC_FEED_LEN] = {0x13, 0x08};

    /*
     * IO Direction and Control used to restore the device IO to the state
     * set prior to the channel Software Reset
     */
    uint8 ioDirection[VP880_IODIR_REG_LEN];
    uint8 ioData[VP880_IODATA_REG_LEN];

    VpStatusType status = VP_STATUS_SUCCESS;
    uint8 data;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    
    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    pLineObj->status &= ~(VP880_BAD_LOOP_SUP);

    pLineObj->pRingingCadence = VP_PTABLE_NULL;
    pLineObj->pCidProfileType1 = VP_PTABLE_NULL;
    pLineObj->pCidProfileType2 = VP_PTABLE_NULL;

    /* Initialize cached transmit and receive gains for SetRelGain to 1.0. */
    pLineObj->gain.gxInt = 0x4000;
    pLineObj->gain.grInt = 0x4000;

    pLineObj->callerId.status = 0x00;

    /* Inititialize API line state variables */
    if (pLineObj->status & VP880_IS_FXO) {
        pLineObj->lineState.currentType = VP_LINE_FXO_LOOP_OPEN;
        pLineObj->lineState.previous = VP_LINE_FXO_LOOP_OPEN;
    } else {
        pLineObj->lineState.currentType = VP_LINE_DISCONNECT;
        pLineObj->lineState.previous = VP_LINE_DISCONNECT;
    }

    /* Force a line state check */
    pLineObj->lineState.condition = VP_CSLAC_STATUS_INVALID;
    pLineObj->dpStruct.hookSt = FALSE;
    pLineObj->dpStruct2.hookSt = FALSE;

    /* Force a codec update */
    pLineObj->codec = VP_NUM_OPTION_CODEC_TYPE_IDS;

    VpInitDP(&pLineObj->dpStruct);
    VpInitDP(&pLineObj->dpStruct2);

    /*
     * Read the IO direction and data for the device IO that will be affected
     * by a software reset
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
        VP880_IODATA_REG_LEN, ioData);

    /* Software reset the channel */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_SW_RESET_CMD, NO_DATA, &data);
    VpSysWait(3);

    /* Restore the previous device IO direction and control */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
        VP880_IODATA_REG_LEN, ioData);

    /*
     * Operating Conditions - Remove all loopbacks, connect TX/RX PCM Hwy
     * Note that TX/RX PCM Highway is set when Set Linestate function is
     * called.
     */
    data = VP880_NORMAL_OP_COND_MODE;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    /* Disable the internal device cadencer .. done in the API */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CADENCE_TIMER_WRT,
        VP880_CADENCE_TIMER_LEN, alwaysOn);

    /* Start the channel out in the standby state or loop open (if FXO)  */
    if (pLineObj->status & VP880_IS_FXO) {
        pLineObj->lineTimers.type = VP_CSLAC_FXO_TIMER;

        /* Disable auto system state control */
        data = VP880_AUTO_SSC_DIS;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
            VP880_SS_CONFIG_LEN, &data);

        data &= ~VP880_IODIR_IO1_MASK;
        data = (VP880_IODIR_IO1_OUTPUT | (VP880_FXO_CID_LINE << 1));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
            VP880_IODIR_REG_LEN, &data);

        pLineObj->digitGenStruct.dtmfOnTime = VP_FXO_DTMF_ON_DEFAULT;
        pLineObj->digitGenStruct.dtmfOffTime = VP_FXO_DTMF_OFF_DEFAULT;
        pLineObj->digitGenStruct.breakTime = VP_FXO_PULSE_BREAK_DEFAULT;
        pLineObj->digitGenStruct.makeTime = VP_FXO_PULSE_MAKE_DEFAULT;
        pLineObj->digitGenStruct.flashTime = VP_FXO_FLASH_HOOK_DEFAULT;
        pLineObj->digitGenStruct.dpInterDigitTime = VP_FXO_INTERDIG_DEFAULT;
        
        status = Vp880ConfigLine(pLineCtx, pAcProfile, pDcOrFxoProfile,
            VP_PTABLE_NULL);
        if (status != VP_STATUS_SUCCESS) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return status;
        }
        /* Activate Codec and enable Supervision */
        Vp880SetLineState(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
    } else {
        pLineObj->lineTimers.type = VP_CSLAC_FXS_TIMER;

        /* Enable Auto Bat Shutdown and disable Auto State Control */
        data = VP880_AUTO_SSC_DIS;

        if ((pDevObj->stateInt & VP880_IS_ABS) &&
             (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]
                < VP880_REV_VC)) {

            VpMpiCmd(deviceId, ecVal[pLineObj->channelId], VP880_ICR6_WRT,
                VP880_ICR6_LEN, absFix2ndOn);

            /* Disable auto battery transitions */
            data |= VP880_AUTO_BAT_SHUTDOWN_EN;
        } else {
            /* Enable auto battery transitions */
            data &= ~VP880_AUTO_BAT_SHUTDOWN_EN;
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
            VP880_SS_CONFIG_LEN, &data);

            pLineObj->onHookTicks = -1;

        /* Complete all other non device senstive items */

        /* Set to Disconnect */
        Vp880SetLineState(pLineCtx, VP_LINE_DISCONNECT);

        /* Initialize default values for Ringing */
        for (ringingParamCount = 0;
             ringingParamCount < VP880_RINGER_PARAMS_LEN;
             ringingParamCount++) {
            pLineObj->ringingParams[ringingParamCount] =
                defaultRingParams[ringingParamCount];
        }
        pLineObj->status &= ~(VP880_UNBAL_RINGING);

        /* Set Default Loop Supervision */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
            VP880_LOOP_SUP_LEN, defaultLoopSup);

        /* Set Default DC Feed */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
            VP880_DC_FEED_LEN, defaultDcFeed);

        if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] > VP880_REV_VC) {
            uint8 converterCfg[VP880_CONV_CFG_LEN] =
                {VP880_METALLIC_AC_V | VP880_CC_500HZ_RATE};
            uint8 deviceMode[VP880_DEV_MODE_LEN];
            /* store the pcm buffer update rate */
            pDevObj->txBufferDataRate = VP880_CC_500HZ_RATE;

            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD,
                VP880_DEV_MODE_LEN, deviceMode);
            deviceMode[0] |= VP880_DEV_MODE_TEST_DATA;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT,
                VP880_DEV_MODE_LEN, deviceMode);
        }

        status = Vp880ConfigLine(pLineCtx, pAcProfile, pDcOrFxoProfile,
            pRingProfile);
        if (status != VP_STATUS_SUCCESS) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return status;
        }
    }

    status = VpImplementDefaultSettings(VP_NULL, pLineCtx);

    /* Post the line init complete event if status is succesfull */
    if (status == VP_STATUS_SUCCESS) {
        pLineObj->lineEvents.response |= VP_LINE_EVID_LINE_INIT_CMP;
    }
    
    pLineObj->relayState = VP_RELAY_NORMAL;

    InitCadenceVars(pLineCtx);
    InitTimerVars(pLineCtx);

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp880InitLine */

/**
 * Vp880ConfigLine
 *  This function reloads a line of a device with the specified parameters.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to be the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 */
VpStatusType
Vp880ConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,    /**< Pointer to AC coefficient data or
                                     * profile index to be applied to this line.
                                     */

    VpProfilePtrType pDcOrFxoProfile,   /**< Pointer to DC Feed (FXS) or Cfg
                                         * (FX0) profile or profile index to be
                                         * applied to this line.
                                         */

    VpProfilePtrType pRingProfile)  /**< Pointer to Ringing profile or profile
                                     * index to apply to this line
                                     */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 loopSuperParams, profileIndex;
    uint8 sysStateConfig[VP880_SS_CONFIG_LEN];

    VpProfileDataType *pMpiData;

    VpProfilePtrType pAcProf = VP_PTABLE_NULL; 
    VpProfilePtrType pDcFxoCfgProf = VP_PTABLE_NULL; 
    VpProfilePtrType pRingProf = VP_PTABLE_NULL; 

    int profIndex;
    uint8 ringTypeByte;

    uint8 data, ringingParamCount;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 opNote4Step1Data[VP880_ICR2_LEN];
    uint8 opNote4Step2Data[VP880_ICR3_LEN];
    uint8 gainCSD[VP880_GR_GAIN_LEN];
    
    uint8 fxoLoopThreshLm[VP880_LOOP_SUP_LEN] = {0x1C, 0xE1, 0x85, 0xFF};
    
    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    profIndex = GetProfileIndex(pAcProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pAcProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_AC, pAcProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pAcProf = pAcProfile;
    } else if (profIndex < VP_CSLAC_AC_PROF_TABLE_SIZE) {
        pAcProf = pDevObj->devProfileTable.pAcProfileTable[profIndex];
        if (!(pDevObj->profEntry.acProfEntry & (0x01 << profIndex))) {
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    profIndex = GetProfileIndex(pDcOrFxoProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pDcOrFxoProfile != VP_PTABLE_NULL) {
            if (pLineObj->status & VP880_IS_FXO) {
                if (VpVerifyProfileType(VP_PROFILE_FXO_CONFIG, pDcOrFxoProfile)
                     != TRUE) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                if (VpVerifyProfileType(VP_PROFILE_DC, pDcOrFxoProfile) 
                    != TRUE) {
                    return VP_STATUS_ERR_PROFILE;                    
                }
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pDcFxoCfgProf = pDcOrFxoProfile;
    } else {
        if (pLineObj->status & VP880_IS_FXO) {
            if (profIndex < VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE) {
                pDcFxoCfgProf = 
                    pDevObj->devProfileTable.pFxoConfigProfileTable[profIndex];

                if (!(pDevObj->profEntry.fxoConfigProfEntry 
                    & (0x01 << profIndex))) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                return VP_STATUS_ERR_PROFILE;
            }
        } else {
            if (profIndex < VP_CSLAC_DC_PROF_TABLE_SIZE) {
                pDcFxoCfgProf = 
                    pDevObj->devProfileTable.pDcProfileTable[profIndex];

                if (!(pDevObj->profEntry.dcProfEntry & (0x01 << profIndex))) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                return VP_STATUS_ERR_PROFILE;            
            }
        }
    }

    profIndex = GetProfileIndex(pRingProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pRingProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_RING, pRingProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pRingProf = pRingProfile;
    } else if (profIndex < VP_CSLAC_RINGING_PROF_TABLE_SIZE) {
        pRingProf = pDevObj->devProfileTable.pRingingProfileTable[profIndex];
        if (!(pDevObj->profEntry.ringingProfEntry & (0x01 << profIndex))) {
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Load AC Coefficients */
    if (pAcProf != VP_PTABLE_NULL) {
        profileIndex = VP_PROFILE_MPI_LEN + 1;
        pMpiData = (VpProfileDataType *)(&pAcProfile[profileIndex]);
        VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
            pAcProfile[VP_PROFILE_MPI_LEN], pMpiData);

        /* Operating Functions - Use loaded coefficients */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_RD,
            VP880_OP_FUNC_LEN, &data);
        data |= VP880_ENABLE_LOADED_COEFFICIENTS;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_WRT,
            VP880_OP_FUNC_LEN, &data);

        /* Update cached transmit and receive gains for SetRelGain */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GX_GAIN_RD,
            VP880_GX_GAIN_LEN, gainCSD);
        pLineObj->gain.gxInt = 0x4000 + VpConvertCsd2Fixed(gainCSD);

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GR_GAIN_RD,
            VP880_GR_GAIN_LEN, gainCSD);
        pLineObj->gain.grInt = VpConvertCsd2Fixed(gainCSD);
    }

    if (pLineObj->status & VP880_IS_FXO) {
        /* Configure an FXO line type */
        if (pDcFxoCfgProf == VP_PTABLE_NULL) {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                VP880_LOOP_SUP_LEN, fxoLoopThreshLm);
        } else {
            profileIndex = VP_FXO_DIALING_PROFILE_DISC_VOLTAGE_MIN;
            loopSuperParams = (pDcFxoCfgProf[profileIndex] << 3);
            loopSuperParams &= 0x38;
            fxoLoopThreshLm[0] &= ~(0x38);
            fxoLoopThreshLm[0] |= loopSuperParams;

            profileIndex = VP_FXO_DIALING_PROFILE_RING_VOLTAGE_MIN;
            loopSuperParams = (pDcFxoCfgProf[profileIndex] & 0x07);
            fxoLoopThreshLm[0] &= ~(0x07);
            fxoLoopThreshLm[0] |= loopSuperParams;

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MIN;
            fxoLoopThreshLm[2] = pDcFxoCfgProf[profileIndex];

            /*
             * Cache the Minimum Ringing Detect Period that is implemented in
             * SW.
             */
            pLineObj->ringDetMin = pDcFxoCfgProf[profileIndex];
            pLineObj->ringDetMin /= 4;

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX_ACT;
            pLineObj->ringDetMax = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX;
            fxoLoopThreshLm[3] = pDcFxoCfgProf[profileIndex];

            if (pLineObj->ringDetMax == 0) {
                pLineObj->ringDetMax = fxoLoopThreshLm[3] / 4;
            }

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_MSB;
            pLineObj->digitGenStruct.dtmfOnTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_LSB;
            pLineObj->digitGenStruct.dtmfOnTime |=  pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_MSB;
            pLineObj->digitGenStruct.dtmfOffTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_LSB;
            pLineObj->digitGenStruct.dtmfOffTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_BREAK;
            pLineObj->digitGenStruct.breakTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_MAKE;
            pLineObj->digitGenStruct.makeTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_MSB;
            pLineObj->digitGenStruct.flashTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_LSB;
            pLineObj->digitGenStruct.flashTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_MSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_LSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                pDcFxoCfgProf[profileIndex];

            VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                VP880_LOOP_SUP_LEN, fxoLoopThreshLm);
        }

        /* Cache this so we don't have to read it all the time */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_RD,
            VP880_LOOP_SUP_LEN, fxoLoopThreshLm);
        pLineObj->lineTimers.timers.fxoTimer.maxPeriod = fxoLoopThreshLm[3];
    } else {
        /* Configure an FXS line type */

        /* Ringing changed if profile passed */
        if (pRingProf != VP_PTABLE_NULL) {
            /*
             * Ringing Profile May affect the system state register, so read
             * what it is before the profile, and set it back to all values
             * except what can change in the profile
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                VP880_SS_CONFIG_LEN, sysStateConfig);

            profileIndex = VP_PROFILE_MPI_LEN + 1;
            pMpiData = (VpProfileDataType *)&pRingProf[profileIndex];
            VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
                pRingProf[VP_PROFILE_MPI_LEN], pMpiData);

            for (ringingParamCount = 0;
                 ringingParamCount < VP880_RINGER_PARAMS_LEN;
                 ringingParamCount++) {
                pLineObj->ringingParams[ringingParamCount] =
                    pRingProf[profileIndex+1+ringingParamCount];
            }
            ringTypeByte = pRingProf[VP_PROFILE_MPI_LEN + 
                pRingProf[VP_PROFILE_MPI_LEN] + VP_PROFILE_RING_TYPE_OFFSET];

            pLineObj->status &= ~(VP880_UNBAL_RINGING);
            pLineObj->status |= (ringTypeByte ? VP880_UNBAL_RINGING : 0x0000);               

            /*
             * Nothing in this register should be allowed to change, but the
             * Ringing profile may have changed this value to be compatible
             * with other device profiles. So correct it.
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                VP880_SS_CONFIG_LEN, sysStateConfig);
        }

        /* Implement ICR2 workaround for ABS version VA only */
        if ((pDevObj->stateInt & VP880_IS_ABS) &&
            (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
                opNote4Step1Data);
            opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX]
                |= VP880_ICR2_MET_SPEED_CTRL;
            opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX+1]
                &= ~VP880_ICR2_MET_SPEED_CTRL;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
                opNote4Step1Data);
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_RD, VP880_ICR3_LEN,
            opNote4Step2Data);
        opNote4Step2Data[VP880_ICR3_LONG_FIXED_INDEX] |= VP880_ICR3_LONG_FIXED;
        opNote4Step2Data[VP880_ICR3_LONG_FIXED_INDEX+1]
            &= ~VP880_ICR3_LONG_FIXED;

        /* Set Default Loop Supervision and DC Feed if one is not provided */
        if (pDcFxoCfgProf != VP_PTABLE_NULL) {
            profileIndex = VP_PROFILE_MPI_LEN + 1;
            pMpiData = (VpProfileDataType *)&pDcFxoCfgProf[profileIndex];

            /* 
             * If the VOC is (being set >= 48V AND pre-ver VC) OR (being set >
             * 48V AND VC or later) we have to perform a few addtl. commands
             * before programming the new value
             */    

            if ((pDevObj->stateInt & VP880_IS_ABS) &&
                (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
            if ((pDcFxoCfgProf[VP880_VOC_PROFILE_POSITION] & 0x1C) >= 0x10) {
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        |= VP880_ICR3_LONG_UNCLAMP;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX+1]
                        |= VP880_ICR3_LONG_UNCLAMP;
                }
            } else {
                if ((pDcFxoCfgProf[VP880_VOC_PROFILE_POSITION] & 0x1C) >= 0x10) {
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        |= VP880_ICR3_LONG_UNCLAMP;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX+1]
                        |= VP880_ICR3_LONG_UNCLAMP;

                    if (!(pDevObj->stateInt & VP880_IS_ABS)) {
                        /* Tracker Workaround */
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                            VP880_ICR2_LEN, opNote4Step1Data);
                        opNote4Step1Data[VP880_ICR2_SWY_LIM_INDEX]
                            |= VP880_ICR2_SWY_LIM_CTRL;
                        opNote4Step1Data[VP880_ICR2_SWY_LIM_INDEX+1]
                            &= ~VP880_ICR2_SWY_LIM_CTRL;
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                            VP880_ICR2_LEN, opNote4Step1Data);
                    }
                }
            }
               
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT, VP880_ICR3_LEN,
                opNote4Step2Data);
            VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
                pDcFxoCfgProf[VP_PROFILE_MPI_LEN], pMpiData);
        } else {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT, VP880_ICR3_LEN,
                opNote4Step2Data);
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880InitProfile()
 *  This function is used to initialize profile tables in Vp880.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  Stores the given profile at the specified index of the profile table.
 */
VpStatusType
Vp880InitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;

    uint8 profIndex8;   /* Used for 8-bit profile table masking */
    uint16 profIndex16; /* Used for 16-bit profile table masking */

    /*
     * If the profile data is an index, indicated by Get Profile Index return
     * value of > -1, return an error (cannot init an indexed entry with an
     * index).
     */
    int profileIndex = GetProfileIndex(pProfile);

    if (profileIndex >= 0) {
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If pProfileIndex is -1, the profile is of pointer type and invalid,
     * otherwise it is an index.  If it's an index, make sure the range is
     * valid.
     */
    profileIndex = GetProfileIndex(pProfileIndex);
    if (profileIndex < 0) {
        return VP_STATUS_INVALID_ARG;
    }

    profIndex8 = (uint8)profileIndex;
    profIndex16 = (uint16)profileIndex;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    /*
     * The correct types are passed, but check to make sure the specific profile
     * type being initialized is valid as well as the index value
     */
    switch(type) {
        case VP_PROFILE_DEVICE:
            if (profIndex8 >= VP_CSLAC_DEV_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_DEVICE, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pDevProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.devProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.devProfEntry |= 
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_AC:
            if (profIndex8 >= VP_CSLAC_AC_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_AC, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pAcProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.acProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.acProfEntry |= 
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_DC:
            if (profIndex8 >= VP_CSLAC_DC_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_DC, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pDcProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.dcProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.dcProfEntry |= 
                            (0x01 << profIndex8);
                    }    
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_RING:
            if (profIndex8 >= VP_CSLAC_RINGING_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_RING, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pRingingProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.ringingProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.ringingProfEntry |= 
                            (0x01 << profIndex8);
                    }        
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_RINGCAD:
            if (profIndex8 >= VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_RINGCAD, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pRingingCadProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.ringCadProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.ringCadProfEntry |= 
                            (0x01 << profIndex8);
                    }        
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_TONE:
            if (profIndex16 >= VP_CSLAC_TONE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_TONE, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pToneProfileTable[profIndex16] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.toneProfEntry &= 
                            ~(0x01 << profIndex16);
                    } else {
                        pDevObj->profEntry.toneProfEntry |= 
                            (0x01 << profIndex16);
                    }        
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_TONECAD:
            if (profIndex16 >= VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_TONECAD, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pToneCadProfileTable[profIndex16] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.toneCadProfEntry &= 
                            ~(0x01 << profIndex16);
                    } else {
                        pDevObj->profEntry.toneCadProfEntry |= 
                            (0x01 << profIndex16);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_METER:
            if (profIndex8 >= VP_CSLAC_METERING_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_METER, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pMeteringProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.meterProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.meterProfEntry |= 
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_CID:
            if (profIndex8 >= VP_CSLAC_CALLERID_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_CID, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pCallerIdProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.cidCadProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.cidCadProfEntry |= 
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_FXO_CONFIG:
            if (profIndex8 >= VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_FXO_CONFIG, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pFxoConfigProfileTable[profIndex8] =
                        pProfile;
                    /* 
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no 
                     * longer valid.
                     */    
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.fxoConfigProfEntry &= 
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.fxoConfigProfEntry |= 
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        default:
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* Vp880InitProfile() */

/**
 * Vp880InitRing()
 *  This function is used to initialize the ringing profile and caller ID
 * cadence on a given line.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to by the line context passed is initialized with the
 * ringing and caller ID profile specified.  The profiles may be specified as
 * either an index into the devic profile table or by profile pointers. This
 * function returns the success code if the device has been initialized and both
 * indexes (if indexes are passed) are within the range of the device profile
 * table.
 */
VpStatusType
Vp880InitRing(
    VpLineCtxType *pLineCtx,        /**< Line Context to modify Ringing
                                     * Parameters for
                                     */
    VpProfilePtrType pCadProfile,   /**< Pointer of a Ringing Cadence profile,
                                     * or the index into the Ringing Cadence
                                     * profile table.
                                     */
    VpProfilePtrType pCidProfile)   /**< Pointer of a Caller ID profile, or the
                                     * index into the Caller ID profile table.
                                     */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpProfilePtrType *pProfileTable;

    int cadIndex = GetProfileIndex(pCadProfile);
    int cidIndex = GetProfileIndex(pCidProfile);

    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP880_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * If the profile passed is an index, make sure it's in the valid range
     * and if so, set the currently used profile to it.
     */
    if ((cadIndex >= 0) && (cadIndex < VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE)) {
        /* Valid Cadence index.  Set it if it's not an invalid table entry */
        if (!(pDevObj->profEntry.ringCadProfEntry & (0x01 << cadIndex))) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);        
            return VP_STATUS_ERR_PROFILE;
        }
        
        pProfileTable = pDevObj->devProfileTable.pRingingCadProfileTable;
        pLineObj->pRingingCadence = pProfileTable[cadIndex];
    } else if (cadIndex >= VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE) {
        /* It's an index, but it's out of range */
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    } else {
        /* This is a pointer. Set it if it's the correct type */
        if(VpVerifyProfileType(VP_PROFILE_RINGCAD, pCadProfile) == TRUE) {
            pLineObj->pRingingCadence = pCadProfile;
        } else {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_PROFILE;
        }
    }
    
    /* Setup Caller ID profile like Ringing Cadence. */
    if ((cidIndex >= 0) && (cidIndex < VP_CSLAC_CALLERID_PROF_TABLE_SIZE)) {
        /* Valid Caller ID index.  Set it if it's not an invalid table entry */
        if (!(pDevObj->profEntry.cidCadProfEntry & (0x01 << cidIndex))) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);        
            return VP_STATUS_ERR_PROFILE;
        }

        pProfileTable = pDevObj->devProfileTable.pCallerIdProfileTable;
        pLineObj->pCidProfileType1 = pProfileTable[cidIndex];
    } else {
        if (cidIndex > VP_CSLAC_CALLERID_PROF_TABLE_SIZE) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG;
        } else {
            /* This is a pointer. Set it if it's the correct type */
            if(VpVerifyProfileType(VP_PROFILE_CID, pCidProfile) == TRUE) {
                pLineObj->pCidProfileType1 = pCidProfile;
            } else {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_ERR_PROFILE;
            }
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp880InitRing */

/**
 * Vp880InitCid()
 *  This function is used to send caller ID information. It accepts an amount of
 * CID message data up to a "full" buffer amount (2 times the amount of the 
 * size used for ContinueCID). It low fills the primary buffer such that the
 * application is interrupted at the earliest time when the API is ready to 
 * accept more data.
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
Vp880InitCid(
    VpLineCtxType *pLineCtx, 
    uint8 length, 
    uint8p pCidData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 primaryByteCount, secondaryByteCount;
    
    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pLineObj->status & VP880_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    if (length > (2 * VP_SIZEOF_CID_MSG_BUFFER)) {
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->callerId.cliIndex = 0;
    pLineObj->callerId.cliMPIndex = 0;
    pLineObj->callerId.cliMSIndex = 0;    
                 
    /* Stop CID if it was in progress */
    pLineObj->callerId.cliTimer = 0;    
    pLineObj->callerId.status = VP_CID_RESET_VALUE;
    pLineObj->callerId.dtmfStatus = VP_CID_DTMF_RESET_VALUE;
    pLineObj->callerId.cidCheckSum = 0;
    
    /* 
     * If length is within the size of just the primary buffer size, then only
     * fill the primary buffer. Otherwise (the length exceeds the size of the
     * primary buffer size) "low fill" the primary buffer and max fill the 
     * secondary buffer. This has the affect of causing a CID Data event 
     * quickly and giving the application a maximum amount of time to refill
     * the message buffer
     */ 
    if (length <= VP_SIZEOF_CID_MSG_BUFFER) {
        pLineObj->callerId.primaryMsgLen = length;
        pLineObj->callerId.secondaryMsgLen = 0;
    } else {
        pLineObj->callerId.primaryMsgLen = (length - VP_SIZEOF_CID_MSG_BUFFER);
        pLineObj->callerId.secondaryMsgLen = VP_SIZEOF_CID_MSG_BUFFER;
    }

    /* 
     * Copy the message data to the primary API buffer. If we're here, there's
     * at least one byte of primary message data. So a check is not necessary
     */
    pLineObj->callerId.status |= VP_CID_PRIMARY_FULL;
    for (primaryByteCount = 0; 
         (primaryByteCount < pLineObj->callerId.primaryMsgLen);
         primaryByteCount++) {
        pLineObj->callerId.primaryBuffer[primaryByteCount] 
            = pCidData[primaryByteCount];
        pLineObj->callerId.cidCheckSum += pCidData[primaryByteCount];
        pLineObj->callerId.cidCheckSum = pLineObj->callerId.cidCheckSum % 256;
    }

    /* Copy the message data to the secondary API buffer if there is any */
    if (pLineObj->callerId.secondaryMsgLen > 0) {
        pLineObj->callerId.status |= VP_CID_SECONDARY_FULL;
        for (secondaryByteCount = 0; 
             (secondaryByteCount < pLineObj->callerId.secondaryMsgLen);
             secondaryByteCount++) {
            pLineObj->callerId.secondaryBuffer[secondaryByteCount] = 
                pCidData[secondaryByteCount + primaryByteCount];
            pLineObj->callerId.cidCheckSum += 
                pCidData[secondaryByteCount + primaryByteCount];                
            pLineObj->callerId.cidCheckSum = 
                pLineObj->callerId.cidCheckSum % 256;
        }    
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}
#endif






