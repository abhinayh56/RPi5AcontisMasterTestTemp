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


#include "proto.h"
#include <assert.h>
#include <string.h>
#include <atomic.h>

#define RESET_RETRY (3)

int baytrail_i2c_wait_bus_not_busy(baytrail_dev_t* const dev)
{
    int timeout = BAYTRAIL_I2C_TIMEOUT * 10;

    while (i2c_reg_read32(dev, DW_IC_STATUS) & DW_IC_STATUS_ACTIVITY) {
        if (timeout <= 0) {
            const pci_bdf_t bdf = pci_bdf(dev->pci);
            logerr(bdf, "timeout waiting for bus ready\n");
            return -ETIMEDOUT;
        }
        timeout--;
        usleep(100);
    }

    return 0;
}

static int baytrail_i2c_process_intr(baytrail_dev_t* const dev)
{
    uint32_t    stat;
    uint32_t    numbers, reg;

    stat = i2c_reg_read32(dev, DW_IC_INTR_STAT);

    if (stat & DW_IC_INTR_TX_ABRT) {
        /* Get the Abort source and clean the ABRT interrupt bit */
        dev->abort_source = i2c_reg_read32(dev, DW_IC_TX_ABRT_SOURCE);
        i2c_reg_read32(dev, DW_IC_CLR_TX_ABRT);

        /* Set status */
        if (dev->abort_source & DW_IC_TX_ARBT_LOST) {
            dev->status = I2C_STATUS_DONE | I2C_STATUS_ARBL;
        }
        else if (dev->abort_source & DW_IC_TX_ABRT_ADDR_NOACK) {
            dev->status = I2C_STATUS_DONE | I2C_STATUS_ADDR_NACK;
        }
        else if (dev->abort_source & DW_IC_TX_ABRT_TXDATA_NOACK) {
            dev->status = I2C_STATUS_DONE | I2C_STATUS_DATA_NACK;
        }
        else {
            dev->status = I2C_STATUS_DONE | I2C_STATUS_ABORT;
        }

        /* Disable interrupt and return interrupt event */
        i2c_reg_write32(dev, DW_IC_INTR_MASK, 0);
        return EOK;
    }

    if (stat & DW_IC_INTR_STOP_DET) {
        /* Clean the STOP_DET interrupt */
        i2c_reg_read32(dev, DW_IC_CLR_STOP_DET);

        /* Read Rx FIFO if required */
        if (dev->rdlen < dev->rxlen) {
            /* Check the received data length */
            numbers = i2c_reg_read32(dev, DW_IC_RXFLR);
            if (dev->rdlen + numbers != dev->rxlen) {
                /* Wrong Received Rx length */
                dev->status = I2C_STATUS_ERROR;
                numbers = 0;
            }

            for ( ; numbers > 0; numbers--) {
                reg = i2c_reg_read32(dev, DW_IC_DATA_CMD);
                *dev->rxbuf++ = (uint8_t)(reg & 0x00FF);
                dev->rdlen++;
            }
        }

        /* Disable interrupt and return interrupt event */
        dev->status |= I2C_STATUS_DONE;
        i2c_reg_write32(dev, DW_IC_INTR_MASK, 0);
        return EOK;
    }

    if (stat & DW_IC_INTR_RX_FULL) {
        /* Note: this bit will be cleaned only when the value of DW_IC_RXFLR less than the value of DW_IC_RX_TL */
        if (dev->rdlen < dev->rxlen) {
            /* How many valid data entries in the receive FIFO */
            numbers = i2c_reg_read32(dev, DW_IC_RXFLR);

            /* Check the received data length */
            if (dev->rdlen + numbers > dev->rxlen) {
                /* Unexpected received length */
                numbers = dev->rxlen - dev->rdlen;
                dev->status |= I2C_STATUS_ERROR;

                /* Disable DW_IC_INTR_RX_FULL interrupt */
                reg = i2c_reg_read32(dev, DW_IC_INTR_MASK) & ~DW_IC_INTR_RX_FULL;
                i2c_reg_write32(dev, DW_IC_INTR_MASK, reg);
            }

            for ( ; numbers > 0; numbers--) {
                reg = i2c_reg_read32(dev, DW_IC_DATA_CMD);
                *dev->rxbuf++ = (uint8_t)(reg & 0x00FF);
                dev->rdlen++;
            }
        }
        else {
            /* Disable DW_IC_INTR_RX_FULL interrupt, it should never go here */
            reg = i2c_reg_read32(dev, DW_IC_INTR_MASK) & ~DW_IC_INTR_RX_FULL;
            i2c_reg_write32(dev, DW_IC_INTR_MASK, reg);
        }
    }

    if (stat & DW_IC_INTR_TX_EMPTY) {
        /* Note: this bit should been cleaned only when the value of DW_IC_TXFLR large than the value of DW_IC_TX_TL */
        unsigned i;
        for (i = 0; i < dev->fifo_depth; i++) {
            if (dev->wrlen >= dev->xlen) {
                /* No more command left, disable R_TX_EMPTY interrupt */
                reg = i2c_reg_read32(dev, DW_IC_INTR_MASK) & ~DW_IC_INTR_TX_EMPTY;
                i2c_reg_write32(dev, DW_IC_INTR_MASK, reg);
                break;
            }

            if (xmit_FIFO_is_full(dev)) {
                /* TXFIFO FULL */
                break;
            }

            if (dev->wrlen < dev->txlen) {
                /* Command for master-transmit */
                if (dev->wrlen == dev->xlen - 1) {
                    /* If last byte for master-transmit, force set STOP condition */
                    reg = *dev->txbuf++ | DW_IC_DATA_CMD_WRITE | DW_IC_DATA_CMD_STOP;
                }
                else {
                    reg = *dev->txbuf++ | DW_IC_DATA_CMD_WRITE;
                }
            }
            else if ((dev->wrlen == dev->txlen) && (0 != dev->txlen)) {
                /* First command for master-receive of isendrecv , need restart cmd */
                if (dev->wrlen == dev->xlen - 1) {
                    /* If last byte for master-receive, force set STOP condition */
                    reg = DW_IC_DATA_CMD_READ | DW_IC_DATA_CMD_RESTART | DW_IC_DATA_CMD_STOP;
                }
                else {
                    reg = DW_IC_DATA_CMD_READ | DW_IC_DATA_CMD_RESTART;
                }
            }
            else {
                /* Command for master-receive */
                if (dev->wrlen == dev->xlen - 1) {
                    /* If last byte for master-receive, force set STOP condition */
                    reg = DW_IC_DATA_CMD_READ | DW_IC_DATA_CMD_STOP;
                }
                else {
                    reg = DW_IC_DATA_CMD_READ;
                }
            }

            i2c_reg_write32(dev, DW_IC_DATA_CMD, reg);
            dev->wrlen++;
        }
    }

    return EAGAIN;
}

