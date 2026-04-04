#include "EcTaskUserBase.h"

EcTaskUserBase::EcTaskUserBase(uint16_t ecTaskUserBaseId, const std::string &ecTaskUserBaseName) : m_ecTaskUserBaseId(ecTaskUserBaseId), m_ecTaskUserBaseName(ecTaskUserBaseName)
{
}

EcTaskUserBase::~EcTaskUserBase()
{
}

uint16_t EcTaskUserBase::getTaskId()
{
    return m_ecTaskUserBaseId;
}

std::string &EcTaskUserBase::getTaskName()
{
    return m_ecTaskUserBaseName;
}
