#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <getopt.h>


#include <awrpaf_component.h>

static char *g_card_name = NULL;
static char *g_file_path = NULL;
static unsigned int g_algo_type = 0;
static unsigned int g_algo_index = 0;
static int g_stream = 0;
enum {
	ALGO_TYPE_INSTALL = 0,
	ALGO_DATA_READ,
	ALGO_PROCESS,
	ALGO_FUNCTION_MAX,

};
static int g_function = -1;
static int g_algo_enable = 0;
static unsigned int g_duration_sec = 5;
static unsigned int g_rate = 48000;
static unsigned int g_channels = 2;
static unsigned int g_period_size = 1024;
static unsigned int g_periods = 4;
static int g_looptest_count = 1;


static int algo_install(void)
{
	int ret;

	ret = awrpaf_component_algo_config_set(0, g_card_name,
			g_algo_index, g_algo_type, g_stream, g_algo_enable);
	return ret;
}

static int algo_read(void)
{
	int ret;
	int fd = -1;
	awrpaf_com_t *com;
	unsigned int total_bytes;
	char *buffer = NULL;
	ssize_t bufferlen = 0;
	unsigned int loop_read_size = 0;

	com = awrpaf_component_stream_init(0);
	if (!com)
		return -1;

	awrpaf_component_stream_set_params_card_name(com, g_card_name);
	awrpaf_component_stream_set_params_stream(com, g_stream);
	awrpaf_component_stream_set_params_rate(com, g_rate);
	awrpaf_component_stream_set_params_channels(com, g_channels);

	awrpaf_component_stream_dump_type_set(com, g_algo_type);
	loop_read_size = awrpaf_component_stream_frames_to_bytes(com, 160);
	awrpaf_component_stream_create(com, loop_read_size);

	if (g_file_path != NULL)
		fd = open(g_file_path, O_CREAT | O_RDWR, 0);
	/*printf("[%s] line:%d fd=%d, path:%s\n", __func__, __LINE__, fd, g_file_path);*/
	total_bytes = g_duration_sec * 2 * g_channels * g_rate;
	buffer = malloc(loop_read_size);
	if (!buffer) {
		printf("no memory\n");
		exit(-1);
	}
	/*printf("[%s] line:%d total_bytes=%d\n", __func__, __LINE__, total_bytes);*/

	while (total_bytes > 0) {
		bufferlen = loop_read_size;
		if (bufferlen > total_bytes)
			bufferlen = total_bytes;
		/*printf("[%s] line:%d bufferlen=%ld, loop_read_size=%d\n", __func__, __LINE__, bufferlen, loop_read_size);	*/
		ret = awrpaf_component_stream_read(com, buffer, &bufferlen);
		if (ret != 0) {
			printf("[%s] line:%d component read failed:%d\n", __func__, __LINE__, ret);
			break;
		}
		if (bufferlen != loop_read_size)
			printf("[%s] line:%d ERROR, bufferlen=%ld, loop_read_size=%d\n", __func__, __LINE__, bufferlen, loop_read_size);
		total_bytes -= bufferlen;
		write(fd, buffer, bufferlen);
	}

	awrpaf_component_stream_stop(com);

	if (fd > 0)
		close(fd);
	if (buffer != NULL)
		free(buffer);

	awrpaf_component_stream_release(com);


	return 0;
}

static int algo_process(void)
{
	int ret, i;
	int fd = -1;
	awrpaf_com_t *com;
	void *input_buffer = NULL;
	ssize_t input_size = 1024;
	ssize_t output_size = 1024;
	void *output_buffer = NULL;
	int algo_params[8] = {1, 2, 3, 4};

	com = awrpaf_component_independence_init(0);
	if (!com)
		return -1;


	awrpaf_component_independence_set_algo_params(com, (void *)algo_params, sizeof(algo_params));
	awrpaf_component_independence_set_buffer_size(com, g_algo_type,
					input_size, output_size, &input_buffer);

	for (i = 0; i < g_looptest_count; i++) {
		memset(input_buffer, 0x11, input_size);
		ret = awrpaf_component_independence_process(com, input_size,
					&output_buffer, &output_size);
		if (ret != 0)
			printf("[%s] line:%d algo process failed:%d\n", __func__, __LINE__, ret);
	}

	if (g_file_path != NULL &&
		(fd = open(g_file_path, O_CREAT | O_RDWR, 0)) > 0) {
		write(fd, output_buffer, output_size);
		close(fd);
	}
	awrpaf_component_independence_release(com);
	return 0;
}

