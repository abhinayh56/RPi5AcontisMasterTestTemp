#include "Ec_slave_encbrkctrl.h"

Ec_slave_encbrkctrl::Ec_slave_encbrkctrl(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_encbrkctrl::~Ec_slave_encbrkctrl()
{
}

EC_T_DWORD Ec_slave_encbrkctrl::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ENC1);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ENC2);

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.BRK1);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.BRK2);

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.ENC1, pBuffer);
	transferInputPdoObject(m_TxPdo.ENC2, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.BRK1, pBuffer);
	transferOutputPdoObject(m_RxPdo.BRK2, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_encbrkctrl::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_encbrkctrl::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Device_ID: " << m_TxPdo.ENC1.value << ", "
	"MFG_Year: " << m_TxPdo.ENC2.value
	<< std::endl;
}

void Ec_slave_encbrkctrl::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Acknowledge: " << m_RxPdo.BRK1.value << ", "
	"Grip_Offset: " << m_RxPdo.BRK2.value
	<< std::endl;
}