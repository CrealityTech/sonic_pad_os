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

#ifndef __SIZE_UNIT__
#define __SIZE_UNIT__

struct size_unit {
	unsigned long long bytes;
	char str[128];
};

int str_to_size(struct size_unit *unit, const char *str);
int num_to_size(struct size_unit *unit, unsigned long long num);

#endif
