/*********************************************************************************
 *                              <rvSipStunApi.h>
 *
 * This file contains the SIP STUN API, which may be used by IP Phone Toolkit
 * users in order to customize its behaviour to its needs.
 *
 *
 * The SIP STUN  API functions are grouped as follows:
 *
 *
 *    Author                         Date
 *    ------                        ------
 *
 *********************************************************************************/

#ifndef RV_SIP_STUN_API_H
#define RV_SIP_STUN_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rvaddress.h"
#include "RvSipTransportTypes.h"

/*@*****************************************************************************
 * Package: RvMtfStunPkg (root)
 * -----------------------------------------------------------------------------
 * Title: STUN integration
 *
 * General: This section contains functions and type definitions that are used 
 *  		for integrating a STUN client with the MTF.
 ****************************************************************************@*/

#define STUN_APP_BODY_STRING_SIZE   2048
#define STUN_DEFAULT_PORT			5060
#define STUN_IP_STR_LEN             55

RV_DECLARE_HANDLE(RvIppStunMgrHandle);


/*@*****************************************************************************
 * Enum: RvIppSipAddressFieldType (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the address header type in
 * SIP messages: a SIP header, or a SDP header that contains an address.
 * The application uses this value to decide on which port to send the STUN
 * request. SDP headers will be sent over RTP or RTCP ports, and SIP headers
 * will be sent over SIP ports.
 ****************************************************************************@*/
typedef enum
{
	RVIPP_SDP_MEDIA_CONN_FIELD = 1,
	/* Indicates the SDP "c=" line under the media description section. */
	RVIPP_SDP_MEDIA_FIELD,
	/* Indicates the SDP "m=" line, which is the media description section. */
	RVIPP_SDP_ORIGIN_FIELD,
	/* Indicates the SDP "o=" line under the session section. */
	RVIPP_SDP_ATTRIBUTE_FIELD,
	/* Indicates the SDP "a=" line under the session section or the media
	   description section. */
	RVIPP_SIP_VIA_FIELD,
	/* Indicates the Via header in the SIP message. */
	RVIPP_SIP_CONTACT_FIELD,
	/* Indicates the Contact header in the SIP message. */
	RVIPP_SDP_MEDIA_CLOSE_FIELD,
	/* Indicates the SDP "m=" line, which is the media description section. */
	RVIPP_SDP_MEDIA_CONN_CLOSE_FIELD
	/* Indicates the SDP "c=" line under the media description section. */

} RvIppSipAddressFieldType;

/*@*****************************************************************************
 * Enum: RvIppStunAddrDataStatus (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the type of STUN message: request or
 * response.
 ****************************************************************************@*/
typedef enum
{
	RvIppStunAddrDataStatus_Error = -1,
	RvIppStunAddrDataStatus_Request,
	RvIppStunAddrDataStatus_Response
} RvIppStunAddrDataStatus;

/*@*****************************************************************************
 * Type: RvIppStunAddrData (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes address translation data used by the
 * MTF's STUN module.
 * It is used by the MTF to request that the application translate a private
 * address into a public one.
 * All fields in this structure are considered read-only fields, except for the
 * outAddr field, which the application has to modify to the public address.
 ****************************************************************************@*/
typedef struct
{
	RvIppSipAddressFieldType	type;
        /* The type of address. This indicates the part of the outgoing SIP
	   message from which the address is taken. */

	RvAddress			inAddr;
        /* The address, found in the outgoing SIP message, that requires the
	   translation. */

	RvAddress			outAddr;
        /* The address to which the inAddr field translates. The value of this
           field is filled in by the application, along with a call to
           RvIppStunAddressResolveComplete(). */

	RvInt16				index;
        /* The index of the address within the given SIP message header.
           The application uses this value to distinguish between different
           addresses located in the same SIP header. The index value is
           0-based. */

	RvIppStunAddrDataStatus		status;
	/* This parameter is used transparently by the MTF, but is not used by
	   the application. */

} RvIppStunAddrData;

/*@*****************************************************************************
 * RvIppStunIsAddressResolveNeededCB (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	Enables support for STUN in the terminal.
 *	Called when the application needs to check whether the STUN resolution
 *	is required for a certain destination address. The application may
 *	decide that the STUN resolution is not needed if the destination address
 * 	resides inside a private network.
 *
 * Arguments:
 *
 * Input:  addrDest    - Destination party address.
 *
 * Return Value: RV_TRUE if resolution is required.
		 RV_FALSE if resolution is not required.
 ****************************************************************************@*/
typedef RvBool (RVCALLCONV *RvIppStunIsAddressResolveNeededCB)(IN RvAddress* addrDest);

