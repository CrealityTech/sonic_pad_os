#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rat_common.h"
#include "ScanPri.h"

#define LIST_DATA_OFFSET(list) 		((int)((list)->Data - (__u8 *)(list)))
#define ENTRY_NAME_OFFSET(entry) 	((int)((entry)->Name - (__u8 *)(entry)))

static int	__ScanAllFiles(ScanInfo_t *pScanInfo); 
static int	__SetupScanProcess(ScanInfo_t *pScanInfo);
static void	__DestoryScanInfo (ScanInfo_t *pScanInfo);
static int	__InsertFileEntryToList(const char *filePath, int format, ScanInfo_t *pScanInfo);
__bool		__ValidFileType(const char *file, __u64 Types, int *Format);

prat_search_cb g_rat_search_cb = NULL;
char * SLIB_strchrlast(char * pstr, char srch_char) {
    char   * pstr_next;
    unsigned int str_len;

    if (pstr == (char *)0)                          /* Rtn NULL if srch str ptr NULL (see Note #2a).                */
    {
        return ((char *)0);
    }
    
    pstr_next  = pstr;
    str_len    = strlen(pstr);
    pstr_next += str_len;
    while (( pstr_next != pstr) &&                  /* Srch str from end until beg (see Note #2c) ...               */
           (*pstr_next != srch_char))               /* ... until srch char found   (see Note #2d).                  */
    {
        pstr_next--;
    }

    if (*pstr_next != srch_char)                    /* If srch char NOT found, str points to NULL; ...              */
    {
        return ((char *)0);                         /* ... rtn NULL (see Notes #2b & #2c).                          */
    }

    return (pstr_next);                             /* Else rtn ptr to found srch char (see Note #2d).              */
}

