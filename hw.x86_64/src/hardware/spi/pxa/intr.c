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
 *  @brief             Write SPI FIFO.
 *  @param  spi        SPI driver handler.
 *  @param  len        Data length in word.
 *
 *  @return            Void.
 */
static void pxa_write_fifo(const pxa_spi_t *const spi, const uint32_t len)
{
    switch(spi->dscale) {
        case 0:
            pxa_write_fifo_uint8(spi, ((uint8_t*)spi->pbuf) + spi->tlen, len);
            break;
        case 1:
            pxa_write_fifo_uint16(spi, ((uint16_t*)spi->pbuf) + spi->tlen, len);
            break;
        case 2:
            pxa_write_fifo_uint32(spi, ((uint32_t*)spi->pbuf) + spi->tlen, len);
            break;
        default:
            spi_slogf(_SLOG_ERROR, "%s: invalid dscale %d", __func__, spi->dscale);
            break;
    }
}

/**
 *  @brief             Read SPI FIFO.
 *  @param  spi        SPI driver handler.
 *  @param  len        Data length in word.
 *
 *  @return            Void.
 */
static void pxa_read_fifo(const pxa_spi_t *const spi, const uint32_t len)
{
    switch(spi->dscale) {
        case 0:
            pxa_read_fifo_uint8(spi, ((uint8_t*)spi->pbuf) + spi->rlen, len);
            break;
        case 1:
            pxa_read_fifo_uint16(spi, ((uint16_t*)spi->pbuf) + spi->rlen, len);
            break;
        case 2:
            pxa_read_fifo_uint32(spi, ((uint32_t*)spi->pbuf) + spi->rlen, len);
            break;
        default:
            spi_slogf(_SLOG_ERROR, "%s: invalid dscale %d", __func__, spi->dscale);
            break;
    }
}

/**
 *  @brief             Process SPI interrupts.
 *  @param  bus        The SPI bus structure
 *
 *  @return            0: Transfer complete; 1: Transfer not complete; otherwise fail.
 */
