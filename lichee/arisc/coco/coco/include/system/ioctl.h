#ifndef __IOCTL_H__
#define __IOCTL_H__

s32 __ioctl(u32 devid, u32 requset, va_list args);
s32 __div(s32 value, s32 div);
void load_bytb(void);
#endif

