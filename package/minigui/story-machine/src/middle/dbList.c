#include "dbList.h"

/*
__db_list_put_tail					__db_list_put_head    __db_list_pop
               ||                                  ||      /\
               ||                                  ||      ||
			   \/                                  \/      ||
			   ----------        ----------        ----------      ----------
			   |        |        |        |        |        |      |        |
			   |  prev  | <----  |  head  |  ----> | next   |----> | target |---->
			   |        |        |        |        |        |      |        |
			   |        |        |        |        |        |      |        |
			   ----------        ----------        ----------      ----------



*/

int errno;
db_list_t* db_list_create(void )
{
	db_list_t* list_head;
	list_head=(db_list_t* )malloc(sizeof(db_list_t));
	if(list_head==NULL){
	      errno=ENOMEM;
	      return NULL;
	}
	list_head->limit_size=0;
	list_head->head=(db_lnode_t* )malloc(sizeof(db_lnode_t));
	if(list_head->head==NULL){
		free(list_head);
		errno=ENOMEM;
		return NULL;
	}
	list_head->head->next=list_head->head->prev=list_head->head;
	list_head->head->data=NULL;
	list_head->current = list_head->head;
	return list_head;
}

int is_list_empty(db_list_t *list_head)
{
	return list_head->limit_size==0?1:0;
}

void __db_list_clear(db_list_t* list_head)
{
	if(list_head == NULL){
//		sm_warn("list head is NULL\n");
		return;
	}

	int i;
	db_lnode_t* current = list_head->head->next;
	while(list_head->limit_size > 0){
		__db_list_delete_node(list_head, &current);
		current = list_head->head->next;
	}
}

inline int __db_list_put_tail(db_list_t* list_head, void* new_node_data)
{
	if(NULL == list_head || NULL == new_node_data){
		errno=EINVAL;
		return -1;
	}
	db_lnode_t *new_node = NULL, *tmp_node = NULL;
	new_node=(db_lnode_t *)malloc(sizeof(db_lnode_t));
	if(new_node == NULL){
		printf("[%s: %d]->{%s} malloc failed\n", __FILE__, __LINE__, __func__);
		return -1;
	}
	new_node->data = new_node_data;

	tmp_node = list_head->head->prev;
	new_node->prev = tmp_node;
	tmp_node->next = new_node;
	new_node->next = list_head->head;
	list_head->head->prev = new_node;
	list_head->limit_size++;

	return 0;
}

inline int __db_list_put_head(db_list_t* list_head, void* new_node_data)
{
	if(NULL == list_head || NULL == new_node_data){
		errno=EINVAL;
		return -1;
	}
	db_lnode_t *new_node = NULL, *tmp_node = NULL;
	new_node=(db_lnode_t *)malloc(sizeof(db_lnode_t));
	if(new_node == NULL){
		printf("[%s: %d]->{%s} malloc failed\n", __FILE__, __LINE__, __func__);
		return -1;
	}
	new_node->data = new_node_data;

	tmp_node = list_head->head->next;
	new_node->next = tmp_node;
	tmp_node->prev = new_node;
	new_node->prev = list_head->head;
	list_head->head->next = new_node;
	list_head->limit_size++;

	return 0;
}

inline void *__db_list_pop(db_list_t* list_head)
{
	if(list_head == NULL){
		return NULL;
	}
	db_lnode_t* current = list_head->head->next;
	if(current != list_head->head){
		void *ret = current->data;
//		printf("ind: [%d]\n", current->index);
		__db_list_delete_node(list_head, &current);
		return ret;
	}else{
		return NULL;
	}
}

inline void *__db_list_search_node(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ))
{
	db_lnode_t* current;
	int count = 0;
	if(list_head == NULL || compare == NULL){
		errno=EINVAL;
		return NULL;
	}
	current=list_head->head->next;
	do{
		if(compare(current->data,find_data)!=0){
//			printf("count: [%d]\n", count++);
			current=current->next;
		}else{
//			printf("seach result count: [%d]\n", count);
			return current->data;;
		}
//		printf("head: [%p], current: [%p] [%p]\n", list_head->head, current, current->next);
	}while(current != list_head->head);
	return NULL;
}

inline void *__db_list_search_and_pop(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ))
{
	void *ret = NULL;
	db_lnode_t* current;
	if(list_head == NULL || find_data == NULL || compare == NULL){
		errno=EINVAL;
		return NULL;
	}
	current=list_head->head->next;
	do{
		if(compare(current->data,find_data)!=0){
			current=current->next;
		}else{
			ret = current->data;
			__db_list_delete_node(list_head, &current);
			return ret;
		}
	}while(current!=list_head->head);
	return NULL;
}

inline int __db_list_delete_node(db_list_t* list_head, db_lnode_t **node)
{
	if(list_head == NULL || list_head->limit_size == 0|| node == NULL || *node == NULL){
		errno=EINVAL;
		return -1;
	}
//	printf("current: [%p], next: [%p], prev: [%p], head: [%p], hnext:[%p]\n", (*node), (*node)->next, (*node)->prev, list_head->head, list_head->head->next);
	(*node)->next->prev = (*node)->prev;
	(*node)->prev->next = (*node)->next;
//	printf("nprev: [%p], pnext: [%p], head: [%p], hnext:[%p]\n", (*node)->next->prev, (*node)->prev->next, list_head->head, list_head->head->next);
	free((*node));
	(*node) = NULL;
	list_head->limit_size--;
	return 0;
}

