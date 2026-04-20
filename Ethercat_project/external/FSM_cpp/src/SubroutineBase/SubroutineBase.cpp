#include "SubroutineBase.h"

SubroutineBase::SubroutineBase(uint32_t id, const std::string& name) : m_id(id), m_name(name)
{
    std::cout << "SubroutineBase constructor" << std::endl;
}

SubroutineBase::~SubroutineBase()
{
    std::cout << "SubroutineBase destructor" << std::endl;
}

uint32_t SubroutineBase::setTaskAddr(TaskAll* taskAll)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    p_taskAll = taskAll;
    std::cout << "SubroutineBase set task address" << std::endl;

    return dwRes;
}

uint32_t SubroutineBase::config()
{
    std::cout << "SubroutineBase config" << std::endl;
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineBase::callback()
{
    std::cout << "SubroutineBase callback" << std::endl;
    return CallbackStatus::SUCCESS;
}

SubroutineTransitionBase::SubroutineTransitionBase(uint32_t id, const std::string& name) : m_id(id), m_name(name)
{
    std::cout << "SubroutineTransitionBase constructor" << std::endl;
}

SubroutineTransitionBase::~SubroutineTransitionBase()
{
    std::cout << "SubroutineTransitionBase destructor" << std::endl;
}

uint32_t SubroutineTransitionBase::setTaskAddr(TaskAll* taskAll)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    p_taskAll = taskAll;
    std::cout << "SubroutineTransitionBase set task address" << std::endl;

    return dwRes;
}

uint32_t SubroutineTransitionBase::config()
{
    std::cout << "SubroutineTransitionBase config" << std::endl;
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineTransitionBase::callback(uint32_t &nextStateId)
{
    std::cout << "SubroutineTransitionBase callback" << std::endl;
    return CallbackStatus::SUCCESS;
}
