#include "EcSlaveEl2008.h"

EcSlaveEl2008::EcSlaveEl2008(uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(slaveAddr, slaveName),
	m_OutputCh_1("DIGITAL_OUTPUT_CH_1", m_path, false, true),
	m_OutputCh_2("DIGITAL_OUTPUT_CH_2", m_path, false, true),
	m_OutputCh_3("DIGITAL_OUTPUT_CH_3", m_path, false, true),
	m_OutputCh_4("DIGITAL_OUTPUT_CH_4", m_path, false, true),
	m_OutputCh_5("DIGITAL_OUTPUT_CH_5", m_path, false, true),
	m_OutputCh_6("DIGITAL_OUTPUT_CH_6", m_path, false, true),
	m_OutputCh_7("DIGITAL_OUTPUT_CH_7", m_path, false, true),
	m_OutputCh_8("DIGITAL_OUTPUT_CH_8", m_path, false, true)
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

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_3);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_4);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_5);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_6);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_7);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_8);

	dwRes |= lookupAllPdoObjectByAddress(m_slaveAddr, m_slaveInfo);

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

	// transferOutputPdoObject(m_rxPdo.Channel_1, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_2, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_3, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_4, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_5, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_6, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_7, pBuffer);
	// transferOutputPdoObject(m_rxPdo.Channel_8, pBuffer);

	transferAllOutputPdoObject(m_rxPdoValue, pBuffer, m_slaveInfo);

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

EC_T_DWORD EcSlaveEl2008::registerPublisher()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::registerSubscriber()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	m_OutputCh_1.subscribe();
	m_OutputCh_2.subscribe();
	m_OutputCh_3.subscribe();
	m_OutputCh_4.subscribe();
	m_OutputCh_5.subscribe();
	m_OutputCh_6.subscribe();
	m_OutputCh_7.subscribe();
	m_OutputCh_8.subscribe();

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

	m_OutputCh_1.get(m_rxPdo.Channel_1.value);
	m_OutputCh_2.get(m_rxPdo.Channel_2.value);
	m_OutputCh_3.get(m_rxPdo.Channel_3.value);
	m_OutputCh_4.get(m_rxPdo.Channel_4.value);
	m_OutputCh_5.get(m_rxPdo.Channel_5.value);
	m_OutputCh_6.get(m_rxPdo.Channel_6.value);
	m_OutputCh_7.get(m_rxPdo.Channel_7.value);
	m_OutputCh_8.get(m_rxPdo.Channel_8.value);

//	std::cout << "m_InputCh_1_8_subscribed: "
//	<< m_rxPdo.Channel_1.value << ", "
//	<< m_rxPdo.Channel_2.value << ", "
//	<< m_rxPdo.Channel_3.value << ", "
//	<< m_rxPdo.Channel_4.value << ", "
//	<< m_rxPdo.Channel_5.value << ", "
//	<< m_rxPdo.Channel_6.value << ", "
//	<< m_rxPdo.Channel_7.value << ", "
//	<< m_rxPdo.Channel_8.value << std::endl;

	return dwRes;
}

EC_T_DWORD EcSlaveEl2008::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;
	
	uint8_t commandCh_1 = 0;
	uint8_t commandCh_2 = 0;
	uint8_t commandCh_3 = 0;
	uint8_t commandCh_4 = 0;
	uint8_t commandCh_5 = 0;
	uint8_t commandCh_6 = 0;
	uint8_t commandCh_7 = 0;
	uint8_t commandCh_8 = 0;

	if(m_rxPdo.Channel_1.value) {commandCh_1 = 1;}
	if(m_rxPdo.Channel_2.value) {commandCh_2 = 1;}
	if(m_rxPdo.Channel_3.value) {commandCh_3 = 1;}
	if(m_rxPdo.Channel_4.value) {commandCh_4 = 1;}
	if(m_rxPdo.Channel_5.value) {commandCh_5 = 1;}
	if(m_rxPdo.Channel_6.value) {commandCh_6 = 1;}
	if(m_rxPdo.Channel_7.value) {commandCh_7 = 1;}
	if(m_rxPdo.Channel_8.value) {commandCh_8 = 1;}

	m_rxPdoValue = (commandCh_1 << 0) | 
					(commandCh_2 << 1) |
					(commandCh_3 << 2) |
					(commandCh_4 << 3) |
					(commandCh_5 << 4) |
					(commandCh_6 << 5) |
					(commandCh_7 << 6) |
					(commandCh_8 << 7);

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
