
#ifndef APP_CMD_H
#define APP_CMD_H

#include <sys/timerfd.h>

#include "RTKMeshTypes.h"
#include "user_cmd_parse.h"
#include "app_handler.h"
#include "generic_on_off.h"
#include "light_lightness.h"

#include "rtkmesh_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  struct {
	const char *cmd;
	const char *arg;
	void (*func) (user_cmd_parse_value_t *p);
	const char *desc;
	char * (*gen) (const char *text, int state);
	void (*disp) (char **matches, int num_matches, int max_length);
} cmd_list_t;

extern cmd_list_t cmd_table[85];

extern int waitForStateTimer_fd;
extern int OnOffTestTimer_fd;
extern uint16_t element_index_record;
extern uint16_t dst_record;
extern uint16_t appkeyindex_g_record;
extern int inquiry_retry_count;
extern uint32_t timer_interval;
extern uint32_t goo_send_count;
extern uint32_t goo_recv_count;
extern uint32_t need_one_response;

extern int status_get_retry;

extern prov_auth_method_t auth_method_record;
extern prov_auth_action_t auth_action_record;
extern prov_auth_size_t   auth_size_record;
extern prov_auth_value_type_t auth_value_type_record;

void cmd_goo_set_for_get_status(user_cmd_parse_value_t *p);

#ifdef __cplusplus
}
#endif
#endif

