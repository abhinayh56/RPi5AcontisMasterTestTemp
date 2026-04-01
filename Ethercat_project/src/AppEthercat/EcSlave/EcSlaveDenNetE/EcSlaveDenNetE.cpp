#include "EcSlaveDenNetE.h"

EcSlaveDenNetE::EcSlaveDenNetE(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
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
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Last_error);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Anlog_input_1_Counts);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Anlog_input_2_Counts);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_inputs_value);

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_Word);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Position_set_point);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Operation_mode);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_torque);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_outputs_set_value);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Analog_output_1_Value);

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
	transferInputPdoObject(m_txPdo.Last_error, pBuffer);
	transferInputPdoObject(m_txPdo.Anlog_input_1_Counts, pBuffer);
	transferInputPdoObject(m_txPdo.Anlog_input_2_Counts, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_inputs_value, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Control_Word, pBuffer);
	transferOutputPdoObject(m_rxPdo.Position_set_point, pBuffer);
	transferOutputPdoObject(m_rxPdo.Operation_mode, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_torque, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_outputs_set_value, pBuffer);
	transferOutputPdoObject(m_rxPdo.Analog_output_1_Value, pBuffer);

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

EC_T_DWORD EcSlaveDenNetE::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveDenNetE::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

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
	"Operation_mode_display: " << m_txPdo.Operation_mode_display.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_actual_value.value << ", "
	"Last_error: " << m_txPdo.Last_error.value << ", "
	"Anlog_input_1_Counts: " << m_txPdo.Anlog_input_1_Counts.value << ", "
	"Anlog_input_2_Counts: " << m_txPdo.Anlog_input_2_Counts.value << ", "
	"Digital_inputs_value: " << m_txPdo.Digital_inputs_value.value
	<< std::endl;
}

void EcSlaveDenNetE::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_Word: " << m_rxPdo.Control_Word.value << ", "
	"Position_set_point: " << m_rxPdo.Position_set_point.value << ", "
	"Operation_mode: " << m_rxPdo.Operation_mode.value << ", "
	"Target_torque: " << m_rxPdo.Target_torque.value << ", "
	"Digital_outputs_set_value: " << m_rxPdo.Digital_outputs_set_value.value << ", "
	"Analog_output_1_Value: " << m_rxPdo.Analog_output_1_Value.value
	<< std::endl;
}
