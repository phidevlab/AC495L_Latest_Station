/** \file apiquery.c
 * apiquery.c
 *
 *  This file contains the query functions used in the Vp880 device API.
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

/* Project Includes */
#include "vp_api_types.h"
#include "sys_service.h"
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"

#ifdef VP880_INCLUDE_TESTLINE_CODE
#include "vp_api_test.h"
#endif

/* Private Functions */
static uint16 Vp880CheckLineEvent(uint16 event, uint16 eventMask,
    VpEventCategoryType eventCat, Vp880LineObjectType *pLineObj);
static uint16 Vp880CheckDevEvent(uint16 event, uint16 eventMask,
    VpEventCategoryType eventCat, Vp880DeviceObjectType *pDevObj);

/**
 * Vp880FindSoftwareInterrupts()
 *  This function checks for active non-masked device and line events.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Returns true if there is an active, non-masked event on either the device
 * or on a line associated with the device.
 */
bool
Vp880FindSoftwareInterrupts(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpLineCtxType *pLineCtx;
    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

    VpOptionEventMaskType eventsMask = pDevObj->deviceEventsMask;
    VpOptionEventMaskType *pEvents = &(pDevObj->deviceEvents);

    /* First clear all device events that are masked */
    pEvents->faults &= ~(eventsMask.faults);
    pEvents->signaling &= ~(eventsMask.signaling);
    pEvents->response &= ~(eventsMask.response);
    pEvents->process &= ~(eventsMask.process);
    pEvents->test &= ~(eventsMask.test);
    pEvents->fxo &= ~(eventsMask.fxo);

    /* Evaluate if any events remain */
    if((pEvents->faults) || (pEvents->signaling) || (pEvents->response) 
    || (pEvents->process) || (pEvents->test) || (pEvents->fxo)) {
        return TRUE;
    }

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            eventsMask = pLineObj->lineEventsMask;
            pEvents = &(pLineObj->lineEvents);

            /* Clear the line events that are masked */
            pEvents->faults &= ~(eventsMask.faults);
            pEvents->signaling &= ~(eventsMask.signaling);
            pEvents->response &= ~(eventsMask.response);
            pEvents->process &= ~(eventsMask.process);
            pEvents->test &= ~(eventsMask.test);
            pEvents->fxo &= ~(eventsMask.fxo);

            /* Evaluate if any events remain */
            if(pEvents->faults || pEvents->signaling || pEvents->response 
            || pEvents->process || pEvents->test || pEvents->fxo) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
 * Vp880GetEvent()
 *  This function reports new events that occured on the device. This function
 * returns one event for each call to it. It should be called repeatedly until
 * no more events are reported for a specific device.  This function does not
 * access the device, it returns status from the phantom registers that are
 * maintained by the API tick routine.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  Returns true if there is an active event for the device.
 */
bool
Vp880GetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent)    /**< Pointer to the results event structure */
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpLineCtxType *pLineCtx;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 i, eventCatLoop;
    uint8 chan;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

