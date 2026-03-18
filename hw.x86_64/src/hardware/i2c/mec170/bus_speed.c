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

int32_t mec170_i2c_set_bus_speed(void *hdl, uint32_t speed, uint32_t *ospeed)
{
    mec170_i2c_dev_t *dev = (mec170_i2c_dev_t *)hdl;

    switch (speed) {
    case MEC_I2C_SPEED_1MHZ:
        dev->param.parameters.i2cSpeed = (uint8_t)SMB_1MHZ;
        break;

    case MEC_I2C_SPEED_100KHZ:
        dev->param.parameters.i2cSpeed = (uint8_t)SMB_100KHZ;
        break;

    case MEC_I2C_SPEED_400KHZ:
        dev->param.parameters.i2cSpeed = (uint8_t)SMB_400KHZ;
        break;

    default:
        i2c_slogf(dev->verbose, _SLOG_INFO, "Requested speed(%d) is not supported, setting default(%d)",
                                                                            speed, MEC_I2C_SPEED_100KHZ);
        speed = MEC_I2C_SPEED_100KHZ;
        dev->param.parameters.i2cSpeed = (uint8_t)SMB_100KHZ;
    }

    /* Return speed if a buffer pointer is provided */
    if (ospeed != NULL) {
        *ospeed = speed;
    }
    return 0;
}