static int  SLIB_stricmp(const char * p1_str, const char * p2_str)
{
    char    *p1_str_next;
    char    *p2_str_next;
    int    cmp_val;
extern	int SLIB_toupper(int c);

    if (p1_str == (char *)0)
    {
        if (p2_str == (char *)0)
        {
            return ((int )0);                      /* If BOTH str ptrs NULL, rtn 0 (see Note #2a).                 */
        }
        cmp_val = (int )0 - (int )SLIB_toupper(*p2_str);
        return (cmp_val);                           /* If p1_str NULL, rtn neg p2_str val (see Note #2b).           */
    }
    if (p2_str == (char *)0)
    {
        cmp_val = (int )SLIB_toupper(*p1_str);
        return (cmp_val);                           /* If p2_str NULL, rtn pos p1_str val (see Note #2c).           */
    }


    p1_str_next = (char *)p1_str;
    p2_str_next = (char *)p2_str;
    p1_str_next++;
    p2_str_next++;
    while ((SLIB_toupper(*p1_str) == SLIB_toupper(*p2_str)) && /* Cmp strs until non-matching char (see Note #2d) ..           */
           (*p1_str      != (char  )0) &&           /* .. or NULL char(s)               (see Note #2e) ..           */
           ( p1_str_next != (char *)0) &&           /* .. or NULL ptr(s) found (see Notes #2f, #2g, & #2h).         */
           ( p2_str_next != (char *)0))
    {
        p1_str_next++;
        p2_str_next++;
        p1_str++;
        p2_str++;
    }


    if (*p1_str != *p2_str)                         /* If strs NOT identical, ...                                   */
    {
         cmp_val = (int )SLIB_toupper(*p1_str) - (int )SLIB_toupper(*p2_str);       /* ... calc & rtn char diff  (see Note #2d1).         */
    }
    else if (*p1_str == (char)0)                    /* If NULL char(s) found, ...                                   */
    {
         cmp_val = 0;                               /* ... strs identical; rtn 0 (see Note #2e).                    */
    }
    else
    {
        if (p1_str_next == (char *)0)
        {
            if (p2_str_next == (char *)0)           /* If BOTH next str ptrs NULL, ...                              */
            {
                cmp_val = (int )0;                 /* ... rtn 0                   (see Note #2f).                  */
            }
            else
            {                                       /* If p1_str_next NULL, ...                                     */
                cmp_val = (int )0 - (int )SLIB_toupper(*p2_str_next);   /* ... rtn neg p2_str_next val (see Note #2g).        */
            }
        }
        else                                        /* If p2_str_next NULL, ...                                     */
        {
            cmp_val = (int )SLIB_toupper(*p1_str_next);        /* ... rtn pos p1_str_next val (see Note #2h).                  */
        }
    }

    return (cmp_val);
}
/*
****************************************************************************************************
*
*             							ScanFilesBegin
*
*  Description:
*				-	begin the scan process.
*
*  Parameters:
*   pDir		-  	pointer to scan directory.
*   FileTypes	-  	types of scan ebook.
*   OnceCnt		-  	read items number when read once. 
*					if zero, I will read all items one time.
*
*  Return value:
*   NULL   		-  	setup scan process failed.
*	others		-	handle to scan process.
****************************************************************************************************
*/
HSCAN ScanFilesBegin(const char *pDir, __u64 FileTypes, int OnceCnt)
{
	ScanInfo_t 	*pScanInfo;
	ScanFileList_t	*pList;
	HSTACK				 hStack;

	if (NULL == pDir) {
		printf("invalid paramter for ScanBookFile\n");
		return 0;
	}
	pScanInfo = (ScanInfo_t *)malloc(sizeof(ScanInfo_t));
	if (NULL == pScanInfo) {
		printf("malloc failed\n");
		return 0;
	}
	
	pList = (ScanFileList_t *)malloc(FILELIST_LEN);
	if (NULL == pList) {
		printf("malloc failed\n");
		free(pScanInfo);
		return 0;
	}
	
	/* initialize file list structure */
	pList->Prev 		= NULL;
	pList->Next			= NULL;
	pList->Alloc		= FILELIST_LEN - LIST_DATA_OFFSET(pList);
	pList->Size 		= 0;
	pList->Top			= 0;
	
	/* initialize scan cursor structure */
	hStack = ScanStackAlloc();
	if (0 == hStack) {
		printf("stack allocate failed\n");
		free(pList);
		free(pScanInfo);
		return 0;
	}
	pScanInfo->FirstList  = pList;
	pScanInfo->LastList	  = pList;
	
	pScanInfo->ScanCursor.CurList = pList;
	pScanInfo->ScanCursor.Offset = 0;
	
	pScanInfo->ScanTypes  = FileTypes;
	pScanInfo->OnceCnt    = OnceCnt;
	pScanInfo->CurCnt     = 0;
	pScanInfo->hStack	  = hStack;
	pScanInfo->pCurDir	  = NULL;
	strcpy(pScanInfo->CurPath, pDir);
	
	pScanInfo->OutEntry.Format 	= RAT_MEDIA_TYPE_UNKNOWN;
	pScanInfo->OutEntry.Path 	= NULL;
	/* setup scan work process */
	if (EPDK_OK != __SetupScanProcess(pScanInfo)) {
		__DestoryScanInfo(pScanInfo);
		printf("setup scan process failed\n");
		return 0;
	}
	return (HSCAN)pScanInfo;
}


/*
****************************************************************************************************
*
*             							ScanMoveToFirst
*
*  Description:
*				-	move scan handle cursor position to first entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToFirst(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	pScanInfo->ScanCursor.CurList = pScanInfo->FirstList;
	pScanInfo->ScanCursor.Offset = 0;
	
	printf("scan cursor move to first...\n");
	return EPDK_OK;
}

int __ScanMoveCursorToFirst(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanInfo_t *pScanInfo;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	cursor->CurList = pScanInfo->FirstList;
	cursor->Offset = 0;
	
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							ScanMoveToNext
*
*  Description:
*				-	move scan handle cursor position to next entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToNext(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	if ((pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Top))
	{
		if (NULL == pScanInfo->ScanCursor.CurList->Next) 
		{
			int Count;
			
			/* the next entry have not read out, should read it by self */
			Count = ScanFilesOneTime(pScanInfo);
			
			if (0 == Count) 
			{
				/* have no valid ebook files, scan is done */
				pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Size;
				return EPDK_FAIL;
			}
			if (pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Top) 
			{
				/* the read out entry not located in current file list */
				pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
				pScanInfo->ScanCursor.Offset = 0;
				return EPDK_OK;
			}
		}
		else 
		{
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
			pScanInfo->ScanCursor.Offset = 0;
			return EPDK_OK;
		}
	}
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->ScanCursor.Offset += pEntry->RecLen;
	
	return EPDK_OK;
}

