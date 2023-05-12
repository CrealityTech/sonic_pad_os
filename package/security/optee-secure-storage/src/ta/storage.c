/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "storage.h"

#include <tee_api.h>
#include <trace.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_PARAM_TYPE(pt) \
do { \
	if ((pt) != param_types) \
		return TEE_ERROR_BAD_PARAMETERS; \
} while (0)

#define VAL2HANDLE(v) (void *)(uintptr_t)(v)

TEE_Result ta_storage_cmd_create(uint32_t __unused param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectHandle o = TEE_HANDLE_NULL;
	void *object_id = NULL;
	TEE_ObjectHandle ref_handle = TEE_HANDLE_NULL;

	object_id = TEE_Malloc(params[0].memref.size, 0);
	if (!object_id)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_MemMove(object_id, params[0].memref.buffer,
		    params[0].memref.size);

	ref_handle = (TEE_ObjectHandle)(uintptr_t)params[2].value.a;

	res = TEE_CreatePersistentObject(params[2].value.b,
					 object_id, params[0].memref.size,
					 params[1].value.a, ref_handle,
					 params[3].memref.buffer,
					 params[3].memref.size, &o);

	TEE_Free(object_id);

	params[1].value.b = (uintptr_t)o;

	return res;
}

TEE_Result ta_storage_cmd_open(uint32_t __unused param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectHandle o = TEE_HANDLE_NULL;
	void *object_id = NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_VALUE_INOUT,
			   TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_NONE));

	object_id = TEE_Malloc(params[0].memref.size, 0);
	if (!object_id)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_MemMove(object_id, params[0].memref.buffer,
		    params[0].memref.size);

	res = TEE_OpenPersistentObject(params[2].value.a,
					object_id, params[0].memref.size,
					params[1].value.a, &o);

	params[1].value.b = (uintptr_t)o;

	TEE_Free(object_id);

	return res;
}

TEE_Result ta_storage_cmd_read(uint32_t __unused param_types, TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[1].value.a);
	int res;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_MEMREF_OUTPUT,
			   TEE_PARAM_TYPE_VALUE_INOUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = TEE_SeekObjectData(o, 0, TEE_DATA_SEEK_SET);
	if (res) {
		printf("ta----seek-failed--%s%x\n", __func__, __LINE__);
	}

	return TEE_ReadObjectData(o, params[0].memref.buffer,
				  params[0].memref.size, &params[1].value.b);
}

TEE_Result ta_storage_cmd_write(uint32_t __unused param_types, TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[1].value.a);
	int res;

	res = TEE_SeekObjectData(o, 0, TEE_DATA_SEEK_SET);
	if (res) {
		printf("ta---seek--failed--%s%x\n", __func__, __LINE__);
	}

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	return TEE_WriteObjectData(o, params[0].memref.buffer,
				   params[0].memref.size);
}

TEE_Result ta_storage_cmd_unlink(uint32_t param_types, TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_CloseAndDeletePersistentObject1(o);

	return TEE_SUCCESS;
}
