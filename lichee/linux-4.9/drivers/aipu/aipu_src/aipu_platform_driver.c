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
 * @file platform_driver_init.c
 * Implementations of the AIPU platform driver probe/remove func
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/clk.h>
#include "config.h"
#include "aipu.h"

#if ((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE))
extern struct aipu_priv z1_platform_priv;
#endif
#if ((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE))
extern struct aipu_priv z2_platform_priv;
#endif

#ifdef CONFIG_OF
static const struct of_device_id aipu_of_match[] = {
#if ((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE))
        {
                .compatible = "armchina,zhouyi-v1-def",
                .data = (void*)&z1_platform_priv,
        },
        {
                .compatible = "armchina,zhouyiv1aipu",
                .data = (void*)&z1_platform_priv,
        },
#endif
#if ((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE))
        {
                .compatible = "armchina,zhouyi-v2-def",
                .data = (void*)&z2_platform_priv,
        },
        {
                .compatible = "armchina,zhouyi-v2",
                .data = (void*)&z2_platform_priv,
        },
#endif
        { }
};
#endif

MODULE_DEVICE_TABLE(of, aipu_of_match);

struct clk *clk_pll_aipu;
struct clk *clk_aipu;
struct clk *clk_aipu_slv;

/**
 * @brief remove operation registered to platfom_driver struct
 *        This function will be called while the module is unloading.
 * @param p_dev: platform devide struct pointer
 * @return 0 if successful; others if failed.
 */
static int aipu_remove(struct platform_device *p_dev)
{
        int ret = 0;
        struct device *dev = &p_dev->dev;
        struct aipu_priv *aipu = platform_get_drvdata(p_dev);

        if (!IS_ERR_OR_NULL(clk_aipu_slv))
        {
            clk_disable_unprepare(clk_aipu_slv);
            dev_info(dev, "clk_aipu_slv disable ok ");
        }

        if (!IS_ERR_OR_NULL(clk_aipu))
        {
            clk_disable_unprepare(clk_aipu);
            dev_info(dev, "clk_aipu disable ok");
        }
        clk_aipu     = NULL;
        clk_aipu_slv = NULL;
        clk_pll_aipu = NULL;

        aipu_priv_disable_interrupt(aipu);

        ret = deinit_aipu_priv(aipu);
        if (ret)
                return ret;

        /* success */
        dev_info(dev, "AIPU KMD remove done");
        return 0;
}

/**
 * @brief probe operation registered to platfom_driver struct
 *        This function will be called while the module is loading.
 *
 * @param p_dev: platform devide struct pointer
 * @return 0 if successful; others if failed.
 */
