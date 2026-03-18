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

i2c_status_t mec170_i2c_smbus_xfer(void *const hdl, const i2c_xfer_op_t op, void *const txbuf,
                                uint32_t const txlen, void *const rxbuf, uint32_t const rxlen)
{
    mec170_i2c_dev_t *dev = (mec170_i2c_dev_t *)hdl;
    access_i2c_struct_ut param;
    uint32_t i;
    int32_t status, ctl_sts;
    uint8_t command;
    i2c_status_t res;

    status = 0;
    command = 0u;

    (void)memcpy(&param, &(dev->param), sizeof(param));

    switch (op) {
    case (READWRITE):
            res = mec170_i2c_send(hdl, txbuf, txlen, 1);
            if (res != I2C_STATUS_DONE) {

                return I2C_STATUS_ERROR;
            }
            if (txlen > 0u) {
                command = ((unsigned char *)txbuf)[0];
            }
            /* fall down to read */
    case (READ):
            param.parameters.i2cWriteCount = 1;
            param.parameters.i2cReadCount = (uint8_t)rxlen;
            (void)memcpy(param.i2cWriteData, &command, param.parameters.i2cWriteCount);
        break;
    case (WRITE):
            param.parameters.i2cWriteCount = (uint8_t)(txlen + 1u);
            param.parameters.i2cReadCount = 0;
            command = ((unsigned char *)txbuf)[0];
            param.i2cWriteData[0] = command;
            for (i = 1; i <= txlen; i++) {
                param.i2cWriteData[i] = ((unsigned char *)txbuf)[i];
            }
        break;
    default:
        return I2C_STATUS_ERROR;
    }

    /* Delegate RD/WR to ECM resmgr */

    ctl_sts = devctl(dev->fd, DCMD_ECM_I2C_RD_WR, &param, sizeof(access_i2c_struct_ut), &status);

    if (ctl_sts != EOK) {
        i2c_slogf(dev->verbose, _SLOG_ERROR, "%s: devctl to ECM failed: %s", __func__, strerror(errno));

            return I2C_STATUS_ERROR;
    }

    if (status != EOK) {

        return I2C_STATUS_ERROR;
    }

    if ((op == READ) || (op == READWRITE)) {
        for (i = 0; i < rxlen; i++) {
            ((unsigned char *)rxbuf)[i] = param.i2cReadData[i];
        }
    }

    return I2C_STATUS_DONE;
}
