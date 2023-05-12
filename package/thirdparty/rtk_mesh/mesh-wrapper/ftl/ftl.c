/////////////////////////////////////////////////
//
// FTL , flash_translation_layer
//
/////////////////////////////////////////////////
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "ftl.h"
#include "ftl_config.h"
#include "trace.h"

static uint8_t flash_temp[MESH_FLASH_BIN_SIZE_MAX];
static int fd_flash_bin = -1;

static int fd_flash = -1;
#ifdef MESH_ANDROID
static const char* mesh_info_path = "/data/misc/bluedroid/flash.txt";
static const char *flname_flash = "/data/misc/bluedroid/rpl_flash.bin";
#else
static const char* mesh_info_path = "./flash.txt";
static const char *flname_flash = "./rpl_flash.bin";
#endif

static config_t * mesh_flash = NULL;

void ftl_flush(void)
{
	config_save(mesh_flash,mesh_info_path);
}

uint32_t ftl_save(const char *section, const char *key, const char *value)
{
  if(!mesh_flash)
  {
    return 1;
	}
	APP_PRINT_INFO3("ftl_save: [%s]  %s=%s",section, key, value);
	config_set_string(mesh_flash, section, key, value);

	return 0;
}

uint32_t ftl_save_bin(void *p_data, uint16_t offset, uint16_t size)
{
	if (!p_data || (offset & 0x3) || (size == 0) || (size & 0x3))
		return 1;

	memcpy(&flash_temp[offset], p_data, size);

	if (fd_flash_bin != -1) {
		int ret;
		ret = lseek(fd_flash_bin, offset, SEEK_SET);
		if (ret == -1) {
			 APP_PRINT_ERROR1("%s: lseek error", __func__);
			return 1;
		}

		ret = write(fd_flash_bin, p_data, size);
		if (ret != size) {
			 APP_PRINT_ERROR3("%s: write err, return %d, actual %d", __func__,
			       ret, size);
		}
		fsync(fd_flash_bin);
	}

	return 0;
}

const char * ftl_load(const char *section, const char *key, const char *def_value)
{
	if(!mesh_flash)
  {
    return NULL;
	}
	const char* tmp = NULL;
	tmp = config_get_string(mesh_flash, section, key, def_value);
	if(tmp)
	{
		APP_PRINT_INFO3("ftl_load: [%s] %s=%s",section, key, tmp);
	}
	else
	{
		APP_PRINT_ERROR2("ftl_load failed: [%s] %s=NULL",section, key);
	}
	return tmp;
}

uint32_t ftl_load_bin(void *p_data, uint16_t offset, uint16_t size)
{
	if (!p_data || (offset & 0x3) || (size == 0) || (size & 0x3))
	{
		if(!p_data) return 2;
		if(offset & 0x3) return 3;
		if(size == 0) return 4;
		if(size & 0x3) return 5;
	}
	memcpy(p_data, &flash_temp[offset], size);

	return 0;
}

uint32_t ftl_delete(const char *section, const char *key, uint8_t flag)
{
	bool ret = true;
  if(!mesh_flash)
  {
    return 1;
	}

	if(flag == 0)
	{
		ret = config_remove_key(mesh_flash, section, key);
		if(!ret)
		{
			APP_PRINT_ERROR2("ftl_delete failed: [%s]  %s",section, key);
			return 2;
		}

	}

	if(flag == 1)
	{
		ret = config_remove_section(mesh_flash, section);
		if(!ret)
		{
			APP_PRINT_ERROR1("ftl_delete failed: [%s] ",section);
			return 3;
		}

	}
	return 0;
}


uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum)
{
	int size = sizeof(flash_temp);
	int ret;
	if(!mesh_info_path)
		return 1;

	if (0 != access(mesh_info_path, F_OK))
	{
		APP_PRINT_WARN2("%s: %s is not exist", __func__, mesh_info_path );
		fd_flash = open(mesh_info_path, O_RDWR | O_CREAT, 0777);
		if (fd_flash == -1)
		{
			APP_PRINT_ERROR3("%s: open2 %s error, %s", __func__, mesh_info_path, strerror(errno));
			assert( fd_flash > -1);
			return 2;
		}
		close(fd_flash);
		fd_flash = -1;
	}

	mesh_flash = config_new(mesh_info_path);
	if (!mesh_flash)
	{
		APP_PRINT_ERROR2("%s file >%s< not found", __func__, mesh_info_path);
		mesh_flash = config_new_empty();
	}

	if (!access(flname_flash, F_OK)) {
		fd_flash_bin = open(flname_flash, O_RDWR);
		if (fd_flash_bin == -1) {
			 APP_PRINT_ERROR3("%s: open %s error, %s", __func__, flname_flash,
			       strerror(errno));
			assert( fd_flash_bin > -1);
			return 3;
		}
		ret = read(fd_flash_bin, flash_temp, sizeof(flash_temp));
		APP_PRINT_WARN4("%s(): read %s, size %d, read ret %d ", __func__, flname_flash, sizeof(flash_temp), ret);
		if (ret == -1) {
			 APP_PRINT_ERROR3("%s: read %s error, %s", __func__, flname_flash, strerror(errno));
			 close(fd_flash_bin);
			 assert( ret > -1);
			 return 4;
		}
	}
	else
	{
		APP_PRINT_WARN2("%s: %s is not exist", __func__, flname_flash );
		fd_flash_bin = open(flname_flash, O_RDWR | O_CREAT, 0644);
		if (fd_flash_bin == -1) {
			 APP_PRINT_ERROR3("%s: open2 %s error, %s", __func__, flname_flash,
				   strerror(errno));
			assert( fd_flash_bin > -1);
			return 5;
		}
		ret = write(fd_flash_bin, flash_temp, size);
		APP_PRINT_WARN4("%s(): write %s, size %d, write ret %d ", __func__, flname_flash, size, ret);
		if (ret != size) {
			APP_PRINT_ERROR3("%s: write %s err, size %d", __func__, flname_flash, ret);
			close(fd_flash_bin);
			assert( ret == size);
			return 6;
		}
		fsync(fd_flash_bin);
	}

	return 0;
}

void ftl_close(void)
{
	if (fd_flash >= 0)
	{
		close(fd_flash);
		fd_flash = -1;
	}

	if(mesh_flash!=NULL)
	{
		config_free(mesh_flash);
	}

	if (fd_flash_bin >= 0) {
		close(fd_flash_bin);
		fd_flash_bin = -1;
	}
	memset(flash_temp, 0, sizeof(flash_temp));

	return;
}

