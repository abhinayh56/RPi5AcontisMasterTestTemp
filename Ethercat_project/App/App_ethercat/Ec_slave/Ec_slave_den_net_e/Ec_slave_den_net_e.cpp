#include "Ec_slave_den_net_e.h"

Ec_slave_den_net_e::Ec_slave_den_net_e(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_den_net_e::~Ec_slave_den_net_e()
{
}

EC_T_DWORD Ec_slave_den_net_e::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Status_Word);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Actual_position);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Actual_velocity);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Operation_mode_display);

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Control_Word);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Position_set_point);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Velocity_set_point);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Operation_mode);

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.Status_Word, pBuffer);
	transferInputPdoObject(m_TxPdo.Actual_position, pBuffer);
	transferInputPdoObject(m_TxPdo.Actual_velocity, pBuffer);
	transferInputPdoObject(m_TxPdo.Operation_mode_display, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.Control_Word, pBuffer);
	transferOutputPdoObject(m_RxPdo.Position_set_point, pBuffer);
	transferOutputPdoObject(m_RxPdo.Velocity_set_point, pBuffer);
	transferOutputPdoObject(m_RxPdo.Operation_mode, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_den_net_e::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_den_net_e::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Status_Word: " << m_TxPdo.Status_Word.value << ", "
	"Actual_position: " << m_TxPdo.Actual_position.value << ", "
	"Actual_velocity: " << m_TxPdo.Actual_velocity.value << ", "
	"Operation_mode_display: " << m_TxPdo.Operation_mode_display.value
	<< std::endl;
}

void Ec_slave_den_net_e::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Control_Word: " << m_RxPdo.Control_Word.value << ", "
	"Position_set_point: " << m_RxPdo.Position_set_point.value << ", "
	"Velocity_set_point: " << m_RxPdo.Velocity_set_point.value << ", "
	"Operation_mode: " << m_RxPdo.Operation_mode.value
	<< std::endl;
}