#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <alsa/asoundlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>
#include "va_audio.h"
#include "va_driver.h"

static unsigned int vol;

struct amixer_control {
    char *name;
    unsigned int value;
};

struct amixer_control controls[] =
{
#ifdef CONFIG_CHIP_R40
    /*r40*/
    {"Left Output Mixer DACL Switch", 1},
    {"Right Output Mixer DACR Switch", 1},
    {"HPL Mux", 1},
    {"HPR Mux", 1},
    {"Headphone Switch", 1}
#endif

#ifdef CONFIG_CHIP_R16
    /*r16*/
    {"AIF1IN0L Mux", 0},
    {"AIF1IN0R Mux", 0},
    {"DACL Mixer AIF1DA0L Switch", 1},
    {"DACR Mixer AIF1DA0R Switch", 1},
    {"Left Output Mixer DACL Switch", 1},
    {"Right Output Mixer DACR Switch", 1},
    {"HP_L Mux", 1},
    {"HP_R Mux", 1},
    {"Headphone Switch", 1}
#endif

#ifdef CONFIG_CHIP_R6
    /*r6*/
    {"head phone power", 1},
    {"ADC MIC Boost AMP en", 1},
    {"ADC mixer mute for mic", 1},
#endif

#ifdef CONFIG_CHIP_R11
    /*r11*/
    {"Speaker Function", 1},
    {"Audio speaker out", 1}
#endif

#ifdef CONFIG_CHIP_R7
    /*r7*/
    {"Speaker Function", 1},
    {"Audio speaker out", 1}
#endif

#ifdef CONFIG_CHIP_C200S
    {"head phone power", 1},
    {"ADC MIC Boost AMP en", 1},
    {"ADC mixer mute for mic", 1},
#endif
};


typedef struct __WAVE_HEADER1
{
    unsigned int       uRiffFcc;       // four character code, "RIFF"
    unsigned int       uFileLen;       // file total length, don't care it

    unsigned int       uWaveFcc;       // four character code, "WAVE"

    unsigned int       uFmtFcc;        // four character code, "fmt "
    unsigned int       uFmtDataLen;    // Length of the fmt data (=16)
    unsigned short       uWavEncodeTag;  // WAVE File Encoding Tag
    unsigned short       uChannels;      // Channels: 1 = mono, 2 = stereo
    unsigned int        uSampleRate;    // Samples per second: e.g., 44100
    unsigned int        uBytesPerSec;   // sample rate * block align
    unsigned short       uBlockAlign;    // channels * bits/sample / 8
    unsigned short       uBitsPerSample; // 8 or 16

    unsigned int       uDataFcc;       // four character code "data"
    unsigned int       uSampDataSize;  // Sample data size(n)

}__attribute__((packed))  wave_header_t;
#define BUF_LEN 1024

static wave_header_t keytone_head;
static char *keytone_buff = NULL;
static unsigned int keytone_len;

static int audio_mixer_cset(char *mixer_name,int value)
{
    char cmd[128] = {0};
    sprintf(cmd,"amixer cset name='%s' %d",mixer_name,value);
    system(cmd);
    return 0;
}

int audio_mixer_get(const char* shell,const char* name)
{
    int bytes;
    char buf[10] = {0};
    char cmd[500] = {0};
    sprintf(cmd,shell,name);
    FILE   *stream;
    printf("%s\n",cmd);
    stream = popen( cmd, "r" );
    if(!stream)
		return -1;
    bytes = fread( buf, sizeof(char), sizeof(buf), stream);
    pclose(stream);
    if(bytes > 0){
            return atoi(buf);
    }else {
            printf("%s --> failed\n",cmd);
            return -1;
    }
}

