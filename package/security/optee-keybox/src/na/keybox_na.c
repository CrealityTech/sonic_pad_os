
#include <tee_client_api.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "libkeybox/secure_storage.h"

void dump(uint8_t *buf, int ttl_len);
void dump(uint8_t *buf, int ttl_len)
{
	int len;
	for (len = 0; len < ttl_len; len++) {
		printf("0x%02x ", ((char *)buf)[len]);
		if (len % 8 == 7) {
			printf("\n");
		}
	}
	printf("\n");
}

void oneline_dump(uint8_t *buf, int ttl_len);
void oneline_dump(uint8_t *buf, int ttl_len)
{
	int len;
	for (len = 0; len < ttl_len; len++) {
		printf("%02x", ((char *)buf)[len]);
	}
}

static void usage(void)
{
        printf("usage: keybox_na [-rw] [-k key_name] <-f key_file>\n");
        printf(" [options]:\n");
        printf(" \t-r\t read key named 'dm_crypt_key'\n");
        printf(" \t-w\t write key named [key_name] with binary [key_file]\n");
        printf(" \t-k\t key name\n");
        printf(" \t-f\t key file, binary\n\n");
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
		oneline_dump((uint8_t *)key, sizeof(key));
	}

failOpen:
	TEEC_FinalizeContext(&ctx);
failInit:
//	printf("%s:finish with %d\n",__func__, teecErr);

	return teecErr;
}

#define SUNXI_SECURE_STORTAGE_INFO_HEAD_LEN (64 + 4 + 4 + 4)
typedef struct {
	char name[64]; //key name
	uint32_t len; //the len fo key_data
	uint32_t encrypted;
	uint32_t write_protect;
	char key_data[4096 -
		      SUNXI_SECURE_STORTAGE_INFO_HEAD_LEN]; //the raw data of key
} sunxi_secure_storage_info_t;
#define SUNXI_UTILS_CMD_SUNXI_SEC_OBJECT_DOWN 1

TEEC_Result secure_object_down(sunxi_secure_storage_info_t *secure_object);
TEEC_Result secure_object_down(sunxi_secure_storage_info_t *secure_object)
{
	TEEC_Context ctx;
	TEEC_Result teecErr;
	TEEC_Session teecSession;
	TEEC_Operation operation;
	int command;

	teecErr = TEEC_InitializeContext(NULL, &ctx);
	if (teecErr != TEEC_SUCCESS)
		goto failInit;

	teecErr = TEEC_OpenSession(&ctx, &teecSession, &ta_UUID,
				   TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
	if (teecErr != TEEC_SUCCESS)
		goto failOpen;

	TEEC_SharedMemory tee_params_p;
	tee_params_p.size  = 5000;
	tee_params_p.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	if (TEEC_AllocateSharedMemory(&ctx, &tee_params_p) != TEEC_SUCCESS)
		goto failOpen;

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);
	operation.started = 1;

	operation.params[0].memref.parent = &tee_params_p;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size   = 0;

	memcpy(tee_params_p.buffer, secure_object, sizeof(sunxi_secure_storage_info_t));

	command = SUNXI_UTILS_CMD_SUNXI_SEC_OBJECT_DOWN;

	//printf("before encrypt\n");
	//dump((uint8_t *)tee_params_p.buffer, 128);
	teecErr = TEEC_InvokeCommand(&teecSession, command, &operation, NULL);
	//printf("after encrypt\n");
	//dump((uint8_t *)tee_params_p.buffer, 128);
	memcpy(secure_object, tee_params_p.buffer, sizeof(sunxi_secure_storage_info_t));
	if ((sunxi_secure_storage_init() == 0) && (teecErr == TEEC_SUCCESS)) {
		sunxi_secure_object_write(secure_object->name, (char *)secure_object,
					  SUNXI_SECURE_STORTAGE_INFO_HEAD_LEN +
						  secure_object->len);
		sunxi_secure_storage_exit(0);
	}

	TEEC_ReleaseSharedMemory(&tee_params_p);
failOpen:
	TEEC_FinalizeContext(&ctx);
failInit:
	printf("%s:finish with %x\n", __func__, teecErr);

	if (teecErr == TEEC_SUCCESS)
		return 0;
	else
		return teecErr;
}

#define OP_NULL 0
#define OP_READ 1
#define OP_WRITE 2

int main(int argc, char **argv)
{
	(void)argc;(void)argv;
	int ret = -1;
	int ch = 0;
	int op_mode = OP_NULL;
	char *key_name = NULL;
	char *key_file = NULL;

	int fd = 0;
	int read_len = 0;
	struct stat st;
	sunxi_secure_storage_info_t secure_object;

	while ((ch = getopt(argc, argv, "rwk:f:")) != -1) {
		switch (ch) {
			case 'r':
				op_mode = OP_READ;
				break;
			case 'w':
				op_mode = OP_WRITE;
				break;
			case 'k':
				key_name = optarg;
				break;
			case 'f':
				key_file = optarg;
				break;
			case '?':
			default:
				printf("Unkonwn option: %c\n", (char)optopt);
				usage();
				return -1;
				break;
		}
	}

	if (op_mode == OP_READ) {
		if (key_name == NULL) {
			usage();
			return -1;
		}

		if (strcmp(key_name, "dm_crypt_key") == 0)
			ret = get_dm_crypt_key();
		else {
			usage();
			return -1;
		}
	} else if (op_mode == OP_WRITE) {
		memset(&secure_object, 0, sizeof(sunxi_secure_storage_info_t));

		if ((key_name == NULL) || (key_file == NULL)) {
			usage();
			return -1;
		}

		if (stat(key_file, &st)) {
			printf("error: can not get key_file stat\n");
			return -1;
		}

		if (st.st_size > 4096 - SUNXI_SECURE_STORTAGE_INFO_HEAD_LEN) {
			printf("error: key_file size is too large!\n");
			return -1;
		}

		fd = open(key_file, O_RDONLY);
		if (fd == -1) {
			printf("error: can not open key_file %s\n", key_file);
			return -1;
		}

		strcpy(secure_object.name, key_name);
		secure_object.encrypted     = 1;
		secure_object.write_protect = 0;

		secure_object.len	   = st.st_size;

		read_len = read(fd, secure_object.key_data, secure_object.len);
		if (read_len == -1) {
			ret = -1;
			goto out;
		}

		ret = secure_object_down(&secure_object);
		if (fd == -1) {
			printf("error: write keybox error!\n");
			goto out;
		}
	} else {
		usage();
		return -1;
	}

out:
	if (op_mode == OP_WRITE)
		close(fd);

	return ret;
}
