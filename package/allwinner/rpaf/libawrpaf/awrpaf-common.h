/*
 * rpaf-test/awrpaf-common.h
 *
 * Copyright (c) 2019 AllWinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * yumingfeng <yumingfeng@allwinnertech.com>
 *
 * This software used for debuging the misc codec function.
 */
#ifndef _AWRPAF_COMMON_H
#define _AWRPAF_COMMON_H

#include <sys/ioctl.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif
#define RPAF_COMPONENT_MAX_NUM (sizeof(unsigned int) << 3)
#define AWRPAF_DEFAULT_TEST_COUNT 10000UL

/** PCM sample format */
typedef enum _snd_pcm_format {
	/** Unknown */
	SND_PCM_FORMAT_UNKNOWN = -1,
	/** Signed 8 bit */
	SND_PCM_FORMAT_S8 = 0,
	/** Unsigned 8 bit */
	SND_PCM_FORMAT_U8,
	/** Signed 16 bit Little Endian */
	SND_PCM_FORMAT_S16_LE,
	/** Signed 16 bit Big Endian */
	SND_PCM_FORMAT_S16_BE,
	/** Unsigned 16 bit Little Endian */
	SND_PCM_FORMAT_U16_LE,
	/** Unsigned 16 bit Big Endian */
	SND_PCM_FORMAT_U16_BE,
	/** Signed 24 bit Little Endian using low three bytes in 32-bit word */
	SND_PCM_FORMAT_S24_LE,
	/** Signed 24 bit Big Endian using low three bytes in 32-bit word */
	SND_PCM_FORMAT_S24_BE,
	/** Unsigned 24 bit Little Endian using low three bytes in 32-bit word */
	SND_PCM_FORMAT_U24_LE,
	/** Unsigned 24 bit Big Endian using low three bytes in 32-bit word */
	SND_PCM_FORMAT_U24_BE,
	/** Signed 32 bit Little Endian */
	SND_PCM_FORMAT_S32_LE,
	/** Signed 32 bit Big Endian */
	SND_PCM_FORMAT_S32_BE,
	/** Unsigned 32 bit Little Endian */
	SND_PCM_FORMAT_U32_LE,
	/** Unsigned 32 bit Big Endian */
	SND_PCM_FORMAT_U32_BE,

	/* only support little endian */
	/** Signed 16 bit CPU endian */
	SND_PCM_FORMAT_S16 = SND_PCM_FORMAT_S16_LE,
	/** Unsigned 16 bit CPU endian */
	SND_PCM_FORMAT_U16 = SND_PCM_FORMAT_U16_LE,
	/** Signed 24 bit CPU endian */
	SND_PCM_FORMAT_S24 = SND_PCM_FORMAT_S24_LE,
	/** Unsigned 24 bit CPU endian */
	SND_PCM_FORMAT_U24 = SND_PCM_FORMAT_U24_LE,
	/** Signed 32 bit CPU endian */
	SND_PCM_FORMAT_S32 = SND_PCM_FORMAT_S32_LE,
	/** Unsigned 32 bit CPU endian */
	SND_PCM_FORMAT_U32 = SND_PCM_FORMAT_U32_LE,

	SND_PCM_FORMAT_LAST = SND_PCM_FORMAT_U32_BE,
} snd_pcm_format_t;

enum snd_stream_direction {
	SND_STREAM_PLAYBACK,
	SND_STREAM_CAPTURE,
};

enum snd_codec_type {
	SND_CODEC_TYPE_PCM,
	SND_CODEC_TYPE_MP3,
	SND_CODEC_TYPE_AAC,
	SND_CODEC_TYPE_OTHER,
};

enum snd_data_type {
	SND_DATA_TYPE_PCM,
	SND_DATA_TYPE_RAW,
	SND_DATA_TYPE_OTHER,
};

enum snd_dsp_comp_mode {
	SND_DSP_COMPONENT_MODE_INDEPENDENCE = 0,
	SND_DSP_COMPONENT_MODE_STREAM,
	SND_DSP_COMPONENT_MODE_ALGO,
};

struct snd_rpaf_xferi {
	int result;
	void *input_buf;
	ssize_t input_length;
	void *output_buf;
	ssize_t output_length;
	void *dump_buf;
	ssize_t dump_length;
	ssize_t dump_type;

};

