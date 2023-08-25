/******************************************************************************
Filename:    ippcodec_h261.h
Description:
*******************************************************************************/
#include "rvsdpmedia.h"
#include "ippcodec.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/*******************************************************************************/
typedef	struct {
	int	qcifMPI;		/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	int	cifMPI;		/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	RvBool	unrestrictedVector; /*BOOLEAN*/
	
	int		maxBitRate;			/*INTEGER (1..192400),	-- units 100 bit/s. Will be extracted from attribute b=TIAS:xxx */

    /* ADDITIONS*/
    eMFResulutionFormat_t   order_privilege;
    RvUint32                framerate;
    const char*		szEncodingName;
} CodecData_h261;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_h261_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_h261 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_h261*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_h261_Construct( OUT CodecData_h261*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );
/******************************************************************************
*  CodecData_h261_to_mediaDsecr
*  --------------------------------
*  General :        Converts data from CodecData_h261 type structure into media
*                   descriptor
*
*
*  Return Value:   RV_OK - all the parameters converted successfully
*                  RV_ERROR_UNKNOWN - otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          data - ptr to codec data to be converted
*                  descr - media descriptor where the converted values are set
*                          The descriptor may hold values. Theses values will be
*                          overridden.
*                  payload - of the codec.
*
*  Output:         none.
******************************************************************************/
RVAPI RvStatus CodecData_h261_to_mediaDsecr( IN CodecData_h261*	data,
											INOUT RvSdpMediaDescr* descr,/*where the codec is located*/
											IN int			payload,
											IN RV_BOOL      listOneXCif
										    );
/*******************************************************************************/
RVAPI void CodecData_h261_Destruct( OUT CodecData_h261*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
