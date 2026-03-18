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

int32_t mec170_i2c_parseopts(mec170_i2c_dev_t * dev, int argc, char *argv[])
{
    bool done = false;
    int32_t  prev_optind, c;
    uint8_t arg_val;

    while (done == false) {
        prev_optind = optind;
        c = getopt(argc, argv, "d:v");
        errno = EOK;

        switch ((char)c) {
        case 'd':
            arg_val = (uint8_t)strtoul(optarg, NULL, 0);
            if ((errno == EOK) && (arg_val < I2C_MAX_BUS_NUM)) {
                dev->param.parameters.i2cBus = arg_val;
            } else {
                i2c_slogf(dev->verbose, _SLOG_ERROR, "invalid bus number");
                return -1;
            }
            break;
        case 'v':
            dev->verbose++;
            break;
        case '?':
            if (optopt == (int32_t)'-') {
                ++optind;
                break;
            }
            return -1;

        case (char)-1:
            if (prev_optind < optind) { /* -- */
                return -1;
            }

            if (argv[optind] == NULL) {
                done = true;
                break;
            }
            if (*argv[optind] != '-') {
                ++optind;
                break;
            }
            return -1;

        case ':':
        default:
            return -1;
        }
    }

    return 0;
}