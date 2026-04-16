#ifndef EC_STATE_ENABLING_H
#define EC_STATE_ENABLING_H

#include "EcState.h"
#include "Subroutine.h"

namespace EcStateEnablingNs
{
    class OnEntrySubroutine : public Subroutine
    {
    public:
        OnEntrySubroutine();

        ~OnEntrySubroutine();

        uint32_t config() override;

        uint32_t callback() override;
    };

    class OnExitSubroutine : public Subroutine
    {
    public:
        OnExitSubroutine();

        ~OnExitSubroutine();

        uint32_t config() override;

        uint32_t callback() override;
    };

    class CallbackSubroutine : public Subroutine
    {
    public:
        CallbackSubroutine();

        ~CallbackSubroutine();

        uint32_t config() override;

        uint32_t callback() override;
    };

    class TransitionSubroutine : public SubroutineTransition
    {
    public:
        TransitionSubroutine();

        ~TransitionSubroutine();

        uint32_t config() override;

        uint32_t callback(uint32_t &nextStateId) override;
    };
}

#endif // EC_STATE_ENABLING_H
