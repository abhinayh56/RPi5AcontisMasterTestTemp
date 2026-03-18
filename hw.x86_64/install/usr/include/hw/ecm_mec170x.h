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
 *  ecm_mec170x.h
 *
 *  Defines the interfaces between the ecm resmgr and clients.
 *
 */
#ifndef ECM_DEVICE_COMMON_H__
#define ECM_DEVICE_COMMON_H__
#include <devctl.h>
#include <stdio.h>
#include <sys/iomgr.h>
#include <sys/iomsg.h>

/* Procmgr Custom Abilities Defs */
#define ABID_ECM_FW_VER "ecm_ops.fw_ver"
#define ABID_ECM_I2C_XFER "ecm_ops.i2c_xfer"
#define ABID_ECM_WDT_CONFIG "ecm_ops.wdt_config"
#define ABID_ECM_WDT_START "ecm_ops.wdt_start"
#define ABID_ECM_WDT_REFRESH "ecm_ops.wdt_refresh"
#define ABID_ECM_WDT_STOP "ecm_ops.wdt_stop"

#define MEC_CHIP_PATHNAME "/dev/mec170x"

#define EC_MBX_SIZE 0x20U /* MEC170x MBX size info for further checks */

/* F/W interface Level Defs */
#define FIRMWARE_TIME_STAMP_SIZE (EC_MBX_SIZE - sizeof(uint32_t))
typedef struct get_firmware_version_struct
{
    union
    {
        uint32_t versions;
        struct
        {
            union
            {
                uint16_t firmwareVersion;
                struct
                {
                    uint8_t firmwareMinorVersion;
                    uint8_t firmwareMajorVersion;
                };
            };
            union
            {
                uint16_t libraryVersion;
                struct
                {
                    uint8_t libraryMinorVersion;
                    uint8_t libraryMajorVersion;
                };
            };
        };
    } version;
    uint8_t firmwareTimeStamp[FIRMWARE_TIME_STAMP_SIZE];
} get_firmware_version_struct_t;

/* WatchDog interface Level Defs */

typedef union wdt_cfg
{
    uint8_t config;
    struct
    {
        uint8_t eventConfig : 2;
        uint8_t resetConfig : 2;
        uint8_t status : 2;
    };
} wdt_cfg_ut;

typedef union wdt_access
{
    struct
    {
        uint8_t refreshPin;
        uint8_t signalPin;
        wdt_cfg_ut config;
    };
    struct
    {
        uint32_t delayTimeout;
        uint32_t eventTimeout;
        uint32_t resetTimeout;
    };
} wdt_access_ut;

/* I2C interface Level Defs */

typedef struct access_i2c_parameter
{
    uint8_t i2cBus;
    uint8_t i2cSpeed;
    uint8_t i2cAddress;
    uint8_t i2cWriteCount;
    uint8_t i2cReadCount;
} access_i2c_parameter_t;

#define ACCESS_I2C_CMD_OVERHEAD (sizeof(access_i2c_parameter_t))
#define I2C_MAX_WRITE_LENGTH (EC_MBX_SIZE - ACCESS_I2C_CMD_OVERHEAD)
#define I2C_MAX_READ_LENGTH EC_MBX_SIZE

typedef union access_i2c_struct
{
    struct
    {
        access_i2c_parameter_t parameters;
        uint8_t i2cWriteData[I2C_MAX_WRITE_LENGTH];
    };
    uint8_t i2cReadData[I2C_MAX_READ_LENGTH];
} access_i2c_struct_ut;

/* devctl command ids */
#define MAX_FORMAT_SIZE 64U
#define DCMD_ACCESS_I2C_CMD 0x10U
#define DCMD_GET_FIRMWARE_VERSION_CMD 0U
#define DCMD_ECM_WD_CONFIG 0x60U
#define DCMD_ECM_WD_START 0x61U
#define DCMD_WDT_REFRESH_CMD 0x62U
#define DCMD_WDT_STOP_CMD 0x63U

/* devctl commands defs */
#define DCMD_ECM_I2C_RD_WR                   \
    __DIOTF(_DCMD_MISC, DCMD_ACCESS_I2C_CMD, \
            access_i2c_struct_ut[MAX_FORMAT_SIZE])
#define DCMD_ECM_GETVER                               \
    __DIOF(_DCMD_MISC, DCMD_GET_FIRMWARE_VERSION_CMD, \
           get_firmware_version_struct_t[MAX_FORMAT_SIZE])
#define DCMD_ECM_WDT_CONFIG \
    __DIOT(_DCMD_MISC, DCMD_ECM_WD_CONFIG, wdt_access_ut[MAX_FORMAT_SIZE])
#define DCMD_ECM_WDT_START \
    __DIOT(_DCMD_MISC, DCMD_ECM_WD_START, wdt_access_ut[MAX_FORMAT_SIZE])
#define DCMD_ECM_WDT_TRIGGER __DION(_DCMD_MISC, DCMD_WDT_REFRESH_CMD)
#define DCMD_ECM_WDT_STOP __DION(_DCMD_MISC, DCMD_WDT_STOP_CMD)
#endif