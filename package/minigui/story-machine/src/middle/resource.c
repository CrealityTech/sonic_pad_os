#include "resource.h"

fontLib *fontlib = NULL;
LangInfo langInfo;

PLOGFONT GetLogFont(enum FontID id)
{
	PLOGFONT mCurLogFont;
	switch (id) {
	case ID_FONT_SIMSUN_20:
		mCurLogFont = fontlib->mFont_SimSun20;
		break;
	case ID_FONT_SIMSUN_25:
		mCurLogFont = fontlib->mFont_SimSun25;
		break;
	case ID_FONT_SIMSUN_30:
		mCurLogFont = fontlib->mFont_SimSun30;
		break;
	case ID_FONT_SIMSUN_40:
		mCurLogFont = fontlib->mFont_SimSun40;
		break;
	case ID_FONT_SIMSUN_50:
		mCurLogFont = fontlib->mFont_SimSun50;
		break;
	case ID_FONT_SIMSUN_GB_20:
		mCurLogFont = fontlib->mFont_SimSun_gb20;
		break;
	case ID_FONT_SIMSUN_GB_25:
		mCurLogFont = fontlib->mFont_SimSun_gb25;
		break;
	case ID_FONT_SIMSUN_GB_30:
		mCurLogFont = fontlib->mFont_SimSun_gb30;
		break;
	case ID_FONT_SIMSUN_GB_40:
		mCurLogFont = fontlib->mFont_SimSun_gb40;
		break;
	case ID_FONT_SIMSUN_GB_50:
		mCurLogFont = fontlib->mFont_SimSun_gb50;
		break;
	default:
		printf("invalid Font ID index: %d\n", id);
		break;
	}
	return mCurLogFont;
}

void InitLogFont()
{
	fontlib = (fontLib *)malloc(sizeof(fontLib));

	fontlib->mFont_SimSun20 = CreateLogFont("ttf", "simsun", "UTF-8",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 20, 0);

	printf("fontlib->mFont_SimSun20=ox%x\n", fontlib->mFont_SimSun20);

	fontlib->mFont_SimSun25 = CreateLogFont("ttf", "simsun", "UTF-8",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 25, 0);

	fontlib->mFont_SimSun30 = CreateLogFont("ttf", "simsun", "UTF-8",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 30, 0);

	fontlib->mFont_SimSun40 = CreateLogFont("ttf", "simsun", "UTF-8",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 40, 0);

	fontlib->mFont_SimSun50 = CreateLogFont("ttf", "simsun", "UTF-8",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 50, 0);

	#if 1
	fontlib->mFont_SimSun_gb20 = CreateLogFont("ttf", "simsun", "GB2312",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 20, 0);

	fontlib->mFont_SimSun_gb25 = CreateLogFont("ttf", "simsun", "GB2312",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 25, 0);

	fontlib->mFont_SimSun_gb30 = CreateLogFont("ttf", "simsun", "GB2312",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 30, 0);

	fontlib->mFont_SimSun_gb40 = CreateLogFont("ttf", "simsun", "GB2312",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 40, 0);

	fontlib->mFont_SimSun_gb50 = CreateLogFont("ttf", "simsun", "GB2312",
	FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
	FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE,
	FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 50, 0);

	#endif
}

void UninitLogFont(void)
{
	if(fontlib->mFont_SimSun20)
	{
		DestroyLogFont(fontlib->mFont_SimSun20);
		fontlib->mFont_SimSun20 = NULL;
	}

	if(fontlib->mFont_SimSun25)
	{
		DestroyLogFont(fontlib->mFont_SimSun25);
		fontlib->mFont_SimSun25 = NULL;
	}

	if(fontlib->mFont_SimSun30)
	{
		DestroyLogFont(fontlib->mFont_SimSun30);
		fontlib->mFont_SimSun30 = NULL;
	}

	if(fontlib->mFont_SimSun40)
	{
		DestroyLogFont(fontlib->mFont_SimSun40);
		fontlib->mFont_SimSun40 = NULL;
	}

	if(fontlib->mFont_SimSun50)
	{
		DestroyLogFont(fontlib->mFont_SimSun50);
		fontlib->mFont_SimSun50 = NULL;
	}
/*GB2312*/
	if(fontlib->mFont_SimSun_gb20)
	{
		DestroyLogFont(fontlib->mFont_SimSun_gb20);
		fontlib->mFont_SimSun_gb20 = NULL;
	}

	if(fontlib->mFont_SimSun_gb25)
	{
		DestroyLogFont(fontlib->mFont_SimSun_gb25);
		fontlib->mFont_SimSun_gb25 = NULL;
	}

	if(fontlib->mFont_SimSun_gb30)
	{
		DestroyLogFont(fontlib->mFont_SimSun_gb30);
		fontlib->mFont_SimSun_gb30 = NULL;
	}

	if(fontlib->mFont_SimSun_gb40)
	{
		DestroyLogFont(fontlib->mFont_SimSun_gb40);
		fontlib->mFont_SimSun_gb40 = NULL;
	}

	if(fontlib->mFont_SimSun_gb50)
	{
		DestroyLogFont(fontlib->mFont_SimSun_gb50);
		fontlib->mFont_SimSun_gb50 = NULL;
	}
	free(fontlib);
	fontlib = NULL;

}

