#include "Subroutine.h"

Subroutine::Subroutine()
{
}

Subroutine::~Subroutine()
{
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

uint32_t SubroutineTransition::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineTransition::callback(uint32_t &nextStateId)
{
    return CallbackStatus::SUCCESS;
}
