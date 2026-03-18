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

int32_t mec170_i2c_version_info(i2c_libversion_t *version)
{
    version->major = I2CLIB_VERSION_MAJOR;
    version->minor = I2CLIB_VERSION_MINOR;
    version->revision = I2CLIB_REVISION;

    return 0;
}

