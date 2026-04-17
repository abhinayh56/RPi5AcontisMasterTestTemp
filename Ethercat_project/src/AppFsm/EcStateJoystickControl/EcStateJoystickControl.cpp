#include "EcStateJoystickControl.h"

EcStateJoystickControlNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateJoystickControlNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::callback()
{
    return 0;
}

EcStateJoystickControlNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateJoystickControlNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::callback()
{
    return 0;
}

EcStateJoystickControlNs::CallbackSubroutine::CallbackSubroutine()
{
}

EcStateJoystickControlNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::callback()
{
    return 0;
}

EcStateJoystickControlNs::TransitionSubroutine::TransitionSubroutine()
{
}

EcStateJoystickControlNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateJoystickControlNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    nextStateId = EcStateData::StateId::DISABLING;

    return CallbackStatus::SUCCESS;
}
