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


enum {
    OPTION_IDX,
    OPTION_VID,
    OPTION_DID,
    OPTION_RX_THRESH,
    OPTION_TX_THRESH,
    OPTION_DMA_RX_THRESH,
    OPTION_DMA_TX_THRESH,
    OPTION_DMA_TX_LOW_THRESH,
    OPTION_DMA,
    OPTION_LOOPBACK,

    NUM_OPTIONS
};

/**
 *  @brief             Process board specific options which is set in SPI config file.
 *  @param  spi        SPI driver handler.
 *  @param  options    Board specific options.
 *
 *  @return            EOK --success otherwise fail.
 */
static int pxa_process_args(pxa_spi_t *spi, char *options)
{
    char       *value;
    int         opt = 0;
    const char *c;
    char       *pxa_opts[] = {
        [OPTION_IDX]                = "idx",                /* PCI device index */
        [OPTION_VID]                = "vid",                /* PCI Vendor ID */
        [OPTION_DID]                = "did",                /* PCI Device ID */
        [OPTION_RX_THRESH]          = "rx_thresh",          /* Receive FIFO Threshold */
        [OPTION_TX_THRESH]          = "tx_thresh",          /* Transmit FIFO Threshold */
        [OPTION_DMA_RX_THRESH]      = "dma_rx_thresh",      /* Receive FIFO Threshold - DMA mode */
        [OPTION_DMA_TX_THRESH]      = "dma_tx_thresh",      /* Transmit FIFO Threshold - DMA mode */
        [OPTION_DMA_TX_LOW_THRESH]  = "dma_tx_low_thresh",  /* Transmit Low FIFO Threshold - DMA mod */
        [OPTION_DMA]                = "dma",                /* Enable DMA exchange mode */
        [OPTION_LOOPBACK]           = "loopback",           /* Loopback interface for testing */
        [NUM_OPTIONS]               = NULL
    };

    /* Set the default value of the SPI device */
    spi->idx               = 0;
    spi->vid               = PCI_VID_Intel_Corp;
    spi->did               = PCI_DID_ANY;
    spi->rx_thresh         = DEFAULT_RX_THRESHOLD;
    spi->tx_thresh         = DEFAULT_TX_THRESHOLD;
    spi->dma_rx_thresh     = DEFAULT_DMA_RX_THRESHOLD;
    spi->dma_tx_thresh     = DEFAULT_DMA_TX_THRESHOLD;
    spi->dma_tx_low_thresh = DEFAULT_DMA_TX_LOW_THRESHOLD;
    spi->dma_mode          = 0;
    spi->loopback          = 0;

    if (options == NULL) {
        spi_slogf(_SLOG_WARNING, "%s: no board specific args passed in", __func__);
        return EOK;
    }

    while ((options != NULL) && (*options != '\0')) {
        c = options;
        opt = getsubopt(&options, pxa_opts, &value);
        if (opt == -1) {
            spi_slogf(_SLOG_ERROR, "%s: unsupported SPI device driver args: %s", __func__, c);
            return EINVAL;
        }

        switch (opt) {
            case OPTION_IDX:
                if (value) {
                    spi->idx = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: idx = %u", __func__, spi->idx);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: no idx value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_VID:
                if (value) {
                    spi->vid = (pci_vid_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: vid = %04x", __func__, spi->vid);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: no vid value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_DID:
                if (value) {
                    spi->did = (pci_did_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: did = %04x", __func__, spi->did);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: no did value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_RX_THRESH:
                if (value) {
                    spi->rx_thresh = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: rx_thresh = %u", __func__, spi->rx_thresh);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: no rx_thresh value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_TX_THRESH:
                if (value) {
                    spi->tx_thresh = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: tx_thresh = %u", __func__, spi->tx_thresh);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: no tx_thresh value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_DMA_RX_THRESH:
                if (value) {
                    spi->dma_rx_thresh = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: dma_rx_thresh = %u", __func__, spi->dma_rx_thresh);
                } else {
                    spi_slogf(_SLOG_WARNING, "%s: no dma_rx_thresh value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_DMA_TX_THRESH:
                if (value) {
                    spi->dma_tx_thresh = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: dma_tx_thresh = %u", __func__, spi->dma_tx_thresh);
                } else {
                    spi_slogf(_SLOG_WARNING, "%s: no dma_tx_thresh value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_DMA_TX_LOW_THRESH:
                if (value) {
                    spi->dma_tx_low_thresh = (uint32_t) strtoul(value, NULL, 0);
                    spi_slogf(_SLOG_DEBUG2, "%s: dma_tx_low_thresh = %u", __func__, spi->dma_tx_low_thresh);
                } else {
                    spi_slogf(_SLOG_WARNING, "%s: no dma_tx_low_thresh value provided", __func__);
                    return EINVAL;
                }
                break;
            case OPTION_DMA:
                spi->dma_mode = 1;          /* Enable DMA exchange mode, default is PIO mode */
                break;
            case OPTION_LOOPBACK:
                spi->loopback = 1;          /* Enable loopback mode, used for testing */
                break;
            default:
                spi_slogf(_SLOG_ERROR, "%s: wrong option", __func__);
                return EINVAL;
        }
    }

    return EOK;
}

/**
 *  @brief             Dump registers value.
 *  @param  spi        SPI driver handler.
 *  @param  from       Start offset.
 *  @param  count      How many registers.
 *
 *  @return            Void.
 */
static void pxa_memory_dump(const pxa_spi_t *const spi, const uint32_t from, const uint32_t count)
{
    char        tmp[4][16];
    uint32_t    i;
    uint32_t    j = 3;
    for (i = 0; i < count; i++) {
        j = i & 3;
        sprintf(tmp[j], " %.8X", ((uint32_t*)(spi->vbase+from))[i]);
        if (j == 3) {
            spi_slogf(_SLOG_DEBUG2, "%s: DUMP %.8X :%s%s%s%s",
                __func__, (unsigned)(from+(i-j)*sizeof(uint32_t)), tmp[0], tmp[1], tmp[2], tmp[3]);
        }
    }
    if (j < 3) {
        uint32_t    k;
        for (k = ++j; k < 4; k++) {
            tmp[k][0] = 0;
        }
        spi_slogf(_SLOG_DEBUG2, "%s: DUMP %.8X :%s%s%s%s",
            __func__, (unsigned)(from+(i-j)*sizeof(uint32_t)), tmp[0], tmp[1], tmp[2], tmp[3]);
    }
}

/**
 *  @brief             Dump all SPI registers value.
 *  @param  spi        SPI driver handler.
 *
 *  @return            Void.
 */
void pxa_dump_spi_registers(const pxa_spi_t *const spi)
{
    pxa_memory_dump(spi, 0x0000, 4);
    pxa_memory_dump(spi, 0x0028, 9);
    pxa_memory_dump(spi, 0x0200, 3);
    pxa_memory_dump(spi, 0x0210, 17);
    pxa_memory_dump(spi, 0x0280, 1);
    pxa_memory_dump(spi, 0x02FC, 1);
}

/**
 *  @brief             Dump all DMA registers value.
 *  @param  spi        SPI driver handler.
 *
 *  @return            Void.
 */
void pxa_dump_dma_registers(const pxa_spi_t *const spi)
{
    pxa_memory_dump(spi, 0x0800, 21);
    pxa_memory_dump(spi, 0x0858, 21);
    pxa_memory_dump(spi, 0x0AC0, 87);
}

/**
 *  @brief             Dump all SPI and DMA registers value.
 *  @param  spi        SPI driver handler.
 *
 *  @return            Void.
 */
void pxa_dump_spi_and_dma_registers(const pxa_spi_t *const spi)
{
    pxa_dump_spi_registers(spi);
    pxa_dump_dma_registers(spi);
}

/**
 *  @brief             Check if the PCI device is been attached or not.
 *  @param  code       PCI specific error codes.
 *
 *  @return            0 = not_attached; 1 = already_attached.
 */
static bool already_attached(const pci_err_t code)
{
    return (code == PCI_ERR_ATTACH_EXCLUSIVE)
        || (code == PCI_ERR_ATTACH_SHARED)
        || (code == PCI_ERR_ATTACH_OWNED);
}

/**
 *  @brief             Find the SPI PCI device and attach it.
 *  @param  spi        SPI driver handler.
 *
 *  @return            EOK --success otherwise fail.
 */
static int pxa_match_device(pxa_spi_t *spi)
{
    pci_bdf_t       bdf = PCI_BDF_NONE;
    pci_err_t       rc  = PCI_ERR_ENODEV;
    pci_devhdl_t    hdl;

    const pci_did_t supported_intel_dids[] =
    {
        0x5ac2,		    /* Broxton Apollo Lake B0:D25:F0 */
        0x5ac4,         /* Broxton Apollo Lake B0:D25:F1 */
        0x5ac6,         /* Broxton Apollo Lake B0:D25:F2 */
        0x0f0e,         /* Bloomfield Hills */
        0x4b2a,         /* Elkhart Lake GSPI Controller #0 B0:D30:F2 */
        0x4b2b,         /* Elkhart Lake GSPI Controller #1 B0:D30:F3 */
        0x4b37,         /* Elkhart Lake GSPI Controller #2 B0:D18:F0 */
    };

    spi->pci = NULL;

    if ((spi->vid != PCI_VID_ANY) && (spi->did != PCI_DID_ANY)) {
        /* Attempt to find the specified device */
        bdf = pci_device_find(spi->idx, spi->vid, spi->did, PCI_CCODE_ANY);
        if (PCI_BDF_NONE != bdf) {
            hdl = pci_device_attach(bdf, pci_attachFlags_EXCLUSIVE_OWNER, &rc);
            if (hdl == NULL) {
                if (already_attached(rc)) {
                    spi_slogf(_SLOG_DEBUG2, "%s: already attached PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u",
                                __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx);
                } else {
                    spi_slogf(_SLOG_ERROR, "%s: unable to attach to PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u %s",
                                __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx, pci_strerror(rc));
                }
                return EINVAL;
            } else {
                spi->pci = hdl;
                spi_slogf(_SLOG_DEBUG2, "%s: attached to PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u",
                             __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx);
                return EOK;
            }
        }
    } else if ((spi->vid == PCI_VID_Intel_Corp) && (spi->did == PCI_DID_ANY)) {
        /* Attempt to find a supported Intel device */
        for (unsigned i = 0; i < ARRAY_SIZE(supported_intel_dids); i++) {
            spi->did = supported_intel_dids[i];
            spi->idx = 0;

            bdf = pci_device_find(spi->idx, spi->vid, spi->did, PCI_CCODE_ANY);
            if (PCI_BDF_NONE != bdf ) {
                hdl = pci_device_attach(bdf, pci_attachFlags_EXCLUSIVE_OWNER, &rc);
                if (hdl != NULL) {
                    spi->pci = hdl;
                    spi_slogf(_SLOG_DEBUG2, "%s: attached to PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u",
                                __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx);
                    return EOK;
                }

                if (already_attached(rc)) {
                    /* Search for the next possible idx/vid/did */
                    spi_slogf(_SLOG_DEBUG2, "%s: already attached PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u, try to find next device",
                                __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx);
                    continue;
                }
                else {
                   spi_slogf(_SLOG_ERROR, "%s: unable to attach to PCI device B%u:D%u:F%u - vid %04x did %04x @ idx%u %s",
                                __func__, PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf), spi->vid, spi->did, spi->idx, pci_strerror(rc));
                    return rc;
                }
            }
        }
    } else {
        spi_slogf(_SLOG_ERROR, "%s: unsupport PCI device VID %04x DID %04x", __func__, spi->vid, spi->did);
        return EINVAL;
    }

    spi_slogf(_SLOG_ERROR, "%s: failed to find SPI device", __func__);
    return EINVAL;
}

/**
 *  @brief             Reset SPI device.
 *  @param  spi        SPI driver handler.
 *
 *  @return            Void.
 */
void pxa_device_reset(const pxa_spi_t *const spi)
{
    /* Reset SPI and SPI DMA */
    pxa_write32(spi, SPI_DEVICE_RESET, 0);
    pxa_write32(spi, SPI_DEVICE_RESET, 7);

    /* Reset TX and RX bit counters */
    pxa_read32(spi, SPI_TX_BIT_COUNT);
    pxa_read32(spi, SPI_RX_BIT_COUNT);
}

/**
 *  @brief             Get and map SPI controller base address, enable PCI command bit, and get irq value
 *  @param  spi        SPI driver handler.
 *
 *  @return            EOK --success otherwise fail.
 */
static int map_in_device_registers(pxa_spi_t *spi)
{
    pci_err_t       rc;
    pci_bdf_t const bdf = pci_bdf(spi->pci);

    /* Attempt to reset the device function */
    rc = pci_device_reset(spi->pci, pci_resetType_e_FUNCTION);
    if ((rc != PCI_ERR_OK) && (rc != PCI_ERR_ENOTSUP)) {
        spi_slogf(_SLOG_ERROR, "%s: pci_device_reset failed: %d %s", __func__, rc, strerror(rc));
        return rc;
    }

    /* Obtain the device base address */
    pci_ba_t    ba[6];
    int_t       nba = (int_t) ARRAY_SIZE(ba);

    rc = pci_device_read_ba(spi->pci, &nba, ba, pci_reqType_e_UNSPECIFIED);
    if (rc != PCI_ERR_OK) {
        spi_slogf(_SLOG_ERROR, "%s: pci_device_read_ba failed: %d %s", __func__, rc, strerror(rc));
        return rc;
    }

    int_t   i;
    for (i = 0; i < nba; i++) {
        if ((ba[i].type == pci_asType_e_MEM) && ((ba[i].attr & pci_asAttr_e_EXPANSION_ROM) == 0)) {
            break;
        }
    }

    if (i >= nba) {
        spi_slogf(_SLOG_ERROR, "%s: memory address space not found", __func__);
        return ENOMEM;
    }

    void *const hw_addr = mmap_device_memory(NULL, ba[i].size, PROT_READ|PROT_WRITE|PROT_NOCACHE, MAP_SHARED, ba[i].addr);
    if (hw_addr == MAP_FAILED) {
        spi_slogf(_SLOG_ERROR, "%s: mmap_device_memory failed", __func__);
        return ENOMEM;
    }

    spi->vbase    = (uintptr_t)hw_addr;
    spi->pbase    = ba[i].addr;
    spi->map_size = ba[i].size;

    /* Obtain the device interrupt number */
    if (spi->bus->irq == -1) {
        pci_irq_t irq;
        rc = pci_device_read_irq(spi->pci, NULL, &irq);
        if (rc != PCI_ERR_OK) {
            spi_slogf(_SLOG_ERROR, "%s: pci_device_read_irq failed: %s", __func__,  pci_strerror(rc));
            return rc;
        }

        spi->bus->irq = irq;
    }

    /* Reset SPI and SPI DMA */
    pxa_device_reset(spi);

    /* Set DMA_REMAP_LO/HI for correct DMA operation */
    pxa_write32(spi, SPI_DMA_REMAP_LO, (uint32_t) spi->pbase);
    pxa_write32(spi, SPI_DMA_REMAP_HI, (uint32_t) (spi->pbase>>32));

    pci_cmd_t pci_cmd;
    rc = pci_device_read_cmd(bdf, &pci_cmd);
    if (rc != PCI_ERR_OK) {
        spi_slogf(_SLOG_ERROR, "%s: pci_device_read_cmd failed: %s", __func__,  pci_strerror(rc));
        return rc;
    }

    pci_cmd |= 4;
    rc = pci_device_write_cmd(spi->pci, pci_cmd, NULL);
    if (rc != PCI_ERR_OK) {
        spi_slogf(_SLOG_ERROR, "%s: pci_device_write_cmd failed: %s", __func__,  pci_strerror(rc));
        return rc;
    }

    return EOK;
}

/**
 *  @brief             Initialize SPI controller and devices.
 *  @param  spi        SPI driver handler.
 *
 *  @return            EOK --success otherwise fail.
 */
static int pxa_init_device(pxa_spi_t *const spi)
{
    int                 status;
    spi_bus_t *const    bus = spi->bus;
    spi_dev_t          *spi_dev = bus->devlist;


    /*
     * Initial device configuration with defaults from config file
     */
    while (spi_dev != NULL) {
        status = pxa_cfg(spi, spi_dev);
        if (status != EOK) {
            spi_slogf(_SLOG_ERROR, "%s: pxa_cfg failed", __func__);
            return status;
        }
        spi_dev = spi_dev->next;
    }

    /* Reset SPI and SPI DMA */
    pxa_device_reset(spi);

    /* Set DMA_REMAP_LO/HI for correct DMA operation */
    if (spi->dma_mode) {
        pxa_write32(spi, SPI_DMA_REMAP_LO, (uint32_t)  spi->pbase);
        pxa_write32(spi, SPI_DMA_REMAP_HI, (uint32_t)  (spi->pbase>>32));
    }

    /* Attach SPI interrupt */
    status = pxa_attach_intr(spi);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: pxa_attach_intr failed", __func__);
    }

    return status;
}

/**
 *  @brief             SPI initialization.
 *  @param  bus        The SPI bus structure.
 *
 *  @return            EOK --success otherwise fail.
 */
int spi_init(spi_bus_t *bus)
{
    pxa_spi_t *spi = NULL;
    int         status = EOK;

    if (bus == NULL) {
        spi_slogf(_SLOG_ERROR, "%s: no SPI bus structure !", __func__);
        return EINVAL;
    }

    spi = calloc(1, sizeof(pxa_spi_t));
    if (spi == NULL) {
        spi_slogf(_SLOG_ERROR, "%s: failed to allocate memory !", __func__);
        return ENOMEM;
    }

    /* Save spi_ctrl to driver structure */
    spi->spi_ctrl = bus->spi_ctrl;
    spi->bus      = bus;

    /* Get other SPI driver functions */
    bus->funcs->spi_fini        = pxa_fini;
    bus->funcs->drvinfo         = pxa_drvinfo;
    bus->funcs->devinfo         = pxa_devinfo;
    bus->funcs->setcfg          = pxa_setcfg;
    bus->funcs->xfer            = pxa_xfer;
    bus->funcs->dma_xfer        = NULL;
    bus->funcs->dma_allocbuf    = NULL;
    bus->funcs->dma_freebuf     = NULL;

    /* Set default value */
    spi->typed_mem_fd = NOFD;
    spi->iid          = -1;
    spi->vbase        = (uintptr_t)MAP_FAILED;

    /* Process args, override the defaults */
    if (bus->bs != NULL) {
        status = pxa_process_args(spi, bus->bs);
        if (status != EOK) {
            spi_slogf(_SLOG_ERROR, "%s: pxa_process_args failed", __func__);
            pxa_fini(spi);
            return status;
        }
    }

    /* Find and attach PCI device */
    status = pxa_match_device(spi);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: pxa_match_device failed", __func__);
        pxa_fini(spi);
        return status;
    }

    /* Get and mmap SPI controller base address, enable PCI command bit, and get irq value */
    status = map_in_device_registers(spi);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: map_in_device_registers failed", __func__);
        pxa_fini(spi);
        return status;
    }

    uint32_t dma_present;
    dma_present = get_field(pxa_read32(spi, SPI_DEVICE_CAPABILITIES), 8, 8) == 0;
    if (dma_present == 0) {
        spi->dma_mode = 0;
        spi_slogf(_SLOG_DEBUG2, "%s: The SPI device does not support DMA",  __func__);
    }

    if (spi->dma_mode) {
        /* Get DMA SPI driver functions */
        bus->funcs->dma_xfer        = pxa_dmaxfer;
        bus->funcs->dma_allocbuf    = pxa_dma_allocbuf;
        bus->funcs->dma_freebuf     = pxa_dma_freebuf;

        /* Open a typed memory file descriptor for DMA buffer allocate */
        spi->typed_mem_fd = posix_typed_mem_open("sysram", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
        if (spi->typed_mem_fd == NOFD) {
            spi_slogf(_SLOG_ERROR, "%s: posix_typed_mem_open failed %s",  __func__, strerror(errno));
            pxa_fini(spi);
            return errno;
        }

        spi_slogf(_SLOG_DEBUG2, "%s: The SPI device use DMA exchange mode",  __func__);
    } else {
        spi_slogf(_SLOG_DEBUG2, "%s: The SPI device use PIO exchange mode",  __func__);
    }

    /* Init SPI device */
    status = pxa_init_device(spi);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: pxa_init_device failed", __func__);
        pxa_fini(spi);
        return status;
    }

    /*
     * Create SPI chip select devices
     */
    status = spi_create_devs(bus->devlist);
    if (status != EOK) {
        spi_slogf(_SLOG_ERROR, "%s: spi_create_devs for %s failed", __func__, bus->devlist->devinfo.name);
        pxa_fini(spi);
        return status;
    }

    /* Save driver structure to drvhdl */
    bus->drvhdl = spi;

    return status;
}
