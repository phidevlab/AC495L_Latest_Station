/*********************************************************************************
 *                              <rvSipStunApi.h>
 *
 * This file contains the SIP STUN API which may be used by the User of the
 * IP Phone Toolkit, in order to tailor the behaviour to its needs,
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

#define STUN_APP_BODY_STRING_SIZE   2048
#define STUN_DEFAULT_PORT			5060
#define STUN_IP_STR_LEN             55

RV_DECLARE_HANDLE(RvIppStunMgrHandle);

typedef enum
{
	RVIPP_SDP_MEDIA_CONN_FIELD = 1,
	RVIPP_SDP_MEDIA_FIELD,
	RVIPP_SDP_ORIGIN_FIELD,
	RVIPP_SDP_ATTRIBUTE_FIELD,
	RVIPP_SIP_VIA_FIELD,
	RVIPP_SIP_CONTACT_FIELD,
	RVIPP_SDP_MEDIA_CLOSE_FIELD,
	RVIPP_SDP_MEDIA_CONN_CLOSE_FIELD

} RvIppSipAddressFieldType;




/*-----------------------------------------------------------------------*/
/*                        RvIppStunAddrData		                     */
/*-----------------------------------------------------------------------*/
typedef enum
{
	RvIppStunAddrDataStatus_Error = -1,
	RvIppStunAddrDataStatus_Request,
	RvIppStunAddrDataStatus_Response
} RvIppStunAddrDataStatus;


/* RvIppStunAddrData
 * ------------------------------------------------------------------------
 * Address translation data structure used by the MTF's STUN module.
 * This struct is used by the MTF to request the application to translate
 * a private address into a public one.
 * The application should view all of the fields in this struct as read
 * only fields, besides the outAddr field, which should be modified by the
 * application to the public address.
 */
typedef struct
{
	RvIppSipAddressFieldType	type;
        /* The type of address. This indicates from which part of the
           outgoing SIP message does the address come from */

	RvAddress					inAddr;
        /* The address that was found in the outgoing SIP message. This
           is the address that requires the translation. */

	RvAddress					outAddr;
        /* The address that the inAddr field translates to. The value
           of this field is filled in by the application, along with a
           call to RvIppStunAddressResolveComplete(). */

	RvInt16						index;
        /* The index of the address within the given SIP message header.
           This can be used by the application to distinguish between
           different addresses located in the same SIP header. The index
           value is 0-based. */

    /* The parameter below is not for the use of the application.
       It is used internally by the MTF. */
	RvIppStunAddrDataStatus		status;

} RvIppStunAddrData;


/******************************************************************************
*  RvIppStunIsAddressResolveNeededCB
*  ----------------------------
*  General :        Callback prototype which should be implemented by user's Stun Client.
*					Check if STUN resolution is required for SIP messages sent
*					to concrete destination endpoint.
*					Implies that this destination resides outside the private network.
*
*
*  Return Value:    RvBool			rvTrue - if destination endpoint lies out of private network
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          addrDest         address of destination endpoint.
*       .
*
*  Output          None.
*
******************************************************************************/
typedef RvBool (RVCALLCONV *RvIppStunIsAddressResolveNeededCB)(IN RvAddress* addrDest);


/******************************************************************************
*  RvIppStunAddressResolveStartCB
*  ----------------------------
*  General :        Callback prototype which should be implemented by user's Stun Client.
*					Starts STUN resolution procedure for a specific {ip:port} pair.
*
*
*  Return Value:    RvStatus			RV_OK - if user started STUN resolution procedure.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			addrData        data describing STUN request.
*									Only addrData.inAddr is relevant
*
*  Output          None
*
******************************************************************************/
typedef RvStatus (RVCALLCONV *RvIppStunAddressResolveStartCB)(IN RvIppStunAddrData* addrData);


