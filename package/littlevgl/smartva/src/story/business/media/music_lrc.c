#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "music_lrc.h"
#include "debug.h"

static char *parse_lrc_get_time_str(char *p, char *buf)
{
	char *left = NULL;
	char *right = NULL;
	int len;

	left = strchr(p,'[');
	if(left == NULL){
		return p;
	}
	right = strchr(left,']');
	if(right == NULL){
		return p;
	}

	len = right - left -1;
	if(len == 8)
		memcpy(buf, left+1, 8);

	return (char *)((unsigned int)p + (unsigned int)right - (unsigned int)left + 1);
}

static char *parse_lrc_get_tag_text(char *p, char *text, char *tag)
{
	char *pos;
	int len;
	char *right = NULL;

	if(!p){
		return NULL;
	}
	pos = strstr(p, tag);
	if(pos == NULL){
		return p;
	}

	right = strchr(pos,']');
	if(right == NULL){
		return p;
	}

	len = right - pos;
	memcpy(text, pos + strlen(tag), len - strlen(tag));
	return pos+len+1+2;
}

char *parse_lrc_get_tag(char *p, music_lrc_info *lrc)
{
	char *next;
	next = parse_lrc_get_tag_text(p, lrc->lang, "lg:");
	next = parse_lrc_get_tag_text(next, lrc->title, "ti:");
	next = parse_lrc_get_tag_text(next, lrc->artist, "ar:");
	next = parse_lrc_get_tag_text(next, lrc->albumArtist, "al:");
	next = parse_lrc_get_tag_text(next, lrc->lrc_author, "by:");
	next = parse_lrc_get_tag_text(next, lrc->lrc_author, "offset:");
#if 1
	debug_info("lang =%s",lrc->lang);
	debug_info("title =%s",lrc->title);
	debug_info("artist =%s",lrc->artist);
	debug_info("albumArtist =%s",lrc->albumArtist);
	debug_info("lrc_author =%s",lrc->lrc_author);
#endif
	if(next == p){
		return NULL;
	}
	return next;
}

int get_line_text(char *p, char *end, char *buf, int buf_len)
{

	int i = 0;
	int len = 0;
	char *next;
	char line_end[2] = {0x0D, 0x0A};

	memset(buf, 0, buf_len);
	unsigned int size = (unsigned int)end - (unsigned int)p;

	//debug_info("size =%d", size);

	for(i = 0;  i < size; i++){
		if(p[i]==line_end[0] && p[i+1]==line_end[1]){
			break;
		}
		buf[i] = p[i];
		len++;
	}
	return len+2;
}


int parse_lrc_get_time(char *line, int *time, int No)
{
#define LINE_TIME_NUM 8
	char *next;
	char str_time[LINE_TIME_NUM][16] = {0};
	int i;
	int ms, second, minute;
	char word[2];
	int num;

	next = line;

	num = 0;
	while(1){
		next = parse_lrc_get_time_str(next, str_time[num]);
		if(next == line){
			return -1;
		}
		if(next[0] != '[')
			break;
		//debug_info("%s", str_time[num]);
		num++;

		if(num >= LINE_TIME_NUM){
			debug_err("format error");
			return -1;
		}
	}

	if(No > num){
		return -1;
	}
	char *str = str_time[num - No];
	word[0] = str[6];
	word[1] = str[7];
	ms = atoi(word);
	word[0] = str[3];
	word[1] = str[4];
	second = atoi(word);
	word[0] = str[0];
	word[1] = str[1];
	minute = atoi(word);
	*time = ms*10 + second*1000 + minute * 60 * 1000;

	 return 0;
}



int parse_lrc_get_text(char *line,char *text)
{
	char *left = NULL;
	char *right = NULL;
	char *next = NULL;
	int len;
	int i;

	next = line;
	while(1){
		if(next[0] != '['){
			break;
		}
		left = strchr(next, '[');
		if(!left){
			return -1;
		}
		right = strchr(next, ']');
		if(!right){
			return -1;
		}
		next = right + 1;
	}
	sprintf(text, "%s\n", next);
	len = strlen(next);
	return len;
}


int parse_lrc_get_time_list(music_lrc_info *lrc)
{
	char line_buf[512];
	int line_len;
	char *next;
	int i = 0;
	int No = 0;
	int ret;
	int count = 0;
	int status = 0;
	int time;

	next = lrc->start;

	while(1){
		line_len = get_line_text(next, lrc->end, line_buf, sizeof(line_buf));
		next = next + line_len;
		//debug_info("line_buf = %s %d", line_buf, line_len);
		ret = parse_lrc_get_time(line_buf, &time, No);
		if(!ret){
			if(time != 0){
				lrc->time_list[i] = time;
				parse_lrc_get_text(line_buf, lrc->text_list[i]);
			}
			count++;
			i++;
		}
		//debug_info("next=%p %p %d %d", next, p+len, time_list[i], count);

		if(next >= lrc->end){
			if(!count){
				return i;
			}
			No++;
			next = lrc->start;
			count = 0;
		}

	}

}

unsigned int parse_lrc_get_current_line(int time, music_lrc_info *lrc)
{

	int i, j;
	unsigned int curr_line = 0 ;
	int cha0, cha1;

	for(i = 0; i < lrc->list_len; i++){
		cha0 = time - lrc->time_list[i];
		if(cha0 > 0){
			curr_line = i;
			break;
		}
	}

	for(j = i; j < lrc->list_len - i; j++){
		cha1 = time - lrc->time_list[j];
		if(cha1 > 0 && cha1 < cha0){
			cha0 = cha1;
			curr_line = j;
		}
	}
	return curr_line;

}

unsigned int parse_lrc_get_next_line(int line, music_lrc_info *lrc)
{

	int i, j;
	unsigned int curr_line = 0 ;
	int cha0, cha1;
	int time = lrc->time_list[line];

	for(i = 0; i < lrc->list_len; i++){
		cha0 = lrc->time_list[i] - time;
		if(cha0 > 0){
			curr_line = i;
			break;
		}
	}

	for(j = i; j < lrc->list_len - i; j++){
		cha1 = lrc->time_list[j] - time;
		if(cha1 > 0 && cha1 < cha0){
			cha0 = cha1;
			curr_line = j;
		}
	}
	return curr_line;

}
