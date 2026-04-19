#include "EcTaskStateBase.h"

EcTaskStateBase::EcTaskStateBase(const std::string &name, uint32_t id) : State(name, id)
{
}

EcTaskStateBase::~EcTaskStateBase()
{
}

uint32_t EcTaskStateBase::setTaskAddr(EcStateData::EcTaskAll ecTaskAll_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll = ecTaskAll_;

    return dwRes;
}

uint32_t EcTaskStateBase::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}