void baytrail_i2c_reset(baytrail_dev_t *const dev)
{
    int ret;
    int reset_retry = RESET_RETRY;
    const pci_bdf_t bdf = pci_bdf(dev->pci);

    /* Abort the I2C transfer */
    i2c_reg_write32(dev, DW_IC_ENABLE, DW_IC_ENABLE_ABORT | DW_IC_ENABLE_STATUS_ENABLE);

    usleep(1000);

    /* Disable the adapter */
    baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_DISABLE);

    do {

        /* Perform soft reset.
         * Notes:
         * If the driver isn’t using DMA, then it is OK to just leave iDMA in Reset.
         * To be 100% sure the DMA is not being used, read register BAR0 + 0xB98
         * (DMACFGREG) bit[0], which is the DMA enable bit. If it is '0', then no
         * need to worry about any pending transactions.
         * At the time this soft reset was added, DMACFGREG is 0. So DMA is NOT being
         * used.
         * Additionally, in case DMA support is added in the future, there is a need
         * to check whether or not DMA transactions need to be stopped before performing
         * soft reset.
         */

        logfyi(bdf, "Assert soft reset. retry=%d\n", reset_retry);
        i2c_reg_write32(dev, DW_IC_SOFT_RESET, DW_IC_SOFT_RESET_ASSERTED);
        usleep(25); //Just miniscule delay

        logfyi(bdf, "De-assert soft reset.\n");
        i2c_reg_write32(dev, DW_IC_SOFT_RESET, DW_IC_SOFT_RESET_NOT_ASSERTED);

        logfyi(bdf, "Re-initializing...");
        ret = baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_DISABLE);
        if (ret == 0) {
            baytrail_i2c_init_registers(dev);
            logfyi(bdf, "I2C reinitialized.\n");
            return;
        }
        else {
            logerr(bdf, "Failed to reinitialize!\n");
        }
    } while (--reset_retry > 0);

    logerr(bdf, "I2C failed to reset!\n");
}

i2c_status_t baytrail_i2c_wait_complete(baytrail_dev_t* const dev)
{
    int     err;

    // Estimate transfer time in us... The calculated time is only used for
    // the timeout, so it doesn't have to be that accurate.  At higher clock
    // rates, a calcuated time of 0 would mess-up the timeout calculation, so
    // round up to 1 us per byte, and wait at least 2.5 ms for the xfer to complete.
    const unsigned xtime_us = max(2500, max(1, 10 * 1000 * 1000 / dev->scl_freq) * dev->xlen);

    const uint64_t to_ns = (uint64_t)xtime_us * 1000 * 50;  // convert to ns and extend to 50 times of estimate time

    while (1) {
        TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_RECEIVE, NULL, &to_ns, NULL);

        err = InterruptWait(0, NULL);

        if (err == -1) {
            const pci_bdf_t bdf = pci_bdf(dev->pci);
            logerr(bdf, "%s: PID_%d %s(%d), stat reg %x\n",
                __FUNCTION__, getpid(), strerror(errno), errno, i2c_reg_read32(dev, DW_IC_STATUS));
            baytrail_i2c_reset(dev);
            return I2C_STATUS_BUSY;
        }

        if (baytrail_i2c_process_intr(dev) == EAGAIN) {
            InterruptUnmask(dev->irq, dev->iid);
            continue;
        }

        if (dev->status & (I2C_STATUS_ARBL | I2C_STATUS_ERROR)) {
            baytrail_i2c_reset(dev);
        }

        /* Disabled interrupts */
        i2c_reg_write32(dev, DW_IC_INTR_MASK, 0);

        InterruptUnmask(dev->irq, dev->iid);

        return (dev->status);
    }

    return (I2C_STATUS_DONE | I2C_STATUS_ERROR);
}
