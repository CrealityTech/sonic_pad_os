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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "rwcheck.h"

#define RANDOM_DATA "/dev/urandom"
#define get_mode(task) ((task)->mode->mode)
#define set_mode(task, _mode) ((task)->mode = &run_mode[_mode])
#define get_strmode(task) ((task)->mode->str_mode)

/*
 * AUTO MODE:
 *  size of each file to write is half of the free space, end testing only
 *  when the less space is less than 64K
 */
#define AUTO_MODE 0
/*
 * SAME MODE:
 *   size of each file to write is the same, end testing only when there is no
 *   enough space
 */
#define SAME_MODE 1
/*
 * UP_MODE:
 *   size of each file will increase by a multiple of 2, loop from beginning
 *   size to end size until there is no enough space
 */
#define UP_MODE 2
struct run_mode {
	int mode;
	const char *str_mode;
};

static struct run_mode run_mode[] = {
	{AUTO_MODE, "Auto Mode"},
	{SAME_MODE, "Same Mode"},
	{UP_MODE, "Up Mode"},
};

#define MAX_PATH_LEN 1024
struct rwtask {
	struct sys_info sys;
	struct run_mode *mode;

	unsigned long long min_free_bytes;
	const char *dir;
	char org[MAX_PATH_LEN];
	bool set_org;
	int jobs;
	int percent;
	unsigned int times;
	unsigned int count;

	struct size_unit sz_begin;
	struct size_unit sz_end;
	union {
		struct size_unit sz_cur;
		struct size_unit sz_same;
	};
	struct size_unit sz_buf;
};

void help(void)
{
	INFO("  Usage: rwcheck [-h] [-d dir] [-t times] [-b size] [-e size]\n");
	INFO("		   [-s size] [-u size] [-p percent] [-i input] [-j jobs]\n");
	INFO("\n");
	INFO("\t-h : show this massage and exit\n");
	INFO("\t-d # : the diretory to check [default currect path]\n");
	INFO("\t-t # : check times\n");
	INFO("\t-b # : [up mode] set begin size\n");
	INFO("\t-e # : [up mode] set end size\n");
	INFO("\t-s # : [same mode] set file size\n");
	INFO("\t-u # : set read/write buf size\n");
	INFO("\t-p # : set maximum ratio of total flash size to check. Eg. -p 95\n");
	INFO("\t-i # : input path of file [default <check_dir>/rwcheck.org]\n");
	INFO("\t	   if file don't existed, create 64K from /dev/urandom\n");
	INFO("\t-j # : multiple jobs\n");
	INFO("\n");
	INFO("  size trailing with k|m|g or not\n");
	INFO("\n");
	INFO("  rwcheck work in 3 mode:\n");
	INFO("  1. -s # : files have the same size, keep testing until no enough space\n");
	INFO("  2. -b # :\n");
	INFO("  2. -e # : file size increase by The multiplier of 2, loop\n");
	INFO("			from beginning to ending size until no enough space\n");
	INFO("  3. none : file size is 50%% of the free space, keep testing until\n");
	INFO("			the less space is less than 64K\n");
}

