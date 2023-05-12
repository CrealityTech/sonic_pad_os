/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_sdk.c
  * @brief    Source file for the common part used by the mesh sdk.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2019-4-15
  * @version  v1.0
  * *************************************************************************************
  */

/* Add Includes here */
#include "mesh_api.h"
#include "mesh_sdk.h"
#include "version_sdk.h"

int mesh_sdk_version(void)
{
    DBG_DIRECT("Mesh SDK Ver: %s, GCID: 0x%x, Build Time: %s", VERSION_BUILD_STR, VERSION_GCID,
               BUILDING_TIME);
    return mesh_version_check(VERSION_BUILD_STR, VERSION_GCID);
}
