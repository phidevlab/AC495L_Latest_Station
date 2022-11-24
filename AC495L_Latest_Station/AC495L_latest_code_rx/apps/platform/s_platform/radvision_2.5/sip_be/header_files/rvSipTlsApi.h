#ifdef RV_CFLAG_TLS
#ifndef RV_SIP_TLS_API_H
#define RV_SIP_TLS_API_H
/*********************************************************************************
 *                              <rvSipTlsApi.h>
 *
 * This file contains the SIP TLS API which may be used by the User of the
 * IP Phone Toolkit, in order to tailor the behaviour to its needs,
 *
 *
 * The SIP TLS  API functions are grouped as follows:
 *
 *********************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "RvSipStackTypes.h"
#include "rvSipControlApi.h"


/* RvIppTlsBufferType
 * ------------------------------------------------------------------------
 * The type of data required by the TLS module to complete its negotiation.
 */
typedef enum
{
	IPP_TLS_UNKNOWN_BUFFER_TYPE = -1,
        /* Unknown buffer type */

	IPP_TLS_CLIENT_KEY_BUFFER,
        /* */

	IPP_TLS_CA_BUFFER,
        /* */

	IPP_TLS_SERVER_KEY_BUFFER,
        /* */

	IPP_TLS_SERVER_CA_BUFFER
        /* */
} RvIppTlsBufferType;

/******************************************************************************
*  RvIppTlsGetBufferCB
*  --------------------------------
*  General :       Callback prototype which retrieves certificate or Key buffer
*                  .
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          
*                  bufferType            - type of buffer to retrieve
*
*  Output:         
*                  tlsBuffer             - placeholder for buffer
*				   tlsBufferLen          - length of buffer 
******************************************************************************/
typedef RvBool (*RvIppTlsGetBufferCB)(IN    RvIppTlsBufferType bufferType,
									OUT	RvChar			*tlsBuffer,
									OUT	RvUint32		*tlsBufferLen);


/***************************************************************************
* RvIppSipTlsPostConnectionAssertionCB
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
typedef void (*RvIppSipTlsPostConnectionAssertionCB)(IN    RvSipTransportConnectionHandle             hConnection,
													 IN    RvSipTransportConnectionAppHandle          hAppConnection,
													 IN    RvChar*                                   strHostName,
													 IN    RvSipMsgHandle                             hMsg,
													 OUT   RvBool*                                   pbAsserted);




/* RvIppSipTlsExtClbks
 * ------------------------------------------------------------------------
 * This structure contains the callbacks associated with TLS that can be used
 * for extensibility purposes by the application.
 */
typedef struct
{
	RvIppTlsGetBufferCB						tlsGetBufferF;
        /* Retrieve a certificate or a key buffer */

	RvIppSipTlsPostConnectionAssertionCB	tlsPostConnectionAssertF;
        /* Override the MTF's assertion regarding the connection's certificate */
} RvIppSipTlsExtClbks;




/******************************************************************************
*  rvIppSipTlsInitConfig
*  --------------------------------
*  General :        Set default TLS values into Sip phone configuration structure
*
*  Return Value:   None
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          cfg       Pointer to Sip phone configuration structure
*  Output:         none.
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipTlsInitConfig(IN RvIppSipPhoneCfg* cfg);

/******************************************************************************
*  rvIppSipTlsRegisterExtClbks
*  --------------------------------
*  General :        Registers SIP TLS user callbacks
*                   Should be called before rvIppSipStackInitialize()
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          clbks              Structure includes pointers to user implementations
*
*  Output:         none.
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipTlsRegisterExtClbks(IN RvIppSipTlsExtClbks* clbks);



#ifdef __cplusplus
}
#endif

#endif /* RV_SIP_TLS_API_H*/
#endif /* RV_CFLAG_TLS */
