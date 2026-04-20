#include "EcStateInitializing.h"

EcStateInitializing::EcStateInitializing() : EcTaskStateBase("INITIALIZING", EcStateData::StateId::INITIALIZING)
{
}

EcStateInitializing::~EcStateInitializing()
{
}

uint32_t EcStateInitializing::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateInitializingNs::OnEntrySubroutine());
    addOnExit(new EcStateInitializingNs::OnExitSubroutine());
    addCallback(new EcStateInitializingNs::CallbackSubroutine());
    addTransition(new EcStateInitializingNs::TransitionSubroutine());

    return dwRes;
}

EcStateInitializingNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateInitializingNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateInitializingNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializingNs::OnEntrySubroutine::callback()
{
    return 0;
}

EcStateInitializingNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateInitializingNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateInitializingNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializingNs::OnExitSubroutine::callback()
{
    return 0;
}

EcStateInitializingNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateInitializingNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateInitializingNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializingNs::CallbackSubroutine::callback()
{
    return 0;
}

EcStateInitializingNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateInitializingNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateInitializingNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializingNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    nextStateId = EcStateData::StateId::DISABLING;

    return CallbackStatus::SUCCESS;
}
