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

DIR_EXT = ./external
DIR_SRC = ./src

DIR_EXT_EC_SDK = $(DIR_EXT)/EC-Master-V3.2-QNX8-ARM_64Bit-Eval
DIR_EXT_ITC    = $(DIR_EXT)/ITC
DIR_EXT_FSM    = $(DIR_EXT)/FSM_cpp/src
DIR_EXT_EIGEN  = $(DIR_EXT)/eigen-5.0.0

DIR_APP_ETHERCAT_MASTER = $(DIR_SRC)/AppEthercatMaster
DIR_APP_ETHERCAT_SLAVE  = $(DIR_SRC)/AppEthercatSlave
DIR_APP_FSM             = $(DIR_SRC)/AppFsm
DIR_APP_INTERFACE       = $(DIR_SRC)/AppInterface
DIR_APP_MAIN            = $(DIR_SRC)/AppMain
DIR_APP_ROBOT_CONTROL   = $(DIR_SRC)/AppRobotControl
DIR_APP_USER            = $(DIR_SRC)/AppUser

# External EtherCAT
INCLUDES += -I$(DIR_EXT_EC_SDK)/SDK/INC
INCLUDES += -I$(DIR_EXT_EC_SDK)/SDK/INC/QNX
INCLUDES += -I$(DIR_EXT_EC_SDK)/Sources/Common

# External ITC
INCLUDES += -I$(DIR_EXT_ITC)
INCLUDES += -I$(DIR_EXT_ITC)/Data_store
INCLUDES += -I$(DIR_EXT_ITC)/Data_store_element

# External FSM
INCLUDES += -I$(DIR_EXT_FSM)/Fsm
INCLUDES += -I$(DIR_EXT_FSM)/State
INCLUDES += -I$(DIR_EXT_FSM)/Subroutine

# External Eigen
INCLUDES += -I$(DIR_EXT_EIGEN)

# App Ethercat Master
INCLUDES += -I$(DIR_APP_ETHERCAT_MASTER)/Common
INCLUDES += -I$(DIR_APP_ETHERCAT_MASTER)/Common/QNX
INCLUDES += -I$(DIR_APP_ETHERCAT_MASTER)/EcMasterDemo
INCLUDES += -I$(DIR_APP_ETHERCAT_MASTER)/EcPdoHandler

include app_includes.mk

# App Main
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskEthercatSlave
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskEthercatSlaveServo
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskRobotControl
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskUser
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskFsm
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskInterface
INCLUDES += -I$(DIR_APP_MAIN)/EcTaskManager

#LIBS += -L/path/to/my/lib/$(PLATFORM)/usr/lib -lmylib
#LIBS += -L../mylib/$(OUTPUT_DIR) -lmylib
LIBS += -L$(DIR_EXT_EC_SDK)/SDK/LIB/QNX8/aarch64
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
SRCS += $(wildcard $(DIR_EXT_EC_SDK)/Sources/Common/*.cpp)

# External FSM
SRCS += $(wildcard $(DIR_EXT_FSM)/Fsm/*.cpp)
SRCS += $(wildcard $(DIR_EXT_FSM)/State/*.cpp)
SRCS += $(wildcard $(DIR_EXT_FSM)/Subroutine/*.cpp)

# App Ethercat Master
SRCS += $(wildcard $(DIR_APP_ETHERCAT_MASTER)/Common/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_MASTER)/Common/QNX/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_MASTER)/EcMasterDemo/*.cpp)

include app_sources.mk

# App Main
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskEthercatSlave/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskEthercatSlaveServo/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskRobotControl/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskUser/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskFsm/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskInterface/*.cpp)
SRCS += $(wildcard $(DIR_APP_MAIN)/EcTaskManager/*.cpp)

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