#include "EcStateEnabled.h"

EcStateEnabledNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateEnabledNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::callback()
{
	std::cout << "ENABLED ENTRY" << std::endl;
    return 0;
}

EcStateEnabledNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateEnabledNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateEnabledNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnExitSubroutine::callback()
{
	std::cout << "ENABLED EXIT" << std::endl;
    return 0;
}

EcStateEnabledNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateEnabledNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateEnabledNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::CallbackSubroutine::callback()
{
	std::cout << "ENABLED CALLBACK" << std::endl;
	ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();
    return 0;
}

EcStateEnabledNs::TransitionSubroutine::TransitionSubroutine() :
		m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true)
{
}

EcStateEnabledNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateEnabledNs::TransitionSubroutine::config()
{
	m_InputCh_3.subscribe();
    return 0;
}

uint32_t EcStateEnabledNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
	m_InputCh_3.get(m_data_3);

	if(ecTaskAll.p_ecTaskEthercatSlaveServo->isFaultClear() == 0)
    {
        std::cout << "ENABLED TRANSITION" << std::endl;
    	nextStateId = EcStateData::StateId::FAULT;
    }
    else if(m_data_3 == true)
	{
		std::cout << "ENABLED TRANSITION" << std::endl;
		nextStateId = EcStateData::StateId::DISABLING;
	}
	else
	{
		nextStateId = EcStateData::StateId::ENABLED;
	}


    return CallbackStatus::SUCCESS;
}
