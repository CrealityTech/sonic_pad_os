#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "ScanPri.h"
#include "Scan.h"
#include "rat_common.h"
#include "rat.h"
#include "rat_npl.h"

__rat_list_t *g_rat_npl[RAT_MEDIA_TYPE_MAX] = {0};
__rat_list_t g_null_rat_npl = {0};

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	open npl by media type.
*				  	
*  Parameters:
*	rat_media_type_t: media type
*
*  Return value:    npl handle
*   NULL   		-  	
*	others		-	
****************************************************************************************************
*/
static __u32 rat_random(__u32 max) {
	srand((unsigned int)time(0));
	return rand()%max;
}
HRATNPL rat_npl_open(rat_media_type_t media_type)
{
	RAT_ENTER_CRITICAL_SECTION;
	if (g_rat_npl[media_type])
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return (HRATNPL)g_rat_npl[media_type];
	}
	
	RAT_LEAVE_CRITICAL_SECTION;
	return (HRATNPL)RAT_NULL_NPL;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	close npl by media type.
*				  	
*  Parameters:
*	hRatNpl:     npl handle
*
*  Return value:    
*   NULL   		-  	
*	others		-	
****************************************************************************************************
*/
int rat_npl_close(HRATNPL hRatNpl)
{

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current file index by file name.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
	
*
*  Return value:
*	int			success, index of the file in npl
*   -1   		-  	fail, no such file.
*	others		-	
****************************************************************************************************
*/
int rat_npl_file2index(HRATNPL hRatNpl, const char* full_file_path)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	ScanedEntry_t * pScanEntry;
	int index;
	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}

	RAT_ENTER_CRITICAL_SECTION;
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}

	pScanInfo = (ScanInfo_t*)(rat_list->hscan);

	if (pScanInfo->CurCnt <= 0)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
		
	__ScanMoveCursorToFirst(rat_list->hscan, &rat_list->NplCursor);
	index = 0;
	do 
	{
		pScanEntry = __ScanGetCurrentCursorData(rat_list->hscan, &rat_list->NplCursor);
		if (NULL == pScanEntry)
		{
			RAT_LEAVE_CRITICAL_SECTION;
			return -1;
		}
		if (0 == strcmp(full_file_path, pScanEntry->Path))
		{
			RAT_LEAVE_CRITICAL_SECTION;
			return index;
		}

		index++;
	} while(EPDK_OK == __ScanMoveCursorToNext(rat_list->hscan, &rat_list->NplCursor));
	
	RAT_LEAVE_CRITICAL_SECTION;
	return -1;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get full file name by index.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    success: EPDK_OK   fail: EPDK_FAIL
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_index2file(HRATNPL hRatNpl, __u32 index, char* file)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	ScanedEntry_t * pScanEntry;
	int n;
	if (0 == hRatNpl)
	{
		return EPDK_FAIL;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}

	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || index >= pScanInfo->CurCnt)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
		
	__ScanMoveCursorToFirst(rat_list->hscan, &rat_list->NplCursor);
	n = 0;
	while (n < index)
	{
		if (EPDK_OK != __ScanMoveCursorToNext(rat_list->hscan, &rat_list->NplCursor))
		{
			break;
		}
		n++;
	}
	if (n == index)
	{
		pScanEntry = __ScanGetCurrentCursorData(rat_list->hscan, &rat_list->NplCursor);
        if(!pScanEntry)
        {
            file[0] = 0;
        	RAT_LEAVE_CRITICAL_SECTION;
        	return EPDK_FAIL;
        }
        else
        {
            strcpy(file, pScanEntry->Path);
    		RAT_LEAVE_CRITICAL_SECTION;
    		return EPDK_OK;
        }		
	}
    else
    {
    	file[0] = 0;
    	RAT_LEAVE_CRITICAL_SECTION;
    	return EPDK_FAIL;
    }
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get next index in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    success: next index   fail: -1
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_get_next(HRATNPL hRatNpl)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	int n;

	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || rat_list->npl_cur_index >= pScanInfo->CurCnt)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}

	n = -1;

	if (RAT_PLAY_MODE_ONLY_ONCE == rat_list->play_mode)
	{
		if (-1 == rat_list->npl_cur_index)
		{
			rat_list->npl_cur_index = 0;
		}
		n = -1;
	}
	else if (RAT_PLAY_MODE_ROTATE_ONE == rat_list->play_mode)
	{
		if (-1 == rat_list->npl_cur_index)
		{
			rat_list->npl_cur_index = 0;
		}		
		n = rat_list->npl_cur_index;
	}
	else if (RAT_PLAY_MODE_ROTATE_ALL == rat_list->play_mode)
	{
		if (rat_list->npl_cur_index < pScanInfo->CurCnt-1)
		{
			rat_list->npl_cur_index++;
		}
		else
		{
			int count;
			count = ScanFilesOneTime((ScanInfo_t *)(rat_list->hscan));
			if (count > 0)
			{
				rat_list->npl_cur_index++;
			}
			else
			{
				rat_list->npl_cur_index = 0;
			}			
		}
		n = rat_list->npl_cur_index;
	}
	else if (RAT_PLAY_MODE_SEQUENCE == rat_list->play_mode)
	{
		if (rat_list->npl_cur_index < pScanInfo->CurCnt-1)
		{
			rat_list->npl_cur_index++;
			n = rat_list->npl_cur_index;
		}
		else
		{
			int count;
			count = ScanFilesOneTime((ScanInfo_t *)(rat_list->hscan));
			if (count > 0)
			{
				rat_list->npl_cur_index++;
				n = rat_list->npl_cur_index;
			}
			else
			{
				//rat_list->npl_cur_index = -1;
				n = -1;
			}			
		}
		
	}
	else
	{
		rat_list->npl_cur_index = rat_random(pScanInfo->CurCnt);
		if (rat_list->npl_cur_index < 0)
		{
			rat_list->npl_cur_index = 0;
		}
		if (rat_list->npl_cur_index > pScanInfo->CurCnt-1)
		{
			rat_list->npl_cur_index = pScanInfo->CurCnt-1;
		}
		n = rat_list->npl_cur_index;
	}

	RAT_LEAVE_CRITICAL_SECTION;
	return n;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get cur index in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    success: cur index   fail: -1
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_get_cur(HRATNPL hRatNpl)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	int ret;
	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || rat_list->npl_cur_index >= pScanInfo->CurCnt)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}

    ret = rat_list->npl_cur_index;

	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get prev index in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    