static int aipu_probe(struct platform_device *p_dev)
{
        int ret = 0;
        struct device *dev = &p_dev->dev;
        struct device_node *dev_node = dev->of_node;
        const struct of_device_id *of_id = NULL;
        struct resource *res = NULL;
        struct resource res_mem;
        struct aipu_priv *aipu = NULL;
        //struct device_node *np = NULL;
        int irqnum = 0;
        int cma_reserve_size = 0;
        u64 base = 0;
        u64 base_size = 0;

        dev_info(dev, "AIPU KMD probe start...\n");
        dev_info(dev, "[Probe 0/3] KMD version: %s %s\n", KMD_BUILD_DEBUG_FLAG, KMD_VERSION);

        /* match */
        of_id = of_match_node(aipu_of_match, dev_node);
        if (!of_id) {
                dev_err(dev, "[Probe 0/3] match node failed\n");
                return -EINVAL;
        }
        aipu = (struct aipu_priv *)of_id->data;

        if (aipu->version == AIPU_VERSION_ZHOUYI_V1)
                dev_info(dev, "[Probe 0/3] AIPU version: zhouyi-v1\n");
        else if (aipu->version == AIPU_VERSION_ZHOUYI_V2)
                dev_info(dev, "[Probe 0/3] AIPU version: zhouyi-v2\n");
        else
                dev_err(dev, "[Probe 0/3] Unrecognized AIPU version: 0x%x\n", aipu->version);

        ret = init_aipu_priv(aipu, dev);
        if (ret)
                return ret;

        /* get AIPU IO */
        res = platform_get_resource(p_dev, IORESOURCE_MEM, 0);
        if (!res) {
                dev_err(dev, "[Probe 1/3] get platform io region failed\n");
                ret = -EINVAL;
                goto probe_fail;
        }
        base = res->start;
        base_size = res->end - res->start + 1;
        dev_dbg(dev, "[Probe 1/3] get AIPU IO region: [0x%llx, 0x%llx]\n",
                base, res->end);

        /* get interrupt number */
        res = platform_get_resource(p_dev, IORESOURCE_IRQ, 0);
        if (!res) {
                dev_err(dev, "[Probe 1/3] get irqnum failed\n");
                ret = -EINVAL;
                goto probe_fail;
        }
        irqnum = res->start;
        dev_dbg(dev, "[Probe 1/3] get IRQ number: 0x%x\n", irqnum);

        ret = aipu_priv_init_core(aipu, irqnum, base, base_size);
        if (ret) {
                goto probe_fail;
        }

        /* get AIPU SoC IO, optional */
        res = platform_get_resource(p_dev, IORESOURCE_MEM, 1);
        if (!res) {
                dev_dbg(dev, "[Probe 1/3] get no platform scc region\n");
                base = 0;
                base_size = 0;
        } else {
                dev_info(dev, "[Probe 1/3] get SoC IO region: [0x%llx, 0x%llx]\n",
                        res->start, res->end - res->start + 1);
                base = res->start;
                base_size = res->end - res->start + 1;
        }
        ret = aipu_priv_init_soc(aipu, base, base_size);
        if (ret)
                goto probe_fail;
        dev_info(dev, "[Probe 1/3] Probe stage 1/3 done: create core\n");

#if 0
        /* get CMA reserved buffer info */
        np = of_parse_phandle(dev->of_node, "memory-region", 0);
        if (!np) {
                dev_err(dev, "[Probe 2/3] No %s specified\n", "memory-region");
                ret = -ENOMEM;
                goto probe_fail;
        }
        if (of_address_to_resource(np, 0, &res_mem))
                goto probe_fail;
        dev_dbg(dev, "[Probe 2/3] get CMA region: [0x%llx, 0x%llx]\n",
                res_mem.start, res_mem.end);

        ret = aipu_priv_add_mem_region(aipu, res_mem.start,
                res_mem.end - res_mem.start + 1, AIPU_MEM_TYPE_CMA);
        if (ret) {
                dev_err(dev, "[Probe 2/3] add new region failed\n");
                goto probe_fail;
        }
        of_node_put(np);
#endif

    /* get CMA reserved buffer info */
	ret = of_property_read_u32(dev->of_node, "cma-reserved-bytes", &cma_reserve_size);
	if (0 != ret) {
	        dev_err(dev, "get cma reserved size property failed!");
	        goto probe_fail;
	}

    ret = aipu_priv_add_mem_region(aipu, res_mem.start,cma_reserve_size, AIPU_MEM_TYPE_CMA);
    if (ret) {
            dev_err(dev, "[Probe 2/3] add new region failed\n");
            goto probe_fail;
    }
    dev_info(dev, "[Probe 2/3] get CMA size 0x%x\n", cma_reserve_size);

#if (defined AIPU_CONFIG_ENABLE_SRAM) && (AIPU_CONFIG_ENABLE_SRAM == 1)
        /* get SRAM reserved buffer info, optional */
        np = of_parse_phandle(dev->of_node, "sram-region", 0);
        if (np) {
                if (of_address_to_resource(np, 0, &res_mem))
                        goto probe_fail;
                dev_dbg(dev, "[Probe 2/3] get SRAM region: [0x%llx, 0x%llx]\n",
                        res_mem.start, res_mem.end);
                ret = aipu_priv_add_mem_region(aipu, res_mem.start,
                        res_mem.end - res_mem.start + 1, AIPU_MEM_TYPE_SRAM);
                if (ret) {
                        dev_err(dev, "[Probe 2/3] add new region failed\n");
                        goto probe_fail;
                }
        } else
                dev_dbg(dev, "[Probe 2/3] No %s specified\n", "sram-region");
        of_node_put(np);
#endif

        dev_info(dev, "[Probe 2/3] Stage 2/3 done: add memory region(s)\n");

        //set clock start
        clk_pll_aipu = of_clk_get(dev_node, 0);
        if (IS_ERR_OR_NULL(clk_pll_aipu)) {
            dev_err(dev, "clk_pll_aipu get failed\n");
            ret = PTR_ERR(clk_pll_aipu);
            goto probe_fail;
        }
        clk_aipu = of_clk_get(dev_node, 1);
        if (IS_ERR_OR_NULL(clk_aipu)) {
            dev_err(dev, "clk_aipu get failed\n");
            ret = PTR_ERR(clk_aipu);
            goto probe_fail;
        }
        clk_aipu_slv = of_clk_get(dev_node, 2);
        if (IS_ERR_OR_NULL(clk_aipu_slv)) {
            dev_err(dev, "clk_aipu_slv get failed\n");
            ret = PTR_ERR(clk_aipu_slv);
            goto probe_fail;
        }
        if (clk_set_parent(clk_aipu, clk_pll_aipu)) {
            dev_err(dev, "set clk_aipu parent fail\n");
            ret = -EBUSY;
            goto probe_fail;
        }
        if (clk_set_rate(clk_aipu, 600 * 1000000)) {
            dev_err(dev, "set clk_aipu rate fail\n");
            ret = -EBUSY;
            goto probe_fail;
        }
        if (clk_prepare_enable(clk_aipu_slv)) {
            dev_err(dev, "clk_aipu_slv enable failed\n");
            ret = -EBUSY;
            goto probe_fail;
        }
        if (clk_prepare_enable(clk_aipu)) {
            dev_err(dev, "clk_aipu enable failed\n");
            ret = -EBUSY;
            goto probe_fail;
        }

        dev_info(dev, "set aipu clock ok!");
        //set clock end

        aipu_priv_enable_interrupt(aipu);
        aipu_priv_print_hw_id_info(aipu);
        dev_info(dev, "[Probe 3/3] Stage 3/3 done: IO read/write\n");

        /* success */
        platform_set_drvdata(p_dev, aipu);
        dev_info(dev, "AIPU KMD probe done");
        goto finish;

        /* failed */
probe_fail:

        if(!IS_ERR_OR_NULL(clk_aipu_slv)) {
            clk_disable_unprepare(clk_aipu_slv);
        }
        if(!IS_ERR_OR_NULL(clk_aipu)) {
            clk_disable_unprepare(clk_aipu);
        }
        clk_aipu = NULL;
        clk_aipu_slv = NULL;
        clk_pll_aipu = NULL;

        deinit_aipu_priv(aipu);

finish:
        return ret;
}

