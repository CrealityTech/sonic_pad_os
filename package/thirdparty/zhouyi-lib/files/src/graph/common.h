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
 * @file  common.h
 * @brief AIPU loadable graph common struct def header
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#define BIN_MAGIC_ARR_LEN      16
#define BIN_EXTRA_DATA_ARR_LEN 8
#define BIN_HDR_SIZE           (sizeof(bin_hdr_t))

#define MAGIC                  "AIPU BIN"

namespace AIRT
{
typedef enum {
    SECTION_TYPE_INPUT = 0,
    SECTION_TYPE_OUTPUT,
    SECTION_TYPE_DUMP,
    SECTION_TYPE_INTERMEDIATE,
    SECTION_TYPE_WEIGHT,
    SECTION_TYPE_BIAS,
    SECTION_TYPE_SCALES,
    SECTION_TYPE_SHIFTS,
    SECTION_TYPE_LUT,
    SECTION_TYPE_NEG_SLOPE
} sub_section_type_t;

typedef struct bin_hdr {
    char     ident[16];
    uint32_t device;
    uint32_t version;
    uint32_t build_version;
    uint32_t hdrsize;
    uint32_t filesize;
    uint32_t type;
    uint32_t flag;
    uint32_t entry;
    uint32_t text_offset;
    uint32_t text_size;
    uint32_t rodata_offset;
    uint32_t rodata_size;
    uint32_t dcr_offset;
    uint32_t dcr_size;
    uint32_t data_offset;
    uint32_t data_size;
    uint32_t bss_offset;
    uint32_t bss_size;
    uint32_t misc_offset;
    uint32_t misc_size;
    int8_t   extra_data[8];
} bin_hdr_t;

#define AIPU_ARCH(device)           ((device)>>20)
#define AIPU_VERSION(device)        (((device)>>16)&0xF)
#define AIPU_CONFIG(device)         ((device)&0xFFFF)
#define BUILD_MAJOR(build_version)  ((build_version)&0xF)
#define BUILD_MINOR(build_version)  (((build_version)>>8)&0xF)
#define BUILD_NUMBER(build_version) ((build_version)>>16)
#define GVERSION_MINOR(gversion)    ((gversion)>>24)
}

#endif /* _COMMON_H_ */