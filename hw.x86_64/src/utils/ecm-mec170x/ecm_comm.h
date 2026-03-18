/*
* Copyright (c) 2025, BlackBerry Limited. All rights reserved.
*
* BlackBerry Limited and its licensors retain all intellectual property and
* proprietary rights in and to this software and related documentation. Any
* use, reproduction, disclosure or distribution of this software and related
* documentation without an express license agreement from BlackBerry Limited
* is strictly prohibited.
*/

/*
 *  ecm_comm.h
 *
 *  Defines the midlevel helper functions between the ecm resource manager and mec170x HW
 * modules.
 *
 */

#ifndef ECM_ECM_COMM_H__
#define ECM_ECM_COMM_H__

#include "hw/ecm_mec170x.h"
#include <errno.h>
#include <hw/inout.h>
#include <secpol/secpol.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>
#include <sys/resmgr.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

#define ABID_ECM_INDX 6U

#define ABID_ECM_FW_VER "ecm_ops.fw_ver"
#define ABID_ECM_I2C_XFER "ecm_ops.i2c_xfer"
#define ABID_ECM_WDT_CONFIG "ecm_ops.wdt_config"
#define ABID_ECM_WDT_START "ecm_ops.wdt_start"
#define ABID_ECM_WDT_REFRESH "ecm_ops.wdt_refresh"
#define ABID_ECM_WDT_STOP "ecm_ops.wdt_stop"

typedef enum
{
    NUM_ABID_ECM_FW_VER,
    NUM_ABID_ECM_I2C_XFER,
    NUM_ABID_ECM_WDT_CONFIG,
    NUM_ABID_ECM_WDT_START,
    NUM_ABID_ECM_WDT_REFRESH,
    NUM_ABID_ECM_WDT_STOP,
} ability_num_et;

typedef struct ecm_dev_s
{
    uintptr_t io;
    int priority;
    int abids[ABID_ECM_INDX];
} ecm_dev_t;

typedef struct ecm_ocb_s
{
    iofunc_ocb_t ocb;
} ecm_ocb_t;

typedef struct ecm_resmgr_s
{
    resmgr_connect_funcs_t connect_funcs;
    resmgr_io_funcs_t io_funcs;
    dispatch_t *dpp;
    dispatch_context_t *ctp;
    int32_t resmgr_id;
} ecm_resmgr_t;

/**
 *  @brief             ECM resmgr permission init function, create promgr
 * acustom abilities.
 *  @param ecm_dev_t   ECM resmgr control struct.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_perms_init(ecm_dev_t *ecm_dev);

/**
 *  @brief             ECM resmgr permission check function.
 *  @param ctp         Resource manager context info.
 *  @param abid        procmgr ability id.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_perm(resmgr_context_t *ctp, uint32_t abid);

/**
 *  @brief             ECM resmgr r/w mode check function.
 *  @param ocb         Open Control Block structure.
 *  @param mode        client mode.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_access(iofunc_ocb_t *ocb, uint32_t mode);

/**
 *  @brief             ECM resmgr buffer integrity check function, for i2c
 * devctl command.
 *  @param ctp         Resource manager context info.
 *  @param msg         IO message.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_i2c(resmgr_context_t *ctp, io_devctl_t *msg);

/**
 *  @brief             ECM resmgr buffer integrity check function, for watchdog
 * devctl command.
 *  @param ctp         Resource manager context info.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_watchdog(resmgr_context_t *ctp);

/**
 *  @brief             ECM resmgr buffer integrity check function, for ecm
 * version devctl command.
 *  @param ctp         Resource manager context info.
 *
 *  @return            EOK for success otherwise errno for failure.
 */
int ecm_check_integrity_version(resmgr_context_t *ctp);

#endif