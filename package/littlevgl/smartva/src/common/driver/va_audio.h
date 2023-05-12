#ifndef _VA_AUDIO_H_
#define _VA_AUDIO_H_

int va_audio_set_volume(int volume);

int va_audio_get_volume(void);

int va_audio_set_mic_AMP_gain_value(int val);

int va_audio_set_mic_mixer_value(int val);

int va_audio_get_mic_AMP_gain_value(int val);

int va_audio_get_mic_mixer_value(int val);

int va_audio_init(void);

int va_audio_keytone_play(void);
int va_audio_keytone_init(const char * partname);
int va_audio_keytone_exit(void);
int va_audio_play_wav_music(const char * partname);

#endif
