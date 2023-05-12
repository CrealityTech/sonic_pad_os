#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
/*#include <tgmath.h>*/

#include "mjson.h"

#ifndef u32
typedef unsigned int u32;
#endif
#ifndef __s32
typedef int __s32;
#endif

static char *playback_pcm_device;
static char *capture_pcm_device;
static char *wav_file;
static int capture_channels;
static int capture_rate;

#define FFTSIZE  	(512)
#define FSFREQ 		(48000)
#define Q23 		(8388608)

static double fVpp = 0.0;
float snr0 = 0.0;
float snr1 = 0.0;
float snr2 = 0.0;
float rate3 = 0.0;
float rate4 = 0.0;

int fVpp_judeg(void)
{
	printf("fVpp=%d mV\r\n", (u32)(fVpp*1000));
	printf("snr0=%d/10\r\n", (u32)(snr0*10));
	printf("snr1=%d/10\r\n", (u32)(snr1*10));
	printf("snr2=%d/10\r\n", (u32)(snr2*10));
	printf("rate3=%d/10\r\n", (u32)(rate3*10));
	printf("rate4=%d/10\r\n", (u32)(rate4*10));

	if((fVpp < 1.5) || (fVpp > 1.7))
	{
		return -1;
	}
	if((snr0 < 45) || (snr1 < 55) || (snr2 < 60))
	{
		return -2;
	}
	if((rate3 < 1) || (rate4 < 1))
	{
		return -3;
	}

	return 0;
}

static double goertzel(__s32 samples[], double freq, int N)
{
	double s_prev = 0.0;
    double s_prev2 = 0.0;
    double coeff, normalizedfreq, power, s;
	double cosval[6] = {1.9957178,   1.9828897,   1.9318517,   1.847759,   1.9832297,   1.9825463};
	int i;

	normalizedfreq = freq / FSFREQ;

	if(freq == 500)
		coeff = cosval[0];
	else if(freq == 1000)
		coeff = cosval[1];
	else if(freq == 2000)
		coeff = cosval[2];
	else if(freq == 3000)
		coeff = cosval[3];
	else if(freq == 990)
		coeff = cosval[4];
	else if(freq == 1010)
		coeff = cosval[5];
	else
		coeff = 0;

	for(i = 0; i < N; i++)
	{
		s = samples[i] + coeff * s_prev - s_prev2;
		s_prev2 = s_prev;
		s_prev = s;
	}
	power = s_prev2*s_prev2 + s_prev*s_prev - coeff * s_prev * s_prev2;
	return power;
}

static int goertzel_convert(__s32 *fSinedata)
{
	int i, ret=0;
//	static double fVpp=0.0;
//	static float snr0 = 0.0, snr1 = 0.0, snr2 = 0.0, rate3 = 0.0, rate4 = 0.0;
	float frequency_row[6] = {500, 1000, 2000, 3000, 990, 1010};
	float magnitude_row[6];

	__s32 fRmax,fRmin;

	//Get the max and min sine data
	fRmax = 0x80000000;
	fRmin = 0x7FFF0000;
	for(i = 1; i < FFTSIZE; i++)
	{
		if(fSinedata[i] > fRmax)
			fRmax = fSinedata[i];
		if(fSinedata[i] < fRmin)
			fRmin = fSinedata[i];
	}
	fVpp = (double)fRmax/Q23 - (double)fRmin/Q23;
	/*printf("fRmax:%d, fRmin:%d, Q23=%u, fVpp=%0.6f\n", fRmax, fRmin, Q23, fVpp);*/

	//Get the Freq and THD
	for(i = 0; i < 6; i++)
	{
		magnitude_row[i] = goertzel(fSinedata, frequency_row[i], FFTSIZE);
	}

	snr0 = 20*log10(magnitude_row[1] / magnitude_row[0]);
	snr1 = 20*log10(magnitude_row[1] / magnitude_row[2]);
	snr2 = 20*log10(magnitude_row[1] / magnitude_row[3]);
	rate3 = magnitude_row[1] / magnitude_row[4];
	rate4 = magnitude_row[1] / magnitude_row[5];

	return ret;
}

