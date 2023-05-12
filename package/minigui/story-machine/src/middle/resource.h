#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

//#define PHOTO_PATH		"/etc/photo/"
//#define LANG_FILE_CN	"/etc/lang/zh-CN.bin"

#define PHOTO_PATH		"/usr/res/photo/"
#define LANG_FILE_CN	"/usr/res/lang/zh-CN.bin"
#define LANG_FILE_EN	"/usr/res/lang/en.bin"
#define LANG_FILE_TW	"/usr/res/lang/zh-TW.bin"
#define LANG_FILE_JPN	"/usr/res/lang/jpn.bin"
#define LANG_FILE_KR	"/usr/res/lang/korean.bin"
#define LANG_FILE_RS	"/usr/res/lang/russian.bin"

enum LangLabelID
{
	LANG_LABEL_HOME = 10,
	LANG_LABEL_MAX = 200,
};

typedef struct {
	PLOGFONT mFont_SimSun20;
	PLOGFONT mFont_SimSun25;
	PLOGFONT mFont_SimSun30;
	PLOGFONT mFont_SimSun40;
	PLOGFONT mFont_SimSun50;

	PLOGFONT mFont_SimSun_gb20;
	PLOGFONT mFont_SimSun_gb25;
	PLOGFONT mFont_SimSun_gb30;
	PLOGFONT mFont_SimSun_gb40;
	PLOGFONT mFont_SimSun_gb50;
}fontLib;

enum FontID {
	ID_FONT_SIMSUN_20,
	ID_FONT_SIMSUN_25,
	ID_FONT_SIMSUN_30,
	ID_FONT_SIMSUN_40,
	ID_FONT_SIMSUN_50,

	ID_FONT_SIMSUN_GB_20,
	ID_FONT_SIMSUN_GB_25,
	ID_FONT_SIMSUN_GB_30,
	ID_FONT_SIMSUN_GB_40,
	ID_FONT_SIMSUN_GB_50,
};

typedef enum {
	LANG_CN = 0,
	LANG_EN = 1,
	LANG_TW = 2,
	LANG_JPN = 3,
	LANG_KR = 4,
	LANG_RS = 5,
	LANG_ERR
}LANGUAGE;

typedef struct {
	LANGUAGE lang;
	char *label[LANG_LABEL_MAX];
}LangInfo;


PLOGFONT GetLogFont(enum FontID id);
void InitLogFont(void);
void UninitLogFont(void);

void InitLangAndLable(void);
void UpdateLangAndLable(LANGUAGE langValue);
void UninitLangAndLable(void);
LANGUAGE getLanguage(void);
const char* getLabel(unsigned int labelIndex);

#endif
