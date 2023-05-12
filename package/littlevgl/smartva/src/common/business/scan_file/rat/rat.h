#ifndef __RAT_H__
#define __RAT_H__
#include <pthread.h>
#include "ScanPri.h"
#include "Scan.h"
#include "rat_common.h"
#include "rat.h"

typedef ScanedEntry_t rat_entry_t;//to store rat entry information.

typedef struct  __rat_list
{
	HSCAN	hscan;
	rat_media_type_t media_type;
	char	str_path[RAT_MAX_FULL_PATH_LEN];
	int	open_cnt;	//引用计数
	int   npl_cur_index;
	ScanListCursor_t NplCursor;	/* point to the npl cursor*/
	rat_play_mode_e play_mode;
    int   modify_flag;//修改标记，标记list里面的文件是否被删除或复制
}__rat_list_t;

pthread_mutex_t g_rat_mutex;
#define RAT_ENTER_CRITICAL_SECTION do { pthread_mutex_lock(&g_rat_mutex);} while(0)

#define RAT_LEAVE_CRITICAL_SECTION do { pthread_mutex_unlock(&g_rat_mutex);} while(0)

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
HRAT rat_open(const char *pDir, rat_media_type_t media_type, unsigned int OnceCnt);

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
int rat_move_cursor_to_first(HRAT hRat);

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
unsigned int rat_move_cursor_forward(HRAT hRat, unsigned int cnt);


/*
*******************************************************************************
*********************
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
*******************************************************************************
*********************
*/
unsigned int rat_move_cursor_backward(HRAT hRat, unsigned int cnt);


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
__bool rat_is_cursor_end(HRAT hRat);

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
int rat_close(HRAT hRat);

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
int rat_get_cur_item_info(HRAT hRat, rat_entry_t* entry);

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
int rat_get_item_info_by_index(HRAT hRat, int index, rat_entry_t* entry);

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
int rat_set_file_for_play(HRAT hRat, char* file);

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
unsigned int rat_get_cur_scan_cnt(HRAT hRat);

int rat_init(void);

int rat_deinit(void);

//clear rat list and npl by root
int rat_clear_all_list_by_root(void* arg);

//clear all rat list and npl
int rat_clear_all_list(void);

rat_media_type_t rat_get_file_type(const char* file_name);

int rat_set_modify_flag(rat_media_type_t media_type, int flag);

int rat_set_modify_flag_all(int flag);

int rat_set_search_cb(prat_search_cb cb);


#endif//__RAT_H__
