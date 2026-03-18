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
#include <hw/inout.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/**
 *  @brief            mec170x lowlevel driver, heck mbx waitstate.
 *  @param io         io address.
 *  @param status     Agent status.
 *  @param cmd        mec170x mbx cmd
 *
 *  @return           EOK for success otherwise errno for failure.
 */
static int ecm_waitstatus(uintptr_t io, uint8_t status, uint8_t cmd)
{
    uint32_t idx;
    uint8_t res;

    /* Loop until time-out or Status */
    for (idx = 0; idx < EC_CMD_TIMEOUT; idx++) {
        res = in8((uint16_t)(io + MBX_RESOURCE_REGISTER));

        /* If status, done */
        if ((res & (AGENT_MASK(AGENT_USER))) == status) {
            return EOK;
        }
        /* Send command if needed */
        if (cmd == 0U) {
            continue;
        }
        /* Retry sending command when mailbox is free */
        for (; idx < EC_CMD_TIMEOUT; idx++) {
            /* Check busy bit */
            res = in8((uint16_t)(io + MBX_BUSY_REGISTER)) & EC_STATUS_REGISTER;
            if (res == 0U) {
                /* Send command */
                out8((uint16_t)(io + MBX_BUSY_REGISTER), cmd);
                break;
            }
        }
    }
    /* Time-out expired */
    return EAGAIN;
}

/**
 *  @brief            mec170x lowlevel driver, byte level mbx transfer.
 *  @param io         io address.
 *  @param reg        register address.
 *  @param operation  cmd operation.
 *  @param data       wr data.
 *  @param result     rd data
 *
 *  @return           EOK for success otherwise errno for failure.
 */
static int ecm_reg_byte_op(uintptr_t io, uint8_t reg, uint8_t operation,
                           uint8_t data, uint8_t *result)
{
    uint8_t res;

    /* Check still active */
    res = in8((uint16_t)(io + MBX_RESOURCE_REGISTER)) & (AGENT_ACTIVE(AGENT_USER));
    if (res == 0U) {
        return EBUSY;
    }
    /* Set the register index */
    out8((uint16_t)(io + EC_REGISTER_INDEX), reg);

    /* Check still active */
    res = in8((uint16_t)(io + MBX_RESOURCE_REGISTER)) & (AGENT_ACTIVE(AGENT_USER));
    if (res == 0U) {
        return EBUSY;
    }

    if (operation == MBX_READ) {
        if (result != NULL) {
            /* Read the data value */
            *result = in8((uint16_t)(io + EC_REGISTER_DATA));
        }
        else {
            return EINVAL;
        }
    }
    else if (operation == MBX_WRITE) {
        /* Write the data value */
        out8((uint16_t)(io + EC_REGISTER_DATA), data);
    }
    else {
        return EINVAL;
    }

    /* Check still active */
    res = in8((uint16_t)(io + MBX_RESOURCE_REGISTER)) & (AGENT_ACTIVE(AGENT_USER));
    if (res == 0U) {
        return EBUSY;
    }
    return EOK;
}

/*	Writing and reading function derived from the function above  */
#define ec_reg_byte_rd(io, reg, res) ecm_reg_byte_op(io, reg, MBX_READ, 0, res)
#define ec_reg_byte_wr(io, reg, val) ecm_reg_byte_op(io, reg, MBX_WRITE, val, NULL)

/**
 *  @brief            mec170x lowlevel driver, buffer level mbx transfer.
 *  @param ecm_dev_t  ecm device structure.
 *  @param cmd        mailbox cmd.
 *  @param tx_buf     tx buffer
 *  @param tx_size    tx size.
 *  @param rx_buf     rx buffer
 *  @param rx_size    rx size
 *
 *  @return           EOK for success otherwise errno for failure.
 */
