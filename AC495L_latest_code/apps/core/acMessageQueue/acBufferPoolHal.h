/**************************************************************/
/* Private - for auditing and debugging only.                 */
/**************************************************************/
#include "quLinkedQueue.h"
#include "acBufferPool.h"

typedef struct {
  char               Name[AC_STDLIB_MAX_OBJECT_NAME+1];
  void               *StartAdr; 
  void               *EndAdr; 
  unsigned int       BuffSize; 
  unsigned int       TotalBuf; 
  unsigned int       Attributes;
  int                AuditTime;
  quTListHeaderEntry quHead[2]; /* 2 Queues, 1 for free, 1 for busy buffers */
  quTElementHead*    quElements;/* buffers header list for buffer manager - quElements[TotalBuf]*/
  int                BuffMaxTime;
  int                BuffAverageTime;
  int                BuffMaxUsage;
  unsigned int	     MagicNumber;
}acBuffPoolDescriptor;


void RegisterPoolAuditDebug(acBuffPoolDescriptor *Pool);
void DeRegisterPool(acBuffPoolDescriptor *Pool);
void acBufferPoolCmdShell(void);
void acBuffAuditPool(acBuffPoolDescriptor *Pool);
void acBufferPoolAudit(void);
