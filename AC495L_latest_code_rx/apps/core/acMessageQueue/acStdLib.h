/*
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* --                                                                        --
* --  ACStdLib - AudioCodes Standard Library - Copyright 2005 - AudioCodes  --
* --                                                                        --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	acStdLib.h
* --
* --	Description		:	Common definitions for all acStdLib objects
* --					
* --	Author			:	Eitan Richardson
* --	
* --	Notes			:   None
* --	                    
* --	Revision History:
* --	
* --	  - 12/4/05 - Eitan - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/

#ifndef	_ACSTDLIB_H
#define	_ACSTDLIB_H

/* Common return codes: */
#define AC_STDLIB_OK		 0
#define AC_STDLIB_ERROR	    -1

/* Size of object names: */
#define AC_STDLIB_MAX_OBJECT_NAME		30

/* when acStdlib function call to other acStdlib function */
#define AC_STDLIB_INTERNAL_APPLICATION	0x80000000

#endif /* _ACSTDLIB_H */
