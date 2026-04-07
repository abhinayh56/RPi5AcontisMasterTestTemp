#include "EcSlaveEl1008.h"

EcSlaveEl1008::EcSlaveEl1008(uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(slaveAddr, slaveName),
	m_InputCh_11("DIGITAL_INPUT_CH_1", "/ethercat/motor_1", false, true),
	m_InputCh_21("DIGITAL_INPUT_CH_2", "/ethercat/motor_1", false, true),
	m_InputCh_31("DIGITAL_INPUT_CH_3", "/ethercat/motor_1", false, true),
	m_InputCh_41("DIGITAL_INPUT_CH_4", "/ethercat/motor_1", false, true),
	m_InputCh_51("DIGITAL_INPUT_CH_5", "/ethercat/motor_1", false, true),
	m_InputCh_61("DIGITAL_INPUT_CH_6", "/ethercat/motor_1", false, true),
	m_InputCh_71("DIGITAL_INPUT_CH_7", "/ethercat/motor_1", false, true),
	m_InputCh_81("DIGITAL_INPUT_CH_8", "/ethercat/motor_1", false, true),
	m_InputCh_12("DIGITAL_INPUT_CH_1", "/ethercat/motor_2", false, true),
	m_InputCh_22("DIGITAL_INPUT_CH_2", "/ethercat/motor_2", false, true),
	m_InputCh_32("DIGITAL_INPUT_CH_3", "/ethercat/motor_2", false, true),
	m_InputCh_42("DIGITAL_INPUT_CH_4", "/ethercat/motor_2", false, true),
	m_InputCh_52("DIGITAL_INPUT_CH_5", "/ethercat/motor_2", false, true),
	m_InputCh_62("DIGITAL_INPUT_CH_6", "/ethercat/motor_2", false, true),
	m_InputCh_72("DIGITAL_INPUT_CH_7", "/ethercat/motor_2", false, true),
	m_InputCh_82("DIGITAL_INPUT_CH_8", "/ethercat/motor_2", false, true),
	m_InputCh_13("DIGITAL_INPUT_CH_1", "/ethercat/motor_3", false, true),
	m_InputCh_23("DIGITAL_INPUT_CH_2", "/ethercat/motor_3", false, true),
	m_InputCh_33("DIGITAL_INPUT_CH_3", "/ethercat/motor_3", false, true),
	m_InputCh_43("DIGITAL_INPUT_CH_4", "/ethercat/motor_3", false, true),
	m_InputCh_53("DIGITAL_INPUT_CH_5", "/ethercat/motor_3", false, true),
	m_InputCh_63("DIGITAL_INPUT_CH_6", "/ethercat/motor_3", false, true),
	m_InputCh_73("DIGITAL_INPUT_CH_7", "/ethercat/motor_3", false, true),
	m_InputCh_83("DIGITAL_INPUT_CH_8", "/ethercat/motor_3", false, true),
	m_InputCh_14("DIGITAL_INPUT_CH_1", "/ethercat/motor_4", false, true),
	m_InputCh_24("DIGITAL_INPUT_CH_2", "/ethercat/motor_4", false, true),
	m_InputCh_34("DIGITAL_INPUT_CH_3", "/ethercat/motor_4", false, true),
	m_InputCh_44("DIGITAL_INPUT_CH_4", "/ethercat/motor_4", false, true),
	m_InputCh_54("DIGITAL_INPUT_CH_5", "/ethercat/motor_4", false, true),
	m_InputCh_64("DIGITAL_INPUT_CH_6", "/ethercat/motor_4", false, true),
	m_InputCh_74("DIGITAL_INPUT_CH_7", "/ethercat/motor_4", false, true),
	m_InputCh_84("DIGITAL_INPUT_CH_8", "/ethercat/motor_4", false, true)
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

	m_InputCh_11.publish();
	m_InputCh_21.publish();
	m_InputCh_31.publish();
	m_InputCh_41.publish();
	m_InputCh_51.publish();
	m_InputCh_61.publish();
	m_InputCh_71.publish();
	m_InputCh_81.publish();
	m_InputCh_12.publish();
	m_InputCh_22.publish();
	m_InputCh_32.publish();
	m_InputCh_42.publish();
	m_InputCh_52.publish();
	m_InputCh_62.publish();
	m_InputCh_72.publish();
	m_InputCh_82.publish();
	m_InputCh_13.publish();
	m_InputCh_23.publish();
	m_InputCh_33.publish();
	m_InputCh_43.publish();
	m_InputCh_53.publish();
	m_InputCh_63.publish();
	m_InputCh_73.publish();
	m_InputCh_83.publish();
	m_InputCh_14.publish();
	m_InputCh_24.publish();
	m_InputCh_34.publish();
	m_InputCh_44.publish();
	m_InputCh_54.publish();
	m_InputCh_64.publish();
	m_InputCh_74.publish();
	m_InputCh_84.publish();

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

	m_InputCh_11.set(m_txPdo.Channel_1.value);
	m_InputCh_21.set(m_txPdo.Channel_2.value);
	m_InputCh_31.set(m_txPdo.Channel_3.value);
	m_InputCh_41.set(m_txPdo.Channel_4.value);
	m_InputCh_51.set(m_txPdo.Channel_5.value);
	m_InputCh_61.set(m_txPdo.Channel_6.value);
	m_InputCh_71.set(m_txPdo.Channel_7.value);
	m_InputCh_81.set(m_txPdo.Channel_8.value);
	m_InputCh_12.set(m_txPdo.Channel_1.value);
	m_InputCh_22.set(m_txPdo.Channel_2.value);
	m_InputCh_32.set(m_txPdo.Channel_3.value);
	m_InputCh_42.set(m_txPdo.Channel_4.value);
	m_InputCh_52.set(m_txPdo.Channel_5.value);
	m_InputCh_62.set(m_txPdo.Channel_6.value);
	m_InputCh_72.set(m_txPdo.Channel_7.value);
	m_InputCh_82.set(m_txPdo.Channel_8.value);
	m_InputCh_13.set(m_txPdo.Channel_1.value);
	m_InputCh_23.set(m_txPdo.Channel_2.value);
	m_InputCh_33.set(m_txPdo.Channel_3.value);
	m_InputCh_43.set(m_txPdo.Channel_4.value);
	m_InputCh_53.set(m_txPdo.Channel_5.value);
	m_InputCh_63.set(m_txPdo.Channel_6.value);
	m_InputCh_73.set(m_txPdo.Channel_7.value);
	m_InputCh_83.set(m_txPdo.Channel_8.value);
	m_InputCh_14.set(m_txPdo.Channel_1.value);
	m_InputCh_24.set(m_txPdo.Channel_2.value);
	m_InputCh_34.set(m_txPdo.Channel_3.value);
	m_InputCh_44.set(m_txPdo.Channel_4.value);
	m_InputCh_54.set(m_txPdo.Channel_5.value);
	m_InputCh_64.set(m_txPdo.Channel_6.value);
	m_InputCh_74.set(m_txPdo.Channel_7.value);
	m_InputCh_84.set(m_txPdo.Channel_8.value);

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
