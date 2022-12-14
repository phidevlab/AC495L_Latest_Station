/** \file apicnt.c
 * apicnt.c
 *
 *  This file contains the control functions for the Vp790 device API.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_790_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp790_api.h"
#include "vp790_api_int.h"
#include "sys_service.h"

/* 
 * Locally used function as part of the 5/3 CID message conversion per the 
 * VE790 User's Guide
 */
static VpStatusType
Vp790GetTxRxPcmMode(
    Vp790LineObjectType *pLineObj,
    VpLineStateType state,
    uint8 *mpiByte);

static VpStatusType
Vp790SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value);  

/* Functions called by SetOptionInternal only */
static void
Vp790MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask,
    VpOptionEventMaskType *pDevEventsMask);      

static VpStatusType
Vp790SetCodec(
    VpLineCtxType *pLineCtx,
    VpOptionCodecType codec);

static VpStatusType 
Vp790SetTimeSlot(
    VpLineCtxType *pLineCtx, 
    uint8 txSlot, 
    uint8 rxSlot);
    
/* Function for setting DTMF generators in special case for SetLineTone */
static VpStatusType
Vp790SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);
    
static VpStatusType
Vp790SendDigit(
    VpLineCtxType *pLineCtx,
    VpDigitGenerationType digitType,
    VpDigitType digit);

/* Function called by API-II functions only to implement Caller ID */    
static uint8
Vp790MirrorByte(
    char byte);

/* 
 * Helper function to determine the TX/RX mode currently set based on line 
 * state, "talk mode" option, and caller ID mode. Called by functions in this
 * file only.
 */
static VpStatusType
Vp790GetTxRxPcmMode(
    Vp790LineObjectType *pLineObj,
    VpLineStateType state,
    uint8 *mpiByte);    
    
/**
 * Vp790SetLineState()
 *  This function is the API-II wrapper function for Set Line State - Internal
 * for the Vp790 API.
 *
 * Preconditions:
 *  Same as Vp790SetLineStateInt()
 *
 * Postconditions:
 *  Same as Vp790SetLineStateInt()
 */
VpStatusType
Vp790SetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpStatusType status;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    /* 
     * Proceed if device state is either in progress or complete and not 
     * calibrating
     */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Clear the "called from API" flag. This affects the cadencer */
    pLineObj->slsCalledFromApi = FALSE;
    status = Vp790SetLineStateInt(pLineCtx, state);
    if (status == VP_STATUS_SUCCESS) {
        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
         && ((state == VP_LINE_RINGING_POLREV) || (state == VP_LINE_RINGING))) {
            pLineObj->lineState.usrCurrent = pLineObj->ringCtrl.ringTripExitSt;
        } else {
            pLineObj->lineState.usrCurrent = state;
        }
    }

    /*
     * Set the "called from API" flag. Convenience for API functions so setting
     * this flag does not need to occur in multiple locations
     */
    pLineObj->slsCalledFromApi = TRUE;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
}

/**
 * Vp790SetLineStateInt()
 *  This function sets the line state for a given channel of a given device. The 
 * valid line states are defined in the VpLineState type.
 *
 * Preconditions:
 *  The line must first be initialized prior to setting the line state.  The 
 * state must be a valid line state as defined in the VpLineState type.
 *
 * Postconditions:
 *  Returns success code if the channel can be set and the line state is valid 
 * for the type of line specified by the line context.  If successfull, the line 
 * specified by the line context is set to the line state specified.
 */
VpStatusType
Vp790SetLineStateInt(
    VpLineCtxType *pLineCtx,    /**< Line context to change line state on */
    VpLineStateType state)      /**< The desired line state to set */
{
    uint8 data = 0x00;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpProfilePtrType pProfile;
    VpStatusType status = VP_STATUS_SUCCESS;
    
    uint8 channelId = pLineObj->channelId;
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 ccr1[VP790_CCR1_LEN];
    uint8 ccr6[VP790_CCR6_LEN];
    uint8 ccr6Mod;
    uint8 currentLineState; /* Used to determine if we're causing a pol rev */

    /* Make sure it's a linestate we can support */
    if ((state < VP_LINE_STANDBY) || (state > VP_LINE_RINGING)) {
        return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_RD, VP790_CCR1_LEN, ccr1);
    ccr1[0] &= ~(VP790_CUT_TXPATH | VP790_CUT_RXPATH);

    status = Vp790GetTxRxPcmMode(pLineObj, state, &ccr6Mod);
    if (status == VP_STATUS_SUCCESS) {
        ccr1[0] |= ccr6Mod;
        VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_WRT, VP790_CCR1_LEN, 
            ccr1);
    } else {
        return status;
    }

    /* We're no longer in the middle of a time function */
    pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
    pLineObj->cadence.timeRemain = 0;

    /* If this function is called by the application, stop the cadencer. */
    if (pLineObj->slsCalledFromApi == FALSE) {
        if ((pLineObj->cadence.status & VP_CADENCE_STATUS_METERING)
         && (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
            pLineObj->lineEvents.process |= VP_LINE_EVID_MTR_ABORT;
            pLineObj->processData = pLineObj->cadence.meteringBurst;
        }
    
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
    }

    /*
     * Special FXS handling to prevent setting the line to ringing if already
     * off-hook
     */
    if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
     && (state == VP_LINE_RINGING)) {
        state = VP_LINE_TALK;
    }
    
    /* Start of D2 errata operational issues #7 */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_RD, VP790_CCR6_LEN, ccr6);

    switch(pLineObj->lineStateBatOption.bat) {
        case VP_OPTION_BAT_AUTO:
            data = VP790_AUTO_BAT_SELECTION;
            break;

        case VP_OPTION_BAT_HIGH:
            data = VP790_HIGH_BAT_SELECTION;
            break;
            
        case VP_OPTION_BAT_LOW:
            data = VP790_LOW_BAT_SELECTION;
            break;

        case VP_OPTION_BAT_BOOST:
            data = VP790_BOOST_BAT_SELECTION;
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
     && (state == VP_LINE_RINGING)) {
        state = VP_LINE_TALK;
    }

    /* Get the data to set in the register corresponding to the desired state */
    switch (state) {
        case VP_LINE_STANDBY:
            data = VP790_SLIC_ST_STANDBY;
            pLineObj->lineState.condition |= VP_CSLAC_CAL_ENABLE;
            break;

        case VP_LINE_TIP_OPEN:
            data = VP790_SLIC_ST_TIPOPEN;
            pLineObj->lineState.condition |= VP_CSLAC_CAL_ENABLE;
            break;

        case VP_LINE_ACTIVE_POLREV:
            data |= VP790_SLIC_ST_POLREV;
        case VP_LINE_ACTIVE:
            data |= VP790_SLIC_ST_ACTIVE;
            pLineObj->lineState.condition &= ~(VP_CSLAC_CAL_ENABLE);
            break;

        case VP_LINE_TALK_POLREV:
            data |= VP790_SLIC_ST_POLREV;
        case VP_LINE_TALK:
            data |= VP790_SLIC_ST_ACTIVE;
            pLineObj->lineState.condition &= ~(VP_CSLAC_CAL_ENABLE);
            break;

        case VP_LINE_OHT_POLREV:
            data = (VP790_SLIC_ST_POLREV | VP790_SLIC_ST_OHT);
            pLineObj->lineState.condition &= ~(VP_CSLAC_CAL_ENABLE);
            break;
            
        case VP_LINE_OHT:
            data = VP790_SLIC_ST_OHT;
            pLineObj->lineState.condition &= ~(VP_CSLAC_CAL_ENABLE);
            break;

        case VP_LINE_DISCONNECT:
            data = VP790_SLIC_ST_DISCONNECT;
            pLineObj->lineState.condition |= VP_CSLAC_CAL_ENABLE;            
            break;

        case VP_LINE_RINGING_POLREV:
        case VP_LINE_RINGING:
            pLineObj->cadence.pActiveCadence = pLineObj->pRingingCadence;
            pProfile = pLineObj->cadence.pActiveCadence;
            if (pProfile == VP_PTABLE_NULL) {
                /* Always on - no need to cadence */
                pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            } else {
                if (!(pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
                    /* We have a cadence and are just starting it */
                    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
                    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
                    pLineObj->cadence.pCurrentPos = 
                        &pProfile[VP_PROFILE_TYPE_SEQUENCER_START];
                    pLineObj->cadence.length = pProfile[VP_PROFILE_LENGTH];
                    pLineObj->cadence.status &= ~VP_CADENCE_STATUS_BRANCHING;
                    return VP_STATUS_SUCCESS;
                }
            }
            /* Cadencing already called, just execute the Ringing State */

            /* 
             * 2nd step of D2 errata operational issues #7 if entering 
             * ringing
             */
            ccr6[0] |= VP790_MASK_TEMPA_INTR;
            VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_WRT, VP790_CCR6_LEN, 
                ccr6);            

            data |= VP790_SLIC_ST_RINGING;
            pLineObj->lineState.condition &= ~(VP_CSLAC_CAL_ENABLE);

            /* 
             * If we're in an active Ringing Cadence, and ready to go into the
             * Ringing state, generate the Ringing Event and indicate that this
             * is the Ringing On event
             */
            if (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) {
                if (pProfile[VP_PROFILE_TYPE_LSB] == VP_PRFWZ_PROFILE_RINGCAD) {
                    pLineObj->lineEvents.process |= VP_LINE_EVID_RING_CAD;
                    pLineObj->processData = VP_RING_CAD_MAKE;
                }
            }
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* If we are exiting ringing, set the Ringing exit timer */
    switch (pLineObj->lineState.current) {
        case VP_LINE_RINGING:
        case VP_LINE_RINGING_POLREV:
            switch(state) {
                case VP_LINE_RINGING:
                case VP_LINE_RINGING_POLREV:
                    break;

                default:
                    /* 
                     * We're exiting ringing, so first set the line timer
                     * debounce so we don't generate false ring trips
                     */
                    if (!(pLineObj->lineState.condition & VP_CSLAC_HOOK)) {
                        pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE] =
                            MS_TO_TICKRATE(pLineObj->ringCtrl.ringExitDbncDur / 8, 
                                pDevObj->devProfileData.tickRate);

                        if (pLineObj->ringCtrl.ringExitDbncDur) {
                            pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE]
                                |= VP_ACTIVATE_LINE_TIMER;
                        }
                    }
                    /* 
                     * Reset the Thermal Fault mask (D2 errata) if the user
                     * originally wanted it unmasked
                     */
                    
                    if (!(pLineObj->lineEventsMask.faults
                        & VP_LINE_EVID_THERM_FLT)) {
                        ccr6[0] &= ~VP790_MASK_TEMPA_INTR;
                    }
                     
                    VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_WRT, 
                        VP790_CCR6_LEN, ccr6);            
                    /* End of D2 errata operational issues #7 */

                    /* 
                     * If we're in an active Ringing Cadence, and ready to exit 
                     * the Ringing state, generate the Ringing Event indicating
                     * that this is the Ringing Off event
                     */
                    if (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) {
                        pProfile = pLineObj->cadence.pActiveCadence;
                        if (pProfile[VP_PROFILE_TYPE_LSB] 
                            == VP_PRFWZ_PROFILE_RINGCAD) {
                            pLineObj->lineEvents.process |= VP_LINE_EVID_RING_CAD;
                            pLineObj->processData = VP_RING_CAD_BREAK;
                        }
                    }
                    break;
            }
            break;

        default:
            break;
    }

    /* 
     * If we were in disconnect, but are exiting, make sure the device is set 
     * back to Activate
     */

    if ((state != VP_LINE_DISCONNECT) 
      && (pLineObj->lineState.current == VP_LINE_DISCONNECT)) {
        VpMpiCmd(deviceId, ecVal[channelId], VP790_ACTIVATE_CMD, NO_DATA, 
            VP_NULL);
    }


    VpMpiCmd(deviceId, ecVal[channelId], VP790_SLIC_STATE_RD,
        VP790_SLIC_STATE_LEN, &currentLineState);

    if ((currentLineState & VP790_SLIC_ST_POLREV) !=
        (data & VP790_SLIC_ST_POLREV)) {
        pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE] =
            MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
                pDevObj->devProfileData.tickRate);
        pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
            |= VP_ACTIVATE_LINE_TIMER;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP790_SLIC_STATE_WRT, 
        VP790_SLIC_STATE_LEN, &data);
        
    /* 
     * If we are not entering or exiting ringing, this results in a rewrite of 
     * the same value that existed when we entered this function.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_WRT, VP790_CCR6_LEN, ccr6);            

    pLineObj->lineState.previous = pLineObj->lineState.current;
    pLineObj->lineState.current = state;

    return VP_STATUS_SUCCESS;
} /* End Vp790SetLineState */

