/**
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef _TRACE_H_
#define _TRACE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#ifdef MESH_ANDROID
#include <utils/Log.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \defgroup    TRACE       Trace
 *
 * \brief       Defines debug trace macros for each module.
 *
 */

/**
 * trace.h
 *
 * \name    TRACE_LEVEL
 * \brief   Log Level Definition.
 * \anchor  TRACE_LEVEL
 */
/**
 * \ingroup TRACE
 */
/**@{*/
#define LEVEL_ERROR     0
#define LEVEL_WARN      1
#define LEVEL_INFO      2
#define LEVEL_TRACE     3
#define LEVEL_NUM       4
/**@}*/

/*
 * DBG_LEVEL is used to control the log printed by DBG_BUFFER().
 * -1          : Print None
 * LEVEL_ERROR : Print ERROR
 * LEVEL_WARN  : Print ERROR, WARN
 * LEVEL_INFO  : Print ERROR, WARN, INFO
 * LEVEL_TRACE : Print ERROR, WARN, INFO, TRACE
 */
#define DBG_LEVEL               LEVEL_TRACE

/* Log type definition */
typedef enum
{
    TYPE_UPPERSTACK_RESET       = 0,    /* Bee1(deprecated) */
    TYPE_UPPERSTACK_FORMAT      = 1,    /* Bee1 */
    TYPE_UPPERSTACK_MESSAGE     = 2,    /* Bee1 */
    TYPE_UPPERSTACK_BINARY      = 3,    /* Bee1 */
    TYPE_UPPERSTACK_STRING      = 4,    /* Bee1 */
    TYPE_UPPERSTACK_BDADDR1     = 5,    /* Bee1 */
    TYPE_UPPERSTACK_BDADDR2     = 6,    /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA1    = 7,    /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA2    = 8,    /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA3    = 9,    /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA4    = 10,   /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA5    = 11,   /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA6    = 12,   /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA7    = 13,   /* Bee1 */
    TYPE_UPPERSTACK_RAMDATA8    = 14,   /* Bee1 */

    TYPE_PLATFORM_DBG_DIRECT    = 16,   /* Bee1 */

    /* type 32~127 reserved for project id, e.g. bumblebee3, bee2 */
    TYPE_BUMBLEBEE3             = 32,
    TYPE_BEE2                   = 33,
    TYPE_MESH					= 34,

    /* type 128~207 reserved for 3rd party definition */

    /* type 220~251 reserved for Bee1 platform debug buffer */
} T_LOG_TYPE;


/* Log subtype definition */
typedef enum
{
    SUBTYPE_DIRECT                  = 0x00,

    SUBTYPE_FORMAT                  = 0x10,
    SUBTYPE_DOWN_MESSAGE            = 0x11,
    SUBTYPE_UP_MESSAGE              = 0x12,

    SUBTYPE_DOWN_SNOOP              = 0x20,
    SUBTYPE_UP_SNOOP                = 0x28,

    SUBTYPE_BDADDR                  = 0x30,

    SUBTYPE_STRING                  = 0x40,

    SUBTYPE_BINARY                  = 0x50,

    SUBTYPE_INDEX                   = 0x60,
} T_LOG_SUBTYPE;

/**
 * trace.h
 *
 * \name    MODULE_ID
 * \brief   Module ID definition.
 * \anchor  MODULE_ID
 */
/**
 * \ingroup TRACE
 */
/**@{*/
typedef enum
{
    /* platform modules */
    MODULE_PATCH                    = 0,
    MODULE_OS                       = 1,
    MODULE_OSIF                     = 2,
    MODULE_BOOT                     = 3,
    MODULE_DLPS                     = 4,
    MODULE_AES                      = 5,
    MODULE_FS                       = 6,

    /* device modules */
    MODULE_KEYSCAN                  = 12,
    MODULE_QDECODE                  = 13,
    MODULE_IR                       = 14,
    MODULE_3DG                      = 15,
    MODULE_ADC                      = 16,
    MODULE_GDMA                     = 17,
    MODULE_I2C                      = 18,
    MODULE_RTC                      = 19,
    MODULE_SPI                      = 20,
    MODULE_TIMER                    = 21,
    MODULE_UART                     = 22,
    MODULE_FLASH                    = 23,
    MODULE_GPIO                     = 24,
    MODULE_PINMUX                   = 25,
    MODULE_PWM                      = 26,
    MODULE_USB                      = 27,
    MODULE_SDIO                     = 28,
    MODULE_CHARGER                  = 29,
    MODULE_DSP                      = 30,
    MODULE_EFUSE                    = 31,

    /* stack modules */
    MODULE_APP                      = 48,
    MODULE_DFU                      = 49,
    MODULE_RFCOMM                   = 50,
    MODULE_PROFILE                  = 51,
    MODULE_PROTOCOL                 = 52,
    MODULE_GAP                      = 53,
    MODULE_BTE                      = 54,
    MODULE_BTIF                     = 55,
    MODULE_GATT                     = 56,
    MODULE_SMP                      = 57,
    MODULE_SDP                      = 58,
    MODULE_L2CAP                    = 59,
    MODULE_HCI                      = 60,
    MODULE_SNOOP                    = 61,
    MODULE_UPPERSTACK               = 62,
    MODULE_LOWERSTACK               = 63,

    MODULE_NUM                      = 64
} T_MODULE_ID;
/**@}*/

