#include "EcStateStandby.h"

EcStateStandby::EcStateStandby(
        EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl* p_ecTaskRobotControl_,
        EcTaskUser* p_ecTaskUser_,
        EcTaskInterface* p_ecTaskInterface_) :
    p_ecTaskEthercatSlave(p_ecTaskEthercatSlave_),
    p_ecTaskEthercatSlaveServo(p_ecTaskEthercatSlaveServo_),
    p_ecTaskRobotControl(p_ecTaskRobotControl_),
    p_ecTaskUser(p_ecTaskUser_),
    p_ecTaskInterface(p_ecTaskInterface_)
{
}

EcStateStandby::~EcStateStandby()
{
}