/**
 * Vp790GetTxRxPcmMode()
 *  This function returns the TX/RX PCM bits for the PCM (enable/disable) mode
 * corresponding to the state passed. The results should be or'-ed with the
 * bits set to 0 prior to calling this function.
 *
 * Preconditions:
 *  None. Mapping function only.
 *
 * Postconditions:
 *  None. Mapping function only.
 */
VpStatusType
Vp790GetTxRxPcmMode(
    Vp790LineObjectType *pLineObj,
    VpLineStateType state,
    uint8 *mpiByte)
{
    switch(pLineObj->pcmTxRxCtrl) {
        case VP_OPTION_PCM_BOTH:
            *mpiByte = 0x00;
            break;
            
        case VP_OPTION_PCM_RX_ONLY:
            *mpiByte = VP790_CUT_TXPATH;
            break;
            
        case VP_OPTION_PCM_TX_ONLY:
            *mpiByte = VP790_CUT_RXPATH;
            break;

        default:
            *mpiByte = 0x00;
            break;
    }

    switch(state) {
        /* Non-Talk States */
        case VP_LINE_STANDBY:
        case VP_LINE_TIP_OPEN:
        case VP_LINE_ACTIVE:
        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_DISCONNECT:
        case VP_LINE_RINGING:
        case VP_LINE_RINGING_POLREV:
            *mpiByte |= (VP790_CUT_TXPATH | VP790_CUT_RXPATH);
            break;

        /* Talk States */
        case VP_LINE_TALK:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT:
        case VP_LINE_OHT_POLREV:
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790MuteChannel()
 *  This function disables or enables the PCM highway for the selected line and
 * should only be called by API internal functions.
 *
 * Preconditions:
 *  The line context must be valid (i.e., pointing to a valid Vp790 line object
 * type).
 *
 * Postconditions:
 *  If mode is TRUE the TX/RX path is cut. If FALSE, the TX/RX path is enabled.
 */
void 
Vp790MuteChannel(
    VpLineCtxType *pLineCtx,    /**< Line affected */
    bool mode)                  /**< TRUE = Disable TX/RX, FALSE = enable */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 preState, postState, mpiByte; 

    /* 
     * Read the status of the Channel Configuration register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_RD, VP790_CCR1_LEN, 
        &preState);
    postState = preState;
    postState &= ~(VP790_CUT_TXPATH | VP790_CUT_RXPATH);

    /* 
     * If disabling, simple. Otherwise enable based on the current line state
     * and the state of the "talk" option. The "talk" option is maintained in
     * the line object and abstracted in Vp790GetTxRxMode() function
     */

    Vp790GetTxRxPcmMode(pLineObj, pLineObj->lineState.current, &mpiByte); 

    if (mode == TRUE) {
        /* 
         * If awaiting DTMF detection, enable TX, disable RX. This is higher
         * priority than Mute mode. Otherwise, disable both TX and RX.
         */

        postState |= VP790_CUT_RXPATH;  /* Mute == TRUE always cuts RX path */        
        if (!(pLineObj->callerId.status & VP_CID_AWAIT_TONE)) {
            /* Not awaiting tone, TX Path is disabled as well */
            postState |= VP790_CUT_TXPATH;
        }
    } else {
        /* 
         * It's possible that a Mute off is occuring because of end of DTMF
         * detection, or end of data generation, or end of Mute period. However,
         * we only need to check if Mute On is still enabled since DTMF 
         * detection will not occur while data is being generated.
         */
        if (pLineObj->callerId.status & VP_CID_MUTE_ON) {
            /* 
             * Some "other" operation completed, but we're still in a Mute On
             * period.
             */
            postState |= (VP790_CUT_RXPATH | VP790_CUT_TXPATH);
        } else  {
            postState |= mpiByte;
        }
    }

    if (postState != preState) {
        VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_WRT, VP790_CCR1_LEN, 
            &postState);            
    }
    return;
}

/**
 * Vp790CommandInstruction()
 *  This function implements the Sequencer Command instruction for the Vp790 
 * device type.
 *
 * Preconditions:
 *  The line must first be initialized and the sequencer data must be valid.
 *
 * Postconditions:
 *  The command instruction currently being pointed to by the sequencer 
 * instruction passed is acted upon.  The sequencer may or may not be advanced, 
 * depending on the specific command instruction being executed.
 */
