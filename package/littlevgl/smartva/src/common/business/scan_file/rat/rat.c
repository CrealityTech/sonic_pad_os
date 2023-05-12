#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rat_common.h"
#include "ScanPri.h"
#include "Scan.h"
#include "rat_npl.h"
#include "rat.h"

extern __rat_list_t* g_rat_npl[RAT_MEDIA_TYPE_MAX];
extern __rat_list_t g_null_rat_npl;

#define RAT_MAX_LIST_CNT	(RAT_MEDIA_TYPE_MAX*2)

__rat_list_t g_rat_list[RAT_MAX_LIST_CNT] = {0};

static __bool has_init = 0;
static const unsigned char _ToUpperTable[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
    'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
    'X',  'Y',  'Z', 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
    'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
    'X',  'Y',  'Z', 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

int SLIB_toupper(int c)

{
    return (int)_ToUpperTable[c];
}

int  SLIB_strnicmp(const char * p1_str, const char * p2_str, unsigned int len_max)
{
    char   * p1_str_next;
    char   * p2_str_next;
    int     cmp_val;
    unsigned int cmp_len;


    if (len_max == 0)                               /* If cmp len equals zero, rtn 0                                */
    {
        return ((int )0);
    }

    if (p1_str == (char *)0)
    {
        if (p2_str == (char *)0)
        {
            return ((int )0);                      /* If BOTH str ptrs NULL,  rtn 0      (see Note #2a).           */
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
    cmp_len     = 0;
    while ((SLIB_toupper(*p1_str) == SLIB_toupper(*p2_str)) && /* Cmp strs until non-matching char (see Note #2d) ..           */
           (*p1_str      != (char  )0) &&           /* .. or NULL char(s)               (see Note #2e) ..           */
           ( p1_str_next != (char *)0) &&           /* .. or NULL ptr(s) found (see Notes #2f, #2g, & #2h);         */
           ( p2_str_next != (char *)0) &&
           ( cmp_len     <  (unsigned int)len_max))     /* .. or len nbr chars cmp'd        (see Note #2j).             */
    {
        p1_str_next++;
        p2_str_next++;
        p1_str++;
        p2_str++;
        cmp_len++;
    }


    if (cmp_len == len_max)                         /* If strs     identical for len nbr of chars,                  */
    {
        return ((int )0);                          /* ... rtn 0 (see Note #2j).                                    */
    }

    if (*p1_str != *p2_str)                         /* If strs NOT identical, ...                                   */
    {
         cmp_val = (int )SLIB_toupper(*p1_str) - (int )SLIB_toupper(*p2_str);  /* ... calc & rtn char diff  (see Note #2d1).              */
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
                cmp_val = (int )0;                 /* ... rtn 0                                                    */
            }
            else                                    /* If p1_str_next NULL, ...                                     */
            {
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
*             							
*
*  Description:
*				-	open the rat process.
*
*  Parameters:
*   pDir		-  	pointer to directory(e:\ f:\....  ).
*   media_type	-  	types of media file.
*   OnceCnt		-  	read items number when read once. 
*					if zero, I will read all items one time.
*
*  Return value:
*   NULL   		-  	setup rat process failed.
*	others		-	handle to rat process.
****************************************************************************************************
*/
HRAT rat_open(const char *pDir, rat_media_type_t media_type, unsigned int OnceCnt)
{
	int i;
	HSCAN hscan;
	__u64 file_type;
    __bool rat_npl_need_update;
    __bool rat_list_research;
    
    rat_list_research = EPDK_FALSE;//文件列表是否被重新搜索
    rat_npl_need_update = EPDK_FALSE;//文件列表被重新搜索后，播放列表是否需要更新

	//media_type = RAT_MEDIA_TYPE_VIDEO_AND_AUDIO;	// for test
//    printf("rat_open: pDir=%s media_type=%d OnceCnt=%d\n", pDir, media_type, OnceCnt);

	RAT_ENTER_CRITICAL_SECTION;
	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (0 == strcmp(pDir, g_rat_list[i].str_path)
			&& media_type == g_rat_list[i].media_type)
		{
			if (0 == g_rat_list[i].open_cnt)
			{
				RAT_LEAVE_CRITICAL_SECTION;
				printf("error, open_cnt=0\n");
				return 0;
			}
			if (0 == g_rat_list[i].hscan)
			{
				RAT_LEAVE_CRITICAL_SECTION;
				printf("error, g_rat_list[i].hrat=NULL\n");
				return 0;
			}

			if(0 == g_rat_list[i].modify_flag)//如果该列表未被复制或删除过，则无需重新搜索
            {
    			g_rat_list[i].open_cnt++;
    			ScanMoveToFirst(g_rat_list[i].hscan);
    			RAT_LEAVE_CRITICAL_SECTION;
    			return (HRAT)&g_rat_list[i];
            }
            else
            {
                rat_list_research = EPDK_TRUE;
                    
                //如果播放列表所在文件列表被重新搜索了，则需把文件列表同步到播放列表
                if(NULL != g_rat_npl[media_type]
                    && g_rat_npl[media_type] == &g_rat_list[i])
                {
                    rat_npl_need_update = EPDK_TRUE;
                }
                
                ScanFree(g_rat_list[i].hscan);
				g_rat_list[i].hscan = 0;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;
                g_rat_list[i].modify_flag = 0;   
                break;
            }
		}
	}

	if (RAT_MEDIA_TYPE_PIC == media_type)
	{
		file_type = PHOTO_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_VIDEO == media_type)
	{
		file_type = MOVIE_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_AUDIO == media_type)
	{
		file_type = MUSIC_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_EBOOK == media_type)
	{
		file_type = EBOOK_SCAN_ALL;
	}
    else if(RAT_MEDIA_TYPE_ALL == media_type)
    {
        file_type = 0;
    }
	else if(RAT_MEDIA_TYPE_VIDEO_AND_AUDIO == media_type)
    {
        file_type = MOVIE_SCAN_ALL|MUSIC_SCAN_ALL;
    }

	else
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return 0;
	}

	hscan = ScanFilesBegin(pDir, file_type, OnceCnt);
	if (0 == hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		printf("ScanFilesBegin fail...\n");
		return 0;
	}

    //文件列表被重新搜索后，其链表首地址被存储的位置不变，不需要重新查找存储位置
    //这样可以避免文件列表重新搜索后，播放列表没有被同步的问题
    if(EPDK_FALSE == rat_list_research)
    {
    	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    	{
    		if (0 == g_rat_list[i].hscan
    			&& 0 == g_rat_list[i].open_cnt)
    		{
    			break;
    		}
    	}
    }

	if (i >= RAT_MAX_LIST_CNT)
	{
		printf("no free space for rat list...\n");
		ScanFree(hscan);
		RAT_LEAVE_CRITICAL_SECTION;
		return 0;
	}

	ScanMoveToFirst(hscan);
	g_rat_list[i].hscan = hscan;
	g_rat_list[i].media_type = media_type;

    //如果列表被重新搜索，说明不是第一次搜索，引用计数加1。
    //否则引用计数为1
    if(EPDK_FALSE == rat_list_research)
    {
	    g_rat_list[i].open_cnt = 1;
    }
    else
    {
        g_rat_list[i].open_cnt++;
    }

    if(g_rat_list[i].open_cnt < 1)//保护一下
    {
        g_rat_list[i].open_cnt = 1;
    }
	strcpy(g_rat_list[i].str_path, pDir);
    //如果播放列表被同步，则需重新设定正确的npl_cur_index
    if(EPDK_TRUE == rat_npl_need_update)
    {
        int total_cnt;

        total_cnt = ScanGetCurCnt(hscan);
        
        //g_rat_list[i].npl_cur_index = 0;	//不在这里做，由应用程序从robin取得文件名，重新调用set_file_for_play
        
        if(g_rat_list[i].npl_cur_index > total_cnt-1)
        {
            g_rat_list[i].npl_cur_index = total_cnt-1;
        }

        if(g_rat_list[i].npl_cur_index < 0)
        {
            g_rat_list[i].npl_cur_index = 0;
        }
    }
    else
    {
        g_rat_list[i].npl_cur_index = 0;	
    }	
	RAT_LEAVE_CRITICAL_SECTION;   

	return (HRAT)&g_rat_list[i];
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat scan handle cursor position to first entry.
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int rat_move_cursor_to_first(HRAT hRat)
{
	int ret; 

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	RAT_ENTER_CRITICAL_SECTION;
	ret = ScanMoveToFirst(pRatList->hscan);
	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat handle cursor position forward.
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   unsigned int   	-  	count of cursor has actually move.
*	
****************************************************************************************************
*/
unsigned int rat_move_cursor_forward(HRAT hRat, unsigned int cnt)
{
    unsigned int i;    
	int ret;     

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}

    i = 0;
	RAT_ENTER_CRITICAL_SECTION;
    while(!ScanIsDone(pRatList->hscan))
    {
        if(i >= cnt)
        {
            break;
        }
        ret = ScanMoveToNext(pRatList->hscan);
        if(EPDK_FAIL == ret)
        {
            break;
        }
        i++;
    }
	
	RAT_LEAVE_CRITICAL_SECTION;
	return i;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat handle cursor backward
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   unsigned int   	-  	count of cursor has actually move.
*	
****************************************************************************************************
*/
unsigned int rat_move_cursor_backward(HRAT hRat, unsigned int cnt)
{
    unsigned int i;
	int ret; 
	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}
	i = 0;
	RAT_ENTER_CRITICAL_SECTION;
    while(i < cnt)
    {
	    ret = ScanMoveToPrev(pRatList->hscan);
        if(EPDK_FAIL == ret)
        {
            break;
        }
        i++;
    }
	RAT_LEAVE_CRITICAL_SECTION;
	return i;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	check npl handle cursor position pointing after the end entry.
*				  	
*  Parameters:
*	hRat		-	handle to npl information.
*
*  Return value:
*   EPDK_TRUE   -  	cursor position pointing the end entry.
*	EPDK_FALSE	-	cursor position not pointing at the end entry.
****************************************************************************************************
*/
__bool rat_is_cursor_end(HRAT hRat)
{
	int ret; 

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	ret = ScanIsDone(pRatList->hscan);
	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	free npl handle.
*
*  Parameters:
*	hRat		-	handle to npl information.
*
*  Return value:
*   	   		NONE
****************************************************************************************************
*/
int rat_close(HRAT hRat)
{
	int i;
	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}

	RAT_ENTER_CRITICAL_SECTION;
	
	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (pRatList->hscan == g_rat_list[i].hscan)
		{
			g_rat_list[i].open_cnt--;
			if (0 == g_rat_list[i].open_cnt)
			{
				ScanFree(pRatList->hscan);
				g_rat_list[i].hscan = 0;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;
                g_rat_list[i].modify_flag = 0;
				if (g_rat_npl[g_rat_list[i].media_type] == pRatList)//删除播放列表
				{
					g_rat_npl[g_rat_list[i].media_type] = NULL;
				}
			}
			break;
		}
	}
	if (i >= RAT_MAX_LIST_CNT)
	{
		printf("error: hrat not found in list...\n");
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	RAT_LEAVE_CRITICAL_SECTION;
	return 0;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current npled file entry.
*				  	
*  Parameters:
*	hRat		-	handle to npl information.
*	entry		-	to store entry info
*  Return value:
*   EPDK_OK   		-  	get current entry ok.
*	EPDK_FAIL		-	get current entry failed.
****************************************************************************************************
*/
int rat_get_cur_item_info(HRAT hRat, rat_entry_t* entry)
{
	int ret; 
	__rat_list_t* pRatList;
	rat_entry_t* pEntry; 
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return EPDK_FAIL;
	}

	RAT_ENTER_CRITICAL_SECTION;
	pEntry = ScanGetCurrent(pRatList->hscan);	
    if(pEntry)
    {
	    memcpy(entry, pEntry, sizeof(rat_entry_t));
        ret = EPDK_OK;
    }
    else
    {
        ret = EPDK_FAIL;
    }
	RAT_LEAVE_CRITICAL_SECTION;
	
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	set current file to be play.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	file		-   full file path
*
*  Return value:
*   EPDK_FAIL   		-  	no such file
*   EPDK_TRUE   		-  	ok
*	others		-	
****************************************************************************************************
*/
int rat_set_file_for_play(HRAT hRat, char* file)
{
	int ret; 
	__rat_list_t* pRatList;	
	HRATNPL hnpl;
	pRatList = (__rat_list_t*)hRat;	
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	g_rat_npl[pRatList->media_type] = pRatList;

	RAT_LEAVE_CRITICAL_SECTION;
	
	//设置当前播放条目
	hnpl = rat_npl_open(pRatList->media_type);
	if (0 != hnpl)
	{
		ret = rat_npl_file2index(hnpl, file);
		if (-1 == ret)
		{
			ret = 0;
		}

		rat_npl_set_cur(hnpl, ret);
		rat_npl_close(hnpl);
	}
	return 0;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current item info by index.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	index		-   zero base index
*	entry		-	to store entry info
*  Return value:
*   EPDK_OK		   		-  	current item info
*   EPDK_FAIL	   		-  	no such item
*	others		-	
****************************************************************************************************
*/
int rat_get_item_info_by_index(HRAT hRat, int index, rat_entry_t* entry)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	rat_entry_t* pEntry; 
	int n;
    int ret;
    
	if (0 == hRat)
	{
		return EPDK_FAIL;
	}		
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRat;
	if (0 == rat_list->hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || index >= pScanInfo->CurCnt)
	{
        printf("pScanInfo->CurCnt=%d\n", pScanInfo->CurCnt);
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
	
	__ScanMoveCursorToFirst(rat_list->hscan, &pScanInfo->TmpCursor);
	n = 0;
	while (n < index)
	{
		if (EPDK_OK != __ScanMoveCursorToNext(rat_list->hscan, &pScanInfo->TmpCursor))
		{
			break;
		}
		n++;
	}
	if (n == index)
	{
		pEntry = __ScanGetCurrentCursorData(rat_list->hscan, &pScanInfo->TmpCursor);
        if(pEntry)
        {
		    memcpy(entry, pEntry, sizeof(rat_entry_t));
            ret = EPDK_OK;
        }
        else
        {
            ret = EPDK_FAIL;
        }
		RAT_LEAVE_CRITICAL_SECTION;
		return ret;
	}
	
	RAT_LEAVE_CRITICAL_SECTION;
	return EPDK_FAIL;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current item info by index.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	index		-   zero base index
*
*  Return value:
*   rat_entry_t*   		-  	current item info
*   NULL		   		-  	no such item
*	others		-	
****************************************************************************************************
*/
unsigned int rat_get_cur_scan_cnt(HRAT hRat)
{	
	__rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	unsigned int cnt;

	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);
	cnt = pScanInfo->CurCnt;
	RAT_LEAVE_CRITICAL_SECTION;

	return cnt;
}

int rat_init(void)
{
	if (1 == has_init)
	{
		return EPDK_OK;	
	}
	pthread_mutex_init(&g_rat_mutex,NULL);
	memset(g_rat_list, 0, sizeof(g_rat_list));
	memset(&g_null_rat_npl, 0, sizeof(g_null_rat_npl));
    rat_set_search_cb(NULL);
	has_init = 1;	
	return EPDK_OK;
}

int rat_deinit(void)
{
	__u8    err;

	if (0 == has_init)
	{
		return EPDK_OK;
	}

    rat_clear_all_list();
    pthread_mutex_destroy(&g_rat_mutex);
    rat_set_search_cb(NULL);

	has_init = 0;
	
	return EPDK_OK;
}

//clear all rat list and npl
int rat_clear_all_list_by_root(void* arg)
{
	int i;
	char str_cur[2]={0};

	str_cur[0] = *(char *)arg;
	RAT_ENTER_CRITICAL_SECTION;

//	printf("rat_clear_all_list, str_cur=%s\n", str_cur);

	//删除播放列表
	for (i = 0 ; i < RAT_MEDIA_TYPE_MAX ; i++)
	{
		if (g_rat_npl[i])
		{
			if(0 == SLIB_strnicmp(str_cur, g_rat_npl[i]->str_path, 1))
			{
//				printf("clear rat npl:g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
				g_rat_npl[i] = NULL;            
			}
		}
		else
		{
			printf("g_rat_npl[%d] is null\n", i);
		}
	}

	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (g_rat_list[i].hscan)
		{            
//			printf("g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);

			if(0 == SLIB_strnicmp(str_cur, g_rat_list[i].str_path, 1))
			{
//				printf("clear rat list,g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
				ScanFree(g_rat_list[i].hscan);
				g_rat_list[i].hscan = 0;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;	                
				g_rat_list[i].modify_flag = 0;
			}
		}
		else
		{
			printf("g_rat_list[%d].hscan is null\n", i);
		}
	}
//	printf("%s%d%s\n", __FILE__, __LINE__, __func__);
	RAT_LEAVE_CRITICAL_SECTION;

	return 0;
}


//clear all rat list and npl
int rat_clear_all_list(void)
{
	int i;

	RAT_ENTER_CRITICAL_SECTION;

//	printf("rat_clear_all_list\n");

	//删除播放列表
	for(i = 0 ; i < RAT_MEDIA_TYPE_MAX ; i++)
	{
		if(g_rat_npl[i])
		{
//			printf("g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
			{
//				printf("clear rat npl:g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
				g_rat_npl[i] = NULL;            
			}
		}
		else
		{
//			printf("g_rat_npl[%d] is null\n", i);
		}
	}
	//删除文件列表
	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (g_rat_list[i].hscan)
		{            
//			printf("g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);

			{
//				printf("clear rat list,g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
				ScanFree(g_rat_list[i].hscan);
				g_rat_list[i].hscan = 0;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;	
				g_rat_list[i].modify_flag = 0;
			}
		}
		else
		{
			printf("g_rat_list[%d].hscan is null\n", i);
		}
	}
//	printf("%s%d%s\n", __FILE__, __LINE__, __func__);
	RAT_LEAVE_CRITICAL_SECTION;

	return 0;
}


rat_media_type_t rat_get_file_type(const char* file_name)
{
	rat_media_type_t media_type = RAT_MEDIA_TYPE_UNKNOWN;

	__ValidFileType(file_name, 0, (int *)(&media_type));
	
	return media_type;
}

int rat_set_modify_flag(rat_media_type_t media_type, int flag)
{
    int i;

    RAT_ENTER_CRITICAL_SECTION;
    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    {
        if (g_rat_list[i].hscan
            && media_type == g_rat_list[i].media_type)
        {                       
            g_rat_list[i].modify_flag = flag;
        }
    }
    RAT_LEAVE_CRITICAL_SECTION;
    return EPDK_OK;
}

int rat_set_modify_flag_all(int flag)
{
    int i;

    printf("rat_set_modify_flag_all: flag = %d\n", flag);

    RAT_ENTER_CRITICAL_SECTION;

    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    {
        if (g_rat_list[i].hscan)
        {                       
            g_rat_list[i].modify_flag = flag;
        }
    }
    
    RAT_LEAVE_CRITICAL_SECTION;

    return EPDK_OK;
}

int rat_set_search_cb(prat_search_cb cb)
{
    g_rat_search_cb = cb;
    return EPDK_OK;
}


