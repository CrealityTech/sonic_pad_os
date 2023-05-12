/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Char <yanjianbo@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sst_log.h"
#include "secure_storage.h"

#define SEC_BLK_SIZE (4096)

int secure_data_write(const char *item_name, char *buffer, int length)
{
    int  ret;
    char buffer_bak[SEC_BLK_SIZE];

    if (!item_name || !buffer) {
        SST_DBG_ERR("[secure_data_write] err: item_name or buffer is null");
        return -1;
    }

    if (length > SEC_BLK_SIZE) {
        SST_DBG_ERR("[secure_data_write] err: key length bigger than 4KB, %d",
              length);
        return -1;
    }
    memset(buffer_bak, 0, SEC_BLK_SIZE);
    strncpy(buffer_bak, buffer, length);

    ret = secure_storage_support();
    if (!ret) { /*support*/
        if (sunxi_secure_storage_init() < 0) {
            SST_DBG_ERR("[secure_data_write]:Secure storage init fail\n");
            return -1;
        }
        if (sunxi_secure_object_write(item_name, buffer_bak, length) < 0) {
            SST_DBG_ERR("[secure_data_write]: Secure storage write fail\n");
            sunxi_secure_storage_exit(0);
            return -1;
        }

        if (sunxi_secure_storage_exit(0) < 0) {
            SST_DBG_ERR("[secure_data_write]:Secure storage exit fail\n");
            return -1;
        }
        SST_DBG_INFO("[secure_data_write]: write data to secure storage done\n");
    }
    return 0;
}

/*
 * Read private area firstly.
 * If the private area read fail, then try the secure storage
 */
int secure_data_read(const char *item_name, char *buffer, int buffer_len,
                      int *data_len)
{
    int ret;

    ret = secure_storage_support();
    if (!ret) { /*support*/
        if (sunxi_secure_storage_init() < 0) {
            SST_DBG_ERR("[secure_data_read]:Secure storage init fail\n");
            goto Next;
        }
        if (sunxi_secure_object_read(item_name, buffer, buffer_len, data_len) <
            0) {
            SST_DBG_ERR("[secure_data_read]: Secure storage read fail\n");
            sunxi_secure_storage_exit(0);
            goto Next;
        }

        if (sunxi_secure_storage_exit(0) < 0) {
            SST_DBG_ERR("[secure_data_read]:Secure storage exit fail\n");
            goto Next;
        }

        SST_DBG_INFO("[secure_data_read]: read data to secure storage done\n");
        return 0;
    }

Next:
    return ret;
}
