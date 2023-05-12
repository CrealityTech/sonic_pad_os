#ifndef __VA_IMAGE_H__
#define __VA_IMAGE_H__

//#define LV_IMAGE_PATH	"/tmp/"
#define LV_IMAGE_PATH	"/usr/res/image/"
#define BYTE_PER_PIXEL	4

void *parse_image_form_file(char *path);
int free_image(void *buf);

#endif /*__VA_IMAGE_H__*/


