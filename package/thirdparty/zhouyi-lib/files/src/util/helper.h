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
 * @file  helper.h
 * @brief UMD helper function header
 */

#ifndef _HELPER_H_
#define _HELPER_H_

/**
 * @brief Align buffer size per page_size (4KB)
 */
#define ALIGN_PAGE(size) (((size)+(4096)-1)&(~((4096)-1)))

/**
 * @brief This function is used to dump memory data into file
 *
 * @param[in] fname File full name
 * @param[in] src   Source location of data
 * @param[in] size  Dumped size
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_SIZE
 * @retval AIPU_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval AIPU_STATUS_ERROR_WRITE_FILE_FAIL
 */
aipu_status_t umd_dump_file_helper(const char* fname, const void* src, unsigned int size);
/**
 * @brief This function is used to load file into memory
 *
 * @param[in]  fname File full name
 * @param[out] dest  Destination of data
 * @param[in]  size  Load size
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_SIZE
 * @retval AIPU_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval AIPU_STATUS_ERROR_READ_FILE_FAIL
 */
aipu_status_t umd_load_file_helper(const char* fname, void* dest, unsigned int size);
/**
 * @brief This function is used to open and mmap a file into memory
 *
 * @param[in]  fname File full name
 * @param[out] data  Pointer to file mmap buffer
 * @param[out] size  File size
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_OPEN_FILE_FAIL
 * @retval AIPU_STATUS_ERROR_MAP_FILE_FAIL
 */
aipu_status_t umd_mmap_file_helper(const char* fname, void** data, unsigned int* size);

#endif /* _HELPER_H_ */
