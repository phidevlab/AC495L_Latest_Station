/** \file vp880_api.h
 * vp880_api.h
 *
 *  Header file that define all the commands for the Vp880 device.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP880_API_H
#define VP880_API_H

#include "vp_hal.h"
#include "vp_CSLAC_types.h"

#define VP880_MAX_NUM_CHANNELS   2

#ifdef VP880_INCLUDE_TESTLINE_CODE
#include "vp_api_test.h"
  #ifdef EZ_MPI_PCM_COLLECT
  #include  "vp_pcm_compute.h"
  #endif
#endif

#define VP880_MAX_MPI_DATA  15   /* Max data from any MPI read command */
#define VP880_INT_SEQ_LEN       22

#ifndef VP880_DC_FEED_LEN
#define VP880_DC_FEED_LEN    0x02
#endif

#ifndef VP880_REGULATOR_PARAM_LEN
#define VP880_REGULATOR_PARAM_LEN   0x03
#endif

#ifndef VP880_LOOP_SUP_LEN
#define VP880_LOOP_SUP_LEN      0x04
#endif

#ifndef VP880_SIGA_PARAMS_LEN
#define VP880_SIGA_PARAMS_LEN       0x0B
#endif

#ifndef VP880_UL_SIGREG_LEN
#define VP880_UL_SIGREG_LEN         0x02
#endif

#ifndef VP880_ICR1_LEN
#define VP880_ICR1_LEN      0x04
#endif

#ifndef VP880_ICR2_LEN
#define VP880_ICR2_LEN      0x04
#endif

#ifndef VP880_ICR3_LEN
#define VP880_ICR3_LEN      0x04
#endif

#ifndef VP880_ICR4_LEN
#define VP880_ICR4_LEN      0x04
#endif

#ifndef VP880_ICR6_LEN
#define VP880_ICR6_LEN      0x02
#endif

#ifndef VP880_REGULATOR_CTRL_LEN
#define VP880_REGULATOR_CTRL_LEN    0x01
#endif

#ifndef VP880_TX_PCM_BUFF_LEN
#define VP880_TX_PCM_BUFF_LEN 0x0E
#endif

#if defined (VP880_INCLUDE_TESTLINE_CODE)
/*
 * #defines used in unit conversion function
 * All units are based on the following calculations
 *      Internal voltage unit   = 480.0  / 2^31 Volts
 *      Internal current unit   = 240e-3 / 2^31 Amps
 *      Internal freq uint      = 12e3   / 2^31 Hz
 */
#define VP880_UNIT_MV                          4474l    /* mV unit */
#define VP880_UNIT_MVRMS                       6327l    /* mVrms unit */
#define VP880_UNIT_ADC_VAB                     -32768l  /* vab step from ADC */
#define VP880_UNIT_ADC_VAB_RMS                 46341l   /* vab step from ADC rms*/
#define VP880_UNIT_DAC_RING                    21076l   /* step due to DAC in ringing*/
#define VP880_UNIT_UA                          8948l    /* uA unit */
#define VP880_UNIT_ADC_IMET_NOR                16384l   /* met I from ADC in normal*/
#define VP880_UNIT_ADC_IMET_RING               32768l   /* met I from ADC in ringing*/
#define VP880_UNIT_ADC_ILG                     11469    /* long current from ADC*/
#define VP880_UNIT_DECHZ                       17896l   /* Deci Hz*/
#define VP880_UNIT_MHZ                         179l     /* mHz */
#define VP880_UNIT_FREQ                        65536l   /* siggen freq step */
#define VP880_UNIT_RAMP_TIME                   1365l    /* Ramp time (-1/3) in ms*/

#define VP880_UNIT_CONVERT(DATA, I_UNIT, O_UNIT) \
    (int32)( ((int32)DATA * I_UNIT) / O_UNIT)

/* converts requested slope into SigGen Freq */
#define VP880_SLOPE_TO_FREQ(AMP,SLOPE) \
    (int16) ( ((int32)SLOPE * (int32)VP880_UNIT_RAMP_TIME) / (int32)AMP)

