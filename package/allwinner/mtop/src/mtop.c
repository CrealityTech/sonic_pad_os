#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MTOP_VERSION "0.5"
#define PLATFORM Linux

#if PLATFORM == Linux

#include "check_hardware.h"
#define PROPERTY_VALUE_MAX 20

#elif PLATFORM == Andriod

#include <cutils/properties.h>

#endif

typedef struct NodeInfo {
	int	 id;
	char *name;
	FILE *fp;
	unsigned int curcnt;
	unsigned int precnt;
	unsigned long long delta;
} NodeInfo;

typedef struct NodeUnit {
	int	 id;
	char *name;
	unsigned int div;
} NodeUnit;

/**
 * NOTE: we allway put totddr at first array whether this node exit or not,
 * fot the totddr is caculated every time.
 */
NodeInfo nodeInfo_sun9i[] = {
	{ 0, "totddr",  NULL, 0, 0, 0},
	{ 1, "cpuddr0",  NULL, 0, 0, 0},
	{ 2, "gpuddr0",  NULL, 0, 0, 0},
	{ 3, "de_ddr0",  NULL, 0, 0, 0},
	{ 4, "vcfddr0",  NULL, 0, 0, 0},
	{ 5, "othddr0",  NULL, 0, 0, 0},
	{ 6, "cpuddr1",  NULL, 0, 0, 0},
	{ 7, "gpuddr1",  NULL, 0, 0, 0},
	{ 8, "de_ddr1",  NULL, 0, 0, 0},
	{ 9, "vcfddr1",  NULL, 0, 0, 0},
	{ 10, "othddr1",  NULL, 0, 0, 0},
};
NodeUnit nodeUnit_sun9i[] = {
	{ 0, "KB",  0},
	{ 1, "MB",  10},
};

NodeInfo nodeInfo_sun8i[] = {
	{ 0, "totddr", NULL, 0, 0, 0},
	{ 1, "cpuddr", NULL, 0, 0, 0},
	{ 2, "gpuddr", NULL, 0, 0, 0},
	{ 3, "de_ddr", NULL, 0, 0, 0},
	{ 4, "ve_ddr", NULL, 0, 0, 0},
	{ 5, "othddr", NULL, 0, 0, 0},
};
NodeUnit nodeUnit_sun8i[] = {
	{ 0, "KB",  0},
	{ 1, "MB",  1024},
};

NodeInfo nodeInfo_sun50i[] = {
	{ 0, "totddr", NULL, 0, 0, 0},
	{ 1, "cpuddr", NULL, 0, 0, 0},
	{ 2, "gpuddr", NULL, 0, 0, 0},
	{ 3, "de_ddr", NULL, 0, 0, 0},
	{ 4, "ve_ddr", NULL, 0, 0, 0},
	{ 5, "othddr", NULL, 0, 0, 0},
};

NodeUnit nodeUnit_sun50i[] = {
	{ 0, "KB",   0},
	{ 1, "MB",  1024},
};

NodeInfo nodeInfo_sun50iw10[] = {
	{ 0, "total_ddr", NULL, 0, 0, 0},
	{ 1, "cpuddr", NULL, 0, 0, 0},
	{ 2, "gpuddr", NULL, 0, 0, 0},
	{ 3, "de0_ddr", NULL, 0, 0, 0},
	{ 4, "de1_ddr", NULL, 0, 0, 0},
	{ 5, "g2d_ddr", NULL, 0, 0, 0},
	{ 6, "ve_ddr", NULL, 0, 0, 0},
	{ 7, "csi_ddr", NULL, 0, 0, 0},
	{ 8, "isp_ddr", NULL, 0, 0, 0},
	{ 9, "eink_ddr", NULL, 0, 0, 0},
};

NodeInfo nodeInfo_sun50iw11[] = {
	{ 0, "total_ddr", NULL, 0, 0, 0},
	{ 1, "cpuddr", NULL, 0, 0, 0},
	{ 2, "mahb_ddr", NULL, 0, 0, 0},
	{ 3, "dma_ddr", NULL, 0, 0, 0},
	{ 4, "ce_ddr", NULL, 0, 0, 0},
	{ 5, "dsp_ddr", NULL, 0, 0, 0},
	{ 6, "ndfc_ddr", NULL, 0, 0, 0},
	{ 7, "r_dma_ddr", NULL, 0, 0, 0},
	{ 8, "aipu_ddr", NULL, 0, 0, 0},
};

NodeInfo *nodeInfo;
NodeUnit *nodeUnit;

unsigned int max;
unsigned long long total;
unsigned long long idx;

int nodeCnt;

