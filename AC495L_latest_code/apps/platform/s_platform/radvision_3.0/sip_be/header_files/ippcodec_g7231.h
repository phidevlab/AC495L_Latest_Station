/******************************************************************************
Filename:    ippcodec_g7231.h
Description:
*******************************************************************************/
#include "rvsdpmedia.h"
#include "ippcodec.h"


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
typedef	struct {
	int			audioFrames;
	int			ptime;
	RvBool		bSilenceSupp;
	/*private*/
}CodecData_g7231;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_g7231_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_g7231 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_g7231*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_g7231_Construct( OUT CodecData_g7231*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/*******************************************************************************/
RVAPI void CodecData_g7231_Destruct( OUT CodecData_g7231*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
