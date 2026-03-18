/*
* Copyright (c) 2025, BlackBerry Limited. All rights reserved.
*
* BlackBerry Limited and its licensors retain all intellectual property and
* proprietary rights in and to this software and related documentation. Any
* use, reproduction, disclosure or distribution of this software and related
* documentation without an express license agreement from BlackBerry Limited
* is strictly prohibited.
*/

#include "MEC170X_def.h"
#include "hw/ecm_mec170x.h"
#include "ecm_comm.h"
#include <errno.h>
#include <fcntl.h>
#include <hw/inout.h>
#include <pthread.h>
#include <secpol/secpol.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>
#include <sys/iomgr.h>
#include <sys/iomsg.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <sys/resmgr.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/* ECM Device global structure, holds io def and permissions */
static ecm_dev_t ecm_dev;

/**
 *  @brief             Parses command-line options for the ECM resmgr.
 *  @param ecm_dev_t     A pointer to the io_gpio_t structure where parsed
 * values are stored.
 *  @param argc        The number of command-line arguments
 *  @param argv        The array of command-line arguments.
 *
 *  @return            EOK on success, or an error code (EINVAL) if invalid
 * options are encountered.
 */
static int32_t ecm_parseopts(ecm_dev_t *dev, int argc, char *argv[])
{
    int c;
    int status = EOK;

    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch ((char)c) {
        case 'p':
            dev->priority = (int)strtol(optarg, NULL, 0);
            if ((errno != 0) || (dev->priority <= 1) || (dev->priority >= 254))
            {
                slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_INFO,
                      "Invalid priority parameter\n");
                status = EINVAL;
            }
            break;

        default:
            slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_INFO,
                  "Unsupported option -%c\n", optopt);
            status = EINVAL;
            break;
        }
    }

    return status;
}

/**
 *  @brief             devctl handling.
 *  @param ctp         Resource manager context info.
 *  @param msg         devctl message.
 *  @param eocb_i      Open Control Block structure.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
static int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, void *eocb_i)
{
    int status = 0;
    void *dptr;
    size_t nbytes;
    int dev_status = 0;
    ecm_ocb_t *eocb = eocb_i;

    status = iofunc_devctl_default(ctp, msg, &eocb->ocb);
    if (status != _RESMGR_DEFAULT) {
        return status;
    }

    nbytes = 0;
    dptr = _IO_INPUT_PAYLOAD(msg);

    switch (msg->i.dcmd) {
    case DCMD_ECM_I2C_RD_WR:

        status = ecm_check_integrity_i2c(ctp, msg);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_access(&eocb->ocb, _IO_FLAG_RD | _IO_FLAG_WR);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_I2C_XFER]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_i2c_xfer(&ecm_dev, dptr, &nbytes);
        break;

    case DCMD_ECM_GETVER:

        status = ecm_check_integrity_version(ctp);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_access(&eocb->ocb, _IO_FLAG_RD);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_FW_VER]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_get_version(&ecm_dev, dptr, &nbytes);
        break;

    case DCMD_ECM_WDT_CONFIG:
        status = ecm_check_integrity_watchdog(ctp);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_access(&eocb->ocb, _IO_FLAG_WR);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_WDT_CONFIG]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_wdt_config(&ecm_dev, dptr, &nbytes);
        break;

    case DCMD_ECM_WDT_START:
        status = ecm_check_integrity_watchdog(ctp);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_access(&eocb->ocb, _IO_FLAG_WR);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_WDT_START]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_wdt_start(&ecm_dev, dptr, &nbytes);
        break;

    case DCMD_ECM_WDT_TRIGGER:
        status = ecm_check_access(&eocb->ocb, _IO_FLAG_WR);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_WDT_REFRESH]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_wdt_trigger(&ecm_dev, NULL, NULL);
        break;

    case DCMD_ECM_WDT_STOP:
        status = ecm_check_access(&eocb->ocb, _IO_FLAG_WR);
        if (status != EOK) {
            return status;
        }

        status = ecm_check_perm(ctp, ecm_dev.abids[NUM_ABID_ECM_WDT_STOP]);
        if (status != EOK) {
            return status;
        }

        dev_status = mec_wdt_stop(&ecm_dev, NULL, NULL);
        break;

    default:
        return EINVAL;
    }
    msg->o.ret_val = dev_status;
    msg->o.nbytes = (uint32_t)nbytes;
    MsgReply(ctp->rcvid, EOK, msg, sizeof(*msg) + nbytes);
    return _RESMGR_NOREPLY;
}

/**
 *  @brief             Main function.
 *  @param void        NONE
 *
 *  @return            EXIT_SUCCESS on success or EXIT_FAILURE for failures.
 */