#endif /*VP880_INCLUDE_TESTLINE_CODE*/

/**< Required Vp880 Device and Line Objects for user instantiation if a Vp880
 * device is used
 */

/**< Structure used to save off filter coefficients for DTMF generation.
 * specifically needed for upstream DTMF generation, which is implemented by
 * generating downstream DTMF and causing 0dB roud-trip echo
 */
typedef struct {
    bool pcmOverride;       /**< TRUE when the Set Tone function is overriding
                             * the line state control of the PCM enabled bits
                             */
    uint8 opCond;           /**< Stores TX/RX PCM path enabled/disabled state
                             * when Set Tone function is overriding the line
                             * state control of the PCM enabled bits
                             */
} Vp880SetTonePcmOverride;

/**< Structure that defines the Vp880 Device Profile. Current as of the first
 * Device Profile version (ver 0).
 */
typedef struct {
    uint8  maxNumInterrupts;
    uint16 pcmClkRate;      /**< Used to verify valid TX/RX Timeslot setting */
    uint8  mClkMask;
    uint16 tickRate;        /**< Primary API-II tick for this device */
    uint8  devCfg1;
    uint8  clockSlot;
} Vp880DeviceProfileType;

/**< Line Status types to minimize code space in line object (compared to each
 * status being maintined by a uint8 type)
 */
typedef enum {
    VP880_INIT_STATUS = 0x0000,

    VP880_IS_FXO = 0x0001,  /**< Set if the line is configured for FXO */

    VP880_SLS_CALL_FROM_API = 0x0002,   /**< Set if Set Line State is called
                                         * from an API function (e.g., cadence).
                                         */

    VP880_BAD_LOOP_SUP = 0x0004,    /**< Set when the Loop Supervision has been
                                     * changed in such a way inconsistent with
                                     * the user's specifications. This is done
                                     * in internal to the API to make some
                                     * functions work (e.g., Msg Wait Pulse).
                                     */

    VP880_UNBAL_RINGING = 0x0008,   /**< Set if this line uses unbal ringing */

    VP880_DP_SET1_DONE = 0x0010,    /**< Set when Dial Pulse detection machine
                                     * is "done" on the current dial pulse using
                                     * the "first" set of DP parameters
                                     */
    VP880_DP_SET2_DONE = 0x0020    /**< Set when Dial Pulse detection machine
                                     * is "done" on the current dial pulse using
                                     * the 2nd set of DP parameters
                                     */
} Vp880LineStatusType;

#if defined (VP880_INCLUDE_TESTLINE_CODE)
/* Definitions for Test arguments */
typedef union {
    VpTestConcludeType          conclude;
    VpTestOpenVType             openV;
    VpTestDcRLoopType           dcRloop;
    VpTestAcRLoopType           acRloop;
    VpTest3EleResAltResType     resFltAlt;
    VpTestLoopCondType          loopCond;
    VpTestLoopbackType          loopback;
    VpTestRampType              ramp;
    VpTestRampInitType          rampInit;
} VpTestArgsType;

/* Used to configure the device for a target current */
typedef struct {
    bool success;
    int16 polarity;
    int16 state;    
    int16 nullI;
    int16 metI;
    uint8 prevSigCtrl;
    uint8 prevSlacState;
    uint8 prevVpGain;
    uint8 prevDcFeed[VP880_DC_FEED_LEN];
} Vp880TargetCurrentType;


