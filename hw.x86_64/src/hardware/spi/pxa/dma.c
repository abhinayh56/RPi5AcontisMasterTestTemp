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
 *  @brief                              SPI DMA configure functiopn.
 *  @param  spi                         SPI driver handler.
 *  @param  dma_channel_offset          SPI DMA channel offset
 *  @param  transfer_type_flowcontrol   SPI_DMA_CTL_LO.TT_FC
 *  @param  channel_class               SPI_DMA_CTL_HI.CH_CLASS
 *  @param  channel_weight              SPI_DMA_CTL_HI.CH_WEIGHT
 *  @param  block_transfer_size         SPI_DMA_CTL_HI.BLOCK_TS
 *  @param  intr_enable                 SPI_DMA_CTL_LO.INT_EN
 *  @param  write_issue_threshold       SPI_DMA_CFG_HI.WR_ISSUE_THD
 *  @param  read_issue_threshold        SPI_DMA_CFG_HI.RD_ISSUE_THD
 *  @param  src_address                 SPI_DMA_SAR_HI / SPI_DMA_SAR_LO
 *  @param  src_fixed_address           SPI_DMA_CTL_LO.SINC
 *  @param  src_reload                  SPI_DMA_CFG_LO.RELOAD_SRC
 *  @param  src_transfer_width          SPI_DMA_CTL_LO.SRC_TR_WIDTH
 *  @param  src_gather_enable           SPI_DMA_CTL_LO.SRC_GATHER_EN
 *  @param  src_gather_count            SPI_DMA_SGR.SGC
 *  @param  src_gather_interval         SPI_DMA_SGR.SGI
 *  @param  src_handshake_peripheral    SPI_DMA_CFG_HI.SRC_PER
 *  @param  src_handshake_polarity      SPI_DMA_CFG_LO.SRC_HS_POL
 *  @param  src_min_read_size           SPI_DMA_CTL_LO.SRC_MSIZE
 *  @param  src_optimize_burst_length   SPI_DMA_CFG_LO.SRC_OPT_BL
 *  @param  src_burst_align             SPI_DMA_CFG_LO.SRC_BURST_ALIGN
 *  @param  dst_address                 SPI_DMA_DAR_HI / SPI_DMA_DAR_LO
 *  @param  dst_fixed_address           SPI_DMA_CTL_LO.DINC
 *  @param  dst_reload                  SPI_DMA_CFG_LO.RELOAD_DST
 *  @param  dst_transfer_width          SPI_DMA_CTL_LO.DST_TR_WIDTH
 *  @param  dst_scatter_enable          SPI_DMA_CTL_LO.DST_SCATTER_EN
 *  @param  dst_scatter_count           SPI_DMA_DSR.DSC
 *  @param  dst_scatter_interval        SPI_DMA_DSR.DSI
 *  @param  dst_handshake_peripheral    SPI_DMA_CFG_HI.DST_PER
 *  @param  dst_handshake_polarity      SPI_DMA_CFG_LO.DST_HS_POL
 *  @param  dst_min_write_size          SPI_DMA_CTL_LO.DST_MSIZE
 *  @param  dst_optimize_burst_length   SPI_DMA_CFG_LO.DST_OPT_BL
 *  @param  dst_burst_align             SPI_DMA_CFG_LO.DST_BURST_ALIGN
 *
 *  @return                             Void.
 */
