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
 * @file log.h
 * Header of LOG macro defination
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <linux/kernel.h>

enum LogLevel {
        LOG_ERR,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,
        LOG_DEFAULT,
        LOG_CLOSE
};

#define LOG(LogLevel, FMT, ARGS...) do { \
        if (LogLevel==LOG_ERR) \
                printk(KERN_ERR "[KMD ERROR] %s():%d   " FMT "\n", __FUNCTION__, __LINE__, ## ARGS); \
        else if (((BUILD_DEBUG_VERSION)&&(LogLevel==LOG_WARN))||(LogLevel==LOG_WARN)) \
                printk(KERN_WARNING "[KMD WARN] %s():%d   " FMT "\n", __FUNCTION__, __LINE__, ## ARGS); \
        else if (LogLevel==LOG_INFO) \
                printk(KERN_INFO "[KMD INFO] " FMT "\n", ## ARGS); \
        else if ((BUILD_DEBUG_VERSION)&&(LogLevel==LOG_DEBUG)) \
                printk(KERN_DEBUG "[KMD DEBUG] " FMT "\n", ## ARGS); \
        else if ((BUILD_DEBUG_VERSION)&&(LogLevel==LOG_DEFAULT)) \
                printk(KERN_INFO "" FMT "\n", ## ARGS); \
        } while (0)
#endif