struct misc_dev_cmd {
	const char *name;
	unsigned int cmd;
};

struct snd_soc_dsp_pcm_params {
	/* eg:0 sndcodec; 1 snddmic; 2 snddaudio0; 3 snddaudio1 */
	int card;
	int device;
	/*
	 * 根据名字匹配:
	 * 0: hw:audiocodec;
	 * 1: hw:snddmic;
	 * 2: hw:snddaudio0;
	 * 3: hw:snddaudio1;
	 */
	char driver[32];
	/* 1:capture; 0:playback */
	enum snd_stream_direction stream;

	/* -- HW params -- */
	snd_pcm_format_t format;		/* SNDRV_PCM_FORMAT_* */
	unsigned int rate;				/* rate in Hz */
	unsigned int channels;
	unsigned int resample_rate;
	/* only for hw substream */
	unsigned int period_size; /* 中断周期 */
	unsigned int periods;          /* 中断周期个数 */
	/* 在流中buffer务必一致大小, 代码中务必检查！ */
	unsigned int buffer_size;	/* 共享buf大小 */
	unsigned int pcm_frames;

	/* data type */
	enum snd_data_type data_type;
	/* mp3 - aac */
	enum snd_codec_type codec_type;
	/* dsp pcm status */
	int status;

	/* 从设备树种获取的私有数据 */
	unsigned int dts_data;

	/* for dsp0 is 1, for dsp1 is 0 */
	unsigned int hw_stream;

	/* dsp data transmission mode */
	unsigned int data_mode;
	/* soc stream wake/sleep */
	unsigned int stream_wake;

	/* 独立算法组件用到的参数:buffer大小 */
	unsigned int input_size;
	unsigned int output_size;
	unsigned int dump_size;
	/* 保存算法用到的参数，具体由算法定义，预留32字节 */
	unsigned int algo_params[8];
};

struct snd_soc_dsp_component_config {
	unsigned int comp_mode;
	unsigned int transfer_type;
	unsigned int component_used;
	/* 代表的是该音频流有多少个组件（最多支持32个for 32bit machine）在用 */
	unsigned int component_type; /* 0x1 << SND_DSP_AUDIO_COMPONENT_AGC/... */
	unsigned int sort_index;
	int component_sort[RPAF_COMPONENT_MAX_NUM];
	struct snd_soc_dsp_pcm_params pcm_params;
};

#define DEV_CMD(command) { #command, command}

/*
 * param[0] = MSGBOX_DSP_AUDIO_COMMAND->MSGBOX_DSP_AUDIO_COMPONENT_COMMAND
 * param[1] = *snd_dsp_audio_framework_component
 * param[2] = NULL
 * param[3] = NULL
 */
enum SND_SOC_DSP_AUDIO_COMPONENT_COMMAND {
	SND_SOC_DSP_COMPONENT_IOCTL_CREATE = _IOW('C', 0x00, struct snd_soc_dsp_component_config),
	SND_SOC_DSP_COMPONENT_IOCTL_REMOVE = _IOW('C', 0x01, int),
	SND_SOC_DSP_COMPONENT_IOCTL_STATUS = _IOR('C', 0x02, int),
	SND_SOC_DSP_COMPONENT_IOCTL_SW_PARAMS = _IOWR('C', 0x03, struct snd_soc_dsp_pcm_params),
	SND_SOC_DSP_COMPONENT_IOCTL_START = _IO('C', 0x04),
	SND_SOC_DSP_COMPONENT_IOCTL_STOP = _IO('C', 0x05),
	SND_SOC_DSP_COMPONENT_IOCTL_WRITE = _IOW('C', 0x06, struct snd_rpaf_xferi),
	SND_SOC_DSP_COMPONENT_IOCTL_READ = _IOR('C', 0x07, struct snd_rpaf_xferi),
	SND_SOC_DSP_COMPONENT_IOCTL_ALGO_SET = _IOW('C', 0x08, struct snd_soc_dsp_component_config),
	SND_SOC_DSP_COMPONENT_IOCTL_ALGO_GET = _IOWR('C', 0x09, struct snd_soc_dsp_component_config),
	SND_SOC_DSP_COMPONENT_IOCTL_UNLINK = _IO('C', 0x0A),
};

#endif
