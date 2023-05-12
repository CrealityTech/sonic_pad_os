// Copyright 2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RWCHECK_H__
#define __RWCHECK_H__

#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#include "log.h"
#include "sys_info.h"
#include "size_unit.h"
#include "fops.h"
#include "crc16.h"
#include "config.h"

#define VERSION "v0.1.0"
#define COMPILE "Compiled in " __DATE__ " at " __TIME__

/* operation macro */
#define min(x, y) ({				\
		typeof(x) min1 = (x);		\
		typeof(y) min2 = (y);		\
		(void) (&min1 == &min2);	\
		min1 < min2 ? min1 : min2; })
#define max(x, y) ({				\
		typeof(x) max1 = (x);		\
		typeof(y) max2 = (y);		\
		(void) (&max1 == &max2);	\
		max1 < max2 ? max2 : max1; })
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define ALIGN(x, a) __ALIGN((x), (a))
#define ALIGN_DOWN(x, a) __ALIGN((x) - ((a) - 1), (a))
#define __ALIGN(x, a) __ALIGN_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#endif /* __RWCHECK_H__ */