static void spi_dma_configure
(
    const pxa_spi_t *const spi,
    const uint32_t  dma_channel_offset,
    const uint32_t  transfer_type_flowcontrol,
    const uint32_t  channel_class,
    const uint32_t  channel_weight,
    const uint32_t  block_transfer_size,
    const uint32_t  intr_enable,
    const uint32_t  write_issue_threshold,
    const uint32_t  read_issue_threshold,

    const uint64_t  src_address,
    const uint32_t  src_fixed_address,
    const uint32_t  src_reload,
    const uint32_t  src_transfer_width,
    const uint32_t  src_gather_enable,
    const uint32_t  src_gather_count,
    const uint32_t  src_gather_interval,
    const uint32_t  src_handshake_peripheral,
    const uint32_t  src_handshake_polarity,
    const uint32_t  src_min_read_size,
    const uint32_t  src_optimize_burst_length,
    const uint32_t  src_burst_align,

    const uint64_t  dst_address,
    const uint32_t  dst_fixed_address,
    const uint32_t  dst_reload,
    const uint32_t  dst_transfer_width,
    const uint32_t  dst_scatter_enable,
    const uint32_t  dst_scatter_count,
    const uint32_t  dst_scatter_interval,
    const uint32_t  dst_handshake_peripheral,
    const uint32_t  dst_handshake_polarity,
    const uint32_t  dst_min_write_size,
    const uint32_t  dst_optimize_burst_length,
    const uint32_t  dst_burst_align
)
{
    pxa_write32(spi, dma_channel_offset + SPI_DMA_SAR_LO, (uint32_t) src_address);
    pxa_write32(spi, dma_channel_offset + SPI_DMA_SAR_HI, (uint32_t) (src_address >> 32));

    pxa_write32(spi, dma_channel_offset + SPI_DMA_DAR_LO, (uint32_t) dst_address);
    pxa_write32(spi, dma_channel_offset + SPI_DMA_DAR_HI, (uint32_t) (dst_address>> 32 ));

    pxa_write32(spi, dma_channel_offset + SPI_DMA_LLP_LO, 0);
    pxa_write32(spi, dma_channel_offset + SPI_DMA_LLP_HI, 0);

    pxa_write32(spi, dma_channel_offset + SPI_DMA_SSTATAR_LO, 0);
    pxa_write32(spi, dma_channel_offset + SPI_DMA_SSTATAR_HI, 0);

    pxa_write32(spi, dma_channel_offset + SPI_DMA_DSTATAR_LO, 0);
    pxa_write32(spi, dma_channel_offset + SPI_DMA_DSTATAR_HI, 0);

    uint32_t ctl_lo = 0;
    set_field(&ctl_lo, 21, 20, transfer_type_flowcontrol);          /* SPI_DMA_CTL_LO.TT_FC */
    set_field(&ctl_lo, 18, 18, dst_scatter_enable);                 /* SPI_DMA_CTL_LO.DST_SCATTER_EN */
    set_field(&ctl_lo, 17, 17, src_gather_enable);                  /* SPI_DMA_CTL_LO.SRC_GATHER_EN */
    set_field(&ctl_lo, 16, 14, src_min_read_size);                  /* SPI_DMA_CTL_LO.SRC_MSIZE */
    set_field(&ctl_lo, 13, 11, dst_min_write_size);                 /* SPI_DMA_CTL_LO.DST_MSIZE */
    set_field(&ctl_lo, 10, 10, src_fixed_address);                  /* SPI_DMA_CTL_LO.SINC */
    set_field(&ctl_lo, 8,  8,  dst_fixed_address);                  /* SPI_DMA_CTL_LO.DINC */
    set_field(&ctl_lo, 6,  4,  src_transfer_width);                 /* SPI_DMA_CTL_LO.SRC_TR_WIDTH */
    set_field(&ctl_lo, 3,  1,  dst_transfer_width);                 /* SPI_DMA_CTL_LO.DST_TR_WIDTH */
    set_field(&ctl_lo, 0,  0,  intr_enable);                        /* SPI_DMA_CTL_LO.INT_EN */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_CTL_LO, ctl_lo);

    uint32_t ctl_hi = 0;
    set_field(&ctl_hi, 31, 29, channel_class);                      /* SPI_DMA_CTL_HI.CH_CLASS */
    set_field(&ctl_hi, 28, 18, channel_weight);                     /* SPI_DMA_CTL_HI.CH_WEIGHT */
    set_field(&ctl_hi, 16, 0,  block_transfer_size);                /* SPI_DMA_CTL_HI.BLOCK_TS */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_CTL_HI, ctl_hi);

    uint32_t cfg_lo = 0;
    set_field(&cfg_lo, 31, 31, dst_reload);                         /* SPI_DMA_CFG_LO.RELOAD_DST */
    set_field(&cfg_lo, 30, 30, src_reload);                         /* SPI_DMA_CFG_LO.RELOAD_SRC */
    set_field(&cfg_lo, 21, 21, src_optimize_burst_length);          /* SPI_DMA_CFG_LO.SRC_OPT_BL */
    set_field(&cfg_lo, 20, 20, dst_optimize_burst_length);          /* SPI_DMA_CFG_LO.DST_OPT_BL */
    set_field(&cfg_lo, 19, 19, src_handshake_polarity);             /* SPI_DMA_CFG_LO.SRC_HS_POL */
    set_field(&cfg_lo, 18, 18, dst_handshake_polarity);             /* SPI_DMA_CFG_LO.DST_HS_POL */
    set_field(&cfg_lo, 3,  3,  1);                                  /* SPI_DMA_CFG_LO.HSHAKE_NP_WR */
    set_field(&cfg_lo, 1,  1,  src_burst_align);                    /* SPI_DMA_CFG_LO.SRC_BURST_ALIGN */
    set_field(&cfg_lo, 0,  0,  dst_burst_align);                    /* SPI_DMA_CFG_LO.DST_BURST_ALIGN */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_CFG_LO, cfg_lo);

    uint32_t cfg_hi = 0;
    set_field(&cfg_hi, 27, 18, write_issue_threshold);              /* SPI_DMA_CFG_HI.WR_ISSUE_THD */
    set_field(&cfg_hi, 17, 8,  read_issue_threshold);               /* SPI_DMA_CFG_HI.RD_ISSUE_THD */
    set_field(&cfg_hi, 7,  4,  dst_handshake_peripheral);           /* SPI_DMA_CFG_HI.DST_PER */
    set_field(&cfg_hi, 3,  0,  src_handshake_peripheral);           /* SPI_DMA_CFG_HI.SRC_PER */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_CFG_HI, cfg_hi);

    uint32_t sgr = 0;
    set_field(&sgr, 31, 20, src_gather_count);                      /* SPI_DMA_SGR.SGC */
    set_field(&sgr, 19, 0,  src_gather_interval);                   /* SPI_DMA_SGR.SGI */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_SGR, sgr);

    uint32_t dsr = 0;
    set_field(&dsr, 31, 20, dst_scatter_count);                     /* SPI_DMA_DSR.DSC */
    set_field(&dsr, 19, 0,  dst_scatter_interval);                  /* SPI_DMA_DSR.DSI */
    pxa_write32(spi, dma_channel_offset + SPI_DMA_DSR, dsr);
}