int delay;
int iter;
int mUnit = 0;
int latency;
unsigned int per;
char output_fn[256];
FILE *output_fp;

int nhardware;
char path_prefix[256];

#define GTBUS_PMU_DIR "/sys/devices/platform/GTBUS_PMU"
#define MBUS_PMU_DIR "/sys/class/hwmon/mbus_pmu/device"

static int mtop_baner();
static int mtop_read();
static void mtop_post();
static void mtop_update();
static void mtop_nsi_update();

static void usage(char *program)
{
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage: %s [-n iter] [-d delay] [-m] [-o FILE] [-h]\n", program);
	fprintf(stdout, "    -n NUM   Updates to show before exiting.\n");
	fprintf(stdout, "    -d NUM   Seconds to wait between update.\n");
	fprintf(stdout, "    -m unit: %s\n", mUnit ? nodeUnit[1].name : nodeUnit[0].name);
	fprintf(stdout, "    -o FILE  Output to a file.\n");
	fprintf(stdout, "    -v Display mtop version.\n");
	fprintf(stdout, "    -h Display this help screen.\n");
	fprintf(stdout, "\n");
}

static void version(void)
{
	fprintf(stdout, "\n");
	fprintf(stdout, "mtop version: %s\n", MTOP_VERSION);
	fprintf(stdout, "\n");
}

static int get_path(char *buf, unsigned int len, char *name)
{
	unsigned int i = 0;

	while (1) {
		int fd;
		char temp[128];
		snprintf(buf, len, "/sys/class/hwmon/hwmon%u", i);
		if (access(buf, F_OK) != 0)
			break;
		snprintf(temp, sizeof(temp), "%s/name", buf);
		fd = open(temp, O_RDONLY);
		memset(temp, 0, sizeof(temp));
		read(fd, temp, sizeof(temp));
		if (!strncmp(temp, name , sizeof(*name) - 1)) {
			return 0;
		}
		i++;
	}
	return -1;
}

static int find_timer_path(void)
{
	unsigned int i = 0;
	char buf[128];
	char temp[128];
	while (1) {
		int fd;
		snprintf(buf, sizeof(buf), "/sys/class/hwmon/hwmon%u/port_timer", i);
		snprintf(temp, sizeof(temp), "/sys/class/hwmon/hwmon%u", i);
		if (access(buf, F_OK) == 0)
			return i;
		if (access(temp, F_OK) != 0)
			break;
		i++;
	}
	return -1;
}

static void set_nsi_pmu_timer(float time)
{
	char command[512];
	unsigned int ret = find_timer_path();
	if (ret == -1)
		printf("there is no nsi driver, please check the driver.\n");
	char pmu_timer[128];
	snprintf(pmu_timer, sizeof(pmu_timer), "/sys/class/hwmon/hwmon%u/port_timer", ret);
	int i = 0;
	int master_id[10] = {0, 1, 11, 12, 13, 14, 15, 16, 17, 20};
	for (i = 0; i < (sizeof(master_id)/sizeof(int)); i++ ) {
		sprintf(command, "echo %d %d > %s", master_id[i], (int)(time*1000*1000), pmu_timer);
		system(command);
	}
}

