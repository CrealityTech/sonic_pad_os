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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "rwcheck.h"

bool is_existed(const char *path)
{
	return !access(path, F_OK | R_OK);
}

bool is_dir(const char *path)
{
	int ret;
	struct stat s;

	ret = stat(path, &s);
	if (ret)
		return false;
	return S_ISDIR(s.st_mode);
}

bool is_file(const char *path)
{
	int ret;
	struct stat s;

	ret = stat(path, &s);
	if (ret)
		return false;
	return S_ISREG(s.st_mode);
}

int copy_file(const char *in, const char *out, unsigned long long size,
		unsigned long long buf_size)
{
	char *buf;
	int fd_in, fd_out;
	int ret = -1;

	buf = malloc(buf_size);
	if (!buf) {
		ERROR("malloc failed\n");
		return -1;
	}

	fd_in = open(in, O_RDONLY);
	if (fd_in < 0) {
		ERROR("open %s failed\n", in);
		goto free_buf;
	}
	fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_out < 0) {
		ERROR("open %s failed\n", out);
		goto close_in;
	}
	/* pre-allocate flash space, in case of no enough space while writing */
	if (ftruncate(fd_out, size)) {
		ERROR("ftruncate %s to %lu failed\n", out, size);
		goto close_out;
	}

	while (size > 0) {
		unsigned long long sz = min(size, buf_size);
		unsigned long long done = 0;

		while (sz > done) {
			int rret;

			rret = read(fd_in, buf + done, sz - done);
			if (rret < 0) {
				ERROR("read %s failed\n", in);
				goto close_out;
			}
			/* lseek to start of file to ensure read full size */
			if (rret != sz - done)
				lseek(fd_in, 0, SEEK_SET);
			done += rret;
		}
		done = write(fd_out, buf, sz);
		if (done != sz) {
			ERROR("write %s failed\n", out);
			goto close_out;
		}
		size -= sz;
	}

	fsync(fd_out);
	ret = 0;
close_out:
	close(fd_out);
close_in:
	close(fd_in);
free_buf:
	free(buf);
	return ret;
}

int append_crc(const char *path)
{
	int fd, ret = -1;
	unsigned long long fsize;
	unsigned short crc = 0;

	fd = open(path, O_RDWR);
	if (fd < 0) {
		ERROR("open %s failed\n", path);
		goto err;
	}

	fsize = file_size(fd);
	if ((ssize_t)fsize < 0)
		goto err;

	ret = get_crc16_by_fd(&crc, fd, fsize - 2);
	if (ret) {
		ERROR("get crc16 failed\n");
		goto err;
	}

	ret = lseek(fd, -2, SEEK_END);
	if (ret < 0) {
		ERROR("lseek %s failed\n", path);
		goto err;
	}

	ret = write(fd, &crc, 2);
	if (ret != 2) {
		ERROR("write %s failed\n", path);
		goto err;
	}

	fsync(fd);
	ret = 0;
err:
	close(fd);
	return ret;
}

char *abs_path(const char *path)
{
	char *ret = NULL, *pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return NULL;

	if (chdir(path) < 0) {
		ERROR("chdir failed\n");
		goto out;
	}
	ret = getcwd(NULL, 0);

	if (chdir(pwd) < 0) {
		free(ret);
		ret = NULL;
	}

out:
	free(pwd);
	return ret;
}

ssize_t file_size(int fd)
{
	struct stat s;

	if (fstat(fd, &s) < 0) {
		ERROR("fstat failed\n");
		return -1;
	}
	return s.st_size;
}
