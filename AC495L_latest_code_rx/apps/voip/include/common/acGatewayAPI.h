/******************************************************************** 
 * acGatewayAPI - AudioCodes Analog VoIP Gateway DSP and SLIC API                  	* 
 *                                                                      *
 * API Function Prototypes                                              * 	
 *                                                                      *
 * File Name:	acGatewayAPI.h                                          *
 *                                                                      *
 * Copyright (C) 2007 AudioCodes Ltd.                                   *
 *                                                                      *
 * Modifications :                                                      *
 *                                                                      *
 * 30/12/07 - Coding started.                                           *
 *                                                                      *
 ********************************************************************/

#ifndef	_ACGATEWAYAPI_H
#define	_ACGATEWAYAPI_H

#include "acGatewayDefs.h"


/************************************************************
******												  ********
******	DSP Channel Configuration and Control API Functions	  ********
******												  ********
 ************************************************************/

    /**
     * @defgroup acgGetDefaultInitDeviceParameters acgGetDefaultInitDeviceParameters
     * @ingroup DEVICE_CONFIGURATION_GENERAL
     * @brief 
     * Gets the default values to the DSP initialization structure.
     * @{
     */

    /**
     * <BR>
     * The @b acgSetDefaultInitDeviceParameters function set default values to the device
     * initialization structure.
     * 
     * @param pParams    [in] ::acgTInitializationParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */
     
int acgGetDefaultInitDeviceParameters(acgTInitializationParameters *pInitializationParams);


    /**
     * @defgroup acgInitializeDSP acgInitializeDSP
     * @ingroup DEVICE_CONFIGURATION_GENERAL
     * @brief 
     * Initialize the DSP.
     * @{
     */

    /**
     * <BR>
     * The @b acgInitializeDSP function initialized the DSP, loading the requested firmaware
     *	and setting default values to the DSP channel information.
     * 
     * This function must be invoked before any AudioCodes Analog VoIP Gateway DSP API's.
     *    
     * @param pParams    [in] ::acgTChannelParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgInitializeDSP(acgTInitializationParameters *pInitializationParams); 
    /**
     * @defgroup acgReInitializeDSP acgReInitializeDSP
     * @ingroup DEVICE_CONFIGURATION_GENERAL
     * @brief 
     * Re-Initialize the DSP.
     * @{
     */

    /**
     * <BR>
     * The @b acgReInitializeDSP function Reinitialized the DSP without loading the requested firmaware
     *	and setting default values to the DSP channel information.
     * 
     * This function must be invoked when dsp have been initialed.
     *    
     * @param pParams    [in] ::acgTInitializationParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgReInitializeDSP(acgTInitializationParameters *pReInitializationParams) ;
    /** @} */

    /**
     * @defgroup acgCloseDSP acgCloseDSP
     * @ingroup DEVICE_CONFIGURATION_GENERAL
     * @brief 
     * Close the DSP.
     * @{
     */

    /**
     * <BR>
     * The @b acgCloseDSP function closes the DSP.
     * 
     * @param pParams   
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgCloseDSP(void); 

    /** @} */

	

    /**
     * @defgroup acgGetDefaultChannelParameters acgGetDefaultChannelParameters
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Get the default channel configuration parameters.
     * @{
     */

    /**
     * <BR>
     * The @b acgGetDefaultChannelParameters function get the default channel parameter values.
     * Note: The original default parameters can be restored at any time by calling this function.
     *    
     * @param pParams    [out] ::acgTChannelParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetDefaultChannelParameters(acgTChannelParameters *pParams); 

    /** @} */

    /**
     * @defgroup acgOpenChannel acgOpenChannel
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Open a DSP channel.
     * @{
     */

    /**
     * <BR>
     * The @b acgOpenChannel function opens a DSP channel with the provided configuration parameters.
	 * When a channel is open it can generate and detect tones and other signals, but it does not generate
	 * RTP packets. In order to activate RTP, the application should call ::acgActivateRTP as well.
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTChannelParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgOpenChannel(int Channel, acgTChannelParameters *pParams); 

    /** @} */


    /**
     * @defgroup acgUpdateChannel acgUpdateChannel
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Update a DSP channel.
     * @{
     */

    /**
     * <BR>
     * The @b acgUpdateChannel function updates a DSP channel with the provided configuration parameters.
	 * When a channel is open and not RTP activated, acgUpdateChannel API will not activate
	 * the RTP.
	 * When a channel is already RTP activated (by calling acgActivateRTP), calling acgUpdateChannel will
	 * remain the channel RTP activated.
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTChannelParameters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgUpdateChannel(int Channel, acgTChannelParameters *pParams); 

    /** @} */


    /**
     * @defgroup acgCloseChannel acgCloseChannel
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Close a DSP channel.
     * @{
     */

    /**
     * <BR>
     * The @b acgCloseChannel function closes a DSP channel and sets it to idle state.
	 *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgCloseChannel(int Channel); 
int Initialize_ssrc(int stn_no);
int acg_init_gpio();
int acg_read_gpio(int gpio_pin);
    /** @} */


    /**
     * @defgroup acgActivateRTP acgActivateRTP
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Activate the DSP channel RTP stream.
     * @{
     */

    /**
     * <BR>
     * The @b acgActivateRTP function activates the RTP stream of a channel that was
	 * previously opended by a call to ::acgOpenChannel.
	 *
     * @param Channel			[in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgActivateRTP(int Channel); 

    /** @} */

    /**
     * @defgroup acgDeactivateRTP acgDeactivateRTP
     * @ingroup CHANNEL_CONFIGURATION_GENERAL
     * @brief 
     * Deactivates (closes) the DSP channel RTP stream.
     * @{
     */

    /**
     * <BR>
     * The @b acgActivateRTP function deactivates the RTP stream of a channel that was
	 * previously activated by a call to ::acgActivateRTP.
	 * Note that after calling ::acgDeactivateRTP the DSP channel is still open, until the
	 * application calls ::acgCloseChannel.
	 *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgDeactivateRTP(int Channel); 

    /** @} */


