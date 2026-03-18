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
 *  @brief             SPI configuration function.
 *  @param  hdl        SPI driver handler.
 *  @param  spi_dev    SPI device structure.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_cfg(const void *const hdl, spi_dev_t *spi_dev)
{
    const pxa_spi_t *const  spi = hdl;
    spi_cfg_t              *cfg = &spi_dev->devinfo.cfg;

    if (spi_dev->devinfo.devno >= SPI_CS_MAX) {
        spi_slogf(_SLOG_ERROR, "%s: devno(%d) should be smaller than %d", __func__, spi_dev->devinfo.devno, SPI_CS_MAX);
        return EINVAL;
    }

    /* Data bits per word, support 4-bit to 32-bit word width */
    const uint32_t nbits = cfg->mode & SPI_MODE_WORD_WIDTH_MASK;
    if ((nbits < 4) || (nbits > 32)) {
        spi_slogf(_SLOG_ERROR, "%s: %d-bit word width is not supported by this controller", __func__, nbits);
        return EINVAL;
    }

    /* Check clock rate */
    if (cfg->clock_rate == 0) {
        spi_slogf(_SLOG_ERROR, "%s: invaide clock rate %d", __func__, cfg->clock_rate);
        return EINVAL;
    }
    if (cfg->clock_rate > spi->bus->input_clk) {
        cfg->clock_rate = (uint32_t) spi->bus->input_clk;
    }

    /* Set SPI_CLK only when it is different than current clock rate */
    if (spi_dev->devinfo.current_clkrate != cfg->clock_rate) {
        /* Calculate the SPI target operational speed */
        uint32_t serial_clock_rate = (uint32_t) ((spi->bus->input_clk + cfg->clock_rate - 1) / cfg->clock_rate);
        serial_clock_rate = min(serial_clock_rate, SSCR0_SCR_MAX);

        /* Update the best rate */
        cfg->clock_rate = (uint32_t) (spi->bus->input_clk / serial_clock_rate);

        /* Update current clock rate */
        spi_dev->devinfo.current_clkrate = cfg->clock_rate;

        spi_slogf(_SLOG_DEBUG2, "%s: input_clk=%d, clock_rate=%d, div=%d", __func__,
                        spi->bus->input_clk, cfg->clock_rate, serial_clock_rate);
    } else {
        spi_slogf(_SLOG_DEBUG2, "%s: device clock rate is already set to %d", __func__, cfg->clock_rate);
    }

    return EOK;
}
