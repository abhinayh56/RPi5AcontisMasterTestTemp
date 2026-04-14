#ifndef EC_TASK_FSM_H
#define EC_TASK_FSM_H

#include "Fsm.h"

class EcTaskFsm
{
public:
    EcTaskFsm();

    ~EcTaskFsm();

    uint32_t config();

    uint32_t update();

private:
    Fsm fsm;
};

#endif // EC_TASK_FSM_H
