/*
* Copyright (c) 2025, BlackBerry Limited. All rights reserved.
*
* BlackBerry Limited and its licensors retain all intellectual property and
* proprietary rights in and to this software and related documentation. Any
* use, reproduction, disclosure or distribution of this software and related
* documentation without an express license agreement from BlackBerry Limited
* is strictly prohibited.
*/

#include "ecm_comm.h"
#include "MEC170X_def.h"
#include "hw/ecm_mec170x.h"
#include <errno.h>
#include <fcntl.h>
#include <hw/inout.h>
#include <pthread.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/mman.h>
#include <sys/procmgr.h>
#include <sys/resmgr.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/*--------------------------------------------------------------------------*
 *                             Helper function                              *
 *--------------------------------------------------------------------------*/
/**
 *  @brief             ECM resmgr permission init function, create promgr
 * acustom abilities.
 *  @param ecm_dev_t   Pointer to ECM resmgr control struct.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_perms_init(ecm_dev_t *ecm_dev)
{
    const char *const ability_names[ABID_ECM_INDX] = {
        ABID_ECM_FW_VER,
        ABID_ECM_I2C_XFER,
        ABID_ECM_WDT_CONFIG,
        ABID_ECM_WDT_START,
        ABID_ECM_WDT_REFRESH,
        ABID_ECM_WDT_STOP,
    };

    /* Custom ability domains */
    const uint32_t ability_domains[ABID_ECM_INDX] = {
        PROCMGR_ADN_ROOT | PROCMGR_ADN_NONROOT,
        PROCMGR_ADN_ROOT,
        PROCMGR_ADN_ROOT,
        PROCMGR_ADN_ROOT,
        PROCMGR_ADN_ROOT | PROCMGR_ADN_NONROOT,
        PROCMGR_ADN_ROOT,
    };

    for (unsigned i = 0; i < ABID_ECM_INDX; i++) {
        ecm_dev->abids[i] =
            procmgr_ability_create(ability_names[i], ability_domains[i]);
        if (ecm_dev->abids[i] <= -1) {
            slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
                  "procmgr_ability_create for '%s' failed: %s(%d)", ability_names[i],
                  strerror(-ecm_dev->abids[i]), -ecm_dev->abids[i]);
            return (-ecm_dev->abids[i]);
        }
    }

    return EOK;
}

/**
 *  @brief             ECM resmgr permission check function.
 *  @param ctp         Resource manager context info.
 *  @param abid        procmgr ability id.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_perm(resmgr_context_t *ctp, uint32_t abid)
{
    struct _client_able able_check = {.ability = abid};

    int const ret =
        ConnectClientInfoAble(ctp->info.scoid, NULL, 0, &able_check, 1);
    if (ret == -1) {
        /* If anything goes wrong, default to denying access */
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "ConnectClientInfoAble failed: %s", strerror(errno));
        return EPERM;
    }

    if (able_check.flags == 1U) {
        return EOK;
    }
    else {
        return EPERM;
    }
}

/**
 *  @brief             ECM resmgr r/w mode check function.
 *  @param ocb         Open Control Block structure.
 *  @param mode        client mode.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_access(iofunc_ocb_t *ocb, uint32_t mode)
{
    int status;

    if ((ocb->ioflag & mode) == 0U) {
        status = EPERM;
    }
    else {
        status = EOK;
    }

    return status;
}

/**
 *  @brief             ECM resmgr buffer integrity check function, for i2c
 * devctl command.
 *  @param ctp         Resource manager context info.
 *  @param msg         IO message.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_i2c(resmgr_context_t *ctp, io_devctl_t *msg)
{
    /*Header-complete*/
    size_t need = sizeof(io_devctl_t) + sizeof(access_i2c_parameter_t);
    if (need > ctp->size) {
        return EBADMSG;
    }

    /*Payload-complete*/
    access_i2c_struct_ut *ih = _IO_INPUT_PAYLOAD(msg);
    need += ih->parameters.i2cWriteCount;
    if (need > ctp->size) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "i2c payload incomplete\n");
        return EBADMSG;
    }

    return EOK;
}

/**
 *  @brief             ECM resmgr buffer integrity check function, for watchdog
 * devctl command.
 *  @param ctp         Resource manager context info.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_watchdog(resmgr_context_t *ctp)
{
    size_t need = sizeof(io_devctl_t) + sizeof(wdt_access_ut);
    if (need > ctp->size) {
        return EBADMSG;
    }

    return EOK;
}

/**
 *  @brief             ECM resmgr buffer integrity check function, for ecm
 * version devctl command.
 *  @param ctp         Resource manager context info.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_version(resmgr_context_t *ctp)
{
    size_t need = sizeof(io_devctl_t) + sizeof(get_firmware_version_struct_t);
    if (need > ctp->info.dstmsglen) {
        return EBADMSG;
    }

    return EOK;
}