static void usage(void)
{
	printf("awrpaf_demo usage:\n");
	printf(" -C,  --card       sound card name\n");
	printf(" -F,  --function   function select, 0:pcm stream algo install;\n");
	printf("                                    1:pcm stream algo read;\n");
	printf("                                    2:independence algo process\n");
	printf(" -t,  --algotype   algo type, e.g. 4:EQ; 20:USER\n");
	printf(" -i,  --algoindex  the sequence of execution of algo, 0 means the first algo\n");
	printf(" -s,  --stream     pcm stream, 0:playback; 1:capture\n");
	printf(" -f,  --filepath   file path\n");
	printf(" -e,  --enable     used for \"algo install\", it means enable algo default\n");
	printf(" -d,  --duration   used for \"algo read\", it means the time of algo read process\n");
	printf(" -c,  --channels   pcm channels, default 2\n");
	printf(" -r,  --rate       pcm rate, default 48000\n");
	printf(" -h,  --help       show help\n");
	printf("\n");
	printf("example(algo install):\n");
	printf("awrpaf_demo -C audiocodec -F 0 -t 4 -i 0 -s 0 -e\n");
	printf("  install the algo with the type 4(EQ), index 0(1st algo), stream 0(playback)\n");
	printf("  and enable default\n");
	printf("example(algo read):\n");
	printf("awrpaf_demo -C audiocodec -F 1 -t 4 -s 0 -d 3 -f /tmp/test.pcm\n");
	printf("  read algo data for 3 seconds, with the algo type 4(EQ), stream 0(playback)\n");
	printf("  and save algo data into /tmp/test.pcm\n");
	printf("example(algo process):\n");
	printf("awrpaf_demo -C audiocodec -F 2 -t 8 -f /tmp/algo_raw_data");
	printf("\n");
	exit(-1);
}

int main(int argc, char *argv[])
{
	int ret = -1;

	while (1) {
		const struct option long_options[] = {
			{"card", required_argument, NULL, 'C'},
			{"function", required_argument, NULL, 'F'},
			{"algotype", required_argument, NULL, 't'},
			{"algoindex", required_argument, NULL, 'i'},
			{"stream", required_argument, NULL, 's'},
			{"filepath", required_argument, NULL, 'f'},
			{"enable", no_argument, NULL, 'e'},
			{"duration", required_argument, NULL, 'd'},
			{"channels", required_argument, NULL, 'c'},
			{"rate", required_argument, NULL, 'r'},
			{"looptest", required_argument, NULL, 'l'},
			{"help", no_argument, NULL, 'h'},
		};
		int option_index = 0;
		int c = 0;
		c = getopt_long(argc, argv, "C:c:F:t:i:s:f:eb:d:r:l:h", long_options, &option_index);
		if (c == -1)
			break;
		switch (c) {
		case 'h':
			usage();
			break;
		case 'C':
			g_card_name = strdup(optarg);
			break;
		case 'F':
			g_function = atoi(optarg);
			if (g_function >= ALGO_FUNCTION_MAX || g_function < 0)
				usage();
			break;
		case 't':
			g_algo_type = atoi(optarg);
			break;
		case 'i':
			g_algo_index = atoi(optarg);
			break;
		case 's':
			g_stream = atoi(optarg);
			break;
		case 'e':
			g_algo_enable = 1;
			break;
		case 'f':
			g_file_path = strdup(optarg);
			break;
		case 'd':
			g_duration_sec = atoi(optarg);
			break;
		case 'r':
			g_rate = atoi(optarg);
			break;
		case 'c':
			g_channels = atoi(optarg);
			break;
		case 'l':
			g_looptest_count = atoi(optarg);
			break;
		default:
			usage();
			break;
		}
	}

	if (optind != argc)
		usage();

	switch (g_function) {
	case ALGO_TYPE_INSTALL:
		ret = algo_install();
		break;
	case ALGO_DATA_READ:
		ret = algo_read();
		break;
	case ALGO_PROCESS:
		ret = algo_process();
		break;
	}

err:
	if (g_card_name)
		free(g_card_name);
	if (g_file_path)
		free(g_file_path);
	return ret;
}
