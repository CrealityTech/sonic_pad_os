#ifndef		__SCAN_STACK_H__
#define		__SCAN_STACK_H__
#include	"common.h"
typedef __hdle HSTACK;
HSTACK	ScanStackAlloc	(void);
int	  	ScanStackPush	(HSTACK hStack, const char *path);
char   *ScanStackPop 	(HSTACK hStack);
__bool  ScanStackEmpty  (HSTACK hStack);
void	ScanStackFree	(HSTACK hStack);
#endif		/* __SCAN_STACK_H__ */

