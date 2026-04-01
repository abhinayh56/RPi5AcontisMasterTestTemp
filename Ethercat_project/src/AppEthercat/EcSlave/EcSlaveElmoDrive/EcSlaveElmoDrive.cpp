#include "EcSlaveElmoDrive.h"

EcSlaveElmoDrive::EcSlaveElmoDrive(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveElmoDrive::~EcSlaveElmoDrive()
{
}

EC_T_DWORD EcSlaveElmoDrive::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_word);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Mode_of_operation_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Position_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Velocity_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Current_actual_value);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_word);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Mode_of_operation);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_Torque);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_Position);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Velocity_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_Outputs);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Status_word, pBuffer);
	transferInputPdoObject(m_txPdo.Mode_of_operation_display, pBuffer);
	transferInputPdoObject(m_txPdo.Position_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Velocity_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_txPdo.Current_actual_value, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Control_word, pBuffer);
	transferOutputPdoObject(m_rxPdo.Mode_of_operation, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_Torque, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_Position, pBuffer);
	transferOutputPdoObject(m_rxPdo.Velocity_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_Outputs, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveElmoDrive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveElmoDrive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Status_word: " << m_txPdo.Status_word.value << ", "
	"Mode_of_operation_display: " << m_txPdo.Mode_of_operation_display.value << ", "
	"Position_actual_value: " << m_txPdo.Position_actual_value.value << ", "
	"Velocity_actual_value: " << m_txPdo.Velocity_actual_value.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_actual_value.value << ", "
	"Digital_Inputs: " << m_txPdo.Digital_Inputs.value << ", "
	"Current_actual_value: " << m_txPdo.Current_actual_value.value
	<< std::endl;
}

void EcSlaveElmoDrive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_word: " << m_rxPdo.Control_word.value << ", "
	"Mode_of_operation: " << m_rxPdo.Mode_of_operation.value << ", "
	"Target_Torque: " << m_rxPdo.Target_Torque.value << ", "
	"Target_Position: " << m_rxPdo.Target_Position.value << ", "
	"Velocity_Offset: " << m_rxPdo.Velocity_Offset.value << ", "
	"Digital_Outputs: " << m_rxPdo.Digital_Outputs.value
	<< std::endl;
}
