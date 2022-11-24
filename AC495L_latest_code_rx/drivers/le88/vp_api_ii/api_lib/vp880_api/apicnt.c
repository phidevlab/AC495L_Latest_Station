/** \file apicnt.c
 * apicnt.c
 *
 *  This file contains the control functions for the Vp880 device API.
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
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"
#include "sys_service.h"

#define VP880_STD_TONE          0
#define VP880_HOWLER_TONE       4
#define VP880_AUS_HOWLER_TONE   8
#define VP880_NTT_HOWLER_TONE   12
#define VP880_TONE_TYPE         5

/**< Profile index for Generator A/B and C/D starting points (std tone) */
typedef enum
{
    VP880_SIGGEN_AB_START = 8,
    VP880_SIGGEN_CD_START = 16,
    VP880_SIGGEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp880_toneProfileParams;

/**< Profile index for ramp tone parameters */
typedef enum
{
    VP880_START_FREQ_MSB = 8,
    VP880_START_FREQ_LSB = 9,
    VP880_START_LEVEL_MSB = 10,
    VP880_START_LEVEL_LSB = 11,

    VP880_STEP_FREQ_MSB = 12,
    VP880_STEP_FREQ_LSB = 13,
    VP880_STEP_LEVEL_MSB = 14,
    VP880_STEP_LEVEL_LSB = 15,

    VP880_STOP_FREQ_MSB = 16,
    VP880_STOP_FREQ_LSB = 17,
    VP880_STOP_LEVEL_MSB = 18,
    VP880_STOP_LEVEL_LSB = 19,

    VP880_RAMP_TONE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp880_rampToneProfileParams;


/**< Function called by control functions to abstract TX/RX PCM enable bits from
 * line state and option set for Talk state mode.
 */
static VpStatusType
Vp880GetTxRxPcmMode(
    Vp880LineObjectType *pLineObj,
    VpLineStateType state,
    uint8 *mpiByte);

/**< Functions called by Set Line State to abstract device states used for ABS
 * and non ABS devices (if they are different). Set line state then operates
 * on device state only (abstracted from API-II line state).
 */
static uint8
Vp880GetLineStateABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

static uint8
Vp880GetLineStateNonABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

/**< Function called by Send Signal only. Implements message waiting pulse. */
static VpStatusType
Vp880SendMsgWaitPulse(
    VpLineCtxType *pLineCtx,
    VpSendMsgWaitType *pMsgWait);

/**< Function called by Send Signal only. Implements Forward Disconnect. */
static VpStatusType
Vp880SendFwdDisc(
    VpLineCtxType *pLineCtx,
    uint16 timeInMs);

/**< Function called by Send Signal only. Implements Polarity Reversal Pulse */
VpStatusType
Vp880SendPolRevPulse(
    VpLineCtxType *pLineCtx,
    uint16 timeIn1Ms);

/**< Function called by Send Signal only. Implements FXO digit generation */
static VpStatusType
Vp880SendDigit(
    VpLineCtxType *pLineCtx,
    VpDigitGenerationType digitType,
    VpDigitType digit);

/**< Function called by Set Option only. Implements the options specified by
 * the user. The calling function implements the Device/Line control. If a line
 * option is set and a device option is passed, the calling function will call
 * this function once for each line and pass it the line contexts. Therefore,
 * this function will only be subjected to either a device context and device
 * option, or a line context and a line option.
 */
static VpStatusType
Vp880SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value);

/* Function called by SetOptionInternal for Event Masking only */
static void
Vp880MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask,
    VpOptionEventMaskType *pDevEventsMask);

/* Function called by SetOptionInternal to set CODEC mode */
static VpStatusType
Vp880SetCodec(
    VpLineCtxType *pLineCtx,
    VpOptionCodecType codec);

/* Function called by SetOptionInternal to set tx and rx timeslot */
static VpStatusType
Vp880SetTimeSlot(
    VpLineCtxType *pLineCtx,
    uint8 txSlot,
    uint8 rxSlot);

/**< Function called by Api Tick only. Processes the FXS line passed for Api
 * Tick based operations
 */
static VpStatusType
Vp880ProcessFxsLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx);

/**< Function called by Api Tick only. Performs ABS Battery Calibration in
 * forward and reverse feed to "fine tune" the battery switch hysterisis.
 */
static void
Vp880AbsCalibration(
    Vp880DeviceObjectType *pDevObj);

/**< Function called by Api only to determine if a particular line is currently
 * running a test.
 */
static bool Vp880IsChnlUndrTst(
    Vp880DeviceObjectType *pDevObj,
    uint8 channelId);

/**
 * Vp880SetLineState()
 *  This function is the API-II wrapper function for Set Line State - Internal
 * for the Vp880 API.
 *
 * Preconditions:
 *  Same as Vp880SetLineStateInt()
 *
 * Postconditions:
 *  Same as Vp880SetLineStateInt()
 */
VpStatusType
Vp880SetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpStatusType status;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
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

    /* Clear the "called from API" flag. This affects the cadencer */
    pLineObj->status &= ~(VP880_SLS_CALL_FROM_API);
    status = Vp880SetLineStateInt(pLineCtx, state);
    if (status == VP_STATUS_SUCCESS) {
        /*
         * Special FXS handling to prevent setting the line to ringing if
         * off-hook
         */
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
    pLineObj->status |= VP880_SLS_CALL_FROM_API;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880SetLineStateInt()
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
Vp880SetLineStateInt(
    VpLineCtxType *pLineCtx,    /**< Line context to change line state on */
    VpLineStateType state)      /**< The desired line state to set */
{
    uint8 userByte, userByteBefore, currentStateByte, mpiData, mpiByte;
    uint8 currentLineState;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpStatusType status;
    VpProfilePtrType pProfile;

    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 tempRingCtrlData = VP880_GEN_CTRL_EN_BIAS;

    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ringCadAllOn[VP880_CADENCE_TIMER_LEN] = {0x07, 0xFF, 0x00, 0x00};
    VpLineStateType currentState = pLineObj->lineState.currentType;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * Read the status of the Operating Conditions register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     * This also performs the line type/state verification.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
        &mpiData);
    mpiData &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
    mpiData &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

    if (pLineObj->setToneOverride.pcmOverride == TRUE) {
        mpiData |= pLineObj->setToneOverride.opCond;
    } else {
        status = Vp880GetTxRxPcmMode(pLineObj, state, &mpiByte);
        if (status == VP_STATUS_SUCCESS) {
            mpiData |= mpiByte;
        } else {
            return status;
        }
    }
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &mpiData);

    /* We're no longer in the middle of a time function */
    pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
    pLineObj->cadence.timeRemain = 0;

    /*
     * If this function is called by the application, stop the cadencer and
     * reset the Loop Supervision if it is incorrect
     */

    if (!(pLineObj->status & VP880_SLS_CALL_FROM_API)) {
        /* If we're in the middle of active cadence, terminate it */
        if ((pLineObj->cadence.status & VP_CADENCE_STATUS_METERING)
         && (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
            pLineObj->lineEvents.process |= VP_LINE_EVID_MTR_ABORT;
            pLineObj->processData = pLineObj->cadence.meteringBurst;
        }

        if (pLineObj->status & VP880_BAD_LOOP_SUP) {
            pLineObj->status &= ~(VP880_BAD_LOOP_SUP);
            VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                VP880_LOOP_SUP_LEN, pLineObj->loopSup);
        }
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
    }

    /* FXO TYPE LINE HANDLING */
    if (pLineObj->status & VP880_IS_FXO) {
        /*
         * FXO is straightforward, just set as defined by termination type since
         * we already know it's not an unsupported state (except error maybe)
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
            VP880_IODATA_REG_LEN, &userByteBefore);
        userByte = userByteBefore;

        if (pLineObj->termType == VP_TERM_FXO_GENERIC) {
            /* Pre-clear both bits for convenience */
            userByte &= ~(VP880_IODATA_IO1 | VP880_FXO_CID_LINE);

        switch(state) {
                case VP_LINE_FXO_TALK:
                    /* IO3/IO1 = 00, so ok */
                    break;

            case VP_LINE_FXO_OHT:
                    /* IO3/IO1 = 01, so set IO1 to 1 */
                    userByte |= VP880_IODATA_IO1;
                break;

            case VP_LINE_FXO_LOOP_CLOSE:
                    /* IO3/IO1 = 10, so set IO3 to 1 */
                    userByte |= VP880_FXO_CID_LINE;
                    break;

                case VP_LINE_FXO_LOOP_OPEN:
                    /* IO3/IO1 = 11, so set IO3 and IO1 to 1 */
                    userByte |= (VP880_FXO_CID_LINE | VP880_IODATA_IO1);
                break;

            default:
                /* This should be redundant from TX/RX PCM code section above */
                return VP_STATUS_INVALID_ARG;
        }
        } else {
            return VP_STATUS_INVALID_ARG;
        }

        /* Set the loop open/close bit */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
            VP880_IODATA_REG_LEN, &userByte);

        if ((userByteBefore & VP880_IODATA_IO1) ^ (userByte & VP880_IODATA_IO1)) {
            pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 0;
        }

        /* Set the FXO CODEC Mode */
        userByte = (VP880_FXO_ACTIVATE_CODEC | VP880_FXO_SUPERVISION_EN);

        /* Perform the FXO Line State change */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
            VP880_SYS_STATE_LEN, &userByte);
    } else {  /* FXS Handling */
        if (pDevObj->stateInt & VP880_IS_ABS) {
            userByte = Vp880GetLineStateABS(pLineCtx, state);
        } else {
            userByte = Vp880GetLineStateNonABS(pLineCtx, state);
        }

        /* Modify userByte depending on the current polarity */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
            VP880_SYS_STATE_LEN, &currentStateByte);

        if (pLineObj->cadence.pActiveCadence != VP_NULL) {
            if ((pLineObj->cadence.status &
                (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_IGNORE_POLARITY)) ==
                (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_IGNORE_POLARITY)) {

                userByte &= ~VP880_SS_POLARITY_MASK;
                userByte |= (currentStateByte & VP880_SS_POLARITY_MASK);
            }
        }

        if ((state == VP_LINE_RINGING) || (state == VP_LINE_RINGING_POLREV)) {
            pLineObj->cadence.pActiveCadence = pLineObj->pRingingCadence;
            /*
             * We're entering a ringing state, so determine if we need to
             * cadence. If we're not cadencing, this is "always on", so we can
             * disable the currently active cadence sequence and immediately
             * implement the ringing state change.
             */
            pProfile = pLineObj->cadence.pActiveCadence;
            if (pProfile == VP_PTABLE_NULL) {
                /* 
                 * Always on - no need to cadence
                 * Note: This step should be done at initialization, but make
                 * sure just in case the initialization was changed.
                 */
                VpMpiCmd(deviceId, ecVal[channelId],
                    VP880_CADENCE_TIMER_WRT, VP880_CADENCE_TIMER_LEN,
                    ringCadAllOn);

                pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            } else {
                /*
                 * We have a non-null cadence. If the cadence was not previously
                 * started, we'll start it here and let the sequencer take over.
                 * Otherwise, it was previously started and this state change is
                 * at the request of the sequencer.
                 */
                if (!(pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
                    /* We have a cadence and are just starting it */
                    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
                    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
                    pLineObj->cadence.pCurrentPos =
                        &pProfile[VP_PROFILE_TYPE_SEQUENCER_START];
                    pLineObj->cadence.length = pProfile[VP_PROFILE_LENGTH];
                    pLineObj->cadence.status &=
                        ~VP_CADENCE_STATUS_IGNORE_POLARITY;
                    pLineObj->cadence.status |=
                        (pProfile[VP_PROFILE_MPI_LEN] & 0x01) ?
                            VP_CADENCE_STATUS_IGNORE_POLARITY : 0;
                    return VP_STATUS_SUCCESS;
                }
            }
            /*
             * Cadencing already called or null cadence.  We're ready to set
             * the line to the Ringing State but we have to first make sure
             * that the signal generator parameters in the device are setup
             * for the ringing profile
             */

            VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
                VP880_GEN_CTRL_LEN, &tempRingCtrlData);

            if (pLineObj->ringingParams != VP_PTABLE_NULL) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_RINGER_PARAMS_WRT,
                    VP880_RINGER_PARAMS_LEN, pLineObj->ringingParams);
            }

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
        }

        /* 
         * If we are exiting ringing and still on-hook at the time, set the 
         * Ringing exit timer
         */
        switch (pLineObj->lineState.currentType) {
            case VP_LINE_RINGING_POLREV:
            case VP_LINE_RINGING:
                switch(state) {
                    case VP_LINE_RINGING_POLREV:
                    case VP_LINE_RINGING:
                        break;

                    default:
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
                         * If we're in an active Ringing Cadence, and ready to
                         * go into another state, generate the Ringing Event
                         * and indicate that this is the Ringing Off event
                         */
                        if (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) {
                            pProfile = pLineObj->cadence.pActiveCadence;
                            if (pProfile[VP_PROFILE_TYPE_LSB] ==
                                 VP_PRFWZ_PROFILE_RINGCAD) {
                                pLineObj->lineEvents.process
                                    |= VP_LINE_EVID_RING_CAD;
                                pLineObj->processData = VP_RING_CAD_BREAK;
                            }
                        }
                        break;
                }
                break;

            default:
                break;
        }

        switch (currentStateByte & VP880_SS_LINE_FEED_MASK) {
            case VP880_SS_DISCONNECT:
            case VP880_SS_TIP_OPEN:
            case VP880_SS_RING_OPEN:
                switch (userByte & VP880_SS_LINE_FEED_MASK) {
                    case VP880_SS_DISCONNECT:
                    case VP880_SS_TIP_OPEN:
                    case VP880_SS_RING_OPEN:
                        break;

                    default:
                        pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] =
                            MS_TO_TICKRATE(VP_DISCONNECT_RECOVERY_TIME,
                                pDevObj->devProfileData.tickRate);

                        pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] |=
                            VP_ACTIVATE_LINE_TIMER;
                        break;
                }
                break;

            default:
                break;
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SLIC_STATE_RD,
            VP880_SLIC_STATE_LEN, &currentLineState);

        if ((currentLineState & VP880_SS_POLARITY_MASK) !=
            (userByte & VP880_SS_POLARITY_MASK)) {
            pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE] =
                MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
                    pDevObj->devProfileData.tickRate);
            pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                |= VP_ACTIVATE_LINE_TIMER;
        }

        /* Perform the SLIC state change */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SLIC_STATE_WRT,
            VP880_SLIC_STATE_LEN, &userByte);
    }

    /*
     * Update the line object previous and current line state variables. Note
     * that this is not updated with ringing cadence until the line is actually
     * set to ringing (i.e., not when the application sets the line to ringing
     * with (possibly) a non-ringing state via the ringing cadence.
     */
    pLineObj->lineState.previous = currentState;
    pLineObj->lineState.currentType = state;

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetRelGain
 *  This function adjusts the GR and GX values for a given channel of a given
 * device.  It multiplies the profile values by a factor from 0.0 to 4.0.  The
 * adjustment factors are specified in the txLevel and rxLevel parameters,
 * which are 2.14 fixed-point numbers.
 *
 * Preconditions:
 *  The line must first be initialized prior to adjusting the gains.  Any
 * pre-existing results must be cleared by calling VpGetResults() before
 * calling this function.
 *
 * Postconditions:
 *  Returns error if device is not initialized or results are not cleared.
 * Otherwise, generates a VE_LINE_EVID_GAIN_CMP event and saves results in
 * the device object for later retrieval by VpGetResults().
 */
VpStatusType
Vp880SetRelGain(
    VpLineCtxType *pLineCtx,    /**< Line context to change gains on */
    uint16 txLevel,             /**< Adjustment to line's relative Tx level */
    uint16 rxLevel,             /**< Adjustment to line's relative Rx level */
    uint16 handle)              /**< Handle value returned with the event */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pLineCtx->pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpRelGainResultsType *relGainResults = &pDevObj->relGainResults;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint32 gxInt, grInt;
    uint8 gainCSD[VP880_GX_GAIN_LEN];

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

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEVICE_BUSY;
    }

    relGainResults->gResult = VP_GAIN_SUCCESS;

    /* Multiply the profile gain values by the requested adjustments. */
    gxInt = (uint32)pLineObj->gain.gxInt * txLevel / 16384;
    grInt = (uint32)pLineObj->gain.grInt * rxLevel / 16384;

    /* If overflow or underflow occurred, generate out-of-range result. */
    /* Requirement: 1.0 <= gxInt <= 4.0 */
    if ((gxInt < (uint32)0x4000) || (gxInt > (uint32)0x10000)) {
        relGainResults->gResult |= VP_GAIN_GX_OOR;
        gxInt = pLineObj->gain.gxInt;
    }
    /* Requirement: 0.25 <= grInt <= 1.0 */
    if ((grInt < (uint32)0x1000) || (grInt > (uint32)0x4000)) {
        relGainResults->gResult |= VP_GAIN_GR_OOR;
        grInt = pLineObj->gain.grInt;
    }

    /*
     * Write adjusted gain values to the device, and remember them for
     * VpGetResults().
     */
    VpConvertFixed2Csd((uint16)(gxInt - 0x4000), gainCSD);
    relGainResults->gxValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GX_GAIN_WRT, VP880_GX_GAIN_LEN,
        gainCSD);

    VpConvertFixed2Csd((uint16)grInt, gainCSD);
    relGainResults->grValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GR_GAIN_WRT, VP880_GR_GAIN_LEN,
        gainCSD);

    /* Generate the gain-complete event. */
    pLineObj->lineEvents.response |= VP_LINE_EVID_GAIN_CMP;
    pLineObj->lineEventHandle = handle;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880MuteChannel()
 *  This function disables or enables the PCM highway for the selected line and
 * should only be called by API internal functions.
 *
 * Preconditions:
 *  The line context must be valid (i.e., pointing to a valid Vp880 line object
 * type).
 *
 * Postconditions:
 *  If mode is TRUE the TX/RX path is cut. If FALSE, the TX/RX path is enabled
 * according to the current line state and mode used for talk states.
 */
void
Vp880MuteChannel(
    VpLineCtxType *pLineCtx,    /**< Line affected */
    bool mode)                  /**< TRUE = Disable TX/RX, FALSE = enable */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 preState, postState, mpiByte;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * Read the status of the Operating Conditions register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
        &preState);
    postState = preState;
    postState &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
    postState &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

    /*
     * If disabling, simple. Otherwise enable based on the current line state
     * and the state of the "talk" option. The "talk" option is maintained in
     * the line object and abstracted in Vp880GetTxRxMode() function
     */

    Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentType, &mpiByte);

    if (mode == TRUE) {
        /*
         * If awaiting DTMF detection, enable TX, disable RX. This is higher
         * priority than Mute mode. Otherwise, disable both TX and RX.
         */

        postState |= VP880_CUT_RXPATH;  /* Mute == TRUE always cuts RX path */
        if (!(pLineObj->callerId.status & VP_CID_AWAIT_TONE)) {
            /* Not awaiting tone, TX Path is disabled as well */
            postState |= VP880_CUT_TXPATH;
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
            postState |= (VP880_CUT_RXPATH | VP880_CUT_TXPATH);
        } else  {
            postState |= mpiByte;
        }
    }

    if (postState != preState) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &postState);
    }
    return;
}