VpStatusType
Vp790CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl, lineState;
    uint8 telTax[VP790_CCR5_LEN];

    switch (pSeqData[0] & VP_SEQ_SUBTYPE_MASK) {
        case VP_SEQ_SUBCMD_SIGGEN:
            VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_RD, 
                VP790_GEN_CTRL_LEN, &sigGenCtrl);
            sigGenCtrl &= ~(VP790_GENB_EN);

            /* Get the signal generator bits and set. Only B Control possible */
            sigGenCtrl |= ((pSeqData[1] & 0x01) ?  VP790_GENB_EN : 0);
            sigGenCtrl |= ((pSeqData[1] & 0x02) ?  VP790_GENB_EN : 0);
            VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_WRT, 
                VP790_GEN_CTRL_LEN, &sigGenCtrl);
            break;

        case VP_SEQ_SUBCMD_LINE_STATE:
            switch(pSeqData[1]) {
                case VP_PROFILE_CADENCE_STATE_STANDBY:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_STANDBY);
                    break;                  

                case VP_PROFILE_CADENCE_STATE_TIP_OPEN:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_TIP_OPEN);
                    break;                  

                case VP_PROFILE_CADENCE_STATE_TALK:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_TALK);
                    break;                  

                case VP_PROFILE_CADENCE_STATE_ACTIVE:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_ACTIVE);
                    break;                  

                case VP_PROFILE_CADENCE_STATE_OHT:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_OHT);
                    break;

                case VP_PROFILE_CADENCE_STATE_DISCONNECT:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_DISCONNECT);
                    break;

                case VP_PROFILE_CADENCE_STATE_RINGING:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_RINGING);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_RINGING:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_RINGING_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_TALK_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_OHT:
                    Vp790SetLineStateInt(pLineCtx, VP_LINE_OHT_POLREV);
                    break;

                default:
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_SEQ_SUBCMD_START_CID:
            if (pLineObj->pCidProfileType1 != VP_PTABLE_NULL) {
                pLineObj->callerId.pCliProfile = pLineObj->pCidProfileType1;

                pLineObj->callerId.status |= VP_CID_IN_PROGRESS;
                pLineObj->callerId.cliTimer = 1;

                pLineObj->callerId.cliIndex = 0;
                /* 
                 * Three byte offset in the primary buffer because an "init"
                 * CID function would have processed the first three bytes and
                 * moved the converted data to the converted buffer
                 */
                pLineObj->callerId.cliMPIndex = 3;  
                pLineObj->callerId.cliMSIndex = 0;    

                pLineObj->callerId.status &= ~VP_CID_SIG_B_VALID;

                pLineObj->callerId.status |= VP_CID_PRIMARY_IN_USE;
                pLineObj->callerId.status &= ~VP_CID_SECONDARY_IN_USE;
            }
            break;

        case VP_SEQ_SUBCMD_WAIT_ON:
            /* 
             * We need to wait on either a CID completion or time operator.
             * Easiest to set a "cid wait" flag and let the time function 
             * read it to determine if it's in a "waiting" mode. If so, then
             * it will determine if cid is in progress. If cid is in progress, 
             * it will continue it's count and proceed otherwise as normal. If
             * cid is not in progress (with "waiting" flag set), it will 
             * terminate the timer operation immediately and clear the waiting
             * flag.
             */
            pLineObj->callerId.status |= VP_CID_WAIT_ON_ACTIVE;
            break;

        case VP_SEQ_SUBCMD_METERING:
            if (pSeqData[1]) {  /* Metering On */
                /* 
                 * Signal Generator A must be disabled before this point
                 * But, that shouldn't be a problem since this API only uses
                 * Generator A for Ringing, so by changing the SLIC state the 
                 * generator will automatically be disabled.
                 * Just keep it in mind.
                 */

                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR5_RD,
                    VP790_CCR5_LEN, telTax);
                if (telTax[0] & VP790_TELATAX_POLREV) {
                    pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE] =
                        MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
                            pDevObj->devProfileData.tickRate);
                    pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                        |= VP_ACTIVATE_LINE_TIMER;
                }

                lineState = (VP790_SLIC_ST_TELETAX | VP790_SLIC_ST_HIGH_BATT);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_SLIC_STATE_WRT, 
                    VP790_SLIC_STATE_LEN, &lineState);
                pLineObj->cadence.meteringBurst++;
            } else {    /* Metering Off */
                Vp790SetLineStateInt(pLineCtx, pLineObj->lineState.current);
            }
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    pLineObj->cadence.index+=2;        
    /* 
     * Check to see if there is more sequence data, and if so, move the
     * sequence pointer to the next command. Otherwise, end this cadence
     */
    if ((pLineObj->cadence.length + VP_PROFILE_LENGTH + 1) > 
         pLineObj->cadence.index) {
        pSeqData+=2;
        pLineObj->cadence.pCurrentPos = pSeqData;
    } else {
        switch(pLineObj->cadence.pActiveCadence[VP_PROFILE_TYPE_LSB]) {
            case VP_PRFWZ_PROFILE_METERING_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_MTR_CMP;
                break;
            
            case VP_PRFWZ_PROFILE_RINGCAD:
                pLineObj->lineEvents.process |= VP_LINE_EVID_RING_CAD;
                pLineObj->processData = VP_RING_CAD_DONE;
                break;

            case VP_PRFWZ_PROFILE_TONECAD:
                pLineObj->lineEvents.process |= VP_LINE_EVID_TONE_CAD;
                break;

            case VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_HOOK_FLASH;
                break;
                
            case VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_PULSE_DIGIT;
                break;
            
            case VP_PRFWZ_PROFILE_DTMF_DIG_GEN:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_DTMF_DIGIT;
                break;
            
            default:
                break;
            
        }
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790SetLineTone()
 *  This function sets the line tone with the cadence specified on the line.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The tone specified by the tone profile is sent on the line at the cadence 
 * specified by the cadence profile.  If the tone is NULL, all line tones are 
 * removed.  If the cadence is NULL, the cadence is set to "Always On".  This 
 * function returns the success code if the tone cadence is a valid tone cadence
 * and the tone profile is a valid tone profile, or in the case where the user
 * passes in profile indexes, if the tone/cadence indexes are within the range 
 * of the device.
 */
VpStatusType
Vp790SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,  /**< A pointer to a tone profile, or an 
                                     * index into the profile table for the tone
                                     * to put on the line.
                                     */
    VpProfilePtrType pCadProfile,   /**< A pointer to a tone cadence profile, or 
                                     * an index into the profile table for the 
                                     * tone cadence to put on the line.
                                     */
    VpDtmfToneGenType *pDtmfControl)    /**< Indicates to send a DTMF tone
                                         * (either upstream or downstream) if
                                         * this parameter is not VP_NULL AND
                                         * the tone specified is VP_PTABLE_NULL
                                         */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpProfilePtrType pToneProf = VP_PTABLE_NULL;
    VpProfilePtrType pCadProf = VP_PTABLE_NULL;
    VpStatusType status = VP_STATUS_SUCCESS;
    
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;

    uint8 sigGenCtrl, sigGenBCount;
    uint8 sigGenB[VP790_SIGB_PARAMS_LEN];

    VpDigitType digit = VP_DIG_NONE;
    VpDirectionType direction = VP_DIRECTION_INVALID;

    int toneIndex = GetProfileIndex(pToneProfile);
    int cadenceIndex = GetProfileIndex(pCadProfile);

    VpDeviceIdType deviceId = pDevObj->deviceId;
    
    /* 
     * Proceed if device state is either in progress or complete and not 
     * calibrating
     */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /* 
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */ 

    /* Verify a good profile (index or pointer) for the tone */
    if (toneIndex < 0) {
        /* 
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pToneProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONE, pToneProfile) != TRUE) {
                status = VP_STATUS_ERR_PROFILE;
            }
        }
        pToneProf = pToneProfile;
    } else if (toneIndex < VP_CSLAC_TONE_PROF_TABLE_SIZE) {
        pToneProf = pDevObj->devProfileTable.pToneProfileTable[toneIndex];
        if (!(pDevObj->profEntry.toneProfEntry & (0x0001 << toneIndex))) {
            /* The profile is invalid -- error. */
            status = VP_STATUS_ERR_PROFILE;
        }
    } else {
        status = VP_STATUS_ERR_PROFILE;
    }
    
    /* Verify a good profile (index or pointer) for the cadence */
    if (cadenceIndex < 0) {
        /* 
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pCadProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONECAD, pCadProfile) != TRUE) {
                status = VP_STATUS_ERR_PROFILE;
            }
        }
        pCadProf = pCadProfile;
    } else if (cadenceIndex < VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
        pCadProf = pDevObj->devProfileTable.pToneCadProfileTable[cadenceIndex];
        if (!(pDevObj->profEntry.toneCadProfEntry & (0x0001 << cadenceIndex))) {
            /* The profile is invalid -- error. */
            status = VP_STATUS_ERR_PROFILE;
        }
    } else {
        status = VP_STATUS_ERR_PROFILE;
    }

    if (status != VP_STATUS_SUCCESS) {
        return status;
    }

    /* Check to see if generating a DTMF tone */
    if (pDtmfControl != VP_NULL) {
        digit = pDtmfControl->toneId;
        if (VpIsDigit(digit) == FALSE) {
            return VP_STATUS_INVALID_ARG;
        }
        
        direction = pDtmfControl->dir;
        if (direction != VP_DIRECTION_DS) {
            return VP_STATUS_INVALID_ARG;
        }
    }

    /* All values are correct. Now to implement */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Disable signal generator B before make any changes */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_RD, VP790_GEN_CTRL_LEN,
        &sigGenCtrl);
    sigGenCtrl &= ~VP790_GENB_EN;
    VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_WRT, VP790_GEN_CTRL_LEN,
        &sigGenCtrl);

    if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
        pLineObj->cadence.pActiveCadence = pCadProf;    
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;

        /* We're no longer in the middle of a time function */
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
        pLineObj->cadence.timeRemain = 0;        
    }

    /* 
     * If tone profile is NULL, and either the pDtmfControl is NULL or it's
     * "digit" member is "Digit None", then shutoff tones (already done) and 
     * stop cadencing. We're done, so return.
     */
    if (((pToneProf == VP_PTABLE_NULL) && (pDtmfControl == VP_NULL))
     || ((pToneProf == VP_PTABLE_NULL) && (digit == VP_DIG_NONE))) {
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        }

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * If we're here, we're sending some tone.  If it's DTMF, we can stop the
     * active cadencer, set the time to "always on" (since the application will
     * tell us when to start/stop).
     *
     * If "direction" is some value other than the initialized value, then
     * the dtmf structure is passed and not NULL
     */

    if (direction != VP_DIRECTION_INVALID) {
        /* Disable currently active cadence */
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        
        /* Send the digit */
        Vp790SendDigit(pLineCtx, VP_DIGIT_GENERATION_DTMF, digit);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* 
     * If we're here, it's not DTMF so use the tone profile passed. Send the 
     * signal generator parameters to the device and wait for 1 frame sync 
     * period to re-enable the generator.
     */
    for (sigGenBCount = 0; 
         sigGenBCount < VP790_SIGB_PARAMS_LEN; 
         sigGenBCount++) {
        
        sigGenB[sigGenBCount] = 
            pToneProf[VP_PROFILE_TYPE_SEQUENCER_START+sigGenBCount];
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP790_SIGB_PARAMS_WRT, 
        VP790_SIGB_PARAMS_LEN, sigGenB);
    VpSysWait(1);

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.status &= ~VP_CADENCE_STATUS_BRANCHING;

    VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_RD, VP790_GEN_CTRL_LEN, 
        &sigGenCtrl);

    if(pCadProf == VP_PTABLE_NULL) {
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        }
        sigGenCtrl |= VP790_GENB_EN;
        VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_WRT, 
            VP790_GEN_CTRL_LEN, &sigGenCtrl);
    } else {
        pLineObj->cadence.pCurrentPos = 
            &(pCadProf[VP_PROFILE_TYPE_SEQUENCER_START]);
    
        pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_BRANCHING;
        pLineObj->cadence.length = pCadProf[VP_PROFILE_LENGTH];
        pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790SetRelayState()
 *  Configures the VP790-controlled relays.
 *
 * Refer to the VP API User's Guide for details about this function.
 */
VpStatusType
Vp790SetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;

    uint8 ioReg[VP790_IO_REG_LEN];
    uint8 fxs282Mask = 0x0F;
    uint8 fxsGenericMask = 0x0E;
    uint8 fxsToTlMask = 0x0E;

    uint8 fxs282Map[] = {
        /* Normal          */   0x01,
        /* Reset           */   0x00,   /* OFF = 0, all others - don't cares */
        /* Testout         */   0x02,
        /* Talk            */   0x01,
        /* Ringing         */   0x05,
        /* Test            */   0x09,
        /* Bridged Test    */   0x0D,
        /* Split Test      */   0x0B,
        /* Disconnect      */   0x03,
        /* Ringing No-load */   0x07,
        /* Ringing Test    */   0x0F
    };

    uint8 fxsToTlMap[] = {
        /* Normal          */   0x00,
        /* Reset           */   0x00,   /* Not supported */
        /* Testout         */   0x02,
        /* Talk            */   0x00,   /* Not supported */
        /* Ringing         */   0x01,   /* Not supported */
        /* Test            */   0x00,   /* Not supported */
        /* Bridged Test    */   0x04,
        /* Split Test      */   0x06,
        /* Disconnect      */   0x00,   /* Not supported */
        /* Ringing No-load */   0x01,   /* Not supported */
        /* Ringing Test    */   0x01    /* Not supported */
    };

    /* Basic error checking */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP790_IO_REG_RD, 
        VP790_IO_REG_LEN, ioReg);
    
    if(pLineObj->termType == VP_TERM_FXS_75282) {
        if (rState <= VP_RELAY_RINGING_TEST) {
            ioReg[0] &= ~fxs282Mask;
            ioReg[0] |= (fxs282Mask & fxs282Map[rState]);
        } else {
            status = VP_STATUS_INVALID_ARG;
        }
    } else if(pLineObj->termType == VP_TERM_FXS_GENERIC) {
        ioReg[0] &= ~fxsGenericMask;
    
        if (rState == VP_RELAY_NORMAL) {
            /* 0 for control bits is correct, no more work necessary */
        } else if (rState == VP_RELAY_BRIDGED_TEST) {
            ioReg[0] |= (fxsGenericMask & 0x04);
        } else {
            status = VP_STATUS_INVALID_ARG;
        }
    } else if (pLineObj->termType == VP_TERM_FXS_TO_TL) {
        ioReg[0] &= ~fxsToTlMask;
    
        switch (rState) {
            case VP_RELAY_NORMAL:
            case VP_RELAY_TESTOUT:
            case VP_RELAY_BRIDGED_TEST:
            case VP_RELAY_SPLIT_TEST:
                ioReg[0] |= (fxsToTlMask & fxsToTlMap[rState]);
                break;
            
            default:
                status = VP_STATUS_INVALID_ARG;
                break;
        }
    } else {
        status = VP_STATUS_INVALID_ARG;
    }
    
    if (status != VP_STATUS_SUCCESS) {
        return status;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP790_IO_REG_WRT, 
        VP790_IO_REG_LEN, ioReg);

    return VP_STATUS_SUCCESS;
} /* Vp790SetRelayState() */

