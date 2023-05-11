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
 * @file zhouyi.c
 * Implementations of the zhouyi AIPU hardware control and interrupt handle operations
 */

#include "../../uk_interface/aipu_errcode.h"
#include "zhouyi.h"

int zhouyi_read_status_reg(struct io_region *io)
{
        return aipu_read32(io, ZHOUYI_STAT_REG_OFFSET);
}

void zhouyi_clear_qempty_interrupt(struct io_region *io)
{
        aipu_write32(io, ZHOUYI_STAT_REG_OFFSET, ZHOUYI_IRQ_QEMPTY);
}

void zhouyi_clear_done_interrupt(struct io_region *io)
{
        aipu_write32(io, ZHOUYI_STAT_REG_OFFSET, ZHOUYI_IRQ_DONE);
}

void zhouyi_clear_excep_interrupt(struct io_region *io)
{
        aipu_write32(io, ZHOUYI_STAT_REG_OFFSET, ZHOUYI_IRQ_EXCEP);
}

int zhouyi_query_cap(struct io_region *io, struct aipu_cap *cap)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if ((!io) || (!cap)) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        cap->isa_version = aipu_read32(io, ZHOUYI_ISA_VERSION_REG_OFFSET);
        cap->tpc_feature = aipu_read32(io, ZHOUYI_TPC_FEATURE_REG_OFFSET);
        cap->aiff_feature = aipu_read32(io, ZHOUYI_HWA_FEATURE_REG_OFFSET);

        /* success */
        cap->errcode = AIPU_ERRCODE_NO_ERROR;

finish:
        return ret;
}

void zhouyi_io_rw(struct io_region *io, struct aipu_io_req *io_req)
{
        if ((!io) || (!io_req)) {
                pr_err("invalid input args io/io_req to be NULL!");
                return;
        }

        /* TBD: offset r/w permission should be checked */

        if (io_req->rw == AIPU_IO_READ)
            io_req->value = aipu_read32(io, io_req->offset);
        else if (io_req->rw == AIPU_IO_WRITE)
            aipu_write32(io, io_req->offset, io_req->value);
        io_req->errcode = AIPU_ERRCODE_NO_ERROR;
}