/**
 * Vp880GetTxRxPcmMode()
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
Vp880GetTxRxPcmMode(
    Vp880LineObjectType *pLineObj,
    VpLineStateType state,  /**< The state associating with PCM mode */
    uint8 *mpiByte) /**< Device Specific byte */
{
    switch(pLineObj->pcmTxRxCtrl) {
        case VP_OPTION_PCM_BOTH:
            *mpiByte = 0x00;
            break;
            
        case VP_OPTION_PCM_RX_ONLY:
            *mpiByte = VP880_CUT_TXPATH;
            break;
            
        case VP_OPTION_PCM_TX_ONLY:
            *mpiByte = VP880_CUT_RXPATH;
            break;

        default:
            *mpiByte = 0x00;
            break;
    }

    switch(state) {
        /* Non-Talk States */
        case VP_LINE_STANDBY:
        case VP_LINE_STANDBY_POLREV:
        case VP_LINE_TIP_OPEN:
        case VP_LINE_ACTIVE:
        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_DISCONNECT:
        case VP_LINE_RINGING:
        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_IS_FXO) {
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP880_CUT_TXPATH | VP880_CUT_RXPATH);
            break;

        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_RING_GND:
            if (!(pLineObj->status & VP880_IS_FXO)) {
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP880_CUT_TXPATH | VP880_CUT_RXPATH);
            break;

        /* Talk States */
        case VP_LINE_TALK:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT:
        case VP_LINE_OHT_POLREV:
            if (pLineObj->status & VP880_IS_FXO) {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_TALK:
            if (!(pLineObj->status & VP880_IS_FXO)) {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        default:
            break;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880GetLineStateABS
 *  Locally used function by Vp880SetLineState to get the line state byte used
 * for ABS devices.
 *
 * Preconditions:
 *  None. State to byte mapping only.
 *
 * Postconditions:
 *  Returns the byte that should be used in the device System State register
 * for the API State passed.
 */
uint8
Vp880GetLineStateABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 returnVal;
    uint8 dcCalVal[VP880_DC_CAL_REG_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_VC) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
            VP880_DC_CAL_REG_LEN, dcCalVal);
    }

    switch(state) {
        case VP_LINE_STANDBY:
            returnVal = VP880_SS_IDLE;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_TIP_OPEN:
            returnVal = VP880_SS_TIP_OPEN;
            break;

        case VP_LINE_ACTIVE:
        case VP_LINE_TALK:
            returnVal = VP880_SS_ACTIVE;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_STANDBY_POLREV:    /* Idle Polrev does not work */
            returnVal = VP880_SS_ACTIVE_POLREV;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        case VP_LINE_OHT:
            returnVal = VP880_SS_ACTIVE_MID_BAT;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_OHT_POLREV:
            returnVal = VP880_SS_ACTIVE_MID_BAT_PR;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        case VP_LINE_DISCONNECT:
            returnVal = VP880_SS_DISCONNECT;
            break;

        case VP_LINE_RINGING:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                returnVal = VP880_SS_UNBALANCED_RINGING;
            } else {
                returnVal = VP880_SS_BALANCED_RINGING;
            }
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                returnVal = VP880_SS_UNBALANCED_RINGING_PR;
            } else {
                returnVal = VP880_SS_BALANCED_RINGING_PR;
            }
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        default:
            returnVal = VP880_SS_DISCONNECT;
            break;
    }

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_VC) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
            VP880_DC_CAL_REG_LEN, dcCalVal);
    }

    return returnVal;
}

/**
 * Vp880GetLineStateNonABS
 *  Locally used function by Vp880SetLineState to get the line state byte used
 * for non-ABS devices.
 *
 * Preconditions:
 *  None. State to byte mapping only.
 *
 * Postconditions:
 *  Returns the byte that should be used in the device System State register
 * for the API State passed.
 */
uint8
Vp880GetLineStateNonABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    switch(state) {
        case VP_LINE_STANDBY:           return VP880_SS_IDLE;
        case VP_LINE_TIP_OPEN:          return VP880_SS_TIP_OPEN;

        case VP_LINE_ACTIVE:
        case VP_LINE_TALK:
        case VP_LINE_OHT:
            return VP880_SS_ACTIVE;

        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT_POLREV:
        case VP_LINE_STANDBY_POLREV:    /* Idle Polrev does not work */
            return VP880_SS_ACTIVE_POLREV;

        case VP_LINE_DISCONNECT:        return VP880_SS_DISCONNECT;
        case VP_LINE_RINGING:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                return VP880_SS_UNBALANCED_RINGING;
            } else {
                return VP880_SS_BALANCED_RINGING;
            }

        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                return VP880_SS_UNBALANCED_RINGING_PR;
            } else {
                return VP880_SS_BALANCED_RINGING_PR;
            }

        default:                        return VP880_SS_DISCONNECT;
    }
}

/**
 * Vp880CommandInstruction()
 *  This function implements the Sequencer Command instruction for the Vp880
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
Vp880CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl, lineState, lsByteCnt;
    uint8 lsConfig[VP880_LOOP_SUP_LEN];
    uint16 tempFreq, tempLevel;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * We know the current value "pSeqData[0]" is 0, now we need to determine if
     * the next command is generator control operator followed by time, or a
     * Line state command -- No other options supported
     */
    switch (pSeqData[0] & VP_SEQ_SUBTYPE_MASK) {
        case VP_SEQ_SUBCMD_SIGGEN:
            VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
                VP880_GEN_CTRL_LEN, &sigGenCtrl);
            sigGenCtrl &= VP880_GEN_ALLOFF;

            /* Get the signal generator bits and set. */
            sigGenCtrl |= ((pSeqData[1] & 0x01) ?  VP880_GENA_EN : 0);
            sigGenCtrl |= ((pSeqData[1] & 0x02) ?  VP880_GENB_EN : 0);
            sigGenCtrl |= ((pSeqData[1] & 0x04) ?  VP880_GENC_EN : 0);
            sigGenCtrl |= ((pSeqData[1] & 0x08) ?  VP880_GEND_EN : 0);

            VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
                VP880_GEN_CTRL_LEN, &sigGenCtrl);
            break;

        case VP_SEQ_SUBCMD_LINE_STATE:
            switch(pSeqData[1]) {
                case VP_PROFILE_CADENCE_STATE_STANDBY:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_STANDBY);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_STANDBY:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_STANDBY_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_TIP_OPEN:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_TIP_OPEN);
                    break;

                case VP_PROFILE_CADENCE_STATE_TALK:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_TALK);
                    break;

                case VP_PROFILE_CADENCE_STATE_ACTIVE:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_ACTIVE);
                    break;

                case VP_PROFILE_CADENCE_STATE_OHT:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT);
                    break;

                case VP_PROFILE_CADENCE_STATE_DISCONNECT:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_DISCONNECT);
                    break;

                case VP_PROFILE_CADENCE_STATE_RINGING:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_RINGING);
                    break;
                
                case VP_PROFILE_CADENCE_STATE_POLREV_RINGING:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_RINGING_POLREV);
                    break;
                
                case VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_ACTIVE_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_TALK:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_TALK_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_POLREV_OHT:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT_POLREV);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_OHT:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_FXO_OHT);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_FXO_LOOP_CLOSE);
                    break;

                case VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_FXO_TALK);
                    break;

                case VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM:
                case VP_PROFILE_CADENCE_STATE_MSG_WAIT_POLREV:
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_RD,
                        VP880_LOOP_SUP_LEN, lsConfig);
                    if (lsConfig[VP880_LOOP_SUP_RT_MODE_BYTE]
                        & VP880_RING_TRIP_AC) {
                        if (!(pLineObj->status & VP880_BAD_LOOP_SUP)) {
                            pLineObj->status |= VP880_BAD_LOOP_SUP;
                            for (lsByteCnt = 0;
                                 lsByteCnt < VP880_LOOP_SUP_LEN; lsByteCnt++) {
                                pLineObj->loopSup[lsByteCnt] =
                                    lsConfig[lsByteCnt];
                            }
                        }

                        /* Force DC Trip */
                        lsConfig[VP880_LOOP_SUP_RT_MODE_BYTE] &=
                            ~VP880_RING_TRIP_AC;
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                            VP880_LOOP_SUP_LEN, lsConfig);
                    }

                    lineState =
                        (pSeqData[1] == VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM) ?
                        VP880_SS_BALANCED_RINGING :
                        VP880_SS_BALANCED_RINGING_PR;

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                        VP880_SYS_STATE_LEN, &lineState);
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
                pLineObj->callerId.cliMPIndex = 0;
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

        case VP_SEQ_SUBCMD_RAMP_GENERATORS:
            tempFreq = (pLineObj->cadence.regData[3] << 8);
            tempFreq |= pLineObj->cadence.regData[4];

            tempLevel = (pLineObj->cadence.regData[5] << 8);
            tempLevel |= pLineObj->cadence.regData[6];
            
            if (pLineObj->cadence.isFreqIncrease == TRUE) {
                /* Check if we're at or above the max frequency */
                if (tempFreq >= pLineObj->cadence.stopFreq) {
                    pLineObj->cadence.isFreqIncrease = FALSE;
                    tempFreq -= pLineObj->cadence.freqStep;
                } else {
                    tempFreq += pLineObj->cadence.freqStep;
                }
            } else {
                if (tempFreq < 
                    (pLineObj->cadence.startFreq - pLineObj->cadence.freqStep)) {
                    pLineObj->cadence.isFreqIncrease = TRUE;
                    tempFreq += pLineObj->cadence.freqStep;
                } else {
                    tempFreq -= pLineObj->cadence.freqStep;
                }
            }
            pLineObj->cadence.regData[3] = (tempFreq >> 8) & 0xFF;
            pLineObj->cadence.regData[4] = tempFreq & 0xFF;

            /* 
             * Check if we're at or above the max level, but make sure we don't
             * wrap around
             */
            if (tempLevel < 
                (pLineObj->cadence.stopLevel - 
                    ((tempLevel * pLineObj->cadence.levelStep) / 10000))) {
                
                tempLevel += ((tempLevel * pLineObj->cadence.levelStep) / 10000);

                pLineObj->cadence.regData[5] = (tempLevel >> 8) & 0xFF;
                pLineObj->cadence.regData[6] = tempLevel & 0xFF;
            }
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
                VP880_SIGAB_PARAMS_LEN, pLineObj->cadence.regData);
            break;

        case VP_SEQ_SUBCMD_METERING:
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SLIC_STATE_RD,
                VP880_SLIC_STATE_LEN, &lineState);

            if (pSeqData[1]) {  /* Metering On */
                lineState |= VP880_SS_METERING_MASK;
                pLineObj->cadence.meteringBurst++;
            } else {    /* Metering Off */
                lineState &= ~VP880_SS_METERING_MASK;
            }

            VpMpiCmd(deviceId, ecVal[channelId], VP880_SLIC_STATE_WRT,
                VP880_SLIC_STATE_LEN, &lineState);
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /*
     * Check to see if there is more sequence data, and if so, move the
     * sequence pointer to the next command. Otherwise, end this cadence
     */
    pLineObj->cadence.index+=2;
    if (pLineObj->cadence.index <=
       (pLineObj->cadence.length + VP_PROFILE_LENGTH + 1)) {
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

            case VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT:
                pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
                pLineObj->processData = VP_SENDSIG_MSG_WAIT_PULSE;
                VpSetLineState(pLineCtx, pLineObj->lineState.usrCurrent);
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
 * Vp880SetLineTone()
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
Vp880SetLineTone(
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
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpProfilePtrType pToneProf = VP_PTABLE_NULL;
    VpProfilePtrType pCadProf = VP_PTABLE_NULL;

    VpDigitType digit = VP_DIG_NONE;
    VpDirectionType direction = VP_DIRECTION_INVALID;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 opCond, sigGenCtrl, sigGenABCount, mpiByte;
    uint8 converterCfg[VP880_CONV_CFG_LEN];

    /* Initialize SigGen A/B values to 0 */
    uint8 sigGenAB[VP880_SIGAB_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8 sigGenCD[VP880_SIGCD_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    uint16 ukHowler[] = {
        0x08, 0x88, 0x07, 0xDB, /* Start Freqency/Amplitude (800Hz,-15dBm0) */
        0x01, 0x04, 0x00, 0x30, /* Step Frequency/Amplitude */
        0x22, 0x22, 0x7F, 0xFF  /* Stop Frequency/Amplitude (3200Hz, +3.14dBm0) */
    };

    uint16 ausHowler[] = {
        0x10, 0x00, 0x07, 0xDB, /* Start Freqency/Amplitude (1500Hz,-15dBm0) */
        0x00, 0xB8, 0x00, 0x18, /* Step Frequency/Amplitude  */
        0x22, 0x22, 0x7F, 0xFF  /* Stop Frequency/Amplitude (3200Hz, +3.14dBm0) */
    };

    uint16 nttHowler[] = {
        0x04, 0x44, 0x07, 0xDB, /* Start Freqency/Amplitude (400Hz,-15dBm0) */
        0x00, 0x00, 0x00, 0x40, /* Step Frequency/Amplitude  */
        0x04, 0x44, 0x7F, 0xFF  /* Stop Frequency/Amplitude (400Hz, +3.14dBm0) */
    };

    uint8 sigGenABOffset = 3;  /* Starting point after filler bytes */

    int toneIndex = GetProfileIndex(pToneProfile);
    int cadenceIndex = GetProfileIndex(pCadProfile);
    uint16 profIndex;   /* Used for bit masking the profile index table */

    VpDeviceIdType deviceId = pDevObj->deviceId;

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
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pToneProf = pToneProfile;
    } else if (toneIndex < VP_CSLAC_TONE_PROF_TABLE_SIZE) {
        profIndex = (uint16)toneIndex;
        pToneProf = pDevObj->devProfileTable.pToneProfileTable[profIndex];
        if (!(pDevObj->profEntry.toneProfEntry & (0x0001 << profIndex))) {
            /* The profile is invalid -- error. */
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    /* Verify a good profile (index or pointer) for the cadence */
    if (cadenceIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pCadProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONECAD, pCadProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pCadProf = pCadProfile;
    } else if (cadenceIndex < VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
        pCadProf = pDevObj->devProfileTable.pToneCadProfileTable[cadenceIndex];
        if (!(pDevObj->profEntry.toneCadProfEntry & (0x0001 << cadenceIndex))) {
            /* The profile is invalid -- error. */
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    if (pDtmfControl != VP_NULL) {
        digit = pDtmfControl->toneId;
        if (VpIsDigit(digit) == FALSE) {
            return VP_STATUS_INVALID_ARG;
        }
        
        direction = pDtmfControl->dir;
        if ((direction != VP_DIRECTION_DS) && (direction != VP_DIRECTION_US)) {
            return VP_STATUS_INVALID_ARG;
        }
    }

    /* All input parameters are valid. */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pLineObj->status & VP880_BAD_LOOP_SUP) {
        pLineObj->status &= ~(VP880_BAD_LOOP_SUP);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
            VP880_LOOP_SUP_LEN, pLineObj->loopSup);
    }

    /*
     * Disable signal generator A/B/C/D before making any changes and stop
     * previous cadences
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);
    sigGenCtrl &= ~VP880_GEN_ALLON;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);

    if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
        pLineObj->cadence.pActiveCadence = pCadProf;
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;

        /* We're no longer in the middle of a time function */
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
        pLineObj->cadence.timeRemain = 0;
    }

    /* We'll almost for sure update this register, so read it now */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
        &opCond);
    opCond &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

    /*
     * If tone profile is NULL, and either the pDtmfControl is NULL or it's
     * "digit" member is "Digit None", then shutoff the tone generators, stop
     * any active cadencing and restore the filter coefficients if they need
     * to be. Also, re-enable the audio path if it was disabled by a previous
     * DTMF generation command
     */
    if (((pToneProf == VP_PTABLE_NULL) && (pDtmfControl == VP_NULL))
     || ((pToneProf == VP_PTABLE_NULL) && (digit == VP_DIG_NONE))) {
        /* Shutoff all tones and stop the cadencing -- already done */

        /* Restore the normal audio path */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_RD, 
            VP880_CONV_CFG_LEN, converterCfg);

        converterCfg[0] &= ~VP880_CONV_CONNECT_BITS;
        converterCfg[0] |= VP880_METALLIC_AC_V;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT, 
            VP880_CONV_CFG_LEN, converterCfg);

        /*
         * If we were sending a DTMF tone, indicated by a PCM override, restore
         * the PCM paths and return control over to set line state (and other
         * functions)
         */
        if (pLineObj->setToneOverride.pcmOverride == TRUE) {
            /*
             * Indicate that we will use line state control for PCM enable
             * bits
             */
            pLineObj->setToneOverride.pcmOverride = FALSE;

            /*
             * Pre-Or the bits and get the correct values based on the current
             * line state, then update the device
             */
            opCond &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
            Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentType, &mpiByte);
            opCond |= mpiByte;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
                VP880_OP_COND_LEN, &opCond);
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

        /* Update the DTMF Generators */
        Vp880SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

        /*
         * Save off the B-Filter coefficients if sending upstream DTMF and
         * overwrite device with "complete reflection" coefficients. Indicate
         * that we want a PCM override of the line state
         */

        pLineObj->setToneOverride.pcmOverride = TRUE;
        pLineObj->setToneOverride.opCond = 0x00;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_RD, 
            VP880_CONV_CFG_LEN, converterCfg);
        converterCfg[0] &= ~VP880_CONV_CONNECT_BITS;
        
        if (direction == VP_DIRECTION_US) {
            converterCfg[0] |= VP880_VOICE_DAC;
        } else {
            converterCfg[0] |= VP880_METALLIC_AC_V;
        }
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT, 
            VP880_CONV_CFG_LEN, converterCfg);

        /*
         * Disable only the receive path since disabling the transmit path
         * also may generate noise upstream (e.g., an unterminated, but
         * assigned timeslot
         */
        pLineObj->setToneOverride.opCond |= VP880_CUT_RXPATH;

        opCond |= pLineObj->setToneOverride.opCond;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &opCond);

        /* Enable only generator A/B */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
        sigGenCtrl |= (VP880_GENB_EN | VP880_GENA_EN);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* If we're here, we're sending a Tone, not DTMF */
    if ((pCadProf != VP_PTABLE_NULL) 
     && ((pCadProf[VP880_TONE_TYPE] == VP880_HOWLER_TONE)
      || (pCadProf[VP880_TONE_TYPE] == VP880_AUS_HOWLER_TONE)
      || (pCadProf[VP880_TONE_TYPE] == VP880_NTT_HOWLER_TONE))) {
        if (pCadProf[VP880_TONE_TYPE] == VP880_HOWLER_TONE) {
            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = ukHowler[0] << 8; 
            pLineObj->cadence.startFreq |= ukHowler[1]; 

            pLineObj->cadence.startLevel = ukHowler[2] << 8; 
            pLineObj->cadence.startLevel |= ukHowler[3]; 

            pLineObj->cadence.freqStep = ukHowler[4] << 8; 
            pLineObj->cadence.freqStep |= ukHowler[5]; 

            pLineObj->cadence.levelStep = ukHowler[6] << 8; 
            pLineObj->cadence.levelStep |= ukHowler[7]; 

            pLineObj->cadence.stopFreq = ukHowler[8] << 8; 
            pLineObj->cadence.stopFreq |= ukHowler[9]; 

            pLineObj->cadence.stopLevel = ukHowler[10] << 8; 
            pLineObj->cadence.stopLevel |= ukHowler[11]; 

            sigGenAB[3] = ukHowler[0]; 
            sigGenAB[4] = ukHowler[1]; 

            sigGenAB[5] = ukHowler[2]; 
            sigGenAB[6] = ukHowler[3]; 
        } else if (pCadProf[VP880_TONE_TYPE] == VP880_AUS_HOWLER_TONE) {
            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = ausHowler[0] << 8; 
            pLineObj->cadence.startFreq |= ausHowler[1]; 

            pLineObj->cadence.startLevel = ausHowler[2] << 8; 
            pLineObj->cadence.startLevel |= ausHowler[3]; 

            pLineObj->cadence.freqStep = ausHowler[4] << 8; 
            pLineObj->cadence.freqStep |= ausHowler[5]; 

            pLineObj->cadence.levelStep = ausHowler[6] << 8; 
            pLineObj->cadence.levelStep |= ausHowler[7]; 

            pLineObj->cadence.stopFreq = ausHowler[8] << 8; 
            pLineObj->cadence.stopFreq |= ausHowler[9]; 

            pLineObj->cadence.stopLevel = ausHowler[10] << 8; 
            pLineObj->cadence.stopLevel |= ausHowler[11]; 

            sigGenAB[3] = ausHowler[0]; 
            sigGenAB[4] = ausHowler[1]; 

            sigGenAB[5] = ausHowler[2]; 
            sigGenAB[6] = ausHowler[3]; 
        } else {
            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = nttHowler[0] << 8;
            pLineObj->cadence.startFreq |= nttHowler[1];

            pLineObj->cadence.startLevel = nttHowler[2] << 8;
            pLineObj->cadence.startLevel |= nttHowler[3];

            pLineObj->cadence.freqStep = nttHowler[4] << 8;
            pLineObj->cadence.freqStep |= nttHowler[5];

            pLineObj->cadence.levelStep = nttHowler[6] << 8;
            pLineObj->cadence.levelStep |= nttHowler[7];

            pLineObj->cadence.stopFreq = nttHowler[8] << 8;
            pLineObj->cadence.stopFreq |= nttHowler[9];

            pLineObj->cadence.stopLevel = nttHowler[10] << 8;
            pLineObj->cadence.stopLevel |= nttHowler[11];

            sigGenAB[3] = nttHowler[0];
            sigGenAB[4] = nttHowler[1];

            sigGenAB[5] = nttHowler[2];
            sigGenAB[6] = nttHowler[3];
        }

        /* Set the signal generator A parameters to the initial value. */
        pLineObj->cadence.isFreqIncrease = TRUE;
        
        /* Make sure C/D are cleared */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGCD_PARAMS_WRT,
            VP880_SIGCD_PARAMS_LEN, sigGenCD);

        /* Program A/B */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
            VP880_SIGAB_PARAMS_LEN, sigGenAB);

        /* Set the parameters in the line object for cadence use */    
        for (sigGenABCount = 0; sigGenABCount < VP880_SIGAB_PARAMS_LEN;
             sigGenABCount++) {
            pLineObj->cadence.regData[sigGenABCount] = sigGenAB[sigGenABCount];             
        }
    } else {
        /*
         * Send the signal generator parameters to the device and enable the
         * Tone Generators -- add in the first 3 bytes (all 0x00)
         */
        for (sigGenABCount = sigGenABOffset, profIndex = 0;
             sigGenABCount < VP880_SIGAB_PARAMS_LEN;
             sigGenABCount++, profIndex++) {
            sigGenAB[sigGenABCount] =
                pToneProf[VP880_SIGGEN_AB_START + profIndex];
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
            VP880_SIGAB_PARAMS_LEN, sigGenAB);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGCD_PARAMS_WRT,
            VP880_SIGCD_PARAMS_LEN,
            (VpProfileDataType *)(&pToneProf[VP880_SIGGEN_CD_START]));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
    }

    if (pCadProf == VP_PTABLE_NULL) {
        /*
         * If a tone is being actived due to caller ID, then do not stop the
         * cadencer
         */
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        }
        sigGenCtrl |= VP880_GEN_ALLON;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
    } else {
        pLineObj->cadence.pCurrentPos =
            &(pCadProf[VP_PROFILE_TYPE_SEQUENCER_START]);
        pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
        pLineObj->cadence.length = pCadProf[VP_PROFILE_LENGTH];
        pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_IGNORE_POLARITY;
        pLineObj->cadence.status |= (pCadProf[VP_PROFILE_MPI_LEN] & 0x01) ?
            VP_CADENCE_STATUS_IGNORE_POLARITY : 0;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetRelayState()
 *  This function controls the state of controlled relays for the VP880 device.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The indicated relay state is set for the given line.
 */
VpStatusType
Vp880SetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpStatusType status = VP_STATUS_SUCCESS;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 ioDirection[VP880_IODIR_REG_LEN];
    uint8 ioData[VP880_IODATA_REG_LEN];
    uint8 icr1[VP880_ICR1_LEN] = {0x00, 0x00, 0xC0, 0x00};

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

    /* Check term type */
    if (pLineObj->termType == VP_TERM_FXS_GENERIC) {
    /* Check requested relay state */    
        if ((rState != VP_RELAY_NORMAL) && 
            (rState != VP_RELAY_BRIDGED_TEST)) { 
            return VP_STATUS_INVALID_ARG;
        }
    } else if ((pLineObj->termType == VP_TERM_FXS_ISOLATE)
            || (pLineObj->termType == VP_TERM_FXS_SPLITTER)) {
        /* Check requested relay state */    
        if ((rState != VP_RELAY_NORMAL) && 
            (rState != VP_RELAY_RESET)  &&
            (rState != VP_RELAY_BRIDGED_TEST)) { 
            return VP_STATUS_INVALID_ARG;
        }    
    } else {
        return VP_STATUS_INVALID_ARG;    
    }    
    
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pLineObj->termType == VP_TERM_FXS_ISOLATE) {
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
        VP880_IODATA_REG_LEN, ioData);

    /* Set the I/O Direction for this termination type */
    ioDirection[0] &= ~VP880_IODIR_IO1_MASK;
    ioDirection[0] |= VP880_IODIR_IO1_OUTPUT;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
        VP880_IODIR_REG_LEN, ioDirection);
    }

    if (pLineObj->termType == VP_TERM_FXS_ISOLATE) {
    switch (rState) {
        case VP_RELAY_NORMAL:
            icr1[3] = 0x00;
            ioData[0] |= 0x01;
            break;
            
        case VP_RELAY_RESET:
            icr1[3] = 0x00;
            ioData[0] &= ~(0x01);
            break;

        case VP_RELAY_BRIDGED_TEST:
            if (!(pDevObj->stateInt & VP880_HAS_TEST_LOAD_SWITCH)) {
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_INVALID_ARG;
            }
            ioData[0] |= 0x01;        
            icr1[3] = 0x40;
            break;

        default:    
            /* There is insufficient error checking above if this occurs */
            status = VP_STATUS_INVALID_ARG;
            break;
    }
    } else { /* VP_TERM_FXS_SPLITTER */
        switch (rState) {
            case VP_RELAY_NORMAL:
                icr1[3] = 0x00;
                ioData[0] &= ~(0x01);
                break;

            case VP_RELAY_RESET:
                icr1[3] = 0x00;
                ioData[0] |= 0x01;
                break;

            case VP_RELAY_BRIDGED_TEST:
                if (!(pDevObj->stateInt & VP880_HAS_TEST_LOAD_SWITCH)) {
                    return VP_STATUS_INVALID_ARG;
                }
                ioData[0] &= ~(0x01);
                icr1[3] = 0x40;
                break;

            default:
                /* There is insufficient error checking above if this occurs */
                status = VP_STATUS_INVALID_ARG;
                break;
        }

    }


    if (VP_STATUS_SUCCESS == status) {
        if (pLineObj->termType == VP_TERM_FXS_ISOLATE) {
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
        VP880_IODATA_REG_LEN, ioData);
        }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
        VP880_ICR1_LEN, icr1);
        
        pLineObj->relayState = rState;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
}

