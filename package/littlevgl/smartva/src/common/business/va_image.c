/**********************
 *      includes
 **********************/
#include "va_image.h"
#include "lvgl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char *stbi_convert_rb(unsigned char *data, int w, int h, int Bpp)
{
	int i, j;
	unsigned char *buf;
	unsigned char tmp;
	
	buf = (unsigned char *)data;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			tmp = buf[2];
			buf[2] = buf[0];	// R
			buf[0] = tmp;		// B
			buf += 4;
 		}
	}
	return data;
}

void *parse_image_form_file(char *path)
{
	int w, h, n;
  	lv_img_dsc_t *dsc = NULL;
	unsigned char *data = NULL;
	
	dsc = (lv_img_dsc_t *)malloc(sizeof(lv_img_dsc_t));
	if(NULL == dsc) {
		goto err;
	}
	memset(dsc, 0, sizeof(lv_img_dsc_t));
	
	data = stbi_load(path, &w, &h, &n, BYTE_PER_PIXEL);
	if(NULL == data) {
		goto err;
	}
	data = stbi_convert_rb(data, w, h, BYTE_PER_PIXEL);
	
	dsc->header.w = w;
	dsc->header.h = h;
	dsc->header.always_zero = 0;
	if (BYTE_PER_PIXEL == 4) {
		dsc->header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	}
	else {
		dsc->header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	}

	dsc->data_size = w * h * BYTE_PER_PIXEL;
	dsc->data = data;
	return dsc;
	
	err:
	if(data) {
		stbi_image_free(data);
	}
	if(dsc) {
		free(dsc);
	}
	return NULL;
}

int free_image(void *buf)
{
	if(NULL == buf) {
		return -1;
	}
	
	lv_img_dsc_t *dsc = (lv_img_dsc_t *)buf;
	if(dsc->data) {
		stbi_image_free(dsc->data);
	}
	if(dsc) {
		free(dsc);
	}

	return 0;
}


