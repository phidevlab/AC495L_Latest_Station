#ifndef SIP_AUTH_CLIENT_H
#define SIP_AUTH_CLIENT_H

#include "RvSipStackTypes.h"
#include "RvSipStack.h"
#include "rpool_API.h"


/*---- E V E N T    H A N D L E R S   I M P L M E N T A T I O N ----------*/	  

void RVCALLCONV AppAuthenticationMD5Ev(IN  RPOOL_Ptr            *pRpoolMD5Input,
                                              IN  RV_UINT32     length,
                                              OUT RPOOL_Ptr     *pRpoolMD5Output);

void RVCALLCONV AppAuthenticationSharedSecretEv(
                                   IN  RvSipAddressHandle            hRequestUri,
                                   IN  RPOOL_Ptr                     *pRpoolRealm,
                                   OUT RPOOL_Ptr                     *pRpoolUserName,
                                   OUT RPOOL_Ptr                     *pRpoolPassword);

void RVCALLCONV AppAuthenticatorGetSharedSecretEv(
								   IN  RvSipAuthenticatorHandle       hAuthenticator,
								   IN  RvSipAppAuthenticatorHandle    hAppAuthenticator,
								   IN  void*                          notUsed1,
								   IN  void*                          notUsed2,
                                   IN  RPOOL_Ptr                     *pRpoolRealm,
                                   OUT RPOOL_Ptr                     *pRpoolUserName,
                                   OUT RPOOL_Ptr                     *pRpoolPassword);

#endif /*SIP_AUTH_CLIENT_H*/
