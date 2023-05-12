/**********************************************************************************
 * This file is CONFIDENTIAL and any use by you is subject to the terms of the
 * agreement between you and Arm China or the terms of the agreement between you
 * and the party authorised by Arm China to disclose this file to you.
 * The confidential and proprietary information contained in this file
 * may only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from Arm China.
 *
 *        (C) Copyright 2020 Arm Technology (China) Co. Ltd.
 *                    All rights reserved.
 *
 * This entire notice must be reproduced on all copies of this file and copies of
 * this file may only be made by a person if such person is permitted to do so
 * under the terms of a subsisting license agreement from Arm China.
 *
 *********************************************************************************/

/**
 * @file  log.h
 * @brief UMD logging macro header
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>
#include "debug.h"

/**
 * @brief Log level
 */
enum LogLevel
{
    LOG_ERR,     /**< serious error messages only */
    LOG_WARN,    /**< warning messages */
    LOG_INFO,    /**< normal informational messages */
    LOG_DEBUG,   /**< debug messages, should be closed after debug done */
    LOG_DEFAULT, /**< default logging messages */
    LOG_CLOSE    /**< close logging messages printing */
};

#if (defined LOG_ENABLE) && (LOG_ENABLE == 1)

#define LOG_DETAILED(flag, FMT, ARGS...) \
    printf("%s%s:%d:%s: " FMT "\n", (flag), __FILE__, __LINE__, __PRETTY_FUNCTION__, ## ARGS);
/**
 * @brief Log macro
 * @param LogLevel log level
 * @param FMT format
 * @param ARGS var arguments
 */
#define LOG(LogLevel, FMT, ARGS...) do { \
    if (LogLevel==LOG_ERR) \
        LOG_DETAILED("[UMD ERROR] ", FMT, ## ARGS) \
    else if (LogLevel==LOG_WARN) \
        LOG_DETAILED("[UMD WARN] ", FMT, ## ARGS) \
    else if (LogLevel==LOG_INFO) \
        printf("[UMD INFO] " FMT "\n", ## ARGS); \
    else if (LogLevel==LOG_DEBUG) \
        LOG_DETAILED("[UMD DEBUG] ", FMT, ## ARGS) \
    else if (LogLevel==LOG_DEFAULT) \
        printf("" FMT "\n", ## ARGS); \
    } while (0)
#else /* ! LOG_ENABLE */
/**
 * @brief Log macro, not implemented when log disabled
 */
#define LOG(LogLevel, FMT, ARGS...)
#endif /* #ifdef LOG_ENABLE */

#endif /* _LOG_H_ */