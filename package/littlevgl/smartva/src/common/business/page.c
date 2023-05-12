#include "page.h"
#include "dbList.h"
#include <assert.h>


db_list_t *page_head = NULL;

static inline int compare_by_id(void *dst, void *src)
{
	if(dst == NULL || src == NULL){
		printf("[%s: %d] paragarms error\n", __FILE__, __LINE__);
		return -1;
	}

	page_interface_t *dst_page = (page_interface_t *)dst;
	page_id_t src_id = *((page_id_t *)src);
	if(src_id == dst_page->info.id)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static void *find_page(page_id_t id)
{
	assert(page_head != NULL);
	return __db_list_search_node(page_head, (void *)&id, compare_by_id);
}

void page_init()
{
	assert(page_head == NULL);
	page_head = db_list_create();
}

void page_uninit()
{
	assert(page_head != NULL);
	__db_list_destory(page_head);
}

void reg_page(page_interface_t *page)
{
	assert(page_head != NULL);
	__db_list_put_tail(page_head, page);
}

void unreg_page(page_interface_t *page)
{
	;
}

static page_id_t current_id = PAGE_NONE;

void create_page(page_id_t id)
{
	page_interface_t *page = (page_interface_t *)find_page(id);
	if(page) {
		if (page->ops.on_create) {
			page->ops.on_create();
		}
		current_id = id;
	}
}

page_id_t current_page(void)
{
	return current_id;
}

void destory_page(page_id_t id)
{
	page_interface_t *page = (page_interface_t *)find_page(id);
	if(page) {
		if (page->ops.on_destroy) {
			page->ops.on_destroy();
		}
	}
}

void show_page(page_id_t id)
{
	page_interface_t *page = (page_interface_t *)find_page(id);
	if(page) {
		if (page->ops.on_show) {
			page->ops.on_show();
		}
	}
}

void hide_page(page_id_t id)
{
	page_interface_t *page = (page_interface_t *)find_page(id);
	if(page) {
		if (page->ops.on_hide) {
			page->ops.on_hide();
		}
	}
}
