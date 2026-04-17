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

# App Ethercat Slave
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcTaskEthercatSlaveBase
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcTaskEthercatSlaveServoBase
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlaveBase
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcCia402
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl1008
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl2008
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl3024
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl6002
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveRfidslave
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveSscIoModule
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEncbrkctrl
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlavePitchDrive
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveDenNetE
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveElmoDrive
INCLUDES += -I$(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveIpos2401MxCat

# App FSM
INCLUDES += -I$(DIR_APP_FSM)/EcState
INCLUDES += -I$(DIR_APP_FSM)/EcStateClearingFault
INCLUDES += -I$(DIR_APP_FSM)/EcStateDisabled
INCLUDES += -I$(DIR_APP_FSM)/EcStateDisabling
INCLUDES += -I$(DIR_APP_FSM)/EcStateEnabled
INCLUDES += -I$(DIR_APP_FSM)/EcStateEnabling
INCLUDES += -I$(DIR_APP_FSM)/EcStateFault
INCLUDES += -I$(DIR_APP_FSM)/EcStateInitialized
INCLUDES += -I$(DIR_APP_FSM)/EcStateInitializing
INCLUDES += -I$(DIR_APP_FSM)/EcStateJoystickControl
INCLUDES += -I$(DIR_APP_FSM)/EcStateReady
INCLUDES += -I$(DIR_APP_FSM)/EcStateStandby

# App User
INCLUDES += -I$(DIR_APP_USER)/EcTaskUserBase
INCLUDES += -I$(DIR_APP_USER)/EcUser_1
INCLUDES += -I$(DIR_APP_USER)/EcUser_2
INCLUDES += -I$(DIR_APP_USER)/EcUser_3

# App Robot Control
INCLUDES += -I$(DIR_APP_ROBOT_CONTROL)/EcTaskRobotControlBase

# App Interface
INCLUDES += -I$(DIR_APP_INTERFACE)/EcTaskInterfaceBase

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

# App Ethercat Slave
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcTaskEthercatSlaveBase/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcTaskEthercatSlaveServoBase/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlaveBase/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcCia402/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl1008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl2008/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl3024/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEl6002/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveRfidslave/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveSscIoModule/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveEncbrkctrl/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlavePitchDrive/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveDenNetE/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveElmoDrive/*.cpp)
SRCS += $(wildcard $(DIR_APP_ETHERCAT_SLAVE)/EcSlave/EcSlaveIpos2401MxCat/*.cpp)

# App FSM
SRCS += $(wildcard $(DIR_APP_FSM)/EcState/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateClearingFault/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateDisabled/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateDisabling/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateEnabled/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateEnabling/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateFault/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateInitialized/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateInitializing/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateJoystickControl/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateReady/*.cpp)
SRCS += $(wildcard $(DIR_APP_FSM)/EcStateStandby/*.cpp)

# App User
SRCS += $(wildcard $(DIR_APP_USER)/EcTaskUserBase/*.cpp)
SRCS += $(wildcard $(DIR_APP_USER)/EcUser_1/*.cpp)
SRCS += $(wildcard $(DIR_APP_USER)/EcUser_2/*.cpp)
SRCS += $(wildcard $(DIR_APP_USER)/EcUser_3/*.cpp)

# App Robot Control
SRCS += $(wildcard $(DIR_APP_ROBOT_CONTROL)/EcTaskRobotControlBase/*.cpp)

# App Interface
SRCS += $(wildcard $(DIR_APP_INTERFACE)/EcTaskInterfaceBase/*.cpp)

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