/**
 * Vp790SendCid()
 *  This function may be used to send Caller ID information on-demand. It 
 * accepts an amount of CID message data up to a "full" buffer amount (2 times 
 * the amount of the size used for ContinueCID). It low fills the primary buffer 
 * such that the application is interrupted at the earliest time when the API 
 * is ready to accept more data.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. The length of the 
 * message (indicated by the length field passed) must not exceed the buffer
 * size.
 *
 * Postconditions:
 * Caller ID information is transmitted on the line.
 */
VpStatusType
Vp790SendCid(
    VpLineCtxType *pLineCtx, 
    uint8 length, 
    VpProfilePtrType pCidProfile, 
    uint8p pCidData)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VpProfilePtrType *pProfileTable;
    uint8 primaryByteCount, secondaryByteCount, remainder;

    int cidIndex = GetProfileIndex(pCidProfile);

    /* 
     * Proceed if device state is either in progress or complete and not 
     * calibrating
     */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (length > (2 * VP_SIZEOF_CID_MSG_BUFFER)) {
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->cidEncodeSize = 5;

    /* Determine if it's an index or profile */
    if ((cidIndex >= 0) && (cidIndex < VP_CSLAC_CALLERID_PROF_TABLE_SIZE)) {
        /* It's an index. Set the line profile to the device indexed profile */
        pProfileTable = pDevObj->devProfileTable.pCallerIdProfileTable;
        pLineObj->pCidProfileType2 = pProfileTable[cidIndex];
    } else {
        if (cidIndex > VP_CSLAC_CALLERID_PROF_TABLE_SIZE) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG;
        } else {
            /* 
             * This is a pointer. Make sure it's the correct type, and if so,
             * set it to the line object
             */
            if(VpVerifyProfileType(VP_PROFILE_CID, pCidProfile) == TRUE) {
                pLineObj->pCidProfileType2 = pCidProfile;
            } else {
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_ERR_PROFILE;
            }
        }
    }

    if (pLineObj->pCidProfileType2 == VP_PTABLE_NULL) {
        pLineObj->callerId.status = 0;  /* Clear all status bits */
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_ERR_PROFILE;
    }
        
    pLineObj->callerId.pCliProfile = pLineObj->pCidProfileType2;
    pLineObj->callerId.cliIndex = 0;

    /*
     * Offset by the first 3 bytes because we're going to use the first
     * 3 bytes immediately to create our 5/3 byte conversion (required by
     * VE790 CID)
     */
    pLineObj->callerId.cliMPIndex = 3;  
    pLineObj->callerId.cliMSIndex = 0;    
    pLineObj->cidEncodeIndex = 0;

    pLineObj->callerId.status |= VP_CID_IN_PROGRESS;
    pLineObj->callerId.status &= ~VP_CID_SIG_B_VALID;
    pLineObj->callerId.status &= ~VP_CID_REPEAT_MSG;
    pLineObj->callerId.status &= ~VP_CID_END_OF_MSG;

    pLineObj->callerId.cliTimer = 1;

    /* 
     * Make sure the primary buffer will get at least 3 bytes, even if it has
     * only 3 bytes. This makes the initial indexing for buffer switching work
     */
     
    remainder = length - VP_SIZEOF_CID_MSG_BUFFER;
    if ((remainder < 3) && (length > VP_SIZEOF_CID_MSG_BUFFER)) {
        pLineObj->callerId.primaryMsgLen = 3;
        pLineObj->callerId.secondaryMsgLen = length - 3; 
    } else {
        if (length <= VP_SIZEOF_CID_MSG_BUFFER) {
            pLineObj->callerId.primaryMsgLen = length;
            pLineObj->callerId.secondaryMsgLen = 0;
        } else {
            pLineObj->callerId.primaryMsgLen = length - VP_SIZEOF_CID_MSG_BUFFER;
            pLineObj->callerId.secondaryMsgLen = VP_SIZEOF_CID_MSG_BUFFER;
        }
    }

    /* 
     * Copy the message data to the primary API buffer. If we're here, there's
     * at least 3 bytes of primary message data. So a check is not necessary
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

    pLineObj->callerId.status |= VP_CID_PRIMARY_IN_USE;
    pLineObj->callerId.status &= ~VP_CID_SECONDARY_IN_USE;
    
    /* Create the 5 byte buffer of caller ID data converted from user data */
    Vp790EncodeData(pLineCtx, pCidData);

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790ContinueCid()
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
Vp790ContinueCid(
    VpLineCtxType *pLineCtx,    /**< Line to continue CID on */
    uint8 length,               /**< Length of data passed not to exceed the 
                                 * buffer length
                                 */
    uint8p pCidData)            /**< CID message data */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 byteCount = 0;    
    uint8 *pMsgLen;
    uint8 *pBuffer;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    pLineObj->callerId.status &= ~VP_CID_END_OF_MSG;

    /* 
     * Proceed if device state is either in progress or complete and not 
     * calibrating
     */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (length >  VP_SIZEOF_CID_MSG_BUFFER) {
        return VP_STATUS_INVALID_ARG;
    }

    /* 
     * When this function is called, the buffer that is in use is flagged
     * by the VpCliGetEncodeByte() function in vp_api_common.c file. That
     * function implements the logic of when to switch between the primary
     * and secondary buffer. This function just needs to fill the bufffer that
     * is not currently in use, starting with the primary (because the primary
     * buffer is also used first for the first part of the message).
     */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (!(pLineObj->callerId.status & VP_CID_PRIMARY_IN_USE)) {
        /* Fill the primary buffer */
        pLineObj->callerId.status |= VP_CID_PRIMARY_FULL;
        pLineObj->callerId.cliMPIndex = 0;
        pMsgLen = &(pLineObj->callerId.primaryMsgLen);
        pBuffer = &(pLineObj->callerId.primaryBuffer[0]);
    } else {
        /* Fill the secondary buffer */
        pLineObj->callerId.status |= VP_CID_SECONDARY_FULL;
        pMsgLen = &(pLineObj->callerId.secondaryMsgLen);        
        pBuffer = &(pLineObj->callerId.secondaryBuffer[0]);
        pLineObj->callerId.cliMSIndex = 0;
    }

    /* 
     * If the length is larger than what can fit in the CID buffer, max out
     * the primary message length value. Otherwise, set the primary message
     * length value to the length
     */
    if (length >= VP_SIZEOF_CID_MSG_BUFFER) {
        *pMsgLen = VP_SIZEOF_CID_MSG_BUFFER;
    } else {
        *pMsgLen = length;
    }

    /* Copy the message data to the API buffer */
    for (byteCount = 0; (byteCount < *pMsgLen); byteCount++) {
        pBuffer[byteCount] = pCidData[byteCount];

        pLineObj->callerId.cidCheckSum += pBuffer[byteCount];
        pLineObj->callerId.cidCheckSum = pLineObj->callerId.cidCheckSum % 256;
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp790ContinueCid() */

/**
 * Vp790CtrlSetCliTone()
 *  This function is called by the API internally to enable or disabel the
 * signal generator used for Caller ID.
 *
 * Preconditions:
 *  The line context must be valid (pointing to a Vp790 line object type
 *
 * Postconditions:
 *  The signal generator used for CID tones is enabled/disabled indicated by
 * the mode parameter passed.
 */
VpStatusType
Vp790CtrlSetCliTone(
    VpLineCtxType *pLineCtx,    /**< Line affected by the CLI tones */
    bool mode)                  /**< TRUE = enabled, FALSE = disable tones */
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl;
    
    VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_RD, VP790_GEN_CTRL_LEN,
        &sigGenCtrl);
    if (mode == TRUE) {
        sigGenCtrl |= VP790_GENB_EN;
    } else {
        sigGenCtrl &= ~(VP790_GENB_EN);
    }
    VpMpiCmd(deviceId, ecVal[channelId], VP790_GEN_CTRL_WRT, VP790_GEN_CTRL_LEN,
        &sigGenCtrl);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790CtrlSetFSKGen()
 *  This function is called by the CID sequencer executed internally by the API
 *
 * Preconditions:
 *  The line context must be valid (pointing to a Vp790 line object type
 *
 * Postconditions:
 *  The data indicated by mode and data is applied to the line. Mode is used
 * to indicate whether the data is "message", or a special character. The 
 * special characters are "channel siezure" (alt. 1/0), "mark" (all 1), or
 * "end of transmission".
 */
void 
Vp790CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,    /**< Line affected by the mode and data */
    VpCidGeneratorControlType mode,      /**< Indicates the type of data being sent. 
                                 * Affects the start and stop bit used
                                 */
    uint8 data)                 /**< 8-bit message data */
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;
    uint8 termData = VP790_SIGGEN_FSK_OFF;
    
    switch(mode) {
        case VP_CID_SIGGEN_EOT:
            data = VP790_DISABLE_CID_WHEN_CMPL;
            break;

        case VP_CID_GENERATOR_DATA:
            if ((pLineObj->callerId.status & VP_CID_END_OF_MSG) ||
                (pLineObj->callerId.status & VP_CID_MID_CHECKSUM)) {
            } else {
            }
            break;

        case VP_CID_GENERATOR_KEYED_CHAR:
            switch(data) {
                case VP_FSK_CHAN_SEIZURE:
                    data = 0x95;
                    break;
                    
                case VP_FSK_MARK_SIGNAL:
                    data = 0xBF;
                    break;
            }
            break;

        default:
            break;
    }

    if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
        VpMpiCmd(deviceId, ecVal[channelId], VP790_CID_DATA_WRT, 
            VP790_CID_DATA_LEN, &data);
    } else {
        VpMpiCmd(deviceId, ecVal[channelId], VP790_CID_DATA_WRT, 
            VP790_CID_DATA_LEN, &termData);
    }

    return;
}

