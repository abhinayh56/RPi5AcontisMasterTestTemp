#include "EcSlaveBase.h"

EcSlaveBase::EcSlaveBase(uint16_t slaveAddr, const std::string &slaveName) : m_slaveAddr(slaveAddr), m_slaveName(slaveName)
{
}

EcSlaveBase::~EcSlaveBase()
{
}

EC_T_DWORD EcSlaveBase::checkSlave()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecatGetCfgSlaveInfo(true, m_slaveAddr, &m_slaveInfo);
	if (dwRes != EC_E_NOERROR)
	{
		std::cout << "EtherCAT Slave at address " << m_slaveAddr << " Not FOUND " << std::endl;
		return 1<<10;
	}

	std::cout << "EtherCAT Slave  " << m_slaveInfo.abyDeviceName << " at address " << m_slaveAddr << "  FOUND " << std::endl;

	return 0;
}

EC_T_DWORD EcSlaveBase::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveBase::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveBase::dispTxPdo()
{
}

void EcSlaveBase::dispRxPdo()
{
}


uint16_t EcSlaveBase::getAddress()
{
	return m_slaveAddr;
}

std::string& EcSlaveBase::getName()
{
	return m_slaveName;
}
