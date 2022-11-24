#ifdef RV_CFLAG_TLS
#ifndef RV_SIP_TLS_API_H
#define RV_SIP_TLS_API_H
/*********************************************************************************
 *                              <rvSipTlsApi.h>
 *
 * This file contains SIP TLS API that may be used by users of the
 * IP Phone Toolkit to tailor its behavior to their needs.
 *
 *
 * The SIP TLS API functions are grouped as follows:
 *
 *********************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "RvSipStackTypes.h"
#include "rvSipControlApi.h"


/*@*****************************************************************************
 * Package: RvMtfTlsPkg (root)
 * -----------------------------------------------------------------------------
 * Title: TLS
 *
 * General: This section contains functions and type definitions that are used 
 *  		for enabling calls with TLS in the MTF.
 ****************************************************************************@*/


/*@*****************************************************************************
 * Enum: RvIppTlsBufferType (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumeration indicates the type of data required by the TLS
 * module to complete its negotiation.
 ****************************************************************************@*/
typedef enum
{
	IPP_TLS_UNKNOWN_BUFFER_TYPE = -1,
        /* Unknown buffer type. */

	IPP_TLS_CLIENT_KEY_BUFFER,
        /* Client key buffer type. */

	IPP_TLS_CA_BUFFER,
        /* Client CA certificate buffer type. */

	IPP_TLS_SERVER_KEY_BUFFER,
        /* Server key buffer type. */

	IPP_TLS_SERVER_CA_BUFFER
        /* Server CA certificate buffer type. */
} RvIppTlsBufferType;

/*@*****************************************************************************
 * RvIppTlsGetBufferCB (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Callback prototype for retrieving a certificate or key buffer.
 *
 * Arguments:
 *
 * Input:  bufferType  - Type of buffer to retrieve.
 *
 * Output: 	tlsBuffer  - Placeholder for the buffer.
 *		tlsBufferLen   - Length of the buffer.
 *
 * Return Value: True if retrieval is successful.
 ****************************************************************************@*/
typedef RvBool (*RvIppTlsGetBufferCB)(IN    RvIppTlsBufferType bufferType,
									OUT	RvChar			*tlsBuffer,
									OUT	RvUint32		*tlsBufferLen);

/*@*****************************************************************************
 * RvIppSipTlsPostConnectionAssertionCB (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *     Overrides the Stack's default post connection assertion.
 *     After a connection has completed the handshake, the Stack must
 *     automatically ensure that the certificate presented was actually
 *     issued for the successfully connected address.
 * 	   This callback is used only if the application needs to override this
 * 	   assertion.
 *
 * Arguments:
 *
 * Input:  hLine          - The handle of the connection that changed the TLS
 *                          state.
 *         hAppConnection - The application handle for the connection.
 *         strHostName    - A NULL-terminated string, indicating the destination
 *                          host name (IP/FQDN) of the connection.
 *         hMsg           - A message (if the connection was asserted against a
 * 			                message).
 *
 * Output: pbAsserted     - Boolean value indicating the result of the assertion.
 *                          RV_TRUE indicates a successful connection assertion.
 *                          RV_FALSE indicates the assertion failed, and the
 *                          connection will be terminated automatically.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*RvIppSipTlsPostConnectionAssertionCB)(
					 IN    RvSipTransportConnectionHandle             hLine,
					 IN    RvSipTransportConnectionAppHandle          hAppConnection,
					 IN    RvChar*                                   strHostName,
					 IN    RvSipMsgHandle                             hMsg,
					 OUT   RvBool*                                   pbAsserted);



/*@*****************************************************************************
 * Type: RvIppSipTlsExtClbks (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes the TLS callbacks that can be used to
 * enable TLS calls. These callbacks are provided during MTF initialization.
 ****************************************************************************@*/
typedef struct
{
	RvIppTlsGetBufferCB						tlsGetBufferF;
        /* Retrieve a certificate or a key buffer */

	RvIppSipTlsPostConnectionAssertionCB	tlsPostConnectionAssertF;
        /* Override the MTF's assertion regarding the certificate of the 
	       connection. */
} RvIppSipTlsExtClbks;


/*@*****************************************************************************
 * rvIppSipTlsInitConfig (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sets default values into the TLS configuration structure.
 *
 * Arguments:
 *
 * Input:  cfg   -    Pointer to the SIP Phone configuration structure.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvIppSipTlsInitConfig(IN RvIppSipPhoneCfg* cfg);

/*@*****************************************************************************
 * rvIppSipTlsRegisterExtClbks (RvMtfTlsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers SIP TLS user callbacks, and is called before
 *         rvIppSipStackInitialize().
 *
 * Arguments:
 *
 * Input:  clbks -  Structure that includes pointers to user implementations.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvIppSipTlsRegisterExtClbks(IN RvIppSipTlsExtClbks* clbks);



#ifdef __cplusplus
}
#endif

#endif /* RV_SIP_TLS_API_H*/
#endif /* RV_CFLAG_TLS */
