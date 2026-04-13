#include "EcSlaveDenNetE.h"

EcSlaveDenNetE::EcSlaveDenNetE(uint16_t slaveAddr, const std::string &slaveName) :
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

EcSlaveDenNetE::~EcSlaveDenNetE()
{
}

EC_T_DWORD EcSlaveDenNetE::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_Word);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_position);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_velocity);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Operation_mode_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_actual_value);

	m_Cia402PdoTx.statusWord.p_isSupported = &m_txPdo.Status_Word.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.Operation_mode_display.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported = &m_txPdo.Actual_position.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported = &m_txPdo.Actual_velocity.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported = &m_txPdo.Torque_actual_value.isSupported;

	m_Cia402PdoTx.statusWord.p_value = &m_txPdo.Status_Word.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value = &m_txPdo.Operation_mode_display.value;
	m_Cia402PdoTx.actualPosition.p_value = &m_txPdo.Actual_position.value;
	m_Cia402PdoTx.actualVelocity.p_value = &m_txPdo.Actual_velocity.value;
	m_Cia402PdoTx.actualTorque.p_value = &m_txPdo.Torque_actual_value.value;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_Word);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Position_set_point);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Velocity_set_point);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Operation_mode);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_torque);

	m_Cia402PdoRx.controlWord.p_isSupported = &m_rxPdo.Control_Word.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.Operation_mode.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported = &m_rxPdo.Position_set_point.isSupported;
	m_Cia402PdoRx.targetVelocity.p_isSupported = &m_rxPdo.Velocity_set_point.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported = &m_rxPdo.Target_torque.isSupported;

	m_Cia402PdoRx.controlWord.p_value = &m_rxPdo.Control_Word.value;
	m_Cia402PdoRx.modeOfOperation.p_value = &m_rxPdo.Operation_mode.value;
	m_Cia402PdoRx.targetPosition.p_value = &m_rxPdo.Position_set_point.value;
	m_Cia402PdoRx.targetVelocity.p_value = &m_rxPdo.Velocity_set_point.value;
	m_Cia402PdoRx.targetTorque.p_value = &m_rxPdo.Target_torque.value;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Status_Word, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_position, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_velocity, pBuffer);
	transferInputPdoObject(m_txPdo.Operation_mode_display, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_actual_value, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Control_Word, pBuffer);
	transferOutputPdoObject(m_rxPdo.Position_set_point, pBuffer);
	transferOutputPdoObject(m_rxPdo.Velocity_set_point, pBuffer);
	transferOutputPdoObject(m_rxPdo.Operation_mode, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_torque, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::registerPublisher()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::registerSubscriber()
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

EC_T_DWORD EcSlaveDenNetE::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::subscribeData()
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

EC_T_DWORD EcSlaveDenNetE::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	// if(m_data_1 == true) {emergencyStop();}
	// if(m_data_2 == true) {clearFault();}
	// if(m_data_3 == true) {disable();}
	// if(m_data_4 == true) {setModePosition();}
	// if(m_data_5 == true) {setModeVelocity();}
	// if(m_data_6 == true) {setModeTorque();}
	// if(m_data_7 == true) {enable();}
	// if(m_data_8 == true) {quickStop();}

	//syncPosition();
	
	// double d_th = 0.0001;
	
	// if(m_data_7 == true)
	// {
	// 	angle_command -= d_th;

	// 	angle_command = std::min(angle_command, 0.0);

	// 	setTargetPosition((int32_t)angle_command);
	// }
	// if(m_data_8 == true)
	// {
	// 	angle_command += d_th;

	// 	angle_command = std::max(angle_command, 500000.0);

	// 	setTargetPosition((int32_t)angle_command);
	// }

	return dwRes;
}

void EcSlaveDenNetE::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Status_Word: " << m_txPdo.Status_Word.value << ", "
	"Actual_position: " << m_txPdo.Actual_position.value << ", "
	"Actual_velocity: " << m_txPdo.Actual_velocity.value << ", "
	"Operation_mode_display: " << (uint16_t)m_txPdo.Operation_mode_display.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_actual_value.value
	<< std::endl;
}

void EcSlaveDenNetE::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_Word: " << m_rxPdo.Control_Word.value << ", "
	"Position_set_point: " << m_rxPdo.Position_set_point.value << ", "
	"Position_set_point: " << m_rxPdo.Velocity_set_point.value << ", "
	"Operation_mode: " << (uint16_t)m_rxPdo.Operation_mode.value << ", "
	"Target_torque: " << m_rxPdo.Target_torque.value
	<< std::endl;
}
