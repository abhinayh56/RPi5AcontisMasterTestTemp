#ifndef SUBROUTINE_BASE_H
#define SUBROUTINE_BASE_H

#include <string>
#include <stdint.h>
#include <iostream>
#include "CallbackStatus.h"

struct TaskAll;

class SubroutineBase
{
public:
    SubroutineBase(uint32_t id, const std::string& name);
    virtual ~SubroutineBase();
    virtual uint32_t setTaskAddr(TaskAll* taskAll);
    virtual uint32_t config();
    virtual uint32_t callback();

protected:
    uint32_t m_id;
    std::string m_name;
    TaskAll* p_taskAll;
};

class SubroutineTransitionBase
{
public:
    SubroutineTransitionBase(uint32_t id, const std::string& name);
    virtual ~SubroutineTransitionBase();
    virtual uint32_t setTaskAddr(TaskAll* taskAll);
    virtual uint32_t config();
    virtual uint32_t callback(uint32_t &nextStateId);

protected:
    uint32_t m_id;
    std::string m_name;
    TaskAll* p_taskAll;
};

#endif // SUBROUTINE_BASE_H