*   int 		-  prev index in npl
*	-1			-  no item
*	others		-	
****************************************************************************************************
*/
int rat_npl_get_prev(HRATNPL hRatNpl)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	int n;
	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || rat_list->npl_cur_index >= pScanInfo->CurCnt)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	n = -1;

	if (RAT_PLAY_MODE_ONLY_ONCE == rat_list->play_mode)
	{
		n = -1;
	}
	else if (RAT_PLAY_MODE_ROTATE_ONE == rat_list->play_mode)
	{
		if (-1 == rat_list->npl_cur_index)
		{
			rat_list->npl_cur_index = 0;
		}		
		n = rat_list->npl_cur_index;
	}
	else if (RAT_PLAY_MODE_ROTATE_ALL == rat_list->play_mode)
	{
		rat_list->npl_cur_index--;
		if (rat_list->npl_cur_index < 0)
		{
			rat_list->npl_cur_index = 0;
			n = -1;
		}
		else
		{
			n = rat_list->npl_cur_index;
		}		
	}
	else if (RAT_PLAY_MODE_SEQUENCE == rat_list->play_mode)
	{
		rat_list->npl_cur_index--;
		if (rat_list->npl_cur_index < 0)
		{
			rat_list->npl_cur_index = 0;
			n = -1;
		}
		else
		{
			n = rat_list->npl_cur_index;
		}	
	}
	else
	{
		rat_list->npl_cur_index = rat_random(pScanInfo->CurCnt-1);
		n = rat_list->npl_cur_index;
	}
	
	RAT_LEAVE_CRITICAL_SECTION;
	return n;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	set cur index in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    success: EPDK_OK   fail: EPDK_FAIL
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_set_cur(HRATNPL hRatNpl, int index)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || index >= pScanInfo->CurCnt)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}	
	
	rat_list->npl_cur_index = index;
	RAT_LEAVE_CRITICAL_SECTION;
	return EPDK_OK;	
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	set play mode in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*	play_mode	-   play mode
*
*  Return value:    success: EPDK_OK   fail: EPDK_FAIL
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_set_play_mode(HRATNPL hRatNpl, rat_play_mode_e play_mode)
{
	__rat_list_t* rat_list;
	
	if (0 == hRatNpl)
	{
		return EPDK_FAIL;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}

	rat_list->play_mode = play_mode;
	
	RAT_LEAVE_CRITICAL_SECTION;
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get play mode in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    success: EPDK_OK   fail: EPDK_FAIL
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_get_play_mode(HRATNPL hRatNpl)
{
	__rat_list_t* rat_list;
	rat_play_mode_e play_mode;

	if (0 == hRatNpl)
	{
		return RAT_PLAY_MODE_ONLY_ONCE;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return RAT_PLAY_MODE_ONLY_ONCE;
	}
	
	play_mode = rat_list->play_mode;
	
	RAT_LEAVE_CRITICAL_SECTION;
	return play_mode;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get total count in npl.
*				  	
*  Parameters:
*	hRatNpl		-	handle to npl information.
*
*  Return value:    total count in npl
*   NULL   		-  	.
*	others		-	
****************************************************************************************************
*/
int rat_npl_get_total_count(HRATNPL hRatNpl)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	int ret;
    
	if (0 == hRatNpl)
	{
		return -1;
	}

	if (RAT_NULL_NPL == hRatNpl)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRatNpl;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);

    ret = pScanInfo->CurCnt;
	
	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