inline int __db_list_insert_before(db_list_t* list_head, int num, void* new_node_data)
{
	int counter=1;
	db_lnode_t* current;
	db_lnode_t* new_node;
	if(list_head==NULL){
		errno=EINVAL;
		return -1;
	}
	if(list_head->limit_size!=0){
		new_node=(db_lnode_t* )malloc(sizeof(db_lnode_t));
		if(new_node==NULL){
			errno=ENOMEM;
			return -1;
		}
		new_node->data=new_node_data;
		new_node->prev=new_node->next=NULL;
		if(num>0&&num<=list_head->limit_size){
			current=list_head->head;
			while(counter<num){
				counter++;
				current=current->next;
			}
			if(counter==1){
				list_head->head->prev=new_node;
				new_node->next=list_head->head;
				list_head->head=new_node;
				list_head->limit_size++;
				return 0;
			}
			current->next->prev=new_node;
			new_node->prev=current->prev;
			current->prev=new_node;
			new_node->next=current;
			list_head->limit_size++;
			return 0;
		}else{
			errno=EINVAL;
			free(new_node);
			return -1;
		}
	}else{
		if(num!=0){
			errno=EINVAL;
			return -1;
		}
		list_head->head->data=new_node_data;
		list_head->limit_size++;
		return 0;
	}
}
inline int __db_list_insert_after(db_list_t* list_head ,int num ,void* new_node_data)
{
#if 0
	if(list_head==NULL){
		errno=EINVAL;
		return -1;
	}
	int counter=0;
	db_lnode_t* current = NULL, new_node = NULL;

	if(num >= list_head->limit_size){

	}
#endif
}
#if 0
inline void __db_lnode_flush(db_list_t* list_head ,int num ,void* new_node_data)
{
	int counter=0;
	db_lnode_t* current;
	if(list_head==NULL || num<0 || num>list_head->limit_size){
		errno=EINVAL;
		return ;
	}
	current=list_head->head;
	while(counter!=num-1){
		current=current->next;
		counter++;
	}
	current->data=new_node_data;
}
#endif
inline void __db_list_delete(db_list_t* list_head ,int num)
{
	if(list_head == NULL || list_head->limit_size == 0 || num >= list_head->limit_size){
		return;
	}

	db_lnode_t* current = list_head->head;
	while(num-- >= 0){
		current = current->next;
	}
//	printf("delete index: [%d]\n", current->index);
//	printf("current: [%p], prev: [%p], next: [%p], head: [%p], hnext:[%p]\n", current, current->prev, current->next, list_head->head, list_head->head->next);
	current->prev->next = current->next;
	current->next->prev = current->prev;
//	printf("cnprev: [%p], cpnext: [%p]\n", current->next->prev, current->prev->next);

	free(current);
	current = NULL;
	list_head->limit_size--;
}
void __db_list_destory(db_list_t* list_head)
{
	if(list_head == NULL){
		return;
	}
	db_lnode_t* current=list_head->head->next;
	db_lnode_t *tmp = NULL;
	int i=0;
	for(;i<list_head->limit_size;i++){
		tmp = current;
		current=current->next;
//		printf("free id: [%d]\n", i);
		free(tmp);
		tmp = NULL;
	}
	free(list_head->head);
	list_head->head = NULL;
	free(list_head);
}

inline void* __db_list_visit(db_list_t* list_head ,int num)
{
	if(list_head == NULL || list_head->limit_size == 0 || num < 0 ||num >= list_head->limit_size){
		return NULL;
	}
	int counter;
	db_lnode_t* current=list_head->head->next;
	for(counter=0;counter<num;counter++){
		current=current->next;
	}
//	printf("visit current: [%p], index: [%d]\n", current, current->index);
//	printf("visiting: current: [%p], next: [%p] head: [%p]\n", current, current->next, list_head->head);
	return current->data;
}

inline void __db_list_travel(db_list_t* list_head ,void(*do_function)(void* ))
{
	if(list_head==NULL || list_head->limit_size == 0 || do_function == NULL){
		return ;
	}
	int i=0;
	db_lnode_t* current=list_head->head->next;
	db_lnode_t *tmp = NULL;
	while(current != list_head->head){
		tmp = current;
		current=current->next;
		do_function(current->data);
	}
}

inline void *get_current_data(db_list_t *list_head)
{
	if(list_head == NULL || list_head->limit_size == 0){
		return NULL;
	}
	list_head->current = list_head->current->next;
	if(list_head->current == list_head->head){
		list_head->current = list_head->current->next;
	}
	return list_head->current->data;
}

void put_current_in_position(db_list_t *list_head, int i)
{
	if(list_head == NULL || list_head->limit_size == 0 || i > list_head->limit_size){
		return ;
	}
	list_head->current = list_head->head;
	while(i-- > 0){
		list_head->current = list_head->current->next;
	}
}

inline int __db_list_search(db_list_t* list_head,void* find_data ,int(*compare)(void* ,void* ))
{
	int counter=0;
	db_lnode_t* current;
	if(list_head==NULL || find_data == NULL){
		errno=EINVAL;
		return -1;
	}
	current=list_head->head;

	while(current->next!=list_head->head){
		if(compare(current->data,find_data)!=0){
			current=current->next;
			counter++;
		}else{
			return counter;
		}
	}

	return -1;
}
