#ifndef SIP_REG_CLIENT_H
#define SIP_REG_CLIENT_H

#include "rvccterminalsip.h"
#include "rvccapi.h"
#include "RvSipStackTypes.h"
#include "RvSipStack.h"
#include "RvSipRegClientTypes.h"
#include "RvSipRegClient.h"


RvBool rvSipControlRegClientCreateObject(
    IN  RvCCProvider*           p,
    IN  RvCCTerminal*           xTerm,
    IN  const RvChar*           scheme,
    IN  RvUint16                port,
										 OUT RvSipRegClientHandle	*regClientObj);


RvStatus rvSipControlRegClientSend(RvSipRegClientHandle		hRegClient, RvCCTerminalSip * term);


RvStatus rvSipControlRegClientSetExpire(RvSipRegClientHandle	hRegClient, RvInt32 nExpire);

/***************************************************************************
 * rvSipControlRegClientSetTo
 * ------------------------------------------------------------------------
 * General: Set To value in Register Client Object
 *          
 * Return Value: RV_OK on success
 *				 RV_ERROR - on failure.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hRegClient -  The sip stack register-client handle
 *
 *			toStr	   -  the new to string
 *
 ***************************************************************************/
RvStatus rvSipControlRegClientSetTo(IN RvSipRegClientHandle	hRegClient,
									IN char*				toStr);

/*---- E V E N T    H A N D L E R S   I M P L M E N T A T I O N ----------*/

void RVCALLCONV AppRegClientStateChangedEvHandler(
                           IN  RvSipRegClientHandle            hRegClient,
                           IN  RvSipAppRegClientHandle         hAppRegClient,
                           IN  RvSipRegClientState             eState,
                           IN  RvSipRegClientStateChangeReason eReason);

RvStatus RVCALLCONV AppRegClientMsgReceivedEvHandler(
	                       IN  RvSipRegClientHandle          hRegClient,
                           IN  RvSipAppRegClientHandle       hAppRegClient,
                           IN  RvSipMsgHandle                hMsg);

RvStatus RVCALLCONV AppRegClientMsgToSendEvHandler(
	                       IN  RvSipRegClientHandle          hRegClient,
                           IN  RvSipAppRegClientHandle       hAppRegClient,
                           IN  RvSipMsgHandle                hMsg);

const RV_CHAR*  getRegClientStateName (IN  RvSipRegClientState  eState);

RvBool rvSipControlIsSameNonce(IN char* receivedNonce,const char* previousNonCe);
#endif /*SIP_REG_CLIENT_H*/