int parse_args(struct rwtask *task, int argc, char **argv)
{
	int ret;
	int opts = 0;

	while ((opts = getopt(argc, argv, ":d:t:b:e:s:u:p:hi:j:")) != EOF) {
		switch (opts) {
		case 'd':
			if (task->dir)
				free((void *)task->dir);
			if (!is_dir(optarg)) {
				ERROR("not existed or directory: %s\n", optarg);
				return -1;
			}
			task->dir = abs_path(optarg);
			if (!task->dir) {
				ERROR("can get absolute path: %s\n", optarg);
				return -1;
			}
			break;
		case 't':
			task->times = (unsigned int)atoi(optarg);
			if (task->times == 0) {
				ERROR("invalid times: %s\n", optarg);
				return -1;
			}
			break;
		case 'b':
		case 'e':
			if (get_mode(task) == SAME_MODE) {
				ERROR("incompatible with -s and -b|-e\n");
				return -1;
			}
			if (opts == 'b')
				ret = str_to_size(&(task->sz_begin), optarg);
			else
				ret = str_to_size(&(task->sz_end), optarg);
			if (ret < 0) {
				ERROR("invalid size: %s\n", optarg);
				return -1;
			}
			set_mode(task, UP_MODE);
			break;
		case 's':
			if (get_mode(task) == UP_MODE) {
				ERROR("incompatible with -s and -b|-e\n");
				return -1;
			}
			ret = str_to_size(&(task->sz_same), optarg);
			if (ret < 0) {
				ERROR("invalid size: %s\n", optarg);
				return -1;
			}
			set_mode(task, SAME_MODE);
			break;
		case 'u':
			ret = str_to_size(&(task->sz_buf), optarg);
			if (ret < 0) {
				ERROR("invalid size: %s\n", optarg);
				return -1;
			}
			break;
		case 'i':
			if (!is_file(optarg)) {
				ERROR("not existed or file: %s\n", optarg);
				return -1;
			}

			strncpy(task->org, optarg, MAX_PATH_LEN);
			task->set_org = true;
			break;
		case 'p':
			task->percent = atoi(optarg);
			if (task->percent < 0 || task->percent > 99) {
				ERROR("invalid percent [1, 99]: %s\n", optarg);
				return -1;
			}
			break;
		case 'j':
			task->jobs = (int)atoi(optarg);
			if (task->jobs <= 0) {
				ERROR("invalid jobs: %s\n", optarg);
				return -1;
			} else if (task->jobs > MAX_JOBS) {
				ERROR("over limit [1, %d]: %s\n", MAX_JOBS, optarg);
				return -1;
			}
			break;
		case 'h':
			help();
			exit(0);
		case '?':
			ERROR("invalid option -%c\n", optopt);
			help();
			return -1;
		case ':':
			ERROR("option -%c requires an argument\n", optopt);
			help();
			return -1;
		}
	}
	return 0;
}

struct rwtask *init_rwtask(void)
{
	int ret = -EINVAL;
	struct rwtask *task;

	task = malloc(sizeof(struct rwtask));
	if (!task) {
		ERROR("malloc for task failed\n");
		return NULL;
	}
	memset(task, 0, sizeof(struct rwtask));

	/* run mode */
	set_mode(task, AUTO_MODE);
	/* run times */
	task->times = DEFAULT_TIMES;
	/* percent of all flash */
	task->percent = DEFAULT_PERCENT;
	/* update sys info */
	task->dir = abs_path(".");
	if (!task->dir)
		goto err;
	ret = update_sys_info(&task->sys, task->dir);
	if (ret)
		goto err;
	/* parallel writing worker */
	task->jobs = DEFAULT_JOBS;
	/* set orgin flag */
	task->set_org = false;
	/* buf size */
	if (task->sys.total_ddr > DEFAULT_BUF_SIZE_DDR_LIMIT)
		num_to_size(&(task->sz_buf), DEFAULT_LARGE_BUF_SIZE);
	else
		num_to_size(&(task->sz_buf), DEFAULT_SMALL_BUF_SIZE);

	return task;
err:
	free(task);
	return NULL;
}