/**
 * Vp790SendDigit()
 *  This function sends a DTMF digit on the line. It creates a sequencer 
 * compatible profile to control the generators and time operators.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The digit specified is sent on the line in the form specified (DTMF only
 * supported).
 */
VpStatusType
Vp790SendDigit(
    VpLineCtxType *pLineCtx,            /**< Line to send a digit on */
    VpDigitGenerationType digitType,    /**< Type of digit to send. May indicate
                                         * DTMF, Dial Pulse, or Hook Flash
                                         */
    VpDigitType digit)                  /**< The digit to send. Used if type of
                                         * digit is DTMF or Dial Pulse
                                         */
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 seqByte;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    if (VpIsDigit(digit) == FALSE) {
        return VP_STATUS_INVALID_ARG;
    }

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
            break;
            
        default:
            return VP_STATUS_INVALID_ARG;
    }
    
    /* Parameters passed are good -- proceed */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous 
     * sequences first
     */
    pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;    /* No active status */

    for (seqByte = 0; seqByte < VP790_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
            Vp790SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

            /* Fixed total length and sequence length for DTMF generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0A;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x06;

            /* 
             * The generators should already have been off. We're programming
             * here, so the sequence will enforce the time to wait for turning
             * the generators back on. Then it will stay on "forever".
             */
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START+1] |= 0x01;
            
            /* After the delay, turn siggen B on */
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START+2]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_SIGGEN);

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START+3] = 
                VP_SEQ_SIGGEN_B_EN;
                
            /* The last time period is 0 == forever */
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START+4]
                = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START+5] |= 0x01;

            /* Tell the cadencer what type of sequencer is being run */
            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = 
                VP_PRFWZ_PROFILE_DTMF_DIG_GEN;
            break;

        default:
            /* 
             * This can only occur if there is an error in the error checking
             * above.
             */
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);            
            return VP_STATUS_INVALID_ARG;
    }
    
    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];
    
    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos = &pLineObj->intSequence[8];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);    
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790SetDTMFGenerators()
 *  This function sets signal generator B for DTMF tone generation.
 *
 * Preconditions:
 *  The line must first be initialized and signal generator B must be disabled.
 *
 * Postconditions:
 *  The signal generator B is set to the DTMF frequencies and level required
 * by the digit passed. Level and frequency controlled by VP790 device. A DTMF
 * tone exists on the line.
 */
VpStatusType
Vp790SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode, 
    VpDigitType digit)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 sigGenParams[VP790_SIGGEN_CONTROL_LEN] = {0x00};

    uint8 columnFreqs[] = {
        0x04,   /* 1209Hz (1, 4, 7, *) */
        0x05,   /* 1336Hz (2, 5, 8, 0) */
        0x06,   /* 1477Hz (3, 6, 9, #) */
        0x07    /* 1633Hz (A, B, C, D) */        
    };

    uint8 rowFreqs[] = {
        0x40,   /* 697Hz (1, 2, 3, A) */
        0x50,   /* 770Hz (4, 5, 6, B) */
        0x60,   /* 852Hz (7, 8, 9, C) */
        0x70    /* 941Hz (*, 0, #, D) */
    };

    /* Set the Column Freqs first */
    switch(digit) {
        case 1:
        case 4:
        case 7:
        case VP_DIG_ASTER:
            sigGenParams[0] |= columnFreqs[0];
            break;

        case 2:
        case 5:
        case 8:
        case VP_DIG_ZERO:
            sigGenParams[0] |= columnFreqs[1];
            break;

        case 3:
        case 6:
        case 9:
        case VP_DIG_POUND:
            sigGenParams[0] |= columnFreqs[2];
            break;

        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
            sigGenParams[0] |= columnFreqs[3];
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Now set the row freqs */
    switch(digit) {
        case 1:
        case 2:
        case 3:
        case VP_DIG_A:
            sigGenParams[0] |= rowFreqs[0];
            break;

        case 4:
        case 5:
        case 6:
        case VP_DIG_B:
            sigGenParams[0] |= rowFreqs[1];
            break;

        case 7:
        case 8:
        case 9:
        case VP_DIG_C:
            sigGenParams[0] |= rowFreqs[2];
            break;

        case VP_DIG_ASTER:
        case VP_DIG_ZERO:
        case VP_DIG_POUND:
        case VP_DIG_D:
            sigGenParams[0] |= rowFreqs[3];
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Load the signal generator B with the DTMF digit */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_SIGGEN_CONTROL_WRT, 
        VP790_SIGGEN_CONTROL_LEN, sigGenParams);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp790SetOption()
 *  This function determines how to process the Option based on pDevCtx, 
 * pLineCtx, and option type.  The actual options are implemented in 
 * Vp790SetOptionInternal
 *
 * Preconditions:
 *  The line must first be initialized if a line context is passed, or the 
 * device must be initialized if a device context is passed.
 *
 * Postconditions:
 *  The option specified is implemented either on the line, or on the device, or 
 * on all lines associated with the device (see the API Reference Guide for 
 * details).
 */
VpStatusType
Vp790SetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    uint8 channelId;
    Vp790DeviceObjectType *pDevObj;
    VpStatusType status = VP_STATUS_INVALID_ARG;
    VpLineCtxType *pLineCtxLocal;

    if (pDevCtx != VP_NULL) {
        pDevObj = pDevCtx->pDevObj;
        
        /* 
         * Proceed if device state is either in progress or complete and not 
         * calibrating
         */
        if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
            if (pDevObj->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEVICE_BUSY;
            }
        } else {
            return VP_STATUS_DEV_NOT_INITIALIZED;
        }

        /* Valid Device Context, we already know Line context is NULL (higher
         * layer SW, process on device if device option, or process on all lines
         * associated with device if line option
         */
        switch (option) {
            case VP_OPTION_ID_EVENT_MASK:  /* Line and Device */
                Vp790SetOptionInternal(VP_NULL, pDevCtx, option, value);

            /* Line Options */
            case VP_OPTION_ID_ZERO_CROSS:
            case VP_OPTION_ID_PULSE_MODE:
            case VP_OPTION_ID_TIMESLOT:
            case VP_OPTION_ID_CODEC:
            case VP_OPTION_ID_PCM_HWY:
            case VP_OPTION_ID_LOOPBACK:
            case VP_OPTION_ID_LINE_STATE:
            case VP_OPTION_ID_RING_CNTRL:
            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                /* 
                 * Loop through all of the valid channels associated with this 
                 * device. Init status variable in case there are currently no
                 * line contexts associated with this device
                 */
                status = VP_STATUS_SUCCESS; 
                for (channelId = 0; 
                     channelId < pDevObj->staticInfo.maxChannels; 
                     channelId++) {
                    pLineCtxLocal = pDevCtx->pLineCtx[channelId];
                    if (pLineCtxLocal != VP_NULL) {
                        status = Vp790SetOptionInternal(pLineCtxLocal, VP_NULL, 
                            option, value);

                        if (status != VP_STATUS_SUCCESS) {
                            return status;
                        }
                    }
                }
                break;

            default:
                /* 
                 * Device option, or option unknown option.  Handle in lower 
                 * layer
                 */
                status = 
                    Vp790SetOptionInternal(VP_NULL, pDevCtx, option, value);
                break;
        }
    } else {
        /* 
         * Line context must be valid, device context is NULL, proceed as normal
         */
        status = Vp790SetOptionInternal(pLineCtx, VP_NULL, option, value);
    }
    return status;
}

/**
 * Vp790SetOptionInternal()
 *  This function implements on the Vp790 device the options specified from 
 * Vp790SetOption().  No other function should call this function.
 *
 * Preconditions:
 *  See Vp790SetOption()
 *
 * Postconditions:
 *  See Vp790SetOption()
 */