/**
 * trace.h
 *
 * \name    MODULE_BITMAP
 * \brief   Module bitmap definition.
 * \anchor  MODULE_BITMAP
 */
/**
 * \ingroup TRACE
 */
/**@{*/
#define MODULE_BIT_PATCH            ((uint64_t)1 << MODULE_PATCH     )
#define MODULE_BIT_OS               ((uint64_t)1 << MODULE_OS        )
#define MODULE_BIT_OSIF             ((uint64_t)1 << MODULE_OSIF      )
#define MODULE_BIT_BOOT             ((uint64_t)1 << MODULE_BOOT      )
#define MODULE_BIT_DLPS             ((uint64_t)1 << MODULE_DLPS      )
#define MODULE_BIT_AES              ((uint64_t)1 << MODULE_AES       )
#define MODULE_BIT_FS               ((uint64_t)1 << MODULE_FS        )

#define MODULE_BIT_KEYSCAN          ((uint64_t)1 << MODULE_KEYSCAN   )
#define MODULE_BIT_QDECODE          ((uint64_t)1 << MODULE_QDECODE   )
#define MODULE_BIT_IR               ((uint64_t)1 << MODULE_IR        )
#define MODULE_BIT_3DG              ((uint64_t)1 << MODULE_3DG       )
#define MODULE_BIT_ADC              ((uint64_t)1 << MODULE_ADC       )
#define MODULE_BIT_GDMA             ((uint64_t)1 << MODULE_GDMA      )
#define MODULE_BIT_I2C              ((uint64_t)1 << MODULE_I2C       )
#define MODULE_BIT_RTC              ((uint64_t)1 << MODULE_RTC       )
#define MODULE_BIT_SPI              ((uint64_t)1 << MODULE_SPI       )
#define MODULE_BIT_TIMER            ((uint64_t)1 << MODULE_TIMER     )
#define MODULE_BIT_UART             ((uint64_t)1 << MODULE_UART      )
#define MODULE_BIT_FLASH            ((uint64_t)1 << MODULE_FLASH     )
#define MODULE_BIT_GPIO             ((uint64_t)1 << MODULE_GPIO      )
#define MODULE_BIT_PINMUX           ((uint64_t)1 << MODULE_PINMUX    )
#define MODULE_BIT_PWM              ((uint64_t)1 << MODULE_PWM       )
#define MODULE_BIT_USB              ((uint64_t)1 << MODULE_USB       )
#define MODULE_BIT_SDIO             ((uint64_t)1 << MODULE_SDIO      )
#define MODULE_BIT_CHARGER          ((uint64_t)1 << MODULE_CHARGER   )
#define MODULE_BIT_DSP              ((uint64_t)1 << MODULE_DSP       )
#define MODULE_BIT_EFUSE            ((uint64_t)1 << MODULE_EFUSE     )

#define MODULE_BIT_APP              ((uint64_t)1 << MODULE_APP       )
#define MODULE_BIT_DFU              ((uint64_t)1 << MODULE_DFU       )
#define MODULE_BIT_RFCOMM           ((uint64_t)1 << MODULE_RFCOMM    )
#define MODULE_BIT_PROFILE          ((uint64_t)1 << MODULE_PROFILE   )
#define MODULE_BIT_PROTOCOL         ((uint64_t)1 << MODULE_PROTOCOL  )
#define MODULE_BIT_GAP              ((uint64_t)1 << MODULE_GAP       )
#define MODULE_BIT_BTIF             ((uint64_t)1 << MODULE_BTIF      )
#define MODULE_BIT_GATT             ((uint64_t)1 << MODULE_GATT      )
#define MODULE_BIT_SMP              ((uint64_t)1 << MODULE_SMP       )
#define MODULE_BIT_SDP              ((uint64_t)1 << MODULE_SDP       )
#define MODULE_BIT_L2CAP            ((uint64_t)1 << MODULE_L2CAP     )
#define MODULE_BIT_HCI              ((uint64_t)1 << MODULE_HCI       )
#define MODULE_BIT_SNOOP            ((uint64_t)1 << MODULE_SNOOP     )
#define MODULE_BIT_LOWERSTACK       ((uint64_t)1 << MODULE_LOWERSTACK)
#define MODULE_BIT_UPPERSTACK       ((uint64_t)1 << MODULE_UPPERSTACK)
/**@}*/
uint64_t *log_module_trace_get(void );