int pxa_process_interrupts(pxa_spi_t *spi)
{
    uint32_t len;

    if (spi->dma_active) {
        /* Mask used DMA interrupt sources */
        pxa_write32(spi, SPI_DMA_MASKERR, 0x0300);
        pxa_write32(spi, SPI_DMA_MASKBLOCK, 0x0200);

        uint32_t raw_err;
        raw_err = pxa_read32(spi, SPI_DMA_RAWERR);

        uint32_t tx_ctl_hi;
        tx_ctl_hi = get_field(pxa_read32(spi, SPI_DMA_TX_CH_BASE + SPI_DMA_CTL_HI), 17, 17);

        uint32_t rx_ctl_hi;
        rx_ctl_hi = get_field(pxa_read32(spi, SPI_DMA_RX_CH_BASE + SPI_DMA_CTL_HI), 17, 17);

        if (raw_err || (tx_ctl_hi == 0) || (rx_ctl_hi ==0 )) {
            spi->dma_error_count++;
            spi->dma_error = 1;
            spi_slogf(_SLOG_ERROR, "%s: DMA exchange error! raw_err=0x%x, tx_ctl_hi=%d, rx_ctl_hi=%d",
                         __func__, raw_err, tx_ctl_hi, rx_ctl_hi);
        }

        /* DMA exchange done... */
        spi->dma_active = 0;
        return SPI_INTR_DONE;
    }

    /* Get interrupt status */
    const uint32_t value = pxa_read32(spi, SSSR);

    /* Reset interrupt status */
    pxa_write32(spi, SSSR, value);

    if (value & SSSR_BCE) {
        spi_slogf(_SLOG_DEBUG2, "%s: Bit Count Error", __func__);
        return SPI_INTR_ERR;
    }
    if (value & SSSR_TUR) {
        spi_slogf(_SLOG_DEBUG2, "%s: Transmit FIFO Under Run", __func__);
        return SPI_INTR_ERR;
    }
    if (value & SSSR_EOC) {
        spi_slogf(_SLOG_DEBUG2, "%s: End of Chain", __func__);
        return SPI_INTR_ERR;
    }
    if (value & SSSR_TINT) {
        spi_slogf(_SLOG_DEBUG2, "%s: Receiver Time-out", __func__);
        return SPI_INTR_ERR;
    }
    if (value & SSSR_PINT) {
        spi_slogf(_SLOG_DEBUG2, "%s: Peripheral Trailing Byte", __func__);
        return SPI_INTR_ERR;
    }
    if (value & SSSR_ROR) {
        spi_slogf(_SLOG_DEBUG2, "%s: Receive FIFO Overrun", __func__);
        return SPI_INTR_ERR;
    }

    if (spi->intr_processing == INTERRUPT_FIRST) {
        if (spi->xlen <= SPI_PIO_PORTION_SIZE) {
            len = spi->xlen;
            spi->intr_processing = INTERRUPT_LAST;
            pxa_write32(spi, SIRF, SIRF_High(len));
        } else {
            len = SPI_PIO_PORTION_SIZE;
            spi->intr_processing = INTERRUPT_MIDDLE;
            pxa_write32(spi, SIRF, SIRF_High(SPI_PIO_PORTION_HALF_SIZE));
        }

        pxa_write_fifo(spi, len);
        spi->tlen += len;

        /* disable Transmit FIFO Interrupt and enable Receive FIFO Interrupt */
        pxa_write32(spi, SSCR1, spi->cr1 | SSCR1_RIE);
    }
    else if (INTERRUPT_MIDDLE == spi->intr_processing) {
        if ((value & SSSR_RFS) != 0) {
            pxa_read_fifo(spi, SPI_PIO_PORTION_HALF_SIZE);
            spi->rlen += SPI_PIO_PORTION_HALF_SIZE;

            len = spi->xlen - spi->tlen;
            if (len > SPI_PIO_PORTION_HALF_SIZE) {
                pxa_write_fifo(spi, SPI_PIO_PORTION_HALF_SIZE);
                spi->tlen += SPI_PIO_PORTION_HALF_SIZE;
            }
            else {
                pxa_write_fifo(spi, len);
                spi->tlen += len;
                pxa_write32(spi, SIRF, SIRF_High(spi->xlen - spi->rlen));
                spi->intr_processing = INTERRUPT_LAST;
            }
        }
        else {
            spi_slogf(_SLOG_ERROR, "%s: no Receive FIFO Service Request", __func__);
            return SPI_INTR_ERR;
        }
    }
    else if (INTERRUPT_LAST == spi->intr_processing) {
        if ((value & SSSR_RFS) != 0) {
            len = spi->xlen - spi->rlen;
            pxa_read_fifo(spi, len);
            spi->rlen += len;

            /* Disable SPI and mask interrupts */
            pxa_write32(spi, SSCR0, spi->cr0);
            pxa_write32(spi, SSCR1, spi->cr1);

            spi->intr_processing = INTERRUPT_INVALID;

            /* Exchange done... */
            spi_slogf(_SLOG_DEBUG2, "%s: SPI exchange done! tlen=%d, rlen=%d", __func__, spi->rlen, spi->rlen);
        }
        else {
            spi_slogf(_SLOG_ERROR, "%s: no Receive FIFO Service Request", __func__);
            return SPI_INTR_ERR;
        }
    }
    else {
        spi_slogf(_SLOG_ERROR, "%s: invalid interrupt stage, spi->intr_processing=%d", __func__, spi->intr_processing);
        return SPI_INTR_ERR;

    }

    if (spi->rlen < spi->xlen) {
        /* Transfer not complete */
        /* Don't unblock xfer function until complete */
        return SPI_INTR_CONTINUE;
    }

    /* There's no more data to send, Exchange done */
    return SPI_INTR_DONE;
}

/**
 *  @brief             Attach SPI interrupts.
 *  @param  bus        The SPI bus structure.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_attach_intr(pxa_spi_t *spi)
{
    const spi_bus_t *const bus = spi->bus;

    /*
     * Attach SPI interrupt
     */
    spi->iid = InterruptAttachEvent(bus->irq, &bus->evt, _NTO_INTR_FLAGS_TRK_MSK);
    if (spi->iid == -1) {
        spi_slogf(_SLOG_ERROR, "%s: InterruptAttachEvent failed: %s", __func__, strerror(errno));
        return errno;
    } else {
        spi_slogf(_SLOG_DEBUG2, "%s: InterruptAttachEvent succeeded: irq %d", __func__, bus->irq);
    }

    return EOK;
}
