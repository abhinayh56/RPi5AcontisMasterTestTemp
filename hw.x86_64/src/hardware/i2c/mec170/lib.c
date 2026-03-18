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

int i2c_master_getfuncs(i2c_master_funcs_t* funcs, int tabsize)
{
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            init, mec170_i2c_init, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            fini, mec170_i2c_fini, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            send, mec170_i2c_send, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            recv, mec170_i2c_recv, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            set_slave_addr, mec170_i2c_set_slave_addr, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            set_bus_speed, mec170_i2c_set_bus_speed, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            version_info, mec170_i2c_version_info, tabsize);
    I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
            driver_info, mec170_i2c_driver_info, tabsize);
    return 0;
}
