#ifndef __MUSIC_LRC_H__
#define __MUSIC_LRC_H__

#define STRLEN 256
#define SHOW_LINE_NUM 5

typedef struct music_lrc_info{
	char lang[64];
	char title[128];
	char artist[128];
	char albumArtist[64];
	char lrc_author[64];
	unsigned int time_list[256];
	unsigned int list_len;
	char text_list[256][STRLEN];

	char *path;
	char *lrc_cache;
	int lrc_size;
	char *start; //start address
	char *end;
	int line_len;
}music_lrc_info;

char *parse_lrc_get_tag(char *p, music_lrc_info *lrc);
int parse_lrc_get_time(char *line, int *time, int No);
int parse_lrc_get_text(char *line, char *text);
int parse_lrc_get_time_list(music_lrc_info *lrc);
unsigned int parse_lrc_get_current_line(int time, music_lrc_info *lrc);
unsigned int parse_lrc_get_next_line(int line, music_lrc_info *lrc);

#endif
