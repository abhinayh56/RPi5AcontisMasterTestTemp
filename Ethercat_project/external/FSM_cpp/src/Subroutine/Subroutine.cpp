#include "Subroutine.h"

Subroutine::Subroutine()
{
}

Subroutine::~Subroutine()
{
}

uint32_t Subroutine::setTaskAddr(EcStateData::EcTaskAll ecTaskAll_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll = ecTaskAll_;

    return dwRes;
}

uint32_t Subroutine::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t Subroutine::callback()
{
    return CallbackStatus::SUCCESS;
}

SubroutineTransition::SubroutineTransition()
{
}

SubroutineTransition::~SubroutineTransition()
{
}

uint32_t SubroutineTransition::setTaskAddr(EcStateData::EcTaskAll ecTaskAll_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll = ecTaskAll_;

    return dwRes;
}

uint32_t SubroutineTransition::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineTransition::callback(uint32_t &nextStateId)
{
    return CallbackStatus::SUCCESS;
}
