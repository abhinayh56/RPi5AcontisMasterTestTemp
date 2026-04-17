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