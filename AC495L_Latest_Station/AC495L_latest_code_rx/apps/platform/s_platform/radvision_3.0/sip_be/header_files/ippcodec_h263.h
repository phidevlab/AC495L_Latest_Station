/******************************************************************************
Filename:    ippcodec_h263.h
Description:
*******************************************************************************/
#include "rvsdpmedia.h"
#include "ippcodec.h"

//#include "MFVideoDefs.h"


#ifdef __cplusplus
extern "C" {
#endif
	
/*******************************************************************************/
typedef	struct {
	int	sqcifMPI;	/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	int	qcifMPI;		/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	int	cifMPI;		/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	int	cif4MPI;		/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	int	cif16MPI;	/*INTEGER (1..32) OPTIONAL,	-- units 1/29.97 Hz*/
	/*must parameters*/
	int		maxBitRate;			/*INTEGER (1..192400),	-- units 100 bit/s*/
	RvBool	unrestrictedVector; /*BOOLEAN*/
	RvBool	arithmeticCoding;	/*BOOLEAN*/
	RvBool	advancedPrediction; /*BOOLEAN*/
	RvBool	pbFrames;			/*BOOLEAN*/
	RvBool	temporalSpatialTradeOffCapability; /*BOOLEAN*/
	/**/
	int	hrdB;		/*INTEGER (0..524287) OPTIONAL,	-- units 128 bits*/
	int	bppMaxKb;	/*INTEGER (0..65535) OPTIONAL,	-- units 1024 bits*/
/*
		slowSqcifMPI	INTEGER (1..3600) OPTIONAL,	-- units seconds/frame
		slowQcifMPI	INTEGER (1..3600) OPTIONAL, --units seconds/frame
		slowCifMPI	INTEGER (1..3600) OPTIONAL, --units seconds/frame
		slowCif4MPI	INTEGER (1..3600) OPTIONAL, --units seconds/frame
		slowCif16MPI	INTEGER (1..3600) OPTIONAL, --units seconds/frame
		errorCompensation	BOOLEAN,
		enhancementLayerInfo	EnhancementLayerInfo OPTIONAL,
		h263Options	H263Options OPTIONAL
*/
    /* ADDITIONS*/
//    struct  
//    {
//        eMFResulutionFormat_t   resolution;
//        RvUint32                order;
//    }                       order_privilege[10];
    eMFResulutionFormat_t   order_privilege;
    
    RvUint32                framerate;
	const char*		        szEncodingName;
}CodecData_h263;
/*******************************************************************************/
/***************************************************************************************
 * CodecData_h263_Construct
 * -------------------------------------------------------------------------------------
 * General:   initialize CodecData_h263 object from RvSdpMediaDescr   
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   CodecData_h263*	data
 *			  
**************************************************************************************/
RVAPI RvStatus CodecData_h263_Construct( OUT CodecData_h263*	data,
				   IN RvSdpMediaDescr* mediaDescr,/*where the codec is located*/
				   IN int			payload
				   );

/******************************************************************************
*  CodecData_h263_to_mediaDsecr
*  --------------------------------
*  General :        Converts data from CodecData_h263 type structure into media
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
*				   listOneXcif - notifies whether the media descriptor is limited to 
*                                only one xCIP option (e.g. in an answer to an offer)
*                                or should list all supported xCIFs (e.g. in an offer).
*
*  Output:         none.
******************************************************************************/
RVAPI RvStatus CodecData_h263_to_mediaDsecr( IN CodecData_h263*	data,
											INOUT RvSdpMediaDescr* descr,/*where the codec is located*/
											IN int			payload,
											RV_BOOL         listOneXCif
										    );
/*******************************************************************************/
RVAPI void CodecData_h263_Destruct( OUT CodecData_h263*	data);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif
