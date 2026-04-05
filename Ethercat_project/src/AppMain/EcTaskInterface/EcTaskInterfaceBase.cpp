#include "EcTaskInterfaceBase.h"

EcTaskInterfaceBase::EcTaskInterfaceBase(uint16_t ecTaskInterfaceId, const std::string &ecTaskInterfaceName) : m_ecTaskInterfaceId(ecTaskInterfaceId), m_ecTaskInterfaceName(ecTaskInterfaceName)
{
}

EcTaskInterfaceBase::~EcTaskInterfaceBase()
{
}

uint16_t EcTaskInterfaceBase::getTaskId()
{
    return m_ecTaskInterfaceId;
}

std::string &EcTaskInterfaceBase::getTaskName()
{
    return m_ecTaskInterfaceName;
}