static int aipu_suspend(struct platform_device *p_dev,pm_message_t state)
{
    struct device *dev = &p_dev->dev;
    struct aipu_priv *aipu = platform_get_drvdata(p_dev);

	if (aipu && aipu_priv_is_idle(aipu)) {
		dev_info(dev, "aipu in idle status !");
	} else {
		dev_err(dev,"aipu in busy status !");
		return -1;
	}

	if (!IS_ERR_OR_NULL(clk_aipu_slv)) {
		clk_disable_unprepare(clk_aipu_slv);
		dev_info(dev, "disable clk_aipu_slv ok");
	}
	if (!IS_ERR_OR_NULL(clk_aipu)) {
		clk_disable_unprepare(clk_aipu);
		dev_info(dev, "disable clk_aipu ok");
	}
	dev_info(dev, "aipu_suspend ok");

	return 0;
}

static int aipu_resume(struct platform_device *p_dev)
{
    struct device *dev = &p_dev->dev;
    struct aipu_priv *aipu = platform_get_drvdata(p_dev);

	if(NULL == aipu) {
		dev_err(dev, "aipu is null ,resume fail...!");
		return -1;
	}

	if (clk_set_parent(clk_aipu, clk_pll_aipu)) {
		dev_err(dev, "set clk_aipu parent fail\n");
	}
	if (clk_set_rate(clk_aipu, 600 * 1000000)) {
		dev_err(dev, "set clk_aipu rate fail\n");
	}
	if (clk_prepare_enable(clk_aipu_slv)) {
		dev_err(dev, "clk_aipu_slv enable failed\n");
	}
	if (clk_prepare_enable(clk_aipu)) {
		dev_err(dev, "clk_aipu enable failed\n");
	}

    aipu_priv_enable_interrupt(aipu);
    aipu_priv_print_hw_id_info(aipu);
	dev_info(dev, "aipu_resume ok.");

	return 0;
}

static struct platform_driver aipu_platform_driver = {
        .probe = aipu_probe,
        .remove = aipu_remove,
        .suspend = aipu_suspend,
        .resume  = aipu_resume,
        .driver = {
                .name = "armchina-aipu",
                .owner = THIS_MODULE,
                .of_match_table = of_match_ptr(aipu_of_match),
        },
};

module_platform_driver(aipu_platform_driver);
MODULE_LICENSE("GPL");
