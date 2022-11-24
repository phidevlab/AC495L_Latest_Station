/******************************************************************************
Filename:    ippcodec_mp4v_es.h
Description:
*******************************************************************************/
#include "rvsdpmedia.h"
#include "ippcodec.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/*******************************************************************************/
typedef	struct {
	RvUint8	profileAndLevel;
	char	config[256];
}CodecData_mp4v_es;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_mp4v_es_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_mp4v_es object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_mp4v_es*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_mp4v_es_Construct( OUT CodecData_mp4v_es*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/*******************************************************************************/
RVAPI void CodecData_mp4v_es_Destruct( OUT CodecData_mp4v_es*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
