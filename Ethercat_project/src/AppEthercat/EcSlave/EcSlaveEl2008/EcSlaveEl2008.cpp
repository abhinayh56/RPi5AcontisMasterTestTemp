#include "EcSlaveEl2008.h"

EcSlaveEl2008::EcSlaveEl2008(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveEl2008::~EcSlaveEl2008()
{
}

EC_T_DWORD EcSlaveEl2008::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_3);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_4);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_5);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_6);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_7);
//	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_8);

	dwRes |= lookupSlaveInfoByAddress(m_slaveAddr, m_slaveInfo);

	return EC_E_NOERROR;
}

EC_T_DWORD EcSlaveEl2008::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

//	transferOutputPdoObject(m_rxPdo.Channel_1, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_2, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_3, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_4, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_5, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_6, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_7, pBuffer);
//	transferOutputPdoObject(m_rxPdo.Channel_8, pBuffer);

	EC_SETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&m_rxPdo), m_slaveInfo.dwPdOffsOut, m_slaveInfo.dwPdSizeOut);

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	time_now_ms += 4;

	uint64_t time_now_s = uint64_t(double(time_now_ms) / 1000.0);

	uint8_t count_now = time_now_s % 9;

	uint8_t valTemp = 0;

	for(int i = 0; i < count_now; i++)
	{
		valTemp |= (1 << i);
	}

//	std::cout << "valTemp: " << (uint16_t)valTemp << std::endl;

	m_rxPdo = valTemp;

	return dwRes;
}

void EcSlaveEl2008::dispTxPdo()
{
}

void EcSlaveEl2008::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName <<
	"Channel_1: " << m_rxPdo.Channel_1.value << ", "
	"Channel_2: " << m_rxPdo.Channel_2.value << ", "
	"Channel_3: " << m_rxPdo.Channel_3.value << ", "
	"Channel_4: " << m_rxPdo.Channel_4.value << ", "
	"Channel_5: " << m_rxPdo.Channel_5.value << ", "
	"Channel_6: " << m_rxPdo.Channel_6.value << ", "
	"Channel_7: " << m_rxPdo.Channel_7.value << ", "
	"Channel_8: " << m_rxPdo.Channel_8.value
	<< std::endl;
}