int main(int argc, char *argv[])
{
	int i;
	unsigned long value, bandwidth;
    char hardware[PROPERTY_VALUE_MAX]={0};

#if PLATFORM == Linux
    get_hardware_name(hardware);
#elif PLATFORM == Android
	property_get("ro.hardware", hardware, "sun9i");
#endif

	if (!strcmp(hardware, "sun9i")) {
		nhardware = 0;
		nodeCnt = sizeof(nodeInfo_sun9i)/sizeof(nodeInfo_sun9i[0]);
		strncpy(path_prefix, GTBUS_PMU_DIR, sizeof(path_prefix));
		nodeInfo = nodeInfo_sun9i;
		nodeUnit = nodeUnit_sun9i;
	} else if (strstr(hardware, "sun8i")) {
		nhardware = 1;
		nodeCnt = sizeof(nodeInfo_sun8i)/sizeof(nodeInfo_sun8i[0]);
		if (get_path(path_prefix, sizeof(path_prefix), "mbus_pmu") != 0)
			strncpy(path_prefix, "/sys/class/hwmon/mbus_pmu/device", sizeof(path_prefix));
		nodeInfo = nodeInfo_sun8i;
		nodeUnit = nodeUnit_sun8i;
		if (!strcmp(hardware, "sun8iw8")) {
			mUnit = 1;
			nodeUnit[1].div = 1;
		}
	} else if (!strcmp(hardware, "sun50iw1") || !strcmp(hardware, "sun50iw3")
			|| !strcmp(hardware, "sun50iw6") || !strcmp(hardware, "sun50iw11")) {
		nhardware = 2;
		if (get_path(path_prefix, sizeof(path_prefix), "mbus_pmu") != 0) {
			printf("mbus_pmu path cannot found\n");
			return -1;
		}
		nodeUnit = nodeUnit_sun50i;
		if (!strcmp(hardware, "sun50iw11")) {
			nodeInfo = nodeInfo_sun50iw11;
			nodeCnt = sizeof(nodeInfo_sun50iw11)/sizeof(nodeInfo_sun50iw11[0]);
		} else {
			nodeInfo = nodeInfo_sun50i;
			nodeCnt = sizeof(nodeInfo_sun50i)/sizeof(nodeInfo_sun50i[0]);
		}
	} else if (!strcmp(hardware, "sun50iw10")) {
		nhardware = 2;
		nodeInfo = nodeInfo_sun50iw10;
		nodeCnt = sizeof(nodeInfo_sun50iw10)/sizeof(nodeInfo_sun50iw10[0]);
		nodeUnit = nodeUnit_sun50i;
		if (get_path(path_prefix, sizeof(path_prefix), "nsi_pmu") != 0) {
			printf("nsi_pmu path cannot found\n");
			return -1;
		}
	} else {
		fprintf(stdout, "not supported platform!\n");
	}

	max = 0;
	total = 0;
	idx = 0;

	delay = 1;
	iter = -1;
	latency = 0;
	per = 0xffffffff;
	memset(output_fn, 0, sizeof(output_fn));
	output_fp = NULL;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-n")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Option -n expects an argument.\n");
				usage(argv[0]);
				exit(-1);
			}
			iter = atoi(argv[++i]);
			// FIXME
			continue;
		}

		if (!strcmp(argv[i], "-d")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Option -d expects an argument.\n");
				usage(argv[0]);
				exit(-1);
			}
			delay = atoi(argv[++i]);
			// FIXME
			continue;
		}

		if (!strcmp(argv[i], "-m")) {
			mUnit = 1;
			continue;
		}

		if (!strcmp(argv[i], "-l")) {
			latency = 1;
			continue;
		}
/*
		if (!strcmp(argv[i], "-p")) {
			per = 1;
			continue;
		}
*/
		if (!strcmp(argv[i], "-o")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Option -o expects an argument.\n");
				usage(argv[0]);
				exit(-1);
			}
			strncpy(output_fn, argv[++i], 256);
			continue;
		}

		if (!strcmp(argv[i], "-v")) {
			version();
			exit(0);
		}

		if (!strcmp(argv[i], "-h")) {
			usage(argv[0]);
			exit(0);
		}

		fprintf(stderr, "Invalid argument \"%s\".\n", argv[i]);
		usage(argv[0]);
		exit(-1);
	}

	fprintf(stdout, "\n");
	fprintf(stdout, "iter: %d\n", iter);
	fprintf(stdout, "dealy: %d\n", delay);
	fprintf(stdout, "unit: %s\n", mUnit ? nodeUnit[1].name : nodeUnit[0].name);
	fprintf(stdout, "output: %s\n", output_fn);
	fprintf(stdout, "\n");

	if (output_fn[0]) {
		output_fp = fopen(output_fn, "wt+");
		if (NULL == output_fp) {
			fprintf(stdout, "Could not open file %s: %s\n", output_fn, strerror(errno));
			exit(-1);
		}
		mtop_baner();
	}

	mtop_read();
	mtop_post();

	if (!strcmp(hardware, "sun50iw10")) {
		set_nsi_pmu_timer(1);
		while (iter == -1 || iter-- > 0) {
			sleep(delay);
			mtop_read();
			mtop_nsi_update();
			mtop_post();
		}
	} else {
		while (iter == -1 || iter-- > 0) {
			sleep(delay);
			mtop_read();
			mtop_update();
			mtop_post();
		}
	}

	if (output_fp) {
		fclose(output_fp);
	}

	return 0;
}

static int mtop_baner(void)
{
	int i;

	for (i = 1; i < nodeCnt; i++) {
		if (per & (1UL << i)) {
			fwrite(nodeInfo[i].name, strlen(nodeInfo[i].name), 1, output_fp);
			if (i+1 < nodeCnt && per & (1UL << (i+1))) {
				fwrite(" ", 1, 1, output_fp);
			}
		}
	}

	fwrite("\n", 1, 1, output_fp);

	return 0;
}

