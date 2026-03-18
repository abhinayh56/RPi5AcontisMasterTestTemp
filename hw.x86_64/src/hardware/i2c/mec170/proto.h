/*
 * Copyright (c) 2025, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */


#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/neutrino.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <hw/i2c.h>
#include <drvr/hwinfo.h>
#include <fcntl.h>

#include <hw/ecm_mec170x.h>

/* MEC170 handles 12 busses 0 - 11 */
#define I2C_MAX_BUS_NUM         12u

/*I2C h/w related defs*/
typedef enum {
    PROBE,        /* Probe functionality */
    READ,         /* Receive operation */
    WRITE,        /* Send operation */
    READWRITE     /* Send Receive operation*/
} i2c_xfer_op_t;

typedef enum {
    SMB_100KHZ,   /*!< SMBUS_INSTANCE 0   */
    SMB_400KHZ,   /*!< SMBUS_INSTANCE 1   */
    SMB_1MHZ,     /*!< SMBUS_INSTANCE 2   */
    SMB_RESERVED, /*!< SMBUS_INSTANCE 3   */
    SMB_BUS_BAD,  /*!< For loop and check */
} smb_bus_speed_t;

#define     MEC_I2C_SPEED_1MHZ       (1000000u)
#define     MEC_I2C_SPEED_100KHZ     (100000u)
#define     MEC_I2C_SPEED_400KHZ     (400000u)
/*
  BEWARE: this _i2c_dev must exactly match _i2c_dev defined in lib/i2c/master/proto.h
*/
typedef struct  {
    iofunc_ocb_t        hdr;
    unsigned            bus_speed;
    i2c_status_t        status;
} i2c_ocb_t;

typedef struct  {
    iofunc_attr_t       hdr;
    dispatch_t          *dpp;
    resmgr_context_t    *ctp;
    int32_t             id;
    i2c_master_funcs_t  mfuncs;

    void                *buf;
    unsigned            buflen;
    unsigned            bus_speed;
    unsigned            default_bus_speed;
    unsigned            verbosity;

    void                *hdl;
} i2c_dev_t;

typedef struct {
    access_i2c_struct_ut param;
    int32_t fd;
    uint32_t verbose;
} mec170_i2c_dev_t;

void *mec170_i2c_init(int argc, char *argv[]);
void mec170_i2c_fini(void *hdl);
int32_t mec170_i2c_parseopts(mec170_i2c_dev_t *dev, int argc, char *argv[]);
int32_t mec170_i2c_set_slave_addr(void *hdl, uint32_t addr, i2c_addrfmt_t fmt);
int32_t mec170_i2c_set_bus_speed(void *hdl, uint32_t speed, uint32_t *ospeed);
int32_t mec170_i2c_version_info(i2c_libversion_t *version);
int32_t mec170_i2c_driver_info(void *hdl, i2c_driver_info_t *info);
i2c_status_t mec170_i2c_send(void*  hdl, void*  buf, uint32_t len, uint32_t  stop);
i2c_status_t mec170_i2c_recv(void *hdl, void *buf, uint32_t len, uint32_t stop);
i2c_status_t mec170_i2c_smbus_xfer(void *const hdl, const i2c_xfer_op_t op, void *const txbuf,
                      uint32_t const txlen, void *const rxbuf, uint32_t const rxlen);
i2c_status_t mec170_i2c_sendrecv(void *hdl, void *txbuf, uint32_t send_len, void *rxbuf,
                                                        uint32_t recv_len, uint32_t stop);
int _i2c_master_sendrecv(resmgr_context_t *ctp, io_devctl_t *msg, i2c_ocb_t *ocb);
int i2c_master_getfuncs(i2c_master_funcs_t *funcs, int tabsize);
#endif