int main(int argc, char **argv)
{
    int ret;
    iofunc_attr_t ioattr;
    ecm_resmgr_t ecm_resmgr;

    ret = ThreadCtl_r(_NTO_TCTL_IO, NULL);
    if (ret < 0) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "ThreadCtl_r failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    if (procmgr_daemon(EXIT_SUCCESS,
                       PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL) == -1) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "procmgr_daemon failed ");
        exit(EXIT_FAILURE);
    }

    ecm_dev.priority = -1;

    ret = ecm_parseopts(&ecm_dev, argc, argv);
    if (ret != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_INFO,
              "using default kernel priority\n");
    }

    if (ecm_dev.priority > 0) {
        if (pthread_setschedprio(pthread_self(), ecm_dev.priority) != EOK) {
            slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
                  "Can't change priority");
            exit(EXIT_FAILURE);
        }
    }

    secpol_file_t *const secpol_hdl =
        secpol_open(NULL, (uint32_t)SECPOL_USE_AS_DEFAULT);

    ecm_dev.io = MBX_BASE;

    ret = ecm_perms_init(&ecm_dev);
    if (ret != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "ecm perm init failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    ecm_resmgr.dpp = dispatch_create_channel(-1, DISPATCH_FLAG_NOLOCK);
    if (ecm_resmgr.dpp == NULL) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "dispatch_create_channel failed%s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_INFO, "Starting ECM Resmgr\n");
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &ecm_resmgr.connect_funcs,
                     _RESMGR_IO_NFUNCS, &ecm_resmgr.io_funcs);

    ecm_resmgr.io_funcs.devctl = io_devctl;
    ecm_resmgr.io_funcs.lock = NULL;
    ecm_resmgr.io_funcs.read = NULL;
    ecm_resmgr.io_funcs.write = NULL;
    ecm_resmgr.io_funcs.unblock = NULL;
    ecm_resmgr.io_funcs.lock_ocb = NULL;
    ecm_resmgr.io_funcs.unlock_ocb = NULL;

    iofunc_attr_init(&ioattr, S_IFCHR | 0666, NULL, NULL);

    ecm_resmgr.resmgr_id = secpol_resmgr_attach(
        NULL, ecm_resmgr.dpp, NULL, MEC_CHIP_PATHNAME, _FTYPE_ANY, 0,
        &ecm_resmgr.connect_funcs, &ecm_resmgr.io_funcs, &ioattr, NULL);
    if (ecm_resmgr.resmgr_id == -1) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "secpol resmgr attach failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    ecm_resmgr.ctp = dispatch_context_alloc(ecm_resmgr.dpp);
    if (ecm_resmgr.ctp == NULL) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "dispatch_context_alloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Transition security type before entering main loop */
    if (secpol_transition_type(NULL, NULL, 0) == -1) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_INFO,
              "secpol_transition_type failed: %s\n", strerror(errno));
    }

    secpol_close(secpol_hdl);

    while (true) {
        if (dispatch_block(ecm_resmgr.ctp) != NULL) {
            if (dispatch_handler(ecm_resmgr.ctp) == -1) {
                break;
            }
        }
        else {
            slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
                  " dispatch_block failed: %s\n", strerror(errno));
            break;
        }
    }

    return EXIT_FAILURE;
}