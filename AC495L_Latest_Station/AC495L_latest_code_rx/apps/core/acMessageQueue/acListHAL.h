/**************************************************************/
/* Private - for auditing and debugging only.                 */
/**************************************************************/

typedef struct 
{
	char			    Name[AC_STDLIB_MAX_OBJECT_NAME+1];
	unsigned int		MagicNumber;
	int			        MaxSize; 
	unsigned int		Attributes;
	quTListHeaderEntry	quHead[2]; 
	quTElementHead*		quElements;
	void **			    ListElements;
}acListDescriptor;

