#ifndef APP_HANDLER_H
#define APP_HANDLER_H
#include <pthread.h>
#include "shared/mainloop.h"
#include "RTKMeshTypes.h"
#include "mesh_api.h"
#include "generic_on_off.h"
#include "light_lightness.h"
#include "shared/queue.h"

#include "app_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_mesh_event_cb(MESH_BT_EVENT_T *ag_event);

void app_mesh_event_handler(MESH_BT_EVENT_T *ag_event);

int32_t app_light_lightness_client_data_handler(MESH_MODEL_LLS_RECV_DATA_T *p);
int32_t generic_on_off_client_data_handler(MESH_MODEL_GOOS_RECV_DATA_T *p);
int32_t light_ctl_client_data_handler(MESH_MODEL_LC_RECV_DATA_T *p);
int32_t config_client_data_handler(MESH_MODEL_LC_RECV_DATA_T *p);
int32_t light_hsl_client_data_handler(MESH_MODEL_RECV_DATA_T *p);

int32_t app_light_lightness_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,void *pargs);
int32_t generic_on_off_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,void *pargs);
int32_t light_ctl_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs);
int32_t config_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs);
bool app_generic_on_off_client_receive(mesh_msg_p pmesh_msg);
int32_t light_hsl_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs);
#ifdef __cplusplus
}
#endif
#endif