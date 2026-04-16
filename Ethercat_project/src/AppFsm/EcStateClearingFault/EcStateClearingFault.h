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
            nextStateId = EcStateData::StateId::INITIALIZING;

            return CallbackStatus::SUCCESS;
        }
    };
}

#endif // EC_STATE_CLEARING_FAULT_H
