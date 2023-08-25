/******************************************************************************
Filename:    sipTls.h
Description: sip TLS support in IPP TK
*******************************************************************************
                Copyright (c) 2005 RADVISION Inc.
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION LTD.

RADVISION LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************/
#ifdef RV_CFLAG_TLS
#ifndef SIP_TLS_H
#define SIP_TLS_H

#include "RvSipTransportTypes.h"

#define TLS_STRING_SIZE             2048
#define TLS_SCHEME					"sips"

#include "rvcflags.h"


/* RvIppTransportTlsCfg
 * ------------------------------------------------------------------------
 * This structure contains the parameters for the configuration of the
 * TLS transport when that is used with the SIP Stack.
 * It is part of the general configuration of the MTF.
 */
typedef struct 
{
	RvUint16						stackTlsPort;
        /* Secure port used for TLS. Default: 5064. */

	RvString						stackTlsAddress;
        /* */

	RvInt16							stackNumOfTlsAddresses;
        /* */

	RvBool							tlsPostConnectAssertFlag;
        /* This parameter is not used by the TLS module at all. It can be
           used by the application for its own purposes.
           It defaults to RV_TRUE */

	RvSipTransportTlsMethod 		tlsMethod;
        /* The SSL methods that will be used in the application's TLS engine.
           Default: RVSIP_TRANSPORT_TLS_METHOD_TLS_V1. */

	RvSipTransportPrivateKeyType 	privateKeyType;
        /* Informs the engine of the type of private key it should use.
           Default: RVSIP_TRANSPORT_PRIVATE_KEY_TYPE_RSA_KEY */

    RvInt32                         certDepth;
        /* Defines the depth that an engine will consider legal in a certificate chain to
           which it is presented before it is considered invalid. Default: 5. */
} RvIppTransportTlsCfg;


/***************************************************************************
 * rvIppTlsStackConfigValidate
 * ------------------------------------------------------------------------
 * General: This function gets pointer to the stack configuration and makes 
 *			validations on the tls parameters configured. 
 *			If it is successfully validate, returns RV_OK.
 *			The sip stack parameter numOfTlsEngines is blocked to the user, 
 *			meaning, that for now, the IPPTK will handle one engine and ignore 
 *			any other number configured by the user.
 *			In addition, the parameter tcpEnabled is also blocked since it 
 *			must be set to RV_TRUE.
 *			No need to make parameters validation, because of the fact that the 
 *			sip stack already makes validation, and in case error, the return 
 *			code will be not equal to RV_OK.
 *
 * Return Value:
 *			RV_OK				- on successful operation
 *			RV_ERROR_UNKNOWN	- on error
 * ------------------------------------------------------------------------
 *  Arguments:
 *  Input:         pStackCfgOld - IPP TK default configuration
 *				   pStackCfgNew - User configuration
 *
 *  Output:        pStackCfgOld - will include user values for allowed parameters
 *				                  and default values for blocked parameters.
***************************************************************************/
RvStatus rvIppTlsStackConfigValidate(RvSipStackCfg *pStackCfgOld, RvSipStackCfg *pStackCfgNew);



/***************************************************************************
 * sipStackTlsConfigInit
 * ------------------------------------------------------------------------
 * General: Initialize SIP Stack configuration TLS parameters.
 *          These parameters are set by application. 
 * Return Value:
 *			RV_OK				
 * ------------------------------------------------------------------------
 *  Arguments:
 *  Input:         sipStackCfg - Pointer to Sip Stack configuration structure
 *				   stackHndl - Pointer to application's transport TLS Configuration
 *
 *  Output:        none
***************************************************************************/
RvStatus sipStackTlsConfigInit(RvSipStackCfg *sipStackCfg, RvIppTransportTlsCfg *transportTlsCfg);


