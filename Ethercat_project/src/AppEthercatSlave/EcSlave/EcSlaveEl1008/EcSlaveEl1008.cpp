#include "EcSlaveEl1008.h"

EcSlaveEl1008::EcSlaveEl1008(uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(slaveAddr, slaveName),
	m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/motor_3", false, true),
	m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/motor_3", false, true),
	m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/motor_3", false, true),
	m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/motor_3", false, true),
	m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/motor_3", false, true),
	m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/motor_3", false, true),
	m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/motor_3", false, true),
	m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/motor_3", false, true)
{
}

EcSlaveEl1008::~EcSlaveEl1008()
{
}

EC_T_DWORD EcSlaveEl1008::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_8);

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Channel_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_8, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::registerPublisher()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	m_InputCh_1.publish();
	m_InputCh_2.publish();
	m_InputCh_3.publish();
	m_InputCh_4.publish();
	m_InputCh_5.publish();
	m_InputCh_6.publish();
	m_InputCh_7.publish();
	m_InputCh_8.publish();

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::registerSubscriber()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	m_InputCh_1.set(m_txPdo.Channel_1.value);
	m_InputCh_2.set(m_txPdo.Channel_2.value);
	m_InputCh_3.set(m_txPdo.Channel_3.value);
	m_InputCh_4.set(m_txPdo.Channel_4.value);
	m_InputCh_5.set(m_txPdo.Channel_5.value);
	m_InputCh_6.set(m_txPdo.Channel_6.value);
	m_InputCh_7.set(m_txPdo.Channel_7.value);
	m_InputCh_8.set(m_txPdo.Channel_8.value);

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl1008::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveEl1008::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1: " << m_txPdo.Channel_1.value << ", "
	"Channel_2: " << m_txPdo.Channel_2.value << ", "
	"Channel_3: " << m_txPdo.Channel_3.value << ", "
	"Channel_4: " << m_txPdo.Channel_4.value << ", "
	"Channel_5: " << m_txPdo.Channel_5.value << ", "
	"Channel_6: " << m_txPdo.Channel_6.value << ", "
	"Channel_7: " << m_txPdo.Channel_7.value << ", "
	"Channel_8: " << m_txPdo.Channel_8.value
	<< std::endl;
}

void EcSlaveEl1008::dispRxPdo()
{
}
