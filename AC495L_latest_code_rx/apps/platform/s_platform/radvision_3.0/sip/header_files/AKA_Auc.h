/*      Copyright (c) 2001,2002 RADVISION Inc. and RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Inc. and RADVISION Ltd.. No part of this document may be
reproduced in any form whatsoever without written prior approval by
RADVISION Inc. or RADVISION Ltd..

RADVISION Inc. and RADVISION Ltd. reserve the right to revise this
publication and make changes without obligation to notify any person of
such revisions or changes.
***********************************************************************/

/***********************************************************************
Filename   : AKA_Auc.c
Description: Authentication Center implementation.
             This file demonstrate the behavior of an Authentication Center.
             It creates the authentication-vector using AKA functions.

    Author                         Date
    ------                        ------
    XXX XXXX                     20040301

************************************************************************/
#if !defined(_AKA_Av_h_)
#define _AKA_Av_h_ 1
/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
/*#include "CommonAppStackUtil.h" */

/*-----------------------------------------------------------------------*/
/*                   Definitions & Constants                             */
/*-----------------------------------------------------------------------*/
typedef RvUint8 u8;

/* this type defines an Authentication Vector. the AKA_AuC functions can fill
   such an AV, by getting the correct key, SQN, and AMF(authentication managment field) */
typedef struct {
    u8 IK[16];   /*128 bits - Integrity Key */
    u8 CK[16];   /*128 bits - Cipher Key */
    u8 AUTN[16]; /*128 bits - Authentication Token */
    u8 RAND[16]; /*128 bits - Random Challenge */
    u8 XRES[16]; /*4-16 octets(=128 bits) */
}AKA_Av;


/********************************************************************************************
 * AkaCreateAVAndRand
 * purpose : Create an Authentication vector.
 *           The function gets the secret KEY(k), SQN and AMF (authentication management field)
 *           parameters, and generates the AV members:
 *           RAND, IK, CK, XRES, AUTN.
 *           
 * input   : k - secret key.
 *           sqn - SQN value.
 *           amf - AMF value.
 * output  : pAv - The Authentication vector struct, filled with information.
 * return  : none.
 ********************************************************************************************/
void AkaCreateAVAndRand(IN u8 k[16], IN u8 sqn[6], IN u8 amf[2], 
                                     OUT AKA_Av* pAV);

/********************************************************************************************
 * AkaCreateAV
 * purpose : fill an Authentication vector which already has a RAND value.
 *           The function gets the secret KEY(k), SQN and AMF (authentication management field)
 *           parameters, and generates the AV members:
 *           RAND, IK, CK, XRES, AUTN.
 *           
 * input   : k - secret key.
 *           sqn - SQN value.
 *           amf - AMF value.
 * output  : pAv - The Authentication vector struct, filled with information.
 * return  : none.
 ********************************************************************************************/
void AkaCreateAV(IN u8 k[16], IN u8 sqn[6], IN u8 amf[2], 
                              OUT AKA_Av* pAV);

/********************************************************************************************
 * AkaCreateClientRES
 * purpose : Creates the RES, IK and CK values, using the given RAND and AUTN values.
 *           Upon receipt of RAND and AUTN:
 *           1. first compute the anonymity key AK = f5K (RAND) and retrieve 
 *              the sequence number SQN = (SQN * AK) * AK.
 *           2. Next computes XMAC = f1(SQN || RAND || AMF) and compares this with MAC which is 
 *              included in AUTN. 
 *              If they are different, set the pbCorrectMac parameter to FALSE.
 *           3. Next the USIM verifies that the received sequence number SQN is in the correct range.
 *              (in our implementation - this is user decision).
 *           4. If the sequence number is considered to be in the correct range however, 
 *              compute RES = f2K (RAND).
 *           5. Finally the USIM computes CK=f3(RAND) and IK=f4(RAND). 
 * input   : rand - random number.
 *           k    - secret key (password).
 *           autn - AUTN value.
 * output  : pAv  - The Authentication vector struct, filled with information.
 *           pbCorrectMac - indication if MAC equals to XMAC.
 * return  : none.
 ********************************************************************************************/
void AkaCreateClientRES(IN u8 rand[16], IN u8 k[16], IN u8 autn[16], 
                                     OUT AKA_Av* pAV, OUT RvBool *pbCorrectMac);

/********************************************************************************************
 * AkaCreateAutsParameter
 * purpose : Create the AUTS parameter to set in Authentication-Info header.
 *           The function gets the secret KEY(k), SQN and AMF (authentication management field)
 *           parameters, and AV structure.
 *           it generates AUTS= SQN^AK|| MAC-S
 *           
 * input   : rand - random number.
 *           k    - secret key.
 *           autn - autn value.
 *           newSqn - new sqn value.
 * Output  : auts - The AUTS string. (not 64 encoded!!)
 * return  : none.
 ********************************************************************************************/
void AkaCreateAutsParameter(IN u8 rand[16], IN u8 k[16], IN u8 autn[16],
                                         IN u8 newSqn[6], 
                                         OUT u8 auts[14]);

/********************************************************************************************
 * AkaGetNewSqnFromAutsParameter
 * purpose : Gets the new SQN value from the AUTS parameter found in the Authorization header.
 *           
 * input   : rand - random number.
 *           k    - secret key.
 *           auts - auts value. (after B64 decode)
 *           
 * Output  : newSqn - new sqn value.
 ********************************************************************************************/
void AkaGetNewSqnFromAutsParameter(IN u8 rand[16], IN u8 k[16], IN u8 auts[14],
                                   OUT u8 newSqn[6] );
/********************************************************************************************
* AkaUseConfiguredOP
* purpose : Set the OP array to have a different values from the default values.
*           
* input   : op - ptr to array of 16 values
* output  : 
* return  : none.
********************************************************************************************/
void AkaUseConfiguredOP(IN u8 *op);
#endif /* _AKA_Av_h_ 1*/





