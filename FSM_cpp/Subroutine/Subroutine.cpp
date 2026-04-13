#include "Subroutine.h"

Subroutine::Subroutine() : m_configuredSubroutine(false)
{
}

Subroutine::~Subroutine()
{
}

uint32_t Subroutine::config()
{
    return CallbackStatus::SUCCESS;
}

uint32_t Subroutine::update()
{
    return CallbackStatus::SUCCESS;
}
