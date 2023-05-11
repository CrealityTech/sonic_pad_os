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
 * @file aipu_sysfs.h
 * sysfs interface implementation file
 */

#include <linux/device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "aipu_sysfs.h"
#include "aipu.h"

static struct aipu_priv *aipu = NULL;

static int print_reg_info(char *buf, const char *name, int offset)
{
        struct aipu_io_req io_req;

        if (!aipu)
                return 0;

        io_req.rw = AIPU_IO_READ;
        io_req.offset = offset;
        aipu_priv_io_rw(aipu, &io_req);
        return snprintf(buf, 1024, "0x%-*x%-*s0x%08x\n", 6, offset, 22, name, io_req.value);
}

static ssize_t sysfs_kmd_version_show(struct device *dev, struct device_attribute *attr, char* buf)
{
        return snprintf(buf, MAX_CHAR_SYSFS, "AIPU KMD Version: %s\n", KMD_VERSION);
}

static ssize_t sysfs_aipu_ext_register_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        int ret = 0;
        char tmp[1024];

        if (!aipu)
                return 0;

        if (aipu->is_suspend) {
                return snprintf(buf, MAX_CHAR_SYSFS,
                        "AIPU is suspended and external registers cannot be read, 0x%x, 0x%llx!\n",
                        aipu->is_suspend, aipu->host_to_aipu_map);
        }

        if (aipu->is_reset) {
                return snprintf(buf, MAX_CHAR_SYSFS,
                        "AIPU is reset and external registers cannot be read!\n");
        }

        ret += snprintf(tmp, 1024, "   AIPU External Register Values\n");
        strcat(buf, tmp);
        ret += snprintf(tmp, 1024, "----------------------------------------\n");
        strcat(buf, tmp);
        ret += snprintf(tmp, 1024, "%-*s%-*s%-*s\n", 8, "Offset", 22, "Name", 10, "Value");
        strcat(buf, tmp);
        ret += snprintf(tmp, 1024, "----------------------------------------\n");
        strcat(buf, tmp);

        ret += print_reg_info(tmp, "Ctrl Reg", 0x0);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "Status Reg", 0x4);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "Start PC Reg", 0x8);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "Intr PC Reg", 0xC);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "IPI Ctrl Reg", 0x10);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "Data Addr 0 Reg", 0x14);
        strcat(buf, tmp);
        ret += print_reg_info(tmp, "Data Addr 1 Reg", 0x18);
        strcat(buf, tmp);
        if (aipu_priv_get_version(aipu) == AIPU_VERSION_ZHOUYI_V1) {
                ret += print_reg_info(tmp, "Intr Cause Reg", 0x1C);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "Intr Status Reg", 0x20);
                strcat(buf, tmp);
        }
        else if (aipu_priv_get_version(aipu) == AIPU_VERSION_ZHOUYI_V2) {
                ret += print_reg_info(tmp, "Data Addr 2 Reg", 0x1C);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "Data Addr 3 Reg", 0x20);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE0 Ctrl Reg", 0xc0);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE0 High Base Reg", 0xc4);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE0 Low Base Reg", 0xc8);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE1 Ctrl Reg", 0xcc);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE1 High Base Reg", 0xd0);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE1 Low Base Reg", 0xd4);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE2 Ctrl Reg", 0xd8);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE2 High Base Reg", 0xdc);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE2 Low Base Reg", 0xe0);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE3 Ctrl Reg", 0xe4);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE3 High Base Reg", 0xe8);
                strcat(buf, tmp);
                ret += print_reg_info(tmp, "ASE3 Low Base Reg", 0xec);
                strcat(buf, tmp);
        }
        ret += snprintf(tmp, 1024, "----------------------------------------\n");
        strcat(buf, tmp);
        return ret;
}

static ssize_t sysfs_aipu_ext_register_store(struct device *dev,
        struct device_attribute *attr, const char * buf, size_t count)
{
        int i = 0;
        int ret = 0;
        char *token = NULL;
        char *buf_dup = NULL;
        int value[3] = { 0 };
        int max_offset = 0;
        struct aipu_io_req io_req;
        aipu_priv_io_rw(aipu, &io_req);

        if (!aipu)
                return 0;

        if (aipu->is_suspend)
                return 0;
        else if (aipu->is_reset)
                return 0;

        buf_dup = (char*)kzalloc(1024, GFP_KERNEL);
        snprintf(buf_dup, 1024, buf);

        dev_dbg(dev, "[SYSFS] user input str: %s", buf_dup);

        for (i = 0; i < 3; i++) {
                token = strsep(&buf_dup, "-");
                if (token == NULL) {
                        dev_err(dev, "[SYSFS] please echo as this format: <reg_offset>-<write time>-<write value>");
                        goto finish;
                }

                dev_dbg(dev, "[SYSFS] to convert str: %s", token);

                ret = kstrtouint(token, 0, &value[i]);
                if (ret) {
                        dev_err(dev, "[SYSFS] convert str to int failed (%d): %s", ret, token);
                        goto finish;
                }
        }

        if (aipu_priv_get_version(aipu) == AIPU_VERSION_ZHOUYI_V1)
                max_offset = 0x20;
        else if (aipu_priv_get_version(aipu) == AIPU_VERSION_ZHOUYI_V2)
                max_offset = 0xec;

        if (value[0] > max_offset) {
                dev_err(dev, "[SYSFS] register offset too large which cannot be write: 0x%x", value[0]);
                goto finish;
        }

