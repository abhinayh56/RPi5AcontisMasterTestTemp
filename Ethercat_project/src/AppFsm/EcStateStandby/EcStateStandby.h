#ifndef EC_STATE_STANDBY_H
#define EC_STATE_STANDBY_H

#include "Subroutine.h"

namespace EcStateStandbyNs
{
    class OnEntrySubroutine : public Subroutine
    {
    public:
        OnEntrySubroutine()
        {
        }

        ~OnEntrySubroutine()
        {
        }

        uint32_t config() override
        {
            return 0;
        }

        uint32_t callback() override
        {
            return 0;
        }
    };

    class OnExitSubroutine : public Subroutine
    {
    public:
        OnExitSubroutine()
        {
        }

        ~OnExitSubroutine()
        {
        }

        uint32_t config() override
        {
            return 0;
        }

        uint32_t callback() override
        {
            return 0;
        }
    };

    class CallbackSubroutine : public Subroutine
    {
    public:
    	CallbackSubroutine()
        {
        }

        ~CallbackSubroutine()
        {
        }

        uint32_t config() override
        {
            return 0;
        }

        uint32_t callback() override
        {
            return 0;
        }
    };

    class TransitionSubroutine : public SubroutineTransition
    {
    public:
        TransitionSubroutine()
        {
        }

        ~TransitionSubroutine()
        {
        }

        uint32_t config() override
        {
            return 0;
        }

        uint32_t callback(uint32_t& nextStateId) override
        {
            nextStateId = 0;

            return CallbackStatus::SUCCESS;
        }
    };
}

#endif // EC_STATE_STANDBY_H

// #ifndef EC_STATE_STANDBY_H
// #define EC_STATE_STANDBY_H

// #include "EcTaskEthercatSlave.h"
// #include "EcTaskEthercatSlaveServo.h"
// #include "EcTaskRobotControl.h"
// #include "EcTaskUser.h"
// #include "EcTaskInterface.h"

// class EcStateStandby
// {
// public:
//     EcStateStandby(
//         EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
//         EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
//         EcTaskRobotControl* p_ecTaskRobotControl_,
//         EcTaskUser* p_ecTaskUser_,
//         EcTaskInterface* p_ecTaskInterface_);
        
//     ~EcStateStandby();

// private:
//     EcTaskEthercatSlave *p_ecTaskEthercatSlave;
//     EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo;
//     EcTaskRobotControl *p_ecTaskRobotControl;
//     EcTaskUser *p_ecTaskUser;
//     EcTaskInterface *p_ecTaskInterface;
// };

// #endif // EC_STATE_STANDBY_H
