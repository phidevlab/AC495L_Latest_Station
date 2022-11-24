/******************************************************************************
Filename:    ippcodec_g722.h
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
}CodecData_g722;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_g722_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_g722 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_g722*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_g722_Construct( OUT CodecData_g722*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/*******************************************************************************/
RVAPI void CodecData_g722_Destruct( OUT CodecData_g722*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
