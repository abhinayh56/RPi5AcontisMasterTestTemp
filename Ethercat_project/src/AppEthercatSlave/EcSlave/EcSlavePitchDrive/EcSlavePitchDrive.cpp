#include "EcSlavePitchDrive.h"

EcSlavePitchDrive::EcSlavePitchDrive(uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(slaveAddr, slaveName),
	m_InputCh_1("DATA_1", m_path, false, true),
	m_InputCh_2("DATA_2", m_path, false, true),
	m_InputCh_3("DATA_3", m_path, false, true),
	m_InputCh_4("DATA_4", m_path, false, true),
	m_InputCh_5("DATA_5", m_path, false, true),
	m_InputCh_6("DATA_6", m_path, false, true),
	m_InputCh_7("DATA_7", m_path, false, true),
	m_InputCh_8("DATA_8", m_path, false, true)
{
}

EcSlavePitchDrive::~EcSlavePitchDrive()
{
}

EC_T_DWORD EcSlavePitchDrive::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_POS);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.STATUS_WD);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_TOR);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.OPMODE_DISP);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ERROR_CODE);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.DIG_IN);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_VEL);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ADC_VAL);

	m_Cia402PdoTx.statusWord.p_isSupported = &m_txPdo.STATUS_WD.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.OPMODE_DISP.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported = &m_txPdo.ACT_POS.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported = &m_txPdo.ACT_VEL.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported = &m_txPdo.ACT_TOR.isSupported;

	m_Cia402PdoTx.statusWord.p_value = &m_txPdo.STATUS_WD.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value = &m_txPdo.OPMODE_DISP.value;
	m_Cia402PdoTx.actualPosition.p_value = &m_txPdo.ACT_POS.value;
	m_Cia402PdoTx.actualVelocity.p_value = &m_txPdo.ACT_VEL.value;
	m_Cia402PdoTx.actualTorque.p_value = &m_txPdo.ACT_TOR.value;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_POSE);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.CONTROL_WD);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_TORQ);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.OP_MODE);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.ERROR_CLEAR);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.DIG_OUT);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_VEL);

	m_Cia402PdoRx.controlWord.p_isSupported = &m_rxPdo.CONTROL_WD.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.OP_MODE.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported = &m_rxPdo.TARGET_POSE.isSupported;
	m_Cia402PdoRx.targetVelocity.p_isSupported = &m_rxPdo.TARGET_VEL.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported = &m_rxPdo.TARGET_TORQ.isSupported;

	m_Cia402PdoRx.controlWord.p_value = &m_rxPdo.CONTROL_WD.value;
	m_Cia402PdoRx.modeOfOperation.p_value = &m_rxPdo.OP_MODE.value;
	m_Cia402PdoRx.targetPosition.p_value = &m_rxPdo.TARGET_POSE.value;
	m_Cia402PdoRx.targetVelocity.p_value = &m_rxPdo.TARGET_VEL.value;
	m_Cia402PdoRx.targetTorque.p_value = &m_rxPdo.TARGET_TORQ.value;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.ACT_POS, pBuffer);
	transferInputPdoObject(m_txPdo.STATUS_WD, pBuffer);
	transferInputPdoObject(m_txPdo.ACT_TOR, pBuffer);
	transferInputPdoObject(m_txPdo.OPMODE_DISP, pBuffer);
	transferInputPdoObject(m_txPdo.ERROR_CODE, pBuffer);
	transferInputPdoObject(m_txPdo.DIG_IN, pBuffer);
	transferInputPdoObject(m_txPdo.ACT_VEL, pBuffer);
	transferInputPdoObject(m_txPdo.ADC_VAL, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.TARGET_POSE, pBuffer);
	transferOutputPdoObject(m_rxPdo.CONTROL_WD, pBuffer);
	transferOutputPdoObject(m_rxPdo.TARGET_TORQ, pBuffer);
	transferOutputPdoObject(m_rxPdo.OP_MODE, pBuffer);
	transferOutputPdoObject(m_rxPdo.ERROR_CLEAR, pBuffer);
	transferOutputPdoObject(m_rxPdo.DIG_OUT, pBuffer);
	transferOutputPdoObject(m_rxPdo.TARGET_VEL, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::registerPublisher()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::registerSubscriber()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	m_InputCh_1.subscribe();
	m_InputCh_2.subscribe();
	m_InputCh_3.subscribe();
	m_InputCh_4.subscribe();
	m_InputCh_5.subscribe();
	m_InputCh_6.subscribe();
	m_InputCh_7.subscribe();
	m_InputCh_8.subscribe();

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	m_InputCh_1.get(m_data_1);
	m_InputCh_2.get(m_data_2);
	m_InputCh_3.get(m_data_3);
	m_InputCh_4.get(m_data_4);
	m_InputCh_5.get(m_data_5);
	m_InputCh_6.get(m_data_6);
	m_InputCh_7.get(m_data_7);
	m_InputCh_8.get(m_data_8);

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	if(m_data_1 == true) {m_rxPdo.OP_MODE.value = 8;}
	if(m_data_2 == true) {m_rxPdo.CONTROL_WD.value = 6;}
	if(m_data_3 == true) {m_rxPdo.CONTROL_WD.value = 7;}
	if(m_data_4 == true) {m_rxPdo.CONTROL_WD.value = 15;}
	if(m_data_5 == true) {m_rxPdo.TARGET_POSE.value = 0;}
	if(m_data_6 == true) {m_rxPdo.TARGET_POSE.value = 8192;}
	if(m_data_7 == true) {m_rxPdo.CONTROL_WD.value = 128; m_rxPdo.OP_MODE.value = 0;}
	if(m_data_8 == true) {m_rxPdo.CONTROL_WD.value = 0; m_rxPdo.OP_MODE.value = 0;}

	return dwRes;
}

void EcSlavePitchDrive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"ACT_POS: " << m_txPdo.ACT_POS.value << ", "
	"STATUS_WD: " << m_txPdo.STATUS_WD.value << ", "
	"ACT_TOR: " << m_txPdo.ACT_TOR.value << ", "
	"OPMODE_DISP: " << (uint16_t)m_txPdo.OPMODE_DISP.value << ", "
	"ERROR_CODE: " << m_txPdo.ERROR_CODE.value << ", "
	"DIG_IN: " << (uint16_t)m_txPdo.DIG_IN.value << ", "
	"ACT_VEL: " << m_txPdo.ACT_VEL.value << ", "
	"ADC_VAL: " << m_txPdo.ADC_VAL.value
	<< std::endl;
}

void EcSlavePitchDrive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"TARGET_POSE: " << m_rxPdo.TARGET_POSE.value << ", "
	"CONTROL_WD: " << m_rxPdo.CONTROL_WD.value << ", "
	"TARGET_TORQ: " << m_rxPdo.TARGET_TORQ.value << ", "
	"OP_MODE: " << (uint16_t)m_rxPdo.OP_MODE.value << ", "
	"ERROR_CLEAR: " << m_rxPdo.ERROR_CLEAR.value << ", "
	"DIG_OUT: " << (uint16_t)m_rxPdo.DIG_OUT.value << ", "
	"TARGET_VEL: " << m_rxPdo.TARGET_VEL.value
	<< std::endl;
}
