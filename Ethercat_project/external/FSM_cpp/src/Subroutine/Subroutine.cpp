#include "Subroutine.h"

Subroutine::Subroutine()
{
}

Subroutine::~Subroutine()
{
}

uint32_t Subroutine::setTaskAddr(
    EcTaskEthercatSlave *p_ecTaskEthercatSlave_,
    EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo_,
    EcTaskRobotControl *p_ecTaskRobotControl_,
    EcTaskUser *p_ecTaskUser_,
    EcTaskInterface *p_ecTaskInterface_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll.p_ecTaskEthercatSlave = p_ecTaskEthercatSlave_;
    ecTaskAll.p_ecTaskEthercatSlaveServo = p_ecTaskEthercatSlaveServo_;
    ecTaskAll.p_ecTaskRobotControl = p_ecTaskRobotControl_;
    ecTaskAll.p_ecTaskUser = p_ecTaskUser_;
    ecTaskAll.p_ecTaskInterface = p_ecTaskInterface_;

    return dwRes;
}

uint32_t Subroutine::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t Subroutine::callback()
{
    return CallbackStatus::SUCCESS;
}

SubroutineTransition::SubroutineTransition()
{
}

SubroutineTransition::~SubroutineTransition()
{
}

uint32_t SubroutineTransition::setTaskAddr(
    EcTaskEthercatSlave *p_ecTaskEthercatSlave_,
    EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo_,
    EcTaskRobotControl *p_ecTaskRobotControl_,
    EcTaskUser *p_ecTaskUser_,
    EcTaskInterface *p_ecTaskInterface_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll.p_ecTaskEthercatSlave = p_ecTaskEthercatSlave_;
    ecTaskAll.p_ecTaskEthercatSlaveServo = p_ecTaskEthercatSlaveServo_;
    ecTaskAll.p_ecTaskRobotControl = p_ecTaskRobotControl_;
    ecTaskAll.p_ecTaskUser = p_ecTaskUser_;
    ecTaskAll.p_ecTaskInterface = p_ecTaskInterface_;

    return dwRes;
}

uint32_t SubroutineTransition::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineTransition::callback(uint32_t &nextStateId)
{
    return CallbackStatus::SUCCESS;
}