VpStatusType
Vp790SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    VpDevCtxType *pDevCtxLocal;

    VpStatusType status = VP_STATUS_SUCCESS;
    
    Vp790LineObjectType *pLineObj;
    Vp790DeviceObjectType *pDevObj;
    VpDeviceIdType deviceId; 

    uint8 ccr1[VP790_CCR1_LEN];
    uint8 ccr1Byte;
    uint8 gioDir[VP790_GIO_DIR_LEN];
    VpOptionDeviceIoType deviceIo;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId, tempCcr6, tempCcr7, txSlot, rxSlot;
    uint8 tempLoopBack, tempSysConfig;
    uint16 eventMask;

    if (pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;

        /* 
         * Proceed if device state is either in progress or complete and not 
         * calibrating
         */
        if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
            if (pDevObj->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEVICE_BUSY;
            }
        } else {
            return VP_STATUS_DEV_NOT_INITIALIZED;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

        switch (option) {
            /* Line Options */
            case VP_OPTION_ID_PULSE_MODE:
                pLineObj->pulseMode = *((VpOptionPulseModeType *)value);
                VpInitDP(&pLineObj->dpStruct);
                break;

            case VP_OPTION_ID_TIMESLOT:
                txSlot = ((VpOptionTimeslotType *)value)->tx;
                rxSlot = ((VpOptionTimeslotType *)value)->rx;
                status = Vp790SetTimeSlot(pLineCtx, txSlot, rxSlot);
                break;

            case VP_OPTION_ID_CODEC:
                status = Vp790SetCodec(pLineCtx, *((VpOptionCodecType *)value));
                break;

            case VP_OPTION_ID_PCM_HWY:
                VpMpiCmd(deviceId, ecVal[channelId], VP790_TX_TS_RD, 
                    VP790_TX_TS_LEN, &txSlot);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_RX_TS_RD, 
                    VP790_RX_TS_LEN, &rxSlot);

                if ( *((VpOptionPcmHwyType *)value) == VP_OPTION_HWY_B ) {
                    txSlot |= VP790_TX_HWY_B;
                    rxSlot |= VP790_RX_HWY_B;
                } else {
                    txSlot &= ~VP790_TX_HWY_B;
                    rxSlot &= ~VP790_RX_HWY_B;
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP790_TX_TS_WRT, 
                    VP790_TX_TS_LEN, &txSlot);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_RX_TS_WRT, 
                    VP790_RX_TS_LEN, &rxSlot);
                break;

            case VP_OPTION_ID_LOOPBACK:
                VpMpiCmd(deviceId, ecVal[channelId], VP790_LOOPBACK_RD, 
                    VP790_LOOPBACK_LEN, &tempLoopBack);

                if (*((VpOptionLoopbackType *)value) == VP_OPTION_LB_TIMESLOT) {
                    tempLoopBack |= VP790_PCM_LOOPBACK;
                } else if (*((VpOptionLoopbackType *)value) == 
                    VP_OPTION_LB_DIGITAL) {
                    
                    tempLoopBack |= VP790_FULL_LOOPBACK;                    
                } else if ( *((VpOptionLoopbackType *)value) == 
                    VP_OPTION_LB_OFF ) {
                    
                    tempLoopBack &= ~VP790_PCM_LOOPBACK;
                    tempLoopBack &= ~VP790_FULL_LOOPBACK; 
                } else {
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
                }

                VpMpiCmd(deviceId, ecVal[channelId], VP790_LOOPBACK_WRT, 
                    VP790_LOOPBACK_LEN, &tempLoopBack);
                break;

            case VP_OPTION_ID_LINE_STATE:
                pLineObj->lineStateBatOption = 
                    *((VpOptionLineStateType *)value);
                Vp790SetLineStateInt(pLineCtx, pLineObj->lineState.current);    
                break;

            case VP_OPTION_ID_EVENT_MASK:
                /* Accept the masking options */
                pLineObj->lineEventsMask = *((VpOptionEventMaskType *)value);
                pDevObj->deviceEventsMask = *((VpOptionEventMaskType *)value);

                /* Unmask the unmaskable */
                VpImplementNonMaskEvents(&pLineObj->lineEventsMask, 
                    &pDevObj->deviceEventsMask);

                /* Mask the non-supported events */
                Vp790MaskNonSupportedEvents(&pLineObj->lineEventsMask, 
                    &pDevObj->deviceEventsMask);

                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_RD, 
                    VP790_CCR6_LEN, &tempCcr6);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR7_RD, 
                    VP790_CCR7_LEN, &tempCcr7);

                /* Evaluate for line faults events */
                eventMask = pLineObj->lineEventsMask.faults;
                if (eventMask & VP_LINE_EVID_THERM_FLT) {
                    tempCcr6 |= VP790_MASK_TEMPA_INTR;
                } else {
                    tempCcr6 &= ~VP790_MASK_TEMPA_INTR;
                }
                if (eventMask & VP_LINE_EVID_DC_FLT) {
                    tempCcr7 |= VP790_MASK_DCFLT;
                } else {
                    tempCcr7 &= ~VP790_MASK_DCFLT;
                }
                if (eventMask & VP_LINE_EVID_AC_FLT) {
                    tempCcr7 |= VP790_MASK_ACFLT;
                } else {
                    tempCcr7 &= ~VP790_MASK_ACFLT;
                }

                /* Evaluate for signaling events */
                eventMask = pLineObj->lineEventsMask.signaling;
                if ((eventMask & VP_LINE_EVID_HOOK_OFF)
                 && (eventMask & VP_LINE_EVID_HOOK_ON)
                 && (eventMask & VP_LINE_EVID_FLASH)
                 && (eventMask & VP_LINE_EVID_STARTPULSE)
                 && (eventMask & VP_LINE_EVID_PULSE_DIG)) {
                    tempCcr6 |= VP790_MASK_HOOK_INTR;
                } else {
                    tempCcr6 &= ~VP790_MASK_HOOK_INTR;
                }
                if ((eventMask & VP_LINE_EVID_GKEY_DET) 
                 && (eventMask & VP_LINE_EVID_GKEY_REL)) {
                    tempCcr6 |= VP790_MASK_GNK_INTR;
                } else {
                    tempCcr6 &= ~VP790_MASK_GNK_INTR;
                }
                /* 
                 * Remaining possibilities are either implemented entirely in 
                 * software, or not supported
                 */

                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR6_WRT, 
                    VP790_CCR6_LEN, &tempCcr6);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR7_WRT, 
                    VP790_CCR7_LEN, &tempCcr7);
                break;

            case VP_OPTION_ID_ZERO_CROSS:                
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR4_RD, 
                    VP790_CCR4_LEN, &tempSysConfig);
                if (*(VpOptionZeroCrossType *)value == VP_OPTION_ZC_NONE) {
                    if (tempSysConfig & VP790_EXT_RINGING) {
                        tempSysConfig |= VP790_CCR4_ZXR_DIS;
                    } else {
                        tempSysConfig &= ~VP790_CCR4_ZXR_DIS;
                    }
                } else {
                    if (tempSysConfig & VP790_EXT_RINGING) {
                        tempSysConfig &= ~VP790_CCR4_ZXR_DIS;
                    } else {
                        tempSysConfig |= VP790_CCR4_ZXR_DIS;
                    }
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR4_WRT, 
                    VP790_CCR4_LEN, &tempSysConfig);
                
                pLineObj->zeroCross = *((VpOptionZeroCrossType *)value);                    
                break;
                
            case VP_OPTION_ID_RING_CNTRL:
                pLineObj->ringCtrl = *((VpOptionRingControlType *)value);
            
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR4_RD, 
                    VP790_CCR4_LEN, &tempSysConfig);

                if (pLineObj->ringCtrl.zeroCross == VP_OPTION_ZC_NONE) {
                    if (tempSysConfig & VP790_EXT_RINGING) {
                        tempSysConfig |= VP790_CCR4_ZXR_DIS;
                    } else {
                        tempSysConfig &= ~VP790_CCR4_ZXR_DIS;
                    }
                } else {
                    if (tempSysConfig & VP790_EXT_RINGING) {
                        tempSysConfig &= ~VP790_CCR4_ZXR_DIS;
                    } else {
                        tempSysConfig |= VP790_CCR4_ZXR_DIS;
                    }
                }

                if ((pLineObj->ringCtrl.ringTripExitSt
                    == VP_LINE_RINGING)
                 || (pLineObj->ringCtrl.ringTripExitSt 
                    == VP_LINE_RINGING_POLREV)) {
                    tempSysConfig |= VP790_CCR4_ARR_DIS;
                } else {
                    tempSysConfig &= ~VP790_CCR4_ARR_DIS;
                    /*
                     * Ring Trip Exit state implemented in API-II since device
                     * is not aware of TX/RX PCM mode -- which also has to be
                     * set on Ring Trip.
                     */
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR4_WRT, 
                    VP790_CCR4_LEN, &tempSysConfig);
                break;

            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                pLineObj->pcmTxRxCtrl = *((VpOptionPcmTxRxCntrlType *)value);
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_RD, 
                    VP790_CCR1_LEN, ccr1);
                ccr1[0] &= ~(VP790_CUT_TXPATH | VP790_CUT_RXPATH);
                Vp790GetTxRxPcmMode(pLineObj, pLineObj->lineState.current, 
                    &ccr1Byte);
                ccr1[0] |= ccr1Byte;                    
                VpMpiCmd(deviceId, ecVal[channelId], VP790_CCR1_WRT, 
                    VP790_CCR1_LEN, ccr1);
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    } else {
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;

        /* 
         * Proceed if device state is either in progress or complete and not 
         * calibrating
         */
        if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
            if (pDevObj->status.state & VP_DEV_IN_CAL) {
                return VP_STATUS_DEVICE_BUSY;
            }
        } else {
            return VP_STATUS_DEV_NOT_INITIALIZED;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
        
        switch (option) {
            case VP_DEVICE_OPTION_ID_PULSE:
                pDevObj->pulseSpecs = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_PULSE2:
                pDevObj->pulseSpecs2 = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                pDevObj->criticalFault = *((VpOptionCriticalFltType *)value);
                break;

            case VP_DEVICE_OPTION_ID_DEVICE_IO:
                deviceIo = *(VpOptionDeviceIoType *)(value);
                
                /* 
                 * If any pin is being configured as output-open collector,
                 * return error because the device does not support it.
                 */
                if ((deviceIo.directionPins_31_0 
                   & deviceIo.outputTypePins_31_0) 
                 || (deviceIo.directionPins_63_32 
                   & deviceIo.outputTypePins_63_32)) {
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
                }
                gioDir[0] = (deviceIo.directionPins_31_0 & VP790_GIO_DIR_MASK);
                VpMpiCmd(deviceId, ecVal[0], VP790_GIO_DIR_WRT, 
                    VP790_GIO_DIR_LEN, gioDir);
                break;
                
            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp790MaskNonSupportedEvents()
 *  This function masks the events that are not supported by the VP790 API-II. 
 * It should only be called by SetOptionInternal when event masks are being
 * modified.
 *
 * Preconditions:
 *  None. Utility function to modify event structures only.
 *
 * Postconditions:
 *  Event structures passed are modified with masked bits for non-supported
 * VP790 API-II events.
 */
void
Vp790MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask, /**< Line Events Mask to modify for
                                             * non-masking
                                             */
    VpOptionEventMaskType *pDevEventsMask)  /**< Device Events Mask to modify
                                             * for non-masking
                                             */
{
    pLineEventsMask->faults |= VP790_NONSUPPORT_FAULT_EVENTS;
    pLineEventsMask->signaling |= VP790_NONSUPPORT_SIGNALING_EVENTS;
    pLineEventsMask->response |= VP790_NONSUPPORT_RESPONSE_EVENTS;
    pLineEventsMask->test |= VP790_NONSUPPORT_TEST_EVENTS;
    pLineEventsMask->process |= VP790_NONSUPPORT_PROCESS_EVENTS;
    pLineEventsMask->fxo |= VP790_NONSUPPORT_FXO_EVENTS;
    pLineEventsMask->packet |= VP790_NONSUPPORT_PACKET_EVENTS;

    pDevEventsMask->faults |= VP790_NONSUPPORT_FAULT_EVENTS;
    pDevEventsMask->signaling |= VP790_NONSUPPORT_SIGNALING_EVENTS;
    pDevEventsMask->response |= VP790_NONSUPPORT_RESPONSE_EVENTS;
    pDevEventsMask->test |= VP790_NONSUPPORT_TEST_EVENTS;
    pDevEventsMask->process |= VP790_NONSUPPORT_PROCESS_EVENTS;
    pDevEventsMask->fxo |= VP790_NONSUPPORT_FXO_EVENTS;
    pDevEventsMask->fxo |= VP790_NONSUPPORT_FXO_EVENTS;
    pDevEventsMask->packet |= VP790_NONSUPPORT_PACKET_EVENTS;

    return;
}

/**
 * Vp790DeviceIoAccess()
 *  This function is used to access device IO pins of the Vp790. See API-II
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
Vp790DeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData)
{
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    
    uint8 gioData[VP790_GIO_DATA_LEN];

    /* 
     * Proceed if device state is either in progress or complete and not 
     * calibrating
     */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
        if (pDevObj->status.state & VP_DEV_IN_CAL) {
            return VP_STATUS_DEVICE_BUSY;
        }
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    
    /* Read the current state of the IO lines */
    VpMpiCmd(deviceId, VP790_EC_CH1, VP790_GIO_DATA_RD,
        VP790_GIO_DATA_LEN, gioData);

    /* 
     * If this is a write operation, detect the bits that should be changed
     * and modify the appropriate io data bits
     */
    if (pDeviceIoData->accessType == VP_DEVICE_IO_WRITE) {
        /* For masked bits, set to 0 first then to 1. */
        gioData[0] &= ~(pDeviceIoData->accessMask_31_0 & VP790_GIO_DATA_MASK);
        gioData[0] |= (pDeviceIoData->accessMask_31_0 
                     & pDeviceIoData->deviceIOData_31_0
                     & VP790_GIO_DATA_MASK);

        /* Write back to the register */
        VpMpiCmd(deviceId, VP790_EC_CH1, VP790_GIO_DATA_WRT,
            VP790_GIO_DATA_LEN, gioData);
    } else if (pDeviceIoData->accessType == VP_DEVICE_IO_READ) {
        /* Clear all bits */
        pDeviceIoData->deviceIOData_31_0 = 0;
        pDeviceIoData->deviceIOData_63_32 = 0;
        
        pDeviceIoData->deviceIOData_31_0 |= 
            (pDeviceIoData->accessMask_31_0 & gioData[0]);
    }

    pDevObj->deviceEvents.response |= VP_DEV_EVID_IO_ACCESS_CMP;
        
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);    
    return VP_STATUS_SUCCESS;
}