static ssize_t  __getline(char **lineptr, ssize_t *n, FILE *stream)
{
	int count=0;
	int buf;

	if(*lineptr == NULL) {
		*n=256;
		*lineptr = (char*)malloc(*n);
	}

	if(( buf = fgetc(stream) ) == EOF ) {
		return -1;
	}

	do
	{
		if(buf=='\n') {
			count += 1;
			break;
		}

		count++;

		*(*lineptr+count-1) = buf;
		*(*lineptr+count) = '\0';

		if(*n <= count)
		{
			printf("count=%d over *n=%d\n", count, (int)*n);
			return -1;
		}

		buf = fgetc(stream);
	} while( buf != EOF);

	return count;
}


int loadLabelFromLangFile(char *langFile)
{
	FILE *fp;
	char *line = NULL;
	ssize_t len = 0;
	ssize_t rel_len = 0;
	int i = 0;

	fp = fopen(langFile, "r");
	if (fp == NULL) {
		printf("open file %s failed\n", langFile);
		return -1;
	}

	i = 0;
	langInfo.label[i] = (char *)malloc(strlen(langFile)+1);
	memcpy((void *)langInfo.label[i], (void *)langFile, strlen(langFile)+1);
	printf("i=%d, len=%d, str=%s\n\n", i, strlen(langFile)+1, langInfo.label[i]);
	i++;
	while ((rel_len = __getline(&line, &len, fp)) != -1)
	{
		langInfo.label[i] = (char *)malloc(rel_len);
		if(langInfo.label[i] == NULL)
		{
			printf("malloc fail\n");
		}

		memcpy((void *)langInfo.label[i], (void *)line, rel_len);
		langInfo.label[i][rel_len-2] = '\0';
		langInfo.label[i][rel_len-1] = '\0';
		printf("i=%d, len=%d, str=%s\n\n", i, (int)rel_len, langInfo.label[i]);

		i++;
	}

	free(line);
	fclose(fp);

	return 0;
}


int initLabel(void)
{
	char dataFile[64];
	if(langInfo.lang == LANG_ERR) {
		printf("lang is no initialized\n");
		return -1;
	}

	if(langInfo.lang == LANG_CN)
	{
		memcpy((void*)dataFile, (void *)LANG_FILE_CN, sizeof(LANG_FILE_CN));
	}
	else if(langInfo.lang == LANG_EN)
		memcpy((void*)dataFile, (void *)LANG_FILE_EN, sizeof(LANG_FILE_EN));
	else if(langInfo.lang == LANG_TW)
		memcpy((void*)dataFile, (void *)LANG_FILE_TW, sizeof(LANG_FILE_TW));
	else if(langInfo.lang == LANG_JPN)
		memcpy((void*)dataFile, (void *)LANG_FILE_JPN, sizeof(LANG_FILE_JPN));
	else if(langInfo.lang == LANG_KR)
		memcpy((void*)dataFile, (void *)LANG_FILE_KR, sizeof(LANG_FILE_KR));
	else if(langInfo.lang == LANG_RS)
		memcpy((void*)dataFile, (void *)LANG_FILE_RS, sizeof(LANG_FILE_RS));
	else {
		printf("invalid lang %d\n", langInfo.lang);
		return -1;
	}

	printf("lang=%d, dataFile=%s\n", langInfo.lang, dataFile);

	if(loadLabelFromLangFile(dataFile) < 0) {
		printf("load label from %s failed\n", dataFile);
		return -1;
	}

	return 0;
}

int UninitLabel(void)
{
	int i;
	for(i=0; i<LANG_LABEL_MAX; i++)
	{
		if(langInfo.label[i] != NULL)
		{
			free(langInfo.label[i]);
			langInfo.label[i] = NULL;
		}
	}
}

void InitLangAndLable(void)
{
	langInfo.lang = LANG_CN;
	initLabel();
}

void UninitLangAndLable(void)
{
	UninitLabel();
	langInfo.lang = LANG_ERR;
}

void UpdateLangAndLable(LANGUAGE langValue)
{
	LANGUAGE newLang = LANG_ERR;

	if(langValue == LANG_TW)
		newLang = LANG_TW;
	else if(langValue == LANG_CN)
		newLang = LANG_CN;
	else if(langValue == LANG_EN)
		newLang = LANG_EN;
	else if(langValue == LANG_JPN)
		newLang = LANG_JPN;
	else if(langValue == LANG_KR)
		newLang = LANG_KR;
	else if(langValue == LANG_RS)
		newLang = LANG_RS;
	else{
		newLang = LANG_ERR;
		printf("invalide langValue=%d\n", langValue);
		return;
	}

	if(newLang != langInfo.lang)
	{
		langInfo.lang = newLang;
	}

	UninitLabel();
	initLabel();
}

LANGUAGE getLanguage(void)
{
	return langInfo.lang;
}

/*
function: get lang label's string

example:
\res\lang\zh-CN.bin下内容为
10 ===== 主界面=====
11 电影
12 音乐
13 相片
5 电影
getLabel(11) = "电影"
getLabel(12) = "音乐"
*/
const char* getLabel(unsigned int labelIndex)
{
	if(labelIndex < LANG_LABEL_MAX)
	{
		return (const char*)langInfo.label[labelIndex];
	}
	else
	{
		printf("invalide label Index: %d, size is %zd\n", (int)labelIndex, LANG_LABEL_MAX);
		return NULL;
	}
}