typedef struct {
    uint8 adcState;
    int16 nextState; /**< Used when a pcm collection routine is started */

    VpTestArgsType testArgs; /**< Saved test input arguments of current test */

    uint8 opCond;                           /**< Saved Operation Condition */
    uint8 opFunction;                       /**< Saved Operation Functions */
    uint8 sigCtrl;                          /**< Signal Generator Control */
    uint8 slacState;                        /**< Saved Slac State */
    uint8 sysConfig;                        /** Saved System Configurations */
    uint8 vpGain;                           /**< Voice Path Gain */
    uint8 switchReg[VP880_REGULATOR_PARAM_LEN]; /**< Switching Reg Parameters */
    uint8 dcFeed[VP880_DC_FEED_LEN];        /**< Saved DC Feed Parameters */
    uint8 disn;                             /**< Digital Imped. Scaling Network */
    
    uint8 icr1[VP880_ICR1_LEN];
    uint8 icr2[VP880_ICR2_LEN];
    uint8 icr3[VP880_ICR3_LEN];
    uint8 icr4[VP880_ICR4_LEN];
    uint8 icr6[VP880_ICR6_LEN];
    uint8 lpSuper[VP880_LOOP_SUP_LEN];      /**< Saved Loop Sup. Parameters */
    uint8 sigGenAB [VP880_SIGA_PARAMS_LEN]; /**< Saved Signal Generator A & B*/   

    /* used for collecting PCM data */
    bool pcmRequest;        /** < indication that pcm data was requested */
    VpPcmOperationMaskType operationMask;
  
    VpPcmOperationResultsType pcmResults; /** < stores the pcm operation results */
    
    /* Used for setting up a current measurment */
    Vp880TargetCurrentType targetI;    
    
    /* Used for common setup functions */
    uint16 commonSetupState;    
        
    /* Used for storing line event mask data */
    VpOptionEventMaskType preTestEventMask;    
    
    /* Used for saving and restoring slac state after calibration */
    uint8 calSlacState;

    /* Used for resflt lg speed up*/
    int16 previousAvg;
    uint16 speedupTime;

    /* The following members are for EZ mode calculations only*/
#ifdef EZ_MPI_PCM_COLLECT
    VpPcmComputeTempType ezPcmTemp;
#endif

} Vp880TestHeapType;

typedef struct {
    Vp880TestHeapType *pTestHeap;
    uint8 testHeapId;
    
    uint8 channelId;    /**< Channel # for "this" line on the device.  Indexed
                         * starting at 0, should not exceed the max number of
                         * lines supported by the device - 1 (max = 2, then
                         * channelId = {0, 1}
                         */

    bool prepared;    /**< indicates if the current test is prepared */
    bool concluding;    /**< indicates that the device is concluding a test */    
    VpTestIdType testId; /** < indicates the test currently running */

    int16 testState;   /**< maintains the currnt state of the current TestId */
    uint16 handle;
  
} Vp880CurrentTestType;

typedef struct {
    int16 nullOffset;
    int16 vabOffset;
    int16 vahOffset;
    int16 valOffset;
    int16 vbhOffset;
    int16 vblOffset;
} Vp880CalOffCoeffs;

#endif /*VP880_INCLUDE_TESTLINE_CODE*/