/**
 * Vp880SendSignal()
 *  This function sends a signal on the line. The type of signal is specified
 * by the type parameter passed. The structure passed specifies the parameters
 * associated with the signal.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The signal specified is applied to the line.
 */
VpStatusType
Vp880SendSignal(
    VpLineCtxType *pLineCtx,
    VpSendSignalType type,
    void *pStruct)
{
    VpDigitType *pDigit;
    VpDigitType digit = VP_DIG_NONE;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpStatusType status;

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

    if (pStruct == VP_NULL) {
        pDigit = &digit;
    } else {
        pDigit = pStruct;
    }

    switch(type) {
        case VP_SENDSIG_MSG_WAIT_PULSE:
            status = Vp880SendMsgWaitPulse(pLineCtx, pStruct);
            break;

        case VP_SENDSIG_DTMF_DIGIT:
            status = Vp880SendDigit(pLineCtx, VP_DIGIT_GENERATION_DTMF, *pDigit);
            break;

        case VP_SENDSIG_PULSE_DIGIT:
            pDigit = (VpDigitType *)pStruct;
            status = Vp880SendDigit(pLineCtx, VP_DIGIT_GENERATION_DIAL_PULSE,
                *pDigit);
            break;

        case VP_SENDSIG_HOOK_FLASH:
            /* prevent case of *pDigit when user passes VP_NULL */
            status = Vp880SendDigit(pLineCtx, VP_DIGIT_GENERATION_DIAL_HOOK_FLASH,
                VP_DIG_NONE);
            break;

        case VP_SENDSIG_FWD_DISCONNECT:
            if (pStruct != VP_NULL) {
                status = Vp880SendFwdDisc(pLineCtx, *((uint16 *)pStruct));
            } else {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_SENDSIG_POLREV_PULSE:
            if (pStruct != VP_NULL) {
                status = Vp880SendPolRevPulse(pLineCtx, *((uint16 *)pStruct));
            } else {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        default:
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    return status;
}

/**
 * Vp880SendMsgWaitPulse()
 *  This function sends a message waiting pulse to the line specified by the
 * by the pMsgWait parameter passed. The structure specifies a voltage, on-time,
 * off-time, and number of pulses.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The message waiting signal specified is applied to the line.
 */
VpStatusType
Vp880SendMsgWaitPulse(
    VpLineCtxType *pLineCtx,
    VpSendMsgWaitType *pMsgWait)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint16 tempTime, firstTimer, secondTimer;
    uint16 tickRate = pDevObj->devProfileData.tickRate;

    VpLineStateType current = pLineObj->lineState.usrCurrent;

    uint8 seqByte, branchCount;
    uint8 addStep = 0;

    uint32 aVolt;
    int32 userVolt;
    uint8 cmdLen = 0x08;  /* Minimum Cadence with infinite on */

    /*
     * Set the signal generator parameters to set the A amplitude and frequency
     * "very low". We'll adjust the bias to the user defined MsgWait voltage
     */
    uint8 sigGenBytes[VP880_SIGA_PARAMS_LEN] = {
        0x00, 0x29, 0x73, 0x04, 0x44, 0x00, 0x15, 0x7F, 0xFD, 0x00, 0x00};

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pLineObj->status & VP880_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If we're already in Ringing, return a failure since we're using a
     * shared resource to accomplish this function.
     */
    if ((current == VP_LINE_RINGING) || (current == VP_LINE_RINGING_POLREV)) {
        return VP_STATUS_FAILURE;
    }

    /*
     * If the voltage is 0, it (previously) meant to use the maximum voltage
     * supported by the line. However, that function has been removed so instead
     * of stopping Message Waiting, just return error code to maintain a bit of
     * backward compatibility (max voltage isn't applied, but it isn't stopped
     * either).
     */
    if ((pMsgWait != VP_NULL) && (pMsgWait->voltage == 0)) {
        return VP_STATUS_INVALID_ARG;
    }

    /* All parameters passed are good -- proceed */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous 
     * sequences first
     */
    pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;    /* No active status */

    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /*
     * If we were previously running a Message Waiting cadence, stop it and
     * generate the event.
     * If we were previously running another cadence, let it continue and
     * return.
     */
    if ((pMsgWait == VP_NULL) || (pMsgWait->onTime == 0)) {
        VpSetLineState(pLineCtx, current);
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;
        pLineObj->lineEvents.process |= VP_LINE_EVID_SIGNAL_CMP;
        pLineObj->processData = VP_SENDSIG_MSG_WAIT_PULSE;
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * Compute the new signal generator A values from the voltage and set the
     * line state that is used to apply the message waiting pulse
     */
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    if (pMsgWait->voltage > 0) {
        userVolt = pMsgWait->voltage;
        pLineObj->intSequence[9] = VP_PROFILE_CADENCE_STATE_MSG_WAIT_NORM;
    } else {
        userVolt = -pMsgWait->voltage;
        pLineObj->intSequence[9] = VP_PROFILE_CADENCE_STATE_MSG_WAIT_POLREV;
    }

    /* Scale by same factor as bit resolution */
    aVolt = userVolt * (uint32)VP880_RINGING_BIAS_FACTOR;

    /* Scale down by the bit resolution of the device */
    aVolt /= (uint32)VP880_RINGING_BIAS_SCALE;

    sigGenBytes[VP880_SIGA_BIAS_MSB] = (aVolt >> 8) & 0xFF;
    sigGenBytes[VP880_SIGA_BIAS_LSB] = (aVolt & 0xFF);

    /* Write the new signal generator parameters */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_RINGER_PARAMS_WRT,
       VP880_RINGER_PARAMS_LEN, sigGenBytes);

    /*
     * Build the rest of the cadence defined by the user input (message state
     * set above). Start by setting the type of profile to an API Message Wait
     * Pulse type
     */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
        VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT;

    /*
     * Set the timers for on/off pulse duration, scale from mS to tickRate
     * and prevent rounding down to 0
     */
    if (pMsgWait->onTime < (tickRate >> 8)) {
        firstTimer = 3;
    } else {
        firstTimer = MS_TO_TICKRATE(pMsgWait->onTime, tickRate);

        /* Prevent 0 for time (because that means "forever") */
        if (firstTimer <= 2) {
            firstTimer = 3;
        }
    }

    branchCount = 0;
    if (firstTimer > 8192) {
        /* Special Handling for using 16-bit time in 14-bit data fields */
        for (; firstTimer > 8192; branchCount++) {
            firstTimer = ((firstTimer >> 1) & 0x3FFF);
        }
        cmdLen+=2;
    }

    pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    tempTime = ((firstTimer - 2) >> 8) & 0x1F;
    pLineObj->intSequence[10] |= tempTime;

    tempTime = ((firstTimer - 2) & 0x00FF);
    pLineObj->intSequence[11] = tempTime;

    if (branchCount) {
        pLineObj->intSequence[12] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
        pLineObj->intSequence[12] |= 0x01;  /* On-Time is the step 1 (0 base) */
        pLineObj->intSequence[13] = branchCount;
        addStep+=2;
    }

    /*
     * If the off-time is 0, we will stay in the previous state forever so the
     * cadencer needs to stop where it is
     */
    if (pMsgWait->offTime == 0) {
        pLineObj->intSequence[VP_PROFILE_LENGTH] = cmdLen;
        pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] =
            (0x04 + addStep);
    } else {
        cmdLen+=4;   /* Add two for the next state and two for the off-time */

        /* In-between pulses we'll return to the current state */
        pLineObj->intSequence[12+addStep]
            = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
        pLineObj->intSequence[13+addStep] =
            ConvertApiState2PrfWizState(current);

        /*
         * Set the timers for on/off pulse duration, scale from mS to tickRate
         * and prevent rounding down to 0
         */
        if (pMsgWait->offTime < (tickRate >> 8)) {
            secondTimer = 3;
        } else {
            secondTimer = MS_TO_TICKRATE(pMsgWait->offTime, tickRate);

            /* Prevent 0 for time (because that means "forever") */
            if (secondTimer <= 2) {
                secondTimer = 3;
            }
        }

        branchCount = 0;
        if (secondTimer > 8192) {
            cmdLen+=2;   /* Add two for the off-time branch loop */
            /* Special Handling for using 16-bit time in 14-bit data fields */
            for (; secondTimer > 8192; branchCount++) {
                secondTimer = ((secondTimer >> 1) & 0x3FFF);
            }
        }

        pLineObj->intSequence[14+addStep] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
        tempTime = ((secondTimer - 2) >> 8) & 0x1F;
        pLineObj->intSequence[14+addStep] |= tempTime;

        tempTime = ((secondTimer - 2) & 0x00FF);
        pLineObj->intSequence[15+addStep] = tempTime;

        if (branchCount) {
            pLineObj->intSequence[16+addStep] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
            pLineObj->intSequence[16+addStep] |= (0x03 + (addStep / 2));
            pLineObj->intSequence[17+addStep] = branchCount;
            addStep+=2;
        }

        /*
         * If the number of cycles is 0, set the branch to repeat forever. If
         * it's 1, don't add a branch statement because the sequence should end
         * after the first cycle, otherwise subtract 1 from the total number of
         * cycles to force the correct number of "repeats" (branch)
         */

        if (pMsgWait->cycles != 1) {
            cmdLen+=2; /* Two more for this last branch operator */
            pLineObj->intSequence[16+addStep] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
            pLineObj->intSequence[17+addStep] = (pMsgWait->cycles) ?
                (pMsgWait->cycles - 1) : pMsgWait->cycles;
        }
    }

    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = cmdLen;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = cmdLen - 4;

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
 * Vp880SendFwdDisc()
 *  This function sends a forward disconnect to the line specified for a duration
 * given in mS.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  A disconnect has been applied to the line, the line state is restored to what
 * it was prior to this function being called. No events are generated while a
 * disconnect is occuring -- the application should know that it is not possible
 * to detect a line condition while no feed is being presented.
 */
VpStatusType
Vp880SendFwdDisc(
    VpLineCtxType *pLineCtx,
    uint16 timeInMs)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpLineStateType current = pLineObj->lineState.usrCurrent;
    VpProfileCadencerStateTypes cadenceState;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 seqByte, index;
    uint16 timeIn5mS = 0;

    if (pLineObj->status & VP880_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    switch(current) {
        case VP_LINE_STANDBY:
            cadenceState = VP_PROFILE_CADENCE_STATE_STANDBY;
            break;

        case VP_LINE_TIP_OPEN:
            cadenceState = VP_PROFILE_CADENCE_STATE_TIP_OPEN;
            break;

        case VP_LINE_TALK:
            cadenceState = VP_PROFILE_CADENCE_STATE_TALK;
            break;

        case VP_LINE_ACTIVE:
            cadenceState = VP_PROFILE_CADENCE_STATE_ACTIVE;
            break;

        case VP_LINE_OHT:
            cadenceState = VP_PROFILE_CADENCE_STATE_OHT;
            break;

        case VP_LINE_DISCONNECT:
            cadenceState = VP_PROFILE_CADENCE_STATE_DISCONNECT;
            break;

        case VP_LINE_RINGING:
            cadenceState = VP_PROFILE_CADENCE_STATE_RINGING;
            break;

        case VP_LINE_RINGING_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_RINGING;
            break;

        case VP_LINE_ACTIVE_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE;
            break;

        case VP_LINE_TALK_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_TALK;
            break;

        case VP_LINE_OHT_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_OHT;
            break;

        default:
            return VP_STATUS_FAILURE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = 0x0000;    /* No active status */

    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Set the cadence type */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = VP_PRFWZ_PROFILE_FWD_DISC_INT;

    /* First step is to go to disconnect */
    index = 0;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_PROFILE_CADENCE_STATE_DISCONNECT;

    /* Then wait for the time specified -- rounded to 5mS increments */
    if (timeInMs < 5) {
        timeIn5mS = 1;
    } else {
        timeIn5mS = timeInMs / 5;
    }
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        |= (timeIn5mS >> 8) & 0x1F;

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (timeIn5mS & 0xFF);

    /* Restore the line state */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = cadenceState;

    /* Then wait for 100mS for the detector to become stable */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = 20; /* 5mS per increment */

    index++; /* Adjust one more for length values */
    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = index + 4;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos =
        &pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SendPolRevPulse()
 *  This function sends a polarity reversal pulse to the line specified for a
 * duration given in mS.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  A polarity reversal has been applied to the line, the line state is restored
 * to what it was prior to this function being called. A hook event may be
 * generated while the polarity reversal is occuring. The application should
 * ignore as appropriate based on expected loop conditions.
 */
VpStatusType
Vp880SendPolRevPulse(
    VpLineCtxType *pLineCtx,
    uint16 timeInMs)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpLineStateType current = pLineObj->lineState.usrCurrent;
    VpProfileCadencerStateTypes cadenceState, polRevState;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 seqByte, index;
    uint16 timeIn5mS = 0;

    if (pLineObj->status & VP880_IS_FXO) {
        return VP_STATUS_INVALID_ARG;
    }

    switch(current) {
        case VP_LINE_STANDBY:
            cadenceState = VP_PROFILE_CADENCE_STATE_STANDBY;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_STANDBY;
            break;

        case VP_LINE_TIP_OPEN:
            cadenceState = VP_PROFILE_CADENCE_STATE_TIP_OPEN;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_TIP_OPEN;
            break;

        case VP_LINE_TALK:
            cadenceState = VP_PROFILE_CADENCE_STATE_TALK;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_TALK;
            break;

        case VP_LINE_ACTIVE:
            cadenceState = VP_PROFILE_CADENCE_STATE_ACTIVE;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE;
            break;

        case VP_LINE_OHT:
            cadenceState = VP_PROFILE_CADENCE_STATE_OHT;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_OHT;
            break;

        case VP_LINE_DISCONNECT:
            cadenceState = VP_PROFILE_CADENCE_STATE_DISCONNECT;
            polRevState = VP_PROFILE_CADENCE_STATE_DISCONNECT;
            break;

        case VP_LINE_RINGING:
            cadenceState = VP_PROFILE_CADENCE_STATE_RINGING;
            polRevState = VP_PROFILE_CADENCE_STATE_POLREV_RINGING;
            break;

        case VP_LINE_RINGING_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_RINGING;
            polRevState = VP_PROFILE_CADENCE_STATE_RINGING;
            break;

        case VP_LINE_ACTIVE_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_ACTIVE;
            polRevState = VP_PROFILE_CADENCE_STATE_ACTIVE;
            break;

        case VP_LINE_TALK_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_TALK;
            polRevState =  VP_PROFILE_CADENCE_STATE_TALK;
            break;

        case VP_LINE_OHT_POLREV:
            cadenceState = VP_PROFILE_CADENCE_STATE_POLREV_OHT;
            polRevState = VP_PROFILE_CADENCE_STATE_OHT;
            break;

        default:
            return VP_STATUS_FAILURE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * This is implemented with the cadencer so we have to stop all previous
     * sequences first
     */
    pLineObj->cadence.status = 0x0000;    /* No active status */

    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    /* Set the cadence type */
    pLineObj->intSequence[VP_PROFILE_TYPE_LSB] =
        VP_PRFWZ_PROFILE_POLREV_PULSE_INT;

    /* First step is to go to polrev state */
    index = 0;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = polRevState;

    /* Then wait for the time specified -- rounded to 5mS increments */
    if (timeInMs < 5) {
        timeIn5mS = 1;
    } else {
        timeIn5mS = timeInMs / 5;
    }
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        |= (timeIn5mS >> 8) & 0x1F;

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (timeIn5mS & 0xFF);

    /* Restore the line state */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = cadenceState;

    /* Then wait for 100mS for the detector to become stable */
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
    index++;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START + index]
        = 20; /* 5mS per increment */

    index++; /* Adjust one more for length values */
    /*
     * Set the line object cadence variables to this sequence and activate the
     * sequencer
     */
    pLineObj->intSequence[VP_PROFILE_LENGTH] = index + 4;
    pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = index;

    pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
    pLineObj->cadence.length = pLineObj->intSequence[VP_PROFILE_LENGTH];

    pLineObj->cadence.pActiveCadence = &pLineObj->intSequence[0];
    pLineObj->cadence.pCurrentPos =
        &pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START];

    pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
    pLineObj->cadence.status |= VP_CADENCE_STATUS_SENDSIG;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SendCid()
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
Vp880SendCid(
    VpLineCtxType *pLineCtx,        /**< Line to send CID on */
    uint8 length,                   /**< Length of the current message data, not
                                     * to exceed the buffer size
                                     */
    VpProfilePtrType pCidProfile,   /**< CID Profile or Profile index to use */
    uint8p pCidData)                /**< CID Message data */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpProfilePtrType *pProfileTable;
    VpProfilePtrType pCidProfileLocal;

    uint8 primaryByteCount, secondaryByteCount;
    VpDeviceIdType deviceId = pDevObj->deviceId;
 
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
    
    if (length > (2 * VP_SIZEOF_CID_MSG_BUFFER)) {
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
    }

    /* Determine if it's an index or profile */
    if ((cidIndex >= 0) && (cidIndex < VP_CSLAC_CALLERID_PROF_TABLE_SIZE)) {
        /* It's an index. Set the line profile to the device indexed profile */
        if (!(pDevObj->profEntry.cidCadProfEntry & (0x0001 << cidIndex))) {
            /* The profile is invalid -- error. */
            return VP_STATUS_ERR_PROFILE;
        }
        pProfileTable = pDevObj->devProfileTable.pCallerIdProfileTable;
        pCidProfileLocal = pProfileTable[cidIndex];
    } else {
        if (cidIndex > VP_CSLAC_CALLERID_PROF_TABLE_SIZE) {
            return VP_STATUS_INVALID_ARG;
        } else {
            /* 
             * This is a pointer. Make sure it's the correct type, and if so,
             * set it to the line object
             */
            if(VpVerifyProfileType(VP_PROFILE_CID, pCidProfile) == TRUE) {
                pCidProfileLocal = pCidProfile;
            } else {
                return VP_STATUS_ERR_PROFILE;
            }
        }
    }

    if (pCidProfileLocal == VP_PTABLE_NULL) {
        return VP_STATUS_ERR_PROFILE;
    }
    
    /* If we're here, all parameters passed are valid */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pLineObj->pCidProfileType2 = pCidProfileLocal;
    pLineObj->callerId.pCliProfile = pLineObj->pCidProfileType2;
    pLineObj->callerId.cliIndex = 0;
    pLineObj->callerId.cliMPIndex = 0;
    pLineObj->callerId.cliMSIndex = 0;    

    pLineObj->callerId.status |= VP_CID_IN_PROGRESS;
    pLineObj->callerId.status &= ~VP_CID_SIG_B_VALID;
    pLineObj->callerId.status &= ~VP_CID_REPEAT_MSG;
    pLineObj->callerId.status &= ~VP_CID_END_OF_MSG;

    pLineObj->callerId.cliTimer = 1;
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

    pLineObj->callerId.status |= VP_CID_PRIMARY_IN_USE;
    pLineObj->callerId.status &= ~VP_CID_SECONDARY_IN_USE;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ContinueCid()
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
Vp880ContinueCid(
    VpLineCtxType *pLineCtx,    /**< Line to continue CID on */
    uint8 length,               /**< Length of data passed not to exceed the 
                                 * buffer length
                                 */
    uint8p pCidData)            /**< CID message data */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 byteCount = 0;    
    uint8 *pMsgLen;
    uint8 *pBuffer;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    pLineObj->callerId.status &= ~VP_CID_END_OF_MSG;

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

    if (length >  VP_SIZEOF_CID_MSG_BUFFER) {
        return VP_STATUS_INVALID_ARG;
    }

    if (length == 0) {
        return VP_STATUS_SUCCESS;
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
        pMsgLen = &(pLineObj->callerId.primaryMsgLen);
        pBuffer = &(pLineObj->callerId.primaryBuffer[0]);
    } else {
        /* Fill the secondary buffer */
        pLineObj->callerId.status |= VP_CID_SECONDARY_FULL;
        pMsgLen = &(pLineObj->callerId.secondaryMsgLen);        
        pBuffer = &(pLineObj->callerId.secondaryBuffer[0]);
    }

    *pMsgLen = length;

    /* Copy the message data to the API buffer */
    for (byteCount = 0; (byteCount < *pMsgLen); byteCount++) {
        pBuffer[byteCount] = pCidData[byteCount];

        pLineObj->callerId.cidCheckSum += pBuffer[byteCount];
        pLineObj->callerId.cidCheckSum = pLineObj->callerId.cidCheckSum % 256;
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
} /* Vp880ContinueCid() */

/**
 * Vp880CtrlSetCliTone()
 *  This function is called by the API internally to enable or disable the
 * signal generator used for Caller ID.
 *
 * Preconditions:
 *  The line context must be valid (pointing to a Vp880 line object type
 *
 * Postconditions:
 *  The signal generator used for CID tones is enabled/disabled indicated by
 * the mode parameter passed.
 */
VpStatusType
Vp880CtrlSetCliTone(
    VpLineCtxType *pLineCtx,    /**< Line affected by the CLI tones */
    bool mode)                  /**< TRUE = enabled, FALSE = disable tones */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* 
     * This function should only be called when the Caller ID sequence is
     * generating an alerting tone. We're using the C/D generators, so disable
     * A/B and enable C/D only (if mode == TRUE).
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);
    sigGenCtrl &= ~(VP880_GEN_ALLON);
            
    if (mode == TRUE) {
        sigGenCtrl |= (VP880_GENC_EN | VP880_GEND_EN);
    }
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880CtrlSetFSKGen()
 *  This function is called by the CID sequencer executed internally by the API
 *
 * Preconditions:
 *  The line context must be valid (pointing to a VP880 line object type
 *
 * Postconditions:
 *  The data indicated by mode and data is applied to the line. Mode is used
 * to indicate whether the data is "message", or a special character. The 
 * special characters are "channel siezure" (alt. 1/0), "mark" (all 1), or
 * "end of transmission".
 */
void 
Vp880CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,    /**< Line affected by the mode and data */
    VpCidGeneratorControlType mode,      /**< Indicates the type of data being sent. 
                                 * Affects the start and stop bit used
                                 */
    uint8 data)                 /**< 8-bit message data */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 fskParam[VP880_CID_PARAM_LEN];
    uint8 tempFskParam;
    bool moreData = TRUE;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_RD, 
        VP880_CID_PARAM_LEN, fskParam);
    tempFskParam = fskParam[0];

    tempFskParam &= ~(VP880_CID_FRAME_BITS);
    tempFskParam &= ~(VP880_CID_DIS);

    switch(mode) {
        case VP_CID_SIGGEN_EOT:
            Vp880MuteChannel(pLineCtx, FALSE);

            tempFskParam |= VP880_CID_DIS;
            moreData = FALSE;
            break;

        case VP_CID_GENERATOR_DATA:
            Vp880MuteChannel(pLineCtx, TRUE);

            tempFskParam |= (VP880_CID_FB_START_0 | VP880_CID_FB_STOP_1);
            if ((pLineObj->callerId.status & VP_CID_END_OF_MSG) ||
                (pLineObj->callerId.status & VP_CID_MID_CHECKSUM)) {
                tempFskParam |= VP880_CID_EOM;
            } else {
                tempFskParam &= ~(VP880_CID_EOM);
            }
            break;
 
        case VP_CID_GENERATOR_KEYED_CHAR:
            Vp880MuteChannel(pLineCtx, TRUE);

            tempFskParam &= ~(VP880_CID_EOM);
            switch(data) {
                case VP_FSK_CHAN_SEIZURE:
                    tempFskParam |= 
                        (VP880_CID_FB_START_0 | VP880_CID_FB_STOP_1);
                    break;
                    
                case VP_FSK_MARK_SIGNAL:
                    tempFskParam |= 
                        (VP880_CID_FB_START_1 | VP880_CID_FB_STOP_1);
                    break;
            }
            break;

        default:
            break;
    }

    if (tempFskParam != fskParam[0]) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_WRT, 
            VP880_CID_PARAM_LEN, &tempFskParam);
    }

    if (moreData == TRUE) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_DATA_WRT, 
            VP880_CID_DATA_LEN, &data);
    }

    return;
}    

