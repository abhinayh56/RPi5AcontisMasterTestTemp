#include "EcSlavePitchDrive.h"

EcSlavePitchDrive::EcSlavePitchDrive(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
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

EC_T_DWORD EcSlavePitchDrive::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlavePitchDrive::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

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
	"OPMODE_DISP: " << m_txPdo.OPMODE_DISP.value << ", "
	"ERROR_CODE: " << m_txPdo.ERROR_CODE.value << ", "
	"DIG_IN: " << m_txPdo.DIG_IN.value << ", "
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
	"OP_MODE: " << m_rxPdo.OP_MODE.value << ", "
	"ERROR_CLEAR: " << m_rxPdo.ERROR_CLEAR.value << ", "
	"DIG_OUT: " << m_rxPdo.DIG_OUT.value << ", "
	"TARGET_VEL: " << m_rxPdo.TARGET_VEL.value
	<< std::endl;
}
