/******************************************************************************
Filename:    ippcodec_g729.h
Description:
*******************************************************************************/
#include "rvsdpmedia.h"
#include "ippcodec.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	
/*******************************************************************************/
typedef	struct {
	int			ptime;
	RvBool		annexa;
	RvBool		annexb;
}CodecData_g729;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_g729_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_g729 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_g729*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_g729_Construct( OUT CodecData_g729*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/*******************************************************************************/
RVAPI void CodecData_g729_Destruct( OUT CodecData_g729*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
