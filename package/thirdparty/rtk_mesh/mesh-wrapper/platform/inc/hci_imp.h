#ifndef _HCI_IMP_H_
#define _HCI_IMP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*hci_callback_func_t)(const void *data, uint8_t size,
							void *user_data);
typedef void (*hci_destroy_func_t)(void *user_data);

#ifdef MESH_ANDROID
int hci_socket_connect();
void hci_socket_close(void );
#endif
bool hci_init(void *param);
bool hci_event_register(uint8_t event, hci_callback_func_t callback, 
						void *user_data, hci_destroy_func_t destroy);
bool hci_event_unregister(uint8_t event );
unsigned int hci_cmd_send( uint16_t opcode,
			   const void *param, uint8_t size,
			   hci_callback_func_t callback, void *user_data,
			   hci_destroy_func_t destroy);
bool hci_deinit(void );
#ifdef __cplusplus
}
#endif

#endif /* _HCI_IMP_H_ */
