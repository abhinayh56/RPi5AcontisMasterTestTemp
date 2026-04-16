#include "EcStateFault.h"

EcStateFaultNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateFaultNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateFaultNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::OnEntrySubroutine::callback()
{
    std::cout << "FAULT ENTRY" << std::endl;
    return 0;
}

EcStateFaultNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateFaultNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateFaultNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::OnExitSubroutine::callback()
{
    std::cout << "FAULT EXIT" << std::endl;
    return 0;
}

EcStateFaultNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateFaultNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateFaultNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::CallbackSubroutine::callback()
{
    std::cout << "FAULT CALLBACK" << std::endl;
    return 0;
}

EcStateFaultNs::TransitionSubroutine::TransitionSubroutine() : m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true)
{
}

EcStateFaultNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateFaultNs::TransitionSubroutine::config()
{
    m_InputCh_2.subscribe();
    return 0;
}

uint32_t EcStateFaultNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    m_InputCh_2.get(m_data_2);
    if (m_data_2)
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
