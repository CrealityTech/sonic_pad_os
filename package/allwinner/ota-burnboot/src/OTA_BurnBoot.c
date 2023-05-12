#include <stdio.h>
#include "Utils.h"
#include "BurnNandBoot.h"
#include "BurnSpinor.h"
#include "BurnSdBoot.h"

int OTA_burnboot0(const char *img_path)
{
    int ret;
    BufferExtractCookie cookie;
    if (getBufferExtractCookieOfFile(img_path, &cookie) < 0) {
        ob_error("getBufferExtractCookieOfFile failed\n");
        return -1;
    }

    initInfo();

    switch (getFlashType()) {
    case FLASH_TYPE_UNKNOW:
        ob_error("getFlashType failed\n");
        ret = -1;
        break;
    case FLASH_TYPE_NOR:
        ret = burnSpinorBoot0(&cookie);
        break;
    case FLASH_TYPE_NAND:
    case FLASH_TYPE_SPINAND:
        ret = burnNandBoot0(&cookie);
        break;
    default:
        ret = burnSdBoot0(&cookie);
        break;
    }

    freeCookie(&cookie);
    return ret;
}

int OTA_burnuboot(const char *img_path)
{
    int ret;
    BufferExtractCookie cookie;
    if (getBufferExtractCookieOfFile(img_path, &cookie) < 0) {
        ob_error("getBufferExtractCookieOfFile failed\n");
        return -1;
    }

    initInfo();

    switch (getFlashType()) {
    case FLASH_TYPE_UNKNOW:
        ob_error("getFlashType failed\n");
        ret = -1;
        break;
    case FLASH_TYPE_NOR:
        ret = burnSpinorUboot(&cookie);
        break;
    case FLASH_TYPE_NAND:
    case FLASH_TYPE_SPINAND:
        ret = burnNandUboot(&cookie);
        break;
    default:
        ret = burnSdUboot(&cookie);
        break;
    }

    freeCookie(&cookie);
    return ret;
}
