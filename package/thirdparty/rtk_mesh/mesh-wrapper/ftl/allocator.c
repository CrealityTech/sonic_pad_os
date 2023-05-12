/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "allocator.h"

char* osi_strdup(const char* str) {
  size_t size = strlen(str) + 1;  // + 1 for the null terminator
  void* ptr = malloc(size);
  assert(ptr);
  if(!ptr)
    return NULL;

  memcpy((char*)ptr, str, size);
  return ptr;
}

char* osi_strndup(const char* str, size_t len) {
  size_t size = strlen(str);
  if (len < size) size = len;

  void* ptr = malloc(size + 1);
  assert(ptr);
  if(!ptr)
    return NULL;

  memcpy((char* )ptr, str, size);
  ((char *)ptr)[size] = '\0';
  return ptr;
}

void* osi_malloc(size_t size) {
  void* ptr = malloc(size);
  assert(ptr);
  if(!ptr)
    return NULL;

  return ptr;
}

void* osi_calloc(size_t size) {
  void* ptr = calloc(1, size);
  assert(ptr);
  if(!ptr)
    return NULL;

  return ptr;
}

void osi_free(void* ptr) {
  if(ptr)
    free(ptr);
}

void osi_free_and_reset(void** p_ptr) {
  assert(p_ptr != NULL);
  if(p_ptr == NULL)
    return;
  osi_free(*p_ptr);
  *p_ptr = NULL;
}

const allocator_t allocator_calloc = {osi_calloc, osi_free};

const allocator_t allocator_malloc = {osi_malloc, osi_free};