/**
 *  @brief             Allocate DMA buffer.
 *  @param  hdl        SPI driver handler.
 *  @param  addr       DMA address pointer.
 *  @param  len        DMA buffer size.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_dma_allocbuf(void *const hdl, dma_addr_t *const addr, const uint32_t len)
{
    pxa_spi_t *const spi = hdl;

    if (spi->typed_mem_fd == NOFD) {
        spi_slogf(_SLOG_ERROR, "%s: no typed_mem_fd",  __func__);
        return EINVAL;
    }

    addr->vaddr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, spi->typed_mem_fd, 0);
    if (addr->vaddr == MAP_FAILED) {
        spi_slogf(_SLOG_ERROR, "%s: mmap failed: %s", __func__, strerror(errno));
        return errno;
    }

    if (mem_offset64(addr->vaddr, NOFD, 1, &addr->paddr, NULL) != 0) {
        spi_slogf(_SLOG_ERROR, "%s: mem_offset64 failed: %s", __func__, strerror(errno));
        return errno;
    }

    addr->len = len;
    spi_slogf(_SLOG_DEBUG2, "%s: vaddr %p, paddr=%lx, len=%d", __func__, addr->vaddr, addr->paddr, addr->len);
    return EOK;
}

/**
 *  @brief             Free DMA buffer.
 *  @param  hdl        SPI driver handler.
 *  @param  addr       DMA address pointer.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_dma_freebuf(void *const hdl, dma_addr_t *const addr)
{
    if (addr->len && addr->vaddr) {
        munmap(addr->vaddr, addr->len);
    }
    addr->vaddr = NULL;
    addr->paddr = 0;
    addr->len   = 0;

    return EOK;
}

/**
 *  @brief             SPI DMA exchange function.
 *  @param hdl         SPI driver handler.
 *  @param spi_dev     SPI device structure pointer.
 *  @param addr        DMA address pointer.
 *  @param tnbytes     The number of transmit bytes.
 *  @param rnbytes     The number of receive bytes.
 *
 *  @return            EOK --success otherwise fail.
 */