/**
 * Vp880SendDigit()
 *  This function sends a DTMF or Dial Pulse digit on an FXO line. It creates
 * a sequencer compatible profile to control the FXO loop open, loop close, and
 * time operators.
 *
 * Preconditions:
 *  The line must first be initialized and must be of FXO type.
 *
 * Postconditions:
 *  The digit specified is sent on the line in the form specified (DTMF or Dial 
 * Pulse).  This function returns the success code if the line is an FXO type of 
 * line, if the digit is between 0 - 9, and if the digit type is either DTMF or 
 * Dial Pulse.
 */
VpStatusType
Vp880SendDigit(
    VpLineCtxType *pLineCtx,            /**< Line to send a digit on */
    VpDigitGenerationType digitType,    /**< Type of digit to send. May indicate
                                         * DTMF, Dial Pulse, or Hook Flash
                                         */
    VpDigitType digit)                  /**< The digit to send. Used if type of
                                         * digit is DTMF or Dial Pulse
                                         */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint16 tempTime, firstTimer, secondTimer;
    
    uint8 seqByte;
    
    VpDeviceIdType deviceId = pDevObj->deviceId;

    if (!(pLineObj->status & VP880_IS_FXO)) {
        return VP_STATUS_INVALID_ARG;
    }
    
    if (VpIsDigit(digit) == FALSE) {
        return VP_STATUS_INVALID_ARG;
    }

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
        case VP_DIGIT_GENERATION_DIAL_PULSE:
        case VP_DIGIT_GENERATION_DIAL_HOOK_FLASH:
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

    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    switch(digitType) {
        case VP_DIGIT_GENERATION_DTMF:
            Vp880SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

            /* Fixed total length and sequence length for FLASH generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0C;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x08;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_SIGGEN);
                
            pLineObj->intSequence[12]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_SIGGEN);

            pLineObj->intSequence[9] = 
                (VP_SEQ_SIGGEN_A_EN | VP_SEQ_SIGGEN_B_EN);

            pLineObj->intSequence[13] = VP_SEQ_SIGGEN_ALL_DISABLED;

            firstTimer = pLineObj->digitGenStruct.dtmfOnTime;
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((firstTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = ((firstTimer - 2) & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.dtmfOffTime;
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((secondTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = ((secondTimer - 2) & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = 
                VP_PRFWZ_PROFILE_DTMF_DIG_GEN;
            break;

        case VP_DIGIT_GENERATION_DIAL_PULSE:
            /* Fixed total length and sequence length for DP generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x10;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x0C;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
                
            pLineObj->intSequence[12] 
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
                
            if (pLineObj->lineState.currentType == VP_LINE_FXO_TALK) {
                pLineObj->intSequence[9] = 
                    VP_PROFILE_CADENCE_STATE_FXO_OHT;
                pLineObj->intSequence[13] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK;
            } else {
                pLineObj->intSequence[9] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;
                pLineObj->intSequence[13] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE;
            }

            firstTimer = pLineObj->digitGenStruct.breakTime;
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((firstTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = ((firstTimer - 2) & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.makeTime;
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((secondTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = ((secondTimer - 2) & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            firstTimer = pLineObj->digitGenStruct.dpInterDigitTime;
            if (digit > 1) {
                pLineObj->intSequence[16] = VP_SEQ_SPRCMD_BRANCH_INSTRUCTION;
                pLineObj->intSequence[17] = digit - 1;

                pLineObj->intSequence[18] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
                tempTime = ((firstTimer - 2) >> 8) & 0x03;
                pLineObj->intSequence[18] |= tempTime;
                tempTime = ((firstTimer - 2) & 0x00FF);
                pLineObj->intSequence[19] |= tempTime;
            } else {
                pLineObj->intSequence[16] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
                tempTime = ((firstTimer - 2) >> 8) & 0x03;
                pLineObj->intSequence[16] |= tempTime;
                tempTime = ((firstTimer - 2) & 0x00FF);
                pLineObj->intSequence[17] |= tempTime;

                pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0E;
                pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] 
                    = 0x0A;
            }

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = 
                VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN;
            break;

        case VP_DIGIT_GENERATION_DIAL_HOOK_FLASH:
            /* Fixed total length and sequence length for FLASH generation */
            pLineObj->intSequence[VP_PROFILE_LENGTH] = 0x0C;
            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB] = 0x08;

            pLineObj->intSequence[VP_PROFILE_TYPE_SEQUENCER_START]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);
                
            pLineObj->intSequence[12]
                = (VP_SEQ_SPRCMD_COMMAND_INSTRUCTION | VP_SEQ_SUBCMD_LINE_STATE);

            if (pLineObj->lineState.currentType == VP_LINE_FXO_TALK) {
                pLineObj->intSequence[9] = 
                    VP_PROFILE_CADENCE_STATE_FXO_OHT;
                pLineObj->intSequence[13] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_TALK;
            } else {
                pLineObj->intSequence[9] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_OPEN;
                pLineObj->intSequence[13] = 
                    VP_PROFILE_CADENCE_STATE_FXO_LOOP_CLOSE;
            }
            
            firstTimer = pLineObj->digitGenStruct.flashTime;
            pLineObj->intSequence[10] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((firstTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[10] |= tempTime;

            tempTime = ((firstTimer - 2) & 0x00FF);
            pLineObj->intSequence[11] |= tempTime;

            secondTimer = pLineObj->digitGenStruct.dpInterDigitTime;
            pLineObj->intSequence[14] = VP_SEQ_SPRCMD_TIME_INSTRUCTION;
            tempTime = ((secondTimer - 2) >> 8) & 0x03;
            pLineObj->intSequence[14] |= tempTime;

            tempTime = ((secondTimer - 2) & 0x00FF);
            pLineObj->intSequence[15] |= tempTime;

            pLineObj->intSequence[VP_PROFILE_TYPE_LSB] = 
                VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN;
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
 * Vp880SetDTMFGenerators()
 *  This function sets signal generator A/B for DTMF tone generation.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The signal generators A/B are set to the DTMF frequencies and level required
 * by the digit passed.
 */
VpStatusType
Vp880SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode, 
    VpDigitType digit)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl[VP880_GEN_CTRL_LEN];
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 sigByteCount;
    uint8 sigOffset = VP_CID_PROFILE_FSK_PARAM_LEN + 2;

    uint8 sigGenABParams[VP880_SIGAB_PARAMS_LEN] = {
        0x00, 0x00, 0x00,  /* RSVD */
        0x00, 0x00, /* Replace with required column Frequency */
        0x1C, 0x32, /* Level = -10dBm */
        0x00, 0x00, /* Replace with required row Frequency */
        0x1C, 0x32  /* Level = -10dBm */
    };

    uint8 columnFreqs[] = {
        0x0C, 0xE5,    /* 1209Hz (1, 4, 7, *) */
        0x0E, 0x40,    /* 1336Hz (2, 5, 8, 0) */
        0x0F, 0xC1,    /* 1477Hz (3, 6, 9, #) */
        0x11, 0x6B     /* 1633Hz (A, B, C, D) */        
        
    };

    uint8 rowFreqs[] = {
        0x07, 0x6F,    /* 697Hz (1, 2, 3, A) */
        0x08, 0x36,    /* 770Hz (4, 5, 6, B) */
        0x09, 0x16,    /* 852Hz (7, 8, 9, C) */
        0x0A, 0x09     /* 941Hz (*, 0, #, D) */
    };

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* 
     * If we're generating caller ID data set the levels based on the data in 
     * the CID profile
     */
    if ((pLineObj->callerId.status & VP_CID_IN_PROGRESS) && 
        (pLineObj->callerId.pCliProfile != VP_PTABLE_NULL)) {
        for (sigByteCount = 0; sigByteCount < (VP880_SIGAB_PARAMS_LEN - 3); 
             sigByteCount++) {
            sigGenABParams[sigByteCount+3] = 
                pLineObj->callerId.pCliProfile[sigOffset + sigByteCount];
        }
    }

    /* Set the Column Freqs first */
    switch(digit) {
        case 1:
        case 4:
        case 7:
        case VP_DIG_ASTER:
            sigGenABParams[3] = columnFreqs[0];
            sigGenABParams[4] = columnFreqs[1];
            break;

        case 2:
        case 5:
        case 8:
        case VP_DIG_ZERO:
            sigGenABParams[3] = columnFreqs[2];
            sigGenABParams[4] = columnFreqs[3];
            break;

        case 3:
        case 6:
        case 9:
        case VP_DIG_POUND:
            sigGenABParams[3] = columnFreqs[4];
            sigGenABParams[4] = columnFreqs[5];
            break;

        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
            sigGenABParams[3] = columnFreqs[6];
            sigGenABParams[4] = columnFreqs[7];
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
            sigGenABParams[7] = rowFreqs[0];
            sigGenABParams[8] = rowFreqs[1];
            break;

        case 4:
        case 5:
        case 6:
        case VP_DIG_B:
            sigGenABParams[7] = rowFreqs[2];
            sigGenABParams[8] = rowFreqs[3];
            break;

        case 7:
        case 8:
        case 9:
        case VP_DIG_C:
            sigGenABParams[7] = rowFreqs[4];
            sigGenABParams[8] = rowFreqs[5];
            break;

        case VP_DIG_ASTER:
        case VP_DIG_ZERO:
        case VP_DIG_POUND:
        case VP_DIG_D:
            sigGenABParams[7] = rowFreqs[6];
            sigGenABParams[8] = rowFreqs[7];
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT, 
        VP880_SIGAB_PARAMS_LEN, sigGenABParams);
   
    /* 
     * If there is no change to generator control required, it is assumed to be
     * set properly prior to this function call.
     */
    if (mode != VP_CID_NO_CHANGE) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD, 
            VP880_GEN_CTRL_LEN, sigGenCtrl);
        sigGenCtrl[0] = VP880_GEN_ALLOFF;
        /* 
         * For DTMF CID, the data passed may be message data, a keyed character
         * (e.g., Mark, Channel Seizure), or End of Transmission. If it's End
         * of Transmission, disable the DTMF generators immediately. Otherwise,
         * enable the DTMF generators
         */    
        if ((mode == VP_CID_GENERATOR_DATA) 
         || (mode == VP_CID_GENERATOR_KEYED_CHAR)) {
            sigGenCtrl[0] |= (VP880_GENA_EN | VP880_GENB_EN);

            /* Setup the line timer for the on-time for DTMF CID */
            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF] = 
                MS_TO_TICKRATE(VP_CID_DTMF_ON_TIME, 
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF] 
                |= VP_ACTIVATE_LINE_TIMER;
                    
            pLineObj->callerId.dtmfStatus |= VP_CID_ACTIVE_ON_TIME;    
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT, 
            VP880_GEN_CTRL_LEN, sigGenCtrl);
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetOption()
 *  This function determines how to process the Option based on pDevCtx, 
 * pLineCtx, and option type.  The actual options are implemented in 
 * Vp880SetOptionInternal
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
Vp880SetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    uint8 channelId;
    Vp880DeviceObjectType *pDevObj;
    VpStatusType status = VP_STATUS_INVALID_ARG;

    VpDevCtxType *pDevCtxLocal;
    VpLineCtxType *pLineCtxLocal;
    VpDeviceIdType deviceId;

    if (pDevCtx != VP_NULL) {
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;

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

        /* 
         * Valid Device Context, we already know Line context is NULL (higher
         * layer SW, process on device if device option, or process on all lines
         * associated with device if line option
         */
        switch (option) {
            case VP_OPTION_ID_EVENT_MASK:  /* Line and Device */
                Vp880SetOptionInternal(VP_NULL, pDevCtx, option, value);

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
                        status = Vp880SetOptionInternal(pLineCtxLocal, VP_NULL, 
                            option, value);

                        if (status != VP_STATUS_SUCCESS) {
                            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
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
                status = Vp880SetOptionInternal(VP_NULL, pDevCtx, option, value);
                break;
        }
    } else {
        /* 
         * Line context must be valid, device context is NULL, proceed as 
         * normal
         */
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;

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
        status = Vp880SetOptionInternal(pLineCtx, VP_NULL, option, value);
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
}

/**
 * Vp880SetOptionInternal()
 *  This function implements on the Vp880 device the options specified from 
 * Vp880SetOption().  No other function should call this function.
 *
 * Preconditions:
 *  See Vp880SetOption()
 *
 * Postconditions:
 *  See Vp880SetOption()
 */
VpStatusType
Vp880SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    VpDevCtxType *pDevCtxLocal;
    VpLineCtxType *pLineCtxLocal;

    VpStatusType status = VP_STATUS_SUCCESS;
    
    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj;
    uint8 tempData[VP880_INT_MASK_LEN], channelId, txSlot, rxSlot;

    VpDeviceIdType deviceId;

    VpOptionDeviceIoType deviceIo;

    uint8 maxChan, mpiData, mpiByte;
    uint8 ioDirection[2] = {0x00, 0x00};
    uint8 tempSysConfig[VP880_SS_CONFIG_LEN];
    uint8 tempLoopBack[VP880_LOOPBACK_LEN];
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint16 eventMask;

    VpOptionEventMaskType *pEventsMask, *pNewEventsMask;
    Vp880DeviceStateIntType lineIsFxo;

    if (pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;

        /* Adjust the EC value for Wideband mode as needed */
        ecVal[channelId] |=
           ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

        switch (option) {
            /* Line Options */
            case VP_OPTION_ID_PULSE_MODE:
                pLineObj->pulseMode = *((VpOptionPulseModeType *)value);
                VpInitDP(&pLineObj->dpStruct);
                VpInitDP(&pLineObj->dpStruct2);
                break;

            case VP_OPTION_ID_TIMESLOT:
                txSlot = ((VpOptionTimeslotType *)value)->tx;
                rxSlot = ((VpOptionTimeslotType *)value)->rx;
                status = Vp880SetTimeSlot(pLineCtx, txSlot, rxSlot);
                break;

            case VP_OPTION_ID_CODEC:
                status = Vp880SetCodec(pLineCtx, *((VpOptionCodecType *)value));
                break;

            case VP_OPTION_ID_PCM_HWY:
                if (*((VpOptionPcmHwyType *)value) != VP_OPTION_HWY_A) {
                    return VP_STATUS_INVALID_ARG;
                }
                break;

            case VP_OPTION_ID_LOOPBACK:
                /* Timeslot loopback via loopback register */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_RD, 
                    VP880_LOOPBACK_LEN, tempLoopBack);
                
                switch(*((VpOptionLoopbackType *)value)) {
                    case VP_OPTION_LB_TIMESLOT:
                        tempLoopBack[0] |= VP880_INTERFACE_LOOPBACK_EN;
                        break;

                    case VP_OPTION_LB_OFF:
                        tempLoopBack[0] &= ~(VP880_INTERFACE_LOOPBACK_EN);
                        break;

                    case VP_OPTION_LB_DIGITAL:
                    default:
                        return VP_STATUS_INVALID_ARG;
                }

                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_WRT, 
                    VP880_LOOPBACK_LEN, tempLoopBack);
                break;

            case VP_OPTION_ID_LINE_STATE:
                /* 
                 * Only supports one type of battery control, so make sure it
                 * is set correctly. If not, return error otherwise continue
                 */
                if (((VpOptionLineStateType *)value)->bat 
                    != VP_OPTION_BAT_AUTO) {
                    return VP_STATUS_INVALID_ARG;
                }
                
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (((VpOptionLineStateType *)value)->battRev == TRUE) {
                    tempSysConfig[0] &= ~(VP880_SMOOTH_PR_EN);
                } else {
                    tempSysConfig[0] |= VP880_SMOOTH_PR_EN;
                }                   
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                break;

            case VP_OPTION_ID_EVENT_MASK:
                pNewEventsMask = (VpOptionEventMaskType *)value;

                /* Zero out the line-specific bits before setting the
                 * deviceEventsMask in the device object. */
                pEventsMask = &pDevObj->deviceEventsMask;                
                pEventsMask->faults =
                    pNewEventsMask->faults & VP_EVCAT_FAULT_DEV_EVENTS;
                pEventsMask->signaling =
                    pNewEventsMask->signaling & VP_EVCAT_SIGNALING_DEV_EVENTS;
                pEventsMask->response =
                    pNewEventsMask->response & VP_EVCAT_RESPONSE_DEV_EVENTS;
                pEventsMask->test =
                    pNewEventsMask->test & VP_EVCAT_TEST_DEV_EVENTS;
                pEventsMask->process =
                    pNewEventsMask->process & VP_EVCAT_PROCESS_DEV_EVENTS;
                pEventsMask->fxo =
                    pNewEventsMask->fxo & VP_EVCAT_FXO_DEV_EVENTS;
                pEventsMask->packet =
                    pNewEventsMask->packet & VP_EVCAT_PACKET_DEV_EVENTS;

                /* Zero out the device-specific bits before setting the
                 * lineEventsMask in the line object. */
                pEventsMask = &pLineObj->lineEventsMask;
                pEventsMask->faults =
                    pNewEventsMask->faults & ~VP_EVCAT_FAULT_DEV_EVENTS;
                pEventsMask->signaling =
                    pNewEventsMask->signaling & ~VP_EVCAT_SIGNALING_DEV_EVENTS;
                pEventsMask->response =
                    pNewEventsMask->response & ~VP_EVCAT_RESPONSE_DEV_EVENTS;
                pEventsMask->test =
                    pNewEventsMask->test & ~VP_EVCAT_TEST_DEV_EVENTS;
                pEventsMask->process =
                    pNewEventsMask->process & ~VP_EVCAT_PROCESS_DEV_EVENTS;
                pEventsMask->fxo =
                    pNewEventsMask->fxo & ~VP_EVCAT_FXO_DEV_EVENTS;
                pEventsMask->packet =
                    pNewEventsMask->packet & ~VP_EVCAT_PACKET_DEV_EVENTS;

                /* Unmask the unmaskable */
                VpImplementNonMaskEvents(&pLineObj->lineEventsMask, 
                    &pDevObj->deviceEventsMask);

                /* Mask those events that the VP880 API-II cannot generate */    
                Vp880MaskNonSupportedEvents(&pLineObj->lineEventsMask, 
                    &pDevObj->deviceEventsMask);

                /* 
                 * The next code section prevents the device from interrupting
                 * the processor if all of the events associated with the 
                 * specific hardware interrupt are masked
                 */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_INT_MASK_RD, 
                    VP880_INT_MASK_LEN, tempData);

                if (pDevObj->deviceEventsMask.faults & VP_DEV_EVID_CLK_FLT) {
                    tempData[0] &= ~VP880_MASK_CFAIL_INT;
                }
                    
                if (!(pLineObj->status & VP880_IS_FXO)) {  /* Line is FXS */
                    /* Mask off the FXO events */
                    pLineObj->lineEventsMask.fxo |= VP_EVCAT_FXO_MASK_ALL;
                    
                    /* Evaluate for line faults events */
                    eventMask = pLineObj->lineEventsMask.faults;
                    if (eventMask & VP_LINE_EVID_THERM_FLT) {
                        tempData[channelId] |= VP880_MASK_TEMPA1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_TEMPA1_INT;
                    }

                    /* Evaluate for signaling events */
                    eventMask = pLineObj->lineEventsMask.signaling;
                    if ((eventMask & VP_LINE_EVID_HOOK_OFF)
                     && (eventMask & VP_LINE_EVID_HOOK_ON)
                     && (eventMask & VP_LINE_EVID_FLASH)
                     && (eventMask & VP_LINE_EVID_STARTPULSE)
                     && (eventMask & VP_LINE_EVID_PULSE_DIG)) {
                        tempData[channelId] |= VP880_MASK_HOOK1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_HOOK1_INT;
                    }
                    if ((eventMask & VP_LINE_EVID_GKEY_DET)
                     && (eventMask & VP_LINE_EVID_GKEY_REL)) {
                        tempData[channelId] |= VP880_MASK_GKEY1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_GKEY1_INT;
                    }

                    /* Implement Operation Note 8 on errata notice V103 */
                    tempData[channelId] &= ~(VP880_MASK_OCALMY_INT);
                } else {  /* Line is FXO */
                    /* Mask off the FXS events */
                    pLineObj->lineEventsMask.signaling 
                        |= VP880_FXS_SIGNALING_EVENTS;
                
                    /* Evaluate for fxo events */
                    eventMask = pLineObj->lineEventsMask.fxo;
                    if ((eventMask & VP_LINE_EVID_LIU)     
                     && (eventMask & VP_LINE_EVID_LNIU)) {
                        tempData[channelId] |= VP880_MASK_LIU1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_LIU1_INT;
                    }
                    
                    if ((eventMask & VP_LINE_EVID_RING_ON)
                     && (eventMask & VP_LINE_EVID_RING_OFF)) {
                        tempData[channelId] |= VP880_MASK_RGDT1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_RGDT1_INT;
                    }

                    if ((eventMask & VP_LINE_EVID_DISCONNECT)
                     && (eventMask & VP_LINE_EVID_RECONNECT)) {
                        tempData[channelId] |= VP880_MASK_DISC1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_DISC1_INT;
                    }

                    if (eventMask & VP_LINE_EVID_POLREV) {
                        tempData[channelId] |= VP880_MASK_POL1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_POL1_INT;
                    }

                    if ((eventMask & VP_LINE_EVID_FEED_DIS)
                     && (eventMask & VP_LINE_EVID_FEED_EN)) {
                        tempData[channelId] |= VP880_MASK_POH1_INT;
                    } else {
                        tempData[channelId] &= ~VP880_MASK_POH1_INT;
                    }
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_INT_MASK_WRT, 
                    VP880_INT_MASK_LEN, tempData);
                break;

            case VP_OPTION_ID_ZERO_CROSS:
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (*(VpOptionZeroCrossType *)value == VP_OPTION_ZC_NONE) {
                    tempSysConfig[0] |= VP880_ZXR_DIS;
                } else {
                    tempSysConfig[0] &= ~(VP880_ZXR_DIS);
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);

                pLineObj->ringCtrl.zeroCross = *((VpOptionZeroCrossType *)value);
                break;
                            
            case VP_OPTION_ID_RING_CNTRL:
                pLineObj->ringCtrl = *((VpOptionRingControlType *)value);
                
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (pLineObj->ringCtrl.zeroCross == VP_OPTION_ZC_NONE) {
                    tempSysConfig[0] |= VP880_ZXR_DIS;
                } else {
                    tempSysConfig[0] &= ~(VP880_ZXR_DIS);
                }
                   
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT, 
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                break;
                
            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                pLineObj->pcmTxRxCtrl = *((VpOptionPcmTxRxCntrlType *)value);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, 
                    VP880_OP_COND_LEN, &mpiData);
                mpiData &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
                mpiData &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

                Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentType,
                    &mpiByte);
                mpiData |= mpiByte;                    
                VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, 
                    VP880_OP_COND_LEN, &mpiData);
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    } else {
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;
        maxChan = pDevObj->staticInfo.maxChannels;

        for (channelId = 0; channelId < maxChan; channelId++) {
            pLineCtxLocal = pDevCtx->pLineCtx[channelId];
            if (pLineCtxLocal != VP_NULL) {
                pLineObj = pLineCtxLocal->pLineObj;

                /* Adjust the EC value for Wideband mode as needed */
                ecVal[channelId] |=
                   ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
                       VP880_WIDEBAND_MODE : 0);
            }
        }

        switch (option) {
            case VP_DEVICE_OPTION_ID_PULSE:
                pDevObj->pulseSpecs = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_PULSE2:
                pDevObj->pulseSpecs2 = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                pDevObj->criticalFault = *((VpOptionCriticalFltType *)value);

                if ((pDevObj->criticalFault.acFltDiscEn == TRUE)
                 || (pDevObj->criticalFault.dcFltDiscEn == TRUE)) {
                    pDevObj->criticalFault.acFltDiscEn = FALSE;
                    pDevObj->criticalFault.dcFltDiscEn = FALSE;
                    return VP_STATUS_INVALID_ARG;
                }

                if (pDevObj->criticalFault.thermFltDiscEn == TRUE) {
                    for (channelId = 0; channelId < maxChan; channelId++) {
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD, 
                            VP880_SS_CONFIG_LEN, tempSysConfig);
                        
                        tempSysConfig[0] |= VP880_ATFS_EN;
                        VpMpiCmd(deviceId, ecVal[channelId], 
                            VP880_SS_CONFIG_WRT, VP880_SS_CONFIG_LEN, 
                            tempSysConfig);
                    }
                } else {
                    for (channelId = 0; channelId < maxChan; channelId++) {
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD, 
                            VP880_SS_CONFIG_LEN, tempSysConfig);
                        
                        tempSysConfig[0] &= ~(VP880_ATFS_EN);
                        VpMpiCmd(deviceId, ecVal[channelId], 
                            VP880_SS_CONFIG_WRT, VP880_SS_CONFIG_LEN, 
                            tempSysConfig);
                    }
                }
                break;

            case VP_DEVICE_OPTION_ID_DEVICE_IO:
                deviceIo = *(VpOptionDeviceIoType *)(value);

                /* Read the current direction pins */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD, 
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }

                for (channelId = 0; channelId < maxChan; channelId++) {
                    pLineCtxLocal = pDevCtx->pLineCtx[channelId];
                    
                    /* Provide a parameter for the line being FXO type if the
                     * line context does not exist. This is the only way to
                     * know which I/O lines can be controlled on the device
                     * when there are less lines associated with the device
                     * than are possible.
                     */
                    lineIsFxo = (channelId == 0) ?
                        (pDevObj->stateInt & VP880_LINE0_IS_FXO) :
                        (pDevObj->stateInt & VP880_LINE1_IS_FXO);

                    /*
                     * I/O 1 on each channel is special because it uses two
                     * bits in the IO direction register to setup I/O and
                     * Output mode and it's used by some termination types.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O 1 if the line is FXS Isolate,
                         * Splitter, or FXO
                         */
                        if ((pLineObj->termType == VP_TERM_FXS_ISOLATE)
                         || (pLineObj->termType == VP_TERM_FXS_SPLITTER)
                         || (pLineObj->status & VP880_IS_FXO)) {
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO1_MASK;

                            if (deviceIo.directionPins_31_0 
                              & (0x01 << channelId)) {
                                if (deviceIo.outputTypePins_31_0 
                                  & (1 << channelId)) {
                                    ioDirection[channelId] 
                                        |= VP880_IODIR_IO1_OPEN_DRAIN;
                                } else {
                                    ioDirection[channelId] 
                                        |= VP880_IODIR_IO1_OUTPUT;
                                }
                            } else {
                                ioDirection[channelId] 
                                    |= VP880_IODIR_IO1_INPUT;
                            }
                        }
                    } else {
                        /* No line associated. So we can change I/O1 */
                        ioDirection[channelId] &= ~VP880_IODIR_IO1_MASK;

                        if (deviceIo.directionPins_31_0 & (0x01 << channelId)) {
                            if (deviceIo.outputTypePins_31_0 
                                & (1 << channelId)) {
                                ioDirection[channelId] 
                                    |= VP880_IODIR_IO1_OPEN_DRAIN;
                            } else {
                                ioDirection[channelId] 
                                    |= VP880_IODIR_IO1_OUTPUT;
                            }
                        } else {
                            ioDirection[channelId] 
                                |= VP880_IODIR_IO1_INPUT;
                        }
                    }

                    /*
                     * I/O 2 is special because it is used by some termination
                     * types.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O2 if line is FXO and is defined
                         * for use
                         */

                        if ((pLineObj->status & VP880_IS_FXO)
                         && (VP880_FXO_CID_LINE == VP880_IODATA_IO2)) {
                        } else {
                    if (deviceIo.directionPins_31_0 & (4 << channelId)) {
                        ioDirection[channelId] |= VP880_IODIR_IO2_OUTPUT;
                    } else {
                        ioDirection[channelId] &= ~VP880_IODIR_IO2_OUTPUT;
                    }
                }
                    } else {
                        if (deviceIo.directionPins_31_0 & (4 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO2_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO2_OUTPUT;
                        }
                    }

                    /*
                     * I/O 3 is special because it is used by some termination
                     * types and exists only on FXO devices.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O3 if line is FXO and is defined
                         * for use
                         */

                        if ((pLineObj->status & VP880_IS_FXO)
                         && (VP880_FXO_CID_LINE == VP880_IODATA_IO3)) {
                        } else {
                            if (deviceIo.directionPins_31_0 & (16 << channelId)) {
                                ioDirection[channelId] |= VP880_IODIR_IO3_OUTPUT;
                            } else {
                                ioDirection[channelId] &= ~VP880_IODIR_IO3_OUTPUT;
                            }
                        }
                    } else {
                        if (deviceIo.directionPins_31_0 & (16 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO3_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO3_OUTPUT;
                        }
                    }

                    /*
                     * I/O 4 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (64 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO4_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO4_OUTPUT;
                        }
                    }

                    /*
                     * I/O 5 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (256 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO5_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO5_OUTPUT;
                        }
                    }

                    /*
                     * I/O 6 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (1024 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO6_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO6_OUTPUT;
                        }
                    }
                }

                /* Set the current device IO control information */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT, 
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    }

    return status;
}

