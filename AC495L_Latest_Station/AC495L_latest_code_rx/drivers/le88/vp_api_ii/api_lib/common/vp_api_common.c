/** \file vp_api_common.c
 * vp_api_common.c
 *
 *  This file contains functions that are common to more than one device type
 * but not accessible to the user
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#include "vp_api_cfg.h"

/* INCLUDES */
#include "vp_api.h"     /* Typedefs and function prototypes for API */
#include "vp_api_int.h" /* Device specific typedefs and function prototypes */

/* Functions used internal to the API -- device dependant */
#if defined (VP_CC_790_SERIES) || defined (VP_CC_880_SERIES) \
 || defined (VP_CC_580_SERIES)

static VpStatusType
VpSeq(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pProfile);

static bool
VpFSKGeneratorReady(
    VpLineCtxType *pLineCtx);

static bool
VpDTMFGeneratorReady(
    VpLineCtxType *pLineCtx);

static VpStatusType
VpCtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

static bool
VpCliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

static void
VpCtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 digit);

static VpDigitType
VpConvertCharToDigitType(
    char digit);

static void
VpCtrlSetDTMFGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);

static void
VpCtrlMuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

static VpStatusType
VpBranchInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

static VpStatusType
VpTimeInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);
#endif

/**
 * ConvertPrfWizPrfType2ApiType()
 *  This function converts from Profile Wizard profile type to VP-API specific
 * profile type.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
VpProfileType
ConvertPrfWizPrfType2ApiType(
    const VpProfileWizProfileType type)   /* Profile to be converted */
{
    switch (type) {
        case VP_PRFWZ_PROFILE_AC:              return VP_PROFILE_AC;
        case VP_PRFWZ_PROFILE_DC:              return VP_PROFILE_DC;
        case VP_PRFWZ_PROFILE_TONE:            return VP_PROFILE_TONE;
        case VP_PRFWZ_PROFILE_TONECAD:         return VP_PROFILE_TONECAD;
        case VP_PRFWZ_PROFILE_RING:            return VP_PROFILE_RING;
        case VP_PRFWZ_PROFILE_CID_TYPE1:       return VP_PROFILE_CID;
        case VP_PRFWZ_PROFILE_CID_TYPE2:       return VP_PROFILE_CID;
        case VP_PRFWZ_PROFILE_METER:           return VP_PROFILE_METER;
        case VP_PRFWZ_PROFILE_RINGCAD:         return VP_PROFILE_RINGCAD;
        case VP_PRFWZ_PROFILE_TEST_CRITERIA:   return VP_PROFILE_TEST_CRITERIA;
        case VP_PRFWZ_PROFILE_TEST_TOPOLOGY:   return VP_PROFILE_TEST_TOPOLOGY;
        case VP_PRFWZ_PROFILE_DEVICE:          return VP_PROFILE_DEVICE;
        case VP_PRFWZ_PROFILE_FXO_CONFIG:      return VP_PROFILE_FXO_CONFIG;
        case VP_PRFWZ_PROFILE_FXS_CTRL:        return VP_PROFILE_CUSTOM_TERM;
        default:                               return VP_NUM_PROFILE_TYPES;
    }
} /* ConvertPrfWizPrfType2ApiType() */

/**
 * GetProfileIndex()
 *  This function returns TRUE if the passed profile pointer is an index or just
 * a normal pointer. If the passed profile pointer is an index then the index is
 * also returned.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
int
GetProfileIndex (
    const VpProfilePtrType pProfile) /* Given Profile pointer */
{
    if((pProfile >= VP_PTABLE_INDEX1) && (pProfile <= VP_PTABLE_INDEX15)){
        if(pProfile == VP_PTABLE_INDEX1) {return 0;}
        else if(pProfile == VP_PTABLE_INDEX2) {return 1;}
        else if(pProfile == VP_PTABLE_INDEX3) {return 2;}
        else if(pProfile == VP_PTABLE_INDEX4) {return 3;}
        else if(pProfile == VP_PTABLE_INDEX5) {return 4;}
        else if(pProfile == VP_PTABLE_INDEX6) {return 5;}
        else if(pProfile == VP_PTABLE_INDEX7) {return 6;}
        else if(pProfile == VP_PTABLE_INDEX8) {return 7;}
        else if(pProfile == VP_PTABLE_INDEX9) {return 8;}
        else if(pProfile == VP_PTABLE_INDEX10) {return 9;}
        else if(pProfile == VP_PTABLE_INDEX11) {return 10;}
        else if(pProfile == VP_PTABLE_INDEX12) {return 11;}
        else if(pProfile == VP_PTABLE_INDEX13) {return 12;}
        else if(pProfile == VP_PTABLE_INDEX14) {return 13;}
        else if(pProfile == VP_PTABLE_INDEX15) {return 14;}
    }
    return -1;
} /* GetProfileIndex() */

/**
 * VpVerifyProfileType()
 *  This function verifies that the profile pointer passed matches the type of
 * profile being passed.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  Returns TRUE if the profile type and profile match.  Otherwise returns
 * FALSE.  Note that a NULL profile is valid and has specific meanings in the
 * API-II depending on the profile.
 */
bool
VpVerifyProfileType(
    VpProfileType type,
    VpProfilePtrType pProfile)
{
    if (pProfile == VP_PTABLE_NULL) {
        return TRUE;
    } else if ((pProfile >= VP_PTABLE_INDEX1)
            && (pProfile <= VP_PTABLE_INDEX15)){
        /* This function does not expect to see profile indexes */
        return FALSE;
    }

    if (ConvertPrfWizPrfType2ApiType(pProfile[VP_PROFILE_TYPE_LSB]) != type) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * VpIsDigit()
 *  This function returns TRUE if the digit passed is a valid VpDigitType,
 * otherwise returns FALSE. Utility function for the API-II.
 */
bool
VpIsDigit(
    VpDigitType digit)
{
    if (/*(digit >= 0) &&*/ (digit <= 9)) {
        return TRUE;
    }
    
    switch(digit) {
        case VP_DIG_ZERO:
        case VP_DIG_ASTER:
        case VP_DIG_POUND:
        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
        case VP_DIG_NONE:
            return TRUE;
        default:
            return FALSE;
    }
}

/* Code used for CSLAC only */
#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES)

static VpStatusType
VpCtrlDetectDTMF(
    VpLineCtxType *pLineCtx,
    bool mode);

/** COMMON INITIALIZATION FUNCTIONS */
/**
 * VpImplementNonMaskEvents()
 *  This function modifies the line and device event structures with the API
 * standard non-masking event bits.  A non-masked event bit is 0.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  The event structures passed are modified by the non-masked event bits.
 */
void
VpImplementNonMaskEvents(
    VpOptionEventMaskType *pLineEventsMask, /**< Line Events Mask to modify for
                                             * non-masking
                                             */
    VpOptionEventMaskType *pDevEventsMask)  /**< Device Events Mask to modify
                                             * for non-masking
                                             */
{
    pLineEventsMask->faults &= ~VP_API_NONMASK_FAULT_EVENTS;
    pLineEventsMask->signaling &= ~VP_API_NONMASK_SIGNALING_EVENTS;
    pLineEventsMask->response &= ~VP_API_NONMASK_RESPONSE_EVENTS;
    pLineEventsMask->test &= ~VP_API_NONMASK_TEST_EVENTS;
    pLineEventsMask->process &= ~VP_API_NONMASK_PROCESS_EVENTS;
    pLineEventsMask->fxo &= ~VP_API_NONMASK_FXO_EVENTS;
    pLineEventsMask->packet &= ~VP_API_NONMASK_PACKET_EVENTS;

    pDevEventsMask->faults &= ~VP_API_NONMASK_FAULT_EVENTS;
    pDevEventsMask->signaling &= ~VP_API_NONMASK_SIGNALING_EVENTS;
    pDevEventsMask->response &= ~VP_API_NONMASK_RESPONSE_EVENTS;
    pDevEventsMask->test &= ~VP_API_NONMASK_TEST_EVENTS;
    pDevEventsMask->process &= ~VP_API_NONMASK_PROCESS_EVENTS;
    pDevEventsMask->fxo &= ~VP_API_NONMASK_FXO_EVENTS;
    pDevEventsMask->packet &= ~VP_API_NONMASK_PACKET_EVENTS;

    return;
}

/**
 * VpImplementDefaultSettings()
 *  This function executes the options to set the device/lines to API-II
 * standard default settings.  It may be passed a valid device context, or a
 * valid line context.  The device and line context do not need to be associated
 * with each other.  This is a convenient function for the API itself to use
 * when a device or line is initialized.
 *
 * Preconditions:
 * None
 *
 * Postconditions:
 * The device and line associated with this device is initialized with default
 * values.
 */
VpStatusType
VpImplementDefaultSettings(
    VpDevCtxType *pDevCtx,      /**< Device to implement for default API-II
                                 * options
                                 */
    VpLineCtxType *pLineCtx)    /**< Line to implement for default API-II
                                 * options
                                 */
{
    VpStatusType status = VP_STATUS_SUCCESS;

    VpSetOptionFuncPtrType pSetOption = VP_NULL;
    VpOptionPulseType pulseSpec;
    VpOptionPulseType pulseSpec2;
    VpOptionCriticalFltType criticalFault;
    VpOptionZeroCrossType zeroCross;
    VpOptionPulseModeType pulseMode;            
    VpOptionCodecType codec;
    VpOptionPcmHwyType pcmHwy;
    VpOptionLoopbackType loopBack;
    VpOptionLineStateType lineState;
    VpOptionRingControlType ringCtrl;
    VpOptionPcmTxRxCntrlType pcmTxRxCtrl;
    
    VpOptionEventMaskType eventMask;

    if ((pDevCtx == VP_NULL) && (pLineCtx == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    if(pDevCtx != VP_NULL) {
        pSetOption = pDevCtx->funPtrsToApiFuncs.SetOption;
    } else {
        pSetOption = pLineCtx->pDevCtx->funPtrsToApiFuncs.SetOption;
    }

    if (pSetOption == VP_NULL) {
        return VP_STATUS_FUNC_NOT_SUPPORTED;
    }

    pulseSpec.breakMin = VP_OPTION_DEFAULT_DP_BREAK_MIN;
    pulseSpec.breakMax = VP_OPTION_DEFAULT_DP_BREAK_MAX;
    pulseSpec.makeMin = VP_OPTION_DEFAULT_DP_MAKE_MIN;
    pulseSpec.makeMax = VP_OPTION_DEFAULT_DP_MAKE_MAX;
    pulseSpec.interDigitMin = VP_OPTION_DEFAULT_DP_INTER_DIG_MIN;
    pulseSpec.flashMin = VP_OPTION_DEFAULT_DP_FLASH_MIN;
    pulseSpec.flashMax = VP_OPTION_DEFAULT_DP_FLASH_MAX;

    pulseSpec2.breakMin = VP_OPTION_DEFAULT_DP_BREAK_MIN2;
    pulseSpec2.breakMax = VP_OPTION_DEFAULT_DP_BREAK_MAX2;
    pulseSpec2.makeMin = VP_OPTION_DEFAULT_DP_MAKE_MIN2;
    pulseSpec2.makeMax = VP_OPTION_DEFAULT_DP_MAKE_MAX2;
    pulseSpec2.interDigitMin = VP_OPTION_DEFAULT_DP_INTER_DIG_MIN2;
    pulseSpec2.flashMin = VP_OPTION_DEFAULT_DP_FLASH_MIN2;
    pulseSpec2.flashMax = VP_OPTION_DEFAULT_DP_FLASH_MAX2;


    pulseMode = VP_OPTION_DEFAULT_PULSE_MODE;

    criticalFault.acFltDiscEn = VP_OPTION_DEFAULT_CF_AC_DIS_EN;
    criticalFault.dcFltDiscEn = VP_OPTION_DEFAULT_CF_DC_DIS_EN;
    criticalFault.thermFltDiscEn = VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN;

    zeroCross = VP_OPTION_DEFAULT_ZERO_CROSS;

    codec = VP_OPTION_DEFAULT_CODEC_MODE;
    pcmHwy = VP_OPTION_DEFAULT_PCM_HWY;

    pcmTxRxCtrl = VP_OPTION_DEFAULT_PCM_TXRX_CNTRL;

    loopBack = VP_OPTION_DEFAULT_LOOP_BACK;
    lineState.bat = VP_OPTION_DEFAULT_LS_BAT;
    lineState.battRev = VP_OPTION_DEFAULT_LS_BAT_REV;
    
    eventMask.faults = VP_OPTION_DEFAULT_FAULT_EVENT_MASK;
    eventMask.signaling = VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK;
    eventMask.response = VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK;
    eventMask.test = VP_OPTION_DEFAULT_TEST_EVENT_MASK;
    eventMask.process = VP_OPTION_DEFAULT_PROCESS_EVENT_MASK;
    eventMask.fxo = VP_OPTION_DEFAULT_FXO_EVENT_MASK;
    eventMask.packet = VP_OPTION_DEFAULT_PACKET_EVENT_MASK;

    ringCtrl.ringExitDbncDur = VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL;
    ringCtrl.ringTripExitSt = VP_OPTION_DEFAULT_RC_RING_EXIT_STATE;
    ringCtrl.zeroCross = VP_OPTION_DEFAULT_RC_ZERO_CROSS; 
    
    pcmTxRxCtrl = VP_OPTION_DEFAULT_PCM_TXRX_CNTRL;

    if (pDevCtx != VP_NULL) {
        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PULSE, 
            &pulseSpec);
        if ((status != VP_STATUS_SUCCESS) &&
            (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }

        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PULSE2,
            &pulseSpec2);
        if ((status != VP_STATUS_SUCCESS) &&
            (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    
            
        /* 
         * Some devices do not support AC/DC Fault detection, so setting the
         * critical fault may not be successful. However, all devices (known)
         * support thermal fault detection, so set that to the default
         */
        status = pSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_CRITICAL_FLT, 
            &criticalFault);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            criticalFault.acFltDiscEn = FALSE;
            criticalFault.dcFltDiscEn = FALSE;
            status = pSetOption(VP_NULL, pDevCtx, 
                VP_DEVICE_OPTION_ID_CRITICAL_FLT, &criticalFault);
            if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
                return status;
            }    
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PULSE_MODE, 
            &pulseMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_CODEC, &codec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PCM_HWY, &pcmHwy);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_LOOPBACK, &loopBack);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_LINE_STATE, 
            &lineState);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_EVENT_MASK, 
            &eventMask);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_RING_CNTRL, 
            &ringCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PCM_TXRX_CNTRL, 
            &pcmTxRxCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    
    }

    if (pLineCtx != VP_NULL) {
        /* Init only line level options */
        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PULSE_MODE, 
            &pulseMode);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_CODEC, &codec);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PCM_HWY, &pcmHwy);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_LOOPBACK, 
            &loopBack);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_LINE_STATE, 
            &lineState);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_RING_CNTRL, 
            &ringCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    

        status = pSetOption(pLineCtx, VP_NULL, VP_OPTION_ID_PCM_TXRX_CNTRL, 
            &pcmTxRxCtrl);
        if ((status != VP_STATUS_SUCCESS) && (status != VP_STATUS_OPTION_NOT_SUPPORTED)) {
            return status;
        }    
    }

    return VP_STATUS_SUCCESS;
}

