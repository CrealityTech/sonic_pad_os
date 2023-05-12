/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     mp_cmd_parse.h
* @brief    Source file for command parse.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-08-13
* @version  v1.0
* *************************************************************************************
*/

#ifndef _MP_CMD_PARSE_H_
#define _MP_CMD_PARSE_H_

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup MP_CMD_PARSE
 * @{
 */

/**
 * @defgroup MP_Cmd_Parser_Exported_Macros MP Command Parser Exported Macros
 * @brief
 * @{
 */
/** @brief mp command max length */
#define MP_CMD_MAX_SIZE     50
/** @} */

/**
 * @defgroup MP_Cmd_Parser_Exported_Types MP Command Parser Exported Types
 * @brief
 * @{
 */

/**
 * @brief mp command execute result
 */
typedef enum
{
    MP_CMD_RESULT_OK,
    MP_CMD_RESULT_ERROR,
} mp_cmd_process_result_t;

/**
 * @brief mp command process callback
 */
typedef mp_cmd_process_result_t (*mp_cmd_process_t)(uint16_t opcode, const uint8_t *data,
                                                    uint32_t len);

/**
 * @brief command table structure
 */
typedef struct
{
    uint16_t opcode;
    uint8_t payload_len;
    mp_cmd_process_t cmd_process;
} mp_cmd_table_t;
/** @} */


/**
 * @defgroup MP_Cmd_Parser_Exported_Functions MP Command Parser Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize mp command
 * @param[in] pcmd_table: user defined command table array
 * @param[in] table_len: command table length
 */
bool mp_cmd_init(const mp_cmd_table_t *pcmd_table, uint32_t table_len);

/**
 * @brief parse mp command
 * @param[in] pdata: data need to parse
 * @param[in] len: data length
 */
void mp_cmd_parse(const uint8_t *pdata, uint8_t len);
/** @} */
/** @} */

END_DECLS

#endif /* _MP_CMD_PARSE_H_ */

