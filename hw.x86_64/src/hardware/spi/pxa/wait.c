/*
 * Copyright (c) 2023, BlackBerry Limited.
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
 * $
 */


#include "pxa-spi.h"


/**
 *  @brief             Wait for SPI transfer complete.
 *  @param  spi        SPI driver handler.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_wait(pxa_spi_t *const spi)
{
    struct timespec     timeout;
    const uint64_t      ns_timeout = spi->xtime_us * 50 * 1000UL;    /* timeout in ns. 50 times for transation time */
    spi_bus_t *const    bus = spi->bus;
    int                 status;

    clock_gettime(CLOCK_MONOTONIC, &timeout);
    nsec2timespec(&timeout, timespec2nsec(&timeout) + ns_timeout);

    while (1) {
        /* The sem_timedwait_monotonic() function decrements the semaphore on successful completion */
        status = sem_timedwait_monotonic(bus->sem, &timeout);

        if (status == -1) {
            spi_slogf(_SLOG_ERROR, "%s: sem_timedwait_monotonic failed: %s", __func__, strerror(errno));
            status = errno;
            break;
        } else {
            /* Received SPI interrupt event */
            status = pxa_process_interrupts(spi);
            InterruptUnmask((int)spi->bus->irq, spi->iid);

            if (status == SPI_INTR_CONTINUE) {
                spi_slogf(_SLOG_DEBUG2, "%s: SPI transfer continue ...", __func__);
            } else if (status == SPI_INTR_DONE) {
                /* Transfer is completed */
                spi_slogf(_SLOG_DEBUG2, "%s: SPI transfer is completed", __func__);
                status = EOK;
                break;
            } else {
                /* Transfer failed */
                spi_slogf(_SLOG_DEBUG2, "%s: SPI transfer failed", __func__);
                status = EIO;
                break;
            }
        }
    }

    return status;
}
