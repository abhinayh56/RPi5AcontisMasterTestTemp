#include "EcTaskUserBase.h"

EcTaskUserBase::EcTaskUserBase(uint16_t ecTaskUserId, const std::string &ecTaskUserName) : m_ecTaskUserId(ecTaskUserId), m_ecTaskUserName(ecTaskUserName)
{
}

EcTaskUserBase::~EcTaskUserBase()
{
}

uint16_t EcTaskUserBase::getTaskId()
{
    return m_ecTaskUserId;
}

std::string &EcTaskUserBase::getTaskName()
{
    return m_ecTaskUserName;
}
