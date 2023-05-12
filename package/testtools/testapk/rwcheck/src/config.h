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

#ifndef __CONFIG_H_
#define __CONFIG_H_

/* macro for size */
#define KB ((unsigned long long)1024)
#define MB ( KB * 1024 )
#define GB ( MB * 1024 )

/* dump stack when error */
#define DUMP_TRACE 0
/* default orgin file name */
#define DEFAULT_ORGIN_FILE "rwcheck.org"
/* default orgin file size */
#define DEFAULT_ORG_SIZE (64 * KB)
/* default read/write file base name */
#define DEFAULT_OUTPUT "rwcheck.tmp"
/* default percent of total size of flash to test */
#define DEFAULT_PERCENT 95
/* default check times */
#define DEFAULT_TIMES 1
/* default buffer size for each read/write for paltfrom with little ddr */
#define DEFAULT_SMALL_BUF_SIZE (512 * KB)
/* default buffer size for each read/write for paltfrom with large ddr */
#define DEFAULT_LARGE_BUF_SIZE (8 * MB)
/* default boundary for little/large ddr */
#define DEFAULT_BUF_SIZE_DDR_LIMIT (64 * MB)
/* default begin size for up mode */
#define DEFAULT_UP_MODE_BEGIN_SIZE (4 * KB)
/* default end size for up mode */
#define DEFAULT_UP_MODE_END_SIZE (16 * GB)
/* default multiplying power for up mode */
#define DEFAULT_UP_MODE_POWER (2)
/* default max size for auto mode */
#define DEFAULT_AUTO_MODE_MAX_SIZE (4 * GB)
/* default end size for auto mode */
#define DEFAULT_AUTO_MODE_END_SIZE (64 * KB)
/* default worker when do write testing */
#define DEFAULT_JOBS 1
/* max worker when do write testing */
#define MAX_JOBS 10

#endif