void log_print(const char* pFuncName, T_LOG_TYPE type, T_LOG_SUBTYPE subtype, T_MODULE_ID module, uint8_t level,char *fmt, ...);

#define COMBINE_TRACE_INFO(type, subtype, module, level)  (uint32_t)(((type)<<24) | ((subtype)<<16) | ((module)<<8) | (level))

/* Internal macro that is wrapped by internal macro DBG_BUFFER. */
#define DBG_BUFFER_INTERNAL(type, sub_type, module, level, fmt, param_num, ...) do {\
			log_print(__func__, type, sub_type, module, level, fmt, ##__VA_ARGS__);\
		} while (0)

#define log_index(type, sub_type, module, level, fmt, param_num, ...); do {\
			log_print(__func__, type, sub_type, module, level, fmt, ##__VA_ARGS__);\
		} while (0)

			
#if (DBG_LEVEL >= LEVEL_ERROR)
#define DBG_BUFFER_LEVEL_ERROR(type, sub_type, module, fmt, param_num, ...) do {\
        DBG_BUFFER_INTERNAL(type, sub_type, module, LEVEL_ERROR, fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#define DBG_INDEX_LEVEL_ERROR(type, sub_type, module, fmt, param_num, ...) do {\
        log_index(COMBINE_TRACE_INFO(type, sub_type, module, LEVEL_ERROR), fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#else
#define DBG_BUFFER_LEVEL_ERROR(type, sub_type, module, fmt, param_num, ...)
#define DBG_INDEX_LEVEL_ERROR(type, sub_type, module, fmt, param_num, ...)
#endif

#if (DBG_LEVEL >= LEVEL_WARN)
#define DBG_BUFFER_LEVEL_WARN(type, sub_type, module, fmt, param_num, ...)  do {\
        DBG_BUFFER_INTERNAL(type, sub_type, module, LEVEL_WARN, fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#define DBG_INDEX_LEVEL_WARN(type, sub_type, module, fmt, param_num, ...) do {\
        log_index(COMBINE_TRACE_INFO(type, sub_type, module, LEVEL_WARN), fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#else
#define DBG_BUFFER_LEVEL_WARN(type, sub_type, module, fmt, param_num, ...)
#define DBG_INDEX_LEVEL_WARN(type, sub_type, module, fmt, param_num, ...)
#endif

#if (DBG_LEVEL >= LEVEL_INFO)
#define DBG_BUFFER_LEVEL_INFO(type, sub_type, module, fmt, param_num, ...)  do {\
        DBG_BUFFER_INTERNAL(type, sub_type, module, LEVEL_INFO, fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#define DBG_INDEX_LEVEL_INFO(type, sub_type, module, fmt, param_num, ...) do {\
        log_index(COMBINE_TRACE_INFO(type, sub_type, module, LEVEL_INFO), fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#else
#define DBG_BUFFER_LEVEL_INFO(type, sub_type, module, fmt, param_num, ...)
#define DBG_INDEX_LEVEL_INFO(type, sub_type, module, fmt, param_num, ...)
#endif

#if (DBG_LEVEL >= LEVEL_TRACE)
#define DBG_BUFFER_LEVEL_TRACE(type, sub_type, module, fmt, param_num, ...) do {\
        DBG_BUFFER_INTERNAL(type, sub_type, module, LEVEL_TRACE, fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#define DBG_INDEX_LEVEL_TRACE(type, sub_type, module, fmt, param_num, ...) do {\
        log_index(COMBINE_TRACE_INFO(type, sub_type, module, LEVEL_TRACE), fmt, param_num, ##__VA_ARGS__);\
    } while (0)
#else
#define DBG_BUFFER_LEVEL_TRACE(type, sub_type, module, fmt, param_num, ...)
#define DBG_INDEX_LEVEL_TRACE(type, sub_type, module, fmt, param_num, ...)
#endif

#define DBG_BUFFER(type, sub_type, module, level, fmt, param_num,...)   \
    DBG_BUFFER_##level(type, sub_type, module, fmt, param_num, ##__VA_ARGS__)

#define DBG_INDEX(type, sub_type, module, level, fmt, param_num,...)   \
    DBG_INDEX_##level(type, sub_type, module, fmt, param_num, ##__VA_ARGS__)

		
extern bool is_log_init;

/**
 * trace.h
 *
 * \brief    Initialize module trace mask.
 *
 * \param[in]   mask    Module trace mask array. Set NULL to load default mask array.
 *
 * \return      None.
 *
 * \ingroup  TRACE
 */
void log_module_trace_init(uint64_t mask[LEVEL_NUM]);

/**
 * trace.h
 *
 * \brief    Enable/Disable the module ID's trace.
 *
 * \param[in]   module_id   The specific module ID defined in \ref MODULE_ID.
 *
 * \param[in]   trace_level The trace level of the module ID defined in \ref TRACE_LEVEL.
 *
 * \param[in]   set         Enable or disable the module ID's trace.
 * \arg \c true     Enable the module ID's trace.
 * \arg \c false    Disable the module ID's trace.
 *
 * \return           The status of setting module ID's trace.
 * \retval true      Module ID's trace was set successfully.
 * \retval false     Module ID's trace was failed to set.
 *
 * \ingroup  TRACE
 */
bool log_module_trace_set(T_MODULE_ID module_id, uint8_t trace_level, bool set);

/**
 * trace.h
 *
 * \brief    Enable/Disable module bitmap's trace.
 *
 * \param[in]   module_bitmap   The module bitmap defined in \ref MODULE_BITMAP.
 *
 * \param[in]   trace_level     The trace level of the module bitmap defined in \ref TRACE_LEVEL.
 *
 * \param[in]   set             Enable or disable the module bitmap's trace.
 * \arg \c true     Enable the module bitmap's trace.
 * \arg \c false    Disable the module bitmap's trace.
 *
 * \return           The status of setting module bitmap's trace.
 * \retval true      Module bitmap's trace was set successfully.
 * \retval false     Module bitmap's trace was failed to set.
 *
 * \ingroup  TRACE
 */
bool log_module_bitmap_trace_set(uint64_t module_bitmap, uint8_t trace_level, bool set);

void log_disable(void);

/* Bluetooth Message Trace Interfaces */
#define BT_MESSAGE_DOWN_PRINT_ERROR(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_DOWN_MESSAGE, MODULE_UPPERSTACK, LEVEL_ERROR, "", 2, length, message)
#define BT_MESSAGE_DOWN_PRINT_WARN(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_DOWN_MESSAGE, MODULE_UPPERSTACK, LEVEL_WARN, "", 2, length, message)
#define BT_MESSAGE_DOWN_PRINT_INFO(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_DOWN_MESSAGE, MODULE_UPPERSTACK, LEVEL_INFO, "", 2, length, message)
#define BT_MESSAGE_DOWN_PRINT_TRACE(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_DOWN_MESSAGE, MODULE_UPPERSTACK, LEVEL_TRACE, "", 2, length, message)
#define BT_MESSAGE_UP_PRINT_ERROR(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_UP_MESSAGE, MODULE_UPPERSTACK, LEVEL_ERROR, "", 2, length, message)
#define BT_MESSAGE_UP_PRINT_WARN(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_UP_MESSAGE, MODULE_UPPERSTACK, LEVEL_WARN, "", 2, length, message)
#define BT_MESSAGE_UP_PRINT_INFO(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_UP_MESSAGE, MODULE_UPPERSTACK, LEVEL_INFO, "", 2, length, message)
#define BT_MESSAGE_UP_PRINT_TRACE(length, message)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_UP_MESSAGE, MODULE_UPPERSTACK, LEVEL_TRACE, "", 2, length, message)


/* Patch Trace Interfaces */
#define PATCH_PRINT_ERROR0(fmt)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 0)
#define PATCH_PRINT_ERROR1(fmt, arg0)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 1, arg0)
#define PATCH_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 2, arg0, arg1)
#define PATCH_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 3, arg0, arg1, arg2)
#define PATCH_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 4, arg0, arg1, arg2, arg3)
#define PATCH_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PATCH_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PATCH_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PATCH_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_ERROR, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PATCH_PRINT_WARN0(fmt)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 0)
#define PATCH_PRINT_WARN1(fmt, arg0)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 1, arg0)
#define PATCH_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 2, arg0, arg1)
#define PATCH_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 3, arg0, arg1, arg2)
#define PATCH_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 4, arg0, arg1, arg2, arg3)
#define PATCH_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PATCH_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PATCH_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PATCH_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_WARN, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PATCH_PRINT_INFO0(fmt)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 0)
#define PATCH_PRINT_INFO1(fmt, arg0)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 1, arg0)
#define PATCH_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 2, arg0, arg1)
#define PATCH_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 3, arg0, arg1, arg2)
#define PATCH_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 4, arg0, arg1, arg2, arg3)
#define PATCH_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PATCH_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PATCH_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PATCH_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_INFO, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PATCH_PRINT_TRACE0(fmt)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 0)
#define PATCH_PRINT_TRACE1(fmt, arg0)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 1, arg0)
#define PATCH_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define PATCH_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define PATCH_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define PATCH_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PATCH_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PATCH_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PATCH_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_INDEX(TYPE_BEE2, SUBTYPE_INDEX, MODULE_PATCH, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth Lower Stack Trace Interfaces */
#define LOWERSTACK_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 0)
#define LOWERSTACK_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define LOWERSTACK_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define LOWERSTACK_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define LOWERSTACK_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define LOWERSTACK_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define LOWERSTACK_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define LOWERSTACK_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define LOWERSTACK_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define LOWERSTACK_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 0)
#define LOWERSTACK_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define LOWERSTACK_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define LOWERSTACK_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define LOWERSTACK_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define LOWERSTACK_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define LOWERSTACK_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define LOWERSTACK_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define LOWERSTACK_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define LOWERSTACK_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 0)
#define LOWERSTACK_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 1, arg0)
#define LOWERSTACK_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define LOWERSTACK_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define LOWERSTACK_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define LOWERSTACK_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define LOWERSTACK_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define LOWERSTACK_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define LOWERSTACK_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define LOWERSTACK_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 0)
#define LOWERSTACK_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 1, arg0)
#define LOWERSTACK_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define LOWERSTACK_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define LOWERSTACK_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define LOWERSTACK_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define LOWERSTACK_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define LOWERSTACK_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define LOWERSTACK_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_LOWERSTACK, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* OS Trace Interfaces */
#define OS_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 0)
#define OS_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define OS_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define OS_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define OS_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define OS_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OS_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OS_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OS_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OS_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 0)
#define OS_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define OS_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define OS_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define OS_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define OS_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OS_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OS_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OS_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OS_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 0)
#define OS_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 1, arg0)
#define OS_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define OS_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define OS_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define OS_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OS_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OS_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OS_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OS_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 0)
#define OS_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 1, arg0)
#define OS_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define OS_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define OS_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define OS_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OS_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OS_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OS_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OS, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* OSIF Trace Interfaces */
#define OSIF_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 0)
#define OSIF_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define OSIF_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define OSIF_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define OSIF_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define OSIF_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OSIF_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OSIF_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OSIF_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OSIF_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 0)
#define OSIF_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define OSIF_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define OSIF_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define OSIF_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define OSIF_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OSIF_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OSIF_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OSIF_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OSIF_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 0)
#define OSIF_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 1, arg0)
#define OSIF_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define OSIF_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define OSIF_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define OSIF_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OSIF_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OSIF_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OSIF_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define OSIF_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 0)
#define OSIF_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 1, arg0)
#define OSIF_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define OSIF_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define OSIF_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define OSIF_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define OSIF_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define OSIF_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define OSIF_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_OSIF, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth HCI Trace Interfaces */
#define HCI_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 0)
#define HCI_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define HCI_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define HCI_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define HCI_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define HCI_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define HCI_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define HCI_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define HCI_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define HCI_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 0)
#define HCI_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define HCI_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define HCI_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define HCI_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define HCI_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define HCI_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define HCI_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define HCI_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define HCI_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 0)
#define HCI_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 1, arg0)
#define HCI_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define HCI_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define HCI_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define HCI_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define HCI_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define HCI_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define HCI_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define HCI_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 0)
#define HCI_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 1, arg0)
#define HCI_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define HCI_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define HCI_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define HCI_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define HCI_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define HCI_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define HCI_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_HCI, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth L2CAP Trace Interfaces */
#define L2CAP_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 0)
#define L2CAP_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define L2CAP_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define L2CAP_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define L2CAP_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define L2CAP_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define L2CAP_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define L2CAP_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define L2CAP_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define L2CAP_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 0)
#define L2CAP_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define L2CAP_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define L2CAP_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define L2CAP_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define L2CAP_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define L2CAP_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define L2CAP_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define L2CAP_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define L2CAP_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 0)
#define L2CAP_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 1, arg0)
#define L2CAP_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define L2CAP_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define L2CAP_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define L2CAP_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define L2CAP_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define L2CAP_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define L2CAP_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define L2CAP_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 0)
#define L2CAP_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 1, arg0)
#define L2CAP_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define L2CAP_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define L2CAP_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define L2CAP_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define L2CAP_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define L2CAP_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define L2CAP_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_L2CAP, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth SDP Trace Interfaces */
#define SDP_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 0)
#define SDP_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define SDP_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define SDP_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define SDP_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define SDP_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SDP_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SDP_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SDP_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SDP_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 0)
#define SDP_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define SDP_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define SDP_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define SDP_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define SDP_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SDP_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SDP_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SDP_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SDP_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 0)
#define SDP_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 1, arg0)
#define SDP_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define SDP_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define SDP_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define SDP_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SDP_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SDP_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SDP_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SDP_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 0)
#define SDP_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 1, arg0)
#define SDP_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define SDP_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define SDP_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define SDP_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SDP_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SDP_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SDP_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SDP, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth SMP Trace Interfaces */
#define SMP_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 0)
#define SMP_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define SMP_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define SMP_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define SMP_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define SMP_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SMP_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SMP_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SMP_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SMP_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 0)
#define SMP_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define SMP_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define SMP_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define SMP_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define SMP_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SMP_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SMP_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SMP_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SMP_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 0)
#define SMP_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 1, arg0)
#define SMP_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define SMP_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define SMP_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define SMP_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SMP_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SMP_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SMP_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SMP_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 0)
#define SMP_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 1, arg0)
#define SMP_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define SMP_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define SMP_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define SMP_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define SMP_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define SMP_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define SMP_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_SMP, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth GATT Trace Interfaces */
#define GATT_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 0)
#define GATT_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define GATT_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define GATT_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define GATT_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define GATT_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GATT_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GATT_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GATT_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GATT_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 0)
#define GATT_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define GATT_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define GATT_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define GATT_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define GATT_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GATT_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GATT_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GATT_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GATT_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 0)
#define GATT_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 1, arg0)
#define GATT_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define GATT_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define GATT_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define GATT_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GATT_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GATT_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GATT_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GATT_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 0)
#define GATT_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 1, arg0)
#define GATT_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define GATT_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define GATT_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define GATT_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GATT_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GATT_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GATT_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GATT, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth BTIF Trace Interfaces */
#define BTIF_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 0)
#define BTIF_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define BTIF_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define BTIF_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define BTIF_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define BTIF_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTIF_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTIF_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTIF_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTIF_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 0)
#define BTIF_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define BTIF_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define BTIF_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define BTIF_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define BTIF_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTIF_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTIF_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTIF_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTIF_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 0)
#define BTIF_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 1, arg0)
#define BTIF_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define BTIF_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define BTIF_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define BTIF_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTIF_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTIF_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTIF_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTIF_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 0)
#define BTIF_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 1, arg0)
#define BTIF_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define BTIF_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define BTIF_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define BTIF_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTIF_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTIF_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTIF_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTIF, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth BTE Trace Interfaces */
#define BTE_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 0)
#define BTE_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define BTE_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define BTE_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define BTE_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define BTE_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTE_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTE_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTE_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTE_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 0)
#define BTE_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define BTE_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define BTE_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define BTE_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define BTE_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTE_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTE_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTE_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTE_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 0)
#define BTE_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 1, arg0)
#define BTE_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define BTE_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define BTE_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define BTE_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTE_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTE_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTE_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define BTE_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 0)
#define BTE_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 1, arg0)
#define BTE_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define BTE_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define BTE_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define BTE_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define BTE_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define BTE_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define BTE_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BTE, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth GAP Trace Interfaces */
#define GAP_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 0)
#define GAP_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define GAP_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define GAP_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define GAP_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define GAP_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GAP_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GAP_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GAP_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GAP_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 0)
#define GAP_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define GAP_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define GAP_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define GAP_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define GAP_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GAP_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GAP_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GAP_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GAP_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 0)
#define GAP_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 1, arg0)
#define GAP_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define GAP_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define GAP_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define GAP_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GAP_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GAP_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GAP_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define GAP_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 0)
#define GAP_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 1, arg0)
#define GAP_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define GAP_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define GAP_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define GAP_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define GAP_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define GAP_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define GAP_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_GAP, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth RFCOMM Trace Interfaces */
#define RFCOMM_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 0)
#define RFCOMM_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define RFCOMM_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define RFCOMM_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define RFCOMM_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define RFCOMM_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define RFCOMM_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define RFCOMM_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define RFCOMM_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define RFCOMM_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 0)
#define RFCOMM_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define RFCOMM_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define RFCOMM_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define RFCOMM_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define RFCOMM_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define RFCOMM_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define RFCOMM_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define RFCOMM_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define RFCOMM_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 0)
#define RFCOMM_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 1, arg0)
#define RFCOMM_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define RFCOMM_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define RFCOMM_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define RFCOMM_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define RFCOMM_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define RFCOMM_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define RFCOMM_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define RFCOMM_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 0)
#define RFCOMM_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 1, arg0)
#define RFCOMM_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define RFCOMM_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define RFCOMM_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define RFCOMM_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define RFCOMM_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define RFCOMM_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define RFCOMM_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_RFCOMM, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth Protocol Trace Interfaces */
#define PROTOCOL_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 0)
#define PROTOCOL_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define PROTOCOL_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define PROTOCOL_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define PROTOCOL_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define PROTOCOL_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROTOCOL_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROTOCOL_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROTOCOL_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROTOCOL_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 0)
#define PROTOCOL_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define PROTOCOL_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define PROTOCOL_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define PROTOCOL_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define PROTOCOL_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROTOCOL_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROTOCOL_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROTOCOL_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROTOCOL_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 0)
#define PROTOCOL_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 1, arg0)
#define PROTOCOL_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define PROTOCOL_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define PROTOCOL_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define PROTOCOL_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROTOCOL_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROTOCOL_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROTOCOL_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROTOCOL_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 0)
#define PROTOCOL_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 1, arg0)
#define PROTOCOL_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define PROTOCOL_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define PROTOCOL_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define PROTOCOL_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROTOCOL_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROTOCOL_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROTOCOL_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROTOCOL, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth Profile Trace Interfaces */
#define PROFILE_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 0)
#define PROFILE_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define PROFILE_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define PROFILE_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define PROFILE_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define PROFILE_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROFILE_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROFILE_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROFILE_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROFILE_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 0)
#define PROFILE_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define PROFILE_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define PROFILE_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define PROFILE_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define PROFILE_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROFILE_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROFILE_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROFILE_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROFILE_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 0)
#define PROFILE_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 1, arg0)
#define PROFILE_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define PROFILE_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define PROFILE_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define PROFILE_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROFILE_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROFILE_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROFILE_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define PROFILE_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 0)
#define PROFILE_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 1, arg0)
#define PROFILE_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define PROFILE_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define PROFILE_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define PROFILE_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define PROFILE_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define PROFILE_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define PROFILE_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_PROFILE, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/**
 * trace.h
 *
 * \name    APP_PRINT_TRACE
 * \brief   Bluetooth APP Trace Interfaces.
 * \anchor  APP_PRINT_TRACE
 */
/**
 * \ingroup TRACE
 */
/**@{*/
#define APP_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 0)
#define APP_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define APP_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define APP_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define APP_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define APP_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define APP_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define APP_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define APP_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define APP_PRINT_WARN0(fmt)     \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 0)
#define APP_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define APP_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define APP_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define APP_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define APP_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define APP_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define APP_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define APP_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define APP_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 0)
#define APP_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 1, arg0)
#define APP_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define APP_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define APP_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define APP_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define APP_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define APP_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define APP_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define APP_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 0)
#define APP_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 1, arg0)
#define APP_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define APP_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define APP_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define APP_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define APP_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define APP_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define APP_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_APP, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
/**@}*/


/* AES Trace Interfaces */
#define AES_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_ERROR, "!!!"fmt, 0)
#define AES_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define AES_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define AES_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define AES_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define AES_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_WARN, "!!*"fmt, 0)
#define AES_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define AES_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define AES_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define AES_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define AES_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_INFO, "!**"fmt, 0)
#define AES_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_INFO, "!**"fmt, 1, arg0)
#define AES_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define AES_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define AES_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_AES, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)


/* DLPS Trace Interfaces */
#define DLPS_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_ERROR, "!!!"fmt, 0)
#define DLPS_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define DLPS_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define DLPS_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define DLPS_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define DLPS_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_WARN, "!!*"fmt, 0)
#define DLPS_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define DLPS_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define DLPS_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define DLPS_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define DLPS_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO, "!**"fmt, 0)
#define DLPS_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO, "!**"fmt, 1, arg0)
#define DLPS_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define DLPS_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define DLPS_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)


/* Boot Trace Interfaces */
#define BOOT_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_ERROR, "!!!"fmt, 0)
#define BOOT_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define BOOT_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define BOOT_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define BOOT_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define BOOT_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_WARN, "!!*"fmt, 0)
#define BOOT_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define BOOT_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define BOOT_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define BOOT_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define BOOT_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_INFO, "!**"fmt, 0)
#define BOOT_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_INFO, "!**"fmt, 1, arg0)
#define BOOT_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define BOOT_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define BOOT_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_BOOT, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)


/* Bluetooth OTA/DFU Trace Interfaces */
#define DFU_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 0)
#define DFU_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define DFU_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define DFU_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define DFU_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define DFU_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define DFU_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define DFU_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define DFU_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define DFU_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 0)
#define DFU_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define DFU_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define DFU_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define DFU_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define DFU_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define DFU_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define DFU_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define DFU_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define DFU_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 0)
#define DFU_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 1, arg0)
#define DFU_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define DFU_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define DFU_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define DFU_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define DFU_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define DFU_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define DFU_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define DFU_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 0)
#define DFU_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 1, arg0)
#define DFU_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define DFU_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define DFU_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define DFU_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define DFU_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define DFU_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define DFU_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_DFU, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/* Bluetooth FLASH/CACHE Trace Interfaces */
#define FLASH_PRINT_ERROR0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 0)
#define FLASH_PRINT_ERROR1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 1, arg0)
#define FLASH_PRINT_ERROR2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 2, arg0, arg1)
#define FLASH_PRINT_ERROR3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 3, arg0, arg1, arg2)
#define FLASH_PRINT_ERROR4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 4, arg0, arg1, arg2, arg3)
#define FLASH_PRINT_ERROR5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define FLASH_PRINT_ERROR6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define FLASH_PRINT_ERROR7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define FLASH_PRINT_ERROR8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_ERROR, "!!!"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define FLASH_PRINT_WARN0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 0)
#define FLASH_PRINT_WARN1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 1, arg0)
#define FLASH_PRINT_WARN2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 2, arg0, arg1)
#define FLASH_PRINT_WARN3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 3, arg0, arg1, arg2)
#define FLASH_PRINT_WARN4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 4, arg0, arg1, arg2, arg3)
#define FLASH_PRINT_WARN5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define FLASH_PRINT_WARN6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define FLASH_PRINT_WARN7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define FLASH_PRINT_WARN8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_WARN, "!!*"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define FLASH_PRINT_INFO0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 0)
#define FLASH_PRINT_INFO1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 1, arg0)
#define FLASH_PRINT_INFO2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 2, arg0, arg1)
#define FLASH_PRINT_INFO3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 3, arg0, arg1, arg2)
#define FLASH_PRINT_INFO4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 4, arg0, arg1, arg2, arg3)
#define FLASH_PRINT_INFO5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define FLASH_PRINT_INFO6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define FLASH_PRINT_INFO7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define FLASH_PRINT_INFO8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_INFO, "!**"fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define FLASH_PRINT_TRACE0(fmt)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 0)
#define FLASH_PRINT_TRACE1(fmt, arg0)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 1, arg0)
#define FLASH_PRINT_TRACE2(fmt, arg0, arg1)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 2, arg0, arg1)
#define FLASH_PRINT_TRACE3(fmt, arg0, arg1, arg2)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 3, arg0, arg1, arg2)
#define FLASH_PRINT_TRACE4(fmt, arg0, arg1, arg2, arg3)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 4, arg0, arg1, arg2, arg3)
#define FLASH_PRINT_TRACE5(fmt, arg0, arg1, arg2, arg3, arg4)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 5, arg0, arg1, arg2, arg3, arg4)
#define FLASH_PRINT_TRACE6(fmt, arg0, arg1, arg2, arg3, arg4, arg5)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 6, arg0, arg1, arg2, arg3, arg4, arg5)
#define FLASH_PRINT_TRACE7(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 7, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
#define FLASH_PRINT_TRACE8(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   \
    DBG_BUFFER(TYPE_BEE2, SUBTYPE_FORMAT, MODULE_FLASH, LEVEL_TRACE, fmt, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#ifdef __cplusplus
}
#endif

#endif /* _TRACE_H_ */
