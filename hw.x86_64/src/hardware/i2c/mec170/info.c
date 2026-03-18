/*
 * Copyright (c) 2025, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */


#include "proto.h"

int32_t mec170_i2c_driver_info(void * hdl, i2c_driver_info_t* info)
{
    mec170_i2c_dev_t * const dev = hdl;

    info->speed_mode = I2C_SPEED_STANDARD | I2C_SPEED_FAST;
    info->addr_mode = (uint32_t)I2C_ADDRFMT_7BIT;
    info->verbosity = dev->verbose;

    return 0;
}