/*0~31*/
int va_audio_set_volume(int volume)
{
#ifdef CONFIG_CHIP_R11
    char *name = "Master Playback Volume";/*r11*/
#endif

#ifdef CONFIG_CHIP_R7
    char *name = "Master Playback Volume";/*r7*/
#endif

#ifdef CONFIG_CHIP_R6
    char *name = "head phone volume"; /*r6*/
#endif

#ifdef CONFIG_CHIP_R40
    char *name = "Headphone volume"; /*r40*/
#endif

#ifdef CONFIG_CHIP_R16
    char *name = "headphone volume"; /*r16*/
#endif

#ifdef CONFIG_CHIP_C200S
	char *name = "head phone volume"; 
#endif

    audio_mixer_cset(name,volume);
	vol = volume;
	return 0;
}

int va_audio_get_volume(void)
{
	return vol;
}
/*0~7 */
int va_audio_set_mic_AMP_gain_value(int val)
{
#ifdef CONFIG_CHIP_R11
    char *name = "MIC1 boost AMP gain control";/*r11*/
#endif

#ifdef CONFIG_CHIP_R7
    char *name = "MIC1 boost AMP gain control";/*r7*/
#endif

#ifdef CONFIG_CHIP_R16
    char *name = "MIC1 boost amplifier gain";/*r16*/
#endif

#ifdef CONFIG_CHIP_R40
    char *name = "MIC1 boost volume";/*r40*/
#endif

#ifdef CONFIG_CHIP_R6
    char *name = "MICIN GAIN control";/*r6*/
#endif

#ifdef CONFIG_CHIP_C200S
	char *name = "MICIN GAIN control";
#endif

    audio_mixer_cset(name,val);
	return 0;
}
/*0~7*/
int va_audio_set_mic_mixer_value(int val)
{
	char *name = "MIC1_G boost stage output mixer control";
    char cmd[100] = {0};

	audio_mixer_cset(name,val);
	return  0;
}

int va_audio_get_mic_AMP_gain_value(int val)
{
#ifdef CONFIG_CHIP_R11
    char *name = "MIC1 boost AMP gain control";/*r11*/
#endif

#ifdef CONFIG_CHIP_R7
    char *name = "MIC1 boost AMP gain control";/*r7*/
#endif

#ifdef CONFIG_CHIP_R16
    char *name = "MIC1 boost amplifier gain";/*r16*/
#endif

#ifdef CONFIG_CHIP_R40
    char *name = "MIC1 boost volume";/*r40*/
#endif

#ifdef CONFIG_CHIP_R6
    char *name = "MICIN GAIN control";/*r6*/
#endif

#ifdef CONFIG_CHIP_C200S
	char *name = "MICIN GAIN control";
#endif

    const char* shell = "volume=`amixer cget name='%s' | grep ': values='`;volume=${volume#*=};echo $volume";
    return audio_mixer_get(shell,name);
}

int va_audio_get_mic_mixer_value(int val)
{
	char *name = "MIC1_G boost stage output mixer control";
    const char* shell = "volume=`amixer cget name='%s' | grep ': values='`;volume=${volume#*=};echo $volume";
    return audio_mixer_get(shell,name);
}

int va_audio_init(void)
{
    int i = 0;
    int num = sizeof(controls)/sizeof(controls[0]);
    for(i = 0;i < num;i++)
    {
        audio_mixer_cset(controls[i].name,controls[i].value);
    }

    return  0;
}


