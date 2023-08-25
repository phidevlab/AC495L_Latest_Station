/** \file vp_api_option.h
 * vp_api_option.h
 *
 * This file contains declaration associated with VP-API Options.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_OPTION
#define VP_API_OPTION

#include "vp_api_types.h"

/** Valid device and line options (see VpSetOption/VpGetOption) */
typedef enum {
    /* Device Options */

    /*
     * Two sets of dial pulse specifications are provided to support distinctive
     * 10pps/20pps detection required for NTT
     */
    VP_DEVICE_OPTION_ID_PULSE,          /**< Pulse and Flash decode timings */
    VP_DEVICE_OPTION_ID_CRITICAL_FLT,   /**< Specify action to take on critical
                                         * faults
                                         */
    /* Line Options */
    VP_OPTION_ID_ZERO_CROSS,    /**< Zero-cross ring-entry/exit options */

    /* Device Options */
    VP_DEVICE_OPTION_ID_RAMP2STBY,  /**< Set ramp time from Disconnect to
                                     * Standby
                                     */
    /* Line Options */
    VP_OPTION_ID_PULSE_MODE,    /**< Specifies pulse digit decode on or off */
    VP_OPTION_ID_TIMESLOT,      /**< Specify transmit and receive timeslots */
    VP_OPTION_ID_CODEC,         /**< Specify PCM encoding */
    VP_OPTION_ID_PCM_HWY,       /**< Select the active PCM highway (A or B) */
    VP_OPTION_ID_LOOPBACK,      /**< Specify loopback mode */
    VP_OPTION_ID_LINE_STATE,    /**< Select default line state modifiers */
    VP_OPTION_ID_EVENT_MASK,    /**< Specify the event mask (En/Disable specific
                                 * events) */
    VP_OPTION_ID_RESERVED_1,    /* Reserved option */
    VP_OPTION_ID_RING_CNTRL,    /**< Options for ringing control. */
    VP_OPTION_ID_RESERVED_2,    /* Reserved option */
    VP_OPTION_ID_DTMF_MODE,     /**< Options for DTMF detection control. */

    VP_DEVICE_OPTION_ID_DEVICE_IO,  /**< Set Device IO configuration option;
                                     * Device option
                                     */
    VP_OPTION_ID_RESERVED_EVENT_MASK_DVP, /* Reserved option; This option
                                           * NOT to be used by the 
                                           * application; Used internally by 
                                           * the API DVP-880 */
    VP_OPTION_ID_PCM_TXRX_CNTRL,    /**< Controls PCM transmit/receive paths */
    VP_DEVICE_OPTION_ID_PULSE2,     /**< Pulse and Flash decode timings */

    VP_OPTION_ID_US_TRANSCODEC,     /**< Specify transmit (upstream) transcoder
                                     * options
                                     */
    VP_OPTION_ID_DS_TRANSCODEC,     /**< Specify receive (downstream)
                                     * transcoder options
                                     */
    VP_OPTION_ID_ECHO_CANCELER,     /**< Specify echo canceller options */

    /**< Universal tone detection filter configuration options */
    VP_OPTION_ID_UTD_1_COEF,
    VP_OPTION_ID_UTD_2_COEF,
    VP_OPTION_ID_UTD_3_COEF,
    VP_OPTION_ID_UTD_4_COEF,
    VP_OPTION_ID_UTD_5_COEF,
    VP_OPTION_ID_UTD_6_COEF,
    VP_OPTION_ID_UTD_7_COEF,
    VP_OPTION_ID_UTD_8_COEF,

    VP_OPTION_ID_CLR_PKT_CNTR,  /* Specify clear packet statistics counter
                                 * option
                                 */
    VP_NUM_OPTION_IDS,
    VP_OPTION_ID_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpOptionIdType;

/** Parameters for dial pulse, flash, and on-hook */
typedef struct {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
} VpOptionPulseType;

/** Method for line control when critical faults are detected */
typedef struct {
    /**< The line is set to disconnect when the specified fault is active and
     * the "En" bit is set TRUE
     */
    bool acFltDiscEn;           /**< AC fault detected */
    bool dcFltDiscEn;           /**< DC fault detected */
    bool thermFltDiscEn;        /**< Thermal fault detected */
} VpOptionCriticalFltType;

/** Method for zero-cross control */
typedef enum {
    VP_OPTION_ZC_M4B,   /**< Zero-Cross On - Make before break */
    VP_OPTION_ZC_B4M,   /**< Zero-Cross On - Break before make */
    VP_OPTION_ZC_NONE,   /**< Turn Zero-Cross control off */
    VP_OPTION_ZC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpOptionZeroCrossType;

/** Dial Pulse decode enable/disable */
typedef enum {
    VP_OPTION_PULSE_DECODE_OFF, /**< Disable Pulse Decode */
    VP_OPTION_PULSE_DECODE_ON,   /**< Enable Pulse Decode */
    VP_OPTION_PULSE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPulseModeType;

/** Transmit/Receive Timeslot setting (timeslot and control) */
typedef struct {
    uint8 tx;   /**< 8-bit TX timeslot */
    uint8 rx;   /**< 8-bit RX timeslot */
} VpOptionTimeslotType;

typedef enum {
    VP_OPTION_ALAW,                 /**< Select G.711 A-Law PCM encoding */
    VP_OPTION_MLAW,                 /**< Select G.711 Mu-Law PCM encoding */
    VP_OPTION_LINEAR,               /**< Select Linear PCM encoding */
    VP_OPTION_WIDEBAND,             /**< Select Wideband PCM encoding */
    VP_NUM_OPTION_CODEC_TYPE_IDS,   /**< Select Linear PCM encoding */
    VP_OPTION_CODEC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionCodecType;

/** PCM Highway Selection (B valid on select devices only) */
typedef enum {
    VP_OPTION_HWY_A,    /**< Select the 'A' PCM Highway */
    VP_OPTION_HWY_B,     /**< Select the 'B' PCM Highway */
    VP_OPTION_HWY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmHwyType;

/** Loopback option selection */
typedef enum {
    VP_OPTION_LB_OFF,           /**< All loopbacks off */

    /* Following loopback options are supported for CSLAC and DVP only */
    VP_OPTION_LB_TIMESLOT,      /**< Perform a timeslot loopback */
    VP_OPTION_LB_DIGITAL,       /**< Perform a full-digital loopback */

    /* Following loopback options are supported for VPP only */
    VP_OPTION_LB_DS2US_PKT,     /**< Loopback downstream packets to upstream
                                 * packets at the HBI interface. */
    VP_OPTION_LB_US2DS_PKT,     /**< Loopback upstream packets to downstream
                                 * packets at the HBI interface. */
    VP_OPTION_LB_US2DS_SMPL,    /**< Loopback encoded upstream samples to
                                 * downstream sample decoder */
    VP_OPTION_LB_DS2US_SMPL,    /**< Loopback decoded downstream samples to
                                 * upstream sample encoder. */
    VP_NUM_LB_OPTIONS,
    VP_OPTION_LB_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionLoopbackType;

/* DevNotes: The names of the following type need to be changed to make use
 * of mapped battery names like Bat1, Bat2 ...*/
/** Active Line State battery supply selection */
typedef enum {
    VP_OPTION_BAT_AUTO,     /**< Automatic Batery selection */
    VP_OPTION_BAT_HIGH,     /**< Use High Batery */
    VP_OPTION_BAT_LOW,      /**< Use Low Batery */
    VP_OPTION_BAT_BOOST,    /**< Include Positive Batery */
    VP_NUM_OPTION_BAT_IDS,
    VP_OPTION_BAT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionBatType;

/** Active Line State battery supply selection */
typedef struct {
    bool battRev;       /**< Smooth/Abrupt Battery Reversal (TRUE = abrupt) */

    VpOptionBatType bat;    /**< Battery selection for Active line state */
} VpOptionLineStateType;

/** Ring control option */
typedef enum {
    VP_LINE_STANDBY,        /**< Low power line feed state */
    VP_LINE_TIP_OPEN,       /**< Tip open circuit state */
    VP_LINE_ACTIVE,         /**< Line Feed w/out VF */
    VP_LINE_ACTIVE_POLREV,  /**< Polarity Reversal Line Feed w/out VF */
    VP_LINE_TALK,           /**< Normal off-hook Active State; Voice Enabled */
    VP_LINE_TALK_POLREV,    /**< Normal Active with reverse polarity;
                             *   Voice Enabled */

    VP_LINE_OHT,            /**< On-Hook tranmission state */
    VP_LINE_OHT_POLREV,     /**< Polarity Reversal On-Hook tranmission state */

    VP_LINE_DISCONNECT,     /**< Denial of service */
    VP_LINE_RINGING,        /**< Ringing state */
    VP_LINE_RINGING_POLREV, /**< Ringing w/Polarity Reversal */

    VP_LINE_FXO_OHT,        /**< FXO Line providing Loop Open w/VF */
    VP_LINE_FXO_LOOP_OPEN,  /**< FXO Line providing Loop Open w/out VF */
    VP_LINE_FXO_LOOP_CLOSE, /**< FXO Line providing Loop Close w/out VF */
    VP_LINE_FXO_TALK,       /**< FXO Line providing Loop Close w/VF */
    VP_LINE_FXO_RING_GND,   /**< FXO Line providing Ring Ground (GS only)*/

    VP_LINE_STANDBY_POLREV, /**< Low power polrev line feed state */

    VP_NUM_LINE_STATES,
    VP_LINE_STATE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineStateType;

typedef struct {
    VpOptionZeroCrossType zeroCross;    /**< LCAS zero cross control */

    uint16 ringExitDbncDur; /**< Ringing Exit Debounce Duration; Used during end 
                             * of ON periods of ringing cadences; 125uS 
                             * resolution
                             */

    VpLineStateType ringTripExitSt; /**< State to automatically switch to upon 
                                     * ring trip
                                     */
}  VpOptionRingControlType;

/** DTMF detection option */
typedef enum {
    VP_OPTION_DTMF_DECODE_OFF,      /**< Disable DTMF Digit Decode */
    VP_OPTION_DTMF_DECODE_ON,       /**< Enable DTMF Digit  Decode */
    VP_OPTION_DTMF_GET_STATUS,      /**< Do not change anything; Just get the 
                                     *   DTMF status  */
    VP_NUM_OPTION_DTMF_IDS,
    VP_OPTION_DTMF_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionDtmfModeControlType;

/* Device I/O Option related definitions */
typedef enum {
    VP_IO_INPUT_PIN,                /* Configure GPIO pin as input pin */
    VP_IO_OUTPUT_PIN,               /* Configure GPIO pin as output pin */
    VP_IO_DIR_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceIoDirectionType;

typedef enum {
    VP_OUTPUT_DRIVEN_PIN,           /* Configure as TTL/CMOS output pin */
    VP_OUTPUT_OPEN_PIN,             /* Configure as open collector/drain 
                                     * output pin */
    VP_OUTPUT_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceOutputPinType;

typedef struct {
    uint32 directionPins_31_0;      /* Device specific IO pin direction 
                                     * (Pins 0 - 31) */
    uint32 directionPins_63_32;     /* Device specific IO pin direction 
                                     * (Pins 32 - 63) */
    uint32 outputTypePins_31_0;     /* Output pin type (Pins 0 - 31) */
    uint32 outputTypePins_63_32;    /* Output pin type (Pins 32 - 63) */
} VpOptionDeviceIoType;

typedef enum {
    VP_OPTION_PCM_BOTH,             /* Enable both PCM transmit and receive 
                                     * paths */
    VP_OPTION_PCM_RX_ONLY,          /* Enable PCM receive path only */
    VP_OPTION_PCM_TX_ONLY,          /* Enable PCM transmit path only */
    VP_PCM_TXRX_CNTRL_ENUM_SIZE=FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmTxRxCntrlType;

/** Direction Specification */
typedef enum {
    VP_DIRECTION_DS,
    VP_DIRECTION_US,
    VP_DIRECTION_INVALID,   /**< Used by the API to determine if the direction
                             * field is valid */
    VP_DIRECTION_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDirectionType;

/** DTMF detection option control */
typedef struct {
    VpOptionDtmfModeControlType dtmfControlMode; /**< DTMF detection 
                                                  * Enable/Disable */
    VpDirectionType direction;                  /**< Detection direction */                    
    uint32 dtmfDetectionSetting;                /**< Indicates the DTMF
                                                 *   detection setting for all 
                                                 *    the lines */
    uint8 dtmfResourcesRemaining;               /**< DTMF decoder resources 
                                                 *   remaining */

} VpOptionDtmfModeType;

/**< The following types are for dealing with upstream/downstream transcodec 
 * Options
 */
/* VPP Packet types */
typedef enum {
    VP_PKT_G711_MULAW        = 0x00, /**< Select G.711 Mu-Law PCM 
                                      * encoding
                                      */
    VP_PKT_G726_32K_AAL2_SO  = 0x02, /**< G.726, 32Kbps CODEC; Least
                                      * significant nibble(4-bits) 
                                      * contains the first sample to be 
                                      * played out.
                                      */
    VP_PKT_G711_ALAW         = 0x08, /**< Select G.711 A-Law PCM 
                                      * encoding
                                      */
    VP_PKT_G729AB            = 0x12, /**< Select G.729AB */
    VP_PKT_LINEAR            = 0x60, /**< Select Linear PCM encoding */
    VP_PKT_G726_32K          = 0x6E, /**< G.726, 32Kbps CODEC; Most 
                                      * significant nibble(4-bits) 
                                      * contains the first sample to be 
                                      * played out.
                                      */
    VP_PKT_SID               = 0x80, /**< Silence Insertion Description 
                                      * packet
                                      */
    VP_PKT_CID               = 0x82, /**< CallerID Packet */
    VP_PKT_NOP               = 0xFF,  /**< No operation packet */
    VP_PKT_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpPacketType;

typedef struct {
    VpPacketType packetType;  /**< Upstream Transcoder Type */
    uint8 packetSize;               /**< Packet size */

    bool silenceSupression;     /**< if TRUE, silence suppression is enabled */
} VpOptionUsTranscoderType;

/**< The following types are for dealing with Downstream Transcoder Options */
typedef enum {
    VP_PLC_TYPE_SILENCE,        /**< Lost packets are replaced with silence */
    VP_PLC_TYPE_PREVIOUS_PKT,   /**< Lost packets are replaced with previous 
                                 * packet
                                 */
    VP_PLC_TYPE_LEG_ALG,        /**< Lost packets are replaced using Legerity
                                 * proprietary algorithm
                                 */
    VP_PLC_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpPlcType;

typedef struct {
    VpPacketType packetType;  /**< Downstream Transcoder Type */
    uint8 packetSize;               /**< Packet size */
    uint8 framesToMute;             /**< Consecutive lost frames after which
                                     * audio mutes; valid range 0 to 7
                                     */
    uint8 ramp;                     /**< Number of samples to ramp into 
                                     * correction valid range 0 to 31
                                     */
    VpPlcType plc;                  /**< PLC Algorithm type */
} VpOptionDsTranscoderType;

/**< The following types are for Echo Canceler Options */
typedef enum {
    VP_EC_TAIL_LENGTH_4MS,
    VP_EC_TAIL_LENGTH_8MS,
    VP_EC_TAIL_LENGTH_12MS,
    VP_EC_TAIL_LENGTH_16MS,
    VP_EC_TAIL_LENGTH_20MS,
    VP_EC_TAIL_LENGTH_24MS,
    VP_EC_TAIL_LENGTH_28MS,
    VP_EC_TAIL_LENGTH_32MS,
    VP_EC_TAIL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpECTailLengthType;

typedef struct {
    VpECTailLengthType tailLength;  /**< Echo Canceler Tail length */

    bool toneDisable;       /**< If TRUE, EC remains active on 2100Hz tone 
                             * detection with phase reversal
                             */
    bool echoCanDisable;    /**< If TRUE, disables EC */
    bool adaptDisable;      /**< If TRUE, disables EC adaption */ 
    bool resetHRegister;    /**< If TRUE, clears H-Register */ 
    bool cngDisable;        /**< If TRUE, disables Comfort Noise Generation */
    bool nlpDisable;        /**< If TRUE, disables Non-Linear processing */
} VpOptionEchoCancelerType;

/**< The following types are for Upstream Tone Detect Filter Options */
typedef struct {
    uint8 utd_id;     /**< UTD Filter number */
    uint8 threshold;  /**< Filter detection threshold */
    uint16 coeff_b2;
    uint16 coeff_b1;
    uint16 coeff_b0;
    uint16 coeff_a2;
    uint16 coeff_a1;
    uint8 debounce;    /**< Debouce time in multiples of 500uS */
} VpOptionUtdCoeffType;

#endif /* VP_API_OPTION */




