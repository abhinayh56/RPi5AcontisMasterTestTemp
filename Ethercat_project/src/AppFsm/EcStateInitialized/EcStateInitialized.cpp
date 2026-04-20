#include "EcStateInitialized.h"

EcStateInitialized::EcStateInitialized() : EcTaskStateBase("INITIALIZED", EcStateData::StateId::INITIALIZED)
{
}

EcStateInitialized::~EcStateInitialized()
{
}

uint32_t EcStateInitialized::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateInitializedNs::OnEntrySubroutine());
    addOnExit(new EcStateInitializedNs::OnExitSubroutine());
    addCallback(new EcStateInitializedNs::CallbackSubroutine());
    addTransition(new EcStateInitializedNs::TransitionSubroutine());

    return dwRes;
}

EcStateInitializedNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateInitializedNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateInitializedNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializedNs::OnEntrySubroutine::callback()
{
    return 0;
}

EcStateInitializedNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateInitializedNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateInitializedNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializedNs::OnExitSubroutine::callback()
{
    return 0;
}

EcStateInitializedNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateInitializedNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateInitializedNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializedNs::CallbackSubroutine::callback()
{
    return 0;
}

EcStateInitializedNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateInitializedNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateInitializedNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateInitializedNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    nextStateId = EcStateData::StateId::DISABLING;

    return CallbackStatus::SUCCESS;
}
