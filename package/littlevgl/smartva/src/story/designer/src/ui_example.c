/**********************
 *      includes
 **********************/
#include "ui_example.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/

/**********************
 *  images and fonts
 **********************/

/**********************
 *  functions
 **********************/
void example_auto_ui_create(example_ui_t *ui)
{

#ifdef LV_USE_BTN
	ui->button_1 = lv_btn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->button_1, 174, 0);
	lv_obj_set_size(ui->button_1, 626, 43);
#endif // LV_USE_BTN

}

void example_auto_ui_destory(example_ui_t *ui)
{
	lv_obj_clean(ui->cont);
}
