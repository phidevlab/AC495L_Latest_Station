/******************************************************************************
Filename:    ippcodec_g711.h
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
	/*private*/
#ifdef  MTF_SUPPORT_G711_ANNEX2
	RvBool		bAnnex2;
#endif
}CodecData_g711;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_g711_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_g711 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_g711*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_g711_Construct( OUT CodecData_g711*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/*******************************************************************************/
RVAPI void CodecData_g711_Destruct( OUT CodecData_g711*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
