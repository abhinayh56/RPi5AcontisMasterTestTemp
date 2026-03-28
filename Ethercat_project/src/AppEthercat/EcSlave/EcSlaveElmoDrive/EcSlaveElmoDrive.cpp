#include "EcSlaveElmoDrive.h"

EcSlaveElmoDrive::EcSlaveElmoDrive(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveElmoDrive::~EcSlaveElmoDrive()
{
}

EC_T_DWORD EcSlaveElmoDrive::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Position_actual_valuie);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_word);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_Position);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_Outputs);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_word);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Position_actual_valuie, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_txPdo.Status_word, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Target_Position, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_Outputs, pBuffer);
	transferOutputPdoObject(m_rxPdo.Control_word, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveElmoDrive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Position_actual_valuie: " << m_txPdo.Position_actual_valuie.value << ", "
	"Digital_Inputs: " << m_txPdo.Digital_Inputs.value << ", "
	"Status_word: " << m_txPdo.Status_word.value
	<< std::endl;
}

void EcSlaveElmoDrive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Target_Position: " << m_rxPdo.Target_Position.value << ", "
	"Digital_Outputs: " << m_rxPdo.Digital_Outputs.value << ", "
	"Control_word: " << m_rxPdo.Control_word.value
	<< std::endl;
}
