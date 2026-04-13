#ifndef FSM_H
#define FSM_H

#include "State.h"
#include <vector>

class Fsm
{
public:
    Fsm();
    ~Fsm();

private:
    int m_initialState;
    int m_currentState;
    std::vector<State*> m_stateVector;
};

#endif // FSM_H
