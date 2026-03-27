ARTIFACT = Ethercat_project

#Build architecture/variant string, possible values: x86, armv7le, etc...
PLATFORM ?= aarch64le

#Build profile, possible values: release, debug, profile, coverage
BUILD_PROFILE ?= debug

CONFIG_NAME ?= $(PLATFORM)-$(BUILD_PROFILE)
OUTPUT_DIR = build/$(CONFIG_NAME)
TARGET = $(OUTPUT_DIR)/$(ARTIFACT)

#Compiler definitions

CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CXX)

#User defined include/preprocessor flags and libraries

#INCLUDES += -I/path/to/my/lib/include
#INCLUDES += -I../mylib/public
DIR_EC_SDK = ./External/EC-Master-V3.2-QNX8-ARM_64Bit-Eval
DIR_APP_ETHERCAT = ./App/App_ethercat
#DIR_APP_ETHERCAT = ./External/EC-Master-V3.2-QNX8-ARM_64Bit-Eval/Examples

# External EtherCAT
INCLUDES += -I$(DIR_EC_SDK)/SDK/INC
INCLUDES += -I$(DIR_EC_SDK)/SDK/INC/QNX
INCLUDES += -I$(DIR_EC_SDK)/Sources/Common

# EtherCAT App (main)
INCLUDES += -I$(DIR_APP_ETHERCAT)/Common
INCLUDES += -I$(DIR_APP_ETHERCAT)/Common/QNX

INCLUDES += -I$(DIR_APP_ETHERCAT)/EcMasterDemo

INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_base
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_cia402
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el1008
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el2008
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el3024
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el6002
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_io_module
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_encbrkctrl
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_rfidslave
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_den_net_e
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_elmo_drive
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_ipos2401_mx_cat
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_pitch_drive
INCLUDES += -I$(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_manager

#LIBS += -L/path/to/my/lib/$(PLATFORM)/usr/lib -lmylib
#LIBS += -L../mylib/$(OUTPUT_DIR) -lmylib
LIBS += -L$(DIR_EC_SDK)/SDK/LIB/QNX8/aarch64

LIBS += -lEcMasterRasServer
LIBS += -lEcMaster
LIBS += -lsocket

#Compiler flags for build profiles
CCFLAGS_release += -O2
CCFLAGS_debug += -g -O0 -fno-builtin
CCFLAGS_coverage += -g -O0 -ftest-coverage -fprofile-arcs
LDFLAGS_coverage += -ftest-coverage -fprofile-arcs
CCFLAGS_profile += -g -O0 -finstrument-functions
LIBS_profile += -lprofilingS

#Generic compiler flags (which include build type flags)
CCFLAGS_all += -Wall -fmessage-length=0
CCFLAGS_all += $(CCFLAGS_$(BUILD_PROFILE))
CCFLAGS_all += -Wno-packed-not-aligned
#Shared library has to be compiled with -fPIC
#CCFLAGS_all += -fPIC
LDFLAGS_all += $(LDFLAGS_$(BUILD_PROFILE))
LIBS_all += $(LIBS_$(BUILD_PROFILE))
DEPS = -Wp,-MMD,$(@:%.o=%.d),-MT,$@

#Macro to expand files recursively: parameters $1 -  directory, $2 - extension, i.e. cpp
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

#Source list
#SRCS = $(call rwildcard, src, c cpp)

# External EtherCAT
SRCS += $(wildcard $(DIR_EC_SDK)/Sources/Common/*.cpp)

# EtherCAT App (main)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Common/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Common/QNX/*.cpp)

SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcMasterDemo/*.cpp)

SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_base/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_cia402/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el1008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el2008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el3024/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_el6002/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_io_module/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_encbrkctrl/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_rfidslave/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_den_net_e/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_elmo_drive/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_ipos2401_mx_cat/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_pitch_drive/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/Ec_slave/Ec_slave_manager/*.cpp)

#Object files list
OBJS = $(addprefix $(OUTPUT_DIR)/,$(addsuffix .o, $(basename $(SRCS))))

#Compiling rule
$(OUTPUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<
$(OUTPUT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<

#Linking rule
$(TARGET):$(OBJS)
	$(LD) -o $(TARGET) $(LDFLAGS_all) $(LDFLAGS) $(OBJS) $(LIBS_all) $(LIBS)

#Rules section for default compilation and linking
all: $(TARGET)

clean:
	rm -fr $(OUTPUT_DIR)

rebuild: clean all

#Inclusion of dependencies (object files to source and includes)
-include $(OBJS:%.o=%.d)