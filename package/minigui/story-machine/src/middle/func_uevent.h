#ifndef __FUNC__UENVET_H
#define __FUNC__UENVET_H


void func_uevent_init();
void post_func_sem(void *param);

typedef int (*func_callback_t)(void *param);
void func_register_callback(func_callback_t callback);

#endif