/**
 * Vp880MaskNonSupportedEvents()
 *  This function masks the events that are not supported by the VP880 API-II. 
 * It should only be called by SetOptionInternal when event masks are being
 * modified.
 *
 * Preconditions:
 *  None. Utility function to modify event structures only.
 *
 * Postconditions:
 *  Event structures passed are modified with masked bits for non-supported
 * VP880 API-II events.
 */
void
Vp880MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask, /**< Line Events Mask to modify for
                                             * non-masking
                                             */
    VpOptionEventMaskType *pDevEventsMask)  /**< Device Events Mask to modify
                                             * for non-masking
                                             */
{
    pLineEventsMask->faults |= VP880_NONSUPPORT_FAULT_EVENTS;
    pLineEventsMask->signaling |= VP880_NONSUPPORT_SIGNALING_EVENTS;
    pLineEventsMask->response |= VP880_NONSUPPORT_RESPONSE_EVENTS;
    pLineEventsMask->test |= VP880_NONSUPPORT_TEST_EVENTS;
    pLineEventsMask->process |= VP880_NONSUPPORT_PROCESS_EVENTS;
    pLineEventsMask->fxo |= VP880_NONSUPPORT_FXO_EVENTS;
    pLineEventsMask->packet |= VP880_NONSUPPORT_PACKET_EVENTS;

    pDevEventsMask->faults |= VP880_NONSUPPORT_FAULT_EVENTS;
    pDevEventsMask->signaling |= VP880_NONSUPPORT_SIGNALING_EVENTS;
    pDevEventsMask->response |= VP880_NONSUPPORT_RESPONSE_EVENTS;
    pDevEventsMask->test |= VP880_NONSUPPORT_TEST_EVENTS;
    pDevEventsMask->process |= VP880_NONSUPPORT_PROCESS_EVENTS;
    pDevEventsMask->fxo |= VP880_NONSUPPORT_FXO_EVENTS;
    pDevEventsMask->packet |= VP880_NONSUPPORT_PACKET_EVENTS;

    return;
}

