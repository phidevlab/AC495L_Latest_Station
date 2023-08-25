
#ifndef RV_COMMON_EX_TYPES_H
#define RV_COMMON_EX_TYPES_H


typedef RvChar* RvCharPtr;
typedef RvUint32 RvMilliseconds;

/* Copy constructor for basic types */

/*#define rvDefaultConstructCopy(d, s, a) (*(d) = *(s), (d))*/
#define charConstructCopy(d, s, a)		rvDefaultConstructCopy(d, s, a)
#define intConstructCopy(d, s, a)		rvDefaultConstructCopy(d, s, a)
#define floatConstructCopy(d, s, a) 	rvDefaultConstructCopy(d, s, a)
#define doubleConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvInt8ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvInt16ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvInt32ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvUint8ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvUint16ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvUint32ConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvBoolConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvUintConstructCopy(d, s, a)	rvDefaultConstructCopy(d, s, a)
#define RvVoidPtrConstructCopy(d, s, a) rvDefaultConstructCopy(d, s, a)
#define RvCharPtrConstructCopy(d, s, a) rvDefaultConstructCopy(d, s, a)

/* Destructor for basic types */
/*#define rvDefaultDestruct(x)*/
#define charDestruct(x) 				rvDefaultDestruct(x)
#define intDestruct(x)					rvDefaultDestruct(x)
#define floatDestruct(x)				rvDefaultDestruct(x)
#define doubleDestruct(x)				rvDefaultDestruct(x)
#define RvInt8Destruct(x)				rvDefaultDestruct(x)
#define RvInt16Destruct(x)				rvDefaultDestruct(x)
#define RvInt32Destruct(x)				rvDefaultDestruct(x)
#define RvUint8Destruct(x)				rvDefaultDestruct(x)
#define RvUint16Destruct(x) 			rvDefaultDestruct(x)
#define RvUint32Destruct(x) 			rvDefaultDestruct(x)
#define RvBoolDestruct(x)				rvDefaultDestruct(x)
#define RvUintDestruct(x)				rvDefaultDestruct(x)
#define RvVoidPtrDestruct(x)			rvDefaultDestruct(x)
#define RvCharPtrDestruct(x)			rvDefaultDestruct(x)


/* Assignment operator for basic types */
#define rvDefaultCopy(d, s) 			(*(d) = *(s), (d))
#define charCopy(d, s)					rvDefaultCopy(d, s)
#define intCopy(d, s)					rvDefaultCopy(d, s)
#define floatCopy(d, s) 				rvDefaultCopy(d, s)
#define doubleCopy(d, s)				rvDefaultCopy(d, s)
#define RvInt8Copy(d, s)				rvDefaultCopy(d, s)
#define RvInt16Copy(d, s)				rvDefaultCopy(d, s)
#define RvInt32Copy(d, s)				rvDefaultCopy(d, s)
#define RvUint8Copy(d, s)				rvDefaultCopy(d, s)
#define RvUint16Copy(d, s)				rvDefaultCopy(d, s)
#define RvUint32Copy(d, s)				rvDefaultCopy(d, s)
#define RvBoolCopy(d, s)				rvDefaultCopy(d, s)
#define RvUintCopy(d, s)				rvDefaultCopy(d, s)
#define RvVoidPtrCopy(d, s) 			rvDefaultCopy(d, s)
#define RvCharPtrCopy(d, s) 			rvDefaultCopy(d, s)

/* Equal operator for basic types */
#define rvDefaultEqual(a, b)			(*(a) == *(b))
#define charEqual(a, b) 				rvDefaultEqual(a, b)
#define intEqual(a, b)					rvDefaultEqual(a, b)
#define floatEqual(a, b)				rvDefaultEqual(a, b)
#define doubleEqual(a, b)				rvDefaultEqual(a, b)
#define RvInt8Equal(a, b)				rvDefaultEqual(a, b)
#define RvInt16Equal(a, b)				rvDefaultEqual(a, b)
#define RvInt32Equal(a, b)				rvDefaultEqual(a, b)
#define RvUint8Equal(a, b)				rvDefaultEqual(a, b)
#define RvUint16Equal(a, b) 			rvDefaultEqual(a, b)
#define RvUint32Equal(a, b) 			rvDefaultEqual(a, b)
#define RvBoolEqual(a, b)				rvDefaultEqual(a, b)
#define RvUintEqual(a, b)				rvDefaultEqual(a, b)
#define RvVoidPtrEqual(a, b)			rvDefaultEqual(a, b)
#define RvCharPtrEqual(a, b)			(strcmp(*(a), *(b)) == 0)

/* Compare operator for basic types */
#define RvInt32Less(a, b) 				(*(a) < *(b))
#define RvCharPtrLess(a, b) 			(strcmp(*(a), *(b)) < 0)

/* Equal only if a and b are the same object. */
#define rvNeverEqual(a, b) ((a) == (b))


#endif /* RV_COMMON_EX_TYPES_H*/
