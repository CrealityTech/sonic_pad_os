#ifndef AWRPAF_COMPONENT_H
#define AWRPAF_COMPONENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define AWRPAF_VERSION	"V1.0"

typedef struct _awrpaf_com awrpaf_com_t;
/*
 * dsp 音频组件部分
 */
enum snd_dsp_component_type {
	/* 前处理 */
	SND_DSP_COMPONENT_AGC = 0, /* 自动增益控制 */
	SND_DSP_COMPONENT_AEC, /* 回声消除 */
	SND_DSP_COMPONENT_NS,  /* 噪声抑制 */

	/* 后处理 */
	SND_DSP_COMPONENT_DECODEC, /* 解码，暂保留 */
	SND_DSP_COMPONENT_EQ,	   /* 均衡器 */
	SND_DSP_COMPONENT_MIXER,   /* 左右声道混音,暂保留*/
	SND_DSP_COMPONENT_DRC,	   /* 动态范围控制 */
	SND_DSP_COMPONENT_REVERB,

	/* 通用组件 */
	SND_DSP_COMPONENT_RESAMPLE, /* 重采样 */
	SND_DSP_COMPONENT_OTHER,

	SND_DSP_COMPONENT_USER = 20,

	SND_DSP_COMPONENT_MAX = 32,
};


/* for algo config */
int awrpaf_component_algo_config_set(int dsp_id, const char *card_name,
				unsigned int algo_index, unsigned int algo_type,
				int stream, int enable);

/* for pcm stream component */
awrpaf_com_t *awrpaf_component_stream_init(int dsp_id);
int awrpaf_component_stream_release(awrpaf_com_t *com);
int awrpaf_component_stream_read(awrpaf_com_t *com, void *addr, ssize_t *len);
int awrpaf_component_stream_stop(awrpaf_com_t *com);
int awrpaf_component_stream_create(awrpaf_com_t *com, unsigned int len);
int awrpaf_component_stream_remove(awrpaf_com_t *com);

int awrpaf_component_stream_set_params_card_name(awrpaf_com_t *com, const char *card_name);
int awrpaf_component_stream_set_params_stream(awrpaf_com_t *com, int stream);
int awrpaf_component_stream_set_params_rate(awrpaf_com_t *com, int rate);
int awrpaf_component_stream_set_params_channels(awrpaf_com_t *com, int channels);
int awrpaf_component_stream_dump_type_set(awrpaf_com_t *com, int algo_type);
int awrpaf_component_stream_frames_to_bytes(awrpaf_com_t *com, unsigned int frames);
int awrpaf_component_stream_bytes_to_frames(awrpaf_com_t *com, unsigned int bytes);

/* for independence component */
awrpaf_com_t *awrpaf_component_independence_init(int dsp_id);
int awrpaf_component_independence_set_algo_params(awrpaf_com_t *com, void *params, unsigned int len);
int awrpaf_component_independence_set_buffer_size(awrpaf_com_t *com, int algo_type,
					ssize_t input_size, ssize_t output_size,
					void **export_input_buffer);
int awrpaf_component_independence_process(awrpaf_com_t *com, ssize_t input_len,
					void **output, ssize_t *output_len);
int awrpaf_component_independence_release(awrpaf_com_t *com);

#ifdef __cplusplus
}
#endif
#endif /* AWRPAF_COMPONENT_H */
