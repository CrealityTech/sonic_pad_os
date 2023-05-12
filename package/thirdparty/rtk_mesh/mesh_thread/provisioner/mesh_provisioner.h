#ifndef _MESH_DEVICE_
#define _MESH_DEVICE_

#include "app_msg.h"
#include "RTKMeshTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  NUM_VENDOR_MODEL_INFO 128

typedef struct {
    uint16_t model_id;
    uint16_t company_id;
} mesh_model_id_t;

typedef struct _vendor_model_entry_t
{
    struct _vendor_model_entry_t *pnext;
	uint8_t element_index;
    mesh_model_id_t model_id;
    model_receive_pf recv_cb;
} vendor_model_entry_t, *vendor_model_entry_p;

typedef struct
{
    vendor_model_entry_t vendor_model_entry[NUM_VENDOR_MODEL_INFO];
    plt_list_t vendor_model_free;
    plt_list_t vendor_model_used;
} vendor_model_queue_t, *vendor_model_queue_p;

extern vendor_model_queue_t vendor_model_queue;
extern uint8_t element_num;

void rtkmesh_element_init(uint8_t elementNum);
void vendor_model_queue_init();
int32_t vendor_model_add(uint8_t element_index, mesh_model_id_t model_id, model_receive_pf recv_cb);
void vendor_model_deinit(mesh_model_info_p pmodel_info);
void  vendor_model_reg(void );
void mesh_stack_init(uint16_t local_unicast_addr);
void app_handle_io_msg(T_IO_MSG io_msg);
#ifdef __cplusplus
}
#endif

#endif
