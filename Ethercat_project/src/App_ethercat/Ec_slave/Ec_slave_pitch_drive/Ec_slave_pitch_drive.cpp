#include "Ec_slave_pitch_drive.h"

Ec_slave_pitch_drive::Ec_slave_pitch_drive(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_pitch_drive::~Ec_slave_pitch_drive()
{
}

EC_T_DWORD Ec_slave_pitch_drive::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ACT_POS);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.STATUS_WD);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ACT_TOR);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.OPMODE_DISP);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ERROR_CODE);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.DIG_IN);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ACT_VEL);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.ADC_VAL);

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.TARGET_POSE);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.CONTROL_WD);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.TARGET_TORQ);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.OP_MODE);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.ERROR_CLEAR);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.DIG_OUT);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.TARGET_VEL);

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.ACT_POS, pBuffer);
	transferInputPdoObject(m_TxPdo.STATUS_WD, pBuffer);
	transferInputPdoObject(m_TxPdo.ACT_TOR, pBuffer);
	transferInputPdoObject(m_TxPdo.OPMODE_DISP, pBuffer);
	transferInputPdoObject(m_TxPdo.ERROR_CODE, pBuffer);
	transferInputPdoObject(m_TxPdo.DIG_IN, pBuffer);
	transferInputPdoObject(m_TxPdo.ACT_VEL, pBuffer);
	transferInputPdoObject(m_TxPdo.ADC_VAL, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.TARGET_POSE, pBuffer);
	transferOutputPdoObject(m_RxPdo.CONTROL_WD, pBuffer);
	transferOutputPdoObject(m_RxPdo.TARGET_TORQ, pBuffer);
	transferOutputPdoObject(m_RxPdo.OP_MODE, pBuffer);
	transferOutputPdoObject(m_RxPdo.ERROR_CLEAR, pBuffer);
	transferOutputPdoObject(m_RxPdo.DIG_OUT, pBuffer);
	transferOutputPdoObject(m_RxPdo.TARGET_VEL, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_pitch_drive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_pitch_drive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"ACT_POS: " << m_TxPdo.ACT_POS.value << ", "
	"STATUS_WD: " << m_TxPdo.STATUS_WD.value << ", "
	"ACT_TOR: " << m_TxPdo.ACT_TOR.value << ", "
	"OPMODE_DISP: " << m_TxPdo.OPMODE_DISP.value << ", "
	"ERROR_CODE: " << m_TxPdo.ERROR_CODE.value << ", "
	"DIG_IN: " << m_TxPdo.DIG_IN.value << ", "
	"ACT_VEL: " << m_TxPdo.ACT_VEL.value << ", "
	"ADC_VAL: " << m_TxPdo.ADC_VAL.value
	<< std::endl;
}

void Ec_slave_pitch_drive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"TARGET_POSE: " << m_RxPdo.TARGET_POSE.value << ", "
	"CONTROL_WD: " << m_RxPdo.CONTROL_WD.value << ", "
	"TARGET_TORQ: " << m_RxPdo.TARGET_TORQ.value << ", "
	"OP_MODE: " << m_RxPdo.OP_MODE.value << ", "
	"ERROR_CLEAR: " << m_RxPdo.ERROR_CLEAR.value << ", "
	"DIG_OUT: " << m_RxPdo.DIG_OUT.value << ", "
	"TARGET_VEL: " << m_RxPdo.TARGET_VEL.value
	<< std::endl;
}
