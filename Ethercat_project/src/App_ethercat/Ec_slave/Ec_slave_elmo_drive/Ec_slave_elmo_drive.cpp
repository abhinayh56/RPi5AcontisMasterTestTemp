#include "Ec_slave_elmo_drive.h"

Ec_slave_elmo_drive::Ec_slave_elmo_drive(uint16_t slaveAddr, const std::string &slaveName) : Ec_slave_base(slaveAddr, slaveName)
{
}

Ec_slave_elmo_drive::~Ec_slave_elmo_drive()
{
}

EC_T_DWORD Ec_slave_elmo_drive::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Position_actual_valuie);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Status_word);

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Target_Position);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Digital_Outputs);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Control_word);

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.Position_actual_valuie, pBuffer);
	transferInputPdoObject(m_TxPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_TxPdo.Status_word, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.Target_Position, pBuffer);
	transferOutputPdoObject(m_RxPdo.Digital_Outputs, pBuffer);
	transferOutputPdoObject(m_RxPdo.Control_word, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_elmo_drive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_elmo_drive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"SLAVE_NAME: " << m_SlaveName << " | " <<
	"Position_actual_valuie: " << m_TxPdo.Position_actual_valuie.value << ", "
	"Digital_Inputs: " << m_TxPdo.Digital_Inputs.value << ", "
	"Status_word: " << m_TxPdo.Status_word.value
	<< std::endl;
}

void Ec_slave_elmo_drive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"SLAVE_NAME: " << m_SlaveName << " | " <<
	"Target_Position: " << m_RxPdo.Target_Position.value << ", "
	"Digital_Outputs: " << m_RxPdo.Digital_Outputs.value << ", "
	"Control_word: " << m_RxPdo.Control_word.value
	<< std::endl;
}
