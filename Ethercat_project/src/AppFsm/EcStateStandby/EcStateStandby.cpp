#include "EcStateStandby.h"

EcStateStandbyNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateStandbyNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::callback()
{
    return 0;
}

EcStateStandbyNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateStandbyNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateStandbyNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::callback()
{
    return 0;
}

EcStateStandbyNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateStandbyNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateStandbyNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::CallbackSubroutine::callback()
{
    return 0;
}

EcStateStandbyNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateStandbyNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateStandbyNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    nextStateId = EcStateData::StateId::FAULT;

    return CallbackStatus::SUCCESS;
}