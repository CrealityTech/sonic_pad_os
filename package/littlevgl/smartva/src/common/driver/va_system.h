#ifndef _VA_SYSTEM_H_
#define _VA_SYSTEM_H_

int va_tfcard_is_exist(void);
int va_tfcard_is_mount_correct(void);
int va_tfcard_get_total_size(void);
int va_tfcard_get_free_size(void);
int va_tfcard_get_block_size(void);
int va_tfcard_format_is_correct(void);
int va_tfcard_format(void);
int va_tfcard_fs_flex(void);

int va_rtc_get_local_time(struct tm *u_time);
int va_rtc_set_local_time(struct tm *u_time);

int va_usb_storage_adcard_on(void);
int va_usb_storage_adcard_off(void);

#endif
