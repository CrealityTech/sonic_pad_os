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

#ifndef __LOG_H_
#define __LOG_H_

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

/* enable this macro if you really need, to show all debug log */
#define DEBUG_LOG 0

void delay_errlog(int err_no, const char *file, unsigned int line,
		const char *func_name, const char *fmt, ...);
void show_errlog(void);

#define INFO(fmt, ...)							\
	do {								\
		fprintf(stdout, fmt, ## __VA_ARGS__);			\
		fflush(stdout);						\
	} while (0)

#define ERROR(fmt, ...)							\
	do {								\
		delay_errlog(errno, __FILE__, __LINE__, __func__,	\
				fmt, ## __VA_ARGS__);			\
	} while (0)

#if DEBUG_LOG

#define DEBUG(fmt, ...)							\
	do {								\
		delay_errlog(errno, __FILE__, __LINE__, __func__,	\
				fmt, ## __VA_ARGS__);			\
		show_errlog();						\
	} while (0)

#else /* DEBUG_LOG */

#define DEBUG(fmt, ...)

#endif /* DEBUG_LOG */
#endif /* __LOG_H_ */
