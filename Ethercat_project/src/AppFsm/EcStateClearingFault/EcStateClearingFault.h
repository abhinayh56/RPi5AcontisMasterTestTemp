#ifndef EC_STATE_CLEARING_FAULT_H
#define EC_STATE_CLEARING_FAULT_H

#include "EcState.h"
#include "Subroutine.h"

namespace EcStateClearingFaultNs
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
        	std::cout << "CLEARING FAULT ENTRY" << std::endl;
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
        	std::cout << "CLEARING FAULT EXIT" << std::endl;
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
        	ecTaskAll.p_ecTaskEthercatSlaveServo->clearFault();
        	std::cout << "CLEARING FAULT CALLBACK" << std::endl;
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
            if(ecTaskAll.p_ecTaskEthercatSlaveServo->checkFault() == 0)
            {
            	std::cout << "CLEARING FAULT TRANSITION" << std::endl;
                nextStateId = EcStateData::StateId::STANDBY;
            }
            else
            {
                nextStateId = EcStateData::StateId::CLEARING_FAULT;
            }

            return CallbackStatus::SUCCESS;
        }
    };
}

#endif // EC_STATE_CLEARING_FAULT_H