#ifdef VP880_INCLUDE_TESTLINE_CODE
#define EVENT_ARRAY_SIZE 6
#else
#define EVENT_ARRAY_SIZE 5
#endif

    uint16 eventArray[EVENT_ARRAY_SIZE];
    uint16 eventMaskArray[EVENT_ARRAY_SIZE];
    VpEventCategoryType eventCat[] = {
        VP_EVCAT_FAULT,
        VP_EVCAT_SIGNALING,
        VP_EVCAT_RESPONSE,
        VP_EVCAT_PROCESS,
        VP_EVCAT_FXO,
        VP_EVCAT_TEST
    };

    pEvent->status = VP_STATUS_SUCCESS;
    pEvent->hasResults = FALSE;

    /* Initialize the arrays for device events */
    for (i = 0; i < EVENT_ARRAY_SIZE; i++) {
        switch(eventCat[i]) {
            case VP_EVCAT_FAULT:
                eventArray[i] = pDevObj->deviceEvents.faults;
                eventMaskArray[i] = pDevObj->deviceEventsMask.faults;
                break;

            case VP_EVCAT_SIGNALING:
                eventArray[i] = pDevObj->deviceEvents.signaling;
                eventMaskArray[i] = pDevObj->deviceEventsMask.signaling;
                break;

            case VP_EVCAT_RESPONSE:
                eventArray[i] = pDevObj->deviceEvents.response;
                eventMaskArray[i] = pDevObj->deviceEventsMask.response;
                break;

            case VP_EVCAT_PROCESS:
                eventArray[i] = pDevObj->deviceEvents.process;
                eventMaskArray[i] = pDevObj->deviceEventsMask.process;
                break;

            case VP_EVCAT_FXO:
                eventArray[i] = pDevObj->deviceEvents.fxo;
                eventMaskArray[i] = pDevObj->deviceEventsMask.fxo;
                break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
            case VP_EVCAT_TEST:
                eventArray[i] = pDevObj->deviceEvents.test;
                eventMaskArray[i] = pDevObj->deviceEventsMask.test;
                break;
#endif

            default:
                /* This can only occur if there's a bug in this code */
                break;
        }
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Look for active device events first */
    for (eventCatLoop = 0; eventCatLoop < EVENT_ARRAY_SIZE; eventCatLoop++) {
        pEvent->eventId = Vp880CheckDevEvent(eventArray[eventCatLoop],
            eventMaskArray[eventCatLoop], eventCat[eventCatLoop], pDevObj);
        if (pEvent->eventId != 0x0000) {
            pEvent->deviceId = deviceId;
            pEvent->channelId = 0;
            pEvent->eventCategory = eventCat[eventCatLoop];
            pEvent->pDevCtx = pDevCtx;
            pEvent->pLineCtx = VP_NULL;
            pEvent->parmHandle = pDevObj->eventHandle;
            pEvent->hasResults = FALSE;

            if (pEvent->eventCategory == VP_EVCAT_RESPONSE) {
                /*
                 * For the events that require a read operation, set the has
                 * results indicator in the event structure
                 */
                 
                switch (pEvent->eventId) {
                    case VP_LINE_EVID_RD_OPTION:
                        pEvent->channelId = pDevObj->getResultsOption.chanId;
                        pEvent->pLineCtx = pDevCtx->pLineCtx[pEvent->channelId];
                        pEvent->hasResults = TRUE;
                        pEvent->eventData = pDevObj->getResultsOption.optionType;
                        break;

                    case VP_DEV_EVID_IO_ACCESS_CMP:                    
                        pEvent->hasResults = TRUE;
                        break;

                    case VP_DEV_EVID_DEV_INIT_CMP:
                        pEvent->eventData = 1;
                        break;

                    default:
                        break;
                }
            }

            if (pEvent->eventCategory == VP_EVCAT_FAULT) {
                switch(pEvent->eventId) {
                    case VP_DEV_EVID_CLK_FLT:
                        pEvent->eventData =
                            (pDevObj->dynamicInfo.clkFault ? TRUE : FALSE);
                        break;

                    case VP_DEV_EVID_BAT_FLT:
                        if ((pDevObj->dynamicInfo.bat1Fault == TRUE)
                         || (pDevObj->dynamicInfo.bat2Fault == TRUE)
                         || (pDevObj->dynamicInfo.bat3Fault == TRUE)) {
                            pEvent->eventData = TRUE;
                        } else {
                            pEvent->eventData = FALSE;
                        }
                        break;

                    default:
                        break;
                }
            }
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return TRUE;
        }
    }

    /*
     * No device events, now look for Line events -- but make sure the line
     * context is valid before looking for a line object
     */
    for(chan = pDevObj->dynamicInfo.lastChan; chan < maxChan; chan++) {
        pLineCtx = pDevCtx->pLineCtx[chan];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            /* The line context is valid, create a line object and initialize
             * the event arrays for this line
             */
            for (i = 0; i < EVENT_ARRAY_SIZE; i++) {
                switch(eventCat[i]) {
                    case VP_EVCAT_FAULT:
                        eventArray[i] = pLineObj->lineEvents.faults;
                        eventMaskArray[i] = pLineObj->lineEventsMask.faults;
                        break;

                    case VP_EVCAT_SIGNALING:
                        eventArray[i] = pLineObj->lineEvents.signaling;
                        eventMaskArray[i] = pLineObj->lineEventsMask.signaling;
                        break;

                    case VP_EVCAT_RESPONSE:
                        eventArray[i] = pLineObj->lineEvents.response;
                        eventMaskArray[i] = pLineObj->lineEventsMask.response;
                        break;

                    case VP_EVCAT_PROCESS:
                        eventArray[i] = pLineObj->lineEvents.process;
                        eventMaskArray[i] = pLineObj->lineEventsMask.process;
                        break;

                    case VP_EVCAT_FXO:
                        eventArray[i] = pLineObj->lineEvents.fxo;
                        eventMaskArray[i] = pLineObj->lineEventsMask.fxo;
                        break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                    case VP_EVCAT_TEST:                        
                        eventArray[i] = pLineObj->lineEvents.test;
                        eventMaskArray[i] = pLineObj->lineEventsMask.test;                        
                        break;
#endif
                    default:
                        /* This can only occur if there's a bug in this code */
                        break;

                }
            }

            /* Check this line events */
            for (eventCatLoop = 0;
                 eventCatLoop < EVENT_ARRAY_SIZE;
                 eventCatLoop++) {
                pEvent->eventId = Vp880CheckLineEvent(eventArray[eventCatLoop],
                    eventMaskArray[eventCatLoop], eventCat[eventCatLoop],
                    pLineObj);

                if (pEvent->eventId != 0x0000) {
                    pEvent->deviceId = deviceId;
                    pEvent->channelId = chan;
                    pEvent->pLineCtx = pDevCtx->pLineCtx[chan];
                    pEvent->pDevCtx = pDevCtx;
                    pEvent->eventCategory = eventCat[eventCatLoop];
                    pEvent->parmHandle = pLineObj->lineEventHandle;
                    pEvent->lineId = pLineObj->lineId;

                    switch(pEvent->eventCategory) {
                        case VP_EVCAT_RESPONSE:
                            switch(pEvent->eventId) {
                                case VP_LINE_EVID_LLCMD_RX_CMP:
                                case VP_LINE_EVID_GAIN_CMP:
                                case VP_LINE_EVID_RD_LOOP:
                                    pEvent->hasResults = TRUE;
                                    break;
                            }
                            break;

                        case VP_EVCAT_SIGNALING:
                            if (pEvent->eventId == VP_LINE_EVID_DTMF_DIG) {
                                /* 
                                 * Upper bits are used for the timestamp.
                                 * Lower bits are used for the digit and the
                                 * make/break bit.
                                 */
                                pEvent->eventData = (pDevObj->timeStamp << 5)
                                    | pLineObj->dtmfDigitSense;
                            } else {
                                pEvent->eventData = pLineObj->signalingData;
                            }
                            break;
                            
                        case VP_EVCAT_FXO:
                            pEvent->eventData = pLineObj->fxoData;
                            break;

                        case VP_EVCAT_PROCESS:
                            pEvent->eventData = pLineObj->processData;
                            break;

                        case VP_EVCAT_FAULT:
                            if (pEvent->eventId == VP_LINE_EVID_THERM_FLT) {
                                pEvent->eventData =
                                    (pLineObj->lineState.condition
                                        & VP_CSLAC_THERM_FLT) ? TRUE : FALSE;
                            }
                            break;
                        
                        case VP_EVCAT_TEST:                            
                            if ( VP_LINE_EVID_TEST_CMP == pEvent->eventId) {
                                pEvent->eventData = pDevObj->testResults.testId;
                                pEvent->hasResults = TRUE;    
                            }
                            break;

                        default:
                            break;
                    }

                    /*
                     * We're returning, so update the device last channel that
                     * was checked so we start at the next channel
                     */
                    pDevObj->dynamicInfo.lastChan = chan + 1;
                    if (pDevObj->dynamicInfo.lastChan >= maxChan) {
                        pDevObj->dynamicInfo.lastChan = 0;
                    }
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return TRUE;
                }
            }
        }
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return FALSE;
} /* End Vp880GetEvent */