int pxa_dmaxfer(void *const hdl, spi_dev_t *const spi_dev, dma_addr_t *const addr, const uint32_t tnbytes, const uint32_t rnbytes)
{
    pxa_spi_t             *spi = hdl;
    const spi_cfg_t *const  cfg = &spi_dev->devinfo.cfg;
    uint32_t                len;
    int                     status = EOK;

    status = pxa_prepare_for_transfer(spi, cfg, tnbytes, rnbytes);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: pxa_prepare_for_transfer failed", __func__);
        return status;
    }

    len = spi->xlen << spi->dscale;
    spi->dma_xfer_count++;
    spi->dma_error = 0;

    pxa_device_reset(spi);

    spi_dma_configure
    (
        spi,                              /* pxa_spi_t *spi, */
        SPI_DMA_TX_CH_BASE,               /* const uint32_t dma_channel_offset */
        SPI_DMA_MEM_TO_DEV_TRANSFER,      /* const uint32_t transfer_type_flowcontrol */
        SPI0_DMA_TX_CHANNEL_CLASS,        /* const uint32_t channel_class */
        SPI0_DMA_TX_CHANNEL_WEIGHT,       /* const uint32_t channel_weight */
        len,                              /* const uint32_t block_transfer_size */
        1,                                /* const uint32_t intr_enable */
        0,                                /* const uint32_t write_issue_threshold */
        0,                                /* const uint32_t read_issue_threshold */

        (uint64_t) addr->paddr,           /* const uint64_t src_address */
        0,                                /* const uint32_t src_fixed_address */
        0,                                /* const uint32_t src_reload */
        spi->dscale,                      /* const uint32_t src_transfer_width */
        0,                                /* const uint32_t src_gather_enable */
        0,                                /* const uint32_t src_gather_count */
        0,                                /* const uint32_t src_gather_interval */
        1,                                /* const uint32_t src_handshake_peripheral */
        0,                                /* const uint32_t src_handshake_polarity */
        0,                                /* const uint32_t src_min_read_size */
        0,                                /* const uint32_t src_optimize_burst_length */
        0,                                /* const uint32_t src_burst_align */

        spi->pbase + SSDR,                /* const uint64_t dst_address */
        1,                                /* const uint32_t dst_fixed_address */
        0,                                /* const uint32_t dst_reload */
        spi->dscale,                      /* const uint32_t dst_transfer_width */
        0,                                /* const uint32_t dst_scatter_enable */
        0,                                /* const uint32_t dst_scatter_count */
        0,                                /* const uint32_t dst_scatter_interval */
        0,                                /* const uint32_t dst_handshake_peripheral */
        0,                                /* const uint32_t dst_handshake_polarity */
        0,                                /* const uint32_t dst_min_write_size */
        0,                                /* const uint32_t dst_optimize_burst_length */
        0                                 /* const uint32_t dst_burst_align */
    );

    spi_dma_configure
    (
        spi,                              /* pxa_spi_t *spi */
        SPI_DMA_RX_CH_BASE,               /* const uint32_t dma_channel_offset */
        SPI_DMA_DEV_TO_MEM_TRANSFER,      /* const uint32_t transfer_type_flowcontrol */
        SPI0_DMA_RX_CHANNEL_CLASS,        /* const uint32_t channel_class */
        SPI0_DMA_RX_CHANNEL_WEIGHT,       /* const uint32_t channel_weight */
        len,                              /* const uint32_t block_transfer_size */
        1,                                /* const uint32_t intr_enable */
        0,                                /* const uint32_t write_issue_threshold */
        0,                                /* const uint32_t read_issue_threshold */

        spi->pbase+SSDR,                  /* const uint64_t src_address */
        1,                                /* const uint32_t src_fixed_address */
        0,                                /* const uint32_t src_reload, */
        spi->dscale,                      /* const uint32_t src_transfer_width */
        0,                                /* const uint32_t src_gather_enable */
        0,                                /* const uint32_t src_gather_count */
        0,                                /* const uint32_t src_gather_interval */
        1,                                /* const uint32_t src_handshake_peripheral */
        0,                                /* const uint32_t src_handshake_polarity */
        0,                                /* const uint32_t src_min_read_size */
        0,                                /* const uint32_t src_optimize_burst_length */
        0,                                /* const uint32_t src_burst_align */

        (uint64_t) addr->paddr,           /* const uint64_t dst_address */
        0,                                /* const uint32_t dst_fixed_address */
        0,                                /* const uint32_t dst_reload */
        spi->dscale,                      /* const uint32_t dst_transfer_width */
        0,                                /* const uint32_t dst_scatter_enable */
        0,                                /* const uint32_t dst_scatter_count */
        0,                                /* const uint32_t dst_scatter_interval */
        0,                                /* const uint32_t dst_handshake_peripheral */
        0,                                /* const uint32_t dst_handshake_polarity */
        0,                                /* const uint32_t dst_min_write_size */
        0,                                /* const uint32_t dst_optimize_burst_length */
        0                                 /* const uint32_t dst_burst_align */
    );

    /* Unmask used DMA interrupt sources */
    pxa_write32(spi, SPI_DMA_MASKERR, 0x0303);
    pxa_write32(spi, SPI_DMA_MASKBLOCK, 0x0202);

    /* Configure SPI */
    pxa_write32(spi, SSCR0, spi->cr0);
    pxa_write32(spi, SSCR1, spi->cr1);
    pxa_write32(spi, SITF, SITF_Low(spi->dma_tx_low_thresh) | SITF_High(spi->dma_tx_thresh));
    pxa_write32(spi, SIRF, SIRF_High(spi->dma_rx_thresh));
    pxa_write32(spi, SSTO, 0);

    /* Enable DMA requests and SPI */
    spi->dma_active = 1;
    pxa_write32(spi, SSCR1, spi->cr1 | SSCR1_TSRE | SSCR1_RSRE);
    pxa_write32(spi, SSCR0, spi->cr0 | SSCR0_SSE);

    /* Enable DMA */
    pxa_write32(spi, SPI_DMA_DMACFGREG, 1);
    pxa_write32(spi, SPI_DMA_CHENREG, 0x0303);

    /* Wait for exchange to finish */
    status = pxa_wait(spi);

    const uint32_t nbits = cfg->mode & SPI_MODE_WORD_WIDTH_MASK;
    spi->tlen = (pxa_read32(spi, SPI_TX_BIT_COUNT) / nbits);
    spi->rlen = (pxa_read32(spi, SPI_RX_BIT_COUNT) / nbits);

    if (status) {
        spi_slogf(_SLOG_ERROR, "%s: dma_xfer timeout: tlen = %d, rlen = %d, xtime = %d us",
             __func__, spi->tlen, spi->rlen, spi->xtime_us);

        pxa_dump_spi_and_dma_registers(spi);
        pxa_device_reset(spi);
        return EINVAL;
    }

    if (spi->dma_error) {
        spi_slogf(_SLOG_ERROR, "%s: dma_error: tlen = %d, rlen = %d",
             __func__, spi->tlen, spi->rlen);

        pxa_dump_spi_and_dma_registers(spi);
        pxa_device_reset(spi);
        return EINVAL;
    }

    pxa_device_reset(spi);

    return EOK;
}