int check_args(struct rwtask *task)
{
	int ret;
	struct sys_info *sys = &task->sys;

	/* update flash information if new dir */
	ret = update_sys_info(&task->sys, task->dir);
	if (ret)
		return -1;

	/* min_free_bytes */
	task->min_free_bytes = 100 - task->percent;
	task->min_free_bytes *= task->sys.total_flash;
	task->min_free_bytes /= 100;

	/* set orgin file */
	snprintf(task->org, MAX_PATH_LEN, "%s/%s", task->dir, DEFAULT_ORGIN_FILE);

	/* buf_size */
	if (task->sz_buf.bytes >= 16 * MB) {
		ERROR("too large buf, should not greater than 16MB free memory\n");
		return -1;
	}
	/* In case of little ram has no enough buffer */
	if (task->sz_buf.bytes >= sys->free_ddr * 60 / 100) {
		ERROR("too large buf, should not greater than 60%% free memory\n");
		return -1;
	}

	/* begin_size & end_size */
	if (get_mode(task) == UP_MODE) {
		if (!task->sz_begin.bytes)
			num_to_size(&(task->sz_begin), DEFAULT_UP_MODE_BEGIN_SIZE);
		if (!task->sz_end.bytes)
			num_to_size(&(task->sz_end), DEFAULT_UP_MODE_END_SIZE);
		if (task->sz_end.bytes > sys->max_file_size(sys))
			num_to_size(&(task->sz_end), sys->max_file_size(sys));
		if (task->sz_begin.bytes > task->sz_end.bytes) {
			ERROR("[UP Mode]: begin size %s larger than end size %s\n",
					task->sz_begin.str, task->sz_end.str);
			return -1;
		}
	} else if (get_mode(task) == SAME_MODE) {
		if (task->sz_same.bytes > sys->max_file_size(sys))
			num_to_size(&(task->sz_same), sys->max_file_size(sys));
	}

	/* orgin file */
	if (task->set_org && !is_file(task->org)) {
		ERROR("not found %s for orgin file\n", task->org);
		return -1;
	}

	return 0;
}

void print_head(struct rwtask *task)
{
	time_t t = time(NULL);
	struct sys_info *sys = &task->sys;

	INFO("\n\trwcheck: do read and write check\n\n");
	INFO("\tversion: %s\n", VERSION);
	INFO("\tbuild: %s\n", COMPILE);
	INFO("\tdate: %s\n", ctime(&t));
	INFO("\tfree/total ddr: %llu/%llu MB\n",
			sys->free_ddr / MB, sys->total_ddr / MB);
	INFO("\tfree/total flash: %llu/%llu MB\n",
			sys->free_flash(sys) / MB, sys->total_flash / MB);
	INFO("\tflash filesystem %s\n", sys->fs_name(sys));
	INFO("\tset mode to %s\n", get_strmode(task));
	if (get_mode(task) == SAME_MODE) {
		INFO("\tset file size to %s\n", task->sz_same.str);
	} else if (get_mode(task) == UP_MODE) {
		INFO("\tset file begin size to %s\n", task->sz_begin.str);
		INFO("\tset file end size to %s\n", task->sz_end.str);
	}
	INFO("\tset times to %u\n", task->times);
	INFO("\tset max percent of total space to %d%%\n", task->percent);
	INFO("\tset buf size to %s\n", task->sz_buf.str);
	INFO("\tset check diretory as %s\n", task->dir);
	INFO("\tset orgin file as %s\n", task->org);
	INFO("\tset jobs as %d\n", task->jobs);
}

int init_orgin_file(struct rwtask *task)
{
	if (!task->org[0])
		return -ENOENT;

	if (task->set_org)
		return 0;

	if (is_existed(task->org))
		return 0;

	/* create orgin file */
	return copy_file(RANDOM_DATA, task->org, DEFAULT_ORG_SIZE, 4 * KB);
}

int deinit_orgin_file(struct rwtask *task)
{
	/*
	 * we remove old orgin to ensure data difference each time
	 * do not remove before create, to avoid destorying problem scene
	 */
	if (!task->set_org)
		remove(task->org);
	return 0;
}

struct pthread_arg {
	struct rwtask *task;
	char path[128];
	unsigned long long bytes;
	int ret;
};

