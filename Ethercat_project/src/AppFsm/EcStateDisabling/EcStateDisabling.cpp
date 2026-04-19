#include "EcStateDisabling.h"

EcStateDisabling::EcStateDisabling() : EcTaskStateBase("STANDBY", EcStateData::StateId::STANDBY)
{
}

EcStateDisabling::~EcStateDisabling()
{
}

uint32_t EcStateDisabling::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateDisablingNs::OnEntrySubroutine());
    addOnExit(new EcStateDisablingNs::OnExitSubroutine());
    addCallback(new EcStateDisablingNs::CallbackSubroutine());
    addTransition(new EcStateDisablingNs::TransitionSubroutine());

    return dwRes;
}

EcStateDisablingNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateDisablingNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::callback()
{
    std::cout << "DISABLING ENTRY" << std::endl;
    return 0;
}

EcStateDisablingNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateDisablingNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateDisablingNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnExitSubroutine::callback()
{
    std::cout << "DISABLING EXIT" << std::endl;
    return 0;
}

EcStateDisablingNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateDisablingNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateDisablingNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::CallbackSubroutine::callback()
{
    std::cout << "DISABLING CALLBACK" << std::endl;
    ecTaskAll.p_ecTaskEthercatSlaveServo->disable();
    ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();
    return 0;
}

EcStateDisablingNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateDisablingNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateDisablingNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    if(ecTaskAll.p_ecTaskEthercatSlaveServo->isFaultClear() == 0)
    {
        std::cout << "DISABLING TRANSITION" << std::endl;
    	nextStateId = EcStateData::StateId::FAULT;
    }
    else if (ecTaskAll.p_ecTaskEthercatSlaveServo->isDisable() == 0)
    {
        std::cout << "DISABLING TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::DISABLED;
    }
    else
    {
        nextStateId = EcStateData::StateId::DISABLING;
    }

    return CallbackStatus::SUCCESS;
}
