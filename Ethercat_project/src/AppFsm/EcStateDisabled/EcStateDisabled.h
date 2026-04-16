#ifndef EC_STATE_DISABLED_H
#define EC_STATE_DISABLED_H

#include "EcState.h"
#include "Subroutine.h"

namespace EcStateDisabledNs
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

    private:
        Data_store_element<bool> m_InputCh_1;
        Data_store_element<bool> m_InputCh_2;
        Data_store_element<bool> m_InputCh_3;
        Data_store_element<bool> m_InputCh_4;
        Data_store_element<bool> m_InputCh_5;
        Data_store_element<bool> m_InputCh_6;
        Data_store_element<bool> m_InputCh_7;
        Data_store_element<bool> m_InputCh_8;

        bool m_data_1 = false;
        bool m_data_2 = false;
        bool m_data_3 = false;
        bool m_data_4 = false;
        bool m_data_5 = false;
        bool m_data_6 = false;
        bool m_data_7 = false;
        bool m_data_8 = false;
    };

    class TransitionSubroutine : public SubroutineTransition
    {
    public:
        TransitionSubroutine();

        ~TransitionSubroutine();

        uint32_t config() override;

        uint32_t callback(uint32_t &nextStateId) override;

    private:
        Data_store_element<bool> m_InputCh_7;
        bool m_data_7 = false;
    };
}

#endif // EC_STATE_DISABLED_H