void *sub_thread_do_create(void *_arg)
{
	struct pthread_arg *arg = (struct pthread_arg *)_arg;
	struct rwtask *task = arg->task;
	unsigned long long bytes = arg->bytes;
	const char *path = arg->path;
	int ret = -1;

	if (is_existed(path)) {
		arg->ret = 0;
		pthread_exit(arg);
	}

	INFO("\r\tcreate\t: %s ... ", path);

	ret = copy_file(task->org, path, bytes, task->sz_buf.bytes);
	if (ret)
		goto err;

	/* the last 2 btyes are reserved for saving crc */
	ret = append_crc(path);
	if (ret)
		goto err;

	INFO("\r\tcreate\t: %s ... OK (%lluK)\n", path, bytes / KB);
err:
	arg->ret = ret;
	pthread_exit(arg);
}

unsigned long long get_task_size_auto_mode(struct rwtask *task)
{
	struct sys_info *sys = &task->sys;
	unsigned long long free_bytes = sys->free_flash(sys);
	unsigned long long test_bytes = 0;

	test_bytes = round_down(free_bytes / 2, KB);
	while (test_bytes && free_bytes - test_bytes < task->min_free_bytes)
		test_bytes = round_down(test_bytes / 2, KB);
	test_bytes = min(test_bytes, DEFAULT_AUTO_MODE_MAX_SIZE);

	if (test_bytes < DEFAULT_AUTO_MODE_END_SIZE)
		return 0;
	return test_bytes;
}

unsigned long long get_task_size_same_mode(struct rwtask *task)
{
	struct sys_info *sys = &task->sys;
	unsigned long long free_bytes = sys->free_flash(sys);

	if (free_bytes <= task->sz_same.bytes)
		return 0;
	return task->sz_same.bytes;
}

unsigned long long get_task_size_up_mode(struct rwtask *task)
{
	struct sys_info *sys = &task->sys;
	unsigned long long free_bytes = sys->free_flash(sys);

	if (task->sz_cur.bytes == 0)
		task->sz_cur.bytes = task->sz_begin.bytes;
	else
		task->sz_cur.bytes *= DEFAULT_UP_MODE_POWER;

	/* larger than end size, reset */
	if (task->sz_cur.bytes > task->sz_end.bytes)
		task->sz_cur.bytes = task->sz_begin.bytes;
	/* larger than free size, reset */
	if (task->sz_cur.bytes >= free_bytes)
		task->sz_cur.bytes = task->sz_begin.bytes;
	/* larger than limit size, reset */
	if (task->min_free_bytes > free_bytes - task->sz_cur.bytes)
		task->sz_cur.bytes = task->sz_begin.bytes;
	return task->sz_cur.bytes;
}

unsigned long long get_test_size(struct rwtask *task)
{
	struct sys_info *sys = &task->sys;
	unsigned long long free_bytes = sys->free_flash(sys);
	unsigned long long test_bytes = 0;

	if (free_bytes == 0 || free_bytes <= task->min_free_bytes)
		return 0;

	switch (get_mode(task)) {
	case AUTO_MODE:
		test_bytes = get_task_size_auto_mode(task);
		break;
	case SAME_MODE:
		test_bytes = get_task_size_same_mode(task);
		break;
	case UP_MODE:
		test_bytes = get_task_size_up_mode(task);
		break;
	}

	if (!test_bytes)
		return 0;

	if (task->min_free_bytes > free_bytes - test_bytes)
		return 0;

	if (test_bytes > sys->max_file_size(sys))
		test_bytes = sys->max_file_size(sys);

	return test_bytes;
}

#define testfile(task, path, len, num, job)				\
do {									\
	if (task->jobs == 1)						\
		snprintf(path, len, "%s/" DEFAULT_OUTPUT ".%d", 	\
				task->dir, num);			\
	else								\
		snprintf(path, len, "%s/" DEFAULT_OUTPUT ".%d_%d",	\
				task->dir, num, job);			\
} while(0)

