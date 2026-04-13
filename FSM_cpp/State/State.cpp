#include "State.h"

State::State() :
    m_configuredStateOnEntry(false),
    m_configuredStateOnExit(false),
    m_configuredStateUpdate(false)
{
}

State::~State()
{
}

uint32_t State::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if (m_configuredStateOnEntry == false)
    {
        for (uint16_t i = 0; i < m_numSubroutineOnEntry; i++)
        {
            dwRes |= m_subroutineOnEntryVector[i]->config();
        }
        if (dwRes == CallbackStatus::SUCCESS)
        {
            m_configuredStateOnEntry = true;
        }
    }

    if (m_configuredStateOnExit == false)
    {
        for (uint16_t i = 0; i < m_numSubroutineonExit; i++)
        {
            dwRes |= m_subroutineOnExitVector[i]->config();
        }
        if (dwRes == CallbackStatus::SUCCESS)
        {
            m_configuredStateOnExit = true;
        }
    }

    if (m_configuredStateUpdate == false)
    {
        for (uint16_t i = 0; i < m_numSubroutineUpdate; i++)
        {
            dwRes |= m_subroutineUpdateVector[i]->config();
        }
        if (dwRes == CallbackStatus::SUCCESS)
        {
            m_configuredStateUpdate = true;
        }
    }

    return dwRes;
}

uint32_t State::onEntry()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (uint16_t i = 0; i < m_numSubroutineOnEntry; i++)
    {
        dwRes |= m_subroutineOnEntryVector[i]->update();
    }

    return dwRes;
}

uint32_t State::onExit()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (uint16_t i = 0; i < m_numSubroutineonExit; i++)
    {
        dwRes |= m_subroutineOnExitVector[i]->update();
    }

    return dwRes;
}

uint32_t State::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (uint16_t i = 0; i < m_numSubroutineUpdate; i++)
    {
        dwRes |= m_subroutineUpdateVector[i]->update();
    }

    return dwRes;
}

uint32_t State::addOnEntry(Subroutine *p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    m_subroutineOnEntryVector.push_back(p_subroutine);
    m_numSubroutineOnEntry = m_subroutineOnEntryVector.size();
    return dwRes;
}

uint32_t State::addOnExit(Subroutine *p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    m_subroutineOnExitVector.push_back(p_subroutine);
    m_numSubroutineonExit = m_subroutineOnExitVector.size();
    return dwRes;
}

uint32_t State::addOnUpdate(Subroutine *p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    m_subroutineUpdateVector.push_back(p_subroutine);
    m_numSubroutineUpdate = m_subroutineUpdateVector.size();
    return dwRes;
}
