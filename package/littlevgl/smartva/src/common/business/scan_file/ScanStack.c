#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ScanStackPri.h"
#define ENTRY_PATH_OFFSET(entry) ((int)((entry)->Path - (__u8 *)(entry)))

/*
****************************************************************************************************
*
*             							ScanStackAlloc
*
*  Description:
*				-	allocate scan stack for save directory path.
*				  	
*  Parameters:
*				NONE
*
*  Return value:
*   NULL   		-  	allocate failed.
*	others		-	handle to scan stack.
****************************************************************************************************
*/
HSTACK ScanStackAlloc(void)
{
	ScanStack_t *pStack;
	
	/* allocate scan stack structure first */
	pStack = (ScanStack_t *)malloc(sizeof(ScanStack_t));
	if (NULL == pStack) {
		printf("malloc failed\n");
		return 0;
	}
	
	/* preallocate STACK_GROWN_LEN space for stack data area,
	 * if the scan stack space not enough, will grown scan stack data space.
	 */
	pStack->Data = (__u8 *)malloc(STACK_GROWN_LEN);
	
	/* initialize scan stack structure */
	pStack->Alloc	=	STACK_GROWN_LEN;
	pStack->Size	= 	0;
	pStack->Top		=	0;
	
	return (HSTACK)pStack;
}


/*
****************************************************************************************************
*
*             							ScanStackPush
*
*  Description:
*				-	push one directory path to scan stack.
*				  	
*  Parameters:
*   hStack		-  	handle to scan stack.
*   path		-  	pointer to push directory path.
*
*  Return value:
*   EPDK_OK   	-  	push succeeded.
*	EPDK_FAIL	-	push failed.
****************************************************************************************************
*/
int	ScanStackPush(HSTACK hStack, const char *path)
{
	ScanStack_t 		*pStack;
	ScanStackEntry_t	*pEntry;
	__u32					 RecLen;
	
	if (0 == hStack || NULL == path) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FAIL;
	}
	pStack = (ScanStack_t *)hStack;
	
	/* calc the pushed path needed space, should align by 64bit(8byte) */
	RecLen = RAT_ALIGN(ENTRY_PATH_OFFSET(pEntry) + strlen(path) + 1, sizeof(__u64));
	
	/* check stack free space enough or not */
	if (RecLen > (pStack->Alloc - pStack->Size)) {
		
		/* stack data area free space not enough, 
		 * gown stack data area space.
		 */
		__u8 *pNewData;
		__u32 Len = pStack->Alloc + STACK_GROWN_LEN;
		pNewData = realloc(pStack->Data, Len);
		if (NULL == pNewData) {
			printf("realloc for scan stack failed\n");
			return EPDK_FAIL;
		}
		pStack->Data = pNewData;
		pStack->Alloc += STACK_GROWN_LEN;
	}
	
	/* push directory to the end of stack */
	pEntry = (ScanStackEntry_t	*)(pStack->Data + pStack->Size);
	pEntry->RecLen	= RecLen;
	pEntry->Prev	= pStack->Top;
	strcpy((char *)(pEntry->Path), path);
	
	/* adjust stack managemant information */
	pStack->Top 	  = pStack->Size;
	pStack->Size	 += pEntry->RecLen;
	
	return EPDK_OK;
}


/*
****************************************************************************************************
*
*             							ScanStackPop
*
*  Description:
*				-	pos the last insert directory path from scan stack.
*				  	
*  Parameters:
*   hStack		-  	handle to scan stack.
*
*  Return value:
*	NULL		-	pop failed.
*   others  	-  	last insert directory path.
****************************************************************************************************
*/
char *ScanStackPop(HSTACK hStack)
{
	ScanStack_t 		*pStack;
	ScanStackEntry_t	*pEntry;
	
	if (0 == hStack) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return NULL;
	}
	pStack 	= (ScanStack_t *)hStack;
	
	/* stack can't be empty */
	if (0 == pStack->Size) {
		printf("scan stack is empty\n");
		return NULL;
	}
	
	/* pop the top entry */
	pEntry	= (ScanStackEntry_t *)(pStack->Data + pStack->Top);
	pStack->Top	  = pEntry->Prev;
	pStack->Size -= pEntry->RecLen;
	
	return (char *)(pEntry->Path);
}


/*
****************************************************************************************************
*
*             							ScanStackEmpty
*
*  Description:
*				-	check scan stack empty or not.
*				  	
*  Parameters:
*   hStack		-  	handle to scan stack.
*
*  Return value:
*	EPDK_TRUE	-	check succeeded.
*   EPDK_FALSE  -  	check failed.
****************************************************************************************************
*/
__bool ScanStackEmpty(HSTACK hStack)
{
	ScanStack_t *pStack;
	
	if (0 == hStack) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FALSE;
	}
	pStack 	= (ScanStack_t *)hStack;
	
	if (0 == pStack->Size) {
		return EPDK_TRUE;
	}
	return EPDK_FALSE;
}


/*
****************************************************************************************************
*
*             							ScanStackFree
*
*  Description:
*				-	destory scan stack.
*
*  Parameters:
*   hStack		-  	handle to scan stack.
*
*  Return value:
*				NONE
****************************************************************************************************
*/
void ScanStackFree(HSTACK hStack)
{
	ScanStack_t *pStack;
	
	if (0 == hStack) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return ;
	}
	pStack 	= (ScanStack_t *)hStack;
	
	if (pStack->Data) {

		free(pStack->Data);
	}
	free((void *)pStack);
}

