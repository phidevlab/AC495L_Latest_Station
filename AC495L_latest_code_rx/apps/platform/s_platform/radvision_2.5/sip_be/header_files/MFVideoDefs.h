//........................................................................
/// @name MFVideoDefs.h
///     
/// @desc provides video definitions for MF
///
/// @created  Oren Hazai
//........................................................................

#ifndef __MF_VIDEO_DEFS__
#define __MF_VIDEO_DEFS__

#include "rvtypes.h"

RV_DECLARE_HANDLE (HVIDEOOBJ);

#ifndef eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION
#define eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION
enum eMFFrameSizeFormat_t { 
    MF_FRAME_WIDTH_SQCIF  = 128,
		MF_FRAME_HEIGHT_SQCIF = 96,
		MF_FRAME_WIDTH_QCIF  = 176,
		MF_FRAME_HEIGHT_QCIF = 144,
		MF_FRAME_WIDTH_CIF   = MF_FRAME_WIDTH_QCIF*2,
		MF_FRAME_HEIGHT_CIF  = MF_FRAME_HEIGHT_QCIF*2,
		MF_FRAME_WIDTH_4CIF   = MF_FRAME_WIDTH_QCIF*4,
		MF_FRAME_HEIGHT_4CIF  = MF_FRAME_HEIGHT_QCIF*4,
		MF_FRAME_WIDTH_16CIF   = MF_FRAME_WIDTH_QCIF*8,
		MF_FRAME_HEIGHT_16CIF  = MF_FRAME_HEIGHT_QCIF*8,
		MF_FRAME_WIDTH_SPFMT  = 0,
		MF_FRAME_HEIGHT_SPFMT  = 0,
		MF_FRAME_WIDTH_QVGA  = 0,
		MF_FRAME_HEIGHT_QVGA  = 0
};


// max CIF bgr buff size
#define MF_MAX_VIDEO_BUFF_MEM_SIZE (MF_FRAME_WIDTH_CIF * MF_FRAME_HEIGHT_CIF * 3 + 500)

typedef enum  
{   /* 10 * frames per sec (for integral values)*/
    MF_FRAME_RATE_75 = 75,
		MF_FRAME_RATE_10 = 100,
		MF_FRAME_RATE_15 = 150,
		MF_FRAME_RATE_20 = 200,
		MF_FRAME_RATE_25 = 250,
		MF_FRAME_RATE_30 = 300
}enVideoFrameRate_t; 


typedef enum  
{
    MF_RESOLUTION_SQCIF, /*sub-QCIF*/
		MF_RESOLUTION_QCIF,
		MF_RESOLUTION_CIF,
		MF_RESOLUTION_4CIF,
		MF_RESOLUTION_16CIF,
		MF_RESOLUTION_SPFMT,
		MF_RESOLUTION_QVGA
}eMFResulutionFormat_t;  
#endif /*eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION*/

#endif //__MF_VIDEO_DEFS__


