#if (0)
******************************************************************************
Filename    :
Description :
******************************************************************************
                Copyright (c) 1999 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVision LTD.

RADVision LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************
$Revision:$
$Date:$
$Author: S. Cipolli$
******************************************************************************
#endif

#ifndef RV_COMMON_ALGO_H
#define RV_ALGO_H

#include "rvtypes.h"

#define rvDeclareForEach(I, T)												\
	void _Rv##I##T##ForEach(I(T) f, I(T) l, void (*z)(T*));

#define rvDefineForEach(I, T)												\
	void _Rv##I##T##ForEach(I(T) f, I(T) l, void (*z)(T*)) {				\
		I(T) i;																\
		for (i = f; i != l; i = I##Next(i))									\
			z(I##Data(i));													\
	}

#define rvDeclareFind(I, T)													\
	I(T) _Rv##I##T##Find(I(T) f, I(T) l, const T* x);
#define rvDefineFind(I, T)													\
	I(T) _Rv##I##T##Find(I(T) f, I(T) l, const T* x) {					\
		I(T) i;																\
		for (i = f; i != l; i = I##Next(i))									\
			if (T##Equal(I##Data(i), x))									\
				return i;													\
		return i;															\
	}

#define rvDeclareFindIf(I, T)												\
	I(T) _Rv##I##T##FindIf(I(T) f, I(T) l,								\
	  RvBool (*p)(T*, void*), void* d);

#define rvDefineFindIf(I, T)												\
	I(T) _Rv##I##T##FindIf(I(T) f, I(T) l,								\
	  RvBool (*p)(T*, void*), void* d) {									\
		I(T) i;																\
		for (i = f; i != l; i = I##Next(i))									\
			if (p(I##Data(i), d))											\
				return i;													\
		return i;															\
	}

/* Public algorithm interface */
/* I = Iterator Type */
/* T = Value Type */

/* Iterator rvForEach(I, T)(Iterator first, Iterator l, void (*func)(T*)) */
#define rvForEach(I, T)		_Rv##I##T##ForEach
/* Iterator rvFind(I, T)(Iterator first, Iterator last, const T* x) */
#define rvFind(I, T)		_Rv##I##T##Find
/* Iterator rvFindIf(I, T)(Iterator f, Iterator l,
	RvBool (*predicate)(T*, void*), void* data) */
#define rvFindIf(I, T)		_Rv##I##T##FindIf

#endif