typedef struct {
    uint8 channelId;    /**< Channel # for "this" line on the device.  Indexed
                         * starting at 0, should not exceed the max number of
                         * lines supported by the device - 1 (max = 2, then
                         * channelId = {0, 1}
                         */

    VpTermType termType;    /**< Termination type */

    Vp880LineStatusType status; /**< Keeps track of several line state/config */

    VpCallerIdType callerId;    /**< Caller ID related information */

    VpSeqDataType cadence;      /**< Sequencer related information */
    VpCidSeqDataType cidSeq;    /**< CID Sequencer related information */

    /**< Array to control internally run sequences */
    VpProfileDataType intSequence[VP880_INT_SEQ_LEN];

    VpDialPulseDetectType dpStruct; /**< Used on FXS lines for detecting pulse
                                     * digits
                                     */

    VpDialPulseDetectType dpStruct2;/**< Used on FXS lines for detecting pulse
                                     * digits using 2nd set of dp specs.
                                     */

    VpDigitGenerationDataType digitGenStruct;   /**< Used on FXO lines for
                                                 * generating pulse digits
                                                 */
    Vp880SetTonePcmOverride setToneOverride;    /**< Saves filter coefficients
                                                 * when upstream DTMF is
                                                 * generated
                                                 */

    VpOptionCodecType codec;    /**< Codec mode used on this line. Maintained in
                                 * the API-II to reduce MPI activity which is a
                                 * shared resource and requires CRITICAL code
                                 * section handling.
                                 */

    VpOptionEventMaskType lineEventsMask;
    VpOptionEventMaskType lineEvents;

    uint16 signaling1;
    uint16 signaling2;

    VpOptionPulseModeType pulseMode;

    uint8 fxoData;          /**< Holds data for FXO events on this line */
    uint8 preRingPolRev;    /**< The polarity detected prior to ring detect */

    uint8 signalingData;    /**< Holds data for Signaling events on this line */
    uint8 signalingData1;   /**< Holds data for Signaling events on this line */
    uint8 signalingData2;   /**< Holds data for Signaling events on this line */

    uint8 processData;      /**< Holds data for Process events on this line */

    VpCslacTimerStruct lineTimers; /**< Timers for "this" line */
    VpApiIntLineStateType lineState;    /**< Line state info used for state
                                         * transition handling and recall
                                         */

    /*
     * Holds user definition for Loop Supervision configuration when
     * "badLoopSup" is TRUE
     */
    uint8 loopSup[VP880_LOOP_SUP_LEN];

    /*
     * Array to hold ringing parameters used in the Signal Generator.  This is
     * needed when signal generator A is set to a tone, then set back to ringing
     * without the user re-specifying the ringing profile
     */
    uint8 ringingParams[VP880_SIGA_PARAMS_LEN];

    VpProfilePtrType pRingingCadence;   /**< Currently used ringing cadence on
                                         * this line
                                         */

    VpProfilePtrType pMeterProfile;     /**< Currently used metering profile on
                                         * this line
                                         */

    VpProfilePtrType pCidProfileType1;  /**< Currently used caller ID profile
                                         * on this line for sequenced cid
                                         */

    VpProfilePtrType pCidProfileType2;  /**< Currently used caller ID profile
                                         * on this line for non-sequenced cid
                                         */

    uint16 lineEventHandle; /**< Line specific event handle information */
    
    VpOptionRingControlType ringCtrl;
    
    VpOptionPcmTxRxCntrlType pcmTxRxCtrl;   /* Defines how the PCM highway is
                                             * set for "talk" linestates
                                             */
  
    int onHookTicks;    /**< Used to count duration since last on-hook used for
                         * ABS/Clare errata
                         */

    uint16 dtmfDigitSense;          /**< Used to hold the DTMF digit reported
                                     * with VpDtmfDigitDetected() until the
                                     * VP_LINE_EVID_DTMF_DIG is generated.
                                     */
                                     
    VpRelayControlType relayState;   /**< Used to hold current line relay state */
                                         
    VpLineIdType lineId;	/**< Application provided value for mapping a line to
                             * a line context
                             */
    struct {
        uint16 gxInt;       /**< Cached GX register, in 2.14 int format */
        uint16 grInt;       /**< Cached GR register, in 2.14 int format */
    } gain;

    uint8 ringDetMax;   /**< Stores the user specified maximum ringing detect
                         * period for FXO lines. This value may be outside the
                         * device range, in which case the SW will implement
                         * upper period detection
                         */
    uint8 ringDetMin;   /**< Stores the user specified maximum ringing detect
                         * period for FXO lines that is within the device range.
                         * This value is used as "minimum" that is detected by
                         * SW. Actual minimum period is supported by the device
                         * period detector itself.
                         */

} Vp880LineObjectType;

typedef enum {
    VP880_IS_ABS = 0x0001,                  /**< Set when the device is ABS type */
    VP880_FORCE_SIG_READ = 0x0002,          /**< Set to force a signaling register read */
    VP880_HAS_TEST_LOAD_SWITCH = 0x0004,    /**< Set when test load swith is available */
    VP880_HAS_CALIBRATE_CIRCUIT = 0x0008,   /**< Set when test cal circuit is available */
    VP880_IS_HIGH_VOLTAGE = 0x0010,         /**< Set when device is a high voltage device */
    VP880_IS_SINGLE_CHANNEL = 0x0020,       /**< Set when a single channel device is found*/
    VP880_DEVICE_DETECTED = 0x0040,         /**< Set when the device is detected */
    VP880_LINE0_IS_FXO = 0x0080,            /**< Set if device detection indicates line0 as FXO */
    VP880_LINE1_IS_FXO = 0x0100,            /**< Set if device detection indicates line1 as FXO */
    VP880_WIDEBAND = 0x0200                 /**< Set if device supports Wideband mode */
} Vp880DeviceStateIntType;
    
typedef enum {
    VP880_ABS_CAL_STATE_NONE = 0x0000,
    VP880_ABS_CAL_STATE_1,
    VP880_ABS_CAL_STATE_2,
    VP880_ABS_CAL_STATE_3,
    VP880_ABS_CAL_STATE_4,
    VP880_ABS_CAL_STATE_5,
    VP880_ABS_CAL_STATE_6,
    VP880_ABS_CAL_STATE_7,
    VP880_ABS_CAL_STATE_8,
    VP880_ABS_CAL_STATE_9,
    VP880_ABS_CAL_STATE_10,
    VP880_ABS_CAL_STATE_11,
    VP880_ABS_CAL_STATE_12,
    VP880_ABS_CAL_STATE_LAST = 0xFFFF
} Vp880AbsCalStateType;

typedef struct {
    VpDeviceIdType deviceId;    /**< Device chip select ID defined by user */
    VpDeviceStaticInfoType staticInfo;  /**< Info that will not change during
                                         * runtime
                                         */
    VpDeviceDynamicInfoType dynamicInfo;    /**< Info that will change during
                                             * runtime
                                             */
    VpDeviceStatusType status;
    Vp880DeviceStateIntType stateInt;

    VpOptionEventMaskType deviceEventsMask;
    VpOptionEventMaskType deviceEvents;

    /*
     * Two sets of dial pulse specifications are provide to support NTT dial
     * pulse detection windows of 10pps and 20pps while excluding 15pps
     */
    VpOptionPulseType pulseSpecs;
    VpOptionPulseType pulseSpecs2;

    VpOptionCriticalFltType criticalFault;

    VpDevTimerType devTimer[VP_DEV_TIMER_LAST];
    uint8 timerChan[2];     /**< Channel ID associated with device timer */
    uint16 timerHandle[2];  /**< Event Handle associated with device timer */
    
    Vp880DeviceProfileType devProfileData;
    VpCSLACDeviceProfileTableType devProfileTable;
    VpCSLACProfileTableEntryType profEntry;

    /**< State of signaling interrupt register - length 2 bytes */
    uint8 intReg[VP880_UL_SIGREG_LEN];  /**< Holds signaling data info for the
                                         * device
                                         */

    uint8 mpiData[VP880_MAX_MPI_DATA];  /**< Buffer for MPI Low level reads to
                                         * hold maximum amount of MPI data that
                                         * is possible
                                         */

    uint8 mpiLen;       /**< Length of data to be copied into mpiData buffer */

    uint16 eventHandle;  /** Application defined event handle */
    uint16 timeStamp;   /**< Used to track event timing. Increment by ticks */

    VpGetResultsOptionsType getResultsOption;
    VpRelGainResultsType relGainResults;


    /* Testing structure */
    VpTestResultType testResults;
#if defined (VP880_INCLUDE_TESTLINE_CODE)
    Vp880CurrentTestType currentTest;

    /**< Used to hold calibration offset coeffs. One per channel */
    Vp880CalOffCoeffs calOffsets[VP880_MAX_NUM_CHANNELS];

#endif /* VP880_INCLUDE_TESTLINE_CODE */

    /*
     * Used to hold battery switch calibration offset. One per channel, per
     * polarity
     */
    uint8 absNormCal[VP880_MAX_NUM_CHANNELS];
    uint8 absPolRevCal[VP880_MAX_NUM_CHANNELS];
    uint8 cachedSwCtrl[VP880_REGULATOR_CTRL_LEN];
    Vp880AbsCalStateType calState;

    /*
     * Used to get better hook granularity and pcm buffered data.
     */
     uint8 txBuffer[VP880_TX_PCM_BUFF_LEN];
     uint8 txBufferDataRate;
} Vp880DeviceObjectType;

#endif  /**< vp880_api.h */