/***************************************** 
****								  *****
****    DSP Signal Generation API Functions	  *****
****								  *****
*****************************************/

    /**
     * @defgroup acgPlayTone acgPlayTone
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Play a tone towards the local TDM side or network side.
     * @{
     */

    /**
     * <BR>
     * The @b acgPlayTone function plays a single DTMF or Call Progress tone
	 * towards the local TDM (telephone) side or remote IP network side.
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTTone.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlayTone(int Channel, acgTTone *pTone);

    /** @} */


    /**
     * @defgroup acgPlayToneSequence acgPlayToneSequence
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Play a sequence of tones towards the local TDM side or network side.
     * @{
     */

    /**
     * <BR>
     * The @b acgPlayToneSequence function plays a sequence of DTMF or Call Progress tones
	 * towards the local TDM (telephone) side or remote IP network side.
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTToneSequence.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlayToneSequence(int Channel, acgTToneSequence *pTones);
    /** @} */


    /**
     * @defgroup acgPlayToneExtended acgPlayToneExtended
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Play a user defined combined tones towards the local TDM side or network side.
     * @{
     */

    /**
     * <BR>
     * The @b acgPlayToneExtended function plays a user defined combined tones
	 * towards the local TDM (telephone) side or remote IP network side.
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTToneExtened.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlayToneExtended(int Channel, acgTToneExtended *pTones);

    /** @} */

    /**
     * @defgroup acgStopTone acgStopTone
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Stop a tone if already been generated towards the local TDM side or network side.
     * @{
     */

    /**
     * <BR>
     * The @b acgStopTone function stops a single tone or a sequence of tones
	 *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStopTone(int Channel);


    /** @} */

    /**
     * @defgroup acgPlayMWI acgPlayMWI
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Activate MWI.
     * @{
     */

    /**
     * <BR>
     * The @b acgPlayMWI function plays message waiting indication
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgECallerIdServiceType.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlayMWI(int Channel, acgECallerIdServiceType CallerIdServiceType);


    /** @} */

    /**
     * @defgroup acgStopMWI acgStopMWI
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Deactivate MWI.
     * @{
     */

    /**
     * <BR>
     * The @b acgStopMWI function stops message waiting indication
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgECallerIdServiceType.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStopMWI(int Channel, acgECallerIdServiceType CallerIdServiceType);

    /** @} */

    /**
     * @defgroup acgPlayCallerID acgPlayCallerID
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Generate caller Id signal towards the local TDM.
     * @{
     */

    /**
     * <BR>
     * The @b acgPlayCallerID function generates caller Id signal
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTCallerId.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlayCallerID(int Channel, acgTCallerId *pCallerId);

    /** @} */

    /**
     * @defgroup acgStopCallerID acgStopCallerID
     * @ingroup SIGNAL_GENERATION
     * @brief 
     * Stop caller Id generation.
     * @{
     */

    /**
     * <BR>
     * The @b acgStopCallerID function stop caller Id generation
	 *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStopCallerID(int Channel);


    /** @} */

    /**
     * @defgroup acgStart3WayConference acgStart3WayConference
     * @ingroup MEDIA_HANDLING
     * @brief 
     * Start 3 way conference.
     * @{
     */

    /**
     * <BR>
     * The @b acgStart3WayConference function starts 3 way conference
	 *
     * @param Channel    [in] .
     * @param ConferenceChannelMate    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStart3WayConference(int Channel, int ConferenceChannelMate);


    /** @} */

    /**
     * @defgroup acgStop3WayConference acgStop3WayConference
     * @ingroup MEDIA_HANDLING
     * @brief 
     * Stop 3 way conference.
     * @{
     */

    /**
     * <BR>
     * The @b acgStop3WayConference function stop 3 way conference
	 *
     * @param Channel    [in] .
     * @param ConferenceChannelMate    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStop3WayConference(int Channel, int ConferenceChannelMate);


    /** @} */

    /**
     * @defgroup acgDecodeMedia acgDecodeMedia
     * @ingroup MEDIA_HANDLING
     * @brief 
     * Send media packet toward DSP.
     * @{
     */

    /**
     * <BR>
     * The @b acgDecodeMedia function sends media packets towards a DSP channel
	 *
     * @param Channel    [in] .
     * @param pParams    [in] ::acgTDecodeMedia.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgDecodeMedia(int Channel, acgTDecodeMedia *pDecodeMedia);


    /** @} */

    /**
     * @defgroup acgGetMedia acgGetMedia
     * @ingroup MEDIA_HANDLING
     * @brief 
     * Get media packet from DSP.
     * @{
     */

    /**
     * <BR>
     * The @b acgGetMedia function gets media packets from a DSP channel.
	 * Two polling methods are supported:
	 * - Blocking - the function will not return until a DSP media packet arrives
	 * - Non-blocking - the function will return immediately with an indication 
	 * whether a media packet is available or not.
	*
     * @param pParams    [out] ::acgTGetMedia.
     *    
     * @par Return values:
     * @li 1 = No packet available.
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetMedia( acgTGetMedia *pGetMedia);

    /** @} */


