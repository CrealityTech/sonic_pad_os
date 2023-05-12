#ifndef __BT_CMD_H__
#define __BT_CMD_H__

typedef struct {
	const char *cmd;
	const char *arg;
	void (*func) (int argc, char *args[]);
	const char *desc;
} cmd_tbl_t;

extern dev_list_t *bonded_devices;
extern dev_list_t *discovered_devices;
extern int32_t __main_terminated;
extern cmd_tbl_t cmd_table[];

#endif
