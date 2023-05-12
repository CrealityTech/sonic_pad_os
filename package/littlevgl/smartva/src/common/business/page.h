#ifndef __PAGE_H__
#define __PAGE_H__

typedef enum
{
    PAGE_NONE = 0,
    PAGE_HOME,
    PAGE_MUSIC,
    PAGE_MOVIE,
	PAGE_CALCULATOR,
    PAGE_CALENDAR,
    PAGE_EXPLORER,
    PAGE_FOLDER,
	PAGE_PHOTO,
    PAGE_SETTING,
    PAGE_EXAMPLE,
    PAGE_MAX
} page_id_t;

typedef struct
{
	 int (*on_create)(void);
	 int (*on_destroy)(void);
	 int (*on_show)(void);
	 int (*on_hide)(void);
} page_ops_t;

typedef struct
{
	 page_id_t id;
	 void *user_data;
} page_info_t;

typedef struct
{
	page_ops_t ops;
	page_info_t info;
} page_interface_t;

void page_init();
void page_uninit();
void reg_page(page_interface_t *page);
void unreg_page(page_interface_t *page);
void create_page(page_id_t id);
void destory_page(page_id_t id);
void show_page(page_id_t id);
void hide_page(page_id_t id);
page_id_t current_page(void);

#define REG_PAGE(name)\
do{\
    extern void REGISTER_##name(void);\
    REGISTER_##name();\
}while(0)


#endif
