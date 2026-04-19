#ifndef EC_STATE_FAULT_H
#define EC_STATE_FAULT_H

#include "EcTaskStateBase.h"
#include "Subroutine.h"

class EcStateFault : public EcTaskStateBase
{
public:
    EcStateFault();
    ~EcStateFault();
    virtual uint32_t addSubroutine() override;
};

namespace EcStateFaultNs
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

    private:
        Data_store_element<bool> m_InputCh_2;
        bool m_data_2 = false;
    };
}

#endif // EC_STATE_FAULT_H
