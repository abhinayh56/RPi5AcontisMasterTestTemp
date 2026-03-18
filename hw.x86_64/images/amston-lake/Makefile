ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

BUILD_TEMPLATE=../../../Templates_build_sdp800
INSTALL=../../install
BOARD=x86_64-seco-amstonlake-uefi
HOST_MKIFS := mkifs

.PHONY: all clean

all: $(BOARD).efi

clean:
	$(RM_HOST) *.efi *.sym *.img

$(BOARD).efi: $(BOARD).build
	$(HOST_MKIFS) -r $(INSTALL) $(MKIFSFLAGS) $^ $@

disk_image:
	$(ECHO_HOST) "Disk image not available for this board"

-include $(BUILD_TEMPLATE)/template.mk
