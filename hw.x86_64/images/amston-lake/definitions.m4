include(`../common-definitions.m4')

# Variant-specific defines/un-defines/overrides can go here

define(`__BOARD_OPTION01_START__', `
    ############################################################################################
    ## ECM driver
    ############################################################################################
    display_msg "Starting ECM driver ..."
foreach(`_x', `    __ECM_DRVR__ _x', __ECM_OPTS__)
dnl')

define(`__ECM_DRVR__', `ecm-mec170x')
define(`__ECM_OPTS__',          `')

define(`__ECM_FILES__', `
################################################################################################
## ECM drivers
################################################################################################
ifdef(`__ECM_DRVR__', `foreach(`_x', `/sbin/_x=_x', __ECM_DRVR__)')
')

define(`__I2C_DRVR__', `i2c-mec170')
define(`__I2C_OPTS__', `-d 10 --u 10 -v')
define(`__I2C_DEV__', `/dev/i2c10')

## WDT kick
define(`__WDT_DRVR__', `wdtkick')
define(`__WDT_OPTS__',  -d 100 -e 30000 -r 29000 -s 29000)

define(`__WDT_START__', `
    #######################################################################
    ## WatchDog utility
    #######################################################################
    display_msg "Starting Watchdog driver..."
    __WDT_DRVR__ __WDT_OPTS__
')

define(`__BOARD_FILES__', `
__ECM_FILES__
__DEBUG_CONSOLE_SCRIPT__

################################################################################################
## Script for launching the block driver
################################################################################################
blk-start.sh={
#!/bin/ksh

PCI_DEVICE_LIST=/dev/shmem/pci_device.list
STD_NULL=/dev/null

## supported list for the EIDE block devices
EIDE_8086_7111=8086/7111

## supported list for the SDMMC block devices
SDMMC_8086_5acc=8086/5acc

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

# check if there is the EIDE device in the list
if grep $EIDE_8086_7111 $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting EIDE block driver ..."
    devb-eide cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=eide
    waitfor /dev/eide0 3
fi

# check if there is the SDMMC device in the list
if  grep $SDMMC_8086_5acc $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting SDMMC block driver ..."
    devb-sdmmc cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') sdio idx=0,vid=0x8086,did=0x5acc disk name=sdmmc
    waitfor /dev/sdmmc0 3
fi
}

################################################################################################
## END OF BUILD SCRIPT
################################################################################################
')
