#include "Ec_slave_el1008.h"

Ec_slave_el1008::Ec_slave_el1008(uint16_t slaveAddr, const std::string &slaveName) : Ec_slave_base(slaveAddr, slaveName)
{
}

Ec_slave_el1008::~Ec_slave_el1008()
{
}

EC_T_DWORD Ec_slave_el1008::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_3);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_4);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_5);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_6);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_7);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_8);

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.Channel_1, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_3, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_4, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_5, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_6, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_7, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_8, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el1008::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_el1008::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1: " << m_TxPdo.Channel_1.value << ", "
	"Channel_2: " << m_TxPdo.Channel_2.value << ", "
	"Channel_3: " << m_TxPdo.Channel_3.value << ", "
	"Channel_4: " << m_TxPdo.Channel_4.value << ", "
	"Channel_5: " << m_TxPdo.Channel_5.value << ", "
	"Channel_6: " << m_TxPdo.Channel_6.value << ", "
	"Channel_7: " << m_TxPdo.Channel_7.value << ", "
	"Channel_8: " << m_TxPdo.Channel_8.value
	<< std::endl;
}

void Ec_slave_el1008::dispRxPdo()
{
}