        dev_info(dev, "[SYSFS] offset 0x%x, time 0x%x, value 0x%x",
               value[0], value[1], value[2]);

        io_req.rw = AIPU_IO_WRITE;
        io_req.offset = value[0];
        io_req.value = value[2];
        for (i = 0; i < value[1]; i++) {
                dev_info(dev, "[SYSFS] writing register 0x%x with value 0x%x", value[0], value[2]);
                aipu_priv_io_rw(aipu, &io_req);
        }

finish:
        return count;
}

#if (defined PLATFORM_HAS_CLOCK_GATING) && (PLATFORM_HAS_CLOCK_GATING == 1)
static ssize_t sysfs_aipu_clock_gating_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        if (!aipu)
                return 0;

        if (aipu->is_suspend)
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in clock gating state and suspended.\n");
        else if (aipu->is_reset)
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in reset state and suspended.\n");
        else
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in normal working state.\n");
}

static ssize_t sysfs_aipu_clock_gating_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
        int do_suspend = 0;
        int do_resume = 0;

        if (!aipu)
                return count;

        if ((strncmp(buf, "1", 1) == 0))
                do_suspend = 1;
        else if ((strncmp(buf, "0", 1) == 0))
                do_resume = 1;

        if ((!aipu->is_suspend) && aipu_priv_is_idle(aipu) &&
                do_suspend) {
                dev_dbg(dev, "enable clock gating\n");
                aipu_priv_enable_clk_gating(aipu);
                aipu->is_suspend = 1;
        } else if (aipu->is_suspend && do_resume) {
                dev_dbg(dev, "disable clock gating\n");
                aipu_priv_disable_clk_gating(aipu);
                aipu->is_suspend = 0;
        }

        return count;
}
#endif

#if (defined PLATFORM_HAS_RESET) && (PLATFORM_HAS_RESET == 1)
static ssize_t sysfs_aipu_reset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        if (!aipu)
                return 0;

        if (aipu->is_reset)
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in reset state and suspended.\n");
        else if (aipu->is_suspend)
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in clock gating state and suspended.\n");
        else
                return snprintf(buf, MAX_CHAR_SYSFS, "AIPU is in normal working state.\n");
}

static ssize_t sysfs_aipu_reset_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
        int do_reset = 0;
        int do_release = 0;

        if (!aipu)
                return count;

        if ((strncmp(buf, "1", 1) == 0))
                do_release = 1;
        else if ((strncmp(buf, "0", 1) == 0))
                do_reset = 1;

        if ((!aipu->is_reset) && do_reset) {
                dev_dbg(dev, "reset AIPU\n");
                aipu_priv_logic_reset(aipu);
                aipu->is_reset = 1;
        } else if (aipu->is_reset && do_release) {
                dev_dbg(dev, "release AIPU to normal state\n");
                aipu_priv_logic_release(aipu);
                aipu_priv_enable_interrupt(aipu);
                aipu->is_reset = 0;
        }

        return count;
}
#endif

static ssize_t sysfs_aipu_job_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        if (!aipu)
                return 0;

        return aipu_job_manager_sysfs_job_show(&aipu->job_manager, buf);
}

static DEVICE_ATTR(kmd_version, 0444, sysfs_kmd_version_show, NULL);
static DEVICE_ATTR(ext_register, 0644, sysfs_aipu_ext_register_show, sysfs_aipu_ext_register_store);
static DEVICE_ATTR(job, 0444, sysfs_aipu_job_show, NULL);
#if (defined PLATFORM_HAS_CLOCK_GATING) && (PLATFORM_HAS_CLOCK_GATING == 1)
static DEVICE_ATTR(clock_gating, 0644, sysfs_aipu_clock_gating_show, sysfs_aipu_clock_gating_store);
#endif
#if (defined PLATFORM_HAS_RESET) && (PLATFORM_HAS_RESET == 1)
static DEVICE_ATTR(reset, 0644, sysfs_aipu_reset_show, sysfs_aipu_reset_store);
#endif

int aipu_create_sysfs(void *aipu_priv)
{
        int ret = 0;

        if (!aipu_priv)
                return -EINVAL;
        aipu = (struct aipu_priv *)aipu_priv;

        device_create_file(aipu->dev, &dev_attr_kmd_version);
        device_create_file(aipu->dev, &dev_attr_ext_register);
        device_create_file(aipu->dev, &dev_attr_job);
#if (defined PLATFORM_HAS_CLOCK_GATING) && (PLATFORM_HAS_CLOCK_GATING == 1)
        device_create_file(aipu->dev, &dev_attr_clock_gating);
#endif
#if (defined PLATFORM_HAS_RESET) && (PLATFORM_HAS_RESET == 1)
        device_create_file(aipu->dev, &dev_attr_reset);
#endif

        return ret;
}

void aipu_destroy_sysfs(void *aipu_priv)
{
        if (!aipu_priv)
                return;

        device_remove_file(aipu->dev, &dev_attr_kmd_version);
        device_remove_file(aipu->dev, &dev_attr_ext_register);
        device_remove_file(aipu->dev, &dev_attr_job);
#if (defined PLATFORM_HAS_CLOCK_GATING) && (PLATFORM_HAS_CLOCK_GATING == 1)
        device_remove_file(aipu->dev, &dev_attr_clock_gating);
#endif
#if (defined PLATFORM_HAS_RESET) && (PLATFORM_HAS_RESET == 1)
        device_remove_file(aipu->dev, &dev_attr_reset);
#endif
        aipu = NULL;
}