int va_audio_play_wav_music(const char * partname)
{
	int i;	int err;
	wave_header_t wav;
	int headwavcntp;
	snd_pcm_t *playback_handle;	snd_pcm_hw_params_t *hw_params;
	FILE *fp = NULL;
	snd_pcm_format_t pcm_fmt;
	char buf[BUF_LEN];

	fprintf(stderr, "open file : %s\n", partname);
	fp = fopen(partname, "r");
	if (fp == NULL) {
		fprintf(stderr, "open test pcm file err\n");
		return -1;
	}

	headwavcntp = fread(&wav, 1, sizeof(wave_header_t), fp);
	if(headwavcntp != sizeof(wave_header_t)){
		printf("read wav file head error!\n");
		fclose(fp);
		return -1;
	}

	printf("read wav file head success \n");
	//printf("bps = %d\n", wav.uBitsPerSample);
	//printf("chn = %d\n", wav.uChannels);
	//printf("fs = %d\n", wav.uSampleRate);

	if(wav.uBitsPerSample == 8){
		pcm_fmt = SND_PCM_FORMAT_S8;
	}else if(wav.uBitsPerSample == 16){
		pcm_fmt = SND_PCM_FORMAT_S16_LE;
	}else{
		printf("uBitsPerSample not support!\n");
		fclose(fp);
		return -1;
	}
		if((err = snd_pcm_open(&playback_handle,  "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0){
			fprintf(stderr, "cannot open audio device record.pcm (%s)\n",  snd_strerror(err));
			fclose(fp);
			return -1;
		}
		if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0){
			fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0){
			fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0){
			fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, pcm_fmt)) < 0){
			fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, wav.uSampleRate, 0)) < 0){
			fprintf(stderr , "cannot set sample rate (%s)\n", snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, wav.uChannels)) <0){
			fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
			goto play_wav_out;
		}
		if((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0){
			fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
			goto play_wav_out;
		}
		snd_pcm_hw_params_free(hw_params);

		while (!feof(fp)) {
			err = fread(buf, 1, BUF_LEN, fp);
			if (err < 0){
				fprintf(stderr, "read pcm from file err\n");
				goto play_wav_out;
			}
			err = snd_pcm_writei(playback_handle, buf , BUF_LEN/4);
			if (err < 0){
				fprintf(stderr, "write to audio interface failed (%s)\n",
				snd_strerror(err));
				goto play_wav_out;
			}
		}

play_wav_out:
	fprintf(stderr, "close file\n");
	fclose(fp);
	fprintf(stderr, "close dev\n");
	snd_pcm_close(playback_handle);	fprintf(stderr, "ok\n");
	return 0;
}

int va_audio_keytone_play(void)
{
	int err;
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_format_t pcm_fmt;
	if(keytone_head.uBitsPerSample == 8){
		pcm_fmt = SND_PCM_FORMAT_S8;
	}else if(keytone_head.uBitsPerSample == 16){
		pcm_fmt = SND_PCM_FORMAT_S16_LE;
	}else{
		printf("uBitsPerSample not support!\n");
		return -1;
	}
	if((err = snd_pcm_open(&playback_handle,  "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0){
		fprintf(stderr, "cannot open audio device record.pcm (%s)\n",  snd_strerror(err));
		return -1;
	}
	if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0){
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0){
		fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0){
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, pcm_fmt)) < 0){
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, keytone_head.uSampleRate, 0)) < 0){
		fprintf(stderr , "cannot set sample rate (%s)\n", snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, keytone_head.uChannels)) <0){
		fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
		goto play_wav_out;
	}
	if((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0){
		fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
		goto play_wav_out;
	}
	snd_pcm_hw_params_free(hw_params);
	snd_pcm_writei(playback_handle, keytone_buff , keytone_len);
play_wav_out:
	snd_pcm_close(playback_handle);
	return 0;
}

int va_audio_keytone_init(const char * partname)
{
	int ret = 0;
	FILE *fp = NULL;
	int r_size;
	int f_size;
	fprintf(stderr, "open file : %s\n", partname);
	fp = fopen(partname, "r");
	if (fp == NULL) {
		fprintf(stderr, "open test pcm file err\n");
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	keytone_buff = malloc(f_size);
	if(!keytone_buff){
		printf("keytone_buff malloc failed!\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	r_size = fread(&keytone_head, 1, sizeof(wave_header_t), fp);
	if(r_size != sizeof(wave_header_t)){
		printf("read 111 wav file head error!\n");
		fclose(fp);
		free(keytone_buff);
		keytone_buff = NULL;
		return -1;
	}
	printf("read wav file head success \n");
	keytone_len = fread(keytone_buff, 1, f_size, fp);
	if(keytone_len <= 0){
		printf("read 222 wav file head error!\n");
		fclose(fp);
		free(keytone_buff);
		keytone_buff = NULL;
		return -1;
	}
	fclose(fp);
	return 0;
}

int va_audio_keytone_exit(void)
{
	if(keytone_buff){
		free(keytone_buff);
		keytone_buff = NULL;
	}
	return 0;
}