/**
 * ConvertApiState2PrfWizState()
 *
 *    Maps an API-II line state into the equivelent state value used in cadence
 *    profiles.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  None
 */
uint8
ConvertApiState2PrfWizState(
    const VpLineStateType state)
{
    VpProfileCadencerStateTypes conversion[] = {
        VP_PROFILE_CADENCE_STATE_STANDBY, VP_PROFILE_CADENCE_STATE_TIP_OPEN,
        VP_PROFILE_CADENCE_STATE_ACTIVE, VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE,
        VP_PROFILE_CADENCE_STATE_TALK, VP_PROFILE_CADENCE_STATE_POLREV_TALK,
        VP_PROFILE_CADENCE_STATE_OHT, VP_PROFILE_CADENCE_STATE_POLREV_OHT,
        VP_PROFILE_CADENCE_STATE_DISCONNECT, VP_PROFILE_CADENCE_STATE_RINGING,
        VP_PROFILE_CADENCE_STATE_POLREV_RINGING,
        VP_PROFILE_CADENCE_STATE_FXO_OHT, VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN,
        VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE,
        VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK,
        VP_PROFILE_CADENCE_STATE_POLREV_STANDBY
    };

    return (uint8)(conversion[state]);
} /* ConvertApiState2PrfWizState() */

/**
 * InitCadenceVars()
 *  This function initializes the Cadence (sequencer) Variables in the line
 * object associated with the line context passed.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  The VpSeqDataType structure variables passed in the line context are
 * initialized to pre-determined values.
 */
void
InitCadenceVars(
    VpLineCtxType *pLineCtx)    /**< Line to initialize API Cadence (Sequencer)
                                 * Variables for
                                 */
{
    VpDeviceType deviceType = pLineCtx->pDevCtx->deviceType;
    void *pLineObj = pLineCtx->pLineObj;
    VpSeqDataType *pCadence;
    uint8 countLoop;

    switch(deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            break;
#endif
        default:
            /* Nothing known to initialize */
            return;
    }

    pCadence->pActiveCadence = VP_PTABLE_NULL;
    pCadence->pCurrentPos = VP_PTABLE_NULL;
    pCadence->status = VP_CADENCE_RESET_VALUE;    /* No active status */
    pCadence->branchAt = 0;
    pCadence->index = 0;
    pCadence->length = 0;

    for (countLoop = 0; countLoop < VP_CSLAC_MAX_BRANCH_DEPTH; countLoop++) {
        pCadence->count[countLoop] = 0;
    }

    pCadence->timeRemain = 0;
}

/**
 * InitTimerVars()
 *  This function initializes the Cadence (sequencer) Variables in the line
 * object associated with the line context passed.
 *
 * Preconditions:
 *  None
 *
 * Postconditions:
 *  The VpSeqDataType structure variables passed in the line context are
 * initialized to pre-determined values.
 */
void
InitTimerVars(
    VpLineCtxType *pLineCtx)    /**< Line to initialize API Timer (internal)
                                 * Variables for
                                 */
{
    uint8 i;
    VpDeviceType deviceType = pLineCtx->pDevCtx->deviceType;
    void *pLineObj = pLineCtx->pLineObj;
    VpCslacTimerStruct *pTimer;

    switch(deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pTimer = &((Vp790LineObjectType *)pLineObj)->lineTimers;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pTimer = &((Vp880LineObjectType *)pLineObj)->lineTimers;
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            pTimer = &((Vp580LineObjectType *)pLineObj)->lineTimers;
            break;
#endif
        default:
            /* Nothing known to initialize */
            return;
    }
    
    if (pTimer->type == VP_CSLAC_FXS_TIMER) {
        for (i = 0; i < VP_LINE_TIMER_LAST; i++) {
            pTimer->timers.timer[i] = 0;
        }
    } else {
        pTimer->timers.fxoTimer.highToLowTime = 0;
        pTimer->timers.fxoTimer.prevHighToLowTime = 0x7FFF;
        pTimer->timers.fxoTimer.noCount = TRUE;
        pTimer->timers.fxoTimer.timeLastPolRev = 0x7FFF;
        pTimer->timers.fxoTimer.timePrevPolRev = 0x7FFF;
        pTimer->timers.fxoTimer.lastStateChange = 0;
    }
}

/**
 * VpSeq()
 *  This function calls the appropriate sequencer function based on the current
 * position in the sequencer and the device type.
 *
 * Preconditions:
 *  The profile passed must be pointing to an instruction that is supported by
 * the device type.
 *
 * Postconditions:
 *  The instruction specified by the profile data is called.  The line context
 * is passed to the called function.  Note:  The line context may be valid or
 * VP_NULL, this function is not affected.  This function returns the success
 * code as long as the pointer is pointing to an instruction that is supported
 * by the device.
 */
VpStatusType
VpSeq(
    VpLineCtxType *pLineCtx,    /**< Line that has an active sequencer */
    VpProfilePtrType pProfile)  /**< Sequence profile, pointing to current
                                 * location in sequence, not typically the
                                 * starting address
                                 */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;

    /*
     * This function is passed a pointer that starts at the current position of
     * the cadence sequence, controlled by the API
     */
    if (pProfile == VP_NULL) {
        return VP_STATUS_INVALID_ARG;
    }

    switch(pProfile[0] & VP_SEQ_OPERATOR_MASK) {
        case VP_SEQ_SPRCMD_COMMAND_INSTRUCTION:
            switch(pDevCtx->deviceType) {
#if defined (VP_CC_880_SERIES)
                case VP_DEV_880_SERIES:
                    return Vp880CommandInstruction(pLineCtx, pProfile);
#endif

#if defined (VP_CC_790_SERIES)
                case VP_DEV_790_SERIES:
                    return Vp790CommandInstruction(pLineCtx, pProfile);
#endif

#if defined (VP_CC_580_SERIES)
                case VP_DEV_580_SERIES:
                    return Vp580CommandInstruction(pLineCtx, pProfile);
#endif

                default:
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_SEQ_SPRCMD_TIME_INSTRUCTION:
            return VpTimeInstruction(pLineCtx, pProfile);

        case VP_SEQ_SPRCMD_BRANCH_INSTRUCTION:
            return VpBranchInstruction(pLineCtx, pProfile);

        default:
            return VP_STATUS_INVALID_ARG;
    }
} /* VpSeq() */

/**
 * VpServiceSeq()
 *  This function tests the line status for an active sequence, and calls the
 * VpSeq function if there is an active sequence.  However, this function does
 * not check to see if the operation being pointed to by the current sequence is
 * supported.
 *
 * Preconditions:
 *  The device context cannot be VP_NULL, and only Vp790 and Vp880 currently
 * supported.
 *
 * Postconditions:
 *  If there is an active cadence that is supported by the line, then it is
 * called (via VpSeq).  If the current operation is not supported, the line
 * object active cadence is removed (i.e., set to inactive).
 */
bool
VpServiceSeq(
    VpDevCtxType *pDevCtx)  /**< Device that has a sequence.  The sequence may
                             * not be active
                             */
{
    uint8 channelId, maxChannels;
    VpDeviceInfoType deviceInfo;
    VpLineCtxType *pLineCtx;
    VpProfilePtrType pCurrentPos;
    void *pLineObj;

    /*
     * pCadence is initialized to VP_NULL to remove compiler warnings
     * (.. pCadence might be used uninitialized..), but this function is called
     * only from API functions for devices that require cadence support.
     * Therefore, pCadence is initialized by the line object association below
     */
    VpSeqDataType *pCadence = VP_NULL;

    deviceInfo.pDevCtx = pDevCtx;
    deviceInfo.pLineCtx = VP_NULL;
    VpGetDeviceInfo(&deviceInfo);

    maxChannels = deviceInfo.numLines;

    for (channelId = 0; channelId < maxChannels; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            switch(pDevCtx->deviceType) {
#if defined (VP_CC_880_SERIES)
                case VP_DEV_880_SERIES:
                    pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
                    break;
#endif
#if defined (VP_CC_790_SERIES)
                case VP_DEV_790_SERIES:
                    pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
                    break;
#endif

#if defined (VP_CC_580_SERIES)
                case VP_DEV_580_SERIES:
                    pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
                    break;
#endif

                default:
                    break;
            }

            if(pCadence->status & VP_CADENCE_STATUS_ACTIVE) {
                pCurrentPos = pCadence->pCurrentPos;
                if(VpSeq(pLineCtx, pCurrentPos) != VP_STATUS_SUCCESS) {
                    pCadence->status &= ~VP_CADENCE_STATUS_ACTIVE;
                    pCadence->pActiveCadence = VP_PTABLE_NULL;
                }
            }
        }
    }

    return TRUE;
} /* VpServiceSeq() */

