#include "EcSlaveEncbrkctrl.h"

EcSlaveEncbrkctrl::EcSlaveEncbrkctrl(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveEncbrkctrl::~EcSlaveEncbrkctrl()
{
}

EC_T_DWORD EcSlaveEncbrkctrl::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ENC1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ENC2);

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.BRK1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.BRK2);

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.ENC1, pBuffer);
	transferInputPdoObject(m_txPdo.ENC2, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.BRK1, pBuffer);
	transferOutputPdoObject(m_rxPdo.BRK2, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEncbrkctrl::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveEncbrkctrl::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"ENC1: " << m_txPdo.ENC1.value << ", "
	"ENC2: " << m_txPdo.ENC2.value
	<< std::endl;
}

void EcSlaveEncbrkctrl::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"BRK1: " << m_rxPdo.BRK1.value << ", "
	"BRK2: " << m_rxPdo.BRK2.value
	<< std::endl;
}
