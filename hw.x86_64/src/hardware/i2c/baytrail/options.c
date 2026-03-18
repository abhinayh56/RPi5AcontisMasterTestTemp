/*
 * Copyright (c) 2015, 2022, BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 */


#include <unistd.h>
#include <pci/pci_id.h>
#include "proto.h"

int baytrail_i2c_parseopts(baytrail_dev_t *dev, int argc, char *argv[])
{
    int c;
    int prev_optind;
    int done = 0;

    dev->idx        = 0;
    dev->vid        = PCI_VID_Intel_Corp;
    dev->did        = PCI_DID_ANY;
    dev->master_cfg = DW_IC_CON_MASTER | DW_IC_CON_SLAVE_DISABLE | DW_IC_CON_RESTART_EN | DW_IC_CON_SPEED_STD | DW_IC_CON_7BITADDR_MASTER;
    dev->scl_freq   = 0;
    dev->clock_khz  = 0;
    dev->verbose    = _SLOG_ERROR;
    dev->irq        = -1;
    dev->iid        = -1;
    dev->sda_hold_time = 0;
    dev->fixed_scl  = 0;

    while (!done) {
        prev_optind = optind;
        c = getopt(argc, argv, "c:d:h:i:Iq:r:v");
        switch (c) {
        case 'i':
            dev->idx = (unsigned)strtoul(optarg, NULL, 0);
            break;
        case 'd':
            dev->did = (pci_did_t)strtoul(optarg, NULL, 0);
            break;
        case 'r':
            dev->vid = (pci_vid_t)strtoul(optarg, NULL, 0);
            break;
        case 'q':
            dev->irq = (pci_irq_t)strtoul(optarg, NULL, 0);
            break;
        case 'c':
            dev->clock_khz = (uint32_t)strtoul(optarg, NULL, 0) / 1000;
            break;
        case 'h':
            dev->sda_hold_time = (unsigned)strtoul(optarg, NULL, 0);
            break;
        case 'v':
            dev->verbose++;
            break;
        case 'I':
            dev->fixed_scl = 1;
            break;
        case '?':
            if (optopt == '-') {
                ++optind;
                break;
            }
            return -1;
        case -1:
            if (prev_optind < optind) return -1;    /* -- */

            if (argv[optind] == NULL) {
                done = 1;
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