/**
 * Vp880CheckDevEvent()
 *  This function performs a check on active device events and compares the
 * event with the event mask.  The event is cleared, and if the event is
 * unmasked it gets returned to the calling function via the return value.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *  If the returned value is other than 0x0000, the event being returned is
 * cleared in the device object.
 */
uint16
Vp880CheckDevEvent(
    uint16 event,
    uint16 eventMask,
    VpEventCategoryType eventCat,
    Vp880DeviceObjectType *pDevObj)
{
    uint8 i;
    uint16 mask;

    for (i = 0, mask = 0x0001; i < 16; i++, (mask = mask << 1)) {
        /* Check to see if an event MAY be reported */
        if ((mask & event) != 0) {
            /*
             * Have to clear the device event so we don't report this event
             * again
             */
            switch(eventCat) {
                case VP_EVCAT_FAULT:
                    pDevObj->deviceEvents.faults &= (~mask);
                    break;

                case VP_EVCAT_SIGNALING:
                    pDevObj->deviceEvents.signaling &= (~mask);
                    break;

                case VP_EVCAT_RESPONSE:
                    pDevObj->deviceEvents.response &= (~mask);
                    break;

                case VP_EVCAT_PROCESS:
                    pDevObj->deviceEvents.process &= (~mask);
                    break;

                case VP_EVCAT_FXO:
                    pDevObj->deviceEvents.fxo &= (~mask);
                    break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                case VP_EVCAT_TEST:
                    pDevObj->deviceEvents.test &= (~mask);                    
                    break;
#endif
                default:
                    break;
            }

            /* If the event is not masked, return the event */
            if ((mask & eventMask) == 0) {
                return mask;
            }
        }
    }

    return 0x0000;
}

/**
 * Vp880CheckLineEvent()
 *  This function performs a check on active line events and compares the
 * event with the event mask.  The event is cleared, and if the event is
 * unmasked it gets returned to the calling function via the return value.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *  If the returned value is other than 0x0000, the event being returned is
 * cleared in the line object.
 */
uint16
Vp880CheckLineEvent(
    uint16 event,
    uint16 eventMask,
    VpEventCategoryType eventCat,
    Vp880LineObjectType *pLineObj)
{
    uint8 i;
    uint16 mask;

    for (i = 0, mask = 0x0001; i < 16; i++, (mask = mask << 1)) {
        /* Check to see if an event MAY be reported */
        if ((mask & event) != 0) {
            /*
             * Have to clear the line event so we don't report this event
             * again
             */
            switch(eventCat) {
                case VP_EVCAT_FAULT:
                    pLineObj->lineEvents.faults &= (~mask);
                    break;

                case VP_EVCAT_SIGNALING:
                    pLineObj->lineEvents.signaling &= (~mask);
                    break;

                case VP_EVCAT_RESPONSE:
                    pLineObj->lineEvents.response &= (~mask);
                    break;

                case VP_EVCAT_PROCESS:
                    pLineObj->lineEvents.process &= (~mask);
                    break;

                case VP_EVCAT_FXO:
                    pLineObj->lineEvents.fxo &= (~mask);
                    break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
               case VP_EVCAT_TEST:
                    pLineObj->lineEvents.test &= (~mask);
                    break;
#endif

                default:
                    break;
            }

            /* If the event is not masked, return the event */
            if ((mask & eventMask) == 0) {
                return mask;
            }
        }
    }
    return 0x0000;
}

/**
 * Vp880GetOption()
 *  This function accesses the option being requested, fills the device object
 * with the data to be returned, and sets the Read Option complete event.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  The device object is filled with the results of the option type being
 * requested and the Read Option Event flag is set.  This function returns the
 * success code if the option type being requested is supported.
 */
