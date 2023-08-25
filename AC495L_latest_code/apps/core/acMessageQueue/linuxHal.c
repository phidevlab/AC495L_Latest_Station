#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "quLinkedQueue.h"
#include "acBufferPool.h"
#include "acBufferPoolHal.h"

void acStdLib_ErrorHandler(int Severity, int ErrorCode, char *File, int LineInFile, 
                               int CID, char *Format, ...)
{
	char ErrStr[513];
	va_list pArguments;

	if (strlen(Format) >= 250)
		strncpy(ErrStr, Format, 254);
	else
	{
		va_start(pArguments,Format);
		/* NirM  151106, vsnprintf (linux) instead of _vsnprintf (WIN32) */
		vsnprintf(ErrStr,512,Format,pArguments);
	}
	ErrStr[512] = 0;

	printf("acMessageQueue Error at %s  line %d : %s\n",File, LineInFile, ErrStr);

	va_end(pArguments);

} 

void RegisterPoolAuditDebug(acBuffPoolDescriptor *Pool)
{
}
void DeRegisterPool(acBuffPoolDescriptor *Pool)
{
}