static int mtop_read(void)
{
	int i;
	char path[256];

	for (i = 1; i < nodeCnt; i++) {
		if (per & (1UL << i)) {
			snprintf(path, sizeof(path), "%s/pmu_%s", path_prefix, nodeInfo[i].name);
			nodeInfo[i].fp = fopen(path, "r");
			if (NULL == nodeInfo[i].fp) {
				fprintf(stderr, "Could not open file %s: %s\n", path, strerror(errno));
				goto open_error;
			}

			fscanf(nodeInfo[i].fp, "%u", &nodeInfo[i].curcnt);
			fclose(nodeInfo[i].fp);
			nodeInfo[i].fp = NULL;
		}
	}

	return 0;

open_error:
	for (i = 1; i < nodeCnt; i++) {
		if (nodeInfo[i].fp) {
			fclose(nodeInfo[i].fp);
			nodeInfo[i].fp = NULL;
		}
	}
	return -1;
}

static void mtop_post(void)
{
	int i;

	for (i = 1; i < nodeCnt; i++) {
		if (per & (1UL << i)) {
			nodeInfo[i].precnt = nodeInfo[i].curcnt;
		}
	}
}

static void mtop_update(void)
{
	int i;
	unsigned int cur_total;
	unsigned int average;
	char buf[1024];

	cur_total = 0;

	nodeInfo[0].delta = 0;
	for (i = 1; i < nodeCnt; i++) {
		if (per & (1UL << i)) {
			if (nodeInfo[i].precnt <= nodeInfo[i].curcnt)
				nodeInfo[i].delta = nodeInfo[i].curcnt - nodeInfo[i].precnt;
			else
				nodeInfo[i].delta = (unsigned int)((nodeInfo[i].curcnt + (unsigned long long)(2^32)) - nodeInfo[i].precnt);

			if (mUnit)
				nodeInfo[i].delta /= nodeUnit[1].div;

			cur_total += nodeInfo[i].delta;
		}
	}
	nodeInfo[0].delta = cur_total;

	if (cur_total > max)
		max = cur_total;
	total += cur_total;
	idx++;
	average = total / idx;

	fprintf(stdout, "total: %llu, ", total);
	fprintf(stdout, "num: %llu, ", idx);
	fprintf(stdout, "Max: %u, ", max);
	fprintf(stdout, "Average: %u\n", average);

	for (i = 0; i < nodeCnt; i++)
		fprintf(stdout, " %7s", nodeInfo[i].name);
	fprintf(stdout, "\n");
	for (i = 0; i < nodeCnt; i++)
		fprintf(stdout, " %7llu", nodeInfo[i].delta);
	fprintf(stdout, "\n");


	if (cur_total == 0)
		cur_total++;
	for (i = 0; i < nodeCnt; i++)
		fprintf(stdout, " %7.2f", (float)nodeInfo[i].delta*100/cur_total);
	fprintf(stdout, "\n\n");

	if (output_fp) {
		for (i = 0; i < nodeCnt; i++) {
			fprintf(output_fp, " %6s", nodeInfo[i].name);
		}
		fprintf(output_fp, "\n");

		for (i = 0; i < nodeCnt; i++) {
			fprintf(output_fp, "%6llu", nodeInfo[i].delta);
		}
		fprintf(output_fp, "\n");

		for (i = 0; i < nodeCnt; i++) {
			fprintf(output_fp, "%6.2f", (float)nodeInfo[i].delta*100/cur_total);
		}
		fprintf(output_fp, "\n");
	}
	if (output_fp) {
		fwrite("\n", 1, 1, output_fp);
		fflush(output_fp);
	}
}
static void mtop_nsi_update(void)
{
	int i, total = 0;

	nodeInfo[0].delta = 0;
	for (i = 1; i < nodeCnt; i++) {
		nodeInfo[i].delta = nodeInfo[i].curcnt;
		if (mUnit)
			nodeInfo[i].delta /= nodeUnit[1].div;
		total += nodeInfo[i].delta;
	}
	nodeInfo[0].delta = total;

	for (i = 0; i < nodeCnt; i++)
		fprintf(stdout, " %9s", nodeInfo[i].name);
	fprintf(stdout, "\n");

	for (i = 0; i < nodeCnt; i++) {
		fprintf(stdout, " %8.2f ", (float)nodeInfo[i].delta * (1000 / (delay * 1000)));
		if (output_fp)
			fprintf(output_fp, "%-9.2f ", (float)nodeInfo[i].delta * (1000 / (delay * 1000)));
	}
	fprintf(stdout, "\n");
	for (i = 0; i < nodeCnt; i++) {
		fprintf(stdout, " %9.2f", (float)nodeInfo[i].delta*100/total);
	}

	fprintf(stdout, "\n");
	fprintf(stdout, "\n");

	if (output_fp) {
		fwrite("\n", 1, 1, output_fp);
		fflush(output_fp);
	}
}
