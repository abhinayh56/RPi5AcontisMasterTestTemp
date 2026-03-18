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
 *  @brief             SPI driver cleanup function.
 *  @param  hdl        SPI driver handler.
 *
 *  @return            Void.
 */
void pxa_fini(void *const hdl)
{
    pxa_spi_t  *const spi = hdl;

    /* Detach the interrupt */
    if (spi->iid != -1) {
        InterruptDetach(spi->iid);
    }

    /* Unmap SPI register */
    if (spi->vbase != (uintptr_t)MAP_FAILED) {
        munmap_device_memory((void *)spi->vbase, spi->map_size);
    }

    /* Close typed memory file descriptor */
    if (spi->typed_mem_fd != NOFD) {
        close(spi->typed_mem_fd);
    }

    /* Detach the PCI device */
    if (spi->pci != NULL) {
        pci_device_detach(spi->pci);
    }

    /* Free SPI structure */
    free(spi);
}