/**
 * Vp880DeviceIoAccess()
 *  This function is used to access device IO pins of the Vp880. See API-II
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
Vp880DeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;
    
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 chanNum, maxChan;
    uint8 ioDataReg[2] = {0x00, 0x00};  /* IO Status from each channel */
    uint32 bitToDevMap[] = {
        0x00000001, /* IO1 Channel 1 */
        0x00000004, /* IO2 Channel 1 */
        0x00000002, /* IO1 Channel 2 */
        0x00000008, /* IO2 Channel 2 */

        /* These signals only exist if FXO type is detected on device */
        0x00000010, /* IO3 Channel 1 */
        0x00000040, /* IO3 Channel 2 */
        0x00000020, /* IO4 Channel 1 */
        0x00000080, /* IO4 Channel 2 */
        0x00000100, /* IO5 Channel 1 */
        0x00000400, /* IO5 Channel 2 */
        0x00000200, /* IO6 Channel 1 */
        0x00000800  /* IO6 Channel 2 */
    };

    VpDeviceIoAccessDataType *pAccessData = 
        &(pDevObj->getResultsOption.optionData.deviceIoData);
    
    maxChan = pDevObj->staticInfo.maxChannels;

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
    
    /* Read the current state of the IO lines */
    for (chanNum = 0; chanNum < maxChan; chanNum++) {
        pLineCtx = pDevCtx->pLineCtx[chanNum];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[chanNum] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }

        VpMpiCmd(deviceId, ecVal[chanNum], VP880_IODATA_REG_RD,
            VP880_IODATA_REG_LEN, &ioDataReg[chanNum]);
    }

    *pAccessData = *pDeviceIoData;

    /* 
     * If this is a write operation, detect the bits that should be changed
     * and send to appropriate channel
     */
    if (pDeviceIoData->accessType == VP_DEVICE_IO_WRITE) {
        for (chanNum = 0; chanNum < maxChan; chanNum++) {
            pLineCtx = pDevCtx->pLineCtx[chanNum];
            if (pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;

                /* Protect access to I/O1 if FXO or Isolate Type termination */
                if ((pLineObj->status & VP880_IS_FXO)
                 || (pLineObj->termType == VP_TERM_FXS_ISOLATE)) {
                } else {
                    ioDataReg[chanNum] &= ~(VP880_IODATA_IO1 
                        & pDeviceIoData->accessMask_31_0);
                        
                    ioDataReg[chanNum] |= (VP880_IODATA_IO1
                        & pDeviceIoData->accessMask_31_0
                        & pDeviceIoData->deviceIOData_31_0);
                }

                /* Protect access to I/O2 if FXO and being used */
                if ((pLineObj->status & VP880_IS_FXO)
                 && (VP880_FXO_CID_LINE == VP880_IODATA_IO2)) {
                } else {
                    ioDataReg[chanNum] &= ~((VP880_IODATA_IO2 << 1) 
                        & pDeviceIoData->accessMask_31_0);

                    ioDataReg[chanNum] |= ((VP880_IODATA_IO2 << 1)
                        & pDeviceIoData->accessMask_31_0
                        & pDeviceIoData->deviceIOData_31_0);
                }
            } else {
                ioDataReg[chanNum] &= ~(VP880_IODATA_IO1
                    & pDeviceIoData->accessMask_31_0);

                    ioDataReg[chanNum] |= (VP880_IODATA_IO1 
                        & pDeviceIoData->accessMask_31_0
                        & pDeviceIoData->deviceIOData_31_0);
                        
                ioDataReg[chanNum] &= ~((VP880_IODATA_IO2 << 1)
                    & pDeviceIoData->accessMask_31_0);

                    ioDataReg[chanNum] |= ((VP880_IODATA_IO2 << 1) 
                        & pDeviceIoData->accessMask_31_0
                        & pDeviceIoData->deviceIOData_31_0);
                }

            /*
             * Protect access to I/O3 if FXO and being used. Prevent
             * access if the device is not detected as FXO on this line
             * since I/O3 is not available
             */
            if (pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;
                if ((pLineObj->status & VP880_IS_FXO)
                 && (VP880_FXO_CID_LINE == VP880_IODATA_IO3)) {
            } else {
                    if (pDevObj->stateInt &
                        (chanNum == 0 ? VP880_LINE0_IS_FXO : VP880_LINE1_IS_FXO)) {
                        ioDataReg[chanNum] &= ~((VP880_IODATA_IO3 << 1)
                    & pDeviceIoData->accessMask_31_0);
                    
                        ioDataReg[chanNum] |= ((VP880_IODATA_IO3 << 1)
                            & pDeviceIoData->accessMask_31_0
                            & pDeviceIoData->deviceIOData_31_0);
                    }
                }
            } else {
                ioDataReg[chanNum] &= ~((VP880_IODATA_IO3 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO3 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);
            }
                    
            /*
             * All other I/O Access lines are not protected, but require that
             * the line on the device is FXO type
             */
            if (pDevObj->stateInt &
                (chanNum == 0 ? VP880_LINE0_IS_FXO : VP880_LINE1_IS_FXO)) {
                ioDataReg[chanNum] &= ~((VP880_IODATA_IO4 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO4 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);

                ioDataReg[chanNum] &= ~((VP880_IODATA_IO5 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO5 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);

                ioDataReg[chanNum] &= ~((VP880_IODATA_IO6 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO6 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);
            }

            VpMpiCmd(deviceId, ecVal[chanNum], VP880_IODATA_REG_WRT,
                VP880_IODATA_REG_LEN, &ioDataReg[chanNum]);
        }
    } else {
        /* 
         * We're reading values, replace the data passed with the data from
         * the device 
         */

        /* Clear all bits */
        pAccessData->deviceIOData_31_0 = 0;
        pAccessData->deviceIOData_63_32 = 0;
        
        /* Set bits from device content */
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO1) 
            ? bitToDevMap[VP880_CH1_IO1] : 0;       
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO2) 
            ? bitToDevMap[VP880_CH1_IO2] : 0;       

        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO3)
            ? bitToDevMap[VP880_CH1_IO3] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO4)
            ? bitToDevMap[VP880_CH1_IO4] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO5)
            ? bitToDevMap[VP880_CH1_IO5] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO6)
            ? bitToDevMap[VP880_CH1_IO6] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO1) 
            ? bitToDevMap[VP880_CH2_IO1] : 0;       
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO2) 
            ? bitToDevMap[VP880_CH2_IO2] : 0;       

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO3)
            ? bitToDevMap[VP880_CH2_IO3] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO4)
            ? bitToDevMap[VP880_CH2_IO4] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO5)
            ? bitToDevMap[VP880_CH2_IO5] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO6)
            ? bitToDevMap[VP880_CH2_IO6] : 0;
    }

    pDevObj->deviceEvents.response |= VP_DEV_EVID_IO_ACCESS_CMP;
    
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);    
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetCodec()
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
Vp880SetCodec(
    VpLineCtxType *pLineCtx,
    VpOptionCodecType codec)    /* Encoding, as defined by LineCodec typedef */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 codecReg;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Basic error checking */
    if ((codec != VP_OPTION_LINEAR) && (codec != VP_OPTION_ALAW)
     && (codec != VP_OPTION_MLAW) && (codec != VP_OPTION_WIDEBAND)) {
        return VP_STATUS_INVALID_ARG;
    }

    if ((codec == VP_OPTION_WIDEBAND)
     && (!(pDevObj->stateInt & VP880_WIDEBAND))) {
        return VP_STATUS_INVALID_ARG;
    }

    /* 
     * If the code mode is already correct, don't change it -- it's a waste of
     * the MPI resource
     */

    if (codec == pLineObj->codec) {
        return VP_STATUS_SUCCESS;
    }
    
    pLineObj->codec = codec;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Read the current state of the codec register */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CODEC_REG_RD, 
        VP880_CODEC_REG_LEN, &codecReg);

    /* Enable the desired CODEC mode */
    switch(codec) {
        case VP_OPTION_LINEAR:              /*16 bit linear PCM */
        case VP_OPTION_WIDEBAND:    /* Wideband asumes Linear PCM */
            codecReg |= VP880_LINEAR_CODEC;
            break;

        case VP_OPTION_ALAW:                /* A-law PCM */
            codecReg &= ~(VP880_LINEAR_CODEC | VP880_ULAW_CODEC);
            break;

        case VP_OPTION_MLAW:                /* u-law PCM */
            codecReg |= VP880_ULAW_CODEC;
            codecReg &= ~(VP880_LINEAR_CODEC);
            break;

        default:
            /* Cannot reach here.  Error checking at top */
            break;
    } /* Switch */

    VpMpiCmd(deviceId, ecVal[channelId], VP880_CODEC_REG_WRT, 
        VP880_CODEC_REG_LEN, &codecReg);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetTimeSlot()
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
Vp880SetTimeSlot(
    VpLineCtxType *pLineCtx,
    uint8 txSlot,       /**< The TX PCM timeslot */
    uint8 rxSlot)       /**< The RX PCM timeslot */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Validate the tx time slot value */
    if(txSlot >= pDevObj->devProfileData.pcmClkRate/64) {
        return VP_STATUS_INVALID_ARG;
    }
    
    /* Validate the rx time slot value */    
    if(rxSlot >= pDevObj->devProfileData.pcmClkRate/64) {
        return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_TX_TS_WRT, 
        VP880_TX_TS_LEN, &txSlot);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_RX_TS_WRT, 
        VP880_RX_TS_LEN, &rxSlot);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880VirtualISR()
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
Vp880VirtualISR(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

#if defined (VP880_INTERRUPT_LEVTRIG_MODE)
    VpSysDisableInt(deviceId);
#endif
    /* Device Interrupt Received */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevObj->status.state |= VP_DEV_PENDING_INT;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ApiTick()
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
Vp880ApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels; 
    bool tempClkFault, tempBat1Fault, tempBat2Fault;
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
    
#if defined (VP880_INTERRUPT_LEVTRIG_MODE)
    VpSysEnableInt(deviceId);
#endif

    /* Ensure that device is initialized */
    if (!(pDevObj->status.state & VP_DEV_INIT_CMP)) {
        if (Vp880FindSoftwareInterrupts(pDevCtx)) {
            *pEventStatus = TRUE;
        }
        
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        Vp880AbsCalibration(pDevObj);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /* Reset event pointers pointers */
    pDevObj->dynamicInfo.lastChan = 0;

    /* Service API Timers */
    Vp880ServiceTimers(pDevCtx);
    VpServiceSeq(pDevCtx);

    /*
     * Test the interrupt to see if there is a pending interrupt.  If there is,
     * read the interrupt registers (if running in an interrupt driven mode).
     * If running in polled mode, automatically read the interrupt/status 
     * registers.
     */

#if defined (VP880_EFFICIENT_POLLED_MODE)
    /* Poll the PIO-INT line */
    pDevObj->status.state |= 
        (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);  
#elif defined (VP880_SIMPLE_POLLED_MODE)
    pDevObj->status.state |= VP_DEV_PENDING_INT;  
#endif

    /* Check to see if a re-evaluation of the signaling register exists */
    if (pDevObj->stateInt & VP880_FORCE_SIG_READ) {
        pDevObj->status.state |= VP_DEV_PENDING_INT;  
    }

    /* Adjust the EC value for Wideband mode as needed */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
    }

    /* Limit the number of interrupts serviced during one tick */
    pDevObj->status.numIntServiced = pDevObj->devProfileData.maxNumInterrupts;

    /* Service all pending interrupts (up to maxNumInterrupts) */
    while ((pDevObj->status.state & VP_DEV_PENDING_INT) 
        && (pDevObj->status.numIntServiced > 0)) {
        /* Read the Interrupt Status Register */
        if (pDevObj->stateInt & VP880_FORCE_SIG_READ) {
            pDevObj->stateInt &= ~VP880_FORCE_SIG_READ;
        } else {
            VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_UL_SIGREG_RD,
                VP880_UL_SIGREG_LEN, pDevObj->intReg);
        }

        /*******************************************************
         *         HANDLE Clock Fail Events                    *
         *******************************************************/

        /* Get the current status of the fault bit */
        tempClkFault = (pDevObj->intReg[0] & VP880_CFAIL_MASK) ? TRUE : FALSE;
        /* 
         * Compare it with what we already know.  If different, generate 
         * events and update the line status bits
         */
        if(tempClkFault ^ pDevObj->dynamicInfo.clkFault) {
            pDevObj->dynamicInfo.clkFault = tempClkFault;
            pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
        }
        
        /* Get the current status of the first battery fault bit */
        tempBat1Fault = (pDevObj->intReg[0] & VP880_OCALMY_MASK) ? TRUE : FALSE;
        tempBat2Fault = (pDevObj->intReg[0] & VP880_OCALMZ_MASK) ? TRUE : FALSE;        

        /* If line 1 is FXO, the Y supply is ignored */
        pLineCtx = pDevCtx->pLineCtx[0];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                if(tempBat1Fault ^ pDevObj->dynamicInfo.bat1Fault) {
                    pDevObj->dynamicInfo.bat1Fault = tempBat1Fault;
                    pDevObj->deviceEvents.faults |= VP_DEV_EVID_BAT_FLT;
                }
            }
        }
        
        /* If line 2 is FXO, the Z supply is ignored */
        pLineCtx = pDevCtx->pLineCtx[1];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                if(tempBat2Fault ^ pDevObj->dynamicInfo.bat2Fault) {
                    pDevObj->dynamicInfo.bat2Fault = tempBat2Fault;
                    pDevObj->deviceEvents.faults |= VP_DEV_EVID_BAT_FLT;
                }
            }
        }

        /* 
         * Compare it with what we already know.  If different, generate 
         * events and update the line status bits
         */
        Vp880ServiceInterrupts(pDevCtx);

        /* 
         * If level triggered, the interrupt may have been disabled (to prevent
         * a flood of interrupts), so reenable it.
         */
    #if defined (VP880_INTERRUPT_LEVTRIG_MODE)
        VpSysEnableInt(deviceId);
    #endif

        /* Clear the current interrupt indication */
        pDevObj->status.state &= ~(VP_DEV_PENDING_INT);
        pDevObj->status.numIntServiced--;

    /* 
     * If operating in Efficient Polled Mode, check to see if the interrupt 
     * line is still indicating an active interrupt. If in simple polled mode,
     * repeat the loop and service interrupts (if anything is changed).
     */
    #if defined (VP880_EFFICIENT_POLLED_MODE)
        /* Poll the PIO-INT line */
        pDevObj->status.state |= 
            (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);  
    #elif defined (VP880_SIMPLE_POLLED_MODE)
        pDevObj->status.state |= VP_DEV_PENDING_INT;  
    #endif
    }/* End while Interrupts*/

    /* Update the dial pulse handler for lines that are set for pulse decode */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                Vp880ProcessFxsLine(pDevObj, pLineCtx);
            }
            /* Currently no Api Tick based operations for FXO lines */
        }
    }

    /* Collect all event activity and report to the calling function */
    if (Vp880FindSoftwareInterrupts(pDevCtx)) {
        *pEventStatus = TRUE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880AbsCalibration()
 *  This function should is called only by Vp880ApiTick() to run an ABS battery
 * calibrary algorithm. The result is a computed DC offset for each polartiy
 * state.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  Upon completion, the Init Device Complete event is generated and the device
 * is considered initialized. The offset values for each line in both normal and
 * reverse polarity are stored in the device object. Note that the offset is
 * independent of the line object since it is a function only of the physical
 * silicon.
 */
void
Vp880AbsCalibration(
    Vp880DeviceObjectType *pDevObj)
{
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 cmdData[VP880_MAX_MPI_DATA];
    uint8 switcherData[VP880_REGULATOR_CTRL_LEN];
    uint8 firstValue[VP880_DC_CAL_REG_LEN] = {0xF4, 0xE6};
    uint8 lastValue[VP880_DC_CAL_REG_LEN] = {0x00, 0x02};
    uint8 icr2Mods[VP880_ICR2_LEN];

    /*
     * To save on memory, the calibration state in terms of the state machine
     * state, channel, and which polarity is being tested are all stored in
     * a single byte. The mask for each parameter as follows.
     */
#define ABS_DC_CAL_STATE_BITS       0x3F
#define ABS_DC_CAL_CHAN_BITS        0x80
#define ABS_DC_CAL_POLREV_BITS      0x40

    /* State values corresponding to what the state is "doing" */
#define ABS_DC_CAL_INIT_STATE       0
#define ABS_DC_CAL_CONNECT_STATE    1
#define ABS_DC_CAL_MEAS_ADJ_STATE   2
#define ABS_DC_CAL_WAIT_STATE       3

    uint8 channelId = ((pDevObj->calState & ABS_DC_CAL_CHAN_BITS) >> 7);

    /* If set, then running line in reverse polarity */
    uint8 revPolTest = (pDevObj->calState & ABS_DC_CAL_POLREV_BITS);

    uint8 chanNum;

    bool complete = FALSE;

    switch(pDevObj->calState & ABS_DC_CAL_STATE_BITS) {
        case ABS_DC_CAL_INIT_STATE:
            VP_DOUT(DBG_INFO, ("\n\rABS_DC_CAL_INIT_STATE at Time %d\n\r",
                pDevObj->timeStamp));

            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_RD,
                VP880_REGULATOR_CTRL_LEN, pDevObj->cachedSwCtrl);

            /* Steps 1 and first part of 2 */
            switcherData[0] = VP880_SWY_OFF;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, switcherData);

            cmdData[0] = VP880_SS_DISCONNECT;
            for (chanNum = 0; chanNum < maxChan; chanNum++) {
                VpMpiCmd(deviceId, ecVal[chanNum], VP880_SYS_STATE_WRT,
                    VP880_SYS_STATE_LEN, cmdData);

                VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_RD,
                    VP880_ICR2_LEN, icr2Mods);
                icr2Mods[VP880_ICR2_FEED_CTRL_INDEX+1] |= VP880_ICR2_FEED_CTRL;
                icr2Mods[VP880_ICR2_FEED_CTRL_INDEX] &= ~VP880_ICR2_FEED_CTRL;
                VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_WRT,
                    VP880_ICR2_LEN, icr2Mods);
            }

            for (chanNum = 0; chanNum < maxChan; chanNum++) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
                    VP880_DC_CAL_REG_LEN, firstValue);
            }

            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
            pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;
            pDevObj->calState |= 0x80;  /* Start with Channel 2 */
            break;

        case ABS_DC_CAL_CONNECT_STATE:
            VP_DOUT(DBG_INFO, ("\n\rABS_DC_CAL_CONNECT_STATE for Chan %d Polarity %d\n\r",
                channelId, revPolTest));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
                VP880_DC_CAL_REG_LEN, firstValue);

            /* Last part of 2 and Step 3 */
            cmdData[0] = (revPolTest) ?
                VP880_SS_ACTIVE_MID_BAT_PR : VP880_SS_ACTIVE_MID_BAT;

            VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, cmdData);

            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
            pDevObj->calState |= ABS_DC_CAL_WAIT_STATE;
            break;

        case ABS_DC_CAL_WAIT_STATE:
            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
            pDevObj->calState |= ABS_DC_CAL_MEAS_ADJ_STATE;
            break;

        case ABS_DC_CAL_MEAS_ADJ_STATE:
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
                VP880_DC_CAL_REG_LEN, cmdData);

            VP_DOUT(DBG_INFO, ("\n\rABS_DC_CAL_MEAS_ADJ_STATE for Chan %d Polarity %d Value 0x%02X 0x%02X\n\r",
                channelId, revPolTest, cmdData[0], cmdData[1]));

            if (cmdData[VP880_DC_CAL_BLIM_INDEX] & VP880_DC_CAL_BLIM) {
                /* Good. Save this value */
                if (revPolTest) {
                    /* Saving the polarity reversal information */
                    pDevObj->absPolRevCal[channelId] =
                        cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0;

                    VP_DOUT(DBG_INFO, ("\n\rSaving PolRev 0x%02X for Ch %d\n\r",
                        pDevObj->absPolRevCal[channelId], channelId));
                } else {
                    /* Saving the normal polarity information */
                    pDevObj->absNormCal[channelId] =
                        cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0;

                    VP_DOUT(DBG_INFO, ("\n\rSaving Normal 0x%02X for Ch %d\n\r",
                        pDevObj->absNormCal[channelId], channelId));
                }

                /* Determine if there's anything else to do */
                switch (channelId) {
                    case 1:
                        if (revPolTest) {
                            /* Change to channel 1, Normal Polarity */
                            pDevObj->calState |= ABS_DC_CAL_CHAN_BITS;
                            pDevObj->calState &= ~ABS_DC_CAL_POLREV_BITS;
                            pDevObj->calState &= 0x7F;

                            VP_DOUT(DBG_INFO, ("\n\rChanging to Normal Polarity for Channel 1 0x%02X\n\r",
                                pDevObj->calState));
                        } else {
                            /* Change to Reverse Polarity */
                            VP_DOUT(DBG_INFO, ("\n\rChanging to PolRev, Channel %d 0x%02X\n\r",
                                channelId, pDevObj->calState));
                            pDevObj->calState |= ABS_DC_CAL_POLREV_BITS;
                        }

                        /* Repeat with new connections */
                        pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
                        pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;
                        break;

                    case 0:
                        if (revPolTest) {
                            /* Done */
                            VP_DOUT(DBG_INFO, ("\n\rCal Complete For Device %d\n\r", deviceId));
                            complete = TRUE;
                        } else {
                            /* Change to Reverse Polarity */
                            pDevObj->calState |= ABS_DC_CAL_POLREV_BITS;

                            /* Repeat with new connections */
                            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
                            pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;

                            VP_DOUT(DBG_INFO, ("\n\rChanging to PolRev for Channel %d 0x%02X\n\r",
                                channelId, pDevObj->calState));
                        }
                        break;

                    default:
                        /* Error */
                        VP_DOUT(DBG_ERROR, ("\n\rChannelId Error %d\n\r", channelId));
                        complete = TRUE;
                        break;
                }
            } else {
                /* Change the current offset and try again */
                if (cmdData[VP880_DC_CAL_ABS_INDEX] & 0x80) {
                    if ((cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0) == 0x80) {
                        cmdData[VP880_DC_CAL_ABS_INDEX] = 0;
                    } else {
                        cmdData[VP880_DC_CAL_ABS_INDEX] -= 16;
                    }
                } else {
                    if ((cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0) == 0x70) {
                        /*
                         * Something wrong happened. Restore back to 0 and end
                         * algorithm.
                         */
                        complete = TRUE;
                        VP_DOUT(DBG_ERROR, ("\n\rCalibration Algorithm Error 0x%02X\n\r",
                            pDevObj->status.state));
                    } else {
                        cmdData[VP880_DC_CAL_ABS_INDEX] += 16;
                    }
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
                    VP880_DC_CAL_REG_LEN, cmdData);

                VP_DOUT(DBG_INFO, ("\n\rAdjusting Offset 0x%02X\n\r",
                    cmdData[0]));
            }
            break;

        default:
            /* oops. shouldn't be here. */
            VP_DOUT(DBG_ERROR, ("\n\rCalibration Case Error %d \n\r",
                (pDevObj->calState & ABS_DC_CAL_STATE_BITS)));
            complete = TRUE;
            break;
    }

    if (complete == TRUE) {
        /* We're done */
        for (chanNum = 0; chanNum < maxChan; chanNum++) {
            VpMpiCmd(deviceId, ecVal[chanNum], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, cmdData);

            VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_RD,
                VP880_ICR2_LEN, icr2Mods);
            icr2Mods[VP880_ICR2_FEED_CTRL_INDEX+1] &= ~VP880_ICR2_FEED_CTRL;
            VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_WRT,
                VP880_ICR2_LEN, icr2Mods);
        }

        VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_DC_CAL_REG_WRT,
            VP880_DC_CAL_REG_LEN, lastValue);

        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, pDevObj->cachedSwCtrl);

        pDevObj->calState = 0;
        pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_IN_CAL);
    }
}

