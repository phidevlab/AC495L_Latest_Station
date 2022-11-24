#define FatalMsg		0x00000001
#define RecoverableMsg	0x00000002
#define NoticeMsg		0x00000004
#define InformMsg		0x00000008
#define DebugMsg		0x00000010
#define FatalInLabMsg	0x00000020
#define TracebackError	0x80000000  /* Its not error type - used for indication of gen trace with this error*/


#define AC_STD_MAGIC	0x22552255
#define AC_STD_OBJECT_VALIDATE(o) ( (o!=NULL) && (o->MagicNumber == AC_STD_MAGIC) )

void acStdLib_ErrorHandler(int Severity, int ErrorCode, char *File, int LineInFile, 
                               int CID, char *Format, ...);

/* NirM 151106, Working as WIN32 in AC48 CPE VTK */
#if 0
#if WIN32
#define BSP_ErrorHandler()
#endif

#ifndef WIN32
asm unsigned int GetLinkRegister(void)
{
  mfspr     r3,LR
}
#endif
#endif