VpStatusType
Vp880GetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle)
{
    VpDevCtxType *pDevCtxLocal;
    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj;
    VpStatusType status = VP_STATUS_SUCCESS;
    VpGetResultsOptionsDataType *pOptionData;

    uint8 channelId, txSlot, rxSlot;
    VpDeviceIdType deviceId;
    uint8 tempLoopBack[VP880_LOOPBACK_LEN];
    uint8 tempSysConfig, maxChan;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 ioDirection[2] = {0x00, 0x00};

    VpLineCtxType *pLineCtxLocal;

    if (pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;
        pOptionData = &(pDevObj->getResultsOption.optionData);

        /* Adjust the EC value for Wideband mode as needed */
        ecVal[channelId] |=
           ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

        if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
            return VP_STATUS_DEVICE_BUSY;
        }

        /*
         * If this function can be executed, we will either access the MPI
         * and/or shared data. So it is best to label the entire function as
         * Code Critical so the data being accessed cannot be changed while
         * trying to be accessed
         */
        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

        pDevObj->getResultsOption.chanId = channelId;

        switch (option) {
            /* Line Options */
            case VP_OPTION_ID_PULSE_MODE:
                pOptionData->pulseModeOption = pLineObj->pulseMode;
                break;

            case VP_OPTION_ID_TIMESLOT:
                VpMpiCmd(deviceId, ecVal[channelId], VP880_TX_TS_RD,
                    VP880_TX_TS_LEN, &txSlot);

                VpMpiCmd(deviceId, ecVal[channelId], VP880_RX_TS_RD,
                    VP880_RX_TS_LEN, &rxSlot);

                pOptionData->timeSlotOption.tx = (txSlot & VP880_TX_TS_MASK);

                pOptionData->timeSlotOption.rx = (rxSlot & VP880_RX_TS_MASK);
                break;

            case VP_OPTION_ID_CODEC:
                pOptionData->codecOption = pLineObj->codec;
                break;

            case VP_OPTION_ID_PCM_HWY:
                pOptionData->pcmHwyOption = VP_OPTION_HWY_A;
                break;

            case VP_OPTION_ID_LOOPBACK:
                /* Timeslot loopback via loopback register */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_RD,
                    VP880_LOOPBACK_LEN, tempLoopBack);

                if ((tempLoopBack[0] & VP880_INTERFACE_LOOPBACK_EN) ==
                     VP880_INTERFACE_LOOPBACK_EN) {
                    pOptionData->loopBackOption = VP_OPTION_LB_TIMESLOT;
                } else {
                    pOptionData->loopBackOption = VP_OPTION_LB_OFF;
                }
                break;

            case VP_OPTION_ID_LINE_STATE:
                /* Battery control is automatic, so force it */
                pOptionData->lineStateOption.bat = VP_OPTION_BAT_AUTO;

                /* Smooth/Abrupt PolRev is controlled in the device */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, &tempSysConfig);

                if (tempSysConfig & VP880_SMOOTH_PR_EN) {
                    pOptionData->lineStateOption.battRev = FALSE;
                } else {
                    pOptionData->lineStateOption.battRev = TRUE;
                }
                break;

            case VP_OPTION_ID_EVENT_MASK:
                /*
                 * In SetOption(), we force all line-specific bits in the
                 * deviceEventsMask to zero.  Likewise, we force all device-
                 * specific bits in the lineEventsMask to zero.  This allows
                 * us to simply OR the two together here.
                 */
                pOptionData->eventMaskOption.faults =
                    pLineObj->lineEventsMask.faults |
                    pDevObj->deviceEventsMask.faults;
                pOptionData->eventMaskOption.signaling =
                    pLineObj->lineEventsMask.signaling |
                    pDevObj->deviceEventsMask.signaling;
                pOptionData->eventMaskOption.response =
                    pLineObj->lineEventsMask.response |
                    pDevObj->deviceEventsMask.response;
                pOptionData->eventMaskOption.test =
                    pLineObj->lineEventsMask.test |
                    pDevObj->deviceEventsMask.test;
                pOptionData->eventMaskOption.process =
                    pLineObj->lineEventsMask.process |
                    pDevObj->deviceEventsMask.process;
                pOptionData->eventMaskOption.fxo =
                    pLineObj->lineEventsMask.fxo |
                    pDevObj->deviceEventsMask.fxo;
                pOptionData->eventMaskOption.packet =
                    pLineObj->lineEventsMask.packet |
                    pDevObj->deviceEventsMask.packet;
                break;

            case VP_OPTION_ID_ZERO_CROSS:
                pOptionData->zeroCross = pLineObj->ringCtrl.zeroCross;
                break;

            case VP_OPTION_ID_RING_CNTRL:
                pOptionData->ringControlOption = pLineObj->ringCtrl;
                break;

            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                pOptionData->pcmTxRxCtrl = pLineObj->pcmTxRxCtrl;
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    } else {
        /*
         * Upper layer checks to be sure that either device context or line
         * context pointers are not null -- so the device context is not null
         * in this case.
         */
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;
        pOptionData = &(pDevObj->getResultsOption.optionData);
        maxChan = pDevObj->staticInfo.maxChannels;

        if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
            return VP_STATUS_DEVICE_BUSY;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

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
                pOptionData->pulseTypeOption = pDevObj->pulseSpecs;
                break;

            case VP_DEVICE_OPTION_ID_PULSE2:
                pOptionData->pulseTypeOption = pDevObj->pulseSpecs2;
                break;

            case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                pOptionData->criticalFaultOption = pDevObj->criticalFault;
                break;

            case VP_OPTION_ID_EVENT_MASK:
                pOptionData->eventMaskOption = pDevObj->deviceEventsMask;
                break;

            case VP_DEVICE_OPTION_ID_DEVICE_IO:
                /* Get the current device IO control information */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }

                pOptionData->deviceIo.outputTypePins_63_32 = 0x00000000;
                pOptionData->deviceIo.directionPins_63_32 = 0x00000000;

                /*
                 * I/O 1 on each channel is special because it uses two bits
                 * in the IO direction register to setup I/O and Output mode
                 */

                if (ioDirection[0] & VP880_IODIR_IO1_OPEN_DRAIN) {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000001;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000001;
                } else if (ioDirection[0] & VP880_IODIR_IO1_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000000;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000001;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000000;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000000;
                }

                /* I/O 2 on each channel can only be either input or output */
                if (ioDirection[0] & VP880_IODIR_IO2_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000004);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000004;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000004);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000004);
                }

                if (ioDirection[1] & VP880_IODIR_IO1_OPEN_DRAIN) {
                    pOptionData->deviceIo.outputTypePins_31_0 |= 0x00000002;
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000002;
                } else if (ioDirection[1] & VP880_IODIR_IO1_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000002);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000002;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000002);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000002);
                }

                if (ioDirection[1] & VP880_IODIR_IO2_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000008);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000008;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000008);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000008);
                }
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    }

    if (status == VP_STATUS_SUCCESS) {
        pDevObj->getResultsOption.optionType = option;
        pDevObj->deviceEvents.response |= VP_LINE_EVID_RD_OPTION;
        pDevObj->eventHandle = handle;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880GetDeviceStatus()
 *  This function returns the status of all lines on a device for the type being
 * requested.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  The location pointed to by the uint32 pointer passed is set (on a per line
 * basis) to either '1' if the status if TRUE on the given line, or '0' if the
 * status is FALSE on the given line for the status being requested.
 */
VpStatusType
Vp880GetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 channelId;
    bool status;
    VpLineCtxType *pLineCtx;

    *pDeviceStatus = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if(pLineCtx != VP_NULL) {
            VpCSLACGetLineStatus(pLineCtx, input, &status);
        }
        *pDeviceStatus |= (((status == TRUE) ? 1 : 0) << channelId);
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880FlushEvents()
 *  This function clears out all events on the device and all events on all
 * lines associated with the device passed.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  All active device events are cleared, and all active line events associated
 * with this device are cleared.
 */
VpStatusType
Vp880FlushEvents(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;
    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pDevObj->deviceEvents.faults = 0;
    pDevObj->deviceEvents.signaling = 0;
    pDevObj->deviceEvents.response = 0;
    pDevObj->deviceEvents.test = 0;
    pDevObj->deviceEvents.process = 0;
    pDevObj->deviceEvents.fxo = 0;
    pDevObj->deviceEvents.packet = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            pLineObj->lineEvents.faults = 0;
            pLineObj->lineEvents.signaling = 0;
            pLineObj->lineEvents.response = 0;
            pLineObj->lineEvents.test = 0;
            pLineObj->lineEvents.process = 0;
            pLineObj->lineEvents.fxo = 0;
            pLineObj->lineEvents.packet = 0;
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880GetResults()
 *  This function fills the results structure passed with the results data found
 * from the event that caused new results.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  If the event structure passed provides the event catagory and ID for a valid
 * results type to read, then the structure passed is filled with the results
 * data.  This function returns the success code if the event catagory and ID is
 * supported by the device.
 */
VpStatusType
Vp880GetResults(
    VpEventType *pEvent,
    void *pResults)
{
    VpDevCtxType *pDevCtx = pEvent->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;
    uint8 commandByte;
    uint8 *pMpiData;
    uint8 mpiDataLen = pDevObj->mpiLen;

    VpGetResultsOptionsDataType *pOptionData =
        &(pDevObj->getResultsOption.optionData);

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    switch(pEvent->eventCategory) {
        case VP_EVCAT_RESPONSE:
            switch (pEvent->eventId) {
                case VP_LINE_EVID_LLCMD_RX_CMP:
                    pMpiData = (uint8 *)pResults;
                    for (commandByte = 0;
                         commandByte < mpiDataLen;
                         commandByte++) {
                        pMpiData[commandByte] = pDevObj->mpiData[commandByte];
                    }
                    break;

				case VP_LINE_EVID_GAIN_CMP:
                    *(VpRelGainResultsType *)pResults =
                        pDevObj->relGainResults;
                    break;

                case VP_DEV_EVID_IO_ACCESS_CMP:                    
                    *((VpDeviceIoAccessDataType *)pResults) =
                        pOptionData->deviceIoData;
                    break;

                case VP_LINE_EVID_RD_OPTION:
                    switch(pDevObj->getResultsOption.optionType) {
                        case VP_DEVICE_OPTION_ID_PULSE:
                            *(VpOptionPulseType *)pResults =
                                pDevObj->pulseSpecs;
                            break;

                        case VP_DEVICE_OPTION_ID_PULSE2:
                            *(VpOptionPulseType *)pResults =
                                pDevObj->pulseSpecs2;
                            break;

                        case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                            *(VpOptionCriticalFltType *)pResults =
                                pOptionData->criticalFaultOption;
                            break;
            
                        case VP_DEVICE_OPTION_ID_DEVICE_IO:
                            *(VpOptionDeviceIoType *)pResults =
                                pOptionData->deviceIo;
                            break;

                        case VP_OPTION_ID_RING_CNTRL:
                            *(VpOptionRingControlType *)pResults =
                                pOptionData->ringControlOption;
                            break;

                        case VP_OPTION_ID_ZERO_CROSS:
                            *(VpOptionZeroCrossType *)pResults =
                                pOptionData->zeroCross;
                            break;

                        case VP_OPTION_ID_PULSE_MODE:
                            *((VpOptionPulseModeType *)pResults) =
                                pOptionData->pulseModeOption;
                            break;

                        case VP_OPTION_ID_TIMESLOT:
                            *(VpOptionTimeslotType *)pResults =
                                pOptionData->timeSlotOption;
                            break;

                        case VP_OPTION_ID_CODEC:
                            *((VpOptionCodecType *)pResults) =
                                pOptionData->codecOption;
                            break;

                        case VP_OPTION_ID_PCM_HWY:
                            *((VpOptionCodecType *)pResults) =
                                pOptionData->pcmHwyOption;
                            break;

                        case VP_OPTION_ID_LOOPBACK:
                            *((VpOptionLoopbackType *)pResults) =
                                pOptionData->loopBackOption;
                            break;

                        case VP_OPTION_ID_LINE_STATE:
                            *((VpOptionLineStateType *)pResults) =
                                pOptionData->lineStateOption;
                            break;

                        case VP_OPTION_ID_EVENT_MASK:
                            *((VpOptionEventMaskType *)pResults) =
                                pOptionData->eventMaskOption;
                            break;

                        case VP_OPTION_ID_PCM_TXRX_CNTRL:
                            *((VpOptionPcmTxRxCntrlType *)pResults) =
                                pOptionData->pcmTxRxCtrl;
                            break;

                        default:
                            status = VP_STATUS_INVALID_ARG;
                            break;
                    }
                    break;

                default:
                    status = VP_STATUS_INVALID_ARG;
                    break;
            }
            break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
        case VP_EVCAT_TEST:
            switch (pEvent->eventId) {
                case VP_LINE_EVID_TEST_CMP:
                    *((VpTestResultType *)pResults) = pDevObj->testResults;
                    break;

                 default:
                    break;
            }
#endif
            break;
        default:
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880ServiceTimers()
 *  This function services active API timers on all channels of deviceId.
 *
 * Preconditions:
 *  This Function must be called from the ApiTick function once per device.
 *
 * Postconditions:
 *  All Active Timers have been serviced.
 */
bool
Vp880ServiceTimers(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint16 tempTimer;
    VpLineTimerType lineTimerType;
    VpDevTimerType devTimerType;

    uint8 channelId, intMaskData[VP880_INT_MASK_LEN];
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    bool retFlag = FALSE;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint16 tickAdder =
        pDevObj->devProfileData.tickRate / (VP_CSLAC_TICKSTEP_0_5MS / 2);

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
    }

    for (devTimerType = 0; devTimerType < VP_DEV_TIMER_LAST; devTimerType++) {
        if (pDevObj->devTimer[devTimerType] >= 0) {
            pDevObj->devTimer[devTimerType]--;
        }

        if (pDevObj->devTimer[devTimerType] == 0) {
            switch(devTimerType) {
                case VP_DEV_TIMER_CLKFAIL:
                    /*
                     * Re-enable clock interrupt to see if condition is still
                     * true
                     */
                    VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_INT_MASK_RD,
                        VP880_INT_MASK_LEN, intMaskData);
                    intMaskData[0] &= ~(VP880_MASK_CFAIL_INT);
                    VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_INT_MASK_WRT,
                        VP880_INT_MASK_LEN, intMaskData);
                    retFlag = TRUE;
                    break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                case VP_DEV_TIMER_TESTLINE:
                    {
                        const void *pTestArgs =
                            (const void*)&pDevObj->currentTest.pTestHeap->testArgs;
                        uint8 testChanId = pDevObj->currentTest.channelId;
                        VpTestLineIntFuncPtrType testline =
                            pDevCtx->funPtrsToApiFuncs.TestLineInt;                            
                        if ((testline != VP_NULL)
                          && (pDevObj->currentTest.testState != -1)) {
                            /*
                             * if the TestLineInt function exists and the
                             * current test state has not been set back to
                             * -1 by test conclude before the timer expired
                             * then run the call back
                             */

                            testline(
                                pDevCtx->pLineCtx[testChanId],
                                pDevObj->currentTest.testId, 
                                pTestArgs,
                                pDevObj->currentTest.handle,
                                TRUE);
                        }
                    }
                    break;

#endif /* VP880_INCLUDE_TESTLINE_CODE */

                default:
                    break;
            }
        }
    }

    /* Iterate through the channels until all timers are serviced */
    for(channelId=0; channelId < maxChan; channelId++ ) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND)
                ? VP880_WIDEBAND_MODE : 0);

            if (!(pLineObj->status & VP880_IS_FXO)) {
            for (lineTimerType = 0; lineTimerType < VP_LINE_TIMER_LAST;
                 lineTimerType++) {

                if (pLineObj->lineTimers.timers.timer[lineTimerType]
                  & VP_ACTIVATE_LINE_TIMER) {

                    tempTimer = (pLineObj->lineTimers.timers.timer[lineTimerType]
                        & VP_TIMER_TIME_MASK);

                    if (tempTimer > 0) {
                        tempTimer--;
                    }

                    pLineObj->lineTimers.timers.timer[lineTimerType] = tempTimer;

                    if (tempTimer == 0) {
                        /* Perform appropriate action based on timerType */
                        switch (lineTimerType) {
                            case VP_LINE_RING_EXIT_DEBOUNCE:
                                    VpMpiCmd(deviceId, ecVal[channelId],
                                    VP880_UL_SIGREG_RD, VP880_UL_SIGREG_LEN, 
                                    pDevObj->intReg);

                                Vp880ServiceInterrupts(pDevCtx);    
                                break;

                            case VP_LINE_CID_DEBOUNCE:
                            case VP_LINE_TIMER_CID_DTMF:
                            case VP_LINE_TIMER_FAULT:
                                case VP_LINE_POLREV_DEBOUNCE:
                                break;

                            default:
                                break;
                        } /* Switch (timerType) */
                        pLineObj->lineTimers.timers.timer[lineTimerType] &=
                            ~(VP_ACTIVATE_LINE_TIMER);

                    } else { /* If timer has not expired */
                        pLineObj->lineTimers.timers.timer[lineTimerType]
                            |= VP_ACTIVATE_LINE_TIMER;
                    }
                } /* if timerType is active     */
            } /* Loop through all timerTypes for chanID */
            } else {
                /* Increment the time since polrev was observed */
                if (pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev
                    < (0x7FFF - tickAdder)) {
                    /*
                     * The time is in 0.25mS increments, but the device
                     * tickrate is something else. Increment by the scaled
                     * amount.
                     */
                    pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev
                        += tickAdder;
                } else {
                    /* Max limit the value of last polrev value */
                    pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev =
                        0x7FFF;
                }

                /* Set tick adder for 1ms increments */
                tickAdder =
                    pDevObj->devProfileData.tickRate / (VP_CSLAC_TICKSTEP_0_5MS * 2);

                if (((uint16)pLineObj->lineTimers.timers.fxoTimer.lastStateChange + tickAdder) > 255) {
                    pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 255;
                } else {
                    pLineObj->lineTimers.timers.fxoTimer.lastStateChange+=tickAdder;
                }

                if (((uint16)pLineObj->lineTimers.timers.fxoTimer.lastNotLiu - tickAdder) <= 0) {
                    pLineObj->lineTimers.timers.fxoTimer.lastNotLiu = 0;
                } else {
                    pLineObj->lineTimers.timers.fxoTimer.lastNotLiu-=tickAdder;
                }
            }
        } /* Line Context Check */
    } /* Loop through channels until no more tests */

    return retFlag;
} /* Vp880ServiceTimers() */

