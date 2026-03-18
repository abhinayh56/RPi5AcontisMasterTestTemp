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

void mec170_i2c_fini(void *hdl)
{
    mec170_i2c_dev_t *dev = (mec170_i2c_dev_t *)hdl;

    if (dev == NULL) {
        return;
    }

    if (dev->fd != -1) {
        (void)close(dev->fd);
    }

    free(hdl);
}

void* mec170_i2c_init(int argc, char *argv[])
{
    mec170_i2c_dev_t *dev;
    uint32_t loglevel = _SLOG_ERROR;

    dev = calloc(1, sizeof(*dev));  // allocate device object
    if (dev == NULL) {
        return NULL;
    }

    /* Set default values */
    dev->param.parameters.i2cBus = 10;
    dev->param.parameters.i2cSpeed = (uint8_t)SMB_100KHZ;
    dev->fd = -1;

    /* Initialize access to ECM ResMgr */
    dev->fd = open(MEC_CHIP_PATHNAME, O_RDWR);

    if (dev->fd == -1) {
        i2c_slogf(loglevel, _SLOG_ERROR, "%s: Access to ecm resmgr failed : %s", __func__, strerror(errno));
        goto fail_cleanup;
    }

    /* Parse options */
    if (mec170_i2c_parseopts(dev, argc, argv) == -1) {
        goto fail_cleanup;
    }

    return dev;

fail_cleanup:
    mec170_i2c_fini(dev);
    return NULL;
}
