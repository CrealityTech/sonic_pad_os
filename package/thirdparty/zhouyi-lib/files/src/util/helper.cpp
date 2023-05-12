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
 * @file  helper.cpp
 * @brief UMD helper function implementation
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include "standard_api_v2.0.0.h"
#include "log.h"

aipu_status_t umd_dump_file_helper(const char* fname, const void* src, unsigned int size)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    int fd = 0;
    int wbytes = 0;

    if ((nullptr == fname) || (nullptr == src))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

    fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1)
    {
        LOG(LOG_ERR, "create bin file failed: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        goto finish;
    }
    if (ftruncate(fd, size) == -1)
    {
        LOG(LOG_ERR, "create bin file failed: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        goto finish;
    }
    wbytes = write(fd, src, size);
    if (wbytes != (int)size)
    {
        LOG(LOG_ERR, "write bin file %s failed, need to write 0x%x bytes, \
            successfully write 0x%x bytes (errno = %d)!\n", fname, size, wbytes, errno);
        ret = AIPU_STATUS_ERROR_WRITE_FILE_FAIL;
        goto finish;
    }

finish:
    if (fd > 0)
    {
        close(fd);
    }
    return ret;
}

aipu_status_t umd_load_file_helper(const char* fname, void* dest, unsigned int size)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    int fd = 0;

    if ((nullptr == fname) || (nullptr == dest))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

    fd = open(fname, O_RDONLY);
    if (fd <= 0)
    {
        LOG(LOG_ERR, "open file failed: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        goto finish;
    }
    if (read(fd, dest, size) < 0)
    {
        LOG(LOG_ERR, "load file failed: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_READ_FILE_FAIL;
    }

finish:
    if (fd > 0)
    {
        close(fd);
    }
    return ret;
}

aipu_status_t umd_mmap_file_helper(const char* fname, void** data, unsigned int* size)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    int fd = 0;
    void* p_file = nullptr;
    struct stat finfo;

    if ((nullptr == fname) || (nullptr == data) || (nullptr == size))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (stat(fname, &finfo) != 0)
    {
        LOG(LOG_ERR, "no such a file: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        goto finish;
    }

    fd = open(fname, O_RDONLY);
    if (fd <= 0)
    {
        LOG(LOG_ERR, "open file failed: %s! (errno = %d)\n", fname, errno);
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        goto finish;
    }

    p_file = mmap(nullptr, finfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == p_file)
    {
        ret = AIPU_STATUS_ERROR_MAP_FILE_FAIL;
        LOG(LOG_ERR, "RT failed in mapping graph file: %s! (errno = %d)\n", fname, errno);
        goto finish;
    }

    /* success */
    *data = p_file;
    *size = finfo.st_size;

finish:
    if (fd > 0)
    {
        close(fd);
    }
    return ret;
}