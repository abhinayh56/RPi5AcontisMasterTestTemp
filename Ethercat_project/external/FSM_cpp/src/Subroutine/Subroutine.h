#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include <stdint.h>
#include "CallbackStatus.h"
#include "EcState.h"

class Subroutine
{
public:
    Subroutine();
    virtual ~Subroutine();
    virtual uint32_t setTaskAddr(
        EcTaskEthercatSlave *p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl *p_ecTaskRobotControl_,
        EcTaskUser *p_ecTaskUser_,
        EcTaskInterface *p_ecTaskInterface_);
    virtual uint32_t config();
    virtual uint32_t callback();

protected:
    EcStateData::EcTaskAll ecTaskAll;
};

class SubroutineTransition
{
public:
    SubroutineTransition();
    virtual ~SubroutineTransition();
    virtual uint32_t setTaskAddr(
        EcTaskEthercatSlave *p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl *p_ecTaskRobotControl_,
        EcTaskUser *p_ecTaskUser_,
        EcTaskInterface *p_ecTaskInterface_);
    virtual uint32_t config();
    virtual uint32_t callback(uint32_t &nextStateId);

protected:
    EcStateData::EcTaskAll ecTaskAll;
};

#endif // SUBROUTINE_H
