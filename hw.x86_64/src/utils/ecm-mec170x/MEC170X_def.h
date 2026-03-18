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
 *  MEC170X_def.h
 *
 *  Defines the lowlevel interface between the ecm resource manager and mec170x HW
 * modules.
 *
 */

#ifndef ECM_MEC170X_DEF_H__
#define ECM_MEC170X_DEF_H__

#include "hw/ecm_mec170x.h"
#include "ecm_comm.h"
#include <stdio.h>
#include <sys/iomgr.h>
#include <sys/iomsg.h>
#include <threads.h>

/* Lowlevel MailBox definitions */
#define MBX_BASE 0x2b0U
#define MBX_RESERVED_BASE 0x0U

#define BAR_FROM_MBX_BASE(x) (x + MBX_RESERVED_BASE)

#define RES_BAR_OFFSET 0U
#define BSY_BAR_OFFSET 4U
#define MBX_BAR_OFFSET 0xcU

#define MBX_RESOURCE_REGISTER (BAR_FROM_MBX_BASE(RES_BAR_OFFSET))
#define MBX_BUSY_REGISTER (BAR_FROM_MBX_BASE(BSY_BAR_OFFSET))
#define MBX_ACCESS_BAR (BAR_FROM_MBX_BASE(MBX_BAR_OFFSET))

#define EC_REGISTER_INDEX MBX_ACCESS_BAR
#define EC_REGISTER_DATA (EC_REGISTER_INDEX + 1U)

#define EC_COMMAND_REGISTER 0U
#define EC_RESULT_REGISTER 1U
#define EC_STATUS_REGISTER 2U
#define EC_MBX_REGISTER 0x10U

#define EC_CMD_TIMEOUT 0x30000U //  Maximum wait loop

/**
 * @brief   MBX Status bitmap values from E.C. to Host.
 * @note    Refer to Microchip ecmx DS00002206D data-sheet for naming and
 *          description.
 * @details The bitmap refers to the data byte of the ACPI-ECI EC0 device, plus
 *          bit 0 for init/deinit. All the agents will have an QUEUED and an
 *          ACTIVE bit to register the activity; this allows at most 4 distinct
 *          agents: see the macros following this enum definition.
 */

#define MBX_OFF 0U        /*!< Disable MBX Interface */
#define MBX_ON 1U         /*!< Enable MBX Interface  */
#define MBX_ACTIVE0 0x40U /*!< MBX AGENT 0 active    */
#define MBX_QUEUED0 0x80U /*!< MBX AGENT 0 idle      */

#define AGENT_IDLE(x) (0U)
#define AGENT_QUEUED(x) (MBX_QUEUED0 >> (2U * x))
#define AGENT_ACTIVE(x) (MBX_ACTIVE0 >> (2U * x))
#define AGENT_MASK(x) (AGENT_QUEUED(x) + AGENT_ACTIVE(x))
#define AGENT_DONE(x) (AGENT_MASK(x))
#define MBX_STATUS_DEFAULT 0U

#define AGENT_BIOS 0U /*!< BIOS AGENT */
#define AGENT_ACPI 1U /*!< ACPI AGENT */
#define AGENT_EAPI 2U /*!< EAPI AGENT */
#define AGENT_USER 3U /*!< USER AGENT */
#define AGENT_NONE 4U /*!< No AGENT   */

#define EC_NO_ERROR 0U               /*!< Success          */
#define EC_UNKNOWN_COMMAND_ERROR 1U  /*!< Unknown command  */
#define EC_INVALID_ARGUMENT_ERROR 2U /*!< Invalid argument */
#define EC_TIMEOUT_ERROR 3U          /*!< Waiting Time-out */
#define EC_DEVICE_ERROR 4U           /*!< Device error     */

#define REQUEST_MBX_ACCESS_CMD \
    0xf0U /*!< First request access command           */
#define RELEASE_MBX_ACCESS_CMD \
    0xf8U /*!< First release access command           */
#define GET_FIRMWARE_VERSION_CMD \
    0U                        /*!< Get firmware version record            */
#define ACCESS_I2C_CMD 0x10U  /*!< Access an I2C channel                  */
#define WDT_CONFIG_CMD 0x60U  /*!< WDT configuration                      */
#define WDT_START_CMD 0x61U   /*!< WDT start                              */
#define WDT_REFRESH_CMD 0x62U /*!< WDT software refresh                   */
#define WDT_STOP_CMD 0x63U    /*!< WDT stop                               */

#define REQUEST_MBX_ACCESS(x) (REQUEST_MBX_ACCESS_CMD + x)
#define RELEASE_MBX_ACCESS(x) (RELEASE_MBX_ACCESS_CMD + x)

#define FIRMWARE_TIME_STAMP_SIZE (EC_MBX_SIZE - sizeof(uint32_t))

#define MBX_READ 0x1U
#define MBX_WRITE 0x2U

/**
 *  @brief            mec170x lowlevel driver, implements get version read only
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_get_version(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);

/**
 *  @brief            mec170x lowlevel driver, implements i2c read write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_i2c_xfer(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);

/**
 *  @brief            mec170x lowlevel driver, implements watchdog config write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_config(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);

/**
 *  @brief            mec170x lowlevel driver, implements watchdog start write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_start(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);

/**
 *  @brief            mec170x lowlevel driver, implements watchdog trigger write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_trigger(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);

/**
 *  @brief            mec170x lowlevel driver, implements watchdog stop write
 * command.
 *  @param ecm_dev_t  ecm device structure.
 *  @param dptr       kernel data pointer
 *  @param nbytes     number of bytes.
 *
 *  @return           EOK for success otherwise errno for failure.
 */
int mec_wdt_stop(ecm_dev_t *ecm_dev, void *dptr, size_t *nbytes);
#endif