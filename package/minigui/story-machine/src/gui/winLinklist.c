#include "winLinklist.h"

db_list_t *winLinklistCreate(db_list_t *winLinklistHead)
{
	winLinklistHead = db_list_create();
	return winLinklistHead;
}

void winLinklistAdd(db_list_t *winLinklistHead, void *winData)
{
	__db_list_put_tail(winLinklistHead, winData);

//	winNode *curWinNode = (winNode *)winData;
//	curWinNode->nodeNum = winLinklistHead->limit_size;
//	__db_list_insert_after(&winLinklistHead ,winLinklistHead->limit_size ,winData);
}

int winLinklistDelete(db_list_t *winLinklistHead, void *winData)
{
	if(winData == NULL){
		printf("[%s: %d] winLinklistDelete winData is NULL\n", __FILE__, __LINE__);
		return -1;
	}
	winNode *curWinNode = (winNode *)winData;
	__db_list_delete(&winLinklistHead, curWinNode->nodeNum);
	return 0;
}

static inline int findWinNode(void *dest, void *src)
{
	if(dest == NULL || src == NULL){
		printf("[%s: %d] paragarms error\n", __FILE__, __LINE__);
		return -1;
	}
	winNode *destWinNode = (winNode *)dest;
	const char *className = (char *)src;
//	printf("[%s: %d]->{%s}, className: {%s}, destWinNode->className: {%s}\n", __FILE__, __LINE__, __func__, className, destWinNode->className);
	int len = strlen(className);
	return strncmp(destWinNode->className, className, len);
}

void *winLinklistFind(db_list_t *winLinklistHead, const char *className)
{
	return __db_list_search_node(winLinklistHead, (void *)className, findWinNode);

#if 0
//	printf("[%s: %d]->{%s}\n", __FILE__, __LINE__, __func__);
	int nodeNum = __db_list_search(&winLinklistHead, (void *)className, findWinNode);
//	printf("[%s: %d]->{%s}, nodeNum: %d, className: {%s}\n", __FILE__, __LINE__, __func__, nodeNum, className);
	if(-1 == nodeNum){
		printf("{%s}  winLinklistHead doesn't have {%s}\n", __func__, className);
		return NULL;
	}
	return __db_list_visit(&winLinklistHead, nodeNum);
#endif
}