/**
 * Vp880ProcessFxsLine()
 *  This function should only be called by Vp880ApiTick on FXS lines. It 
 * performs all line processing for operations that are Tick based
 *
 * Preconditions:
 *  Conditions defined by purpose of Api Tick.  
 *
 * Postconditions:
 *  The Api variables and events (as appropriate) for the line passed have been
 * updated.
 */
VpStatusType
Vp880ProcessFxsLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x00, 0x02};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 opNote4Step1Data[VP880_ICR2_LEN];
    uint8 channelId = pLineObj->channelId;
    bool dpStatus[2] = {FALSE, FALSE};
    VpOptionEventMaskType lineEvents1;
    VpOptionEventMaskType lineEvents2;

    lineEvents1.signaling = 0;
    lineEvents2.signaling = 0;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* 
     * If the line is configured for Dial Pulse Detection, run the Dial Pulse
     * detection code. Dial Pulse detection code will generate the appropriate
     * events
     */
    if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_ON) {
        dpStatus[0] = VpUpdateDP(pDevObj->devProfileData.tickRate,
            &pDevObj->pulseSpecs, &pLineObj->dpStruct, &lineEvents1);

        dpStatus[1] = VpUpdateDP(pDevObj->devProfileData.tickRate,
            &pDevObj->pulseSpecs2, &pLineObj->dpStruct2, &lineEvents2);

        /*
         * The state machines will not necessarily complete at the same time, so
         * keep track of each and when both are done, report a passing digit if
         * one exists, or invalid if no criteria was met.
         */
        if (dpStatus[0] == TRUE) {
            pLineObj->status |= VP880_DP_SET1_DONE;
            pLineObj->signaling1 = lineEvents1.signaling;
        }

        if (dpStatus[1] == TRUE) {
            pLineObj->status |= VP880_DP_SET2_DONE;
            pLineObj->signaling2 = lineEvents2.signaling;
        }

        if ((pLineObj->status & (VP880_DP_SET1_DONE | VP880_DP_SET2_DONE)) ==
            (VP880_DP_SET1_DONE | VP880_DP_SET2_DONE)) {
            if (pLineObj->dpStruct.digits != -1) {
                pLineObj->signalingData = pLineObj->dpStruct.digits;
                pLineObj->lineEvents.signaling |= pLineObj->signaling1;
                pLineObj->lineEventHandle = VP_DP_PARAM1;
            } else {
                pLineObj->signalingData = pLineObj->dpStruct2.digits;
                pLineObj->lineEvents.signaling |= pLineObj->signaling2;
                pLineObj->lineEventHandle = VP_DP_PARAM2;
            }

            if (pLineObj->signalingData == 0) {
                pLineObj->signalingData = pLineObj->lineEventHandle;
                pLineObj->lineEventHandle = pDevObj->timeStamp;
            }

            pLineObj->status &= ~(VP880_DP_SET1_DONE | VP880_DP_SET2_DONE);
            pLineObj->signaling1 = 0;
            pLineObj->signaling2 = 0;
        }
    }

    /* If Caller ID sequencer is in progress, update. */
    if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
        VpCidSeq(pLineCtx);
    }

    /* The following code is an ABS workaround for the Clare FXO interface */
    /* onhook tick counter for ABS devices*/
    /* Implementing the BSW ABS Debounce Work around */

    if ((pDevObj->stateInt & VP880_IS_ABS) &&
        (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
        if (-1 != pLineObj->onHookTicks) {
            if (0 == pLineObj->onHookTicks) {
                /* When we're done, restore the ICR6 values */
                VpMpiCmd(deviceId, ecVal[channelId],
                    VP880_ICR6_WRT, VP880_ICR6_LEN, absFix2ndOn);
            }
            
            if ((ON_HOOK_TIMER - ON_HOOK_ABS_CLARE_DELAY) == 
                pLineObj->onHookTicks) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                    VP880_ICR2_LEN, opNote4Step1Data);
                opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX]
                    |= VP880_ICR2_MET_SPEED_CTRL;
                opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX+1]
                    &= ~VP880_ICR2_MET_SPEED_CTRL;
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                    VP880_ICR2_LEN, opNote4Step1Data);
            }
            pLineObj->onHookTicks--;
        }
    }                 

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ServiceInterrupts()
 *  This function should only be called by Vp880ApiTick when an interrupt 
 * occurs.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  The Global Signaling Register is read and the data is stored in the device 
 * object.  Depending on the dial pulse mode option set, the hook event (on/off) 
 * is generated if a hook status changed.  All FXO events are reported by this 
 * function (i.e., no other processing necessary). This function will return 
 * TRUE if an event has been generated.
 */
