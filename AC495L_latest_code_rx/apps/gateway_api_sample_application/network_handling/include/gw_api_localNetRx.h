/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	gw_api_localNetRx.h                                 *
 *                                                                  *
 * Copyright (C) 2006 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 30/10/06 - Coding started.										*
 *																	*
 ********************************************************************/
/*****************************************************************************
*                                                                            
* 		DESCRIPTION:  		                                                             
*                                                                            
******************************************************************************/

#ifndef	__GW_API_NET_RX_h
#define	__GW_API_NET_RX_h

/****************************************************************************
*
*	Name:		acl_UDPrxJob
*----------------------------------------------------------------------------
*	Abstract:	the rx thread from the network to the dsp
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
void *acl_UDPrxJob(void *arg);
void setT38(int Channel);
void unsetT38(int Channel);
int IST38(int Channel);


#endif /* __GW_API_NET_RX_h */