/**
 * VpBranchInstruction()
 *  This function implements the Sequencer Branch instruction for the CSLAC
 * device types.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The branch count is either set if this is the first time for this branch, or
 * the branch count is decremented if this branch instruction has been executed
 * before.  If the branch count is decremented to 0, the sequencer index is
 * increased.  If the branch count is 0 at the first time the particular branch
 * is executed, the branch is repeated forever.  If the branch count is not 0,
 * the sequencer is set back to the instruction specified in the profile.  This
 * function can only return VP_SUCCESS since any valid combination of "branch
 * to" and "branch count" is valid.
 */
VpStatusType
VpBranchInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpSeqDataType *pCadence;
    VpOptionEventMaskType *pLineEvents;
    uint8 length, index;
    uint8 *pEventData;
    VpLineStateType lineState;
    uint8 branchDepth;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    uint8 *pIntSeqType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            lineState = ((Vp790LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp790LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            if (!(((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            lineState = ((Vp880LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp880LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            if (!(((Vp580DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp580LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp580LineObjectType *)pLineObj)->processData;
            lineState = ((Vp580LineObjectType *)pLineObj)->lineState.usrCurrent;
            pIntSeqType =  &((Vp580LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    length = pCadence->length;
    index = pCadence->index;

    if (pCadence->status & VP_CADENCE_STATUS_BRANCHING) {
        /*
         * We're already branching, but possibly at a step after this point. In
         * other words, we may have been branched back to a branch step
         * Determine if we are repeating this step, or if we are being branched
         * back from a later step
         */
        if (index < pCadence->branchAt) {
             /* 
              * We're at an earlier step in the branch loop, so use the second
              * set of branch timers
              */
             branchDepth = VP_CSLAC_BRANCH_LVL_1;

             if (!(pCadence->status & VP_CADENCE_STATUS_BRANCHING_LVL2)) {
                 pCadence->status |= VP_CADENCE_STATUS_BRANCHING_LVL2;
                 pCadence->count[branchDepth] = pSeqData[1];
             }
        } else {
            /* This is a continuation from this branch */
            branchDepth = VP_CSLAC_BRANCH_LVL_0; 
        }

        if (pCadence->count[branchDepth] > 0) {
            /*
             * If the repeat value set in the profile is = 0, this means repeat
             * forever.  Therefore, don't decrement the actual count value
             */
            if (pSeqData[1] != 0) {
                pCadence->count[branchDepth]--;
            }

            /* Send the profile pointer back to the branch location */
            /* Account for header offset */
            pCadence->index = (((pSeqData[0] & 0x1F) * 2)
                + VP_PROFILE_TYPE_SEQUENCER_START);
            pCadence->pCurrentPos =
                &(pCadence->pActiveCadence[pCadence->index]);
        } else {
            /*
             * We don't need to repeat this branch.  Just see if the profile is
             * complete
             */

            index+=2;
            if (index < (length + VP_PROFILE_LENGTH + 1)) {
                pCadence->index = index;
                pCadence->pCurrentPos+=2;
                if (pCadence->status & VP_CADENCE_STATUS_BRANCHING_LVL2) {
                    pCadence->status &= ~VP_CADENCE_STATUS_BRANCHING_LVL2;
                } else {
                    pCadence->status &= ~VP_CADENCE_STATUS_BRANCHING;
                }
            } else {  /* The profile is complete. */
                switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                    case VP_PRFWZ_PROFILE_METERING_GEN:
                        pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                        break;
                    
                    case VP_PRFWZ_PROFILE_RINGCAD:
                        pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                        *pEventData = VP_RING_CAD_DONE;
                        break;

                    case VP_PRFWZ_PROFILE_TONECAD:
                        pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                        break;

                    case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_HOOK_FLASH;
                        break;
                        
                    case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_PULSE_DIGIT;
                        break;
                    
                    case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_DTMF_DIGIT;
                        break;

                    case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                        pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                        *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                        VpSetLineState(pLineCtx, lineState);
                        *pIntSeqType = 0;
                        break;
                    
                    default:
                        break;
                    
                }
                pCadence->status = VP_CADENCE_RESET_VALUE;
            }
        }
    } else {
        /*
         * We are not branching, so this is the first branching loop. Set the
         * parameter to indicate this step is branching.
         */
        branchDepth = VP_CSLAC_BRANCH_LVL_0;
        pCadence->count[branchDepth] = pSeqData[1];
        pCadence->branchAt = index;

        if(pCadence->count[branchDepth] == 0) {
            /*
             * This means branch forever.  Implement by setting to max value
             * here, and not decreasing in steps above
             */
            pCadence->count[branchDepth] = 0xFF;
        } else {
            /* Repeat already (following lines) */
            pCadence->count[branchDepth]--;  
        }
        /* Account for header offset */
        pCadence->index =
            (((pSeqData[0] & 0x1F) * 2) + VP_PROFILE_TYPE_SEQUENCER_START);
        pCadence->pCurrentPos = &(pCadence->pActiveCadence[pCadence->index]);
        pCadence->status |= VP_CADENCE_STATUS_BRANCHING;
    }

    /* If we've disabled the cadence, clear the active cadence pointer */
    if (!(pCadence->status & VP_CADENCE_STATUS_ACTIVE)) {
        pCadence->pActiveCadence = VP_PTABLE_NULL;
    }
    
    return VP_STATUS_SUCCESS;
}

/**
 * VpTimeInstruction()
 *  This function implements the Sequencer Time instruction for the CSLAC device
 * types.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The timer is decremented and when it decreases to 0, the pointer in the
 * sequence profile (passed) is updated to the next command past the time
 * operator currently being executed. If there are no more operators, then
 * the cadence is stopped.
 */
VpStatusType
VpTimeInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpSeqDataType *pCadence;
    VpCallerIdType *pCid;
    VpOptionEventMaskType *pLineEvents;
    uint8 *pEventData;
    uint16 tickRate;
    bool forever = FALSE;

    VpLineStateType lineState;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    uint8 *pIntSeqType;
    uint16 msInTick;

    /* Time in sequence is in 5mS incremements.  We need to convert to TICKS */
    uint16 timeInSeq = (((pSeqData[0] & 0x1F) << 8) | pSeqData[1]);

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
            
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp790DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            pCid = &((Vp790LineObjectType *)pLineObj)->callerId;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            lineState = ((Vp790LineObjectType *)pLineObj)->lineState.usrCurrent;            
            pIntSeqType =  &((Vp790LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            if (!(((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            pCid = &((Vp880LineObjectType *)pLineObj)->callerId;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            lineState = ((Vp880LineObjectType *)pLineObj)->lineState.usrCurrent;            
            pIntSeqType =  &((Vp880LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif

#if defined (VP_CC_580_SERIES)
        case VP_DEV_580_SERIES:
            if (!(((Vp580DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            pCadence = &((Vp580LineObjectType *)pLineObj)->cadence;
            tickRate =
                ((Vp580DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            pLineEvents = &((Vp580LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp580LineObjectType *)pLineObj)->processData;
            lineState = ((Vp580LineObjectType *)pLineObj)->lineState.usrCurrent;            
            pCid = VP_NULL;
            pIntSeqType =  &((Vp580LineObjectType *)pLineObj)->intSequence[VP_PROFILE_TYPE_LSB];
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (pCadence->status & VP_CADENCE_STATUS_MID_TIMER) {
        pCadence->timeRemain--;
    } else {
        pCadence->status |= VP_CADENCE_STATUS_MID_TIMER;
        pCadence->timeRemain = MS_TO_TICKRATE((timeInSeq * 5), tickRate);

        if (pCadence->timeRemain == 0) {
            /* Always is selected.  End the cadence and leave the state as is */
            pCadence->status = VP_CADENCE_RESET_VALUE;
            forever = TRUE;
        } else {
            /*
             * Find out how long in ms 1 "tick" is, then subtract that amount
             * from the time required in the cadence. Lower limit 1 tick.
             */

            msInTick = TICKS_TO_MS(1, tickRate);
            /*
             * If the time specified in the sequence can be executed with at
             * least one tick, then subtract one "tick" worth of time
             */
            if ((timeInSeq * 5) > msInTick) {
                pCadence->timeRemain =
                    MS_TO_TICKRATE(((timeInSeq * 5) - msInTick), tickRate);
            }
        }
    }

    /* If the time is over, move on to the next sequence if there is one */
    if (pCadence->timeRemain == 0) {
        pCadence->index+=2;

        if (pCadence->index < (pCadence->length + VP_PROFILE_LENGTH + 1)) {
            pSeqData+=2;
            pCadence->pCurrentPos = pSeqData;
        } else {  /* The profile is complete. */
            switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                case VP_PRFWZ_PROFILE_METERING_GEN:
                    pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                    break;
            
                case VP_PRFWZ_PROFILE_RINGCAD:
                    if (forever == FALSE) {
                        pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                        *pEventData = VP_RING_CAD_DONE;
                    }
                    break;

                case VP_PRFWZ_PROFILE_TONECAD:
                    pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                    break;

                case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_HOOK_FLASH;
                    break;
                
                case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_PULSE_DIGIT;
                    break;
            
                case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_DTMF_DIGIT;
                    break;

                case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                    *pIntSeqType = 0;
                    break;

                case VP_PRFWZ_PROFILE_FWD_DISC_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_FWD_DISCONNECT;
                    *pIntSeqType = 0;
                    break;

                case VP_PRFWZ_PROFILE_POLREV_PULSE_INT:
                    pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                    *pEventData = VP_SENDSIG_POLREV_PULSE;
                    *pIntSeqType = 0;
                    break;
            
                default:
                    break;
            
            }
            pCadence->status = VP_CADENCE_RESET_VALUE;
        }
        pCadence->status &= ~VP_CADENCE_STATUS_MID_TIMER;
    } else {
        /* Check to see if we're in the middle of a Wait on function. If so,
         * check to see if we still need to wait on CID (only supported wait
         * on operator). If CID is complete, terminate the timer function.
         * If not, continue..
         */
        if ((pCid != VP_NULL) && (pCid->status & VP_CID_WAIT_ON_ACTIVE)) {
            if (pCid->status & VP_CID_IN_PROGRESS) {
                /* Do nothing */
            } else {
                /* Terminate this timer operation and the wait on */
                pCid->status &= ~ VP_CID_WAIT_ON_ACTIVE;
                pCadence->status &= ~VP_CADENCE_STATUS_MID_TIMER;
                pCadence->timeRemain = 0;

                pCadence->index+=2;

                if (pCadence->index <
                    (pCadence->length + VP_PROFILE_LENGTH + 1)) {
                    pSeqData+=2;
                    pCadence->pCurrentPos = pSeqData;
                } else {  /* The profile is complete. */
                    switch(pCadence->pActiveCadence[VP_PROFILE_TYPE_LSB]) {
                        case VP_PRFWZ_PROFILE_METERING_GEN:
                            pLineEvents->process |= VP_LINE_EVID_MTR_CMP;
                            break;
            
                        case VP_PRFWZ_PROFILE_RINGCAD:
                            pLineEvents->process |= VP_LINE_EVID_RING_CAD;
                            *pEventData = VP_RING_CAD_DONE;
                            break;

                        case VP_PRFWZ_PROFILE_TONECAD:
                            pLineEvents->process |= VP_LINE_EVID_TONE_CAD;
                            break;

                        case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_HOOK_FLASH;
                            break;
                
                        case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_PULSE_DIGIT;
                            break;
            
                        case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_DTMF_DIGIT;
                            break;

                        case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_MSG_WAIT_PULSE;
                            VpSetLineState(pLineCtx, lineState);
                            *pIntSeqType = 0;
                            break;

                        case VP_PRFWZ_PROFILE_FWD_DISC_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_FWD_DISCONNECT;
                            *pIntSeqType = 0;
                            break;

                        case VP_PRFWZ_PROFILE_POLREV_PULSE_INT:
                            pLineEvents->process |= VP_LINE_EVID_SIGNAL_CMP;
                            *pEventData = VP_SENDSIG_POLREV_PULSE;
                            *pIntSeqType = 0;
                            break;
            
                        default:
                            break;
            
                    }
                    pCadence->status = VP_CADENCE_RESET_VALUE;
                }
            }
        }
    }
    /* If we've disabled the cadence, clear the active cadence pointer */
    if (!(pCadence->status & VP_CADENCE_STATUS_ACTIVE)) {
        pCadence->pActiveCadence = VP_PTABLE_NULL;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * VpCidSeq()
 *  This function services an active Caller ID Timer. This function runs when
 * the CLI timer for the passed channel is active. The function reads the
 * current caller ID sequence that is pointed to in the caller ID structure for
 * the given channel. This pointer is assigned when ever the function
 * CliStartCli is called. Additionally, the CliStartCli function sets the timer
 * to 1 to seed the CLI process.
 *
 *  This routine is broken up into two functional stages. The first stage
 * handles CLI tasks that are not time related while the second stage handles
 * time related task. Non-time related tasks include things such as muting a
 * channel, pol-rev and EOT (end of transmission). Time related tasks include
 * timing of the MARK signal, the SEIZURE signal and ACK detection as well as
 * timing for sending encoded data bytes to the device FSK generator.
 *
 * Preconditions:
 *  This Function must be called from the ApiTick function.
 *
 * Postconditions:
 *  The Caller ID State Machine is updated. Returns TRUE, if a user defined
 * event was encountered
 */
VpStatusType
VpCidSeq(
    VpLineCtxType *pLineCtx)    /**< Line that has an active CID sequence */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    VpSetLineStateFuncPtrType SetLineState;

    VpDeviceIdType deviceId;
    uint16 tickRate;
    uint8 ecVal, channelId;
    uint8 indexVal;  /* Use this when several index vals are needed */

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    VpCallerIdType *pCidStruct;
    VpLineStateType lineState;
    
    VpDigitType digit;
    
    uint16 uiCliOpCode;
    VpStatusType retFlag = VP_STATUS_SUCCESS;
    uint8 startOfCliData, mpiLen;
    uint16 cliTimer = 0;
    uint16 tempDebounceTime = 0;

    uint16 index;
    uint8 scratchData[1];
    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            SetLineState = Vp790SetLineStateInt;

            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            if (!(((Vp790DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }
            tickRate =
                ((Vp790DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;

            switch(channelId) {
                case 0: ecVal = VP790_EC_CH1;   break;
                case 1: ecVal = VP790_EC_CH2;   break;
                case 2: ecVal = VP790_EC_CH3;   break;
                case 3: ecVal = VP790_EC_CH4;   break;
                default:
                    return VP_STATUS_FAILURE;
            }
            break;

#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            SetLineState = Vp880SetLineStateInt;

            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            if (!(((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_INIT_CMP)) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEV_NOT_INITIALIZED;
            }

            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;

            switch(channelId) {
                case 0: ecVal = VP880_EC_CH1;   break;
                case 1: ecVal = VP880_EC_CH2;   break;
                default:
                    pCidStruct->status &= ~VP_CID_IN_PROGRESS;
                    return VP_STATUS_FAILURE;
            }
            /* Adjust the EC value for Wideband mode as needed */
            ecVal |= ((((Vp880LineObjectType *)pLineObj)->codec == VP_OPTION_WIDEBAND)
                ? VP880_WIDEBAND_MODE : 0);
            break;
#endif

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Determine if the timer is running. */
    if(pCidStruct->cliTimer > 0) {
        pCidStruct->cliTimer--;
        if (pCidStruct->status & VP_CID_REPEAT_MSG) {
            if (VpFSKGeneratorReady(pLineCtx) == TRUE) {
                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    pCidStruct->currentData);
            }
        }

        if(pCidStruct->cliTimer != 0) {
            return VP_STATUS_SUCCESS;
        }
    } else {
        return VP_STATUS_SUCCESS;
    }
    pCidStruct->status &= ~VP_CID_REPEAT_MSG;

    /*
     * Find where the start of the CLI command data is (excluding the MPI
     * command/data used to set the tone generator(s)
     */
    mpiLen = pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN];

    /*
     * Start of CLI commands on the LSB (word aligned). Exact location found
     * by adding the start elements offset to the end of the mpi command data
     * (found from the location of the mpi command length + the actual length 
     * of the mpi data).
     */
    startOfCliData = VP_CID_PROFILE_FSK_PARAM_LEN + mpiLen + 
        VP_CID_PROFILE_START_OF_ELEMENTS_LSB;

    /* Get the current index for the CLI profile. */
    index = pCidStruct->cliIndex;
    pCidStruct->cliDebounceTime = 0;

    /*
     * This section of code tests to see if a CPE ACK was received.
     * If the variable cliAwaitTone is TRUE, then test to see if the ACK
     * was received prior to the timeout specified in the CLI_DETECT portion
     * of the CLI profile. If the ACK was not received, you can not send the
     * CID information so terminate the CLI sequence.
     */
    if (pCidStruct->status & VP_CID_AWAIT_TONE) {
        VpCtrlDetectDTMF(pLineCtx, FALSE);
        /* 
         * This would have been set to VP_DIG_NONE prior to starting the DTMF
         * digit detection. So any other value (whether in make or break
         * interval) is indication of DTMF digit detected during detection
         * interval.
         */
        
        digit = pCidStruct->digitDet;
    
        if ((digit == pCidStruct->cliDetectTone1) 
         || (digit == pCidStruct->cliDetectTone2)) {
            /* The ACK tone was detected, continue with Caller ID */
        } else {
            /* Ack tone not detected, stop Caller ID */
            VpCliStopCli(pLineCtx);
            return VP_STATUS_SUCCESS;
        }
    }

    /*
     * If the sending of FSK data needs to be terminated, then send the
     * terminate FSK control to the FSK generator. This happens at the end
     * of all FSK related signaling such as channel seizure.
     */
    if (pCidStruct->status & VP_CID_TERM_FSK) {
        VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
        pCidStruct->status &= ~VP_CID_TERM_FSK;
        pCidStruct->cliTimer = MS_TO_TICKRATE((tickRate >> 8), tickRate);
        return VP_STATUS_SUCCESS;
    }

    /*
     * Handle zero-time-length codes. Zero time codes are op-codes in the
     * profile that are executed but have no time associated with them. In
     * other words the CLI sequence immediately moves on to the next state.
     * they include POL-REV, Channel mute, and EOT. The while loop churns
     * through the profile until a time related element is encountered.
     */
    uiCliOpCode = pCidStruct->pCliProfile[startOfCliData + index];

    while ( (index <= pCidStruct->pCliProfile[startOfCliData - 2]) &&
            ( (uiCliOpCode == VP_CLI_POLREV) ||
              (uiCliOpCode == VP_CLI_EOT) ||
              (uiCliOpCode == VP_CLI_MUTEON) ||
              (uiCliOpCode == VP_CLI_MUTEOFF)) ) {

        switch (uiCliOpCode) {
            /* Mute both the upstream and down stream transmission paths. */
            case VP_CLI_MUTEON:
                index+=2;
                pCidStruct->status |= VP_CID_MUTE_ON;
                VpCtrlMuteChannel(pLineCtx, TRUE);
                break;

            /*
             * Re-enable audio transmission in both the upstream and downstream
             * directions.
             */
            case VP_CLI_MUTEOFF:
                index+=2;
                pCidStruct->status &= ~(VP_CID_MUTE_ON);
                VpCtrlMuteChannel(pLineCtx, FALSE);
                break;

            /* Invert the polarity of the line. */
            case VP_CLI_POLREV:
                index+=2;
                VpGetLineState(pLineCtx, &lineState);
                switch(lineState) {
                    case VP_LINE_STANDBY:
                    case VP_LINE_ACTIVE:
                        SetLineState(pLineCtx, VP_LINE_ACTIVE_POLREV);
                        break;

                    case VP_LINE_ACTIVE_POLREV:
                        SetLineState(pLineCtx, VP_LINE_ACTIVE);
                        break;

                    case VP_LINE_TALK:
                        SetLineState(pLineCtx, VP_LINE_TALK_POLREV);
                        break;

                    case VP_LINE_TALK_POLREV:
                        SetLineState(pLineCtx, VP_LINE_TALK);
                        break;

                    case VP_LINE_OHT:
                        SetLineState(pLineCtx, VP_LINE_OHT_POLREV);
                        break;

                    case VP_LINE_OHT_POLREV:
                        SetLineState(pLineCtx, VP_LINE_OHT);
                        break;
                    default:
                        break;
                }
                break;

            /* Indicates the End Of Transmission for the CLI sequence */
            case VP_CLI_EOT:
                VpCliStopCli(pLineCtx);
                return VP_STATUS_SUCCESS;

            default:
                index+=2;
                break;
        }
        uiCliOpCode = pCidStruct->pCliProfile[startOfCliData + index];
    }

    /*
     * Process all time based CLI profile codes. This includes timing of
     * channel seizure, ACK detect, MARK, and the message data.
     */
    if (index <= pCidStruct->pCliProfile[startOfCliData - 2]) {
        /* Switch on CLI Profile Element type at the current index. */
        switch (pCidStruct->pCliProfile[startOfCliData + index]) {

            /*
             * Set up the CLI sequence for detection of the CPE ACK. This state
             * will stop any running sequence, disable the sig gen, stop any
             * FSK activity, and set a time out for the tone detection. If
             * the time-out time is reached, the CPE did not ACK and the CLI
             * sequence will be aborted.
             */
            case VP_CLI_DETECT:
                /* Turn off the tone generator and turn FSK off */
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);

                /* Read the timeout time */
                index++;
                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);
                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                /* Read which tones to detect. */
                index+=2;

                /* 
                 * The data from profile wizard can be shifted 4 right and be
                 * interpreted directly as a VpDigitType (other than 0xFF for
                 * Digit Type None)
                 */
                pCidStruct->cliDetectTone1 =
                    pCidStruct->pCliProfile[startOfCliData + index];
                if (pCidStruct->cliDetectTone1 != 0xFF) {
                    pCidStruct->cliDetectTone1 = 
                        ((pCidStruct->cliDetectTone1 >> 4) & 0xFF);
                }
                if (VpIsDigit(pCidStruct->cliDetectTone1) == FALSE) {
                    VpCliStopCli(pLineCtx);
                    return VP_STATUS_INVALID_ARG;
                }

                index+=2;
                pCidStruct->cliDetectTone2 =
                    pCidStruct->pCliProfile[startOfCliData + index];

                if (pCidStruct->cliDetectTone2 != 0xFF) {
                    pCidStruct->cliDetectTone2 = 
                        ((pCidStruct->cliDetectTone2 >> 4) & 0xFF);                    
                }
                if (VpIsDigit(pCidStruct->cliDetectTone2) == FALSE) {
                    VpCliStopCli(pLineCtx);
                    return VP_STATUS_INVALID_ARG;
                }

                /* Start the tone detector. */
                index+=2;
                VpCtrlDetectDTMF(pLineCtx, TRUE);
                break;

            /*
             * This case sets up the signal generator to generate tones that
             * are defined in the profile. This will include things like the call
             * waiting beep. This case does not actually start the signal generator
             * it only sets it up.
             */
            case VP_CLI_ALERTTONE:
                /* Stop any running sequences and disable any FSK activity. */
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);

                /*
                 * Set the timer to return as soon as possible before enabling the
                 * tone generator.
                 */
                index++;    /* Get to the length of MPI data to send */
                cliTimer = (tickRate >> 8);

                /*
                 * Send the MPI data to the device starting at the point after the
                 * length of data field
                 */

#if 0
                SendProfileMpi(deviceId, ecVal, pCidStruct->pCliProfile, 
                    (startOfCliData + index));
#endif
#if 1
                indexVal = startOfCliData + index + 1;
                VpMpiCmd(deviceId, ecVal, NOOP_CMD,
                    pCidStruct->pCliProfile[startOfCliData + index],
                    (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));
#endif
                pCidStruct->status &= ~VP_CID_SIG_B_VALID;

                /* Get to the next command after the MPI data */
                index += pCidStruct->pCliProfile[startOfCliData + index];
                index += 2;
                break;

            /*
             * This case starts the signal generator for the time specified in
             * the CLI profile. It is assumed the signal generator was set up
             * first in the previous case.
             */
            case VP_CLI_ALERTTONE2:
                VpCtrlSetCliTone(pLineCtx, TRUE);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

            /*
             * This case creates a silent period of the time specified in the
             * profile. This is done by disabling the signal generator and
             * FSK generator.
             */
            case VP_CLI_SILENCE:
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

             /*
              * This case creates a silent period and prevents hook switch from
              * being detected for cliDebouncing time. Currently the debounce
              * period does nothing and should be enabled by creating a mask hook
              * method in the LIU.
              */
            case VP_CLI_SILENCE_MASKHOOK:
                VpSetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index++;
                tempDebounceTime =
                    (pCidStruct->pCliProfile[startOfCliData + index] << 8) & 0xFF00;
                index++;
                tempDebounceTime |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                tempDebounceTime *= VP_CID_TIMESCALE;

                pCidStruct->cliDebounceTime =
                    MS_TO_TICKRATE(tempDebounceTime, tickRate);

                index+=2;
                break;

            /*
             * This case creates the channel seizure signal for the time specified.
             */
            case VP_CLI_CHANSEIZURE:
                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmd(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                VpCtrlSetCliTone(pLineCtx, FALSE);
                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    VP_FSK_CHAN_SEIZURE);

                pCidStruct->currentData = VP_FSK_CHAN_SEIZURE;
                pCidStruct->status |= VP_CID_REPEAT_MSG;

                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                pCidStruct->status |= VP_CID_TERM_FSK;
                break;

             /*
              * This case creates the mark signal for the time specified using the
              * FSK generator.
              */
            case VP_CLI_MARKSIGNAL:
                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmd(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_KEYED_CHAR,
                    VP_FSK_MARK_SIGNAL);

                pCidStruct->currentData = VP_FSK_MARK_SIGNAL;
                pCidStruct->status |= VP_CID_REPEAT_MSG;

                index++;

                cliTimer = pCidStruct->pCliProfile[startOfCliData + index];
                cliTimer = ((cliTimer << 8) & 0xFF00);

                index++;
                cliTimer |=
                    (pCidStruct->pCliProfile[startOfCliData + index] & 0x00FF);
                cliTimer *= VP_CID_TIMESCALE;

                index+=2;
                break;

            /* This case sends the actual message data (FSK Format). */
            case VP_CLI_MESSAGE:
                /* If the FSK Generator was not setup before, set it now */
                if (!(pCidStruct->status & VP_CID_SIG_B_VALID)) {
                    indexVal = VP_CID_PROFILE_FSK_PARAM_LEN + 1;
                    VpMpiCmd(deviceId, ecVal, NOOP_CMD,
                        pCidStruct->pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN],
                        (VpProfileDataType *)(&pCidStruct->pCliProfile[indexVal]));

                    pCidStruct->status |= VP_CID_SIG_B_VALID;
                }

                if (VpFSKGeneratorReady(pLineCtx) == TRUE) {
                    /* Send next data in buffer */

                    if(VpCliGetEncodedByte(pLineCtx, scratchData) == TRUE) {
                        VpCtrlSetFSKGen(pLineCtx, VP_CID_GENERATOR_DATA,
                            scratchData[0]);
                    } else {
                        /* We're done. Go to the next element */
                        VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
                        index+=2;
                    }
                }

                /* Force a check for buffer update at the next tick  */
                cliTimer = (tickRate >> 8);
                break;

            /* This case sends the actual message data (DTMF Format). */
            case VP_CLI_DTMF_MESSAGE:
                if (VpDTMFGeneratorReady(pLineCtx) == TRUE) {
                    /* Send next data in buffer */
                    if(VpCliGetEncodedByte(pLineCtx, scratchData) == TRUE) {
                        VpCtrlSetDTMFGen(pLineCtx, VP_CID_GENERATOR_DATA,
                            VpConvertCharToDigitType(scratchData[0]));
                    } else {
                        /* We're done. Go to the next element */
                        VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
                        index+=2;
                    }
                }

                /* Force a check for buffer update at the next tick  */
                cliTimer = (tickRate >> 8);
                break;                

            /* Shouldn't be possible */
            default:
                index += 2;
                break;

        } /* End of Switch (CLI Element Type) */
    } /* if index <= number of elements */

    /*
     * If the CLI sequencer has indexed passed the number of elements in the
     * the profile, then stop the CLI sequence. This condition is true if
     * the ACK was not received, the EOT marker in the profile was reached
     * or an error occurred. Otherwise, continue normally.
     */
    if (index > pCidStruct->pCliProfile[startOfCliData - 2]) {
        VpCliStopCli(pLineCtx);
    } else {
        pCidStruct->cliIndex = index;
        pCidStruct->cliTimer = MS_TO_TICKRATE(cliTimer, tickRate);
    }
    return retFlag;
}

/**
 * VpFSKGeneratorReady()
 *  This function returns TRUE if the FSK Generator is ready to accept the next
 * CID message byte, FALSE otherwise.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None. Status of FSK Generator only is reported. Otherwise line is unaffected
 */
bool
VpFSKGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790FSKGeneratorReady(pLineCtx);
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            return Vp880FSKGeneratorReady(pLineCtx);
#endif
        default:
            return TRUE;
    }
}

/**
 * VpDTMFGeneratorReady()
 *  This function returns TRUE if the DTMF Generator is ready to accept the next
 * CID message byte, FALSE otherwise.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  None. Status of DTMF Generator only is reported. Otherwise line is unaffected
 */
bool
VpDTMFGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    bool returnVal = TRUE;

#if defined (VP_CC_880_SERIES)
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    uint16 tickRate;

    VpCslacTimers *pLineTimers;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;    

    VpCallerIdType *pCidStruct;

    switch (deviceType) {
        case VP_DEV_880_SERIES:
            pLineTimers = &((Vp880LineObjectType *)pLineObj)->lineTimers.timers;
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            tickRate =
                ((Vp880DeviceObjectType *)pDevObj)->devProfileData.tickRate;
            break;

        default:
            return TRUE;
    }

    if (!(pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] & VP_ACTIVATE_LINE_TIMER)) {
        /* 
         * Timer appears available. If we're in an on-time already, change to 
         * the off-time and return FALSE (i.e., cannot program another DTMF
         * number yet). If it's in an off-time, it's now complete so return
         * TRUE.
         */
        if (pCidStruct->dtmfStatus & VP_CID_ACTIVE_ON_TIME) {
            pCidStruct->dtmfStatus &= ~VP_CID_ACTIVE_ON_TIME;
            
            /* Setup the line timer for the off-time for DTMF CID */
            pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] = 
                MS_TO_TICKRATE(VP_CID_DTMF_OFF_TIME, tickRate);

            pLineTimers->timer[VP_LINE_TIMER_CID_DTMF] |= VP_ACTIVATE_LINE_TIMER;
            pCidStruct->dtmfStatus |= VP_CID_ACTIVE_OFF_TIME;
                
            VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
            returnVal = FALSE;
        } else if (pCidStruct->dtmfStatus & VP_CID_ACTIVE_OFF_TIME) {
            pCidStruct->dtmfStatus &= ~VP_CID_ACTIVE_OFF_TIME;

            /* Nothing more to do here */
            returnVal = TRUE;
        }
    } else {
        returnVal = FALSE;
    }     
#endif

    return returnVal;
}

/**
 * VpCliStopCli()
 *  This function stops the CLI sequence on the passed line.
 *
 * Preconditions
 *  None
 *
 * Postconditions
 *  The caller ID sequence, if running, will be aborted.
 */
void
VpCliStopCli(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    void *pLineObj = pLineCtx->pLineObj;

    uint8 *pEventData;
    VpOptionEventMaskType *pLineEvents;    

    VpCallerIdType *pCidStruct;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            pCidStruct->cliTimer = 0;
            Vp790SetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp790LineObjectType *)pLineObj)->processData;
            break;

#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            pCidStruct->cliTimer = 0;
            Vp880SetLineTone(pLineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL);
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pEventData = &((Vp880LineObjectType *)pLineObj)->processData;
            break;
#endif
        default:
            return;
    }

    VpCtrlSetDTMFGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
    VpCtrlSetFSKGen(pLineCtx, VP_CID_SIGGEN_EOT, 0);
    VpCtrlDetectDTMF(pLineCtx, FALSE);

    pCidStruct->status &= ~(VP_CID_MUTE_ON);
    VpCtrlMuteChannel(pLineCtx, FALSE);

    pCidStruct->status &= ~VP_CID_IN_PROGRESS;
    pLineEvents->process |= VP_LINE_EVID_CID_DATA;
    *pEventData = 1;
}

/**
 * VpCliGetEncodedByte()
 *  This function returns an encoded byte of data that is suitable for writing
 * the FSK generator (device dependent).
 *
 * Preconditions
 *  Must have a valid CLI packet in to work from.
 *
 * Postconditions
 *  The per-channel caller ID buffer will be updated with encoded data.
 */
bool
VpCliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790CliGetEncodedByte(pLineCtx, pByte);
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            return Vp880CliGetEncodedByte(pLineCtx, pByte);
#endif
        default:
            return FALSE;
    }
}

/**
 * VpCtrlSetCliTone()
 *  This function is called by the API internally to enable or disable the
 * signal generator used for Caller ID.
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  The signal generator used for CID tones is enabled/disabled indicated by
 * the mode parameter passed.
 */
VpStatusType
VpCtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            return Vp790CtrlSetCliTone(pLineCtx, mode);
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            return Vp880CtrlSetCliTone(pLineCtx, mode);
#endif

        default:
            return VP_STATUS_FUNC_NOT_SUPPORTED;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * VpCtrlDetectDTMF()
 *  This function is called by the API internally to enable or disable the
 * system level dtmf detector (if present).
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  If implemented, DTMF detection is enabled (or disabled) in the system 
 * services layer. This function does not call the system services function for
 * devices that support internal DTMF detection.
 */
VpStatusType
VpCtrlDetectDTMF(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpDeviceIdType deviceId;
    uint8 channelId;
    VpCallerIdType *pCidStruct;

    void *pDevObj = pDevCtx->pDevObj;
    void *pLineObj = pLineCtx->pLineObj;

    if ((pDevObj == VP_NULL) || (pLineObj == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }
    
    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;            
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            break;
#endif

        default:
            return VP_STATUS_FUNC_NOT_SUPPORTED;
    }

    /* 
     * IF enabling DTMF detection, make sure the TX PCM is enabled. Otherwise,
     * return TX/RX to states determined by Mute On/Off and linestate
     */ 
    if (mode == TRUE) {
        pCidStruct->status |= VP_CID_AWAIT_TONE;
        pCidStruct->digitDet = VP_DIG_NONE;        
        VpCtrlMuteChannel(pLineCtx, TRUE);
        VpSysDtmfDetEnable(deviceId, channelId);
    } else {
        pCidStruct->status &= ~(VP_CID_AWAIT_TONE);
        VpSysDtmfDetDisable(deviceId, channelId);
        VpCtrlMuteChannel(pLineCtx, FALSE);
    }

    return VP_STATUS_SUCCESS;
}

/**
 * VpCtrlSetFSKGen()
 *  This function is called by the CID sequencer executed internally by the API
 *
 * Preconditions:
 *  The line context must be valid
 *
 * Postconditions:
 *  The data indicated by mode and data is applied to the line. Mode is used
 * to indicate whether the data is "message", or a special character. The 
 * special characters are "channel siezure" (alt. 1/0), "mark" (all 1), or
 * "end of transmission".
 */
void
VpCtrlSetFSKGen(
    VpLineCtxType *pLineCtx, 
    VpCidGeneratorControlType mode,
    uint8 data)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            Vp790CtrlSetFSKGen(pLineCtx, mode, data);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            Vp880CtrlSetFSKGen(pLineCtx, mode, data);
            break;
#endif

        default:
            break;
    }
    return;
}

/**
 * VpConvertCharToDigitType()
 *  This function is called by the CID sequencer executed internally by the API.
 * It converts a character to a VpDigitType and is used for functions requiring
 * a VpDigitType specifically.
 *
 * Preconditions:
 *  None. Utility function only.
 *
 * Postconditions:
 *  The character passed is converted/returned as a VpDigitType
 */
VpDigitType
VpConvertCharToDigitType(
    char digit)
{
    VpDigitType vpDig;
    
    switch(digit) {
        case '0':
            vpDig = VP_DIG_ZERO;
            break;
            
        case 'A':
            vpDig = VP_DIG_A;
            break;

        case 'B':
            vpDig = VP_DIG_B;
            break;

        case 'C':
            vpDig = VP_DIG_C;
            break;

        case 'D':
            vpDig = VP_DIG_D;
            break;

        case '*':
            vpDig = VP_DIG_ASTER;
            break;

        case '#':
            vpDig = VP_DIG_POUND;
            break;
        
        default:
            vpDig = (uint8)digit-48;
            break;
    }
    return vpDig;
}    

/**
 * VpCtrlSetDTMFGen()
 *  This function sets the DTMF generators of the device and if DTMF message
 * data is in progress, disables the TX/RX PCM highway. If this is the end
 * of DTMF message data transmission, then the TX/RX PCM is re-enabled based
 * on the line state and the tx/rx mode set for "talk" states.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The DTMF signal generators are set to the CID specified level and the digit
 * passed is applied to the line (if mode == VP_CID_GENERATOR_DATA).
 */
void
VpCtrlSetDTMFGen(
    VpLineCtxType *pLineCtx, 
    VpCidGeneratorControlType mode,
    VpDigitType digit)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;

    if (mode == VP_CID_GENERATOR_DATA) {
        VpCtrlMuteChannel(pLineCtx, TRUE);
    } else {
        VpCtrlMuteChannel(pLineCtx, FALSE);
    }

    switch (pDevCtx->deviceType) {
#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            Vp880SetDTMFGenerators(pLineCtx, mode, digit);
            break;
#endif

        default:
            break;
    }
    return;
}

/**
 * VpCtrlMuteChannel()
 *  This function disables the TX/RX PCM highway if mode == TRUE, otherwise it 
 * enables the TX/RX PCM highway based on line state and the option set for
 * TX/RX enable mode in talk states.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The TX/RX PCM mode is set on the line.
 */
void
VpCtrlMuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            Vp790MuteChannel(pLineCtx, mode);
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            Vp880MuteChannel(pLineCtx, mode);
            break;
#endif
        default:
            break;
    }
    return;
}

/**
 * VpUpdateDP()
 *  This function measures the timing of the on/off-hook conditions for Dial
 * Pulsing and Flash Hook events (as well as the immediate Off-Hook, and long
 * On-Hook events).
 *
 * Preconditions:
 *  The pointers passed (Dial Pulse specifications, Dial Pulse data structure,
 * and Line Events structure) cannot be VP_NULL.
 *
 * Postconditions:
 *  The line events structure is updated with an event if the on/off-hook
 * variables provided in the dial pulse structure meet the specifications
 * provided in the dial pulse specification structure. Note: This function has
 * no knowledge of line context/objects, so it can be used for any type of
 * line.
 */
bool
VpUpdateDP(
    uint16 tickRate,    /**< Device API Tickrate used to measure real
                         * on/off-hook time
                         */

    VpOptionPulseType *pPulseSpecs,     /**< Dial Pulse specifications to apply
                                         * to on/off-hooks
                                         */
    VpDialPulseDetectType *pDpStruct,   /**< Structure used to maintain dial
                                         * pulse status
                                         */
    VpOptionEventMaskType *pLineEvents) /**< Line event structure to be modified
                                         * if an event is detected (and needs to
                                         * be reported).
                                         */
{
    bool eventStatus = FALSE;
    uint16 break_min, break_max;
    uint16 make_min, make_max;
    uint16 flash_min, flash_max;
    uint16 interDigitMin;

    if ((pPulseSpecs == VP_NULL)
     || (pDpStruct == VP_NULL)
     || (pLineEvents == VP_NULL)) {
        return FALSE;
    }

    /* Convert tickRate to 125uS then get the real counts */
    break_min = MS_TO_TICKRATE((pPulseSpecs->breakMin / 8), tickRate);
    break_max= MS_TO_TICKRATE((pPulseSpecs->breakMax / 8), tickRate);
    make_min = MS_TO_TICKRATE((pPulseSpecs->makeMin / 8), tickRate);
    make_max = MS_TO_TICKRATE((pPulseSpecs->makeMax / 8), tickRate);
    flash_min = MS_TO_TICKRATE((pPulseSpecs->flashMin / 8), tickRate);
    flash_max = MS_TO_TICKRATE((pPulseSpecs->flashMax / 8), tickRate);
    interDigitMin = MS_TO_TICKRATE((pPulseSpecs->interDigitMin / 8), tickRate);

    switch(pDpStruct->state) {
        case VP_DP_DETECT_STATE_IDLE:
            if(pDpStruct->hookSt) {
                /*
                 * We are off-hook after being on-hook for a "long time". Start
                 * dial pulsing
                 */
                pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                pDpStruct->lc_time = 0;
                eventStatus = TRUE;
                pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                pLineEvents->signaling |= VP_LINE_EVID_HOOK_OFF;
            }
            break;

        case VP_DP_DETECT_STATE_LOOP_OPEN:
            pDpStruct->lo_time++;

            if(pDpStruct->hookSt) {
                if ((pDpStruct->lo_time >= break_min)
                 && (pDpStruct->lo_time <= break_max)) {
                    /* 
                     * We think we just dialed a pulse, but don't count it if
                     * this sequence of on/off hooks has already been marked
                     * as bad
                     */
                    VP_DOUT(DBG_INFO, ("\n\rDigit Open Time %d\n\r",
                        pDpStruct->lo_time));


                    if (pDpStruct->digits >= 0) {
                        pDpStruct->digits++;
                    }
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                } else if ((pDpStruct->lo_time >= flash_min)
                        && (pDpStruct->lo_time <= flash_max)) {
                    /* We did a hook flash */
                    pDpStruct->signalingData =
                        pDpStruct->lo_time * (tickRate >> 8);

                    eventStatus = TRUE;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                    pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                    pLineEvents->signaling |= VP_LINE_EVID_FLASH;

                    VpInitDP(pDpStruct);
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                    pDpStruct->lo_time = flash_max-1;
                } else {
                    /* Neither a dial pulse, nor a hook flash */
                    VpInitDP(pDpStruct);
                    pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;

                    /* Mark this sequence of digits invalid */
                    pDpStruct->digits = -1;
                }
                pDpStruct->lc_time = 0;
            } else {
                if (pDpStruct->lo_time > flash_max) {
                    /* We're on-hook, report and start over */
                    VpInitDP(pDpStruct);
                    eventStatus = TRUE;
                    pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                    pLineEvents->signaling &= ~VP_LINE_EVID_PULSE_DIG;
                    pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                    pLineEvents->signaling |= VP_LINE_EVID_HOOK_ON;
                }
            }
            break;

        case VP_DP_DETECT_STATE_LOOP_CLOSE:
            /* Limit the value lc_time can reach so we don't overflow */
            if(pDpStruct->lc_time < 100) {
                pDpStruct->lc_time++;
            }

            /* Check to see if we're still off-hook */
            if(pDpStruct->hookSt) {
                /* We're still off-hook. Did we reach the interdigit time? */
                if (pDpStruct->lc_time == interDigitMin) {
                    /* Interdigit time reached. Report digits collected */
                    if (pDpStruct->digits > 0) {
                        /* We have dialed digits */
                        eventStatus = TRUE;
                        pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                        pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                        pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                        pLineEvents->signaling |= VP_LINE_EVID_PULSE_DIG;
                        pDpStruct->signalingData = pDpStruct->digits;
                    } else {
                        /*
                         * If we're still off-hook, we either didn't collect
                         * digits, the digits were invalid, this was an initial
                         * off-hook or a Hook Flash. If the digits were not
                         * invalid, the event has been reported. Only report an
                         * event for invalid digits.
                         */
                        if (pDpStruct->digits < 0) {
                            eventStatus = TRUE;
                            pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_ON;
                            pLineEvents->signaling &= ~VP_LINE_EVID_HOOK_OFF;
                            pLineEvents->signaling &= ~VP_LINE_EVID_FLASH;
                            pLineEvents->signaling |= VP_LINE_EVID_PULSE_DIG;
                            pDpStruct->signalingData = VP_DIG_NONE;
                        }
                    }
                }
            } else {
                /*
                 * We're on-hook, but the only thing we can do here is clear
                 * out all digits if the make time fails DP criteria and mark
                 * this sequence of digits invalid.
                 */
                if (pDpStruct->lc_time > make_max) {
                    eventStatus = TRUE;
                    pLineEvents->signaling |= VP_LINE_EVID_STARTPULSE;
                }

                if((pDpStruct->lc_time > make_max) || (pDpStruct->lc_time < make_min)) {
                    VpInitDP(pDpStruct);                    
                }
                VP_DOUT(DBG_INFO, ("\n\rDigit Close Time %d\n\r",
                    pDpStruct->lc_time));

                pDpStruct->state = VP_DP_DETECT_STATE_LOOP_OPEN;
                pDpStruct->lo_time = 0;
            }
        break;

        default:
            return FALSE;
    }

    return eventStatus;
}

/**
 * VpInitDP()
 *  Initializes the dial pulse structure variable passed to values required by
 * VpUpdateDP.
 *
 * Preconditions:
 *  The pointer passed (Dial data structure) cannot be VP_NULL.
 *
 * Postconditions:
 *  The data passed in the dial pulse data structure is initialized.
 */
void
VpInitDP(
    VpDialPulseDetectType *pDpStruct)   /**< Dial pulse structure to init */
{
    if (pDpStruct != VP_NULL) {
        pDpStruct->digits = 0;

        if (pDpStruct->hookSt == FALSE) {
            pDpStruct->state = VP_DP_DETECT_STATE_IDLE;
            pDpStruct->lo_time = 255;
            pDpStruct->lc_time = 0;
        } else {
            pDpStruct->state = VP_DP_DETECT_STATE_LOOP_CLOSE;
            pDpStruct->lo_time = 0;
            pDpStruct->lc_time = 255;
        }
    }
    return;
}

/**
 * VpCSLACGetLineStatus()
 *  This function returns the status of the type being request for the line
 * (context) being passed.
 *
 * Preconditions:
 *  The line context pointer passed must be valid.
 *
 * Postconditions:
 *  The location pointed to by the boolean pointer passed is set to either TRUE
 * or FALSE depending on the status of the line for the type of input requested.
 * This function returns the success status code if the information requested
 * is valid for the line type (FXO/FXS) being passed.
 */
VpStatusType
VpCSLACGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    VpDialPulseDetectType *pDpStruct;
    VpApiIntLineStateType *pLineState;
    VpDeviceDynamicInfoType *pDynamicInfo;
    bool fxo;
    VpOptionPulseModeType pulseMode;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDpStruct = &((Vp790LineObjectType *)pLineObj)->dpStruct;
            pLineState = &((Vp790LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp790DeviceObjectType *)pDevObj)->dynamicInfo;
            fxo = FALSE;
            pulseMode = ((Vp790LineObjectType *)pLineObj)->pulseMode;
            break;

#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pDpStruct = &((Vp880LineObjectType *)pLineObj)->dpStruct;
            pLineState = &((Vp880LineObjectType *)pLineObj)->lineState;
            pDynamicInfo = &((Vp880DeviceObjectType *)pDevObj)->dynamicInfo;
            if (((Vp880LineObjectType *)pLineObj)->status & VP880_IS_FXO) {
                fxo = TRUE;
            } else {
                fxo = FALSE;
            }
            pulseMode = ((Vp880LineObjectType *)pLineObj)->pulseMode;
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (fxo == FALSE) {
        switch(input) {
            case VP_INPUT_HOOK:
                if(pulseMode == VP_OPTION_PULSE_DECODE_ON) {
                    if (pDpStruct->state == VP_DP_DETECT_STATE_IDLE) {
                        *pStatus = FALSE;
                    } else {
                        *pStatus = TRUE;
                    }
                } else {
                    *pStatus = (pLineState->condition & VP_CSLAC_HOOK) 
                        ? TRUE : FALSE;
                }
                break;

            case VP_INPUT_RAW_HOOK:
                *pStatus = (pLineState->condition & VP_CSLAC_HOOK) 
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_GKEY:
                *pStatus = (pLineState->condition & VP_CSLAC_GKEY) 
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_CLK_FLT:
                *pStatus = (pDynamicInfo->clkFault) ? TRUE : FALSE;
                break;

            case VP_INPUT_THERM_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_THERM_FLT) 
                    ? TRUE : FALSE;                
                break;

            case VP_INPUT_AC_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_AC_FLT) 
                    ? TRUE : FALSE;                
                break;

            case VP_INPUT_DC_FLT:
                *pStatus = (pLineState->condition & VP_CSLAC_DC_FLT) 
                    ? TRUE : FALSE;                
                break;

            case VP_INPUT_BAT1_FLT:
                *pStatus = (pDynamicInfo->bat1Fault) ? TRUE : FALSE;
                break;

            case VP_INPUT_BAT2_FLT:
                *pStatus = (pDynamicInfo->bat2Fault) ? TRUE : FALSE;
                break;

            case VP_INPUT_BAT3_FLT:
                *pStatus = (pDynamicInfo->bat3Fault) ? TRUE : FALSE;
                break;

            default:
                return VP_STATUS_INVALID_ARG;
        }
    } else {
        switch(input) {
            case VP_INPUT_RINGING:
                *pStatus = (pLineState->condition & VP_CSLAC_RINGING) 
                    ? TRUE : FALSE;                
                break;

            case VP_INPUT_POLREV:
                *pStatus = (pLineState->condition & VP_CSLAC_POLREV)
                    ? TRUE : FALSE;
                break;

            case VP_INPUT_LIU:
            case VP_INPUT_FEED_DIS:
            case VP_INPUT_DISCONNECT:
                *pStatus = (pLineState->condition & VP_CSLAC_DISC) 
                    ? TRUE : FALSE;                
                break;

            case VP_INPUT_FEED_EN:
            case VP_INPUT_CONNECT:
                *pStatus = (pLineState->condition & VP_CSLAC_DISC) 
                    ? FALSE : TRUE;                
                break;

            case VP_INPUT_BAT1_FLT:
            case VP_INPUT_BAT2_FLT:
            case VP_INPUT_BAT3_FLT:
                *pStatus = FALSE;
                break;

            default:
                return VP_STATUS_INVALID_ARG;
        }
    }
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACInitMeter()
 *  This function is used to initialize metering parameters. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function initializes metering parameters as per given profile.
 */
VpStatusType
VpCSLACInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId;
    VpProfilePtrType pMetering;
    VpCSLACDeviceProfileTableType *pDevProfTable;
    VpProfileDataType *pMpiData;
    
    uint8 channelId, ecVal, meterProfEntry;
    bool deviceInit = FALSE;
    int tableSize = VP_CSLAC_METERING_PROF_TABLE_SIZE;

    int meterIndex = GetProfileIndex(pMeterProfile);

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp790LineObjectType *)pLineObj)->channelId;
            pDevProfTable = &((Vp790DeviceObjectType *)pDevObj)->devProfileTable;
            deviceInit = (((Vp790DeviceObjectType *)pDevObj)->status.state 
                & VP_DEV_INIT_CMP);
            meterProfEntry = 
                ((Vp790DeviceObjectType *)pDevObj)->profEntry.meterProfEntry;

            switch(channelId) {
                case 0: ecVal = VP790_EC_CH1;   break;
                case 1: ecVal = VP790_EC_CH2;   break;
                case 2: ecVal = VP790_EC_CH3;   break;
                case 3: ecVal = VP790_EC_CH4;   break;
                default:
                    return VP_STATUS_FAILURE;
            }
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            channelId = ((Vp880LineObjectType *)pLineObj)->channelId;
            pDevProfTable = &((Vp880DeviceObjectType *)pDevObj)->devProfileTable;
            deviceInit = (((Vp880DeviceObjectType *)pDevObj)->status.state 
                & VP_DEV_INIT_CMP);

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_IN_CAL) {
                deviceInit = FALSE;
            }

            meterProfEntry = 
                ((Vp880DeviceObjectType *)pDevObj)->profEntry.meterProfEntry;
            
            switch(channelId) {
                case 0: ecVal = VP880_EC_CH1;   break;
                case 1: ecVal = VP880_EC_CH2;   break;
                default:
                    return VP_STATUS_FAILURE;
            }

            /* Adjust the EC value for Wideband mode as needed */
            ecVal |= ((((Vp880LineObjectType *)pLineObj)->codec == VP_OPTION_WIDEBAND)
                ? VP880_WIDEBAND_MODE : 0);
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (!(deviceInit)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }
    
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * If the profile passed is an index, make sure it's in the valid range
     * and if so, set the currently used profile to it.
     */
    if ((meterIndex >= 0) && (meterIndex < tableSize)) {
        if (!(meterProfEntry & (0x01 << meterIndex))) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_PROFILE;
        }

        pMetering = pDevProfTable->pMeteringProfileTable[meterIndex];
        /* Valid Cadence Entry. Set it if the profile has been initialized */
        if (pMetering != VP_PTABLE_NULL) {
            pMpiData = (VpProfileDataType *)(&pMetering[VP_PROFILE_MPI_LEN+1]);
            VpMpiCmd(deviceId, ecVal, NOOP_CMD,
                pMetering[VP_PROFILE_MPI_LEN], pMpiData);
        }
    } else if (meterIndex >= tableSize) {
        /* It's an index, but it's out of range */
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    } else {
        /* This is a valid metering pointer. Set it */
        pMpiData = (VpProfileDataType *)(&pMeterProfile[VP_PROFILE_MPI_LEN+1]);
        VpMpiCmd(deviceId, ecVal, NOOP_CMD,
            pMeterProfile[VP_PROFILE_MPI_LEN], pMpiData);
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACStartMeter()
 *  This function starts (can also abort) metering pulses on the line. See
 * VP-API-II documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  Metering pulses are transmitted on the line.
 */
VpStatusType
VpCSLACStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters)
{
    uint16 tempTime;
    uint8 *pIntSequence;
    VpSeqDataType *pCadence;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpOptionEventMaskType *pLineEvents;
    VpDeviceIdType deviceId;
    uint8 *pMeterCnt;
     
    bool deviceInit = FALSE;
    uint8 index;
    bool timeOops = FALSE;
        
    VpSetLineStateFuncPtrType SetLineState = 
        pDevCtx->funPtrsToApiFuncs.SetLineState;

    VpLineStateType currentType;
    uint8 seqLen, seqByte;    

    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;    

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            pIntSequence = &((Vp790LineObjectType *)pLineObj)->intSequence[0];
            pCadence = &((Vp790LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            currentType = ((Vp790LineObjectType *)pLineObj)->lineState.current; 
            seqLen = VP790_INT_SEQ_LEN;            
            deviceInit = (((Vp790DeviceObjectType *)pDevObj)->status.state 
                          & VP_DEV_INIT_CMP);
            pMeterCnt = &((Vp790LineObjectType *)pLineObj)->processData;                          
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pIntSequence = &((Vp880LineObjectType *)pLineObj)->intSequence[0];
            pCadence = &((Vp880LineObjectType *)pLineObj)->cadence;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            currentType = ((Vp880LineObjectType *)pLineObj)->lineState.currentType;
            seqLen = VP880_INT_SEQ_LEN;
            deviceInit = (((Vp880DeviceObjectType *)pDevObj)->status.state 
                          & VP_DEV_INIT_CMP);

            /*
             * Do not proceed if the device calibration is in progress. This could
             * damage the device.
             */
            if (((Vp880DeviceObjectType *)pDevObj)->status.state & VP_DEV_IN_CAL) {
                deviceInit = FALSE;
            }

            pMeterCnt = &((Vp880LineObjectType *)pLineObj)->processData;                          
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    if (!(deviceInit)) {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* 
     * If we're not in a valid line state where metering is possible, generate
     * an event that metering was aborted for FXS lines and return success. 
     * Error on FXO lines.
     */
    switch(currentType) {
        case VP_LINE_TIP_OPEN:
        case VP_LINE_DISCONNECT:
            pLineEvents->process |= VP_LINE_EVID_MTR_ABORT;
            *pMeterCnt = pCadence->meteringBurst;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_SUCCESS;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_TALK:
        case VP_LINE_FXO_RING_GND:
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG; 

        default:
            break;
    }

    /* Clear out the internal sequencer */
    for (seqByte = 0; seqByte < seqLen; seqByte++) {
        pIntSequence[seqByte] = 0x00;
    }

    /* Stop all other cadences if they were active */
    pCadence->status = VP_CADENCE_RESET_VALUE;

    /* 
     * Number of meters = 0 will stop metering. Generate the event, report 
     * number of metering pulses generated to this point and return.
     */
    if (numMeters == 0) {
        pLineEvents->process |= VP_LINE_EVID_MTR_ABORT;
        *pMeterCnt = pCadence->meteringBurst;
        pCadence->pActiveCadence = VP_PTABLE_NULL;
        SetLineState(pLineCtx, currentType);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* 
     * We are starting a new metering session. Reset the metering pulse count
     * that is used if metering is aborted.
     */
    pCadence->meteringBurst = 0;

    /* 
     * Build the sequence in the order that it will be executed. Not necessary,
     * but makes reading this code easier.
     */
     
    /* Set the type of profile to "metering internal" */
    pIntSequence[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_METERING_GEN;

    /* And set the starting command to start metering */
    index = 0;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_METERING);
    index++;        
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START + index] = 0x01;

    /* 
     * Set the metering on time after metering is enabled. Time command takes
     * two bytes. Metering time is specified in 10mS incr, Cadence in 5mS.
     * If the time specified is non-zero by the user, prevent the math from
     * converting it to 0.
     */    
    index++; 
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 
        VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = onTime * 2;  /* Change from 10mS to 5mS ticks */
    if (tempTime) {
        tempTime = ((tempTime - 2) >> 8) & 0x03;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;
        if (tempTime == 0) {
            timeOops = TRUE;
        }
        index++;        
        tempTime = ((onTime * 2 - 2) & 0x00FF);
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;
        if (tempTime == 0) {
            timeOops = TRUE;
        }

        if (timeOops == TRUE) {
            pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= 1;
        }
    } else {
        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;
    }

    /* 
     * If the on-time is = 0, the Sequencer automatically suspends indefinitely 
     * at the current state. Otherwise, it will proceed to the next step.
     */

    /* Then turn the metering off */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] 
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_METERING);
    
    index++;    /* This should set index = 5 at this point */        
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;
    
    /* 
     * Set the metering off time after metering is enabled. Time command takes
     * two bytes. Metering time is specified in 10mS incr, Cadence in 5mS.
     * If the time specified is non-zero by the user, prevent the math from
     * converting it to 0.
     */    
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 
        VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = offTime * 2;  /* Change from 10mS to 5mS ticks */
    if (tempTime) {
        timeOops = FALSE;
        tempTime = ((tempTime - 2) >> 8) & 0x03;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;
        
        if (tempTime == 0) {
            timeOops = TRUE;
        }

        index++;
        tempTime = ((offTime * 2 - 2) & 0x00FF);
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= tempTime;

        if (tempTime == 0) {
            timeOops = TRUE;
        }

        if (timeOops == TRUE) {
            pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] |= 1;
        }
    } else {
        index++;
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 0x00;
    }

    /* 
     * Condition of numMeters = 0 (stop metering) is taken care of at top.
     * Then, condition numMeters != 0 and on-time is (meter forever) is taken
     * care of as a consequence of the Sequencer (i.e., suspend for any time 
     * operator set to 0).
     * All other operators will count some number of metering pulses, end, then
     * go back to the state the line was in when the sequence started.
     */

    /* Can only be 1 or > 1, not 0 */ 
    if (numMeters > 1) {
        /* 
         * If more than 1, we'll branch back to the start of the metering on
         * until all metering pulses occur.
         */
        index++; 
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 
            VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;

        index++;            
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = numMeters - 1;
    } else {
        /* If exactly equal to 1, no branch is necessary. */
    }
    
    /* 
     * When the metering is complete, return to the current line state. Note
     * that this step is not reached if metering is infinite (on-time = 0)
     */
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 
        (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
    index++;
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_START+index] = 
        ConvertApiState2PrfWizState(currentType);

    /* Sequence Length is index + 1 because index is 0 based */
    pIntSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index + 1;

    /* 
     * Profile Length is always sequence lengh + 4 because of header 
     * definition.
     */
    pIntSequence[VP_PROFILE_LENGTH] = 
        pIntSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] + 4;
    
    pCadence->index = VP_PROFILE_TYPE_SEQUENCER_START;
    pCadence->length = pIntSequence[VP_PROFILE_LENGTH];
    
    pCadence->pActiveCadence = &pIntSequence[0];
    pCadence->pCurrentPos = &pIntSequence[8];

    pCadence->status |= VP_CADENCE_STATUS_ACTIVE;
    pCadence->status |= VP_CADENCE_STATUS_METERING;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACClearResults()
 *  This function clears the device read events so that normal read operations
 * may occur. It is done by the application when the device is busy (in read
 * moode) and the application does not know what to read.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized.
 *
 * Postconditions:
 *  Device is no longer busy with read status.
 */
VpStatusType
VpCSLACClearResults(
    VpDevCtxType *pDevCtx)
{
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpDeviceIdType deviceId;
    VpOptionEventMaskType *pDeviceEvents;

    void *pDevObj = pDevCtx->pDevObj;

    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pDeviceEvents = &((Vp790DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pDeviceEvents = &((Vp880DeviceObjectType *)pDevObj)->deviceEvents;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDeviceEvents->response &= (~VP_CSLAC_READ_RESPONSE_MASK);
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    
    return VP_STATUS_SUCCESS;
}

/**
 * VpCSLACDtmfDigitDetected()
 *  This function is used to set a value in the API-II CSLAC library (where
 * applicable) indicating that a DTMF digit was detected in an external 
 * application/process.
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
VpCSLACDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpDeviceIdType deviceId;
    VpDeviceType deviceType = pDevCtx->deviceType;
    VpCallerIdType *pCidStruct;
    VpOptionEventMaskType *pLineEvents;
    uint16p pDtmfDigitSense;
    
    void *pLineObj = pLineCtx->pLineObj;
    void *pDevObj = pDevCtx->pDevObj;
    
    switch (deviceType) {
#if defined (VP_CC_790_SERIES)
        case VP_DEV_790_SERIES:
            pCidStruct = &((Vp790LineObjectType *)pLineObj)->callerId;
            deviceId = ((Vp790DeviceObjectType *)pDevObj)->deviceId;
            pLineEvents = &((Vp790LineObjectType *)pLineObj)->lineEvents;
            pDtmfDigitSense = &((Vp790LineObjectType *)pLineObj)->dtmfDigitSense;
            break;
#endif

#if defined (VP_CC_880_SERIES)
        case VP_DEV_880_SERIES:
            pCidStruct = &((Vp880LineObjectType *)pLineObj)->callerId;
            deviceId = ((Vp880DeviceObjectType *)pDevObj)->deviceId;
            pLineEvents = &((Vp880LineObjectType *)pLineObj)->lineEvents;
            pDtmfDigitSense = &((Vp880LineObjectType *)pLineObj)->dtmfDigitSense;
            break;
#endif
        default:
            return VP_STATUS_INVALID_ARG;
    }

    switch (sense) {
        case VP_DIG_SENSE_BREAK:
        case VP_DIG_SENSE_MAKE:
            /* 
             * If the CID sequencer is waiting for a CPE ACK tone, report
             * the DTMF event to the CID sequencer, but mask it from the
             * application.
             */
            if ((pCidStruct->status & VP_CID_AWAIT_TONE) != 0) {
                VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
                pCidStruct->digitDet = digit;
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_SUCCESS;
            }
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }        

    /* Toggle the DTMF_DIG event.  If two DTMF_DIG events are received within
     * the same API tick period, report neither. */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pLineEvents->signaling ^= VP_LINE_EVID_DTMF_DIG;
    *pDtmfDigitSense = digit | sense;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    
    return VP_STATUS_SUCCESS;
}

/**
 * VpConvertCsd2Fixed()
 *  This function returns a 2.14 fixed-point number whose value matches (as
 * nearly as possible) the value of a given CSD (canonical signed digit)
 * number.
 *
 * Preconditions:
 *  The CSD number must be split into a two-byte array consisting of the high
 * byte followed by the low byte.  Its value must be between 0 and 4.0.
 *
 * Postconditions:
 *  If the value of the passed CSD number is less than 0, 0 will be returned.
 * If the value is greater than or equal to 4.0, 65535 will be returned,
 * which means 3.99994 in 2.14 representation.
 */

uint16
VpConvertCsd2Fixed(
    uint8 *csdBuf)
{
    uint16 csd = (csdBuf[0] << 8) + csdBuf[1];
    int32 result;
    int8 bitPos, C, m;

    /* 2.14 fixed-point format has values ranging from 0 to 3.999.... The bits
     * have the following values:
     *
     *   bit    15    14    13    12          2     1     0
     *       +-----+-----+-----+-----+     +-----+-----+-----+
     * value | 2^1 | 2^0 | 2^-1| 2^-2| ... |2^-12|2^-13|2^-14|
     *       +-----+-----+-----+-----+     +-----+-----+-----+
     */

    /*
     * CSD format is as follows:
     *
     *   bit   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
     *       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * field |C40|    m40    |C30|    m30    |C20|    m20    |C10|    m10    |
     *       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *
     * where the represented value is calculated as follows (Cxy = 0 or 1
     * above corresponds to Cxy = 1 or -1, respectively, below):
     *
     *      C10 * 2^-m10 * (
     *          1 + C20 * 2^-m20 * (
     *              1 + C30 * 2^-m30 * (
     *                  1 + C40 * 2^-m40 )))
     */

    /*
     * Alternate formula which can be computed with unsigned ints:
     *
     *          C10 * 2^-m10
     *        + C10 * C20 * 2^(-m10-m20)
     *        + C10 * C20 * C30 * 2^(-m10-m20-m30)
     *        + C10 * C20 * C30 * C40 * 2^(-m10-m20-m30-m40)
     */

    C = m = result = 0;
    for (bitPos = 0; bitPos < 16; bitPos += 4) {
        C ^= (csd >> (bitPos + 3)) & 1;
        m += (csd >> bitPos) & 7;
        if (C == 0) {
            result += 0x4000 >> m;
        } else {
            result -= 0x4000 >> m;
        }
    }

    /* Ensure that the result can be stored in a uint16. */
    if (result > 0xFFFF) {
        result = 0xFFFF;
    } else if (result < 0) {
        result = 0;
    }

    return result;
} /* VpConvertCsd2Fixed() */

/**
 * VpConvertFixed2Csd()
 *  This function returns a four-nibble CSD (canonical signed digit) number
 * whose value matches (as nearly as possible) the supplied 2.14 fixed-point
 * number.
 *
 * Preconditions:
 *
 * Postconditions:
 *  The CSD number will be placed into a two-byte array (high byte first) at
 * the address specified in the csdBuf parameter.
 */
void
VpConvertFixed2Csd(
    uint16 fixed,
    uint8 *csdBuf)
{
#define CSD_NIBBLES 4
    uint16 error, power, greaterPower, smallerPower, distGreater, distSmaller;
    uint16 C, m, result, sum = 0;
    int8 n, gp, sp;

    /* Data structure for holding the four terms composing the CSD number. */
    typedef struct {
        bool sign;
        int power;
    } term;
    term t[CSD_NIBBLES + 1];
    t[0].power = 0;
    t[0].sign = 0;

    /*
     * Split the 2.14 value into a sum of powers of 2,
     *   s1 * 2^p1  +  s2 * 2^p2  +  s3 * 2^p3  +  s4 * 2^p4
     * where for term x,
     *   sx = 1 or -1,
     *   px <= 0.
     */
    for (n = 1; n <= CSD_NIBBLES; n++) {

        if (sum == fixed) break;

        /*
         * If current sum is less than actual value, then the next term
         * should be added; otherwise the next term should be
         * subtracted.
         */
        if (sum < fixed) {
            t[n].sign = 0;
            error = fixed - sum;
        } else {
            t[n].sign = 1;
            error = sum - fixed;
        }

        /* If error > 1, then term = +/-1. */
        if (error > 0x4000) {
            t[n].power = 0;
        } else {

            /*
             * Calculate greaterPower = the smallest power of 2 greater
             * than error.  Calculate smallerPower = the largest power
             * of 2 less than error.
             */
            greaterPower = 0x4000; gp = 0;
            for (power = 0x2000; power > error; power >>= 1) {
                greaterPower >>= 1; gp--;
            }
            smallerPower = greaterPower >> 1; sp = gp - 1;

            /*
             * Is error closer to greaterPower or smallerPower?
             * Whichever is closer, choose that for the value of the
             * next term.
             */
            distGreater = greaterPower - error;
            distSmaller = error - smallerPower;
            if (distGreater < distSmaller) {
                t[n].power = gp;
            } else {
                t[n].power = sp;
            }

            /*
             * The power of this term can differ from the power of the
             * previous term by no more than 7.
             */
            if (t[n - 1].power - t[n].power > 7) {
                t[n].power = t[n - 1].power - 7;
            }
        }

        /* Add or subtract the term to the sum, depending on sign. */
        if (t[n].sign == 0) {
            sum += (uint16)1 << (14 + t[n].power);
        } else {
            sum -= (uint16)1 << (14 + t[n].power);
        }
    }

    /*
     * If we reached the exact value with terms left over, fill these
     * extra terms with dummy values which don't affect the CSD value.
     */
    while (n <= CSD_NIBBLES) {
        if (n == 1) {
            t[1] = t[0];
            t[2].power = 0;
            t[2].sign = 1;
            n += 2;
        } else {
            /*
             * Increase the number of terms by replacing the last term
             * with two new terms whose sum is the old term.
             */
            if (t[n - 1].power == t[n - 2].power) {
                t[n - 1].power--;
                t[n] = t[n - 1];
            } else {
                t[n] = t[n - 1];
                t[n - 1].power++;
                t[n].sign = !(t[n - 1].sign);
            }
            n++;
        }
    }

    /* Compute nibble values from the terms. */
    result = 0;
    for (n = 1; n <= CSD_NIBBLES; n++) {
        int8 bitPos = (n - 1) * 4;
        C = (t[n].sign != t[n - 1].sign);
        m = -(t[n].power - t[n - 1].power);
        result |= (C << (bitPos + 3)) | (m << bitPos);
    }

    /* Split the uint16 result into high and low bytes. */
    csdBuf[0] = (uint8)(result >> 8);
    csdBuf[1] = (uint8)(result & 0xFF);
} /* VpConvertFixed2Csd() */

#endif /* VP_CC_880_SERIES || VP_CC_790_SERIES */






