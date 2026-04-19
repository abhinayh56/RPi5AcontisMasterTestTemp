#ifndef EC_STATE_READY_H
#define EC_STATE_READY_H

#include "EcTaskStateBase.h"
#include "Subroutine.h"

class EcStateReady : public EcTaskStateBase
{
public:
    EcStateReady();
    ~EcStateReady();
    virtual uint32_t addSubroutine() override;
};

namespace EcStateReadyNs
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

        uint32_t callback(uint32_t& nextStateId) override;
    };
}

#endif // EC_STATE_READY_H
