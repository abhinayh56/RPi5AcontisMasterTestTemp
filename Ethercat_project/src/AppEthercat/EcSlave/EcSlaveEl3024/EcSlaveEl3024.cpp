#include "EcSlaveEl3024.h"

EcSlaveEl3024::EcSlaveEl3024(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveEl3024::~EcSlaveEl3024()
{
}

EC_T_DWORD EcSlaveEl3024::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Al_Compact_Channel_1_Value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Al_Compact_Channel_2_Value);

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Al_Compact_Channel_1_Value, pBuffer);
	transferInputPdoObject(m_txPdo.Al_Compact_Channel_2_Value, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl3024::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveEl3024::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_txPdo.Al_Compact_Channel_1_Value.value << ", "
	"Grip_Offset: " << m_txPdo.Al_Compact_Channel_2_Value.value
	<< std::endl;
}

void EcSlaveEl3024::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | "
	<< std::endl;
}