/**
 * Vp790SetCodec()
 *  This function sets the codec mode on the line specified.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The codec mode on the line is set.  This function returns the success code 
 * if the codec mode specified is supported.
 */
VpStatusType
Vp790SetCodec(
    VpLineCtxType *pLineCtx,
    VpOptionCodecType codec)    /* Encoding, as defined by LineCodec typedef */
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 codecReg;
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;

    /* Basic error checking */
    if ((codec != VP_OPTION_LINEAR) && (codec != VP_OPTION_ALAW)
     && (codec != VP_OPTION_MLAW)) {
        return VP_STATUS_INVALID_ARG;
    }

    /* 
     * If the code mode is already correct, don't change it -- it's a waste of
     * the MPI resource
     */

    if (codec == pLineObj->codec) {
        return VP_STATUS_SUCCESS;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Read the current state of the codec register */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_CODEC_REG_RD, 
        VP790_CODEC_REG_LEN, &codecReg);

    /* Determine the what bits need to be manipulated */
    switch (codec) {
        case VP_OPTION_LINEAR:              /*16 bit linear PCM */
            codecReg |= VP790_LINEAR_CODEC;
            break;

        case VP_OPTION_ALAW:                /* A-law PCM */
            codecReg &= ~(VP790_ULAW_CODEC | VP790_LINEAR_CODEC);
            break;

        case VP_OPTION_MLAW:                /* u-law PCM */
            codecReg |= VP790_ULAW_CODEC;
            codecReg &= ~VP790_LINEAR_CODEC;
            break;

        default:                       /* Did we pass an invalid range */
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG;
    }

    /*
     * Write the new bit field to the codec register and update the slacInfo
     * structure.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_CODEC_REG_WRT, 
        VP790_CODEC_REG_LEN, &codecReg);

    pLineObj->codec = codec;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;

} /* End Vp790SetCodec */

/**
 * Vp790SetTimeSlot()
 *  This function set the RX and TX timeslot for a device channel. Valid 
 * timeslot numbers start at zero. The upper bound is system dependent.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The timeslots on the line are set.  This function returns the success code 
 * if the timeslot numbers specified are within the range of the device based on 
 * the PCLK rate.
 */
VpStatusType
Vp790SetTimeSlot(
    VpLineCtxType *pLineCtx,
    uint8 txSlot,       /* The TX PCM timeslot */
    uint8 rxSlot)       /* The RX PCM timeslot */
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;

    /* Validate the tx time slot value */
    if((txSlot & ~VP790_PCMHWY_SEL) >= pDevObj->devProfileData.pcmClkRate/64) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Validate the rx time slot value */
    if((rxSlot & ~VP790_PCMHWY_SEL) >= pDevObj->devProfileData.pcmClkRate/64) { 
        return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    VpMpiCmd(deviceId, ecVal[channelId], VP790_TX_TS_WRT, VP790_TX_TS_LEN, 
        &txSlot);/* Write the TX Time Slot */
    VpMpiCmd(deviceId, ecVal[channelId], VP790_RX_TS_WRT, VP790_RX_TS_LEN, 
        &rxSlot);/* Write the RX Time Slot */
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;

} /* End Vp790SetTimeSlot */

/**
 * Vp790VirtualISR()
 *  This function is called everytime the device causes an interrupt
 * 
 * Preconditions
 *  A device interrupt has just occured
 *
 * Postcondition
 *  This function should be called from the each device's ISR.
 *  This function could be inlined to improve ISR performance.
 */
VpStatusType
Vp790VirtualISR(
    VpDevCtxType *pDevCtx)
{
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

#if defined (VP790_INTERRUPT_LEVTRIG_MODE)
    VpSysDisableInt(deviceId);
#endif
    /* Device Interrupt Received */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevObj->status.state |= VP_DEV_PENDING_INT;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
} /* Vp790VirtualISR */

/**
 * Vp790ApiTick()
 *  This function should be called on a periodic basis or attached to an 
 * interrupt.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  The value passed (by pointer) is set to TRUE if there is an updated event.  
 * The user should call the GetEventStatus function to determine the cause of 
 * the event (TRUE value set).  This function always returns the success code.
 */
VpStatusType
Vp790ApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus)
{
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp790LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 intReg;  /* Place holder for the interrupt register (3f) */
    VpLineCtxType *pLineCtx;

    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};

    uint16 timeStampPre, tickAdder;

    *pEventStatus = FALSE; 

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* 
     * The timestamp is in 0.5mS increments, but the device tickrate is 
     * something else. So increment by the scaled amount and detect rollover
     * by finding if the previous value is greater than the new value.
     */
    timeStampPre = pDevObj->timeStamp;
    tickAdder = pDevObj->devProfileData.tickRate / VP_CSLAC_TICKSTEP_0_5MS;
    pDevObj->timeStamp+=tickAdder;

    if (timeStampPre > pDevObj->timeStamp) {
        pDevObj->deviceEvents.signaling |= VP_DEV_EVID_TS_ROLLOVER;
    }

#if defined (VP790_INTERRUPT_LEVTRIG_MODE)
    VpSysEnableInt(deviceId);
#endif

    /* Check to see if the device is calibrating if so return no events */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        /* Continue Calibration, and return TRUE if current event */
        Vp790ContinueCalibrate(pDevCtx);

        if (Vp790FindSoftwareInterrupts(pDevCtx)) {
            *pEventStatus = TRUE;
        }
        
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEVICE_BUSY;
    }
    
    /*
     * If we are past calibration, we should also be past device init. But,
     * we may have had an initialization error that should prevent further
     * Api activity.
     */
    if (!(pDevObj->status.state & VP_DEV_INIT_CMP)) {
        /* It is still possible to have low level read command events */
        if (Vp790FindSoftwareInterrupts(pDevCtx)) {
            *pEventStatus = TRUE;
        }
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /* Reset event pointers pointers */
    pDevObj->dynamicInfo.lastChan = 0;

    /* Service API Timers */
    Vp790ServiceTimers(pDevCtx);
    VpServiceSeq(pDevCtx);

    /* Update the dial pulse handler for lines that are set for pulse decode */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_ON) {
                if(VpUpdateDP(pDevObj->devProfileData.tickRate,
                    &pDevObj->pulseSpecs, &pLineObj->dpStruct,
                    &pLineObj->lineEvents) == TRUE) {
                    pLineObj->signalingData = pLineObj->dpStruct.signalingData;
                }

                if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
                    VpCidSeq(pLineCtx);
                }
            }
        }
    }

    /*
     * Test the interrupt to see if there is a pending interrupt.  If there is,
     * read the interrupt registers (if running in an interrupt driven mode).
     * If running in polled mode, automatically read the interrupt/status 
     * registers.
     */

#if defined (VP790_EFFICIENT_POLLED_MODE)
    /* Poll the device PIO-INT line */
    pDevObj->status.state |= 
        (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);  
#elif defined (VP790_SIMPLE_POLLED_MODE)
    pDevObj->status.state |= VP_DEV_PENDING_INT;
#endif

    /* Limit the number of interrupts serviced during one tick */
    pDevObj->status.numIntServiced = pDevObj->devProfileData.maxNumInterrupts;

    /* Service all pending interrupts (up to maxNumInterrupts) */
    while ((pDevObj->status.state & VP_DEV_PENDING_INT) 
        && (pDevObj->status.numIntServiced > 0)) {
        /* Clear the current interrupt indication */
        pDevObj->status.state &= ~VP_DEV_PENDING_INT;
        pDevObj->status.numIntServiced--;

        /* 
         * The following Interrupts servicing procedure follows that described
         * in the VE790 Chipset User's Guide 
         */
        /* Read the Interrupt Status Register */
        VpMpiCmd(deviceId, ecVal[0], VP790_INTRPT_RD, VP790_INTRPT_LEN, &intReg);

        /* HANDLE GLOBAL (TYPE I) INTERRUPTS */
        if ((intReg & VP790_INTR_GLOB) && (intReg & VP790_INTR_INT)) {
            /* Service the Device interrupt */
            Vp790ServiceType1Int(pDevCtx);
        } else if (intReg) {
            /* Servicing a channel specific interrupt, so decode the channel */
            channelId = (uint8)((intReg & VP790_GLOB_CHAN_MASK) >> 4);
 
            /* HANDLE SUPERVISION (TYPE II) INTERRUPTS */
            if (!(intReg & VP790_INTR_SIG)) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx != VP_NULL) {
                    Vp790ServiceType2Int(pLineCtx, intReg);
                }
            } else if (intReg & VP790_INTR_SIG) {
                /* HANDLE SIGNALING (TYPE III) INTERRUPTS */            
                pLineCtx = pDevCtx->pLineCtx[channelId];
                if (pLineCtx != VP_NULL) {
                    Vp790ServiceType3Int(pLineCtx);
                }
            }
        } else {
            /* There are no active interrupts - do nothing */
        }

        /* Do we need to go through the interrupt loop again? 
         *
         * If using simple poll, an active interrupt may be pending if there
         * was just at least one interrupt serviced (device will queue the
         * interrupts)
         *
         * If using efficient polled, test the interrupt line itself from the
         * user defined function.
         *
         * If level triggered, the interrupt may have been disabled (to prevent
         * a flood of interrupts), so reenable it.
         */
    #if defined (VP790_SIMPLE_POLLED_MODE)
        if ((intReg & VP790_INTR_INT) == 0) {
            /* 
             * If there isn't an active interrupt break from while interrupts 
             * loop else there is an active interrupt
             */
            break;
        } else {
            /*  Force another loop iteration     */
            pDevObj->status.state |= VP_DEV_PENDING_INT;
        }
    #elif defined (VP790_INTERRUPT_LEVTRIG_MODE)
        VpSysEnableInt(deviceId);
    #elif defined (VP790_EFFICIENT_POLLED_MODE)
        pDevObj->status.state |= 
            (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);      
    #endif
    }/* End while Interrupts*/

