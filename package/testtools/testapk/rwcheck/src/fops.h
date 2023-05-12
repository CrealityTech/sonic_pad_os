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

#ifndef __FOPS_H__
#define __FOPS_H__

#include <sys/types.h>
#include <stdbool.h>

bool is_existed(const char *path);
bool is_dir(const char *path);
bool is_file(const char *path);
int copy_file(const char *in_path, const char *out_path, unsigned long long bytes,
		unsigned long long buf_bytes);
int append_crc(const char *path);
char *abs_path(const char *path);
ssize_t file_size(int fd);

#endif
