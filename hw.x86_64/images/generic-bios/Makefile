ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

BUILD_TEMPLATE=../../../Templates_build_sdp800
INSTALL=../../install
BOARD=x86_64-generic-bios
HOST_MKIFS := mkifs

.PHONY: all clean

# Note that the hypervisor-enabled boot image variant, does not build by default.
# Once you have QNX Hypervisor installed, you can build it by running 'make hyp'.
all: $(BOARD).bin $(BOARD)-smmu.bin $(BOARD)-hypervisor.build

clean:
	$(RM_HOST) *.bin *.sym *.img

$(BOARD).bin: $(BOARD).build
	$(HOST_MKIFS) -r $(INSTALL) $(MKIFSFLAGS) $^ $@

#SMMUMAN-enabled boot image variant
$(BOARD)-smmu.bin: $(BOARD)-smmu.build
	$(HOST_MKIFS) -r $(INSTALL) $(MKIFSFLAGS) $^ $@

#Hypervisor-enabled boot image variant (includes SMMUMAN)
hyp: $(BOARD)-hyp.bin
$(BOARD)-hyp.bin: $(BOARD)-hypervisor.build
	$(HOST_MKIFS) -r $(INSTALL) $(MKIFSFLAGS) $^ $@

# If the hypervisor boot image variant exists, it will be included in the
# disk image below.
disk_image: $(BOARD).bin $(BOARD)-smmu.bin
	$(SHELL) ../mkdiskimage

-include $(BUILD_TEMPLATE)/template.mk