bool
Vp880ServiceInterrupts(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    
    VpProfilePtrType pCadence;
    uint8 channelId, slacState;
    VpCslacLineCondType tempHookSt, tempGnkSt, tempThermFault;
    VpCslacLineCondType tempRingingSt, tempDiscSt, tempPolRevSt;
    VpLineStateType state;

    uint8 absFix1stOn[VP880_ICR6_LEN] = {0x00, 0x02};
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x08, 0x02};
    uint8 icr2Reg[VP880_ICR2_LEN];
    bool retFlag = FALSE;
    uint8 *pTxBuffer = pDevObj->txBuffer;
    uint8 rcn = pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION];
    uint16 polRevPeriod;

    /* Adjust the EC value for Wideband mode as needed */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
    }

    /* Read this buffer once per tick so it will be refreshed */
    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] > VP880_REV_VC) {
        if (pDevObj->status.numIntServiced ==
            pDevObj->devProfileData.maxNumInterrupts) {
            VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_TX_PCM_BUFF_RD,
                VP880_TX_PCM_BUFF_LEN, pTxBuffer);
        }
    }

    for (channelId = 0;
         channelId < pDevObj->staticInfo.maxChannels;
         channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            state = pLineObj->lineState.currentType;

            if (!(pLineObj->status & VP880_IS_FXO)) {
                /* 
                 * If debouncing for Ring Exit or Caller ID, ignore hook.
                 * Otherwise process.
                 */
                if ((pLineObj->lineTimers.timers.timer[VP_LINE_CID_DEBOUNCE] 
                    & VP_ACTIVATE_LINE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE] 
                    & VP_ACTIVATE_LINE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                    & VP_ACTIVATE_LINE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT]
                    & VP_ACTIVATE_LINE_TIMER)
                 || ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)
                 && (pLineObj->intSequence[VP_PROFILE_TYPE_LSB] == VP_PRFWZ_PROFILE_FWD_DISC_INT))
                 || ((state == VP_LINE_TIP_OPEN) || (state == VP_LINE_DISCONNECT))) {
                    tempHookSt = (pLineObj->lineState.condition & VP_CSLAC_HOOK);
                } else {
                    if (pDevObj->intReg[channelId] & VP880_HOOK1_MASK) {
                        tempHookSt = VP_CSLAC_HOOK;
                    } else {
                        tempHookSt = 0;
                    }
                }

                if (pDevObj->intReg[channelId] & VP880_GNK1_MASK) {
                    tempGnkSt = VP_CSLAC_GKEY;
                } else {
                    tempGnkSt = 0;
                }

                if (pDevObj->intReg[channelId] & VP880_TEMPA1_MASK) {
                    tempThermFault = VP_CSLAC_THERM_FLT;
                } else {
                    tempThermFault = 0;
                }                

                /* If the hook conditions changed, continue line processing */
                if((pLineObj->lineState.condition & VP_CSLAC_HOOK) 
                    != tempHookSt) {
                    /* 
                     * There was a sufficient hook activity to stop the active 
                     * CID -- unless the CID sequence knew this would happen and
                     * set the debounce flag. In which case, let CID continue.
                     */
                    if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
                        if (pLineObj->callerId.status & VP_CID_IS_DEBOUNCE) {
                            /* Hook event is fully debounced and ready to go */
                            pLineObj->callerId.status &= ~VP_CID_IS_DEBOUNCE;
                        } else {
                            VpCliStopCli(pLineCtx);
                            Vp880SetLineTone(pLineCtx, VP_PTABLE_NULL, 
                                VP_PTABLE_NULL, VP_NULL);
                        }
                    }

                    if (tempHookSt) {
                        VP_DOUT(DBG_INFO, ("\n\rOff-Hook on Line %d at Time %d\n\r",
                            channelId, pDevObj->timeStamp));

                        pLineObj->lineState.condition |= VP_CSLAC_HOOK;
                        pLineObj->dpStruct.hookSt = TRUE;
                        pLineObj->dpStruct2.hookSt = TRUE;

                        if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_OFF) {
                            pLineObj->lineEvents.signaling |= 
                                VP_LINE_EVID_HOOK_OFF;
                            pLineObj->lineEventHandle = pDevObj->timeStamp;
                            retFlag = TRUE;
                        }

                        /*
                         * Implement the CLARE ABS Debounce Work around if line
                         * is not under test.
                         */
                        if (((pDevObj->stateInt & VP880_IS_ABS) &&
                             (rcn < VP880_REV_VC))
                         && (Vp880IsChnlUndrTst(pDevObj, channelId) == FALSE)) {
                            /* stopping abs BSW glitch work around */
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                                VP880_ICR2_LEN, icr2Reg);
                            icr2Reg[VP880_ICR2_MET_SPEED_INDEX]
                                &= ~VP880_ICR2_MET_SPEED_CTRL;
                            icr2Reg[VP880_ICR2_MET_SPEED_INDEX+1]
                                &= ~VP880_ICR2_MET_SPEED_CTRL;
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                                VP880_ICR2_LEN, icr2Reg);

                            /* Backdoor command for ABS errata */
                            VpMpiCmd(deviceId, ecVal[channelId],
                                VP880_ICR6_WRT, VP880_ICR6_LEN,
                                absFix2ndOn);
                            pLineObj->onHookTicks = ON_HOOK_TIMER;
                        }
                        VpMpiCmd(deviceId, ecVal[channelId],
                            VP880_SYS_STATE_RD,
                            VP880_SYS_STATE_LEN,
                            &slacState);

                        VP_DOUT(DBG_INFO, ("\n\rLine State is 0x%02X\n\r",
                            slacState));

                        switch(slacState & VP880_SS_LINE_FEED_MASK) {
                            /*
                             * Feed states where the SLIC needs to be put
                             * into low battery mode to prevent thermal
                             * overload.
                             */
                            case (VP880_SS_ACTIVE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_IDLE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_ACTIVE_MID_BAT & VP880_SS_LINE_FEED_MASK):
                                slacState &= ~VP880_SS_LINE_FEED_MASK;
                                slacState |= VP880_SS_ACTIVE;
                                VpMpiCmd(deviceId, ecVal[channelId],
                                    VP880_SYS_STATE_WRT,
                                    VP880_SYS_STATE_LEN,
                                    &slacState);
                                break;

                            default:
                                /*
                                 * Another state that either should not
                                 * cause off-hook detection, or state that
                                 * is handled by API-II functionality (e.g.,
                                 * Ring Trip).
                                 */
                                break;
                        }

                        /* 
                         * If an off-hook is detected when the active cadence
                         * is a Message Waiting Pulse on the line, restore the
                         * line state.
                         */
                        pCadence = pLineObj->cadence.pActiveCadence;
                        if (pCadence != VP_PTABLE_NULL) {
                            if (pCadence[VP_PROFILE_TYPE_LSB] == 
                                VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT) {
                                Vp880SetLineState(pLineCtx, state);
                            }
                        }

                        /* 
                         * If an off-hook is detected during the user set state
                         * of Ringing (incl. ringing and silent interval) while
                         * a test is running, don't allow the api to go to the
                         * ringtrip state
                         */
                        if(Vp880IsChnlUndrTst(pDevObj, channelId) == TRUE) {
                            /* Do not change line state during test */
                        } else {
                        if ((state == VP_LINE_RINGING) 
                         || (state == VP_LINE_RINGING_POLREV)) {
                            Vp880SetLineState(pLineCtx, 
                                pLineObj->ringCtrl.ringTripExitSt);
                        }
                        }
                    } else {
                        VP_DOUT(DBG_INFO, ("\n\rOn-Hook on Line %d at Time %d\n\r",
                            channelId, pDevObj->timeStamp));

                        pLineObj->lineState.condition &= ~(VP_CSLAC_HOOK);
                        pLineObj->dpStruct.hookSt = FALSE;
                        pLineObj->dpStruct2.hookSt = FALSE;

                        if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_OFF) {
                            /* 
                             * If this is the first time after initialization 
                             * that we are checking for on-hook and it is 
                             * on-hook, don't generate an interrupt
                             */
                            if (!(pLineObj->lineState.condition & VP_CSLAC_STATUS_VALID)) {
                                pLineObj->lineEvents.signaling |= 
                                    VP_LINE_EVID_HOOK_ON;
                                pLineObj->lineEventHandle = pDevObj->timeStamp;    
                                retFlag = TRUE;
                            }
                        }

                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
                            VP880_SYS_STATE_LEN, &slacState);

                        /* Implementing the BSW ABS Debounce Work around */
                        if ((pDevObj->stateInt & VP880_IS_ABS) &&
                             (rcn < VP880_REV_VC)) {

                            switch(slacState & VP880_SS_LINE_FEED_MASK) {
                                /* 
                                 * If we are detecting on-hook because we caused
                                 * it, do not implement the errata fix.
                                 */
                                case VP880_SS_TIP_OPEN:
                                case VP880_SS_DISCONNECT:
                                case VP880_SS_SHUTDOWN:
                                    break;

                                default:
                                    /* make sure the line is not being tested*/                                
                                    if(Vp880IsChnlUndrTst(pDevObj,
                                        channelId) == TRUE) {
                                        /* 
                                         * skip the work around if chan is 
                                         * not under test 
                                         */
                                        break;
                                    } else {
                                        /* Backdoor command for ABS errata */
                                        VpMpiCmd(deviceId, ecVal[channelId],
                                            VP880_ICR6_WRT, VP880_ICR6_LEN,
                                            absFix1stOn);
                                        pLineObj->onHookTicks = -1;
                                    }
                                    break;
                            }
                        }

                        switch(slacState & VP880_SS_LINE_FEED_MASK) {
                            /*
                             * Feed states where the SLIC needs to be put
                             * into high battery mode to optimize
                             */
                            case (VP880_SS_ACTIVE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_IDLE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_ACTIVE_MID_BAT & VP880_SS_LINE_FEED_MASK):
                                slacState &= ~VP880_SS_LINE_FEED_MASK;
                                if(pLineObj->lineState.usrCurrent == VP_LINE_STANDBY) {
                                    slacState |= VP880_SS_IDLE;
                                } else {
                                    slacState |= VP880_SS_ACTIVE;
                                }
                                VpMpiCmd(deviceId, ecVal[channelId],
                                    VP880_SYS_STATE_WRT,
                                    VP880_SYS_STATE_LEN,
                                    &slacState);
                                break;

                            default:
                                /*
                                 * Another state that either should not
                                 * cause off-hook detection, or state that
                                 * is handled by API-II functionality (e.g.,
                                 * Ring Trip).
                                 */
                                break;
                        }
                    }
                }

                /* If the gkey conditions changed, continue line processing */
                if((pLineObj->lineState.condition & VP_CSLAC_GKEY) 
                    != tempGnkSt) {

                    if (tempGnkSt) {
                        pLineObj->lineEvents.signaling |= VP_LINE_EVID_GKEY_DET;
                        pLineObj->lineState.condition |= VP_CSLAC_GKEY;                        
                    } else {
                        pLineObj->lineEvents.signaling |= VP_LINE_EVID_GKEY_REL;
                        pLineObj->lineState.condition &= ~(VP_CSLAC_GKEY);                        
                    }
                    retFlag = TRUE;
                }

                if((pLineObj->lineState.condition & VP_CSLAC_THERM_FLT) 
                    != tempThermFault) {
                    
                    pLineObj->lineState.condition &= ~(VP_CSLAC_THERM_FLT);
                    pLineObj->lineState.condition |= tempThermFault;

                    pLineObj->lineEvents.faults |= VP_LINE_EVID_THERM_FLT;
                    retFlag = TRUE;
                }
            } else {
                if ((pLineObj->lineTimers.timers.fxoTimer.lastStateChange
                     < VP_FXO_STATE_CHANGE_DEBOUNCE) ||
                    ((pLineObj->lineTimers.timers.fxoTimer.lastStateChange
                     < VP_FXO_RING_TRIP_DEBOUNCE)
                  && (pLineObj->lineState.condition & VP_CSLAC_RINGING))) {
                    tempPolRevSt =
                        (pLineObj->lineState.condition & VP_CSLAC_POLREV);
                    tempRingingSt =
                        (pLineObj->lineState.condition & VP_CSLAC_RINGING);
                    tempDiscSt =
                        (pLineObj->lineState.condition & VP_CSLAC_DISC);
                } else {
                    if (pDevObj->intReg[channelId] & VP880_DISC1_MASK) {
                        tempDiscSt = VP_CSLAC_DISC;
                    } else {
                        tempDiscSt = 0;
                    }

                    if (pDevObj->intReg[channelId] & VP880_POL1_MASK) {
                        tempPolRevSt = VP_CSLAC_POLREV;
                    } else {
                        tempPolRevSt = 0;
                    }

                    if (tempPolRevSt !=
                        (pLineObj->lineState.condition & VP_CSLAC_POLREV)) {
                        pLineObj->lineState.condition &= ~VP_CSLAC_POLREV;
                        pLineObj->lineState.condition |= tempPolRevSt;
                        pLineObj->lineState.condition &= ~VP_CSLAC_POLREV_REPORTED;

                        /*
                         * Capture the period of the last two pol revs. Used for
                         * Ringing Detection
                         */
                        pLineObj->lineTimers.timers.fxoTimer.prevHighToLowTime =
                            ((pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev
                            + pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev)
                            / 4);

                        pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev =
                            pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev;
                        pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev = 0;
                    }

                    if (pDevObj->intReg[channelId] & VP880_LIU1_MASK) {
                        pLineObj->lineState.condition |= VP_CSLAC_LIU;
                    } else {
                        pLineObj->lineState.condition &= ~VP_CSLAC_LIU;
                        pLineObj->lineTimers.timers.fxoTimer.lastNotLiu =
                            pLineObj->ringDetMax * 2;
                    }
                }

                polRevPeriod =
                    pLineObj->lineTimers.timers.fxoTimer.prevHighToLowTime;

                if ((pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev / 4)
                    < pLineObj->ringDetMax) {
                    if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                        pLineObj->fxoData = VP_POLREV_REVERSE;
                } else {
                        pLineObj->fxoData = VP_POLREV_NORMAL;
                    }
                }

                /* Evaluate the detectors */
                /*
                 * If the LIU Threshold has been exceeded, it's definitely not
                 * PolRev, but may be ringing. If it has been completely
                 * debounced, then Ringing is removed if we previously had
                 * Ringing.
                 */
                if (pLineObj->lineTimers.timers.fxoTimer.lastNotLiu) {
                    /*
                     * The threshold has been exceeded for a period within the
                     * debounce interval. Check on Ringing condition.
                     */

                    if ((polRevPeriod <= pLineObj->ringDetMax)
                     && (polRevPeriod >= pLineObj->ringDetMin)) {
                        tempRingingSt = VP_CSLAC_RINGING;
                    } else if ((pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev / 4)
                        > pLineObj->ringDetMax) {
                        /*
                         * This occurs because we had a recent LIU threshold,
                         * but the frequency is not correct. No action other
                         * than clearing Ringing state is necessary.
                         */
                    tempRingingSt = 0;
                        if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                            pLineObj->fxoData = VP_POLREV_REVERSE;
                        } else {
                            pLineObj->fxoData = VP_POLREV_NORMAL;
                        }
                    } else {
                        /*
                         * This prevents compiler warning because it forces
                         * the value to an initialized state
                         */
                        tempRingingSt =
                            (pLineObj->lineState.condition & VP_CSLAC_RINGING);
                }                
                } else {
                    tempRingingSt = 0;

                    /* We were not ringing, so process for polrev event */
                    if (!(pLineObj->lineState.condition & VP_CSLAC_RINGING)) {
                        /*
                         * Require a 5ms delay (plus LIU 2ms the debounce time)
                         * from previous polrev to occur before allowing it to
                         * be detected as "Not Ringing". This gives time for
                         * most ringing signals to exceed the LIU threshold.
                         */
                        if ((pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev / 4) >= 5) {
                            if (!(pLineObj->lineState.condition & VP_CSLAC_POLREV_REPORTED)) {
                                pLineObj->lineState.condition |= VP_CSLAC_POLREV_REPORTED;

                                /*
                                 * Based on how Ringing behaves, we could get out
                                 * of sync w.r.t., PolRev. So don't report an event
                                 * unless the PolRev changed.
                                 */
                                if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                                    if (pLineObj->fxoData != VP_POLREV_REVERSE) {
                                        pLineObj->fxoData = VP_POLREV_REVERSE;
                                        pLineObj->lineEventHandle = pDevObj->timeStamp;
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                                        retFlag = TRUE;
                                        pLineObj->preRingPolRev = VP_POLREV_REVERSE;
                                    }
                } else {
                                    if (pLineObj->fxoData != VP_POLREV_NORMAL) {
                                        pLineObj->fxoData = VP_POLREV_NORMAL;
                                        pLineObj->lineEventHandle = pDevObj->timeStamp;
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                                        pLineObj->preRingPolRev = VP_POLREV_NORMAL;
                                        retFlag = TRUE;
                                    }
                                }
                            }
                        }
                    }
                }                                

                /*
                 * Our cached state is inconsistent with recently
                 * detected conditions. Generate the event.
                 */
                if((pLineObj->lineState.condition & VP_CSLAC_RINGING) 
                    != tempRingingSt) {
                    pLineObj->lineEventHandle = pDevObj->timeStamp;

                    if (tempRingingSt) {
                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_ON;
                        pLineObj->lineState.condition |= VP_CSLAC_RINGING;
                    } else {
                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_OFF;
                        pLineObj->lineState.condition &= ~(VP_CSLAC_RINGING);

                        if (pLineObj->preRingPolRev != pLineObj->fxoData) {
                            pLineObj->preRingPolRev = pLineObj->fxoData;
                            pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                        }
                    }
                    retFlag = TRUE;
                }

                /* If the feed conditions changed, continue line processing */
                if((pLineObj->lineState.condition & VP_CSLAC_DISC) 
                    != tempDiscSt) {
                    pLineObj->lineEventHandle = pDevObj->timeStamp;

                    switch(pLineObj->lineState.currentType) {
                        case VP_LINE_FXO_TALK:
                        case VP_LINE_FXO_LOOP_CLOSE:
                            if (tempDiscSt) {
                                pLineObj->lineState.condition 
                                    |= VP_CSLAC_DISC;
                                pLineObj->lineEvents.fxo 
                                    |= VP_LINE_EVID_DISCONNECT;
                            } else {
                                pLineObj->lineState.condition 
                                    &= ~(VP_CSLAC_DISC);
                                pLineObj->lineEvents.fxo |= 
                                    VP_LINE_EVID_RECONNECT;
                            }
                            break;

                        default:
                            if (tempDiscSt) {
                                pLineObj->lineState.condition 
                                    |= VP_CSLAC_DISC;
                                pLineObj->lineEvents.fxo 
                                    |= VP_LINE_EVID_FEED_DIS;
                            } else {
                                pLineObj->lineState.condition 
                                    &= ~(VP_CSLAC_DISC);
                                pLineObj->lineEvents.fxo |= 
                                    VP_LINE_EVID_FEED_EN;
                            }                        
                            break;
                    }
                    retFlag = TRUE;
                }                
            }
        }
    }

    return retFlag;
}

/**
 * Vp880IsChnlUndrTst()
 *  This function determines if a particular line of a device is currently
 * running a test.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Device not affected. Return value TRUE if the line is currently running a
 * test, FALSE otherwise.
 */
static bool
Vp880IsChnlUndrTst(
    Vp880DeviceObjectType *pDevObj,
    uint8 channelId)
{
#ifdef VP880_INCLUDE_TESTLINE_CODE
    if ((TRUE == pDevObj->currentTest.prepared) &&
        (channelId == pDevObj->currentTest.channelId)) {
        return TRUE;
    }
#endif
    return FALSE;
}

/**
 * Vp880LowLevelCmd()
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
Vp880LowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    int i;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
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

#endif