int __ScanMoveCursorToNext(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanFileEntry_t	*pEntry;
		
	if ((cursor->Offset == cursor->CurList->Top)) 
	{
		if (NULL == cursor->CurList->Next) 
		{			
			return EPDK_FAIL;
		}
		else
		{
			cursor->CurList = cursor->CurList->Next;
			cursor->Offset = 0;
			return EPDK_OK;
		}
	} 	
	
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + cursor->Offset);
	cursor->Offset += pEntry->RecLen;
	
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							ScanMoveToPrev
*
*  Description:
*				-	move scan handle cursor position to prev entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToPrev(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	if (0 == (pScanInfo->ScanCursor.Offset)) 
	{
		if (NULL == (pScanInfo->ScanCursor.CurList->Prev))
		{
			/* have reached begin of file list */
			return EPDK_FAIL;
		}
		else 
		{
			/* move to the last entry of current prev list */
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Prev;
			pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Top;
			return EPDK_OK;
		}
	}

	if ((pScanInfo->ScanCursor.CurList->Size) == (pScanInfo->ScanCursor.Offset)) {
		pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Top;
		return EPDK_OK;
	}
	
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->ScanCursor.Offset = pEntry->Prev;
	return EPDK_OK;
}

int __ScanMoveCurosrToPrev(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanFileEntry_t	*pEntry;
	
	if (0 == (cursor->Offset)) 
	{
		if (NULL == (cursor->CurList->Prev)) 
		{
			/* have reached begin of file list */
			return EPDK_FAIL;
		}
		else 
		{
			/* move to the last entry of current prev list */
			cursor->CurList = cursor->CurList->Prev;
			cursor->Offset = cursor->CurList->Top;
			return EPDK_OK;
		}
	}

	if ((cursor->CurList->Size) == (cursor->Offset)) 
	{
		cursor->Offset = cursor->CurList->Top;
		return EPDK_OK;
	}
	
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + \
		cursor->Offset);
	cursor->Offset = pEntry->Prev;
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							CursorIsDone
*
*  Description:
*				-	check scan handle cursor position pointing at the end entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_TRUE   -  	cursor position pointing the end entry.
*	EPDK_FALSE	-	cursor position not pointing at the end entry.
****************************************************************************************************
*/
__bool ScanIsDone(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return EPDK_FALSE;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	if ((EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) && \
		(NULL == pScanInfo->pCurDir)) {
		if ((pScanInfo->ScanCursor.CurList == pScanInfo->LastList) && \
			(pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Size)) {
			return EPDK_TRUE;
		}
	}
	return EPDK_FALSE;
}

/*
****************************************************************************************************
*
*             							ScanFree
*
*  Description:
*				-	free scan handle.
*
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   	   		NONE
****************************************************************************************************
*/
void ScanFree(HSCAN hScan)
{
	__DestoryScanInfo((ScanInfo_t *)hScan);
}


/*
****************************************************************************************************
*
*             							ScanGetCurrent
*
*  Description:
*				-	get current scaned file entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   NULL   		-  	get current entry failed.
*	others		-	pointer to scan cursor current entry.
****************************************************************************************************
*/
ScanedEntry_t *ScanGetCurrent(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return NULL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	if ((pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Size)) {
		if (NULL == pScanInfo->ScanCursor.CurList->Next) {
			
			/* buffer have no data */
			return NULL;
		} else {
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
			pScanInfo->ScanCursor.Offset = 0;
		}
	}
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->OutEntry.Format = pEntry->Format;
	pScanInfo->OutEntry.Path   = (char *)(pEntry->Name);
	
	return &(pScanInfo->OutEntry);
}

ScanedEntry_t *__ScanGetCurrentCursorData(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return NULL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	if ((cursor->Offset == cursor->CurList->Size)) {
		if (NULL == cursor->CurList->Next) {
			
			/* buffer have no data */
			return NULL;
		} else {
			cursor->CurList = cursor->CurList->Next;
			cursor->Offset = 0;
		}
	}
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + \
		cursor->Offset);
	pScanInfo->OutEntryCursor.Format = pEntry->Format;
	pScanInfo->OutEntryCursor.Path   = (char *)(pEntry->Name);
	
	return &(pScanInfo->OutEntryCursor);
}

