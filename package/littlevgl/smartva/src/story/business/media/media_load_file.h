#ifndef _READ_FILE_LIST_H_
#define _READ_FILE_LIST_H_

#include "rat_common.h"

typedef struct media_file_list_t
{
	HRAT				media_hrat;
	rat_media_type_t	media_type;
	int 				total_num;
	int 				play_index;
}media_file_list_t;

#define MEDIA_FILE_LIST_MAX_NUM 	200

typedef enum _char_enc
{
    ENCODE_ISO_8859_1 = 0,
    ENCODE_USC_2 = 1,
    ENCODE_UTF_16_BE = 2,
    ENCODE_UTF_8 = 3,
    ENCODE_ASCII = 4,
	ENCODE_UNICODE = 5,
	ENCODE_UTF_32 = 6,
	ENCODE_GB2312 = 7,
    ENCODE_DEFAULT = ENCODE_UTF_8,
}char_enc;



char_enc get_file_lang(char *file);

media_file_list_t *media_load_file(rat_media_type_t media_type, char *path);
int media_unload_file(void);
char *media_get_file_path(int index);
char *media_get_path_to_name(char *path);
void media_set_play_file_index(int index);
unsigned int media_get_play_file_index(void);
unsigned int media_get_file_total_num(void);
char *media_get_next_file_path(void);
char *media_get_last_file_path(void);
void media_move_cursor_to_first(void);

#endif