/****************************************
****								  ****
****		Speceific API Functions		  ****
****								  ****
****************************************/


    /**
     * @defgroup acgReinitializeEchoCanceller acgReinitializeEchoCanceller
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Reinitialize echo canceller.
     * @{
     */

    /**
     * <BR>
     * The @b acgReinitializeEchoCanceller function reinitializes the echo canceller,
     * EC will init internally and re-learn signals.
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgReinitializeEchoCanceller(int Channel);


    /** @} */

    /**
     * @defgroup acgDebugInformationActivation acgDebugInformationActivation
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Debug information activation.
     * @{
     */

    /**
     * <BR>
     * The @b acgDebugInformationActivation function sets which debug information will be activated or deactivated.
     * The recorded packet will be send to remote IP and port which has 
     * been set in acgInitializeDSP().
     * @param pParams    [in] ::acgTDebugInformation.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgDebugInformationActivation(acgTDebugInformation *pDebugInformation);


    /** @} */

    /**
     * @defgroup acgStartFax acgStartFax
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Start fax.
     * @{
     */

    /**
     * <BR>
     * The @b acgStartFax function activates fax
     *
     * @param Channel    [in] .
     * @param pParams   [in] ::acgEFaxModemTransportMethod.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStartFax(int Channel, acgEFaxModemTransportMethod FaxModemTransportMethod);


    /** @} */

     /**
     * @defgroup acgStopFax acgStopFax
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Stop fax.
     * @{
     */

    /**
     * <BR>
     * The @b acgStopFax function stops fax
     *
     * @param Channel    [in] .
     * @param pParams   [in] ::acgEFaxModemTransportMethod.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStopFax(int Channel, acgEFaxModemTransportMethod FaxModemTransportMethod);


    /** @} */

     /**
     * @defgroup acgSetRegularRTCPParameters 
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Stop fax.
     * @{
     */

    /**
     * <BR>
     * The @b acgSetRegularRTCPParameters function set regular rtcp parameters
     *
     * @param Channel    [in] .
     * @param pParams   [in] ::acgTRegularRTCPParamters.
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */
int acgSetRegularRTCPParameters(int Channel, acgTRegularRTCPParamters *pRegularRTCPParameters);


    /** @} */

     /**
     * @defgroup acgGetRegularRTCPParameters 
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Stop fax.
     * @{
     */

    /**
     * <BR>
     * The @b acgSetRegularRTCPParameters function get regular rtcp parameters
     *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */
int acgGetRegularRTCPParameters(int Channel);


    /** @} */

     /**
     * @defgroup acgSetNtpTimeStamp 
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * @{
     */

    /**
     * <BR>
     * The @b acgSetNtpTimeStamp function get ntp time stamp
     *
     * @param Channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */
int acgSetNtpTimeStamp(int Channel, acgTNtpTimeStamp *pNtpTimeStamp);



    /** @} */


/***************************************
****								****
**** 	Event Handling API Functions	   	****
****								****
****************************************/

    /**
     * @defgroup acgGetEvent acgGetEvent
     * @ingroup EVENT_HANDLING
     * @brief 
     * Get a DSP event.
     * @{
     */

    /**
     * <BR>
     * The @b acgGetEvent function gets the next DSP event. Two polling methods are supported:
	 * - Blocking - the function will not return until a DSP event occurs
	 * - Non-blocking - the function will return immediately with an event or no-event indication
	 *
     * @param pParams   [in] ::acgTEventInfo.
     * @param pParams    [in] ::acgEPollingMethod
     
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod);

    /** @} */

    /**
     * @defgroup acgGetTelephonyEvent acgGetTelephonyEvent
     * @ingroup EVENT_HANDLING
     * @brief 
     * Get a telephony (FXS) event.
     * @{
     */

    /**
     * <BR>
     * The @b acgGetTelephonyEvent function gets the next FXS event. Two polling methods are supported:
	 * - Blocking - the funciton will not return until a DSP event has occured
	 * - Non-blocking - the funciton will return immediately with an event or no-event indication
	 *
     * @param pParams   [in] ::acgTEventInfo.
     * @param pParams    [in] ::acgEPollingMethod
     *
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetTelephonyEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod);

    /** @} */

	

/******************************
***							***
*** 		SLIC API Functions	   	***
***							***
*******************************/


    /**
     * @defgroup acgInitializeFXS acgInitializeFXS
     * @ingroup SLIC_CONFIGURATION_GENERAL
     * @brief 
     * Initialize the SLIC device.
     * @{
     */

    /**
     * <BR>
     * The @b acgInitializeFXS function initializes the SLIC device
	 *
     * @param pParams   [in] ::acgTFXSInitializationParameters.
     *
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgInitializeFXS(acgTFXSInitializationParameters	*pFXSInitializationParameters);

    /** @} */

    /**
     * @defgroup acgCloseFXS acgCloseFXS
     * @ingroup SLIC_CONFIGURATION_GENERAL
     * @brief 
     * Close the SLIC device.
     * @{
     */

    /**
     * <BR>
     * The @b acgCloseFXS function closes the SLIC device
     *
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgCloseFXS(void);

    /** @} */

	

    /**
     * @defgroup acgStartRing acgStartRing
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Start ring on a FXS port.
     * @{
     */

    /**
     * <BR>
     * The @b acgStartRing function starts ringing on a FXS port
	 *
     * @param channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStartRing(int channel);

    /** @} */


    /**
     * @defgroup acgStopRing acgStopRing
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Stop ring on a FXS port.
     * @{
     */

    /**
     * <BR>
     * The @b acgStopRing function stops ringing on a FXS port
	 *
     * @param channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgStopRing(int channel);

    /** @} */


  /** @} */


    /**
     * @defgroup acgInitializeDAA acgInitializeDAA
     * @ingroup EVENT_HANDLING
     * @brief 
     * Initialize the DAA device.
     * @{
     */

    /**
     * <BR>
     * The @b acgInitializeDAA function initializes the DAA device
	 *
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgInitializeDAA(void);

    /** @} */

	
    /**
     * @defgroup acgGetDAAEvent acgGetDAAEvent
     * @ingroup EVENT_HANDLING
     * @brief 
     * Get a telephony (FXO) event.
     * @{
     */

    /**
     * <BR>
     * The @b acgGetDAAEvent function gets the next FXO event. Two polling methods are supported:
	 * - Blocking - the function will not return until a DSP event has occured
	 * - Non-blocking - the funciton will return immediately with an event or no-event indication
	 *
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetDAAEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod);


    /** @} */


    /**
     * @defgroup acgSeizeLine acgSeizeLine
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Seize a DAA port.
     * @{
     */

    /**
     * <BR>
     * The @b acgSeizeLine function seizes a DAA port
	 *
     * @param channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgSeizeLine(int channel);



    /** @} */


    /**
     * @defgroup acgReleaseLine acgReleaseLine
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Release a DAA port.
     * @{
     */

    /**
     * <BR>
     * The @b acgReleaseLine function releases a DAA port
	 *
     * @param channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */


int acgReleaseLine(int channel);

    /** @} */


    /**
     * @defgroup acgGenerateFlash acgGenerateFlash
     * @ingroup SPECIFIC_FUNCTIONS
     * @brief 
     * Generate flash hook toward DAA.
     * @{
     */

    /**
     * <BR>
     * The @b acgGenerateFlash function generates flash hook towards a DAA port
	 *
     * @param channel    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */


int acgGenerateFlash(int channel);




    /** @} */

    /**
     * @defgroup acgCloseFXS acgCloseFXS
     * @ingroup SLIC_CONFIGURATION_GENERAL
     * @brief 
     * Close the SLIC device.
     * @{
     */

    /**
     * <BR>
     * The @b acgCloseFXS function closes the SLIC device
     *
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgCloseDAA(void);

    /** @} */


/**
     * @defgroup acgGetVersionInfo acgGetVersionInfo
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Get firmware version info from dsp driver.
     * @{
     */

    /**
     * <BR>
     * The @b Get firmware version info from dsp driver
	 *
     * @param pGetVersionInfo    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgGetVersionInfo(acgTGetVersionInfo *pGetVersionInfo);

    /** @} */	

/**
     * @defgroup acgPlaybackStart acgPlayback
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Get firmware version info from dsp driver.
     * @{
     */

    /**
     * <BR>
     * The @b Get firmware version info from dsp driver
     *
     * @param pGetVersionInfo    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlaybackStart(int Channel, int pbType, acgECoder	coder, char *pstrFileName);

    /** @} */
        
/**
     * @defgroup acgPlaybackEnd acgPlayback
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Get firmware version info from dsp driver.
     * @{
     */

    /**
     * <BR>
     * The @b Get firmware version info from dsp driver
     *
     * @param pGetVersionInfo    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgPlaybackEnd(int Channel);

    /** @} */
        
/**
     * @defgroup acgPlaybackDataGet acgPlayback
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Get firmware version info from dsp driver.
     * @{
     */

    /**
     * <BR>
     * The @b Get firmware version info from dsp driver
     *
     * @param pGetVersionInfo    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

void acgPlaybackDataGet(int getsize, int Channel, unsigned char *pDataRead);
        
    /** @} */
        
/**
     * @defgroup acgTimeSlotManipulation acgPlayback
     * @ingroup SLIC_TELEPHONY_COMMANDS
     * @brief 
     * Get firmware version info from dsp driver.
     * @{
     */

    /**
     * <BR>
     * The @b performs PCM highway timeslot manipulation
     *
     * @param pAttr    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */

int acgTimeSlotManipulation (/*int Channel,*/ acgTTimeSlotManipulationAttr *pAttr);


    /** @} */

    /**
     * @defgroup acgNWayConferenceOpen acgNWayConferenceOpen
     * @ingroup MEDIA_HANDLING
     * @brief 
     * open a N way conference.
     * @{
     */

    /**
     * <BR>
     * The @b acgNWayConferenceOpen function open a N way conference
	 *
     * @param ConfChannel    [in] .
     * @param pNWayConference    [in] .
     *    
     * @par Return values:
     * @li 0 = Successful.
     * @li Negative Value = An Error Occured
     * 
     * @req
     * @b Header: Declared in acGatewayAPI.h.
     */
#endif /*_ACGATEWAYAPI_H*/