int do_create(struct rwtask *task)
{
	int num = 0, ret = 0, job;
	unsigned long long size;
	struct pthread_arg pargs[MAX_JOBS] = {{0}};
	pthread_t pid[MAX_JOBS] = {0};

	INFO("\t--- CREATE ---\n");

	if (get_mode(task) == UP_MODE)
		num_to_size(&(task->sz_cur), 0);

	while ((size = get_test_size(task))) {
		/* create thread */
		for (job = 0; job < task->jobs; job++) {
			struct pthread_arg *p = &pargs[job];

			p->task = task;
			p->bytes = size / task->jobs;
			testfile(task, p->path, 128, num, job);
			if (is_existed(p->path)) {
				ret = 0;
				goto out;
			}

			ret = pthread_create(pid + job, NULL,
					sub_thread_do_create, (void *)p);
			if (ret) {
				ERROR("pthread create failed\n");
				goto out;
			}
		}
		/* waiting end */
		for (job = 0; job < task->jobs; job++) {
			struct pthread_arg *p;

			if (!pid[job])
				continue;

			pthread_join(pid[job], (void **)&p);
			if ((ret = p->ret)) {
				ERROR("wait pthread end failed\n");
				goto out;
			}
		}
		num++;
	}

	return 0;
out:
	for (job = 0; job < task->jobs; job++) {
		if (!pid[job])
			continue;
		pthread_cancel(pid[job]);
	}
	return ret;
}

int do_check(struct rwtask *task)
{
	char file[128];
	int num = 0, job, ret = -1;
	struct sys_info *sys = &task->sys;
	unsigned short crc;

	/* do clean memory before check to ensure read data from flash */
	sys->clean_mem();

	INFO("\t--- CHECK ---\n");

	while (1) {
		for (job = 0; job < task->jobs; job++) {
			testfile(task, file, 128, num, job);
			if (!is_file(file)) {
				task->count = num;
				return 0;
			}

			INFO("\tcheck\t: %s ... ", file);

			ret = get_crc16_by_path(&crc, file, CRC16_MAX_BYTES);
			if (ret)
				return ret;
			if (check_crc16(crc) == true) {
				INFO("OK\n");
				continue;
			}
			testfile(task, file, 128, num + 1, job);
			if (is_file(file)) {
				INFO("FAILED (crc error)\n");
				errno = EIO;
				testfile(task, file, 128, num, job);
				ERROR("%s crc error\n", file);
				return -1;
			}
			INFO("OK: ignore the last one\n");
		}
		num++;
	}
}

int do_remove(struct rwtask *task)
{
	struct sys_info *sys = &task->sys;
	int num, jobs;
	char file[100];

	INFO("\t--- REMOVE ---\n");
	for (num = task->count - 1; num >= 0; num--) {
		for (jobs = 0; jobs < task->jobs; jobs++) {
			testfile(task, file, 100, num, jobs);
			if (!is_existed(file))
				goto out;

			INFO("\tremove\t: %s ... ", file);
			if (remove(file) != 0) {
				ERROR("remove %s failed", file);
				return -EINVAL;
			}
			INFO("OK\n");
		}
	}

out:
	if (task->min_free_bytes >= sys->free_flash(sys)) {
		errno = ENOSPC;
		ERROR("no enough space as already used over %u%%\n",
				task->percent);
		return -1;
	}
	sync();
	return 0;
}

int begin(struct rwtask *task)
{
	int ret = 0;

	print_head(task);
	while (task->times-- > 0) {
		INFO("\n");

		ret = init_orgin_file(task);
		if (ret)
			return ret;

		ret = do_create(task);
		if (ret)
			return ret;

		ret = do_check(task);
		if (ret)
			return ret;

		ret = do_remove(task);
		if (ret)
			return ret;

		ret = deinit_orgin_file(task);
		if (ret)
			return ret;
	}

	return 0;
}

int main(int argc, char **argv)
{
	int ret = -1;
	struct rwtask *task;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	task = init_rwtask();
	if (!task)
		goto err;

	ret = parse_args(task, argc, argv);
	if (ret)
		goto err;

	ret = check_args(task);
	if (ret)
		goto err;

	ret = begin(task);
	if (ret)
		goto err;

	return 0;
err:
	if (task)
		free(task);
	show_errlog();
	return ret;
}
