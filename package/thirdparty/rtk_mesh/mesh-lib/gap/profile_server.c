#include "profile_server.h"


bool server_add_service(T_SERVER_ID *p_out_service_id, uint8_t *p_database, uint16_t length,
                        const T_FUN_GATT_SERVICE_CBS srv_cbs)
{
	return false;
}

bool server_send_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                      uint8_t *p_data, uint16_t data_len, T_GATT_PDU_TYPE type) 
{
	return false;
}
