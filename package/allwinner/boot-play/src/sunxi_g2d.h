#ifndef __SUNXI_G2D_H__
#define __SUNXI_G2D_H__

#include <stdint.h>
#include "g2d_driver_enh.h"

int init_config_g2d(uint32_t width, uint32_t height, g2d_fmt_enh fmt);
int uninit_config_g2d();
int g2d_rotate(uint8_t *in_buffer, uint64_t len, uint8_t *out_buffer);

#endif
