#ifndef __DBLIST_H__
#define __DBLIST_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct db_lnode{
	void* data;
//	int index;
	struct db_lnode* prev;
	struct db_lnode* next;
}db_lnode_t;

typedef struct db_list{
	int limit_size;
	db_lnode_t *current;
	db_lnode_t* head;
}db_list_t;

extern db_list_t* db_list_create(void );
extern int is_list_empty(db_list_t *list_head);
extern int __db_list_insert_before(db_list_t* list_head, int num, void* new_node_data);
extern int __db_list_insert_after(db_list_t* list_head ,int num ,void* new_node_data);
//extern void __db_lnode_flush(db_list_t* list_head ,int num ,void* new_node_data);
extern void __db_list_delete(db_list_t* list_head ,int num);
extern void __db_list_destory(db_list_t* list_head);
extern void* __db_list_visit(db_list_t* list_head ,int num);
extern void __db_list_travel(db_list_t* list_head ,void(*do_function)(void* ));
extern int __db_list_search(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ));
extern int __db_list_delete_node(db_list_t* list_head, db_lnode_t **node);
extern void *__db_list_search_and_pop(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ));
extern int __db_list_put_tail(db_list_t* list_head, void* new_node_data);
extern void *__db_list_pop(db_list_t* list_head);
extern void *__db_list_search_node(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ));
extern int __db_list_put_head(db_list_t* list_head, void* new_node_data);
extern void __db_list_clear(db_list_t* list_head);
extern void *get_current_data(db_list_t *list_head);
extern void put_current_in_position(db_list_t *list_head, int i);


#endif
