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

#ifndef __SYS_H__
#define __SYS_H__

/* the follow Filesystem types are from 'man 2 statfs' */
#define FS_ADFS_MAGIC		0xadf5
#define FS_AFFS_MAGIC		0xADFF
#define FS_BEFS_MAGIC		0x42465331
#define FS_BFS_MAGIC		0x1BADFACE
#define FS_CIFS_MAGIC		0xFF534D42
#define FS_CODA_MAGIC		0x73757245
#define FS_COH_MAGIC		0x012FF7B7
#define FS_CRAMFS_MAGIC		0x28cd3d45
#define FS_DEVFS_MAGIC		0x1373
#define FS_EFS_MAGIC		0x00414A53
#define FS_EXT_MAGIC		0x137D
#define FS_EXT2_OLD_MAGIC	0xEF51
#define FS_EXT2_MAGIC		0xEF53
#define FS_EXT3_MAGIC		0xEF53
#define FS_EXT4_MAGIC		0xEF53
#define FS_HFS_MAGIC		0x4244
#define FS_HPFS_MAGIC		0xF995E849
#define FS_HUGETLBFS_MAGIC	0x958458f6
#define FS_ISOFS_MAGIC		0x9660
#define FS_JFFS2_MAGIC		0x72b6
#define FS_JFS_MAGIC		0x3153464a
#define FS_MINIX_MAGIC		0x137F
#define FS_MINIX_MAGIC2		0x138F
#define FS_MINIX2_MAGIC		0x2468
#define FS_MINIX2_MAGIC2	0x2478
#define FS_MSDOS_MAGIC		0x4d44
#define FS_NCP_MAGIC		0x564c
#define FS_NFS_MAGIC		0x6969
#define FS_NTFS_MAGIC		0x5346544e
#define FS_OPENPROM_MAGIC  	0x9fa1
#define FS_PROC_MAGIC		0x9fa0
#define FS_QNX4_MAGIC		0x002f
#define FS_REISERFS_MAGIC  	0x52654973
#define FS_ROMFS_MAGIC		0x7275
#define FS_SMB_MAGIC		0x517B
#define FS_SYSV2_MAGIC		0x012FF7B6
#define FS_SYSV4_MAGIC		0x012FF7B5
#define FS_TMPFS_MAGIC		0x01021994
#define FS_UDF_MAGIC		0x15013346
#define FS_UFS_MAGIC		0x00011954
#define FS_USBDEVICE_MAGIC 	0x9fa2
#define FS_VXFS_MAGIC		0xa501FCF5
#define FS_XENIX_MAGIC		0x012FF7B4
#define FS_XFS_MAGIC		0x58465342
#define FS_XIAFS_MAGIC		0x012FD16D
/* the follow Filesystem types are from FS codes */
#define FS_UBIFS_MAGIC		0x24051905

#define FILE_SIZE_UNLIMITED ((unsigned long long)(~0ULL>>1))

struct _fs {
	unsigned long long magic;
	const char str[20];
	long long max_file_size;
};

struct sys_info {
	/* all the follow size in Bytes */
	unsigned long long total_ddr;
	unsigned long long free_ddr;
	unsigned long long total_flash;
	/*
	 * free_flash should be a function rather than variable
	 * because rwcheck always needs the real-time free space.
	 */
	unsigned long long (*free_flash)(struct sys_info *);
	unsigned long long (*max_file_size)(struct sys_info *);
	const char *(*fs_name)(struct sys_info *);
	int (*clean_mem)(void);

	/* private */
	const struct _fs *fs;
	const char *dir;
};

int update_sys_info(struct sys_info *sys, const char *dir);

#endif
