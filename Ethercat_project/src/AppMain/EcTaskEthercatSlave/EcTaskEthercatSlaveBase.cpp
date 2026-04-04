#include "EcTaskEthercatSlaveBase.h"

EcTaskEthercatSlaveBase::EcTaskEthercatSlaveBase(uint16_t ecTaskEthercatId, const std::string &ecTaskEthercatName) : m_ecTaskEthercatId(ecTaskEthercatId), m_ecTaskEthercatName(ecTaskEthercatName)
{
}

EcTaskEthercatSlaveBase::~EcTaskEthercatSlaveBase()
{
}

uint16_t EcTaskEthercatSlaveBase::getTaskId()
{
    return m_ecTaskEthercatId;
}

std::string &EcTaskEthercatSlaveBase::getTaskName()
{
    return m_ecTaskEthercatName;
}
