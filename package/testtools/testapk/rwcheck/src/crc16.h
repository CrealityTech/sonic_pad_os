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

#ifndef __CRC16_H
#define __CRC16_H

#include <stdbool.h>
#include <sys/types.h>

#define CRC16_MAX_BYTES ((unsigned long long)-1)
int get_crc16_by_buf(unsigned short *crc16, const char *buf, unsigned long long bytes);
int get_crc16_by_fd(unsigned short *crc16, int fd, unsigned long long bytes);
int get_crc16_by_path(unsigned short *crc16, const char *path, unsigned long long bytes);
bool check_crc16(unsigned short crc);

#endif
