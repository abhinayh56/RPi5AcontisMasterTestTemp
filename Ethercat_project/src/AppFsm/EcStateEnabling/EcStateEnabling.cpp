#include "EcStateEnabling.h"

EcStateEnabling::EcStateEnabling() : EcTaskStateBase("ENABLING", EcStateData::StateId::ENABLING)
{
}

EcStateEnabling::~EcStateEnabling()
{
}

uint32_t EcStateEnabling::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateEnablingNs::OnEntrySubroutine());
    addOnExit(new EcStateEnablingNs::OnExitSubroutine());
    addCallback(new EcStateEnablingNs::CallbackSubroutine());
    addTransition(new EcStateEnablingNs::TransitionSubroutine());

    return dwRes;
}

EcStateEnablingNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateEnablingNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::callback()
{
    std::cout << "ENABLING ENTRY" << std::endl;
    return 0;
}

EcStateEnablingNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateEnablingNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateEnablingNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnExitSubroutine::callback()
{
    std::cout << "ENABLING EXIT" << std::endl;
    return 0;
}

EcStateEnablingNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateEnablingNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateEnablingNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::CallbackSubroutine::callback()
{
    std::cout << "ENABLING CALLBACK" << std::endl;
    ecTaskAll.p_ecTaskEthercatSlaveServo->enable(); 
    ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();
    return 0;
}

EcStateEnablingNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateEnablingNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateEnablingNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
	if(ecTaskAll.p_ecTaskEthercatSlaveServo->isFaultClear() == 0)
    {
        std::cout << "ENABLING TRANSITION" << std::endl;
    	nextStateId = EcStateData::StateId::FAULT;
    }
    else if(ecTaskAll.p_ecTaskEthercatSlaveServo->isEnable() == 0)
    {
        std::cout << "ENABLING TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::ENABLED;
    }
	else
    {
        nextStateId = EcStateData::StateId::ENABLING;
    }

    return CallbackStatus::SUCCESS;
}
