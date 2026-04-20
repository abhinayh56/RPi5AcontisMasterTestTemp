#include "EcStateReady.h"

EcStateReady::EcStateReady() : EcTaskStateBase("READY", EcStateData::StateId::READY)
{
}

EcStateReady::~EcStateReady()
{
}

uint32_t EcStateReady::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateReadyNs::OnEntrySubroutine());
    addOnExit(new EcStateReadyNs::OnExitSubroutine());
    addCallback(new EcStateReadyNs::CallbackSubroutine());
    addTransition(new EcStateReadyNs::TransitionSubroutine());

    return dwRes;
}

EcStateReadyNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateReadyNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateReadyNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::OnEntrySubroutine::callback()
{
    return 0;
}

EcStateReadyNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateReadyNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateReadyNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::OnExitSubroutine::callback()
{
    return 0;
}

EcStateReadyNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateReadyNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateReadyNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::CallbackSubroutine::callback()
{
    return 0;
}

EcStateReadyNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateReadyNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateReadyNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    nextStateId = EcStateData::StateId::DISABLING;

    return CallbackStatus::SUCCESS;
}
