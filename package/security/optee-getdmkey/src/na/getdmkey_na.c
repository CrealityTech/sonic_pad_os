
#include <tee_client_api.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void dump(uint8_t *buf, int ttl_len);
void dump(uint8_t *buf, int ttl_len)
{
	int len;
	for (len = 0; len < ttl_len; len++) {
		printf("%02x", ((char *)buf)[len]);
	}
}

#define SUNXI_UTILS_UUID \
{ 0x475f147a, 0x69c5, 0x11ea, \
	{ 0x10, 0xb8, 0x00, 0x50, 0x56, 0x97, 0x78, 0x83 } }

#define SUNXI_UTILS_CMD_GET_DM_CRYPT_KEY 0
static const TEEC_UUID ta_UUID = SUNXI_UTILS_UUID;
TEEC_Result get_dm_crypt_key(void);
TEEC_Result get_dm_crypt_key(void){
	TEEC_Context ctx;
	TEEC_Result teecErr;
	TEEC_Session teecSession;
	TEEC_Operation operation;
	int command;
	uint8_t key[32];

//	printf("NA:init context\n");

	teecErr = TEEC_InitializeContext(NULL, &ctx);
	if (teecErr != TEEC_SUCCESS)
		goto failInit;

//	printf("NA:open session\n");
	teecErr = TEEC_OpenSession(&ctx, &teecSession, &ta_UUID,
				   TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
	if (teecErr != TEEC_SUCCESS)
		goto failOpen;

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes =
		TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT,
				 TEEC_NONE, TEEC_NONE);
	operation.started = 1;

	operation.params[0].tmpref.buffer = key;
	operation.params[0].tmpref.size   = sizeof(key);
	operation.params[1].value.a   = sizeof(key);

	command = SUNXI_UTILS_CMD_GET_DM_CRYPT_KEY;

//	printf("NA:invoke command\n");
	teecErr = TEEC_InvokeCommand(&teecSession, command, &operation, NULL);
	if (teecErr == TEEC_SUCCESS) {
		dump((uint8_t *)key, sizeof(key));
	}

failOpen:
	TEEC_FinalizeContext(&ctx);
failInit:
//	printf("%s:finish with %d\n",__func__, teecErr);

	return teecErr;
}


int main(int argc, char **argv)
{
	(void)argc;(void)argv;
	TEEC_Result teecErr;

	teecErr = get_dm_crypt_key();

	return teecErr;
}

