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
 *  @brief             Prepare for SPI exchange.
 *  @param spi         SPI driver handler.
 *  @param cfg         The SPI config structure.
 *  @param tnbytes     The number of transmit bytes.
 *  @param rnbytes     The number of receive bytes.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_prepare_for_transfer(pxa_spi_t *spi, const spi_cfg_t *const cfg, const uint32_t tnbytes, const uint32_t rnbytes)
{
    static const uint32_t dscales[] = {0,0,1,2,2};

    /* Data bits per word */
    const uint32_t nbits = cfg->mode & SPI_MODE_WORD_WIDTH_MASK;
    if ((nbits < SPI_MIN_WORD_WIDTH) || (nbits > SPI_MAX_WORD_WIDTH)) {
        return EINVAL;
    }

    /* Data bytes per word */
    spi->dlen = (nbits + 7) / 8;
    spi->dscale = dscales[spi->dlen];

    /* Transation data length - word */
    const uint32_t nbytes = max(tnbytes, rnbytes);
    spi->xlen =  nbytes >> spi->dscale;
    if ((spi->xlen <= 0) || (nbytes != (spi->xlen << spi->dscale))) {
        return EINVAL;
    }

    /* Check clock rate */
    if ((cfg->clock_rate == 0) || (cfg->clock_rate > spi->bus->input_clk)) {
        spi_slogf(_SLOG_ERROR, "%s: invaide clock rate %d", __func__, cfg->clock_rate);
        return EINVAL;
    }

    /* Calculate the transation operational speed */
    uint32_t serial_clock_rate = (uint32_t) ((spi->bus->input_clk + cfg->clock_rate - 1) / cfg->clock_rate);
    serial_clock_rate = min(serial_clock_rate, SSCR0_SCR_MAX);

    /* Update cr0 value */
    spi->cr0 |= SSCR0_FrameFmt(0);
    spi->cr0 |= SSCR0_SCRate(serial_clock_rate);
    spi->cr0 |= SSCR0_DSSize(nbits);
    if (nbits > 16) {
        spi->cr0 |= SSCR0_EDSS;
    }

    /* Update cr1 value */
    if (cfg->mode & SPI_MODE_CPOL_1) {
        spi->cr1 |= SSCR1_SPO;    /* Clock Polarity */
    }
    if (cfg->mode & SPI_MODE_CPHA_1) {
        spi->cr1 |= SSCR1_SPH;    /* Clock Phase */
    }
    if (spi->loopback) {
        spi->cr1 |= SSCR1_LBM;    /* Loop-Back Mode */
    }

    /* Calculate the transaction time, minimal 1 us */
    spi->xtime_us = max(1, (uint64_t)nbits * 1000 * 1000 * spi->xlen / cfg->clock_rate);

    /* Reset RX and TX length */
    spi->rlen = 0;
    spi->tlen = 0;

    return EOK;
}

/**
 *  @brief             SPI transfer function.
 *  @param hdl         SPI driver handler.
 *  @param spi_dev     SPI device structure pointer.
 *  @param buf         The buffer which stores the transfer data.
 *  @param tnbytes     The number of transmit bytes.
 *  @param rnbytes     The number of receive bytes.
 *
 *  @return            EOK --success otherwise fail.
 */
int  pxa_xfer(void *const hdl, spi_dev_t *const spi_dev, uint8_t *const buf, const uint32_t tnbytes, const uint32_t rnbytes)
{
    pxa_spi_t             *spi = hdl;
    const spi_cfg_t *const  cfg = &spi_dev->devinfo.cfg;
    int                     status = EOK;

    spi->pbuf = buf;
    spi->xfer_count++;
    spi->dma_active = 0;

    status = pxa_prepare_for_transfer(spi, cfg, tnbytes, rnbytes);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: pxa_prepare_for_transfer failed", __func__);
        return status;
    }

    /* Configure SPI device */
    pxa_device_reset(spi);

    spi->intr_processing = INTERRUPT_FIRST;

    pxa_write32(spi, SSCR0, spi->cr0);
    pxa_write32(spi, SSCR1, spi->cr1);
    pxa_write32(spi, SITF, SITF_Low(1) | SITF_High(1));
    pxa_write32(spi, SIRF, SIRF_High(SPI_PIO_PORTION_HALF_SIZE));
    pxa_write32(spi, SSTO, 0);

    /* Enable Transmit FIFO Interrupt and SPI */
    pxa_write32(spi, SSCR1, spi->cr1 | SSCR1_TIE);
    pxa_write32(spi, SSCR0, spi->cr0 | SSCR0_SSE);

    /* Wait for exchange to finish */
    status = pxa_wait(spi);
    if (status) {
        spi_slogf(_SLOG_ERROR, "%s: xfer timeout: xlen = %d, tlen = %d, rlen = %d, xtime = %d us",
             __func__, spi->xlen, spi->tlen, spi->rlen, spi->xtime_us);

        pxa_dump_spi_and_dma_registers(spi);
        pxa_device_reset(spi);
        pxa_dump_spi_and_dma_registers(spi);

        return status;
    }

    pxa_device_reset(spi);

    return status;
}
