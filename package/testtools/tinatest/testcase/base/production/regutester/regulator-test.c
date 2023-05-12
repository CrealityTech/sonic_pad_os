#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define MAX_VOL_NAME	"max_microvolts"
#define MIN_VOL_NAME	"min_microvolts"
#define MAX_REGU_NUM	32
#define REGU_SUMMARY	"/sys/kernel/debug/regulator/regulator_summary"

#ifdef DEBUG
#define pr_dbg(fmt, ...)   printf("[%s %d]"fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define pr_dbg(fmt, ...)
#endif

#define pr_info(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf("[%s %d]"fmt, __func__, __LINE__, ##__VA_ARGS__)


/*
 * get hardware spec value
 * @hardware:soc hardware name
 * @revision:
 * @chipid:soc diff-serial
 */
void get_hardware_name(char *hardware)
{
	const char *cpuinfo = "/proc/cpuinfo";
	char *data = NULL;
	size_t len = 0, limit = 2048;
	int fd, n;
	char *x, *hw, *rev, *id;

	fd = open(cpuinfo, O_RDONLY);
	if (fd < 0) {
		printf("Failed to open %s: %s (%d).\n", cpuinfo,
		       strerror(errno), errno);
		return;
	}

	for (;;) {
		x = realloc(data, limit);
		if (!x) {
			printf("Failed to allocate memory to read %p\n",
			       (void *)cpuinfo);
			goto done;
		}
		data = x;

		n = read(fd, data + len, limit - len);

		if (n < 0) {
			printf("Failed to reading %s: %s (%d)", cpuinfo,
			       strerror(errno), errno);
			goto done;
		}
		len += n;

		if (len < limit)
			break;

		/* We filled the buffer, so increase size and loop to read more */
		limit *= 2;
	}

	data[len] = 0;
	hw = strstr(data, "\nHardware");

	if (hw) {
		x = strstr(hw, ": ");
		if (x) {
			x += 2;
			n = 0;
			while (*x && *x != '\n') {
				if (!isspace(*x))
					hardware[n++] = tolower(*x);
				x++;
				if (n == 16)
					break;
			}
			hardware[n] = 0;
		}
	} else {
		int dt_fd;
		dt_fd = open("/proc/device-tree/model", O_RDONLY);
		if (dt_fd > 0) {
			read(dt_fd, hardware, 20);
			close(dt_fd);
		}
	}

done:
	close(fd);
	free(data);
}


#if 0
static int get_axp_name(char *name, int len)
{
	int fd;
	int cnt;

	fd = open(AXP_NAME, O_RDONLY);
	if (fd < 0) {
		pr_err("open %s err.\n", AXP_NAME);
		return -1;
	}

	cnt = read(fd, name, len);
	if (cnt <= 0) {
		pr_err("read  %s err.\n", AXP_NAME);
		return -1;
	}
	name[cnt-1] = 0;

	pr_info("axp name : %s.\n", name);

	return 0;
}
#endif

static int check_regu_vol(const char *axp, const char *regu, int vol_mv)
{
	return 0;
}

static int set_vol(const char *path, int vol_mv)
{
	int fd, ret;
	int len;
	char pathname[256];
	char vol[128];

	if (path == NULL) {
		pr_err("invalid path.\n");
		return -1;
	}

	memset(pathname, 0, sizeof(pathname));
	memset(vol, 0, sizeof(vol));
	sprintf(vol, "%d", vol_mv*1000);
	len = strlen(vol);

	/*set max_microvolts */
	sprintf(pathname, "%s/%s", path, MAX_VOL_NAME);

	fd = open(pathname, O_RDWR);
	if (fd < 0) {
		pr_err("open %s err.\n", pathname);
		return -1;
	}
	ret = write(fd, vol, len);
	if (ret != len) {
		pr_err("set err: %s :%dmV.\n", pathname, vol_mv);
		return -1;
	}
	close(fd);

	/*set min_microvolts */
	sprintf(pathname, "%s/%s", path, MIN_VOL_NAME);
	fd = open(pathname, O_RDWR);
	if (fd < 0) {
		pr_err("open %s err.\n", pathname);
		return -1;
	}
	ret = write(fd, vol, len);
	if (ret != len) {
		pr_err("set err: %s :%dmV.\n", pathname, vol_mv);
		return -1;
	}
	close(fd);

	return 0;
}


static int get_regu_path(const char *regu, char *pathname, int len)
{
	int i, ret;
	int fd;
	char hardware[64] = {0};
	char regulator[128] = {0};
	char path[256] = {0};

	get_hardware_name(hardware);
	//pr_info("hardware : %s\n", hardware);

	if (!strcmp(hardware, "sun50iw10")) {
		for (i = 0; i < MAX_REGU_NUM; i++) {
			sprintf(path, "/sys/class/regulator/regulator.%d/name", i);
			if (access(path, F_OK)){
				pr_err("can not find regulator %s.\n", regu);
				return -1;
			}
			fd = open(path, O_RDONLY);
			if (fd < 0) {
				pr_err("open %s err.\n", path);
				return -1;
			}

			ret = read(fd, regulator, sizeof(regulator));
			if (ret < 0) {
				pr_err("get regulator name err.\n");
				return -1;
			}
			if (strstr(regulator, regu)) {
				sprintf(path, "/sys/class/regulator/regulator.%d", i);
				sprintf(pathname, "%s/reg-virt-consumer.%d-%s", path, i-1, regu);
				pr_info("regulator path: %s.\n", pathname);
				return 0;
			}
		}
		if (i == MAX_REGU_NUM)
			return -1;
	}

}


int set_regulator_vol(const char *regu, int vol)
{
	int ret;
	char path[256];

	memset(path, 0, sizeof(path));
	ret = get_regu_path(regu, path, sizeof(path));
	if (ret < 0) {
		pr_err("get path err %s: %dmV err\n",regu, vol);
		return -1;
	}

	ret = set_vol(path, vol);
	if (ret < 0) {
		pr_err("set %s: %dmV err\n",regu, vol);
		return -1;
	}

	pr_info("set %s: %dmV.\n", regu, vol);

	return 0;
}

int show_regulator_info(void)
{
	int fd, cnt;
	char buf[128];

	fd = open(REGU_SUMMARY, O_RDONLY|O_NONBLOCK);
	if (fd < 0) {
		pr_err("open err.\n");
		return -1;
	}
	while(1) {
		memset(buf, 0, sizeof(buf));
		cnt = read(fd, buf, sizeof(buf) -1);
		if (cnt < 0)
			return -1;
		buf[cnt] = '\0';
		printf("%s", buf);
		/* has read the final info, exit. */
		if (cnt != sizeof(buf) -1)
			break;
	}

	return 0;

}

void usage(void)
{
	printf("usage: regulator-test [option] [argv...]\n");
	printf("-s [regulator-name] [vol]: set regulator vol.\n");
	printf("Example: regulator-test -s aldo1 1800: set aldo1 1800mV.\n\n");
	printf("-p : printf all regulator info.\n");
}

int main(int argc, char **argv)
{
	signed char c;
	char regu[128];
	int vol;

	optind = 0;
	memset(regu, 0, sizeof(regu));

	if (argc == 1) {
		usage();
		return -1;
	}
	while ((c = getopt(argc, argv, "s:pah")) != -1) {
		switch(c) {
			case 's':
				strcpy(regu, optarg);
				vol = atoi(argv[optind]);
				set_regulator_vol(regu, vol);
				break;
			case 'p':
				pr_info("show all regulator vol.\n");
				show_regulator_info();
				break;
			case 'h':
			default:
				usage();
				return -1;
		}

	}

	return 0;
}
