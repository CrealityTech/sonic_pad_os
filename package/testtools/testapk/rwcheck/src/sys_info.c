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
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/vfs.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "rwcheck.h"

#define MEMINFO "/proc/meminfo"
#define CLEANMEM "/proc/sys/vm/drop_caches"

/*
 * vfat should not larger than 4G(4294967296)
 * but i don't know why that allway end in 4294967295.
 * so, for fat fs, we limit (4294967296 - 2)
 */
#define FAT_MAX_SIZE ((unsigned long long)4 * GB - 2)

static struct _fs fs_info[] = {
	{FS_MSDOS_MAGIC, "fat", FAT_MAX_SIZE},
	{FS_EXT2_MAGIC, "ext", FILE_SIZE_UNLIMITED},
	{FS_JFFS2_MAGIC, "jffs2", FILE_SIZE_UNLIMITED},
	{FS_NTFS_MAGIC, "ntfs", FILE_SIZE_UNLIMITED},
	{FS_TMPFS_MAGIC, "tmpfs", FILE_SIZE_UNLIMITED},
	{FS_UBIFS_MAGIC, "ubifs", FILE_SIZE_UNLIMITED},
	{FS_CIFS_MAGIC, "cifs", FILE_SIZE_UNLIMITED},
	{0, "Other", FILE_SIZE_UNLIMITED},
};

const struct _fs *get_fs(const char *dir)
{
	struct statfs sfs;
	int i;

	if (!is_dir(dir)) {
		ERROR("not existed or directory: %s\n", dir);
		return NULL;
	}

	if (statfs(dir, &sfs) < 0) {
		ERROR("statfs %s failed\n", dir);
		return NULL;
	}

	for (i = 0; i < ARRAY_SIZE(fs_info) - 1; i++) {
		struct _fs *info = &fs_info[i];

		DEBUG("try match fs %s(%llu) with %lu\n", info->str,
				info->magic, sfs.f_type);
		if ((unsigned long long)sfs.f_type == info->magic)
			return info;
	}
	return &fs_info[i];
}

unsigned long long get_total_mem(void)
{
	unsigned long long size_kb = 0;
	char buf[1024]= {0}, *p;
	FILE *fp;

	fp = fopen(MEMINFO, "r");
	if (fp == NULL) {
		ERROR("open %s failed\n", MEMINFO);
		return 0;
	}

	if (fread(buf, 1, 1024, fp) == 0 && ferror(fp)) {
		ERROR("fread %s failed\n", MEMINFO);
		goto err;
	}

	p = strstr(buf, "MemTotal");
	if (p == NULL) {
		ERROR("not found 'MemTotal' in %s\n", MEMINFO);
		goto err;
	}

	if (sscanf(p, "%*s%llu%*s", &size_kb) != 1) {
		ERROR("sscanf %s failed\n", MEMINFO);
		size_kb = 0;
	}

err:
	fclose(fp);
	return size_kb * KB;
}

int clean_mem(void)
{
	int ret = -1, fd, uid;

	/* only root has permissiom to control CLEANMEM */
#define ROOT_UID 0
	uid = getuid();
	if (uid != ROOT_UID)
		return 0;

	sync();
	fd = open(CLEANMEM, O_WRONLY);
	if (fd < 0) {
		ERROR("open %s failed\n", CLEANMEM);
		return fd;
	}

	ret = write(fd, "3", 1);
	if (ret != 1)
		ERROR("write '3' to %s failed\n", CLEANMEM);
	else
		ret = 0;

	close(fd);
	return ret;
}

unsigned long long get_free_mem(void )
{
	unsigned long long free_kb = 0;
	char buf[1024]= {0}, *p;
	FILE *fp;

	/* we must sync and clean cache before */
	sync();
	clean_mem();

	fp = fopen(MEMINFO, "r");
	if (fp == NULL) {
		ERROR("open %s failed\n", MEMINFO);
		return 0;
	}

	if (fread(buf, 1, 1024, fp) == 0 && ferror(fp)) {
		ERROR("fread %s failed\n", MEMINFO);
		goto err;
	}

	p = strstr(buf, "MemFree");
	if (p == NULL) {
		ERROR("not found 'MemFree' in %s\n", MEMINFO);
		goto err;
	}

	if (sscanf(p, "%*s%llu%*s", &free_kb) != 1) {
		ERROR("sscanf %s failed\n", MEMINFO);
		free_kb = 0;
	}

err:
	fclose(fp);
	return  free_kb * KB;
}

unsigned long long get_free_flash(struct sys_info *sys)
{
	struct statfs sfs;
	const char *dir = sys->dir;

	if (!sys->dir)
		return 0;

	if (!is_dir(dir)) {
		ERROR("not existed or directory: %s\n", dir);
		return 0;
	}

	if (statfs(dir, &sfs) < 0)
		return 0;

	/* get free by f_bavail rather than f_bfree */
	return sfs.f_bsize * sfs.f_bavail;
}

unsigned long long get_total_flash(const char *dir)
{
	struct statfs sfs;

	if (!is_dir(dir)) {
		ERROR("not existed or directory: %s\n", dir);
		return 0;
	}

	if (statfs(dir, &sfs) < 0) {
		ERROR("statfs %s failed\n", dir);
		return 0;
	}

	return sfs.f_bsize * sfs.f_blocks;
}

unsigned long long get_max_file_size(struct sys_info *sys)
{
	if (sys->dir && sys->fs)
		return sys->fs->max_file_size;
	return 0;
}

const char *get_fs_name(struct sys_info *sys)
{
	if (sys->dir && sys->fs)
		return sys->fs->str;
	return 0;
}

int update_sys_info(struct sys_info *sys, const char *dir)
{
	if (!sys->total_ddr)
		sys->total_ddr = get_total_mem();
	if (!sys->free_ddr)
		sys->free_ddr = get_free_mem();
	if (!sys->total_ddr || !sys->free_ddr)
		return -1;
	sys->clean_mem = clean_mem;

	/* If no dir, no need to get flash information */
	if (!dir)
		return 0;
	if (sys->dir && !strcmp(sys->dir, dir))
		return 0;

	sys->total_flash = get_total_flash(dir);
	if (!sys->total_flash)
		return -1;

	sys->fs = get_fs(dir);
	if (!sys->fs)
		return -1;

	if (sys->dir)
		free((void *)sys->dir);
	sys->dir = strdup(dir);
	if (!sys->dir)
		return -1;

	sys->free_flash = get_free_flash;
	sys->max_file_size = get_max_file_size;
	sys->fs_name = get_fs_name;
	return 0;
}
