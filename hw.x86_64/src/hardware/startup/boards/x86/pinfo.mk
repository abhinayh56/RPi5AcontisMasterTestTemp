define PINFO
PINFO DESCRIPTION=x86 startup
endef

PADDR_SIZE_x86 = $(firstword $(filter 32, $(VARIANTS)) 64)
PADDR_SIZE=$(PADDR_SIZE_$(CPU))
LIB_VARIANT := $(subst .32,,$(subst .o,,a.$(subst .$(VADDR_SIZE)bit,,$(COMPOUND_VARIANT)))$(PSTAG_$(PADDR_SIZE))$(VSTAG_$(VADDR_SIZE)))