/***************************************************************************
 * rvIppTlsInit
 * ------------------------------------------------------------------------
 * General: This function initializes the Tls security. In this function we 
 *			use openSSL to load the certificates and key, therefore, the files 
 *			are built in a format openSSL recognizes. 
 *			Keys and certificates can be read with different function and than 
 *			be passed to the engine configuration struct. Construct one TLS 
 *			client engine.
 *			The client engine holds a certificate of a trusted entity 
 *			(certificate authority).
 *
 * Return Value:
 *			RV_OK				- on successful operation
 *			RV_ERROR_UNKNOWN	- on error
 * ------------------------------------------------------------------------
 *  Arguments:
 *  Input:         pStackCfg - IPP TK configuration
 *					stackHndl - Pointer to sip stack handle
 *
 *  Output:        None
***************************************************************************/
RvStatus rvIppTlsInit(RvIppTransportTlsCfg *ippTlsCfg, RvSipStackHandle	stackHndl);



/***************************************************************************
* AppTransportConnectionTlsPostConnectionAssertionEv
* ------------------------------------------------------------------------
* General:  This callback is used to override the stack's default post connection
*           assertion. once a connection has completed the hand shake, it is
*           necessary to make sure that the certificate presented was indeed issued
*           for the address to which the connection was made. that assertion is
*           automatically done by the stack, if for some reason, the application would
*           like to over ride this assertion it can implement this callback.
*           In this example we always override the stack's decision.
* Return Value: -
* ------------------------------------------------------------------------
* Arguments:
* Input:   hConnection   - The handle of the connection that changed TLS state
*          hAppConnection - The application handle for the connection
*          strHostName   - A NULL terminated string, indicating the host name
*                          (IP/FQDN) that the connection was meant to connect to.
*          hMsg          - a message if the connection was asserted against a message.
* Output: pbAsserted     - Fill that boolean with the result of your assertion.
*                          RV_TRUE - indicated you asserted the connection, successfully.
*                          RV_FALSE - indicates the assertion failed. the connection
*                                     will be terminated automatically.
***************************************************************************/
void RVCALLCONV AppTransportConnectionTlsPostConnectionAssertionEv(
																   IN    RvSipTransportConnectionHandle             hConnection,
																   IN    RvSipTransportConnectionAppHandle          hAppConnection,
																   IN    RvChar*                                   strHostName,
																   IN    RvSipMsgHandle                             hMsg,
																   OUT   RvBool*                                   pbAsserted);
/***************************************************************************
* AppTransportConnectionTlsStateChanged
* ------------------------------------------------------------------------
* General:  This callback is used by SIP stack to notify the IPPTK on TLS
*           connection state changes. This callback is called only for
*           TLS state changes and not for connection state changes.
* Return Value: RvStatus
* ------------------------------------------------------------------------
* Arguments:
* Input:   hConnection  - The handle of the connection that changed TLS state
*          eState       - The connection state
*          eReason      - The reason for the state change
***************************************************************************/
RvStatus RVCALLCONV AppTransportConnectionTlsStateChanged(
														  IN    RvSipTransportConnectionHandle             hConnection,
														  IN    RvSipTransportConnectionAppHandle       hAppConnection,
														  IN    RvSipTransportConnectionTlsState              eState,
														  IN    RvSipTransportConnectionStateChangedReason eReason);
														  

/***************************************************************************
 * rvIppTLSSetCallBackFunctions
 * ------------------------------------------------------------------------
 * General: This function sets IPP TLS call back functions in the transport manager.
 *
 * Return Value:
 *			RV_OK				- on successful operation
 *			RV_ERROR_UNKNOWN	- on error
 * ------------------------------------------------------------------------
 *  Arguments:
 *  Input:         hTransportMgr          - handle to the Transport Manager.
 *                 sipTransportEvHandlers - hanlde to the Callbacks
 *  Output:        None
**************************************************************************/
RvStatus rvIppTLSSetCallBackFunctions(RvSipTransportMgrHandle		hTransportMgr,
                                  RvSipTransportMgrEvHandlers*	sipTransportEvHandlers);


/***************************************************************************
 * rvIppTlsGetPort
 * ------------------------------------------------------------------------
 * General: get a port to be used by IPP-TK on TLS transaction 
 * Parameters: none
 * Return: the port to be used by IPP-TK on TLS transaction 
 ***************************************************************************/
RvUint16 rvIppTlsGetPort(void);

#endif /* SIP_TLS_H*/
#endif /* RV_CFLAG_TLS */
