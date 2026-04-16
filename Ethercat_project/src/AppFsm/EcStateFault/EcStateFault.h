#ifndef EC_STATE_FAULT_H
#define EC_STATE_FAULT_H

#include "EcState.h"
#include "Subroutine.h"

namespace EcStateFaultNs
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
            std::cout << "FAULT ENTRY" << std::endl;
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
            std::cout << "FAULT EXIT" << std::endl;
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
            std::cout << "FAULT CALLBACK" << std::endl;
            return 0;
        }
    };

    class TransitionSubroutine : public SubroutineTransition
    {
    public:
        TransitionSubroutine() :
            m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true)
        {
        }

        ~TransitionSubroutine()
        {
        }

        uint32_t config() override
        {
            m_InputCh_2.subscribe();
            return 0;
        }

        uint32_t callback(uint32_t& nextStateId) override
        {
            m_InputCh_2.get(m_data_2);
            if(m_data_2)
			{
                std::cout << "FAULT TRANSITION" << std::endl;
				nextStateId = EcStateData::StateId::CLEARING_FAULT;
			}
            else
            {
                nextStateId = EcStateData::StateId::FAULT;
            }

            return CallbackStatus::SUCCESS;
        }

    private:
        Data_store_element<bool> m_InputCh_2;
        bool m_data_2 = false;
    };
}

#endif // EC_STATE_FAULT_H