/**
 * Vp880FSKGeneratorReady()
 *  This function is used for Caller ID to determine if the FSK generator is
 * ready to accept another byte. It uses the device caller ID state machine
 * and signaling (caller ID status) register. This function should be called
 * from an API internal function only.
 *
 * Returns:
 *  TRUE if the FSK generator for Caller ID can accept a byte, FALSE otherwise.
 */
bool
Vp880FSKGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 channelId = pLineObj->channelId;
    uint8 cidParam[VP880_CID_PARAM_LEN];
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 cidState;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* If we're here, the generator should be enabled */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_RD,
        VP880_CID_PARAM_LEN, cidParam);
    cidState = (cidParam[0] & VP880_CID_STATE_MASK);

    if (cidParam[0] &  VP880_CID_DIS) {
        cidParam[0] &= ~(VP880_CID_DIS);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_WRT,
            VP880_CID_PARAM_LEN, cidParam);
    }

    /* Now check to see if it's ready to accept (more) CID data */
#if 0
printf(" - GenState 0x%02X", cidState);
fflush(NULL);
#endif

    if (cidState == VP880_CID_STATE_FULL) {
        return FALSE;
    } else {
#if 0
        return TRUE;
#else
        if ((cidState == VP880_CID_STATE_IDLE)
         || (cidState == VP880_CID_STATE_URUN)) {
            return TRUE;
        } else {
            /* Read the Interrupt Status Register */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_UL_SIGREG_RD,
                VP880_UL_SIGREG_LEN, pDevObj->intReg);

            /*
             * Force APITick to re-evaluate the signaling bits in case this
             * read caused the interrupt line to become inactive
             */
            pDevObj->stateInt |= VP880_FORCE_SIG_READ;

            if (pDevObj->intReg[channelId] & VP880_CID1_RDY_MASK) {
                return TRUE;
            } else {
                return FALSE;
            }
        }
