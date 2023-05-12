/*
 * This confidential and proprietary software should be used
 * under the licensing agreement from Allwinner Technology.
 *
 * Copyright (C) 2020 Allwinner Technology Limited
 * All rights reserved.
 *
 * Author: Albert Yu <yuxyun@allwinnertech.com>
 *
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from Allwinner Technology Limited.
 */

__kernel void vec3_add(__global const int *a, __global const int *b, __global int *c)
{
    int i = get_global_id(0);

    c[i] = a[i] + b[i];
}
