#ifndef  _MIDDLE_WARE_H_
#define  _MIDDLE_WARE_H_

#include "player_int.h"
#include "common.h"
#include "display.h"
#include "power.h"
#include "dbList.h"
#include "sunxi_display_v2.h"
#include "resource.h"

#define MID_DEBUG
#define MID_WARN
#define MID_ERROR

#ifdef MID_DEBUG
#define mid_debug(fmt, ...) \
	do {fprintf(stdout, "[%s %d]dbg: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define mid_debug(fmt, ...) \
	do {;} while (0)
#endif

#ifdef MID_WARN
#define mid_warn(fmt, ...) \
	do {fprintf(stdout, "[%s %d]wrn: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define mid_warn(fmt, ...) \
	do {;} while (0)
#endif

#ifdef MID_ERROR
#define mid_error(fmt, ...) \
	do {fprintf(stdout, "[%s %d]err: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define mid_error(fmt, ...) \
	do {;} while (0)
#endif

#endif
