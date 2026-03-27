#include "Ec_slave_el2008.h"

Ec_slave_el2008::Ec_slave_el2008(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_el2008::~Ec_slave_el2008()
{
}

EC_T_DWORD Ec_slave_el2008::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_3);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_4);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_5);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_6);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_7);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_8);

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.Channel_1, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_3, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_4, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_5, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_6, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_7, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_8, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el2008::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_el2008::dispTxPdo()
{
}

void Ec_slave_el2008::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Channel_1: " << m_RxPdo.Channel_1.value << ", "
	"Channel_2: " << m_RxPdo.Channel_2.value << ", "
	"Channel_3: " << m_RxPdo.Channel_3.value << ", "
	"Channel_4: " << m_RxPdo.Channel_4.value << ", "
	"Channel_5: " << m_RxPdo.Channel_5.value << ", "
	"Channel_6: " << m_RxPdo.Channel_6.value << ", "
	"Channel_7: " << m_RxPdo.Channel_7.value << ", "
	"Channel_8: " << m_RxPdo.Channel_8.value
	<< std::endl;
}