/******************************************************************************
*  RvIppAddressResolveReplyCB
*  ----------------------------
*  General :        Callback prototype which  may be implemented by user's Stun Client.
*					It must be implemented if user wants to receive replies from STUN server to Sip signaling socket.
*					This callback will be called for each message received by Sip signaling socket.
*					The callback should test if a current message is received from STUN server,
*                   handle this message, and discard one.
*
*
*  Return Value:    RV_OK if success.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			buf				received data.
*					size			size of received data.
*
*
*  Output          bDiscardMsg		RV_TRUE if this buffer should be discarded.
*
******************************************************************************/
// TODO: remove first 2 parameters of callback when done
typedef RvStatus (RVCALLCONV *RvIppAddressResolveReplyCB)(
    IN  RvAddress*                          addr,
    IN  RvChar*                             buf,
    IN  RvSize_t                            size,
	OUT RvBool*                             bDiscardMsg);

/******************************************************************************
*  RvIppStunAddressResolveComplete
*  ----------------------------
*  General :        This API should be called by user's Stun Client when STUN resolution procedure
*					for concrete {ip:port} has completed or failed.
*
*
*  Return Value:    RV_OK if success.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			addrData        data describing STUN response.
*									addrData.inAddr - resolution is request for.
*									addrData.outAddr- "mapped to" response of STUN server.
*					bStatusOK		rvTrue if STUN resolution procedure succeeded, else - rvFalse.
*
*
*  Output          None.
*
******************************************************************************/
RVAPI RvStatus RVCALLCONV RvIppStunAddressResolveComplete(
    IN RvIppStunAddrData*   addrData,
    IN RvBool               bStatusOK);


/* RvIppStunMgrParam
 * ------------------------------------------------------------------------
 * This structure contains the parameters for the configuration of the
 * STUN module in the MTF.
 */
typedef	struct
{
	RvIppStunIsAddressResolveNeededCB	isAddressResolveNeededCB;
        /* Callback function to check if an address resolution is required or not. */
	
	RvIppStunAddressResolveStartCB		addressResolveStartCB;
        /* Callback function requests for address resolution of private addresses to
           public ones. */

	RvIppAddressResolveReplyCB			addressResolveReplyCB;
        /* Callback function enabling to  test if buffer received on the SIP signaling
           socket is sent by the STUN server.
	       If user's STUN client always requests replies to be sent to its own socket
           then this parameter should be set NULL. */

} RvIppStunMgrParam;



/******************************************************************************
*  RvIppStunMgrCreate
*  ----------------------------
*  General :        Initialize STUN support inside toolkit - StunMgr object.
*
*
*  Return Value:    Handle of StunMgr object.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			param        parameters which StunMgr object to be initialized with.
*
*
*  Output          None.
*
******************************************************************************/
RVAPI RvIppStunMgrHandle RVCALLCONV RvIppStunMgrCreate(IN RvIppStunMgrParam* param);


/******************************************************************************
*  RvIppStunMgrDelete
*  ----------------------------
*  General :        Stops STUN support inside toolkit - destroy StunMgr object.
*
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			Handle of StunMgr object.
*
*
*  Output          None.
*
******************************************************************************/
RVAPI void RVCALLCONV RvIppStunMgrDelete(IN RvIppStunMgrHandle stunMgrHndl);


/******************************************************************************
*  RvIppStunMgrGetSendMethod
*  ----------------------------
*  General :        This API  is used when user's Stun Client tries to send a STUN message over
*					Sip signaling socket in order to get NAT mapping of its address.
*
*
*  Return Value:    RV_OK if success.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:			stunMgr		StunMgr handle
*
*  Output          method        pointer to structure containing a function should be called in order to send buffer over socket.
*
******************************************************************************/
RVAPI void RVCALLCONV RvIppStunMgrGetSendMethod(
    IN  RvIppStunMgrHandle  stunMgr,
    OUT SendMethod**        method);
/*---------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* RV_SIP_STUN_API_H*/
