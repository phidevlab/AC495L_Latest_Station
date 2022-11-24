/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetTx.h                                 *
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

#ifndef	__IPHONE_NET_TX_h
#define	__IPHONE_NET_TX_h

/****************************************************************************
*                                                                            
*	Name:		networking_rtpSocketSend			 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:		sending RTP+T.38 packets to network 
*----------------------------------------------------------------------------                                                                            
*	Input:		 
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of error -1 else 0                           
******************************************************************************/
int networking_rtpSocketSend(char* buff, int len, int channel);

int networking_transmitStunReq(char* buff,
                                                              int len,
                                                              int socketSfd,
                                                              char *remoteAddress,
                                                              int remotePort);


#endif /* __IPHONE_NET_TX_h */




