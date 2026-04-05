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
DIR_EXTERNAL = ./external
DIR_EC_SDK = ./external/EC-Master-V3.2-QNX8-ARM_64Bit-Eval
DIR_APP_MAIN = ./src/AppMain
DIR_APP_ETHERCAT = ./src/AppEthercatSlave
#DIR_APP_ETHERCAT = ./external/EC-Master-V3.2-QNX8-ARM_64Bit-Eval/Examples
DIR_ITC = ./external/ITC
DIR_EIGEN = ./external/eigen-5.0.0

# External EtherCAT
INCLUDES += -I$(DIR_EC_SDK)/SDK/INC
INCLUDES += -I$(DIR_EC_SDK)/SDK/INC/QNX
INCLUDES += -I$(DIR_EC_SDK)/Sources/Common

# EtherCAT App (main)
INCLUDES += -I$(DIR_APP_ETHERCAT)/Common
INCLUDES += -I$(DIR_APP_ETHERCAT)/Common/QNX

INCLUDES += -I$(DIR_APP_ETHERCAT)/EcMasterDemo

INCLUDES += -I$(DIR_ITC)
INCLUDES += -I$(DIR_ITC)/Data_store
INCLUDES += -I$(DIR_ITC)/Data_store_element
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcPdoHandler
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveBase
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcCia402
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl1008
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl2008
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl3024
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl6002
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveSscIoModule
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEncbrkctrl
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveRfidslave
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveDenNetE
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveElmoDrive
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlaveIpos2401MxCat
INCLUDES += -I$(DIR_APP_ETHERCAT)/EcSlave/EcSlavePitchDrive

INCLUDES += -I$(DIR_APP_MAIN)/EcTaskEthercatSlave
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskRobotControl
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskUser
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskInterface
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskManager

# Eigen library (header-only)
INCLUDES += -I$(DIR_EIGEN)

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

SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskEthercatSlave/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskRobotControl/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskUser/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskInterface/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskManager/*.cpp)

SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveBase/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcCia402/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl1008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl2008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl3024/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEl6002/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveSscIoModule/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveEncbrkctrl/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveRfidslave/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveDenNetE/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveElmoDrive/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlaveIpos2401MxCat/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT)/EcSlave/EcSlavePitchDrive/*.cpp)

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