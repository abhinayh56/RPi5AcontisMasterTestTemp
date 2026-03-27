#include "Ec_slave_rfidslave.h"

Ec_slave_rfidslave::Ec_slave_rfidslave(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_rfidslave::~Ec_slave_rfidslave()
{
}

EC_T_DWORD Ec_slave_rfidslave::registerTxPdos()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::registerRxPdos()
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

EC_T_DWORD Ec_slave_rfidslave::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::transferRxPdo()
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

EC_T_DWORD Ec_slave_rfidslave::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}
