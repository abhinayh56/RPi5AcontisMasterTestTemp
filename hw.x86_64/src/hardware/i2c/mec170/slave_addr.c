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

int32_t mec170_i2c_set_slave_addr(void *hdl, uint32_t addr, i2c_addrfmt_t fmt)
{

    mec170_i2c_dev_t * const dev = (mec170_i2c_dev_t *)hdl;

    /* Currently only support 7bit addresses */
    if (fmt != I2C_ADDRFMT_7BIT) {
        i2c_slogf(dev->verbose, _SLOG_INFO, "Unsupported address format (%x), only 7bit allowed", fmt);
        errno = EINVAL;
        return -1;
    }

    dev->param.parameters.i2cAddress = (uint8_t)(addr << 1);

    return 0;
}

