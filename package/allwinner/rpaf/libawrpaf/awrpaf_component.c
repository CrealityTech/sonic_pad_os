#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <awrpaf_component.h>
#include "awrpaf-common.h"

struct _awrpaf_com {
	int fd;
	int dsp_id;
	struct snd_soc_dsp_component_config com_config;
	struct snd_rpaf_xferi xferi;
};

static int file_read(const char *path, char *str, unsigned int *len)
{
	int fd;
	unsigned int size;

	if (!len)
		return -1;
	size = *len;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;
	*len = read(fd, str, size);
	close(fd);
	return 0;
}

static int file_write(const char *path, char *str)
{
	int fd;

	fd = open(path, O_RDWR);
	if (fd < 0)
		return -1;
	write(fd, str, strlen(str));
	close(fd);
	return 0;
}

int awrpaf_component_algo_config_set(int dsp_id, const char *card_name,
				unsigned int algo_index, unsigned int algo_type,
				int stream, int enable)
{
	int fd, ret = -1;
	char devpath[256];
	struct snd_soc_dsp_component_config com_config = {0};
	struct snd_soc_dsp_pcm_params *pcm_params = &com_config.pcm_params;
	char *devnode = NULL;
	char strValue[16];
	unsigned strLen = sizeof(strValue);
	int value;

	snprintf(devpath, sizeof(devpath), "/dev/rpaf-dsp%d", dsp_id);
	fd = open(devpath, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed, return %d\n", devpath, fd);
		goto err;
	}

	com_config.comp_mode = SND_DSP_COMPONENT_MODE_ALGO;
	com_config.component_type = algo_type;
	com_config.sort_index = algo_index;
	strcpy(pcm_params->driver, card_name);
	pcm_params->stream = stream;
	com_config.component_used = enable;

	if (pcm_params->stream == 0) {
		devnode = "playback_component_type";
	} else {
		devnode = "capture_component_type";
	}

	if (!strcmp(pcm_params->driver, "audiocodec")) {
		snprintf(devpath, sizeof(devpath), "/sys/module/sun50iw11_cpudai/parameters/%s", devnode);
	} else if (strstr(pcm_params->driver, "daudio")) {
		snprintf(devpath, sizeof(devpath), "/sys/module/snd_soc_sunxi_hifi_daudio/parameters/%s", devnode);
	} else if (strstr(pcm_params->driver, "dmic")) {

	} else {
		goto err;
	}

	memset(strValue, 0, sizeof(strValue));
	file_read(devpath, strValue, &strLen);
	if (strLen <= 1)
		goto err;
	strValue[strLen-1] = '\0';
	value = atoi(strValue);
	if (enable && !(value & (1 << algo_type))) {
		value |= (1 << algo_type);
		snprintf(strValue, sizeof(strValue), "0x%x", value);
		file_write(devpath, strValue);
	} else if (!enable && (value & (1 << algo_type))) {
		value &= (~(1 << algo_type));
		snprintf(strValue, sizeof(strValue), "0x%x", value);
		file_write(devpath, strValue);
	}

	ret = ioctl(fd, SND_SOC_DSP_COMPONENT_IOCTL_ALGO_SET, &com_config);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent algo set failed, %d\n", ret);
		goto err;
	}
err:
	if (fd > 0)
		close(fd);
	return ret;
}

awrpaf_com_t *awrpaf_component_stream_init(int dsp_id)
{
	int fd;
	awrpaf_com_t *com = NULL;
	char path[128];

	if (dsp_id < 0 || dsp_id > 1)
		return NULL;

	com = calloc(1, sizeof(awrpaf_com_t));
	if (!com)
		return NULL;

	snprintf(path, sizeof(path), "/dev/rpaf-dsp%d", dsp_id);
	fd = open(path, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed, return %d\n", path, fd);
		goto err;
	}
	com->fd = fd;
	com->dsp_id = dsp_id;

	/* component_config */
	com->com_config.component_type = 0;
	com->com_config.comp_mode = SND_DSP_COMPONENT_MODE_STREAM;

	int i;
	for (i = 0; i < ARRAY_SIZE(com->com_config.component_sort); i++)
		com->com_config.component_sort[i] = -1;

	/* component_config.pcm_params */
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	pcm_params->rate	= 48000;
	pcm_params->channels	= 2;
	pcm_params->format	= SND_PCM_FORMAT_S16_LE;
	pcm_params->card	= 0;
	pcm_params->device	= 0;
	/*pcm_params->stream	= SND_PCM_STREAM_PLAYBACK;*/
	pcm_params->stream	= 0;
	pcm_params->hw_stream    = 0;
	pcm_params->data_type	= SND_DATA_TYPE_PCM;
	pcm_params->codec_type	= SND_CODEC_TYPE_PCM;
	pcm_params->period_size = 1024;
	pcm_params->periods	= 4;
	pcm_params->buffer_size = 1024 * 4;
	pcm_params->pcm_frames	= 1024;
	strcpy(pcm_params->driver, "audiocodec");


	return com;
err:
	if (com)
		free(com);
	return NULL;
}

int awrpaf_component_stream_set_params_card_name(awrpaf_com_t *com, const char *card_name)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;
	strcpy(pcm_params->driver, card_name);
	/*TODO*/
	pcm_params->card = 0;
	return 0;
}

int awrpaf_component_stream_set_params_stream(awrpaf_com_t *com, int stream)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	pcm_params->stream = stream;
	return 0;
}

int awrpaf_component_stream_set_params_rate(awrpaf_com_t *com, int rate)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	pcm_params->rate = rate;
	return 0;
}

int awrpaf_component_stream_set_params_channels(awrpaf_com_t *com, int channels)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	pcm_params->channels = channels;
	return 0;
}

int awrpaf_component_stream_frames_to_bytes(awrpaf_com_t *com, unsigned int frames)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	return (frames * 2 * pcm_params->channels);
}

int awrpaf_component_stream_bytes_to_frames(awrpaf_com_t *com, unsigned int bytes)
{
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	return (bytes / (2 * pcm_params->channels));
}

int awrpaf_component_stream_release(awrpaf_com_t *com)
{
	if (!com)
		return -1;
	if (com->com_config.comp_mode == SND_DSP_COMPONENT_MODE_STREAM)
		awrpaf_component_stream_remove(com);
	if (com->fd) {
		close(com->fd);
		com->fd = -1;
	}
	if (com->xferi.dump_buf)
		free(com->xferi.dump_buf);
	if (com->xferi.output_buf)
		free(com->xferi.output_buf);
	if (com->xferi.input_buf)
		free(com->xferi.input_buf);
	free(com);
	return 0;
}

int awrpaf_component_add_item(awrpaf_com_t *com, enum snd_dsp_component_type type)
{
	int i;
	if (!com)
		return -1;

	com->com_config.component_type |= (1 << type);
	for (i = 0; i < ARRAY_SIZE(com->com_config.component_sort); i++) {
		if (com->com_config.component_sort[i] == type) {
			fprintf(stderr, "component type:%d already install.\n", type);
			return -1;
		} else if (com->com_config.component_sort[i] == -1) {
			com->com_config.component_sort[i] = type;
			com->xferi.dump_type = type;
			return 0;
		}
	}

	return -1;
}

int awrpaf_component_stream_create(awrpaf_com_t *com, unsigned int len)
{
	int ret;
	struct snd_rpaf_xferi *xfer;

	if (!com)
		return -1;
	if (com->com_config.comp_mode != SND_DSP_COMPONENT_MODE_STREAM)
		return -1;

	xfer= &com->xferi;
	xfer->dump_length = awrpaf_component_stream_bytes_to_frames(com, len);
	xfer->dump_buf = malloc(len);
	if (!xfer->dump_buf) {
		fprintf(stderr, "no memory.\n");
		return -1;
	}
	/*
	 * 必须要memset, 产生缺页异常向内核申请页面
	 * 否则component_read的时候会失败
	 */
	memset(xfer->dump_buf, 0, len);
	xfer->result = 0;
	/*printf("[%s] line:%d dump_buf=%p\n", __func__, __LINE__, xfer->dump_buf);*/

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_CREATE, &com->com_config);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent create failed, %d\n", ret);
		goto err;
	}
	return ret;
err:
	if (xfer->dump_buf) {
		free(xfer->dump_buf);
		xfer->dump_buf = NULL;
	}

	return 0;
}

int awrpaf_component_stream_remove(awrpaf_com_t *com)
{
	struct snd_rpaf_xferi *xfer;
	int status, ret;

	if (!com)
		return -1;
	if (com->com_config.comp_mode != SND_DSP_COMPONENT_MODE_STREAM)
		return -1;

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_REMOVE, &status);
	if (ret != 0) {
		fprintf(stderr, "rpaf component remove failed, %d\n", ret);
		return ret;
	}

	return 0;
}

int awrpaf_component_set_buffer(awrpaf_com_t *com, unsigned int len)
{
	int ret = -1;
	struct snd_rpaf_xferi *xfer;

	if (!com)
		return -1;

	xfer= &com->xferi;
	if (com->com_config.comp_mode == SND_DSP_COMPONENT_MODE_STREAM) {
		xfer->dump_length = len/4;
		xfer->dump_buf = malloc(xfer->dump_length*4);
		if (!xfer->dump_buf) {
			fprintf(stderr, "no memory.\n");
			return -1;
		}
		memset(xfer->dump_buf, 0, xfer->dump_length);
		xfer->result = 0;
		/*printf("[%s] line:%d dump_buf=%p\n", __func__, __LINE__, xfer->dump_buf);*/
	} else {
		/*TODO*/
		xfer->dump_buf = malloc(len);
		if (!xfer->dump_buf) {
			fprintf(stderr, "no memory.\n");
			return -1;
		}
		xfer->result = 0;
		xfer->dump_length = len;

		xfer->output_buf = malloc(len);
		if (!xfer->output_buf) {
			fprintf(stderr, "no memory.\n");
			goto err;
		}
		xfer->output_length = len;

		xfer->input_buf = malloc(len);
		if (!xfer->input_buf) {
			fprintf(stderr, "no memory.\n");
			goto err;
		}
		xfer->input_length = len;
	}


	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_CREATE, &com->com_config);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent create failed, %d\n", ret);
		goto err;
	}
	return ret;
err:
	if (xfer->dump_buf) {
		free(xfer->dump_buf);
		xfer->dump_buf = NULL;
	}
	if (xfer->output_buf) {
		free(xfer->output_buf);
		xfer->output_buf = NULL;
	}
	if (xfer->input_buf) {
		free(xfer->input_buf);
		xfer->input_buf = NULL;
	}
	return ret;
}

int awrpaf_component_dump(awrpaf_com_t *com, void **addr, ssize_t *len)
{
	int ret;
	if (!com || !addr || !len)
		return -1;
	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_READ, &com->xferi);
	*addr = com->xferi.dump_buf;
	*len = com->xferi.dump_length;
	if (ret < 0) {
		fprintf(stderr, "rpaf compent read failed, %d\n", ret);
		return -1;
	}
	*addr = com->xferi.dump_buf;
	*len = com->xferi.dump_length;

	return ret;
}

int awrpaf_component_process(awrpaf_com_t *com,
			void *input, ssize_t input_len,
			void **output, ssize_t *output_len)
{
	int ret;
	if (!com || !input || !output || !output_len)
		return -1;
	/*TODO*/
	if (input_len != com->xferi.input_length)
		fprintf(stderr, "input_len=%ld, %ld\n", input_len, com->xferi.input_length);
	memcpy(com->xferi.input_buf, input, input_len);
	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_WRITE, &com->xferi);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent read failed, %d\n", ret);
		return -1;
	}
	*output = com->xferi.output_buf;
	*output_len = com->xferi.output_length;

	return ret;
}

int awrpaf_component_stream_algo_list_set(awrpaf_com_t *com, int algos_type, int *algos_sort, int algo_num)
{
	struct snd_soc_dsp_component_config *com_config	= &com->com_config;
	int component_sort[RPAF_COMPONENT_MAX_NUM];
	if (algo_num > RPAF_COMPONENT_MAX_NUM)
		return -1;
	/*printf("[%s] line:%d algo_num=%d\n", __func__, __LINE__, algo_num);*/
	com_config->component_type = algos_type;
	memcpy(com_config->component_sort, algos_sort, sizeof(int) * algo_num);
	return 0;
}

int awrpaf_component_stream_dump_type_set(awrpaf_com_t *com, int algo_type)
{
	struct snd_soc_dsp_component_config *com_config	= &com->com_config;

	com->xferi.dump_type = algo_type;
	com_config->transfer_type = algo_type;
	return 0;
}

int awrpaf_component_stream_read(awrpaf_com_t *com, void *addr, ssize_t *len)
{
	int ret;
	struct snd_soc_dsp_component_config *com_config	= &com->com_config;
	struct snd_soc_dsp_pcm_params *pcm_params = &com_config->pcm_params;

	com->xferi.dump_length = awrpaf_component_stream_bytes_to_frames(com, *len);
	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_READ, &com->xferi);
	if (ret < 0) {
		fprintf(stderr, "rpaf component read failed, %d\n", ret);
		return ret;
	}
	/*printf("[%s] line:%d len=%d, dump_len=%d, result=%d\n", __func__, __LINE__,*/
		/**len, com->xferi.dump_length, com->xferi.result);*/
	memcpy(addr, com->xferi.dump_buf, awrpaf_component_stream_frames_to_bytes(com, com->xferi.dump_length));

	return ret;
}

int awrpaf_component_stream_stop(awrpaf_com_t *com)
{
	int ret, status;

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_STOP, &status);
	if (ret < 0) {
		fprintf(stderr, "rpaf component stop failed, %d\n", ret);
		return ret;
	}

	return ret;
}

/* independence component api */


awrpaf_com_t *awrpaf_component_independence_init(int dsp_id)
{
	int fd;
	awrpaf_com_t *com = NULL;
	char path[128];

	if (dsp_id < 0 || dsp_id > 1)
		return NULL;

	com = calloc(1, sizeof(awrpaf_com_t));
	if (!com)
		return NULL;

	snprintf(path, sizeof(path), "/dev/rpaf-dsp%d", dsp_id);
	fd = open(path, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed, return %d\n", path, fd);
		goto err;
	}
	com->fd = fd;
	com->dsp_id = dsp_id;

	/* component_config */
	com->com_config.component_type = 0;
	com->com_config.comp_mode = SND_DSP_COMPONENT_MODE_INDEPENDENCE;

	int i;
	for (i = 0; i < ARRAY_SIZE(com->com_config.component_sort); i++)
		com->com_config.component_sort[i] = -1;

	/* component_config.pcm_params */
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	pcm_params->rate	= 48000;
	pcm_params->channels	= 2;
	pcm_params->format	= SND_PCM_FORMAT_S16_LE;
	pcm_params->card	= 0;
	pcm_params->device	= 0;
	/*pcm_params->stream	= SND_PCM_STREAM_PLAYBACK;*/
	pcm_params->stream	= 0;
	pcm_params->hw_stream    = 0;
	pcm_params->data_type	= SND_DATA_TYPE_PCM;
	pcm_params->codec_type	= SND_CODEC_TYPE_PCM;
	pcm_params->period_size = 1024;
	pcm_params->periods	= 4;
	pcm_params->buffer_size = 1024 * 4;
	pcm_params->pcm_frames	= 1024;

	return com;
err:
	if (com)
		free(com);
	return NULL;
}

int awrpaf_component_independence_set_algo_params(awrpaf_com_t *com, void *params, unsigned int len)
{
	struct snd_soc_dsp_pcm_params *pcm_params;

	if (!com)
		return -1;
	pcm_params = &com->com_config.pcm_params;
	if (len > sizeof(pcm_params->algo_params)) {
		fprintf(stderr, "algo params size:%ld, but request %d\n",
			sizeof(pcm_params->algo_params), len);
		return -1;
	}
	memcpy(pcm_params->algo_params, params, len);
	return 0;
}


int awrpaf_component_independence_set_buffer_size(awrpaf_com_t *com, int algo_type,
					ssize_t input_size, ssize_t output_size,
					void **export_input_buffer)
{
	int ret = -1;
	int status;
	struct snd_rpaf_xferi *xfer;
	struct snd_soc_dsp_pcm_params *pcm_params = &com->com_config.pcm_params;

	if (!com || !export_input_buffer)
		return -1;

	if (com->com_config.comp_mode != SND_DSP_COMPONENT_MODE_INDEPENDENCE)
		return -1;

	xfer= &com->xferi;
	xfer->output_buf = malloc(output_size);
	if (!xfer->output_buf) {
		fprintf(stderr, "no memory.\n");
		goto err;
	}
	xfer->output_length = output_size;

	xfer->input_buf = malloc(input_size);
	if (!xfer->input_buf) {
		fprintf(stderr, "no memory.\n");
		goto err;
	}
	xfer->input_length = input_size;
	*export_input_buffer = xfer->input_buf;

	pcm_params->input_size = input_size;
	pcm_params->output_size = output_size;
	pcm_params->dump_size = output_size;

	com->com_config.component_type = (1 << algo_type);
	com->com_config.component_sort[0] = algo_type;

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_CREATE, &com->com_config);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent create failed, %d\n", ret);
		goto err;
	}

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_START, &status);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent start failed, %d,%d\n", ret, status);
		return -1;
	}

	return ret;
err:
	if (xfer->output_buf) {
		free(xfer->output_buf);
		xfer->output_buf = NULL;
	}
	if (xfer->input_buf) {
		free(xfer->input_buf);
		xfer->input_buf = NULL;
	}
	return ret;
}

int awrpaf_component_independence_process(awrpaf_com_t *com, ssize_t input_len,
					void **output, ssize_t *output_len)
{
	int ret;
	struct snd_rpaf_xferi *xfer;

	if (!com || !output || !output_len)
		return -1;
	if (com->com_config.pcm_params.input_size < input_len ||
		com->com_config.pcm_params.output_size > *output_len) {
		fprintf(stderr, "input or output size is invalid.\n");
		return -1;
	}

	xfer= &com->xferi;
	xfer->input_length = input_len;
	xfer->output_length = *output_len;

	ret = ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_WRITE, &com->xferi);
	if (ret < 0) {
		fprintf(stderr, "rpaf compent write failed, %d\n", ret);
		return -1;
	}
	*output = xfer->output_buf;
	*output_len = xfer->output_length;

	return ret;
}

int awrpaf_component_independence_release(awrpaf_com_t *com)
{
	if (!com)
		return -1;
	if (com->com_config.comp_mode != SND_DSP_COMPONENT_MODE_INDEPENDENCE)
		return -1;

	if (com->fd > 0) {
		int status;
		if (ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_STOP, &status) < 0)
			fprintf(stderr, "rpaf compent stop failed\n");
		if (ioctl(com->fd, SND_SOC_DSP_COMPONENT_IOCTL_REMOVE, &status) < 0)
			fprintf(stderr, "rpaf compent remove failed\n");
		close(com->fd);
		com->fd = -1;
	}
	if (com->xferi.output_buf)
		free(com->xferi.output_buf);
	if (com->xferi.input_buf)
		free(com->xferi.input_buf);
	free(com);
	return 0;
}