/*
****************************************************************************************************
*
*             							ScanGetCurCnt
*
*  Description:
*				-	get current scaned file count.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   The number of current files number, -1 if failed.
****************************************************************************************************
*/
int ScanGetCurCnt(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (0 == hScan) {
		printf("%s %d invalid parameter\n", __FILE__, __LINE__);
		return -1;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	return pScanInfo->CurCnt;
}


static int __SetupScanProcess(ScanInfo_t *pScanInfo)
{
	int Count;
	
	/* push current path to stack, ready for scan files */
	ScanStackPush(pScanInfo->hStack, pScanInfo->CurPath);
	
	if (0 == pScanInfo->OnceCnt) {
		
		/* scan all files at one time */
		Count = __ScanAllFiles(pScanInfo);
		pScanInfo->CurCnt = Count;
		if (0 == Count) {
			printf("have not find any valid media file\n");
		}
		return EPDK_OK;
	} else {
		/* scan pScanInfo->OnceCnt files one time */
		Count = ScanFilesOneTime(pScanInfo);
		
		if (0 == Count) {
			printf("have not find any valid ebook file\n");
		}
		return EPDK_OK;
	}
}


static int __ScanAllFiles(ScanInfo_t *pScanInfo)
{
	struct dirent	*pDirent;
	int			 Format = RAT_MEDIA_TYPE_UNKNOWN;
	int			 Count  = 0;
    
    while(EPDK_TRUE != ScanStackEmpty(pScanInfo->hStack)) {
    	
    	/* pop directory from stack and save to pScanInfo->CurPath */
    	strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
        	return Count;
        }
        
        while (NULL != (pDirent = readdir(pScanInfo->pCurDir))) {
        	
        	if (pDirent->d_type == FSYS_ATTR_DIRECTORY) {
			
				/* ignore "." and ".." sub-directory */
        		if (pDirent->d_name[0] == '.') {
        			continue;
        		}
        		/* sub-directory push to stack */
				strcpy(pScanInfo->tempPath, pScanInfo->CurPath);
				strcat(pScanInfo->tempPath, "/");
				strcat(pScanInfo->tempPath, (char*)pDirent->d_name);
				ScanStackPush(pScanInfo->hStack, pScanInfo->tempPath);
			} else {
				if (EPDK_TRUE == __ValidFileType((const char *)(pDirent->d_name),
												  pScanInfo->ScanTypes,
												  &Format)) {
					/* store valid ebook file full path to pScanInfo file list */
					strcpy(pScanInfo->tempPath, pScanInfo->CurPath);
					strcat(pScanInfo->tempPath, "/");
					strcat(pScanInfo->tempPath, (char*)pDirent->d_name);
					if (EPDK_OK != __InsertFileEntryToList(pScanInfo->tempPath,
														   Format,
														   pScanInfo)) {
						printf("insert file entry to list failed\n");
						closedir(pScanInfo->pCurDir);
						pScanInfo->pCurDir = NULL;
						return Count;
					}
					Count++;
                    if (g_rat_search_cb) {
                        g_rat_search_cb(pDirent->d_name, Count);
                    }
				}
			}
        }
        /* close current open directory, continue to scan sub-directory */
        closedir(pScanInfo->pCurDir);
        pScanInfo->pCurDir = NULL;
    }
    return Count;
}

