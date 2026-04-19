#ifndef EC_TASK_STATE_BASE_H
#define EC_TASK_STATE_BASE_H

#include "State.h"
#include "EcState.h"

class EcTaskStateBase : public State
{
public:
    EcTaskStateBase(const std::string &name, uint32_t id);
    virtual ~EcTaskStateBase();
    virtual uint32_t setTaskAddr(EcStateData::EcTaskAll ecTaskAll_);
    virtual uint32_t addSubroutine();
};

#endif // EC_TASK_STATE_BASE_H
