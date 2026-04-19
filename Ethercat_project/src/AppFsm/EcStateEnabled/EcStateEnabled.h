#ifndef EC_STATE_ENABLED_H
#define EC_STATE_ENABLED_H

#include "EcTaskStateBase.h"
#include "Subroutine.h"

class EcStateEnabled : public EcTaskStateBase
{
public:
    EcStateEnabled();
    ~EcStateEnabled();
    virtual uint32_t addSubroutine() override;
};

namespace EcStateEnabledNs
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
        Data_store_element<bool> m_InputCh_3;
		bool m_data_3 = false;
    };
}

#endif // EC_STATE_ENABLED_H
