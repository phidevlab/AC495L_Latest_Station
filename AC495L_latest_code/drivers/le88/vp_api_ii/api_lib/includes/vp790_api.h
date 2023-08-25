/** \file vp790_api.h
 * vp790_api.h
 *
 *  Header file that defines the Vp790 Device and Line Objects
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP790_API_H
#define VP790_API_H

#include "vp_hal.h"
#include "vp_CSLAC_types.h"

#define VP790_MAX_MPI_DATA  0x0E    /**< Max data from any MPI read command */
#define VP790_INT_SEQ_LEN   22
/*
 * Required Vp790_ Device and Line Objects for user instantiation if a Vp790
 * device is used
 */
typedef struct {
    uint8  maxNumInterrupts;
    uint16 pcmClkRate;
    uint8  mClkMask;
    uint8  mClkCorrection;
    uint16 tickRate;
    uint8  devCfg1;
    uint8  clockSlot;
    uint8  calibrationTime;
} Vp790DeviceProfileType;

typedef struct {
    uint8 channelId;    /**< Channel # associated with "this" line on the 
                         * device
                         */

    uint8 cidEncodeBuff[6]; /**< Encoded CID data from user data */
    uint8 cidEncodeIndex;   /**< current index into encoded buffer */
    uint8 cidEncodeSize;    /**< Size of valid CID encoded data */    
    
    VpTermType termType;    /**< Termination type of this line */

    VpOptionCodecType codec;    /**< Maintains the currently used code mode to
                                 * reduce MPI activity
                                 */

    VpOptionLineStateType lineStateBatOption;

    VpCallerIdType callerId;    /**< Caller ID related information */

    VpSeqDataType cadence;      /**< Sequencer related information */
    VpCidSeqDataType cidSeq;    /**< CID Sequencer related information */    

    /**< Array to control internally run sequences */
    VpProfileDataType intSequence[VP790_INT_SEQ_LEN];

    bool slsCalledFromApi;  /**< TRUE if SetLineState is called by the API */
    
    bool cadenceOnTimeComplete; /**< TRUE when CAD in Sig Register gets set */

    VpDialPulseDetectType dpStruct; /**< Dial Pulse Parameters for detecting
                                     * digits on FXS lines
                                     */

    VpOptionEventMaskType lineEventsMask;
    VpOptionEventMaskType lineEvents;

    VpOptionPulseModeType pulseMode;

    uint8 signalingData;    /**< Holds data for Signaling events on this line */
    uint8 processData;      /**< Holds data for Process events on this line */        

    VpCslacTimerStruct lineTimers; /**< Timers for "this" line */

    VpApiIntLineStateType lineState;    /**< Line state info, used for state
                                         * transition handling.
                                         */

    VpOptionPcmTxRxCntrlType pcmTxRxCtrl;   /* Defines how the PCM highway is 
                                             * set for "talk" linestates
                                             */
    VpOptionRingControlType ringCtrl;

    VpOptionZeroCrossType zeroCross;    /**< Defines how zero crossing is
                                         * implemented. The device doesn't really
                                         * support zero crossing "methods", but
                                         * storing what the user selected is
                                         * necessary for Get Option (so the
                                         * operations of Set/Get can be
                                         * orthogonal)
                                         */
                                             

    VpVirtualDeviceReg virtualDeviceReg;    /**< Virtual device register to 
                                             * minimize device access
                                             */

    VpProfilePtrType pRingingCadence;   /**< Currently used ringing cadence on
                                         * this line
                                         */
                                         
    VpProfilePtrType pCidProfileType1;  /**< Currently used caller ID profile
                                         * on this line for sequenced cid
                                         */

    VpProfilePtrType pCidProfileType2;  /**< Currently used caller ID profile
                                         * on this line for non-sequenced cid
                                         */

    uint16 lineEventHandle; /**< Line specific event handle information */

    uint16 dtmfDigitSense;          /**< Used to hold the DTMF digit reported
                                     * with VpDtmfDigitDetected() until the
                                     * VP_LINE_EVID_DTMF_DIG is generated.
                                     */                        

    VpLineIdType lineId;	/**< Application provided value for mapping a line to
                             * a line context
                             */
} Vp790LineObjectType;

typedef struct {
    VpDeviceIdType deviceId;    /**< Device chip select ID or type defined by 
                                 * user
                                 */

    VpDeviceStaticInfoType staticInfo;      /**< Info that will not change 
                                             * during runtime
                                             */
    VpDeviceDynamicInfoType dynamicInfo;    /**< Info that will change during 
                                             * runtime
                                             */
    VpDeviceStatusType status;             

    VpOptionEventMaskType deviceEventsMask;
    VpOptionEventMaskType deviceEvents;

    VpOptionPulseType pulseSpecs;
    VpOptionPulseType pulseSpecs2;

    VpOptionCriticalFltType criticalFault;

    VpDevTimerType devTimer[VP_DEV_TIMER_LAST];
    Vp790DeviceProfileType devProfileData;
    VpCSLACDeviceProfileTableType devProfileTable;
    VpCSLACProfileTableEntryType profEntry;

    uint8 mpiData[VP790_MAX_MPI_DATA];  /**< Buffer for MPI Low level reads to 
                                         * hold maximum amount of MPI data that
                                         * is possible
                                         */

    uint8 mpiLen;       /**< Length of data to be copied into mpiData buffer */

    uint16 eventHandle; /**< Device level event handle information */
    uint16 timeStamp;

    uint8 luTable[16];  /**< Lookup table used for bit manipulation of Caller 
                         * ID data.
                         */

    VpGetResultsOptionsType getResultsOption;
} Vp790DeviceObjectType;
        
#endif  /* vp790_api.h */




