#ifndef EC_STATE_INITIALIZING_H
#define EC_STATE_INITIALIZING_H

#include "Subroutine.h"

namespace EcStateInitializingNs
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

#endif // EC_STATE_INITIALIZING_H
