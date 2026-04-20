#ifndef FSM_H
#define FSM_H

#include "../State/State.h"
#include <vector>
#include <iostream>
#include "EcTaskStateBase.h"

class Fsm
{
public:
    Fsm(uint32_t initialStateId);
    
    ~Fsm();

    uint32_t addState(EcTaskStateBase* p_state);

    uint32_t config();

    uint32_t update();

private:
    uint32_t m_initialStateId;
    uint32_t m_currentStateId;
    uint32_t m_nextStateId;
    uint32_t m_currentStateIndex;
    uint32_t m_nextStateIndex;

    std::vector<EcTaskStateBase*> m_stateVector;
    uint32_t m_numState;

    uint32_t update_number = 1;
};

#endif // FSM_H