int ScanFilesOneTime(ScanInfo_t *pScanInfo)
{
	struct dirent *pDirent;
	int		   Format = RAT_MEDIA_TYPE_UNKNOWN;
	int		   Count  = 0;
    
    /* should open directory if necessary */
    if (NULL == pScanInfo->pCurDir) {
    	if (EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) {
			pScanInfo->CurCnt += Count;
        	return Count;
        }
    	/* pop directory from stack and save to pScanInfo->CurPath */
    	strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
			pScanInfo->CurCnt += Count;
        	return Count;
        }
    }
    while(1) {
        while (NULL != (pDirent = readdir(pScanInfo->pCurDir))) {
        	
        	if (pDirent->d_type == FSYS_ATTR_DIRECTORY) {
			
				/* ignore "." and ".." sub-directory */
        		if (pDirent->d_name[0] == '.') {
        			continue;
        		}
        		/* sub-directory push to stack */
				sprintf(pScanInfo->tempPath, "%s/%s", 
							 pScanInfo->CurPath, pDirent->d_name);
				ScanStackPush(pScanInfo->hStack, pScanInfo->tempPath);
			} else {
				if (EPDK_TRUE == __ValidFileType((const char *)(pDirent->d_name), \
												  pScanInfo->ScanTypes,
												  &Format)) {
					/* store valid ebook file full path to pScanInfo file list */
					sprintf(pScanInfo->tempPath, "%s/%s", 
								 pScanInfo->CurPath, pDirent->d_name);
					if (EPDK_OK != __InsertFileEntryToList(pScanInfo->tempPath,
														   Format,
														   pScanInfo)) {
						printf("insert file entry to list failed\n");
						closedir(pScanInfo->pCurDir);
						pScanInfo->pCurDir = NULL;
						pScanInfo->CurCnt += Count;
						return Count;
					}
					/* get one valid ebook file */
					Count++;
                    if (g_rat_search_cb) {
                        g_rat_search_cb(pDirent->d_name, Count);
                    }
				}
			}
			if (Count >= pScanInfo->OnceCnt) {
				/* read once work finished */
				pScanInfo->CurCnt += Count;
				return Count;
			}
        }
        /* close current open directory, continue to scan sub-directory */
        closedir(pScanInfo->pCurDir);
        pScanInfo->pCurDir = NULL;
        
        /* check sub-directory stack empty or not */
        if (EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) {
        	break;
        }
        
        /* pop directory from stack and save to pScanInfo->CurPath */
    	strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
			pScanInfo->CurCnt += Count;
        	return Count;
        }
    }
    
    /* not enough valid ebook file for read */
	pScanInfo->CurCnt += Count;
    return Count;
}

static int __InsertFileEntryToList(const char *filePath, 
								   int format,
								   ScanInfo_t *pScanInfo)
{
	ScanFileList_t  *pLastList;
	ScanFileEntry_t *pEntry;
	__u32				  RecLen;
	
	/* insert file full path to the last file list of pScanInfo */
	pLastList = pScanInfo->LastList;
	
	RecLen = RAT_ALIGN(ENTRY_NAME_OFFSET(pEntry) + strlen(filePath) + 1, sizeof(__u64));
	
	/* check the last file list have enough space for filePath insert */
	if (RecLen > (pLastList->Alloc - pLastList->Size)) {
		
		/* free space not enough, allocate a new file list structure */
		ScanFileList_t  *pNewList;
		pNewList = (ScanFileList_t *)malloc(FILELIST_LEN);
		if (NULL == pNewList) {
			printf("malloc failed\n");
			return EPDK_FAIL;
		}
		/* initialize allocated file list */
		pNewList->Prev 		= pLastList;
		pLastList->Next 	= pNewList;
		pNewList->Next		= NULL;
		pNewList->Alloc		= FILELIST_LEN - LIST_DATA_OFFSET(pNewList);
		pNewList->Size 		= 0;
		pNewList->Top 		= 0;	
		
		/* insert filePath to the allocated file list */
		pScanInfo->LastList = pNewList;
		pLastList 			= pScanInfo->LastList;
	}
	
	/* store filePath to the end of last file list */
	pEntry 			= (ScanFileEntry_t	*)(pLastList->Data + pLastList->Size);
	pEntry->RecLen	= RecLen;
	pEntry->Prev 	= pLastList->Top;	/* prev entry location */
	pEntry->Format	= format;
	strcpy((char *)(pEntry->Name), filePath);
	
	/* adjust the last file list management information */
	pLastList->Top	  = pLastList->Size;
	pLastList->Size  += RecLen;
	
	return EPDK_OK;
}