static int audio_data_verify(int *data)
{
	int ret, i;
	for (i = 0; i < FFTSIZE; i++) {
		data[i] >>= 8;
#if 0
		printf(" 0x%06X", data[i]);
		if ((i+1) % 16 == 0)
			printf("\n");
#endif
	}

	ret = goertzel_convert(data);
	ret = fVpp_judeg();

	return ret;
}

static void audio_parse_configs(void)
{
	playback_pcm_device = mjson_fetch_string(
		"/base/audio/playback_and_capture_autotest/playback_pcm_device");
	capture_pcm_device = mjson_fetch_string(
		"/base/audio/playback_and_capture_autotest/capture_pcm_device");
	wav_file = mjson_fetch_string(
		"/base/audio/playback_and_capture_autotest/wav_file");
	capture_channels = mjson_fetch_int(
		"/base/audio/playback_and_capture_autotest/capture_channels");
}

int main(int argc, char *argv[])
{
	char cmd[512];
	const char *raw_data = "/tmp/record.wav";
	int fd, ret, i, j;
	char *data;
	int **mono_data;
	unsigned int frames_bytes;
	unsigned int data_size, format_bytes = 32 / 8;

	audio_parse_configs();

	system("echo 0x7032004 0x18383 > /sys/class/sunxi_dump/write");
	system("amixer -q cset name='Speaker volume' 31");
	system("amixer -q cset name='MIC1 gain volume' 16");
	system("amixer -q cset name='MIC2 gain volume' 16");
	system("amixer -q cset name='MIC3 gain volume' 16");
	system("amixer -q cset name='MIC4 gain volume' 16");
	system("amixer -q cset name='MIC5 gain volume' 16");

	/* playback 1 seconds */
	snprintf(cmd, sizeof(cmd), "aplay -D%s -f S24_LE -r 48000 -c 2 -t raw %s -d 1 --period-size 1024 --buffer-size 4096 &", playback_pcm_device, wav_file);
	system(cmd);
	/*printf("[%s] line:%d cmd:%s\n", __func__, __LINE__, cmd);*/
	/* capture 1 seconds */
	snprintf(cmd, sizeof(cmd), "arecord -Dhw:audiocodec -f S24_LE -r 48000 -c %d -d 1 --period-size 1024 --buffer-size 4096 %s",
		capture_channels, raw_data);
	system(cmd);
	/*printf("[%s] line:%d cmd:%s\n", __func__, __LINE__, cmd);*/

	frames_bytes = capture_channels * format_bytes;
	data_size = frames_bytes * FFTSIZE;

	data = malloc(data_size);
	if (!data) {
		printf("no memory\n");
		return -ENOMEM;
	}
	mono_data = calloc(capture_channels, sizeof(int *));
	if (!mono_data) {
		printf("no memory\n");
		ret = -ENOMEM;
		goto fail;	}
	fd = open(raw_data, O_RDONLY);
	if (fd < 0) {
		printf("%s isn't exist.\n", raw_data);
		ret = -1;
		goto fail;
	}

	/* ignore 24000 sample, 24000/48000=500ms */
	lseek(fd, frames_bytes * 24000 + 44, SEEK_SET);
	read(fd, data, data_size);
	close(fd);

	for (i = 0; i < capture_channels; i++) {
		mono_data[i] = malloc(data_size/capture_channels);
#if 1
		for (j = 0; j < FFTSIZE; j++) {
			int index = j * frames_bytes + i * format_bytes;
			memcpy(&mono_data[i][j], &data[index], sizeof(int));
		}
#else
		int rawFd = open("/tmp/buf0.pcm", O_RDONLY);
		read(rawFd, mono_data[i], data_size/capture_channels);
		close(rawFd);
#endif
	}
	for (i = 0; i < capture_channels; i++) {
		printf("MIC%d test\n", i + 1);
		ret = audio_data_verify(mono_data[i]);
		if (ret != 0) {
			printf("auido data verify failed\n");
			goto fail;
		} else {
			printf("audio data veriy successed.\n");
		}
	}
fail:
	if (mono_data) {
		for (i = 0; i < capture_channels; i++) {
			if (mono_data[i])
				free(mono_data[i]);
		}
		free(mono_data);
	}
	if (data)
		free(data);
	system("echo 0x7032004 0x1A0A0> /sys/class/sunxi_dump/write");
	return ret;
}
