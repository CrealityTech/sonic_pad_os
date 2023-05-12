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
#include <stdlib.h>

#include "rwcheck.h"

void num_to_str(char *str, int len, unsigned long long bytes)
{
	if (bytes >= GB)
		snprintf(str, len, "%llu GB", bytes / GB);
	else if (bytes >= MB)
		snprintf(str, len, "%llu MB", bytes / MB);
	else if (bytes >= KB)
		snprintf(str, len, "%llu KB", bytes / KB);
	else
		snprintf(str, len, "%llu B", bytes);
}

int str_to_size(struct size_unit *unit, const char *str)
{
	unsigned long long size;
	char c;

	c = str[strlen(str) - 1];
	size = atol(str);
	if (size == 0) {
		ERROR("invalid size %s\n", str);
		return -EINVAL;
	}

	switch (c) {
	case '0'...'9': unit->bytes = size; break;
	case 'k':
	case 'K': unit->bytes = size * KB; break;
	case 'm':
	case 'M': unit->bytes = size * MB; break;
	case 'g':
	case 'G': unit->bytes = size * GB; break;
	default:
		  return -EINVAL;
	}

	num_to_str(unit->str, 128, unit->bytes);
	return 0;
}

int num_to_size(struct size_unit *unit, unsigned long long num)
{
	unit->bytes = num;
	num_to_str(unit->str, 128, unit->bytes);
	return 0;
}
