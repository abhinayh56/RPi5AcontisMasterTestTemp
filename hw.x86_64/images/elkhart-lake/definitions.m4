include(`../common-definitions.m4')

# Variant-specific defines/un-defines/overrides can go here
define(`__STARTUP_OPTS__',      `')

## Block driver
define(`__BLOCK_DRVR__',        `devb-ahci, devb-nvme, devb-ram, devb-sdmmc')

define(`__DEVB_DRVR_START__', `
    ############################################################################################
    ## Block driver
    ############################################################################################
    display_msg "Starting SDMMC driver ..."
    devb-sdmmc cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') sdio idx=0,vid=0x8086,did=0x4b47,irq=16 disk name=emmc
    waitfor /dev/emmc0

    devb-sdmmc cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') sdio idx=0,vid=0x8086,did=0x4b48,irq=17 disk name=sd
    waitfor /dev/sd0

    ## Start SATA / NVMe driver if device exists
    ksh /proc/boot/blk-start.sh
')

## Network driver - none defined on this variant, so remove these inherited macros
undefine(`__NET_DRVR__')
undefine(`__NET_OPTS__')
undefine(`__NET_DEV__')

## Serial driver
define(`__DEVC_DRVR__',         `devc-serpci, devc-serusb')

define(`__DEVC_START__', `
    ############################################################################################
    ## Serial driver
    ############################################################################################
    display_msg "Starting serial driver ..."
    devc-serpci vid=0x8086,did=0x4b96,pci=0 -e -b115200 ifdef(`__SMMU__', `-o smmu=on')
    waitfor /dev/ser1

    devc-serpci vid=0x8086,did=0x4b4d,pci=0 -e -b115200 ifdef(`__SMMU__', `-o smmu=on')
    waitfor /dev/ser2
    TERM=qansi on -t /dev/ser2 /bin/ksh
')

## I2C driver
define(`__I2C_DRVR__',          `i2c-baytrail')
define(`__I2C_OPTS__',          `-d 0x4b7b -r 0x8086 -q 30 --u3, -d 0x4b4b -r 0x8086 -q 31 --u4')
define(`__I2C_DEV__',           `/dev/i2c3, /dev/i2c4')

## PCI driver
define(`__PCI_HW_DRVR__',       `pci_hw-Intel_x86.so')

## SPI driver
define(`__IO_SPI_DRVR__',          `spi-pxa')
define(`__IO_SPI_CFG_CONTENTS__', `
[globals]
verbose=5

[bus]
busno=1
name=spi1
irq=19
input_clock=1000000
# PIO exchange mode
bs=idx=0,vid=0x8086,did=0x4b2b

# DMA exchange mode
#bs=dma,idx=0,vid=0x8086,did=0x4b2b

#loop back mode for test
#bs=idx=0,vid=0x8086,did=0x4b2b,loopback

[dev]
parent_busno=1
devno=0
name=dev0
clock_rate=100000
cpha=1
')

define(`__BOARD_FILES__', `
__DEBUG_CONSOLE_SCRIPT__

################################################################################################
## Script for launching the block driver
################################################################################################
blk-start.sh={
#!/bin/ksh

PCI_DEVICE_LIST=/dev/shmem/pci_device.list
STD_NULL=/dev/null

## check if there is the pci device list
if [ ! -r $PCI_DEVICE_LIST ]; then
    pci-tool -v > $PCI_DEVICE_LIST
fi

# redirect stderr to slog2
exec 2> /dev/console

# check if there is the NVMe device in the list
if  grep "Non-volatile Memory Subsystem (NVMe Interface IO Controller)" $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting NVMe block driver ..."
    devb-nvme cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=nvme
    waitfor /dev/nvme0 3
fi

# check if there is the SATA device in the list
if  grep "SATA Mass Storage Controller (AHCI Interface)" $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting AHCI block driver ..."
    devb-ahci cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=sata
    waitfor /dev/sata0 3
fi
}

################################################################################################
## END OF BUILD SCRIPT
################################################################################################
')