static FmtIdx_t ValidFmtTbl[] = {
	{ ".txt",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_TXT	},		
	{ ".h",	    RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_H    },
	{ ".c",	    RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_C    },
	{ ".ini",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_INI	},
	{ ".lrc",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_LRC	},
	{ ".srt",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_SRT  },
    { ".ssa",  	RAT_MEDIA_TYPE_EBOOK,      	EBOOK_SCAN_SSA	},
    { ".ass",   RAT_MEDIA_TYPE_EBOOK,     	EBOOK_SCAN_ASS	},

   	{ ".AVI",  	RAT_MEDIA_TYPE_VIDEO,       MOVIE_SCAN_AVI  },
    { ".DIVX", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_AVI  },
   // { ".PMP",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_PMP  },
    { ".3GP",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_3GP  },
    { ".MP4",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_MP4  },
    { ".M4V",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MP4  },
    { ".F4V",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_FLV  },
    { ".MPG",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MPG  },
    { ".MPEG", 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MPG  },
    { ".VOB",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_VOB  },
    { ".DAT",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_DAT  },
    { ".RM",   	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_RM   },
    { ".RAM",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_RM   },
    //{ ".RMVB",	RAT_MEDIA_TYPE_VIDEO,    	MOVIE_SCAN_RMVB },
    { ".FLV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_FLV  },
    { ".WMV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_WMV  },
    { ".ASF" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_ASF  },
    { ".MKV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MKV  },
    { ".PSR" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_PSR  },
    { ".TP"  , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_TP   },
    { ".TRP" , 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".TS"  , 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".M2TS", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".MOV", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_MOV  },

	{ ".MP3",  	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP3  },
    { ".MP2",  	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP2  },
    { ".MP1",   RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP1   },
   // { ".WMA",  	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_WMA   },
    { ".OGG",	RAT_MEDIA_TYPE_AUDIO,    	MUSIC_SCAN_OGG },
    { ".APE" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_APE  },
    { ".FLAC" , RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_FLAC  },
    //{ ".AC3" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_AC3  },
   // { ".DTS" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_DTS  },
   // { ".RA" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_RA  },
    { ".AAC"  , RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_AAC   },
    { ".AMR" , 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_AMR   },
    //{ ".ATRC"  ,RAT_MEDIA_TYPE_AUDIO,       MUSIC_SCAN_ATRC   },
   // { ".MID", 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_MID   },
    { ".WAV", 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_WAV  },
    { ".M4A", 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_M4A  },

	{ ".JFIF" , RAT_MEDIA_TYPE_PIC,     	PHOTO_SCAN_JFIF  },
    { ".JPEG"  ,RAT_MEDIA_TYPE_PIC,     	PHOTO_SCAN_JPEG   },
    { ".PNG" , 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_PNG   },
    { ".BMP"  , RAT_MEDIA_TYPE_PIC,         PHOTO_SCAN_BMP   },
    { ".JPG", 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_JPG   },
    { ".GIF", 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_GIF  },

    { ".IMG", 	RAT_MEDIA_TYPE_FIRMWARE,    IMAGE_SCAN_IMG  },
        

    { NULL,  	RAT_MEDIA_TYPE_UNKNOWN,  		0			    }
};

__bool __ValidFileType(const char *file, __u64 Types, int *Format)
{
	char    	 	*Suffix;
    FmtIdx_t 	*pFmt;

    if(NULL == file) {
        return EPDK_FALSE;
    }
    
    /* seek to extended name */
    Suffix = SLIB_strchrlast((char *)file, '.');
    if (NULL == Suffix) {
    	return EPDK_FALSE;
    }   
    
    /* look in ebook valid format table */
    pFmt = ValidFmtTbl;
    while (pFmt->Suffix)
    {
    	if(SLIB_stricmp(Suffix, pFmt->Suffix) == 0)
        {
            if ((pFmt->ScanFlag & Types) || 0 == Types)
            {
            	*Format = pFmt->Format;
            	return EPDK_TRUE;
        	}
            else
            {
        		return EPDK_FALSE;
        	}
        }
        pFmt++;
    }

    if(0 == Types)//RAT_MEDIA_TYPE_ALL
    {
        *Format = RAT_MEDIA_TYPE_UNKNOWN;
        return EPDK_TRUE;
    }
    
    return EPDK_FALSE;   
}

static void __DestoryScanInfo(ScanInfo_t *pScanInfo)
{
	ScanFileList_t	*pFree;
	ScanFileList_t	*pNextFree;
	
	/* close current */
	if (pScanInfo->pCurDir) {
		closedir(pScanInfo->pCurDir);
	}
	
	/* free scan stack */
	ScanStackFree(pScanInfo->hStack);
	
	/* free scaned file lists of pScanInfo*/
	pFree = pScanInfo->FirstList;
	while (pFree) {
		pNextFree = pFree->Next;
		free(pFree);
		pFree = pNextFree;
	}
	
	/* free pScanInfo last */
	free(pScanInfo);
}
#include <pthread.h>