#if defined (VP790_INTERRUPT_LEVTRIG_MODE)
    VpSysEnableInt(deviceId);
#endif

    /* Collect all event activity and report to the calling function */
    if (Vp790FindSoftwareInterrupts(pDevCtx)) {
        *pEventStatus = TRUE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* End Vp790ApiTick */

/**
 * Vp790CalCodec()
 *  This function starts the internal calibration routine of a device. The 
 *  internal calibrate routine calibrates the analog amplifiers of the selected 
 *  device.
 *
 * Preconditions:
 *  The device must first be initialized prior to calibrating.
 *
 * Postconditions:
 *  Returns VP_STATUS_SUCCESS.
 *  
 *  After this function has returned, the device will be calibrating, and all
 * lines will be in the TIP_OPEN state. No VoicePath API calls can be performed
 * while the device is calibrating (functions will return a busy code).  
 * After the calibration has completed (10ms), all line states are returned to 
 * their previous state, and an API event (calibration complete) is issued to 
 * indicate the completion.
 *
 * WARNING!!
 *  This function places all channels of the device into TIP_OPEN! Any active 
 *  calls may be permanently destroyed by this action (depending on the 
 *  application's call state machine).
 */
VpStatusType
Vp790CalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode)
{
    uint8 channelId;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    
    VpLineCtxType *pLineCtxLocal;
    Vp790LineObjectType *pLineObj;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    VpStatusType status = VP_STATUS_SUCCESS;

    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }
    
    if (mode == VP_DEV_CAL_NOW) {
        return Vp790CalCodecInt(pDevCtx);
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    for(channelId = 0; channelId < maxChan; channelId++) {
        pLineCtxLocal = pDevCtx->pLineCtx[channelId];
        if (pLineCtxLocal != VP_NULL) {
            pLineObj = pLineCtxLocal->pLineObj;
            if (!(pLineObj->lineState.condition & VP_CSLAC_CAL_ENABLE)) {
                pDevObj->deviceEvents.response |= VP_EVID_CAL_BUSY;
                pDevObj->deviceEvents.response &= ~VP_EVID_CAL_CMP;
                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_SUCCESS;
            }
        }
    }

    status = Vp790CalCodecInt(pDevCtx);
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* End Vp790CalCodec */

/**
 * Vp790CalCodecInt()
 *  This function puts all lines into Tip Open and starts the device 
 * calibration.
 *
 * Preconditions:
 *  The device must first be initialized prior to calibrating.
 *
 * Postconditions:
 *  Returns VP_STATUS_SUCCESS.
 *  
 *  After this function has returned, the device will be calibrating, and all
 * lines will be in the TIP_OPEN state. No VoicePath API calls can be performed
 * while the device is calibrating (functions will return a busy code).  
 * After the calibration has completed (10ms), all line states are returned to 
 * their previous state, and an API event (calibration complete) is issued to 
 * indicate the completion.
 *
 * WARNING!!
 *  This function places all channels of the device into TIP_OPEN! Any active 
 *  calls may be permanently destroyed by this action (depending on the 
 *  application's call state machine).
 */
VpStatusType
Vp790CalCodecInt(
    VpDevCtxType *pDevCtx)
{
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId, data;    
    uint8 userByte[VP790_SLIC_STATE_LEN];
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

    /* 
     * We need to make sure that all channels on the device are set to Tip Open,
     * even if they are not assigned line contexts.
     */
    for(channelId = 0; channelId < maxChan; channelId++) {
        VpMpiCmd(deviceId, ecVal[channelId], VP790_EC_WRT, VP790_EC_LEN,
            &ecVal[channelId]);

        /* Put Chipset in the TIP_OPEN state */
        data = VP790_SLIC_ST_TIPOPEN;
        VpMpiCmd(deviceId, ecVal[channelId], VP790_SLIC_STATE_WRT, 
            VP790_SLIC_STATE_LEN, &data);
    }

   /* Start the API calibration timer */
    pDevObj->status.calibrating = pDevObj->devProfileData.calibrationTime;
    pDevObj->status.state |= VP_DEV_IN_CAL;

    /* Issue the Calibration MPI Command */
    VpMpiCmd(deviceId, ecVal[0], VP790_CALIBRATE_CMD, NO_DATA, userByte);
    return VP_STATUS_SUCCESS;    
}

/**
 * Vp790LowLevelCmd()
 *  This function provides direct MPI access to the line/device.
 *
 * Preconditions:
 *  The device associated with the line, and the line must first be initialized.
 *
 * Postconditions:
 *  The command data is passed over the MPI bus and affects only the line passed 
 * if the command is line specific, and an event is generated.  If a read 
 * command is performed, the user must read the results or flush events.  This 
 * function returns the success code if the device is not already in a state 
 * where the results must be read.
 */
VpStatusType
Vp790LowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp790DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    int i;

    uint8 ecVal[] = {VP790_EC_CH1, VP790_EC_CH2, VP790_EC_CH3, VP790_EC_CH4};
    uint8 channelId = pLineObj->channelId;

    if (pDevObj->deviceEvents.response & VP790_READ_RESPONSE_MASK) {
        return VP_STATUS_DEVICE_BUSY;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    if(pCmdData[0] & 0x01) { /* Read Command */
        VpMpiCmd(deviceId, ecVal[channelId], pCmdData[0], len, 
            &(pDevObj->mpiData[0]));
        pDevObj->mpiLen = len;    
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_RX_CMP;
    } else {
        VpMpiCmd(deviceId, ecVal[channelId], pCmdData[0], len, &pCmdData[1]);
        for (i = 0; i < len; i++) {
            pDevObj->mpiData[i] = pCmdData[i];
        }
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_TX_CMP;
    }
    pLineObj->lineEventHandle = handle;    

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp790MirrorByte()
 *  This function inverts the lower nibble of the byte passed. It is used as
 * part of the 5/3 byte CID message conversion for the VE790 device.
 *
 * Preconditions:
 *  None. Byte inversion only.
 *
 * Postconditions:
 *  None. Returns the inversion of the byte passed.
 */
uint8
Vp790MirrorByte(
    char byte)
{
    char luTable[] = {
        0x00, 0x08, 0x04, 0x0C, 0x02, 0x0A, 0x06, 0x0E, 0x01, 0x09, 0x05,
        0x0D, 0x03, 0x0B, 0x07, 0x0F
    };

    char lsNib;

    lsNib = luTable[byte>>4];
    byte = (luTable[byte & 0x0F] << 4) | lsNib;
    return byte;
}

/**
 * Vp790EncodeData()
 *  This function performs the 5/3 byte CID message conversion for the VE790 
 * device. It operates on the first three bytes in the message data passed and
 * writes the five converted byte to the line object pointed to by the line
 * context passed.
 *
 * Preconditions:
 *  The line context passed must be pointing to an instantiated Vp790 type line
 * object.
 *
 * Postconditions:
 *  The line object is updated with 5 bytes of CID message data ready to be
 * written to the device.
 */
VpStatusType
Vp790EncodeData(
    VpLineCtxType *pLineCtx,
    uint8 *pMessageData)
{
    Vp790LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 *pCliData = pLineObj->cidEncodeBuff;
    uint8 cliIndex = 0;
    uint8 byte1 = pMessageData[0];
    uint8 byte2 = pMessageData[1];
    uint8 byte3 = pMessageData[2];
    uint8 done = 0;

    while (done == 0) {
        /*
         * Move data to API Buffer:
         *  Reverse the bit order,
         *  Add Start and Stop Bits,
         *  Break data up into 6bit chunks,
         *  Prepend bits 10 to the 6bit chunks for SLAC programming.
         *  If we've reached the end of the user buffer, then fill remaining
         *   API buffer with 1's.
         * Each 3 bytes of user buffer will equal 5 bytes of API buffer.
         */

        /* Encode User Byte 1 */
        pCliData[cliIndex++] = ((Vp790MirrorByte(byte1) >> 3) & 0x1F) | 0x80;
        pCliData[cliIndex] = (((Vp790MirrorByte(byte1) << 3) & 0x38) | 0x84);
        if (done == 1) {
            pCliData[cliIndex++] |= 0x03;
            pCliData[cliIndex++] = 0xC0;     /* End of Message Indicator */
        
        /* Encode User Byte 2 */
        } else {
            pCliData[cliIndex++] |= ((Vp790MirrorByte(byte2) >> 7) & 0x01);
            pCliData[cliIndex++] = ((Vp790MirrorByte(byte2) >> 1) & 0x3F) | 0x80;
            pCliData[cliIndex] = (((Vp790MirrorByte(byte2) << 5) & 0x20) | 0x90);
            if (done == 2) {
                pCliData[cliIndex++] |= 0x0F;
                pCliData[cliIndex++] = 0xC0; /*End of Message Indicator */
            
            /* Encode User Byte 3 */
            } else {
                pCliData[cliIndex++] |= ((Vp790MirrorByte(byte3) >> 5) & 0x07);
                pCliData[cliIndex++] = 
                    ((Vp790MirrorByte(byte3) << 1) & 0x3E) | 0x81;
            }
            done = 3;
        }
    }
    if (done == 3) {
        pCliData[cliIndex] = 0xC0;
    }

    return VP_STATUS_SUCCESS;
}

#endif