/*@*****************************************************************************
 * RvIppStunAddressResolveStartCB (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	The application should implement this callback to enable support
 *	for STUN in the terminal. Starts the STUN resolution procedure for a 
 *  specific {ip:port} pair.
 *
 * Arguments:
 *
 * Input:  addrData    - Data that describes the STUN request.
 *
 * Return Value: RV_OK if the user started a STUN resolution procedure
 *               successfully, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvIppStunAddressResolveStartCB)(IN RvIppStunAddrData* addrData);

/*@*****************************************************************************
 * RvIppAddressResolveReplyCB (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
*	Enables support for STUN in the terminal.
 *	Relevant for the application to receive replies from the STUN server to
 *	the SIP signaling socket. This callback is used for each message received
 *  by the SIP signaling socket. It tests whether the current message was
 * 	received from the STUN server, then processes this message, and discards
 * 	the message if required.
 *
 * Arguments:
 *
 * Input:  addrData    - Data describing the STUN request.
 *
 * Output: bDiscardMsg - RV_TRUE if this buffer should be discarded.
 *
 * Return Value: RV_OK if the user started a STUN resolution procedure
		         successfully, other if not.
 ****************************************************************************@*/
/* TODO: remove first 2 parameters of callback when done*/
typedef RvStatus (RVCALLCONV *RvIppAddressResolveReplyCB)(
    IN  RvAddress*                          addr,
    IN  RvChar*                             buf,
    IN  RvSize_t                            size,
	OUT RvBool*                             bDiscardMsg);

/*@*****************************************************************************
 * RvIppStunAddressResolveComplete (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	This function is run when the STUN resolution procedure for a specific
 * 	{ip:port} is complete.
 *
 * Arguments:
 *
 * Input:	addrData    	- Data describing the STUN request:
 *		addrData.inAddr     - Address requiring resolution.
 *		addrData.outAddr    - "Mapped to" response of the STUN server.
 *		bStatusOK	        - RV_TRUE if the STUN resolution procedure succeeded,
 *				              RV_FALSE if the STUN resolution failed.
 *
 * Return Value: RV_OK if the procedure is successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV RvIppStunAddressResolveComplete(
    IN RvIppStunAddrData*   addrData,
    IN RvBool               bStatusOK);


/*@*****************************************************************************
 * Type: RvIppStunMgrParam (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes the parameters for the MTF STUN
 * module configuration.
 ****************************************************************************@*/
typedef	struct
{
	RvIppStunIsAddressResolveNeededCB	isAddressResolveNeededCB;
        /* Checks whether an address resolution is required or not. */

	RvIppStunAddressResolveStartCB		addressResolveStartCB;
        /* Requests for address resolution of private addresses to public. */

	RvIppAddressResolveReplyCB			addressResolveReplyCB;
        /* Enables testing if the buffer received on the SIP signaling socket
 	   was sent by the STUN server.
	   If the user's STUN client always requests to send replies to its own
	   socket, set this parameter to NULL. */

} RvIppStunMgrParam;


/*@*****************************************************************************
 * RvIppStunMgrCreate (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	Initializes the STUN module in the MTF. This function includes only objects
 * 	required by the MTF for integration between the MTF and the STUN client.
 * 	It does not include initialization of the STUN client.
 *
 * Arguments:
 *
 * Input:	param    - Configuration parameters of the MTF STUN module.
 *
 * Return Value: Handle for the STUN module object (StunMgr).
 ****************************************************************************@*/
RVAPI RvIppStunMgrHandle RVCALLCONV RvIppStunMgrCreate(IN RvIppStunMgrParam* param);

/*@*****************************************************************************
 * RvIppStunMgrDelete (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	Terminates the STUN module in the MTF. This function includes only objects
 * 	required by the MTF for integration between the MTF and the STUN client.
 * 	It does not include the termination of the STUN client.
 *
 * Arguments:
 *
 * Input:	stunMgrHndl    - Handle for the STUN module object (StunMgr).
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV RvIppStunMgrDelete(IN RvIppStunMgrHandle stunMgrHndl);

/*@*****************************************************************************
 * RvIppStunMgrGetSendMethod (RvMtfStunPkg)
 * -----------------------------------------------------------------------------
 * General:
 *	This function is run when the application's STUN client tries to send a STUN
 *	message over the SIP signaling port to get the NAT mapping for
 *	its address.
 *
 * Arguments:
 *
 * Input:	stunMgr		- Handle for the STUN module object (StunMgr).
 *
 * Output: 	method		- Pointer to a structure containing a function
 *				          that is called to send a buffer over a socket.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV RvIppStunMgrGetSendMethod(
    IN  RvIppStunMgrHandle  stunMgr,
    OUT SendMethod**        method);



#ifdef __cplusplus
}
#endif

#endif /* RV_SIP_STUN_API_H*/
