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

i2c_status_t mec170_i2c_sendrecv(void *hdl, void *txbuf, uint32_t send_len,
                                    void *rxbuf, uint32_t recv_len, uint32_t stop)
{
    mec170_i2c_dev_t* const dev = (mec170_i2c_dev_t *)hdl;

    if (stop == 0U) {
        i2c_slogf(dev->verbose, _SLOG_ERROR, "Non-stop condition is not supported");
        return I2C_STATUS_ERROR;
    }
    return mec170_i2c_smbus_xfer(dev, READWRITE, txbuf, send_len, rxbuf, recv_len);
}

/*
    BEWARE: This function overrides _i2c_master_sendrecv in lib/i2c/master/sendrecv.c
            and is called from _i2c_devctl in lib/i2c/master/devctl.c
*/

int _i2c_master_sendrecv(resmgr_context_t *ctp, io_devctl_t *msg, i2c_ocb_t *ocb)
{
    i2c_dev_t           *dev = (i2c_dev_t *)ocb->hdr.attr;
    i2c_sendrecv_t      *hdr;
    void                *txbuf, *rxbuf;
    i2c_status_t        mstatus;
    uint32_t            maxbuflen;
    uint32_t            loglevel;

    loglevel = dev->verbosity;

    /* Client message didn't supply promised data */
    if (((sizeof(*msg) + sizeof(*hdr)) > ctp->size)) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Incomplete sendrecv hdr");
        return EBADMSG;
    }

    hdr = _IO_INPUT_PAYLOAD(msg);

    if (hdr->recv_len > I2C_MAX_READ_LENGTH) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Requested buffer size(%u) exceeds I2C transaction data limit(%d)",
                                                                                    hdr->recv_len, I2C_MAX_READ_LENGTH);
        return EBADMSG;
    }

    if (hdr->send_len > I2C_MAX_WRITE_LENGTH) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Requested buffer size(%u) exceeds I2C transaction data limit(%d)",
                                                                                    hdr->send_len, I2C_MAX_WRITE_LENGTH);
        return EBADMSG;
    }

    maxbuflen = max(hdr->send_len, hdr->recv_len);

    if (maxbuflen == 0U) {
        (void)memset(&msg->o, 0, sizeof(msg->o));
        (void)MsgReply(ctp->rcvid, EOK, msg, (sizeof(*msg) + sizeof(*hdr)));
        return _RESMGR_NOREPLY;
    }

    if ((sizeof(*hdr) + hdr->send_len) > msg->i.nbytes) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Incomplete send data");
        return EINVAL;
    }

    if ((sizeof(*hdr) + hdr->recv_len) > msg->i.nbytes) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Receive buffer too short");
        return EINVAL;
    }

    /* Combined messages check (ctp->offset != 0) has been done in _i2c_devctl() in libi2c-master */
    if ((sizeof(*msg) + sizeof(*hdr) + maxbuflen) > ctp->msg_max_size) {
        if (maxbuflen > dev->buflen) {
            free(dev->buf);
            dev->buf = malloc(maxbuflen);
            if (dev->buf == NULL) {
                dev->buflen = 0;
                return ENOMEM;
            }
            dev->buflen = maxbuflen;
        }
    }

    if ((sizeof(*msg) + sizeof(*hdr) + hdr->send_len) > ctp->msg_max_size) {
        int64_t status;
        status = resmgr_msgread(ctp, dev->buf, hdr->send_len, sizeof(*msg) + sizeof(*hdr));
        if (status < 0) {
            return errno;
        }
        if ((uint32_t)status < hdr->send_len) {
            return EBADMSG;
        }
        txbuf = dev->buf;
        /* Check payload fits to safely access hdr + 1 */
    } else {
        if (((sizeof(*msg) + sizeof(*hdr) + hdr->send_len) > ctp->size)) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Bad message: payload exceeds buffer");
            return EBADMSG;
        }
        txbuf = (void *)(hdr + 1);
    }

    if ((sizeof(*msg) + sizeof(*hdr) + hdr->recv_len) > ctp->msg_max_size) {
        rxbuf = dev->buf;
    } else {
        rxbuf = (void *)(hdr + 1);
    }

    SETIOV(&ctp->iov[0], msg, sizeof(*msg) + sizeof(*hdr));
    SETIOV(&ctp->iov[1], rxbuf, hdr->recv_len);

    if (dev->bus_speed != ocb->bus_speed) {
        if (dev->mfuncs.set_bus_speed(dev->hdl, ocb->bus_speed, NULL) == -1) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Bad bus speed: %d:", ocb->bus_speed);
            return EIO;
        }
        dev->bus_speed = ocb->bus_speed;
    }

    if (dev->mfuncs.set_slave_addr(dev->hdl, hdr->slave.addr, hdr->slave.fmt) == -1) {
        i2c_slogf(loglevel, _SLOG_ERROR, "Bad slave address: %x ", hdr->slave.addr);
        return errno;
    }
    ocb->status = I2C_STATUS_DONE;

    mstatus = mec170_i2c_sendrecv(dev->hdl, txbuf, hdr->send_len, rxbuf, hdr->recv_len, hdr->stop);
    ocb->status = mstatus;
    if (mstatus != I2C_STATUS_DONE) {
        if ((mstatus & I2C_STATUS_DONE) == 0U) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Master sendrecv did not terminate for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_ERROR) != 0U) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Master sendrecv timeout for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_ADDR_NACK) != 0U) {
            i2c_slogf(loglevel, _SLOG_WARNING, "Master sendrecv address NACK for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_DATA_NACK) != 0U) {
            i2c_slogf(loglevel, _SLOG_WARNING, "Master sendrecv data NACK for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_ARBL) != 0U) {
            i2c_slogf(loglevel, _SLOG_WARNING, "Master sendrecv ARBL for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_BUSY) != 0U) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Master sendrecv BUSY for slave 0x%x", hdr->slave.addr);
        }
        if ((mstatus & I2C_STATUS_ABORT) != 0U) {
            i2c_slogf(loglevel, _SLOG_ERROR, "Master sendrecv ABORT for slave 0x%x", hdr->slave.addr);
        }
        /* General error code - more info is available through a devctl */
        return EIO;
    }
    if (hdr->send_len > 0u) {
        dev->hdr.flags |= (((uint32_t)IOFUNC_ATTR_MTIME | (uint32_t)IOFUNC_ATTR_CTIME));
    }
    if (hdr->recv_len > 0u) {
        dev->hdr.flags |= ((uint32_t)IOFUNC_ATTR_ATIME);
    }

    (void)memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = (int32_t)hdr->recv_len;
    return _RESMGR_NPARTS(2);
}

