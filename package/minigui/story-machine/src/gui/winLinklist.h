#ifndef __WINLINKLIST_H__
#define __WINLINKLIST_H__

#include "dbList.h"
#include "middle_inc.h"
#include "winInclude.h"

typedef struct winNode{
	const char *className;
	int nodeNum;
	int (*winStart)(HWND parent, void *data);
	HWND owner;
	void *data;
}winNode;

extern db_list_t *winLinklistCreate(db_list_t *winLinklistHead);
extern void winLinklistAdd(db_list_t *winLinklistHead, void *winData);
extern int winLinklistDelete(db_list_t *winLinklistHead, void *winData);
extern void *winLinklistFind(db_list_t *winLinklistHead, const char *className);

#endif
