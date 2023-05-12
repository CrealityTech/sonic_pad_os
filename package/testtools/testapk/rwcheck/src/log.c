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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "rwcheck.h"

#if DUMP_TRACE
#include <execinfo.h>
#endif

#define MAX_ERRLOG_LEN 1024
#define DUMP_TRACE_BUF_SIZE 1024
static struct errlog {
	int _errno;
	unsigned int line;
	const char *file;
	const char *func;
	char log[MAX_ERRLOG_LEN];
	struct errlog *next;
#if DUMP_TRACE
	void *trace[DUMP_TRACE_BUF_SIZE];
	int trace_cnt;
#endif
} *errlog;

void delay_errlog(int err_no, const char *file, unsigned int line,
		const char *func_name, const char *fmt, ...)
{
	va_list ap;
	struct errlog *err;

	err = malloc(sizeof(*err));
	if (!err) {
		printf("malloc for errlog failed\n");
		return;
	}
	memset(err, 0, sizeof(*err));

	err->_errno = err_no;
	err->file = file;
	err->line = line;
	err->func = func_name;

	va_start(ap, fmt);
	vsnprintf(err->log, MAX_ERRLOG_LEN, fmt, ap);
	va_end(ap);

	if (errlog) {
		struct errlog *next = errlog, *pre;
		while (next) {
			pre = next;
			next = errlog->next;
		}
		pre->next = err;
	} else {
		errlog = err;
	}

#if DUMP_TRACE
	err->trace_cnt = backtrace(err->trace, DUMP_TRACE_BUF_SIZE);
#endif
}

void show_errlog(void)
{
	struct errlog *err;
#if DUMP_TRACE
	char **str;
	int i;
#endif

	while ((err = errlog)) {
		fprintf(stderr, "\n\t--- ERROR INFO ---\n");
		fprintf(stderr, "\tfile: %s (%d)\n", err->file, err->line);
		fprintf(stderr, "\terrno: %s (%d)\n", strerror(err->_errno),
				err->_errno);
		fprintf(stderr, "\tinfo: %s", err->log);
		if (err->log[strlen(err->log) - 1] != '\n')
			fprintf(stderr, "\n");

#if DUMP_TRACE
		fprintf(stderr, "\tstack:\n");
		str = backtrace_symbols(err->trace, err->trace_cnt);
		for (i = 0; i < err->trace_cnt; i++)
			fprintf(stderr, "\t  | %s\n", str[i]);
		free(str);
#endif
		errlog = errlog->next;
		free(err);
	}
}
