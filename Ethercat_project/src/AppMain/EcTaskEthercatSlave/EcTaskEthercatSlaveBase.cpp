#include "EcTaskEthercatSlaveBase.h"

EcTaskEthercatSlaveBase::EcTaskEthercatSlaveBase(uint16_t ecTaskEthercatSlaveId, const std::string &ecTaskEthercatSlaveName) : m_ecTaskEthercatSlaveId(ecTaskEthercatSlaveId), m_ecTaskEthercatSlaveName(ecTaskEthercatSlaveName)
{
}

EcTaskEthercatSlaveBase::~EcTaskEthercatSlaveBase()
{
}

uint16_t EcTaskEthercatSlaveBase::getTaskId()
{
    return m_ecTaskEthercatSlaveId;
}

std::string &EcTaskEthercatSlaveBase::getTaskName()
{
    return m_ecTaskEthercatSlaveName;
}
