/* (C) Copyright 2019-2025
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * chengguo <chengguo@allwinnertech.com>
 *
 * some simple description for this code
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#ifndef _AIPU_SUNXI_PLATFORM_H_
#define _AIPU_SUNXI_PLATFORM_H_

#if defined(CONFIG_ARCH_SUN50IW11)
#ifndef BUILD_ZHOUYI_V1
#define BUILD_ZHOUYI_V1
#endif

#define BUILD_PLATFORM_DEFAULT 1
#define BUILD_DEBUG_VERSION 0
#else
#define BUILD_DEBUG_VERSION 1
#endif

#endif
