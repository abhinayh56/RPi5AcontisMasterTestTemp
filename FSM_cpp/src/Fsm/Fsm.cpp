#include "Fsm.h"

Fsm::Fsm(uint32_t initialStateId) :
    m_initialStateId(initialStateId),
    m_currentStateId(initialStateId),
    m_nextStateId(initialStateId),
    m_currentStateIndex(0),
    m_nextStateIndex(0),
    m_numState(0)
{
    // m_stateVector.resize(3);
}

Fsm::~Fsm()
{
}

uint32_t Fsm::addState(State* p_state)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_stateVector.push_back(p_state);
    m_numState = m_stateVector.size();
    
    std::cout << "---\n";
    std::cout << "added state | id: " << m_stateVector[m_numState-1]->m_id << ", name: " << m_stateVector[m_numState-1]->m_name << std::endl;
    std::cout << "num states: " << m_numState << std::endl;

    return dwRes;
}

uint32_t Fsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "---\nconfiguring states" << std::endl;

    for(uint32_t i=0; i<m_numState; i++)
    {
        std::cout << "---\n";
        std::cout << "configuring state | id: " << m_stateVector[i]->m_id << ", name: " << m_stateVector[i]->m_name << std::endl;
        dwRes |= m_stateVector[i]->config();
    }

    return dwRes;
}

uint32_t Fsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "\n  N: " << update_number << "\n  state name: " << m_stateVector[m_currentStateIndex]->m_name << std::endl;
    update_number++;

    dwRes |= m_stateVector[m_currentStateIndex]->update(m_nextStateId);

    for(uint32_t i=0; i<m_numState; i++)
    {
        if((m_stateVector[i]->m_id) == m_nextStateId)
        {
            m_currentStateIndex = i;
            m_currentStateId = m_nextStateId;
            // dwRes |= m_stateVector[i]->update(m_currentStateId);
        }
    }

    return dwRes;
}
