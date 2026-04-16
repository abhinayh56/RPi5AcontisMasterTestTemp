#ifndef EC_STATE_DISABLED_H
#define EC_STATE_DISABLED_H

#include "EcState.h"
#include "Subroutine.h"

namespace EcStateDisabledNs
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
            std::cout << "DISABLED ENTRY" << std::endl;
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
            std::cout << "DISABLED EXIT" << std::endl;
            return 0;
        }
    };

    class CallbackSubroutine : public Subroutine
    {
    public:
        CallbackSubroutine() :
            m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/el1008", false, true),
            m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true),
            m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true),
            m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/el1008", false, true),
            m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/el1008", false, true),
            m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/el1008", false, true),
            m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true),
            m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/el1008", false, true)
        {
        }

        ~CallbackSubroutine()
        {
        }

        uint32_t config() override
        {
            m_InputCh_1.subscribe();
            m_InputCh_2.subscribe();
            m_InputCh_3.subscribe();
            m_InputCh_4.subscribe();
            m_InputCh_5.subscribe();
            m_InputCh_6.subscribe();
            m_InputCh_7.subscribe();
            m_InputCh_8.subscribe();

            return 0;
        }

        uint32_t callback() override
        {
            std::cout << "DISABLED CALLBACK" << std::endl;

            m_InputCh_1.get(m_data_1);
            m_InputCh_2.get(m_data_2);
            m_InputCh_3.get(m_data_3);
            m_InputCh_4.get(m_data_4);
            m_InputCh_5.get(m_data_5);
            m_InputCh_6.get(m_data_6);
            m_InputCh_7.get(m_data_7);
            m_InputCh_8.get(m_data_8);

            // if(m_data_1 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
            // if(m_data_2 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->clearFault();      std::cout << "---\nclearFault      : "<< out << std::endl;}
            // if(m_data_3 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
            if(m_data_4 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModePosition(); std::cout << "---\nsetModePosition : "<< out << std::endl;}
            if(m_data_5 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeVelocity(); std::cout << "---\nsetModeVelocity : "<< out << std::endl;}
            if(m_data_6 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeTorque();   std::cout << "---\nsetModeTorque   : "<< out << std::endl;}
            // if(m_data_7 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->enable();          std::cout << "---\nenable          : "<< out << std::endl;}
            // if(m_data_8 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

            ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();
            
            return 0;
        }

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
        TransitionSubroutine() :
            m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true)
        {
        }

        ~TransitionSubroutine()
        {
        }

        uint32_t config() override
        {
            m_InputCh_7.subscribe();
            return 0;
        }

        uint32_t callback(uint32_t& nextStateId) override
        {
            m_InputCh_7.get(m_data_7);

        	if(ecTaskAll.p_ecTaskEthercatSlaveServo->checkFault() != 0)
			{
                std::cout << "DISABLED TRANSITION" << std::endl;
				nextStateId = EcStateData::StateId::FAULT;
			}
            else if(m_data_7 == true)
            {
                std::cout << "DISABLED TRANSITION" << std::endl;
                nextStateId = EcStateData::StateId::ENABLING;
            }
        	else
        	{
        		nextStateId = EcStateData::StateId::DISABLED;
        	}

            return CallbackStatus::SUCCESS;
        }

    private:
        Data_store_element<bool> m_InputCh_7;
        bool m_data_7 = false;
    };
}

#endif // EC_STATE_DISABLED_H
