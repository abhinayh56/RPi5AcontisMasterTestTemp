#include "EcStateClearingFault.h"

EcStateClearingFault::EcStateClearingFault() : EcTaskStateBase("STANDBY", EcStateData::StateId::STANDBY)
{
}

EcStateClearingFault::~EcStateClearingFault()
{
}

uint32_t EcStateClearingFault::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateClearingFaultNs::OnEntrySubroutine());
    addOnExit(new EcStateClearingFaultNs::OnExitSubroutine());
    addCallback(new EcStateClearingFaultNs::CallbackSubroutine());
    addTransition(new EcStateClearingFaultNs::TransitionSubroutine());

    return dwRes;
}

EcStateClearingFaultNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateClearingFaultNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::callback()
{
    std::cout << "CLEARING FAULT ENTRY" << std::endl;
    return 0;
}

EcStateClearingFaultNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateClearingFaultNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::callback()
{
    std::cout << "CLEARING FAULT EXIT" << std::endl;
    return 0;
}

EcStateClearingFaultNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateClearingFaultNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::callback()
{
    ecTaskAll.p_ecTaskEthercatSlaveServo->faultClear();
    std::cout << "CLEARING FAULT CALLBACK" << std::endl;
    return 0;
}

EcStateClearingFaultNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateClearingFaultNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    if (ecTaskAll.p_ecTaskEthercatSlaveServo->isFaultClear() == 0)
    {
        std::cout << "CLEARING FAULT TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::STANDBY;
    }
    else
    {
        nextStateId = EcStateData::StateId::CLEARING_FAULT;
    }

    return CallbackStatus::SUCCESS;
}
