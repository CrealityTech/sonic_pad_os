/**********************
 *      includes
 **********************/
#include "moc_example.h"
#include "ui_example.h"
#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
typedef struct
{
	example_ui_t ui;
} example_para_t;
static example_para_t para;


/**********************
 *  functions
 **********************/
static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		destory_page(PAGE_EXAMPLE);
		create_page(PAGE_HOME);
	}
}

static int create_example(void)
{
	para.ui.cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(para.ui.cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color = LV_COLOR_BLUE;
	cont_style.body.grad_color = LV_COLOR_BLUE;
	lv_cont_set_style(para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);
	lv_cont_set_layout(para.ui.cont, LV_LAYOUT_OFF);
	lv_cont_set_fit(para.ui.cont, LV_FIT_NONE);

	static lv_style_t back_btn_style;
	lv_style_copy(&back_btn_style, &lv_style_pretty);
	back_btn_style.text.font = &lv_font_roboto_28;
	lv_obj_t *back_btn = lv_btn_create(para.ui.cont, NULL);
	lv_obj_align(back_btn, para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_t *back_lable = lv_label_create(back_btn, NULL);
	lv_label_set_text(back_lable, LV_SYMBOL_LEFT);
	lv_obj_set_event_cb(back_btn, back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);

	example_auto_ui_create(&para.ui);

	// open tplayerdemo || willow

	lv_obj_t *scn = lv_scr_act();
	static lv_style_t scn_style;
	lv_style_copy(&scn_style, &lv_style_plain);
	scn_style.body.main_color.full = 0x00000000;
	scn_style.body.grad_color.full = 0x00000000;
	lv_obj_set_style(scn, &scn_style);

	cont_style.body.main_color.full = 0x00000000;
	cont_style.body.grad_color.full = 0x00000000;
	lv_cont_set_style(para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);

	static lv_style_t hbar_style;
	lv_style_copy(&hbar_style, &lv_style_pretty);
	hbar_style.body.main_color.full = 0x800000FF;
	hbar_style.body.grad_color.full = 0x800000FF;
	lv_btn_set_style(para.ui.button_1, LV_BTN_STYLE_REL, &hbar_style);
	lv_btn_set_style(para.ui.button_1, LV_BTN_STYLE_PR, &hbar_style);

	return 0;
}

static int destory_example(void)
{
	example_auto_ui_destory(&para.ui);
	lv_obj_del(para.ui.cont);

	return 0;
}

static int show_example(void)
{
	lv_obj_set_hidden(para.ui.cont, 0);

	return 0;
}

static int hide_example(void)
{
	lv_obj_set_hidden(para.ui.cont, 1);

	return 0;
}

static page_interface_t page_example = 
{
	.ops =
	{
		create_example,
		destory_example,
		show_example,
		hide_example,
	}, 
	.info =
	{
		.id         = PAGE_EXAMPLE, 
		.user_data  = NULL
	}
};

void REGISTER_PAGE_EXAMPLE(void)
{
	reg_page(&page_example);
}
