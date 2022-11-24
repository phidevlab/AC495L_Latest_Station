/**************************************************************/
/* Private - for auditing and debugging only.                 */
/**************************************************************/
#include "acStdLib.h"

typedef struct 
{
	char		    	Name[AC_STDLIB_MAX_OBJECT_NAME+1];
	unsigned int		MagicNumber;
	int			        MaxSize; 
	int		        	MaxBufSize;
	int		        	ApplicationID;
	unsigned int		Attributes;
	unsigned int		PoolId;
	quTListHeaderEntry	quHead[2]; 
	quTElementHead*		quElements;
}acBufferListDescriptor;

void acBufferListCmdShell(void);
