/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     platform_diagnose.h
  * @brief    Head file for platform diagnose related.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-9-28
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PLATFORM_DIAGNOSE_H
#define _PLATFORM_DIAGNOSE_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "platform_macros.h"
#include "trace.h"
#include <stdio.h>

/** @addtogroup Mesh_Platform_Diagnose Mesh Platform Diagnose
  * @brief debug related
  * @{
  */

/** @defgroup Mesh_Platform_Diagnose_Exported_Macros Mesh Platform Diagnose Exported Macros
  * @brief
  * @{
  */

/** @brief Mesh Module */
typedef enum
{
    MM_COMMON,
    MM_GAP_SCHED,
    MM_PB_ADV,
    MM_PB_GATT,
    MM_SERVICE,
    MM_PROV,
    MM_PROXY,
    MM_BEACON,
    MM_BEARER, //!< 8
    MM_NETWORK,
    MM_TRANSPORT,
    MM_FRND,
    MM_HB,
    MM_TRANS_PING,
    MM_ACCESS,
    MM_RPL,
    MM_SECURITY, //!< 16
    MM_FLASH,
    MM_MODEL,
    MM_APP,
    MM_NUM = 32
} mesh_module_t;

extern char* mesh_level_prompt[4];

/** @{ */
#define MESH_LOG_LEVEL_COUNT                    4 //!< 0 LEVEL_ERROR (Highest) | 1 LEVEL_WARN | 2 LEVEL_INFO | 3 LEVEL_TRACE (Lowest)
#define MESH_LOG_LEVEL                          LEVEL_TRACE //!< log of lower level won't be outputed
#define MESH_LOG_LEVEL_SIZE                     ((MM_NUM + 31)/32) //!< sizeof(uint32_t)
/** @} */

///@cond
//#define ARG_T(t)                                t
//#define ARG_N(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,N,...)  N
//#define ARG_N_HELPER(...)                       ARG_T(ARG_N(__VA_ARGS__))
//#define COUNT_ARG1(...)                         ARG_N_HELPER(__VA_ARGS__, 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
//#define COUNT_ARG(...)                          ((COUNT_ARG1(1, ##__VA_ARGS__) - 1)%20)

uint8_t *diag_bin_to_hex(uint8_t *pbin, int8_t plen);

#ifndef MM_ID
#define MM_ID MM_COMMON
#endif
#define LOG_PRINT1(module, level, fmt, ...)     do\
    {\
        if(level <= MESH_LOG_LEVEL && module < MM_NUM && mesh_log_switch[level][module/32] & (1 << (module % 32)))\
		{\
			log_print(__func__,TYPE_MESH,SUBTYPE_FORMAT,MODULE_APP,level," "#module" "fmt,##__VA_ARGS__);\
		}\
    } while(0)
#define LOG_PRINT(module, level, fmt, ...)      LOG_PRINT1(module, level, fmt, ##__VA_ARGS__)
#define LOG_DUMP1(module, level, pdata, len)    do\
    {\
        if(level <= MESH_LOG_LEVEL && mesh_log_switch[level][module/32] & (1 << (module % 32)))\
		{\
			log_print(__func__,TYPE_MESH,SUBTYPE_FORMAT,MODULE_APP,level," "#module" hex: %s", diag_bin_to_hex(pdata,len));\
		}\
	} while(0)
#define LOG_DUMP(module, level, pdata, len)     LOG_DUMP1(module, level, pdata, len)
///@endcond

/** @} */

/** @defgroup Mesh_Platform_Diagnose_Exported_Functions Mesh Platform Diagnose Exported Functions
  * @brief
  * @{
  */

/** @defgroup Mesh_LOG_API Mesh LOG API
  * @brief C-type printf api
  *
  * format print & raw data dump, each has four trace types.
  * -# ERROR   : Fatal, Procedure Cannot Advance (Log Token !!!)
  * -# WARN    : Abnormal Condition Occurred, But Procedure Can Advance (Log Token !!*)
  * -# INFO    : Important Notification (Log Token !**)
  * -# TRACE   : Verbose Debug, Security Info
  * @{
  */
#define printe(fmt, ...)                        LOG_PRINT(MM_ID, LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define printw(fmt, ...)                        LOG_PRINT(MM_ID, LEVEL_WARN, fmt, ##__VA_ARGS__)
#define printi(fmt, ...)                        LOG_PRINT(MM_ID, LEVEL_INFO, fmt, ##__VA_ARGS__)
#define printt(fmt, ...)                        LOG_PRINT(MM_ID, LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define dprinte(pdata, len)                     LOG_DUMP(MM_ID, LEVEL_ERROR, pdata, len)
#define dprintw(pdata, len)                     LOG_DUMP(MM_ID, LEVEL_WARN, pdata, len)
#define dprinti(pdata, len)                     LOG_DUMP(MM_ID, LEVEL_INFO, pdata, len)
#define dprintt(pdata, len)                     LOG_DUMP(MM_ID, LEVEL_TRACE, pdata, len)
/** @} */

/** @brief assert */
//#define diag_assert(expr)                       //PLATFORM_ASSERT(expr)
#define diag_assert(expr)                       //assert(expr) //Linux platform

/** @brief log level control */
extern uint32_t mesh_log_switch[MESH_LOG_LEVEL_COUNT][MESH_LOG_LEVEL_SIZE];

/**
  * @brief Set mesh modules debug diagnose level
  * @param[in] level: log level @ref MESH_LOG_LEVEL_COUNT
  * @param[in] module_bitmap: bitmap of mesh modules, 0 means disable the log of the corresponding module
  * @return operation result
  * @retval true: operation success
  * @retval false: operation failure
  *
  * <b>Example usage</b>
  * \code{.c}
    void mesh_stack_init(void)
    {
        ...
        uint32_t log_value[MESH_LOG_LEVEL_SIZE] = {0};
        diag_level_set(LEVEL_TRACE, log_value);
        ...
    }
  * \endcode
  */
bool diag_level_set(uint8_t level, uint32_t *module_bitmap);

/** @} */

/** @} End of group Mesh_Platform_Diagnose */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PLATFORM_DIAGNOSE_H */
