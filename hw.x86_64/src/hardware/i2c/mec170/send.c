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

i2c_status_t mec170_i2c_send(void*  hdl, void*  buf, uint32_t len, uint32_t  stop)
{
    mec170_i2c_dev_t* const dev = (mec170_i2c_dev_t *)hdl;

    if (stop == 0U) {
        i2c_slogf(dev->verbose, _SLOG_ERROR, "Non-stop condition is not supported");

        return I2C_STATUS_ERROR;
    }

    if (len > I2C_MAX_WRITE_LENGTH) {
        i2c_slogf(dev->verbose, _SLOG_ERROR, "Requested buffer size(%u) exceeds I2C transaction data limit(%d)", len, I2C_MAX_WRITE_LENGTH);

        return I2C_STATUS_ABORT;
    }

    if (len == 1u) { /*Only command, nothing to write*/

        return I2C_STATUS_DONE;
    }

    return mec170_i2c_smbus_xfer(dev, WRITE, buf, len, NULL, 0);
}

