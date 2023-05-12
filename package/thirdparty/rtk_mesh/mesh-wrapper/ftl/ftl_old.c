/////////////////////////////////////////////////
//
// FTL , flash_translation_layer
//
/////////////////////////////////////////////////

//#define VERSION_BBpro 1
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <trace.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

static uint8_t flash_temp[5120];
static int fd_flash = -1;

#ifdef MESH_ANDROID
static const char *flname_flash = "/data/data/com.baidu.mesh.provisioner/flash.bin";
static const char *flname_flash_backup = "/data/data/com.baidu.mesh.provisioner/flash_backup.bin";
#else
static const char *flname_flash = "flash.bin";
static const char *flname_flash_backup = "flash_backup.bin";
#endif


// return 0 success
// return !0 fail
uint32_t ftl_save_old(void *p_data, uint16_t offset, uint16_t size)
{
	if (!p_data || (offset & 0x3) || (size == 0) || (size & 0x3))
		return 1;

	memcpy(&flash_temp[offset], p_data, size);

	if (fd_flash != -1) {
		int ret;
		ret = lseek(fd_flash, offset, SEEK_SET);
		if (ret == -1) {
			 APP_PRINT_ERROR1("%s: lseek error", __func__);
			return 1;
		}

		ret = write(fd_flash, p_data, size);
		if (ret != size) {
			 APP_PRINT_ERROR3("%s: write err, return %d, actual %d", __func__,
			       ret, size);
		}
		fsync(fd_flash);
	}

	return 0;
}

// return 0 success
// return !0 fail
uint32_t ftl_load_old(void *p_data, uint16_t offset, uint16_t size)
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

// return 0 success
// return !0 fail
uint32_t ftl_init_old(uint32_t u32PageStartAddr, uint8_t pagenum)
{
	int ret;
	int size = sizeof(flash_temp);

	if (!access(flname_flash, F_OK)) {
		fd_flash = open(flname_flash, O_RDWR);
		if (fd_flash == -1) {
			APP_PRINT_ERROR3("%s(): open %s error, %s", __func__, flname_flash,
			       strerror(errno));
			return 1;
		}
		ret = read(fd_flash, flash_temp, sizeof(flash_temp));
		if (ret == -1) {
			APP_PRINT_ERROR2("%s(): read error, %s", __func__, strerror(errno));
			goto err;
		}
		if(ret != sizeof(flash_temp))
		{
			APP_PRINT_ERROR4("%s(): %s read error, expect %d, ret %d", __func__, flname_flash, sizeof(flash_temp), ret);
			goto err;
		}

		return 0;
	}

	memset(flash_temp,0,size);
	return 0;
err:
	close(fd_flash);
	fd_flash = -1;
	return 1;
}

void ftl_close_old(void)
{
	if (fd_flash >= 0) {
		close(fd_flash);
		rename(flname_flash, flname_flash_backup);
		fd_flash = -1;
	}
	memset(flash_temp, 0, sizeof(flash_temp));
}














