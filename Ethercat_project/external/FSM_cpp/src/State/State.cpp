#include "State.h"

State::State(const std::string &name, uint32_t id) : 
	m_name(name),
	m_id(id),
	m_numOnEntry(0),
	m_numTransition(0),
	m_numCallback(0),
	m_numOnExit(0),
	m_idNext(id),
	m_onEntryFlag(false)
{
}

State::~State()
{
}

uint32_t State::addOnEntry(Subroutine* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onEntryVector.push_back(p_subroutine);
    m_numOnEntry = m_onEntryVector.size();

    return dwRes;
}

uint32_t State::addTransition(SubroutineTransition* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_transitionVector.push_back(p_subroutine);
    m_numTransition = m_transitionVector.size();

    return dwRes;
}

uint32_t State::addCallback(Subroutine* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_callbackVector.push_back(p_subroutine);
    m_numCallback = m_callbackVector.size();

    return dwRes;
}

uint32_t State::addOnExit(Subroutine* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onExitVector.push_back(p_subroutine);
    m_numOnExit = m_onExitVector.size();

    return dwRes;
}

uint32_t State::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnEntry; i++)
    {
        dwRes |= m_onEntryVector[i]->setTaskAddr(ecTaskAll);
    }
    for(uint32_t i=0; i<m_numTransition; i++)
    {
        dwRes |= m_transitionVector[i]->setTaskAddr(ecTaskAll);
    }
    for(uint32_t i=0; i<m_numCallback; i++)
    {
        dwRes |= m_callbackVector[i]->setTaskAddr(ecTaskAll);
    }
    for(uint32_t i=0; i<m_numOnExit; i++)
    {
        dwRes |= m_onExitVector[i]->setTaskAddr(ecTaskAll);
    }

    for(uint32_t i=0; i<m_numOnEntry; i++)
    {
        dwRes |= m_onEntryVector[i]->config();
    }
    for(uint32_t i=0; i<m_numTransition; i++)
    {
        dwRes |= m_transitionVector[i]->config();
    }
    for(uint32_t i=0; i<m_numCallback; i++)
    {
        dwRes |= m_callbackVector[i]->config();
    }
    for(uint32_t i=0; i<m_numOnExit; i++)
    {
        dwRes |= m_onExitVector[i]->config();
    }

    return dwRes;
}

uint32_t State::update(uint32_t &nextStateId)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if(m_onEntryFlag == false)
    {
        dwRes |= onEntry();
        m_onEntryFlag = true;
    }

    dwRes |= callback();

    dwRes |= transition(nextStateId);
    m_idNext = nextStateId;

    if(m_idNext != m_id)
    {
        dwRes |= onExit();
        m_idNext = 0;
        m_onEntryFlag = false;
    }

    return dwRes;
}

uint32_t State::onEntry()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnEntry; i++)
    {
        dwRes |= m_onEntryVector[i]->callback();
    }

    return dwRes;
}

uint32_t State::transition(uint32_t &nextStateId)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numTransition; i++)
    {
        dwRes |= m_transitionVector[i]->callback(nextStateId);
    }

    return dwRes;
}

uint32_t State::callback()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numCallback; i++)
    {
        dwRes |= m_callbackVector[i]->callback();
    }

    return dwRes;
}

uint32_t State::onExit()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnExit; i++)
    {
        dwRes |= m_onExitVector[i]->callback();
    }

    return dwRes;
}
