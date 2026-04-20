#include "StateBase.h"

StateBase::StateBase(const std::string &name, uint32_t id) : 
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

StateBase::~StateBase()
{
}

uint32_t StateBase::addOnEntry(SubroutineBase* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onEntryVector.push_back(p_subroutine);
    m_numOnEntry = m_onEntryVector.size();

    return dwRes;
}

uint32_t StateBase::addTransition(SubroutineTransitionBase* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_transitionVector.push_back(p_subroutine);
    m_numTransition = m_transitionVector.size();

    return dwRes;
}

uint32_t StateBase::addCallback(SubroutineBase* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_callbackVector.push_back(p_subroutine);
    m_numCallback = m_callbackVector.size();

    return dwRes;
}

uint32_t StateBase::addOnExit(SubroutineBase* p_subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onExitVector.push_back(p_subroutine);
    m_numOnExit = m_onExitVector.size();

    return dwRes;
}

uint32_t StateBase::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnEntry; i++)
    {
        dwRes |= m_onEntryVector[i]->setTaskAddr(p_taskAll);
    }
    for(uint32_t i=0; i<m_numTransition; i++)
    {
        dwRes |= m_transitionVector[i]->setTaskAddr(p_taskAll);
    }
    for(uint32_t i=0; i<m_numCallback; i++)
    {
        dwRes |= m_callbackVector[i]->setTaskAddr(p_taskAll);
    }
    for(uint32_t i=0; i<m_numOnExit; i++)
    {
        dwRes |= m_onExitVector[i]->setTaskAddr(p_taskAll);
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

uint32_t StateBase::update(uint32_t &nextStateId)
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

uint32_t StateBase::onEntry()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnEntry; i++)
    {
        dwRes |= m_onEntryVector[i]->callback();
    }

    return dwRes;
}

uint32_t StateBase::transition(uint32_t &nextStateId)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numTransition; i++)
    {
        dwRes |= m_transitionVector[i]->callback(nextStateId);
    }

    return dwRes;
}

uint32_t StateBase::callback()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numCallback; i++)
    {
        dwRes |= m_callbackVector[i]->callback();
    }

    return dwRes;
}

uint32_t StateBase::onExit()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_numOnExit; i++)
    {
        dwRes |= m_onExitVector[i]->callback();
    }

    return dwRes;
}