static int ecm_send_command(ecm_dev_t *dev, uint8_t cmd, void *tx_buf,
                            uint8_t tx_size, void *rx_buf, uint8_t rx_size)
{
    int status;
    uint8_t *buf;
    uint32_t idx;
    uint8_t res;

    /* Wait for BIOS agent idle */
    status = ecm_waitstatus(dev->io, (AGENT_IDLE(AGENT_USER)), 0);
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Mailbox agent not available\n");
        goto err;
    }

    /* BIOS agent is idle: we can request access */
    status = ecm_waitstatus(dev->io, (AGENT_ACTIVE(AGENT_USER)),
                            (REQUEST_MBX_ACCESS(AGENT_USER)));
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Request mailbox agent failed\n");
        goto err;
    }

    if ((tx_size > EC_MBX_SIZE) || (rx_size > EC_MBX_SIZE)) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Tx/Rx size greater than mailbox size\n");
        status = EMSGSIZE;
        goto err;
    }

    /* Prepare MBX data */
    buf = (uint8_t *)tx_buf;
    for (idx = 0; idx < tx_size; idx++) {
        status = ec_reg_byte_wr(dev->io, EC_MBX_REGISTER + idx, buf[idx]);
        if (status != EOK) {
            break;
        }
    }

    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Mailbox buffer write failed\n");
        goto err;
    }

    /* Send command */
    status = ec_reg_byte_wr(dev->io, EC_COMMAND_REGISTER, cmd);
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Command write failed\n");
        goto err;
    }

    /* Wait for completion */
    status = ecm_waitstatus(dev->io, (AGENT_DONE(AGENT_USER)), 0);
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Mailbox did not complete after command write\n");
        goto err;
    }

    /* Get result code */
    status = ec_reg_byte_rd(dev->io, EC_RESULT_REGISTER, &res);
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR, "Result read failed\n");
        goto err;
    }

    /* Get result code and translate it */
    switch (res) {
    case EC_NO_ERROR:
        status = EOK;
        break;

    case EC_UNKNOWN_COMMAND_ERROR:
        status = EPERM;
        break;

    case EC_INVALID_ARGUMENT_ERROR:
        status = EINVAL;
        break;

    case EC_TIMEOUT_ERROR:
        status = EAGAIN;
        break;

    case EC_DEVICE_ERROR:
        status = ENODEV;
        break;

    default:
        status = EIO;
        break;
    }
    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "Command failed status=%d res=%d\n", status, res);
        goto err;
    }

    /* Read return data */
    buf = (uint8_t *)rx_buf;
    for (idx = 0; idx < rx_size; idx++) {
        status = ec_reg_byte_rd(dev->io, (uint8_t)(EC_MBX_REGISTER + idx), &buf[idx]);
        if (status != EOK) {
            break;
        }
    }

    if (status != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR, "Mailbox read failed\n");
        goto err;
    }

err:
    /* Release access, ignoring eventual time-out */
    if (ecm_waitstatus(dev->io, (AGENT_IDLE(AGENT_USER)),
                       (RELEASE_MBX_ACCESS(AGENT_USER))) != EOK) {
        slogf(_SLOG_SETCODE(_SLOG_SYSLOG, 0), _SLOG_ERROR,
              "failed to release access: ErrorCode %i\n", status);
    }

    return status;
}

/**
 *  @brief            mec170x lowlevel driver, implements get version read only
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_get_version(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;
    get_firmware_version_struct_t *version;

    version = dptr;
    *nbytes = sizeof(*version);
    dev_status = ecm_send_command(ecm_dev, GET_FIRMWARE_VERSION_CMD, NULL, 0, version,
                         (uint8_t)sizeof(get_firmware_version_struct_t));

    return dev_status;
}

/**
 *  @brief            mec170x lowlevel driver, implements i2c read write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_i2c_xfer(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;
    access_i2c_struct_ut *i2c_msg;

    i2c_msg = dptr;
    *nbytes = sizeof(*i2c_msg);
    dev_status = ecm_send_command(ecm_dev, ACCESS_I2C_CMD, i2c_msg,
                                  (uint8_t)(sizeof(access_i2c_parameter_t) +
                                            i2c_msg->parameters.i2cWriteCount),
                                  i2c_msg->i2cReadData,
                                  (uint8_t)(i2c_msg->parameters.i2cReadCount));

    return dev_status;
}

/**
 *  @brief            mec170x lowlevel driver, implements watchdog config write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_config(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;
    wdt_access_ut *config_ecm;

    config_ecm = dptr;
    *nbytes = sizeof(*config_ecm);
    dev_status = ecm_send_command(ecm_dev, WDT_CONFIG_CMD, config_ecm,
                                  (uint8_t)sizeof(wdt_access_ut), NULL, 0);

    return dev_status;
}

/**
 *  @brief            mec170x lowlevel driver, implements watchdog start write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_start(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;
    wdt_access_ut *config_ecm;

    config_ecm = dptr;
    *nbytes = sizeof(*config_ecm);
    dev_status = ecm_send_command(ecm_dev, WDT_START_CMD, config_ecm,
                                  (uint8_t)sizeof(wdt_access_ut), NULL, 0);

    return dev_status;
}

/**
 *  @brief            mec170x lowlevel driver, implements watchdog trigger write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_trigger(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;

    dev_status = ecm_send_command(ecm_dev, WDT_REFRESH_CMD, NULL, 0, NULL, 0);

    return dev_status;
}

/**
 *  @brief            mec170x lowlevel driver, implements watchdog stop write
 * command.
 *  @param ecm_dev_t  pointer to ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_stop(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes)
{
    int dev_status;

    dev_status = ecm_send_command(ecm_dev, WDT_STOP_CMD, NULL, 0, NULL, 0);

    return dev_status;
}