#endif
    }
}

/**
 * Vp880CliGetEncodedByte()
 *  This function returns an encoded byte of data that is suitable for writing
 * the FSK generator (device dependent).
 *
 * Preconditions
 *  Must have a valid CLI packet in to work from.
 *
 * Postconditions
 *  The per-channel caller ID buffer will be updated with encoded data.
 *
 */
bool
Vp880CliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 nextByte = '\0';

    VpOptionEventMaskType *pLineEvents = &(pLineObj->lineEvents);
    VpCallerIdType *pCidStruct = &(pLineObj->callerId);

    uint8 checkSumIndex = VP_CID_PROFILE_FSK_PARAM_LEN +
        pLineObj->callerId.pCliProfile[VP_CID_PROFILE_FSK_PARAM_LEN] +
        VP_CID_PROFILE_CHECKSUM_OFFSET_LSB;

    if (pLineObj->callerId.status & VP_CID_MID_CHECKSUM) {
        pLineObj->callerId.status &= ~VP_CID_MID_CHECKSUM;
        pCidStruct->status |= VP_CID_END_OF_MSG;
        *pByte = '\0';
        return FALSE;
    }

    /* Check to determine which buffer is in use to index the message data */
    if (pCidStruct->status & VP_CID_PRIMARY_IN_USE) {
        /*
         * If the index is at the length of the buffer, we need to switch
         * buffers if there is more data
         */
        if (pCidStruct->cliMPIndex >= pCidStruct->primaryMsgLen) {
            /*
             * At the end of the Primary Buffer. Flag an event and indicate to
             * the API that this buffer is no longer being used and we can
             * accept more data
             */
            pCidStruct->status &= ~VP_CID_PRIMARY_IN_USE;
            pCidStruct->status &= ~VP_CID_PRIMARY_FULL;

            if (pCidStruct->status & VP_CID_SECONDARY_FULL) {
                pLineEvents->process |= VP_LINE_EVID_CID_DATA;
                pLineObj->processData = 0;

                pCidStruct->status |= VP_CID_SECONDARY_IN_USE;
                pCidStruct->cliMSIndex = 1;
                *pByte = pCidStruct->secondaryBuffer[0];
                nextByte = pCidStruct->secondaryBuffer[1];
            } else {
                if (pLineObj->callerId.pCliProfile[checkSumIndex]) {
                    *pByte = (uint8)(~pLineObj->callerId.cidCheckSum + 1);
                    pLineObj->callerId.status |= VP_CID_MID_CHECKSUM;
                } else {
                    *pByte = '\0';
                }
            }
        } else {
            *pByte = pCidStruct->primaryBuffer[pCidStruct->cliMPIndex];

            /* Get the next byte to be sent after the current byte */
            if ((pCidStruct->cliMPIndex+1) >= pCidStruct->primaryMsgLen) {
                if (pCidStruct->status & VP_CID_SECONDARY_FULL) {
                    nextByte = pCidStruct->secondaryBuffer[0];
                }
            } else {
                nextByte =
                    pCidStruct->primaryBuffer[pCidStruct->cliMPIndex+1];
            }
        }
        pCidStruct->cliMPIndex++;
    } else if (pCidStruct->status & VP_CID_SECONDARY_IN_USE) {
        /*
         * If the index is at the length of the buffer, we need to switch
         * buffers if there is more data
         */
        if (pCidStruct->cliMSIndex >= pCidStruct->secondaryMsgLen) {
            /*
             * At the end of the Secondary Buffer. Flag an event and indicate to
             * the API that this buffer is no longer being used and is empty
             */
            pLineEvents->process |= VP_LINE_EVID_CID_DATA;
            pLineObj->processData = 0;

            pCidStruct->status &= ~VP_CID_SECONDARY_IN_USE;
            pCidStruct->status &= ~VP_CID_SECONDARY_FULL;

            if (pCidStruct->status & VP_CID_PRIMARY_FULL) {
                pLineEvents->process |= VP_LINE_EVID_CID_DATA;
                pLineObj->processData = 0;

                pCidStruct->status |= VP_CID_PRIMARY_IN_USE;
                pCidStruct->cliMPIndex = 1;
                *pByte = pCidStruct->primaryBuffer[0];
                nextByte = pCidStruct->primaryBuffer[1];
            } else {
                /* There is no more data in either buffer */
                if (pLineObj->callerId.pCliProfile[checkSumIndex]) {
                    *pByte = (uint8)(~pLineObj->callerId.cidCheckSum + 1);
                    pLineObj->callerId.status |= VP_CID_MID_CHECKSUM;
                } else {
                    *pByte = '\0';
                }
            }
        } else {
            *pByte = pCidStruct->secondaryBuffer[pCidStruct->cliMSIndex];

            /* Get the next byte to be sent after the current byte */
            if ((pCidStruct->cliMSIndex+1) >= pCidStruct->secondaryMsgLen) {
                if (pCidStruct->status & VP_CID_PRIMARY_FULL) {
                    nextByte = pCidStruct->primaryBuffer[0];
                }
            } else {
                nextByte =
                    pCidStruct->secondaryBuffer[pCidStruct->cliMSIndex+1];
            }
        }
        pCidStruct->cliMSIndex++;
    }

    if ((!(pCidStruct->status & VP_CID_PRIMARY_IN_USE))
     && (!(pCidStruct->status & VP_CID_SECONDARY_IN_USE))) {
        if(pCidStruct->status & VP_CID_MID_CHECKSUM) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if ((nextByte == '\0')
    && (!(pLineObj->callerId.pCliProfile[checkSumIndex]))) {
        pCidStruct->status |= VP_CID_END_OF_MSG;
    }

    return TRUE;
}

/* Testing functions */
VpStatusType
Vp880TestLine(                                             
    VpLineCtxType *pLineCtx,                                  
    VpTestIdType test,                                        
    const void *pArgsUntyped,                                 
    uint16 handle)
{
	return VP_STATUS_SUCCESS;
}
    
VpStatusType
Vp880TestLineInt(                                             
    VpLineCtxType *pLineCtx,                                  
    VpTestIdType test,                                        
    const void *pArgsUntyped,                                 
    uint16 handle,                                            
    bool callback)
{
	return VP_STATUS_SUCCESS;
}

#endif






